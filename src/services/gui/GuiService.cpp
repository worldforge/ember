#include "GuiService.h"
#include <main/DimeServices.h>

int dime::GuiService::refresh()
{
	return myRootWidget.draw();
}

void dime::GuiService::MouseMotion(InputDevice *mouse, const SDLKey &key, dime::InputMapping::InputSignalType signaltype)
{
	int x = mouse->getAxisPosition(0);
	int y = mouse->getAxisPosition(1);
}

dime::GuiService::GuiService()
{
	setName( "GuiService" );
	setDescription( "Handles the Widgets that make up the GUI" );

	myInputService = dime::DimeServices::getInstance()->getInputService();
	dime::InputDevice *mouse = myInputService->getInputDevice(InputDevice::MOUSE);
	myInputService->addInputMapping(new InputMapping(mouse, slot(*this,&GuiService::MouseMotion)));
}