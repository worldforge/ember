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

#ifndef PARAGUISERVICE_H
#define PARAGUISERVICE_H

// Included headers from the current project
#include "framework/Service.h"
#include "GuiService.h"
#include "ParaGuiApplication.h"

// Included custom library headers

// Included system headers
#include <string>
#include <paragui/pgapplication.h>

namespace dime {

/**
 * This service provides a GUI widget system.
 * It can create various basic GUI Widgets, and connect them to listeners / data sources (of Data Model type?).
 * It also holds the root window.
 *
 *
 * @author Hans Häggström
 */
class ParaGuiService : public GuiService

{
    //======================================================================
    // Private Variables
    //======================================================================
   private:
    
    

    //======================================================================
    // Public Methods
    //======================================================================
    public:

    //----------------------------------------------------------------------
    // Constructors

    /**
     * Creates a new GuiService using default values.
     */
    ParaGuiService()
    {
        setName( "ParaGuiService" );
        setDescription( "Provides a GUI widget wrapper for ParaGUI." );
    }


    //----------------------------------------------------------------------
    // Destructor

    /**
     * Deletes a GuiService instance.
     */
    virtual ~ParaGuiService()
    {
        // TODO: Free any allocated resources here.
    }


    //----------------------------------------------------------------------
    // Getters



    //----------------------------------------------------------------------
    // Setters



    //----------------------------------------------------------------------
    // Other public methods

	
    //----------------------------------------------------------------------
    // Methods inherited from Service
    /**
     * This method is used to start the service.
     * It takes care of aquiring needed resources, initializing
     * data structures, and so on. <p>
     *
     * If initialization fails, it should set appropriate status code and
     * status text.  It could also write an entry into a log through the logging
     * service.  <p>
     *
     * @returns success or error code
     */
    virtual int start()
    {
        setRunning( true );
        
        return 0;
    }
    /**
     * This method stops the service, and frees any used resources.
     *
     * @ param code code which represents the cause of the service halt
     */
    virtual void stop( int code )
    {
        setRunning( false );
    }
    
    /**
     * This creates an application window based on the ParaGUI Application
     *
     * @ param width the desired width of the Application
     * @ param height the desired height of the Application
     */
    virtual Application *createApplication(int width, int height);

    //======================================================================
    // Protected Methods
    //======================================================================
    protected:


    //======================================================================
    // Private Methods
    //======================================================================
    private:


    //======================================================================
    // Disabled constructors and operators
    //======================================================================
    private:

    /**
     * Copy constructor not provided.
     */
    ParaGuiService( const GuiService &source )
    {
    }


    /**
     * Assignment operator not provided.
     */
    ParaGuiService &operator= ( const GuiService &source )
    {
        return *this;
    }


}; // End of class

} // End of application namespace

#endif



