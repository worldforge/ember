//////////////////////////////////////////////////////////////////////////////
//   File name:  ttwindow.h
//
//   Class:      TTWindow (Tea Time Productions Window class)
//
//   Project:    Gui
//   Author:     Hans H„ggstr”m,  hans.haggstromelsinki.fi
//   Base class: -
//   Decendants: lots.
//   Purpose:    The most general window object.  Contains x,y,w,h properties,
//               style info, draw and event handler functions.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef TTWINDOW_H
#define TTWINDOW_H

#include <allegro.h>
#include <time.h>
#include <iostream.h>
#include "ttguidef.h"
#include "ttwstyle.h"
#include "tttime.h"
#include "ptrlist.h"


class TTFadeColorEntry; // Declared and defined in ttwindow.cc

class TTWindow {
  public:
    /*----[ Constructors & destructor: ]------------------------------------*/
    TTWindow( TTApplication *application_ = NULL );
    // Default constructor.

    TTWindow( const TTWindow &source );
    // Copy constructor.

    TTWindow( int x_, int y_, int w_, int h_,
              TTWindow      *parent_      = NULL,
              TTApplication *application_ = NULL );
    // Constructor.  If application is null it uses parent application.

    virtual ~TTWindow();
    // Destructor.


    /*----------------------------------------------------------------------*/
    /*----[ Drawing and other: ]--------------------------------------------*/
    virtual void draw( BITMAP *dest );
    // Draws an empty window.  Override in derived classes.

    virtual bool update( BITMAP *dest );
    // Draws the parts of the window that have changed since last draw.
    // Returns true if anything was drawn.

    /*
    virtual bool hasChanged();
    // Returns true if the window has changed since the last call to
    // draw. (or since the last call to hasChanged() ?)
    */

    virtual void doTurn();
    // Sort of idle loop.  Gets called frequently.  Don't do anything too
    // time consuming, or the system will slow down.

    virtual TTWindow *getWindowAt( int x_, int y_ );
    // Returns window at specified (absolute) coordinates, NULL if none.
    // If coordinates are outside the window return null.
    // Else check all childrens, topmost first, if none of them,
    // then return this.

    virtual void updateScrCoordinates( int parentScrBorX, int parentScrBorY );
    // This method is called by the parent when it has moved or scrolled
    // (or any of its parents moved or scrolled).
    // 'parentScrBorX/Y' are the scrX/Y coordinates of the parent added
    // to the borderWidthLeft and Top, to give the absolute screen coordinates
    // of the topleft of the visible scrollable area (hope that made sense).
    // The scrX/Y variables of this window object get updated (by adding
    // posX/Y of this object to parentScrBorX/Y).
    // Calls updateChildCoordinates to update child objects coordinates also.

    virtual void updateChildCoordinates() {}
    // Calls updateScrCoordinates for all child or contained objects.
    // Called when window moves or scrolls (or parent moves or scrolls etc.)).
    // Override in TTForm (and eventual other objects with some type of
    // sub-objects).


    /*----------------------------------------------------------------------*/
    /*----[ Dragging: ]-----------------------------------------------------*/
    virtual bool objectDragDrop( int x, int y, int button, TTWindow *obj );
    // Called when an other window object is dragged and dropped on this
    // window.  x and y are the mouse coordinates and button is the mouse
    // button used in the dragging.  obj is the window object dropped.
    // Returns true if the object is accepted, false if the drag should be
    // cancelled and the object returned to its previous host and coordinates.

/*** (no TTMouseCursor yet)  **Note: have a flag indicating whether to call this function or not, to save some time??
    virtual TTMouseCursor *objectDragOver( int x, int y, int button, TTWindow *obj );
    // Called when an other window object is dragged over this window.
    // x and y are the mouse coordinates and button is the mouse
    // button used in the dragging.  obj is the window object beeing dragged.
    // Returns the cursor that the mouse cursor should change into.
*/
    
    /*----------------------------------------------------------------------*/
    /*----[ High level keyboard events: ]-----------------------------------*/
    virtual bool keyPress( int key );
    // A key was pressed.
    // key is the value returned by allegros readkey -function.
    // (it contains both scancode and ascii info.)
    // Returns true if the keypress was processed.
    // Override the event methods in derived classes when neccesary.

