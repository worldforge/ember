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
#include "services/platform/BitmapRenderer.h"
#include "services/platform/ColorRenderer.h"
#include "services/platform/BorderRenderer.h"


// Included custom library headers
// (1) Include the signal system in headers file.
#if SIGC_MAJOR_VERSION == 1 && SIGC_MINOR_VERSION == 0
#include <sigc++/signal_system.h>
#else
#include <sigc++/signal.h>
#include <sigc++/object.h>
#endif

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
    /*
     * Rectangle renderer used to draw what's behind the the text
     */
    RectangleRenderer *myBackRect;
	
	/**
	 * Border Renderer used to draw a border around this text box
	 */
	 BorderRenderer *myBorder;

    /*
     * Position of Caret in textbox
     */
    unsigned int myCaretPos;

    /*
     * The text in our window visible or not
     */
    Font::FontString myText;

    /*
     * Position of left hand char
     */
    unsigned int myLeft;

    /*
     * Position of right hand char
     */
    unsigned int myRight;

    //======================================================================
    // Public Methods
    //======================================================================
    public:

    //----------------------------------------------------------------------
    // Constructors

    /**
     * Creates a new TextBox using default values.
     */
    TextBox() : Label(), myCaretPos(0), myLeft(0), myRight(0)
    {
    }

	TextBox(Font::FontString text, const Rectangle& rect) : Label(text, Rectangle(rect.getX()+11, rect.getY()+11, rect.getWidth()-11,rect.getHeight()-11)),
	  myCaretPos(text.length()),
	  myText(text),
	  myRight(text.length()-1)
    {
	  myBackRect = new ColorRenderer(rect,255,255,255);
	  myBorder = new BorderRenderer(rect, 2, new ColorRenderer(rect, 125, 125, 255));
      rebuildRight();
    }

    TextBox(std::string text, const Rectangle& rect) :
        Label(text, Rectangle(rect.getX()+11, rect.getY()+11, rect.getWidth()-11,rect.getHeight()-11)),
      myCaretPos(text.length()),
      myText(text.begin(), text.end()),
      myRight(text.length()-1)
    {
	  myBackRect = new ColorRenderer(rect,255,255,255);
	  myBorder = new BorderRenderer(rect, 2, new ColorRenderer(rect, 125, 125, 255));
      Font::FontString temp;
/*	
      for(const char* c = text.c_str();*c != '\0';c++)
	{
	  temp+=(Uint16)c;
	}
      assert(text.length()==temp.length());

      myText = temp;
      */
      rebuildRight();
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

        // Update text displayed in box 
	rebuildRight();
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
    /**
     * Connect a slot here to observe when enter is pressed.
     */
    SigC::Signal1<void, const Font::FontString&, SigC::Marshal<void> > onEnter;
 

    //======================================================================
    // Protected Methods
    //======================================================================
    protected:

    /*
     * Calculates where the caret should be for us and draws it
     */
    void drawCaret(DrawDevice* target);

    /*
     * Rebuild chars in textbox from left hand direction
     */
    void rebuildLeft();

    /*
     * Rebuild chars in textbox from right hand direction
     */
    void rebuildRight();

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


