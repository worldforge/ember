
#include "FullHist.h"
#include "GotIter.h"

FullHist*
FullHist::
extendBySubConstit()
{
  int hp = back->hpos;
  int& bcpos = back->cpos;
  if(bcpos > hp) bcpos++; //???;
  else if(bcpos == 0) bcpos = hp+1;
  else bcpos--;
  ////cerr << "npcpos " << bcpos << " " << back->size << endl;
  if(bcpos < back->size) return back->fharray[bcpos];
  else return back;
}

FullHist*
FullHist::
extendByEdge(Edge* e1)
{
  //cerr << "ebe " << *e1 << endl;
  if(back) assert(back->term != 47);
  e = e1;
  LeftRightGotIter gi(e1);
  Item* itm;
  int i = 0;
  while(gi.next(itm))
    {
      int termInt = itm->term()->toInt();
      //cerr << "ebei " << termInt << endl;
      FullHist* st = new FullHist(termInt, this,itm);
      assert(i < 64);
      fharray[i++] = st;
      st->cpos = 0;
    }
  //cerr << "ebe ret " << *fharray[hpos] << endl;
  size = i;
  cpos = hpos; //a mess.  hpos was set during meRule Prob;
  return fharray[cpos];
}

/*
FullHist*
FullHist::
extendBySubConstit()
{
  int& bcpos = back->cpos;
  bcpos++; 
  //cerr << "npcpos " << bcpos << " " << back->size << endl;
  if(bcpos < back->size) return back->fharray[bcpos];
  else return back;
}

FullHist*
FullHist::
extendByEdge(Edge* e1)
{
  //cerr << "ebe " << *e1 << endl;
  if(back) assert(back->term != 47);
  e = e1;
  LeftRightGotIter gi(e1);
  Item* itm;
  int i = 0;
  while(gi.next(itm))
    {
      int termInt = itm->term()->toInt();
      //cerr << "ebei " << termInt << endl;
      FullHist* st = new FullHist(termInt, this,itm);
      assert(i < 64);
      fharray[i++] = st;
      st->cpos = 0;
      assert(no47());
    }
  //cerr << "ebe ret " << *fharray[hpos] << endl;
  size = i;
  cpos = 0;
  return fharray[0];
}
*/
FullHist*
FullHist::
retractByEdge()
{
  //assert(cpos == size);
  int i = 0;
  for(; i < size ; i++)
    {
      delete fharray[i];
    }
  return this;
}
  
  
ostream&
operator<<(ostream& os, const FullHist& fh)
{
  FullHist* bfh = fh.back;
  if(bfh)
    {
      os << bfh->term << "/" ;
      if(bfh->hd) os << *bfh->hd;
      os << "--";
    }
  os << fh.term << "/" ;
  if(fh.hd) os << *fh.hd;
  return os;
}
