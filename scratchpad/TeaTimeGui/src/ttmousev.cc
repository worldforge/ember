//////////////////////////////////////////////////////////////////////////////
//   File name:  ttmousev.cc
//
//   Unit:       Mouse events
//
//   Project:    Gui & general
//   Author:     Hans H„ggstr”m,  hans.haggstrom@helsinki.fi
//   Uses:       Allegro
//   Purpose:    To provide detection and queuing of mouse events in allegro.
//               Events are: button down, button up and mouse move.
//
//
//   Work diary:
// Date      Time     Lines   Things done
// ~~~~~~~~~ ~~~~~~~~ ~~~~~~~ ~~~~~~~~~~~~~~~~~~~~~~
// 17.5.98   0:30     (56)    Header
// 17.5      2:00     240     All.  Not tested yet.  The locking bussiness
//                            was new to me, hope it's ok.
// 17.5      1:00     261     Debugged and seems to work fine.
// 19.5      0:30     330     Fixed detection of multiple events at the same
//                            time, and fixed the MOUSE_EVENTQUE_MASK.
//
// Notes & things to do:
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// * Uses a circular queue to store mouse events.
//
// * Can things be locked twice?  Just to be on the safe side I added
//   a flag to make sure they're only locked once.
//
//////////////////////////////////////////////////////////////////////////////

#include "ttmousev.h"
#include "iostream.h"

/*--------------------------------------------------------------------------*/
// Event queue struct:
struct TTMouseEventQueueNode {
  int pos;  // x and y in high/low words (someway).
  // int type; // the flags that the event handler gets.
  int button;
  int event;
};

// Static variables:
static bool ttMouseEvInstalled = false;
static bool thingsLocked       = false;
static void (*ttOldMouseCallback)(int flags) = NULL;

// The event queue:
static volatile struct TTMouseEventQueueNode mouseEvents[TTGUI_MOUSE_EVENTQUEUE_SIZE];
static volatile unsigned int  firstFreeMouseEvent = 1;
static volatile unsigned int  lastReadMouseEvent  = 0;
static volatile bool blockEvents         = false; // Semaphore.  If true, the event
                                        // handler must not modify the queue.

/*--------------------------------------------------------------------------*/
void ttMouseEventHandler( int flags ) {
  // Check semaphore and if there is room:
  if ((firstFreeMouseEvent == lastReadMouseEvent) || blockEvents) return;

  // Extract all events from flags and store in event queue:
  if (flags & MOUSE_FLAG_LEFT_DOWN) {
    mouseEvents[firstFreeMouseEvent].button = TTGUI_MOUSE_LEFT;
    mouseEvents[firstFreeMouseEvent].event  = TTGUI_MOUSE_DOWN;
    mouseEvents[firstFreeMouseEvent].pos    = mouse_pos;
    firstFreeMouseEvent = (++firstFreeMouseEvent) & TTGUI_MOUSE_EVENTQUEUE_MASK;
    if (firstFreeMouseEvent == lastReadMouseEvent) return;
  }
  if (flags & MOUSE_FLAG_LEFT_UP) {
    mouseEvents[firstFreeMouseEvent].button = TTGUI_MOUSE_LEFT;
    mouseEvents[firstFreeMouseEvent].event  = TTGUI_MOUSE_UP;
    mouseEvents[firstFreeMouseEvent].pos    = mouse_pos;
    firstFreeMouseEvent = (++firstFreeMouseEvent) & TTGUI_MOUSE_EVENTQUEUE_MASK;
    if (firstFreeMouseEvent == lastReadMouseEvent) return;
  }
  if (flags & MOUSE_FLAG_RIGHT_DOWN) {
    mouseEvents[firstFreeMouseEvent].button = TTGUI_MOUSE_RIGHT;
    mouseEvents[firstFreeMouseEvent].event  = TTGUI_MOUSE_DOWN;
    mouseEvents[firstFreeMouseEvent].pos    = mouse_pos;
    firstFreeMouseEvent = (++firstFreeMouseEvent) & TTGUI_MOUSE_EVENTQUEUE_MASK;
    if (firstFreeMouseEvent == lastReadMouseEvent) return;
  }
  if (flags & MOUSE_FLAG_RIGHT_UP) {
    mouseEvents[firstFreeMouseEvent].button = TTGUI_MOUSE_RIGHT;
    mouseEvents[firstFreeMouseEvent].event  = TTGUI_MOUSE_UP;
    mouseEvents[firstFreeMouseEvent].pos    = mouse_pos;
    firstFreeMouseEvent = (++firstFreeMouseEvent) & TTGUI_MOUSE_EVENTQUEUE_MASK;
    if (firstFreeMouseEvent == lastReadMouseEvent) return;
  }
  if (flags & MOUSE_FLAG_MIDDLE_DOWN) {
    mouseEvents[firstFreeMouseEvent].button = TTGUI_MOUSE_MIDDLE;
    mouseEvents[firstFreeMouseEvent].event  = TTGUI_MOUSE_DOWN;
    mouseEvents[firstFreeMouseEvent].pos    = mouse_pos;
    firstFreeMouseEvent = (++firstFreeMouseEvent) & TTGUI_MOUSE_EVENTQUEUE_MASK;
    if (firstFreeMouseEvent == lastReadMouseEvent) return;
  }
  if (flags & MOUSE_FLAG_MIDDLE_UP) {
    mouseEvents[firstFreeMouseEvent].button = TTGUI_MOUSE_MIDDLE;
    mouseEvents[firstFreeMouseEvent].event  = TTGUI_MOUSE_UP;
    mouseEvents[firstFreeMouseEvent].pos    = mouse_pos;
    firstFreeMouseEvent = (++firstFreeMouseEvent) & TTGUI_MOUSE_EVENTQUEUE_MASK;
    if (firstFreeMouseEvent == lastReadMouseEvent) return;
  }
  if (flags & MOUSE_FLAG_MOVE) {
    mouseEvents[firstFreeMouseEvent].button = TTGUI_MOUSE_NOBUTTON;
    mouseEvents[firstFreeMouseEvent].event  = TTGUI_MOUSE_MOVE;
    mouseEvents[firstFreeMouseEvent].pos    = mouse_pos;
    firstFreeMouseEvent = (++firstFreeMouseEvent) & TTGUI_MOUSE_EVENTQUEUE_MASK;
    if (firstFreeMouseEvent == lastReadMouseEvent) return;
  }

  // Move firstFree to point to next position in queue:
  //firstFreeMouseEvent++;
  //if (firstFreeMouseEvent >= TTGUI_MOUSE_EVENTQUEUE_SIZE) firstFreeMouseEvent = 0;

} // ttMouseEventHandler
END_OF_FUNCTION( ttMouseEventHandler );

