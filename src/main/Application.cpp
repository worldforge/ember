/*
 *  File:       Application.cpp
 *  Summary:    The class which initializes the GUI.
 *  Written by: nikal
 *
 *  Copyright (C) 2001, 2002 nikal. 
 *  This code is distributed under the GPL.
 *  See file COPYING for details. 
 *
 *  Change History (most recent first):    
 *
 *      $Log$
 *      Revision 1.14  2002-04-24 22:38:00  aglanor
 *      modified dimeservices and main app so MetaserverService is included, can be instantiated and is polled each step of the main loop
 *
 *      Revision 1.13  2002/04/20 17:19:24  nikal
 *      Events now have their own classes which are passed to the listeners.  and buttonup, buttondown, mousexit, and mouseenter all work properly.
 *
 *      Revision 1.12  2002/04/17 04:02:09  nikal
 *      Dime is now using the event generator, and events have been sped up.  (previously we were only processing one event per screen refresh.  now we process all events the do the screen refresh.)  The event system is very close to being in place, new widgets will come over the next few days.
 *
 *      Revision 1.11  2002/04/15 05:53:15  nikal
 *      *** empty log message ***
 *
 *      Revision 1.10  2002/04/07 05:38:25  adamgreg
 *
 *      Made GuiService draw widgets to a DrawDevice associated with it. So that the Gui can now be drawn to areas other than the main screen if desired. Also more general for when an OpenGL version turns up.
 *
 *      Revision 1.9  2002/04/02 21:15:40  tim
 *      Fixed the project files/Install for VisualC++
 *
 *      Revision 1.8  2002/03/31 19:15:45  tim
 *      Bugfixes, MSVC compatibility fixes, Since boost is working ImageService is now caching
 *
 *      Revision 1.7  2002/03/30 09:33:06  adamgreg
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
		
#ifdef USE_CPP_UNIT
		CppUnit::TextTestResult result;
		CppUnit::TestFactoryRegistry::getRegistry().makeTest()->run(&result);
		result.print(cerr);

		//See <services/input/InputServiceTest.cpp> for how to write your own tests.
#endif 

		// Initialize the InputService
		myInputService = InputService::getInstance();
	
		// Mouse and Keyboard are needed, so create them

		new MouseDevice;
		new KeyboardDevice;

		// Initialize the DrawDevice
		myDrawDevice = new SDLDrawDevice(myScreen);

		// Initialize the GuiService.
		myGuiService = DimeServices::getInstance()->getGuiService();

		// Set the DrawDevice target for GuiService
		myGuiService->setDrawTarget(myDrawDevice);

		// Initialize and start the MetaserverService.
		myMetaserverService = DimeServices::getInstance()->getMetaserverService();
		myMetaserverService->start();

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

	// SDL event polling
        SDL_Event nextEvent;
        while(SDL_PollEvent(&nextEvent)) 
            {
                dime::DimeServices::getInstance()->getInputService()->handleEvent(nextEvent);
            }
        myGuiService->refresh();

	// Metaserver polling
	// TODO: this shall later be a DefaultPoll::Poll to poll all eris connections
	myMetaserverService->poll();
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

	void Application::quit()
	{
		myShouldQuit = true;
	}

}
