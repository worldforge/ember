/*
 *  File:       Application.h
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
 *      Revision 1.1  2002-01-20 16:20:39  nikal
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


namespace dime 
{
    namespace main 
    {
        class Application 
        {
        public:
            /**
             * Ctor for dime::main::Application.  Allcoates and pops up an SDL window 
             * of width, and height with title.
             *
             * @param width The width of the Application window(default=640)
             * @param height The height of the Application window(default=480)
             * @param title The title of the Application window(default="Dime Application")
             */      
            Application(int width=640, int height=480, std::string title="Dime Application");

            /**
             * Dtor for dime::main::Application.  Free the current surface.
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
            int myWidth;
            int myHeight;
            bool myShouldQuit;
       
            SDL_Surface *myScreen;
       
        };//class Application
    }//namespace main
}//namespace dime

#endif
