/*
    Copyright (C) 2001,2002  Martin Pollard (Xmp), Lakin Weckerd (nikal)

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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <Eris/Types.h>

#include "test/TestServices.h"
#include "services/DimeServices.h"
#include "services/image/ImageService.h"
#include "framework/Exception.h"
#include "services/gui/widget/Button.h"
#include "services/logging/LoggingService.h"
#include "services/config/ConfigService.h"
#include "Application.h"

void quitButton(dime::Button* button)
{
  dime::Application::getInstance()->quit();    
}


int main(int argc, char **argv)
{
  bool success = runTests();

  if(!success)
    {
      //return 1;
    }
    
  try
    {
      dime::ImageService::getInstance()->addPath("./bin/");
      dime::ImageService::getInstance()->addPath("../../bin/");

      // Bind Escape to quit
      dime::InputService* pIS = dime::InputService::getInstance();
      pIS->addInputMapping( new dime::InputMapping( pIS->getInputDevice(dime::InputDevice::KEYBOARD),
						    SDLK_ESCAPE, false,
						    SigC::slot(*dime::Application::getInstance(),
							       &dime::Application::escPressed)));

      dime::Application::getInstance()->getStateMgr()->setState("initial state");
      dime::Application::getInstance()->mainLoop();
    }
  catch ( dime::Exception e )
    {
      std::cerr << "Uncaught Dime exception: "<< e.getError() << std::endl;
    }
  catch ( Eris::BaseException e )
    {
      std::cerr << "Uncaught Eris exception: "<< e._msg << std::endl;
    }
  catch ( char* e )
    {
      std::cerr << "Uncaught exception: "<< e << std::endl;
    }
  catch ( ... )
    {
      std::cerr << "Unknown exception dying" << std::endl;
    }
    
  return 0;
}
