#include "Button.h"

int dime::Button::draw()
{
	myBackground->render(&SDLDrawDevice(SDL_GetVideoSurface()));
}

void dime::Button::highlight()
{
	myBackground->Gradient(GRADIENT,dime::Color(255,0,0),dime::Color(0,255,0),dime::Color(0,0,255),dime::Color(0,0,0));
}

void dime::Button::lowlight()
{
	myBackground->Gradient(GRADIENT,dime::Color(0,0,0),dime::Color(0,0,255),dime::Color(0,255,0),dime::Color(255,0,0));
}

bool dime::Button::checkMouseEvent(std::vector<int> coords)
{
	if (coords.size() != 2)
	{
		return false; // Invalid coordinates vector.
	}
	else
	{
		if ((coords[0] > getX()) && (coords[0] < getX() + getDimension().getWidth()) && (coords[1] > getY()) && (coords[1] < getY() + getDimension().getHeight()))
		{
			// Coordinates lie inside this widget.
			if (!myMouseIsInside)
			{
				// Mouse has just entered widget.
				myMouseIsInside = true;
				onMouseEnter.emit(this);
			}

			// Coordinates lie inside this widget.
			// TODO: If a mouse click, give focus.
			return true;
		}
		else
		{
			// Coordinates are outside of this widget.
			if (myMouseIsInside)
			{
				// Mouse just exited this widget.
				myMouseIsInside = false;
				onMouseExit.emit(this);
			}
			return false;
		}
	}
}