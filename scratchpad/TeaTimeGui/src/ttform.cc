//////////////////////////////////////////////////////////////////////////////
//   File name:  ttform.cc
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
//   Work diary:
// Date      Time     Lines   Things done
// ~~~~~~~~~ ~~~~~~~~ ~~~~~~~ ~~~~~~~~~~~~~~~~~~~~~~
// 25.5.1998 3:00     339     Implementation.
// 26.5      1:20     416     Added arrow navigation and did small fixes.
// ..
// 12.9      0:40     480     Added scrolling.
//
//
// Notes & things to do:
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// * Specifying a parent in the TTWindow constructor won't add it to the parents
//   child list.  I say we remove the parent parameter from the constructor.
//
// * Check that win->parent is a TTForm or decendant in addWindow()..
//
//////////////////////////////////////////////////////////////////////////////


#include "ttform.h"

/*--------------------------------------------------------------------------*/
// class TTForm
/*--------------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/*----[ Constructors & destructor: ]------------------------------------*/
/*----------------------------------------------------------------------*/
TTForm::TTForm( TTApplication *application_ = NULL )
  : TTWindow (application_), childWindows() {
// Default constructor.

  selectedChild           = NULL;
  showFocusOutline        = false;
  useParentColorForBorder = false;
  
  movable                 = true;

  initStyle();
}

/*--------------------------------------------------------------------------*/
TTForm::TTForm( const TTForm &source )
  : TTWindow (source), childWindows() {
// Copy constructor.

  // Copy all child objects... Oh well..
  // Guess that isn't that important right now..

  selectedChild = NULL;
  showFocusOutline = false;
  useParentColorForBorder = source.useParentColorForBorder;

  initStyle();
}

/*--------------------------------------------------------------------------*/
TTForm::TTForm( int x_, int y_, int w_, int h_,
                TTWindow      *parent_      = NULL,
                TTApplication *application_ = NULL )
  : TTWindow (x_, y_, w_, h_, parent_, application_), childWindows()  {
// Constructor.  If application is null it uses parent application.

  selectedChild = NULL;
  showFocusOutline = false;
  useParentColorForBorder = false;

  initStyle();
}

/*--------------------------------------------------------------------------*/
TTForm::~TTForm() {
// Destructor.

  // Destroy all childs:
  childWindows.deleteAll();
  selectedChild = NULL; // just in case
}

/*----------------------------------------------------------------------*/
/*----[ Window management: ]--------------------------------------------*/
/*----------------------------------------------------------------------*/
void TTForm::addWindow( TTWindow *win ) {
  if (!win) return;
  if (childWindows.getIndex(win) != 0) return;

  // Remove from old place if neccesary:
  if (win->getParent() && win->getParent()->inherits("TTForm")) {
    // Check that win->parent really is a TTForm object.
    // It should be, if somebody hasn't fiddled directly with the parent field...    
    
    ((TTForm*)win->getParent())->removeWindow( win );
  }

  // Add to child windows:
  childWindows.addLast(win);
  win->setParent(this);
  if (win->getApplication() == NULL) win->setApplication( getApplication() );
  win->updateScrCoordinates( getScrX() + getBorderWidthLeft(),
                             getScrY() + getBorderWidthTop() );

  // Redraw..
  redrawAll = true;

} // addWindow

/*--------------------------------------------------------------------------*/
void TTForm::removeWindow( TTWindow *win ) {
  if (!win) return;
  if (childWindows.getIndex(win) == 0) return; //! PtrList indexing starts from one!..

  // Remove from childWindows:
  childWindows.remove(win);
  win->setParent(NULL);

  // Redraw..
  redrawAll = true;

} // removeWindow

/*----------------------------------------------------------------------*/
/*----[ Drawing and other: ]--------------------------------------------*/
/*----------------------------------------------------------------------*/
void TTForm::draw( BITMAP *dest ) {
// Draws the child windows.

  TTWindow::draw( dest );

  // Set clipping rectangle to just inside border:
  TTClipRect oldcr = ttGetClipRect( dest );
  ttSetClipRect( dest, getScrX() + getBorderWidthLeft(),
                       getScrX() + getVisibleW() - getBorderWidthRight(),
                       getScrY() + getBorderWidthTop(),
                       getScrY() + getVisibleH() - getBorderWidthBottom(),
                       true, true );

  // Draw child windows:
  TTWindow *p;
  for (p = childWindows.first(); !childWindows.isOutside(); p = childWindows.next())
    if (p) p->draw( dest );

  // Restore clipping:
  ttSetClipRect( dest, oldcr );

} // draw

