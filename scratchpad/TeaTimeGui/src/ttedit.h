//////////////////////////////////////////////////////////////////////////////
//   File name:  ttedit.h
//
//   Class:      TTEdit  Editable text field.
//
//   Project:    Gui
//   Author:     Hans H„ggstr”m,  hans.haggstrom@helsinki.fi
//   Base class: TTStatic
//   Decendants:
//   Purpose:    Editable text field.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef TTEDIT_H
#define TTEDIT_H

#include <allegro.h>
#include <time.h>
#include "string"
#include "ttGuigfx.h"
#include "ttguidef.h"
#include "ttwstyle.h"
#include "ttwindow.h"
#include "misc_str.h"
#include "ttstatic.h"


class TTEdit : public TTStatic {
  public:
    /*----[ Constructors & destructor: ]------------------------------------*/
    TTEdit( TTApplication *application_ = NULL );
    // Default constructor.

    TTEdit( const TTEdit &source );
    // Copy constructor.

    TTEdit( int x_, int y_, int w_, int h_,
            TTWindow      *parent_      = NULL,
            TTApplication *application_ = NULL );
    // Constructor.  If application is null it uses parent application.

    virtual ~TTEdit();
    // Destructor.


    /*----------------------------------------------------------------------*/
    /*----[ Drawing and other: ]--------------------------------------------*/
    virtual void draw( BITMAP *dest );
    // Draws text field

    virtual bool update( BITMAP *dest );
    // Draws the parts of the window that have changed since last draw.
    // Returns true if anything was drawn.

    virtual void doTurn();
    // Sort of idle loop.  Gets called frequently.  Don't do anything too
    // time consuming, or the system will slow down.

    /*----------------------------------------------------------------------*/
    /*----[ High level keyboard events: ]-----------------------------------*/
    virtual bool keyPress( int key );
    // Check and process a keypress.
    // Returns true if the keypress was processed.

    /*----------------------------------------------------------------------*/
    /*----[ High level mouse events: ]--------------------------------------*/
    virtual void mouseDblClick( int x_, int y_, int button, TTWindow *win );
    // Selects all text

    /*----[ Low level mouse events: ]---------------------------------------*/
    virtual void mouseDown( int x_, int y_, int button, TTWindow *win );
    // Starts selection

    virtual void mouseUp( int x_, int y_, int button, TTWindow *win );
    // Ends selection

    virtual void mouseMove( int x_, int y_, TTWindow *win );
    // Changes selection


    /*----------------------------------------------------------------------*/
    /*----[ Cursor control: ]-----------------------------------------------*/
    virtual void moveCursor( int index );
    // Moves the cursor to character at position index (0 = first).

    /*----------------------------------------------------------------------*/
    /*----[ Selection control: ]--------------------------------------------*/
    virtual void startSelection(int index);
    // Starts a selection process (call from mouse down or shift down etc.)
    // Index is the character number in the text to start from. 0 is just
    // before the first char, text.length() is just after the last char.

    virtual void endSelection(int index);
    // Ends a selection process (call from mouse/shift up etc.)
    // Index is the character number in the text to start from. 0 is just
    // before the first char, text.length() is just after the last char.

    virtual void cancelSelection();
    // Cancels a selection process (call from rmose etc.)
    // Restores old selection??  not at the moment..


    /*----------------------------------------------------------------------*/
    /*----[ Style: ]--------------------------------------------------------*/

    virtual void initStyle( TTGuiStyle *style_ = NULL );
    // Copies the given TTGuiStyle, or, if it's NULL,
    // copies the global style settings, modifies them,
    // and calls the initStyle of its child-windows if it has any.
    // Called automatically on creation.


    /*----------------------------------------------------------------------*/
    /*----[ Inserting and removing text: ]----------------------------------*/

    virtual void insertText( int index, string text, bool check = true );
    // Inserts the given text at the given position.  Keeps track of
    // selection, replaces selection with given text if replace selection
    // flag is true.
    // If check is true (default) it calls checkText.

    virtual void removeText( int index, int len, bool check = true );
    // Removes specified nr of chars at the given position.  Keeps track of
    // selection, Removes selected text instead if replace selection flag
    // is true.
    // If check is true (default) it calls checkText.

    virtual void checkText();
    // Can be used to filter/format the text.  Use insert/removeText (with
    // check parameter set to false) to change the text, don't alter text
    // directly!  (this way selection and cursor is handled automatically).
    // Called every time insert/removeText is called.
    // Removes newlines if multiline is false.

    /*----------------------------------------------------------------------*/
    /*----[ Class name: ]---------------------------------------------------*/

    static string getClassName() { return "TTEdit"; }
    // Returns name of class.  Can be used for manual run-time type checking.
    // Make sure to override in derived classes.  Always use exact name of class.

    static bool inherits( string ancestor ) {
    // Returns true if this class or any of its parent classes has the
    // class name given as parameter. Make sure to override in derived classes.
      if (ancestor == getClassName()) return true;
      else return TTStatic::inherits( ancestor );
    }
    
