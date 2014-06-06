
#include "ClassRule.h"
#include "Item.h"
#include "Bst.h"

Val* sibling_tree(Val* tree, int dir);
Val* rel2_tree(Val* at, int curd);

extern int fh_parent_term(FullHist* treeh);
extern int fh_term(FullHist* treeh);
extern Val* curVal;
extern Val* gcurVal;
extern int curDir;

vector<ClassRule>  ClassRule::rBundles2_[MAXNUMNTTS][MAXNUMNTS];
vector<ClassRule>  ClassRule::rBundles3_[MAXNUMNTTS][MAXNUMNTS];
vector<ClassRule>  ClassRule::rBundlesm_[MAXNUMNTTS][MAXNUMNTS];

CRuleBundle&
ClassRule::
getCRules(FullHist* treeh, int wh)
{
  int d = treeh->term;
  int m;
  int modm = Term::stopTerm->toInt();
  if(wh == MCALCRULES)
    {
      m = fh_term(treeh);
    }
  else m = fh_parent_term(treeh);
  if (wh==3)
    {
      //cerr << "GCR3 " <<d << " " << m << " " << rBundles3_[d][m-modm].size() << endl;
      return rBundles3_[d][m-modm];
    }
  else if(wh == MCALCRULES) return rBundlesm_[d][m];
  return rBundles2_[d][m-modm];
}

Val*
ClassRule::
apply(FullHist* treeh)
{
  Val* trdTree;
  //int wI = Feature::whichInt;
  //cerr << "AP " << *this << endl;
  //if(curVal) cerr << "APCV " << *curVal << endl;
  if(rel_ == 0)
    {
      trdTree = gcurVal;
    }
  else if(rel_ == 1) trdTree = sibling_tree(curVal, curDir);
  else if(rel_ == 2) trdTree = rel2_tree(curVal, curDir);
  else if(rel_ == 4) trdTree = curVal;
  else error("bad relation");
  if(!trdTree)
    {
      return NULL;
    }
  int ct = trdTree->trm();
  //cerr << "foudn " << t_ << " " << ct << " " << *trdTree << endl;
  if(t_ != ct)
    {
      return NULL;
    }
  return trdTree;
}

  
void
ClassRule::
readCRules(ECString path)
{
  ECString flnm = path;
  flnm += "rules.txt";
  ifstream is(flnm.c_str());
  int wh = 2;
  //cerr << "RCR" << endl;
  int modm = Term::stopTerm->toInt();

  assert(is);

  ECString tmp;
  for( ; ; )
    {
      int d, m, r, t;
      is >> tmp;
      if(tmp == "Thirds:")
	{
	  wh = 3;
	  continue;
	}
      //cerr << "T1 " << tmp << endl;
      if(!is) break;
      d = Term::get(tmp)->toInt();
      is >> tmp;
      m = Term::get(tmp)->toInt();
      is >> r;
      r--;
      is >> tmp;
      t = Term::get(tmp)->toInt();
      assert(is);
      ClassRule cr(d,m,r,t);
      //cerr << "RR " << cr << endl;
      if(wh == 3) rBundles3_[d][m-modm].push_back(cr);
      else rBundles2_[d][m-modm].push_back(cr);
    }
  flnm = path;
  flnm += "rules.m";
  ifstream ism(flnm.c_str());
  if(!ism) return;
  ism >> tmp; // all thirds;
  for( ; ; )
    {
      ECString tmp;
      int d, m, t;
      ism >> tmp;
      //cerr << "T1 " << tmp << endl;
      if(!ism) break;
      d = Term::get(tmp)->toInt();
      ism >> tmp;
      m = Term::get(tmp)->toInt();
      ism >> tmp;
      t = Term::get(tmp)->toInt();
      assert(ism);
      ClassRule cr(d,m,0,t);
      rBundlesm_[d][m].push_back(cr);
    }
}
      
Val*
sibling_tree(Val* at, int curd)
{
  //Val* sib;
  if(curd == 0) return NULL;
  const Bsts& trs = at->bsts();
  //cerr << "ST " << curd << " " << trs.size() << endl;
  if(trs.size() < 2) return NULL;
  Val* ans;
  if(curd > 1) 
    {
      ans = trs.back()->nth(0);;
      assert(ans);
      return ans;
    }
  ans = trs.front()->nth(0);
  assert(ans);
  //cerr << "ST2 " << *ans << endl;
  return ans;
}

Val*
rel2_tree(Val* at, int curd)
{
  //cerr << "In rel 2 " << curd << endl;
  if(curd == 0) return NULL;
  Val* sib;
  const Bsts& trs = at->bsts();
  if(trs.size() < 2) return NULL;
  if(curd > 1) sib = trs.back()->nth(0);
  else sib = trs.front()->nth(0);

  return NULL; // added EDF - missing return statement
  /*
  if(!sib || sib->wrd != at->wrd || !sib->e)
    {
      return NULL;
    }
  Edge* e = sib->e;
  int sibhp = e->headPos()-1;
  Vals& subs = sib->subtrees;
  //cerr << "r22 " << subs.size() << " " << sibhp << endl;
  if(curd == 1)
    {
      if(sibhp > 0) return subs.front();
      if(subs.size() < 2) return NULL;
      ValIter ati = subs.begin();
      ati++;
      return (*ati);
    }
  if(sibhp < subs.size()-1) return subs.back();
  if(subs.size() < 2) return NULL;
  Vals::reverse_iterator ati = subs.rbegin();
  //cerr << "QQ " << *((*ati)->item) << endl;
  ati++;
  //cerr << *at->e << endl;
  //cerr << "CC " << *e << " " << subs.size() << " " << sibhp << endl;
  return(*ati);
  */
}

