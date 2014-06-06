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


#include "MeChart.h"
#include "fhSubFns.h"
#include "edgeSubFns.h"
#include "GotIter.h"
#include "InputTree.h"
#include "CntxArray.h"
#include "headFinder.h"
#include "Bst.h"

extern bool badHead(Edge* e);

bool sufficiently_likely(Edge* edge);
bool sufficiently_likely(const Item* itm);

LeftRightGotIter* globalGi = NULL;

int depth = 0;
Val* curVal = NULL;
Val* gcurVal = NULL;
int curDir = -1;

void prDp()
{
  for(int i = 0 ; i < depth ; i++)
    cerr << " ";
}

bool
useKn(int i, int whichInt)
{
  if(whichInt == WWCALC) return true;
  else return false;
}
  
void
MeChart::
init(ECString path)
{
  Feat::Usage = PARSE;
  addEdgeSubFeatureFns();
  addSubFeatureFns();

  ECString tmpA[MAXNUMCALCS] = {"r","h","u","m","l","lm","ru","rm","tt",
				"s","t","ww","dummy","dummy","dummy"};

  for(int which = 0 ; which < Feature::numCalcs ; which++)
    {
      ECString tmp = tmpA[which];
      Feature::init(path, tmp); 
      if(tmp == "s" || tmp == "t") continue;
      ECString ftstr(path);
      ftstr += tmp;
      ftstr += ".g";
      ifstream fts(ftstr.c_str());
      if(!fts) cerr << "could not find " << ftstr << endl;
      assert(fts);
      FeatureTree* ft = new FeatureTree(fts); //puts it in root;
      if(tmp == "ww") continue;
      Feature::readLam(which, tmp, path);
    }
  int cntxSzReq = Feature::total[TCALC];
  int scSz = Feature::total[SCALC];
  if(scSz > cntxSzReq) cntxSzReq = scSz;
  //assert(CntxArray::sz == cntxSzReq);
  //assert(CntxArray::sz == (Feature::total[UCALC] -1));
} 

Bst&
MeChart::
findMapParse()
{
  if(printDebug() > 8)
    {
      prDp();
      cerr << "In findMapParse" << endl;
    }
  Item* s = topS();
  assert(s);
  fillInHeads();
  int s1Int = s->term()->toInt();
  FullHist s1Fh(s1Int,NULL);
  Bst& bst = bestParse(s, &s1Fh,NULL,NULL,0);
  return bst;
}

