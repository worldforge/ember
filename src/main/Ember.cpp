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
#include <services/gui/widget/Button.h>
#include <services/gui/widget/Panel.h>
#include <services/gui/widget/Label.h>
#include <services/gui/widget/TextBox.h>
#include <services/gui/widget/Console.h>
#include <framework/Exception.h>


void quitButton(dime::Button* button)
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
	    dime::TextBox myTestTextBox("TextBox!", dime::Rectangle(13,43,97,32));

            dime::Button myTestButton(dime::Rectangle(535,450,100,25));
            
            myTestButton.setBackground(new dime::BitmapRenderer(myTestButton.getRectangle(),"quitbutton2.png", dime::BitmapRenderer::TILE));
            myTestButton.setHighlightBackground(new dime::BitmapRenderer(myTestButton.getRectangle(),"quitbutton.png", dime::BitmapRenderer::TILE));
            myTestButton.setPressedBackground(new dime::BitmapRenderer(myTestButton.getRectangle(),"quitbutton3.png", dime::BitmapRenderer::TILE));
            
            dime::Panel myTestPanel(dime::Rectangle(550,0,90,90));
            myTestPanel.setBackground(new dime::BitmapRenderer(myTestPanel.getRectangle(),"dimelogo_small2.png", dime::BitmapRenderer::CENTER));
            myTestButton.onClicked.connect(SigC::slot(quitButton));
            
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
	  std::cerr << "Uncaught Dime exception: "<< e.getError() << std::endl;
        }
    catch ( Eris::BaseException e )
      {
	std::cerr << "Uncaught Eris exception: "<< e._msg << std::endl;
      }
    catch ( ... )
      {
	std::cerr << "Unknown exception dying" << std::endl;
      }
    
    return 0;
}
