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

#ifndef ITEM_H 
#define ITEM_H

#include "Wrd.h"
#include "Edge.h"
#include <map>
#include <set>
#include "AnswerTree.h"
#include "CntxArray.h"
#include "Bst.h"

class Term;
class Word;

typedef set<Edge*, less<Edge*> > EdgeSet;
typedef EdgeSet::iterator EdgeSetIter;
typedef pair<EdgeSet,BstMap> ItmGHeadInfo;
typedef map<Wrd, ItmGHeadInfo, less<Wrd> > HeadMap;
typedef map<int,HeadMap, less<int> > PosMap;
typedef HeadMap::iterator HeadIter;
typedef PosMap::iterator PosIter;

class           Item
{
public:
    Item( //const Wrd* hd,
	 const Term * _term, int _start, int _finish );
    Item() {}
    Item( const Item& );
    ~Item();
    int		    operator== (const Item& item) const;
    friend ostream& operator<< ( ostream& os, const Item& item );
    const Term *    term() const { return term_; }
    const Wrd*     word() const { return word_; }
    const Wrd*&    word() { return word_; }
    int             start() const {return start_;}
    int&            start() {return start_;}
    int             finish() const {return  finish_;}
    int&            finish() {return  finish_;}
    list<Edge*>&  needme() {return needme_;}
    list<Edge*>&  ineed() {return ineed_;}
    void            check();
    double            prob() const {return prob_;}
    double            poutside() const {return poutside_;}
    /* storeP can be used as beta for rParse */
    double            beta() const {return storeP_;}
    double&           beta() {return storeP_;}
    double            storeP() const {return storeP_;}
    double &          prob() {return prob_;}
    double &          poutside() {return poutside_;}
    double &          storeP() {return storeP_;}
    Bst&              stored(CntxArray& ca) { return bstFind(ca, stored_); }
    PosMap&          posAndheads() { return posAndheads_; }
    void            set(const Term * _term, int _start);
    void	    operator= (const Item& itm);
 private:
    int             start_;
    int             finish_;
    const Term *    term_;
    const Wrd *    word_;
    list<Edge*>  needme_;	/* A list of rules requiring a term starting
				 * at start */

    list<Edge*>  ineed_;	// needme = rules predicted by this (art) item
				// ineed = rules that predict this (art) item
    double           prob_;
    double           poutside_;
    double           storeP_;	
    BstMap           stored_;
    PosMap           posAndheads_;
};

typedef list<Item*> Items;
typedef Item *	Item_star;

#endif /* !ITEM_H */
