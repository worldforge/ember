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

#ifndef LABEL_H
#define LABEL_H

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
 * The Label interface.
 *
 * More detailed description of the class, it's purpose, what it does,
 * how to use it and so on.
 *
 * A short piece of example code demonstarting how this class it is used,
 * and in what context, is encouraged.
 *
 * @author Xmp (Martin Pollard)
 *
 * @see Widget
 *
 */
class Label : public Widget
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
    // Protected Variables
    //======================================================================
    protected:

	FontRenderer *myFontRenderer;
    
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
     * Creates a new Label using default values.
     */
    Label() 
        : Widget()
    {
#ifdef _MSC_VER
      dime::Font *font = dime::FontService::getInstance()->loadFont("..\\bin\\nasal.ttf",16);
#else
      dime::Font *font = dime::FontService::getInstance()->loadFont("../../bin/nasal.ttf",16);
#endif 
      assert(font);
      myFontRenderer = new FontRenderer(FontRenderer::BLENDED, Font::FontString(), font, Color(100,100,0), Rectangle(0,0,0,0));
    }
    
    /**
     * Creates a new Label using default values.
     */
    Label(const Rectangle& rect) 
        : Widget(rect)
    {
#ifdef _MSC_VER
      dime::Font *font = dime::FontService::getInstance()->loadFont("..\\bin\\nasal.ttf",16);
#else
      dime::Font *font = dime::FontService::getInstance()->loadFont("../../bin/nasal.ttf",16);
#endif 
      assert(font);
      myFontRenderer = new FontRenderer(FontRenderer::BLENDED, Font::FontString(), font, Color(0,0,255), rect);
    }

    
	/**
     * Creates a new Label using rect and text.
     */
    Label(const Font::FontString text, const Rectangle& rect) 
        : Widget(rect)
    {
#ifdef _MSC_VER
      dime::Font *font = dime::FontService::getInstance()->loadFont("..\\bin\\nasal.ttf",16);
#else
      dime::Font *font = dime::FontService::getInstance()->loadFont("../../bin/nasal.ttf",16);
#endif 
      assert(font);
      myFontRenderer = new FontRenderer(FontRenderer::BLENDED, text, font, Color(0,0,255), rect);
    }
	/**
     * Creates a new Label using rect and text.
     */
    Label(const std::string text, const Rectangle& rect) 
        : Widget(rect)
    {
#ifdef _MSC_VER
      dime::Font *font = dime::FontService::getInstance()->loadFont("..\\bin\\nasal.ttf",16);
#else
      dime::Font *font = dime::FontService::getInstance()->loadFont("../../bin/nasal.ttf",16);
#endif 
      assert(font);
      Font::FontString fontString;
      for(unsigned int i = 0; i < text.length(); ++i)
          {
              fontString+=text[i];
          }
      
      myFontRenderer = new FontRenderer(FontRenderer::BLENDED, fontString, font, Color(0,0,255), rect);
    }
    /**
     * Copy constructor.
     */
    Label( const Label &source )
    {
        // Use assignment operator to do the copy
        // NOTE: If you need to do custom initialization in the constructor this may not be enough.
        *this = source;
    }


    /**
     * Assignment operator.
     */
    Label &operator= ( const Label &source )
    {
        // Copy fields from source class to this class here.
		myFontRenderer = new FontRenderer(*source.myFontRenderer);

        // Return this object with new value
        return *this;
    }


    //----------------------------------------------------------------------
    // Destructor

    /**
     * Deletes a Label instance.
     */
    virtual ~Label()
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
    virtual Font::FontString getText() const
    {
        return myFontRenderer->getText();
    }


    //----------------------------------------------------------------------
    // Setters

    /**
     * Sets the text of this label widget
	 *
	 * @param text The new text to go in label
     */
    virtual void setText( const Font::FontString& text ) {
		myFontRenderer->setText(text);
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


