//////////////////////////////////////////////////////////////////////////////
//   File name:  ttstatic.cc
//
//   Class:      TTStatic  Static text
//
//   Project:    Gui
//   Author:     Hans H„ggstr”m,  hans.haggstrom@helsinki.fi
//   Base class: TTWindow
//   Decendants: TTEdit
//   Purpose:    Static, non-changing, text.
//
//
//   Work diary:
// Date      Time     Lines   Things done
// ~~~~~~~~~ ~~~~~~~~ ~~~~~~~ ~~~~~~~~~~~~~~~~~~~~~~
// 1.6.1998  0:30    (141)    Header
// 3.6       0:30     256     Started at the implementation,...
// 13.8      1:30     323     getIndexAt and started on selection drawing.
// 13.8      2:00     417     Selections & doubleclick & stuff.
// 13.8      1:00     426     Fixed some selection bugs.  Now it seems to work fine =)  (and i need sleep--yawn.. |)
// 13.8      0:30     444     Fixed multiwindow and other bugs.  Now loses selection with focus.  Only selectable TTStatics can get the focus.
// 13.9      0:05     308     Moved selection handling to TTEdit
// 18.9      0:15     377     Moved auto height adjusting here from TTEdit.
// 19.9      0:40     456     Added getCharAt
// 20.9      0:10     466     Fixed adjust height bug, had forgot borderwidth.
// 20.9      1:00     477     Fooled around with the scrolling stuff.
//                            Major design flaw there, it's very hard to
//                            implement now...  Have to rewrite/redesign it.
//
// Notes & things to do:
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//
//////////////////////////////////////////////////////////////////////////////

#include "ttstatic.h"
#include <iostream.h>

/*--------------------------------------------------------------------------*/
// class TTStatic
/*--------------------------------------------------------------------------*/

/*----[ Constructors & destructor: ]------------------------------------*/
TTStatic::TTStatic( TTApplication *application_ )
  : TTWindow(application_) {
// Default constructor.
  selectionStart = selectionLength = 0;
  multiline      = false;
  align          = TTGUI_ALIGN_LEFT;
  text           = "";

  tabstop          = false;
  showFocusOutline = false;

  setAutoadjustHeight( false );

  initStyle();
} // Constructor

/*--------------------------------------------------------------------------*/
TTStatic::TTStatic( const TTStatic &source )
  : TTWindow( source ) {
// Copy constructor.
  selectionStart = source.selectionStart;
  selectionLength= source.selectionLength;
  multiline      = source.multiline;
  align          = source.align;
  text           = source.text;

  tabstop          = false;
  showFocusOutline = false;

  setAutoadjustHeight( source.autoAdjustHeight );

  initStyle();
} // Copycon

/*--------------------------------------------------------------------------*/
TTStatic::TTStatic( int x_, int y_, int w_, int h_,
                    TTWindow      *parent_      = NULL,
                    TTApplication *application_ = NULL )
  : TTWindow( x_, y_, w_, h_, parent_, application_ ) {
// Constructor.  If application is null it uses parent application.

  selectionStart = selectionLength = 0;
  multiline      = false;
  align          = TTGUI_ALIGN_LEFT;
  text           = "";

  tabstop          = false;
  showFocusOutline = false;

  setAutoadjustHeight( false );

  initStyle();
} // Constructor

/*--------------------------------------------------------------------------*/
TTStatic::~TTStatic() {
// Destructor.
} // Destructor

/*----------------------------------------------------------------------*/
/*----[ Drawing and other: ]--------------------------------------------*/
void TTStatic::draw( BITMAP *dest ) {
// Draws static text

  // Draw background:
  TTWindow::draw( dest );

  // Draw text:
  drawText( dest );

} // draw

/*--------------------------------------------------------------------------*/
void TTStatic::drawText( BITMAP *dest ) {
// Draws the text using aligment etc.
  if(!dest) return;

  if (!multiline)
    drawTextLine( dest, text, 0 );
  else { // Multiline
    // Get font height:
    FONT *textFont;
    if (getWinStyle()) textFont = getWinStyle()->textFont;
    else               textFont = font;
    int th = text_height( textFont ) + TTGUI_TEXT_VERTICAL_SPACING;

    // Init variables:
    string s           = text;
    int    ty          = 0;

    // Draw rows:
    while (s.length() > 0 && ty <= getVisibleH() + getScrollY()) {
      s   = drawTextLine( dest, s, ty, (int)text.length()-(int)s.length() );
      ty += th;
    }
  }

} // drawText

