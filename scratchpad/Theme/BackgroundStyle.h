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

#ifndef BACKGROUNDSTYLE_H
#define BACKGROUNDSTYLE_H

// Included headers from the current project
#include <services/platform/RectangleRenderer.h>
// Included custom library headers

// Included system headers

namespace dime {
namespace theme {
	
/**
 * Abstract class for BackgroundStyles which provides the "createRR" interface.
 *
 * Provides the RectangleRenderer *createRectangleRenderer(Rectangle &rect) interface.
 * how to use it and so on.
 *  
 *
 * @author Lakin Wecker aka nikal
 *
 */

class BackgroundStyle
{


    //======================================================================
    // Public Methods
    //======================================================================
    public:

    //----------------------------------------------------------------------
    // Destructor

    /**
     * Deletes a BackgroundStyle instance.
     */
    virtual ~BackgroundStyle ()
    {
        // TODO: Free any allocated resources here.
    }


    //----------------------------------------------------------------------
    // Other public methods
   
    //----------------------------------------------------------------------
    // The only interface this object requires
    virtual RectangleRenderer *createRenderer(Rectangle &rect);
    

}; // End of BackgroundStyle

} // End of theme namespace
} // End of dime namespace

#endif
