//////////////////////////////////////////////////////////////////////////////
//   File name:  ttwstyle.cc
//
//   Class:      TTWindowStyle and TTApplicationStyle
//
//   Project:    Gui
//   Author:     Hans H„ggstr”m,  hans.haggstrom@helsinki.fi
//   Base class: -
//   Decendants: -
//   Purpose:    Holds info about text/bg colors, font, bg bitmap etc.
//
//
//   Work diary:
// Date      Time     Lines   Things done
// ~~~~~~~~~ ~~~~~~~~ ~~~~~~~ ~~~~~~~~~~~~~~~~~~~~~~
// 16.5.98   0:20     (91)    Header
// 16.5      1:00     166     Implementation
// 15.8      2:00     (152)   Rewrote the header for new, cleaner and better system.
// 15-16.8   2:30     271     Implemented the classes in the new way.
// 17.8      ?                Found out the new way had some impracticalities
//                            and rewrote all the style stuff once again...
// 19.9      0:10     269     Replaced cursorBlinkRate and cursorPulsate with
//                            cursorStayColorTime and cursorChangeColorTime.
// 18.1.99   0:20     296     Added bitmap pointers to various mouse cursors.
//
// Notes & things to do:
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  * Might do color remapping table background support sometime.
//  * Inline getMouseCursorPic ??  hmm...  any use of it at all? not much..
//
//////////////////////////////////////////////////////////////////////////////

#include "ttwstyle.h"

/*--------------------------------------------------------------------------*/
// class TTWindowStyle
/*--------------------------------------------------------------------------*/
// Used in TTGuiStyle to hold one style (editfield or button etc).

TTWindowStyle::TTWindowStyle( int     textColor_,
                              int     bgColor_,
                              int     textStyle_,
                              FONT   *textFont_,
                              BITMAP *bgBitmap_,
                              int     borderStyle_,
                              int     borderWidth_,
                              int     areaStyle_,
                              int     areaXOffs_,
                              int     areaYOffs_ ) {
// Creates new style with given values.
// Uses black and light grey as the text and bg colors if they are -1.

  // If text/bg color not given get black and light grey from current
  // palette info if there is one:
  PaletteInfo *palinf = PaletteInfo::getCurrent();
  if (textColor_ == -1) {
    if (palinf) textColor_ = palinf->blackIndex;
    else textColor_ = makecol( 0, 0, 0 );
  }

  if (bgColor_ == -1)
    bgColor_ = makecol( 192, 192, 192 );

  // Assign values to fields:
  textColor    = textColor_;
  bgColor      = bgColor_;
  textStyle    = textStyle_;
  textFont     = textFont_;

  bgBitmap     = bgBitmap_;
  borderStyle  = borderStyle_;
  borderWidth  = borderWidth_;

  areaStyle    = areaStyle_;
  areaXOffs    = areaXOffs_;
  areaYOffs    = areaYOffs_;

} // Constructor

/*--------------------------------------------------------------------------*/
TTWindowStyle::TTWindowStyle( const TTWindowStyle &source ) {
// Copy constructor.
  textColor    = source.textColor;
  textFont     = source.textFont;
  textStyle    = source.textStyle;

  bgColor      = source.bgColor;
  bgBitmap     = source.bgBitmap;
  areaStyle    = source.areaStyle;
  areaXOffs    = source.areaXOffs;
  areaYOffs    = source.areaYOffs;

  borderStyle  = source.borderStyle;
  borderWidth  = source.borderWidth;
} // Copy constructor.

/*--------------------------------------------------------------------------*/
TTWindowStyle::~TTWindowStyle() {
// Destructor
} // Destructor.

/*--------------------------------------------------------------------------*/
// End of class TTWindowStyle
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
// Class TTGuiStyle
/*--------------------------------------------------------------------------*/
// Used to hold style info by every window object.

TTGuiStyle::TTGuiStyle( TTApplicationStyle *globalSettings_,
                        TTWindowStyle      *winStyle_ ) {
// Constructor

  globalSettings = globalSettings_;

  // Copy fields from global settings if they exist:
  TTGuiStyle *globStyle = NULL;
  if (globalSettings) {
    globStyle = &(globalSettings->defaultGuiStyle);

    selectionTextColor         = globStyle->selectionTextColor;
    selectionBgColor           = globStyle->selectionBgColor;

    buttonTextColorChange      = globStyle->buttonTextColorChange;
    buttonMouseOverTextColor   = globStyle->buttonMouseOverTextColor;
    buttonPressedTextColor     = globStyle->buttonPressedTextColor;
    buttonTextColorSpeed       = globStyle->buttonTextColorSpeed;

    mouseCursorPic             = NULL; // NULL means we look at global settings anyway.

    cursorOnColor              = globStyle->cursorOnColor;
    cursorOffColor             = globStyle->cursorOffColor;
  }
  else {  // No global settings found
    // Hardwired defaults:
    selectionTextColor         = makecol( 255, 255, 255 );
    selectionBgColor           = makecol(   0, 200, 180 );

    buttonTextColorChange      = true;
    buttonMouseOverTextColor   = makecol( 160, 128,   0 );
    buttonPressedTextColor     = makecol( 220, 180,   0 );
    buttonTextColorSpeed       = 5;

    mouseCursorPic             = NULL;

    cursorOnColor              = makecol(   0,   0,   0 );
    cursorOffColor             = makecol( 192, 192, 192 );
  } // no global settings

  // Copy winStyle from parameter if given:
  if (winStyle_)  winStyle = *winStyle_;

} // Constructor.

