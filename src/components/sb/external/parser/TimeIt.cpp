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

#include "TimeIt.h"
#include <fstream>

void
TimeIt::
befSent()
{
  lastTime = clock();
  currTime = clock();
  lastTimeSec = (double)lastTime/(double)CLOCKS_PER_SEC;
  currTimeSec = (double)currTime/(double)CLOCKS_PER_SEC;
  elapsedTime = currTimeSec - lastTimeSec;
  if(elapsedTime < 0) elapsedTime += 2147;
  totAccessTime += elapsedTime;
  lastTime = currTime;
}

void
TimeIt::
betweenSent(Bchart* chart)
{
  currTime = clock();
  lastTimeSec = (double)lastTime/(double)CLOCKS_PER_SEC;
  currTimeSec = (double)currTime/(double)CLOCKS_PER_SEC;
  elapsedTime = currTimeSec - lastTimeSec;
  if(elapsedTime < 0) elapsedTime += 2147;
  cerr << "Parsing time = " << elapsedTime
       << "\tEdges created = " << chart->totEdgeCountAtS()
       << "\tEdges poped = " << chart->popedEdgeCountAtS() << endl;
  totParseTime += elapsedTime;
  //totEdges += chart->totEdgeCountAtS();
  //totPopedEdges += chart->popedEdgeCountAtS();
  totEdges += chart->totEdgeCountAtS();
  totPopedEdges += chart->popedEdgeCountAtS();
  lastTime = clock();
}


void
TimeIt::
aftSent()
{
  currTime = clock();
  lastTimeSec = (double)lastTime/(double)CLOCKS_PER_SEC;
  currTimeSec = (double)currTime/(double)CLOCKS_PER_SEC;
  elapsedTime = currTimeSec - lastTimeSec;
  if(elapsedTime < 0) elapsedTime += 2147;
  cerr << "Sem Parsing time = " << elapsedTime << endl;
  totSemParseTime += elapsedTime;
}

void
TimeIt::
finish(int totSents)
{
  cout << "Av access time = " << totAccessTime/totSents
       << "\t Av parse time = "
       << totParseTime/totSents
       << "\t Av stats time = "
       << totSemParseTime/totSents
       << "\nAv edges created = "
       << (float)totEdges/totSents
       << "\tAv edges poped = "
       << (float)totPopedEdges/totSents
       << endl;
}
