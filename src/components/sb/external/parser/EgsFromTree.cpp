#include "Bchart.h"
#include "headFinder.h"
#include "ECString.h"
#include "Feature.h"
#include "GotIter.h"
#include "math.h"

extern LeftRightGotIter* globalGi;

void
Bchart::
rPendFactor()
{
    endPos = wrd_count_;
    const char* endwrd = NULL;
    if(wrd_count_ > 0) endwrd = sentence_[wrd_count_-1].lexeme().c_str();
    if(endwrd  && finalPunc(endwrd)) endPos = wrd_count_-1;
    else if(wrd_count_ > 2)
      {
	endwrd = sentence_[wrd_count_-2].lexeme().c_str();
	if(finalPunc(endwrd)) endPos = wrd_count_-2;
	else
	  {
	    endwrd = sentence_[wrd_count_-3].lexeme().c_str();
	    if(finalPunc(endwrd)) endPos = wrd_count_-3;
	  }
      }
    cerr << "GG" << endl;
}

Item*
Bchart::
edgesFromTree(InputTree* tree)
{
  int b0;
  b0 = tree->num();
  const Term* trm = Term::get(tree->term());
  assert(trm);
  //cerr << "ARI " << *trm << " " << b0 << endl;
  if(printDebug() > 1005)
    cerr << "EFIE " << trm->name() << " " << b0 << endl;
  /* If this is a terminal node, the rhs will be a word; otherwise it
     will be a rule expansion consisting of several Item s.
   */
  if(trm->terminal_p())
    {
      ECString tmpW1 = tree->word();
      char chars[512];
      ECString tmpW = toLower(tmpW1.c_str(), chars);
      
      int wInt = wtoInt(tmpW);
      Item* lhs = add_item(b0, trm, tree->start());
      lhs->start() = tree->start();
      lhs->finish() = tree->finish();
      Item* rhs = add_item2(b0, trm, wInt,tmpW);
      rhs->finish() = tree->finish();
      rhs->start() = tree->start();
      if(!lhs && !rhs)
	{
	  return NULL;
	}

      Items subItems;
      subItems.push_back(stops[tree->start()]);
      subItems.push_back(rhs);
      subItems.push_back(stops[tree->finish()]);
      Edge* edg = add_edge(lhs, subItems);
      if(!edg)
	{
	  return NULL;
	}
      edg->prob() = pHst(wInt,trm->toInt());
      edg->num() = b0;
      if(printDebug() > 5)
	cerr << "LHS " << *lhs << " " << tmpW  << edg->prob() << endl;
	  
      return lhs;
    }
  else
    {
      Item* lhs = add_item(b0, trm, -1);
      lhs->start() = tree->start();
      lhs->finish() = tree->finish();
      assert(lhs);
      Items subItems;
      subItems.push_back(stops[tree->start()]);
      InputTreesIter iti = tree->subTrees().begin();
      for( ; iti != tree->subTrees().end() ; iti++)
	{
	  InputTree* stree = (*iti);
	  cerr << "WBA "<< stree->term() << *stree   << endl;
	  Item* itm = edgesFromTree(stree);
	  if(!itm)
	    {
	      return NULL;
	    }
	  subItems.push_back(itm);
	}
      subItems.push_back(stops[tree->finish()]);
      Edge* edg = add_edge(lhs, subItems);
      if(!edg)
	{
	  return NULL;
	}
      edg->num() = b0;
      assignRProb(edg);
      if (printDebug() > 5)
	{
	  cerr << "Saw edge " << *edg << ": p=" << edg->prob() << endl;
	}
      //cerr << "endeFE " << *edg << endl;
      return lhs;
      rPendFactor();
    }
  return NULL;
}

Item*
Bchart::
in_chart(int b, const Term* trm, bool& wasThere)
{
  Item* itm;
  itm = addtochart(trm);
  wasThere = false;
  return itm;
}

Item*
Bchart::
add_item(int b, const Term* trm, int wInt )
{
  //cerr << "AI " << b << " " << *trm << " " << wInt << endl;
  Item* ans = NULL;
  Wrd* wrd = NULL;
  bool wasthere;
  if(!trm->terminal_p()) return in_chart(b,trm,wasthere);
  if(wInt < 0) return in_chart(b,trm, wasthere);
  ans = addtochart(trm);
  newWord(intToW(wInt), wInt,ans);
  return ans;
}

void
Bchart::
newWord(ECString wrdstr, int wInt, Item* ans)
{
  int len = sentence_.length();
  Wrd* wrd = &sentence_[len];
  sentence_.length()++;
  wrd_count_++;
  wrd->lexeme() =wrdstr;
  wrd->toInt() = wInt;
  ans->word() = wrd;
}

Item*
Bchart::
add_item2(int b, const Term* trm, int wInt, ECString wrdstr)
{
  //cerr << "AI " << b << " " << *trm << " " << wInt << endl;
  Item* ans = NULL;
  Wrd* wrd = NULL;
  //bool wasthere;
  ans = addtochart(trm);
  newWord(wrdstr, wInt, ans);
  return ans;
}

