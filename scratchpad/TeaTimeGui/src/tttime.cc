//////////////////////////////////////////////////////////////////////////////
//   File name:  ttime.cc
//
//   Unit:       Timer
//
//   Project:    Gui
//   Author:     Hans H„ggstr”m,  hans.haggstrom@helsinki.fi
//   Purpose:    A simple interrupt timer that increments a counter every
//               millisecond, and wrapper routines around it.
//
//
//   Work diary:
// Date      Time     Lines   Things done
// ~~~~~~~~~ ~~~~~~~~ ~~~~~~~ ~~~~~~~~~~~~~~~~~~~~~~
// 17.8.98   0:10     50      All
//
// Notes & things to do:
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//////////////////////////////////////////////////////////////////////////////

#include "tttime.h"

/*--------------------------------------------------------------------------*/
volatile long ttMsCounter;
          
/*--------------------------------------------------------------------------*/
void ttTimerHandler(...) {
  ttMsCounter++;
}
END_OF_FUNCTION(ttTimerHandler);

/*--------------------------------------------------------------------------*/
void ttInstallMsTimer() {
// Call this once before calling ttGetTime.
  LOCK_VARIABLE(ttMsCounter);
  LOCK_FUNCTION(ttTimerHandler);
  install_int(ttTimerHandler, 1);
  ttMsCounter = 0;
} // ttInstallMsTimer

/*--------------------------------------------------------------------------*/
void ttRemoveMsTimer() {
// Removes the timer handler (frees up one allegro timer).
  remove_int(ttTimerHandler);
  ttMsCounter = 0;
} //ttRemoveMsTimer

/*--------------------------------------------------------------------------*/
long ttGetMsTimer() {
// Returns a timer value that increments once every millisecond.
  return ttMsCounter;
} // ttGetMsTimer

/*--------------------------------------------------------------------------*/

