//////////////////////////////////////////////////////////////////////////////
//   File name:  ttedit.cc
//
//   Class:      TTStatic  Static text
//
//   Project:    Gui
//   Author:     Hans H„ggstr”m,  hans.haggstrom@helsinki.fi
//   Base class: TTWindow
//   Decendants:
//   Purpose:    Editable text fields
//
//
//   Work diary:
// Date      Time     Lines   Things done
// ~~~~~~~~~ ~~~~~~~~ ~~~~~~~ ~~~~~~~~~~~~~~~~~~~~~~
// 13.9.98   0:30     289     Started.  Used TTStatic as base, moved the text
//                            selection here (selection drawing is still done
//                            in TTStatic, this way we don't have to duplicate
//                            the drawing code).
// 18.9      0:20     320     Added autoAdjustHeight.
// 18.9      1:00     352     Debugged and fixed autoAdjustHeight.
// 18.9      0:05     292     Moved autoAdjustHeight to TTStatic.
// 19.9      3:00     397     Added cursor.  Should be transparent or something.
// 20.9      1:00     466     Added editing (chars, arrows, home/end, bs) cool =)
// 20.9      1:15     555     Added shift-selection and started on insert/removeText.
// 20.9      2:15     630     Completed insert/removeText and added and fine
//                            tuned some cursor moving hotkeys.
// 20.9      0:30     659     Added checkText and debugged single-line mode.
// 20.9      0:30     660     Added selectAll(), fixed something (can't remember what).
// 20.9      2:30     727     Debugging, all kind of small buggfixes and
//                            improvements (on selectable, editable, enabled,
//                            (mouse) selection, etc).
//
// Notes & things to do:
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//
//////////////////////////////////////////////////////////////////////////////

#include "ttedit.h"
#include <iostream.h>

/*--------------------------------------------------------------------------*/
// class TTEdit
/*--------------------------------------------------------------------------*/

/*----[ Constructors & destructor: ]------------------------------------*/
TTEdit::TTEdit( TTApplication *application_ = NULL )
  : TTStatic(application_) {
// Default constructor.
  selectionStartPlace = 0;
  selectable       = true;
  editable         = true;
  selectingText    = false;
  selectAllText    = false;
  replaceSelection = true;
  persistentSelection = false;

  tabstop          = true;
  showFocusOutline = true;

  cursorVisible    = false;
  cursorIndex      = 0;
  cursorColor      = 0;
  cursorX = cursorY = cursorW = cursorH = 0;

  initStyle();
} // Constructor

/*--------------------------------------------------------------------------*/
TTEdit::TTEdit( const TTEdit &source )
  : TTStatic( source ) {
// Copy constructor.
  selectionStartPlace = 0;
  selectable     = source.selectable;
  editable       = source.editable;
  selectingText  = false;
  selectAllText  = false;

  replaceSelection    = source.replaceSelection;
  persistentSelection = source.persistentSelection;

  tabstop          = source.tabstop;
  showFocusOutline = source.showFocusOutline;

  cursorVisible    = source.cursorVisible;
  cursorIndex      = source.cursorIndex;
  cursorColor      = source.cursorColor;
  cursorStyle      = source.cursorStyle;
  calculateCursorPos();

  initStyle();
} // Copycon

/*--------------------------------------------------------------------------*/
TTEdit::TTEdit( int x_, int y_, int w_, int h_,
                TTWindow      *parent_      = NULL,
                TTApplication *application_ = NULL )
  : TTStatic( x_, y_, w_, h_, parent_, application_ ) {
// Constructor.  If application is null it uses parent application.

  selectionStartPlace = 0;
  selectable     = true;
  editable       = true;
  selectingText  = false;
  selectAllText  = false;
  replaceSelection = true;
  persistentSelection = false;

  tabstop          = true;
  showFocusOutline = true;

  cursorVisible    = false;
  cursorIndex      = 0;
  cursorColor      = 0;
  cursorX = cursorY = cursorW = cursorH = 0;

  initStyle();
} // Constructor

/*--------------------------------------------------------------------------*/
TTEdit::~TTEdit() {
// Destructor.
} // Destructor

/*----------------------------------------------------------------------*/
/*----[ Drawing and other: ]--------------------------------------------*/
void TTEdit::draw( BITMAP *dest ) {
// Draws static text

  // Draw background:
  TTWindow::draw( dest );

  // Draw text:
  drawText( dest );

  // Draw cursor:
  if (cursorVisible && isEnabled())
    ttDrawCursor( dest, this, cursorX, cursorY, cursorW, cursorH,
                  cursorColor, cursorStyle );

} // draw

