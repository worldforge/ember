/*
    Copyright (C) 2004  Miguel Guzman (Aganor)

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

#include "GUIManager.h"
#include "services/EmberServices.h"
#include "services/config/ConfigService.h"
#include "services/scripting/ScriptingService.h"

#include <CEGUIWindowManager.h>
#include <CEGUISchemeManager.h>
#include <CEGUIExceptions.h>
#include <CEGUIFactoryModule.h>
#include <elements/CEGUIStaticText.h>
#include <elements/CEGUIPushButton.h>

#include "widgets/Widget.h"
#include "widgets/ConsoleWidget.h"
#include "MousePicker.h"

#include "AvatarCamera.h"
#include "EmberOgre.h"
#include "input/Input.h"

#include "widgets/WidgetDefinitions.h"

#include "EmberEntity.h"
#include "EmberPhysicalEntity.h"
// #include "PersonEmberEntity.h"
#include "AvatarEmberEntity.h"

#include "framework/IScriptingProvider.h"

//#include "GUIScriptManager.h"
#include "components/ogre/scripting/LuaScriptingProvider.h"

#include "GUICEGUIAdapter.h"

// #include <SDL.h>

#ifdef __WIN32__
#include <windows.h>
#include <direct.h>
#endif

#include "EntityWorldPickListener.h"

template<> EmberOgre::GUIManager* Ember::Singleton<EmberOgre::GUIManager>::ms_Singleton = 0;

namespace EmberOgre {


GUIManager::GUIManager(Ogre::RenderWindow* window, Ogre::SceneManager* sceneMgr) 
: mWindow(window), mGuiRenderer(0), mGuiCommandMapper("gui", "key_bindings_gui")
, ToggleInputMode("toggle_inputmode", this, "Toggle the input mode.")
, ReloadGui("reloadgui", this, "Reloads the gui.")
{
	mGuiCommandMapper.restrictToInputMode(Input::IM_GUI );

	///we need this here just to force the linker to also link in the WidgetDefinitions
	WidgetDefinitions w;
	
	try {
		
		S_LOG_INFO("Starting CEGUI");
		mDefaultScheme = "EmberLook";
		S_LOG_VERBOSE("Setting default scheme to "<< mDefaultScheme);
	
		Ember::ConfigService* configSrv = Ember::EmberServices::getSingletonPtr()->getConfigService();
		chdir(configSrv->getEmberDataDirectory().c_str());
		
		//use a macro from CEGUIFactoryModule
		DYNLIB_LOAD( "libCEGUIFalagardBase.so");
		
		Ember::EmberServices::getSingleton().getScriptingService()->registerScriptingProvider(new LuaScriptingProvider());
		
		
		mGuiRenderer = new CEGUI::OgreCEGUIRenderer(window, Ogre::RENDER_QUEUE_OVERLAY, false, 0, sceneMgr);
//		mScriptManager = new GUIScriptManager();
		CEGUI::ResourceProvider* resourceProvider = mGuiRenderer->createResourceProvider();
		resourceProvider->setDefaultResourceGroup("Gui");
		
		Ember::IScriptingProvider* provider;
		provider = Ember::EmberServices::getSingleton().getScriptingService()->getProviderFor("LuaScriptingProvider");
		if (provider != 0) {
			LuaScriptingProvider* luaScriptProvider = static_cast<LuaScriptingProvider*>(provider);
			mGuiSystem = new CEGUI::System(mGuiRenderer, & luaScriptProvider->getScriptModule(), resourceProvider, (CEGUI::utf8*)"cegui/datafiles/configs/cegui.config"); 
			mScriptingProviders.push_back(provider);
		} else {
			mGuiSystem = new CEGUI::System(mGuiRenderer, resourceProvider, (CEGUI::utf8*)"cegui/datafiles/configs/cegui.config"); 
		}
		
		mWindowManager = &CEGUI::WindowManager::getSingleton();


			
		try {
			mGuiSystem->setDefaultMouseCursor(getDefaultScheme(), (CEGUI::utf8*)"MouseArrow");
		} catch (const CEGUI::Exception& ex) {
			S_LOG_FAILURE("CEGUI - could not set mouse pointer. Make sure that the correct scheme " << getDefaultScheme() << " is available. Message: " << ex.getMessage().c_str());
			throw Ember::Exception(ex.getMessage().c_str());
		}
		
		
		mSheet = mWindowManager->createWindow((CEGUI::utf8*)"DefaultGUISheet", (CEGUI::utf8*)"root_wnd");
		mGuiSystem->setGUISheet(mSheet); 
		mSheet->activate();
		mSheet->moveToBack();

		BIND_CEGUI_EVENT(mSheet, CEGUI::ButtonBase::EventMouseButtonDown, GUIManager::mSheet_MouseButtonDown);
		BIND_CEGUI_EVENT(mSheet, CEGUI::Window::EventInputCaptureLost, GUIManager::mSheet_CaptureLost);
			
		//set a default tool tip
		CEGUI::System::getSingleton().setTooltip(getDefaultScheme() + "/Tooltip");
		
		S_LOG_INFO("CEGUI system set up");

		mPicker = new MousePicker();
		
		///create a new entity world pick listener which listens for event
		///TODO: should this really be here?
		mEntityWorldPickListener = new EntityWorldPickListener();
		
		///don't connect it yet since there's no AvatarCamera yet, wait until that's created
		EmberOgre::getSingleton().EventAvatarControllerCreated.connect(sigc::mem_fun(*this, &GUIManager::EmberOgre_AvatarControllerCreated));
		
		getInput().EventKeyPressed.connect(sigc::mem_fun(*this, &GUIManager::pressedKey));
		getInput().setInputMode(Input::IM_GUI);
		
		///add adapter for CEGUI
		mCEGUIAdapter = new GUICEGUIAdapter(mGuiSystem, mGuiRenderer);
		getInput().addAdapter(mCEGUIAdapter);
		
		mGuiCommandMapper.bindToInput(getInput());
		
		///connect to the creation of the avatar, since we want to switch to movement mode when that happens
		EmberOgre::getSingleton().EventCreatedAvatarEntity.connect(sigc::mem_fun(*this, &GUIManager::EmberOgre_CreatedAvatarEntity));
		
		Ogre::Root::getSingleton().addFrameListener(this);
		
	
	} catch (const CEGUI::Exception& ex) {
		S_LOG_FAILURE("GUIManager - error when creating gui. Message: " << ex.getMessage().c_str());
		throw Ember::Exception(ex.getMessage().c_str());
	
	}

}


GUIManager::~GUIManager()
{
	S_LOG_INFO("Shutting down GUI manager.");
	//can't delete this one since cegui will then throw a segfault
/*	delete mGuiSystem;
	mGuiSystem = 0;*/
	
	for (std::set<Widget*>::iterator I = mWidgets.begin(); I != mWidgets.end(); ++I) {
		delete *I;
	}
	
	for (std::vector<Ember::IScriptingProvider*>::iterator I = mScriptingProviders.begin(); I != mScriptingProviders.end(); ++I) {
		delete *I;
	}
	
	
	Ogre::Root::getSingleton().removeFrameListener(this);
	delete mCEGUIAdapter;
	
	delete mEntityWorldPickListener;
	delete mPicker;
	delete mGuiRenderer;
	//delete mMousePicker;
	//mMousePicker = 0;

}

