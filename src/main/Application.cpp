/*
 *  File:       Application.cpp
 *  Summary:    The class which initializes the GUI.
 *  Written by: nikal
 *
 *  Copyright  2001 nikal. 
 *  This code is distributed under the LGPL.
 *  See file COPYING for details. 
 *
 *  Change History (most recent first):    
 *
 *      $Log$
 *      Revision 1.2  2002-01-24 10:58:14  tim
 *      Changed observer handling of the LoggingService to a more C++-convenient way (thanks goes to Nikal)
 *      Added the Visual C++ 6.0 workspace and a small readme to the CVS
 *
 *      Revision 1.1  2002/01/20 16:20:39  nikal
 *      Cleaning up the style of Application.h/cpp and Dime
 *
 *      Revision 1.1  2002/01/12 23:43:26  nikal
 *      adding the skeleton Application and a basic int main()
 *
 *      Revision 1.2  2001/12/09 20:40:02  nikal
 *      *** empty log message ***
 *
 */

#include "Application.h"
#include <iostream>

namespace dime
{
    namespace main
    {

        Application::Application(int width, int height, std::string title)
            : myShouldQuit(false) 
        {
            if(width >=0)
                {
                    myWidth=width;
                }
            if(height >=0)
                {
                    myHeight=height;
                }
            if((SDL_Init(SDL_INIT_VIDEO)==-1)) 
                {
					std::cerr << "Couldn't init SDL";
                    return;
                } 
            else 
                {
                    myScreen = SDL_SetVideoMode(myWidth, myHeight, 8, SDL_SWSURFACE);
                }
            // Setting title.  The second argument is for an Icon. 
            // Remember if passing a std::string to a char * function, you must use .c_str()
            SDL_WM_SetCaption(title.c_str(), NULL);
        }

        Application::~Application() {
            if(myScreen != NULL) 
                {
                    SDL_FreeSurface(myScreen);
                }
            SDL_Quit();
        }

        void Application::mainLoopStep() 
        {
            SDL_Event nextEvent;
            if(SDL_PollEvent(&nextEvent)) 
                {
                    switch(nextEvent.type) {
                    case SDL_ACTIVEEVENT:
                        std::cout << " SDL_ACTIVEEVENT\n";
                        break;
                    case SDL_KEYDOWN:
                        std::cout << " SDL_KEYDOWN\n";
                        break;
                    case SDL_KEYUP:
                        // Looking for the ESC key.
                        if(nextEvent.key.keysym.sym==SDLK_ESCAPE)
                            {
                                myShouldQuit = true;
								std::cout << "Caught escape. Quitting...\n";
                            }
                        std::cout << " SDL_KEYUP\n";
                        break;
                    case SDL_MOUSEMOTION:
                        //generates too much noise -nikal
                        //std::cout << " SDL_MOUSEMOTION\n";
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        std::cout << " SDL_MOUSEBUTTONDOWN\n";
                        break;
                    case SDL_MOUSEBUTTONUP:
                        std::cout << " SDL_MOUSEBUTTONUP\n";
                        break;
                    case SDL_SYSWMEVENT:
                        std::cout << "SDL_SYSWMEVENT\n";
                        break;
                    case SDL_VIDEORESIZE:
                        std::cout << "SDL_VIDEORESIZE\n";
                        break;
                    case SDL_VIDEOEXPOSE:
                        std::cout << "SDL_VIDEOEXPOSE\n";
                        break;
                    case SDL_QUIT:
                        myShouldQuit = true;
                        break;
                    }
     
                }
            //return nextEvent;
        }

        void Application::mainLoop() 
        {
            while(myShouldQuit == false) 
                {
                    mainLoopStep();
                }
        }
        bool Application::shouldQuit() 
        {
            return myShouldQuit;
        }

    }
}