/*--------------------------------------------------------------------------*/
bool TTEdit::update( BITMAP *dest ) {
// Draws the parts of the window that have changed since last draw.
// Returns true if anything was drawn.

  bool updated = false;

  updated = TTWindow::update( dest );

  // Check if cursorcolor has changed, in that case redraw it..
  // Add redraw cursor flag too, and set redraw flag in movecursor..

  if (updated) {
    // Draw text:
    drawText( dest );

    // Draw cursor if anything changed:
    if (isEnabled())
      ttDrawCursor( dest, this, cursorX, cursorY, cursorW, cursorH,
                    cursorColor, cursorStyle );
  }

  return updated;
} // update

/*--------------------------------------------------------------------------*/
void TTEdit::doTurn() {
// Sort of idle loop.  Gets called frequently.  Don't do anything too
// time consuming, or the system will slow down.

  TTWindow::doTurn();

  // Sort of a kludge: selects all text on a double click,
  // howewver, the clicks are apparently registered afterwards(or something)
  // and that unselects things again.
  if (selectAllText) {
    selectAll();
    selectAllText   = false;
    releaseMouse();
  }

  // Lose selection if focus lost:
  if (selectionLength > 0 && !hasFocus()) {
    cancelSelection();
    releaseMouse();
  }

} // doTurn

/*--------------------------------------------------------------------------*/
/*----[ Cursor control: ]---------------------------------------------------*/
/*--------------------------------------------------------------------------*/
void TTEdit::moveCursor( int index ) {
// Moves the cursor to character at position index (0 = first).
  cursorIndex = index;
  if (cursorIndex < 0 || cursorIndex > text.length() ||
      (!isSelectable() && !isEditable()) )
    // Hide cursor if it is outside range, or if the text is neither
    // selectable nor editable (then there is no use for the cursor):
    cursorVisible = false;
  else if (hasFocus())
    cursorVisible = true; // ?? Hmm.. should we do this?

  calculateCursorPos();

  // Update selection:
  if (selectingText && !(mouse_b & 1)) {
    setSelectionStart( selectionStartPlace );
    setSelectionEnd( cursorIndex );
  }

} // moveCursor

/*----------------------------------------------------------------------*/
/*----[ Selection control: ]--------------------------------------------*/
/*----------------------------------------------------------------------*/
void TTEdit::startSelection(int index) {
// Starts a selection process (call from mouse down or shift down etc.)
// Index is the character number in the text to start from. 0 is just
// before the first char, text.length() is just after the last char.

  // Check if selectable:
  if (!isSelectable()) {
    setSelectionLength(0);
    selectingText = false;
    return;
  }

  // Continue on previous selection if at edge:
  if (index == getSelectionStart()) {
    // Continue from start of prev. selection:
    selectionStartPlace = getSelectionEnd();
    setSelectionStart( selectionStartPlace );
    setSelectionEnd( index );

  } else if (index == getSelectionEnd()) {
    // Continue from end of prev. selection:
    selectionStartPlace = getSelectionStart();
    setSelectionStart( selectionStartPlace );
    setSelectionEnd( index );

  } else {
    // New selection:
    selectionStartPlace = index;
    setSelectionStart( index );
    setSelectionEnd( index );
  }

  // Set intrnal selection flag:
  selectingText = true;

} // startSelection

/*--------------------------------------------------------------------------*/
void TTEdit::endSelection(int index) {
// Ends a selection process (call from mouse/shift up etc.)
// Index is the character number in the text to start from. 0 is just
// before the first char, text.length() is just after the last char.

  selectingText = false;
  setSelectionStart( selectionStartPlace );
  setSelectionEnd( index );
  if (!isSelectable()) setSelectionLength(0);

} // endSelection

/*--------------------------------------------------------------------------*/
void TTEdit::cancelSelection() {
// Cancels a selection process (call from rmose etc.)
// Restores old selection??  not at the moment..

  selectingText = false;
  setSelectionStart( 0 );
  setSelectionEnd( 0 );
} // cancelSelection

