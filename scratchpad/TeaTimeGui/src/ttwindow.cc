//////////////////////////////////////////////////////////////////////////////
//   File name:  ttwindow.cc
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
//
//   Work diary:
// Date      Time     Lines   Things done
// ~~~~~~~~~ ~~~~~~~~ ~~~~~~~ ~~~~~~~~~~~~~~~~~~~~~~
// 16.5.1998 2:00    (156)    Header & empty functions + lotsa planning.
// 16.5      1:00     248     Implementation.
// 17.5      0:30     324     Added all the focusColor stuff.
//                             Now _that_ looks good =)  It's  ...lethal.  :-)
// 19.5      0:15     357     Added mouseEnter & mouseExit.
// 19.5      0:05     382     Added captureKeyboard stuff
// 19.5      1:00     429     All kind of include problems with mouse/keyb
//                             capturing, but now it ought to be figured out.
// 19.6      1:00     453     Made small bugfixes to the doubleclick code.
//                             The Logitec midle button -> instant double click
//                             mousedriver still seems to confuce the code,
//                             I suppose it's because the clicks are right
//                             after one another, but I can't figure out why
//                             it doesn't get it sometimes.  Mostly it works, tough.
// 17.8      1:10     600     Implemented new style system.
// ?         ?        692     Added colorfades.  Still buggy...
// 12.9      0:10     693     Fixed some colorfade bug, but still buggy..
// 12.9      2:00     733     Now the colorfades work.  The linked list code
//                             was full of stupid, stupid bugs...
//                             Programmers rule #1: Bugs _love_ linked lists.
//                             I should really use my PtrList library,
//                             can't understand why i didn't.
//                             The colorfade does slow down the framerate however..
//                             What should I do?  How to optimize it?
// 12.9      0:10     748     Added oldPos to avoid some calculations, now
//                             colorfades seem to work acceptably fast.
// 12.9      0:20     740     Made own colorFades list for every window obj,
//                             using the PtrList class.  No problems.
//                             Works fine.  Better performance, too.
// 12.9      0:40     780     Added immediate and repeat flags to fadeColor. Nice =)
// 12.9      2:00     878     Added scrolling.
// 12.9      0:40     872     Debugged scrolling.  Now it works.
// 12.9      1:50     977     Added autoscrolling.  Works fine, just need
//                             indicators.  Some softing could also be done.
//                             Its quite kludgy however... A physic based
//                             general scrolling interface would be nice,
//                             with delta, acceleration and friction..
//                             It could be used with arrow+ctrl too, or something.
// 13.9      0:05     985     Added useParentColorForBorder -flag.
//                             Kludgy, but I suppose it'll work.
//
// 19.1.1999 4:00             Added mouse dragging methods (not implemented yet).
// 19.1      3:00    1205     Implemented most dragging stuff.
// 19.1      0:40    1218     Compiled and run.  It really works!
//                             It just has some problems with coordinates.
// 23.1      3:30    1220     Added new coordinate, size and scroll system
//                             prototypes and interface to the header.
// 24.1      3:00    1472     Implemented most of the new coordinate system in TTWindow.
// 05.02     1:45    1480     Implemented new coord sys in all the other
//                             window objects.  Invented updateChildCoordinates()
// 06.02     1:00    1488     Fixed dragging.
//
// Notes & things to do:
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  * Move window dragging specific variables to the application?  Yep!
//  * The getWindowAt-method contais a small kludge, it assumes that dragged
//     windows are 'invisible'.  Could cause problems if getWindowAt is used
//     for something else than getting the pointed window.
//  * Add clip info to draw function?
//  * Change name from TTWindow to TTWindowObj.
//     More correct and less confusing...
//  * Add all sorts of event callbacks so that user can have own functions
//     that are called when a button is pressed / mouse enters, etc..
//     Need to work those out a bit, but should be straightforward.
//     They should just be pointers to functions that take a pointer to a
//     windows object, and a value that tells the type of event(?),
//     and add code that calls them, and then they can just be registered
//     as callback-function properties and assigned in the editor..
//
//////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>

#include "ttwindow.h"
#include "ttguigfx.h"  // Can't be included in the header, causes circle
                       //  reference there.
#include "ttapplic.h"  // Same thing with this one..
#include "ttform.h"    // And here... what a mess :-)

/*--------------------------------------------------------------------------*/
class TTFadeColorEntry {
// Used for storing colorfade info.
 public:
  int    startR, startG, startB; // Start color.
  float  deltaR, deltaG, deltaB; // Difference from start to dest color.
  int    curR,   curG,   curB;   // Current color value.
  int    destColor;              // Destination color in normal allegro color format.
  int   *variable;               // Variable holding color value to change.
  time_t startTime;              // Time of color fade start.
  int    length_ms;              // Length of fade.
  bool   active;                 // True if this entry is currently 'played'.
  bool   repeat;                 // If true the entry will not be removed when complete.
  float  oldPos;                 // Holds old value of pos-variable in doColorFade.  Used to avoid unnecesary calculations.
  PtrList<TTFadeColorEntry*> *list; // List that this entry belongs to.

  //-------------------------------------------------
  TTFadeColorEntry( PtrList<TTFadeColorEntry*> &list_,
                    int &variable_, int destColor_, int length_ms_,
                    bool immediate, bool repeat_ ) {
  // Constructor.  List is a colorfade list to add this fade to.

    // Init fields:
    variable  = &variable_;
    length_ms = length_ms_;
    destColor = destColor_;
    repeat    = repeat_;
    list      = &list_;
    initFields();

    // If immediate is true then delete any previous colorfades for this variable:
    if (immediate) {
      for (list->first(); !list->isOutside(); ) {
        if ( list->getCurrent() &&
             list->getCurrent()->variable == variable )
          list->remove();
        else
          list->next();
      }
    }
    else {
      // Check if there already is any colorfades modifying this variable,
      // in that case set active to false:
      for (list->first(); !list->isOutside(); list->next())
        if ( list->getCurrent() &&
             list->getCurrent()->variable == variable &&
             list->getCurrent()->active ) {
          active = false;
          break;
        }
    }

    // Add to list:
    list->addLast(this);
  } // Constructor

  //-------------------------------------------------
  ~TTFadeColorEntry() {
    // Remove from linked list:
    list->remove(this);
  } // Destructor