    virtual bool captureKeyboard();
    // Captures the keyboard events, all keyboard events are sent to this window
    // until releaseKeyboard is called.  Returns true if keyboard was successfully
    // captured (if some other window already had it captureKeyboard fails).

    virtual void releaseKeyboard();
    // Releases the keyboard if it was captured by captureKeyboard().

    /*----[ Low level keyboard events: ]------------------------------------*/
    /*
    virtual void keyDown( int key );
    // A key was pressed down.
    // key is the value returned by allegros readkey -function.
    // (it contains both scancode and ascii info.)

    virtual void keyUp( int key );
    // A key was released.
    // key is the value returned by allegros readkey -function.
    // (it contains both scancode and ascii info.)
    */

    /*----[ High level mouse events: ]--------------------------------------*/
    virtual void mouseClick( int x, int y, int button, TTWindow *win );
    // A mouseclick.  button is 1 for left, 2 for right and 3 for middle.
    // win is the window at position x, y (normally 'this').

    virtual void mouseDblClick( int x, int y, int button, TTWindow *win );
    // A mouse button was double-clicked.
    // button is 1 for left, 2 for right and 3 for middle.
    // win is the window at position x, y (normally 'this').

    virtual bool mouseDragStart( int x, int y, int button, TTWindow *win );
    // The window object was dragged with some mouse button
    // (=mouse button was pressed and mouse was moved more than a few pixels).
    // Obj is the dragged window, button is the dragging mouse button.
    // Returns true if it is ok to start the dragging (the window object
    // captures the mouse and moves when the mouse is moved).
    // False if the drag should be ignored.

    virtual void mouseDragCancel( int x, int y, int button, TTWindow *win );
    // The dragging was canceled by pressing any other mouse button than
    // the one that started the drag.  The window object is automatically
    // returned to its previous host object and coordinates.
    // Button is the button pressed to cancel the drag.

    virtual bool mouseDragEnd( int x, int y, int button, TTWindow *dest );
    // The mouse button used to drag the window object was released.
    // 'dest' is the window object under the mouse (and the dragged window).
    // Return true if the drag is accepted, false if the dragged window should
    // be returned to its previous host an coordinates.
    // Calls the objectDragDrop method of destination window,
    // and cancells the drag if destination->objectDragDrop returns false.

    virtual void mouseEnter( int x, int y, TTWindow *win );
    // The mouse has just entered win.
    // win is the window at position x, y (normally 'this').

    virtual void mouseExit( int x, int y, TTWindow *win );
    // The mouse has just left win.
    // win is the window at position x, y (normally 'this').

    virtual bool captureMouse();
    // Captures the mouse events, all mouse events are sent to this window
    // until releaseMouse is called.  Returns true if mouse was successfully
    // captured (if some other window already had it captureMouse fails).

    virtual void releaseMouse();
    // Releases the mouse if it was captured by captureMouse().

    /*----[ Low level mouse events: ]---------------------------------------*/
    virtual void mouseDown( int x, int y, int button, TTWindow *win );
    // A mouse button was pressed.
    // button is 1 for left, 2 for right and 3 for middle.
    // win is the window at position x, y (normally 'this').

    virtual void mouseUp( int x, int y, int button, TTWindow *win );
    // A mouse button was released.
    // button is 1 for left, 2 for right and 3 for middle.
    // win is the window at position x, y (normally 'this').

    virtual void mouseMove( int x, int y, TTWindow *win );
    // The mouse was moved.  x and y are the new mouse coordinates.
    // win is the window at position x, y (normally 'this').
    // Check for dragging and calls mouseDragStart if neccesary.


    /*----------------------------------------------------------------------*/
    /*----[ Color fades: ]--------------------------------------------------*/

  protected: // Avoid creating colorfade entries for variables outside the
             // class instance that could be deleted and cause the fade
             // routines to reference invalid memory.
    virtual void fadeColor( int *variable, int destColor, int time_ms,
                            bool immediate = false, bool repeat = false );
    // Fade the colorvalue stored in variable to destColor in time_ms milliseconds.
    // If there is new color fades for a variable while it is still fading, 
    // it completes the first colorfade before beginning on the next.
    // If immediate is true the colorfade start immediately, removing all
    // previous fades for the variable.  If repeat is true the colorfade is
    // not removed from the queue when its done, but will repeat again
    // when the other colorfades for that variable have performed.
  public:

