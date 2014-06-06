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
#include "Item.h"	// need Item, Items definitions
#include "GotIter.h"
#include "Bchart.h"
#include <math.h>
#include "Feature.h"

#define DEMFAC .999
#define EDGE_CHUNKSIZE		1000
//int Edge::numEdges = 0;

//Edge *	Edge::edge_cache_;


Edge::
~Edge()
{   
  //numEdges--;
}

bool
Edge::
check()
{
  GotIter gi(this); 
  Item* f;
  while(gi.next(f))
    assert(f->finish() == 0 || f->finish() > f->start());
  return true;
}

int
Edge::
ccInd()
{
  const Term* trm = lhs();
  int tint = trm->toInt();
  ECString tNm = trm->name();
  bool sawComma = false;
  bool sawColen = false;
  bool sawCC = false;
  int numTrm = 0;
  Item* itm;
  LeftRightGotIter gi(this);  
  int pos = 0;
  /*Change next line to indicate which non-terminals get specially
    marked to indicate that they are conjoined together */
  if(!trm->isNP() && !trm->isS() && !trm->isVP()) return tint;
  while( gi.next(itm) )
    {
      const Term* subtrm = itm->term();
      if(subtrm == Term::stopTerm) continue;
      if(subtrm == trm) numTrm++;

      if(pos != 0 && subtrm->isCC()) sawCC = true;
      if(pos != 0 && subtrm->isComma()) sawComma = true;
      if(pos != 0 && subtrm->isColon()) sawColen = true;
      
      pos++;
    }
  if(trm->isNP() && numTrm == 2 && !sawCC) return Term::lastNTInt()+1;
  if((sawComma || sawColen || sawCC) && numTrm >= 2)
    return tint+Term::lastNTInt();
  return tint;
}


Edge::
Edge(ConstTerm* trm)
  : lhs_( trm ), loc_( -1 ), num_(-1),
  pred_(NULL), finishedParent_( NULL ), item_(NULL),
  status_(0), start_(-1),
  heapPos_(-1),
  demerits_(0),
  prob_(1.2f) // encourage constits???
{
  if(lhs_->isVP()) prob_ *= 1.4f; //???;
  // VPs are badly modeled by system;  This is only called during first parse.
}

Edge::
Edge( Edge& src, Item& itm, int right )
: lhs_( src.lhs_ ), loc_( src.loc_ ),  start_(src.start_),
  finishedParent_( NULL ), num_(-1),
  status_(right),
  item_(&itm),
  heapPos_(-1),
  demerits_(src.demerits_),
  leftMerit_(src.leftMerit()),
  rightMerit_(src.rightMerit()),
  prob_(src.prob())
{
  //numEdges++;
  if(start_ == -1)
    start_ = itm.start();
  if(loc_ == -1) loc_ = itm.finish();
  if(right) loc_ = itm.finish();
  else start_ = itm.start();

  if(!src.item_) //it has no item
    {
      pred_ = NULL;
    }
  else
    {
      pred_ = &src;
      pred_->sucs_.push_front(this);
      //cerr << *pred_ << " has suc " << *this << endl;
    }
  prob_ *= (float)itm.prob();
}

Edge::
Edge( Item& itm )
: lhs_( itm.term() ), loc_( itm.finish() ),  finishedParent_( &itm ),
  status_(2), pred_(NULL), start_(itm.start()),
  item_(NULL),
  heapPos_(-1), num_(-1),
  demerits_(0),
  prob_( (float)itm.prob() ),
  leftMerit_(1),
  rightMerit_(1)
{
  //numEdges++;
}

void 
Edge::
print( ostream& os )
{
    if(!item_) //dummy rule
      {
	if(finishedParent_)
	  {
	    os << *finishedParent_ << " -> ";
	  }
	else
	  {
	    os << *lhs_ << " -> ";
	  }
      }
    else 
    {
        os << *lhs_ << "(" << start() << ", " << loc_ << ") -> ";
	LeftRightGotIter gi(this);
	//const Term * tempRHS;
	Item* itm;
	while( gi.next(itm) )
	  {
	    if(itm->term() == Term::stopTerm) continue;
	    os << *itm << " ";
	  }
    }
}

void
Edge::
setmerit()
{
  merit_ = (float)( prob_*leftMerit_*rightMerit_*pow(DEMFAC,demerits_) );
}

Item*
Edge::
headItem()
{
  GotIter gotiter(this);
  Item* ans;
  Item* next;
  while(gotiter.next(next))  //the head will be the the last thing in gotiter;
    ans = next;
  return ans;  
}

int
Edge::
headPos(int i /*=0*/)
{
  if(!pred()) return i;
  Edge* prd = pred();
  //cerr << "H " << *item() << endl;
  if(prd->start() > start()) return prd->headPos(i+1);
  else if(prd->start() == start()
	  && item()->term() == Term::stopTerm
	  && item()->start() == start())
    {
      //cerr << "HPST " << *(prd->item()) << " " << i << endl;
      return prd->headPos(i+1);
    }
  else return pred()->headPos(i);
}
