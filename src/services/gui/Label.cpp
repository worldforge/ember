#include "Label.h"

int dime::Label::draw(DrawDevice *target)
{
	// Use fontservice to draw
	myFont.render(target);
	//TODO: what should be returned here?
	return 0;
}
