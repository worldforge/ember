#include "ParaGuiApplication.h"

dime::ParaGuiApplication::ParaGuiApplication(int w, int h) 
{
       
    // TODO (nikal 2002.02.17) Refactor this out into a better system for themes.
    myApplication.LoadTheme("default");
    
    myApplication.InitScreen(w, h, 0, SDL_SWSURFACE);
}


void dime::ParaGuiApplication::run()
{
    myApplication.Run();
    
}

bool dime::ParaGuiApplication::eventLoop()
{
    //TODO (nikal 2002.02.17) implement this.
}

bool dime::ParaGuiApplication::handleEvent(SDL_Event *event)
{
    //TODO (nikal 2002.02.17) implement this.
}

void dime::ParaGuiApplication::setRootContainert(Container *container)
{
    //TODO (nikal 2002.02.17) implement this.
}

dime::Container *dime::ParaGuiApplication::getRootContainer()
{
    //TODO (nikal 2002.02.17) implement this.
}

dime::ParaGuiApplication::~ParaGuiApplication() 
{
}
