//////////////////////////////////////////////////////////////////////////////
//   File name:  ttmousev.h
//
//   Unit:       Mouse events
//
//   Project:    Gui & general
//   Author:     Hans H„ggstr”m,  hans.haggstrom@helsinki.fi
//   Uses:       Allegro
//   Purpose:    To provide detection and queuing of mouse events in allegro.
//               Events are: button down, button up and mouse move.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef TTMOUSEV_H
#define TTMOUSEV_H

#include <allegro.h>
#include "ttguidef.h" // For mouse constants.

#define TTGUI_MOUSE_EVENTQUEUE_SIZE 512
#define TTGUI_MOUSE_EVENTQUEUE_MASK 0x1ff  // To avoid a mod in the event
                                           // handler.  Overkill? Maybe.

/*--------------------------------------------------------------------------*/
void ttInstallMouseEvents();
// Installs the event handler if it wasn't already installed.

void ttRemoveMouseEvents();
// Removes the event handler if it was installed and restores
// possible previous event handlers.

bool ttGetMouseEvent( int &x, int &y, int &button, int &event );
// returns true if there was an event. In that case x and y contain
// the location (in case of mouse move they contain the location after the
// move), button is one of TTGUI_MOUSE_LEFT, ..RIGHT or ..MIDDLE
// and event is one of TTGUI_MOUSE_DOWN, ..UP or ..MOVE.

bool ttSimulateMouseEvent( int x, int y, int button, int event );
// Simulate mouse event.  See above for description of parameters.
// Also moves the allegro mouse so that mouse_x and mouse_y are in
// sync with the events (in case some code uses both mouse_x/y and events).
// Returns true on success and false if the event buffer is full.

/*--------------------------------------------------------------------------*/

#endif


