/*
 Copyright (C) 2002  Adam Gregory
	
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

#ifndef RRFACTORY_H
#define RRFACTORY_H

// Included headers from the current project
#include "src/services/platform/RectangleRenderer.h"
#include "src/services/platform/GridRecRenderer.h"
#include "src/services/platform/GradientRecRenderer.h"
#include "src/services/platform/BmpRecRenderer.h"
#include "src/services/platform/ColourRecRenderer.h"

// Included custom library headers

// Included system headers

namespace dime {

/**
 * Factory functions to create RectangleRenderer objects of the correct type.
 *
 * @author Adam Gregory aka Adamgreg
 *
 * @see RectangleRenderer
 *
 * @todo Replace class with a namespace since there's no member data?
 */

class RRFactory
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


    //======================================================================
    // Public Methods
    //======================================================================
    public:

    //----------------------------------------------------------------------
    // Constructors

    /**
     * Creates a new RRFactory using default values.
     */
     RRFactory()
     {
     }

    /**
     * Copy constructor.
     */
    RRFactory( const RRFactory &source )
    {
        // Use assignment operator to do the copy
        // NOTE: If you need to do custom initialization in the constructor this may not be enough.
        *this = source;
    }


    /**
     * Assignment operator.
     */
    RRFactory &operator= ( const RRFactory &source )
    {
        // Copy fields from source class to this class here.

        // Return this object with new value
        return *this;
    }


    //----------------------------------------------------------------------
    // Destructor

    /**
     * Deletes a RRFactory instance.
     */
    virtual ~RRFactory ()
    {
        // TODO: Free any allocated resources here.
    }


    //----------------------------------------------------------------------
    // Getters

    // Example of a getter method:

    /**
     * Gets the value of CreateRR of this RRFactory
     */
    RectangleRenderer& getCreateRR() const
    {
        return myCreateRR;
    }


    //----------------------------------------------------------------------
    // Setters

    /**
     * Sets the value of CreateRR of this RRFactory.
     */
    void setCreateRR( RectangleRenderer& createRR )
    {
        myCreateRR = createRR ;
    }


    //----------------------------------------------------------------------
    // Other public methods
	
	/**
	 * Creates a new ColourRecRenderer with the values supplied.
	 */
	RectangleRenderer& myCreateRR(const Rectangle &rect, Uint8 red, Uint8 green, Uint8 blue)
	{
		return ColourRecRenderer(rect, red, green, blue);
	}
	
	/**
	 * Creates a new ColourRecRenderer with the values supplied.
	 */
	RectangleRenderer& myCreateRR(const Rectangle &rect, const Color &color)
	{
		return ColourRecRenderer(rect, color);
	}

	/**
	 * Creates a new BmpRecRenderer with the values supplied.
	 */
	RectangleRenderer& myCreateRR(const Rectangle &rect, std::string filename, BitmapStyle style)
	{
		return BmpRecRenderer(rect, filename, style);
	}

	/**
	 * Creates a new GradientRecRenderer with the values supplied.
	 */
	RectangleRenderer& myCreateRR(const Rectangle &rect, Color topleft, Color topright,
		Color bottomleft, Color bottomright)
	{
		return GradientRecRenderer(rect, topleft, topright, bottomleft, bottomright);
	}

	/**
	 * Creates a new GradientRecRenderer with the values supplied.
	 */
	RectangleRenderer& myCreateRR(const Rectangle &rect, Color color1, Color color2, GradientType type)
	{
		return GradientRecRenderer(rect, color1, color2, type);
	}

	/**
	 * Creates a new GridRecRenderer with the values supplied.
	 */
	RectangleRenderer& myCreateRR(const Rectangle &rect, std::vector<RectangleRenderer>
		&gridVector, int columns, int rows, std::vector<int> &lines)
	{
		return GridRecRenderer(rect, gridVector, columns, rows, lines);
	}
	
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


}; // End of RRFactory

} // End of dime namespace

#endif