  //-------------------------------------------------
  void initFields() {
  // Inits fields at start of animation:
    startR    = getr(*variable);
    startG    = getg(*variable);
    startB    = getb(*variable);
    deltaR    = getr(destColor) - startR;
    deltaG    = getg(destColor) - startG;
    deltaB    = getb(destColor) - startB;
    active    = true;
    oldPos    = -1;

    // Get start time:
    startTime = ttGetMsTimer();
  } // initFields

}; // end of class TTFadeColorEntry

/*--------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------*/
// Class TTWindow
/*--------------------------------------------------------------------------*/

time_t TTWindow::prevMouseDownTime       = 123456;
int    TTWindow::prevMouseButton         = 0;
int    TTWindow::prevMouseX              = 0;
int    TTWindow::prevMouseY              = 0;
bool   TTWindow::prevMouseClickWasDouble = false;
int    TTWindow::mouseDragButton         = 0;
int    TTWindow::mouseButtonDown         = 0;
int    TTWindow::draggedOriginalX        = 0;
int    TTWindow::draggedOriginalY        = 0;
int    TTWindow::draggedMouseXOffs       = 0;
int    TTWindow::draggedMouseYOffs       = 0;
TTWindow *TTWindow::mouseDownWin         = NULL;
TTWindow *TTWindow::draggedWin           = NULL;
TTWindow *TTWindow::draggedOriginalHost  = NULL;

/*----[ Constructors & destructor: ]------------------------------------*/
TTWindow::TTWindow( TTApplication *application_ )
     : style(NULL), colorFades()  {
// Default constructor.
  posX = posY = scrX = scrY = 0;
  scrollX = scrollY = 0;
  virtualH = virtualW = visibleH = visibleW = 0;
  scrollDX = scrollDY = 0;
  setBorderWidths( 0, 0, 0, 0 );
  scrollInProgress   = false;
  
  enabled            = true;
  visible            = true;
  parent             = NULL;
  application        = application_;
  //changed           = true;
  showFocusOutline   = true;
//  autoscroll         = false;

  tabstop            = true;

  redrawAll          = true;
  redrawBorder       = true;
  redrawArea         = true;
  redrawText         = true;
  redrawFocusOutline = true;

  // Copy style from global settings:
  initStyle();

  focusColor         = getBgFocusColor();

  // Init autoscroll countdown:
/*
  autoscrollCountdown = TTGUI_AUTOSCROLL_STARTDELAY;
  if (getApplicationStyle())
    autoscrollCountdown = getApplicationStyle()->autoScrollStartDelay;
  autoscrollTimer = ttGetMsTimer();
  autoscrollSpeedFactor = 0;
*/
  useParentColorForBorder = true;
} // Default constructor

/*--------------------------------------------------------------------------*/
TTWindow::TTWindow( const TTWindow &source )
      : style(source.style), colorFades()  {
// Copy constructor.
  posX              = source.posX;
  posY              = source.posY;
  scrX              = source.scrX;
  scrY              = source.scrY;
  scrollX           = source.scrollX;
  scrollY           = source.scrollY;
  virtualW          = source.virtualW;
  virtualH          = source.virtualH;
  visibleW          = source.visibleW;
  visibleH          = source.visibleH;
  borderWidthTop    = source.borderWidthTop;
  borderWidthBottom = source.borderWidthBottom;
  borderWidthLeft   = source.borderWidthLeft;
  borderWidthRight  = source.borderWidthRight;
  scrollInProgress  = false;
  scrollDX = scrollDY = 0;
  
  enabled           = source.enabled;
  visible           = source.visible;
  tabstop           = source.tabstop;
//  autoscroll        = source.autoscroll;
  parent            = source.parent;
  application       = source.application;
  //changed           = true;
  //focus             = false;
  showFocusOutline  = source.showFocusOutline;
  focusColor        = getBgFocusColor();
  redrawAll         = true;

/*
  // Init autoscroll countdown:
  autoscrollCountdown = TTGUI_AUTOSCROLL_STARTDELAY;
  if (getApplicationStyle())
    autoscrollCountdown = getApplicationStyle()->autoScrollStartDelay;
  autoscrollTimer = ttGetMsTimer();
  autoscrollSpeedFactor = 0;
*/
  useParentColorForBorder = source.useParentColorForBorder;
} // Copy constructor

/*--------------------------------------------------------------------------*/
TTWindow::TTWindow( int x, int y, int w, int h,
                    TTWindow      *parent_,
                    TTApplication *application_ )
     : style(NULL), colorFades() {
// Constructor.
  parent      = parent_;
  application = application_;

  setVisibleAreaSize( w, h );
  setVirtualAreaSize( w, h );
  setPos( x, y );
  setScroll( 0, 0 );
  
  if (parent) {
    int sx, sy;
    sx = parent->getScrX() + parent->getBorderWidthLeft() + getPosX() - (int)parent->getScrollX();
    sy = parent->getScrY() + parent->getBorderWidthTop()  + getPosY() - (int)parent->getScrollY();
    setScr( sx, sy );
  }
  else setScr( x, y );

  scrollInProgress  = false;
  visible           = true;
  enabled           = true;
  //changed           = true;
  tabstop           = true;
//  autoscroll        = false;
  //focus             = false;
  redrawAll         = true;
  showFocusOutline  = true;
  movable           = false;
  dragable          = false;

  // Copy style from host-window/global settings:
  initStyle();

  focusColor        = getBgFocusColor();

/*
  // Init autoscroll countdown:
  autoscrollCountdown = TTGUI_AUTOSCROLL_STARTDELAY;
  if (getApplicationStyle())
    autoscrollCountdown = getApplicationStyle()->autoScrollStartDelay;
  autoscrollTimer = ttGetMsTimer();
  autoscrollSpeedFactor = 0;
*/

  useParentColorForBorder = true;
} // constructor

