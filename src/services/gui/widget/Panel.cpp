/*
    Copyright (C) 2002  Adam Gregory

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

#include "Panel.h"

int dime::Panel::draw(DrawDevice *target)
{
  //iterate through children Widgets, telling them all to draw.
  int retval; //return value

  if (myBackground) myBackground->render(target);

  std::vector<Widget*>::iterator end = myChildren.end();
  for (std::vector<Widget*>::iterator i = myChildren.begin(); i != end; i++)
    {
      retval = (*i)->draw(target);
      if (retval != 0) return retval;
    }
  return 0;
}

bool dime::Panel::checkMouseEvent(std::vector<int> coords)
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
	    }
	  std::vector<Widget*>::iterator end = myChildren.end();
	  for (std::vector<Widget*>::iterator i = myChildren.begin(); i != end; i++)
	    {
	      if ((*i)->checkMouseEvent(coords))
		{
		  return true; // Coordinates also inside a child of this widget.
		}
	    }
	  // Coordinates lie inside this widget and none of its children.
	  // TODO: If a mouse click then give focus to THIS widget, since no children qualify.
	  return true;
	}
      else
	{
	  // Coordinates are outside of this widget.
	  if (myMouseIsInside)
	    {
	      // Mouse just exited this widget.
	      myMouseIsInside = false;
	    }
	  return false;
	}
    }
}
