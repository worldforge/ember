/*
    Copyright (C) 2001,2002  Martin Pollard (Xmp), Lakin Weckerd (nikal)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

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

	    dime::Console myTestConsole(dime::Rectangle(10,300,620,120));

	    dime::Label myTestLabel("Dime test!",dime::Rectangle(10,10,200,30));
	    dime::TextBox myTestTextBox("TextBox!", dime::Rectangle(10,40,100,35));

            dime::Button myTestButton(dime::Rectangle(535,450,100,25));
            
            myTestButton.setBackground(dime::RectangleRenderer(myTestButton.getRectangle(),"quitbutton2.png", dime::RectangleRenderer::TILE));
            myTestButton.setHighlightBackground(dime::RectangleRenderer(myTestButton.getRectangle(),"quitbutton.png", dime::RectangleRenderer::TILE));
            
            dime::Panel myTestPanel(dime::Rectangle(558,10,72,90));
            myTestPanel.setBackground(dime::RectangleRenderer(myTestPanel.getRectangle(),"dimelogo_small.png", dime::RectangleRenderer::TILE));
            
            myTestButton.onMouseUp.connect(SigC::slot(onMouseButtonUp));
            myTestButton.onMouseDown.connect(SigC::slot(onMouseButtonDown));
            
            dime::InputService * pIS = dime::InputService::getInstance();
            pIS->addInputMapping(new dime::InputMapping(pIS->getInputDevice(dime::InputDevice::KEYBOARD), SDLK_ESCAPE, false,
                                                        SigC::slot(*myApp, &dime::Application::escPressed)));
            
            dime::DimeServices::getInstance()->getGuiService()->getRootWidget().addWidget(&myTestPanel);
	    dime::DimeServices::getInstance()->getGuiService()->getRootWidget().addWidget(&myTestLabel);
	    dime::DimeServices::getInstance()->getGuiService()->getRootWidget().addWidget(&myTestTextBox);
	    dime::DimeServices::getInstance()->getGuiService()->getRootWidget().addWidget(&myTestConsole);
	    dime::DimeServices::getInstance()->getGuiService()->getRootWidget().addWidget(&myTestButton);          
            myApp->mainLoop();
        }
    catch ( dime::Exception e )
        {
			std::cout << e.getError() << std::endl;
        }
    
    return 0;
}