/*--------------------------------------------------------------------------*/
TTWindow::~TTWindow() {
// Destructor.

  // Release mouse if we had it captured:
  releaseMouse();

  // Delete color fade entries:
  colorFades.deleteAll();

  // Notify parent?? nah, probably it already knows?? hmm..
  // Maybe would be better to notify anyway.
} // destructor

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
void TTWindow::draw( BITMAP *dest ) {
// Draws an empty window.  Override in derived classes.

  if (!visible) return; // Hmmm?? here?

  // Draw border, area and possibly outline:
  if (useParentColorForBorder && getParent() && getParent()->getWinStyle())
    ttDrawBorder( dest, this, getParent()->getWinStyle()->bgColor );
  else
    ttDrawBorder( dest, this );

  ttDrawArea  ( dest, this );
  if (showFocusOutline) ttDrawFocusOutline( dest, this );

  // Update changed flags:
  //changed = false;
  redrawAll = redrawBorder = redrawArea = redrawText = false;
} // draw

/*--------------------------------------------------------------------------*/
bool TTWindow::update( BITMAP *dest ) {
// Draws the parts of the window that have changed since last draw.
// Returns true if anything was drawn.

  bool redrawn = false;

  if (redrawAll) {
    draw( dest );
    return true;
  }
  if (redrawArea) {
    ttDrawArea  ( dest, this );
    redrawArea = false;
    redrawText = true;
    redrawn    = true;
  }
  if (redrawBorder) {
    ttDrawBorder( dest, this );
    redrawBorder = false;
    redrawn      = true;
  }
  if (redrawFocusOutline) {
    if (showFocusOutline) ttDrawFocusOutline( dest, this );
    redrawFocusOutline = false;
    redrawn            = true;
  }

  return redrawn;

} // update

/*--------------------------------------------------------------------------*/
/*
bool TTWindow::hasChanged() {
// Returns true if the window has changed since the last call to
// draw. (or since the last call to hasChanged() ?)

  return (redrawAll || redrawBorder || etc...

  return changed;
} // hasChanged
*/

/*----------------------------------------------------------------------*/
void TTWindow::doTurn() {
// Sort of idle loop.  Gets called frequently.  Don't do anything too
// time consuming, or the system will slow down.

  // Update colorfades:
  doColorFades();

  // Do scrolling:
  doScroll();

  /*
  // Check autoscroll:
  if ( autoscroll ) {
    // Init variables:
    int mx = mouse_x;
    int my = mouse_y;
    int x1 = getXOffs() + getX();
    int y1 = getYOffs() + getY();
    int x2 = x1 + getW();
    int y2 = y1 + getH();
    long curtime    = ttGetMsTimer();
    long diftime    = curtime - autoscrollTimer;
    int  startDelay = TTGUI_AUTOSCROLL_STARTDELAY;
    int  border     = TTGUI_AUTOSCROLL_BORDERWIDTH;

    // Extend border outside window when autoscroll is going on:
    if (autoscrollCountdown < 0) {
      x1 -= border;
      x2 += border;
      y1 -= border;
      y2 += border;
      border *= 2;
    }

    // If inside window..:
    if ((mx >= x1 && mx < x2 && my >= y1 && my < y2) || hasCapturedMouse()) {

     // Check if on scrollzone and calculate scroll delta:
      int dx = 0;
      int dy = 0;
      int speed  = TTGUI_AUTOSCROLL_SPEED;
      TTApplicationStyle *appStyle = getApplicationStyle();
      if (appStyle) speed = appStyle->autoScrollSpeed;

      // Calculate delta:
      if (mx < x1+border) dx =  ((border - (mx - x1)) * speed) / border;
      if (mx > x2-border) dx = -((border - (x2 - mx)) * speed) / border;
      if (my < y1+border) dy =  ((border - (my - y1)) * speed) / border;
      if (my > y2-border) dy = -((border - (y2 - my)) * speed) / border;

      // If was on scrollzone then check autoscrollCountdown and scroll if < 0:
      if (appStyle) startDelay = appStyle->autoScrollStartDelay;
      if (dx != 0 || dy != 0) {
        // On scrollzone:
        autoscrollCountdown -= diftime;
        if (autoscrollCountdown < 0) {
          autoscrollSpeedFactor += TTGUI_AUTOSCROLL_ACCELERATION;
          if (autoscrollSpeedFactor > 1) autoscrollSpeedFactor = 1;
          autoscrollCountdown = -TTGUI_AUTOSCROLL_STAYDELAY;
          // Scroll:
          setScrollOffset( getScrollXOffs() - (int)(dx*autoscrollSpeedFactor),
                           getScrollYOffs() - (int)(dy*autoscrollSpeedFactor) );
        }
      }
      else {
        // Not on scrollzone:
        //autoscrollSpeedFactor -= TTGUI_AUTOSCROLL_ACCELERATION;
        //if (autoscrollSpeedFactor < 0) autoscrollSpeedFactor = 0;
        autoscrollSpeedFactor = 0.2;
        autoscrollCountdown += diftime;
        if (autoscrollCountdown > startDelay) autoscrollCountdown = startDelay;
      }

    } else {
      // Not inside window:
      //autoscrollSpeedFactor -= TTGUI_AUTOSCROLL_ACCELERATION;
      //if (autoscrollSpeedFactor < 0) autoscrollSpeedFactor = 0;
      autoscrollSpeedFactor = 0.2;
      autoscrollCountdown += diftime;
      if (autoscrollCountdown > startDelay) autoscrollCountdown = startDelay;
    }

    // Update autoscrollTimer for next call:
    autoscrollTimer = curtime;

  } // end autoscroll.
  */

} // doTurn

/*--------------------------------------------------------------------------*/
TTWindow *TTWindow::getWindowAt( int x, int y ) {
// Returns window at specified coordinates, NULL if none.
// If coordinates are outside the window return null.
// Else check all childrens, topmost first, if none of them,
// then return this.

  // If this window is beeing dragged then assume it isn't
  // really 'here' and return NULL.  This is a bit kludgy...  Hmm.  ****
  if (mouseDragButton && draggedWin == this) return NULL;

  // Check if point inside this window:
  if ( x < getScrX() || x >= getScrX() + getVisibleW() ||
       y < getScrY() || y >= getScrY() + getVisibleH() )
    return NULL;
  else
    return this;

} // getWindowAt


