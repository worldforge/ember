#include "Widget.h"
#include <assert.h>

void dime::Widget::setParent(Widget* parent)
{
	if (myParent != NULL) myParent->removeWidget(this); // Remove this Widget's previous listing before changing parents.
	
	assert(parent);
	myParent = parent;
}

dime::Widget &dime::Widget::operator= ( const dime::Widget &source )
{
    // Copy fields from source class to this class here.
    myRectangle = source.myRectangle;
    myMaxDimension = source.myMaxDimension;
    myMinDimension = source.myMinDimension;
    myPrefDimension = source.myPrefDimension;
    //Return this object with new value
    return *this;
}

dime::Widget::~Widget()
{
	if (myParent != NULL) myParent->removeWidget(this);
}

void dime::Widget::addWidget(Widget *source)
{
	assert(source);
	myChildren.push_back(source);
	source->setParent(this);
}

int dime::Widget::removeWidget(Widget* target)
{
	//iterate through children Widgets, erasing target if it is amongst them.
	std::vector<Widget*>::iterator end = myChildren.end();
	for (std::vector<Widget*>::iterator i = myChildren.begin(); i != end; i++)
	{
		if (*i == target)
		{
			myChildren.erase(i);
			return 0; //success.
		}
	}
	return 1; //target is not amongst myChildren.
}

int dime::Widget::draw(DrawDevice *target)
{
	//iterate through children Widgets, telling them all to draw.
	int retval; //return value
	
	std::vector<Widget*>::iterator end = myChildren.end();
	for (std::vector<Widget*>::iterator i = myChildren.begin(); i != end; i++)
	{
		retval = (*i)->draw(target);
		if (retval != 0) return retval;
	}
	return 0;
}

bool dime::Widget::checkMouseEvent(std::vector<int> coords)
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
