//////////////////////////////////////////////////////////////////////////////
//
//  Class:     TTList
//  Project:   TTGui
//  Author:    Hans H„ggstr”m,  (hans.haggstrom@helsinki.fi)
//  Inherits:  TTForm
//  Decendants:(TTToolbar, TTPopupMenu, TTDropdownList ... etc)
//  Purpose:   One dimensional list of TTWindows with scroll capabilities.
//             Subclass this for toolbars, popup menus and dropdown lists.
//
//
//  Work diary:
//  Date     Hours  Lines  Things done
//  ~~~~~~~~ ~~~~~~ ~~~~~~ ~~~~~~~~~~~~~~~
//  06.02.99 1:00   173    Header and constructors/destructors.  Compiles.
//  06.02    2:00   331    On the screen!  Looks interesting.
//                         Have to add in scrolling, showing scrollbuttons
//                         only when neccesary and put back the min/max
//                         thickness/length in some form for the whole list(?).
//                         Possibly add dragging support (for reordering items?)  Hmmmm.... maybe not.
//                         And ad sorting and autosorting system.
//
//  Notes & things to do:
//  ~~~~~~~~~~~~~~~~~~~~~~~~
//   * Add function for setting sorting algorithm and sorting and
//     setting autosort on or off.
//
//////////////////////////////////////////////////////////////////////////////

#include "ttlist.h"

//----------------------------------------------------------------------------
// Class TTList
//----------------------------------------------------------------------------

TTList::TTList( TTApplication *application_ )
  : TTForm(application_) {
// Default constructor.

  direction = TTGUI_LIST_DIRECTION_HORIZONTAL;

/*
  minThickness = 0;
  maxThickness = 80; // Dumb hardwired default...
  minLength    = 0;
  maxLength    = 200; // Dumb hardwired default...
*/
//  selected     = -1;   // No selected item.
    
  highlightSelected = false;
  showScrollArrows  = true;
  itemSpacing       = 0;

  highlightStyle    = NULL; //! Where to get it from/what to use?

  scrollArrowLength = 16; // Dumb hardwired default..

  // Create scroll arrow buttons and add them to the window:
  scrollArrowEnd    = new TTButton( application_ );
  scrollArrowStart  = new TTButton( application_ );
  itemWindow        = new TTForm( application_ );

  if (itemWindow) {
    itemWindow->setMovable(false);
    itemWindow->setDragable(false);
    if (itemWindow->getWinStyle()) itemWindow->getWinStyle()->borderWidth = 0;
    itemWindow->setBorderWidths( 0, 0, 0, 0 );
  }
  
  addWindow( scrollArrowEnd );
  addWindow( scrollArrowStart );
  addWindow( itemWindow );

  // Adjust buttons and itemwindow:
  adjustList();

} // Default constructor.


//----------------------------------------------------------------------------
TTList::TTList( const TTList &source )
  : TTForm(source) {
// Copy constructor.

  //! Should the items be copied too?

  direction         = source.direction;

/*
  minThickness      = source.minThickness;
  maxThickness      = source.maxThickness;
  minLength         = source.minLength;
  maxLength         = source.maxLength;
*/
//!  selected          = -1; // no items..
    
  highlightSelected = source.highlightSelected;
  showScrollArrows  = source.showScrollArrows;

  highlightStyle    = source.highlightStyle;

  scrollArrowLength = source.scrollArrowLength;
  itemSpacing       = source.itemSpacing;

  // Copy scroll arrow buttons and add them to the window:
  if (source.scrollArrowEnd)
    scrollArrowEnd  = new TTButton( *source.scrollArrowEnd );
  else scrollArrowEnd = NULL;
    
  if (source.scrollArrowStart)
    scrollArrowStart  = new TTButton( *source.scrollArrowStart );
  else scrollArrowStart = NULL;
  
  if (source.itemWindow)
    itemWindow  = new TTForm( *source.itemWindow );
  else itemWindow = NULL;

  if (itemWindow) {
    itemWindow->setMovable(false);
    itemWindow->setDragable(false);
    if (itemWindow->getWinStyle()) itemWindow->getWinStyle()->borderWidth = 0;
    itemWindow->setBorderWidths( 0, 0, 0, 0 );
  }
  
  addWindow( scrollArrowEnd );
  addWindow( scrollArrowStart );
  addWindow( itemWindow );

  // Just for safety:
  adjustList();

} // Copy constructor.

//----------------------------------------------------------------------------
TTList::~TTList() {
// Destructor.

  // All contained items (including scrollbuttons) deleted in
  // TTForm destructor.

} // Destructor.

//----------------------------------------------------------------------------
void TTList::addItem( TTWindow *win ) {
// Adds win to last position in list.
// Removes win from any other places within the list first.
// Does nothing if win is NULL.
// Adjusts size of window to fit in limits given to list if neccesary.

  if (!itemWindow) return;
  
  // Add to item window:
  itemWindow->addWindow( win );
  
/*
  // Sort list if autosort is on:
  if (autosort) {
    childWindows.sort();

    // Move scrollbuttons back to their place at start of list:
    childWindows.remove( scrollArrowStart );
    childWindows.addFirst( scrollArrowStart );
    childWindows.remove( scrollArrowEnd );
    childWindows.addFirst( scrollArrowEnd );
  }
*/
  // Adjust list:
  adjustList();

} // addWindow