    virtual void doColorFades();
    // Updates the active colorfades.


    /*----------------------------------------------------------------------*/
    /*----[ Scrolling: ]----------------------------------------------------*/

    // (Se below for setting and getting scroll offset.)

    virtual void scrollTo( int x, int y, long timeInMs );
    // Softly scrolls to the part of the virtual area that starts at x,y
    // in specified number of milliseconds.
    // For example scrollTo( 0, 0, 1000) scrolls to the upper left corner
    // of the virtual area from current position using exactly one second.
    // A new scroll command can be given anytime without problems.

    virtual void scrollToPosWithSpeed( int x, int y, float pixelsPerSecond );
    // Softly scrolls to the part of the virtual area that starts at x,y
    // with specified speed.
    // For example scrollTo( 0, 0, 100) scrolls to the upper left corner
    // of the virtual area from current position with the average speed
    // of 100 pixels per second (it still starts and stops softly, though).
    // A new scroll command can be given anytime without problems.

    virtual void setScrollDelta( float dx, float dy );
    // Stops any ongoing scrolling and sets fixed scroll velocity.
    // Can be used to implement custom scrolling.
    // Automatically stops when it hits edge of visible screen.

    virtual void doScroll();
    // Does the scrolling.  This method is called from doTurn().


    /*----------------------------------------------------------------------*/
    /*----[ Style: ]--------------------------------------------------------*/

    virtual void initStyle( TTGuiStyle *style_ = NULL );
    // Copies the given TTGuiStyle, or, if it's NULL,
    // copies the global style settings, modifies them,
    // and calls the initStyle of its child-windows if it has any.
    // Called automatically on creation.

    /*----------------------------------------------------------------------*/
    /*----[ Class name: ]---------------------------------------------------*/

    static string getClassName() { return "TTWindow"; }
    // Returns name of class.  Can be used for manual run-time type checking.
    // Make sure to override in derived classes.  Always use exact name of class.

    static bool inherits( string ancestor ) {
    // Returns true if this class or any of its parent classes has the
    // class name given as parameter. Make sure to override in derived classes.
      if (ancestor == getClassName()) return true;
      // Check parent here.
      else return false;
    }
    
    /*----------------------------------------------------------------------*/
    /*----[ Cecking: ]------------------------------------------------------*/
    
    virtual void checkCoordinates();
    // Checks that all coordinates, sizes and borders are in range and
    // correct.  Corrects what it can (sets correct borderWidthTop/... for
    // example).  Might be called after changing style stuff or something.


    /*----------------------------------------------------------------------*/
    /*----[ Accessors: ]----------------------------------------------------*/

    // Enabled & visible:
    virtual void enable()     { enabled = true;  }
    virtual void disable()    { enabled = false; }
    virtual bool isEnabled()  { return enabled;  }
    virtual void setEnabled( bool enabled_ )  { enabled = enabled_; }

    virtual void show()       { visible = true;  }
    virtual void hide()       { visible = false; }
    virtual bool isVisible()  { return visible;  }
    virtual void setVisible( bool visible_ )  { visible = visible_; }

    virtual bool hasTabstop() { return tabstop; }

    virtual bool isMovable()  { return movable;  }
    virtual bool isDragable() { return dragable; }
    virtual void setMovable(  bool b ) { movable  = b; }
    virtual void setDragable( bool b ) { dragable = b; }    

    // Parent & application:
    virtual TTWindow      *getParent()      { return parent;      }
    virtual TTApplication *getApplication() { return application; }

    virtual void           setParent( TTWindow *win )           { parent = win; }
    virtual void           setApplication( TTApplication *app ) { application = app; }

    // Capturing:
    virtual bool hasCapturedMouse();
    virtual bool hasCapturedKeyboard();

    // Style:
    virtual TTGuiStyle         *getGuiStyle()  { return &style;          }
    virtual TTWindowStyle      *getWinStyle()  { return &style.winStyle; }
    virtual TTApplicationStyle *getApplicationStyle();

    // Focus:
    virtual int  getFocusColor()          { return focusColor; }
    virtual bool hasFocus();
    virtual void setFocus( bool focus_ );