Bst&
MeChart::
bestParse(Item* itm, FullHist* h, Val* cval, Val* gcval, int cdir)
{
  curVal = cval;
  gcurVal = gcval;
  curDir = cdir;
  Bst& bst = recordedBP(itm, h);
  curVal = gcurVal = NULL;
  curDir = -1;
  if(bst.explored())
    {
      if(printDebug() > 19)
	{
	  prDp();
	  cerr << "already known bestParse(" << *itm << ", ...) has p = "
	       << bst.prob() << endl;
	}
      return bst;
    }
  if(printDebug() > 10)
    {
      prDp();
      cerr << "bestParse(" << *itm << ", ...)" << endl;
    }
  bst.explored() = true;  //David McClosky bug;
  int itermInt = itm->term()->toInt();
  PosMap& pm = itm->posAndheads();
  PosIter pi = pm.begin();
  ECString bestW;
  for( ; pi != pm.end() ; pi++ )
    {
      int posInt = (*pi).first;
      if(printDebug() > 16)
	{
	  prDp();
	  cerr << "consider Pos(" << *itm << ") = " << posInt << endl;
	}
      HeadMap& hm = (*pi).second;
      /* we are using collected counts for p(u|t) */
      float hposprob = 1;
      float chposprob = 1;
      /* if we have reached a preterminal, then termInt == posInt
	 and p(posInt|termInt) == 1 */
      if( itermInt != posInt)
	{
          curVal = cval;
          gcurVal = gcval;
          curDir = cdir;
	  hposprob = meProb(posInt, h, UCALC); 
	  if(hposprob == 0) hposprob = .00001f; //??? this can happen;
          curVal = gcurVal = NULL;
          curDir = -1;
	  if(printDebug() > 16)
	    {
	      prDp();
	      cerr <<  "p(pos) = " <<  hposprob << endl;
	    }
	}
      h->preTerm = posInt;
      HeadIter hi = hm.begin(); 
      for( ;hi != hm.end();hi++)
	{
	  const Wrd& subhw = (*hi).first;
	  int wrdInt = subhw.toInt();
	  ECString subh = subhw.lexeme();
	  if(printDebug() > 16)
	    {
	      prDp();
	      cerr << "consider head(" << *itm << ") = " << subh << endl;
	    }
	  float hprob = 0;
	  if(wrdInt >= 0 && wrdInt <= lastKnownWord)
	    {
	      hprob = (float)pCapgt(&subhw,posInt); 
	      hprob *= (1 - pHugt(posInt)); 
              curVal = cval;
              gcurVal = gcval;
              curDir = cdir;
	      float hprob2 = meHeadProb(wrdInt, h);
              curVal = gcurVal = NULL;
              curDir = -1;
	      hprob *= hprob2;
	      if(hprob < 0)
		{
		  cerr << posInt << " " << pHugt(posInt) <<" "<<hprob2 << endl;
		  assert(hprob >=0);
		}
	    }
	  //hprob can be zero if lower case NNPS.
	  if(wrdInt > lastKnownWord || hprob == 0)
	    {
	      hprob = (float)psutt(&subhw,posInt);
	    }
	  if(printDebug() > 16)
	    {
	      prDp();
	      cerr << "p(hd) = "<< hprob << endl;
	    }
	  float hhprob = (hposprob * hprob);
	  if(hhprob < 0)
	    {
	      cerr << hposprob << " " << hprob << endl;
	      assert(hhprob >= 0);
	    }
	  h->hd = &subhw;
	  Bst&  
	    bst2 = bestParseGivenHead(posInt,subhw,itm,h,(*hi).second,cval,gcval);
          if(bst2.empty()) continue;
          Val* nval = new Val();
	  Val* oldval0 = bst2.nth(0);
          nval->prob() = oldval0->prob()*hhprob;
          nval->bsts().push_back(&bst2);
	  nval->status = EXTRAVAL;
          bst.push(nval);
          bst.sum() += bst2.sum()*hhprob;
	}
    }
  Val* nbest = bst.pop();
  if(nbest) bst.addnth(nbest);
  if(printDebug() > 10)
    {
      prDp();
      cerr << "Bestp for " << *itm << " = " << bst.prob() <<endl;
    }
  return bst;
}

