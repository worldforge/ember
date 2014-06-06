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

#ifndef COMBINEBESTS_H
#define COMBINEBESTS_H

#include "AnswerTree.h"
#include "Term.h"
#include "AnsHeap.h"

typedef map<double,AnsTree> AnsMap;

class AnsTreeQueue
{
 public:
  AnsTreeQueue() : size(0), worstPos(-1), worstP(-1) 
    {
      for(int i = 0 ; i < NTH ; i++) ansList[i].first = -1;
    }
  void refresh()
    {
      for(int i = 0 ; i < size ; i++)
	{
	  ansList[i].first = -1;
	  ansList[i].second.subTrees.clear();
	}
      size = 0;
      worstPos = -1;
      worstP = -1;
    }
  AnsTreePair& pop();
  AnsTreePair& index(int i) { return ansList[i]; }
  void push(double prob, AnsTree& tree);
  int size;
  int worstPos;
  double worstP;
  AnsTreePair ansList[NTH];
};

class CombineBests
{
 public:
  void setBests(AnsTreeStr& atp);
  void addTo(AnsTreeStr& atp, double prob);
  AnsTreeQueue atq;
};

class CombineBestsT
{
 public:
  CombineBestsT(const Term* tm, double rprb);
  void extendTrees(AnsTreeStr& ats, int dir);
  double rprob;
  const Term* trm;
  AnsTreeQueue atq0;
  AnsTreeQueue atq1;
  bool whichIsCur;
  AnsTreeQueue& curAtq() { return whichIsCur ? atq1 : atq0; }
  AnsTreeQueue& pastAtq() { return whichIsCur ? atq0 : atq1; }
  void switchQueues()
    {
      whichIsCur = !whichIsCur;
      //cerr << "WIC " << curAtq().size << " " << pastAtq().size << endl;
      curAtq().refresh();
    }
};

class CombineBestsGh
{
 public:
  void setBests(AnsTreeStr& atp);
  void addTo(CombineBestsT& cbt);
  AnsTreeQueue atq;
};

#endif /* ! COMBINEBESTS_H */