//----------------------------------------------------------------------------
void TTList::removeItem( TTWindow *win ) {
// Removes win from the list if it was in it.

  if (!itemWindow) return;
  
  // Remove from itemWindow:
  itemWindow->removeWindow( win );

  // Adjust size && position of all the other items:
  adjustList();

} // removeWindow

//----------------------------------------------------------------------------
void TTList::setItemIndex( TTWindow *win, int index ) {
// If win is in the list its position is changed to index (0=start
// of list, >= getListSize = last in list.
  if (!win || !itemWindow || win->getParent() != itemWindow) return;

  itemWindow->setWindowIndex( win, index );

  // Adjust all list item positions:
  adjustList();
  
} // setItemIndex

//----------------------------------------------------------------------------
void TTList::sort( int (*sortFunc)( TTWindow *a, TTWindow*b ), bool autosort_) {
// Sorts the list using sortFunc.  SortFunc is a pointer to a function
// that should return zero if a equals b, a positive number if a comes
// after b and a negative number if a comes before b.
// If autosort is true the sorting will be turned on and all subsequently
// added items will be sorted.  To turn off sorting pass NULL and false.

  if (!itemWindow) return;

  itemWindow->childWindows.newSortValues( sortFunc, autosort_ );
  if (!autosort_) itemWindow->childWindows.sort();
  autosort = autosort_;  //! Not really neccesary..

/*!
  // Move scrollbuttons back to their place at start of list:
  childWindows.remove( scrollArrowStart );
  childWindows.addFirst( scrollArrowStart );
  childWindows.remove( scrollArrowEnd );
  childWindows.addFirst( scrollArrowEnd );
*/
  adjustList();

} // sort

//----------------------------------------------------------------------------
void TTList::setListSizeBoundaries( int minW_, int maxW_,
                                    int minH_, int maxH_ ) {
// Sets boundaries for list size.
// The contained window objects will be resized to fit within the boundaries.

  minW = MAX( minW_, 0 );
  maxW = MAX( maxW_, minW );

  minH = MAX( minH_, 0 );
  maxH = MAX( maxH_, minH );

  adjustList();

} // setListSizeBoundaries

