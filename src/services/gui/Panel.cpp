#include "Panel.h"

int dime::Panel::draw(DrawDevice *target)
{
	//iterate through children Widgets, telling them all to draw.
	int retval; //return value

	myBackground.render(target);

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