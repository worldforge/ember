/*
 *  File:       Application.cpp
 *  Summary:    The class which initializes the GUI.
 *  Written by: nikal
 *
 *  Copyright (C) 2001, 2002 nikal. 
 *  This code is distributed under the LGPL.
 *  See file COPYING for details. 
 *
 *  Change History (most recent first):    
 *
 *      $Log$
 *      Revision 1.7  2002-03-30 09:33:06  adamgreg
 *
 *      Input now successfully obtained by GuiService from InputService. Button Widget added. Widget events work. Proper use of RectangleRenderers.
 *      The upshot is : pretty thing on screen that does stuff. Check it out!
 *
 *      Revision 1.6  2002/02/18 21:51:06  tim
 *      Added CppUnit stuff.
 *
 *      Revision 1.5  2002/01/27 23:20:53  nikal
 *      Some minor changes reflecting Dime's decision to use only dime:: as a namespace
 *
 *      Revision 1.4  2002/01/27 17:57:52  nikal
 *      Created and added a LogObserver which prints messages out to std::cerr.  Ther is a small bug. not sure where.
 *
 *      Revision 1.3  2002/01/27 00:26:37  nikal
 *      Hacking logging into Dime.
 *
 *      Revision 1.2  2002/01/24 10:58:14  tim
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
#include "DimeServices.h"
#include <iostream>
#include <iomanip>

//TODO: Put this into a config file/configuration for MSVC
#define USE_CPP_UNIT

//Needed for CppUnit
#ifdef USE_CPP_UNIT
	#include <cppunit/TextTestResult.h> 
	#include <cppunit/TestSuite.h> 
	#include <cppunit/extensions/TestFactoryRegistry.h> 
#endif

namespace dime
{
    /**
     * Predefined implementation of Observer-class for std::cerr 
     * The format of messages written to std::cerr is the following:
     *
     * timeStamp importance file "\t" line "\t" message
     */

    class CerrLogObserver: public dime::LoggingService::Observer
    {
    public:
        CerrLogObserver()
        {
        }

        virtual void onNewMessage(const std::string & message, const std::string & file, const int & line, 
                                  const dime::LoggingService::MessageImportance & importance, const time_t & timeStamp)
        {
            tm * ctm = localtime(&timeStamp); //currentLocalTime was too long, sorry

            std::cerr << "[" << ctm->tm_year+1900 << "-" << ctm->tm_mon+1 << "-" << ctm->tm_mday 
                      << " " << ctm->tm_hour << ":" <<  ctm->tm_min << ":" << ctm->tm_sec << "] "
                      << "[File: " << file << ", Line #:" <<  line << "] (";
            if(importance == dime::LoggingService::CRITICAL) 
                {
                    std::cerr << "CRITICAL";
                }
            else  if(importance == dime::LoggingService::ERROR)
                {
                    std::cerr << "ERROR";
                } 
            else if(importance == dime::LoggingService::WARNING)
                {
                    std::cerr << "WARNING";
                }
            else
                {
                    std::cerr << "INFO";
                }
            std::cerr << ") " <<message << std::endl;
        }

    private: 

    };

    Application::Application(int width, int height, std::string title)
        : myShouldQuit(false) 
    {
        myLoggingService = DimeServices::getInstance()->getLoggingService();
        myLoggingService->addObserver(new CerrLogObserver());
		
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
                myLoggingService->log(__FILE__, __LINE__, dime::LoggingService::ERROR, 
                                      "Couldn't init SDL");
                //std::cerr << "Couldn't init SDL";
                return;
            } 
        else 
            {
                myScreen = SDL_SetVideoMode(myWidth, myHeight, 16, SDL_SWSURFACE);
            }
        // Setting title.  The second argument is for an Icon. 
        // Remember if passing a std::string to a char * function, you must use .c_str()
        SDL_WM_SetCaption(title.c_str(), NULL);
			
		// Initialize the GuiService.
		myGuiService = DimeServices::getInstance()->getGuiService();
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
                    myLoggingService->log(__FILE__, __LINE__, dime::LoggingService::INFO, 
                                          "SDL_ACTIVEEVENT");
                    break;
                case SDL_KEYDOWN:
                    myLoggingService->log(__FILE__, __LINE__, dime::LoggingService::INFO, 
                                          "SDL_KEYDOWN");
                    break;
                case SDL_KEYUP:
                    // Looking for the ESC key.
                    if(nextEvent.key.keysym.sym==SDLK_ESCAPE)
                        {
                            myShouldQuit = true;
                            myLoggingService->log(__FILE__, __LINE__, dime::LoggingService::INFO, 
                                                  "Caught escape. Quitting...");
                        }
                    myLoggingService->log(__FILE__, __LINE__, dime::LoggingService::INFO, 
                                          "SDL_KEYUP");
                    break;
                case SDL_MOUSEMOTION:
                    //generates too much noise -nikal
                    //std::cout << " SDL_MOUSEMOTION\n";
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    myLoggingService->log(__FILE__, __LINE__, dime::LoggingService::INFO, 
                                          "SDL_MOUSEBUTTONDOWN");
                    break;
                case SDL_MOUSEBUTTONUP:
                    myLoggingService->log(__FILE__, __LINE__, dime::LoggingService::INFO, 
                                          "SDL_MOUSEBUTTONUP");
                    break;
                case SDL_SYSWMEVENT:
                    myLoggingService->log(__FILE__, __LINE__, dime::LoggingService::INFO, 
                                          "SDL_SYSWMEVENT");
                    break;
                case SDL_VIDEORESIZE:
                    myLoggingService->log(__FILE__, __LINE__, dime::LoggingService::INFO, 
                                          "SDL_VIDEORESIZE");
                    break;
                case SDL_VIDEOEXPOSE:
                    myLoggingService->log(__FILE__, __LINE__, dime::LoggingService::INFO, 
                                          "SDL_VIDEOEXPOSE");
                    break;
                case SDL_QUIT:
                    myShouldQuit = true;
                    break;
                }

				dime::DimeServices::getInstance()->getInputService()->handleEvent(nextEvent);
            }
		myGuiService->refresh();
    }

    void Application::mainLoop() 
    {

#ifdef USE_CPP_UNIT
		CppUnit::TextTestResult result;
		CppUnit::TestFactoryRegistry::getRegistry().makeTest()->run(&result);
		result.print(cerr);

		//See <services/input/InputServiceTest.cpp> for how to write your own tests.
#endif 

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
