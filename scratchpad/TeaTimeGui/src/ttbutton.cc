//////////////////////////////////////////////////////////////////////////////
//   File name:  ttbutton.cc
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
// 19.5.1998 0:30     (130)   Header
// 19.5      1:00      259    Implementation.  I should really sleep instead.
// 23.5      0:20      286    Added caption color for downpressed button.
// 25.5      0:45      311    Small improvememnts.
// 26.5      1:00      336    -"- and stuff.
// 12.9      0:40      340    Added new colorfade to button caption. Looks ok.
// 06.02.99  0:30      348    Added callback function.
//
// Notes & things to do:
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//
///////////////////////////////////////////////////
#include "ttbutton.h"

/*--------------------------------------------------------------------------*/
// class TTButton : public TTWindow {
/*--------------------------------------------------------------------------*/

/*----[ Constructors & destructor: ]------------------------------------*/
TTButton::TTButton( TTApplication *application_ )
  : TTWindow ( application_ ) {
// Default constructor.

  caption              = "";
  pressedDown          = false;
  destCaptionColor     = getNormalCaptionColor();
  captionColor         = getNormalCaptionColor();
  wasMouseDownOnButton = false;
  wasSpaceDownOnButton = false;
  pressCounter         = 0;
  useParentColorForBorder = false;
  buttonFunc              = NULL;

  initStyle();
} // Default constructor

/*--------------------------------------------------------------------------*/
TTButton::TTButton( const TTButton &source )
  : TTWindow (source) {
// Copy constructor.

  caption              = source.caption;
  pressedDown          = false;
  destCaptionColor     = getNormalCaptionColor();
  captionColor         = getNormalCaptionColor();
  wasMouseDownOnButton = false;
  wasSpaceDownOnButton = false;
  pressCounter         = 0;
  useParentColorForBorder = source.useParentColorForBorder;
  buttonFunc              = source.buttonFunc;

  initStyle();
} // Copy constructor.

/*--------------------------------------------------------------------------*/
TTButton::TTButton( int x_, int y_, int w_, int h_,
                    TTWindow      *parent_      = NULL,
                    TTApplication *application_ = NULL )
  : TTWindow (x_, y_, w_, h_, parent_, application_) {
// Constructor.  If application is null it uses parent application.

  caption              = "";
  pressedDown          = false;
  destCaptionColor     = getNormalCaptionColor();
  captionColor         = getNormalCaptionColor();
  wasMouseDownOnButton = false;
  wasSpaceDownOnButton = false;
  pressCounter         = 0;
  useParentColorForBorder = false;
  buttonFunc              = NULL;
  //showFocusOutline = false;

  initStyle();
} // Constructor

/*--------------------------------------------------------------------------*/
TTButton::~TTButton() {
// Destructor.

} // Destructor.

/*--------------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*----[ Drawing and other: ]--------------------------------------------*/
void TTButton::draw( BITMAP *dest ) {
// Draws border, area, outline and caption.
// Should it call the parent draw?  Probably, altough in that case
// it has to draw the border twice when pressed.

  if (!visible || !dest) return; // Hmmm?? here?
  TTWindow::draw( dest );

  if (pressedDown) {
    style.winStyle.areaXOffs++;
    style.winStyle.areaYOffs++;
    ttDrawArea(dest, this);
    style.winStyle.areaXOffs--;
    style.winStyle.areaYOffs--;
  }

  // Draw caption:
  bool wasDropshadow = false;
  if (pressedDown && style.winStyle.textStyle == TTGUI_TEXT_DROPSHADOW) {
    style.winStyle.textStyle = TTGUI_TEXT_SHADOW;
    wasDropshadow = true;
  }

  if (pressedDown) ttDrawText( dest, this, caption, captionColor, true, 1, 1);
  else             ttDrawText( dest, this, caption, captionColor, true, 0, 0);

  if (wasDropshadow) style.winStyle.textStyle = TTGUI_TEXT_DROPSHADOW;

  // Draw pressed border if pressed down:
  if (pressedDown) {
    style.winStyle.borderStyle++; // Assume the downpressed style is directly
    ttDrawBorder( dest, this );   // after the up-style..
    style.winStyle.borderStyle--;
  }

} // draw

/*--------------------------------------------------------------------------*/
bool TTButton::update( BITMAP *dest ) {
// Draws the parts of the window that have changed since last draw.
// Returns true if anything was drawn.
  if (!visible || !dest) return false; // Hmmm?? here?

  bool b = TTWindow::update( dest );
  if (redrawText) {

    // Draw caption:
    if (pressedDown) ttDrawText( dest, this, caption, captionColor, true, 1, 1);
    else             ttDrawText( dest, this, caption, captionColor, true, 0, 0);

    redrawText = false;
    b          = true;
  }

  return b;
} // update

/*--------------------------------------------------------------------------*/
void TTButton::doTurn() {
// Sort of idle loop.  Gets called frequently.  Don't do anything too
// time consuming, or the system will slow down.

  // Call parent do turn:
  TTWindow::doTurn();

  // Check space:
  if (wasSpaceDownOnButton && !key[KEY_SPACE]) releaseButton();

  if (pressCounter > 1) pressCounter--;
  if (pressCounter == 1) {
    releaseButton();
    pressCounter = 0;
  }

} // doTurn

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
// Button functions:

