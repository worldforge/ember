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

// Some code originally written for Sear by Simon Goodall, University of Southampton
// Original Copyright (C) 2001 - 2002 


#include "Console.h"
#include "framework/ConsoleObject.h"
#include "services/platform/RectangleRenderer.h"
#include "services/font/FontService.h"
#include "services/font/FontRenderer.h"
#include "services/logging/LoggingService.h"
#include "services/platform/ColorRenderer.h"

namespace dime {

//%TODO Xmp,3: abstract SDL_GetTicks()
Console::Console(const Rectangle& rect) :
  Widget(rect),
  fullHeight(rect.getHeight()),
  consoleHeight(rect.getHeight()),
  animateConsole(0)
{
  // Precache our renderers
  // Make panel slightly transparent
  myRectangleRenderer = new ColorRenderer(rect,Color(0.0f,0.0f,100.0f,8.5f));
  dime::Font *font = dime::FontService::getInstance()->loadFont(FONT_FILE,FONT_HEIGHT);
  assert(font);
  myFontRenderer = new FontRenderer(FontRenderer::BLENDED, Font::FontString(), font, Color(255.0f,255.0f,0,255.0f), rect);

  myBackend = ConsoleBackend::getMainConsole();
}

Console::~Console() {
    LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::VERBOSE) << "Shutting down console."<< ENDM;  
}

int Console::draw(DrawDevice* target){
  // If we are animating and putting console into visible state,
  //  the raise height a tad
  if (animateConsole && myVisible) {
    consoleHeight += CONSOLE_SPEED;

    // Have we reached full height?
    if (consoleHeight >= fullHeight) {
      // Disable animation
      consoleHeight = fullHeight;
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
  return 0;
}

void Console::renderConsoleMessages(DrawDevice *ddevice) {
  const std::list<std::string> msgs = myBackend->getConsoleMessages();
  std::list<std::string>::const_iterator I;
  int i;

  // Render console panel
  int consoleOffset = fullHeight - consoleHeight;
  myRectangleRenderer->render(ddevice);

  // Render console messages
  int font_height = myFontRenderer->getFont()->getHeight();
  for (I = msgs.begin(), i = 0; I != msgs.end(); I++, i++) {
    int j = msgs.size() - i;
    myFontRenderer->setRectangle(Rectangle(myRectangle.getX()+CONSOLE_TEXT_OFFSET_X,
				     myRectangle.getY()+CONSOLE_TEXT_OFFSET_Y + j * font_height - consoleOffset,
				     myRectangle.getWidth(),font_height));
    myFontRenderer->setText(*I);
    myFontRenderer->render(ddevice);
  }

  // Render current command string
  std::string str = CONSOLE_PROMPT_STRING + myCommand + CONSOLE_CURSOR_STRING;
  myFontRenderer->setRectangle(Rectangle(myRectangle.getX()+CONSOLE_TEXT_OFFSET_X,
				   myRectangle.getY()+CONSOLE_TEXT_OFFSET_Y - consoleOffset,myRectangle.getWidth(),font_height));
  myFontRenderer->setText(str);
  myFontRenderer->render(ddevice);
}

bool Console::toggleVisible() {
  // Start the animation	
  animateConsole = 1;
  // Toggle state
  return Widget::toggleVisible();
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
		myBackend->runCommand(myCommand);
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