/*----------------------------------------------------------------------*/
/*----[ High level keyboard events: ]-----------------------------------*/
/*--------------------------------------------------------------------------*/
bool TTEdit::keyPress( int pressedKey ) {
// Check and process a keypress.
// Returns true if the keypress was processed.

  // Check if enabled:
  if (!isEnabled()) return false;

  // If textfield is neither selectable nor editable there is no point in
  // moving the cursor / trying to write:
  if (!isSelectable() && !isEditable()) return false;

  // Get ascii & scancode values:
  int keyAscii = pressedKey & 0xff;
  int keyScan  = pressedKey >> 8;

  // Check text selecting:
  if ( keyScan == KEY_LEFT  ||
       keyScan == KEY_RIGHT ||
       keyScan == KEY_UP    ||
       keyScan == KEY_DOWN  ||
       keyScan == KEY_HOME  ||
       keyScan == KEY_END   ||
       keyScan == KEY_PGUP  ||
       keyScan == KEY_PGDN ) {

    if (selectingText && !(mouse_b & 1) && !(key[KEY_RSHIFT] || key[KEY_LSHIFT])) {
      endSelection( cursorIndex );
    }

    if (!selectingText && (key[KEY_RSHIFT] || key[KEY_LSHIFT])) {
      // Start selection:
      startSelection( cursorIndex );

    } else if (!selectingText && !persistentSelection) {
      // Remove selection:
      setSelectionLength( 0 );
    }
  }
  else {
    if (selectingText && !(mouse_b & 1)) {
      endSelection( cursorIndex );
    }
  }

  // Check for select all (CTRL+A):
  if (keyScan == KEY_A && (key[KEY_LCONTROL] || key[KEY_RCONTROL])) {
    selectAll();
    return true;
  }

  // Check backspace:
  if (keyScan == KEY_BACKSPACE && isEditable()) {
    removeText( cursorIndex-1, 1 );
    return true;
  }

  // Check delete:
  if (keyScan == KEY_DEL && isEditable()) {
    removeText( cursorIndex, 1 );
    return true;
  }

  // Check CONTROL + keys:
  if (key[KEY_LCONTROL] || key[KEY_RCONTROL]) {
    if (keyScan == KEY_RIGHT) {
      int i = cursorIndex;
      for ( ; i < text.length() && !(text[i] == ' ' || text[i] == '\n'); i++);
      for ( ; i < text.length() &&  (text[i] == ' ' || text[i] == '\n'); i++);
      if (i > text.length()) i = text.length();
      moveCursor( i );
      return true;
    }
    if (keyScan == KEY_LEFT) {
      int i = cursorIndex;
      i--;
      for ( ; i >= 0 && !(text[i] == ' ' || text[i] == '\n'); i--);
      for ( ; i >= 0 &&  (text[i] == ' ' || text[i] == '\n'); i--);
      i++;
      if (i < 0) i = 0;
      moveCursor( i );
      return true;
    }
    if (keyScan == KEY_HOME) {
      moveCursor( 0 );
      return true;
    }
    if (keyScan == KEY_END) {
      moveCursor( text.length() );
      return true;
    }
  } // end control.

  // Check arrow keys:
  if (keyScan == KEY_RIGHT) {
    if (cursorIndex < text.length()) moveCursor(cursorIndex + 1);
    return true;
  }
  if (keyScan == KEY_LEFT) {
    if (cursorIndex > 0) moveCursor(cursorIndex - 1);
    return true;
  }
  if (keyScan == KEY_UP && multiline) {
    int tx = cursorX + getScrX() - getScrollX() + cursorW / 2 + 1;
    int ty = cursorY + getScrY() - getScrollY() - 1 - TTGUI_TEXT_VERTICAL_SPACING;
    int index = getIndexAt( tx, ty ); // Should give line above?
    moveCursor(index);
    return true;
  }
  if (keyScan == KEY_DOWN && multiline) {
    int tx = cursorX + getScrX() - getScrollX() + cursorW / 2 + 1;
    int ty = cursorY + getScrY() - getScrollY() + cursorH + 2 + TTGUI_TEXT_VERTICAL_SPACING;
    int index = getIndexAt( tx, ty ); // Should give line below?
    moveCursor(index);
    return true;
  }

  // Check home/end:
  if (keyScan == KEY_HOME) {
    int i;
    for (i = cursorIndex; i > 0 && text[i-1] != '\n'; i--) ;
    moveCursor( i );
    return true;
  }
  if (keyScan == KEY_END) {
    int i;
    for (i = cursorIndex; i < text.length() && text[i] != '\n'; i++) ;
    moveCursor( i );
    return true;
  }

  // Check for enter/return:
  if (keyScan == KEY_ENTER && multiline && isEditable() ) {
    string t = "\n";
    insertText( cursorIndex, t );
    return true;
  }

  // Write key to editfield if normal char:
  if (keyAscii >= 32 && cursorVisible && isEditable() ) {
    string t = " ";
    t[0]     = keyAscii;
    insertText( cursorIndex, t );
    return true;
  }

  // Not processed:
  return false;
} // keyPress

