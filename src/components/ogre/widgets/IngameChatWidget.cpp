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
#include "GUIManager.h"
#include "EmberEntity.h"
#include "EmberPhysicalEntity.h"
#include "model/Model.h"

#include <elements/CEGUIListbox.h> 
#include <elements/CEGUIListboxTextItem.h> 
#include <elements/CEGUIStaticText.h> 
#include <Eris/Entity.h>
#include "IngameChatWidget.h"

#include "EmberOgre.h"
#include "AvatarCamera.h"
#include "Avatar.h"

#include "EmberEntity.h"
#include "EmberPhysicalEntity.h"
#include "PersonEmberEntity.h"
#include "AvatarEmberEntity.h"

#include "services/EmberServices.h"
#include "services/config/ConfigService.h"



namespace EmberOgre {

IngameChatWidget::IngameChatWidget(GUIManager* guiManager)
: Widget::Widget(guiManager)
{
	Ember::ConfigService* configSrv = Ember::EmberServices::getInstance()->getConfigService();
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

}


IngameChatWidget::~IngameChatWidget()
{
}

void IngameChatWidget::buildWidget()
{
	
	//mMainWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)"widgets/ChatWidget.xml", "Chat/");
//	mMainWindow->setAlwaysOnTop(true);
/*	mMainWindow = mWindowManager->createWindow((CEGUI::utf8*)"DefaultGUISheet", (CEGUI::utf8*)"IngameChatWidget/MainWindow");
	mMainWindow->setPosition(CEGUI::Point(0.0f, 0.0f));
	mMainWindow->setSize(CEGUI::Size(1.0f, 1.0f));
	mMainWindow->setVisible(true);
	mMainWindow->setEnabled(true);
	
//	mChatTextBox = static_cast<CEGUI::MultiLineEditbox*>(CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Chat/TextBox"));
	getMainSheet()->addChildWindow(mMainWindow); */
	
	mGuiManager->AppendIGChatLine.connect(SigC::slot(*this, &IngameChatWidget::appendIGChatLine));

}

void IngameChatWidget::appendIGChatLine(const std::string& line, EmberEntity* entity)
{
	//don't show anything if it's the avatar
	//TODO: make this better
	AvatarEmberEntity* avatarEntity = dynamic_cast<AvatarEmberEntity*>(entity);
	if (avatarEntity) {
		return;
	}
	
	EmberPhysicalEntity* physicalEntity = dynamic_cast<EmberPhysicalEntity*>(entity);

	if (physicalEntity) {
		CEGUI::Window* chatWindow;
		ActiveChatWindowMap::iterator I = mActiveChatWindows.find(entity->getId());
		if (I == mActiveChatWindows.end()) {
			//there is no chat window for this entity, let's create one
			chatWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)"cegui/widgets/IngameChatWidget.xml", std::string("IngameChatWidget/") + entity->getId() + "/");
			getMainSheet()->addChildWindow(chatWindow);
			
			ActiveChatWindow activeWindow;
			activeWindow.window = chatWindow;
			activeWindow.entity = physicalEntity;
			activeWindow.elapsedTimeSinceLastUpdate = 0.0f;
			activeWindow.windowManager = mWindowManager;
			
			placeWindowOnEntity(chatWindow, physicalEntity);
			mActiveChatWindows[physicalEntity->getId()] = activeWindow;
			
			activeWindow.appendIGChatLine(line);
		} else {
			I->second.elapsedTimeSinceLastUpdate = 0.0f;
			I->second.appendIGChatLine(line);
/*			chatWindow = I->second.window;*/
		}
		
		//chatWindow->setAlpha(1.0f);
		
		//TODO: make this better
		//cegui seems to crash when calling it
		//chatWindow->moveToBack();
		
		
	}

}



void IngameChatWidget::placeWindowOnEntity( CEGUI::Window* window, EmberPhysicalEntity* entity)
{
	//make sure that the window stays on the entity
	Ogre::Vector3 screenCoords;
/*	Ogre::Vector3 entityWorldCoords = entity->getSceneNode()->_getWorldAABB().getCenter();
	
	Ogre::Vector3 entityWorldCoords = entity->getSceneNode()->getWorldPosition();*/
//	entity->_update();
	Ogre::Vector3 entityWorldCoords = entity->getModel()->getWorldBoundingBox(true).getCenter();
	//check what the new position is in screen coords
	bool result = EmberOgre::getSingletonPtr()->getMainCamera()->worldToScreen(entityWorldCoords, screenCoords);
	if (result) {
		window->setVisible(true);
		window->setPosition(CEGUI::Point(screenCoords.x - (window->getWidth() * 0.5), screenCoords.y - (window->getHeight() * 0.5)));
	} else {
		window->setVisible(false);
	}
}



