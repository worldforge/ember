//////////////////////////////////////////////////////////////////////////////
//   File name:  ttform.h
//
//   Class:      TTForm
//
//   Project:    Gui
//   Author:     Hans H„ggstr”m,  hans.haggstromelsinki.fi
//   Base class: TTWindow
//   Decendants:
//   Purpose:    A window object that can contain other window objects.
//
//
//////////////////////////////////////////////////////////////////////////////

#ifndef TTFORM_H
#define TTFORM_H

#include <allegro.h>
#include <time.h>
#include "ttguidef.h"
#include "ttwstyle.h"
#include "ttwindow.h"
#include "ptrlist.h"
#include "ttguigfx.h"

class TTForm : public TTWindow {

  public:
    /*----[ Constructors & destructor: ]------------------------------------*/
    TTForm( TTApplication *application_ = NULL );
    // Default constructor.

    TTForm( const TTForm &source );
    // Copy constructor.

    TTForm( int x_, int y_, int w_, int h_,
            TTWindow      *parent_      = NULL,
            TTApplication *application_ = NULL );
    // Constructor.  If application is null it uses parent application.

    virtual ~TTForm();
    // Destructor.


    /*----------------------------------------------------------------------*/
    /*----[ Window management: ]--------------------------------------------*/
    virtual void addWindow( TTWindow *win );
    virtual void removeWindow( TTWindow *win );
    
    virtual void setWindowIndex( TTWindow *win, int index ) {
    // Sets the index of win to index (starting from 0).  Use this
    // to change the tab-stop order or drawing order (higher index
    // is drawn on top of lower index).  If win isn't in this form
    // nothing is done.
      if (!win) return;

      if (childWindows.remove( win ))
        // Only add it back if it already was in the list:
        // (note that ptrlist starts indexing from one.)
        childWindows.addAt( win, index + 1 );
    }

    /*----------------------------------------------------------------------*/
    /*----[ Drawing and other: ]--------------------------------------------*/
    virtual void draw( BITMAP *dest );
    // Draws an empty window.  Override in derived classes.

    virtual bool update( BITMAP *dest );
    // Draws the parts of the window that have changed since last draw.
    // Returns true if anything was drawn.

    virtual void doTurn();
    // Sort of idle loop.  Gets called frequently.  Don't do anything too
    // time consuming, or the system will slow down.

    virtual void updateChildCoordinates();
    // Calls updateScrCoordinates for all child or contained objects.
    // Called when the window moves or scrolls (or the parent moves or scrolls etc.).

    virtual TTWindow *getWindowAt( int x, int y );
    // Returns window at specified coordinates, NULL if none.
    // If coordinates are outside the window return null.
    // Else check all childrens, topmost first, if none of them,
    // then return this.

    /*----------------------------------------------------------------------*/
    /*----[ High level keyboard events: ]-----------------------------------*/

    virtual bool keyPress( int key );
    // A key was pressed.
    // key is the value returned by allegros readkey -function.
    // (it contains both scancode and ascii info.)
    // Returns true if the keypress was processed.

    /*----------------------------------------------------------------------*/
    /*----------------------------------------------------------------------*/
    virtual TTWindow *getNeighbour( TTWindow *win, int dx, int dy );
    // Returns the closest neighbour to win in given direction.
    // dx and dy should be -1, 0 or 1.
    // If there is no neighbour in that direction it returns win.

    /*----------------------------------------------------------------------*/
    /*----------------------------------------------------------------------*/
    virtual void select( TTWindow *win );
    // Makes win the selected window in forms and other multi child windows.
    // All keyboard input is directed to the selcted window unless the current
    // window doesn't have the focus.  Used to direct keyb. input to the
    // focused window.  This function will call the select function of its
    // parent too.

    /*----------------------------------------------------------------------*/
    /*----[ Scrolling: ]----------------------------------------------------*/

/*
    virtual void setScrollOffset( int nx, int ny );
    // Sets offset of upper left corner inside window.  Checks boundaries.
    // Updates child windows position.

    virtual void checkBoundaries();
    // Calls TTWindow::checkBoundaries(), and updates children.
*/
    /*----------------------------------------------------------------------*/
    /*----[ Style: ]--------------------------------------------------------*/

    virtual void initStyle( TTGuiStyle *style_ = NULL );
    // Copies the given TTGuiStyle, or, if it's NULL,
    // copies the global style settings, modifies them,
    // and calls the initStyle of its child-windows if it has any.
    // Called automatically on creation.

    /*----------------------------------------------------------------------*/
    /*----[ Class name: ]---------------------------------------------------*/

    static string getClassName() { return "TTForm"; }
    // Returns name of class.  Can be used for manual run-time type checking.
    // Make sure to override in derived classes.  Always use exact name of class.

    static bool inherits( string ancestor ) {
    // Returns true if this class or any of its parent classes has the
    // class name given as parameter. Make sure to override in derived classes.
      if (ancestor == getClassName()) return true;
      else return TTWindow::inherits( ancestor );
    }
    
    /*----------------------------------------------------------------------*/
    /*----[ Accessors: ]----------------------------------------------------*/

    //virtual void setStyle( TTWindowStyle *style_ ) { style = style_; }
    // Set style of contained/child objects also?

    virtual int getNrOfWindows() { return childWindows.size(); }
    // Returns nr of contained window objects.

  /*------------------------------------------------------------------------*/
  
    PtrList<TTWindow*>  childWindows;  //! Needed in TTlist.. How do friends work?
    
  protected:
    TTWindow           *selectedChild;

}; // class TTForm.

#endif


