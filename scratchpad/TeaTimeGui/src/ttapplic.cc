//////////////////////////////////////////////////////////////////////////////
//   File name:  ttapplic.cc
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
// 19.5      1:20     277     Basic implementation.
// 19.5      1:30     321     All kind of include problems with mouse/keyb
//                            capturing, but now it ought to be figured out.
// 17.8      1:00     454     Implemented the new style system and made hardwired defaults...
// 17.8      0:30     455     Fixed annoying linewidth==0 bug that caused the borders to dissapear.  Looks beautiful =)
// 18.1.99   0:30     512     Added support for custom mouse cursors in
//                            different window objects.
// 19.1      1:30     511     Drew some mouse pointers and tested.  Works =-)
//
// Notes & things to do:
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  * Load default mouse cursors from a default datafile?
//
//
//
//////////////////////////////////////////////////////////////////////////////

#include "ttapplic.h"


/*--------------------------------------------------------------------------*/
// class TTApplication
/*--------------------------------------------------------------------------*/

/*----[ Constructors & destructor: ]----------------------------------------*/
TTApplication::TTApplication() {
// Default constructor

  mainWindow          = NULL;
  mouseCaptureWin     = NULL;
  keyboardCaptureWin  = NULL;
  pointedWindow       = NULL;
  focusedWindow       = NULL;
  style               = new TTApplicationStyle();
  setupCalled         = false;
  running             = false;
  stopApplication     = false;
  shutDownApplication = false;

  // Set hardwired colors for different gui gizmos
  // (editfields, buttons, general backgr, etc..):
  setDefaultStyle();
} // Constructor

/*--------------------------------------------------------------------------*/
TTApplication::~TTApplication() {
// Destructor.  Calls shutDown(), it won't work correctly if the
// application is running, however, so make sure to stop it first.

  // Delete windows??  Hmm.. why not:
  if (mainWindow) delete mainWindow;
  if (style)      delete style;

} // Destructor

/*--------------------------------------------------------------------------*/
/*----[ Running: ]----------------------------------------------------------*/
/*--------------------------------------------------------------------------*/

void TTApplication::setup() {
// Initializes main window and its child windows and generally puts
// things together before running the application.  Called only once,
// before the program is run the first time (if the user doesn't call it
// many times directly, of course).
// Doesn't do very much here, override in derived applications.
// (But be sure to call this version first, before executing your own setup)
  if (setupCalled) return; // nothing to do.. ?

  // Install mouse event handler if not already installed:
  ttInstallMouseEvents();

  // Install millisecond timer:
  ttInstallMsTimer();

  setupCalled = true;
} // setup

/*--------------------------------------------------------------------------*/
void TTApplication::start( BITMAP *dest ) {
// Runs the application.  Calls setup if it hasn't been called before.
// Starts a loop that checks for mouse and keyboard input,  calls
// doTurn of the child windows, and draws the application to dest.

  bool redrawScreen = false;

  // Don't start if already running:
  if (running) return;

  // Call setup if neccesary:
  if (!setupCalled) setup();

  // Don't start if setup failed:
  if (!setupCalled) return;

  // Remove mouse from possible previous bitmap:
  show_mouse(NULL);

  // Draw application:
  draw(dest);

  // Show mouse on dest bitmap:
  show_mouse(dest);

  // Main loop:
  while (!stopApplication && !shutDownApplication) {
    // Check for input and do general program code:
    doTurn();

    // Draw things that have changed:
    show_mouse(NULL);  // Hide mouse
    // Hide outline animations.
    redrawScreen = update(dest);
    // Show outline animations at new positions.
    show_mouse(dest);  // Show mouse

    // Blit to screen if neccesary:
    if (dest != screen && redrawScreen && dest != NULL) {
      vsync();                                           // Wait for vsync
      blit(dest, screen, 0, 0, 0, 0, dest->w, dest->h);  // Copy dest to screen
    }

  } // end of main loop

  // Update flags:
  running         = false;
  stopApplication = false;

  // Check for application shutdown:
  if (shutDownApplication) {
    shutDownApplication = false;
    shutDown();
  }

} // start

/*--------------------------------------------------------------------------*/
void TTApplication::stop() {
// Stops the application.  The application may be restarted again
// by calling start, so don't destroy anything vital that was allocated
// in setup().

  if (running) stopApplication = true;
} // stop

/*--------------------------------------------------------------------------*/
void TTApplication::shutDown() {
// Frees windows and resources allocated in setup().

  // Can't shut down while running:
  if (running) return;

  // Remove millisecond timer:
  ttRemoveMsTimer();

  // Remove mouse event handler if it is installed:
  ttRemoveMouseEvents();

  setupCalled = false;
} // shutDown


