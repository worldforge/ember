//////////////////////////////////////////////////////////////////////////////
//   File name:  ttapplic.h
//
//   Class:      TTApplication
//
//   Project:    Gui
//   Author:     Hans H„ggstr”m,  hans.haggstrom@helsinki.fi
//   Base class: -
//   Decendants: User derived applications.
//   Purpose:    Encapsulate general application level aspects of the gui,
//               eg. mouse and keyboard events and provide a top level
//               interface for drawing and execution.
//
//
//   Work diary:
// Date      Time     Lines   Things done
// ~~~~~~~~~ ~~~~~~~~ ~~~~~~~ ~~~~~~~~~~~~~~~~~~~~~~
// 17.5.98   1:00?   (163)    Header
// 17.5      0:30    (170)    Misc stuff..
// 19.5
//
// Notes & things to do:
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//
//
//////////////////////////////////////////////////////////////////////////////

#ifndef TTAPPLIC_H
#define TTAPPLIC_H

#include <allegro.h>
#include "ttguidef.h"
#include "ttwstyle.h"
#include "ttwindow.h"
#include "ttmousev.h"  // Mouse events & queue.
#include "tttime.h"    // Simple millisecond counter interrupt.

/*--------------------------------------------------------------------------*/
class TTApplication {
  public:
    /*----[ Constructors & destructor: ]------------------------------------*/
    TTApplication();
    // Default constructor

    virtual ~TTApplication();
    // Destructor.  Calls shutDown(), it won't work correctly if the
    // application is running, however, so make sure to stop it first.

    /*----------------------------------------------------------------------*/
    /*----[ Running: ]------------------------------------------------------*/

    virtual void setup();
    // Initializes main window and its child windows and generally puts
    // things together before running the application.  Called only once,
    // before the program is ran the first time (if the user doesn't call it
    // many times directly, of course).
    // Doesn't do very much here, override in derived applications.
    // (But be sure to call this version first, before executing your own setup)

    virtual void start( BITMAP *dest );
    // Runs the application.  Calls setup if it hasn't been called before.
    // Starts a loop that checks for mouse and keyboard input,  calls
    // doTurn of the child windows, and draws the application to dest.

    virtual void stop();
    // Stops the application.  The application may be restarted again
    // by calling start, so don't destroy anything vital that was allocated
    // in setup().

    virtual void shutDown();
    // Frees windows and resources allocated in setup().

    /*----------------------------------------------------------------------*/
    /*----[ Drawing and other: ]--------------------------------------------*/

    virtual void draw( BITMAP *dest );
    // Draws the main window to the specified bitmap.

    virtual bool update( BITMAP *dest );
    // Draws all the parts that have changed since the last call to draw.
    // Returns true if anything was drawn.

    virtual void doTurn();
    // Calls the doTurn method of the main window. (And the main window probably
    // calls doTurn of all its child -windows.)
    // Can be used to perform general computation and stuff.
    // However, it shouldn't lock up or work very slowly, that would slow
    // down the response of the UI too much.  It's probably called quite
    // regularily, think of it as an idle loop callback function.

    virtual void outlineAnimation( int sx1, int sy1, int sx2, int sy2,
                                   int dx1, int dy1, int dx2, int dy2,
                                   TTWindow *winToShow = NULL,
                                   fix speed = 5 );
    // Starts an animation of a hollow box (drawn with xor) moving
    // from (sx1, sy1, sx2, sy2) to (dx1, dy1, dx2, dy2) with an average
    // speed of (speed * style.linewidth) pixels per frame.
    // When it's done the visible property of winToShow is set to true
    // (if winToShow != NULL).


    /*----------------------------------------------------------------------*/
    /*----[ Keyboard events: ]----------------------------------------------*/

    virtual void checkKeyboard();
    // Checks the keyboard input and redirects keyboard events to the main
    // window or the window that has captured the keyboard.

    virtual bool captureKeyboard( TTWindow *win );
    // Captures keyboard input and directs it to the specified window until
    // releaseKeyboard is called.  Returns true on success, false if keyboard
    // already captured.

    virtual void releaseKeyboard( TTWindow *win );
    // Releases the keyboard if it is captured by win,
    // keyboard input again goes to the window that has the focus.


    /*----------------------------------------------------------------------*/
    /*----[ Mouse events: ]-------------------------------------------------*/

    virtual void checkMouse();
    // Checks the mouse input queue and redirects mouse events to the main
    // window or the window that has captured the mouse.

    virtual bool captureMouse( TTWindow *win );
    // Captures mouse input and directs it to the specified window until
    // releaseMouse is called.  Returns true on success, false if mouse
    // already captured.

    virtual void releaseMouse( TTWindow *win );
    // Releases the mouse if it was captured by win,
    // mouse input again goes to the window the mouse currently is in.

    virtual void updatePointedWindow( int mx, int my );
    // Checks which window the mouse pointer currently is in and
    // sets pointedWindow to point at it.  Calls mouseExit for the previous
    // pointedWindow and mouseEnter for the new one.

    /*----------------------------------------------------------------------*/
    /*----[ Style: ]--------------------------------------------------------*/
    virtual void setDefaultStyle();
    // Sets the hardwired default style for gui colors, cursorblinkrate, etc.

    /*----------------------------------------------------------------------*/
    /*----[ Accessors: ]----------------------------------------------------*/
    virtual bool                isRunning()     { return running;    }
    virtual TTWindow           *getMainWindow() { return mainWindow; }
    virtual TTApplicationStyle *getStyle()      { return style;      }
    virtual TTWindow           *getMouseCaptureWindow()    { return mouseCaptureWin; }
    virtual TTWindow           *getKeyboardCaptureWindow() { return keyboardCaptureWin; }
    virtual TTWindow           *getPointedWindow()         { return pointedWindow;      }
    virtual TTWindow           *getFocusedWindow()         { return focusedWindow;      }
    virtual void                setFocusedWindow(TTWindow *win) { focusedWindow = win;  }
    virtual void                setMainWindow   (TTWindow *win) { mainWindow    = win;  }

  /*------------------------------------------------------------------------*/
  protected:
    TTWindow *mainWindow;       // Should contain all other windows in the application.
    TTWindow *mouseCaptureWin;  // NULL if mouse not captured, else all mouse
                                // events are sent to the specified window.
    TTWindow *keyboardCaptureWin;  // NULL if keyboard not captured, else all
                         // keyboard events are sent to the specified window.
                         // (Usefull for example when configuring input keys
                         //  in games, pressing TAB won't shift away from the
                         //  input gizmo.)
    TTWindow *pointedWindow;   // The window the mouse pointer currently is in.
    TTWindow *focusedWindow;   // The currently focused window.

    TTApplicationStyle *style;  // Global settings for gui color, cursor blinkrate and all such stuff.

    bool setupCalled;          // True if setup has been called and the main window is setup ok.
    bool running;              // True if application is running (== in the start() function).
    bool stopApplication;      // Raise flag to stop application if it's running.
    bool shutDownApplication;  // Raise flag to shut down application if it's running.

    // List of outline animations in progress:

    // List of keyboard accelerators and functions to call:

    //TTClipboard[Slot?] *clipboard

}; // class TTApplication.

/*--------------------------------------------------------------------------*/
#endif


