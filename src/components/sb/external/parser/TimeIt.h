/*
 * Copyright 1999, 2005 Brown University, Providence, RI.
 * 
 *                         All Rights Reserved
 * 
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose other than its incorporation into a
 * commercial product is hereby granted without fee, provided that the
 * above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Brown University not be used in
 * advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission.
 * 
 * BROWN UNIVERSITY DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR ANY
 * PARTICULAR PURPOSE.  IN NO EVENT SHALL BROWN UNIVERSITY BE LIABLE FOR
 * ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef TIMEIT_H
#define TIMEIT_H

#include <time.h>
#include "Bchart.h"

class TimeIt
{
 public:
  TimeIt() : totAccessTime(0), totParseTime(0), totSemParseTime(0),
    totEdges(0), totPopedEdges(0) {}
  void befSent();
  void betweenSent(Bchart* chart);
  void aftSent();
  void finish(int totSents);
  int    totEdges;
  int    totPopedEdges;
  double totAccessTime;
  double totParseTime;
  double totSemParseTime;
  clock_t lastTime;
  clock_t currTime;
  double lastTimeSec;
  double currTimeSec;
  double elapsedTime;
};

#endif /* ! TIMEIT_H */
