#include "ParaGuiService.h"

dime::Application *dime::ParaGuiService::createApplication(int width, int height)
{
    return (Application *)new ParaGuiApplication(width, height);
}

