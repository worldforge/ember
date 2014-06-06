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

#include <set>
#include "headFinder.h"
#include "headFinderCh.h"
#include "Term.h"
#include "InputTree.h"

set<ECString,less<ECString> > head1s;
set<ECString,less<ECString> > head2s;

void
readHeadInfoEn(ECString& path)
{
  ECString headStrg(path);
  headStrg += "headInfo.txt";
  ifstream headStrm(headStrg.c_str());
  assert(headStrm);

  ECString next, next2;
  headStrm >> next;
  assert(next == "1");
  int whichHeads = 1;
  while(headStrm)
    {
      headStrm >> next;
      if(!headStrm) break;
      if(next == "2")
	{
	  whichHeads = 2;
	  continue;
	}
      
      headStrm >> next2;
      //cerr << "NN " << next << " " << next2 << endl;
      if(!headStrm) error("Bad format for headInfo.txt");
      next += next2;
      if(whichHeads == 1) head1s.insert(next);
      else head2s.insert(next);
    }
}

int
headPriority(ECString lhsString, ECString rhsString, int ansPriority) 
{
  const Term* rhsTerm = Term::get(rhsString);
  if(!rhsTerm) return 11;
  ECString both(lhsString);
  both += rhsString;
  if(lhsString == "PP" && ansPriority == 1) return 10;//make fst IN head of PP
  if(head1s.find(both) != head1s.end()) return 1;
  else if(ansPriority <= 2) return 10;
  else if(rhsString == lhsString)
    return 2; //lhs constit. e.g. np -> NP , np;
  else if(head2s.find(both) != head2s.end()) return 3;
  else if(ansPriority == 3) return 10;
  else if(rhsTerm->terminal_p() && !rhsTerm->isPunc()) return 4;
  else if(ansPriority == 4) return 10;
  else if(!rhsTerm->terminal_p() && rhsTerm->name() != "PP")
    return 5;
  else if(ansPriority == 5) return 10;
  else if(!rhsTerm->terminal_p()) return 6;
  else if(ansPriority == 6) return 10;
  else return 7;
}


int
headPosFromTreeEn(InputTree* tree)
{
  int   ansPriority = 10;
  ECString lhsString(tree->term());
  if(lhsString == "") lhsString = "S1";
  int   pos = -1;
  int   ans = -1;

  ConstInputTreesIter subTreeIter = tree->subTrees().begin();
  InputTree   *subTree;
  for( ; subTreeIter != tree->subTrees().end() ; subTreeIter++ )
    {
      subTree = *subTreeIter;
      pos++;
      ECString rhsString(subTree->term());
      int nextPriority = headPriority(lhsString, rhsString, ansPriority);
      //cerr << "Npri " << nextPriority << lhsString << " " << rhsString
      //   << endl;
      if(nextPriority <= ansPriority)
	{
	  ans = pos;
	  ansPriority = nextPriority;
	}
    }
  return ans;
}

int
headPosFromTree(InputTree* tree)
{
  if(Term::Language == "Ch") return headPosFromTreeCh(tree);
  else return headPosFromTreeEn(tree);
}

void
readHeadInfo(ECString& path)
{
  if(Term::Language == "Ch") return readHeadInfoCh(path);
  else return readHeadInfoEn(path);
}
  
