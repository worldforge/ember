//////////////////////////////////////////////////////////////////////////////
//   File name:  ttstatic.h
//
//   Class:      TTStatic  Static text
//
//   Project:    Gui
//   Author:     Hans H„ggstr”m,  hans.haggstrom@helsinki.fi
//   Base class: TTWindow
//   Decendants: TTEdit
//   Purpose:    Static, non-changing, text.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef TTSTATIC_H
#define TTSTATIC_H

#include <allegro.h>
#include <time.h>
#include "string"
#include "ttGuigfx.h"
#include "ttguidef.h"
#include "ttwstyle.h"
#include "ttwindow.h"
#include "misc_str.h"


class TTStatic : public TTWindow {
  public:
    /*----[ Constructors & destructor: ]------------------------------------*/
    TTStatic( TTApplication *application_ = NULL );
    // Default constructor.

    TTStatic( const TTStatic &source );
    // Copy constructor.

    TTStatic( int x_, int y_, int w_, int h_,
              TTWindow      *parent_      = NULL,
              TTApplication *application_ = NULL );
    // Constructor.  If application is null it uses parent application.

    virtual ~TTStatic();
    // Destructor.


    /*----------------------------------------------------------------------*/
    /*----[ Drawing and other: ]--------------------------------------------*/
    virtual void draw( BITMAP *dest );
    // Draws static text

    virtual void drawText( BITMAP *dest );
    // Draws the text using aligment etc.

    virtual string drawTextLine( BITMAP *dest, string s, int yoffs_,
                                 int nr_of_prev_chars = 0 );
    // Draws a line of text.  Returns the undrawn lines.

    virtual bool update( BITMAP *dest );
    // Draws the parts of the window that have changed since last draw.
    // Returns true if anything was drawn.


    /*----------------------------------------------------------------------*/
    /*----[ Get index: ]----------------------------------------------------*/

    virtual int getIndexAt( int x_, int y_ );
    // Returns index of char at position x,y.

    virtual void getCharAt( int index, int *cx, int *cy = NULL,
                            int *cw = NULL, int *ch = NULL );
    // Returns x,y,width and height of character at specified index.
    // If index is outside it returns cx=cy=cw=ch=0.

    /*----------------------------------------------------------------------*/
    /*----[ Style: ]--------------------------------------------------------*/

    virtual void initStyle( TTGuiStyle *style_ = NULL );
    // Copies the given TTGuiStyle, or, if it's NULL,
    // copies the global style settings, modifies them,
    // and calls the initStyle of its child-windows if it has any.
    // Called automatically on creation.

    /*----------------------------------------------------------------------*/
    /*----[ Class name: ]---------------------------------------------------*/

    static string getClassName() { return "TTStatic"; }
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

    // Wrapping:
    virtual bool getMultiline()         { return multiline; }
    virtual void setMultiline( bool b ) { multiline = b;  adjustHeight();  redrawText = true; }

    virtual int  getAlignment()        { return align; }
    virtual void setAlignment( int a ) { align = a;    redrawText = true; }

    // Text:
    virtual string getText()             { return text; }
    virtual void   setText( string s )   { text = s;     redrawText = true; }

    // Auto height adjusting:
    virtual bool getAutoadjustHeight() { return autoAdjustHeight; }
    virtual void setAutoadjustHeight( bool b) {
      autoAdjustHeight = b;
      adjustHeight();
    }

    virtual void setVisibleAreaSize(int w, int h);
   // Adjusts the height to fit text lines if autoAdjustHeight is true.


  /*------------------------------------------------------------------------*/
  protected:
    virtual int  calculateAdjustedHeight(); // Calculates the height of the
                                            // text depending on autoAdjustHeight.
    virtual void adjustHeight(); // Adjusts the height of the text.
                                 // Uses calculateAdjustedHeight().

    virtual void adjustVirtualSize()  { /* .... */ }
    // Enlarges / shrinks the virtual size to fit the current size of the text.


    int    selectionStart,
           selectionLength;

    bool   multiline;
    int    align;
    string text;

    bool   autoAdjustHeight;  // If true height will be set to somewhat
                              // over fontheight on non multiline editfields,
                              // and rounded down to nearest full row on
                              // multiline editfields.

}; // class TTStatic.

#endif