/*--------------------------------------------------------------------------*/
/*----[ Drawing and other: ]------------------------------------------------*/
/*--------------------------------------------------------------------------*/

void TTApplication::draw( BITMAP *dest ) {
// Draws the main window to the specified bitmap.

  if (!dest) return;
  if (mainWindow) mainWindow->draw(dest);
} // draw

/*--------------------------------------------------------------------------*/
bool TTApplication::update( BITMAP *dest ) {
// Draws all the parts that have changed since the last call to draw.
// Returns true if anything was drawn.

  if (!dest || !mainWindow) return false;
  return mainWindow->update(dest);
} // update

/*--------------------------------------------------------------------------*/
void TTApplication::doTurn() {
// Calls the doTurn method of the main window. (And the main window probably
// calls doTurn of all its child -windows.)
// Can be used to perform general computation and stuff.
// However, it shouldn't lock up or work very slowly, that would slow
// down the response of the UI too much.  It's probably called quite
// regularily, think of it as an idle loop callback function.

  // Process input:
  checkMouse();
  checkKeyboard();

  // Call doTurn of all window objects in application:
  if (mainWindow) mainWindow->doTurn();

} // doTurn

/*--------------------------------------------------------------------------*/
void TTApplication::outlineAnimation( int sx1, int sy1, int sx2, int sy2,
                                      int dx1, int dy1, int dx2, int dy2,
                                      TTWindow *winToShow = NULL,
                                      fix speed = 5 ) {
// Starts an animation of a hollow box (drawn with xor) moving
// from (sx1, sy1, sx2, sy2) to (dx1, dy1, dx2, dy2) with an average
// speed of 'speed' pixels per frame.
// When it's done the visible property of winToShow is set to true
// (if winToShow != NULL).

  // Create new outline animation.
} // outlineAnimation


/*--------------------------------------------------------------------------*/
/*----[ Keyboard events: ]--------------------------------------------------*/
/*--------------------------------------------------------------------------*/

void TTApplication::checkKeyboard() {
// Checks the keyboard input and redirects keyboard events to the main
// window or the window that has captured the keyboard.
  if (keypressed()) {
    int k = readkey();
    if (keyboardCaptureWin) keyboardCaptureWin->keyPress(k);
    else if (mainWindow) mainWindow->keyPress(k);  // or keyPressed(k) ?
  }
} // checkKeyboard

/*--------------------------------------------------------------------------*/
bool TTApplication::captureKeyboard( TTWindow *win ) {
// Captures keyboard input and directs it to the specified window until
// releaseKeyboard is called.  Returns true on success, false if keyboard
// already captured.
  if (keyboardCaptureWin == win) return true;  // Already has it.
  if (keyboardCaptureWin)        return false; // Somebody else has it.

  keyboardCaptureWin = win;
  return true;
} // captureKeyboard

/*--------------------------------------------------------------------------*/
void TTApplication::releaseKeyboard( TTWindow *win ) {
// Releases the keyboard if it is captured by win,
// keyboard input again goes to the window that has the focus.

  if (keyboardCaptureWin == win)
    keyboardCaptureWin = NULL;
} // releaseKeyboard


/*--------------------------------------------------------------------------*/
/*----[ Mouse events: ]-----------------------------------------------------*/
/*--------------------------------------------------------------------------*/

void TTApplication::checkMouse() {
// Checks the mouse input queue and redirects mouse events to the main
// window or the window that has captured the mouse.

  int x, y, button, event, loop = 0;
  while ( loop < TTGUI_MAX_CHECKMOUSE_LOOP &&
          ttGetMouseEvent( x, y, button, event ) ) {
    loop++;

    // Select mouse event destination:
    TTWindow *eventDest  = NULL;

    // Get the window the mouse is over:
    updatePointedWindow( x, y );

    if (mouseCaptureWin) eventDest = mouseCaptureWin;
    else                 eventDest = pointedWindow;

    if (!eventDest) continue; // Read away the rest of the mouse events. (?)


    // Determine event type and redirect it:
    switch (event) {
      case TTGUI_MOUSE_UP:
        eventDest->mouseUp( x, y, button, pointedWindow );
      break;

      case TTGUI_MOUSE_DOWN:
        eventDest->mouseDown( x, y, button, pointedWindow );
      break;

      case TTGUI_MOUSE_MOVE:
        eventDest->mouseMove( x, y, pointedWindow );
      break;

      default:
      break;
    } // end switch.

  } // End while

} // checkMouse