    virtual void select( TTWindow *win )
      { if (parent) parent->select(this); win = win; /*avoid irritating unused parameter warning*/}
    // Makes win the selected window in forms and other multi child windows.
    // All keyboard input is directed to the selcted window unless the current
    // window doesn't have the focus.  Used to direct keyb. input to the
    // focused window.  This function will call the select function of its
    // parent too.

    // Mouse over:
    virtual bool mouseIsOver();  // True if mouse is over this window.


    // Accessors for coordinates & sizes:
    virtual int   getPosX()              { return posX;              }
    virtual int   getPosY()              { return posY;              }
    virtual int   getScrX()              { return scrX;              }
    virtual int   getScrY()              { return scrY;              }
    virtual float getScrollX()           { return scrollX;           }
    virtual float getScrollY()           { return scrollY;           }
    virtual int   getVisibleW()          { return visibleW;          }
    virtual int   getVisibleH()          { return visibleH;          }
    virtual int   getVirtualW()          { return virtualW;          }
    virtual int   getVirtualH()          { return virtualH;          }
    virtual int   getBorderWidthTop()    { return borderWidthTop;    }
    virtual int   getBorderWidthBottom() { return borderWidthBottom; }
    virtual int   getBorderWidthLeft()   { return borderWidthLeft;   }
    virtual int   getBorderWidthRight()  { return borderWidthRight;  }
    virtual int   getBorderWT()          { return borderWidthTop;    }
    virtual int   getBorderWB()          { return borderWidthBottom; }
    virtual int   getBorderWL()          { return borderWidthLeft;   }
    virtual int   getBorderWR()          { return borderWidthRight;  }

    virtual bool  getScrollInProgress()  { return scrollInProgress;  }
    virtual bool  getInstantScroll()     { return instantScroll;     }


    virtual void setPos( int x, int y );
    // Sets the offset from parents topleft visible area (+borders) to
    // the upperleft corner of the visible area of this window object.
    
    virtual void setScr( int x, int y );
    // Sets the absolute screen coordinates for the upperleft corner
    // of the visible area.

    virtual void setScroll( float x, int y );
    // Sets the offset from the topleft corner of the virtual area to the
    // topleft corner of the visible area.

    virtual void setVisibleAreaSize( int w, int h );
    // Sets the size of the visible area (the size of the visible
    // (not counting overlapping etc.) window on screen, including borders).
    
    virtual void setVirtualAreaSize( int w, int h );
    // Sets the size of the virtual area.  That is the atrea you can scroll
    // around, but of which only a part might be visible at a time
    // (in the visible window).

    virtual void setAreaSize( int w, int h ) {
    // Sets visible area to w, h and virtual area to w, h - border widths:
      w = MAX( w, 0);
      h = MAX( h, 0);
      setVisibleAreaSize( w, h );
      setVirtualAreaSize( w - getBorderWidthLeft() - getBorderWidthRight(),
                          h - getBorderWidthTop()  - getBorderWidthBottom() );
    }
    
    virtual void setBorderWidths( int top, int bottom, int left, int right );
    // Sets the total amount of border in pixels at the (inner) edges of the
    // visible screen.  This border should include title and scrollbars and
    // such things as well.  Note that this method doesn't alter the border
    // appearance, normally the library user doesn't have to call this directly.

    


  /*------------------------------------------------------------------------*/
  protected:
  
    /* I think theese could be moved into the code...
         if it doesn't get to messy from taht. */
    virtual int getBgFocusColor() {
      if (parent && parent->getWinStyle())
        return parent->getWinStyle()->bgColor;
      else
        return style.winStyle.bgColor;
    }

    virtual int getStartFocusColor() {
      if (style.globalSettings) return
        style.globalSettings->focusOutlineStartColor;
      else return 166; // Silly hardwired value.
    }

    virtual int getMidFocusColor() {
      if (style.globalSettings) return
        style.globalSettings->focusOutlineMidColor;
      else return 174; // Silly hardwired value.
    }

    virtual int getFinalFocusColor() {
      if (style.globalSettings) return
        style.globalSettings->focusOutlineFinalColor;
      else return 171; // Silly hardwired value.
    }

    virtual int getOutFocusColor() {
      if (style.globalSettings) return
        style.globalSettings->focusOutlineOutColor;
      else return 166; // Silly hardwired value.
    }

