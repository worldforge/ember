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
//  06.02.99
//
//  Notes & things to do:
//  ~~~~~~~~~~~~~~~~~~~~~~~~
//
//
//////////////////////////////////////////////////////////////////////////////

#ifndef TTLIST_H
#define TTLIST_H

#include <stdlib.h>
#include <allegro.h>
//#include <time.h>
#include <string>

#include "ttapplic.h"
#include "ttguigfx.h"
#include "ttguidef.h"
#include "ttwindow.h"
#include "ttwstyle.h"
#include "ttform.h"
#include "ttbutton.h"
#include "ptrlist.h"


#define TTGUI_LIST_SCROLLSPEED  3

class TTList : public TTForm {
// One dimensional list of windows.

  public:
    TTList( TTApplication *application_ = NULL );
    // Default constructor.

    TTList( const TTList &source );
    // Copy constructor.

    virtual ~TTList();
    // Destructor.

    virtual void addItem( TTWindow *win );
    // Adds win to last position in list.
    // Removes win from any other places within the list first.
    // Does nothing if win is NULL.
    // Adjusts size of window to fit in limits given to list if neccesary.
    
    virtual void removeItem( TTWindow *win );
    // Removes win from the list if it was in it.

    virtual void setItemIndex( TTWindow *win, int index );
    // If win is in the list its position is changed to index (0=start
    // of list, >= getListSize = last in list.

    virtual void sort( int (*sortFunc)( TTWindow *a, TTWindow*b ), bool autosort);
    // Sorts the list using sortFunc.  SortFunc is a pointer to a function
    // that should return zero if a equals b, a positive number if a comes
    // after b and a negative number if a comes before b.
    // If autosort is true the sorting will be turned on and all subsequently
    // added items will be sorted.  To turn off sorting pass NULL and false.
    
    virtual int getListSize() {
      // Returns nr of window objects in list.
      if (itemWindow) return itemWindow->getNrOfWindows();
      else return 0;      
    }

    virtual void setListSizeBoundaries( int minW_, int maxW_,
                                        int minH_, int maxH_ );
    // Sets boundaries for list size.
    // The contained window objects will be resized to fit within the boundaries.

    virtual void adjustList();
    // Checks the sizes and positions of all contained items and
    // adjusts them to fit in th given limits.


    virtual void doTurn() {
    // Override.  Check if scrollbuttons pressed.
      TTForm::doTurn();
      
      if (scrollArrowStart && scrollArrowStart->isPressed())
        scrollItems( -TTGUI_LIST_SCROLLSPEED );
        
      if (scrollArrowEnd && scrollArrowEnd->isPressed())
        scrollItems(  TTGUI_LIST_SCROLLSPEED );
    }

    virtual void scrollItems( float speed ) {
    // Scroll items forward or backward.
      if (!itemWindow) return;
      
      if (direction == TTGUI_LIST_DIRECTION_VERTICAL)
        itemWindow->setScrollDelta( 0, speed );
      else
        itemWindow->setScrollDelta( speed, 0 );
    } // scrollItems

    //!virtual int getPreferedWidth();
    // Returns the ideal width of the list, given the size of its
    // items and the min/max thickness/length fields.

    //!virtual int getPreferedHeight();
    // Returns the ideal height of the list, given the size of its
    // items and the min/max thickness/length fields.


    virtual void setShowScrollArrows( bool show );
    // Sets scroll arrows on or off.
    
    virtual void setScrollArrowLength( int len );
    // Sets scroll arrow length.    

    virtual void setItemSpacing( int space ) {
      // Sets distance between list items.
      itemSpacing = space;
      adjustList();
    }

    virtual void setMultiselect( bool b ) {
    // If true then many items can be selected (using CTRL or SHIFT)
      multiselect = b;
    }

    virtual void setChangeListShape( bool b ) {
    // If true the shape of the list is changed within given limits to fit
    // to the contents.
      changeListShape = b;
      adjustList();
    }

    virtual void setHideScrollArrowsWhenUnnecesary( bool b ) {
    // If true and showScrollArrows is true the scrollarrows will only
    // be shown if there is items outsiode the listwindow to scroll to.
    // (Can anybody come up with a shorter name for this one? :)
      hideScrollArrowsWhenUnnecesary = b;
    }


    virtual void setVisibleAreaSize( int w, int h ) {
    // Override.  Calls adjustList.
      TTForm::setVisibleAreaSize( w, h );
      
      // Adjust boundaries to include new w & h:
      maxW = MAX( maxW, w );
      minW = MIN( minW, w );
      maxH = MAX( maxH, h );
      minH = MIN( minH, h );

      //! Reshape list:
      //! adjustList();  // Can't call this here, -> circle call!
    }

    virtual void setDirection( int dir ) {
    // Set list direction.  Use the TTGUI_LIST_DIRECTION_HORIZONTAL/VERTICAL
    // macros.
      if (dir == direction) return;
      direction = dir;
      adjustList();
    }
    
  protected:

    //!virtual void adjustItemSize();
    // Adjusts size of one item in list to fit within the limits given.

    int direction;


//!    bool scrollable;   // Add to TTWindow later.
//!    bool autoscroll;   // Add to TTWindow later.
    bool highlightSelected;
    bool showScrollArrows;
    bool hideScrollArrowsWhenUnnecesary; // If thereis nothing to scroll the buttons will be hidden.
    
    bool changeListShape; // Changes list shape within the given boundaries to fit to contents.
    int  minW, maxW, minH, maxH; // List boundaries.

    bool autosort;
    bool multiselect;  // If true many items can be selected.  Use TTWindow::selected.
//!    int  currentIndex;  // Index of selected item.  Only used when !multiselect.
                        // -1 (/negative) = none selected.

    TTWindowStyle *highlightStyle; //! Here? hmm..

    int scrollArrowLength;
    int itemSpacing;         // Distance between list items.

    TTButton *scrollArrowStart;
    TTButton *scrollArrowEnd;
    TTForm   *itemWindow;   // Contains listed items.
    
}; // class TTList.


#endif //TTLIST_H