/*--------------------------------------------------------------------------*/
string TTStatic::drawTextLine( BITMAP *dest, string s, int yoffs_,
                               int nr_of_prev_chars ) {
// Draws a line of text.  Returns the undrawn lines.

  // Extract all chars up to the first newline:
  string t = "";
  unsigned int i, j;
  for (i = 0; i < s.length() && s[i] != '\n'; i++) ;

  // Replace the newline with space(s):
  if (i < s.length() && s[i] == '\n') {
    s[i] = ' ';
    i++;
  }

  j = i;
  for (; i < s.length(); i++) t += s[i];  // Copy the rest of the chars to t.
  if (j < s.length()) s[j] = '\0';
  //**  s.remove( j, s.length());  <-Would crash when j > length.

  // Make c -string:
  char *cstr = string2char(s);

  // Variables:
  FONT *textFont;
  int rowX = TTGUI_TEXT_HORIZONTAL_MARGINALS,
      rowY = TTGUI_TEXT_VERTICAL_MARGINALS + yoffs_;

  // Get font:
  if (getWinStyle()) textFont = getWinStyle()->textFont;
  else textFont = font;

  ////int textW = text_width( textFont, cstr );  //text_length???, not used anyway?

  // Selection:
  // (Not used in TTStatic, but TTEdit and decendants use these drawing routines.)
  int sStart  = selectionStart - nr_of_prev_chars,
      sLength = selectionLength;
  if (sStart < 0) {
    sLength += sStart;
    sStart   = 0;
  }

  // Draw:
  switch (align) {
    case TTGUI_ALIGN_RIGHT:
      rowX += getVirtualW() - text_length(textFont, cstr);
      ttDrawText( dest, this, s, -1, false, rowX, rowY, true,
                  sStart, sLength );
    break;

    case TTGUI_ALIGN_CENTER:
      rowX += getVirtualW() - text_length(textFont, cstr) / 2;
      ttDrawText( dest, this, s, -1, false, rowX, rowY, true,
                  sStart, sLength );
    break;

    case TTGUI_ALIGN_JUSTIFY:
    //break;
    case TTGUI_ALIGN_LEFT:
    //break;
    default:
      ttDrawText( dest, this, s, -1, false, rowX, rowY, true,
                  sStart, sLength );
    break;
  } // end switch.

  // Delete cstring:
  delete[] cstr;

  // Return unprocessed part of input string:
  return t;

} // drawTextLine

/*--------------------------------------------------------------------------*/
bool TTStatic::update( BITMAP *dest ) {
// Draws the parts of the window that have changed since last draw.
// Returns true if anything was drawn.

  bool updated = false;

  updated = TTWindow::update( dest );

  if (redrawText) {
    drawText( dest );
    updated = true;
  }

  return updated;
} // update

/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
int TTStatic::getIndexAt( int x_, int y_ ) {
// Returns index of char at position x,y.

  if (text.length() <= 0) return 0;

  // Offset of coordinate from upper left corner of text:
  int xo = x_ - (getScrX() + TTGUI_TEXT_HORIZONTAL_MARGINALS),
      yo = y_ - (getScrY() + TTGUI_TEXT_VERTICAL_MARGINALS);

  // Adjust for scroll offset:
  xo += getScrollX();
  yo += getScrollY();

  // Add border:
  xo -= getBorderWidthLeft();
  yo -= getBorderWidthTop();

  // Check if above:
  // (if single line then dont return if above or bellow -> more ergonomical
  //  selection process.)
  if (yo < 0 && multiline) return 0;

  // Get font height:
  FONT *textFont;
  if (getWinStyle()) textFont = getWinStyle()->textFont;
  else               textFont = font;
  int th = text_height( textFont ) + TTGUI_TEXT_VERTICAL_SPACING;

  // Get the right textline if multiline:
  unsigned int i = 0;
  if (multiline) {

    // Calculate the textline that the point is on:
    int line = 1;
    if (th > 0) line = yo / th + 1;

    // Move to corresponding line in text:
    int line_in_text = 1;
    for (i = 0; i < text.length() && line_in_text < line; i++)
      if (text[i] == '\n') line_in_text++;

    // Move past newline char:
    //if (line_in_text > 1) i++;
  }
  /*
  else { // Single line:
    // Check if bellow:
    if (yo > th) return text.length();
  }
  */
  // Check if point is to the left of the text:
  if (xo < 0) return i;

  // Check if point is bellow the text:
  if (i >= text.length()) return text.length();

  // Move along the line and calculate the x coordinate until the point is encountered:
  int len = 0;
  char c[2];  // (text_length() wants a c-string..)
  c[1] = '\0';
  for ( ; i < text.length() && (!multiline || text[i] != '\n'); i++) {
    c[0] = text[i];
    len += text_length(textFont, c);
    if (len > xo) return i;
  }

  // Return last char on the line:
  return i;

} // getIndexAt