/*--------------------------------------------------------------------------*/
/*----[ Dragging: ]---------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
bool TTWindow::objectDragDrop( int mx, int my, int button, TTWindow *obj ) {
// Called when an other window object is dragged and dropped on this
// window.  x and y are the mouse coordinates and button is the mouse
// button used in the dragging.  obj is the window object dropped.
// Returns true if the object is accepted, false if the drag should be
// cancelled and the object returned to its previous host and coordinates.

  if (!obj) return false;

  // Ok if object just moved and that is allowed:
  if (obj->movable && draggedOriginalHost == this) return true;

  // Ok if object drag&dropped and that is allowed:
  if (obj->dragable && draggedOriginalHost != this) return true;

  // Nope, send back to startpoint:
  return false;
  
} // objectDragDrop

//----------------------------------------------------------------------------
/*** (no TTMouseCursor yet)  **Note: have a flag indicating whether to call this function or not, to save some time??
TTMouseCursor *TTWindow::objectDragOver( int x, int y, int button, TTWindow *obj ) {
// Called when an other window object is dragged over this window.
// x and y are the mouse coordinates and button is the mouse
// button used in the dragging.  obj is the window object beeing dragged.
// Returns the cursor that the mouse cursor should change into,
// or NULL for no change.

} // objectDragOver
*/
    
/*--------------------------------------------------------------------------*/
/*----[ High level keyboard events: ]---------------------------------------*/
/*--------------------------------------------------------------------------*/
bool TTWindow::keyPress( int key ) {
// A key was pressed.
// key is the value returned by allegros readkey -function.
// (it contains both scancode and ascii info.)
// Returns true if the keypress was processed.
// Override the event methods in derived classes when neccesary.
  return false;
} // keyPress

/*--------------------------------------------------------------------------*/
bool TTWindow::captureKeyboard() {
// Captures the keyboard events, all keyboard events are sent to this window
// until releaseKeyboard is called.

  // Call application capture keyboard method:
  if (application)
    return application->captureKeyboard(this);
  else
    return true; // Hmm..??  what would be best here?

} // captureKeyboard

/*--------------------------------------------------------------------------*/
void TTWindow::releaseKeyboard() {
// Releases the keyboard if it was captured by captureKeyboard().

  // Call application release keyboard method:
  if (application) application->releaseKeyboard(this);

} // releaseKeyboard

/*--------------------------------------------------------------------------*/
bool TTWindow::hasCapturedKeyboard() {
  if (application)
    return application->getKeyboardCaptureWindow() == this;
  else
    return true; // No application -> assume true???

} // hasCapturedKeyboard


/*--------------------------------------------------------------------------*/
/*----[ Low level keyboard events: ]------------------------------------*/
/*--------------------------------------------------------------------------*/
/*
void TTWindow::keyDown( int key ) {
// A key was pressed down.
// key is the value returned by allegros readkey -function.
// (it contains both scancode and ascii info.)

  // Make keyPress event:
  keyPress( key );
} // keyDown
*/
/*--------------------------------------------------------------------------*/
/*
void TTWindow::keyUp( int key ) {
// A key was released.
// key is the value returned by allegros readkey -function.
// (it contains both scancode and ascii info.)
} // keyUp
*/

/*--------------------------------------------------------------------------*/
/*----[ High level mouse events: ]--------------------------------------*/
/*--------------------------------------------------------------------------*/
void TTWindow::mouseClick( int mx, int my, int button, TTWindow *win ) {
// A mouseclick.  button is 1 for left, 2 for right and 3 for middle.
// win is the window at position x, y (normally 'this').

} // mouseClick

/*--------------------------------------------------------------------------*/
void TTWindow::mouseDblClick( int mx, int my, int button, TTWindow *win ) {
// A mouse button was double-clicked.
// button is 1 for left, 2 for right and 3 for middle.
// win is the window at position x, y (normally 'this').

} // mouseDblClick

//----------------------------------------------------------------------------
bool TTWindow::mouseDragStart( int mx, int my, int button, TTWindow *obj ) {
// The window object was dragged with some mouse button
// (=mouse button was pressed and mouse was moved more than a few pixels).
// obj is the dragged window, button is the dragging mouse button.
// Returns true if it is ok to start the dragging (the window object
// captures the mouse and moves when the mouse is moved).
// False if the drag should be ignored.

  // Return if window object can't be moved or dragged:
  if (!movable && !dragable) return false;

  // If already dragging then release old drag;
  if (mouseDragButton) mouseDragCancel( mx, my, button, NULL/*?*/ );

  // Capture mouse if possible:
//****  if (!captureMouse()) return false;  !*** Is this neccesary? maybe not.

  // Init drag variables:
  mouseDragButton     = button;  // Works as dragging-on flag too.
                                 // Should we use a separate bool for that?
  draggedWin          = obj;
  draggedOriginalX    = draggedWin->getScrX();
  draggedOriginalY    = draggedWin->getScrY();
  draggedOriginalHost = draggedWin->getParent();

  // Get offset from mouse pointer to upper left corner of dragged window:
  draggedMouseXOffs   = draggedWin->getScrX() - prevMouseX;
  draggedMouseYOffs   = draggedWin->getScrY() - prevMouseY;

  // If dragging to other window objects is posible move dragged window
  // to application main window:
  if ( dragable && draggedWin->getApplication() ) {
    TTWindow *mainWin = draggedWin->getApplication()->getMainWindow();

    // Make sure mainWin has the addWindow-method and move the dragged
    // window to uppermost in the main window:
    // (Use typecasting.. can't be avoided.)
    if (mainWin && mainWin->inherits("TTForm")) {
      TTForm *p = (TTForm*)mainWin;
      p->addWindow( draggedWin );
    }
  }
  else if ( movable && draggedWin->getParent() &&
            draggedWin->getParent()->inherits("TTForm")) {
    // Move to front of all other gui objects under this parent,
    // To make it easier to move:
    TTForm *p = (TTForm*)draggedWin->getParent();
    p->removeWindow( draggedWin );
    p->addWindow( draggedWin );
  }


  // Set mouse cursor to drag cursor of the dragged window:

  /****** !! Implement !! *********/

  return true;
} // mouseDragStart

