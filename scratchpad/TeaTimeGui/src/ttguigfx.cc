//////////////////////////////////////////////////////////////////////////////
//   File name:  ttguigfx.cc
//
//   Unit:       Gui graphics routines.  (Almost) all the lowest level drawing
//               stuff are here.
//
//   Project:    Gui
//   Author:     Hans H„ggstr”m,  hans.haggstromelsinki.fi
//   Based on:   -
//   Purpose:    Borders, areas and allegro text wrapper(?).
//
//
//   Work diary:
// Date      Time     Lines   Things done
// ~~~~~~~~~ ~~~~~~~~ ~~~~~~~ ~~~~~~~~~~~~~~~~~~~~~~
// 16.5.98   0:30     158     Borders
// 16.5      2:00     276     Added area and improved & debugged borders.
// 17.5      0:30     325     Focus outline drawing.  Idea:  Make it flash
//                            up to a specified color and back again when
//                            the focus changes.  Would look cooool =)
//                            Or give it startcolor, topcolor an final color,
//                            and nr of steps.  Just interpolate colorchange
//                            linearily over palette -> only (smooth) changes
//                            within one hue.
//
// 25.5      2:30     510     ttDrawText && clipstuff && colorstuff.
// 13.8      0:15     565     Started to add support for selected text.
// 13.8      3:00     705     Added support for selected text.
// 12.9      0:30     731     Added scrolling support.
// 13.9      0:15     734     Added color parameter to draw border.
// 13.9      0:01     735     Fixed text shadow for selected text.
// 18.9      2:00     857     Added TTGUI_TEXT_SOFT and colormixing.
// 19.9      0:30     914     Added ttDrawCursor.
// 20.9      0:10     943     Fixed ttDrawCursor clipping bug.
//
// Notes & things to do:
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// * Skip palinfo stuff?
// * Separate border shape and colors/style: round, diamond, tab & normal
//   borders that go up, down, updown, softly, etc...
// * Add (scalable) marks and simple symbols: arrows, checks, dots, etc.
// * Make some general functions for clipping rectangle manipulation etc..
// * Use ttGetColormix to calculate shadow / highlight.
// * What about transparent cursor? or something..
//
//////////////////////////////////////////////////////////////////////////////

#include <iostream.h>
#include "ttguigfx.h"
#include "misc_str.h"
#include "ptrlist.h"

#define TTGUIGFX_COLORMIX_CACHE_SIZE 32

/*--------------------------------------------------------------------------*/
/*----[ Colormixing: ]------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
class TTColormixCacheEntry {
  public:
    int col1, col2, col1strength, mixcol;

    TTColormixCacheEntry( int col1_, int col2_, int col1strength_, int mixcol_ ) {
      col1         = col1_;
      col2         = col2_;
      col1strength = col1strength_;
      mixcol       = mixcol_;
    }
}; // class TTColormixCacheEntry.

/*--------------------------------------------------------------------------*/
PtrList<TTColormixCacheEntry*> ttColormixCache =
  PtrList<TTColormixCacheEntry*>();

/*--------------------------------------------------------------------------*/
int ttGetColormix( int col1, int col2, int col1strength ) {
// Returns mix of colors col1 and col2, computed as:
// (col1 * col1strength + col2) / (col1strength + 1).
// Uses cache to speed up mixing.

  // Search for previous mix in cache:
  for (ttColormixCache.first(); !ttColormixCache.isOutside(); ttColormixCache.next()) {
    TTColormixCacheEntry *p = ttColormixCache.current();
    if ( p &&
         p->col1 == col1 &&
         p->col2 == col2 &&
         p->col1strength == col1strength ) {

      // Move cache hit entry to front of list:
      if (!ttColormixCache.atStart()) {
        ttColormixCache.remove();
        ttColormixCache.addFirst(p);
      }

      // Get cached mixcolor:
      return p->mixcol;
    }
  }

  // No cache hit was found, calculate color:
  int r = ( getr(col1)*col1strength + getr(col2) ) / (col1strength+1);
  int g = ( getg(col1)*col1strength + getg(col2) ) / (col1strength+1);
  int b = ( getb(col1)*col1strength + getb(col2) ) / (col1strength+1);
  int mixcolor = makecol( r, g, b );

  // If list is full replace the last cache entry and move it to front,
  // else create a new cache entry:
  TTColormixCacheEntry *p;
  if (ttColormixCache.size() >= TTGUIGFX_COLORMIX_CACHE_SIZE && ttColormixCache.last()) {
    p = ttColormixCache.last();
    p->col1         = col1;
    p->col2         = col2;
    p->col1strength = col1strength;
    p->mixcol       = mixcolor;
    ttColormixCache.remove();
    ttColormixCache.addFirst( p );
  }
  else {
    // List not full:
    // Create new cache entry:
    p = new TTColormixCacheEntry( col1, col2, col1strength, mixcolor );
    ttColormixCache.addFirst( p );
  }

  return mixcolor;
} // ttGetColormix

