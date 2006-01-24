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

#include "services/EmberServices.h"
#include "services/config/ConfigService.h"
#include "services/scripting/ScriptingService.h"

#include "EmberOgrePrerequisites.h"
#include "GUIManager.h"
#include <CEGUIWindowManager.h>
#include <CEGUISchemeManager.h>
#include <CEGUIExceptions.h>
#include <CEGUIFactoryModule.h>
#include <elements/CEGUIStaticText.h>
#include <elements/CEGUIPushButton.h>

#include "widgets/Widget.h"
#include "widgets/ConsoleWidget.h"
#include "MousePicker.h"

#include "EmberEventProcessor.h"
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

#include <SDL.h>

#ifdef __WIN32__
#include <windows.h>
#include <direct.h>
#endif

template<> EmberOgre::GUIManager* Ember::Singleton<EmberOgre::GUIManager>::ms_Singleton = 0;

namespace EmberOgre {

const std::string GUIManager::SCREENSHOT("screenshot");
const std::string GUIManager::TOGGLEINPUTMODE("toggleinputmode");


GUIManager::GUIManager(Ogre::RenderWindow* window, Ogre::SceneManager* sceneMgr) 
: mWindow(window), mInput(0)

{

	WidgetDefinitions w;
	
	try {
		
		S_LOG_INFO("STARTING CEGUI");
	
		Ember::ConfigService* configSrv = Ember::EmberServices::getSingletonPtr()->getConfigService();
		chdir(configSrv->getEmberDataDirectory().c_str());
		
		//use a macro from CEGUIFactoryModule
		DYNLIB_LOAD( "libCEGUITaharezLook.so");
		
		Ember::EmberServices::getSingleton().getScriptingService()->registerScriptingProvider(new LuaScriptingProvider());
		
		
		mGuiRenderer = new CEGUI::OgreCEGUIRenderer(window, Ogre::RENDER_QUEUE_OVERLAY, false, 0, sceneMgr);
//		mScriptManager = new GUIScriptManager();
		Ember::IScriptingProvider* provider;
		provider = Ember::EmberServices::getSingleton().getScriptingService()->getProviderFor("LuaScriptingProvider");
		if (provider != 0) {
			LuaScriptingProvider* luaScriptProvider = static_cast<LuaScriptingProvider*>(provider);
			mGuiSystem = new CEGUI::System(mGuiRenderer, & luaScriptProvider->getScriptModule(), (CEGUI::utf8*)"cegui/datafiles/configs/cegui.config"); 
		
		} else {
			mGuiSystem = new CEGUI::System(mGuiRenderer); 
		}
		//mGuiSystem = new CEGUI::System(mGuiRenderer, &mScriptManager->getScriptModule(), (CEGUI::utf8*)"cegui/datafiles/configs/cegui.config"); 
//		mGuiSystem = new CEGUI::System(mGuiRenderer); 
		
		mWindowManager = &CEGUI::WindowManager::getSingleton();


//		fprintf(stderr, dlerror());
		try {	
//			CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"cegui/datafiles/schemes/DAoC.scheme");
			CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"cegui/datafiles/schemes/TaharezLook.scheme");
			
			try {
				mGuiSystem->setDefaultMouseCursor((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MouseArrow");
			} catch (const CEGUI::Exception&) {
				S_LOG_FAILURE("CEGUI - could not set mouse pointer. Make sure that the correct scheme (TaharezLook) is available.");
			}
			try {
				mGuiSystem->setDefaultFont((CEGUI::utf8*)"Tahoma-10"); 
//				mGuiSystem->setDefaultFont((CEGUI::utf8*)"Tahoma-8"); 
			} catch (const CEGUI::Exception&) {
				S_LOG_FAILURE("CEGUI - could not set default font.");
			}
		
		
		} catch (const CEGUI::Exception&) {
			S_LOG_FAILURE("CEGUI - could not create default scheme.");
		}
		
		
		mSheet = mWindowManager->createWindow((CEGUI::utf8*)"DefaultGUISheet", (CEGUI::utf8*)"root_wnd");
		mGuiSystem->setGUISheet(mSheet); 
		mSheet->activate();
		mSheet->moveToBack();

		BIND_CEGUI_EVENT(mSheet, CEGUI::ButtonBase::EventMouseButtonDown, GUIManager::mSheet_MouseButtonDown);
		BIND_CEGUI_EVENT(mSheet, CEGUI::Window::EventInputCaptureLost, GUIManager::mSheet_CaptureLost);
			
		//set a default tool tip
		CEGUI::System::getSingleton().setTooltip("TaharezLook/Tooltip");
		
		S_LOG_INFO("CEGUI - SYSTEM SET UP");

		MousePicker* picker = new MousePicker();
		pushMousePicker(picker);
					
		mInput = new Input(mGuiRenderer->getWidth(), mGuiRenderer->getHeight());
		mInput->EventKeyPressed.connect(sigc::mem_fun(*this, &GUIManager::pressedKey));
		mInput->setInputMode(Input::IM_GUI);
		
		//add adapter for CEGUI
		mCEGUIAdapter = new GUICEGUIAdapter(mGuiSystem, mGuiRenderer);
		mInput->addAdapter(mCEGUIAdapter);
		
		//connect to the creation of the avatar, since we want to switch to movement mode when that happens
		EmberOgre::getSingleton().EventCreatedAvatarEntity.connect(sigc::mem_fun(*this, &GUIManager::EmberOgre_CreatedAvatarEntity));
		
		Ogre::Root::getSingleton().addFrameListener(this);
		
	
	} catch (const CEGUI::Exception&) {
		S_LOG_FAILURE("GUIManager - error when creating gui.");
	
	}

}


GUIManager::~GUIManager()
{
	delete mInput;
	
	//can't delete this one since cegui will then throw a segfault
/*	delete mGuiSystem;
	mGuiSystem = 0;*/
	delete mGuiRenderer;
	//delete mMousePicker;
	//mMousePicker = 0;

}

void GUIManager::initialize()
{
	Ember::ConfigService* configSrv = Ember::EmberServices::getSingletonPtr()->getConfigService();
	chdir(configSrv->getEmberDataDirectory().c_str());
	try {
		mDebugText = (CEGUI::StaticText*)mWindowManager->createWindow((CEGUI::utf8*)"TaharezLook/StaticText", (CEGUI::utf8*)"DebugText");
		mSheet->addChildWindow(mDebugText);
		mDebugText->setMaximumSize(CEGUI::Size(1.0f, 0.1f));
		mDebugText->setPosition(CEGUI::Point(0.0f, 0.93f));
		mDebugText->setSize(CEGUI::Size(1.0f, 0.1f));
		
		mDebugText->setFrameEnabled(false);
		mDebugText->setBackgroundEnabled(false);
		//stxt->setHorizontalFormatting(StaticText::WordWrapCentred);
	

		//the console and quit widgets are not lua scripts, and should be loaded explicit
		mConsoleWidget = static_cast<ConsoleWidget*>(createWidget("ConsoleWidget"));
		createWidget("Quit");
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
	
	Ember::ConsoleBackend::getMainConsole()->registerCommand(SCREENSHOT,this);

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


// bool GUIManager::confirm(const std::string & text)
// {
// 	CEGUI::Window window = mWindowManager->createWindow((CEGUI::utf8*)"TaharezLook/FrameWindow", (CEGUI::utf8*)"ConfirmBox");
// 	
// 	CEGUI::PushButton yesButton = (CEGUI::PushButton*)mWindowManager->createWindow((CEGUI::utf8*)"TaharezLook/Button", (CEGUI::utf8*)"ConfirmBox/Yes");
// 	yesButton->setText("Yes");
// 	yes
// 	CEGUI::PushButton noButton = (CEGUI::PushButton*)mWindowManager->createWindow((CEGUI::utf8*)"TaharezLook/Button", (CEGUI::utf8*)"ConfirmBox/No");
// 	CEGUI::StaticText text = (CEGUI::PushButton*)mWindowManager->createWindow((CEGUI::utf8*)"TaharezLook/StaticText", (CEGUI::utf8*)"ConfirmBox/Text");
// 	
// 	
// 	window->addChildWindow(yesButton);
// 	window->addChildWindow(noButton);
// 	window->addChildWindow(text);
// 	mSheet->addChildWindow(window);
// 	
// }

void GUIManager::setDebugText(const std::string& text)
{
	if (mDebugText)
	{
		mDebugText->setText(text);
	}
}

Input* GUIManager::getInput() const
{
	return mInput;
}


CEGUI::Window* GUIManager::getMainSheet() 
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


const std::string GUIManager::_takeScreenshot() 
{
	// retrieve current time
	time_t rawtime;
	struct tm* timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	
	// construct filename string
	// padding with 0 for single-digit values
	std::stringstream filename;
	filename << "screenshot_" << ((*timeinfo).tm_year + 1900); // 1900 is year "0"
	int month = ((*timeinfo).tm_mon + 1); // January is month "0"
	if(month <= 9) 
	{
		filename << "0";	
	}
	filename << month;
	int day = (*timeinfo).tm_mday;
	if(day <= 9) 
	{
		filename << "0";	
	}
	filename << day << "_";
	int hour = (*timeinfo).tm_hour;
	if(hour <= 9) 
	{
		filename << "0"; 
	}
	filename << hour;
	int min = (*timeinfo).tm_min;
	if(min <= 9) 
	{
		filename << "0";	 
	}
	filename << min;
	int sec = (*timeinfo).tm_sec;
	if(sec <= 9) 
	{
		filename << "0";
	} 
	filename << sec << ".png";

	//make sure the directory exists
	const std::string dir = Ember::EmberServices::getSingletonPtr()->getConfigService()->getHomeDirectory() + "/screenshots/";
	struct stat tagStat;
	int ret;
	ret = stat( dir.c_str(), &tagStat );
	if (ret == -1) {
#ifdef __WIN32__
		mkdir(dir.c_str());
#else
		mkdir(dir.c_str(), S_IRWXU);
#endif
	}
	
	// take screenshot
	mWindow->writeContentsToFile(dir + filename.str());
	return dir + filename.str();
}



bool GUIManager::frameStarted(const Ogre::FrameEvent& evt)
{
	try {	
		CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);
	} catch (const CEGUI::Exception& ex) {
		S_LOG_WARNING("Error in CEGUI: " << ex.getMessage().c_str());
	}

	
	mInput->processInput(evt);

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
	S_LOG_INFO("CEGUI - MAIN SHEET CAPTURING INPUT");
	CEGUI::Window* aWindow = CEGUI::Window::getCaptureWindow();
	if (aWindow) {
		aWindow->releaseInput();
		aWindow->deactivate();
	}
	//mSheet->activate();
	//mSheet->captureInput();

	if (getMousePicker()) {
		CEGUI::Point position = CEGUI::MouseCursor::getSingleton().getDisplayIndependantPosition();
		Ogre::Real x = position.d_x;
		Ogre::Real y = position.d_y;
		MousePickerArgs pickerArgs;
		pickerArgs.windowX = mouseArgs.position.d_x;
		pickerArgs.windowY = mouseArgs.position.d_y;
		getMousePicker()->doMousePicking(x, y, pickerArgs);
	}


	return true;
}

bool GUIManager::mSheet_CaptureLost(const CEGUI::EventArgs& args)
{
	S_LOG_INFO("CEGUI - MAIN SHEET RELEASE INPUT");
	return true;
}

const bool GUIManager::isInMovementKeysMode() const {
	return mSheet->isCapturedByThis() || !isInGUIMode(); 
}

const bool GUIManager::isInGUIMode() const { 
	return mInput && (mInput->getInputMode() == Input::IM_GUI); 
}

void GUIManager::takeScreenshot()
{
	std::string result = _takeScreenshot();
	result = "Wrote image: " + result;
	setDebugText(result);
	S_LOG_INFO(result);
	Ember::ConsoleBackend::getMainConsole()->pushMessage(result);
}


void GUIManager::pressedKey(const SDL_keysym& key, Input::InputMode inputMode)
{
 		//toggle the console
 		//we've put it here because we wan't the console to always be available
 		if(key.sym == SDLK_F12 && mConsoleWidget)
		{
			mConsoleWidget->toggleActive();
/*			t->buildWidget();*/
		}

		//take screenshot		
		if(key.sym == SDLK_PRINT || key.sym == SDLK_SYSREQ )
		{
			takeScreenshot();
		}
		
		///allow caps lock to toggle input mode
		if (key.sym == SDLK_CAPSLOCK) 
		{
			mInput->toggleInputMode();
		}
		
		//switch render mode
		if(key.sym == SDLK_F7)
		{
			setDebugText("Switching rendermode.");
			Ogre::Camera* ogreCamera = EmberOgre::getSingleton().getMainCamera()->getCamera();
			if (ogreCamera->getDetailLevel() == Ogre::SDL_SOLID) {
				ogreCamera->setDetailLevel(Ogre::SDL_WIREFRAME);
			} else {
				ogreCamera->setDetailLevel(Ogre::SDL_SOLID);
			}
		}
		
		//switch between full screen
/*		if(e->getKey() == Ogre::KC_F6)
		{
			mWindow->
		}*/

}



void GUIManager::runCommand(const std::string &command, const std::string &args)
{
	if(command == SCREENSHOT) {
		//just take a screen shot
		takeScreenshot();
	} else if (command == TOGGLEINPUTMODE) {
		mInput->toggleInputMode();
	}
}

void GUIManager::pushMousePicker( MousePicker * mousePicker )
{
	mMousePickers.push(mousePicker);
}

MousePicker * GUIManager::popMousePicker()
{
	//only pop if there's more than one registered picker
	if (mMousePickers.size() > 1) 
		mMousePickers.pop();
	return mMousePickers.top();
}

void GUIManager::EmberOgre_CreatedAvatarEntity(AvatarEmberEntity* entity)
{
	//switch to movement mode, since it appears most people don't know how to change from gui mode
	getInput()->setInputMode(Input::IM_MOVEMENT);
}

const std::string& GUIManager::getLayoutDir() const
{
	static std::string dir("cegui/datafiles/layouts/");
	return dir;
}



}


