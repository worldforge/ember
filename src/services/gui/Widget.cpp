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
    onMouseClick = source.onMouseClick;
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