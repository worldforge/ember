#include "Container.h"

int dime::Container::Draw()
{
	//iterate through children Widgets, telling them all to draw.
	int retval; //return value
	
	std::vector<Widget*>::iterator end = myChildren.end();
	for (std::vector<Widget*>::iterator i = myChildren.begin(); i != end; i++)
	{
		retval = (*i)->Draw();
		if (retval != 0) return retval;
	}
	return 0;
}

void dime::Container::AddWidget(Widget *source)
{
	assert(source);
	myChildren.push_back(source);
	source->SetParent(this);
}

int dime::Container::RemoveWidget(Widget* destructee)
{
	//iterate through children Widgets, erasing destructee if it is amongst them.
	std::vector<Widget*>::iterator end = myChildren.end();
	for (std::vector<Widget*>::iterator i = myChildren.begin(); i != end; i++)
	{
		if (*i == destructee)
		{
			myChildren.erase(i);
			return 0; //success.
		}
	}
	return 1; //destructee is not amongst myChildren.
}