//----------------------------------------------------------------------------
void TTWindow::mouseDragCancel( int mx, int my, int button, TTWindow *win ) {
// The dragging was canceled by pressing any other mouse button than
// the one that started the drag.  The window object is
// returned to its previous host object and coordinates.
// Button is the button pressed to cancel the drag.

  // Move dragged win to previous host and coordinates:
  if (draggedWin) {

    // Move window back to original host if it was moved away:
    if ( draggedOriginalHost && draggedOriginalHost != draggedWin->getParent() &&
         draggedOriginalHost->inherits("TTForm")) {
      TTForm *p = (TTForm*)draggedOriginalHost;
      p->addWindow( draggedWin );
    }

    // Restore original location:
    draggedWin->setScr(draggedOriginalX, draggedOriginalY);
  }

  // Display outline animation that zooms from the drag position back to the
  // old position:
  
  /****** !! Implement !! *********/

  // Update mouse cursor pic:

  /****** !! Implement !! *********/
  
  // Clear drag flag:
  mouseDragButton = 0;
  draggedWin      = NULL;

} // mouseDragCancel

//----------------------------------------------------------------------------
bool TTWindow::mouseDragEnd( int mx, int my, int button, TTWindow *dest ) {
// The mouse button used to drag the window object was released.
// 'dest' is the window object under the mouse (and the dragged window).
// Return true if the drag is accepted, false if the dragged window should
// be returned to its previous host an coordinates.
// Calls the objectDragDrop method of destination window,
// and cancells the drag if destination->objectDragDrop returns false.

  // If the destination doesn't exist (outside any form, etc.) then cancel drag:
  if (!dest) return false;
  
  // Call objectDragDrop of destination...**
  if (!dest->objectDragDrop( mx, my, button, draggedWin ))
    return false;

  // Update mouse cursor pic:

  /****** !! Implement !! *********/
  
  // Clear drag flag:
  mouseDragButton = 0;
  draggedWin      = NULL;

  return false;
} // mouseDragEnd

/*--------------------------------------------------------------------------*/
void TTWindow::mouseEnter( int mx, int my, TTWindow *win ) {
// The mouse has just entered win.
// win is the window at position x, y (normally 'this').

  //setFocus(true);
} // mouseEnter

/*--------------------------------------------------------------------------*/
void TTWindow::mouseExit( int mx, int my, TTWindow *win ) {
// The mouse has just left win.
// win is the window at position x, y (normally 'this').

  //setFocus(false);
} // mouseExit

/*--------------------------------------------------------------------------*/
bool TTWindow::captureMouse() {
// Captures the mouse events, all mouse events are sent to this window
// until releaseMouse is called.

  // Call application capture mouse method:
  if (application)
    return application->captureMouse(this);
  else
    return true; // Hmm..??  what would be best here?

} // captureMouse

/*--------------------------------------------------------------------------*/
void TTWindow::releaseMouse() {
// Releases the mouse if it was captured by captureMouse().

  // Call application release mouse method:
  if (application) application->releaseMouse(this);

} // releaseMouse

/*--------------------------------------------------------------------------*/
bool TTWindow::hasCapturedMouse() {
  if (application)
    return application->getMouseCaptureWindow() == this;
  else
    return true; // No application -> assume true???

} // hasCapturedMouse


/*--------------------------------------------------------------------------*/
/*----[ Low level mouse events: ]---------------------------------------*/
/*--------------------------------------------------------------------------*/
void TTWindow::mouseDown( int mx, int my, int button, TTWindow *win ) {
// A mouse button was pressed.
// button is 1 for left, 2 for right and 3 for middle.
// win is the window at position x, y (normally 'this').

  // Check for double click:
  time_t dblClickTime = TTGUI_DBLCLICK_TIME;
  if (style.globalSettings)
    dblClickTime = style.globalSettings->dblClickTime;

  time_t c = clock();
  if ( c                       >= prevMouseDownTime   &&
       c - prevMouseDownTime   <= dblClickTime        &&
       ABS( prevMouseX - mx )  <= TTGUI_DBLCLICK_DIST &&
       ABS( prevMouseY - my )  <= TTGUI_DBLCLICK_DIST &&
       prevMouseButton         == button              &&
       prevMouseClickWasDouble == false  ) {
    prevMouseClickWasDouble = true;
    mouseDblClick( mx, my, button, win );
  }

  prevMouseDownTime = clock();
  prevMouseButton   = button;
  prevMouseX        = mx;
  prevMouseY        = my;

  // Update mouseButtonDown (used for drag detection):
  if (mouseButtonDown) mouseButtonDown = -1;
  else {
    mouseButtonDown = button;
    mouseDownWin    = win;
  }

  // Check for drag canceling:
  if (mouseDragButton) {
    mouseDragButton = 0;
    mouseDragCancel( mx, my, button, win );
  }

} // mouseDown

/*--------------------------------------------------------------------------*/
void TTWindow::mouseUp( int mx, int my, int button, TTWindow *win ) {
// A mouse button was released.
// button is 1 for left, 2 for right and 3 for middle.
// win is the window at position x, y (normally 'this').

  // Make mouseclick event:
  if (prevMouseClickWasDouble) {
    prevMouseClickWasDouble = false;
    // Click anyway if it wasn't the same button used for the dbl click:
    if (button != prevMouseButton)
      mouseClick( mx, my, button, win );
  }
  else mouseClick( mx, my, button, win );

  // Update mouseButtonDown (used for drag detection):
  if (mouseButtonDown == button || !mouse_b) mouseButtonDown = 0;

  // Check for drag drop:
  if (mouseDragButton && mouseDragButton == button) {
    mouseDragButton = 0;
    // Drop and cancel drag if drop rejected:
    if (!mouseDragEnd( mx, my, button, win))
      mouseDragCancel( mx, my, button, win );
  }

} // mouseUp

/*--------------------------------------------------------------------------*/
void TTWindow::mouseMove( int mx, int my, TTWindow *win ) {
// The mouse was moved.  x and y are the new mouse coordinates.
// win is the window at position x, y (normally 'this').
// Check for dragging and calls mouseDragStart if neccesary.


  // Check if mouse moved far enough with button pressed to start drag:
  if (mouseButtonDown > 0 && mouseDownWin &&
       ( abs(mx-prevMouseX) >= TTGUI_DRAGSTART_DIST ||
         abs(my-prevMouseY) >= TTGUI_DRAGSTART_DIST ) ) {
    // Start drag:
    mouseDragStart( mx, my, mouseButtonDown, mouseDownWin );
  } // end drag start.

  
  // Move dragged window if dragging is on:
  if (mouseDragButton && draggedWin) {
    draggedWin->setScr( mx + draggedMouseXOffs, my + draggedMouseYOffs );

    // Set mouse cursor if required:
    if (win) {
      /***! Cursor not implemented yet..
      TTCursor *p = win->objectDragOver( mx, my, mouseDragButton, draggedWin );
      if (p) p->set();
      */
    }
  }
} // mouseMove


