#include "Application.h"
#include <main/DimeServices.h>
#include <services/image/ImageService.h>
#include <services/gui/Button.h>
#include <services/gui/Panel.h>
//#include <SDL/SDL_keysym.h>


void mouseenterbutton(dime::Widget* button)
{
	((dime::Button *)button)->highlight();
}

void mouseexitbutton(dime::Widget* button)
{
	((dime::Button *)button)->lowlight();
}

int main(int argc, char **argv)
{
    dime::Application myApp;
	
	dime::ImageService::getInstance()->addPath("./bin/");
	dime::ImageService::getInstance()->addPath("../../bin/");

	dime::Button myTestButton;
	myTestButton.setRectangle(dime::Rectangle(300,300,100,75));
	myTestButton.setBackground(dime::RectangleRenderer(BITMAP,myTestButton.getRectangle(),"masonwindowback.jpg"));
	myTestButton.setHighlightBackground(dime::RectangleRenderer(BITMAP,myTestButton.getRectangle(),"masonwindowback2.jpg"));

	myTestButton.onMouseEnter.connect(SigC::slot(mouseenterbutton)); //TODO: internalize this to Button.
	myTestButton.onMouseExit.connect(SigC::slot(mouseexitbutton));

	dime::Panel myTestPanel;
	myTestPanel.setRectangle(dime::Rectangle(200,200,300,200));
	myTestPanel.setBackground(dime::RectangleRenderer(BITMAP,myTestPanel.getRectangle(),"masonwindowback2.jpg"));
	myTestPanel.addWidget(&myTestButton);

	dime::InputService * pIS = dime::InputService::getInstance();

	pIS->addInputMapping(new dime::InputMapping(pIS->getInputDevice(dime::InputDevice::KEYBOARD), SDLK_ESCAPE, false,
		SigC::slot(myApp, &dime::Application::escPressed)));

	dime::DimeServices::getInstance()->getGuiService()->getRootWidget().addWidget(&myTestPanel);
	
    myApp.mainLoop();
    return 0;
}
