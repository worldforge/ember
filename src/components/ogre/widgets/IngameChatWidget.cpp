//
// C++ Implementation: IngameChatWidget
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2005
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//



#include "Widget.h"
#include "../GUIManager.h"
#include "../EmberEntity.h"

#include <CEGUIWindowManager.h>
#include <CEGUIExceptions.h>
#include <CEGUIFont.h>
#include <elements/CEGUIListbox.h> 
#include <elements/CEGUIListboxTextItem.h> 
#include <elements/CEGUIPushButton.h>
#include <elements/CEGUIGUISheet.h>
#include <Eris/Entity.h>
#include "IngameChatWidget.h"

#include "../EmberOgre.h"
#include "../AvatarCamera.h"
#include "../Avatar.h"

#include "../EmberEntity.h"
#include "../EmberPhysicalEntity.h"
#include "../AvatarEmberEntity.h"

#include "services/EmberServices.h"
#include "services/config/ConfigService.h"
#include "services/server/ServerService.h"


using namespace CEGUI;
namespace EmberOgre {


IngameChatWidget::~IngameChatWidget()
{
}

void IngameChatWidget::buildWidget()
{
	Ember::ConfigService* configSrv = Ember::EmberServices::getSingletonPtr()->getConfigService();
	if (configSrv->itemExists("ingamechatwidget", "timeshown")) {
		timeShown = (double)configSrv->getValue("ingamechatwidget", "timeshown");
	} else {
		timeShown = 0;
	}
	
	if (configSrv->itemExists("ingamechatwidget", "distanceshown")) {
		distanceShown = (double)configSrv->getValue("ingamechatwidget", "distanceshown");
	} else {
		distanceShown = 100;
	}
	
	//mMainWindow = WindowManager::getSingleton().loadWindowLayout((utf8*)"widgets/ChatWidget.xml", "Chat/");
//	mMainWindow->setAlwaysOnTop(true);
/*	mMainWindow = mWindowManager->createWindow((utf8*)"DefaultGUISheet", (utf8*)"IngameChatWidget/MainWindow");
	mMainWindow->setPosition(Point(0.0f, 0.0f));
	mMainWindow->setSize(Size(1.0f, 1.0f));
	mMainWindow->setVisible(true);
	mMainWindow->setEnabled(true);
	
//	mChatTextBox = static_cast<MultiLineEditbox*>(WindowManager::getSingleton().getWindow((utf8*)"Chat/TextBox"));
	getMainSheet()->addChildWindow(mMainWindow); */
	
	mGuiManager->AppendIGChatLine.connect(sigc::mem_fun(*this, &IngameChatWidget::appendIGChatLine));

}

void IngameChatWidget::appendIGChatLine(const std::string& line, EmberEntity* entity)
{
	//don't show anything if it's the avatar
	AvatarEmberEntity* avatarEntity = EmberOgre::getSingleton().getAvatar()->getAvatarEmberEntity();
	if (avatarEntity == entity) {
		return;
	}
	

	Window* chatWindow;
	ActiveChatWindowMap::iterator I = mActiveChatWindows.find(entity->getId());
	if (I == mActiveChatWindows.end()) {
		Ogre::Vector3 entityWorldCoords = entity->getWorldBoundingBox(true).getCenter();
		Ogre::Vector3 cameraCoords = EmberOgre::getSingletonPtr()->getMainCamera()->getCamera()->getDerivedPosition();
		Ogre::Vector3 diff = entityWorldCoords - cameraCoords;
		
		if (diff.length() <= distanceShown) {
		
			//there is no chat window for this entity, let's create one
			chatWindow = WindowManager::getSingleton().loadWindowLayout(mGuiManager->getLayoutDir()+"IngameChatWidget.xml", std::string("IngameChatWidget/") + entity->getId() + "/");
			getMainSheet()->addChildWindow(chatWindow);
			
			ActiveChatWindow* activeWindow = new ActiveChatWindow(chatWindow, entity, mWindowManager, *this);
			
			mActiveChatWindows[entity->getId()] = activeWindow;
			
			mActiveChatWindows[entity->getId()]->updateText(line);
		}
	} else {
		I->second->updateText(line);
	}

}






void IngameChatWidget::frameStarted( const Ogre::FrameEvent & event )
{
	ActiveChatWindowMap::iterator I = mActiveChatWindows.begin();
	ActiveChatWindowMap::iterator I_end = mActiveChatWindows.end();
	std::vector<std::string> windowsToRemove;
	
	for (;I != I_end; ++I) {
		ActiveChatWindow* window = I->second;
		window->frameStarted(event );

	
		Ogre::Vector3 entityWorldCoords = window->getEntity()->getWorldBoundingBox(true).getCenter();
		
		//Ogre::Vector3 entityWorldCoords = window->getEntity()->getSceneNode()->_getWorldAABB().getCenter();
		Ogre::Vector3 cameraCoords = EmberOgre::getSingletonPtr()->getMainCamera()->getCamera()->getDerivedPosition();
		Ogre::Vector3 diff = entityWorldCoords - cameraCoords;
		
		if (diff.length() > distanceShown) {
			windowsToRemove.push_back(I->first);
		} else {
		
			//unless timeShown is 0 windows will fade over time
			if (timeShown != 0) {
				//make the windows fade over time
				window->getWindow()->setAlpha(1 - (window->getElapsedTimeSinceLastUpdate() / timeShown));
				if (window->getElapsedTimeSinceLastUpdate() >= timeShown) {
					windowsToRemove.push_back(I->first);
				}
			}
		}
	}
	
	std::vector<std::string>::iterator J = windowsToRemove.begin();
	std::vector<std::string>::iterator J_end = windowsToRemove.end();
	for (;J != J_end; ++J) {
//		mWindowManager->destroyWindow(mActiveChatWindows[*J].window);
		removeWidget(*J);
	}	
	
}

void IngameChatWidget::removeWidget(const std::string& windowName)
{
	delete mActiveChatWindows[windowName];
	mActiveChatWindows.erase(windowName);
}

// void IngameChatWidget::removeWidget(ActiveChatWindow* window)
// {
// 	delete mActiveChatWindows[window];
// 	mActiveChatWindows.erase(window);
// }



};