/*--------------------------------------------------------------------------*/
/*----[ Color fades: ]------------------------------------------------------*/
/*--------------------------------------------------------------------------*/

void TTWindow::fadeColor( int *variable, int destColor, int time_ms,
                          bool immediate, bool repeat ) {
// Fade the colorvalue stored in variable to destColor in time_ms milliseconds.  
// If there is new color fades for a variable while it is still fading, 
// it completes the first colorfade before beginning on the next.
// If immediate is true the colorfade start immediately, removing all
// previous fades for the variable.  If repeat is true the colorfade is
// not removed from the queue when its done, but will repeat again
// when the other colorfades for that variable have performed.

  // Check parameters:
  if (!variable) return;

  // Create new fadecolor entry (use p to avoid unnecesary warning messages):
  TTFadeColorEntry *p;
  p = new TTFadeColorEntry( colorFades, *variable, destColor, time_ms,
                            immediate, repeat );

} // fadeColor

/*--------------------------------------------------------------------------*/
void TTWindow::doColorFades() {
// Updates the active colorfades.

  // Loop through all active fadecolor entries:
  TTFadeColorEntry *p = NULL;
  TTFadeColorEntry *q = NULL;
  colorFades.first();
  while( !colorFades.isOutside() ) {
    p = colorFades.current();

    // Skip waiting entries:
    if (!p || !p->active) {
      colorFades.next();
      continue;
    }

    // If time <= 0 then change color instantly:
    float pos;
    if (p->length_ms <= 0)
      pos = 1;
    else // Calculate new color:
      pos = ((float)ttGetMsTimer() - (float)(p->startTime)) / (float)(p->length_ms);
    if (pos < 0 || pos > 1) pos = 1; // Take care of timer overroll/overflow.

    // Check if there was any change since last time:
    if (pos == p->oldPos) {
      colorFades.next();
      continue;
    }
    p->oldPos = pos; // Store new pos value.

    // Calculate rgb:
    int r = p->startR + (int)(p->deltaR * pos);
    int g = p->startG + (int)(p->deltaG * pos);
    int b = p->startB + (int)(p->deltaB * pos);
    *(p->variable) = makecol( r, g, b );

    // Check if ready:
    if (pos >= 1) {

      // Move to next element in list and store a pointer to it:
      colorFades.next();
      q = colorFades.current();

      // Activate first waiting entry that modifies the same variable:
      bool found = false;
      for (; !colorFades.isOutside(); colorFades.next())
        if ( colorFades.current() &&
             colorFades.current()->variable == p->variable) {
          // Init fields & start colorfade (sets to active etc):
          colorFades.current()->initFields();
          found = true;
          break;
        }
      // (Loop around list and check items 0 to p-1 too if not found above: )
      if (!found)
        for (  colorFades.first();
              !colorFades.isOutside() && colorFades.current() != p;
              colorFades.next() )
          if ( colorFades.current() &&
               colorFades.current()->variable == p->variable) {
            // Init fields & start colorfade (sets to active etc):
            colorFades.current()->initFields();
            break;
          }

      // If repeat is true then set old entry to inactive,
      // else delete the old entry:
      if (p->repeat) {
        p->active = false;
      }
      else
        delete p;

      // Move to previously stored next list element:
      colorFades.getIndex(q);

    }
    else colorFades.next(); // Move forward in list.
  } // endwhile

} // doColorFades

/*--------------------------------------------------------------------------*/
/*----[ Scrolling: ]--------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
/*
void TTWindow::setScrollOffset( int nx, int ny ) {
// Sets offset of upper left corner inside window.  Checks boundaries.
  if (scrollXOffs == nx && scrollYOffs == ny) return;
  scrollXOffs  = nx;
  scrollYOffs  = ny;
  checkBoundaries();
} // setScrollXOffs

void TTWindow::scrollX( int deltaX ) {
// Scrolls window.  Checks boundaries.
  if (deltaX == 0) return;
  setScrollOffset( scrollXOffs + deltaX, scrollYOffs );
} // scrollX

void TTWindow::scrollY( int deltaY ) {
// Scrolls window.  Checks boundaries.
  if (deltaY == 0) return;
  setScrollOffset( scrollXOffs, scrollYOffs + deltaY );
} // scrollY

*/
/*--------------------------------------------------------------------------*/
/*
void TTWindow::checkBoundaries() {
// Checks scroll offset, virtual h/w and w/h and makes sure
// the visible part of the window is inside the virtual window.
// Uppdates pointed window and forces redraw of windows.
// Called by the above scroll methods.

  // Check virtual w/h:
  if (virtualW < TTGUI_WIN_MIN_WIDTH ) virtualW = TTGUI_WIN_MIN_WIDTH;
  if (virtualH < TTGUI_WIN_MIN_HEIGHT) virtualH = TTGUI_WIN_MIN_HEIGHT;
  if (w        < TTGUI_WIN_MIN_WIDTH ) w        = TTGUI_WIN_MIN_WIDTH;
  if (h        < TTGUI_WIN_MIN_HEIGHT) h        = TTGUI_WIN_MIN_HEIGHT;


  // Check if negative scroll offs:
  if (scrollXOffs < 0) scrollXOffs = 0;
  if (scrollYOffs < 0) scrollYOffs = 0;

  // Check if too big scroll offs:
  if (scrollXOffs + w > virtualW) scrollXOffs = virtualW - w;
  if (scrollYOffs + h > virtualH) scrollYOffs = virtualH - h;

  // Check if too big window (bigger than virtual window):
  if (scrollXOffs < 0) {
    scrollXOffs = 0;
    w = virtualW;
  }
  if (scrollYOffs < 0) {
    scrollYOffs = 0;
    h = virtualH;
  }

  // Update pointed window:
  if (application) application->updatePointedWindow( mouse_x, mouse_y );

  // Force redraw:
  if (parent) parent->redrawAll = true;
  redrawAll = true;

} // checkBoundaries
*/
/*--------------------------------------------------------------------------*/
/*----[ Style: ]------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/

void TTWindow::initStyle( TTGuiStyle *style_ ) {
// Copies the given TTGuiStyle, or, if it's NULL,
// copies the global style settings, modifies them,
// and calls the initStyle of its child-windows if it has any.
// Called automatically on creation.

  // Copy given or global style:
  if (style_)
    style = *style_;
  else if (getApplication() && getApplication()->getStyle())
    style = getApplication()->getStyle()->defaultGuiStyle;
  // (else use the default values given at style creation..)

  int bw = ttBorderWidth(this);
  setBorderWidths( bw, bw, bw, bw );

} // initStyle

/*--------------------------------------------------------------------------*/
/*----[ Accessors: ]--------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
bool TTWindow::hasFocus() {

  if (application)
    return application->getFocusedWindow() == this;
  else
    return true; // Assume true if no application specified.

} // hasFocus

/*--------------------------------------------------------------------------*/
void TTWindow::setFocus( bool focus_ ) {

  if (application && focus_) {
    // Check if already focused:
    if (application->getFocusedWindow() == this) return;
    // Unfocus previous:
    if (application->getFocusedWindow())
      application->getFocusedWindow()->setFocus(false);
    // Focus this:
    application->setFocusedWindow(this);
    select(this);
  }
  else if (application && !focus_) {
    // Check if already not focused:
    if (application->getFocusedWindow() != this) return;
    // Unfocus (set focused win to NULL:
    application->setFocusedWindow(NULL);
  }

  // Manage focus outline color fading:
  int speed = 500; // Hardwired emergency default.
  if (getApplicationStyle())
    speed = getApplicationStyle()->focusOutlineSpeed;

  if (hasFocus()) {
    // Fade from start to mid to final focus outline color:
    fadeColor( &focusColor, getStartFocusColor(), 0, true);
    fadeColor( &focusColor, getMidFocusColor(),   speed * 0.33 );
    fadeColor( &focusColor, getFinalFocusColor(), speed * 0.66 );
  }
  else {
    // Fade to OutFocusColor and change to normal background:
    //fadeColor( &focusColor, getOutFocusColor(), speed, true );
    fadeColor( &focusColor, getBgFocusColor(),  speed, true );
  }

} // setFocus

