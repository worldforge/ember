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

#include <fstream>
#include <iostream>
#include "Feature.h"
#include "ECString.h"
#include "utils.h"
#include "FeatureTree.h"
#include "math.h"

SubFeature* SubFeature::array_[MAXNUMCALCS][MAXNUMFS];
int SubFeature::total[MAXNUMCALCS];
int (*SubFeature::Funs[MAXNUMFS])(FullHist*);
int (*SubFeature::PRFuns[2])(int);
int      SubFeature::ufArray[MAXNUMCALCS][MAXNUMFS];
int      SubFeature::splitPts[MAXNUMCALCS][MAXNUMFS];
Feature* Feature::array_[MAXNUMCALCS][MAXNUMFS];
int Feature::total[MAXNUMCALCS];
float*     Feature::lambdas_[MAXNUMCALCS][MAXNUMFS];
int Feature::conditionedFeatureInt[MAXNUMCALCS];
int  Feature::whichInt;
int Feature::assumedFeatVal;
int (*Feature::conditionedEvent)(FullHist*);
int (*Feature::assumedSubFeat)(FullHist*);
FTypeTree Feature::ftTree[MAXNUMCALCS];
FTypeTree* Feature::ftTreeFromInt[MAXNUMCALCS][MAXNUMFS];
float Feature::logFacs[MAXNUMCALCS][MAXNUMFS]; 
bool  Feature::isLM = false;
int   Feature::numCalcs = 11;

int MinCount = 2;

void
Feature::
assignCalc(ECString conditioned)
{
  if(conditioned == "h") whichInt = HCALC;
  else if(conditioned == "u") whichInt = UCALC;
  else if(conditioned == "r") whichInt = RCALC;
  else if(conditioned == "ru") whichInt = RUCALC;
  else if(conditioned == "rm") whichInt = RMCALC;
  else if(conditioned == "tt") whichInt = TTCALC;
  else if(conditioned == "l") whichInt = LCALC;
  else if(conditioned == "lm") whichInt = LMCALC;
  else if(conditioned == "s") whichInt = SCALC;
  else if(conditioned == "t") whichInt = TCALC;
  else if(conditioned == "ww") whichInt = WWCALC;
  else
    {
      assert(conditioned == "m");
      whichInt = MCALC;
    }
}

void
Feature::
init(ECString& path, ECString& conditioned)
{
  assignCalc(conditioned);
  int f;
  for(f = 0 ; f < MAXNUMFS ; f++)
    {
      float* vec = new float[15];
      lambdas_[whichInt][f] = vec;
      for(int k = 0 ; k < 15 ; k++) vec[k] = 0.0;
    }
  ECString dataECString(path);
  dataECString += "featInfo.";
  dataECString += conditioned;
  ifstream dataStrm(dataECString.c_str());
  assert(dataStrm);

  int auxCnts[MAXNUMFS];
  int i;
  for(i = 0 ; i < MAXNUMFS ; i++) auxCnts[i] = 0;
  Feature::ftTreeFromInt[whichInt][0] = &(Feature::ftTree[whichInt]);

  int conditionedInt;
  dataStrm >> conditionedInt;
  conditionedFeatureInt[whichInt] = conditionedInt;
  int num;
  for(num = 0 ;  ; num++)
    {
      int n, subf, pos, cpr;
      ECString nm;
      ECString tmp;
      dataStrm >> tmp;
      if(tmp == "--") break;
      n = atoi(tmp.c_str());
      dataStrm >> nm;
      dataStrm >> subf;
      dataStrm >> pos;
      dataStrm >> tmp;
      
      if(tmp == "|")
	cpr = -1;
      else
	{
	  cpr = atoi(tmp.c_str());
	  dataStrm >> tmp;
	  assert(tmp == "|");
	}
      array_[whichInt][n-1] = new Feature(n, nm, subf, pos, cpr);
      array_[whichInt][n-1]->auxCnt = auxCnts[pos];
      auxCnts[pos]++;
      createFTypeTree(Feature::ftTreeFromInt[whichInt][pos], n, whichInt);
    }
  Feature::total[whichInt] = num;
  for(num = 0 ;  ; num++)
    {
      int n, fn;
      ECString nm;
      ECString tmp;
      dataStrm >> tmp;
      if(tmp == "--") break;
      n = atoi(tmp.c_str());
      dataStrm >> nm;
      dataStrm >> fn;
      list<int> featList;
      for( ; ; )
	{
	  dataStrm >> tmp;
	  if(tmp == "|") break;
	  int f = atoi(tmp.c_str());
	  featList.push_back(f);
	}
      SubFeature::fromInt(n, whichInt)
	= new SubFeature(n, nm, fn, featList);
      assert(SubFeature::fromInt(n, whichInt));
    }
  SubFeature::total[whichInt] = num;
  /* set the universal function num on feats from their subfeats */
  for(num = 0 ; num < Feature::total[whichInt] ; num++)
    {
      Feature* f = array_[whichInt][num];
      f->usubFeat = SubFeature::fromInt(f->subFeat,whichInt)->usf;
    }
  /* set up the table from universal subfeat nums to subfeat nums */
  for(num = 0 ; num < MAXNUMFS ; num++)
    SubFeature::ufArray[whichInt][num] = -1;
  for(num = 0 ; num < SubFeature::total[whichInt] ; num++)
    {
      SubFeature* sf = SubFeature::fromInt(num,whichInt);
      SubFeature::ufArray[whichInt][sf->usf] = num;
    }
}

