#include "TextBox.h"

int dime::TextBox::draw(DrawDevice *target)
{
	// Draw the background rect
	myBackRect.render(target);

	// Use fontservice to draw the text of the widget
	Label::draw(target);
	//TODO: what should be returned here?
	return 0;
}