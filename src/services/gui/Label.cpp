#include "Label.h"

int dime::Label::draw(DrawDevice *target)
{
	// Use fontservice to draw
	myFontRenderer.render(target);
	//TODO: what should be returned here?
	return 0;
}
