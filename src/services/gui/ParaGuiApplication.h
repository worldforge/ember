/*
    Copyright (C) 2002  Lakin Wecker

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef PARAGUIAPPLICATION_H
#define PARAGUIAPPLICATION_H

// Included headers from the current project
#include "Application.h"
#include "Container.h"
// Included custom library headers

// Included system headers
#include <SDL/SDL.h>
#include <paragui/pgapplication.h>

namespace dime {

/**
 * Holds the main window and main event loop for the GUI.
 *
 * A Singleton which holds the Main Window  for the Application.
 * It holds the rootWidget pointer also.  It is recommended that the 
 * rootWidget be a container of some sort.
 * It also handles the event loop for the GUI.  you can call: 
 * Application->run(); which will return after the application has been closed.
 * or Application->eventLoop(); which will return after all events are processed.
 * or Application->processEvent(SDL_Event *); which will just process the one event
 * and will leave the event-queue alone.
 *
 *
 * @author Lakin Wecker
 *
 *
 */
class ParaGuiApplication : public Application

{

    //======================================================================
    // Private Member Variables
    //======================================================================
    private:
    /**
     * The actuall ParaGui Application which this class wraps.
     */
    PG_Application myApplication;

    /**
     * The width of the Application
     */
    int myWidth;
    
    /**
     * The height of the Application
     */
    int myHeight;
    

    //======================================================================
    // Public Methods
    //======================================================================
    public:


    //======================================================================
    // Ctor
    //======================================================================
    /**
     * Creates a new ParaGuiApplication using default values.
     */
    ParaGuiApplication(int w, int h);

    //----------------------------------------------------------------------
    // Destructor
    
    /**
     * Deletes a ParaGuiApplication instance.
     */
    virtual ~ParaGuiApplication();
    

    //----------------------------------------------------------------------
    // API

    /**
     * This function takes no arguments and returns when the application has been closed.
     */
    virtual void run();
    
    /**
     * This function reads only the current event queue and processes them.
     *
     * @return True if the application is still running and false if it has been closed.
     */
    virtual bool eventLoop();
    
    /**
     * This function takes one SDL_event and processes it.
     *
     * @param event the SDL_event struct which contains the event.
     *
     * @return false if application should quit
     */
    virtual bool handleEvent(SDL_Event *event);
    
    /**
     * Add the rootContainer to the ParaGuiApplication.
     *
     * @param container The container which well serve as the rootContainer for the GUI
     *
     */
    virtual void setRootContainert(Container *container);

    /**
     *  Returns the root Container so widgets can be added to it.
     *
     * @return the root container
     */
   virtual  Container *getRootContainer();
    

}; // End of class
} // End of application namespace

#endif


