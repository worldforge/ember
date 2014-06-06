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

#ifndef MECHART_H
#define MECHART_H

#include "Bchart.h"
#include "FullHist.h"
#include "FeatureTree.h"
#include "Feature.h"
#include "Item.h"
#include "Bst.h"

class MeChart : public Bchart
{
 public:
  MeChart(SentRep & sentence)
     : Bchart( sentence ) {}
  double triGram();
  static void init(ECString path);
  Bst& findMapParse();
  Bst& bestParse(Item* itm, FullHist* h,Val* cat,Val* gcat,int cdir);
  Bst& bestParseGivenHead(int posInt, const Wrd& wd, Item* itm,
				 FullHist* h,ItmGHeadInfo& ighInfo,
				 Val* cat,Val* gcat);
  void  fillInHeads();
  bool  headsFromEdges(Item* itm);
  bool  headsFromEdge(Edge* e);
  Item * headItem(Edge* edge);
  void  getHt(FullHist* h, int* subfVals, int whichInt = SCALC);
  float getpHst(const ECString& hd, int t);
  Bst& recordedBP(Item* itm, FullHist* h);
  Bst& recordedBPGH(Item* itm, BstMap& atm, FullHist* h);
  float meHeadProb(int wInt, FullHist* h);
  float meProb(int val, FullHist* h, int which);
  float meRuleProb(Edge* e, FullHist* h);
  void  getRelFeats(int c, int c2, int which, Feat* relFeat[],
		    FeatureTree* fts[], FullHist* h, int facPos);

  int     ccbucket(float val, float* buckets , int sz);
  static void    initCCArrays(ECString path);
  static void    initccarray(ifstream& is, float lenArray[6][8]);
  float   ccLenProb(Edge* edge, int effend);
};

#endif