/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/*----[ Clipping rectangle functions: ]-------------------------------------*/
/*--------------------------------------------------------------------------*/

TTClipRect ttGetClipRect( BITMAP *bm ) {
// Returns clipping rectangle of the given bitmap.
  TTClipRect cr;

  cr.cl = 0;
  cr.cr = 0;
  cr.ct = 0;
  cr.cb = 0;
  cr.clipOn = false;
  if (!bm) return cr;

  cr.cl = bm->cl;
  cr.cr = bm->cr;
  cr.ct = bm->ct;
  cr.cb = bm->cb;
  cr.clipOn = bm->clip;
  return cr;
} // ttGetClipRect

/*--------------------------------------------------------------------------*/

void ttSetClipRect( BITMAP *bm, TTClipRect cr, bool inside ) {
// Sets clipping rectangle of bm to cr.  If inside is true then any edge of
// cr that would go outside the current clipping rectangle of bm is clipped,
// so that the current clipping rectangle of bm defines the maximum dimensions
// of the new clipping rectangle.  Um.

  if (!bm) return;

  if (inside) {
    if (cr.cl > bm->cl) bm->cl = cr.cl;
    if (cr.cr < bm->cr) bm->cr = cr.cr;
    if (cr.ct > bm->ct) bm->ct = cr.ct;
    if (cr.cb < bm->cb) bm->cb = cr.cb;
  }
  else {
    bm->cl = cr.cl;
    bm->cr = cr.cr;
    bm->ct = cr.ct;
    bm->cb = cr.cb;
  }

  bm->clip = cr.clipOn;

} // ttSetClipRect

/*--------------------------------------------------------------------------*/
void ttSetClipRect( BITMAP *bm, int cl, int cr, int ct, int cb, bool clip, bool inside ) {
// Sets clipping rectangle of bm to cr.  If inside is true then any edge of
// cr that would go outside the current clipping rectangle of bm is clipped,
// so that the current clipping rectangle of bm defines the maximum dimensions
// of the new clipping rectangle.  Um.

  if (!bm) return;

  if (inside) {
    if (cl > bm->cl) bm->cl = cl;
    if (cr < bm->cr) bm->cr = cr;
    if (ct > bm->ct) bm->ct = ct;
    if (cb < bm->cb) bm->cb = cb;
  }
  else {
    bm->cl = cl;
    bm->cr = cr;
    bm->ct = ct;
    bm->cb = cb;
  }

  bm->clip = clip;

} // ttSetClipRect

/*--------------------------------------------------------------------------*/
/*----[ Color brightness functions: ]---------------------------------------*/
/*--------------------------------------------------------------------------*/

int ttAdjustColorLum( int color, float delta ) {
// Returns color adjusted with delta. Delta range from -1 to 1, where
// 1 represents the full range to white or black.

  PaletteInfo *palinfo = PaletteInfo::getCurrent();
  if (palinfo) {
    int d;
    d = (int)((float)palinfo->hueLength * delta);
    return palinfo->modifyLumRel(color, d);
  }
  else {
    int r = getr(color),
        g = getg(color),
        b = getb(color);
    delta = delta * 63;
    return makecol(r+(int)delta, g+(int)delta, b+(int)delta);
  }

} // ttAdjustColorLum

/*--------------------------------------------------------------------------*/
/*----[ Border functions: ]-------------------------------------------------*/
/*--------------------------------------------------------------------------*/

int ttBorderWidth( TTWindow *win ) {
// Returns the width of the border specified in style, 0 if style == NULL.
  if (!win) return 0;
  int w = 1;
  if (win->getApplicationStyle()) w  = win->getApplicationStyle()->linewidth;
  if (win->getWinStyle())         w *= win->getWinStyle()->borderWidth;
  return w;
} // ttBorderWidth