    virtual void checkVirtualAreaVisibility();
    // This is just a helper function used by the coordinate and size setters
    // to make sure that as much as posible of the virtual area is in the
    // visible area, and that scrollX/Y always stays positive or zero.
    
    // Coordinates and size:
    int   posX, posY,         // Offset to top left of visible window from parent window top left.
          scrX, scrY;         // Screencoordinates of topleft of visible window.
    float scrollX, scrollY;   // Offset to top left of visible window from the top left of the virtual window.
    int   visibleW, visibleH, // The size of the window object on the screen (if it isn't covered by anything)
          virtualW, virtualH; // The size of the whole scrollable area ([part of] it is visible in the Visible Window inside the border).
    int   borderWidthTop,     // Width of border in the indicated direction
          borderWidthBottom,  // (inside visible window).
          borderWidthLeft,
          borderWidthRight;

    // Scrolling:
    bool  scrollInProgress;         // True if a scroll is under way.  In that case the scroll data is stored in the following fields.
    int   scrollGoalX, scrollGoalY; // Destination coordinates for scrollX and scrollY (what they will be when the scroll ends).
    float scrollDX, scrollDY,       // Current velocity vector (delta) of the scroll.
          scrollAccX, scrollAccY,   // Current acceleration of the above (added to them every ms).
          scrollAcc2X, scrollAcc2Y; // Change to this acceleration after about half the time allowed for the scroll has passed.
    long  scrollAccChangeTime,      // Time when we should swith to scrollAcc2.
          endScrollTime,            // Time when the scroll is to end.  At that point scrollX/Y should equal scrollGoalX/Y.
          prevScrollUpdateTime;     // When did we scroll last. Used in doScroll() when updating scrolls.
    // Note: (!scrollInProgress && (scrollDX != 0 || ..Y != 0))
    // means that an user scroll is in progress.

    // Other fields:
    TTGuiStyle     style;       // Colors, font, borderstyle, etc.
    TTWindow      *parent;      // Host window (the one that contains this window object).
    TTApplication *application; // The application that owns this window.

    // Flags:
    bool enabled;
    bool visible;
    bool tabstop;
    bool showFocusOutline; // Normally true.
    bool instantScroll;    // If this is true all the scrolling is instanteous (and the nice softscroll is never used).
//** does not feel nice  **  bool autoscroll;
    bool movable;   // The window object can be moved within its parent form by dragging with the mouse.
    bool dragable;  // The window object can be dragged to any visible window object by dragging with the mouse.
    //bool changed; // True if window has changed since last draw(??) call.

    // Redraw flags, redraw if set to true:
    /*  currently not really used, the redrawing stuff needs to be worked out*/
    bool redrawAll;
    bool redrawBorder;
    bool redrawArea;
    bool redrawText;
    bool redrawFocusOutline;

    // Focus stuff:
    int  focusColor;       // Color of the focus outline.

    // Double click stuff:
    static time_t prevMouseDownTime;
    static int    prevMouseX;
    static int    prevMouseY;
    static int    prevMouseButton;
    static bool   prevMouseClickWasDouble;

    // Mouse drag stuff:
    static int mouseDragButton;   // 0 if not dragging, else nr of mouse button that started the drag.
    static int mouseButtonDown;   // Nr of pressed mouse button, 0 if none and -1 if more than one.  Used only/mainly for drag start detection.
    static TTWindow *mouseDownWin;// The window any mouse button was last pressed in. (used for dragging)
    static TTWindow *draggedWin;  // The window object beeing dragged.
    static int draggedOriginalX;  // Original x coordinate for the dragged window.
    static int draggedOriginalY;  // Original y coordinate for the dragged window.
    static int draggedMouseXOffs; // XOffset from mouse coordinate to top left of dragged window.
    static int draggedMouseYOffs; // YOffset from mouse coordinate to top left of dragged window.
    static TTWindow *draggedOriginalHost; // The original host window for the window object beeing dragged.
    
    // Color fades:
    PtrList<TTFadeColorEntry*> colorFades;

    // Autoscroll variables:
    int  autoscrollCountdown;
    long autoscrollTimer;
    float autoscrollSpeedFactor;

    // Border color:
    bool useParentColorForBorder; // False for forms and buttons (etc..)
                                  // and true for others...

}; // class TTWindow.

#endif