/*--------------------------------------------------------------------------*/
bool TTForm::update( BITMAP *dest ) {
// Draws the parts of the window that have changed since last draw.
// Returns true if anything was drawn.

  if (dest == NULL) {
    redrawArea = redrawBorder = redrawText = redrawAll = false;
    return false;
  }

  // If redrawArea is true we have to redraw all children:
  bool redrawChildren = redrawArea;
  bool redrawn        = false;

  redrawn = TTWindow::update( dest );

  // Set clipping rectangle to just inside border:
  TTClipRect oldcr = ttGetClipRect( dest );
  ttSetClipRect( dest, getScrX() + getBorderWidthLeft(),
                       getScrX() + getVisibleW() - getBorderWidthRight(),
                       getScrY() + getBorderWidthTop(),
                       getScrY() + getVisibleH() - getBorderWidthBottom(),
                       true, true );
                       
  // Loop through the child window list and redraw:
  // If any child window is updated then we have to redraw the others
  // in case they overlap. Implement proper overlapping control later if
  // neccesary.
  TTWindow *p;
  for (p = childWindows.first(); !childWindows.isOutside(); p = childWindows.next()) {
    bool b = false;
    if (p) {
      if (redrawChildren) {
        p->draw( dest );
        redrawn = true;
      }
      else b = p->update( dest );
    }
    redrawn        = redrawn || b;
    redrawChildren = redrawChildren || b;
  }

  // Restore clipping:
  ttSetClipRect( dest, oldcr, false );

  return redrawn;

} // update

/*--------------------------------------------------------------------------*/
void TTForm::doTurn() {
// Sort of idle loop.  Gets called frequently.  Don't do anything too
// time consuming, or the system will slow down.

  TTWindow::doTurn();
  TTWindow *p;
  for (p = childWindows.first(); !childWindows.isOutside(); p = childWindows.next())
    if (p) {
      p->doTurn();
      // Restore current position in childWindows list if changed by any of
      // the called childs.. (neccesary? well.. no harm).
      if (childWindows.current() != p) childWindows.getIndex( p );
    }

} // doTurn

/*--------------------------------------------------------------------------*/
void TTForm::updateChildCoordinates() {
// Calls updateScrCoordinates for all child or contained objects.
// Called when the window moves or scrolls (or the parent moves or scrolls etc.).

  TTWindow *p;
  for (p = childWindows.first(); !childWindows.isOutside(); p = childWindows.next())
    if (p) p->updateScrCoordinates( getScrX() + getBorderWidthLeft(),
                                    getScrY() + getBorderWidthTop()  );
  
} // updateChildCoordinates

/*--------------------------------------------------------------------------*/
TTWindow *TTForm::getWindowAt( int x_, int y_ ) {
// Returns window at specified coordinates, NULL if none.
// If coordinates are outside the window return null.
// Else check all childrens, topmost first, if none of them,
// then return this.

  // Check if outside:
  if ( x_ < getScrX() || x_ >= getScrX() + getVisibleW() ||
       y_ < getScrY() || y_ >= getScrY() + getVisibleH() )
    return NULL;

  // Check if it was the border:
  if ( x_ <  getScrX() + getBorderWidthLeft() ||
       x_ >= getScrX() + getVisibleW() - getBorderWidthRight() ||
       y_ <  getScrY() + getBorderWidthTop()  ||
       y_ >= getScrY() + getVisibleH() - getBorderWidthBottom() )
    return this;

  // Check children:
  TTWindow *p;
  TTWindow *win = NULL;
  for (p = childWindows.last(); !childWindows.isOutside(); p = childWindows.prev()) {
    if (p) win = p->getWindowAt( x_, y_ );
    if (win) return win;
  }

  return this;

} // getWindowAt