void IngameChatWidget::frameStarted( const Ogre::FrameEvent & event )
{
	ActiveChatWindowMap::iterator I = mActiveChatWindows.begin();
	ActiveChatWindowMap::iterator I_end = mActiveChatWindows.end();
	std::vector<std::string> windowsToRemove;
	
	for (;I != I_end; ++I) {
		ActiveChatWindow* window = &(I->second);
		
		placeWindowOnEntity(window->window, window->entity);
		
		window->elapsedTimeSinceLastUpdate += event.timeSinceLastFrame;
		
		Ogre::Vector3 entityWorldCoords = window->entity->getModel()->getWorldBoundingBox(true).getCenter();
		Ogre::Vector3 cameraCoords = EmberOgre::getSingletonPtr()->getMainCamera()->getCamera()->getDerivedPosition();
		Ogre::Vector3 diff = entityWorldCoords - cameraCoords;
		
		if (diff.length() > distanceShown) {
			windowsToRemove.push_back(I->first);
		} else {
			//unless timeShown is 0 windows will fade over time
			if (timeShown != 0) {
				//make the windows fade over time
				window->window->setAlpha(1 - (window->elapsedTimeSinceLastUpdate / timeShown));
				if (window->elapsedTimeSinceLastUpdate >= timeShown) {
					windowsToRemove.push_back(I->first);
				}
			}
		}
	}
	
	std::vector<std::string>::const_iterator J = windowsToRemove.begin();
	std::vector<std::string>::const_iterator J_end = windowsToRemove.end();
	for (;J != J_end; ++J) {
		mWindowManager->destroyWindow(mActiveChatWindows[*J].window);
		mActiveChatWindows.erase(*J);
	}	
	
}




};


bool EmberOgre::IngameChatWidget::ActiveChatWindow::buttonResponse_Click(const CEGUI::EventArgs& args)
{
	const CEGUI::MouseEventArgs *mouseArgs = dynamic_cast<const CEGUI::MouseEventArgs*>(&args);
	if (mouseArgs) {
		const CEGUI::String text = mouseArgs->window->getText();
		EmberOgre::getSingleton().getAvatar()->getAvatarEmberEntity()->getErisAvatar()->say(std::string(text.c_str()) );
	}
//	removeMenu();
	return true;
}

void EmberOgre::IngameChatWidget::ActiveChatWindow::appendIGChatLine( const std::string & line )
{

	CEGUI::StaticText* textWidget = static_cast<CEGUI::StaticText*>(window->getChild(std::string("IngameChatWidget/") + entity->getId() + "/Text"));
	textWidget->setText(line);
	
	if (entity->hasSuggestedResponses())
	{
		CEGUI::Window* responseWidget = static_cast<CEGUI::Window*>(window->getChild(std::string("IngameChatWidget/") + entity->getId() + "/" + "ResponseList"));
			
		
		//remove all existing response windows
		std::vector<CEGUI::Window*>::const_iterator responses_I = responseTextWidgets.begin();
		std::vector<CEGUI::Window*>::const_iterator responses_I_end = responseTextWidgets.end();
		for (; responses_I != responses_I_end; ++responses_I)
		{
			windowManager->destroyWindow(*responses_I);
		}
		responseTextWidgets.clear();
		

		//for each response, create a button
		std::vector<std::string> responses = entity->getSuggestedResponses();
		std::vector<std::string>::const_iterator I = responses.begin();
		std::vector<std::string>::const_iterator I_end = responses.end();
		int i = 0;
		std::stringstream ss;
		
		for (;I != I_end; ++I)
		{
			std::stringstream ss_;
			ss_ << i;
			CEGUI::StaticText* responseText = static_cast<CEGUI::StaticText*>(windowManager->createWindow((CEGUI::utf8*)"TaharezLook/StaticText", std::string("IngameChatWidget/") + entity->getId() + "/Response/" + ss_.str()));
			
			
			
			responseText->subscribeEvent(CEGUI::ButtonBase::EventMouseButtonUp, 
				boost::bind(&ActiveChatWindow::buttonResponse_Click, this, _1));
			responseText->setText(*I);
			responseText->setSize(CEGUI::Size(1.0f, 0.3f));
			responseText->setPosition(CEGUI::Point(0.0f, i * 0.3f));
			responseText->setInheritsAlpha(true);	
			responseText->setHorizontalFormatting(CEGUI::StaticText::WordWrapLeftAligned);
			responseText->setFrameEnabled(false);
			responseWidget->addChildWindow(responseText);
			responseTextWidgets.push_back(responseText);
			
			++i;
			
			ss << *I << "\n";
		}
		//responseWidget->setText(ss.str());
		
	}

}
