//////////////////////////////////////////////////////////////////////////////
//
//  Project:   Guitest
//  Author:    Hans H„ggstr”m,  (hans.haggstrom@helsinki.fi)
//  Purpose:   Test the features and gizmos of TeaTimeGUI.
//  Based on:  gamebase.cc
//
//
//  Work diary:
//  Date     Hours  Lines  Things done
//  ~~~~~~~~ ~~~~~~ ~~~~~~ ~~~~~~~~~~~~~~~                                                         }{
//  17.1.99  4:00   384    Started project and spent a few hours bugghunttting a silly bug.  (    (00)  <- silly bug  (i needed setDefaultStyle))
//                                                                                                 ""
//
//  Notes & things to do:
//  ~~~~~~~~~~~~~~~~~~~~~~~~
//
//
//////////////////////////////////////////////////////////////////////////////

#include <iostream.h>
#include <stdio.h>
#include <string>
#include <allegro.h>
#include <math.h>
#include <time.h>

#include <allegttf.h>

#include "load_cel.h"
#include "ttcache.h"
#include "ttgui.h"

/*--------------------------------------------------------------------------*/
/*----[ Defines: ]----------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
#define GAME_SCREENSIZEX      640
#define GAME_SCREENSIZEY      480
#define GAME_COLORDEPTH       8
#define GAME_TARGET_FPS       30
#define GAME_MAX_LOGIC_LOOPS  5

//----------------------------------------------------------------------------
// Load_bitmap -wrapper for picCache:
//----------------------------------------------------------------------------
BITMAP *bitmap_loader( char *filename ) {
  PALETTE dummyPal;
  return load_bitmap( filename, dummyPal );
}


/*--------------------------------------------------------------------------*/
/*----[ Global variables ]--------------------------------------------------*/
/*--------------------------------------------------------------------------*/
bool    quit      = false;  // Quit gui when true.
BITMAP *dbuffer   = NULL;   // Double buffer screen for gfx.

PALETTE palette;

// Gui application:
TTApplication application;

// Set up load cahce for bitmaps:
TTCache<BITMAP*> picCache( bitmap_loader, destroy_bitmap, NULL );

// Prototye:
void done();
//void init_gui();
//void init_font();

/*--------------------------------------------------------------------------*/
/*----[ Timing: ]-----------------------------------------------------------*/
//----------------------------------------------------------------------------
volatile int target_game_time = 0;
int          actual_game_time = 0;

/*--------------------------------------------------------------------------*/
void increment_target_game_time(...) {
   target_game_time++;
}
END_OF_FUNCTION(increment_target_game_time);


/*--------------------------------------------------------------------------*/
/*----[ Error ]-------------------------------------------------------------*/
//----------------------------------------------------------------------------
// Shuts down the program and displays an error message.
void game_error(string msg) {
  // Shut down (done() should not call error()).
  done();
  cerr << msg << endl;
  exit(1);

} // game_error

//----------------------------------------------------------------------------
void exitIf (bool b, char *errmsg) {
  if (b) {
    fprintf (stderr, "Error: %s\n", errmsg);
    exit(1);
  }
}

/*--------------------------------------------------------------------------*/
/*----[ Gui callback functions: ]-------------------------------------------*/
//----------------------------------------------------------------------------

void func_exterminate( TTWindow *w ) {
  quit = true;
}

void func_beep( TTWindow *w ) {
  cout << "\a";
}

void func_size( TTWindow *w ) {
  if (w) {
    w->setVisibleAreaSize( 32, 32 );
  }
}



/*--------------------------------------------------------------------------*/
/*----[ Init ]--------------------------------------------------------------*/
//----------------------------------------------------------------------------
// Initializes the program, starts allegro, loads graphics etc.
void init() {

  allegro_init();

  // Init colordepth:
  set_color_depth(GAME_COLORDEPTH);
  
  if (set_gfx_mode(GFX_AUTODETECT, GAME_SCREENSIZEX, GAME_SCREENSIZEY, 0, 0) < 0)
    game_error( allegro_error );

  install_timer();
  install_keyboard();
  
  int numMousebuttons = install_mouse();
  exitIf( numMousebuttons == -1, "Couldn't initialize mouse\n");

  // Init paletteinfo:
  PaletteInfo *palInfo = new PaletteInfo(16, 31, 15, 16, 16);
  if (palInfo) palInfo->setAsCurrent();


  dbuffer = create_bitmap(SCREEN_W, SCREEN_H);
  if (!dbuffer) game_error("Couldn't reserve memory for double-buffer, out of mem??");

  clear(dbuffer);

  // Lock timing counter and interrupt:
  LOCK_VARIABLE(target_game_time);
  LOCK_FUNCTION(increment_target_game_time);

  // Instal timer interrupt:
  install_int_ex(increment_target_game_time, BPS_TO_TIMER( GAME_TARGET_FPS ));

  // Install CEL-file loaders:
  register_bitmap_file_type( "cel", m_load_cel, NULL);

  show_mouse(screen);

} // init