Bst&
MeChart::
bestParseGivenHead(int posInt, const Wrd& wd, Item* itm,
		   FullHist* h, ItmGHeadInfo& ighInfo, Val* cval, Val* gcval)
{
  EdgeSet& es = ighInfo.first;
  BstMap&  atm = ighInfo.second;
  //int subfVals[MAXNUMFS];
  curVal = cval;
  gcurVal = gcval;
  Bst& bst = recordedBPGH(itm, atm, h);
  if(bst.explored())
    {
      if(printDebug() > 19)
	{
	  int subfv[MAXNUMFS];
	  getHt(h, subfv);
	  CntxArray ca(subfv);
	  prDp();
	  cerr << "bpknown for " << posInt << ", " << wd
	       << ", " << *itm << ") : " << bst.prob() << " " << ca <<endl;
	}
      curVal=gcurVal=NULL;
      return bst;
    }
  bst.explored() = true;
  curVal=gcurVal=NULL;
  const Term* trm = itm->term();
  if(trm->terminal_p())
    {
      Val* nval = new Val;
      nval->prob() = 1;
      nval->trm1() = itm->term()->toInt();
      nval->wrd1() = itm->word()->toInt();
      nval->status = TERMINALVAL;
      bst.addnth(nval);
      bst.sum() = nval->prob();
      return bst;
    }
  if(printDebug() > 10)
    {
      prDp();
      cerr << "bestParseGivenHead(" << posInt << ", " << wd
	   << ", " << *itm  << ")" << endl;
    }
  double bestP = 0;
  double sumP = 0;
  EdgeSetIter ei = es.begin();
  for( ; ei != es.end() ; ei++)
    {
      Edge* e = *ei;
      if(!sufficiently_likely(e))
	{
	  continue;
	}

      float edgePg = 1;
      int finish = e->loc();

      int effVal = effEnd(finish);
      if(itm->term()->isRoot()) edgePg = 1;
      else if(Feature::isLM) edgePg = 1;   // change by EDF - changed from '==' to '='
      else if(effVal == 1)
	edgePg = endFactor;
      else if(effVal == 0) edgePg = midFactor;
      h->e = e;
      if(printDebug() > 20)
	{
	  prDp();
	  cerr << "consid " << *e << endl;
	}
      
      gcurVal = gcval;
      float prob = meRuleProb(e,h);
      gcurVal=NULL;
 
      int headPos = h->hpos; //set during meRuleProb
      double nextP = prob * edgePg;
      double nextPs = nextP;
      Item* sitm;
      //LeftRightGotIter gi(e); 
      MiddleOutGotIter gi(e);
      Val* val = new Val(e, nextPs);
      int pos = 0;
      depth++;
      h = h->extendByEdge(e);
      bool zeroProb = false;
      while( gi.next(sitm,pos) )
	{
	  //cerr << "Looking at " << *sitm << endl;
	  if(zeroProb)
	    {
	      h = h->extendBySubConstit();
	      continue;
	    }
	  if(sitm->term() == Term::stopTerm)
	    {
	      h = h->extendBySubConstit(); 
	      continue;
	    }
	  if(pos == 0)
	    {
	      h->preTerm = posInt; 
	      h->hd = &wd;
	      ItmGHeadInfo& ighi = sitm->posAndheads()[posInt][wd]; 
	      Bst&
		bst2 = bestParseGivenHead(posInt,wd,sitm,h,ighi,val,cval);
              curVal = gcurVal = NULL;
              curDir = -1;
	      if(bst2.empty())
		{
		  zeroProb = true;
		}
              val->extendTrees(bst2,pos); 
              nextPs *= bst2.sum();

	    }
	  else
	    {
	      Bst& bst2 = bestParse(sitm,h,val,cval,pos);
	      if(bst2.empty())
		{
		  zeroProb = true;
		}
              val->extendTrees(bst2,pos); 
              nextPs *= bst2.sum();
	    }
	  if(printDebug() > 39)
	    {
	      prDp();
	      cerr << "FullHist from " << *h;
	    }
	  h = h->extendBySubConstit(); 
	  if(printDebug() > 39)
	    cerr << " -> " << *h << endl;
	}
      if(!zeroProb) bst.push(val);
      if(printDebug() > 20)
	{
	  prDp();
	  cerr << "P(" << *e << " | " << wd << " ) = " ;
	  cerr << bestP;
	  cerr << "\n"; 
	}
      depth--;
      sumP += nextPs;
      h->retractByEdge(); 
      if(printDebug() > 20)
        {
          prDp();
          cerr << "Val: " << *val << endl;
        }
    }
  Val* vbest = bst.pop();
  if(vbest) bst.addnth(vbest);
  bst.sum() = sumP;
  if(printDebug() > 10)
    {
      prDp();
      cerr << "Bestpgh for "<<*itm << ", " << wd << " = " << bst.prob()<< endl;
    }
  return bst;
}

void
MeChart::
fillInHeads()
{
  for (int j = 0 ; j <  wrd_count_ ; j++)
    {
      // now look at every bucket of length j 
      for (int i = 0 ; i < wrd_count_ - j ; i++)
	{
	  list<Item*>::iterator itmitr =regs[j][i].begin();
	  list<Item*> doover;
	  Item* itm;
	  for( ; itmitr != regs[j][i].end() ; itmitr++)
	    {
	      itm = *itmitr;
	      if(!sufficiently_likely(itm)) continue;
	      const Term* trm = itm->term();
	      int trmInt = trm->toInt();
	      if(trm->terminal_p())
		{
		  HeadMap& hm = itm->posAndheads()[trmInt]; 
		  hm[*itm->word()];
		  continue;
		}
	      else doover.push_back(itm);
	      headsFromEdges(itm);
	    }
	  bool cont = true;
	  int timesAgain = 0;
	  //;while(cont && timesAgain++ < 2)
	  while(cont && timesAgain++ < 4)
	    {
	      cont = false;
	      list<Item*>::iterator lii = doover.begin();
	      for( ; lii != doover.end() ; lii++)
		{
		  bool tmp = headsFromEdges(*lii);
		  if(tmp) cont = tmp;
		}
	      timesAgain++;
	    }
	}
    }
}