/*--------------------------------------------------------------------------*/
void TTStatic::getCharAt( int index, int *cx, int *cy, int *cw, int *ch ) {
// Returns x,y,width and height of character at specified index.
// If index is outside it returns cx=cy=cw=ch=0.

  // Check parameters:
  if (cx == NULL && cy == NULL && cw == NULL && ch == NULL) return;

  // Check if index outside:
  if (index < 0 || index > text.length() ) {
    if (cx) *cx = 0;
    if (cy) *cy = 0;
    if (cw) *cw = 0;
    if (ch) *ch = 0;
    return;
  }

  // Init vars:
  unsigned int i = 0;
  unsigned int lineStart = 0;
  int tx = TTGUI_TEXT_HORIZONTAL_MARGINALS,
      ty = TTGUI_TEXT_VERTICAL_MARGINALS,
      tw = 0,
      th = 0;

  // Adjust for scroll offset:
  //tx -= getScrollXOffs();
  //ty -= getScrollYOffs();

  // Get font height:
  FONT *textFont;
  if (getWinStyle()) textFont = getWinStyle()->textFont;
  else               textFont = font;
  th = text_height( textFont );  // Get height.
  int fonth = th + TTGUI_TEXT_VERTICAL_SPACING;

  // Get y:
  if (cy || cx)
  {
    for (i = 0; i < index && i < text.length(); i++)
      if (text[i] == '\n')
      {
        ty        += fonth;
        lineStart  = i+1;
        if (!multiline)
        {
          if (cx) *cx = 0;
          if (cy) *cy = 0;
          if (cw) *cw = 0;
          if (ch) *ch = 0;
          return;
        }
      }
  }

  // Get x:
  if (cx) {
    // Move along the line and calculate the x coordinate until the index is encountered:
    char c[2];  // (text_length() wants a c-string..)
    c[1] = '\0';
    for (i = lineStart; i < index && i < text.length(); i++) {
      c[0] = text[i];
      tx += text_length(textFont, c);
    }
  }

  // Get width:
  if (cw) {
    char c[2];  // (text_length() wants a c-string..)
    if (index >= text.length() || text[index] == '\n') c[0] = ' ';
    else c[0] = text[index];
    c[1] = '\0';
    tw   = text_length(textFont, c);
  }

  // Return values:
  if (cx) *cx = tx;
  if (cy) *cy = ty;
  if (cw) *cw = tw;
  if (ch) *ch = th;

} // getCharAt

/*--------------------------------------------------------------------------*/
/*----[ Style: ]------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/

void TTStatic::initStyle( TTGuiStyle *style_ ) {
// Copies the given TTGuiStyle, or, if it's NULL,
// copies the global style settings, modifies them,
// and calls the initStyle of its child-windows if it has any.
// Called automatically on creation.

  // Call parent:
  //TTWindow::initStyle(style_); Not neccesary, already called.

  // Change border style to no borders:
  style.winStyle.borderStyle = TTGUI_BORDER_NONE;

} // initStyle

/*--------------------------------------------------------------------------*/
/*----[ Accessors: ]--------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
void TTStatic::setVisibleAreaSize(int w, int h) {
// Adjusts the height to fit text lines if autoAdjustHeight is true.

  // Call parent setVisible size:
  TTWindow::setVisibleAreaSize( w, h );

  // Calculate adjusted height:
  int ah = calculateAdjustedHeight();

  // If adjusted height differs from current height then set height again:
  if (ah != getVisibleH())
    TTWindow::setVisibleAreaSize( w, ah );

} // setH

/*--------------------------------------------------------------------------*/
/*----[ Auto adjust height: ]-----------------------------------------------*/
/*--------------------------------------------------------------------------*/

int TTStatic::calculateAdjustedHeight() {
// Calculates the height of the textfield depending on autoAdjustHeight.

  // If autoadjusting of textheight is off we just return current height:
  if (!autoAdjustHeight) return getVisibleH();

  // Get font height:
  FONT *textFont;
  if (getWinStyle()) textFont = getWinStyle()->textFont;
  else               textFont = font;
  int th = text_height( textFont );

  // Height:
  int newh = th + TTGUI_TEXT_VERTICAL_MARGINALS * 2 +
             getBorderWidthTop() + getBorderWidthBottom();

  // Increase newh in steps of one row until reaches current height:
  if (getMultiline()) {
    while (newh + th + TTGUI_TEXT_VERTICAL_SPACING <= getVisibleH())
      newh += th + TTGUI_TEXT_VERTICAL_SPACING;
  }

  // If calculated height was less than current height, then return
  // calculated height, else return old height:
  if (newh <= getVisibleH()) return newh;
  else return getVisibleH();

} // calculateAdjustedHeight

/*--------------------------------------------------------------------------*/
void TTStatic::adjustHeight() {
// Adjusts the height of the textfield depending on autoAdjustHeight.

  // If autoadjusting of textheight is off we have nothing to do here:
  if (!autoAdjustHeight) return;

  // Call set height with current height, setH automatically adjusts the
  // height when neccesary:
  setVisibleAreaSize( getVisibleW(), getVisibleH() );

} // adjustHeight

/*--------------------------------------------------------------------------*/
// End of class TTStatic.
/*--------------------------------------------------------------------------*/