/*--------------------------------------------------------------------------*/
/*----[ Done ]----------------------------------------------------------*/
//----------------------------------------------------------------------------
// Shuts down the program, frees memory etc.
void done() {

  // Shut down application:
  application.shutDown();

  remove_int(increment_target_game_time);

  if (dbuffer) destroy_bitmap(dbuffer);
  dbuffer = NULL;  
} // done

//----------------------------------------------------------------------------
/*--------------------------------------------------------------------------*/
void init_font() {
  // Init antialiasing:
  //antialias_init(mypal);

  // Loads the arial font:
  FONT *f;
  f = load_ttf_font("fe______.ttf", 9, 2);
  if (f) font = f;
  else cerr << "couldn't load arial.ttf\n";

} // init_font

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void init_gui() {
// Sets up the gui stuff.

  // Init default style to make sure the current palette is used to get the colors:
  application.setDefaultStyle();

  // Add mouse cursors:
  TTApplicationStyle *appStyle = application.getStyle();
  if (appStyle) {
//    appStyle->mouseCursorPic          = picCache.get( "mousecur.cel" );
    appStyle->mouseMovePic             = picCache.get( "mousemov.cel" );
    appStyle->mouseResizeVerticalPic   = picCache.get( "mousever.cel" );
    appStyle->mouseResizeHorizontalPic = picCache.get( "mousehor.cel" );
    appStyle->mouseResizeRightPic      = picCache.get( "mouserig.cel" );
    appStyle->mouseResizeLeftPic       = picCache.get( "mouselef.cel" );  
  }
    
  // Create form:
  TTForm *win  = new TTForm( 10, 10, 500, 400, NULL, &application );
  exitIf (!win, "Couldn't create form");
  TTWindowStyle *style = win->getWinStyle();
  if (style) {
    //style->bgBitmap    = m_load_cel("gripbg3.cel", dummypal);
    style->areaStyle   = TTGUI_AREA_TILE;
  }
  win->setDragable( true );

  // Set test mousecursor:
  TTGuiStyle *guiStyle = win->getGuiStyle();
  if (guiStyle) guiStyle->mouseCursorPic = picCache.get( "mousecur.cel" );

  TTButton *button1 = new TTButton( 10, 10, 80, 25, NULL, &application );
  exitIf (!button1, "Couldn't create button1");
  button1->setCaption( "Push me!" );
  button1->setCallbackFunction( func_beep );

  TTButton *button2 = new TTButton( 65, 45, 80, 25, NULL, &application );
  exitIf (!button2, "Couldn't create button2");
  button2->setCaption( "Exterminate" );
  button2->setCallbackFunction( func_exterminate );
  //button2->showFocusOutline = false;
  // Exterminate-button-style:
  style = button2->getWinStyle();
  if (style) {
    style->borderStyle = TTGUI_BORDER_SOFTUP;
    style->borderWidth = 3;
    style->textStyle   = TTGUI_TEXT_LOWERED;
    //style->textStyle   = TTGUI_TEXT_SOFT;
    //style->bgBitmap    = m_load_cel("gripbg1.cel", dummypal);
    style->areaStyle   = TTGUI_AREA_TILE;
  }
  button2->setMovable( true );
  
  // Set test mousecursor:
  guiStyle = button2->getGuiStyle();
  if (guiStyle) guiStyle->mouseCursorPic = picCache.get( "mousemov.cel" );


  TTButton *button3 = new TTButton( 100, 10, 100, 25, NULL, &application );
  exitIf (!button3, "Couldn't create button3");
  button3->setCaption( "Don't push me!" );
  button3->setCallbackFunction( func_size );
  //button3->disable();

  TTStatic *static1 = new TTStatic( 10, 50, 100, 50, NULL, &application );
  exitIf (!static1, "Couldn't create static1");
  static1->setText("This is a test\nOh yeah, this is a test. \nYepp");
  static1->setMultiline(true);
  static1->setAutoadjustHeight( true );
  static1->setDragable( true );
  //static1->setSelectable(true);

  TTStatic *static2 = new TTStatic( 90, 80, 140, 30, NULL, &application );
  exitIf (!static2, "Couldn't create static2");
  static2->setText("This is a one-liner\nAnd this is a hidden part=)");
  static2->setMultiline(false);
  static2->setMovable( true );
  static2->setDragable( true );
  //static2->setSelectable(true);

  TTEdit *edit1 = new TTEdit( 90, 100, 100, 20, NULL, &application );
  exitIf (!edit1, "Couldn't create edit1");
  edit1->setMultiline(true);
  edit1->setText("Edit me!"/* \nI am line 2!\nLine 3\nAnd so on, add 4."*/);
  edit1->setAutoadjustHeight(true);
  edit1->setPersistentSelection(true);
  edit1->setVirtualAreaSize(300, 300);
  edit1->setVisibleAreaSize( edit1->getVisibleW(), 60);
  edit1->setEnabled(true);
  edit1->setEditable(true);
  edit1->setSelectable(true);
  edit1->setMovable( true );
  style = edit1->getWinStyle();
  if (style) {
    //style->borderStyle = TTGUI_BORDER_SOFTUP;
    //style->borderWidth = 3;
    //style->textStyle   = TTGUI_TEXT_LOWERED;
    //style->textStyle   = TTGUI_TEXT_SOFT;
    //style->textStyle   = TTGUI_TEXT_RAISED;
    //style->bgBitmap    = m_load_cel("gripbg1.cel", dummypal);
    //style->areaStyle   = TTGUI_AREA_TILE;
  }

  // ------------[ Test TTList: ]---------------
  TTList *list1 = new TTList( &application );
  exitIf (!list1, "Couldn't create list1");
  list1->setChangeListShape( true );
  list1->setItemSpacing( 8 );
  list1->setMovable( true );
  list1->setDragable( true );
  list1->setDirection(1);
  list1->setAreaSize( 80, 400 );
  list1->setPos( 300, 20 );
  list1->addItem( button1 );
  list1->addItem( button2 );
  list1->addItem( button3 );
  list1->addItem( static1 );
  list1->setListSizeBoundaries( 32, 80, 32, 120 );

  // Set test mousecursor:
  guiStyle = edit1->getGuiStyle();
  if (guiStyle) guiStyle->mouseCursorPic = picCache.get( "mousever.cel" );

//  win->addWindow( button1 );
//  win->addWindow( button3 );
//  win->addWindow( button2 );
//  win->addWindow( static1 );
  win->addWindow( static2 );
  win->addWindow( edit1   );
  win->addWindow( list1   );

  application.setMainWindow( win );
  application.setup();

} // init_gui

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void show_intro(BITMAP *dbuf) {
// Shows intro
// Loads & shows splash screen.

  clear(screen);
  
  BITMAP *splashscr;// = picCache.get( "splash.cel");
  splashscr = m_load_cel("splash.cel", palette);
  
  exitIf (splashscr == NULL, "couldn't load splash.cel");
  
  set_palette(palette);

  // Init paletteinfo:  (hmm.. This paletteinfo stuff is quite unneccesary i think.  We should just drop it....)
  PaletteInfo *palInfo = new PaletteInfo(16, 31, 15, 16, 16);
  if (palInfo) palInfo->setAsCurrent();

  
  blit(splashscr, screen, 0, 0, (SCREEN_W-splashscr->w) / 2,
                                (SCREEN_H-splashscr->h) / 2,
                                splashscr->w, splashscr->h);

} // show_intro