/*----------------------------------------------------------------------*/
/*----[ High level keyboard events: ]-----------------------------------*/
/*----------------------------------------------------------------------*/
bool TTForm::keyPress( int c ) {
// A key was pressed.
// c is the value returned by allegros readkey -function.
// (it contains both scancode and ascii info.)
// Returns true if the keypress was processed.

  // Call parent keypress:
  TTWindow::keyPress(c);

  TTWindow *newSelected = NULL;

  // Check for TAB, SHIFT + TAB and ALT + ARROW KEYS(?):
  if ((c >> 8) == KEY_TAB) {
    childWindows.getIndex(selectedChild);
    TTWindow *p = childWindows.current();

    if (key[KEY_LSHIFT] || key[KEY_RSHIFT]) {
      p = childWindows.prev();
      if (p == NULL) p = childWindows.last();
      while ( p != selectedChild && p && (!p->hasTabstop() || !p->isEnabled()) || !p->isVisible() ) {
        p = childWindows.prev();
        if (p == NULL) p = childWindows.last();
      }
    }
    else {
      p = childWindows.next();
      if (p == NULL) p = childWindows.first();
      while ( p != selectedChild && p && (!p->hasTabstop() || !p->isEnabled()) || !p->isVisible() ) {
        p = childWindows.next();
        if (p == NULL) p = childWindows.first();
      }
    }
    newSelected = p;
    if (!newSelected) newSelected = selectedChild;
  } // if tab

  if (key[KEY_ALT]) {
    if ((c >> 8) == KEY_UP) newSelected = getNeighbour( selectedChild,  0, -1 );
    else if ((c >> 8) == KEY_DOWN) newSelected = getNeighbour( selectedChild,  0,  1 );
    else if ((c >> 8) == KEY_LEFT) newSelected = getNeighbour( selectedChild, -1,  0 );
    else if ((c >> 8) == KEY_RIGHT) newSelected = getNeighbour( selectedChild,  1,  0 );
  } // if alt

  // Set new selected child if it changed:
  if (newSelected) {
    selectedChild = newSelected;
    selectedChild->setFocus(true);
    return true;
  }

  // Call selected child keypress and return if the keypress was processed:
  if (selectedChild && selectedChild->keyPress( c )) return true;


  // Check arrow keys (moves to nearest child up/down/left/right):
  if      ((c >> 8) == KEY_UP)    newSelected = getNeighbour( selectedChild,  0, -1 );
  else if ((c >> 8) == KEY_DOWN)  newSelected = getNeighbour( selectedChild,  0,  1 );
  else if ((c >> 8) == KEY_LEFT)  newSelected = getNeighbour( selectedChild, -1,  0 );
  else if ((c >> 8) == KEY_RIGHT) newSelected = getNeighbour( selectedChild,  1,  0 );

  // Set new selected child if it changed:
  if (newSelected) {
    selectedChild = newSelected;
    selectedChild->setFocus(true);
    return true;
  }

  // Keypress not processed:
  return false;

} // keypress

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
TTWindow *TTForm::getNeighbour( TTWindow *win, int dx, int dy ) {
// Returns the closest neighbour to win in given direction.
// dx and dy should be -1, 0 or 1.
// If there is no neighbour in that direction it returns win.
  if (!win) return NULL;
  if (dx == 0 && dy == 0) return win;
  if (childWindows.size() <= 1) return win; // We're alone

  // 'Normalize' dx & dy:
  if      (dx > 0) dx =  1;
  else if (dx < 0) dx = -1;
  if      (dy > 0) dy =  1;
  else if (dy < 0) dy = -1;

  // Get centre of win:
  int wx = win->getPosX() + win->getVisibleW() / 2;
  int wy = win->getPosY() + win->getVisibleH() / 2;

  // Init closest:
  int       closest    = -1;
  TTWindow *closestWin = win;

  // Loop all children:
  TTWindow *p;
  for (p = childWindows.first(); !childWindows.isOutside(); p = childWindows.next()) {
    if (!p || p == win) continue;

    // Jump over gizmos that aren't visible, enabled and have tabstop(?hmm):
    if (!p->hasTabstop() || !p->isVisible() || !p->isEnabled()) continue;

    // Get child centre:
    int cx = p->getPosX() + p->getVisibleW() / 2;
    int cy = p->getPosY() + p->getVisibleH() / 2;

    // Continue if not in right direction:
    /*
    if      (dy == 0 && dx * (cx - wx) < ABS(cy-wy)) continue;
    else if (dx == 0 && dy * (cy - wy) < ABS(cx-wx)) continue;
    else if (dx * (cx - wx) < 0 || dy * (cy - wy) < 0) continue;
    */
    if      (dy == 0 && dx * (cx - wx) <= 0) continue;
    else if (dx == 0 && dy * (cy - wy) <= 0) continue;
    else if (dx * (cx - wx) < 0 || dy * (cy - wy) < 0) continue;

    // Calc distance, giving priority to windows that lie on a direct
    // line from win:
    int dist2 = (cx-wx) * (cx-wx) + (cy-wy) * (cy-wy);
    if      (dy == 0) dist2 += 8*(cy-wy) * (cy-wy);
    else if (dx == 0) dist2 += 8*(cx-wx) * (cx-wx);

    if (closest == -1 || dist2 <= closest) {
      closest    = dist2;
      closestWin = p;
    }
  } // end for

  return closestWin;

} // getNeighbour

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
void TTForm::select( TTWindow *win ) {
    // Makes win the selected window in forms and other multi child windows.
    // All keyboard input is directed to the selcted window unless the current
    // window doesn't have the focus.  Used to direct keyb. input to the
    // focused window.  This function will call the select function of its
    // parent too.

  int i = childWindows.getIndex(win);
  if (i != 0) {
    selectedChild = win;
    //! if (selectedChild) selectedChild->setFocus(true);
  }

  if (win) win->setFocus(true); //! Hmm.. will this fix the nested form selecting?

  if (parent) parent->select(this);

}