/*--------------------------------------------------------------------------*/
TTApplicationStyle *TTWindow::getApplicationStyle() {

  return style.globalSettings;
  //if (application) return application->getStyle();
  //else return NULL;
} // getApplicationStyle

/*--------------------------------------------------------------------------*/
bool TTWindow::mouseIsOver() {

  if (application) return application->getPointedWindow() == this;
  else return getWindowAt(mouse_x, mouse_y) == this;
} // mouseIsOver

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
/*
void TTWindow::setX(int newx) {
  if (newx == x) return;
  x = newx;
  if (application) application->updatePointedWindow( mouse_x, mouse_y );
  if (parent) parent->redrawAll = true;
  redrawAll = true;
}

void TTWindow::setY(int newy) {
  if (newy == y) return;
  y = newy;
  if (application) application->updatePointedWindow( mouse_x, mouse_y );
  if (parent) parent->redrawAll = true;
  redrawAll = true;
}

void TTWindow::setW(int neww) {
  if (neww == w) return;
  w = neww;
  checkBoundaries();
}

void TTWindow::setH(int newh) {
  if (newh == h) return;
  h = newh;
  checkBoundaries();
}

void TTWindow::setVirtualW(int newvw) {
  if (newvw == virtualW) return;
  virtualW = newvw;
  checkBoundaries();
}

void TTWindow::setVirtualH(int newvh) {
  if (newvh == virtualH) return;
  virtualH = newvh;
  checkBoundaries();
}

void TTWindow::setXOffs(int newxoffs) {
  if (newxoffs == xoffs) return;
  xoffs     = newxoffs;
  redrawAll = true;
}

void TTWindow::setYOffs(int newyoffs) {
  if (newyoffs == yoffs) return;
  yoffs     = newyoffs;
  redrawAll = true;
}
*/

//----------------------------------------------------------------------------
//----[ Scrolling: ]----------------------------------------------------------
//----------------------------------------------------------------------------
void TTWindow::scrollTo( int x, int y, long timeInMs ) {
// Softly scrolls to the part of the virtual area that starts at x,y
// in specified number of milliseconds.
// For example scrollTo( 0, 0, 1000) scrolls to the upper left corner
// of the virtual area from current position using exactly one second.
// A new scroll command can be given anytime without problems.

} // scrollTo

//----------------------------------------------------------------------------
void TTWindow::scrollToPosWithSpeed( int x, int y, float pixelsPerSecond ) {
// Softly scrolls to the part of the virtual area that starts at x,y
// with specified speed.
// For example scrollTo( 0, 0, 100) scrolls to the upper left corner
// of the virtual area from current position with the average speed
// of 100 pixels per second (it still starts and stops softly, though).
// A new scroll command can be given anytime without problems.

} // scrollToPosWithSpeed

//----------------------------------------------------------------------------
void TTWindow::setScrollDelta( float dx, float dy ) {
// Stops any ongoing scrolling and sets fixed scroll velocity.
// Can be used to implement custom scrolling.
// Automatically stops when it hits edge of visible screen.

  scrollInProgress = false;
  scrollDX         = dx;
  scrollDY         = dy;

} // setScrollDelta

//----------------------------------------------------------------------------
void TTWindow::doScroll() {
// Does the scrolling.  This method is called from doTurn().

  if (scrollInProgress) {
    /*
      ***** ! implement ! *****
    */
  }
  else if (scrollDX != 0 || scrollDY != 0) {
    // User scroll:
    float oldsx = getScrollX(),
          oldsy = getScrollY();
        
    setScroll( getScrollX() + scrollDX, getScrollY() + scrollDY );

    // Check hit edge of scrollwindow:
    if (oldsx == getScrollX()) scrollDX = 0;
    if (oldsy == getScrollY()) scrollDY = 0;
  }

} // doScroll

//----------------------------------------------------------------------------
//----[ Checking: ]-----------------------------------------------------------
//----------------------------------------------------------------------------

