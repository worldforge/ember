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


#include "Console.h"
#include "framework/ConsoleObject.h"
#include "services/platform/RectangleRenderer.h"
#include "services/font/FontService.h"
#include "services/font/FontRenderer.h"
#include "services/logging/LoggingService.h"

#ifdef _MSC_VER
#define FONT_FILE "..\\bin\\nasal.ttf"
#else
#define FONT_FILE "../../bin/nasal.ttf"
#endif 

namespace dime {
const char* Console::CONSOLE_PROMPT_STRING = "> ";
const char* Console::CONSOLE_CURSOR_STRING = "_";


// TODO: abstract SDL_GetTicks()
Console::Console(const Rectangle& rect) :
  Widget(rect),
  animateConsole(0),
  consoleHeight(CONSOLE_HEIGHT),
  console_messages(std::list<std::string>()),
  screen_messages(std::list<screenMessage>())
{
  // Register console commands
  registerCommand(TOGGLE_CONSOLE, this);
  registerCommand(LIST_CONSOLE_COMMANDS, this);
}

Console::~Console() {
    LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Shutting down console."<< ENDM;  
}

void Console::pushMessage(const std::string &message, int type, int duration) {
  // Is this a screen message
  if (type & SCREEN_MESSAGE) {	
    //If we have reached our message limit, remove the oldest message regardless of duration
    if (screen_messages.size() >= MAX_MESSAGES) screen_messages.erase(screen_messages.begin());
    screen_messages.push_back(screenMessage(message, SDL_GetTicks() + duration));
  }
  // Is this a console message?
  if (type & CONSOLE_MESSAGE) {
    //If we have reached our message limit, remove the oldest message regardless of duration
    if (console_messages.size() >= MAX_MESSAGES) console_messages.erase(console_messages.begin());
    console_messages.push_back(message);
  }
}

int Console::draw(DrawDevice* target){
  // If we are animating and putting console into visible state,
  //  the raise height a tad
  if (animateConsole && myVisible) {
    consoleHeight += CONSOLE_SPEED;
    // Have we reached full height?
    if (consoleHeight >= CONSOLE_HEIGHT) {
      // Disable animation
      consoleHeight = CONSOLE_HEIGHT;
      animateConsole = 0;
    }
    renderConsoleMessages(target);
  // If we are animating and putting console into hidden state,
  //  the lower height a tad
  } else if (animateConsole && !myVisible) {
    consoleHeight -= CONSOLE_SPEED;
    // Have we become visible?
    if (consoleHeight <= 0) {
      //Disable animation
      consoleHeight = 0;
      animateConsole = 0;
    }
    renderConsoleMessages(target);
  // Else are we just plain visible?    
  } else if (myVisible) {
    renderConsoleMessages(target);
  }
  //Screen messages are always visible
  renderScreenMessages(target);
  return 0;
}

// TODO: refactor this to do the majority of the renderer creation in 
//       the constructor
void Console::renderConsoleMessages(DrawDevice *ddevice) {
  std::list<std::string>::const_iterator I;
  int i;

  // Render console panel
  int consoleOffset = CONSOLE_HEIGHT - consoleHeight;
  // Make panel slightly transparent
  RectangleRenderer rrenderer(myRectangle,Color(0.0f,0.0f,100.0f,8.5f));
  rrenderer.render(ddevice);

  dime::Font *font = dime::FontService::getInstance()->loadFont(FONT_FILE,16);
  assert(font);
  FontRenderer frenderer(FontRenderer::BLENDED, Font::FontString(), font, Color(255.0f,255.0f,0,255.0f), myRectangle);

  // Render console messages
  int font_height = font->getHeight();
  for (I = console_messages.begin(), i = 0; I != console_messages.end(); I++, i++) {
    int j = console_messages.size() - i;
    frenderer.setRectangle(Rectangle(myRectangle.getX()+CONSOLE_TEXT_OFFSET_X,
				     myRectangle.getY()+CONSOLE_TEXT_OFFSET_Y + j * font_height - consoleOffset,
				     myRectangle.getWidth(),font_height));
    frenderer.setText(*I);
    frenderer.render(ddevice);
  }

  // Render current command string
  std::string str = CONSOLE_PROMPT_STRING + myCommand + CONSOLE_CURSOR_STRING;
  frenderer.setRectangle(Rectangle(myRectangle.getX()+CONSOLE_TEXT_OFFSET_X,
				   myRectangle.getY()+CONSOLE_TEXT_OFFSET_Y - consoleOffset,myRectangle.getWidth(),font_height));
  frenderer.setText(str);
  frenderer.render(ddevice);
}

void Console::renderScreenMessages(DrawDevice *ddevice) {/*
  if (screen_messages.empty()) return;	
  std::list<screenMessage>::const_iterator I;
  int i;
  _renderer->stateChange("font");
  _renderer->setColour(1.0f, 1.0f, 0.0f, 1.0f);
  // Get screen height so we can calculate offset correctly
  int height = _renderer->getWindowHeight();
  //Get time so we can remove expired messages
  unsigned int current_time = SDL_GetTicks(); 
  //Render messges
  for (I = screen_messages.begin(), i = 0; I != screen_messages.end(); I++, i++) {
    const std::string str = (const std::string)((*I).first);
    _renderer->print(CONSOLE_TEXT_OFFSET_X, height - ((i + 1) * FONT_HEIGHT ), const_cast<char*>(str.c_str()), 0);
  }
  //Remove expired messages
  //TODO this currently only removes the messages from  the top of the list
  //     should a messages at the top have a longer expiry time than one
  //     later in the list, the later one will stay in the list until the 
  //     higher entries have been removed
  bool loop = true;
  while (loop) {
  if (screen_messages.empty()) break;	
  loop = false; // break unless it gets set to true again
  screenMessage sm = *screen_messages.begin(); // Get first messge
  unsigned int message_time = sm.second;
  if (current_time > message_time) { // Check expiry time
  screen_messages.erase(screen_messages.begin());
  loop = true; // Go again
  }
  }*/
}


bool Console::toggleVisible() {
  // Start the animation	
  animateConsole = 1;
  // Toggle state
  return Widget::toggleVisible();
}

void Console::registerCommand(const std::string &command, ConsoleObject *object) {
    LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Registering: " << command << ENDM;
  // Assign the ConsoleObject to the command
  _registered_commands[command] = object;
}

void Console::runCommand(const std::string &command) {
  if (command.empty()) return; // Ignore empty string
  // Grab first character of command string
  char c = command.c_str()[0];
  // Check to see if command is a command, or a speech string
  if ((c != '/' && c != '+' && c != '-')) {
    // Its a speech string, so SAY it
    // FIXME /say is not always available!
    runCommand(std::string("/say ") + command);
    return; 
  }
  // If command has a leading /, remove it
  std::string command_string = (c == '/')? command.substr(1) : command;
  // Split string into command / arguments pair
  Tokeniser tokeniser = Tokeniser();
  tokeniser.initTokens(command_string);
  std::string cmd = tokeniser.nextToken();
  std::string args = tokeniser.remainingTokens();
  //Grab object registered to the command
  ConsoleObject* con_obj = _registered_commands[cmd];
  // Print all commands apart form toggle console to the console
  if (cmd != TOGGLE_CONSOLE) pushMessage(command_string, CONSOLE_MESSAGE, 0);
  // If object exists, run the command
  if (con_obj) con_obj->runCommand(cmd, args);
  else { // Else print error message
    LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Unknown command:"<<command<< ENDM;
    pushMessage("Unknown command" , CONSOLE_MESSAGE, 0);
  }
}

void Console::runCommand(const std::string &command, const std::string &args) {
  // This command toggles the console
  if (command == TOGGLE_CONSOLE) {
    toggleVisible();
  }
  // This commands prints all currently registers commands to the Log File
  else if (command == LIST_CONSOLE_COMMANDS) {
    for (std::map<std::string, ConsoleObject*>::const_iterator I = _registered_commands.begin(); I != _registered_commands.end(); I++) {
      // TODO - should we check to see if I->second is valid?
      LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << I->first<< ENDM;
    }
  }
}

bool Console::keyPress( KeyPressEvent *event )
{
    if (event->getState() == KeyPressEvent::PRESSED)
        {
            switch (event->getKey().getKey())
	      {
	      case SDLK_BACKSPACE:
		if (myCommand.length()!=0)
		  {
		    myCommand.erase(myCommand.length()-1,1);
		  }
		break;
	      case SDLK_RETURN:
	      {
		runCommand(myCommand);
		myCommand="";
	      }
	      break;
	      case SDLK_TAB:
	      {
		// TODO: Pass focus to next control
	      }
	      break;
	      default:
		if (event->getKey().getUnicode() < 0x80 && event->getKey().getUnicode() > 0) {
		  myCommand+=(char)event->getKey().getUnicode();
		}
	      }

	  return true;
	}
  return false;
}

} /* namespace dime */
