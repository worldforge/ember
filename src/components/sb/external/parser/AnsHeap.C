/*
 * Copyright 2005 Brown University, Providence, RI.
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
#include "AnsHeap.h"

bool AnsTreeHeap::print = false;

AnsTreeHeap::
~AnsTreeHeap()
{
  int i;
  for( i = 0 ; i < unusedPos_ ; i++) delete array[i];
}

void
AnsTreeHeap::
insert(AnsTreePair* atp)
{
  if(!(unusedPos_ < AHeapSize))
    {
      cerr << "UP = " << unusedPos_ << endl;
      assert(unusedPos_ < AHeapSize);
    }
  if(print)
    cerr << "heap insertion of atp at " << unusedPos_ << endl;
  array[unusedPos_] = atp;
  upheap(unusedPos_);
  if(unusedPos_ == nth)
    {
      assert(front());
      //cerr << "Bef pop " << front()->first << endl;
      unusedPos_++;
      assert(array[1]);
      //cerr << "BP2 " << array[1]->first << endl;
      AnsTreePair* atp2 = pop();
      delete atp2;
      assert(front());
    }
  else unusedPos_++;
}

bool
AnsTreeHeap::
upheap(int pos)
{
  if(print) cerr << "in Upheap " << pos << endl;
  if(pos == 0) return false;
  AnsTreePair* atp = array[pos];
  double merit = atp->first;
  int   parPos = parent(pos);
  AnsTreePair* par = array[parPos];
  double pmerit = par->first;
  if(merit < pmerit)
    {
      array[parPos] = atp;
      array[pos] = par;
      if(print) cerr << "Put " << pos << " in " << parPos << endl;
      upheap(parPos);
      return true;
    }
  else if(print)
    {
      cerr << "upheap of " << merit << "stopped by "
	<< parPos << " " << pmerit << endl;
    }
  return false;
}


AnsTreePair*
AnsTreeHeap::
pop()
{
  if(print)
    cerr << "popping" << endl;
  if(unusedPos_ == 0) return NULL;
  AnsTreePair* retVal = array[0];
  del_(0);
  return retVal;
}

void
AnsTreeHeap::
downHeap(int pos)
{
  if(print) cerr << "downHeap " << pos << endl;
  if(pos >= unusedPos_-1) return;
  AnsTreePair* par = array[pos];
  double merit = par->first;
  int lc = left_child(pos);
  int rc = right_child(pos);
  int largec;
  int lcthere = 0;
  AnsTreePair* lct;
  if(lc < unusedPos_)
    {
      lct = array[lc];
      if(lct) lcthere = 1;
    }
  int rcthere = 0;
  AnsTreePair* rct;
  if(rc < unusedPos_)
    {
      rct = array[rc];
      if(rct) rcthere = 1;
    }
  if(!lcthere && !rcthere) return;
  assert(lcthere);
  if(!rcthere || (lct->first > rct->first))
    largec = lc;
  else largec = rc;
  AnsTreePair* largeatp = array[largec];
  if(merit <= largeatp->first) 
    {
      if(print) cerr << "downheap of " << merit << " stopped by "
		     << " " << largeatp->first << endl;
      return;
    }
  array[pos] = largeatp;
  array[largec] = par;
  downHeap(largec);
}

void
AnsTreeHeap::
del_(int pos)
{
  if(print) cerr << "del_ " << pos << endl;
  assert(unusedPos_);
  if(pos == (unusedPos_ - 1) )
    {
      unusedPos_--;
      array[unusedPos_] = NULL;
      return;
    }
  /* move the final edge in heap to empty position */
  array[pos] = array[unusedPos_ - 1];
  if(!array[pos])
    {
      error("Never get here");
      return;
    }
  unusedPos_--;
  array[unusedPos_] = NULL;
  if(upheap(pos)) return;
  downHeap(pos);
}
