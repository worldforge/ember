#include "GuiService.h"

int dime::GuiService::refresh()
{
	return myRootWidget.draw();
}