/*--------------------------------------------------------------------------*/
/*----[ Scrolling: ]--------------------------------------------------------*/
/*--------------------------------------------------------------------------*/

/*
void TTForm::setScrollOffset( int nx, int ny ) {
// Sets offset of upper left corner inside window.  Checks boundaries.
// Updates child windows position.
  if (scrollXOffs == nx && scrollYOffs == ny) return;
  TTWindow::setScrollOffset( nx, ny );

  // Update child windows:
  TTWindow *p;
  for (p = childWindows.first(); !childWindows.isOutside(); p = childWindows.next())
    if (p) {
      p->setXOffs( x - scrollXOffs );
      p->setYOffs( y - scrollYOffs );
    }
} // setScrollOffset
*/
/*--------------------------------------------------------------------------*/
/*
void TTForm::checkBoundaries() {
// Calls TTWindow::checkBoundaries(), and updates children.

  // Call inherited function:
  TTWindow::checkBoundaries();

  // Update child windows:
  TTWindow *p;
  for (p = childWindows.first(); !childWindows.isOutside(); p = childWindows.next())
    if (p) {
      p->setXOffs( x - scrollXOffs );
      p->setYOffs( y - scrollYOffs );
    }

} // checkBoundaries

*/
/*--------------------------------------------------------------------------*/
/*----[ Style: ]------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/

void TTForm::initStyle( TTGuiStyle *style_ ) {
// Copies the given TTGuiStyle, or, if it's NULL,
// copies the global style settings, modifies them,
// and calls the initStyle of its child-windows if it has any.
// Called automatically on creation.

  // Call parent:
  // TTWindow::initStyle(style_); Not neccesary, already called from constructor...

  // Call initStyle of children:
  TTWindow *p;
  for (p = childWindows.first(); !childWindows.isOutside(); p = childWindows.next())
    if (p) p->initStyle( style_ );

} // initStyle

/*----------------------------------------------------------------------*/
/*----[ Accessors: ]----------------------------------------------------*/
/*----------------------------------------------------------------------*/

//----------------------------------------------------------------------------
//virtual void setStyle( TTWindowStyle *style_ ) { style = style_; }
// Set style of contained/child objects also?
/*
void TTForm::setX(int newx) {
  if (x == newx) return;
  TTWindow::setX( newx );

  // Uppdate xoffs of childwindows:
  TTWindow *p;
  for (p = childWindows.first(); !childWindows.isOutside(); p = childWindows.next())
    if (p) p->setXOffs( x - scrollXOffs );

} // setX
*/
/*--------------------------------------------------------------------------*/
/*
void TTForm::setY(int newy) {
  if (y == newy) return;
  TTWindow::setY( newy );

  // Uppdate yoffs of childwindows:
  TTWindow *p;
  for (p = childWindows.first(); !childWindows.isOutside(); p = childWindows.next())
    if (p) p->setYOffs( y - scrollYOffs );

} // setY
*/
/*--------------------------------------------------------------------------*/
// End of class TTForm.
/*--------------------------------------------------------------------------*/