/*----------------------------------------------------------------------*/
/*----[ High level mouse events: ]--------------------------------------*/
/*----------------------------------------------------------------------*/
void TTEdit::mouseDblClick( int x_, int y_, int button, TTWindow *win ) {
// A mouse button was double-clicked.
// button is 1 for left, 2 for right and 3 for middle.
// win is the window at position x, y (normally 'this').

  // Call parent mouseDblClick:
  TTWindow::mouseDblClick( x_, y_, button, win );

  // Select all text:
  if (win == this && button == TTGUI_MOUSE_LEFT) {
    selectAllText = true;
  }
} // mouseDblClick

/*----[ Low level mouse events: ]---------------------------------------*/
void TTEdit::mouseDown( int x_, int y_, int button, TTWindow *win ) {
// A mouse button was pressed.
// button is 1 for left, 2 for right and 3 for middle.
// win is the window at position x, y (normally 'this').

  // Call parent mouseDown:
  TTWindow::mouseDown( x_, y_, button, win );

  // Get focus on mouse down if the text is selectable or editable:
  if ( (isSelectable() || isEditable() || hasTabstop()) &&
       !hasFocus() && isEnabled() )
    setFocus(true);

  // Do selection stuff:
  if (win == this && button == TTGUI_MOUSE_LEFT && isEnabled() && isSelectable()) {
    startSelection( getIndexAt( x_, y_ ) );
    captureMouse();
  }
  else if (selectingText && button == TTGUI_MOUSE_RIGHT) {
    // Selection canceled:
    cancelSelection();
    releaseMouse();
  }

  // Show cursor:
  moveCursor( getIndexAt( x_, y_ ) );

} // mouseDown

/*--------------------------------------------------------------------------*/
void TTEdit::mouseUp( int x_, int y_, int button, TTWindow *win ) {
// A mouse button was released.
// button is 1 for left, 2 for right and 3 for middle.
// win is the window at position x, y (normally 'this').

  // Call parent mouseUp:
  TTWindow::mouseUp( x_, y_, button, win );

  // Do selection stuff:
  if (selectingText && button == TTGUI_MOUSE_LEFT) {
    // Selection completed:
    endSelection( getIndexAt( x_, y_ ) );
    releaseMouse();
  }

} // mouseUp

/*--------------------------------------------------------------------------*/
void TTEdit::mouseMove( int x_, int y_, TTWindow *win ) {
// The mouse was moved.  x and y are the new mouse coordinates.
// win is the window at position x, y (normally 'this').

  // Call parent mouseMove:
  TTWindow::mouseMove(x_, y_, win);

  // Selection stuff:
  if (mouse_b & 1) {
    int index = getIndexAt( x_, y_ );
    if (selectingText) {
      setSelectionStart( selectionStartPlace );
      setSelectionEnd( index );
    }

    // Show/move cursor:
    moveCursor( index );
  }

} // mouseMove

/*--------------------------------------------------------------------------*/
/*----[ Style: ]------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/

void TTEdit::initStyle( TTGuiStyle *style_ ) {
// Copies the given TTGuiStyle, or, if it's NULL,
// copies the global style settings, modifies them,
// and calls the initStyle of its child-windows if it has any.
// Called automatically on creation.

  // Call parent:
  //TTWindow::initStyle(style_); Not neccesary, already called.

  // Select winstyle to use (Override in inherited classes when neccesary):
  TTApplicationStyle *appStyle = getApplicationStyle();
  if (appStyle)
    style.winStyle = appStyle->editStyle;

  // Start cursor blinking:
  int stayTime, changeTime;
  if (appStyle) {
    stayTime   = appStyle->cursorStayColorTime;
    changeTime = appStyle->cursorChangeColorTime;
  }
  else {
    stayTime   = TTGUI_CURSOR_STAYCOLOR_TIME;
    changeTime = TTGUI_CURSOR_CHANGECOLOR_TIME;
  }
  fadeColor( &cursorColor, style.cursorOnColor,  0,          true, false ); // Clear old fades.
  fadeColor( &cursorColor, style.cursorOnColor,  stayTime,   false, true );
  fadeColor( &cursorColor, style.cursorOffColor, changeTime, false, true );
  fadeColor( &cursorColor, style.cursorOffColor, stayTime,   false, true );
  fadeColor( &cursorColor, style.cursorOnColor,  changeTime, false, true );

  // Set cursor style:
  if (appStyle) cursorStyle = appStyle->cursorStyle;
  else          cursorStyle = TTGUI_CURSOR_VERTICAL;

} // initStyle

/*--------------------------------------------------------------------------*/
/*----[ Inserting and removing text: ]--------------------------------------*/
/*--------------------------------------------------------------------------*/