/*--------------------------------------------------------------------------*/
void ttDrawBorder( BITMAP *dest, TTWindow *win, int color ) {
// Draws a border of width ttBorderWidth() just inside the given window,
// using the borderstyle and width specified in win->style.
// If color is given then it is used instead of the one found in win->style.

  // Check parameters:
  if (!dest || !win) return;

  // Get style:
  TTWindowStyle *style;
  style = win->getWinStyle();
  if (!style) return;

  // Calculate colors:
  int dark, shadow, flat, highlight, bright, black, c1, c2, c3, c4;
  flat      = style->bgColor;
  if (color != -1) flat = color;
  dark      = ttAdjustColorLum( flat, 0.0625 * -2 * TTGUI_BORDER_CONTRAST );
  shadow    = ttAdjustColorLum( flat, 0.0625 * -1 * TTGUI_BORDER_CONTRAST );
  highlight = ttAdjustColorLum( flat, 0.0625 *  1 * TTGUI_BORDER_CONTRAST );
  bright    = ttAdjustColorLum( flat, 0.0625 *  2 * TTGUI_BORDER_CONTRAST );
  black     = ttAdjustColorLum( flat, -2);

  int bs = style->borderStyle;
  if ( bs == TTGUI_BORDER_UP ||
       bs == TTGUI_BORDER_UPDOWN ||
       bs == TTGUI_BORDER_SOFTUP ) {
    c1 = bright;
    c2 = highlight;
    c3 = shadow;
    c4 = dark;
  }
  else {
    c1 = dark;
    c2 = shadow;
    c3 = highlight;
    c4 = bright;
  }

  if (bs == TTGUI_BORDER_PRESSED) {
    c4 = c3;
    c3 = flat;
    bs = TTGUI_BORDER_DOWN;
  }

  // Calculate coordinates:
  int x1, y1, x2, y2, w, lw, i = 0;
  x1 = win->getScrX();
  y1 = win->getScrY();
  x2 = win->getScrX() + win->getVisibleW() - 1;
  y2 = win->getScrY() + win->getVisibleH() - 1;
  if (x2 < x1 || y2 < y1) return;
  w  = ttBorderWidth(win);
  lw = 1;
  if (win->getApplicationStyle()) lw = win->getApplicationStyle()->linewidth;

  // Draw border:
  switch (bs) {

     case TTGUI_BORDER_FLAT:
       for (i = 0; i < w; i++)
         rect( dest, x1+i, y1+i, x2-i, y2-i, flat );
     break;

     case TTGUI_BORDER_DARK:
       for (i = 0; i < w; i++)
         rect( dest, x1+i, y1+i, x2-i, y2-i, dark );
     break;

     case TTGUI_BORDER_BRIGHT:
       for (i = 0; i < w; i++)
         rect( dest, x1+i, y1+i, x2-i, y2-i, bright );
     break;

     case TTGUI_BORDER_BLACK:
       for (i = 0; i < w; i++)
         rect( dest, x1+i, y1+i, x2-i, y2-i, black );
     break;

     case TTGUI_BORDER_UP:
     case TTGUI_BORDER_DOWN:
       i = 0;
       if (style->borderWidth > 1)
         for (i = 0; i < lw && i < w; i++) {
           vline( dest, x1+i,   y1+i, y2-i-1, c1);
           hline( dest, x1+i+1, y1+i, x2-i-1, c1);
           vline( dest, x2-i,   y1+i, y2-i,   c4);
           hline( dest, x1+i,   y2-i, x2-i-1, c4);
         }
       for (; i < lw*2 && i < w; i++) {
         vline( dest, x1+i,   y1+i, y2-i-1, c2);
         hline( dest, x1+i+1, y1+i, x2-i-1, c2);
         vline( dest, x2-i,   y1+i, y2-i,   c3);
         hline( dest, x1+i,   y2-i, x2-i-1, c3);
       }
       for (; i < w; i++) {
         // Replace with a rect() call...
         vline( dest, x1+i,   y1+i, y2-i-1, flat);
         hline( dest, x1+i+1, y1+i, x2-i-1, flat);
         vline( dest, x2-i,   y1+i, y2-i,   flat);
         hline( dest, x1+i,   y2-i, x2-i-1, flat);
       }
       line( dest, x1, y2, x1+w, y2-w, flat);
       line( dest, x2, y1, x2-w, y1+w, flat);
     break;

     case TTGUI_BORDER_UPDOWN:
     case TTGUI_BORDER_DOWNUP:
       for (i = 0; i < lw && i < w / 2; i++) {
         vline( dest, x1+i,   y1+i, y2-i-1, c2);
         hline( dest, x1+i+1, y1+i, x2-i-1, c2);
         vline( dest, x2-i,   y1+i, y2-i,   c3);
         hline( dest, x1+i,   y2-i, x2-i-1, c3);
       }
       for (; i < w - lw; i++) {
         // Replace with a rect() call...
         vline( dest, x1+i,   y1+i, y2-i-1, flat);
         hline( dest, x1+i+1, y1+i, x2-i-1, flat);
         vline( dest, x2-i,   y1+i, y2-i,   flat);
         hline( dest, x1+i,   y2-i, x2-i-1, flat);
       }
       for (; i < w; i++) {
         vline( dest, x1+i,   y1+i, y2-i-1, c3);
         hline( dest, x1+i+1, y1+i, x2-i-1, c3);
         vline( dest, x2-i,   y1+i, y2-i,   c2);
         hline( dest, x1+i,   y2-i, x2-i-1, c2);
       }
       line( dest, x1, y2, x1+w, y2-w, flat);
       line( dest, x2, y1, x2-w, y1+w, flat);
     break;

     case TTGUI_BORDER_SOFTUP:
     case TTGUI_BORDER_SOFTDOWN:
       for (i = 0; i < lw && i < w / 2; i++) {
         vline( dest, x1+i,   y1+i, y2-i-1, c2);
         hline( dest, x1+i+1, y1+i, x2-i-1, c2);
         vline( dest, x2-i,   y1+i, y2-i,   c3);
         hline( dest, x1+i,   y2-i, x2-i-1, c3);
       }
       for (; i < w/2 + lw && i < w; i++) {
         vline( dest, x1+i,   y1+i, y2-i-1, c1);
         hline( dest, x1+i+1, y1+i, x2-i-1, c1);
         vline( dest, x2-i,   y1+i, y2-i,   c4);
         hline( dest, x1+i,   y2-i, x2-i-1, c4);
       }
       for (; i < w; i++) {
         vline( dest, x1+i,   y1+i, y2-i-1, c2);
         hline( dest, x1+i+1, y1+i, x2-i-1, c2);
         vline( dest, x2-i,   y1+i, y2-i,   c3);
         hline( dest, x1+i,   y2-i, x2-i-1, c3);
       }
       line( dest, x1, y2, x1+w, y2-w, flat);
       line( dest, x2, y1, x2-w, y1+w, flat);
     break;

     default:
     break;
  } // end switch.

} // ttDrawBorder

