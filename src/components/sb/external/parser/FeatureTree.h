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

#ifndef FEATURETREE_H
#define FEATURETREE_H

#include <assert.h>
#include <fstream>
#include <iostream>
#include "Feat.h"
#include "FBinaryArray.h"

class FeatureTree;

#define ROOTIND -99
#define AUXIND -9
#define NULLIND 9999999
class FeatureTree
{
public:
  FeatureTree() :auxNd(NULL), back(NULL),
    ind_(NULLIND), count(0) {}
  FeatureTree(int i) : ind_(i), auxNd(NULL), back(NULL),
    count(0){}
  FeatureTree(int i, FeatureTree* b)
    : ind_(i), auxNd(NULL), back(b) {}
  FeatureTree(istream& is);
  //FeatureTree(istream& is, istream& idxs, int asVal);
  void read(istream& is, FTypeTree* ftt);
  int  readOneLevel0(istream& is, int c);
  FeatureTree* follow(int val, int auxCnt);
  static FeatureTree* roots(int which) { return roots_[which]; }
  void         printFfCounts(int asVal, int depth, ostream& os);
  friend ostream&  operator<<(ostream& os, const FeatureTree& ft);

  int ind() const { return ind_; }
  int ind_;
  int count;
  //int specFeatures;
  FeatureTree* back;
  FeatureTree* auxNd;
  FBinaryArray feats;
  FTreeBinaryArray subtree;
 private:
  static FeatureTree* roots_[20];
  void othReadFeatureTree(istream& is, FTypeTree* ftt, int cnt);
  void printFfCounts2(int asVal, int depth, ostream& os);
};

#endif /* ! FEATURETREE_H */
