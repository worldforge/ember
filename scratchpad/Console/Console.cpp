// This file may be redistributed and modified only under the terms of
// the GNU General Public License (See COPYING for details).

// Rewritten for Dime by Martin Pollard (Xmp)

// Originally written for Sear by Simon Goodall, University of Southampton
// Original Copyright (C) 2001 - 2002 

//#include "common/Utility.h" // Check for functions we need 
#include "services/logging/LoggingService.h"
//#include "common/Log.h" // Quite similar to ours will redo the logging
// statements line for line

#include "Console.h"
#include "ConsoleObject.h"
//#include "System.h" // What to replace with?
//#include "Graphics.h" // Ditto
//#include "Render.h" // Ditto

namespace dime {

Console::Console(System *system) :
  animateConsole(0),
  showConsole(0), // To be replaced by widget's in built show and hide feature (TO GO)
  consoleHeight(0),
  console_messages(std::list<std::string>()), // (TO STAY)
  screen_messages(std::list<screenMessage>()), // (TO STAY)
  _system(system),// Used to get access to getTime() and the renderer (TO GO)
  _renderer(NULL)
{ }

Console::~Console() {}

bool Console::init() {
  // Grab the render object. This is not available when console is created
  // and cannot be passed into the constructor.
  _renderer = _system->getGraphics()->getRender();
  // Preload texture
  panel_id = _renderer->requestTexture("ui", "panel");
  // Register console commands
  registerCommand(TOGGLE_CONSOLE, this);
  registerCommand(LIST_CONSOLE_COMMANDS, this);
  return true;
}

void Console::shutdown() {
  Log::writeLog("Shutting down console.", Log::LOG_DEFAULT);
  
}

void Console::pushMessage(const std::string &message, int type, int duration) {
  // Is this a screen message
  if (type & SCREEN_MESSAGE) {	
    //If we have reached our message limit, remove the oldest message regardless of duration
    if (screen_messages.size() >= MAX_MESSAGES) screen_messages.erase(screen_messages.begin());
    screen_messages.push_back(screenMessage(message, _system->getTime() + duration));
  }
  // Is this a console message?
  if (type & CONSOLE_MESSAGE) {
    //If we have reached our message limit, remove the oldest message regardless of duration
    if (console_messages.size() >= MAX_MESSAGES) console_messages.erase(console_messages.begin());
    console_messages.push_back(message);
  }
}

void Console::draw(const std::string &command) {
  // If we are animating and putting console into visible state,
  //  the raise height a tad
  if (animateConsole && showConsole) {
    consoleHeight += CONSOLE_SPEED;
    // Have we reached full height?
    if (consoleHeight >= CONSOLE_HEIGHT) {
      // Disable animation
      consoleHeight = CONSOLE_HEIGHT;
      animateConsole = 0;
    }
    renderConsoleMessages(command);
  // If we are animating and putting console into hidden state,
  //  the lower height a tad
  } else if (animateConsole && !showConsole) {
    consoleHeight -= CONSOLE_SPEED;
    // Have we become visible?
    if (consoleHeight <= 0) {
      //Disable animation
      consoleHeight = 0;
      animateConsole = 0;
    }
    renderConsoleMessages(command);
  // Else are we just plain visible?    
  } else if (showConsole) {
    renderConsoleMessages(command);
  }
  //Screen messages are always visible
  renderScreenMessages();
}

void Console::renderConsoleMessages(const std::string &command) {
  if (!_renderer) {
    Log::writeLog("Console: Error - Renderer object not created", Log::LOG_ERROR);
    return;
  }
  std::list<std::string>::const_iterator I;
  int i;
  //Render console panel
  int consoleOffset = CONSOLE_HEIGHT - consoleHeight;
  _renderer->stateChange("panel");
  //Make panel slightly transparent
  _renderer->setColour(0.0f, 0.0f, 1.0f, 0.85f);
  _renderer->drawTextRect(0, 0, _renderer->getWindowWidth(), consoleHeight, panel_id);
  _renderer->stateChange("font");
  _renderer->setColour(1.0f, 1.0f, 0.0f, 1.0f);
  //Render console messges
  for (I = console_messages.begin(), i = 0; I != console_messages.end(); I++, i++) {
    int j = console_messages.size() - i;
    _renderer->print(CONSOLE_TEXT_OFFSET_X, CONSOLE_TEXT_OFFSET_Y + j * FONT_HEIGHT - consoleOffset, (char*)(*I).c_str(), 0);
  }
  //Render current command string
  std::string str = CONSOLE_PROMPT_STRING + command + CONSOLE_CURSOR_STRING;
  _renderer->print(CONSOLE_TEXT_OFFSET_X, CONSOLE_TEXT_OFFSET_Y - consoleOffset, (char *)str.c_str(), 0);
}

void Console::renderScreenMessages() {
  if (screen_messages.empty()) return;	
  std::list<screenMessage>::const_iterator I;
  int i;
  _renderer->stateChange("font");
  _renderer->setColour(1.0f, 1.0f, 0.0f, 1.0f);
  // Get screen height so we can calculate offset correctly
  int height = _renderer->getWindowHeight();
  //Get time so we can remove expired messages
  unsigned int current_time = _system->getTime();
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
  }
}

void Console::toggleConsole() {
  // Start the animation	
  animateConsole = 1;
  // Toggle state
  showConsole = !showConsole;
}

void Console::registerCommand(const std::string &command, ConsoleObject *object) {
  Log::writeLog(std::string("registering: ") + command, Log::LOG_INFO);
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
    Log::writeLog(std::string("Unknown command: ") + command, Log::LOG_ERROR);
    pushMessage("Unknown command" , CONSOLE_MESSAGE, 0);
  }
}

void Console::runCommand(const std::string &command, const std::string &args) {
  // This command toggles the console
  if (command == TOGGLE_CONSOLE) {
    toggleConsole();
  }
  // This commands prints all currently registers commands to the Log File
  else if (command == LIST_CONSOLE_COMMANDS) {
    for (std::map<std::string, ConsoleObject*>::const_iterator I = _registered_commands.begin(); I != _registered_commands.end(); I++) {
      // TODO - should we check to see if I->second is valid?
      Log::writeLog(I->first, Log::LOG_INFO);
    }
  }
}

} /* namespace dime */