bool EmberOgre::IngameChatWidget::ActiveChatWindow::buttonResponse_Click(const EventArgs& args)
{
	const MouseEventArgs *mouseArgs = static_cast<const MouseEventArgs*>(&args);
	if (mouseArgs) {
		///each button contains a static text window, which is the one containg the actual text
		const String text = mouseArgs->window->getChild(0)->getText();
		Ember::EmberServices::getSingletonPtr()->getServerService()->say(std::string(text.c_str()));
	}
//	removeMenu();
	return true;
}

void EmberOgre::IngameChatWidget::ActiveChatWindow::updateText( const std::string & line )
{
	
	GUISheet* textWidget = static_cast<GUISheet*>(mWindow->getChild(std::string("IngameChatWidget/") + mEntity->getId() + "/Text"));
	textWidget->setText(line);
	mElapsedTimeSinceLastUpdate = 0;
	
	if (mEntity->hasSuggestedResponses())
	{
		Window* responseWidget = static_cast<Window*>(mWindow->getChild(std::string("IngameChatWidget/") + mEntity->getId() + "/" + "ResponseList"));
			
		
		//remove all existing response windows
		std::vector<Window*>::const_iterator responses_I = mResponseTextWidgets.begin();
		std::vector<Window*>::const_iterator responses_I_end = mResponseTextWidgets.end();
		for (; responses_I != responses_I_end; ++responses_I)
		{
			mWindowManager->destroyWindow(*responses_I);
		}
		mResponseTextWidgets.clear();
		

		//for each response, create a button
		std::vector<std::string> responses = mEntity->getSuggestedResponses();
		std::vector<std::string>::const_iterator I = responses.begin();
		std::vector<std::string>::const_iterator I_end = responses.end();
		int i = 0;
		std::stringstream ss;
		
		float heightSize = 1.0f;
		if (responses.size() > 0) {
			heightSize = 1.0f / responses.size();
		}
		
		for (;I != I_end; ++I)
		{
			std::stringstream ss_;
			ss_ << i;
			PushButton* responseTextButton = static_cast<PushButton*>(mWindowManager->createWindow(GUIManager::getSingleton().getDefaultScheme() + "/Button", std::string("IngameChatWidget/") + mEntity->getId() + "/Response/" + ss_.str()));
			GUISheet* responseText = static_cast<GUISheet*>(mWindowManager->createWindow(GUIManager::getSingleton().getDefaultScheme() + "/StaticText", std::string("IngameChatWidget/") + mEntity->getId() + "/ResponseText/" + ss_.str()));
			
			
			
			BIND_CEGUI_EVENT(responseTextButton, ButtonBase::EventMouseClick,IngameChatWidget::ActiveChatWindow::buttonResponse_Click );
			responseText->setText(*I);
			responseText->setSize(UVector2(UDim(0.8f, 0), UDim(0.9f, 0)));
			responseText->setPosition(UVector2(UDim(0.1f, 0), UDim(0.05f, 0)));
			responseText->setProperty("HorzFormatting", "WordWrapLeftAligned");
 			responseText->setProperty("FrameEnabled", "false");
 			responseText->setProperty("BackgroundEnabled", "false");
			responseText->setInheritsAlpha(true);
			///we need to disable and deactivate it so it won't recieve any input (input should go to the button instead)
			responseText->deactivate();
			responseText->disable();
			
			
			responseTextButton->setSize(UVector2(UDim(1.0f, 0), UDim(heightSize, 0.0f)));
			responseTextButton->setPosition(UVector2(UDim(0.0f, 0),UDim(i * heightSize, 0.0f)));
			responseTextButton->setInheritsAlpha(true);	
			///hide the button
			//responseTextButton->setAlpha(0.0f);	
			responseTextButton->addChildWindow(responseText);
			responseTextButton->setTooltipText(*I);
			responseWidget->addChildWindow(responseTextButton);
			mResponseTextWidgets.push_back(responseTextButton);
			
			++i;
			
// 			ss << *I << "\n";
		}
		//responseWidget->setText(ss.str());
		
	}

}

