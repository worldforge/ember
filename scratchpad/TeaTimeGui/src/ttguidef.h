//////////////////////////////////////////////////////////////////////////////
//   File name:  ttguidef.h
//
//   Project:    Gui
//   Author:     Hans H„ggstr”m,  hans.haggstrom@helsinki.fi
//   Purpose:    Bordertype definitions etc.
//
// Notes & things to do:
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//
//
//////////////////////////////////////////////////////////////////////////////

#ifndef TTGUIDEF_H
#define TTGUIDEF_H

// Bordertype constants:
#define TTGUI_BORDER_NONE     0
#define TTGUI_BORDER_FLAT     1
#define TTGUI_BORDER_BLACK    2
#define TTGUI_BORDER_BRIGHT   3
#define TTGUI_BORDER_DARK     4
#define TTGUI_BORDER_UP       5
#define TTGUI_BORDER_PRESSED  6
#define TTGUI_BORDER_DOWN     7
#define TTGUI_BORDER_UPDOWN   8
#define TTGUI_BORDER_DOWNUP   9
#define TTGUI_BORDER_SOFTUP   10
#define TTGUI_BORDER_SOFTDOWN 11

// Areabitmap type constants:
#define TTGUI_AREA_NONE      0  // No bitmap and no filled rectangle, just transparent.
#define TTGUI_AREA_FLAT      1  // Always use rectfill, even if there is a bitmap.
#define TTGUI_AREA_CORNER    2
#define TTGUI_AREA_CENTER    3
#define TTGUI_AREA_STRETCH   4
#define TTGUI_AREA_TILE      5

// Textstyles:
#define TTGUI_TEXT_NORMAL     0
#define TTGUI_TEXT_DROPSHADOW 1
#define TTGUI_TEXT_SHADOW     2
#define TTGUI_TEXT_GRAYED     3
#define TTGUI_TEXT_RAISED     4
#define TTGUI_TEXT_LOWERED    5
#define TTGUI_TEXT_SOFT       6

// Text spacing:
#define TTGUI_TEXT_VERTICAL_SPACING     1
#define TTGUI_TEXT_VERTICAL_MARGINALS   2
#define TTGUI_TEXT_HORIZONTAL_MARGINALS 3

// Border color contrast: (maybe this should be an user changable value..?)
#define TTGUI_BORDER_CONTRAST 2

// Double click time (max time between clicks to count as doubleclick,
// time is given in system ticks CLOCKS_PER_SEC / sec  as got by time.h::clock() ):
#define TTGUI_DBLCLICK_TIME  ((time_t) 26 )
#define TTGUI_DBLCLICK_DIST  2  // Max distance between clicks to count as double.

// Drag start distance (in pixels):
// (how much the mouse have to be moved with button pressed to start a drag.)
#define TTGUI_DRAGSTART_DIST 4

// Mouse constants:
#define TTGUI_MOUSE_NOBUTTON 0
#define TTGUI_MOUSE_LEFT     1
#define TTGUI_MOUSE_RIGHT    2
#define TTGUI_MOUSE_MIDDLE   4

#define TTGUI_MOUSE_DOWN     1
#define TTGUI_MOUSE_UP       2
#define TTGUI_MOUSE_MOVE     4

#define TTGUI_MAX_CHECKMOUSE_LOOP 10 // How many mouse events maximally to
                                     // read from the queue at one time.
// Windows:
#define TTGUI_WIN_MIN_HEIGHT 4
#define TTGUI_WIN_MIN_WIDTH  4

// Text alignment:
#define TTGUI_ALIGN_LEFT     0
#define TTGUI_ALIGN_RIGHT    1
#define TTGUI_ALIGN_CENTER   3
#define TTGUI_ALIGN_JUSTIFY  4

// Keyrepeat:
#define TTGUI_KEYREPEAT_DELAY  1000    // Test these values later, they are just invented..
#define TTGUI_KEYREPEAT_SPEED  20

// Autoscroll:
#define TTGUI_AUTOSCROLL_STARTDELAY   1000
#define TTGUI_AUTOSCROLL_SPEED        8
#define TTGUI_AUTOSCROLL_BORDERWIDTH  16
#define TTGUI_AUTOSCROLL_STAYDELAY    1000   // How long after mouse moved away from scroll area that the autoscroll will start immediately when entering a new scroll area. (phew:)
#define TTGUI_AUTOSCROLL_ACCELERATION 0.01
#define TTGUI_AUTOSCROLL_FRICTION     0.1

// Cursor:
#define TTGUI_CURSOR_STAYCOLOR_TIME   150
#define TTGUI_CURSOR_CHANGECOLOR_TIME 450
#define TTGUI_CURSOR_VERTICAL         0   //  |
#define TTGUI_CURSOR_HORIZONTAL       1   //  _
#define TTGUI_CURSOR_I_BEAM           2   //  I
#define TTGUI_CURSOR_BOX              3   //  []  Hollow box
#define TTGUI_CURSOR_BAR              4   //  ##  Filled box
#define TTGUI_CURSOR_TARGET           5   //  ::  Small corners

// Lists:
#define TTGUI_LIST_DIRECTION_HORIZONTAL 0
#define TTGUI_LIST_DIRECTION_VERTICAL   1


// Prototype for TTApplication (it's needed in ttwindow.h..):
class   TTApplication;

// Prototype for TTWindow (it's needed in ttproper.h..):
class TTWindow;

#endif


