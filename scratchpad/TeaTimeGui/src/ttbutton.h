//////////////////////////////////////////////////////////////////////////////
//   File name:  ttbutton.h
//
//   Class:      TTButton
//
//   Project:    Gui
//   Author:     Hans H„ggstr”m,  hans.haggstromelsinki.fi
//   Base class: TTWindow
//   Decendants:
//   Purpose:    A normal push-button.
//
//
//   Work diary:
// Date      Time     Lines   Things done
// ~~~~~~~~~ ~~~~~~~~ ~~~~~~~ ~~~~~~~~~~~~~~~~~~~~~~
// 19.5.1998
//
//
// Notes & things to do:
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  * Change callback system later to work with registered functions etc.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef TTBUTTON_H
#define TTBUTTON_H

#define TTGUI_BUTTON_STD_DOWNTIME 6

#include <allegro.h>
#include <time.h>
#include <string>
#include "ttguigfx.h"
#include "ttguidef.h"
#include "ttwindow.h"
#include "ttwstyle.h"


class TTButton : public TTWindow {
  public:
    /*----[ Constructors & destructor: ]------------------------------------*/
    TTButton( TTApplication *application_ = NULL );
    // Default constructor.

    TTButton( const TTButton &source );
    // Copy constructor.

    TTButton( int x_, int y_, int w_, int h_,
              TTWindow      *parent_      = NULL,
              TTApplication *application_ = NULL );
    // Constructor.  If application is null it uses parent application.

    virtual ~TTButton();
    // Destructor.


    /*----------------------------------------------------------------------*/
    /*----[ Drawing and other: ]--------------------------------------------*/
    virtual void draw( BITMAP *dest );
    // Draws border, area, outline and caption.
    // Should it call the parent draw?  Probably, altough in that case
    // it has to draw the border twice when pressed.

    virtual bool update( BITMAP *dest );
    // Draws the parts of the window that have changed since last draw.
    // Returns true if anything was drawn.

//    virtual bool hasChanged();
    // Returns true if the window has changed since the last call to
    // draw. (or since the last call to hasChanged() ?)

    virtual void doTurn();
    // Sort of idle loop.  Gets called frequently.  Don't do anything too
    // time consuming, or the system will slow down.

    /*--------------------------------------------------------------------------*/
    // Button functions:

    virtual void pressButton( bool waitForRelease );
    // Presses down the button.
    // If waitFor release is false the button is released after a short moment,
    // else the button stays down until releaseButton is called.

    virtual void releaseButton();
    // Releases the button and calls doFunction.

    virtual void cancelPress();
    // Cancels the button press when dragging the mouse outside the window.

    virtual void doFunction();
    // Executes the button function.

    /*--------------------------------------------------------------------------*/
    virtual bool keyPress( int keybKey );
    // Press button if enter / space.. ?
    // Returns true if the keypress was processed.

    virtual void mouseEnter( int x, int y, TTWindow *win );
    // Highlights caption text if not disabled.
    // Also represses the button if mouse dragged back inside from outside.

    virtual void mouseExit( int x, int y, TTWindow *win );
    // Set caption text color back to normal.
    // Also depresses the button if mouse dragged outside.

    virtual void mouseDown( int x, int y, int button, TTWindow *win );
    // Presses the button

    virtual void mouseUp( int x, int y, int button, TTWindow *win );
    // Releases button

    virtual bool isPressed()  { return pressedDown; }

    virtual string getCaption()                 { return caption;   }
    virtual void   setCaption( string newcap )  { caption = newcap; }

    virtual void setFocus( bool focus_ );
    // Used to cancel space-presses when focus changes.

    /*----------------------------------------------------------------------*/
    /*----[ Function to call: ]---------------------------------------------*/

    virtual void setCallbackFunction( void (*buttonFunc_)( TTWindow *button ) ) {
    // Sets the function to call when button is pressed.  Should take
    // a pointer to a window object (the button) and four integers for
    // mouse pos an button or the key used to press the button.
      buttonFunc = buttonFunc_;
    }
    /*----------------------------------------------------------------------*/
    /*----[ Class name: ]---------------------------------------------------*/

    static string getClassName() { return "TTButton"; }
    // Returns name of class.  Can be used for manual run-time type checking.
    // Make sure to override in derived classes.  Always use exact name of class.

    static bool inherits( string ancestor ) {
    // Returns true if this class or any of its parent classes has the
    // class name given as parameter. Make sure to override in derived classes.
      if (ancestor == getClassName()) return true;
      else return TTWindow::inherits( ancestor );
    }
    
    /*----------------------------------------------------------------------*/
    /*----[ Style: ]--------------------------------------------------------*/

    virtual void initStyle( TTGuiStyle *style_ = NULL );
    // Copies the given TTGuiStyle, or, if it's NULL,
    // copies the global style settings, modifies them,
    // and calls the initStyle of its child-windows if it has any.
    // Called automatically on creation.

  /*------------------------------------------------------------------------*/
  protected:
    virtual int getNormalCaptionColor() {
      return style.winStyle.textColor;
    }

    virtual int getHighlightedCaptionColor() {
      return style.buttonMouseOverTextColor;
    }

    virtual int getPressedCaptionColor() {
      return style.buttonPressedTextColor;
    }

    string caption;
    bool   pressedDown;          // True if button down.
    int    destCaptionColor;     // Destination color for caption.
    int    captionColor;         // Current color for caption.
    bool   wasMouseDownOnButton; // True if left mouse was pressed on button
                                 // and not yet released.
    bool   wasSpaceDownOnButton; // True if space was pressed on button
                                 // and not yet released.
    int    pressCounter;

    void (*buttonFunc)( TTWindow *button );
    // Function to call when button pressed.  Returns pointer to button
    // object (as a window pointer)


}; // class TTButton.

#endif


