#include "GuiService.h"
#include <services/logging/LoggingService.h>

void dime::GuiService::refresh()
{
	if (myDrawTarget != NULL)
	{
		myRootWidget.draw(myDrawTarget);
	}
	else
	{
		dime::LoggingService::getInstance()->log(__FILE__, __LINE__, dime::LoggingService::ERROR, "Unable to refresh Gui as no draw target has been specified");
	}
}

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

dime::GuiService::GuiService()
{
	setName( "GuiService" );
	setDescription( "Handles the Widgets that make up the GUI" );
	
	myDrawTarget = NULL;
	
	dime::LoggingService::getInstance()->log(__FILE__, __LINE__, dime::LoggingService::INFO, "GuiService initialized.");
	
	myRootWidget.setRectangle(dime::Rectangle(0,0,640,480));	//TODO: get real screen dimensions. Maybe do this elsewhere.
	
	myInputService = dime::InputService::getInstance();
	dime::InputDevice *mouse = myInputService->getInputDevice(dime::InputDevice::MOUSE);
	myInputService->addInputMapping(new dime::InputMapping(mouse, SigC::slot(*this,&dime::GuiService::MouseMotion)));
	myInputService->addInputMapping(new dime::InputMapping(mouse, mouse, SDLK_LEFT_MB, SDLK_RIGHT_MB, KMOD_NONE, dime::InputMapping::InputSignalType(InputMapping::KEY_PRESSED | InputMapping::KEY_RELEASED | InputMapping::EVENT_OCCURED), SigC::slot(*this,&dime::GuiService::MouseClick)));
}