void GUIManager::initialize()
{
	Ember::ConfigService* configSrv = Ember::EmberServices::getSingletonPtr()->getConfigService();
	chdir(configSrv->getEmberDataDirectory().c_str());
	try {
		mDebugText = (CEGUI::StaticText*)mWindowManager->createWindow(getDefaultScheme() + "/StaticText", (CEGUI::utf8*)"DebugText");
		mSheet->addChildWindow(mDebugText);
		mDebugText->setMaximumSize(CEGUI::Size(1.0f, 0.1f));
		mDebugText->setPosition(CEGUI::Point(0.0f, 0.93f));
		mDebugText->setSize(CEGUI::Size(1.0f, 0.1f));
		
		mDebugText->setFrameEnabled(false);
		mDebugText->setBackgroundEnabled(false);
		//stxt->setHorizontalFormatting(StaticText::WordWrapCentred);
	

		//the console and quit widgets are not lua scripts, and should be loaded explicit
		mConsoleWidget = static_cast<ConsoleWidget*>(createWidget("ConsoleWidget"));
		if (!mConsoleWidget) {
			throw Ember::Exception("Could not create console widget.");
		}
		createWidget("Quit");
	} catch (const std::exception& e) {
		S_LOG_FAILURE("GUIManager - error when initializing widgets: " << e.what());
		throw e;
	} catch (const CEGUI::Exception& e) {
		S_LOG_FAILURE("GUIManager - error when initializing widgets: " << e.getMessage().c_str());
		throw e;
	}
	try {
		//this should be defined in some kind of text file, which should be different depending on what game you're playing (like mason)
		try {
		//load the bootstrap script which will load all other scripts
			Ember::EmberServices::getSingleton().getScriptingService()->loadScript("cegui/datafiles/lua_scripts/Bootstrap.lua");
		} catch (const Ogre::Exception& ex)
		{
			S_LOG_FAILURE("Error when loading bootstrap script. Error message: " << ex.getFullDescription());
		}
		
		createWidget("StatusIconBar");
		createWidget("IngameChatWidget");
		createWidget("InventoryWidget");
		createWidget("InspectWidget");
		createWidget("MakeEntityWidget");
		createWidget("JesusEdit");
		createWidget("ServerWidget");
		createWidget("Help");
		createWidget("MeshPreview");

	} catch (const std::exception& e) {
		S_LOG_FAILURE("GUIManager - error when initializing widgets: " << e.what());

	} catch (const CEGUI::Exception& e) {
		S_LOG_FAILURE("GUIManager - error when initializing widgets: " << e.getMessage().c_str());
	}
	
}