/*--------------------------------------------------------------------------*/
void ttDrawFocusOutline( BITMAP *dest, TTWindow *win ) {
// Draws an outline just outside the window using application bgColor or
// focusColor depending on wether the window has the focus or not.

  // Check parameters:
  if (!dest || !win) return;

  // Get focus color:
  int col = win->getFocusColor();

  // Calculate coordinates:
  int x1, y1, x2, y2;
  x1 = win->getScrX() - 1;
  y1 = win->getScrY() - 1;
  x2 = win->getScrX() + win->getVisibleW();
  y2 = win->getScrY() + win->getVisibleH();
  if (x2 < x1 || y2 < y1) return;

  // Draw outline with rounded corners:
  hline( dest, x1+1, y1,   x2-1, col );
  hline( dest, x1+1, y2,   x2-1, col );
  vline( dest, x1,   y1+1, y2-1, col );
  vline( dest, x2,   y1+1, y2-1, col );

} // ttDrawFocusOutline


/*--------------------------------------------------------------------------*/
/*----[ Area functions: ]---------------------------------------------------*/
/*--------------------------------------------------------------------------*/

void ttDrawArea( BITMAP *dest, TTWindow *win ) {
// Draws an area specified by win->style just inside the border of the given
// window, using the borderstyle and width specified in win->style.

  // Check parameters:
  if (!dest || !win) return;

  // Get style:
  TTWindowStyle *style;
  style = win->getWinStyle();
  if (!style) return;

  // Return if empty area style:
  if (style->areaStyle == TTGUI_AREA_NONE) return;

  // Calculate coordinates:
  int x1, y1, x2, y2, bw, w, h, xc, yc, i, j, sx, sy, vh, vw, clipx2, clipy2;
  vw = win->getVirtualW();
  vh = win->getVirtualH();
  bw = ttBorderWidth(win);
  
  x1 = win->getScrX() + win->getBorderWidthLeft(); // Or possibly use just bw?
  y1 = win->getScrY() + win->getBorderWidthTop();
  x2 = win->getScrX() + win->getVisibleW() - 1 - win->getBorderWidthRight();
  y2 = win->getScrY() + win->getVisibleH() - 1 - win->getBorderWidthBottom();
    
/*
  x1 = win->getX() + win->getXOffs() + bw;
  y1 = win->getY() + win->getYOffs() + bw;
  x2 = win->getX() + win->getXOffs() + vw - 1 - bw;
  y2 = win->getY() + win->getYOffs() + vh - 1 - bw;
  clipx2 = win->getX() + win->getXOffs() + win->getW() - 1 - bw;
  clipy2 = win->getY() + win->getYOffs() + win->getH() - 1 - bw;
*/
  clipx2 = x2;  /*** ! Fix this stuff later.... ! ****/
  clipy2 = y2;
  
  if (x2 < x1 || y2 < y1) return;
  w  = style->bgBitmap->w;
  h  = style->bgBitmap->h;
  sx = win->getScrollX();
  sy = win->getScrollY();
  int axo = style->areaXOffs;
  int ayo = style->areaYOffs;
  xc = (x1 + x2 - w + 1) / 2 + axo;
  yc = (y1 + y2 - h + 1) / 2 + ayo;

  // If no bitmap is given then draw filled rectangle (or apply color
  // remapping table on background..):
  if (!style->bgBitmap || style->areaStyle == TTGUI_AREA_FLAT || w == 0 || h == 0) {
    // if (colorRemaping table) apply color remapping table..
    rectfill( dest, x1, y1, clipx2, clipy2, style->bgColor );
    return;
  }

  // Set clipping rectangle:
  TTClipRect clipRect;
  clipRect = ttGetClipRect( dest );
  ttSetClipRect( dest, x1, clipx2+1, y1, clipy2+1, true, true );

  // Apply scrolling:
  x1 -= sx;
  x2 -= sx;
  xc -= sx;
  y1 -= sy;
  y2 -= sy;
  yc -= sy;


  // Draw bitmap using specified area style:
  switch ( style->areaStyle ) {

     case TTGUI_AREA_CORNER:
       // Blit bitmap:
       blit( style->bgBitmap, dest, 0, 0, x1, y1, w, h );

       // Fill rest of the area with solid color:
       if(x2-x1 >= w) rectfill( dest, x1+w, y1,   x2, y1+h-1, style->bgColor );
       if(y2-y1 >= h) rectfill( dest, x1,   y1+h, x2, y2,     style->bgColor );
     break;

     case TTGUI_AREA_CENTER:
       // Blit bitmap:
       blit( style->bgBitmap, dest, 0, 0, xc, yc, w, h );

       // Fill rest of the area with solid color:
       if(y2-y1 >  h) rectfill( dest, x1,   y1,   x2,   yc-1, style->bgColor );
       if(x2-x1 >  w) rectfill( dest, x1,   yc,   xc-1, yc+h, style->bgColor );
       if(x2-x1 >= w) rectfill( dest, xc+w, yc,   x2,   yc+h, style->bgColor );
       if(y2-y1 >= h) rectfill( dest, x1,   yc+h, x2,   y2,   style->bgColor );
     break;

     case TTGUI_AREA_STRETCH:
       stretch_blit( style->bgBitmap, dest, 0, 0, w, h, x1, y1, x2-x1+1, y2-y1+1 );
     break;

     case TTGUI_AREA_TILE:
       if (axo > 0) axo = axo % w - w;
       if (ayo > 0) ayo = ayo % h - h;
       for (j = y1+ayo; j <= y2; j += h)
         for (i = x1+axo; i <= x2; i += w)
           blit( style->bgBitmap, dest, 0, 0, i, j, w, h );
     break;

     default:
     break;
  } // end switch.

  // Restore clipping rectangle:
  ttSetClipRect( dest, clipRect );

} // ttDrawArea

