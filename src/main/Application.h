/*
 *  File:       Application.h
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
 *      Revision 1.18  2002-09-07 13:38:10  aglanor
 *      Configuration service is now started with the application. I've also break the log line in the src/main Makefile.am which loads the serrvice static libs, now it's more readable.
 *
 *      Revision 1.17  2002/08/30 19:31:33  aglanor
 *      sound service is now created and started in the application
 *
 *      Revision 1.16  2002/05/13 19:37:53  nikal
 *      Preliminary Unicode support.  I think this breaks TextBox, but I'm not sure. :)
 *
 *      Revision 1.15  2002/05/11 00:02:43  xmp
 *      Final fixes and tidy up.  Application now a singleton.
 *
 *      Revision 1.14  2002/05/10 23:51:25  xmp
 *      A slightly broken attempt to turn Application into a singleton.  Still needs work
 *
 *      Revision 1.13  2002/04/26 12:16:51  xmp
 *      Replaced my previous hacky style #ifdefs with one #if per metaserver location.
 *
 *      Revision 1.12  2002/04/25 22:35:32  xmp
 *      Three changes rating: minor/bugfix
 *      -Ok MSVC #ifdef's in to avoid use of ERIS.
 *      -Made a few variables initialise to NULL in eventgenerator to fix segfaults
 *      -Added a few items to the MSVC project file
 *
 *      Revision 1.11  2002/04/24 22:38:00  aglanor
 *      modified dimeservices and main app so MetaserverService is included, can be instantiated and is polled each step of the main loop
 *
 *      Revision 1.10  2002/04/20 20:37:04  tim
 *      Compatibility and bug fixes
 *
 *      Revision 1.9  2002/04/15 05:53:15  nikal
 *      *** empty log message ***
 *
 *      Revision 1.8  2002/04/08 07:51:28  adamgreg
 *
 *      Added simple panels. Cleaned up the way my Gui code was using SDLDrawDevices instead of the DrawDevice base class. Made GuiService get the root widget dimensions properly. Moved Rectangle.h and Dimension.h to services/platform/ because they're used there and not just in the gui, and are pretty useful for other things. Probably did some other things I've forgotten about :). Mostly just cleaning it up.
 *
 *      Revision 1.7  2002/04/07 05:38:25  adamgreg
 *
 *      Made GuiService draw widgets to a DrawDevice associated with it. So that the Gui can now be drawn to areas other than the main screen if desired. Also more general for when an OpenGL version turns up.
 *
 *      Revision 1.6  2002/03/31 19:15:45  tim
 *      Bugfixes, MSVC compatibility fixes, Since boost is working ImageService is now caching
 *
 *      Revision 1.5  2002/03/30 09:33:06  adamgreg
 *
 *      Input now successfully obtained by GuiService from InputService. Button Widget added. Widget events work. Proper use of RectangleRenderers.
 *      The upshot is : pretty thing on screen that does stuff. Check it out!
 *
 *      Revision 1.4  2002/01/27 23:20:53  nikal
 *      Some minor changes reflecting Dime's decision to use only dime:: as a namespace
 *
 *      Revision 1.3  2002/01/27 17:57:52  nikal
 *      Created and added a LogObserver which prints messages out to std::cerr.  Ther is a small bug. not sure where.
 *
 *      Revision 1.2  2002/01/27 00:26:37  nikal
 *      Hacking logging into Dime.
 *
 *      Revision 1.1  2002/01/20 16:20:39  nikal
 *      Cleaning up the style of Application.h/cpp and Dime
 *
 *      Revision 1.2  2002/01/13 01:50:09  zzorn
 *      Changed comments to use /** so that doxygen generates documentation for them.
 *
 *      Revision 1.1  2002/01/12 23:43:26  nikal
 *      adding the skeleton Application and a basic int main()
 *
 *      Revision 1.2  2001/12/09 20:40:02  nikal
 *      *** empty log message ***
 *
 */

#ifndef DIME_APPLICATION_H
#define DIME_APPLICATION_H

#include <list>
#include <string>
#include <SDL/SDL.h>
#include <main/DimeServices.h>
#include <services/platform/SDLDrawDevice.h>

namespace dime 
{
    class Application : public SigC::Object
    {
        //======================================================================
        // Inner Classes, Typedefs, and Enums
        //======================================================================
    public:
        //======================================================================
        // Public Methods
        //======================================================================
    public:    
        /**
         * Ctor for dime::::Application.  Allcoates and pops up an SDL window 
         * of width, and height with title.
         *
         * @param width The width of the Application window(default=640)
         * @param height The height of the Application window(default=480)
         * @param title The title of the Application window(default="Dime Application")
         */      
        Application(int width=640, int height=480, std::string title="Dime Application");

        /**
         * Dtor for dime::::Application.  Free the current surface.
         *
         */
        ~Application();
       
        /**
         * Main loop step.   Does one iteration of the mainloop. 
         *
         *
         */
        void mainLoopStep();

        /**
         * the Main loop.  Returns when application has received an "exit" command
         *
         */
        void mainLoop();
       
        /**
         * return true if application has received an "exit" command else false.
         *
         * @return true if "shouldquit" else false
         */
        bool shouldQuit();

		/**
		 * Causes the application to quit.
		 */
		void quit();
	   
		void escPressed(InputDevice * , InputDevice * keyDevice, const DimeKey &key, InputMapping::InputSignalType signaltype)
		{
			quit();
		}
		
		//----------------------------------------------------------------------
		// Singleton

		/**
		 * Returns the Application instance.
		 */
		static Application *getInstance()
		{
			if( theApplication )
			{
				return theApplication;
			}
			else
			{
				theApplication = new Application();
				return theApplication;
			}
		}

    private:
        /**
         * The width of the screen
         */
        int myWidth;

        /**
         * The height of the screen
         */
        int myHeight;

        /**
         * An internal flag used to signal when to quite.
         */
        bool myShouldQuit;
       
        /**
         * Our pointer to the LoggingService
         */
        dime::LoggingService *myLoggingService;

		/**
		* Our pointer to the ConfigService
		 */
		dime::ConfigService *myConfigService;

        /**
         * Our pointer to the GuiService
         */
        dime::GuiService *myGuiService;

        /**
         * Our pointer to the InputService
         */
        dime::InputService *myInputService;

        /**
         * Our pointer to the MetaserverService
         */
#if defined( _MSC_VER ) && ( _MSC_VER < 1300 )
// GNDN: MSVC < version 7 is broken
#else
        dime::MetaserverService *myMetaserverService;
#endif
        /**
         * Our pointer to the SDL_surface we use as the screen
         */
        SDL_Surface *myScreen;

		/**
		 * Our pointer to a DrawDevice wrapped around the SDL_surface we use as the screen
		 */
		SDLDrawDevice *myDrawDevice;

		/**
		* Our pointer to the SoundService
		 */
		dime::SoundService *mySoundService;

		/**
		 * Pointer to ourselves
		 */
		static Application *theApplication;
       
    };//class Application
}//namespace dime

#endif