void GUIManager::EmitEntityAction(const std::string& action, EmberEntity* entity)
{
	EventEntityAction.emit(action, entity);
}


Widget* GUIManager::createWidget()
{
	return createWidget("Widget");
}

Widget* GUIManager::createWidget(const std::string& name)
{
	Widget* widget;
	try {
	
		widget = WidgetLoader::createWidget(name);
		if (widget == 0) {
			S_LOG_FAILURE( "Could not find widget with name " << name );
			return 0;
		}
		widget->init(this);
		widget->buildWidget();
		addWidget(widget);
		S_LOG_INFO(  "Successfully loaded widget " << name );
	} catch (const std::exception& e) {
		S_LOG_FAILURE(  "Error when loading widget " << name );
		return 0;
	} catch (const CEGUI::Exception& e) {
		S_LOG_FAILURE(  "Error when loading widget " << name );
		return 0;
	}
	return widget;
}


void GUIManager::setDebugText(const std::string& text)
{
	if (mDebugText)
	{
		mDebugText->setText(text);
	}
}

Input& GUIManager::getInput() const
{
	return EmberOgre::getSingleton().getInput();
}


CEGUI::Window* GUIManager::getMainSheet() const
{ 
	return mSheet; 
}

void GUIManager::removeWidget(Widget* widget)
{
	mWidgets.erase(widget);
}

void GUIManager::addWidget(Widget* widget)
{
	mWidgets.insert(widget);
}






bool GUIManager::frameStarted(const Ogre::FrameEvent& evt)
{
	try {	
		CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);
	} catch (const CEGUI::Exception& ex) {
		S_LOG_WARNING("Error in CEGUI: " << ex.getMessage().c_str());
	}
