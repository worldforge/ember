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

#ifndef EDGE_H
#define EDGE_H

#include "Term.h"
#include "utils.h"

class Item;

class Edge
{
public:
    friend class GotIter;
    friend class SuccessorIter;
    friend class LeftRightGotIter;
    Edge(ConstTerm* trm);
    Edge( Edge& edge, Item& itm, int right );
    Edge( Item& itm );

    Edge( const Edge& src ) { error("edge copying no longer exists"); }

    Edge() : num_(-1) {}
    ~Edge();
    bool            check(); 

    int 	    operator== (const Edge& rhs) { return this == &rhs; }
    bool	    finished() const;
    ConstTerm *	    lhs() const { return lhs_; }
    int 	    heapPos() const {   return heapPos_;   }
    int&	    heapPos() {   return heapPos_;   }
    short	    start() const {   return start_;   }
    short&	    start() {   return start_;   }
    list<Edge*>	    sucs() const { return sucs_; }
    list<Edge*>&    sucs() { return sucs_; }
    short	    loc() const {   return loc_;   }
    short&	    loc() {   return loc_;   }
    Item*           item() const { return item_; }
    Edge*           pred() const { return pred_; }
    float&          prob() { return prob_; }
    float           prob() const { return prob_; }

    int             headPos(int i = 0);
    Item*           headItem();

    /* only used in rParse, when merits are not used */
    float           beta() const { return leftMerit_; }
    float&          beta() { return leftMerit_; }

    float           leftMerit() const { return leftMerit_; }
    float&          leftMerit() { return leftMerit_; }
    float           rightMerit() const { return rightMerit_; }
    float&          rightMerit() { return rightMerit_; }
    int             demerits() const { return demerits_; }
    int&            demerits() { return demerits_; }
    short           num() const { return num_; }
    short&          num() { return num_; }

    float	    merit() { return merit_; }
    void	    setmerit();
    short           status() const { return status_; }
    short&          status() { return status_; }
    void	    print( ostream& os );
    friend ostream& operator<< (ostream& os, Edge& edge )
		    { edge.print( os ); return os;}
    void            setFinishedParent( Item* par )
                      { finishedParent_ = par ; }
    Item           *finishedParent() { return finishedParent_; }
    int            ccInd();
    static int      numEdges;
    static float    DemFac;

private:
    short           num_;
    short           status_; 
    short           start_;
    short           loc_;
    ConstTerm 	   *lhs_;
    Item           *finishedParent_;
    Item           *item_;
    Edge*           pred_;
    int             heapPos_;
    int             demerits_;

    float          leftMerit_;
    float          rightMerit_;
    float          prob_;
    float          merit_;
    list<Edge*> sucs_;
};

typedef list<Edge*> Edges;

#endif	/* ! EDGE_H */
