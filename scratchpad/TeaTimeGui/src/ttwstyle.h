//////////////////////////////////////////////////////////////////////////////
//   File name:  ttwstyle.h
//
//   Class:      TTWindowStyle and TTApplicationStyle
//
//   Project:    Gui
//   Author:     Hans H„ggstr”m,  hans.haggstrom@helsinki.fi
//   Base class: -
//   Decendants: -
//   Purpose:    Holds info about text/bg colors, font, bg bitmap etc.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef TTWSTYLE_H
#define TTWSTYLE_H

#include <allegro.h>
#include <time.h>
#include "m_grafx.h"
#include "ttguidef.h"

// Prototype:
class TTApplicationStyle;

/*--------------------------------------------------------------------------*/
class TTWindowStyle {
// Used in TTGuiStyle to hold one style (editfield or button etc).
  public:

    TTWindowStyle( int     textColor_   = -1,
                   int     bgColor_     = -1,
                   int     textStyle_   = TTGUI_TEXT_NORMAL,
                   FONT   *textFont_    = NULL,
                   BITMAP *bgBitmap_    = NULL,
                   int     borderStyle_ = TTGUI_BORDER_NONE,
                   int     borderWidth_ = 1,
                   int     areaStyle_   = TTGUI_AREA_CORNER,
                   int     areaXOffs_   = 0,
                   int     areaYOffs_   = 0 );
    // Creates new style with given values.
    // Uses black and light grey as the text and bg colors if they are -1.

    TTWindowStyle( const TTWindowStyle &source );
    // Copy constructor.

    ~TTWindowStyle();
    // Destructor.


    /*----[ Fields: ]-----------------------------------------------------*/

    int     textColor;   // Color of text (allegro color format).
    FONT   *textFont;    // If textFont is NULL the default font ('font') is used.
    int     textStyle;   // Shadow / disabled / normal / etc.

    int     bgColor;     // Background color.
    BITMAP *bgBitmap;    // Possible background bitmap.
    int     areaStyle;   // Tiled/centered/stretched etc. background bitmap.
    int     areaXOffs;   // Offset in tiled or corner mode from left edge.
    int     areaYOffs;   // Offset in tiled or corner mode from upper edge.

    int     borderStyle; // TTGUI_BORDER_UP /_DOWN etc..
    int     borderWidth; // Width of border (will be multiplied with application lineWidth before drawing)

}; // class TTWindowStyle.
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
class TTGuiStyle {
// Used to hold style info by every window object.
  public:
    TTGuiStyle( TTApplicationStyle *globalSettings_,
                TTWindowStyle      *winStyle_ = NULL );
    // Constructor.

    TTGuiStyle( const TTGuiStyle &source );
    // Copy constructor.

    ~TTGuiStyle();
    // Destructor.
    
/**** Not used and slow anyway.
    BITMAP *getMouseCursorPic();
    // Returns the local mouse cursor pic if it is != NULL, else the
    // application mouse cursor, NULL if the system default should be used.
*/

    /*----[ Fields: ]-----------------------------------------------------*/

    TTWindowStyle winStyle;        // Textcolor & font, background color or
                                   // bitmap, borderstyle & width, etc.

    int  selectionTextColor;       // Color of selected text & other things.
    int  selectionBgColor;         // Background color of selected text, icons and other things.

    bool buttonTextColorChange;    // If true the button caption colors change when mouse is over the button and when button is pressed.
    int  buttonMouseOverTextColor; // Color of button caption when mouse is on the button.
    int  buttonPressedTextColor;   // Color of button caption when the button is pressed.
    int  buttonTextColorSpeed;     // Speed with which the caption color changes.

    int  cursorOnColor;            // Color of cursor when it's 'on'.
    int  cursorOffColor;           // Color of cursor when it's 'off' (typically same as bg color).

    BITMAP *mouseCursorPic;        // Mouse cursor when over the object. If NULL then applicationStyle defaults are used.

    TTApplicationStyle *globalSettings; // Settings that aren't window-object specific.
                                        // Normally points to TTApplication::globalSettings.
}; // class TTGuiStyle.
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
class TTApplicationStyle {
// Used to hold style info that isn't window-object specific.
  public:
    TTApplicationStyle();
    // Default constructor.

    TTApplicationStyle( const TTApplicationStyle &source );
    // Copy constructor.

    ~TTApplicationStyle();
    // Destructor.


    /*----[ Fields: ]-----------------------------------------------------*/

    TTWindowStyle backgroundStyle; // General form & menu background.  The area that is grey in Win95.
    TTWindowStyle buttonStyle;     // Buttons and other pressable gizmos.
    TTWindowStyle editStyle;       // Edit fields, checkboxes, etc.
    TTWindowStyle menuStyle;       // Menu, menubar and popupmenu style.
    TTWindowStyle titlebarStyle;   // Title/caption bar style.
    TTWindowStyle helpStyle;       // Tooltips, context sensitive help.

    TTGuiStyle defaultGuiStyle;  // Default gui style.  This is used as source
                                 // for the guiStyle of all window-objects in
                                 // the application.  Its winStyle field is
                                 // initialized to backgroundStyle.

    int  dblClickTime;           // Max time between clicks to count as doubleclick.

    int  keyRepeatDelay;         // Pause in ms before a key starts repeating when it's pressed.
    int  keyRepeatSpeed;         // How many ms between each keypress event generated when key is pressed.

    int  autoScrollStartDelay;   // The pause in ms before automatic window scrolling starts when mouse is at the border.
    int  autoScrollSpeed;        // Speedfactor, speed also depends on how close to the border the mouse is.

    int  cursorStayColorTime;    // How many milliseconds the cursor color should stay the same between colorchanges.
    int  cursorChangeColorTime;  // In how many milliseconds the cursor should change from on to off color or back.
    int  cursorStyle;            // vertical, horizontal, I-bar, block?, etc.  a TTGUI_CURSOR_*** -constant.

    BITMAP *mouseCursorPic;      // Default mouse cursor. If NULL, the system default is used.
    BITMAP *mouseMovePic;             // + Move in all directions -cursor.     If NULL, default or system default is used.
    BITMAP *mouseResizeHorizontalPic; // - Horizontal resizeing mouse cursor.    "
    BITMAP *mouseResizeVerticalPic;   // | Vertical resizeing mouse cursor.     -"-
    BITMAP *mouseResizeRightPic;      // \ Resize upleft - downright.
    BITMAP *mouseResizeLeftPic;       // / Resize upright - downleft.
    BITMAP *mouseWaitPic;             // Z Ye olde hourglass mouse cursor.
//    BITMAP *mouseBusyPic;             // z Computer working but mouse usable.  Hmm.. is this needed?  Nah. It complicates things by mixing in cpu sleepiness with mouse cursor managing.
    BITMAP *mouseDragPic;             // @ Dragging cursor (hand etc).

    int  focusOutlineStartColor; // Color of focus rectangle around a gizmo when it gets the focus.
    int  focusOutlineMidColor;   // -"- in the middle of the flash-color-cycle.
    int  focusOutlineFinalColor; // -"- end -"-.  This color stays as the focus outline.
    int  focusOutlineOutColor;   // The color to which the outline changes before turning to bg color when focus is lost.
    int  focusOutlineSpeed;      // Speed of flash-color-cycle in ms.

    int  linewidth;              // For borders etc.  Usefull when using many different video modes.  Normally 1.

}; // class TTApplicationStyle.
/*--------------------------------------------------------------------------*/

#endif