    /*----------------------------------------------------------------------*/
    /*----[ Accessors: ]----------------------------------------------------*/

    virtual bool isEditable() { return editable; }
    virtual void setEditable(bool b) {
      editable = b;
      // Change background color to indicate editability:
      TTApplicationStyle *appStyle = getApplicationStyle();
      if (appStyle) {
        if (editable)
          style.winStyle.bgColor = appStyle->editStyle.bgColor;
        else
          style.winStyle.bgColor = appStyle->backgroundStyle.bgColor;
      }
    }


    // Text:
    virtual void setText( string newText ) {
      // Uses insert/remove text to set the text.
      setSelectionLength( 0 );  // No selection, avoid replaceSelection problems.
      removeText( 0, text.length() );
      insertText( 0, newText );
    }

    // Focus:
    virtual void setFocus( bool focus_ );
     // Calls TTWindow::setFocus and hides/shows cursor.

    // Cursor:
    virtual int  getCursorIndex()   { return cursorIndex;   }
    virtual bool getCursorVisible() { return cursorVisible; }

    virtual void setCursorVisible( bool cursorVis ) {

      // No point in showing cursor if the text is neither editable
      // nor selectable:
      if (!isSelectable() && !isEditable()) cursorVis = false;

      cursorVisible = cursorVis;
      if (cursorVisible) calculateCursorPos(); // Hide cursor if it is outside index.
      redrawText    = true;
    }

    // Selection:
    virtual bool isSelectable()           { return selectable;       }
    virtual bool getReplaceSelection()    { return replaceSelection; }
    virtual void setReplaceSelection( bool b ) { replaceSelection = b; }
    virtual void setSelectable( bool b )       { selectable = b;        redrawText = true; }

    virtual int  getSelectionStart()      { return selectionStart;  }
    virtual int  getSelectionLength()     { return selectionLength; }
    virtual int  getSelectionEnd()        { return selectionStart + selectionLength; }
    virtual void setSelectionStart(int i) {
      selectionStart = i;
      redrawText     = true;
      checkSelection();
    }

    virtual void setSelectionLength(int l)   {
      selectionLength = l;
      checkSelection();
      redrawText     = true;
    }

    virtual void setSelectionEnd(int i)   {
      if (i < selectionStart) {
        selectionLength = selectionStart - i;
        selectionStart  = i;
      }
      else selectionLength = i - selectionStart;

      checkSelection();
      redrawText     = true;
    }

    virtual string getSelection();
    // Returns selected text.

    virtual void selectAll() {
      // Selects all text.
      startSelection( 0 );
      endSelection( text.length() );
    }

    virtual void setPersistentSelection( bool b ) { persistentSelection = b;    }
    virtual bool getPersistentSelection()         { return persistentSelection; }

    virtual void setSelectAllOnGetFocus( bool b ) { selectAllOnGetFocus = b;    }
    virtual bool getSelectAllOnGetFocus()         { return selectAllOnGetFocus; }

  /*------------------------------------------------------------------------*/
  protected:
    virtual void calculateCursorPos();
    // Calculates cursor x, y, w & h from cursor index.

    //-------------------------------------------------
    virtual void checkSelection() {
    // Swaps selection end and start markers if neccesary and makes sure they
    // are in range.
      if (!selectable) {
        // Not selectable:
        selectionStart  = 0;
        selectionLength = 0;
        return;
      }

      if (selectionLength <  0) selectionLength = 0;
      if (selectionStart  <  0) selectionStart  = 0;
      if (selectionStart  >= text.length() ) selectionStart = text.length();
      if (selectionLength > text.length() - selectionStart)
        selectionLength = text.length() - selectionStart;


      // If don't have multiple lines the selection has to end before the first newline:
      if (!multiline) {
        int i;
        for (i = 0; i < text.length() && text[i] != '\n'; i++) ;
        if (selectionStart + selectionLength > i) selectionLength = i - selectionStart;
        if (selectionLength < 0) {
          selectionLength = 0;
          selectionStart  = 0;
        }
      }
    } // checkSelection
    //-------------------------------------------------

    int    selectionStartPlace;  // Used when selecting.

    bool   selectable;
    bool   editable;           // True if the text can be edited (by keyb, rmouse paste etc, still possible to use setText, insertText, etc)
    bool   replaceSelection; // If true then selection is replaced with newly entered text.
    bool   selectAllOnGetFocus; // Select all text when gets focus if true.
    bool   persistentSelection; // If true selection won't dissapear if text is edited.

    bool   selectingText;       // Internal flag.
    bool   selectAllText;       // Internal flag/kludge.

    // Cursor:
    bool   cursorVisible;
    int    cursorIndex;
    int    cursorColor;
    int    cursorStyle;
    int    cursorX, cursorY, cursorW, cursorH;
     // Cursor x and y are relative to upper left corner of (virtual) textwindow.

}; // class TTEdit.

#endif


