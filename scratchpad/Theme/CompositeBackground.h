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

#ifndef COMPOSITEBACKGROUND_H
#define COMPOSITEBACKGROUND_H

// Included headers from the current project

// Included custom library headers

// Included system headers

namespace dime {

/**
 * Short sentence with a general description of the class, ending in period.
 *
 * More detailed description of the class, it's purpose, what it does,
 * how to use it and so on.
 *
 * A short piece of example code demonstarting how this class it is used,
 * and in what context, is encouraged.
 *
 * @author Lakin Wecker aka nikal
 *
 * NOTE: You can also specify the author for individual methods
 * if different persons have created them.
 * It is also possible to have multiple @author tags for a method.
 * Only add yourself as an @author if you have done serious work
 * on a class/method, and can help fixing bugs in it, etc.
 * If you just fixed a bug or added a short code snipplet you
 * don't need to add yourself.
 *
 * @see OtherSubsystem::AnOtherRelatedClass
 *
 * NOTE: Add other related classes here, doxygen will create links to them.
 */

class CompositeBackground
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

        int myCols;
        int myRows;
        std::list<BackgroundStyle> myMyBackgroundCells;

    //======================================================================
    // Public Methods
    //======================================================================
    public:

    //----------------------------------------------------------------------
    // Constructors

    /**
     * Creates a new CompositeBackground using default values.
     */
     CompositeBackground()
     {
     }

    /**
     * Copy constructor.
     */
    CompositeBackground( const CompositeBackground &source )
    {
        // Use assignment operator to do the copy
        // NOTE: If you need to do custom initialization in the constructor this may not be enough.
        *this = source;
    }


    /**
     * Assignment operator.
     */
    CompositeBackground &operator= ( const CompositeBackground &source )
    {
        // Copy fields from source class to this class here.

        // Return this object with new value
        return *this;
    }


    //----------------------------------------------------------------------
    // Destructor

    /**
     * Deletes a CompositeBackground instance.
     */
    virtual ~CompositeBackground ()
    {
        // TODO: Free any allocated resources here.
    }


    //----------------------------------------------------------------------
    // Getters

    // Example of a getter method:

    /**
     * Gets the value of Cols of this CompositeBackground
     */
    int getCols() const
    {
        return myCols;
    }

    /**
     * Gets the value of Rows of this CompositeBackground
     */
    int getRows() const
    {
        return myRows;
    }

    /**
     * Gets the value of MyBackgroundCells of this CompositeBackground
     */
    std::list<BackgroundStyle> getMyBackgroundCells() const
    {
        return myMyBackgroundCells;
    }


    //----------------------------------------------------------------------
    // Setters

    /**
     * Sets the value of Cols of this CompositeBackground
     */
    void setCols( int cols )
    {
        myCols = cols ;
    }

    /**
     * Sets the value of Rows of this CompositeBackground
     */
    void setRows( int rows )
    {
        myRows = rows ;
    }

    /**
     * Sets the value of MyBackgroundCells of this CompositeBackground
     */
    void setMyBackgroundCells( std::list<BackgroundStyle> myBackgroundCells )
    {
        myMyBackgroundCells = myBackgroundCells ;
    }


    //----------------------------------------------------------------------
    // Other public methods
    // NOTE: Group related public methods together and crate a separator comment like above for them.
    
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


}; // End of CompositeBackground

} // End of dime namespace

#endif
