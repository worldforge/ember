#include "Application.h"
#include <main/DimeServices.h>
#include <services/gui/Button.h>
//#include <SDL/SDL_keysym.h>


void mouseenterbutton(dime::Button* button)
{
	button->highlight();
}

void mouseexitbutton(dime::Button* button)
{
	button->lowlight();
}

int main(int argc, char **argv)
{
    dime::Application myApp;
	
	dime::Button myButton;
	myButton.setRectangle(dime::Rectangle(300,200,150,120));
	myButton.setBackground(dime::RectangleRenderer(BITMAP,myButton.getRectangle(),"masonwindowback.jpg"));
	myButton.setHighlightBackground(dime::RectangleRenderer(BITMAP,myButton.getRectangle(),"masonwindowback2.jpg"));

	myButton.onMouseEnter.connect(SigC::slot(mouseenterbutton));
	myButton.onMouseExit.connect(SigC::slot(mouseexitbutton));

	dime::InputService * pIS = dime::InputService::getInstance();

	pIS->addInputMapping(new dime::InputMapping(pIS->getInputDevice(dime::InputDevice::KEYBOARD), SDLK_ESCAPE, false,
		SigC::slot(myApp, &dime::Application::escPressed)));

	dime::DimeServices::getInstance()->getGuiService()->getRootWidget().addWidget(&myButton);
	
    myApp.mainLoop();
    return 0;
}