/*--------------------------------------------------------------------------*/
void ttInstallMouseEvents() {
// Installs the event handler if it wasn't already installed.

  if (ttMouseEvInstalled) return;

  // Check that allegro mousehandler is installed?

  // Store possible old event handler:
  ttOldMouseCallback = mouse_callback;

  // Lock function and variables to make sure they aren't swapped away:
  if (!thingsLocked) {
    LOCK_VARIABLE(mouseEvents);
    LOCK_VARIABLE(firstFreeMouseEvent);
    LOCK_VARIABLE(lastReadMouseEvent);
    LOCK_VARIABLE(blockEvents);
    LOCK_FUNCTION(ttMouseEventHandler);
    thingsLocked = true;
  }

  // Install own event handler:
  mouse_callback = ttMouseEventHandler;

  ttMouseEvInstalled = true;
} // ttInstallMouseEvents

/*--------------------------------------------------------------------------*/
void ttRemoveMouseEvents() {
// Removes the event handler if it was installed and restores
// possible previous event handlers.

  if (!ttMouseEvInstalled) return;

  // Restore old event handler:
  mouse_callback = ttOldMouseCallback;

  ttMouseEvInstalled = false;
} // ttRemoveMouseEvents

/*--------------------------------------------------------------------------*/
bool ttGetMouseEvent( int &x, int &y, int &button, int &event ) {
// returns true if there was an event. In that case x and y contain
// the location (in case of mouse move they contain the location after the
// move), button is one of TTGUI_MOUSE_LEFT, ..RIGHT or ..MIDDLE
// and event is one of TTGUI_MOUSE_DOWN, ..UP or ..MOVE.

  // Turn events off:
  // Mouse moves/clicks made under this time won't be registered.
  blockEvents = true;

  // Get position of next unread event:
  unsigned int next;
  next  = lastReadMouseEvent + 1;
  if (next >= TTGUI_MOUSE_EVENTQUEUE_SIZE) next = 0;
//next &= TTGUI_MOUSE_EVENTQUEUE_MASK;
// Why doesn't the above line work??

  // Check if there is events:
  if (next == firstFreeMouseEvent) {
    // Turn events on:
    blockEvents = false;
    return false;
  }

  // Move lastRead to point to next position in queue:
  lastReadMouseEvent = next;

  // Read event:
  button = mouseEvents[next].button;
  event  = mouseEvents[next].event;
  unsigned int pos  = mouseEvents[next].pos;

  // Turn events on:
  blockEvents = false;

  // Extract fields:
  x = pos >> 16;
  y = pos & 0xffff;

  // Ye olde debugline:
  //cerr << "lastRead = " << lastReadMouseEvent << ", firstFree = " << firstFreeMouseEvent << "\n";
  //cerr << "x = " << x << ", y = " << y
  //     << ", button "  << button << ", event " << event << endl;
  //cerr << "clock " << clock() << endl;

/*
  // Read event:
  int          type = mouseEvents[next].type;
  unsigned int pos  = mouseEvents[next].pos;

  // Turn events on:
  blockEvents = false;

  // Ye olde debugline:
  //cerr << "lastRead = " << lastReadMouseEvent << ", firstFree = " << firstFreeMouseEvent << "\n";

  // Extract fields:
  x = pos >> 16;
  y = pos & 0xffff;
  switch (type) {
    case MOUSE_FLAG_LEFT_DOWN:
      button = TTGUI_MOUSE_LEFT;
      event  = TTGUI_MOUSE_DOWN;
    break;

    case MOUSE_FLAG_LEFT_UP:
      button = TTGUI_MOUSE_LEFT;
      event  = TTGUI_MOUSE_UP;
    break;

    case MOUSE_FLAG_RIGHT_DOWN:
      button = TTGUI_MOUSE_RIGHT;
      event  = TTGUI_MOUSE_DOWN;
    break;

    case MOUSE_FLAG_RIGHT_UP:
      button = TTGUI_MOUSE_RIGHT;
      event  = TTGUI_MOUSE_UP;
    break;

    case MOUSE_FLAG_MIDDLE_DOWN:
      button = TTGUI_MOUSE_MIDDLE;
      event  = TTGUI_MOUSE_DOWN;
    break;

    case MOUSE_FLAG_MIDDLE_UP:
      button = TTGUI_MOUSE_MIDDLE;
      event  = TTGUI_MOUSE_UP;
    break;

    case MOUSE_FLAG_MOVE:
      button = TTGUI_MOUSE_NOBUTTON;
      event  = TTGUI_MOUSE_MOVE;
    break;

    default: // Prepare for anything:
      button = TTGUI_MOUSE_NOBUTTON;
      event  = TTGUI_MOUSE_MOVE; // Just something..
    break;
  } // end switch.
*/
  // There was an event:
  return true;

} // ttGetMouseEvent

