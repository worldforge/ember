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

#include "widgets/ServerWidget.h"
#include "widgets/Widget.h"
#include "widgets/ConsoleWidget.h"
#include "widgets/ChatWidget.h"
#include "widgets/EntityPickerWidget.h"
#include "widgets/InventoryWidget.h"
#include "widgets/ServerBrowserWidget.h"
#include "widgets/InspectWidget.h"
#include "widgets/MakeEntityWidget.h"
#include "widgets/GiveWidget.h"
#include "widgets/DebugWidget.h"
#include "widgets/IngameChatWidget.h"
#include "MousePicker.h"

#include "EmberEventProcessor.h"
#include "AvatarCamera.h"
#include "EmberOgre.h"
#include "input/Input.h"

#include <SDL.h>

namespace EmberOgre {


GUIManager::GUIManager(Ogre::RenderWindow* window, Ogre::SceneManager* sceneMgr) 
: mWindow(window)
, mKeyListener(0)

{

// 	mEventProcessor = new EmberEventProcessor();
// 	mEventProcessor->initialise(Ogre::Root::getSingleton().getAutoCreatedWindow());
// 	mEventProcessor->startProcessingEvents();

	
	try {
	
		fprintf(stdout, "STARTING CEGUI\n");
		
		mGuiRenderer = new CEGUI::OgreRenderer(window, Ogre::RENDER_QUEUE_OVERLAY, false, 3000, sceneMgr);
		mGuiRenderer->setTargetSceneManager(sceneMgr);
		mGuiSystem = new CEGUI::System(mGuiRenderer); 
		
		mWindowManager = &CEGUI::WindowManager::getSingleton();


		Ember::ConfigService* configSrv = Ember::EmberServices::getInstance()->getConfigService();
		//chdir(configSrv->getEmberDataDirectory().c_str());
	
		try {	
			CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"cegui/datafiles/schemes/TaharezLook.scheme");
			
			try {
				mGuiSystem->setDefaultMouseCursor((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MouseArrow");
			} catch (CEGUI::Exception e) {
				fprintf(stderr, "CEGUI - could not set mouse pointer. Make sure that the correct scheme (TaharezLook) is available.\n");
			}

			try {
				mGuiSystem->setDefaultFont((CEGUI::utf8*)"Tahoma-10"); 
			} catch (CEGUI::Exception e) {
				fprintf(stderr, "CEGUI - could not set default font.\n");
			}
		
		
		} catch (CEGUI::Exception e) {
			fprintf(stderr, "CEGUI - could not create default scheme.\n");
		}
		
		
		mSheet = mWindowManager->createWindow((CEGUI::utf8*)"DefaultGUISheet", (CEGUI::utf8*)"root_wnd");
		mGuiSystem->setGUISheet(mSheet); 
		mSheet->subscribeEvent(CEGUI::ButtonBase::EventMouseButtonDown, 
			boost::bind(&GUIManager::mSheet_MouseButtonDown, this, _1));
		mSheet->subscribeEvent(CEGUI::Window::EventInputCaptureLost, 
			boost::bind(&GUIManager::mSheet_CaptureLost, this, _1));
			

		fprintf(stderr, "CEGUI - SYSTEM SET UP\n");

		mMousePicker = new MousePicker();
		
		mInput = new Input(mGuiSystem, mGuiRenderer);
//	mGUIManager->getInput()->MouseMoved.connect(SigC::slot(*this, &AvatarCamera::mouseMoved));
		
		mInput->KeyPressed.connect(SigC::slot(*this, &GUIManager::pressedKey));
/*		mEventProcessor->addKeyListener(this);
		mEventProcessor->addMouseMotionListener(this);
		mEventProcessor->addMouseListener(this);*/
		Ogre::Root::getSingleton().addFrameListener(this);
	
	} catch (CEGUI::Exception e) {
		fprintf(stderr, "GUIManager - error when creating gui.\n");
	
	}

}


GUIManager::~GUIManager()
{
	delete mInput;
	mInput = 0;
	
	delete mGuiSystem;
	mGuiSystem = 0;
	delete mGuiRenderer;
	mGuiRenderer = 0;
	delete mMousePicker;
	mMousePicker = 0;

}

void GUIManager::initialize()
{
	Ember::ConfigService* configSrv = Ember::EmberServices::getInstance()->getConfigService();
	chdir(configSrv->getEmberDataDirectory().c_str());
	try {
		mDebugText = (CEGUI::StaticText*)mWindowManager->createWindow((CEGUI::utf8*)"TaharezLook/StaticText", (CEGUI::utf8*)"DebugText");
		mSheet->addChildWindow(mDebugText);
		mDebugText->setMaximumSize(CEGUI::Size(1.0f, 0.1f));
		mDebugText->setPosition(CEGUI::Point(0.0f, 0.9f));
		mDebugText->setSize(CEGUI::Size(1.0f, 0.1f));
		
		mDebugText->setFrameEnabled(false);
		mDebugText->setBackgroundEnabled(false);
		//stxt->setHorizontalFormatting(StaticText::WordWrapCentred);
	
		fprintf(stdout, "CEGUI - CREATING CONSOLE\n");
	
		mConsoleWidget = new ConsoleWidget(this);
		mConsoleWidget->buildWidget();
		addWidget(mConsoleWidget);
		fprintf(stdout, "CEGUI - CREATED CONSOLE\n");
	
		IngameChatWidget* ingamechatWidget = new IngameChatWidget(this);
		ingamechatWidget->buildWidget();
		addWidget(ingamechatWidget);		
		
		
		DebugWidget* debugWidget = new DebugWidget(this);
		debugWidget->buildWidget();
		addWidget(debugWidget);
		
		
		ChatWidget* chatWidget = new ChatWidget(this);
		chatWidget->buildWidget();
		addWidget(chatWidget);		
		
		
		InventoryWidget* inventory = new InventoryWidget(this);
		inventory->buildWidget();
		addWidget(inventory);			
		
		ServerBrowserWidget* serverBrowser = new ServerBrowserWidget(this);
		serverBrowser->buildWidget();
		addWidget(serverBrowser);
		
		InspectWidget* inspectBrowser = new InspectWidget(this);
		inspectBrowser->buildWidget();
		addWidget(inspectBrowser);
		
		MakeEntityWidget* makeEntity = new MakeEntityWidget(this);
		makeEntity->buildWidget();
		addWidget(makeEntity);
		
		ServerWidget* serverWidget = new ServerWidget(this);
		serverWidget->buildWidget();
		addWidget(serverWidget);
		
		GiveWidget* giveWidget = new GiveWidget(this);
		giveWidget->buildWidget();
		addWidget(giveWidget);
		
		EntityPickerWidget* entityPicker = new EntityPickerWidget(this);
		entityPicker->buildWidget();
		addWidget(entityPicker);		
		
		CEGUI::Window* helpWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)"cegui/widgets/HelpWidget.xml", "Help/");
		mSheet->addChildWindow(helpWindow );
	} catch (std::exception e) {
		fprintf(stderr, "GUIManager - error when initializing widgets.\n");

	} catch (CEGUI::Exception e) {
		fprintf(stderr, "GUIManager - error when initializing widgets.\n");
	}
	
}

