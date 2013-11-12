/*
 Copyright (C) 2004  Miguel Guzman (Aganor)
 Copyright (C) 2005  Erik Hjortsberg <erik.hjortsberg@gmail.com>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "GUIManager.h"
#include "EmberOgre.h"
#include "EmberEntity.h"
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
#include "components/cegui/ScrapNativeClipboardProvider.h"

#include "components/lua/LuaScriptingProvider.h"

#include "services/input/Input.h"
#include "services/EmberServices.h"
#include "services/config/ConfigService.h"
#include "services/scripting/ScriptingService.h"
#include "services/server/ServerServiceSignals.h"

#include "framework/IScriptingProvider.h"
#include "framework/Time.h"

#include <Eris/View.h>

#include <OgreRenderWindow.h>
#include <OgreViewport.h>
#include <OgreTextureManager.h>
#include <OgreRoot.h>

#include <CEGUI/WindowManager.h>
#include <CEGUI/SchemeManager.h>
#include <CEGUI/Exceptions.h>
#include <CEGUI/FactoryModule.h>
#include <CEGUI/Clipboard.h>
#include <CEGUI/ScriptModules/Lua/ScriptModule.h>
#include <CEGUI/RendererModules/Ogre/ResourceProvider.h>
#include <CEGUI/RendererModules/Ogre/ImageCodec.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include <CEGUI/widgets/PushButton.h>
#include <CEGUI/widgets/MultiLineEditbox.h>
#include <CEGUI/widgets/Editbox.h>
#include <CEGUI/widgets/Tooltip.h>

#include <sigc++/bind.h>

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
		ToggleInputMode("toggle_inputmode", this, "Toggle the input mode."), ReloadGui("reloadgui", this, "Reloads the gui."), ToggleGui("toggle_gui", this, "Toggle the gui display"), mConfigService(configService), mMainLoopController(mainLoopController), mGuiCommandMapper("gui", "key_bindings_gui"), mSheet(0), mWindowManager(0), mWindow(window), mGuiSystem(0), mGuiRenderer(0), mOgreResourceProvider(0), mOgreImageCodec(0), mCursorWorldListener(0), mLuaScriptModule(0), mIconManager(0), mActiveWidgetHandler(0), mCEGUILogger(new Cegui::CEGUILogger()), mRenderedStringParser(0), mEntityTooltip(0), mNativeClipboardProvider(nullptr)
{
	mGuiCommandMapper.restrictToInputMode(Input::IM_GUI);

	serverSignals.GotView.connect(sigc::mem_fun(*this, &GUIManager::server_GotView));

	//we need this here just to force the linker to also link in the WidgetDefinitions
	WidgetDefinitions w;

	try {

		S_LOG_INFO("Starting CEGUI");
		mDefaultScheme = "EmberLook";
		S_LOG_VERBOSE("Setting default scheme to "<< mDefaultScheme);

		if (configService.getPrefix() != "") {
			//We need to set the current directory to the prefix before trying to load CEGUI.
			//The reason for this is that CEGUI loads a lot of dynamic modules, and in some build configuration
			//(like AppImage) the lookup path for some of these are based on the installation directory of Ember.
			if (chdir(configService.getPrefix().c_str())) {
				S_LOG_WARNING("Failed to change to the prefix directory '" << configService.getPrefix() << "'. Gui loading might fail.");
			}
		}

		//The OgreCEGUIRenderer is the main interface between Ogre and CEGUI. Note that the third argument tells the renderer to render the gui after all of the regular render queues have been processed, thus making sure that the gui always is on top.
		mGuiRenderer = &CEGUI::OgreRenderer::create(*window);
		mOgreResourceProvider = &CEGUI::OgreRenderer::createOgreResourceProvider();
		mOgreResourceProvider->setDefaultResourceGroup("Gui");

		mOgreImageCodec = &CEGUI::OgreRenderer::createOgreImageCodec();

		IScriptingProvider* provider = EmberServices::getSingleton().getScriptingService().getProviderFor("LuaScriptingProvider");
		if (provider != 0) {
			Lua::LuaScriptingProvider* luaScriptProvider = static_cast<Lua::LuaScriptingProvider*>(provider);
			mLuaScriptModule = &LuaScriptModule::create(luaScriptProvider->getLuaState());
			if (luaScriptProvider->getErrorHandlingFunctionName().size() != 0) {
				mLuaScriptModule->setDefaultPCallErrorHandler(luaScriptProvider->getErrorHandlingFunctionName());
				mLuaScriptModule->executeString(""); //We must call this to make CEGUI set up the error function internally. If we don't, CEGUI will never correctly set it up. The reason for this is that we never use the execute* methods in the CEGUI lua module later on, instead loading our scripts ourselves. And CEGUI is currently set up to require the execute* methods to be called in order for the error function to be registered.
			}
			mGuiSystem = &CEGUI::System::create(*mGuiRenderer, mOgreResourceProvider, 0, mOgreImageCodec, mLuaScriptModule, "cegui/datafiles/configs/cegui.config");

			EmberServices::getSingleton().getScriptingService().EventStopping.connect(sigc::mem_fun(*this, &GUIManager::scriptingServiceStopping));
		} else {
			mGuiSystem = &CEGUI::System::create(*mGuiRenderer, mOgreResourceProvider, 0, mOgreImageCodec, 0, "cegui/datafiles/configs/cegui.config");
		}
		CEGUI::SchemeManager::SchemeIterator schemeI(SchemeManager::getSingleton().getIterator());
		if (schemeI.isAtEnd()) {
			// 			S_LOG_FAILURE("Could not load any CEGUI schemes. This means that there's something wrong with how CEGUI is setup. Check the CEGUI log for more detail. We'll now exit Ember.");
			throw Exception("Could not load any CEGUI schemes. This means that there's something wrong with how CEGUI is setup. Check the CEGUI log for more detail. We'll now exit Ember.");
		}

		mNativeClipboardProvider = new Ember::Cegui::ScrapNativeClipboardProvider();
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
	for (WidgetStore::iterator I = widgetStoreCopy.begin(); I != widgetStoreCopy.end(); ++I) {
		S_LOG_INFO("Deleting widget " << (*I)->getPrefix() << ".");
		delete *I;
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
	widgetsToLoad.push_back("IngameChatWidget");
	//	widgetsToLoad.push_back("JesusEdit");
	widgetsToLoad.push_back("Help");
	widgetsToLoad.push_back("MeshPreview");

	for (std::vector<std::string>::iterator I = widgetsToLoad.begin(); I != widgetsToLoad.end(); ++I) {
		try {
			S_LOG_VERBOSE("Loading widget " << *I);
			createWidget(*I);
		} catch (const std::exception& e) {
			S_LOG_FAILURE("Error when initializing widget " << *I << "." << e);
		}
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
}

void GUIManager::entity_Talk(const Domain::EntityTalk& entityTalk, EmberEntity* entity)
{
	AppendIGChatLine.emit(entityTalk, entity);
}

void GUIManager::scriptingServiceStopping()
{
	mGuiSystem->setScriptingModule(0);
	if (mLuaScriptModule) {
		LuaScriptModule::destroy(*mLuaScriptModule);
	}
	mLuaScriptModule = 0;
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
		return 0;
	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Error when creating new window of type " << windowType << " with name " << windowName << "." << ex);
		return 0;
	}
}

Widget* GUIManager::createWidget()
{
	return createWidget("Widget");
}

Widget* GUIManager::createWidget(const std::string& name)
{
	Widget* widget(0);
	try {

		widget = WidgetLoader::createWidget(name);
		if (widget == 0) {
			S_LOG_FAILURE("Could not find widget with name " << name);
			return 0;
		}
		widget->init(this);
		widget->buildWidget();
		addWidget(widget);
		S_LOG_INFO("Successfully loaded widget " << name);
	} catch (const std::exception& e) {
		S_LOG_FAILURE("Error when loading widget " << name << "." << e);
		return 0;
	}
	return widget;
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
	WidgetStore::iterator I = std::find(mWidgets.begin(), mWidgets.end(), widget);
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
	WidgetStore::iterator I = mWidgets.begin();
	WidgetStore::iterator I_end = mWidgets.end();

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

void GUIManager::pressedKey(const SDL_keysym& key, Input::InputMode inputMode)
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

		if (mGuiRenderer->isRenderingEnabled()) {
			S_LOG_INFO("Disabling GUI");
			mGuiRenderer->setRenderingEnabled(false);

			getInput().removeAdapter(mCEGUIAdapter);

		} else {
			S_LOG_INFO("Enabling GUI");
			mGuiRenderer->setRenderingEnabled(true);

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
	getInput().setInputMode(Input::IM_MOVEMENT);
	mSheet->removeChild(mWorldLoadingScreen->getWindowPtr());

}

void GUIManager::EmberOgre_WorldCreated(World& world)
{
	mEntityTooltip = new EntityTooltip(world, *static_cast<EmberEntityTooltipWidget*>(mWindowManager->createWindow("EmberLook/EntityTooltip", "EntityTooltip")), *mIconManager);
	mCursorWorldListener = new CursorWorldListener(mMainLoopController, *mSheet, world.getMainCamera());
	mSheet->addChild(mWorldLoadingScreen->getWindowPtr());
}

void GUIManager::EmberOgre_WorldDestroyed()
{
	delete mEntityTooltip;
	mEntityTooltip = 0;
	delete mCursorWorldListener;
	mCursorWorldListener = 0;
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

