#include "Widget.h"
Dime::Widget &Dime::Widget::operator= ( const Dime::Widget &source );
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
    return &this;
}
