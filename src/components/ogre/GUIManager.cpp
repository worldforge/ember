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

#include "EmberOgrePrerequisites.h"
#include "GUIManager.h"
#include <CEGUISchemeManager.h>
#include <CEGUIExceptions.h>
#include <elements/CEGUIStaticText.h>
#include <elements/CEGUIPushButton.h>

#include "widgets/Widget.h"
#include "widgets/ConsoleWidget.h"
// #include "widgets/ServerBrowserWidget.h"
// #include "widgets/ServerWidget.h"
// #include "widgets/ChatWidget.h"
// #include "widgets/EntityPickerWidget.h"
// #include "widgets/InventoryWidget.h"
// #include "widgets/InspectWidget.h"
// #include "widgets/MakeEntityWidget.h"
// #include "widgets/GiveWidget.h"
// #include "widgets/DebugWidget.h"
// #include "widgets/IngameChatWidget.h"
#include "MousePicker.h"

#include "EmberEventProcessor.h"
#include "AvatarCamera.h"
#include "EmberOgre.h"
#include "input/Input.h"

#include "widgets/WidgetDefinitions.h"

#include <SDL.h>

namespace EmberOgre {


GUIManager::GUIManager(Ogre::RenderWindow* window, Ogre::SceneManager* sceneMgr) 
: mWindow(window), mInput(0)

{

// 	mEventProcessor = new EmberEventProcessor();
// 	mEventProcessor->initialise(Ogre::Root::getSingleton().getAutoCreatedWindow());
// 	mEventProcessor->startProcessingEvents();
	WidgetDefinitions w;
	
	try {
	
		fprintf(stdout, "STARTING CEGUI\n");
		
		mGuiRenderer = new CEGUI::OgreCEGUIRenderer(window, Ogre::RENDER_QUEUE_OVERLAY, false, 0, sceneMgr);
		//mGuiRenderer->setTargetSceneManager(sceneMgr);
		mGuiSystem = new CEGUI::System(mGuiRenderer); 
		
		mWindowManager = &CEGUI::WindowManager::getSingleton();


		Ember::ConfigService* configSrv = Ember::EmberServices::getInstance()->getConfigService();
		//chdir(configSrv->getEmberDataDirectory().c_str());
		dlopen( "libCEGUITaharezLook.so", RTLD_NOW );
		fprintf(stderr, dlerror());
		try {	
//			CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"cegui/datafiles/schemes/DAoC.scheme");
			CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"cegui/datafiles/schemes/TaharezLook.scheme");
			
			try {
				mGuiSystem->setDefaultMouseCursor((CEGUI::utf8*)"TaharezLookMouse", (CEGUI::utf8*)"MouseArrow");
			} catch (CEGUI::Exception& e) {
				fprintf(stderr, "CEGUI - could not set mouse pointer. Make sure that the correct scheme (TaharezLook) is available.\n");
			}
			try {
				mGuiSystem->setDefaultFont((CEGUI::utf8*)"Tahoma-10"); 
//				mGuiSystem->setDefaultFont((CEGUI::utf8*)"Tahoma-8"); 
			} catch (CEGUI::Exception& e) {
				fprintf(stderr, "CEGUI - could not set default font.\n");
			}
		
		
		} catch (CEGUI::Exception& e) {
			fprintf(stderr, "CEGUI - could not create default scheme.\n");
		}
		
		
		mSheet = mWindowManager->createWindow((CEGUI::utf8*)"DefaultGUISheet", (CEGUI::utf8*)"root_wnd");
		mGuiSystem->setGUISheet(mSheet); 
		BIND_CEGUI_EVENT(mSheet, CEGUI::ButtonBase::EventMouseButtonDown, GUIManager::mSheet_MouseButtonDown);
		BIND_CEGUI_EVENT(mSheet, CEGUI::Window::EventInputCaptureLost, GUIManager::mSheet_CaptureLost);
			

		fprintf(stderr, "CEGUI - SYSTEM SET UP\n");

		MousePicker* picker = new MousePicker();
		pushMousePicker(picker);
		
		mInput = new Input(mGuiSystem, mGuiRenderer);
		mInput->KeyPressed.connect(SigC::slot(*this, &GUIManager::pressedKey));
		
		Ogre::Root::getSingleton().addFrameListener(this);
	
	} catch (CEGUI::Exception& e) {
		fprintf(stderr, "GUIManager - error when creating gui.\n");
	
	}

}


