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
	myButton.setX(300);
	myButton.setY(200);
	myButton.setDimension(dime::Dimension(75,50));
	
	SDL_Rect a;
	a.x = myButton.getX();
	a.y = myButton.getY();
	a.w = myButton.getDimension().getWidth();
	a.h = myButton.getDimension().getHeight();
	myButton.setBackground(new dime::RectangleRenderer(GRADIENT,&a,
		dime::Color(0,0,0),dime::Color(0,0,255),dime::Color(0,255,0),dime::Color(255,0,0)));

	myButton.onMouseEnter.connect(slot(mouseenterbutton));
	myButton.onMouseExit.connect(slot(mouseexitbutton));
	
	dime::DimeServices::getInstance()->getGuiService()->getRootWidget().addWidget(&myButton);
	
    myApp.mainLoop();
    return 0;
}

