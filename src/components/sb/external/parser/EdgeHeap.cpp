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

#include "Edge.h"
#include "EdgeHeap.h"

EdgeHeap::
~EdgeHeap()
{
  int i;
  for(i = 0 ; i < unusedPos_ ; i++) delete array[i];
}

EdgeHeap::
EdgeHeap()
{
  int i;
  for (i = 0 ; i < HeapSize ; i++) array[i] = NULL;
  print = false;
  unusedPos_ = 0;
}


void
EdgeHeap::
insert(Edge* edge)
{
  if(print)
    cerr << "heap insertion of " << *edge << " at " << unusedPos_ << endl;
  assert(unusedPos_ < HeapSize);
  array[unusedPos_] = edge;
  edge->heapPos() = unusedPos_;
  upheap(unusedPos_);
  unusedPos_++;
  assert(unusedPos_ < HeapSize);
}

bool
EdgeHeap::
upheap(int pos)
{
  if(print) cerr << "in Upheap " << pos << endl;
  if(pos == 0) return false;
  assert(pos < HeapSize);
  Edge* edge = array[pos];
  assert(edge->heapPos() == pos);
  double merit = edge->merit();
  int   parPos = parent(pos);
  assert(parPos < HeapSize);
  Edge* par = array[parPos];
  assert(par->heapPos() == parPos);
  if(merit > par->merit())
    {
      assert(parPos < HeapSize);
      array[parPos] = edge;
      edge->heapPos() = parPos;
      assert(pos < HeapSize);
      array[pos] = par;
      par->heapPos() = pos;
      if(print) cerr << "Put " << *edge << " in " << parPos << endl;
      upheap(parPos);
      return true;
    }
  else if(print)
    {
      cerr << "upheap of " << merit << "stopped by "
	<< *par << " " << par->merit() << endl;
    }
  return false;
}


Edge*
EdgeHeap::
pop()
{
  if(print)
    cerr << "popping" << endl;
  if(unusedPos_ == 0) return NULL;
  Edge* retVal(array[0]);
  assert(retVal->heapPos() == 0);
  del_(0);
  retVal->heapPos() = -1;
  return retVal;
}

void
EdgeHeap::
downHeap(int pos)
{
  if(print) cerr << "downHeap " << pos << endl;
  if(pos >= unusedPos_-1) return;
  assert(pos < HeapSize);
  Edge* par = array[pos];
  assert(par->heapPos() == pos);
  double merit = par->merit();
  int lc = left_child(pos);
  int rc = right_child(pos);
  int largec;
  int lcthere = 0;
  Edge* lct;
  if(lc < unusedPos_)
    {
      assert(lc < HeapSize);
      lct = array[lc];
      if(lct)
	{ lcthere = 1;
	  assert(lct->heapPos() == lc);
	}
    }
  int rcthere = 0;
  Edge* rct;
  if(rc < unusedPos_)
    {
      rct = array[rc];
      if(rct)
	{
	  rcthere = 1;
	  assert(rct->heapPos() == rc);
	}
    }
  if(!lcthere && !rcthere) return;
  assert(lcthere);
  if(!rcthere || (lct->merit() > rct->merit()))
    largec = lc;
  else largec = rc;
  Edge* largeEdg = array[largec];
  if(merit >= largeEdg->merit())
    {
      if(print) cerr << "downheap of " << merit << " stopped by "
		     << *largeEdg << " " << largeEdg->merit() << endl;
      return;
    }
  array[pos] = largeEdg;
  largeEdg->heapPos() = pos;
  array[largec] = par;
  par->heapPos() = largec;
  downHeap(largec);
}

void
EdgeHeap::
del(Edge* edge)
{
  if(print)
    cerr << "del " << edge << endl;
  int pos = edge->heapPos();
  assert( pos < unusedPos_ && pos >= 0);
  del_( pos );
}

void
EdgeHeap::
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
  array[pos]->heapPos() = pos;
  array[unusedPos_ -1] = NULL;
  unusedPos_--;
  if(upheap(pos)) return;
  downHeap(pos);
}
/*
void
EdgeHeap::
check()
{
  if(size() > 0) array[0]->check();
  for(int i = 1 ; i < unusedPos_ ; i++)
    {
      assert(array[i]);
      array[i]->check();
      if(!(array[parent(i)]->merit() >= array[i]->merit()))
	{
	 cerr << "For i = " << i <<  " parent_i = "
	   << parent(i) << " "
	   << *(array[parent(i)])
	   << " at " << array[parent(i)]->merit() 
	   << " not higher than " << *(array[i])
	   << " at " << array[i]->merit() 
	     << endl;
	 assert(array[parent(i)]->merit() >= array[i]->merit());
       }
    }
}
*/
