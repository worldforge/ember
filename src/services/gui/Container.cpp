#include "Container.h"

virtual int dime::Container::draw()
{
	//iterate through children Widgets, telling them all to draw.
	int retval; //return value
	
	std::vector<Widget*>::iterator end = myChildren.end();
	for (std::vector<Widget*>::iterator i = myChildren.begin(); i != end; i++)
	{
		retval = (*i)->draw();
		if (retval != 0) return retval;
	}
	return 0;
}

void dime::Container::addWidget(Widget *source)
{
	assert(source);
	myChildren.push_back(source);
	source->setParent(this);
}

int dime::Container::removeWidget(Widget* target)
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