void TTButton::pressButton( bool waitForRelease ) {
// Presses down the button.
// If waitFor release is false the button is released after a short moment,
// else the button stays down until releaseButton is called.
  if (isEnabled() == false) return;
  if (!waitForRelease) pressCounter = TTGUI_BUTTON_STD_DOWNTIME;
  pressedDown = true;
  setFocus(true);
} // pressButton

/*--------------------------------------------------------------------------*/
void TTButton::releaseButton() {
// Releases the button and calls doFunction.
  setFocus(true);
  wasMouseDownOnButton = false;
  wasSpaceDownOnButton = false;
  pressedDown          = false;
  doFunction();
//  destCaptionColor = getNormalCaptionColor(); // check mouse here..

} // releaseButton

/*--------------------------------------------------------------------------*/
void TTButton::cancelPress() {
// Cancels the button press when dragging the mouse outside the window and
// releasing the mouse button.
  pressedDown          = false;
  wasMouseDownOnButton = false;
  wasSpaceDownOnButton = false;
} // cancelPress

/*--------------------------------------------------------------------------*/
void TTButton::doFunction() {
// Executes the button function.

  if (isEnabled() && buttonFunc)
    (*buttonFunc)( this );
    
} // doFunction

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
bool TTButton::keyPress( int c ) {
// Press button if enter / space.. ?
// Returns true if the keypress was processed.

  if (TTWindow::keyPress(c)) return true;

  if ( (c >> 8) == KEY_ENTER) {
    pressButton( false );
    return true;
  }
  else if ((c >> 8) == KEY_SPACE) {
    pressButton( true );
    wasSpaceDownOnButton = true;
    return true;
  }

  return false;
} // keyPress

/*--------------------------------------------------------------------------*/
void TTButton::mouseEnter( int x, int y, TTWindow *win ) {
// Highlights caption text if not disabled.
// Also represses the button if mouse dragged back inside from outside.

  if (isEnabled()) {

    if (wasMouseDownOnButton) {
      pressedDown      = true;

      if (style.buttonTextColorChange)
        fadeColor( &captionColor, getPressedCaptionColor(), style.buttonTextColorSpeed, true );
    }
    else if (style.buttonTextColorChange)
      fadeColor( &captionColor, getHighlightedCaptionColor(), style.buttonTextColorSpeed, true );
  }

  TTWindow::mouseEnter(x, y, win);

} // mouseEnter

/*--------------------------------------------------------------------------*/
void TTButton::mouseExit( int x, int y, TTWindow *win ) {
// Set caption text color back to normal.
// Also depresses the button if mouse dragged outside.

  if (style.buttonTextColorChange)
    fadeColor( &captionColor, getNormalCaptionColor(), style.buttonTextColorSpeed, true  );

  if (wasMouseDownOnButton) pressedDown = false;

  TTWindow::mouseExit(x, y, win);

} // mouseExit

/*--------------------------------------------------------------------------*/
void TTButton::mouseDown( int x, int y, int button, TTWindow *win ) {
// Presses the button

  if (isEnabled()) {
    if (win == this && button == TTGUI_MOUSE_LEFT)  {
      wasMouseDownOnButton = true;
      wasSpaceDownOnButton = false;
      pressButton(true);
      //pressedDown          = true;
      captureMouse();
      if (style.buttonTextColorChange)
        fadeColor( &captionColor, getPressedCaptionColor(), style.buttonTextColorSpeed, true  );
      // Move mouse down with button:
      position_mouse(mouse_x+1, mouse_y+1);
    }
  }

  TTWindow::mouseDown(x, y, button, win);
} // mouseDown

/*--------------------------------------------------------------------------*/
void TTButton::mouseUp( int x, int y, int button, TTWindow *win ) {
// Releases button

  if (button == TTGUI_MOUSE_LEFT && wasMouseDownOnButton)  {
    if (win == this) {
      releaseButton();
      if (style.buttonTextColorChange)
        fadeColor( &captionColor, getHighlightedCaptionColor(), style.buttonTextColorSpeed, true  );
    }
    else cancelPress();

    releaseMouse();

    // Move mouse back with button:
    position_mouse(mouse_x-1, mouse_y-1);
  }

  TTWindow::mouseUp(x, y, button, win);
} // mouseUp

/*--------------------------------------------------------------------------*/
void TTButton::setFocus( bool focus_ ) {
// Used to cancel space-presses when focus changes.
  if (!focus_ && wasSpaceDownOnButton) cancelPress();
  TTWindow::setFocus( focus_ );
} // setFocus

/*--------------------------------------------------------------------------*/
/*----[ Style: ]------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/

void TTButton::initStyle( TTGuiStyle *style_ ) {
// Copies the given TTGuiStyle, or, if it's NULL,
// copies the global style settings, modifies them,
// and calls the initStyle of its child-windows if it has any.
// Called automatically on creation.

  // Call parent:
  //TTWindow::initStyle(style_); Not neccesary, already called from constructor...

  // Select winstyle to use (Override in inherited classes when neccesary):
  if (getApplicationStyle())
    style.winStyle = getApplicationStyle()->buttonStyle;

} // initStyle

/*--------------------------------------------------------------------------*/
// End of class TTButton.
/*--------------------------------------------------------------------------*/

