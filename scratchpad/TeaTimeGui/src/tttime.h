//////////////////////////////////////////////////////////////////////////////
//   File name:  ttime.h
//
//   Unit:       Timer
//
//   Project:    Gui
//   Author:     Hans H„ggstr”m,  hans.haggstrom@helsinki.fi
//   Purpose:    A simple interrupt timer that increments a counter every
//               millisecond, and wrapper routines around it.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef TTTIME_H
#define TTTIME_H

#include <allegro.h>

void ttInstallMsTimer();
// Call this once before calling ttGetTime.

void ttRemoveMsTimer();
// Removes the timer handler (frees up one allegro timer).

long ttGetMsTimer();
// Returns a timer value that increments once every millisecond.

#endif
