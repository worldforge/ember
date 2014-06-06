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

#include "Item.h"
#include <iostream>

void
Item::
set(const Term * _term, int _start)
{
  term_ = _term;
  start_ = _start;
  needme_.clear();
  ineed_.clear();
  word_ = NULL;
  storeP_ = 0.0;
  stored_.clear();
  posAndheads_.clear();
}

void
Item::
operator=(const Item& itm) 
{
  term_ = itm.term();
  start_ = itm.start();
  finish_ = itm.finish();
  word_ = itm.word();
}


Item::
Item(const Item& itm) :
  term_(itm.term()), start_(itm.start()), finish_(itm.finish()),
  word_(itm.word()) {}

Item::
~Item()
{
  //cerr << "Deleting " << *this << endl;
  /*
  PosIter pi = posAndheads_.begin();
  for( ; pi != posAndheads_.end() ; pi++)
    {
      HeadMap& hm = (*pi).second;
      HeadMap::iterator hmi = hm.begin();
      for( ; hmi != hm.end() ; hmi++)
	{
	  //cerr << "HMI " << (*hmi).first << endl;
	  ItmGHeadInfo& igh = (*hmi).second;
	  BstMap& atm = igh.second;
	  BstMap::iterator atmi = atm.begin();
	  for( ; atmi != atm.end(); atmi++)
	    {
	      Bst& atp= (*atmi).second;
	      //if(atp.trees[0]) delete atp.trees[0];
	    }
	}
    }
  */
}

Item::
Item(
     //const Word* hd,
     const Term * _term, int _start, int _finish)
: //head_ (hd),
  term_( _term ), start_( _start ), finish_( _finish ),
  needme_(), ineed_(),
  prob_( 1.0 ), word_( NULL ), storeP_( 0.0 ), poutside_( 0.0 )
{}

ostream&
operator<< (ostream& os, const Item& item)
{
    os << *item.term() << "(" << item.start();
    os << ", " << item.finish();
    //os << ", " << item.head()->lexeme();
    os << ")";
    return os;
}

int
Item::
operator== (const Item& item) const
{
    return ( //head_ == item.head() &&
	    term_ == item.term()
	    && start_ == item.start()
	    && finish_ == item.finish());
}

void            
Item::
check()
{
  assert(start() < finish() || !finish());
  list<Edge*>::iterator nmIter = needme_.begin();
  list<Edge*>::iterator inIter = ineed_.begin();
  for( ; nmIter != needme_.end() ; nmIter++)
    (*nmIter)->check();
  for( ; inIter != ineed_.end() ; inIter++)
    (*inIter)->check();
}