void TTWindow::updateScrCoordinates( int parentScrBorX, int parentScrBorY ) {
// This method is called by the parent when it has moved or scrolled
// (or any of its parents moved or scrolled).
// 'parentScrBorX/Y' are the scrX/Y coordinates of the parent added
// to the borderWidthLeft and Top, to give the absolute screen coordinates
// of the topleft of the visible scrollable area (hope that made sense).
// The scrX/Y variables of this window object get updated (by adding
// posX/Y of this object to parentScrBorX/Y).
// Calls updateChildCoordinates to update child objects coordinates also.

  scrX = parentScrBorX + posX;  // Hmm.. why didn't I use setScr() ?
  scrY = parentScrBorY + posY;  // I'm sure I had a good reason...

  updateChildCoordinates();

} // updateScrCoordinates

//----------------------------------------------------------------------------
void TTWindow::checkCoordinates() {
// Checks that all coordinates, sizes and borders are in range and
// correct.  Corrects what it can (sets correct borderWidthTop/... for
// example).  Might be called after changing style stuff or something.

  // Get border sizes..

  // Get parent scrX/Y and set own scrX/Y ?? Hmm? neccesary???

  // Check scrollX/Y:
  checkVirtualAreaVisibility();

  // Call checkCoordinates of child/contained objects if anything changed (or just always..)
  
} // checkCoordinates

//----------------------------------------------------------------------------
void TTWindow::checkVirtualAreaVisibility() {
// This is just a helper function used by the coordinate and size setters
// to make sure that as much as posible of the virtual area is in the
// visible area, and that scrollX/Y always stays positive or zero.
  float dif, oldsx = scrollX, oldsy = scrollY;

  // Check if 'empty' room in bottom or right part of visible area:
  dif = (virtualW + scrollX) - (visibleW - borderWidthLeft - borderWidthRight);
  if (dif < 0) scrollX += dif;
  dif = (virtualH + scrollY) - (visibleH - borderWidthTop - borderWidthBottom);
  if (dif < 0) scrollY += dif;
  
  // Check if 'empty' room in top or left part of visible area:
  if (scrollX < 0) scrollX = 0;
  if (scrollY < 0) scrollY = 0;

  // Update scrX/Y of all child/containde objects if scroll changed:
  if (oldsx != scrollX || oldsy != scrollY) {
    updateChildCoordinates();    
  }

} // checkVirtualAreaVisibility

//----------------------------------------------------------------------------
//----[ Coordinate stuff ]----------------------------------------------------
//----------------------------------------------------------------------------
void TTWindow::setPos( int x, int y ) {
// Sets the offset from parents topleft visible area (+borders) to
// the upperleft corner of the visible area of this window object.

  if (posX == x && posY == y) return;
  
  posX = x;
  posY = y;

  // Update scr:
  if (parent) {
    scrX = parent->getScrX() + parent->getBorderWidthLeft() + posX - (int)parent->getScrollX();
    scrY = parent->getScrY() + parent->getBorderWidthTop()  + posY - (int)parent->getScrollY();
  }
  else {
    scrX = posX;
    scrY = posY;
  }

  // Move child objects:
  updateChildCoordinates();

  // Recalculate the window that is under the mouse:
  if (application) application->updatePointedWindow( mouse_x, mouse_y );

} // setPos

//----------------------------------------------------------------------------
void TTWindow::setScr( int x, int y ) {
// Sets the absolute screen coordinates for the upperleft corner
// of the visible area.

  if (scrX == x && scrY == y) return;

  // Change pos so that scrX/Y become x/y.
  setPos( posX + x - scrX, posY + y - scrY );

} // setScr

//----------------------------------------------------------------------------
void TTWindow::setScroll( float x, int y ) {
// Sets the offset from the topleft corner of the virtual area to the
// topleft corner of the visible area.

  //! Searching for bug:
  scrollX = scrollY = 0;

/*! Disabled because of a bug that caused the editbox to scroll away the text.
 *! Still haven't found the bug...  must be someone who incorrectly clls setScroll.
  if (x < 0) x = 0;
  if (y < 0) y = 0;

  if (scrollX == x && scrollY == y) return;

  scrollX = x;
  scrollY = y;
*/
  // Move child objects:
  updateChildCoordinates();

} // setScroll

//----------------------------------------------------------------------------
void TTWindow::setVisibleAreaSize( int w, int h ) {
// Sets the size of the visible area (the size of the visible
// (not counting overlapping etc.) window on screen, including borders).

  if (visibleW == w && visibleH == h) return;

  visibleW = w;
  visibleH = h;

  checkVirtualAreaVisibility();

} // setVisibleAreaSize

//----------------------------------------------------------------------------
void TTWindow::setVirtualAreaSize( int w, int h ) {
// Sets the size of the virtual area.  That is the atrea you can scroll
// around, but of which only a part might be visible at a time
// (in the visible window).

  if (virtualW == w && virtualH == h) return;

  virtualW = w;
  virtualH = h;

  checkVirtualAreaVisibility();

  // (Move all child/contained objects that were left outside.)
  // (if their posX/Y is >= than virtualW/H set it to virtualW/H - their visualW/H,
  // or zero, whichever is bigger.
  updateChildCoordinates(); //! Hmm.. does this routine check for that?

} // setVirtualAreaSize

//----------------------------------------------------------------------------
void TTWindow::setBorderWidths( int top, int bottom, int left, int right ) {
// Sets the total amount of border in pixels at the (inner) edges of the
// visible screen.  This border should include title and scrollbars and
// such things as well.  Note that this method doesn't alter the border
// appearance, normally the library user doesn't have to call this directly.

  int oldTop  = borderWidthTop;
  int oldLeft = borderWidthLeft;

  if (top    < 0) top    = 0;
  if (bottom < 0) bottom = 0;
  if (left   < 0) left   = 0;
  if (right  < 0) right  = 0;

  if ( borderWidthTop  == top  && borderWidthBottom == bottom &&
       borderWidthLeft == left && borderWidthRight  == right ) return;

  borderWidthTop    = top;
  borderWidthBottom = bottom;
  borderWidthLeft   = left;
  borderWidthRight  = right;

  checkVirtualAreaVisibility();

  // Update all child object scrX/Y if the top or left borders changed:
  if (oldTop != borderWidthTop || oldLeft != borderWidthLeft)
    updateChildCoordinates();

} // setBorderWidths

/*--------------------------------------------------------------------------*/
// End of class TTWindow.
/*--------------------------------------------------------------------------*/

