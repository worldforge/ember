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

#ifndef TEXTUREBACKGROUND_H
#define TEXTUREBACKGROUND_H

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

class TextureBackground
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

        std::string myTexturePath;
        Color myColor;
        std::string myStyle;

    //======================================================================
    // Public Methods
    //======================================================================
    public:

    //----------------------------------------------------------------------
    // Constructors

    /**
     * Creates a new TextureBackground using default values.
     */
     TextureBackground()
     {
     }

    /**
     * Copy constructor.
     */
    TextureBackground( const TextureBackground &source )
    {
        // Use assignment operator to do the copy
        // NOTE: If you need to do custom initialization in the constructor this may not be enough.
        *this = source;
    }


    /**
     * Assignment operator.
     */
    TextureBackground &operator= ( const TextureBackground &source )
    {
        // Copy fields from source class to this class here.

        // Return this object with new value
        return *this;
    }


    //----------------------------------------------------------------------
    // Destructor

    /**
     * Deletes a TextureBackground instance.
     */
    virtual ~TextureBackground ()
    {
        // TODO: Free any allocated resources here.
    }


    //----------------------------------------------------------------------
    // Getters

    // Example of a getter method:

    /**
     * Gets the value of TexturePath of this TextureBackground
     */
    std::string getTexturePath() const
    {
        return myTexturePath;
    }

    /**
     * Gets the value of Color of this TextureBackground
     */
    Color getColor() const
    {
        return myColor;
    }

    /**
     * Gets the value of Style of this TextureBackground
     */
    std::string getStyle() const
    {
        return myStyle;
    }


    //----------------------------------------------------------------------
    // Setters

    /**
     * Sets the value of TexturePath of this TextureBackground
     */
    void setTexturePath( std::string texturePath )
    {
        myTexturePath = texturePath ;
    }

    /**
     * Sets the value of Color of this TextureBackground
     */
    void setColor( Color color )
    {
        myColor = color ;
    }

    /**
     * Sets the value of Style of this TextureBackground
     */
    void setStyle( std::string style )
    {
        myStyle = style ;
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


}; // End of TextureBackground

} // End of dime namespace

#endif
