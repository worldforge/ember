#include "Application.h"
#include <main/DimeServices.h>
#include <services/image/ImageService.h>
#include <services/gui/Button.h>
#include <services/gui/Panel.h>
#include <services/gui/Label.h>
#include <services/gui/TextBox.h>
//#include <SDL/SDL_keysym.h>
#include <iostream>

void onMouseButtonDown(dime::MouseButtonEvent *event)
{
}

void onMouseButtonUp(dime::MouseButtonEvent *event)
{
    dime::Application *myApp = dime::Application::getInstance();
    myApp->quit();    
}


int main(int argc, char **argv)
{
    dime::Application *myApp = NULL;
	dime::Application::getInstanceNew();
	
	dime::ImageService::getInstance()->addPath("./bin/");
	dime::ImageService::getInstance()->addPath("../../bin/");

	dime::Label myTestLabel("Dime test!",dime::Rectangle(10,10,200,30));

	dime::TextBox myTestTextBox("TextBox!", dime::Rectangle(10,40,100,35));

	dime::Button myTestButton(dime::Rectangle(300,300,100,25));
	myTestButton.setBackground(dime::RectangleRenderer(dime::RectangleRenderer::BITMAP,myTestButton.getRectangle(),"quitbutton2.png"));
	myTestButton.setHighlightBackground(dime::RectangleRenderer(dime::RectangleRenderer::BITMAP,myTestButton.getRectangle(),"quitbutton.png"));

	dime::Panel myTestPanel(dime::Rectangle(200,200,300,200));
	myTestPanel.setBackground(dime::RectangleRenderer(dime::RectangleRenderer::BITMAP,myTestPanel.getRectangle(),"masonwindowback2.jpg"));

	myTestPanel.addWidget(&myTestButton);

    myTestButton.onMouseUp.connect(SigC::slot(onMouseButtonUp));
    myTestButton.onMouseDown.connect(SigC::slot(onMouseButtonDown));
    

	dime::InputService * pIS = dime::InputService::getInstance();

	pIS->addInputMapping(new dime::InputMapping(pIS->getInputDevice(dime::InputDevice::KEYBOARD), SDLK_ESCAPE, false,
		SigC::slot(*myApp, &dime::Application::escPressed)));

	dime::DimeServices::getInstance()->getGuiService()->getRootWidget().addWidget(&myTestPanel);
	dime::DimeServices::getInstance()->getGuiService()->getRootWidget().addWidget(&myTestLabel);
	dime::DimeServices::getInstance()->getGuiService()->getRootWidget().addWidget(&myTestTextBox);
	
    myApp->mainLoop();
    return 0;
}
