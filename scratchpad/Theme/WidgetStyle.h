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

#ifndef WIDGETSTYLE_H
#define WIDGETSTYLE_H

// Included headers from the current project

// Included custom library headers

// Included system headers

namespace dime {

/**
 * Used by Widgets to create Renderers to render themselves.
 *
 * Contains the information about WidgetStates and their appropriate  
 * Styles, and can create Renderers for these States for the widget.
 *
 * dime::WidgetStyle *myWidgetStyle = ThemeService::getWidgetStyle(
 *                                                       myWidgetID,
 *                                                       myOptionalThemeID
 *                                                                );
 * dime::FontRenderer myFontRenderer = myWidgetStyle->createFontRenderer(
 *                                                              widgetState,
 *                                                              myRectangle
 *                                                                      );
 * dime::BackgroundRenderer myBGR = myWidgetStyle->createBackgroundRenderer(
 *                                                              widgetState,
 *                                                              myRectangle
 *                                                                         );
 *
 *
 * @author Lakin Wecker aka nikal
 *
 *
 * @see dime::Renderer 
 * @see dime::ThemeService
 *
 */

class WidgetStyle
{


    //======================================================================
    // Private Variables
    //======================================================================/
    private:

        std::map<dime::WidgetState, dime::Event> myEvents;
        std::string myWidgetName;
        dime::Theme::Style myDefaultStyle;
        std::string myWidgetID;
        std::map<dime::WidgetState, dime::Theme::Style> myStyles;

    //======================================================================
    // Public Methods
    //======================================================================
    public:

    //----------------------------------------------------------------------
    // Constructors

    /**
     * Creates a new WidgetStyle using default values.
     */
     WidgetStyle()
     {
     }

    /**
     * Copy constructor.
     */
    WidgetStyle( const WidgetStyle &source )
    {
        // Use assignment operator to do the copy
        // NOTE: If you need to do custom initialization in the constructor this may not be enough.
        *this = source;
    }


    /**
     * Assignment operator.
     */
    WidgetStyle &operator= ( const WidgetStyle &source )
    {
        // Copy fields from source class to this class here.
	myEvents = source.getEvents();
	myWidgetName = source.getWidgetName();
	myDefaultStyle = source.getDefaultStyle();
	myWidgetID = source.getWidgetID();
	myStyle = source.getStyles();
        // Return this object with new value
        return *this;
    }


    //----------------------------------------------------------------------
    // Destructor

    /**
     * Deletes a WidgetStyle instance.
     */
    virtual ~WidgetStyle ()
    {
        // TODO: Free any allocated resources here.
    }


    //----------------------------------------------------------------------
    // Getters

    // Example of a getter method:

    /**
     * Gets the value of Events of this WidgetStyle
     */
    std::map<dime::WidgetState, dime::Event> getEvents() const
    {
        return myEvents;
    }

    /**
     * Gets the value of WidgetName of this WidgetStyle
     */
    std::string getWidgetName() const
    {
        return myWidgetName;
    }

    /**
     * Gets the value of DefaultStyle of this WidgetStyle
     */
    dime::Theme::Style getDefaultStyle() const
    {
        return myDefaultStyle;
    }

    /**
     * Gets the value of WidgetID of this WidgetStyle
     */
    std::string getWidgetID() const
    {
        return myWidgetID;
    }

    /**
     * Gets the value of Styles of this WidgetStyle
     */
    std::map<dime::WidgetState, dime::Theme::Style> getStyles() const
    {
        return myStyles;
    }


    //----------------------------------------------------------------------
    // Setters

    /**
     * Sets the value of Events of this WidgetStyle
     */
    void setEvents( std::map<dime::WidgetState, dime::Event> events )
    {
        myEvents = events ;
    }

    /**
     * Sets the value of WidgetName of this WidgetStyle
     */
    void setWidgetName( std::string widgetName )
    {
        myWidgetName = widgetName ;
    }

    /**
     * Sets the value of DefaultStyle of this WidgetStyle
     */
    void setDefaultStyle( dime::Theme::Style defaultStyle )
    {
        myDefaultStyle = defaultStyle ;
    }

    /**
     * Sets the value of WidgetID of this WidgetStyle
     */
    void setWidgetID( std::string widgetID )
    {
        myWidgetID = widgetID ;
    }

    /**
     * Sets the value of Styles of this WidgetStyle
     */
    void setStyles( std::map<dime::WidgetState, dime::Theme::Style> styles )
    {
        myStyles = styles ;
    }


    //----------------------------------------------------------------------
    // Other public methods
    // NOTE: Group related public methods together and crate a separator comment like above for them.
    
    /**
     * Creates a new background renderer which renders appropriately for given state.
     * This Renderer will use rect to position itself. 
     */
    dime::BackgroundRenderer *createBackgroundRenderer(dime::WidgetState state,
		    					dime::Rectangle &rect);
    

    /**
     * Creates a new font renderer which renders appropriately for given state.
     * This Renderer uses rect to position itself.
     */
    dime::FontRenderer *createFontRenderer(dime::WidgetState state,
		    				dime::Rectangle &rect);



}; // End of WidgetStyle

} // End of dime namespace

#endif
