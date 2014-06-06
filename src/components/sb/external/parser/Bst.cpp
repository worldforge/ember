
#include "Bst.h"

void
Bst::
tester(Val* oval)
{
  return;
  Bsts::const_iterator bi = oval->bsts().begin();
  //cerr << "OBS " << oval->bsts().size() << endl;
  int jj = 0;
  for( ; bi != oval->bsts().end() ; bi++)
    {
      cerr << "JJ" << jj << endl;
      jj++;
    }
  cerr << "AFT" << endl;
}
  
bool
Bst::
ptst(Val* val)
{
  int sz = heap.size();
  for(int i = 0 ; i < sz ; i++)
    if(*val==*heap.index(i))
      {
	cerr << "Bad val " << *val << endl;
	return false;
      }
  return true;
}  
  

Val*
Bst::
next(int n)
{
  int hsz = heap.size();
  //cerr << "Need " << n << "th variation out of " << num()
  // << " with " << hsz << " on heap. " << done_<< endl;
  assert(n <= num());
  if(num() > n) return nbest[n];
  if(done_) return NULL;
  double oprob = 1;
  if(n > 0)
    {
      assert(nbest[n-1]);
      oprob = nbest[n-1]->prob();
    }
  Val* val = nbest[n-1];
  //cerr << "   For " << *val << endl;
  
  for(int i = 0 ; i < val->len() ; i++)
    {
      bool stop = false;
      Val* nv = Val::newIth(i, val,stop);
      if(nv)
	{
	  //cerr << "Got the possible variation " 
	  //   << *nv  << " "<< heap.size()<< " "
	  //   << i << " " << n << " " << num() << endl;
	  //assert(ptst(nv));
	  heap.push(nv);
	}
      if(stop) break;
    }
  if(heap.size()==0)
    {
      done_ = true;
      return NULL;
    }
  Val* ans = heap.pop();
  //assert(ans->check());
  //cerr << "The desired variation is " << *ans << " " << num_ << " "
  //   << hsz << " " << heap.size() << endl;
  /*
  if(!ans->edge())
    {
      if(!ans->len() == 1)
	{
	  cerr << "Odd situation" << endl;
	}
      else
	{
	  int wh2 = ans->vec(0);
	  Bst& b2 = ithBst(0,ans->bsts());
	  Val* subans = b2.nth(wh2);
	  if(!subans->edge())
	    {
	      cerr << "Odd 2" << endl;
	    }
	  cerr << "Subedge = " << *subans->edge() << endl;
	}
    }
  */
  nbest.push_back(ans);
  num_++;
  return ans;
}

int vlens = 100;
Bst::
~Bst()
{
  int i;
  assert(nbest.size() == num());
  for(i = 0 ; i < num() ; i++)
    {
      Val* v = nbest[i];
      delete v;
    }
}
 
Val::
~Val(){}

short
Val::
trm() const
{
  //return trm_; //???;
  if(trm_ >= 0) return trm_;
  if(wrd_ == -2) return trm_;
  if(wrd_ != -1)
    {
      cerr << "Bad trm and wrd " << trm_ << " " << wrd_ << endl;
      assert(wrd_ == -1);
    }
  assert(status == EXTRAVAL);
  if(len() != 1) return trm_;
  return bsts_.front()->nth(0)->trm();
}

int
Val::
wrd() const
{
  //return wrd_; //??;
  if(wrd_ >= 0) return wrd_;
  if(wrd_ == -2) return wrd_;
  assert(wrd_ == -1);
  assert(status == EXTRAVAL);
  if(len() != 1) return wrd_;
  return bsts_.front()->nth(0)->wrd();
}

Val::
Val(Edge* e, double prb)
  :  edge_(e), len_(0), prob_(prb), wrd_(-2), status(NORMALVAL)
{
  trm_ = e->lhs()->toInt();
  assert(trm_ >= 0 && trm_ < 400);
}

void  
Val::
extendTrees(Bst& bst2,int pos)
{
  len_++;
  vec_.push_back(0);
  if(bsts_.size() == 0) bsts_.push_back(&bst2);
  else if(pos < 2) bsts_.push_front(&bst2);
  else bsts_.push_back(&bst2);
  prob_ *= bst2.prob();
}

Bst&
ithBst(int i, Bsts& bsts)
{
  Bsts::iterator bi = bsts.begin();
  for(int j = 0 ; j < i ; j++) bi++;
  return (**bi);
}

bool
operator==(Val& v1, Val& v2)
{
  if(v1.edge() != v2.edge()) return false;
  shortIter si2 = v2.vec().begin();
  for(shortIter si = v1.vec().begin(); si != v1.vec().end() ; si++)
    {
      if((*si) != (*si2)) return false;
      si2++;
    }
  return true;
}

ostream&
operator<<(ostream& os, const Val& v)
{
  os << "<";
  if(v.edge()) os << *(v.edge());
  else os << v.trm() << " " << v.wrd();
  os << ", ";
  v.vec();
  int sz = (int)v.vec().size();
  for(int i = 0 ; i < sz ; i++)
    {
      os << v.vec()[i];
      if(i < sz-1) os << ",";
    }
  os << ">";
  return os;
}
 
Val*
Val::
newIth(int ith, Val* oval, bool& stop)
{
  int ithc = oval->vec(ith);
  if(ithc > 0) stop = true;
  short nxtI = ithc +1;;
  //cerr<< "Wnt " << nxtI << "th var on pos " << ith << " of " << *oval<< endl;
  if(oval->wrd() >= 0) return NULL; //???;
  double ovalprob = oval->prob();
  //if(oval->status == TERMINALVAL) return NULL;
  Val* nval = ithBst(ith,oval->bsts()).next(nxtI);
  if(!nval) return NULL;
  double ovalcompprob = ithBst(ith, oval->bsts()).nth(ithc)->prob();
  double nprob = nval->prob();
  //cerr << "Its prob is " << nprob << endl;
  if(nprob < 0) return NULL;
  Val* ans = new Val(oval);
  ans->vec(ith) = nxtI;
  double frac = nprob/ovalcompprob;
  ans->prob() *= frac;
  assert(nxtI <= ithBst(ith,ans->bsts()).num());
  //cerr << "ith is " << ith << " " << ans->vec().size() << " "
  //   << ans->vec()[ith] << " " << frac << endl;
  //cerr << "The var is " << *ans << endl;
  //assert(ans->check());
  return ans;
}

Val::
Val(Val* oval)
  : edge_(oval->edge()), len_(oval->len()), prob_(oval->prob()),
    wrd_(oval->wrd()), trm_(oval->trm()), status(oval->status)
{
  assert(trm_ >= -1 && trm_ < 400);
  vector<short>::const_iterator vsi = oval->vec().begin();
  for( ; vsi != oval->vec().end() ; vsi++) vec_.push_back(*vsi);
  Bsts::const_iterator bi = oval->bsts().begin();
  for( ; bi != oval->bsts().end() ; bi++)
    {
      bsts_.push_back(*bi);
    }
};
  

bool
Val::
check()
{
  vector<short>::const_iterator vsi = vec().begin();
  Bsts::const_iterator bi = bsts().begin();
  for( ; bi != bsts().end() ; bi++)
    {
      if((*bi)->num() < (*vsi)) return false;
      vsi++;
    }
  
  return true;
}

Bst&
bstFind(CntxArray& ca, BstMap& bm)
{
  BstMap::iterator bi = bm.find(ca);
  if(bi == bm.end())
    {
      return bm[ca];
    }
  else return (*bi).second;
}

