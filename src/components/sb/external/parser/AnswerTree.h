
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

#ifndef ANSWERTREE_H
#define ANSWERTREE_H

#include "Edge.h"
#include <vector>
#include <list>
#include "CntxArray.h"

#define NTH 50
class AnsTree;

typedef list<AnsTree*> AnsTrees;
typedef AnsTrees::iterator AnsTreeIter;
typedef pair<double, AnsTree> AnsTreePair;

class
AnsTree
{
 public:
  AnsTree() : trm(-1), wrd(-1) {};
  void operator=(const AnsTree& rhs)
    {
      trm = rhs.trm;
      wrd = rhs.wrd;
      subTrees = rhs.subTrees;
    }
  short trm;
  int wrd;
  AnsTrees subTrees;
};

class AnsTreeStr
{
public:
  AnsTreeStr() : sum(0)
    {
      int i;
      numCreated++;
      for(i = 0 ; i < NTH ; i++)
	{
	  probs[i] = -1;
	}
    }
  double sum;
  double probs[NTH];
  AnsTree  trees[NTH];
  static int numCreated;
};

typedef map<CntxArray, AnsTreeStr, less<CntxArray> > AnsTreeMap;
AnsTreeStr& atpFind(CntxArray& hi, AnsTreeMap& atm);

#endif