Edge*
Bchart::
add_edge(Item* lhs, Items& rhs)
{
  int headPos = headPosFromItems(lhs, rhs);
  //cerr << "In add_edge " << *lhs << " "
  //   << rhs.size() << " " << headPos << endl;

  Items::iterator ii = rhs.begin();
  int pos = 0;
  for( ; ii != rhs.end() ; ii++)
    {
      if(pos == headPos) break;
      pos++;
    }
  Items::iterator ii2 = ii;
  ii2++;
  Edge  dummyedge(lhs->term());
  dummyedge.start() = lhs->start();
  dummyedge.loc() = lhs->finish();
  Edge* prevEdge = &dummyedge;
  for( ; pos >= 0 ; pos--)
    {
      Item* item = (*ii);
      //cerr << "bne " << *item << endl;
      Edge*   newEdge = new Edge(*prevEdge, *item, 0);
      prevEdge  = newEdge;
      cerr << "ae1 " << *item << " " << *newEdge << endl;
      alreadyPoped[alreadyPopedNum++] = newEdge;  //so it will be gced.;
      if(item->term() != Term::stopTerm) item->needme().push_back(newEdge);
      if(pos > 0) ii--;
    }
  for( ; ii2 != rhs.end() ; ii2++)
    {
      Item* item = (*ii2);
      Edge*   newEdge = new Edge(*prevEdge, *item, 1);
      prevEdge  = newEdge;
      cerr << "ae2 " << *item << " " << *newEdge << endl;
      alreadyPoped[alreadyPopedNum++] = newEdge;  //so it will be gced.;
      if(item->term() != Term::stopTerm) item->needme().push_back(newEdge);
    }
  prevEdge->setFinishedParent( lhs );    
  /* Since the chart itself only stores Items, the Edges in the chart are
     stored in the 'ineed' variable in the corresponding Item.
   */
  lhs->ineed().push_back(prevEdge);
  return prevEdge;
}

int
Bchart::
headPosFromItems(Item* lhs, Items& rhs)
{
  int   ansPriority = 9999;
  ECString lhsString(lhs->term()->name());
  if(lhsString == "") lhsString = "S1";
  int   pos = -1;
  int   ans = -1;

  Items::iterator ii = rhs.begin();
  Item   *subi;
  for( ; ii != rhs.end() ; ii++ )
    {
      subi = (*ii);
      pos++;
      const Term* trm = subi->term();
      int nextPriority = 12;
      if(trm)
	{
	  ECString rhsString(subi->term()->name());
	  if(subi->term() == Term::stopTerm) continue;
	  nextPriority = headPriority(lhsString, rhsString, ansPriority);
	}
      if(nextPriority <= ansPriority)
	{
	  ans = pos;
	  ansPriority = nextPriority;
	}
    }
  return ans;
}

void
Bchart::
assignRProb(Edge* edge)
{
  LeftRightGotIter lrgi(edge);
  globalGi = &lrgi;
  int sz = lrgi.size();
  int i;
  int hp = edge->headPos();
  double ans = 1;
  cerr << "ARP " << *edge << endl;
  for(i = 0 ; i < sz ; i++)
    {
      lrgi.pos() = i;
      int whichInt = i < hp ? LCALC : i == hp ? MCALC : RCALC;
      FullHist fh(edge);
      fh.pos = i;
      double p = meFHProb(lrgi.index(i)->term(),fh, whichInt);
      //cerr << "p" << whichInt << "(" << *lrgi.index(i) << ")= " << p << endl;
      ans *= p;
    }
  //if(ans == 0) cerr << "Assigned zero prob to " << *edge << endl;
  edge->prob() = (float)ans;
}


void
Bchart::
propagateItemBeta(Item* itm, double quant)
{
  double obeta = itm->beta();
  double nbeta = obeta+quant;
  itm->beta() = nbeta;
  if(printDebug() > 70)
    cerr << "IS " << obeta << " " << nbeta << " " << *itm << " "
	 << (int)itm->needme().size() << endl;
  if(obeta* 1.1 > nbeta) return;
  list<Edge*>::iterator  nmi = itm->needme().begin();
  for( ; nmi != itm->needme().end() ; nmi++)
    {
      //cerr << "BCE " << **nmi << endl;
      computeEdgeBeta(itm, *nmi);
    }
}
  
  
void
Bchart::
computeEdgeBeta(Item* itm, Edge* edge)
{
  double obeta = edge->beta();
  //int i;
  LeftRightGotIter lr(edge);
  double ebeta = 1;
  Item* sitm;
  for( ; lr.next(sitm) ; )
    {
      if(sitm->term() == Term::stopTerm) continue;
      double bta = sitm->beta();
      //cerr << "CEFB " << bta << " " << *sitm << endl;
      if(bta == 0)
	{
	  if(printDebug() > 78) cerr << "COB " << *sitm << endl;
	  return;
	}
      ebeta *= bta;
    }
  edge->beta() += (float)( ebeta* edge->prob() );
  if(printDebug() > 70)
    {
      cerr << "SetEB " << edge->beta() << " " << *edge << endl;
      cerr << ebeta << " " << edge->prob() << endl;
    }
  propagateItemBeta(edge->finishedParent(), edge->beta()-obeta);
}

void
Bchart::
set_Betas()
{
  vector<Item*>::iterator ii = itemsToDelete.begin();
  for( ; ii != itemsToDelete.end() ; ii++)
    {
      Item* itm = (*ii);
      if(!itm->word()) continue;
      if(printDebug() > 30) cerr << "SB " << *itm << endl;
      assert(itm);
      if(itm->start() < 0) continue;
      propagateItemBeta(itm,1);
    }
}

