#include "Widget.h"
#include "Container.h"

void dime::Widget::setParent(Container* parent)
{
	if (myParent != NULL) myParent->removeWidget(this); // Remove this Widget's previous listing before changing parents.
	
	assert(parent);
	myParent = parent;
}

dime::Widget &dime::Widget::operator= ( const dime::Widget &source )
{
    // Copy fields from source class to this class here.
    onMouseUp = source.onMouseUp;
    onMouseDown = source.onMouseDown;
    onMouseMove = source.onMouseMove;
    onMouseEnter = source.onMouseEnter;
    onMouseExit = source.onMouseExit;
    onKeyPress = source.onKeyPress;
    myX = source.myX;
    myY = source.myY;
    myDimension = source.myDimension;
    myMaxDimension = source.myMaxDimension;
    myMinDimension = source.myMinDimension;
    myPrefDimension = source.myPrefDimension;
    //Return this object with new value
    return *this;
}

dime::Widget::~Widget()
{
	myParent->removeWidget(this);
}



bool dime::Widget::checkMouseEvent(std::vector<int> coords)
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