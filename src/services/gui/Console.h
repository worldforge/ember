/*
    Copyright (C) 2002  Martin Pollard (Xmp), Simon Goodall

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

// Rewritten for Dime by Martin Pollard (Xmp)

// Originally written for Sear by Simon Goodall, University of Southampton
// Original Copyright (C) 2001 - 2002 

#ifndef CONSOLE_H
#define CONSOLE_H

// Included headers from the current project
#include "framework/ConsoleObject.h"
#include "services/gui/Widget.h"

// Included custom library headers

// Included system headers
#include <string>
#include <list>
#include <map>



namespace dime {
const int MAX_MESSAGES = 5;
const int FONT_HEIGHT = 15;
const int LINE_SPACING = 5;
const int CONSOLE_TEXT_OFFSET_X = 5;
const int CONSOLE_TEXT_OFFSET_Y = 5;

const int CONSOLE_SPEED = 10;
const int CONSOLE_HEIGHT = 100;

const char* CONSOLE_PROMPT_STRING = "> ";
const char* CONSOLE_CURSOR_STRING = "_";

#define CONSOLE_MESSAGE 0x1
#define SCREEN_MESSAGE  0x2

// Forward Declarations
class ConsoleObject;
class System;
class Render;

/**
 * The console widget
 *
 * The console class handles the onscreen console, the screen messages and keeps track of all the console commands in use.
 *
 * Example code?
 *
 * @author Xmp (Martin Pollard)
 * @author Simon Goodall
 *
 * @see Widget
 *
 */
class Console : public ConsoleObject, public Widget {
public:
  Console(const Rectangle& rect);
  ~Console();
  
  bool init();
  void shutdown();
 
  /**
   * Add a message to the console and/or screen message queue
   * message is the message string
   * type is where to display the message: CONSOLE_MESSAGE and/or SCREEN_MESSAGE
   * duration is how long the SCREEN_MESSAGE lasts before being removed
   */ 
  void pushMessage(const std::string &message, int type, int duration);
  /**
    * Draws the widget, and/or its children.
    */
  virtual int draw(DrawDevice *target);
  /**
   * This method renders the console.
   * command is the current command string
   */ 
  void draw(const std::string &command);
  /**
   * Toggles whether the console is visible or not
   */ 
  virtual bool toggleVisible();

  /**
    * A key was pressed.
    * key is the value returned by allegros readkey -function.
    * (it contains both scancode and ascii info.)
    * Returns true if the keypress was processed.
    * Override the event methods in derived classes when neccesary.
    */

  virtual bool keyPress( KeyPressEvent *event);

  /**
   * Registers a command with the console
   * command is the command to register
   * object is the originating object
   */ 
  void registerCommand(const std::string &command, ConsoleObject *object);
  /**
   * This is the method the determines what object the pass the command onto
   * command is the command string to process
   */ 
  void runCommand(const std::string &command);

  /**
   * This is the ConsoleObject method.
   * command is the command to run
   * args is the commands arguments
   */ 
  void runCommand(const std::string &command, const std::string &args);

protected:
  // Pair used to assign a duration to the screen message
  typedef std::pair<std::string, unsigned int> screenMessage;

  /**
   *  This render the console messges plus the current command string
   *  command is the current command string
   */ 
  void renderConsoleMessages(const std::string &command);
  /**
   * This renders the screen messages
   */ 
  void renderScreenMessages();

  bool animateConsole; // Flag determining whether console is moving
  int consoleHeight; // the height of the console. determined by number of messages allowed
  std::list<std::string> console_messages; // Current console messages
  std::list<screenMessage> screen_messages; // Current screen messages

  // Mapping of registered commands to assoicated object
  std::map<std::string, ConsoleObject*> _registered_commands;
 
  // List of Console's console commands
  static const char * const TOGGLE_CONSOLE = "toggle_console";
  static const char * const LIST_CONSOLE_COMMANDS = "list_commands";
  
};

} /* namespace dime */
#endif /* CONSOLE_H */
