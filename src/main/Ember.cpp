#include "Application.h"
#include <main/DimeServices.h>
#include <services/image/ImageService.h>
#include <services/gui/Button.h>
#include <services/gui/Panel.h>
#include <services/gui/Label.h>
//#include <SDL/SDL_keysym.h>
#include <iostream>

void onMouseButtonDown(dime::MouseButtonEvent *event)
{
}

void onMouseButtonUp(dime::MouseButtonEvent *event)
{
}


int main(int argc, char **argv)
{
    dime::Application myApp;
	
	dime::ImageService::getInstance()->addPath("./bin/");
	dime::ImageService::getInstance()->addPath("../../bin/");

	dime::Label myTestLabel("Dime test!",dime::Rectangle(10,10,200,30));

	dime::Button myTestButton(dime::Rectangle(300,300,100,75));
	myTestButton.setBackground(dime::RectangleRenderer(BITMAP,myTestButton.getRectangle(),"masonwindowback.jpg"));
	myTestButton.setHighlightBackground(dime::RectangleRenderer(BITMAP,myTestButton.getRectangle(),"masonwindowback2.jpg"));

	dime::Panel myTestPanel(dime::Rectangle(200,200,300,200));
	myTestPanel.setBackground(dime::RectangleRenderer(BITMAP,myTestPanel.getRectangle(),"masonwindowback2.jpg"));
	myTestPanel.addWidget(&myTestButton);
    myTestButton.onMouseUp.connect(SigC::slot(onMouseButtonUp));
    myTestButton.onMouseDown.connect(SigC::slot(onMouseButtonDown));
    

	dime::InputService * pIS = dime::InputService::getInstance();

	pIS->addInputMapping(new dime::InputMapping(pIS->getInputDevice(dime::InputDevice::KEYBOARD), SDLK_ESCAPE, false,
		SigC::slot(myApp, &dime::Application::escPressed)));

	dime::DimeServices::getInstance()->getGuiService()->getRootWidget().addWidget(&myTestPanel);
	
    myApp.mainLoop();
    return 0;
}
