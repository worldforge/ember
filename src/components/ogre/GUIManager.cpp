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
#include "CEGUI/CEGUIBase.h"
#include "CEGUI/CEGUIExceptions.h"
#include "CEGUI/CEGUISystem.h"
#include "CEGUI/CEGUISchemeManager.h"
#include "CEGUI/CEGUIWindow.h"
#include "CEGUI/CEGUIWindowManager.h"
#include "CEGUI/CEGUIImageset.h"
#include "CEGUI/elements/CEGUIStaticImage.h"
#include "CEGUI/elements/CEGUIPushButton.h"
#include "CEGUI/renderers/OgreGUIRenderer/ogrerenderer.h"
#include <Ogre.h>

namespace DimeOgre {


GUIManager::GUIManager(Ogre::RenderWindow* window, Ogre::SceneManager* sceneMgr) 
{

	try {
	
		fprintf(stderr, "CEGUI - ENTERING CTOR\n");
		
		CEGUI::OgreRenderer* GUIRenderer = new CEGUI::OgreRenderer(window, Ogre::RENDER_QUEUE_OVERLAY, false, 3000, sceneMgr);
		fprintf(stderr, "CEGUI - RENDERER CREATED\n");
		
		//GUIRenderer->setTargetSceneManager(sceneMgr);
		fprintf(stderr, "CEGUI - AND SCENE MANAGER SET\n");
		
		new CEGUI::System(GUIRenderer); 
		fprintf(stderr, "CEGUI - SYSTEM CREATED\n");

		CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"testscheme.xml");
		fprintf(stderr, "CEGUI - TEST SCHEME LOADED\n");
		
		CEGUI::System::getSingleton().setDefaultMouseCursor((CEGUI::utf8*)"TaharezImagery", (CEGUI::utf8*)"MouseArrow");
		CEGUI::System::getSingleton().setDefaultFont((CEGUI::utf8*)"Tahoma-14"); 
		fprintf(stderr, "CEGUI - DEFAULTS SET\n");
		
		CEGUI::Window* sheet = CEGUI::WindowManager::getSingleton().createWindow((CEGUI::utf8*)"DefaultGUISheet", (CEGUI::utf8*)"root_wnd");
		CEGUI::System::getSingleton().setGUISheet(sheet); 
		fprintf(stderr, "CEGUI - SHEET CREATED\n");
		
		
		CEGUI::Window* fwnd = CEGUI::WindowManager::getSingleton().createWindow((CEGUI::utf8*)"Taharez Frame Window", (CEGUI::utf8*)"DemoWindow");
		fwnd->setPosition(CEGUI::Point(0.01f, 0.01f));
		fwnd->setSize(CEGUI::Size(0.43f, 0.1f));
		fwnd->setText((CEGUI::utf8*)"Worldforge - proud CEGUI user");
		sheet->addChildWindow(fwnd); 
		fprintf(stderr, "CEGUI - SAMPLE WINDOW CREATED\n");
		
		CEGUI::Window* button = CEGUI::WindowManager::getSingleton().createWindow((CEGUI::utf8*)"Taharez Button", (CEGUI::utf8*)"SomeButton");
		button->setPosition(CEGUI::Point(0.08f, 0.05f));
		button->setSize(CEGUI::Size(0.1f, 0.1f));
		//fwnd->setText((CEGUI::utf8*)"Worldforge - proud CEGUI user");
		sheet->addChildWindow(button); 
		fprintf(stderr, "CEGUI - SAMPLE BUTTON CREATED\n");
	
		mChat = CEGUI::WindowManager::getSingleton().createWindow((CEGUI::utf8*)"Taharez MultiLineEditbox", (CEGUI::utf8*)"chat");
		mChat->setPosition(CEGUI::Point(0.01f, 0.5f));
		mChat->setSize(CEGUI::Size(0.4f, 0.4f));
		mChat->setAlpha(0.5f);
		mChat->setFont((CEGUI::utf8*)"Tahoma-8");
		mChat->setText((CEGUI::utf8*)"IG Chat>>\n");
		sheet->addChildWindow(mChat);
		
		/*
		CEGUI::PushButton* btn = (CEGUI::PushButton*)CEGUI::WindowManager::getSingleton().createWindow((CEGUI::utf8*)"Taharez Button", (CEGUI::utf8*)"Demo7/Window1/Quit");
		fwnd->addChildWindow(btn);
		btn->setMaximumSize(CEGUI::Size(1.0f, 1.0f));
		btn->setPosition(CEGUI::Point(0.02f, 0.1f));
		btn->setSize(CEGUI::Size(0.25f, 0.1f));
		btn->setText((CEGUI::utf8*)"Exit Demo");
		fprintf(stderr, "CEGUI - SAMPLE BUTTON CREATED\n");
		*/
		//
                // CEGUI logo
                //
		/*
                CEGUI::StaticImage* clogo = (CEGUI::StaticImage*)CEGUI::WindowManager::getSingleton().createWindow((CEGUI::utf8*)"Taharez StaticImage", (CEGUI::utf8*)"Cegui Logo");
                sheet->addChildWindow(clogo);
                clogo->setFrameEnabled(false);
                clogo->setBackgroundEnabled(false);
		CEGUI::Imageset* giset = CEGUI::ImagesetManager::getSingleton().createImageset((CEGUI::utf8*)"ogregui.xml");
                clogo->setImage(&giset->getImage((CEGUI::utf8*)"CeguiLogo"));
                clogo->setPosition(CEGUI::Point(0.9125f, 0.9115f));
                clogo->setSize(CEGUI::Size(0.075, 0.0385f));
                clogo->setAlwaysOnTop(true);
                clogo->setAlpha(0.9f);
                clogo->setEnabled(false);
                clogo->setInheritsAlpha(false);
		
		sheet->show();
		sheet->activate();
		fwnd->show();
		fwnd->activate();
		btn->show();
		fwnd->activate();
		//CEGUI::System::getSingleton().renderGUI();
		fprintf(stderr, "CEGUI - GUI RENDERED\n");
		*/
		
	} catch(CEGUI::Exception e) {
		fprintf(stderr, "Exception creating CEGUI - can't show message because it's protected. WTF?\n");
	}
	
}
GUIManager::~GUIManager()
{}

void GUIManager::appendOOGChatLine(std::string line) 
{
	CEGUI::String current = mChat->getText();
	current.append(line);
	current.append((CEGUI::utf8*)"\n");
	mChat->setText(current);
	fprintf(stderr, "CEGUI - SHOULD APPEND LINE HERE");
}

/*
MotionManager* MotionManager::_instance = 0;

MotionManager & MotionManager::getSingleton(void)
{
	if(_instance == 0)
		_instance = new MotionManager;
	return *_instance;
}
*/
template<> GUIManager* dime::Singleton<GUIManager>::ms_Singleton = 0;

}