void
Feature::
printLambdas(ostream& res)
{
   int b,f;
   for(b = 1; b < 15 ; b++)
     {
       res << b ;
       for(f = 2; f <= Feature::total[whichInt] ; f++)
	 {
	   res << "\t";
	   float val = Feature::getLambda(whichInt, f, b);
	   if(val > .998f) val = .998f;  //nothing too close to 1; 
	   res << val;
	 }
       res << "\n";
     }
}

void
Feature::
createLam(int which, ECString tmp, ECString path)
{
  ECString npath("/ltmp/discrim/FOLDS/");
  int b,f;
  int tot = Feature::total[which];
  for(int i = 0 ; i < 20 ; i++)
    {
      ECString ftstr(npath);
      ftstr += intToString(i) + "/";
      ftstr += tmp;
      ftstr += ".lambdas";
      ifstream fts(ftstr.c_str());
      assert(fts);
      for(b = 1; b < 15 ; b++)
	{
	  int bb;
	  if(!fts)
	    {
	      cerr << "Trouble reading lambs for " << which << " in " << ftstr
		   << endl;
	      assert(fts);
	    }
	  fts >> bb ;
	  //cerr << bb << endl;
	  if(bb != b)
	    {
	      cerr << tmp << " " << b << " " << bb << endl;
	      assert(bb == b);
	    }
	  for(f = 2; f <= tot ; f++)
	    {
	      float lam;
	      assert(fts);
	      fts >> lam;
	      //cerr << which << " " << f << " " << b << " " << lam << endl;
	      if(i == 0) Feature::setLambda(which,f,b,lam);
	      else lamVal(which,f,b) += lam;
	    }
	}
    }
  for(b = 1; b < 15 ; b++)
    for(f = 2; f <= tot ; f++) lamVal(which,f,b) /= 20.0;
  
  ECString ftstr(path);
  ftstr += tmp;
  ftstr += ".lambdas";
  ofstream res(ftstr.c_str());
  assert(res);
  res.precision(3);
  printLambdas(res);
} 


void
Feature::
readLam(int which, ECString tmp, ECString path)
{
  ECString ftstr(path);
  ftstr += tmp;
  ftstr += ".lambdas";
  ifstream fts(ftstr.c_str());
  assert(fts);
  int b,f;
  int tot = Feature::total[which];

  if(Feature::isLM)
    {
      /* This for loop is removed for old bucketing; */
      for(f = 2 ; f <= tot ; f++)
	{
	  float logBase;
	  fts >> logBase;
	  logFacs[which][f] = (float)( 1.0/log(logBase) );
	}
    }
  for(b = 1; b < 15 ; b++)
     {
       int bb;
       if(!fts)
	 {
	   cerr << "Trouble reading lambs for " << which << " in " << ftstr
		<< endl;
	   assert(fts);
	 }
       fts >> bb ;
       //cerr << bb << endl;
       if(bb != b)
	 {
	   cerr << tmp << " " << b << " " << bb << endl;
	   assert(bb == b);
	 }
       for(f = 2; f <= tot ; f++)
	 {
	   float lam;
	   assert(fts);
	   fts >> lam;
	   //cerr << which << " " << f << " " << b << " " << lam << endl;
	   Feature::setLambda(which,f,b,lam);
	 }
     }
} 

void
Feature::
createFTypeTree(FTypeTree* posftTree, int n, int which)
{
  assert(posftTree);
  if(!posftTree->left)
    {
      posftTree->left = new FTypeTree(n);
      Feature::ftTreeFromInt[which][n] = posftTree->left;
    }
  else if(!posftTree->right)
    {
      posftTree->right = new FTypeTree(AUXIND);
      createFTypeTree(posftTree->right, n, which);
    }
  else createFTypeTree(posftTree->right, n, which);
}     
