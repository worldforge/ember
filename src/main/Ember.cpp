#include "Application.h"
#include <main/DimeServices.h>
#include <services/gui/Button.h>

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
	myButton.setBackground(dime::RectangleRenderer(BITMAP,&myButton.getRectangle().getSDL_Rect(),"masonwindowback.jpg"));

	myButton.onMouseEnter.connect(slot(mouseenterbutton));
	myButton.onMouseExit.connect(slot(mouseexitbutton));
	
	dime::DimeServices::getInstance()->getGuiService()->getRootWidget().addWidget(&myButton);
	
    myApp.mainLoop();
    return 0;
}