/*--------------------------------------------------------------------------*/
bool ttSimulateMouseEvent( int x, int y, int button, int event ) {
// Simulate mouse event.  See above for description of parameters.
// Also moves the allegro mouse so that mouse_x and mouse_y are in
// sync with the events (in case some code uses both mouse_x/y and events).
// Returns true on success and false if the event buffer is full.

  // Clip coordinates to screen:
  if (x < 0) x = 0;
  else if (x >= SCREEN_W) x = SCREEN_W-1;
  if (y < 0) y = 0;
  else if (y >= SCREEN_H) y = SCREEN_H-1;

  // Encode event:
  // X & y,  store x in high and y in low word:
  unsigned int pos;
  pos  = x << 16;
  pos |= y;

/*
  // Mouse flags:
  int flags = 0;
  if (event  == TTGUI_MOUSE_MOVE) flags = MOUSE_FLAG_MOVE;
  else if (button == TTGUI_MOUSE_LEFT   && event == TTGUI_MOUSE_DOWN) flags = MOUSE_FLAG_LEFT_DOWN;
  else if (button == TTGUI_MOUSE_LEFT   && event == TTGUI_MOUSE_UP)   flags = MOUSE_FLAG_LEFT_UP;
  else if (button == TTGUI_MOUSE_RIGHT  && event == TTGUI_MOUSE_DOWN) flags = MOUSE_FLAG_RIGHT_DOWN;
  else if (button == TTGUI_MOUSE_RIGHT  && event == TTGUI_MOUSE_UP)   flags = MOUSE_FLAG_RIGHT_UP;
  else if (button == TTGUI_MOUSE_MIDDLE && event == TTGUI_MOUSE_DOWN) flags = MOUSE_FLAG_MIDDLE_DOWN;
  else if (button == TTGUI_MOUSE_MIDDLE && event == TTGUI_MOUSE_UP)   flags = MOUSE_FLAG_MIDDLE_UP;
*/
  // Turn events off:
  blockEvents = true;

  // Check if there is room:
  if (firstFreeMouseEvent == lastReadMouseEvent) {
    // Turn events on:
    blockEvents = false;
    return false;
  }

  // Store event:
  mouseEvents[firstFreeMouseEvent].pos  = pos;
//  mouseEvents[firstFreeMouseEvent].type = flags;
  mouseEvents[firstFreeMouseEvent].button = button;
  mouseEvents[firstFreeMouseEvent].event  = event;

  // Move firstFree to point to next position in queue:
  firstFreeMouseEvent++;
  if (firstFreeMouseEvent >= TTGUI_MOUSE_EVENTQUEUE_SIZE) firstFreeMouseEvent = 0;
// firstFreeMouseEvent = firstFreeMouseEvent & TTGUI_MOUSE_EVENTQUEUE_MASK;
// Why doesn't the above line work??

  // Turn events on:
  blockEvents = false;

  // Reposition mouse:
  position_mouse(x, y);

  // All ok:
  return true;

} // ttSimulateMouseEvent

/*--------------------------------------------------------------------------*/


