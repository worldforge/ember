#include "GuiService.h"
#include <main/DimeServices.h>

int dime::GuiService::refresh()
{
	return myRootWidget.draw();
}

void dime::GuiService::MouseMotion(InputDevice *mouse, const SDLKey &key, dime::InputMapping::InputSignalType signaltype)
{
	std::vector<int> coords(2);
	cout << "got a mouse event. Mouse at " << coords[0] << "," << coords[1];
	coords[0] = mouse->getAxisPosition(0);
	coords[1] = mouse->getAxisPosition(1);
	myRootWidget.checkMouseEvent(coords);
}

dime::GuiService::GuiService()
{
	setName( "GuiService" );
	setDescription( "Handles the Widgets that make up the GUI" );
	dime::DimeServices::getInstance()->getLoggingService()->log(__FILE__, __LINE__, dime::LoggingService::INFO, "GuiService initialized.");

	myRootWidget.setX(0);
	myRootWidget.setY(0);
	myRootWidget.setDimension(Dimension(1024,768));

	myInputService = dime::DimeServices::getInstance()->getInputService();
	dime::InputDevice *mouse = myInputService->getInputDevice(InputDevice::MOUSE);
	myInputService->addInputMapping(new InputMapping(mouse, slot(*this,&GuiService::MouseMotion)));
}