void GUIManager::setDebugText(std::string text)
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
	
	// take screenshot
	mWindow->writeContentsToFile(filename.str());
	return filename.str();
}


// void GUIManager::pollMouse(const Ogre::FrameEvent& evt)
// {
// 	int mouseX, mouseY;
// 	unsigned int mouseState;
// 	
// 	mouseState = SDL_GetMouseState( &mouseX, &mouseY );
// 	
// 	if (mouseState & SDL_BUTTON(SDL_BUTTON_LMASK)) {
// 		if (mMouseState & !SDL_BUTTON(SDL_BUTTON_LMASK)) {
// 			//left mouse button pressed
// 			mGuiSystem->injectMouseButtonDown(CEGUI::LeftButton);
// 		}
// 	} else if (mMouseState & SDL_BUTTON(SDL_BUTTON_LMASK)) {
// 		//left mouse button released
// 		mGuiSystem->injectMouseButtonUp(CEGUI::LeftButton);
// 
// 	}
// 	
// 	if (mouseState & SDL_BUTTON(SDL_BUTTON_RMASK)) {
// 		if (mMouseState & !SDL_BUTTON(SDL_BUTTON_RMASK)) {
// 			//right mouse button pressed
// 			
// 			//if the right mouse button is pressed, switch from gui mode
// 			
// 			//SDL_WM_GrabInput(SDL_GRAB_ON);
// 			mInGUIMode = false;
// 			
// 		}
// 	} else if (mMouseState & SDL_BUTTON(SDL_BUTTON_RMASK)) {
// 		//right mouse button released
// 		//SDL_WM_GrabInput(SDL_GRAB_ON);
// 		mInGUIMode = true;
// 	}
// 	
// 	if (mouseState & SDL_BUTTON(SDL_BUTTON_MMASK)) {
// 		if (mMouseState & !SDL_BUTTON(SDL_BUTTON_MMASK)) {
// 			//middle mouse button pressed
// 			mGuiSystem->injectMouseButtonDown(CEGUI::MiddleButton);
// 			
// 		}
// 	} else if (mMouseState & SDL_BUTTON(SDL_BUTTON_MMASK)) {
// 		//middle mouse button released
// 		mGuiSystem->injectMouseButtonUp(CEGUI::MiddleButton);
// 	}
// 	
// 	mMouseState = mouseState;
// 	
// 	
// 	
// 	//has the mouse moved?
// 	if (mMouseX != mouseX || mMouseY != mouseY)
// 	{
// 
// 		if (mInGUIMode) {
// 			
// 			CEGUI::MouseCursor::getSingleton().setPosition(CEGUI::Point((mouseX), (mouseY))); 
// 			mGuiSystem->injectMouseMove(0.0f, 0.0f);
// 			
// 			
// /*			std::stringstream ss;
// 	//		ss << (e->getX() * mGuiRenderer->getWidth()) << ":" << e->getY() * mGuiRenderer->getHeight();
// 			ss << (e->getX() * mGuiRenderer->getWidth()) << " ( " << e->getX() << " ) :" << (e->getY() * mGuiRenderer->getHeight()) << " ( " << e->getY() << " )";
// 			setDebugText(ss.str());*/
// 			
// 			//mGuiSystem->injectMouseMove(e->getRelX() * mGuiRenderer->getWidth(), e->getRelY() * mGuiRenderer->getHeight());
// 		} else {
// 			Ogre::Real diffX, diffY;
// 			int width = mGuiRenderer->getWidth();
// 			int height = mGuiRenderer->getHeight();
// 			diffX = ( width / mMouseX) - (width / mouseX);
// 			diffY = ( height / mMouseY) - (height / mouseY);
// 			if (mMouseMotionListener) {
// 				mMouseMotionListener->mouseMoved(mouseX, mouseY, diffX, diffX, evt.timeSinceLastFrame):
// //				mMouseMotionListener->mouseMoved(e);
// 			}
// 			
// 			//keep the cursor in place while in non-gui mode
// 			mouseX = mMouseX;
// 			mouseY = mMouseY;
// 			SDL_WarpMouse(mMouseX, mMouseY);
// 		}
// 	
// 	}
// 	mMouseX = mouseX;
// 	mMouseY = mouseY;
// 
// }