/*--------------------------------------------------------------------------*/
/*----[ Text functions: ]---------------------------------------------------*/
/*--------------------------------------------------------------------------*/

void ttDrawText( BITMAP *dest, TTWindow *win, string text, int color,
                 bool center, int textXOffs, int textYOffs, bool drawBg,
                 int selStart, int selLength ) {
// Draws given text at center of window if center is true, else at upper
// left corner.  Adds text x/y offsets to calculated coordinate.
// Uses win->style to determine text font and if text should be disabled
// or enabled. If color == -1 the default text color from the style is used.
// selStart and selLength describe a possible selection, that area will be
// drawn with selection colorstyle.

  // Check parameters:
  if (!dest || !win || text == "") return;

  //// Get style & colors:
  TTWindowStyle *style = win->getWinStyle();
  if (!style) return;
  int bgCol;
  int fgCol;

  // Check normal style:
  if ( selStart + selLength < 0 || selStart >= text.length() || selLength == 0) {
    drawBg = false; // No background.
    bgCol  = style->bgColor;
    fgCol  = style->textColor;
  }
  // Check selected style:
  else if (selStart <= 0 && selStart + selLength >= text.length()) {
    TTGuiStyle *guiStyle = win->getGuiStyle();
    if (win->getGuiStyle()) {
      fgCol = guiStyle->selectionTextColor;
      bgCol = guiStyle->selectionBgColor;
    }
    else {
      // If no gui style given, use inversion:
      fgCol = style->bgColor;
      bgCol = style->textColor;
    }
  }
  // Mixed style, use recursion:
  else {
    int idx  = 0;
    int xo   = 0;
    char *cs = NULL;
    string t;
    // Draw normal part before selection:
    if (selStart > 0) {
      t = "";
      for (idx = 0; idx < text.length() && idx < selStart && text[idx] != '\n' && text[idx] != '\0'; idx++) t += text[idx];
      ttDrawText( dest, win, t, color, center, textXOffs, textYOffs, drawBg, 0, 0 );

      // Get width of text-part:
      if (win->getWinStyle()) {
        cs = string2char(t);
        xo += text_length( win->getWinStyle()->textFont, cs );
        if (cs) delete[] cs;
      }
    }

    // Draw selection:
    if ( selStart < text.length() && selLength > 0) {
      for (t=""; idx < text.length() && idx < selStart+selLength && text[idx] != '\n' && text[idx] != '\0'; idx++) t += text[idx];

      /* Doesn't seem to work.  Why?
      // Draw selected newlines as (lots of?) spaces:
      if (idx < text.length() && idx < selStart+selLength && text[idx] == '\n') {
        t += " "; // ** Make some code to calculate how many spaces to add to fill the whole line..  Or just add a 'fill bg to eol' flag or something to ttDrawtext...
      }
      */

      // Draw text:
      ttDrawText( dest, win, t, color, center, textXOffs+xo, textYOffs, drawBg, 0, selLength+1 );

      // Get width of text-part:
      if (win->getWinStyle()) {
        cs = string2char(t);
        xo += text_length( win->getWinStyle()->textFont, cs );
        if (cs) delete[] cs;
      }
    }

    // Draw normal part after selection:
    if (selStart + selLength < text.length()) {
      for (t=""; idx < text.length() && text[idx] != '\n' && text[idx] != '\0'; idx++) t += text[idx];
      ttDrawText( dest, win, t, color, center, textXOffs+xo, textYOffs, drawBg, 0, 0 );
    }

    return;
  } // End recursive selection drawing.


  // Calc color and font:
  if (color == -1)  color = fgCol;
  FONT          *textFont = style->textFont;
  if (!textFont) textFont = font; // Use default font.

  // Calculate shadow and highlight:
  int shadow    = ttAdjustColorLum( bgCol, -0.0625 * TTGUI_BORDER_CONTRAST );
  int highlight = ttAdjustColorLum( bgCol,  0.0625 * TTGUI_BORDER_CONTRAST );

  // Calculate coordinates:
  int x1, y1, x2, y2, bw, xc, yc, cx1, cx2, cy1, cy2;
  bw = ttBorderWidth(win);
  cx1 = win->getScrX() + win->getBorderWidthLeft();
  cy1 = win->getScrY() + win->getBorderWidthTop();
  cx2 = win->getScrX() + win->getVisibleW() - 1 - win->getBorderWidthRight();
  cy2 = win->getScrY() + win->getVisibleH() - 1 - win->getBorderWidthBottom();

  x1 = cx1 - win->getScrollX();
  y1 = cy1 - win->getScrollY();
  x2 = x1 + win->getVirtualW() - 1;
  y2 = y1 + win->getVirtualH() - 1;
  if (x2 < x1 || y2 < y1) return;
  xc = textXOffs;
  yc = textYOffs;
  if (center) {
    xc += (x1 + x2 + 1) / 2;
    yc += (y1 + y2 + 1 - text_height(textFont)) / 2;
  }
  else {
    xc += x1;
    yc += y1;
  }

  // Set clipping rectangle:
  TTClipRect clipRect;
  clipRect = ttGetClipRect( dest );
  ttSetClipRect( dest, cx1, cx2+1, cy1, cy2+1, true, true );

  // Get c string:
  char *cstr = string2char(text);
  if (!cstr) return;

  // Get text style:
  int textStyle = style->textStyle;

  // Check if disabled:
  if (!win->isEnabled()) textStyle = TTGUI_TEXT_GRAYED;

  // Check if soft(ed) text:
  if (textStyle == TTGUI_TEXT_SOFT) {
    // Calculate mix of textcolor and background color.  Put a mainly
    // bgcol mix in shadow and a bit more textcolored mix in highlight:
    shadow    = ttGetColormix( bgCol, color, 7 );
    highlight = ttGetColormix( bgCol, color, 3 );
    //textStyle = TTGUI_TEXT_SHADOW;
  }

  text_mode(-1); // Transparent text

  // Draw text:
  switch (textStyle) {
    case TTGUI_TEXT_SHADOW:
      if (center) {
        if (drawBg) {
          text_mode(bgCol);
          textout_centre( dest, textFont, cstr, xc,   yc,   color);
          text_mode(-1);
        }
        //textout_centre( dest, textFont, cstr, xc+1, yc+1, shadow);
        textout_centre( dest, textFont, cstr, xc  , yc+1, shadow);
        textout_centre( dest, textFont, cstr, xc+1, yc  , shadow);
        textout_centre( dest, textFont, cstr, xc,   yc,   color);
      } else {
        if (drawBg) {
          text_mode(bgCol);
          textout( dest, textFont, cstr, xc,   yc,   color);
          text_mode(-1);
        }
        //textout( dest, textFont, cstr, xc+1, yc+1, shadow);
        textout( dest, textFont, cstr, xc  , yc+1, shadow);
        textout( dest, textFont, cstr, xc+1, yc  , shadow);
        textout( dest, textFont, cstr, xc,   yc,   color);
      }
    break;

    case TTGUI_TEXT_DROPSHADOW:
      if (center) {
        if (drawBg) {
          text_mode(bgCol);
          textout_centre( dest, textFont, cstr, xc, yc, color);
          text_mode(-1);
        }
        textout_centre( dest, textFont, cstr, xc+2, yc+2, shadow);
        textout_centre( dest, textFont, cstr, xc,   yc,   color);
      } else {
        if (drawBg) {
          text_mode(bgCol);
          textout( dest, textFont, cstr, xc, yc, color);
          text_mode(-1);
        }
        textout( dest, textFont, cstr, xc+2, yc+2, shadow);
        textout( dest, textFont, cstr, xc,   yc,   color);
      }
    break;

    case TTGUI_TEXT_GRAYED:
      if (center) {
        if (drawBg) {
          text_mode(bgCol);
          textout_centre( dest, textFont, cstr, xc,   yc,   color);
          text_mode(-1);
        }
        textout_centre( dest, textFont, cstr, xc+1, yc+1, highlight);
        textout_centre( dest, textFont, cstr, xc,   yc,   shadow);
      } else {
        if (drawBg) {
          text_mode(bgCol);
          textout( dest, textFont, cstr, xc,   yc,   color);
          text_mode(-1);
        }
        textout( dest, textFont, cstr, xc+1, yc+1, highlight);
        textout( dest, textFont, cstr, xc,   yc,   shadow);
      }
    break;

    case TTGUI_TEXT_RAISED:
      if (center) {
        if (drawBg) {
          text_mode(bgCol);
          textout_centre( dest, textFont, cstr, xc,   yc,   color);
          text_mode(-1);
        }
        //textout_centre( dest, textFont, cstr, xc-1, yc-1, highlight);
        textout_centre( dest, textFont, cstr, xc,   yc-1, highlight);
        textout_centre( dest, textFont, cstr, xc-1, yc,   highlight);
        //textout_centre( dest, textFont, cstr, xc+1, yc+1, shadow);
        textout_centre( dest, textFont, cstr, xc,   yc+1, shadow);
        textout_centre( dest, textFont, cstr, xc+1, yc,   shadow);
        textout_centre( dest, textFont, cstr, xc,   yc,   color);
      } else {
        if (drawBg) {
          text_mode(bgCol);
          textout( dest, textFont, cstr, xc,   yc,   color);
          text_mode(-1);
        }
        //textout( dest, textFont, cstr, xc-1, yc-1, highlight);
        textout( dest, textFont, cstr, xc,   yc-1, highlight);
        textout( dest, textFont, cstr, xc-1, yc,   highlight);
        //textout( dest, textFont, cstr, xc+1, yc+1, shadow);
        textout( dest, textFont, cstr, xc,   yc+1, shadow);
        textout( dest, textFont, cstr, xc+1, yc,   shadow);
        textout( dest, textFont, cstr, xc,   yc,   color);
      }
    break;

    case TTGUI_TEXT_LOWERED:
      if (center) {
        if (drawBg) {
          text_mode(bgCol);
          textout_centre( dest, textFont, cstr, xc,   yc,   color);
          text_mode(-1);
        }
        //textout_centre( dest, textFont, cstr, xc+1, yc+1, highlight);
        textout_centre( dest, textFont, cstr, xc,   yc+1, highlight);
        textout_centre( dest, textFont, cstr, xc+1, yc,   highlight);
        //textout_centre( dest, textFont, cstr, xc-1, yc-1, shadow);
        textout_centre( dest, textFont, cstr, xc,   yc-1, shadow);
        textout_centre( dest, textFont, cstr, xc-1, yc,   shadow);
        textout_centre( dest, textFont, cstr, xc,   yc,   color);
      } else {
        if (drawBg) {
          text_mode(bgCol);
          textout( dest, textFont, cstr, xc,   yc,   color);
          text_mode(-1);
        }
        //textout( dest, textFont, cstr, xc+1, yc+1, highlight);
        textout( dest, textFont, cstr, xc,   yc+1, highlight);
        textout( dest, textFont, cstr, xc+1, yc,   highlight);
        //textout( dest, textFont, cstr, xc-1, yc-1, shadow);
        textout( dest, textFont, cstr, xc,   yc-1, shadow);
        textout( dest, textFont, cstr, xc-1, yc,   shadow);
        textout( dest, textFont, cstr, xc,   yc,   color);
      }
    break;

    case TTGUI_TEXT_SOFT:
      if (center) {
        if (drawBg) {
          text_mode(bgCol);
          textout_centre( dest, textFont, cstr, xc,   yc,   color);
          text_mode(-1);
        }
        textout_centre( dest, textFont, cstr, xc+1, yc+1, shadow);
        textout_centre( dest, textFont, cstr, xc+1, yc-1, shadow);
        textout_centre( dest, textFont, cstr, xc-1, yc+1, shadow);
        textout_centre( dest, textFont, cstr, xc-1, yc-1, shadow);
        textout_centre( dest, textFont, cstr, xc,   yc+1, highlight);
        textout_centre( dest, textFont, cstr, xc+1, yc,   highlight);
        textout_centre( dest, textFont, cstr, xc,   yc-1, highlight);
        textout_centre( dest, textFont, cstr, xc-1, yc,   highlight);
        textout_centre( dest, textFont, cstr, xc,   yc,   color);
      } else {
        if (drawBg) {
          text_mode(bgCol);
          textout( dest, textFont, cstr, xc,   yc,   color);
          text_mode(-1);
        }
        textout( dest, textFont, cstr, xc+1, yc+1, shadow);
        textout( dest, textFont, cstr, xc+1, yc-1, shadow);
        textout( dest, textFont, cstr, xc-1, yc+1, shadow);
        textout( dest, textFont, cstr, xc-1, yc-1, shadow);
        textout( dest, textFont, cstr, xc,   yc+1, highlight);
        textout( dest, textFont, cstr, xc+1, yc,   highlight);
        textout( dest, textFont, cstr, xc,   yc-1, highlight);
        textout( dest, textFont, cstr, xc-1, yc,   highlight);
        textout( dest, textFont, cstr, xc,   yc,   color);
      }
    break; // TTGUI_TEXT_SOFT

    case TTGUI_TEXT_NORMAL:
    default:
      if (center) {
        if (drawBg) text_mode(bgCol);
        textout_centre( dest, textFont, cstr, xc, yc, color);
      }
      else {
        if (drawBg) text_mode(bgCol);
        textout( dest, textFont, cstr, xc, yc, color);
      }
    break;
  } // end switch.

  // Delete c string:
  delete[] cstr;

  // Transparent text background:
  text_mode(-1);

  // Restore clipping rectangle:
  ttSetClipRect( dest, clipRect );

} // ttDrawText

