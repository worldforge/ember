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
 *      Revision 1.41  2002-11-12 18:52:24  xmp
 *      2002-11-12 M Pollard <circlemaster@blueyonder.co.uk>
 *              * configure.in: added to allow older distros to build
 *
 *              * configure.ac: modified to workaround bug in libxml's AM_PATH (not
 *                setting CPPFLAGS when checking only CFLAGS)
 *
 *              * MetaserverService.cpp/h, Application.cpp/h: Removed DataModel, it's
 *                buggy.
 *
 *              * Dime.cpp Applicatition.cpp/h StateManager.cpp:
 *                Moved Widget loading into StateManager.  Fixed a circular calling bug.
 *
 *      Known bug in that the widgets still aren't quite loading yet.  I'll have this fixed soon.  Escape key to quit still works however.
 *
 *      Revision 1.40  2002/11/12 17:46:47  xmp
 *      Readded configure.in
 *
 *      Revision 1.39  2002/10/09 13:41:22  xmp
 *      -Updated documentation
 *      -Polished a few things
 *      -Added quit command
 *
 *      Revision 1.38  2002/10/04 14:44:32  xmp
 *      Major code cleanup of Application Class.  Removed variables that were duplicated in DimeServices
 *
 *      Revision 1.37  2002/09/07 13:38:10  aglanor
 *      Configuration service is now started with the application. I've also break the log line in the src/main Makefile.am which loads the serrvice static libs, now it's more readable.
 *
 *      Revision 1.36  2002/08/30 19:31:33  aglanor
 *      sound service is now created and started in the application
 *
 *      Revision 1.35  2002/08/28 21:46:35  xmp
 *      Support for new verbose logging mode
 *
 *      Revision 1.34  2002/08/27 17:47:05  xmp
 *      Turn this on and bingo
 *
 *      Revision 1.33  2002/07/15 22:51:33  nikal
 *      Moved the errant #include to the top of the file.
 *
 *      Revision 1.32  2002/07/14 16:34:43  tim
 *      *** empty log message ***
 *
 *      Revision 1.31  2002/07/03 11:38:48  xmp
 *      Hook eris's logging service into Dime's
 *
 *      Revision 1.30  2002/07/02 22:00:13  tim
 *      Dump level added
 *
 *      Revision 1.29  2002/07/02 19:13:49  tim
 *      Test usage of DDM in MetaserverService
 *
 *      Revision 1.28  2002/07/02 03:35:18  nikal
 *      Added OpenGL DrawDevice.  It's not complete.  It needs a blitSurface function implemenation. It compiles, and hasn't been tested, but I don't have the time.  Anyone can feel free to implement it, or test it.  I will as soon as I have time if I can.
 *
 *      Revision 1.27  2002/05/27 00:25:56  nikal
 *      Tests are now being run
 *
 *      Revision 1.26  2002/05/25 22:17:54  xmp
 *      Add serverservice to the main build.
 *
 *      Revision 1.25  2002/05/16 04:26:57  nikal
 *       I believe this enable testing with CPPUNIT to be done on a MakeCheck
 *
 *      Revision 1.24  2002/05/15 20:15:57  tim
 *      First DataModel release!
 *
 *      Revision 1.23  2002/05/14 04:54:44  tim
 *      Minor fixes
 *
 *      Revision 1.22  2002/05/13 20:52:39  nikal
 *      Unicode should be working
 *
 *      Revision 1.21  2002/05/11 00:02:43  xmp
 *      Final fixes and tidy up.  Application now a singleton.
 *
 *      Revision 1.20  2002/05/10 23:56:41  tim
 *      *** empty log message ***
 *
 *      Revision 1.19  2002/05/10 22:35:49  tim
 *      Fixed RepititionDevice (by adding something to SDL_INIT)
 *
 *      Revision 1.18  2002/04/30 09:17:51  nikal
 *      Remember to use proper capitalization in directory and file names.
 *
 *      Revision 1.17  2002/04/29 19:23:48  xmp
 *      Moved to new eris polling method
 *
 *      Revision 1.16  2002/04/26 12:16:50  xmp
 *      Replaced my previous hacky style #ifdefs with one #if per metaserver location.
 *
 *      Revision 1.15  2002/04/25 22:35:32  xmp
 *      Three changes rating: minor/bugfix
 *      -Ok MSVC #ifdef's in to avoid use of ERIS.
 *      -Made a few variables initialise to NULL in eventgenerator to fix segfaults
 *      -Added a few items to the MSVC project file
 *
 *      Revision 1.14  2002/04/24 22:38:00  aglanor
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
#include "services/datamodel/DataObject.h"
#include "services/datamodel/DataModelService.h"
#include "framework/ConsoleBackend.h"

#include <iostream>
#include <iomanip>
#include <strstream>
#include <iterator>
#ifndef WIN32
#include <stdio.h>
#endif

#if defined( _MSC_VER ) && ( _MSC_VER < 1300 )
// GNDN: MSVC < version 7 is broken
#else
#include <Eris/PollDefault.h>
#include <Eris/Log.h>
#endif

namespace dime
{
    /**
     * Predefined implementation of Observer-class for std::cerr 
     * The format of messages written to std::cerr is the following:
     *
     * timeStamp importance file "\t" line "\t" message
     */

