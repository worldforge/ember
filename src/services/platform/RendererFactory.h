/*
 Copyright (C) 2002  Martin Pollard (Xmp)
	
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

#ifndef RENDERERFACTORY_H
#define RENDERERFACTORY_H

// Included headers from the current project
#include "BitmapRenderer.h"
#include "ColorRenderer.h"
#include "GradientRenderer.h"
#include "Rectangle.h"

// Included custom library headers
// LibXML2
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

// Included system headers
#include <string>

namespace dime {

/**
 * A factory class for fetching renderers stored in XML and returning them.
 *
 * More detailed description of the class, it's purpose, what it does,
 * how to use it and so on.
 *
 * A short piece of example code demonstarting how this class it is used,
 * and in what context, is encouraged.
 *
 * @author Martin Pollard aka Xmp
 *
 * NOTE: You can also specify the author for individual methods
 * if different persons have created them.
 * It is also possible to have multiple @author tags for a method.
 * Only add yourself as an @author if you have done serious work
 * on a class/method, and can help fixing bugs in it, etc.
 * If you just fixed a bug or added a short code snipplet you
 * don't need to add yourself.
 *
 * @see dime::Renderer
 *
 *
 */

class RendererFactory
{
    //======================================================================
    // Inner Classes, Typedefs, and Enums
    //======================================================================
    public:


    //======================================================================
    // Public Constants
    //======================================================================
    public:


    //======================================================================
    // Private Constants
    //======================================================================
    private:


    //======================================================================
    // Private Variables
    //======================================================================/
    private:
    static RendererFactory* theInstance;


    //======================================================================
    // Public Methods
    //======================================================================
    public:

    //----------------------------------------------------------------------
    // Constructors

    /**
     * Creates a new RendererFactory using default values.
     */
     RendererFactory()
     {
     }

    /**
     * Copy constructor.
     */
    RendererFactory( const RendererFactory &source )
    {
        // Use assignment operator to do the copy
        // NOTE: If you need to do custom initialization in the constructor this may not be enough.
        *this = source;
    }


    /**
     * Assignment operator.
     */
    RendererFactory &operator= ( const RendererFactory &source )
    {
        // Copy fields from source class to this class here.

        // Return this object with new value
        return *this;
    }


    //----------------------------------------------------------------------
    // Destructor

    /**
     * Deletes a RendererFactory instance.
     */
    virtual ~RendererFactory ()
    {
        // TODO: Free any allocated resources here.
    }


    //----------------------------------------------------------------------
    // Getters

    /**
     * Returns a renderer with specified name and rect.
     */
    Renderer* getRenderer( const std::string& name,
			   const Rectangle& rect,
			   xmlNodePtr stateNode,
			   xmlDocPtr doc );

    /**
     * Returns an instance of the factory
     */
    static dime::RendererFactory *getInstance()
    {
        if (theInstance == NULL)
	  theInstance = new dime::RendererFactory();

	return theInstance;
    }

    //----------------------------------------------------------------------
    // Setters


    //----------------------------------------------------------------------
    // Other public methods
    
    //======================================================================
    // Protected Methods
    //======================================================================
    protected:

    /**
     * Returns the renderer obtained from parsing rendererNode
     */
    Renderer* RendererFactory::parseRenderer( const Rectangle& rect,
					      xmlNodePtr rendererNode,
					      xmlDocPtr doc );

    //======================================================================
    // Private Methods
    //======================================================================
    private:


    //======================================================================
    // Disabled constructors and operators
    //======================================================================
    private:


}; // End of RendererFactory

} // End of dime namespace

#endif