bool
MeChart::
headsFromEdges(Item* itm)
{
  bool ans = false;
  list<Edge*>::iterator eli = itm->ineed().begin();
  Edge* e;
  // for each edge we look for all of its possible head preterms, and all
  // of the possible heads, and file this edge for that case 
  for( ; eli != itm->ineed().end() ; eli++)
    {
      e = *eli;
      if(!sufficiently_likely(e)) continue;
      Item* ehd = e->headItem();
      PosIter epi = ehd->posAndheads().begin();
      if(epi == ehd->posAndheads().end()) continue;
      for( ; epi != ehd->posAndheads().end() ; epi++ )
	{
	  int posInt = (*epi).first;
	  
	  if(itm->posAndheads().find(posInt) == itm->posAndheads().end())
	    ans = true;
	  HeadMap& ihm = itm->posAndheads()[posInt];
	  HeadMap& ehm = (*epi).second;
	  HeadIter ehi = ehm.begin();
	  for( ; ehi != ehm.end() ; ehi++ )
	    {
	      const Wrd& hd = (*ehi).first;
	      if(ihm.find(hd) == ihm.end())
		{
		  if(printDebug() > 16)
		    {
		      prDp();
		      cerr << "attach hd " << *itm << " " << hd << endl;
		    }
		  ans = true;
		}
	      EdgeSet& se = ihm[hd].first;
	      se.insert(e);
	    }
	}
    }
  return ans;
}
     
Bst& 
MeChart::
recordedBPGH(Item* itm, BstMap& atm, FullHist* h)
{
  int subfv[MAXNUMFS];
  int i;
  for(i = 0 ; i < MAXNUMFS ; i++) subfv[i] = -1;

  if(!itm->term()->terminal_p())
    {
      getHt(h, subfv);
    }
  CntxArray ca(subfv);
  return bstFind(ca, atm);
}

Bst& 
MeChart::
recordedBP(Item* itm, FullHist* h)
{
  int subfv[MAXNUMFS];
  getHt(h, subfv, TCALC);
  CntxArray ca(subfv);
  return itm->stored(ca); 
}

float
MeChart::
meHeadProb(int wInt, FullHist* h)
{
  float ans = meProb(wInt, h, HCALC);
  return ans;
}

float
MeChart::
meRuleProb(Edge* edge, FullHist* h)
{
  if(printDebug() > 30)
    {
      prDp();
      cerr << "In meruleprob " << *h << " " << *edge
	   << " " << edge->headPos() <<endl;
    }
  int i;
  int hpos = edge->headPos(); 
  h->hpos = hpos;
  LeftRightGotIter gi(edge);
  globalGi = &gi;
  Item* got;
  float ans = 1;
  for(i=0 ;  ; i++ )
    {
      if(i >= gi.size()) break;
      got = gi.index(i);
      h->pos = i;
      int cVal = got->term()->toInt();
      int whichInt = LCALC;
      if(h->pos == hpos) whichInt = MCALC;
      else if(h->pos > hpos) whichInt = RCALC;
      ans *= meProb(cVal, h, whichInt);
      if(ans == 0) break;
    }
  if(printDebug() > 30)
    {
      prDp();
      cerr << "merp = " << ans << endl;
    }
  globalGi = NULL;
  return ans;
}

