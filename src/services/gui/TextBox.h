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
#include "Label.h"
#include "services/font/FontService.h"
#include "services/font/FontRenderer.h"
#include "services/platform/RectangleRenderer.h"


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
class TextBox : public Label

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
	RectangleRenderer myBackRect;
    
	//======================================================================
    // Public Methods
    //======================================================================
    public:

    //----------------------------------------------------------------------
    // Constructors

    /**
     * Cretaes a new TextBox using default values.
     */
	TextBox() : Label()
    {
    }

	TextBox(Font::FontString text, const Rectangle& rect) : Label(text, Rectangle(rect.getX()+11, rect.getY()+11, rect.getWidth()-11,rect.getHeight()-11)),
                                                            myBackRect(rect,"textboxback.png", RectangleRenderer::STRETCH)
    {
    }

    TextBox(std::string text, const Rectangle& rect) :
        Label(text, Rectangle(rect.getX()+11, rect.getY()+11, rect.getWidth()-11,rect.getHeight()-11)),
        myBackRect(rect,"textboxback.png", RectangleRenderer::STRETCH)
        
    {
    }
    /**
     * Copy constructor.
     */
    TextBox( const TextBox &source )
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
        // TODO: Free any allocated resources here.
    }


    //----------------------------------------------------------------------
    // Getters


    //----------------------------------------------------------------------
    // Setters

    //----------------------------------------------------------------------
    // Other public methods
    /**
     * Draws the widget, and/or its children.
     */
    virtual int draw(DrawDevice *target);
    /**
     * Takes keyPresses and feeds them into the textbuffer.
     */
    virtual bool keyPress( KeyPressEvent *event);

    virtual void mouseUp( MouseButtonEvent* event) {
        Widget::onMouseUp(event);
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


}; // End of class

} // End of application namespace

#endif


