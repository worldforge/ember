//////////////////////////////////////////////////////////////////////////////
//   File name:  ttguigfx.h
//
//   Unit:       Gui graphics routines.  (Almost) all the lowest level drawing
//               stuff are here.
//
//   Project:    Gui
//   Author:     Hans H„ggstr”m,  hans.haggstromelsinki.fi
//   Based on:   -
//   Purpose:    Borders, areas and allegro text wrapper(?).
//
//////////////////////////////////////////////////////////////////////////////

#ifndef TTGUIGFX_H
#define TTGUIGFX_H

#include <string>
#include <allegro.h>
#include "ttguidef.h"
#include "ttwindow.h"
#include "ttwstyle.h"
#include "m_grafx.h"

typedef struct TTClipRect {
  int cl, cr, ct, cb, clipOn;
} TTClipRect;


/*--------------------------------------------------------------------------*/
/*----[ Clipping rectangle functions: ]-------------------------------------*/
/*--------------------------------------------------------------------------*/

TTClipRect ttGetClipRect( BITMAP *bm );
// Returns clipping rectangle of the given bitmap.

void ttSetClipRect( BITMAP *bm, TTClipRect cr, bool inside = false );
void ttSetClipRect( BITMAP *bm, int cl, int cr, int ct, int cb, bool clip, bool inside = false );
// Sets clipping rectangle of bm to cr.  If inside is true then any edge of
// cr that would go outside the current clipping rectangle of bm is clipped,
// so that the current clipping rectangle of bm defines the maximum dimensions
// of the new clipping rectangle.  Um.


/*--------------------------------------------------------------------------*/
/*----[ Color brightness functions: ]---------------------------------------*/
/*--------------------------------------------------------------------------*/

int ttAdjustColorLum( int color, fix delta );
// Returns color adjusted with delta. Delta range from -1 to 1, where
// 1 represents the full range to white or black.

/*--------------------------------------------------------------------------*/
/*----[ Color mixing: ]-----------------------------------------------------*/
/*--------------------------------------------------------------------------*/

int ttGetColormix( int col1, int col2, int col1strength );
// Returns mix of colors col1 and col2, computed as:
// (col1 * col1strength + col2) / (col1strength + 1).
// Uses cache to speed up mixing (best speedup in palettecolor modes where
// makecol is slow).  Don't use for frequent, random color mixing, as it
// blows the cache (cache is of oldest-out type).
// Used internally for text shadows, etc.

/*--------------------------------------------------------------------------*/
/*----[ Border functions: ]-------------------------------------------------*/
/*--------------------------------------------------------------------------*/

int ttBorderWidth( TTWindow *win );
// Returns the width of the border specified in style, 0 if style == NULL.

void ttDrawBorder( BITMAP *dest, TTWindow *win, int color = -1 );
// Draws a border of width ttBorderWidth() just inside the given window,
// using the borderstyle and width specified in win->style.
// If color is given then it is used instead of the one found in win->style.

void ttDrawFocusOutline( BITMAP *dest, TTWindow *win );
// Draws an outline just outside the window using application bgColor or
// focusColor depending on wether the window has the focus or not.

/*--------------------------------------------------------------------------*/
/*----[ Area functions: ]---------------------------------------------------*/
/*--------------------------------------------------------------------------*/

void ttDrawArea( BITMAP *dest, TTWindow *win );
// Draws an area specified by win->style just inside the border of the given
// window, using the borderstyle and width specified in win->style.

/*--------------------------------------------------------------------------*/
/*----[ Text functions: ]---------------------------------------------------*/
/*--------------------------------------------------------------------------*/

void ttDrawText( BITMAP *dest, TTWindow *win, string text, int color = -1,
                 bool center = true, int textXOffs = 0, int textYOffs = 0,
                 bool drawBg = false, int selStart = 0, int selLength = 0 );
// Draws given text at center of window if center is true, else at upper
// left corner.  Adds text x/y offsets to calculated coordinate.
// Uses win->style to determine text font and if text should be disabled
// or enabled.  If color == -1 the default text color from the style is used.
// selStart and selLength describe a possible selection, that area will be
// drawn with selection colorstyle.  If drawBg is true a rectangle of the
// correct style is drawn behind the selected text.

/*--------------------------------------------------------------------------*/
/*----[ Cursor functions: ]-------------------------------------------------*/
/*--------------------------------------------------------------------------*/

void ttDrawCursor( BITMAP *dest, TTWindow *win, int x, int y, int w, int h,
                   int color, int style );
// Draws a cursor at specified coordinates (relative to the upper left corner
// of the given window) with specified width, height and style (uses
// TTGUI_CURSOR_*** macros for styles).

/*--------------------------------------------------------------------------*/

#endif


