/*
 *  File:       Application.h
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
 *      Revision 1.5  2002-03-30 09:33:06  adamgreg
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

namespace dime 
{
    class Application 
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
         * Our pointer to the GuiService
         */
        dime::GuiService *myGuiService;

        /**
         * Our pointer to the SDL_surface we use as the screen
         */
        SDL_Surface *myScreen;
       
    };//class Application
}//namespace dime

#endif
