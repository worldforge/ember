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
#include "framework/ConsoleBackend.h"
#include "services/gui/widget/Widget.h"
#include "services/platform/Renderer.h"
#include "services/font/FontRenderer.h"

// Included custom library headers

// Included system headers
#include <string>


namespace dime {

// Forward Declarations
class ConsoleObject;

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
class Console : public Widget {
  //======================================================================
  // Private Constants
  //======================================================================
 private:
  static const int FONT_HEIGHT = 12;
  static const int LINE_SPACING = 5;
  static const int CONSOLE_TEXT_OFFSET_X = 5;
  static const int CONSOLE_TEXT_OFFSET_Y = 5;

  static const int CONSOLE_SPEED = 10;

  static const char* const CONSOLE_PROMPT_STRING = "> ";
  static const char* const CONSOLE_CURSOR_STRING = "_";

#ifdef _MSC_VER
  static const char* const FONT_FILE = "..\\bin\\wf_opal.ttf";
#else
  static const char* const FONT_FILE = "../../bin/wf_opal.ttf";
#endif 

  //======================================================================
  // Public Methods
  //======================================================================
 public:
  Console(const Rectangle& rect);
  ~Console();
   
  /**
    * Draws the widget, and/or its children.
    */
  virtual int draw(DrawDevice *target);

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

protected:
  /**
   * ConsoleBackend that we are displaying
   */
  ConsoleBackend* myBackend;

  /**
   * Height of console when full size
   */
  int fullHeight;

  /**
   * The height of the console. determined by number of messages allowed
   */
  int consoleHeight;

  /**
   * Flag determining whether console is moving
   */
  bool animateConsole;

  /**
   * Cached FontRenderer
   */
  FontRenderer* myFontRenderer;

  /**
   * Cached Renderer
   */
  Renderer* myRenderer;

  /**
   * Current command string
   */
  std::string myCommand;

  /**
   *  This render the console messges plus the current command string
   *  command is the current command string
   */ 
  void renderConsoleMessages(DrawDevice* ddevice);
};

} /* namespace dime */
#endif /* CONSOLE_H */
