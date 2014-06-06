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

#ifndef GOTITER_H
#define GOTITER_H

#include "Edge.h"
#include "Item.h"

class           GotIter
{
 public:
  GotIter(Edge* edge);
  bool     next(Item*& itm);
 private:
  Edge*        whereIam;
};

class           LeftRightGotIter
{
 public:
  LeftRightGotIter(Edge* edge);
  bool    next(Item*& itm);
  Item*   index(int i) const { assert(i < 400); return lrarray[i]; }
  int     size() const { return size_; }
  int&    pos() { return pos_; }
 private:
  void         makelrgi(Edge* edge);
  Item*        lrarray[400];
  int          pos_;
  int          size_;
};

class           MiddleOutGotIter
{
 public:
  MiddleOutGotIter(Edge* edge);
  bool    next(Item*& itm, int& pos);
  int     size() const { return size_; }
  int     dir() { return dir_; }
 private:
  void         makelrgi(Edge* edge);
  Item*        lrarray[64];
  int          pos_;
  int          size_;
  int          dir_;
  Item*        firstRight_;
};

class           SuccessorIter
{
 public:
  SuccessorIter(Edge* edge) : edge_(edge), edgeIter( edge->sucs().begin() ) {}
  bool    next(Edge*& itm);
 private:
  Edge*  edge_;
  list<Edge*>::iterator edgeIter;
};
    
class          NeedmeIter
{
 public:
  NeedmeIter(Item* itm);
  bool    next(Edge*& e);
  void resizeStack();

 private:
	int curStackSize;	
	int stackIncreaseSize;
	int stackptr;
	typedef	Edge* EdgePtr;
	EdgePtr*	stack;
  //Edge*        stack[64000]; //??? was 32;
};
  
#endif	/* ! GOTITER_H */
