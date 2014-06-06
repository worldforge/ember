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
 * ACTION OF CONTRHACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "Term.h"
#include "Feature.h"
#include "FullHist.h"
#include "Edge.h"
#include "headFinder.h"
#include "Bchart.h"
#include "MeChart.h"
#include "GotIter.h"
#include "ClassRule.h"

extern LeftRightGotIter* globalGi;
extern Val* curVal;
int nullWordInt;
Val*  tree_ruleTree(FullHist* treeh, int ind);

int
fh_term(FullHist* fh)
{
  return fh->term;
}

int
fh_parent_term(FullHist* fh)
{
  int s1int = Term::rootTerm->toInt();
  FullHist* par = fh->back;
  if(!par) return s1int;
  else return  par->term;
}

int
toBe(const string& parw)
{
  if(parw == "was" || parw == "is" || parw == "be" || parw == "been"
     || parw == "are" || parw == "were" || parw == "being")
    return 1;
  else return 0;
}

int
fh_parent_pos(FullHist* fh)
{
  static int stopint = 0;
  if(!stopint)
    {
      ECString stopnm("STOP");
      stopint = Term::get(stopnm)->toInt();
    }
  FullHist* par = fh->back;
  if(!par) return stopint;
  int ans = par->preTerm;
  if(ans < 2 && toBe(par->hd->lexeme())) return 48;
  return ans;
}
int
fh_term_before(FullHist* fh)
{
  static int stopint = 0;
  if(!stopint)
    {
      ECString stopnm("STOP");
      stopint = Term::get(stopnm)->toInt();
    }
  FullHist* par = fh->back;
  if(!par) return stopint;
  int i = 0;
  for( ; i < par->size ; i++ )
    {
      FullHist* st = par->fharray[i];
      if(st != fh) continue;
      if( i == 0)
        {
          return stopint;
        }
      st = par->fharray[i-1];
      assert(st);
      return st->term;
    }
  error("Should never get here fh_term_b");
  return -1;
}

int
fh_term_after(FullHist* fh)
{
  static int stopint = 0;
  if(!stopint)
    {
      ECString stopnm("STOP");
      stopint = Term::get(stopnm)->toInt();
    }
  FullHist* par = fh->back;
  if(!par) return stopint;
  int i = 0;
  for( ; i < par->size ; i++ )
    {
      FullHist* st = par->fharray[i];
      if(st != fh) continue;
      i++;
      if(i == par->size)
        {
          return stopint;
        }
      st = par->fharray[i];
      assert(st);
      return st->term;
    }
  error("Should never get here fh_term_after");
  return -1;
}

int
fh_pos(FullHist* fh)
{
  return fh->preTerm;
}


int
fh_head(FullHist* tree)
{
  int ans = tree->hd->toInt();
  assert(ans >= -1);
  return ans;
}

int
fh_parent_head(FullHist* tree)
{
  Val* spectree = NULL;
  if(Feature::isLM) spectree = tree_ruleTree(tree, 2);
  if(spectree)
    {
      //cerr << "found  st " << endl;
      return spectree->wrd();
    }
  FullHist* pt = tree->back;
  if(!pt) return nullWordInt;
  int ans = pt->hd->toInt();
  assert(ans >= -1);
  return ans;
}

int
fh_grandparent_head(FullHist* tree)
{
  FullHist* pt = tree->back;
  if(!pt) return nullWordInt;
  pt = pt->back;
  if(!pt) return nullWordInt;
  
  int ans = pt->hd->toInt();
  assert(ans >= -1);
  return ans;
}

int
fh_grandparent_term(FullHist* fh)
{
  int s1int = Term::rootTerm->toInt();
  FullHist* par = fh->back;
  if(!par) return s1int;
  FullHist* gpar = par->back;
  if(!gpar) return s1int;
  else return  gpar->term;
}

int
fh_grandparent_pos(FullHist* fh)
{
  static int stopint = 0;
  if(!stopint)
    {
      ECString stopnm("STOP");
      stopint = Term::get(stopnm)->toInt();
    }
  FullHist* par = fh->back;
  if(!par) return stopint;
  par = par->back;
  if(!par) return stopint;
  return par->preTerm;
}

