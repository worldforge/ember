#include "Button.h"

int dime::Button::draw(SDLDrawDevice *target)
{
	myCurrentBackground->render(&SDLDrawDevice(SDL_GetVideoSurface()));
	//TODO: what should be returned here?
	return 0;
}

void dime::Button::highlight()
{
	myCurrentBackground = &myHighlightBackground;
}

void dime::Button::lowlight()
{
	myCurrentBackground = &myStandardBackground;
}

bool dime::Button::checkMouseEvent(std::vector<int> coords)
{
	if (coords.size() != 2)
	{
		return false; // Invalid coordinates vector.
	}
	else
	{
		if (myRectangle.contains(coords))
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