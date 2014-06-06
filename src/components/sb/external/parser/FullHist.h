
#ifndef FULLHIST_H
#define FULLHIST_H

#include "Edge.h"
#include <list>
#include "Wrd.h"
#include <iostream>
#include <fstream>
#include "ECString.h"

class FullHist;
typedef list<FullHist*>::iterator FullHistIter;

class
FullHist
{
public:
  FullHist() : cpos(0), e(NULL), back(NULL),hd(NULL) {}
  FullHist(Edge* e1) : cpos(0), e(e1), back(NULL){}
  FullHist(int tint, FullHist* fh)
    : cpos(0), term(tint), back(fh), pos(-1),hd(NULL){}
  FullHist(int tint, FullHist* fh, Item* i)
    : cpos(0), itm(i), term(tint), back(fh), pos(-1),hd(NULL){}
  FullHist* extendByEdge(Edge* e1);
  FullHist* extendBySubConstit();
  FullHist* retractByEdge();
  FullHist* nth(int n)
    {
      if(n < 0 || n >= size) return NULL;
      else return fharray[n];
    }
  friend ostream& operator<<(ostream& os, const FullHist& fh);
 int hpos;
  int pos;
  int term;
  int preTerm;
  Edge* e;
  Item* itm;
  const Wrd* hd;
  FullHist* back;
  FullHist* fharray[64];
  int cpos;
  int size;
};

#endif
