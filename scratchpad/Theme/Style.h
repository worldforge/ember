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

#ifndef STYLE_H
#define STYLE_H

// Included headers from the current project

// Included custom library headers

// Included system headers

namespace dime {
namespace theme {
	
/**
 * Holds the information about a theme style.
 * 
 * This is an internally used class and shouldn't 
 * be used outside of the ThemeingService, instead
 * use a WidgetStyle which will provide you with Renderers.
 *
 * @author Lakin Wecker
 *
 * @see dime::Widgetstyle
 * @see dime::ThemeService
 * @see dime::Renderer
 *
 * NOTE: Add other related classes here, doxygen will create links to them.
 */

class Style
{


    //======================================================================
    // Private Variables
    //======================================================================/
    private:

        dime::BackgroundStyle myBackgroundStyle;
        dime::TextStyle myTextStyle;
        dime::Theme::Style myParentStyle;
        dime::IconStyle myIconStyle;
        std::string myStyleID;
        dime::Color myForeColor;

    //======================================================================
    // Public Methods
    //======================================================================
    public:

    //----------------------------------------------------------------------
    // Constructors

    /**
     * Creates a new Style using default values.
     */
     Style()
     {
     }

    /**
     * Copy constructor.
     */
    Style( const Style &source )
    {
        // Use assignment operator to do the copy
        // NOTE: If you need to do custom initialization in the constructor this may not be enough.
        *this = source;
    }


    /**
     * Assignment operator.
     */
    Style &operator= ( const Style &source )
    {
        // Copy fields from source class to this class here.
	myBackgroundStyle = source.getBackgroundStyle();
	myTextStyle = source.getTextStyle();
	myParentStyle = source.getParentStyle();
	myIconStyle = source.getIconStyle();
	myStyleID = source.getStyleID();
	myForeColor = source.getForeColor();
        // Return this object with new value
        return *this;
    }


    //----------------------------------------------------------------------
    // Destructor

    /**
     * Deletes a Style instance.
     */
    virtual ~Style ()
    {
        // TODO: Free any allocated resources here.
    }


    //----------------------------------------------------------------------
    // Getters

    // Example of a getter method:

    /**
     * Gets the value of BackgroundStyle of this Style
     */
    dime::BackgroundStyle getBackgroundStyle() const
    {
        return myBackgroundStyle;
    }

    /**
     * Gets the value of TextStyle of this Style
     */
    dime::TextStyle getTextStyle() const
    {
        return myTextStyle;
    }

    /**
     * Gets the value of ParentStyle of this Style
     */
    dime::Theme::Style getParentStyle() const
    {
        return myParentStyle;
    }

    /**
     * Gets the value of IconStyle of this Style
     */
    dime::IconStyle getIconStyle() const
    {
        return myIconStyle;
    }

    /**
     * Gets the value of StyleID of this Style
     */
    std::string getStyleID() const
    {
        return myStyleID;
    }

    /**
     * Gets the value of ForeColor of this Style
     */
    dime::Color getForeColor() const
    {
        return myForeColor;
    }


    //----------------------------------------------------------------------
    // Setters

    /**
     * Sets the value of BackgroundStyle of this Style
     */
    void setBackgroundStyle( dime::BackgroundStyle backgroundStyle )
    {
        myBackgroundStyle = backgroundStyle ;
    }

    /**
     * Sets the value of TextStyle of this Style
     */
    void setTextStyle( dime::TextStyle textStyle )
    {
        myTextStyle = textStyle ;
    }

    /**
     * Sets the value of ParentStyle of this Style
     */
    void setParentStyle( dime::Theme::Style parentStyle )
    {
        myParentStyle = parentStyle ;
    }

    /**
     * Sets the value of IconStyle of this Style
     */
    void setIconStyle( dime::IconStyle iconStyle )
    {
        myIconStyle = iconStyle ;
    }

    /**
     * Sets the value of StyleID of this Style
     */
    void setStyleID( std::string styleID )
    {
        myStyleID = styleID ;
    }

    /**
     * Sets the value of ForeColor of this Style
     */
    void setForeColor( dime::Color foreColor )
    {
        myForeColor = foreColor ;
    }




}; // End of Style

} // End of theme namespace
} // End of dime namespace

#endif