float
MeChart::
meProb(int cVal, FullHist* h, int whichInt)
{
  if(printDebug() > 68)
    {
      prDp();
      cerr << "meP" << whichInt << "(" << cVal << " | " << *h << ")" <<endl;
    }
  //int subfVals[MAXNUMFS];
  FeatureTree* ginfo[MAXNUMFS];  
  ginfo[0] = FeatureTree::roots(whichInt);
  float smoothedPs[MAXNUMFS];
  Feature::whichInt = whichInt;

  float ans = 1;
 
  for(int i = 1 ; i <= Feature::total[whichInt] ; i++)
    {
      int knp = useKn(i,whichInt);
      ginfo[i] = NULL;
      Feature* feat = Feature::fromInt(i, whichInt); 
      /* e.g., g(rtlu) starts from where g(rtl) left off (after tl)*/
      int searchStartInd = feat->startPos;

      if(i > 1) smoothedPs[i] = smoothedPs[i-1];
      FeatureTree* strt = ginfo[searchStartInd];
      if(!strt)
	{
	  continue;
	}
      SubFeature* sf = SubFeature::fromInt(feat->subFeat, whichInt);
      int nfeatV = (*(sf->fun))(h);
      FeatureTree* histPt = strt->follow(nfeatV, feat->auxCnt); 
      ginfo[i] = histPt;
      if(i == 1)
	{
	  smoothedPs[0] = 1;
	  if(!histPt)
	    {
	      cerr << cVal << " " << whichInt << " " << nfeatV << " " << searchStartInd <<" " << feat->auxCnt << endl;
	      assert(histPt);
	    }
	  Feat* f =histPt->feats.find(cVal);
	  if(!f)
	    {
	      if(printDebug() > 60)
		{
		  prDp();
		  cerr << "Zero p" << feat->name << " " << nfeatV << endl;
		}
	      if(whichInt == HCALC) return 0.001f;
	      return 0.0;
	    }
	  smoothedPs[1] = f->g();
	  if(printDebug() > 68)
	    {
	      prDp();
	      cerr << i << " " << nfeatV << " " << smoothedPs[1] << endl;
	    }
	  for(int j = 2; j <= Feature::total[whichInt] ; j++)
	    smoothedPs[j] = 0;
	  ans = smoothedPs[1];
	  continue;
	}
      if(!histPt)
	{
	  continue;
	}

      int b;
      if(Feature::isLM)
	{
	  /* this section for new bucketing */
	  float sz = (float)histPt->feats.size();
	  float estm = (float)histPt->count / sz;
	  b = bucket(estm, whichInt,i);
	}
      else
	{
	  /* this section for old bucketing */
	  float estm = histPt->count * smoothedPs[1];
	  b = bucket(estm);
	}

      Feat* ft = histPt->feats.find(cVal);
      float unsmoothedVal;
      if(!ft) unsmoothedVal = 0;
      else unsmoothedVal = ft->g();
      float lam = 1;
      if(!knp) lam = Feature::getLambda(whichInt, i, b);
      float uspathprob = lam*unsmoothedVal;

      float osmoothedVal;
      /* First version is for parsing, second for language modeling */
      if(Feature::isLM)
	osmoothedVal = smoothedPs[i-1]; //for deleted interp.
      else osmoothedVal = smoothedPs[searchStartInd];

      float oneMlam = (1-lam);
      if(knp)
        {
          oneMlam = (float)( histPt->count/1000.0 );
        }

      float smpathprob = oneMlam*osmoothedVal;
      float nsmoothedVal = uspathprob+smpathprob;
      smoothedPs[i] = nsmoothedVal;
      ans *= (nsmoothedVal/osmoothedVal);

      if(printDebug() > 68)
	{
	  prDp();
	  cerr << i << " " << nfeatV << " "
	       << b <<" "<<unsmoothedVal << " " << lam << " " 
	       << nsmoothedVal <<  endl;
	}
    }
  if(whichInt == HCALC) ans *= 600;
  if(printDebug() > 30)
    {
      prDp();
      cerr<<"p"<<whichInt<< "(" << cVal << "|" << *h << ") = " << ans << endl;
    }
  return ans;
}

void
MeChart::
getHt(FullHist* h, int* subfVals, int whichTree)
{
  int i;
  for(i = 1 ; i < MAXNUMFS ; i++) subfVals[i] = -1;
  for(i = 1 ; i <= Feature::total[whichTree] ; i++)
    {
      Feature* ft = Feature::fromInt(i, whichTree); 
      int sfInt = ft->subFeat;
      SubFeature* sf = SubFeature::fromInt(sfInt, whichTree);
      int val = (*sf->fun)(h);
      subfVals[sfInt] = val;
    }
  //cerr << "done getHt" << endl;
}
    
bool
sufficiently_likely(const Item* itm)
{
  double pout = itm->poutside();
  double pin = itm->prob();
  double factor = .0008;
  //double factor = .00008; //exp;
  if((pout * pin) > factor) return true;  
  return false;
}

bool
sufficiently_likely(Edge* edge)
{
  Item* fp = edge->finishedParent();
  if(!fp) return false;
  if(!sufficiently_likely(fp)) return false;
  GotIter gotIter(edge);
  Item* got;
  while( gotIter.next(got) )
    {
      if(got->term() == Term::stopTerm) continue;
      if(!sufficiently_likely(got)) return false;
    }
  //return true;
  double factorE = .00001;
  //double factorE = .000001; //exp;
  double pout = fp->poutside();
  double pin = edge->prob();
  if(pout*pin > factorE) return true;
  //cerr << "Edge filtered " << *edge << endl;
  return false;
}

double
MeChart::
triGram()
{
  int i, wInt;
  double ans = 1.0;
  double np;
  FullHist fh;
  for(i = 0 ; i < wrd_count_ ; i++)
    {
      wInt = sentence_[i].toInt();
      if(wInt > lastKnownWord)
	{
	  ans *= .0006; //unknown word prob = .0006/600 = .000001
	  continue;
	}
      fh.pos = i;
      np = meProb(wInt, &fh, WWCALC);
      if(printDebug() > 30)
	cerr << "Wprob " << i << " " << wInt<< " " << np << endl;
      ans *= np;
      ans *= 600;
    }
  fh.pos = wrd_count_;
  ECString tmp("^^");
  wInt = wtoInt(tmp);
  np = meProb(wInt, &fh, WWCALC);
  ans *= np;
  return ans;
}