EmberOgre::IngameChatWidget::ActiveChatWindow::ActiveChatWindow( Window * window, EmberEntity * entity, WindowManager * windowManager, IngameChatWidget& containerWidget)
 : mWindow(window), 
 mElapsedTimeSinceLastUpdate(0.0f), 
 mEntity(entity), 
 mWindowManager(windowManager), 
 mContainerWidget(containerWidget)
{
	entity->BeingDeleted.connect(sigc::mem_fun(*this, &ActiveChatWindow::entity_BeingDeleted));
	try {
		Window* nameWidget = static_cast<Window*>(mWindow->getChild(std::string("IngameChatWidget/") + mEntity->getId() + "/" + "EntityName"));
		nameWidget->setText(entity->getName());
	} catch (const Exception& ex) {
		S_LOG_FAILURE("Could not get widget EntityName. Exception: " << ex.getMessage().c_str());
	}
	
	
	placeWindowOnEntity();
}

EmberOgre::IngameChatWidget::ActiveChatWindow::~ActiveChatWindow()
{
	mWindowManager->destroyWindow(mWindow);
}

void EmberOgre::IngameChatWidget::ActiveChatWindow::entity_BeingDeleted()
{
	mContainerWidget.removeWidget(this->getEntity()->getId());
}

void EmberOgre::IngameChatWidget::ActiveChatWindow::placeWindowOnEntity()
{
	//make sure that the window stays on the entity
	Ogre::Vector3 screenCoords;
	
	bool result = false;
	Ogre::Vector3 entityWorldCoords = getEntity()->getWorldBoundingSphere(true).getCenter();
	entityWorldCoords.y = getEntity()->getWorldBoundingBox(true).getMaximum().y;
	//check what the new position is in screen coords
	result = EmberOgre::getSingletonPtr()->getMainCamera()->worldToScreen(entityWorldCoords, screenCoords);
	
	if (result) {
		mWindow->setVisible(true);
		mWindow->setPosition(UVector2(UDim(screenCoords.x, -(mWindow->getWidth().asAbsolute(0) * 0.5)), UDim(screenCoords.y,  -(mWindow->getHeight().asAbsolute(0) * 0.5))));
	} else {
		mWindow->setVisible(false);
	}
}
void EmberOgre::IngameChatWidget::ActiveChatWindow::frameStarted( const Ogre::FrameEvent & event )
{
	placeWindowOnEntity();
	
	increaseElapsedTime(event.timeSinceLastFrame);
	

}

EmberOgre::EmberEntity * EmberOgre::IngameChatWidget::ActiveChatWindow::getEntity( )
{
	return mEntity;
}

Window * EmberOgre::IngameChatWidget::ActiveChatWindow::getWindow( )
{
	return mWindow;
}

void EmberOgre::IngameChatWidget::ActiveChatWindow::increaseElapsedTime( float timeSlice )
{
	mElapsedTimeSinceLastUpdate += timeSlice;
}