/*--------------------------------------------------------------------------*/
bool TTApplication::captureMouse( TTWindow *win ) {
// Captures mouse input and directs it to the specified window until
// releaseMouse is called.  Returns true on success, false if mouse
// already captured.

  if (mouseCaptureWin == win) return true;  // Already has it.
  if (mouseCaptureWin)        return false; // Somebody else has it.

  mouseCaptureWin = win;

  // Set mouse cursor to the default specified for the capturing window,
  // or the default cursor if the window doesn't have any.
  if (mouseCaptureWin) {
    if (mouseCaptureWin->getGuiStyle())
      set_mouse_sprite( mouseCaptureWin->getGuiStyle()->mouseCursorPic );
    else if (style)
      set_mouse_sprite( style->mouseCursorPic );
    else set_mouse_sprite( NULL );
  }
  
  return true;
} // captureMouse

/*--------------------------------------------------------------------------*/
void TTApplication::releaseMouse( TTWindow *win ) {
// Releases the mouse if it was captured by win,
// mouse input again goes to the window the mouse currently is in.

  if (mouseCaptureWin == win) {
    mouseCaptureWin = NULL;
    
    // Set mouse cursor to the default specified for the pointed window,
    // or the default cursor if the window doesn't have any.
    if (pointedWindow && pointedWindow->getGuiStyle())
      set_mouse_sprite( pointedWindow->getGuiStyle()->mouseCursorPic );
    else if (style)
      set_mouse_sprite( style->mouseCursorPic );
    else set_mouse_sprite( NULL );
  }
} // releaseMouse

/*--------------------------------------------------------------------------*/
void TTApplication::updatePointedWindow( int mx, int my ) {
// Checks which window the mouse pointer currently is in and
// sets pointedWindow to point at it.  Calls mouseExit for the previous
// pointedWindow and mouseEnter for the new one.

  if (!mainWindow) {
    pointedWindow = NULL;
    return;
  }

  // Check if pointedWindow changed:
  TTWindow *p = mainWindow->getWindowAt( mx, my );
  if (p != pointedWindow) {
  
    // Exit old window object:
    if (pointedWindow) pointedWindow->mouseExit( mx, my, p );
    
    // Set default (desktop(?)) cursor if the mouse isn't captured
    // and no new window is pointed at:
    if (!mouseCaptureWin && !p) {
      if (style) set_mouse_sprite( style->mouseCursorPic );
      else set_mouse_sprite( NULL );
    }
      
    if (p) {
      // Enter new windows object:
      p->mouseEnter( mx, my, p );
      
      // Set mouse cursor to that of the new pointed window
      // (if it has one and the mouse isn't captured):
      if (!mouseCaptureWin && p->getGuiStyle())
        set_mouse_sprite( p->getGuiStyle()->mouseCursorPic );
    }

    pointedWindow = p;
  }

} // updatePointedWindow

