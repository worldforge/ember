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

#include "FBinaryArray.h"
#include "Feat.h"
#include "FeatureTree.h"
void
FBinaryArray::
set(int sz) { size_ = sz; array_ = new Feat[sz]; };

Feat*
FBinaryArray::
index(int i) { return &array_[i]; }

Feat*
FBinaryArray::
find(const int id) const
{
  int top = size_;
  int bot = -1;
  int  midInd;
  for( ; ; )
    {
      if( top <= bot+1 )
	{
	  return NULL;
	}
      int mid = (top+bot)/2;
      Feat* midH = &array_[mid];
      midInd = midH->ind();
      if( id == midInd) return midH;
      else if( id < midInd) top = mid;
      else bot = mid;
    }
}

void
FTreeBinaryArray::
set(int sz) { size_ = sz; array_ = new FeatureTree[sz]; };

FeatureTree*
FTreeBinaryArray::
index(int i) { return &array_[i]; }

FeatureTree*
FTreeBinaryArray::
find(const int id) const
{
  int top = size_;
  int bot = -1;
  int  midInd;
  for( ; ; )
    {
      if( top <= bot+1 )
	{
	  return NULL;
	}
      int mid = (top+bot)/2;
      FeatureTree* midH = &array_[mid];
      midInd = midH->ind();
      if( id == midInd) return midH;
      else if( id < midInd) top = mid;
      else bot = mid;
    }
}
