/*
  Copyright (C) 2002  Lakin Wecker

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

#include "Widget.h"
#include <assert.h>

long dime::Widget::theIDNext = 0;

void dime::Widget::setParent(Widget* parent)
{
	if (myParent != NULL) myParent->removeWidget(this); // Remove this Widget's previous listing before changing parents.
	
	assert(parent);
	myParent = parent;
}

void dime::Widget::setEventGenerator(EventGenerator *generator)
{
    //assert(generator);
    myEventGenerator = generator;
    
    std::vector<Widget*>::iterator end = myChildren.end();
    for (std::vector<Widget*>::iterator i = myChildren.begin(); i != end; ++i)
        {
            (*i)->setEventGenerator(generator);
        }
    
}



dime::Widget &dime::Widget::operator= ( const dime::Widget &source )
{
    // Copy fields from source class to this class here.
    myRectangle = source.myRectangle;
    myMaxDimension = source.myMaxDimension;
    myMinDimension = source.myMinDimension;
    myPrefDimension = source.myPrefDimension;
    myVisible = source.myVisible;
    //Return this object with new value
    return *this;
}

dime::Widget::~Widget()
{
  if (myParent != NULL) myParent->removeWidget(this);
  removeAllWidgets();
}

void dime::Widget::addWidget(Widget *source)
{
    assert(source);
    myChildren.push_back(source);
    source->setEventGenerator(myEventGenerator);
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

void dime::Widget:: removeAllWidgets()
{
  // TODO: Check this deletes all the child widgets
  myChildren.clear();
}

int dime::Widget::draw(DrawDevice *target)
{
	//iterate through children Widgets, telling them all to draw.
	int retval; //return value
	
	std::vector<Widget*>::iterator end = myChildren.end();
	for (std::vector<Widget*>::iterator i = myChildren.begin(); i != end; i++)
	{
		if( (*i)->myVisible == true )
		  {
		    retval = (*i)->draw(target);
		    if (retval != 0) return retval;
		  }
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
			//%TODO nikal,2: If a mouse click then give focus to THIS widget, since no children qualify.
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



dime::Widget *dime::Widget::getWidgetAt(int x, int y)
{
    dime::Widget *returnWidget = NULL;
    if (myRectangle.contains(x, y))
        {
            std::vector<Widget*>::iterator end = myChildren.end();
            for (std::vector<Widget*>::iterator i = myChildren.begin(); i != end && returnWidget==NULL; i++)
                {
                    returnWidget = (*i)->getWidgetAt(x, y);
                }
            if(returnWidget==NULL)
                {
                    //points lie inside, but aren't our child widgets... must be us.
                    returnWidget=this;
                }
            		
        }
    return returnWidget;
	
}

inline bool dime::Widget::captureMouse() {
  return myEventGenerator->captureMouse(this);
}

inline void dime::Widget::releaseMouse() {
  myEventGenerator->releaseMouse(this);
}