int
fh_ccparent_term(FullHist* h)
{
  int s1int = Term::rootTerm->toInt();
  FullHist* par = h->back;
  if(!par) return s1int;
  int trmInt = par->term;
  if(trmInt != h->term) return trmInt;
  int ccedtrmInt = par->e->ccInd();
  return ccedtrmInt;
}

int
fh_size(FullHist* fh)
{
  static int bucs[9] = {1, 3, 6, 10, 15, 21, 28, 36, 999};
  int sz = fh->e->loc() - fh->e->start();
  for(int i = 0 ; i < 9 ; i++)
    if(sz <= bucs[i]) return i;
  assert("Never get here");
  return -1;
}

extern MeChart* curChart;

int
fh_effEnd(FullHist* h)
{
  //return 0; //dummy;
  if(h->term == Term::rootTerm->toInt()) return 1;
  FullHist* par = h->back;
  assert(par->e);
  return curChart->effEnd(par->e->loc());
}

int
is_effEnd(FullHist* tree, FullHist* child)
{
  if(!tree) return 1;
  if(Term::fromInt(tree->term)->isRoot()) return 1;
  int loc = 0;
  int sz = tree->size;
  FullHist** fhs = tree->fharray;
  for( ; ; loc++)
    {
      assert(loc < sz);
      FullHist* nxt = fhs[loc];
      assert(nxt);
      if(nxt != child) continue;
      loc++;
      if(loc == sz)
        return is_effEnd(tree->back,tree);
      nxt = fhs[loc];
      ECString ntrmNm = Term::fromInt(nxt->term)->name();
      const Term* ntrm = Term::get(ntrmNm);
      if(ntrm== Term::stopTerm)
	return is_effEnd(tree->back,tree);
      if(ntrm->isColon() || ntrm->isFinal()) return 1;
      if(ntrm->isComma()) return 0;
      loc++;
      if(loc == sz) return 0;
      nxt = fhs[loc];
      ntrmNm = Term::fromInt(nxt->term)->name();
      if(ntrmNm == "''") return 1;
      return 0;
    }
  error("should not get here");
  return 0;
}


int
fh_vE(FullHist* treeh)
{
  int v = fh_parent_pos(treeh);
  int e = is_effEnd(treeh->back, treeh);
  //cerr << e << " " << *treeh->e << endl;
  return v+(e*MAXNUMNTS);
}

int
fh_mE(FullHist* treeh)
{
  int m = fh_grandparent_term(treeh);
  int e = is_effEnd(treeh->back, treeh);
  //if(treeh->e) cerr << "Me " << e << " " << treeh->term << endl;
  return m+(e*50);
}

int
fh_true(FullHist* h) {return 1;}


int
fh_ngram(FullHist* fh, int n, int l)
{
  //cerr << "fhng " << n << " " << l << " "
    //   << fh->pos << " " << *fh->e << endl;
  static int stopTermInt = -1;
  if(stopTermInt < 0)
    stopTermInt = Term::stopTerm->toInt();

  int pos = fh->pos;
  int hpos = fh->hpos; //???;
  int m = pos + (n * l);
  if(m < 0) return stopTermInt;
  if(m > hpos && l > 0)
    {
      return stopTermInt;
    }
  LeftRightGotIter* lrgi = globalGi;
  assert(lrgi);
  if(m >= lrgi->size()) return stopTermInt;
  Item* got = lrgi->index(m);
  assert(got);
  int ans = got->term()->toInt();
  return ans;
}

int
fh_left0(FullHist* fhh)
{
  return fh_ngram(fhh, 0, 0);
}

int
fh_left1(FullHist* fhh)
{
  return fh_ngram(fhh, 1, 1);
}

int
fh_left2(FullHist* fhh)
{
  return fh_ngram(fhh, 2, 1);
}

int
fh_left3(FullHist* fhh)
{
  return fh_ngram(fhh, 3, 1);
}

int
fh_right1(FullHist* fhh)
{
  return fh_ngram(fhh, 1, -1);
}

int
fh_right2(FullHist* fhh)
{
  return fh_ngram(fhh, 2, -1);
}

int
fh_right3(FullHist* fhh)
{
  return fh_ngram(fhh, 3, -1);
}