/*--------------------------------------------------------------------------*/
TTGuiStyle::TTGuiStyle( const TTGuiStyle &source ) {
// Copy constructor.

    globalSettings             = source.globalSettings;

    winStyle                   = source.winStyle;

    selectionTextColor         = source.selectionTextColor;
    selectionBgColor           = source.selectionBgColor;

    buttonTextColorChange      = source.buttonTextColorChange;
    buttonMouseOverTextColor   = source.buttonMouseOverTextColor;
    buttonPressedTextColor     = source.buttonPressedTextColor;
    buttonTextColorSpeed       = source.buttonTextColorSpeed;

    mouseCursorPic             = source.mouseCursorPic;

    cursorOnColor              = source.cursorOnColor;
    cursorOffColor             = source.cursorOffColor;

} // Copy constructor.

/*--------------------------------------------------------------------------*/
TTGuiStyle::~TTGuiStyle() {
// Destructor.
} // Destructor.

//----------------------------------------------------------------------------
/**** Not used and slow anyway. ****

BITMAP *TTGuiStyle::getMouseCursorPic() {
// Returns the local mouse cursor pic if it is != NULL, else the
// application mouse cursor, NULL if the system default should be used.

  if (mouseCursorPic) return mouseCursorPic;
  else if (globalSettings) return globalSettings->mouseCursorPic;
  else return NULL; // Use allegro default.
  
} // getMouseCursorPic
*/

/*--------------------------------------------------------------------------*/
// End of class TTGuiStyle.
/*--------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------*/
// Class TTApplicationStyle
/*--------------------------------------------------------------------------*/
// Used to hold style info for applications.

TTApplicationStyle::TTApplicationStyle()
      : backgroundStyle(),
        buttonStyle(),
        editStyle(),
        menuStyle(),
        titlebarStyle(),
        helpStyle(),
        defaultGuiStyle(this) {
// Default constructor

  // Hardwired defaults:
  dblClickTime             = TTGUI_DBLCLICK_TIME;

  keyRepeatDelay           = TTGUI_KEYREPEAT_DELAY;
  keyRepeatSpeed           = TTGUI_KEYREPEAT_SPEED;

  autoScrollStartDelay     = TTGUI_AUTOSCROLL_STARTDELAY;
  autoScrollSpeed          = TTGUI_AUTOSCROLL_SPEED;

  cursorStayColorTime      = TTGUI_CURSOR_STAYCOLOR_TIME;
  cursorChangeColorTime    = TTGUI_CURSOR_CHANGECOLOR_TIME;
  cursorStyle              = TTGUI_CURSOR_VERTICAL;

  mouseCursorPic           = NULL;
  mouseMovePic             = NULL;
  mouseResizeHorizontalPic = NULL;
  mouseResizeVerticalPic   = NULL;
  mouseResizeRightPic      = NULL;
  mouseResizeLeftPic       = NULL;
  mouseWaitPic             = NULL;
//  mouseBusyPic             = NULL;
  mouseDragPic             = NULL;

  focusOutlineStartColor   = makecol( 170, 150, 30 );
  focusOutlineMidColor     = makecol( 220, 210, 50 );
  focusOutlineFinalColor   = makecol( 200, 190, 30 );
  focusOutlineOutColor     = makecol( 170, 150, 30 );
  focusOutlineSpeed        = 1000;

  linewidth                = 1;
} // Default constructor.

/*--------------------------------------------------------------------------*/
TTApplicationStyle::TTApplicationStyle( const TTApplicationStyle &source )
      : backgroundStyle(source.backgroundStyle),
        buttonStyle(source.buttonStyle),
        editStyle(source.editStyle),
        menuStyle(source.menuStyle),
        titlebarStyle(source.titlebarStyle),
        helpStyle(source.helpStyle),
        defaultGuiStyle(source.defaultGuiStyle) {
// Copy constructor

/*
  backgroundStyle          = source.backgroundStyle;
  buttonStyle              = source.buttonStyle;
  editStyle                = source.editStyle;
  menuStyle                = source.menuStyle;
  titlebarStyle            = source.titlebarStyle;
  helpStyle                = source.helpStyle;
  */
  dblClickTime             = source.dblClickTime;

  keyRepeatDelay           = source.keyRepeatDelay;
  keyRepeatSpeed           = source.keyRepeatSpeed;

  autoScrollStartDelay     = source.autoScrollStartDelay;
  autoScrollSpeed          = source.autoScrollSpeed;

  cursorStayColorTime      = source.cursorStayColorTime;
  cursorChangeColorTime    = source.cursorChangeColorTime;
  cursorStyle              = source.cursorStyle;

  mouseCursorPic           = source.mouseCursorPic;
  mouseMovePic             = source.mouseMovePic;
  mouseResizeHorizontalPic = source.mouseResizeHorizontalPic;
  mouseResizeVerticalPic   = source.mouseResizeVerticalPic;
  mouseResizeRightPic      = source.mouseResizeRightPic;
  mouseResizeLeftPic       = source.mouseResizeLeftPic;
  mouseWaitPic             = source.mouseWaitPic;
//  mouseBusyPic             = source.mouseBusyPic;
  mouseDragPic             = source.mouseDragPic;

  focusOutlineStartColor   = source.focusOutlineStartColor;
  focusOutlineMidColor     = source.focusOutlineMidColor;
  focusOutlineFinalColor   = source.focusOutlineFinalColor;
  focusOutlineOutColor     = source.focusOutlineOutColor;
  focusOutlineSpeed        = source.focusOutlineSpeed;

  linewidth                = source.linewidth;
} // Copy constructor.

/*--------------------------------------------------------------------------*/
TTApplicationStyle::~TTApplicationStyle() {
// Destructor
} // Destructor.

/*--------------------------------------------------------------------------*/
// End of class TTApplicationStyle.
/*--------------------------------------------------------------------------*/

