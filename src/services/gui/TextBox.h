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

#ifndef TEXTBOX_H
#define TEXTBOX_H

// Included headers from the current project
#include "Widget.h"
#include "services/font/FontService.h"
#include "services/font/FontRenderer.h"


// Included custom library headers
// (1) Include the signal system in headers file.
#include <sigc++/signal_system.h>

// Included system headers


namespace dime {

/**
 * The editbox interface
 *
 * More detailed description of the class, it's purpose, what it does,
 * how to use it and so on.
 *
 * A short piece of example code demonstarting how this class it is used,
 * and in what context, is encouraged.
 *
 * @author Martin Pollard (xmp)
 *
 * @see Widget
 *
 */
class TextBox : public Widget

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
    //======================================================================
    private:

    // NOTE: Class variables are prefixed with "my", static variables are
    //       prefixed with "the".

    /**
     * NOTE: This is the text stored by the static widget
     */
    std::string  myText;
    FontRenderer *myFontRenderer;

    //======================================================================
    // Public Methods
    //======================================================================
    public:

    //----------------------------------------------------------------------
    // Constructors

    /**
     * Cretaes a new TextBox using default values.
     */
	TextBox() : Widget()
    {
    }


    /**
     * Copy constructor.
     */
    TextBox( const TextBox &source ) : myFontRenderer(source.myFontRenderer)
    {
        // Use assignment operator to do the copy
        // NOTE: If you need to do custom initialization in the constructor this may not be enough.
        *this = source;
    }


    /**
     * Assignment operator.
     */
    TextBox &operator= ( const TextBox &source )
    {
        // Copy fields from source class to this class here.

        // Return this object with new value
        return *this;
    }


    //----------------------------------------------------------------------
    // Destructor

    /**
     * Deletes a TextBox instance.
     */
    virtual ~TextBox()
    {
		if (myParent != NULL) myParent->removeWidget(this);

		delete myFontRenderer;
        // TODO: Free any allocated resources here.
    }


    //----------------------------------------------------------------------
    // Getters

    /**
     * Returns the text of the Label
     */
    virtual std::string getText() const
    {
        return myText;
    }


    //----------------------------------------------------------------------
    // Setters

    /**
     * Sets the text of this label widget
	 *
	 * @param text The new text to go in label
     */
    virtual void setText( std::string text ) {
		myText = text;		
	}

	/**
	 * Sets the FontRenderer used to draw this Widget
	 *
	 * @param font The new font renderer
	 */
	virtual void setFont( FontRenderer *font ) {
		myFontRenderer = font;
	}


    //----------------------------------------------------------------------
    // Other public methods

	/**
	 * Draws the widget, and/or its children.
	 */
    virtual int draw(DrawDevice *target);


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


}; // End of class

} // End of application namespace

#endif


