#include "Application.h"
#include <test/TestServices.h>
#include <main/DimeServices.h>
#include <services/image/ImageService.h>
#include <services/gui/Button.h>
#include <services/gui/Panel.h>
#include <services/gui/Label.h>
#include <services/gui/TextBox.h>
#include <services/gui/Console.h>
#include <framework/Exception.h>
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
    bool success = runTests();
    if(!success)
        {
            //return 1;
        }
    
    try
        {
            
            dime::Application *myApp = dime::Application::getInstance();

            dime::ImageService::getInstance()->addPath("./bin/");
            dime::ImageService::getInstance()->addPath("../../bin/");

	    dime::Console myTestConsole(dime::Rectangle(300,120,300,120));

	    dime::Label myTestLabel("Dime test!",dime::Rectangle(10,10,200,30));
	    dime::TextBox myTestTextBox("TextBox!", dime::Rectangle(10,40,100,35));

            dime::Button myTestButton(dime::Rectangle(300,300,100,25));
            
            myTestButton.setBackground(dime::RectangleRenderer(myTestButton.getRectangle(),"quitbutton2.png", dime::RectangleRenderer::TILE));
            myTestButton.setHighlightBackground(dime::RectangleRenderer(myTestButton.getRectangle(),"quitbutton.png", dime::RectangleRenderer::TILE));
            
            dime::Panel myTestPanel(dime::Rectangle(200,200,300,200));
            myTestPanel.setBackground(dime::RectangleRenderer(myTestPanel.getRectangle(),"masonwindowback2.jpg", dime::RectangleRenderer::STRETCH));
            myTestPanel.addWidget(&myTestButton);
            
            myTestButton.onMouseUp.connect(SigC::slot(onMouseButtonUp));
            myTestButton.onMouseDown.connect(SigC::slot(onMouseButtonDown));
            
            dime::InputService * pIS = dime::InputService::getInstance();
            pIS->addInputMapping(new dime::InputMapping(pIS->getInputDevice(dime::InputDevice::KEYBOARD), SDLK_ESCAPE, false,
                                                        SigC::slot(*myApp, &dime::Application::escPressed)));
            
            dime::DimeServices::getInstance()->getGuiService()->getRootWidget().addWidget(&myTestPanel);
	    dime::DimeServices::getInstance()->getGuiService()->getRootWidget().addWidget(&myTestLabel);
	    dime::DimeServices::getInstance()->getGuiService()->getRootWidget().addWidget(&myTestTextBox);
	    dime::DimeServices::getInstance()->getGuiService()->getRootWidget().addWidget(&myTestConsole);
          
            myApp->mainLoop();
        }
    catch ( dime::Exception e )
        {
			std::cout << e.getError() << std::endl;
        }
    
    return 0;
}