// 	if (mPreviousInputMode == IM_GUI) {
// 		if (!mInput->getInputMode()) {
// 			EventInputModeChanged.emit(IM_MOVEMENT);
// 			mPreviousInputMode = IM_MOVEMENT;
// 		}
// 	} else {
// 		if (mInput->isInGUIMode()) {
// 			EventInputModeChanged.emit(IM_GUI);
// 			mPreviousInputMode = IM_GUI;
// 		}
// 	}
	
	
	
	//iterate over all widgets and send them a frameStarted event
	std::set<Widget*>::iterator I = mWidgets.begin();
	std::set<Widget*>::iterator I_end = mWidgets.end();
	
	for (; I != I_end; ++I) {
		Widget* aWidget = *I;
		try {
			aWidget->frameStarted(evt);
		} catch (const CEGUI::Exception& ex) {
			S_LOG_WARNING("Error in CEGUI: " << ex.getMessage().c_str());
		}
	}
	
	EventFrameStarted.emit(evt.timeSinceLastFrame);
	
	return true;


}

bool GUIManager::mSheet_MouseButtonDown(const CEGUI::EventArgs& args)
{
	
	const CEGUI::MouseEventArgs& mouseArgs = static_cast<const CEGUI::MouseEventArgs&>(args);
	S_LOG_VERBOSE("Main sheet is capturing input");
	CEGUI::Window* aWindow = CEGUI::Window::getCaptureWindow();
	if (aWindow) {
		aWindow->releaseInput();
		aWindow->deactivate();
	}
	//mSheet->activate();
	//mSheet->captureInput();

	if (mPicker) {
		const CEGUI::Point& position = CEGUI::MouseCursor::getSingleton().getDisplayIndependantPosition();
		MousePickerArgs pickerArgs;
		pickerArgs.windowX = mouseArgs.position.d_x;
		pickerArgs.windowY = mouseArgs.position.d_y;
		mPicker->doMousePicking(position.d_x, position.d_y, pickerArgs);
	}


	return true;
}

bool GUIManager::mSheet_CaptureLost(const CEGUI::EventArgs& args)
{
	S_LOG_VERBOSE("Main sheet lost input");
	return true;
}

const bool GUIManager::isInMovementKeysMode() const {
	return mSheet->isCapturedByThis() || !isInGUIMode(); 
}

const bool GUIManager::isInGUIMode() const { 
	return getInput().getInputMode() == Input::IM_GUI; 
}

void GUIManager::pressedKey(const SDL_keysym& key, Input::InputMode inputMode)
{
}



void GUIManager::runCommand(const std::string &command, const std::string &args)
{
	if (command == ToggleInputMode.getCommand()) {
		getInput().toggleInputMode();
	} else if (command == ReloadGui.getCommand()) {
		Ogre::TextureManager* texMgr = Ogre::TextureManager::getSingletonPtr();
		Ogre::ResourcePtr resource = texMgr->getByName("cegui/" + getDefaultScheme() + ".png");
		if (!resource.isNull()) {
			resource->reload();
		}
	}
}

// void GUIManager::pushMousePicker( MousePicker * mousePicker )
// {
// 	mMousePickers.push(mousePicker);
// }

// MousePicker * GUIManager::popMousePicker()
// {
// 	///only pop if there's more than one registered picker
// 	if (mMousePickers.size() > 1) 
// 		mMousePickers.pop();
// 	return mMousePickers.top();
// }

void GUIManager::EmberOgre_CreatedAvatarEntity(AvatarEmberEntity* entity)
{
	///switch to movement mode, since it appears most people don't know how to change from gui mode
	getInput().setInputMode(Input::IM_MOVEMENT);
}

void GUIManager::EmberOgre_AvatarControllerCreated(AvatarController& controller)
{
	EmberOgre::getSingleton().getMainCamera()->pushWorldPickListener(mEntityWorldPickListener);
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

EntityWorldPickListener* GUIManager::getEntityPickListener() const
{
	return mEntityWorldPickListener;
}


}