    Application * Application::theApplication = NULL;

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
		
	    std::cerr.fill('0');
            std::cerr << "[";
	    std::cerr.width(2);		
	    std::cerr << (ctm->tm_year/*+1900*/)%100 << "-";
	    std::cerr.width(2);					
	    std::cerr << ctm->tm_mon+1 << "-";
	    std::cerr.width(2);			
	    std::cerr << ctm->tm_mday << " ";
	    std::cerr.width(2);
	    std::cerr << ctm->tm_hour << ":";
	    std::cerr.width(2);
	    std::cerr <<  ctm->tm_min << ":";
	    std::cerr.width(2);			
	    std::cerr << ctm->tm_sec << "] ";			
	    std::cerr  << "[File: " << file << ", Line #:" <<  line << "] (";

            if(importance == dime::LoggingService::CRITICAL)
                {
                    std::cerr << "CRITICAL";
                }
            else  if(importance == dime::LoggingService::FAILURE)
                {
                    std::cerr << "FAILURE";
                } 
            else if(importance == dime::LoggingService::WARNING)
                {
                    std::cerr << "WARNING";
                }
            else if(importance == dime::LoggingService::INFO)
                {
                    std::cerr << "INFO";
                }
	    else
                {
                    std::cerr << "VERBOSE";
                }
            std::cerr << ") " <<message << std::endl;
        }

    private: 

    };

#if 0
  void onMetaserverService(PDataObject p, DataType t)
  {
    //fired whenever the state changed
    std::strstream dump;
    dump << "onMetaserverService called." << std::endl;
    dump << "dumping /servers {" << std::endl;
    
    DataModelService::dump(DataObject::getRoot("/"), dump, true, 0);
    
    dump << "}";
    dump.put(0);

    LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO)
      << dump.str() << ENDM;
  }
#endif

    Application::Application(int width, int height, std::string title) :
      myWidth(width), myHeight(height), myShouldQuit(false) 
    {


        LoggingService *logging = DimeServices::getInstance()->getLoggingService();
	CerrLogObserver* obs = new CerrLogObserver();
	obs->setFilter(LoggingService::VERBOSE);
	logging->addObserver(obs);

        if(myWidth < 0)
	  {
	    myWidth=0;
	  }
        if(myHeight < 0)
	  {
	    myHeight = 0;
	  }


        if((SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER)==-1)) 
            {
                logging->log(__FILE__, __LINE__, dime::LoggingService::FAILURE, 
                                      "Couldn't init SDL");
                return;
            } 
        else 
            {    
                
                myScreen = SDL_SetVideoMode(myWidth, myHeight, 16, SDL_SWSURFACE);
            }

	SDL_EnableUNICODE(1);

        // Setting title.  The second argument is for an Icon. 
        // Remember if passing a std::string to a char * function, you must use .c_str()
        SDL_WM_SetCaption(title.c_str(), NULL);

	// Initialize the InputService
	myInputService = InputService::getInstance();

	// Initialize the ConfigService
	DimeServices::getInstance()->getConfigService()->start();

	// Mouse and Keyboard are needed, so create them
	// %TODO xmp,5: WHAT THE...? I suspect that these are in error
	new MouseDevice;
	new KeyboardDevice;

	// Initialize the GuiService.
	// Initialize the DrawDevice
	// Set the DrawDevice target for GuiService
	DimeServices::getInstance()->getGuiService()->setDrawTarget(new SDLDrawDevice(myScreen));

#ifndef WIN32
	// Test that /dev/dsp is availible
	FILE *temp = fopen("/dev/dsp","w");
	if (temp) {
	  fclose(temp);
#endif
	// Initialize the SoundService
	DimeServices::getInstance()->getSoundService()->start();
#ifndef WIN32
	}
#endif

	// Initialize and start the MetaserverService.
#if defined( _MSC_VER ) && ( _MSC_VER < 1300 )
// GNDN: MSVC < version 7 is broken
#else
	// Set Eris Logging Level
	Eris::setLogLevel(Eris::LOG_DEBUG);

	DimeServices::getInstance()->getMetaserverService()->start();

#if 0		
	//just for test:
	PDataObject metaState = DataObject::getRoot("/servers/state"); 
	metaState->addConnection(SigC::slot(onMetaserverService), POST_VALUE_CHANGE);

	onMetaserverService(metaState, POST_VALUE_CHANGE);
#endif
	// Create and start ServerService
	DimeServices::getInstance()->getServerService()->start();
#endif

	ConsoleBackend::getMainConsole()->registerCommand(CMD_QUIT,this);
	
	// Bring our StateManager into being
	getStateMgr();
    }

    Application::~Application()
    {
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
        dime::DimeServices::getInstance()->getGuiService()->refresh();

	// Eris polling
#if defined( _MSC_VER ) && ( _MSC_VER < 1300 )
// GNDN: MSVC < version 7 is broken
#else
	Eris::PollDefault::poll();
#endif
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

    void Application::runCommand(const std::string& cmd, const std::string& args)
    {
      if (cmd==CMD_QUIT){
	quit();
      }
    }
}
