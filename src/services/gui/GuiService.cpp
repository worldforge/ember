#include "GuiService.h"
#include <services/logging/LoggingService.h>

void dime::GuiService::refresh()
{
    
    if (myDrawTarget != NULL)
        {
            myRootWidget.draw(myDrawTarget);
            myDrawTarget->update();
        }
	else
        {
            dime::LoggingService::getInstance()->log(__FILE__, __LINE__, dime::LoggingService::FAILURE, "Unable to refresh Gui as no draw target has been specified");
	}

    
}
/*
  void dime::GuiService::MouseMotion(InputDevice *mouse, const SDLKey &key, dime::InputMapping::InputSignalType signaltype)
  {
  std::vector<int> coords(2);
  coords[0] = mouse->getAxisPosition(0);
  coords[1] = mouse->getAxisPosition(1);
  myRootWidget.checkMouseEvent(coords);
  }
  
  void dime::GuiService::MouseClick(InputDevice *mouse, const SDLKey &key, dime::InputMapping::InputSignalType signaltype)
  {
  std::vector<int> coords(2);
  coords[0] = mouse->getAxisPosition(0);
  coords[1] = mouse->getAxisPosition(1);
  dime::LoggingService::getInstance()->slog(__FILE__, __LINE__, dime::LoggingService::INFO) << "Mouse click of button " << key << " at (" << coords[0] << ", " << coords[1] << ") received" << ENDM;
  myRootWidget.checkMouseEvent(coords);
  }
*/

dime::GuiService::GuiService()
{
    setName( "GuiService" );
    setDescription( "Handles the Widgets that make up the GUI" );
    
    myEventGenerator = new dime::EventGenerator(&myRootWidget);
    myRootWidget.setEventGenerator(myEventGenerator);
    
    myDrawTarget = NULL;
    
    dime::LoggingService::getInstance()->log(__FILE__, __LINE__, dime::LoggingService::INFO, "GuiService initialized.");
    myInputService = dime::InputService::getInstance();
    
    
  
}
