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
#include "Bst.h"

bool ValHeap::print = false;

ValHeap::
~ValHeap()
{
  //int i;
  ValsIter vi = array.begin();
  for(  ; vi != array.end() ; vi++) delete (*vi);
}

void
ValHeap::
push(Val* atp)
{
  assert(atp);
  if(print)
    cerr << "heap insertion of atp at " << unusedPos_ << endl;
  assert((int)array.size() >= unusedPos_);
  if(array.size() ==  unusedPos_) array.push_back(atp);
  else array[unusedPos_] = atp;
  upheap(unusedPos_);
  unusedPos_++;
}

bool
ValHeap::
upheap(int pos)
{
  if(print) cerr << "in Upheap " << pos << " " << (int)array.size() << endl;
  if(pos == 0) return false;
  Val* atp = array[pos];
  assert(atp);
  double merit = atp->fom();
  int   parPos = parent(pos);
  Val* par = array[parPos];
  double pmerit = par->fom();
  if(print) cerr << "merits " << merit << " " << pmerit<< endl;
  if(merit > pmerit)
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


Val*
ValHeap::
pop()
{
  if(print)
    cerr << "popping" << endl;
  if(unusedPos_ == 0) return NULL;
  Val* retVal = array[0];
  del_(0);
  return retVal;
}

void
ValHeap::
downHeap(int pos)
{
  if(print) cerr << "downHeap " << pos << endl;
  if(pos >= unusedPos_-1) return;
  Val* par = array[pos];
  double merit = par->fom();
  int lc = left_child(pos);
  int rc = right_child(pos);
  int largec;
  int lcthere = 0;
  Val* lct;
  if(lc < unusedPos_)
    {
      lct = array[lc];
      if(lct) lcthere = 1;
    }
  int rcthere = 0;
  Val* rct;
  if(rc < unusedPos_)
    {
      rct = array[rc];
      if(rct) rcthere = 1;
    }
  if(!lcthere && !rcthere) return;
  assert(lcthere);
  if(!rcthere || (lct->fom() > rct->fom()))
    largec = lc;
  else largec = rc;
  Val* largeatp = array[largec];
  if(merit >= largeatp->fom()) 
    {
      if(print) cerr << "downheap of " << merit << " stopped by "
		     << " " << largeatp->fom() << endl;
      return;
    }
  array[pos] = largeatp;
  array[largec] = par;
  downHeap(largec);
}

void
ValHeap::
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
  downHeap(pos);
}
