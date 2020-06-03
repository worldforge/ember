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
#include <Eris/Avatar.h>

#include <OgreRenderWindow.h>
#include <OgreRoot.h>
#include <OgreTextureManager.h>

#include <CEGUI/WindowManager.h>
#include <CEGUI/SchemeManager.h>
#include <CEGUI/FactoryModule.h>
#include <CEGUI/ScriptModules/Lua/ScriptModule.h>
#include "CEGUIOgreRenderer/ResourceProvider.h"
#include "CEGUIOgreRenderer/ImageCodec.h"
#include <CEGUI/widgets/MultiLineEditbox.h>
#include <CEGUI/widgets/Editbox.h>
#include "components/cegui/CEGUISetup.h"
#include "components/ogre/widgets/HitDisplayer.h"

#ifdef _WIN32
#include "platform/platform_windows.h"
#endif

using namespace CEGUI;
using namespace Ember::OgreView::Gui;
using namespace Ember;

namespace Ember {
namespace OgreView {

unsigned long GUIManager::msAutoGenId(0);

GUIManager::GUIManager(Cegui::CEGUISetup& ceguiSetup, ConfigService& configService, ServerServiceSignals& serverSignals, MainLoopController& mainLoopController) :
		ToggleInputMode("toggle_inputmode", this, "Toggle the input mode."),
		ReloadGui("reloadgui", this, "Reloads the gui."),
		ToggleGui("toggle_gui", this, "Toggle the gui display"),
		mCeguiSetup(ceguiSetup),
		mConfigService(configService),
		mMainLoopController(mainLoopController),
		mGuiCommandMapper("gui", "key_bindings_gui"),
		mSheet(nullptr),
		mWindowManager(nullptr),
		mLuaScriptModule(nullptr),
		mEnabled(true),
		mRenderedStringParser(std::make_unique<Cegui::ColouredRenderedStringParser>()),
		mQuickHelp(std::make_unique<Gui::QuickHelp>()),
		mEntityTooltip(nullptr),
		mNativeClipboardProvider(std::make_unique<Ember::Cegui::SDLNativeClipboardProvider>()),
		mWidgetDefinitions(new WidgetDefinitions()){



	mGuiCommandMapper.restrictToInputMode(Input::IM_GUI);

	serverSignals.GotView.connect(sigc::mem_fun(*this, &GUIManager::server_GotView));


	try {

		S_LOG_INFO("Starting CEGUI");
		mDefaultScheme = "EmberLook";
		S_LOG_VERBOSE("Setting default scheme to " << mDefaultScheme);


		IScriptingProvider* provider = EmberServices::getSingleton().getScriptingService().getProviderFor("LuaScriptingProvider");
		if (provider != nullptr) {
			auto* luaScriptProvider = dynamic_cast<Lua::LuaScriptingProvider*>(provider);
			mLuaScriptModule = &LuaScriptModule::create(luaScriptProvider->getLuaState());
			if (!luaScriptProvider->getErrorHandlingFunctionName().empty()) {
				mLuaScriptModule->setDefaultPCallErrorHandler(luaScriptProvider->getErrorHandlingFunctionName());
				mLuaScriptModule->executeString("");
				// We must call this to make CEGUI set up the error function internally. If we don't, CEGUI will never correctly set it up.
				// The reason for this is that we never use the execute* methods in the CEGUI lua module later on,
				// instead loading our scripts ourselves. And CEGUI is currently set up
				// to require the execute* methods to be called in order for
				// the error function to be registered.
			}
			mCeguiSetup.getSystem().setScriptingModule(mLuaScriptModule);

			EmberServices::getSingleton().getScriptingService().EventStopping.connect(sigc::mem_fun(*this, &GUIManager::scriptingServiceStopping));
		}

		mCeguiSetup.getSystem().getClipboard()->setNativeProvider(mNativeClipboardProvider.get());

		mCeguiSetup.getSystem().setDefaultCustomRenderedStringParser(mRenderedStringParser.get());
		mWindowManager = &CEGUI::WindowManager::getSingleton();

		EntityTooltip::registerFactory();

		mSheet.reset(mWindowManager->createWindow("DefaultWindow", "root_wnd"));
		mCeguiSetup.getSystem().getDefaultGUIContext().setRootWindow(mSheet.get());
		mSheet->activate();
		mSheet->moveToBack();
		mSheet->setDistributesCapturedInputs(false);

		mWorldLoadingScreen = std::make_unique<WorldLoadingScreen>();

		S_LOG_INFO("CEGUI system set up");

		getInput().EventKeyPressed.connect(sigc::mem_fun(*this, &GUIManager::pressedKey));
		getInput().setInputMode(Input::IM_GUI);

		//add adapter for CEGUI, this will route input event to the gui
		mCEGUIAdapter = std::make_unique<GUICEGUIAdapter>(mCeguiSetup.getSystem(), mCeguiSetup.getRenderer());
		getInput().addAdapter(mCEGUIAdapter.get());

		mGuiCommandMapper.bindToInput(getInput());

		//connect to the creation of the avatar, since we want to switch to movement mode when that happens
		EmberOgre::getSingleton().EventCreatedAvatarEntity.connect(sigc::mem_fun(*this, &GUIManager::EmberOgre_CreatedAvatarEntity));
		EmberOgre::getSingleton().EventWorldCreated.connect(sigc::mem_fun(*this, &GUIManager::EmberOgre_WorldCreated));
		EmberOgre::getSingleton().EventWorldBeingDestroyed.connect(sigc::mem_fun(*this, &GUIManager::EmberOgre_WorldBeingDestroyed));

		mActiveWidgetHandler = std::make_unique<Gui::ActiveWidgetHandler>(*this);

		Ogre::Root::getSingleton().addFrameListener(this);


	} catch (const CEGUI::Exception& ex) {
		S_LOG_FAILURE("GUIManager - error when creating gui." << ex);
		throw Exception(ex.getMessage().c_str());
	}

	if (chdir(configService.getEmberDataDirectory().generic_string().c_str())) {
		S_LOG_WARNING("Failed to change to the data directory '" << configService.getEmberDataDirectory().string() << "'.");
	}

	mWidgetDefinitions->registerWidgets(*this);


}

GUIManager::~GUIManager() {
	S_LOG_INFO("Shutting down GUI manager.");
	mCeguiSetup.getSystem().setDefaultCustomRenderedStringParser(nullptr);
	mWorldLoadingScreen.reset();
	if (mCEGUIAdapter) {
		getInput().removeAdapter(mCEGUIAdapter.get());
	}
	mWidgetDefinitions.reset();
	mWidgets.clear();
	WidgetLoader::removeAllWidgetFactories();
	CEGUI::WindowManager::getSingleton().cleanDeadPool();
	EntityTooltip::deregisterFactory();

	Ogre::Root::getSingleton().removeFrameListener(this);
}

void GUIManager::initialize() {
	try {
		createWidget("Quit");
	} catch (const std::exception& e) {
		S_LOG_FAILURE("GUIManager - error when initializing widgets." << e);
		throw e;
	}
	try {
		mIconManager = std::make_unique<Gui::Icons::IconManager>();
	} catch (const std::exception& e) {
		S_LOG_FAILURE("GUIManager - error when creating icon manager." << e);
	}

	try {
		mEntityIconManager = std::make_unique<Gui::EntityIconManager>(*this);
	} catch (const std::exception& e) {
		S_LOG_FAILURE("GUIManager - error when creating entity icon manager." << e);
	}
	try {
		mActionBarIconManager = std::make_unique<Gui::ActionBarIconManager>(*this);
	} catch (const std::exception& e) {
		S_LOG_FAILURE("GUIManager - error when creating ActionBar icon manager." << e);
	}

	std::vector<std::string> widgetsToLoad;
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

void GUIManager::render() {
	if (mEnabled) {
		CEGUI::System::getSingleton().renderAllGUIContexts();
	}
}

void GUIManager::server_GotView(Eris::View* view) {
	//The View has a shorter lifespan than ours, so we don't need to store references to the connections.
	view->EntityCreated.connect(sigc::mem_fun(*this, &GUIManager::view_EntityCreated));
	view->EntitySeen.connect(sigc::mem_fun(*this, &GUIManager::view_EntityCreated));
}

void GUIManager::view_EntityCreated(Eris::Entity* entity) {
	//It's safe to cast to EmberEntity, since all entities in the system are guaranteed to be of this type.
	auto* emberEntity = dynamic_cast<EmberEntity*>(entity);
	//The Entity has a shorter lifespan than ours, so we don't need to store references to the connections.
	emberEntity->EventTalk.connect(sigc::bind(sigc::mem_fun(*this, &GUIManager::entity_Talk), emberEntity));
	emberEntity->Emote.connect(sigc::bind(sigc::mem_fun(*this, &GUIManager::entity_Emote), emberEntity));
}

void GUIManager::entity_Talk(const EntityTalk& entityTalk, EmberEntity* entity) {
	AppendIGChatLine.emit(entityTalk, entity);
}

void GUIManager::entity_Emote(const std::string& description, EmberEntity* entity) {
	//If it's our own entity we should just print what it says.
	if (entity == entity->getView()->getAvatar().getEntity()) {
		AppendAvatarImaginary(description);
	} else {
		AppendAvatarImaginary(entity->getName() + " " + description);
	}
}

void GUIManager::scriptingServiceStopping() {
	mCeguiSetup.getSystem().setScriptingModule(nullptr);
	if (mLuaScriptModule) {
		LuaScriptModule::destroy(*mLuaScriptModule);
	}
	mLuaScriptModule = nullptr;
}

void GUIManager::EmitEntityAction(const std::string& action, EmberEntity* entity) {
	EventEntityAction.emit(action, entity);
}

CEGUI::Window* GUIManager::createWindow(const std::string& windowType) {
	std::stringstream ss;
	ss << "_autoWindow_" << (msAutoGenId++);
	return createWindow(windowType, ss.str());
}

CEGUI::Window* GUIManager::createWindow(const std::string& windowType, const std::string& windowName) {
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

Widget* GUIManager::createWidget() {
	return createWidget("Widget");
}

Widget* GUIManager::createWidget(const std::string& name) {
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

void GUIManager::destroyWidget(Widget* widget) {
	if (!widget) {
		S_LOG_WARNING("Trying to destroy null widget.");
		return;
	}
	removeWidget(widget);
}

CEGUI::Texture& GUIManager::createTexture(Ogre::TexturePtr& ogreTexture, std::string name) {
	if (name.empty()) {
		name = ogreTexture->getName();
	}
	return mCeguiSetup.getRenderer().createTexture(name, ogreTexture);
}

Input& GUIManager::getInput() const {
	return Input::getSingleton();
}

CEGUI::Window* GUIManager::getMainSheet() const {
	return mSheet.get();
}

void GUIManager::removeWidget(Widget* widget) {
	auto I = std::find_if(mWidgets.begin(), mWidgets.end(), [widget](const std::unique_ptr<Widget>& item) { return item.get() == widget; });
	if (I != mWidgets.end()) {
		mWidgets.erase(I);
	}
}

void GUIManager::addWidget(Widget* widget) {
	mWidgets.emplace_back(std::unique_ptr<Widget>(widget));
}

bool GUIManager::frameStarted(const Ogre::FrameEvent& evt) {
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
		auto& aWidget = *I;
		try {
			aWidget->frameStarted(evt);
		} catch (const CEGUI::Exception& ex) {
			S_LOG_WARNING("Error in CEGUI." << ex);
		}
	}

	EventFrameStarted.emit(evt.timeSinceLastFrame);

	return true;

}

bool GUIManager::isInMovementKeysMode() const {
	return mSheet->isCapturedByThis() || !isInGUIMode();
}

bool GUIManager::isInGUIMode() const {
	return getInput().getInputMode() == Input::IM_GUI;
}

void GUIManager::pressedKey(const SDL_Keysym& key, Input::InputMode inputMode) {
	if (((key.mod & KMOD_CTRL) || (key.mod & KMOD_LCTRL) || (key.mod & KMOD_RCTRL))) {
		if (key.sym == SDLK_c) {
			mCeguiSetup.getSystem().getDefaultGUIContext().injectCopyRequest();
		} else if (key.sym == SDLK_x) {
			mCeguiSetup.getSystem().getDefaultGUIContext().injectCutRequest();
		} else if (key.sym == SDLK_v) {
			mCeguiSetup.getSystem().getDefaultGUIContext().injectPasteRequest();
		}
	}
}

void GUIManager::runCommand(const std::string& command, const std::string& args) {
	if (command == ToggleInputMode.getCommand()) {
		getInput().toggleInputMode();
	} else if (command == ToggleGui.getCommand()) {

		S_LOG_VERBOSE("Toggle Gui Initiated -- " << getInput().getInputMode());

		if (mEnabled) {
			S_LOG_INFO("Disabling GUI");
			mEnabled = false;

			getInput().removeAdapter(mCEGUIAdapter.get());

		} else {
			S_LOG_INFO("Enabling GUI");
			mEnabled = true;

			getInput().addAdapter(mCEGUIAdapter.get());
		}
	} else if (command == ReloadGui.getCommand()) {
		Ogre::TextureManager* texMgr = Ogre::TextureManager::getSingletonPtr();
		Ogre::ResourcePtr resource = texMgr->getByName("cegui/" + getDefaultScheme() + ".png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		if (resource) {
			resource->reload();
		}
	}
}

void GUIManager::EmberOgre_CreatedAvatarEntity(EmberEntity& entity) {
	//switch to movement mode, since it appears most people don't know how to change from gui mode
	varconf::Variable var;
	if (!EmberServices::getSingleton().getConfigService().getValue("input", "automovementmode", var)
		|| (var.is_bool() && (bool) var)) {
		getInput().setInputMode(Input::IM_MOVEMENT);
	}
}

void GUIManager::EmberOgre_WorldCreated(World& world) {
	UniqueWindowPtr<EmberEntityTooltipWidget> tooltipWindow(dynamic_cast<EmberEntityTooltipWidget*>(mWindowManager->createWindow("EmberLook/EntityTooltip", "EntityTooltip")));
	mEntityTooltip = std::make_unique<EntityTooltip>(world, std::move(tooltipWindow), *mIconManager);
	mCursorWorldListener = std::make_unique<CursorWorldListener>(mMainLoopController, *mSheet, world);

	UniqueWindowPtr<CEGUI::Window> labelWindow(WindowManager::getSingleton().loadLayoutFromFile(GUIManager::getSingleton().getLayoutDir() + "Hit.layout"));

	mHitDisplayer = std::make_unique<HitDisplayer>(*mSheet, labelWindow, world.getMainCamera().getCamera(), world.getView(), world.getSceneManager());
}

void GUIManager::EmberOgre_WorldBeingDestroyed() {
	mHitDisplayer.reset();
	mEntityTooltip.reset();
	mCursorWorldListener.reset();
}

Gui::EntityTooltip* GUIManager::getEntityTooltip() const {
	return mEntityTooltip.get();
}

std::string GUIManager::getLayoutDir() const {
	return "cegui/datafiles/layouts/";
}

const std::string& GUIManager::getDefaultScheme() const {
	return mDefaultScheme;
}

Gui::Icons::IconManager* GUIManager::getIconManager() const {
	return mIconManager.get();
}

Gui::EntityIconManager* GUIManager::getEntityIconManager() const {
	return mEntityIconManager.get();
}

Gui::ActionBarIconManager* GUIManager::getActionBarIconManager() const {
	return mActionBarIconManager.get();
}

CEGUI::Renderer* GUIManager::getGuiRenderer() const {
	return &mCeguiSetup.getRenderer();
}

}
}

