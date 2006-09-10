//
// C++ Implementation: InputCommandMapper
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2006
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

#ifndef CONSOLEWIDGET_H
#define CONSOLEWIDGET_H

#include "framework/ConsoleBackend.h"
#include "framework/ConsoleCommandWrapper.h"
#include <elements/CEGUIMultiLineEditbox.h>
#include <elements/CEGUIEditbox.h>


namespace EmberOgre {
	
class Widget;
	
/**
* The main console. This behaves like most game consoles such as quake etc.
* Switch between opened and closed through the method toggleActive()
*/
class ConsoleWidget : public Widget
{
public:

	ConsoleWidget();
	virtual ~ConsoleWidget();
	void buildWidget();
	virtual void frameStarted(const Ogre::FrameEvent & event);
	
	/**
	*toggles the console on and off
	*/
	void toggleActive();
	
	/**
	* Starts to hide the console.
	*/
	void close();
	
	/**
	* Starts to show the console.
	*/
	void open();

	bool pushMessage(const std::string& message);
	
	/**
	 *    reimplement ConsoleObject::runCommand to catch the "inspect $entityid" command
	 * @param command 
	 * @param args 
	 */
	virtual void runCommand(const std::string &command, const std::string &args);
	
	
	/**
	 *    Puts the focus on the console, allowing for input.
	 */
	void focus();
		
	const Ember::ConsoleCommandWrapper ToggleConsole;
	const Ember::ConsoleCommandWrapper ConsoleFocus;
	
protected:
	CEGUI::MultiLineEditbox* mConsoleTextBox;
	CEGUI::Editbox* mInputBox;
	
        Ember::ConsoleBackend *myBackend;
        enum State { CS_OPEN, CS_CLOSED, CS_OPENING, CS_CLOSING };
        State mState;
	
	
	bool consoleTextBox_Character(const CEGUI::EventArgs& args);
	bool consoleTextBox_KeyUp(const CEGUI::EventArgs& args);
	bool consoleInputBox_TextAcceptedEvent(const CEGUI::EventArgs& args);
	
	//the speed of the window when swithing between open and closed, 0.25 by default
	Ogre::Real mSpeedOfWindow;
	// the text of the current command line saved when brwosing the history
	std::string mCommandLine;
	bool mTabPressed;
	int mSelected;
	
	
	
};

}
#endif // CONSOLEWIDGET_H