int
fh_noopenQr(FullHist* fh)
{
  Edge* edge = fh->e;
  int pos = fh->pos;
  LeftRightGotIter*  lrgi = globalGi;
  Item* got;
  int i;
  bool sawOpen = false;
  for(i = 0 ; i < lrgi->size() ; i++)
    {
      if(i == pos) break;
      got = lrgi->index(i);
      const Term* trm = got->term();
      if(trm->isOpen()) sawOpen = true;
      else if(trm->isClosed()) sawOpen = false;
    }
  if(sawOpen) return 0;
  else return 1;
}

int
fh_noopenQl(FullHist* fh)
{
  Edge* edge = fh->e;
  int pos = fh->pos;
  int hpos = fh->hpos;
  LeftRightGotIter*  lrgi = globalGi;
  Item* got;
  int i;
  bool sawOpen = false;
  
  for(i = hpos ; i >= 0 ; i--)
    {
      if(i == pos) break;
      //if(i <= (pos+3)) break; //??? +3 because we already know about next 3;
      got = lrgi->index(i);
      const Term* trm = got->term();
      if(trm->isClosed()) sawOpen = true;
      else if(trm->isOpen()) sawOpen = false;
    }
  if(sawOpen) return 0;
  else return 1;
}

int
fh_Bl(FullHist* treeh)
{
  error("fh_Bl should never be called");
  return -1;
}

int
fh_Br(FullHist* treeh)
{
  error("fh_Br should never be called");
  return -1;
}

Val*
tree_ruleTree(FullHist* treeh,int ind)
{
  bool isM = false;
  Val* secTree = NULL;
  CRuleBundle& crules = ClassRule::getCRules(treeh,ind);
  //cerr << "TR " << crules.size() << endl;
  for(int i = 0 ; i < (int)crules.size() ; i++)
    {
      Val* trdTree = crules[i].apply(treeh);
      if(trdTree) return trdTree;
    }
  return NULL;
}

int
tree_ruleHead_third(FullHist* treeh)
{
  Val* specTree = NULL;
  if(Feature::isLM) specTree = tree_ruleTree(treeh, 2);
  if(specTree)
    {
      FullHist* pt = treeh->back;
      if(!pt) return nullWordInt;
      int ans = pt->hd->toInt();
      return ans;
    }
  Val* trdtree = tree_ruleTree(treeh,3);
  if(!trdtree) return nullWordInt;
  else return trdtree->wrd();
}


int
tree_watpos(int pos)
{
  if(pos < 0) return nullWordInt;
  int ans = curChart->sentence_[pos].toInt();
  //assert(ans >= 0);
  return ans;
}

int
fh_w1(FullHist* treeh)
{
  int zpos = treeh->pos; 
  return tree_watpos(zpos-1);
}

int
fh_w2(FullHist* treeh)
{
  int zpos = treeh->pos;
  return tree_watpos(zpos-2);
}

void
addSubFeatureFns()
{
  /*
    0 t  fh_term
    1 l  fh_parent_term
    2 u  fh_pos
    3 h  fh_head
    4 i  fh_parent_head
    5 T  fh_true
    6 v  fh_parent_pos
    7 b  fh_term_before
    //8 a  fh_term_after
    8 mE fh_mE  
    9 m  fh_grandparent_term
    10 w fh_grandparent_pos
    11 j tree_ruleHead_third    
    12 c fh_ccparent_term
    13 L1 fh_left1
    14 L1 fh_left2
    15 R1 fh_right1
    16 R1 fh_right2
    17 Qr fh_noopenQr
    18 L0 fh_left0;
    19 L3 fh_left3
    20 R3 fh_right3
    21 Qr fh_noopenQl
    22 Bl fh_Bl
    23 Br fh_Br
    24 vE fh_vE
    25 w1 fh_w1
    26 w2 fh_w2
    */
  int (*funs[27])(FullHist*)
    = {fh_term, fh_parent_term, fh_pos, fh_head,
       fh_parent_head, fh_true, fh_parent_pos, fh_term_before, fh_mE,
       fh_grandparent_term,fh_grandparent_pos,tree_ruleHead_third,
       fh_ccparent_term, fh_left1, fh_left2, fh_right1, fh_right2,
       fh_noopenQr, fh_left0,fh_left3,fh_right3,fh_noopenQl,fh_Bl,
       fh_Br,fh_vE, fh_w1,fh_w2};
  int i;
  for(i = 0 ; i < 27 ; i++) SubFeature::Funs[i] = funs[i];
  ECString temp("^^");
  nullWordInt = Bchart::wtoInt(temp);
}