bool GUIManager::frameStarted(const Ogre::FrameEvent& evt)
{
		
		
		//std::stringstream ss;
//		ss << (e->getX() * mGuiRenderer->getWidth()) << ":" << e->getY() * mGuiRenderer->getHeight();
		//ss << (e->getX() * mGuiRenderer->getWidth()) << " ( " << e->getX() << " ) :" << (e->getY() * mGuiRenderer->getHeight()) << " ( " << e->getY() << " )";
		//setDebugText(ss.str());

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

	if (mMousePicker) {
		CEGUI::Point position = CEGUI::MouseCursor::getSingleton().getDisplayIndependantPosition();
		Ogre::Real x = position.d_x;
		Ogre::Real y = position.d_y;

		mMousePicker->doMousePicking(x, y, mouseArgs);
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
	return mInput->isInGUIMode(); 
}


void GUIManager::pressedKey(const SDL_keysym& key, bool isInGuimode)
{
 		//toggle the console
 		//we've put it here because we wan't the console to always be available
 		if(key.sym == SDLK_F12)
		{
			mConsoleWidget->toggleActive();
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


// void GUIManager::keyPressed (Ogre::KeyEvent *e)
// {
// 	if (mInGUIMode) {
// 		// do event injection
// 		// key down
// 		mGuiSystem->injectKeyDown(e->getKey());
// 	
// 		// now character
// 		mGuiSystem->injectChar(e->getKeyChar());
// 	
// 		e->consume();
// 	} else {
// 		if (mKeyListener) {
// 			mKeyListener->keyPressed(e);
// 		}
// 	}
// 	
// }
// 
// 
// void GUIManager::keyReleased (Ogre::KeyEvent *e)
// {
// 
// 	if (mInGUIMode) {
// 		//toggle the console
// 		//we've put it here because we wan't the console to always be available
// 		if(e->getKey() == Ogre::KC_GRAVE || e->getKey() == Ogre::KC_F12)
// 		{
// 			mConsoleWidget->toggleActive();
// 		}
// 
// 		//take screenshot		
// 		if(e->getKey() == Ogre::KC_F8)
// 		{
// 			setDebugText("Wrote image: " +takeScreenshot());
// 		}
// 		
// 		//switch render mode
// 		if(e->getKey() == Ogre::KC_F7)
// 		{
// 			setDebugText("Switching rendemode.");
// 			Ogre::Camera* ogreCamera = EmberOgre::getSingleton().getMainCamera()->getCamera();
// 			if (ogreCamera->getDetailLevel() == Ogre::SDL_SOLID) {
// 				ogreCamera->setDetailLevel(Ogre::SDL_WIREFRAME);
// 			} else {
// 				ogreCamera->setDetailLevel(Ogre::SDL_SOLID);
// 			}
// 		}
// 		
// 		//switch between full screen
// /*		if(e->getKey() == Ogre::KC_F6)
// 		{
// 			mWindow->
// 		}*/
// 		
// 		
// 		mGuiSystem->injectKeyUp(e->getKey());
// 		e->consume();
// 	} else {
// 		if (mKeyListener) {
// 			mKeyListener->keyReleased(e);
// 		}
// 	}
// 
// 
// }



// void GUIManager::mousePressed (Ogre::MouseEvent *e)
// {
// 	mMousePressedOgreEvent = e;
// 	//if the right mouse button is pressed, switch from gui mode
// 	if (e->getButtonID() == Ogre::MouseEvent::BUTTON1_MASK) {
// 	
// 		SDL_WM_GrabInput(SDL_GRAB_ON);
// 		mInGUIMode = false;
// 	}
// 	if (mInGUIMode) {
// 		mGuiSystem->injectMouseButtonDown(convertOgreButtonToCegui(e->getButtonID()));
// 		e->consume();
// 	} else {
// 		if (mMouseListener) {
// 			mMouseListener->mousePressed(e);
// 		}
// 	}
// 
// }
// 
// 
// void GUIManager::mouseReleased (Ogre::MouseEvent *e)
// {
// 	mMouseReleasedOgreEvent = e;
// 	//if the right mouse button is released, switch to gui mode
// 	if (e->getButtonID() == Ogre::MouseEvent::BUTTON1_MASK) {
// 		SDL_WM_GrabInput(SDL_GRAB_OFF);
// 		mInGUIMode = true;
// 	}
// 	if (mInGUIMode) {
// 		mGuiSystem->injectMouseButtonUp(convertOgreButtonToCegui(e->getButtonID()));
// 		e->consume();
// 	} else {
// 		if (mMouseListener) {
// 			mMouseListener->mouseReleased(e);
// 		}
// 	}
// 
// }

// CEGUI::MouseButton GUIManager::convertOgreButtonToCegui(int ogre_button_id)
// {
// 	switch (ogre_button_id)
// 	{
// 	case Ogre::MouseEvent::BUTTON0_MASK:
// 		return CEGUI::LeftButton;
// 		break;
// 
// 	case Ogre::MouseEvent::BUTTON1_MASK:
// 		return CEGUI::RightButton;
// 		break;
// 
// 	case Ogre::MouseEvent::BUTTON2_MASK:
// 		return CEGUI::MiddleButton;
// 		break;
// 
// 	case Ogre::MouseEvent::BUTTON3_MASK:
// 		return CEGUI::X1Button;
// 		break;
// 
// 	default:
// 		return CEGUI::LeftButton;
// 		break;
// 	}
// 
// }




template<> GUIManager* Ember::Singleton<GUIManager>::ms_Singleton = 0;

}
