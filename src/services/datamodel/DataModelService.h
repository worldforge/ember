/*
    Copyright (C) 2002  Tim Enderling

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

#ifndef DATAMODELSERVICE_H
#define DATAMODELSERVICE_H

// Include other headers of the current program here
#include "FolderProvider.h"
#include <framework/Service.h>

// Include library headers here

#include <string>
#include <list>
#include <sstream>
#include <stdio.h> 

// Include system headers here

namespace dime {

//======================================================================
// Short type macros
//======================================================================

/**
 * Service managing the data model. Retrieves DataObjects by their paths.
 *
 * See http://www.worldforge.org/dev/eng/clients/dime/developer_doc/DataModelFW
 * for more info on the data model.
 * 
 * @author Tim Enderling
 */

class DataModelService //: public Service
{
    //======================================================================
    // Public Constants and enums
    //======================================================================
    public:	
	

	
	//======================================================================
    // Inner Classes and typedefs
    //======================================================================	 
	public:

    //======================================================================
    // Private Constants
    //======================================================================
    private:

    //======================================================================
    // Private Variables
    //======================================================================
    private:
	FolderProvider myRootProvider;

    //======================================================================
    // Public Methods
    //======================================================================
    public:

    /**
     * Functions for Data Model Providers
     */
    FolderProvider * getRootProvider()
	{
		return &myRootProvider;
	}

	/**
	 * Helper functions
	 */

	static void dump(PDataObject toDump, std::stringstream & dumpDest, bool recursive = true,
																	int level = 0);

	//----------------------------------------------------------------------
    // Singleton

    /**
     * Returns the DataModelService instance.
     */
    static DataModelService *getInstance()
    {
		static DataModelService singleinstance;
		return &singleinstance;
    }

	//----------------------------------------------------------------------
    // Constructors

	DataModelService()
	{
		myRootProvider.myPath = "/";
		myRootProvider.mySubpath = "";
	}

    //----------------------------------------------------------------------
    // Destructor

    //----------------------------------------------------------------------
    // Getters

    //----------------------------------------------------------------------
    // Setters

    //----------------------------------------------------------------------
    // Other public methods



    //======================================================================
    // Protected Methods
    //======================================================================
    protected:


    //======================================================================
    // Private Methods
    //======================================================================
    private:
							

}; // DataModelService

} // namespace dime

#endif
