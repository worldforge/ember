#include "GuiService.h"
#include <main/DimeServices.h>

int dime::GuiService::refresh()
{
	return myRootWidget.draw();
}

dime::GuiService::GuiService()
{
	setName( "GuiService" );
	setDescription( "Handles the Widgets that make up the GUI" );

	myInputService = dime::DimeServices::getInstance()->getInputService();
	myMouse = myInputService->getInputDevice(InputDevice::MOUSE);
	myInputService->addInputMapping(new InputMapping(myMouse, slot(*this,&GuiService::MouseMotion)));
}