/*--------------------------------------------------------------------------*/
/*----[ Style: ]------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
void TTApplication::setDefaultStyle() {
// Sets the hardwired default style for gui colors, cursorblinkrate, etc.
  if (!style) {
    style = new TTApplicationStyle();
    if (!style) return;
  }

  // Hardwired defaults (mostly same as in ttwstyle.cc):
  style->dblClickTime             = TTGUI_DBLCLICK_TIME;
  style->keyRepeatDelay           = TTGUI_KEYREPEAT_DELAY;
  style->keyRepeatSpeed           = TTGUI_KEYREPEAT_SPEED;
  style->autoScrollStartDelay     = TTGUI_AUTOSCROLL_STARTDELAY;
  style->autoScrollSpeed          = TTGUI_AUTOSCROLL_SPEED;
  style->cursorStayColorTime      = TTGUI_CURSOR_STAYCOLOR_TIME;
  style->cursorChangeColorTime    = TTGUI_CURSOR_CHANGECOLOR_TIME;
  style->cursorStyle              = TTGUI_CURSOR_I_BEAM;
  style->focusOutlineStartColor   = makecol( 192, 192, 192 );//makecol( 170, 150, 50 );
  style->focusOutlineMidColor     = makecol( 128, 220, 255 );//makecol( 255, 220, 130 );
  style->focusOutlineFinalColor   = makecol(   0, 180, 200 );//makecol( 200, 190, 80 );
  style->focusOutlineOutColor     = makecol( 192, 192, 192 );//makecol( 170, 150, 50 );
  style->focusOutlineSpeed        = 350;
  style->linewidth                = SCREEN_W / 1000 + 1;  // Default 1, 2 if resolution 1024*7xx etc..


  style->defaultGuiStyle.selectionTextColor       = makecol( 255, 255, 255 );
  style->defaultGuiStyle.selectionBgColor         = makecol(   0, 180, 200 );
  style->defaultGuiStyle.buttonTextColorChange    = true;
  style->defaultGuiStyle.buttonMouseOverTextColor = makecol(   0, 180, 200 );
  style->defaultGuiStyle.buttonPressedTextColor   = makecol( 128, 220, 255 );
  style->defaultGuiStyle.buttonTextColorSpeed     = 150;
  style->defaultGuiStyle.cursorOnColor            = makecol( 128, 220, 255 );
  style->defaultGuiStyle.cursorOffColor           = makecol(   0,  90, 100 );
  style->defaultGuiStyle.globalSettings           = style;


  style->backgroundStyle.textColor   = makecol(0, 0, 0);
  style->backgroundStyle.textStyle   = TTGUI_TEXT_NORMAL;
  style->backgroundStyle.textFont    = font;
  style->backgroundStyle.bgColor     = makecol(160,160,160);
  style->backgroundStyle.bgBitmap    = NULL;
  style->backgroundStyle.borderStyle = TTGUI_BORDER_UP;
  style->backgroundStyle.borderWidth = 2;
  style->backgroundStyle.areaStyle   = TTGUI_AREA_TILE;
  style->backgroundStyle.areaXOffs   = 0;
  style->backgroundStyle.areaYOffs   = 0;

  style->buttonStyle.textColor       = makecol(0, 0, 0);
  style->buttonStyle.textStyle       = TTGUI_TEXT_SHADOW;
  style->buttonStyle.textFont        = font;
  style->buttonStyle.bgColor         = makecol(192,192,192);
  style->buttonStyle.bgBitmap        = NULL;
  style->buttonStyle.borderStyle     = TTGUI_BORDER_UP;
  style->buttonStyle.borderWidth     = 2;
  style->buttonStyle.areaStyle       = TTGUI_AREA_TILE;
  style->buttonStyle.areaXOffs       = 0;
  style->buttonStyle.areaYOffs       = 0;

  style->editStyle.textColor         = makecol(0, 0, 0);
  style->editStyle.textStyle         = TTGUI_TEXT_SHADOW;
  style->editStyle.textFont          = font;
  style->editStyle.bgColor           = makecol(255,255,255);
  style->editStyle.bgBitmap          = NULL;
  style->editStyle.borderStyle       = TTGUI_BORDER_SOFTDOWN;
  style->editStyle.borderWidth       = 2;
  style->editStyle.areaStyle         = TTGUI_AREA_TILE;
  style->editStyle.areaXOffs         = 0;
  style->editStyle.areaYOffs         = 0;

  style->menuStyle.textColor         = makecol(0, 0, 0);
  style->menuStyle.textStyle         = TTGUI_TEXT_NORMAL;
  style->menuStyle.textFont          = font;
  style->menuStyle.bgColor           = makecol(192,192,192);
  style->menuStyle.bgBitmap          = NULL;
  style->menuStyle.borderStyle       = TTGUI_BORDER_DOWN;
  style->menuStyle.borderWidth       = 2;
  style->menuStyle.areaStyle         = TTGUI_AREA_TILE;
  style->menuStyle.areaXOffs         = 0;
  style->menuStyle.areaYOffs         = 0;

  style->titlebarStyle.textColor     = makecol(200,180,100);
  style->titlebarStyle.textStyle     = TTGUI_TEXT_DROPSHADOW;
  style->titlebarStyle.textFont      = font;
  style->titlebarStyle.bgColor       = makecol(100,90,50);
  style->titlebarStyle.bgBitmap      = NULL;
  style->titlebarStyle.borderStyle   = TTGUI_BORDER_DOWN;
  style->titlebarStyle.borderWidth   = 2;
  style->titlebarStyle.areaStyle     = TTGUI_AREA_TILE;
  style->titlebarStyle.areaXOffs     = 0;
  style->titlebarStyle.areaYOffs     = 0;

  style->helpStyle.textColor         = makecol(0,0,0);
  style->helpStyle.textStyle         = TTGUI_TEXT_NORMAL;
  style->helpStyle.textFont          = font;
  style->helpStyle.bgColor           = makecol(170,155,40);
  style->helpStyle.bgBitmap          = NULL;
  style->helpStyle.borderStyle       = TTGUI_BORDER_BLACK;
  style->helpStyle.borderWidth       = 1;
  style->helpStyle.areaStyle         = TTGUI_AREA_TILE;
  style->helpStyle.areaXOffs         = 0;
  style->helpStyle.areaYOffs         = 0;

  // Set default window style to backgroundStyle:
  style->defaultGuiStyle.winStyle = style->backgroundStyle;
} // setDefaultStyle

/*--------------------------------------------------------------------------*/
// End of class TTApplication.
/*--------------------------------------------------------------------------*/