/*--------------------------------------------------------------------------*/
/*----[ Main ]--------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
int main() {

  // Init the gui and allegro:
  init();

  // Timing:
  int logicLoopsDone   = TRUE;

  // Fps counter:
  time_t prevsec = time(NULL);
  int frames  = 0;
  int fps     = 0;

  // Load font:
  init_font();

  // Show the intro:
  show_intro(dbuffer);

  // Setup gui:
  init_gui();

  // Draw gui to dbuffer:
  application.draw(dbuffer);

  show_mouse(dbuffer);

  // Main loop:
  while (!quit) {

    // Logic:
    while( actual_game_time < target_game_time &&
           logicLoopsDone < GAME_MAX_LOGIC_LOOPS && !quit ) {
             
      // Check ESC:
      if (key[KEY_ESC]) quit = true;

      // Get input & do other logic stuff:
      application.doTurn();

      actual_game_time++;
      logicLoopsDone++;
    } // End of logic loop.


    // Draw graphics:
    if (logicLoopsDone > 0) {
      logicLoopsDone = 0;

      // Hide mouse:
      show_mouse(NULL);

      // Uppdate screen:
      application.draw(dbuffer);
      //application.update(dbuffer);

      // Fps counter:
      frames++;
      time_t curtime = time(NULL);
      if (prevsec != curtime) {
        prevsec = curtime;
        fps = frames;
        frames = 0;
      }
      textprintf(dbuffer, font, SCREEN_W-120, 0, /*color+*/fps, "Fps: %d", fps);

      show_mouse(dbuffer);
      
      // Wait for vsync (this flickers in Win95, disable?)
      vsync();

      // Copy dbuffer to screen:
      blit( dbuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H );
      
      show_mouse(screen); // Two showmouses.. but if only on screen -> flicker
                          // if only on dbuffer -> freezes when program pauses/etc.
                          // Now it sometimes is like a very short mousetrail
                          // (a la windows mouse settings), which is ok IMHO.
      clear(dbuffer);
    } // End of draw graphics.

  } // End of main loop


  // Shut down the gui and allegro:
  done();

  // Return to os:
  return 0;

} // End of program
//----------------------------------------------------------------------------


