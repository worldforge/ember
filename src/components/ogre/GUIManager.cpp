/*
 Copyright (C) 2004  Miguel Guzman (Aganor)
 Copyright (C) 2005  Erik Ogenvik <erik@ogenvik.org>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "GUIManager.h"
#include "EmberOgre.h"
#include "domain/EmberEntity.h"
#include "GUICEGUIAdapter.h"
#include "World.h"

#include "widgets/EntityTooltip.h"
#include "widgets/Widget.h"
#include "widgets/QuickHelp.h"
#include "widgets/WidgetDefinitions.h"
#include "widgets/icons/IconManager.h"
#include "widgets/EntityIconManager.h"
#include "widgets/ActionBarIconManager.h"
#include "widgets/WorldLoadingScreen.h"

#include "camera/MainCamera.h"
#include "gui/ActiveWidgetHandler.h"
#include "gui/CursorWorldListener.h"

#include "components/cegui/CEGUILogger.h"
#include "components/cegui/ColouredRenderedStringParser.h"
#include "components/cegui/SDLNativeClipboardProvider.h"

#include "components/lua/LuaScriptingProvider.h"

#include "services/EmberServices.h"
#include "services/config/ConfigService.h"
#include "services/scripting/ScriptingService.h"
#include "services/server/ServerServiceSignals.h"

#include "framework/IScriptingProvider.h"

#include <Eris/View.h>

#include <OgreRenderWindow.h>
#include <OgreRoot.h>

#include <CEGUI/WindowManager.h>
#include <CEGUI/SchemeManager.h>
#include <CEGUI/FactoryModule.h>
#include <CEGUI/ScriptModules/Lua/ScriptModule.h>
#include <CEGUI/RendererModules/Ogre/ResourceProvider.h>
#include <CEGUI/RendererModules/Ogre/ImageCodec.h>
#include <CEGUI/widgets/PushButton.h>
#include <CEGUI/widgets/MultiLineEditbox.h>
#include <CEGUI/widgets/Editbox.h>

#ifdef _WIN32
#include "platform/platform_windows.h"
#endif

template<> Ember::OgreView::GUIManager* Ember::Singleton<Ember::OgreView::GUIManager>::ms_Singleton = 0;

using namespace CEGUI;
using namespace Ember::OgreView::Gui;
using namespace Ember;

namespace Ember
{
namespace OgreView
{

unsigned long GUIManager::msAutoGenId(0);

GUIManager::GUIManager(Ogre::RenderWindow* window, ConfigService& configService, ServerServiceSignals& serverSignals, MainLoopController& mainLoopController) :
		ToggleInputMode("toggle_inputmode", this, "Toggle the input mode."), ReloadGui("reloadgui", this, "Reloads the gui."), ToggleGui("toggle_gui", this, "Toggle the gui display"), mConfigService(configService), mMainLoopController(mainLoopController), mGuiCommandMapper("gui", "key_bindings_gui"), mSheet(0), mWindowManager(0), mWindow(window), mGuiSystem(0), mGuiRenderer(0), mOgreResourceProvider(0), mOgreImageCodec(0), mCursorWorldListener(0), mEnabled(true), mLuaScriptModule(0), mIconManager(0), mActiveWidgetHandler(0), mCEGUILogger(new Cegui::CEGUILogger()), mRenderedStringParser(0), mEntityTooltip(0), mNativeClipboardProvider(nullptr)
{

//Check that CEGUI is built with Freetype support. If not you'll get a compilation error here.
#ifndef CEGUI_HAS_FREETYPE
CEGUI is not built with Freetype
#endif

	mGuiCommandMapper.restrictToInputMode(Input::IM_GUI);

	serverSignals.GotView.connect(sigc::mem_fun(*this, &GUIManager::server_GotView));

	//we need this here just to force the linker to also link in the WidgetDefinitions
	WidgetDefinitions w;

	try {

		S_LOG_INFO("Starting CEGUI");
		mDefaultScheme = "EmberLook";
		S_LOG_VERBOSE("Setting default scheme to "<< mDefaultScheme);

		if (!configService.getPrefix().empty()) {

#ifndef __WIN32__
			//The environment variable CEGUI_MODULE_DIR points to where CEGUI can find its modules. Lets check if they are under the prefix,
			//for relocatable packages. We need to check both "lib64" and "lib".
			std::string modulePath = configService.getPrefix() + "/lib64/cegui-0.8";
			if (std::ifstream(modulePath).good()) {
				setenv("CEGUI_MODULE_DIR", modulePath.c_str(), 1);
				S_LOG_INFO("Setting CEGUI_MODULE_DIR to " << modulePath);
			} else {
				modulePath = configService.getPrefix() + "/lib/cegui-0.8";
				if (std::ifstream(modulePath).good()) {
					setenv("CEGUI_MODULE_DIR", modulePath.c_str(), 1);
					S_LOG_INFO("Setting CEGUI_MODULE_DIR to " << modulePath);
				}
			}
#endif
			//We need to set the current directory to the prefix before trying to load CEGUI.
			if (chdir(configService.getPrefix().c_str())) {
				S_LOG_WARNING("Failed to change to the prefix directory '" << configService.getPrefix() << "'. Gui loading might fail.");
			}
		}

		//The OgreCEGUIRenderer is the main interface between Ogre and CEGUI.
		// Note that the third argument tells the renderer to render the gui after all of the
		// regular render queues have been processed, thus making sure that the gui always is on top.
		mGuiRenderer = &CEGUI::OgreRenderer::create(*window);

		//We'll do our own rendering, interleaved with Ogre's, so we'll turn off the automatic rendering.
		mGuiRenderer->setRenderingEnabled(false);
		mGuiRenderer->setFrameControlExecutionEnabled(false);

		mOgreResourceProvider = &CEGUI::OgreRenderer::createOgreResourceProvider();
		mOgreResourceProvider->setDefaultResourceGroup("Gui");

		mOgreImageCodec = &CEGUI::OgreRenderer::createOgreImageCodec();

		IScriptingProvider* provider = EmberServices::getSingleton().getScriptingService().getProviderFor("LuaScriptingProvider");
		if (provider != nullptr) {
			Lua::LuaScriptingProvider* luaScriptProvider = static_cast<Lua::LuaScriptingProvider*>(provider);
			mLuaScriptModule = &LuaScriptModule::create(luaScriptProvider->getLuaState());
			if (!luaScriptProvider->getErrorHandlingFunctionName().empty()) {
				mLuaScriptModule->setDefaultPCallErrorHandler(luaScriptProvider->getErrorHandlingFunctionName());
				mLuaScriptModule->executeString(""); //We must call this to make CEGUI set up the error function internally. If we don't, CEGUI will never correctly set it up. The reason for this is that we never use the execute* methods in the CEGUI lua module later on, instead loading our scripts ourselves. And CEGUI is currently set up to require the execute* methods to be called in order for the error function to be registered.
			}
			mGuiSystem = &CEGUI::System::create(*mGuiRenderer, mOgreResourceProvider, nullptr, mOgreImageCodec, mLuaScriptModule, "cegui/datafiles/configs/cegui.config");

			EmberServices::getSingleton().getScriptingService().EventStopping.connect(sigc::mem_fun(*this, &GUIManager::scriptingServiceStopping));
		} else {
			mGuiSystem = &CEGUI::System::create(*mGuiRenderer, mOgreResourceProvider, nullptr, mOgreImageCodec, 0, "cegui/datafiles/configs/cegui.config");
		}
		CEGUI::SchemeManager::SchemeIterator schemeI(SchemeManager::getSingleton().getIterator());
		if (schemeI.isAtEnd()) {
			// 			S_LOG_FAILURE("Could not load any CEGUI schemes. This means that there's something wrong with how CEGUI is setup. Check the CEGUI log for more detail. We'll now exit Ember.");
			throw Exception("Could not load any CEGUI schemes. This means that there's something wrong with how CEGUI is setup. Check the CEGUI log for more detail. We'll now exit Ember.");
		}
		mNativeClipboardProvider = new Ember::Cegui::SDLNativeClipboardProvider();
		mGuiSystem->getClipboard()->setNativeProvider(mNativeClipboardProvider);

		mRenderedStringParser = new Cegui::ColouredRenderedStringParser();
		mGuiSystem->setDefaultCustomRenderedStringParser(mRenderedStringParser);
		mWindowManager = &CEGUI::WindowManager::getSingleton();

		EntityTooltip::registerFactory();

		mSheet = mWindowManager->createWindow("DefaultWindow", "root_wnd");
		mGuiSystem->getDefaultGUIContext().setRootWindow(mSheet);
		mSheet->activate();
		mSheet->moveToBack();
		mSheet->setDistributesCapturedInputs(false);

		mWorldLoadingScreen = new WorldLoadingScreen();

		S_LOG_INFO("CEGUI system set up");

		getInput().EventKeyPressed.connect(sigc::mem_fun(*this, &GUIManager::pressedKey));
		getInput().setInputMode(Input::IM_GUI);

		//add adapter for CEGUI, this will route input event to the gui
		mCEGUIAdapter = new GUICEGUIAdapter(mGuiSystem, mGuiRenderer);
		getInput().addAdapter(mCEGUIAdapter);

		mGuiCommandMapper.bindToInput(getInput());

		//connect to the creation of the avatar, since we want to switch to movement mode when that happens
		EmberOgre::getSingleton().EventCreatedAvatarEntity.connect(sigc::mem_fun(*this, &GUIManager::EmberOgre_CreatedAvatarEntity));
		EmberOgre::getSingleton().EventWorldCreated.connect(sigc::mem_fun(*this, &GUIManager::EmberOgre_WorldCreated));
		EmberOgre::getSingleton().EventWorldDestroyed.connect(sigc::mem_fun(*this, &GUIManager::EmberOgre_WorldDestroyed));

		mActiveWidgetHandler = new Gui::ActiveWidgetHandler(*this);

		Ogre::Root::getSingleton().addFrameListener(this);

		mQuickHelp = new Gui::QuickHelp();

		getInput().EventSizeChanged.connect(sigc::mem_fun(*this, &GUIManager::input_SizeChanged));

	} catch (const CEGUI::Exception& ex) {
		S_LOG_FAILURE("GUIManager - error when creating gui." << ex);
		throw Exception(ex.getMessage().c_str());
	}

	if (chdir(configService.getEmberDataDirectory().c_str())) {
		S_LOG_WARNING("Failed to change to the data directory '" << configService.getEmberDataDirectory() << "'.");
	}

}

GUIManager::~GUIManager()
{
	S_LOG_INFO("Shutting down GUI manager.");

	WidgetStore widgetStoreCopy(mWidgets);
	for (auto& widget : widgetStoreCopy) {
		S_LOG_INFO("Deleting widget " << widget->getPrefix() << ".");
		delete widget;
	}

	delete mWorldLoadingScreen;
	delete mActiveWidgetHandler;
	delete mEntityIconManager;
	delete mActionBarIconManager;
	delete mIconManager;
	delete mCursorWorldListener;

	CEGUI::System::destroy();

	if (mOgreResourceProvider) {
		mGuiRenderer->destroyOgreResourceProvider(*mOgreResourceProvider);
	}
	if (mOgreImageCodec) {
		mGuiRenderer->destroyOgreImageCodec(*mOgreImageCodec);
	}

	//note that we normally would delete the mCEGUILogger here, but we don't have to since mGuiSystem will do that in it's desctructor, even though it doesn't own the logger
	Ogre::Root::getSingleton().removeFrameListener(this);
	delete mCEGUIAdapter;

	if (mGuiRenderer) {
		CEGUI::OgreRenderer::destroy(*mGuiRenderer);
	}
	if (mLuaScriptModule) {
		LuaScriptModule::destroy(*mLuaScriptModule);
	}
	delete mRenderedStringParser;
	delete mQuickHelp;

	WidgetLoader::removeAllWidgetFactories();

	delete mCEGUILogger;
	delete mNativeClipboardProvider;

}

void GUIManager::initialize()
{
	try {
		createWidget("Quit");
	} catch (const std::exception& e) {
		S_LOG_FAILURE("GUIManager - error when initializing widgets." << e);
		throw e;
	}
	try {
		mIconManager = new Gui::Icons::IconManager();
	} catch (const std::exception& e) {
		S_LOG_FAILURE("GUIManager - error when creating icon manager." << e);
	}

	try {
		mEntityIconManager = new Gui::EntityIconManager(*this);
	} catch (const std::exception& e) {
		S_LOG_FAILURE("GUIManager - error when creating entity icon manager." << e);
	}
	try {
		mActionBarIconManager = new Gui::ActionBarIconManager(*this);
	} catch (const std::exception& e) {
		S_LOG_FAILURE("GUIManager - error when creating ActionBar icon manager." << e);
	}

	std::vector<std::string> widgetsToLoad;
	widgetsToLoad.emplace_back("IngameChatWidget");
	widgetsToLoad.emplace_back("Help");
	widgetsToLoad.emplace_back("MeshPreview");

	for (auto& widget : widgetsToLoad) {
		try {
			S_LOG_VERBOSE("Loading widget " << widget);
			createWidget(widget);
		} catch (const std::exception& e) {
			S_LOG_FAILURE("Error when initializing widget " << widget << "." << e);
		}
	}

}

void GUIManager::render()
{
	if (mEnabled) {
		CEGUI::System::getSingleton().renderAllGUIContexts();
	}
}

void GUIManager::input_SizeChanged(int width, int height)
{
	mGuiSystem->notifyDisplaySizeChanged(CEGUI::Sizef(width, height));
}

void GUIManager::server_GotView(Eris::View* view)
{
	//The View has a shorter lifespan than ours, so we don't need to store references to the connections.
	view->EntityCreated.connect(sigc::mem_fun(*this, &GUIManager::view_EntityCreated));
	view->EntitySeen.connect(sigc::mem_fun(*this, &GUIManager::view_EntityCreated));
}

void GUIManager::view_EntityCreated(Eris::Entity* entity)
{
	//It's safe to cast to EmberEntity, since all entities in the system are guaranteed to be of this type.
	EmberEntity* emberEntity = static_cast<EmberEntity*>(entity);
	//The Entity has a shorter lifespan than ours, so we don't need to store references to the connections.
	emberEntity->EventTalk.connect(sigc::bind(sigc::mem_fun(*this, &GUIManager::entity_Talk), emberEntity));
	emberEntity->Emote.connect(sigc::bind(sigc::mem_fun(*this, &GUIManager::entity_Emote), emberEntity));
}

void GUIManager::entity_Talk(const EntityTalk& entityTalk, EmberEntity* entity)
{
	AppendIGChatLine.emit(entityTalk, entity);
}

void GUIManager::entity_Emote(const std::string& description, EmberEntity* entity)
{
	AppendAvatarImaginary(entity->getName() + " " + description);
}

void GUIManager::scriptingServiceStopping()
{
	mGuiSystem->setScriptingModule(nullptr);
	if (mLuaScriptModule) {
		LuaScriptModule::destroy(*mLuaScriptModule);
	}
	mLuaScriptModule = nullptr;
}

void GUIManager::EmitEntityAction(const std::string& action, EmberEntity* entity)
{
	EventEntityAction.emit(action, entity);
}

CEGUI::Window* GUIManager::createWindow(const std::string& windowType)
{
	std::stringstream ss;
	ss << "_autoWindow_" << (msAutoGenId++);
	return createWindow(windowType, ss.str());
}

CEGUI::Window* GUIManager::createWindow(const std::string& windowType, const std::string& windowName)
{
	try {
		CEGUI::Window* window = mWindowManager->createWindow(windowType, windowName);
		return window;
	} catch (const CEGUI::Exception& ex) {
		S_LOG_FAILURE("Error when creating new window of type " << windowType << " with name " << windowName << ".\n" << ex.getMessage().c_str());
		return nullptr;
	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Error when creating new window of type " << windowType << " with name " << windowName << "." << ex);
		return nullptr;
	}
}

Widget* GUIManager::createWidget()
{
	return createWidget("Widget");
}

Widget* GUIManager::createWidget(const std::string& name)
{
	try {

		Widget* widget = WidgetLoader::createWidget(name);
		if (widget == nullptr) {
			S_LOG_FAILURE("Could not find widget with name " << name);
			return nullptr;
		}
		widget->init(this);
		widget->buildWidget();
		addWidget(widget);
		S_LOG_INFO("Successfully loaded widget " << name);
		return widget;
	} catch (const std::exception& e) {
		S_LOG_FAILURE("Error when loading widget " << name << "." << e);
		return nullptr;
	}
}

void GUIManager::destroyWidget(Widget* widget)
{
	if (!widget) {
		S_LOG_WARNING("Trying to destroy null widget.");
		return;
	}
	removeWidget(widget);
	delete widget;
}

CEGUI::Texture& GUIManager::createTexture(Ogre::TexturePtr& ogreTexture, std::string name)
{
	if (name.empty()) {
		name = ogreTexture->getName();
	}
	return mGuiRenderer->createTexture(name, ogreTexture);
}

Input& GUIManager::getInput() const
{
	return Input::getSingleton();
}

CEGUI::Window* GUIManager::getMainSheet() const
{
	return mSheet;
}

void GUIManager::removeWidget(Widget* widget)
{
	auto I = std::find(mWidgets.begin(), mWidgets.end(), widget);
	if (I != mWidgets.end()) {
		mWidgets.erase(I);
	}
}

void GUIManager::addWidget(Widget* widget)
{
	mWidgets.push_back(widget);
}

bool GUIManager::frameStarted(const Ogre::FrameEvent& evt)
{
	try {
		CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);
		CEGUI::System::getSingleton().getDefaultGUIContext().injectTimePulse(evt.timeSinceLastFrame);
	} catch (const CEGUI::Exception& ex) {
		S_LOG_WARNING("Error in CEGUI." << ex);
	}

	//iterate over all widgets and send them a frameStarted event
	auto I = mWidgets.begin();
	auto I_end = mWidgets.end();

	for (; I != I_end; ++I) {
		Widget* aWidget = *I;
		try {
			aWidget->frameStarted(evt);
		} catch (const CEGUI::Exception& ex) {
			S_LOG_WARNING("Error in CEGUI." << ex);
		}
	}

	EventFrameStarted.emit(evt.timeSinceLastFrame);

	return true;

}

bool GUIManager::isInMovementKeysMode() const
{
	return mSheet->isCapturedByThis() || !isInGUIMode();
}

bool GUIManager::isInGUIMode() const
{
	return getInput().getInputMode() == Input::IM_GUI;
}

void GUIManager::pressedKey(const SDL_Keysym& key, Input::InputMode inputMode)
{
	if (((key.mod & KMOD_CTRL)|| (key.mod & KMOD_LCTRL) || (key.mod & KMOD_RCTRL))) {
		if (key.sym == SDLK_c) {
			mGuiSystem->getDefaultGUIContext().injectCopyRequest();
		} else if (key.sym == SDLK_x) {
			mGuiSystem->getDefaultGUIContext().injectCutRequest();
		} else if (key.sym == SDLK_v) {
			mGuiSystem->getDefaultGUIContext().injectPasteRequest();
		}
	}
}

void GUIManager::runCommand(const std::string &command, const std::string &args)
{
	if (command == ToggleInputMode.getCommand()) {
		getInput().toggleInputMode();
	} else if (command == ToggleGui.getCommand()) {

		S_LOG_VERBOSE("Toggle Gui Initiated -- " << getInput().getInputMode());

		if (mEnabled) {
			S_LOG_INFO("Disabling GUI");
			mEnabled = false;

			getInput().removeAdapter(mCEGUIAdapter);

		} else {
			S_LOG_INFO("Enabling GUI");
			mEnabled = true;

			getInput().addAdapter(mCEGUIAdapter);
		}
	} else if (command == ReloadGui.getCommand()) {
		Ogre::TextureManager* texMgr = Ogre::TextureManager::getSingletonPtr();
		Ogre::ResourcePtr resource = texMgr->getByName("cegui/" + getDefaultScheme() + ".png");
		if (!resource.isNull()) {
			resource->reload();
		}
	}
}

void GUIManager::EmberOgre_CreatedAvatarEntity(EmberEntity& entity)
{
	//switch to movement mode, since it appears most people don't know how to change from gui mode
	varconf::Variable var;
	if (!EmberServices::getSingleton().getConfigService().getValue("input", "automovementmode", var)
			|| (var.is_bool() && (bool)var)) {
		getInput().setInputMode(Input::IM_MOVEMENT);
	}
}

void GUIManager::EmberOgre_WorldCreated(World& world)
{
	mEntityTooltip = new EntityTooltip(world, *static_cast<EmberEntityTooltipWidget*>(mWindowManager->createWindow("EmberLook/EntityTooltip", "EntityTooltip")), *mIconManager);
	mCursorWorldListener = new CursorWorldListener(mMainLoopController, *mSheet, world.getMainCamera());
}

void GUIManager::EmberOgre_WorldDestroyed()
{
	delete mEntityTooltip;
	mEntityTooltip = nullptr;
	delete mCursorWorldListener;
	mCursorWorldListener = nullptr;
}

Gui::EntityTooltip* GUIManager::getEntityTooltip() const
{
	return mEntityTooltip;
}

const std::string& GUIManager::getLayoutDir() const
{
	static std::string dir("cegui/datafiles/layouts/");
	return dir;
}

const std::string& GUIManager::getDefaultScheme() const
{
	return mDefaultScheme;
}

Gui::Icons::IconManager* GUIManager::getIconManager() const
{
	return mIconManager;
}

Gui::EntityIconManager* GUIManager::getEntityIconManager() const
{
	return mEntityIconManager;
}

Gui::ActionBarIconManager* GUIManager::getActionBarIconManager() const
{
	return mActionBarIconManager;
}

CEGUI::Renderer* GUIManager::getGuiRenderer() const
{
	return mGuiRenderer;
}

}
}