/*--------------------------------------------------------------------------*/
/*----[ Cursor functions: ]-------------------------------------------------*/
/*--------------------------------------------------------------------------*/

void ttDrawCursor( BITMAP *dest, TTWindow *win, int x, int y, int w, int h,
                   int color, int style ) {
// Draws a cursor at specified coordinates (relative to the upper left corner
// of the given window) with specified width, height and style (uses
// TTGUI_CURSOR_*** macros for styles).

  // Check parameters:
  if (!dest || !win) return;
  if (w <= 0 || h <= 0) return;

  // Calculate coordinates:
  int x1, y1, x2, y2, bw, t, wx1, wy1, wx2, wy2, cx1, cx2, cy1, cy2;
  bw  = ttBorderWidth(win);
  
  cx1 = win->getScrX() + win->getBorderWidthLeft();
  cy1 = win->getScrY() + win->getBorderWidthTop();
  cx2 = win->getScrX() + win->getVisibleW() - 1 - win->getBorderWidthRight();
  cy2 = win->getScrY() + win->getVisibleH() - 1 - win->getBorderWidthBottom();

  wx1 = cx1 - win->getScrollX();
  wy1 = cy1 - win->getScrollY();
  wx2 = wx1 + win->getVirtualW() - 1;
  wy2 = wy1 + win->getVirtualH() - 1;

  x1  = wx1 + x - 1;
  y1  = wy1 + y - 1;
  x2  = x1  + w;
  y2  = y1  + h + 1;

  // Set draw mode to transparent:
//  drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);

  // Set clipping rectangle:
  TTClipRect clipRect;
  clipRect = ttGetClipRect( dest );
  ttSetClipRect( dest, cx1, cx2, cy1, cy2, true, true );

  // Draw cursor:
  switch (style) {
    case TTGUI_CURSOR_HORIZONTAL:
      line( dest, x1, y2, x2, y2, color );
    break;

    case TTGUI_CURSOR_BOX:
      rect( dest, x1, y1, x2, y2, color );
    break;

    case TTGUI_CURSOR_BAR:
      rectfill( dest, x1, y1, x2, y2, color );
    break;

    case TTGUI_CURSOR_I_BEAM:
      t = (y2 - y1) / 8;
      line( dest, x1,   y1+1, x1,   y2-1, color );
      line( dest, x1-t, y1,   x1+t, y1,   color );
      line( dest, x1-t, y2,   x1+t, y2,   color );
    break;

    case TTGUI_CURSOR_TARGET:
      t = (y2 - y1) / 4;
      line( dest, x1, y1, x1, y1+t, color );
      line( dest, x1, y2, x1, y2-t, color );
      line( dest, x2, y1, x2, y1+t, color );
      line( dest, x2, y2, x2, y2-t, color );

      t = (x2 - x1) / 4;
      line( dest, x1+1, y1, x1+t, y1, color );
      line( dest, x1+1, y2, x1+t, y2, color );
      line( dest, x2-1, y1, x2-t, y1, color );
      line( dest, x2-1, y2, x2-t, y2, color );
    break;

    case TTGUI_CURSOR_VERTICAL:
    default:
      line( dest, x1, y1, x1, y2, color );

    break;
  } // end of switch.

  // Set draw mode to solid (would be nice if we could just restore the previous drawing mode):
//  drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);

  // Restore clipping rectangle:
  ttSetClipRect( dest, clipRect );

} // ttDrawCursor

/*--------------------------------------------------------------------------*/

