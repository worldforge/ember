/*
    Copyright (C) 2002  Lakin Wecker aka (nikal)

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

#include "Button.h"

int dime::Button::draw(DrawDevice *target)
{
	(*myCurrentBackground)->render(target);
	//%TODO nikal,2: what should be returned here?
	return 0;
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
				//onMouseEnter.emit(this);
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
				//onMouseExit.emit(this);
			}
			return false;
		}
	}
}

void dime::Button::mouseUp( MouseButtonEvent *event ) 
{
  if (myPressed)
    {
      // Uncapture the mouse
      releaseMouse();
      myPressed = false;

      if (myRectangle.contains(event->getPos()))
	{
	  onClicked.emit(this);
	}
    }

  Widget::mouseUp(event);
}