//----------------------------------------------------------------------------
/*
void TTList::adjustItemSize( TTWindow *win ) {
// Adjusts size of one item in list to fit within the limits given.

  if (!win || win->getParent() != this) return;

  int w, h;
  if (direction == TTLIST_DIRECTION_VERTICAL) {
    // Vertical list:

    w = win->getVisibleW();
    h = win->getVisibleH();

    w = MAX( minThickness, w );
    w = MIN( maxThickness, w );
    h = MAX( minLength, h );
    h = MIN( maxLength, h );

    win->setVisibleAreaSize(w, h);
  } else {
    // Horizontal list:
    
    w = win->getVisibleW();
    h = win->getVisibleH();

    w = MAX( minLength, w );
    w = MIN( maxLength, w );
    h = MAX( minThickness, h );
    h = MIN( maxThickness, h );

    win->setVisibleAreaSize(w, h);
  }

} // adjustItemSize
*/
//----------------------------------------------------------------------------
void TTList::adjustList() {
// Checks the sizes and positions of all contained items and
// adjusts them to fit in th given limits.

  if (!itemWindow) return;

  bool showStartButton = showScrollArrows && scrollArrowStart,
       showEndButton   = showScrollArrows && scrollArrowEnd;

  // Recalculate list size if changeListShape is true:
  int w = 0, h = 0;
  if (changeListShape) {
  
    // Loop all items and calculate thickness:    
    TTWindow *p;
    for (p = itemWindow->childWindows.first() ;
             itemWindow->childWindows.inside();
         p = itemWindow->childWindows.next())
      if (p) {
        if (direction == TTGUI_LIST_DIRECTION_VERTICAL) {
          // Vertical list:
          //! thickness = MAX( thickness, p->getVisibleW())
          h += p->getVisibleH() + itemSpacing;
          w = MAX( w, p->getVisibleW());
        } else {
          // Horizontal list:
          //! thickness = MAX( thickness, p->getVisibleH())
          w += p->getVisibleW() + itemSpacing;
          h = MAX( h, p->getVisibleH());
        }      
      }

    // Adjust length for arrow precence:
    if (direction == TTGUI_LIST_DIRECTION_VERTICAL) {
      // Vertical list:
      h -= itemSpacing;
      if (!showStartButton) h -= itemSpacing + scrollArrowLength;
      if (!showEndButton)   h -= itemSpacing + scrollArrowLength;
    } else {
      // Horizontal list:
      w -= itemSpacing;
      if (!showStartButton) w -= itemSpacing + scrollArrowLength;
      if (!showEndButton)   w -= itemSpacing + scrollArrowLength;
    }

    // Add borders:
    w += getBorderWidthLeft() + getBorderWidthRight();
    h += getBorderWidthTop()  + getBorderWidthBottom();
    
    // Clip to boundaries:
    w = MAX( w, minW );
    w = MIN( w, maxW );
    h = MAX( h, minH );
    h = MIN( h, maxH );
    
    // Set window size:
    setVisibleAreaSize( w, h );

    // Set item window size:
  }
  
  // Get width and height of list area inside borders:
  int innerW = getVisibleW() - getBorderWidthLeft() - getBorderWidthRight();
  int innerH = getVisibleH() - getBorderWidthTop()  - getBorderWidthBottom();


  // Adjust size and position of all windows in list:
  int len = 0;
  TTWindow *p;
  for ( p = itemWindow->childWindows.first() ;
            itemWindow->childWindows.inside();
        p = itemWindow->childWindows.next() )
    if (p) {
      if (direction == TTGUI_LIST_DIRECTION_VERTICAL) {
      // Vertical list:
        p->setPos( 0, len );
        p->setVisibleAreaSize( innerW, p->getVisibleH() );
        len += p->getVisibleH();
      } else {
      // Horizontal list:
        p->setPos( len, 0 );
        p->setVisibleAreaSize( p->getVisibleW(), innerH );
        len += p->getVisibleW();
      }
      len += itemSpacing;
    }

  // Remove extra space after last item:
  if (len > 0) len -= itemSpacing;


  // Set visible area of itemWindow:


  // Set virtual and visible area of itemWindow:
  int ls = 0, le = 0;
  if (showStartButton) ls = scrollArrowLength;
  if (showEndButton)   le = scrollArrowLength;
  if (direction == TTGUI_LIST_DIRECTION_VERTICAL) {
    // Vertical list:
    itemWindow->setVirtualAreaSize( innerW, len );
    itemWindow->setVisibleAreaSize( innerW, innerH - ls - le );    
    itemWindow->setPos( 0, ls );
  } else {
    // Horizontal list:
    itemWindow->setVirtualAreaSize( len, innerH );
    itemWindow->setVisibleAreaSize( innerW - ls - le, innerH );
    itemWindow->setPos( ls, 0 );
  }


  // Adjust start scrollbutton if visible:
  if (showStartButton) {
    //! len += scrollArrowLength;
    //! scrollArrowStart->setScr( getScrX() + getBorderWidthLeft(),
    //!                           getScrY() + getBorderWidthTop() );
    scrollArrowStart->setPos( 0, 0 );
    if (direction == TTGUI_LIST_DIRECTION_VERTICAL) {
    // Vertical list:
      scrollArrowStart->setAreaSize( innerW, scrollArrowLength );
    } else {
    // Horizontal list:
      scrollArrowStart->setAreaSize( scrollArrowLength, innerH );
    }  
  }
  
  // Adjust end scrollbutton if visible:
  if (showEndButton) {
    //! Not neccesary..:  len += itemSpacing;
    if (direction == TTGUI_LIST_DIRECTION_VERTICAL) {
    // Vertical list:
      //! scrollArrowEnd->setScr( getScrX() + getBorderWidthLeft(),
      //!                         getScrY() + getVisibleH()
      //!                         - getBorderWidthBottom() - scrollArrowLength );
      scrollArrowEnd->setPos( 0, innerH - scrollArrowLength );
      scrollArrowEnd->setAreaSize( innerW, scrollArrowLength );
    } else {
    // Horizontal list:
      //! scrollArrowEnd->setScr( getScrX() + getVisibleW()
      //!                         - getBorderWidthRight() - scrollArrowLength,
      //!                         getScrY() + getBorderWidthTop() );
      scrollArrowEnd->setPos( innerW - scrollArrowLength, 0 );
      scrollArrowEnd->setAreaSize( scrollArrowLength, innerH );
    }  
  }


  
} // adjustList

//----------------------------------------------------------------------------
/*
int TTList::getPreferedWidth() {
// Returns the ideal width of the list, given the size of its
// items and the min/max thickness/length fields.


  return getVisibleW();
} // getPreferedWidth
*/
//----------------------------------------------------------------------------
/*
int TTList::getPreferedHeight() {
// Returns the ideal height of the list, given the size of its
// items and the min/max thickness/length fields.


  return getVisibleH();
} // getPreferedHeight
*/
//----------------------------------------------------------------------------
void TTList::setShowScrollArrows( bool show ) {
// Sets scroll arrows on or off.

  showScrollArrows = show;

  if (scrollArrowStart) scrollArrowStart->setVisible( show );
  if (scrollArrowEnd)   scrollArrowEnd->setVisible( show );

  adjustList();

} // setShowscrollArrows

//----------------------------------------------------------------------------
void TTList::setScrollArrowLength( int len ) {
// Sets scroll arrow length.

  if (len < 0) len = 0;
  
  scrollArrowLength = len;

  adjustList();

} // setscrollArrowLength

//----------------------------------------------------------------------------
// End of class TTList.
//----------------------------------------------------------------------------

