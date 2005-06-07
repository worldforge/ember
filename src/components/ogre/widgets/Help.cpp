//
// C++ Implementation: Help
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
#include "Help.h"
#include "Widget.h"
#include "../GUIManager.h"
#include <CEGUIImagesetManager.h> 
#include <CEGUIImageset.h> 
#include "framework/ConsoleBackend.h"
#include <elements/CEGUIFrameWindow.h>

namespace EmberOgre {

const char * const Help::HELP= "help";

Help::Help()
 : Widget()
{
	Ember::ConsoleBackend::getMainConsole()->registerCommand(HELP,this);

}


Help::~Help()
{
}


void Help::buildWidget()
{

	loadMainSheet("HelpWidget.xml", "Help/");
	//mMainWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)"cegui/widgets/HelpWidget.xml", "Help/");
	//mMainWindow->setVisible(false);
	 
	
 	CEGUI::Window* textbox  = CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Help/TextBox");
 	CEGUI::String text = "Welcome to ember.\nThis help window can be accessed anytime either by clicking on the help icon down to the left, or by entering the command \"/help\" in the console (F12 will expand the console).\n\n---Getting started---\nThe first thing you would want to do is to connect to a server. Use the server browser window for this. It should have connected to the metaserver and presented a list with available servers. You can also enter your own server address in the textbox in the lower left corner.\nOnce connected, you must either create an account or log into an already existing. This is pretty straightforward. Once you've logged in, you must either choose an existing character, or create a new one. Also pretty self explanatory.\n\n---Moving in the world---\nIf you press and hold the right mouse button you enter \"movement mode\". While in \"movement mode\", mouse movement will pan the camera. Use the wasd keys to move the avatar. Pressing shift will make the avatar run, though the server might not always keep up with this.\n\n---Interacting with the world---\nYour main way of interacting with the world is by clicking with the left mouse button on world entities. This will bring up an action menu. Choose from the menu to perform the desired action.\n * Touch : this will make the avatar touch the entity, provoking different responses depending on the type of entity touched. A NPC will speak with you, while a pig might decide to try to run away from you. Try touching different entities in the world to find out their different responses.\n * Take : moves the entity to your inventory, it the server allows it.\n * Give : displays the Give dialog. Beware! Currently the server allows you to give almost anything to any entity, sometimes without the possibility to get it back.\n * Inspect : shows debug info about the entity.\n * Use : Tries to use the currently wielded entity on the selected entity. Try to use an axe with a tree, or a cleaver with a pig.\n\n---Inventory---\nThe inventory can be brought up by the command \"/show_inventory\". You can drop or wield items in your inventory. Wielding means that that entity will be used when performing the \"Use\" action on another entity.\n\n--- Console ---\nAt the top of the screen is the console. It can be expanded and shrunk by pressing F12. The console serves two purposes: it allows you to say things in the world, and it allows you to enter commands to the client. All commands are prefixed by \"/\", for example \"/hide_chat\" and \"/show_chat\". Enter \"/list_commands\" for a list of all commands.\n\n--- Commands and keys ---\nHere's a list of the most common keys and some useful commands:\n\n When in movement mode:\n  w : forward\n  s : back\n  a : left\n  d : right\n  shift : run\n\n General keys\n  F12 : toggle console\n  F8 : take screenshot and save to ~./ember/screenshots\n  F7 : toggle wireframe mode\n\n Commands:\n  /quit : quits\n  /connect <server> : connects to a server\n  /login <name> <password> : tries to log in\n  /add <name> <gender> <type> : adds a new character, for example: /add Erik male settler\n\n\n\nFor more questions or info, see www.worldforge.org";
	textbox->setText(text);

	
	Ogre::TexturePtr texPtr = Ogre::TextureManager::getSingleton().load("gui/icon_help.png", "General");
	
	//create a CEGUI texture from our Ogre texture
	CEGUI::Texture* ceguiTexture = mGuiManager->getGuiRenderer()->createTexture(texPtr);
	
	//we need a imageset in order to create GUI elements from the ceguiTexture
	CEGUI::Imageset* imageSet = CEGUI::ImagesetManager::getSingleton().createImageset("Help", ceguiTexture);
	
	//we only want one element: the whole texture
	imageSet->defineImage("fullicon", CEGUI::Rect(0,0,42,41), CEGUI::Point(0,0));
	
	
	mHelpIcon = static_cast<CEGUI::StaticImage*>(mWindowManager->createWindow((CEGUI::utf8*)"TaharezLook/StaticImage", (CEGUI::utf8*)"Help/Icon"));
	mHelpIcon->setPosition(CEGUI::Absolute, CEGUI::Point(10, 700));
	mHelpIcon->setSize(CEGUI::Absolute, CEGUI::Size(42, 41));
	//mMainWindow->setAlpha(0.5f);
	mHelpIcon->setVisible(true);
	mHelpIcon->setEnabled(true);
	mHelpIcon->setFrameEnabled(false);
	//mMainWindow->setAlwaysOnTop(true);
	mHelpIcon->setImage(&imageSet->getImage("fullicon"));
	//mHelpIcon->setBackgroundEnabled(false);
	mHelpIcon->setBackgroundColours(CEGUI::colour(1,1,1,0));
	
	BIND_CEGUI_EVENT(mHelpIcon, CEGUI::Window::EventMouseClick, Help::Icon_Click)
		
	getMainSheet()->addChildWindow(mHelpIcon); 
//	getMainSheet()->addChildWindow(mMainWindow); 
	enableCloseButton();


}



bool Help::Icon_Click(const CEGUI::EventArgs& args)
{
	show();
}

void Help::show()
{
	if (mMainWindow) {
		mMainWindow->setVisible(true);
		mMainWindow->moveToFront();
	}
}

void Help::hide()
{
	if (mMainWindow) {
		mMainWindow->setVisible(false);
	}
}


void Help::runCommand(const std::string &command, const std::string &args)
{
	if(command == HELP)
	{
		show();
	} else {
		Widget::runCommand(command, args);
	}

}


};