GUIManager::~GUIManager()
{
	delete mInput;
	mInput = 0;
	
	//can't delete this one since cegui will then throw a segfault
/*	delete mGuiSystem;
	mGuiSystem = 0;*/
	delete mGuiRenderer;
	mGuiRenderer = 0;
	//delete mMousePicker;
	//mMousePicker = 0;

}

void GUIManager::initialize()
{
	Ember::ConfigService* configSrv = Ember::EmberServices::getInstance()->getConfigService();
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
	

		mConsoleWidget = dynamic_cast<ConsoleWidget*>(createWidget("ConsoleWidget"));
		createWidget("IngameChatWidget");
		createWidget("DebugWidget");
		createWidget("Performance");
		createWidget("ChatWidget");
		createWidget("InventoryWidget");
		createWidget("ServerBrowserWidget");
		createWidget("InspectWidget");
		createWidget("MakeEntityWidget");
		createWidget("JesusEdit");
		createWidget("ServerWidget");
		createWidget("GiveWidget");
		createWidget("EntityPickerWidget");
		createWidget("Help");

/*		CEGUI::Window* helpWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)"cegui/widgets/HelpWidget.xml", "Help/");
		mSheet->addChildWindow(helpWindow );*/
	} catch (std::exception& e) {
		fprintf(stderr, "GUIManager - error when initializing widgets.\n");

	} catch (CEGUI::Exception& e) {
		fprintf(stderr, "GUIManager - error when initializing widgets.\n");
	}
	
}

Widget* GUIManager::createWidget(const std::string& name)
{
	Widget* widget;
	try {
	
		widget = WidgetLoader::createWidget(name);
		if (widget == 0) {
			S_LOG_FAILURE( "Could not find widget with name " << name )
			return 0;
		}
		widget->init(this);
		widget->buildWidget();
		addWidget(widget);
		S_LOG_INFO(  "Successfully loaded widget " << name )
	} catch (std::exception& e) {
		S_LOG_FAILURE(  "Error when loading widget " << name )
		return 0;
	} catch (CEGUI::Exception& e) {
		S_LOG_FAILURE(  "Error when loading widget " << name )
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


const std::string GUIManager::takeScreenshot() 
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
	const std::string dir = Ember::EmberServices::getInstance()->getConfigService()->getHomeDirectory() + "/screenshots/";
	struct stat tagStat;
	int ret;
	ret = stat( dir.c_str(), &tagStat );
	if (ret == -1) {
		mkdir(dir.c_str(), S_IRWXU);
	}
	
	// take screenshot
	mWindow->writeContentsToFile(dir + filename.str());
	return filename.str();
}



bool GUIManager::frameStarted(const Ogre::FrameEvent& evt)
{
		

	mInput->processInput(evt);

	//iterate over all widgets and send them a frameStarted event
	std::set<Widget*>::iterator I = mWidgets.begin();
	std::set<Widget*>::iterator I_end = mWidgets.end();
	
	for (; I != I_end; ++I) {
		Widget* aWidget = *I;
		aWidget->frameStarted(evt);
	}
	
	
	return true;


}

bool GUIManager::mSheet_MouseButtonDown(const CEGUI::EventArgs& args)
{
	const CEGUI::MouseEventArgs& mouseArgs = dynamic_cast<const CEGUI::MouseEventArgs&>(args);
	fprintf(stderr, "CEGUI - MAIN SHEET CAPTURING INPUT\n");
	CEGUI::Window* aWindow = CEGUI::Window::getCaptureWindow();
	if (aWindow) {
		aWindow->releaseInput();
		aWindow->deactivate();
	}
	mSheet->activate();
	mSheet->captureInput();

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
	fprintf(stderr, "CEGUI - MAIN SHEET RELEASE INPUT\n");
	return true;
}

const bool GUIManager::isInMovementKeysMode() const {
	return mSheet->isCapturedByThis() || !isInGUIMode(); 
}

const bool GUIManager::isInGUIMode() const { 
	return mInput && mInput->isInGUIMode(); 
}


void GUIManager::pressedKey(const SDL_keysym& key, bool isInGuimode)
{
 		//toggle the console
 		//we've put it here because we wan't the console to always be available
 		if(key.sym == SDLK_F12 && mConsoleWidget)
		{
			mConsoleWidget->toggleActive();
/*			t->buildWidget();*/
		}

		//take screenshot		
		if(key.sym == SDLK_F8)
		{
			setDebugText("Wrote image: " +takeScreenshot());
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


template<> GUIManager* Ember::Singleton<GUIManager>::ms_Singleton = 0;

}


