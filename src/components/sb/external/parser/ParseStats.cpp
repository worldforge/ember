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

#include "ECString.h"
#include "ParseStats.h"
#include "utils.h"

istream&
operator >>( istream& is, ParseStats& ps )
{
  ps.readInput( is );
  return is;
}

float
ParseStats::
precision()
{
  int inGu = numInGuessed;
  if(inGu == 0)  return 0;
  float prc = (float)numCorrect/(float)inGu;
  return prc;
}

float
ParseStats::
recall()
{
  int inGo = numInGold;
  if(inGo == 0) return 0;
  float rec = (float)numCorrect/(float)inGo;
  return rec;
}

float
ParseStats::
fMeasure()
{
  float prc = precision();
  float rec = recall();

  float sm = prc+rec;
  if(sm == 0) return 0;
  return (2*prc*rec)/sm;
}

void
ParseStats::
readInput( istream& is )
{
  ECString bracket;
  is >> bracket;
  if( !is ) return;
  if( bracket != "<" ) error( "No open bracket for ParseStats" );
  is >> numInGold;
  is >> numInGuessed;
  is >> numCorrect;
  is >> bracket;
  if( bracket != ">" ) error( "No close bracket for ParseStats" );
}

ParseStats&
ParseStats::
operator+= ( const ParseStats& src )
{
  numInGold += src.numInGold;
  numInGuessed += src.numInGuessed;
  numCorrect += src.numCorrect;
  return *this;
}

void
ParseStats::
operator= ( const ParseStats& src )
{
  numInGold = src.numInGold;
  numInGuessed = src.numInGuessed;
  numCorrect = src.numCorrect;
}

ostream&
operator <<( ostream& os, const ParseStats& ps )
{
  os << "< " << ps.numInGold << " " << ps.numInGuessed << " "
     << ps.numCorrect << " >\n";
  return os;
}
