// This is a template that describes a layout for header files.

// Additional guidelines:
//
// - The indent is four spaces
//
// - Opening braces on new lines
//
// - if's, while's etc always have a block with opening and closing brace,
//   even if they just consist of one line.
//
// - Classes are always Capitalized for each word,
//   methods start with lower caps for first word,
//   upper caps for first letter of subsequent words.
//
// - Constants in ALL_CAPS with underscore between words.
//
// - No public variables are allowed, getter and setter methods are used instead.
//
// - Use descriptive names for classes, methods, and variables, avoid acronyms.
//   See Ottinger's naming rules for more info:
//   http://www.objectmentor.com/publications/naming.htm
//
// - Leave the different separator comments of the template header
//   (such as Protected Methods, Public Constants, etc) in place even if you don't
//   use them initially.  This way there is a clear place where to put new methods,
//   variables, and constants of different types, and the code stays clearer.
//
//
//   TODO: What standard should be used for todo's, notes, and such?
//   One possibility would be TODO( user, date, importance ): <message>
//   Here different arguments can be left out, and if there's no arguments
//   the parenthesis can also be left out.  Examples:
//
//     TODO: Fix the water boiler!
//     TODO( zzorn ): Shouldn't we put tea in this container?
//     TODO( brenda, 2002-01-20 ): Added a temporary kludge to start the coffee machine.
//     TODO( zzorn, 2002-01-20, optional ): It would be nice to support coffee at some time too.
//     TODO( brenda, required ): We absolutely need to add support for coffee here!
//
//   Are user, date, and importance parameters enough?  Are there other important things,
//   that might be useful to be able to automatically process in some way?
//
//   "NOTE:" -is used similarily to TODO's, but is used to bring some detail
//   to programmers attention that may be unintuitive or easy to miss otherwise,
//   or some other meta-comment about the program (?).
//
//   Some other syntax is used in some other parts of worldforge
//   (something to do with % -characters, IIRC).   We could examine if this is suitable for us.
//
//   "Fixme" is another todo type tag used sometimes.
//
//   Whatever syntax we settle for, it would be nice to have scripts that can
//   extract the todo notes from the code, order them according to importance,
//   or some other parameter, and include them on a web page, with links
//   back to the original code (in bonsai or some other web cvs system?
//   Or at least file and line number otherwise).
//

//  ----------------------< Start of template >---------------------------
/*
    Copyright (C) 2002  <name of author>

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
    // Private Variables
    //======================================================================
    private:

    /**
     * NOTE: This is the text stored by the static widget
     */
    std::string  myText;
	//FontRenderer myFont;


    //======================================================================
    // Public Methods
    //======================================================================
    public:

    //----------------------------------------------------------------------
    // Constructors

    /**
     * Creates a new Label using default values.
     */
	Label() : Widget()
    {
    }

    /**
     * Creates a new Label using rect and text.
     */
    Label(std::string text, Rectangle rect) : Widget(rect), myText(text)
    {

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
		myText = source.myText;

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
       // TODO: Free any allocated resources here.
    }


    //----------------------------------------------------------------------
    // Getters

    // Example of a getter method:

    /**
     * Returns the text of the Label
     */
    virtual std::string getText() const
    {
        return myText;
    }


    //----------------------------------------------------------------------
    // Setters

    // Example of a setter method:

    /**
     * Sets the name of this gizmo.
     * If there already is a gizmo with this name, a number will be
     * appended to the name, and the naming will be attempted again.
     *
     * @param name The new name of the gizmo.
     */
    virtual void setText( std::string text ) {
		myText = text;		
	}

	/**
	 * Sets the font used to draw this Widget
	 */
	virtual void setFont( /*const FontRenderer &font*/ ) {
		/*
		myFont = font;
		*/
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