void TTEdit::insertText( int index, string newtext, bool check ) {
// Inserts the given text at the given position.  Keeps track of selection,
// replaces selection with given text if replace selection flag is true.

  // Replace selection if flag set:
  if (replaceSelection && getSelectionLength() > 0) {
    index = getSelectionStart();
    removeText( getSelectionStart(), getSelectionLength() );
    moveCursor(index); // Move cursor to new text.
  }

  // Check boundaries:
  if (index < 0) index = 0;
  if (index > text.length()) index = text.length();
  text.insert( index, newtext);

  // Check persistentSelection:
  if (!persistentSelection) {
    setSelectionLength( 0 );
  } else {
    // Move selection:
    int selStart = getSelectionStart();
    int selLen   = getSelectionLength();
    if (selStart > index)
      setSelectionStart( selStart + newtext.length() );
    else if (selStart + selLen > index)
      setSelectionLength( selLen + newtext.length() );
  }

  // Move cursor:
  if (cursorIndex >= index)
    moveCursor( cursorIndex + newtext.length() );

  // Check text:
  if (check) checkText();

} // insertText

/*--------------------------------------------------------------------------*/
void TTEdit::removeText( int index, int len, bool check ) {
// Removes specified nr of chars at the given position.  Keeps track of
// selection, Removes selected text instead if replace selection flag is true.
// If check is true (default) it calls the checkText method.

  // Remove selection instead if replaceSelection is true.  Logical?
  if (replaceSelection && getSelectionLength() > 0) {
    index = getSelectionStart();
    len   = getSelectionLength();
    moveCursor(index); // Move cursor to removed text.
  }

  // Check boundaries:
  if (index < 0) {
    len  += index;
    index = 0;
  }
  if (len + index > text.length()) len = text.length() - index;
  if (index > text.length()) return;
  if (len <= 0) return;

  // Check persistentSelection:
  if (!persistentSelection) {
    setSelectionLength( 0 );
  } else { // Move selection:
    int selStart = getSelectionStart();
    int selLen   = getSelectionLength();

    // Move selection start:
    if (selStart >= index)
      setSelectionStart( MAX( selStart - len, index ) );

    // Change selection length:
    int overlap = 0;
    if (selStart < index) {
      overlap += MAX( 0, (selStart + selLen) - index );
      overlap -= MAX( 0, (selStart + selLen) - (index + len) );
    }
    else {
      overlap += MAX( 0, (index + len) - selStart );
      overlap -= MAX( 0, (index + len) - (selStart +selLen) );
    }
    setSelectionLength( selLen - overlap );
  }

  // Remove text:
  text.erase( index, len );  // Was: remove.

  // Move cursor:
  if (cursorIndex >= index && cursorIndex < index + len)
    moveCursor( index );
  else if (cursorIndex >= index + len)
    moveCursor( cursorIndex - len );

  // Check text:
  if (check) checkText();

} // removeText

/*--------------------------------------------------------------------------*/
void TTEdit::checkText() {
// Can be used to filter/format the text.  Use insert/removeText (with
// check parameter set to false) to change the text, don't alter text
// directly!  (this way selection and cursor is handled automatically).
// Called every time insert/removeText is called.
// Removes newlines if multiline is false.

  // Replace newlines with spaces if multiline is false:
  if (!multiline) {
    int i;
    for (i = 0; i < text.length(); )
      if (text[i] == '\n') {
        removeText( i, 1, false );
        //insertText( i, " ", false );
      }
      else i++;
  }

} // checkText

/*--------------------------------------------------------------------------*/
/*----[ Accessors: ]--------------------------------------------------------*/
/*--------------------------------------------------------------------------*/

string TTEdit::getSelection() {
// Returns selected text.
  string s = "";
  int i, len = text.length();
  for ( i = selectionStart;
        i < len && i < selectionStart + selectionLength;
        i++)  s += text[i];
  return s;
} // getSelection


/*--------------------------------------------------------------------------*/
void TTEdit::setFocus( bool focus_ ) {
// Calls TTWindow::setFocus and hides/shows cursor.

  // Call parent setFocus:
  TTWindow::setFocus( focus_ );

  // Show/hide cursor:
  setCursorVisible( hasFocus() );

  // Check selectAllOnGetFocus:
  if (hasFocus() && selectAllOnGetFocus) selectAll();

} // setFocus

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
void TTEdit::calculateCursorPos() {
// Calculates cursor x, y, w & h from cursor index.
  //if (!cursorVisible) return;

  getCharAt( cursorIndex, &cursorX, &cursorY, &cursorW, &cursorH );

} // calculateCursorPos

/*--------------------------------------------------------------------------*/
// End of class TTEdit.
/*--------------------------------------------------------------------------*/



