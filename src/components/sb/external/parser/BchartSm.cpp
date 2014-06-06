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

#include "Bchart.h"
#include "Feature.h"
#include "FeatureTree.h"
#include "utils.h"
#include "math.h"
#include "string.h"
#include "GotIter.h"

extern LeftRightGotIter* globalGi;

void
Bchart::
makepUgT(ECString path)
{
  int i;
  int nm = Term::lastTagInt()+1;
  ECString pth(path);
  for(int f = 0 ; f < 20 ; f++)
    {
      ECString pUString(pth);
      pUString += "pUgT.txt";
      ifstream pUstream(pUString.c_str());
      if(!pUstream)
	{
	  cerr << "Could not find " << pUString << endl;
	  assert(pUstream);
	}
      for( i = 0 ; i <  nm ; i++ )
	{
	  int t;
	  pUstream >> t;
	  float p;
	  if(f == 0)
	    {
	      pUstream >> p;
	      pHugt(t) = p;
	      //cerr << "set pHugt " << t << " = " << p << endl;
	      pUstream >> p;
	      pHcapgt(t) = p;
	      pUstream >> p;
	      pHhypgt(t) = p;
	    }
	  else
	    {
	      pUstream >> p;
	      pHugt(t) += p;
	      //cerr << "set pHugt " << t << " = " << p << endl;
	      pUstream >> p;
	      pHcapgt(t) += p;
	      pUstream >> p;
	      pHhypgt(t) += p;
	    }
	}
    }
  for( i = 0 ; i <  nm ; i++ )
    {
      pHugt(i) /= 20.0;
      pHcapgt(i) /= 20.0;
      pHhypgt(i) /= 20.0;
    }
  ECString resultsString(path);
  resultsString += "pUgT.txt";
  ofstream     resultsStream(resultsString.c_str());
  assert(resultsStream);
  for(i = 0 ; i < nm ; i++)
    {
      resultsStream << i << "\t";
      resultsStream << pHugt(i) << "\t";
      resultsStream << pHcapgt(i) << "\t";
      resultsStream << pHhypgt(i) << "\n";
    }
}

void
Bchart::
readpUgT(ECString path)
{
  ECString pUstring(path);
  pUstring += "pUgT.txt";
  ifstream pUstream(pUstring.c_str());
  assert(pUstream);
  for( int i = 0 ; i <=  Term::lastNTInt() ; i++ )
    {
      int t;
      pUstream >> t;
      float p;
      pUstream >> p;
      pHugt(t) = p;
      //cerr << "set pHugt " << t << " = " << p << endl;
      pUstream >> p;
      if(p == 0) p = .00001f;  //Anything might be capitalized;
      pHcapgt(t) = p;
      pUstream >> p;
      pHhypgt(t) = p;
    }
}

void
Bchart::
readTermProbs(ECString& path)
{
  int i;
  for(i = 0 ; i < MAXSENTLEN ; i++) stops[i] = new Item(Term::stopTerm, i, i);
  //makepUgT(path); //???;
  readpUgT(path);
    
  ECString pTstring(path);
  pTstring += "endings.txt";
  ECString ppTstring(path);
  ppTstring += "nttCounts.txt";
  ifstream pTstream(pTstring.c_str());
  assert(pTstream);
  ifstream ppTstream(ppTstring.c_str());
  assert(ppTstream);

  int numpT;
  pTstream >> numpT;
  pHegt_ = new Wwegt[numpT];
  egtSize_ = numpT;
  i = 0;
  while(pTstream)
    {
      int t;
      ECString  e;
      float p;
      pTstream >> t;
      if(!pTstream) break;
      assert(i < numpT);
      pTstream >> e;
      pTstream >> p;
      pHegt_[i].t = t;
      pHegt_[i].e = e;
      pHegt_[i].p = p;
      i++;
    }
  /* read in counts of each non-term to get p(non-term) in pT */
  int sumTag = 0;
  int sumNT = 0;
  int nums[MAXNUMNTTS];
  for(i = 0 ; i < MAXNUMNTTS ; i++) nums[i] = 0;
  while(ppTstream)
    {
      int t;
      ppTstream >> t;
      if(!ppTstream) break;
      
      assert(t < MAXNUMNTTS);
      ppTstream >> nums[t];
      if(t > Term::lastTagInt()) sumNT += nums[t];
      else sumTag += nums[t];
    }
  float sumTagf = (float)sumTag;
  float sumNTf = (float)sumNT;
  for(i = Term::lastTagInt()+1  ; i < MAXNUMNTTS ; i++)
    {
      if(nums[i] == 0) continue;
      pT(i) = (float)nums[i]/sumNTf;
    }
  ECString wlistString(path);
  //wlistString += "probSum.txt";
  wlistString += "pSgT.txt";
  ifstream wlistStream(wlistString.c_str());
  assert(wlistStream);
  int wnum = 0;
  ECString w;
  wlistStream >> w;  //first entry is number of entries
  lastKnownWord = atoi(w.c_str())-1;
  lastWord = lastKnownWord;
  while(wlistStream)
    {
      wlistStream >> w;
      if(!wlistStream) break;
      ECString dummy;
      for( ; ; )
	{
	  wlistStream >> dummy;
	  if(dummy == "|") break;
	  int trmInt = atoi(dummy.c_str());
	  float prb;
	  wlistStream >> prb;
	  if(prb < .001) continue;
	  const Term* trm = Term::fromInt(trmInt);
	  assert(trm);
	  if(trm->terminal_p() == COLON) Term::Colons.push_back(w);
	  else if(trm->terminal_p() == FINAL) Term::Finals.push_back(w);
	}
      int cnt;
      wlistStream >> cnt;
      invWordMap[wnum] = w;
      wordMap[w] = wnum++;
    }
}

void
Bchart::
initDenom()
{
  static int startState = -1;

  int eosInt = Term::stopTerm->toInt();
  /* we compute p(w_0,i t^j) in parray[j][1],
     then move it to parray[j][0].
     each time we also compute the component of p(w_0,n) due to p(w_i)
     = p(w_0,i)/p(w_0,i-1) and put this in denomProbs[i] */
  float parray[MAXNUMNTS][2];
  int i, j;
  for(i = 0 ; i < MAXNUMNTS ; i++)
    for(j = 0 ; j < 2 ; j++)
      parray[i][j] = 0;
  for(i = 0 ; i < MAXSENTLEN ; i++)
    denomProbs[i] = 0;
  
  if(startState < 0)
    startState = eosInt; 
  parray[startState][0] = 1;
  assert(wrd_count_ < 1000);
  /* compute p(w_0,n t) for all n */
  for(i = 0 ; i < wrd_count_ ; i++)
    {
      float pw0n = 0; // = p(w_{0,i}) = sum_j p(w_{0,i}, j)
      /* for n = i, compute p(w_0,n t) for all t */

      list<float>& wpl = wordPlist(&(sentence_[i]), i);
      list<float>::iterator wpli = wpl.begin();
      for( ; wpli != wpl.end() ; wpli++)
	{
	  float pw0nt = 0;
	  int trmInt = (int)(*wpli);
	  wpli++;
	  float prb = *wpli;
	  if(prb == 0) cerr << "Zero prob from wordPlist, "
	    << sentence_[i] << ", " << trmInt << endl;
	  assert(prb >= 0);
	  for(int k = 0 ; k < MAXNUMNTS ; k++)
	    {
	      float pk = parray[k][0];
	      if(pk == 0) continue;
	      float smb = computepTgT(k,trmInt);
	      assert(smb > 0);
	      assert(pk > 0);
	      pw0nt += pk * prb * smb;
	    }
	  parray[trmInt][1] = pw0nt;
	  if(printDebug(1000))
	    cerr << "initD " << i << "\t" << trmInt << "\t" << pw0nt << endl;
	  pw0n += pw0nt;
	}

      if(pw0n == 0)
	{
	  cerr << "Zero at pos " << i << " word = "
	   << sentence_[i] << endl;
	  /*If we get here it means that all of the beta values are zero.
	    So assign them .00001 * P(word|tag) as a default to avoid
	    all zeros */
	  wpli = wpl.begin();
	  for( ; wpli != wpl.end() ; wpli++)
	    {
	      int trmInt = (int)(*wpli);
	      wpli++;
	      float prb = *wpli;
	      float pw0nt = prb * .00001f;
	      cerr << "Assigning " << trmInt << " prob = " << pw0nt << endl;
	      parray[trmInt][1] = pw0nt;
	      pw0n += pw0nt;
	    }
	}
      assert(pw0n > 0);
      /* now compute the pwarray value we care about */
      denomProbs[i] = pw0n;
      if(printDebug(1000))
	cerr << "denomProb " << i << " = " << pw0n << endl;
      /* and now transfer the values from the [1] column to the [0] */
      for(j = 0 ; j < MAXNUMNTS ; j++)
	{
	  /* each time through we devide probs by p(wi-1|w_0,i-1), and thus
	     all of the figures are p(...|w_o,i) */
	  parray[j][0] = parray[j][1]/pw0n;
	  /* I hope this is something like the relative prob of the tag
	     being t^j at this point in the sent */
	  parray[j][1] = 0;
	}

      wpli = wpl.begin();
      for( ; wpli != wpl.end() ; wpli++) 
	{
	  float pw0nt = 0;
	  int trmInt = (int)(*wpli);
	  wpli++;
	  float prb = *wpli;
	  prb /= denomProbs[i];

	  const Term*  possTerm = Term::fromInt(trmInt);
	  //Item *item =  new Item(possTerm, i, i + 1);
	  Item * item = addtochart(possTerm);
	  item->start() = i;
	  item->finish() = i+1;
	  item->word() = &sentence_[i];
	  item->prob() = prb; 
	  item->prob() *= 1.2;  // 1.1 factor to overcome bigram superiority;
	  Edge* nEdge = new Edge(*item);   
	  // this next is a hack so that that the merit of nEdge will come
	    // out right/
	  nEdge->leftMerit() = (float)( parray[trmInt][0]/item->prob() );
	  //cerr << "plstop for " << *item << " = "
	  //<< parray[trmInt][0] << " / " << item->prob()
	  //<< " = " << nEdge->plstopGt() << endl;
	  nEdge->setmerit();
	  heap->insert(nEdge); 
	  ++ruleiCounts_;
	}
    }

  /* finally, compute the dummy p(dummy eos | prev)
     = sum_i p(w,t^i|prev) * p(eos | t^i) */
  float ans = 0;
  for(i = 0 ; i <= Term::lastTagInt() ; i++)
    {
      float pwti = parray[i][0];
      if(pwti == 0) continue;
      float sbg = computepTgT(i, eosInt);
      ans += sbg * pwti;
    }
  if(printDebug(1000))
    cerr << "initD " << wrd_count_ << "\tSTOP("<<eosInt<<")\t" << ans << endl;
  denomProbs[wrd_count_] = ans;
}

int
Bchart::
wtoInt(ECString& w)
{
  map<ECString, int, less<ECString> >::iterator wmi = wordMap.find(w);
  if(wmi != wordMap.end()) return (*wmi).second;
  wmi = newWordMap.find(w);
  if(wmi != newWordMap.end()) return (*wmi).second;
  lastWord++;
  newWordMap[w] = lastWord;
  newWords.push_back(w);
  return lastWord;
}

ECString
Bchart::
intToW(int n)
{
  if(n <= lastKnownWord) return invWordMap[n];
  else return newWords[n-lastKnownWord-1];
}

list<float>&
Bchart::
wordPlist(Wrd* word, int word_num)
{
  list<float>& ans = wordPlists[word_num];
  if(!ans.empty()) return ans;
  if(printDebug(500))
    cerr << "wordPlist " << *word << endl;
  char temp[512];
  ECString head(word->lexeme());
  ECString headL(toLower(head.c_str(),temp));
  int wint = wtoInt(headL); 
  //cerr << "WTI " << headL << " " << wint << endl;
  word->toInt() = wint;
  if(word->lexeme() == "^^")
    {
      ans.push_back((float)Term::stopTerm->toInt());
      ans.push_back(1.0);
      return ans;
    }
  if( wint <= lastKnownWord )
    {
      int i;
      for( i = 0 ; i <= Term::lastNTInt() ; i ++ )
	{
	  float pwgt = pHst(wint,i);
	  //cerr << "pwgt " << i << " " << pwgt << endl;
	  if(pwgt == 0) continue;
	  float prob = (float)psktt(word,i); 
	  if(prob == 0) continue;
	  assert( prob > 0);
	  ans.push_back((float)i);
	  ans.push_back(prob);
	  if(printDebug(7777)) cerr << "\t" << i << "\t" << prob << endl;
	}
      if(!ans.empty())
	{
	  return ans;
	}
    }
  // in the case of a word that is only known as an NNPS, but we see it
  // uncapitalized, the above will assign 0 prob and ans will be empty.       
  // if this happens, we treat it like an unknown word.;
  for(int i = 0 ; i <= Term::lastTagInt() ; i++)
    {
      if(i == Term::stopTerm->toInt()) continue;
      float phut = pHugt(i);
      if(phut == 0) continue;
      float prob = (float)psutt(word,i);
      if(prob == 0) continue;
      assert(prob > 0);
      if(printDebug(7777)) cerr << "Uk\t" << i << "\t" << prob << endl;
      ans.push_back((float)i);
      ans.push_back(prob);
    }
  assert(!ans.empty());
  return ans;
}


double
Bchart::
psktt(Wrd* shU, int t)
{
  double ans = pHst(shU->toInt(), t); 
  double phcp = 1;
  phcp = pCapgt(shU,t);
  ans *= phcp;
  double put = pHugt(t);
  ans *= (1-put);
  if(ans < 0)
    {
      cerr << phcp << " " << put << endl;
      cerr << "psktt( " << *shU << " | " << t << " ) = " << ans << endl;
      assert(ans >= 0);
    }
  return ans;
}

double
Bchart::
pCapgt(const Wrd* shU, int t)
{
  if(Bchart::caseInsensitive) return 1.0;
  if(Term::Language == "Ch") return 1.0;
  int word_num = shU->loc();
  const ECString& lex0 = sentence_[0].lexeme();
  if(word_num == 0) return 1;
  else if(word_num == 1 &&
	  (lex0 == "``" || lex0 == "-LCB-" || lex0 == "-LRB-"))
    return 1;
  //cerr << "pCapgt = " << pcap << endl;
  if(shU->lexeme().length() < 2) return 1;  //ignore words of length 1;
  char temp[512];
  ECString sh(toLower(shU->lexeme().c_str(),temp));
  bool cap = false;
  /* if all caps, ignore capitalization evidence */
  if(shU->lexeme()[0] != sh[0] && shU->lexeme()[1] != sh[1]) return 1;
  if(shU->lexeme()[0] != sh[0] && shU->lexeme()[1] == sh[1]) cap = true;
  double pcap = pHcapgt(t);  
  return cap ? pcap : (1 - pcap);
}

float
Bchart::
pHst(int wordInt, int t)
{
  assert(wordInt >= 0);
  FeatureTree* strt = FeatureTree::roots(HCALC);
  assert(strt);
  FeatureTree* histPt = strt->follow(t, 0);
  if(!histPt) return 0;
  Feat* ft = histPt->feats.find(wordInt);
  if(!ft) return 0;
  else return ft->g();
}


double
Bchart::
psutt(const Wrd* shU, int t)
{
  //cerr << "Unknown word: " << shU << " for tag: " << t << endl; 
  double ans = pHugt(t);
  //cerr << "pHugt = " << ans << endl;
  assert(ans >= 0);
  if(ans == 0) return 0;
  double phyp = 1;
  if(Term::Language != "Ch") phyp = pHypgt(shU->lexeme(),t);
  ans *= phyp;
  //cerr << "pHypgt = " << phyp << endl;
  double phcp = 1;
  if(Term::Language != "Ch") phcp = pCapgt(shU,t);
  ans *= phcp;
  ans *= .0001;
  assert(ans >= 0);
  if(Term::fromInt(t)->openClass())
    {
      char temp[512];
      ECString sh(toLower(shU->lexeme().c_str(),temp));
      float phegt = pegt(sh,t);
      if(phegt == 0) phegt = .00001f;
      ans *= phegt;
    }
  else
    ans *= .0001;
  ans *= 600;
  assert(ans >= 0);
  //cerr << "psutt( " << shU << " | " << t << " ) = " << ans << endl;
  return ans;
}

int 
Bchart::
bucket(float val)
{
  for(int i = 0 ; i < 14 ; i++)
    if(val <= bucketLims[i])
      return i;
  return 14;
}

int 
Bchart::
bucket(float val, int whichInt, int whichFt)
{
  assert(whichInt < Feature::numCalcs);
  assert(whichFt < MAXNUMFS);
  float logFac = Feature::logFacs[whichInt][whichFt];
  float lval = logFac *log(val);
  int lvi = (int)lval;
  lvi++;
  if(lvi <= 14) return lvi;
  return 14;
}

double
Bchart::
pHypgt(const ECString& shU, int t)
{
  //return 1.0  //ADD to IGNORE hypenization for unknown words
  bool hyp = false;
  const char* hyppos =  strpbrk(shU.c_str(), "-");
  if(hyppos) hyp = true;
  double phyp = pHhypgt(t);  
  return hyp ? phyp : (1 - phyp);
}

float
Bchart::
pegt(ECString& sh, int t)
{
  //return 1.0  //ADD to IGNORE endings for unknown words
  int len = (int)sh.length();
  if(len < 3) return .01f;
  ECString e(sh,len-2,2);
  float phegt = pHegt(e,t);
  //cerr << "pegt( " << sh <<", " << e << " | " << t << " ) = " << phegt << endl;
  return phegt;
}

float
Bchart::
pHegt(ECString& es, int t)
{
  int top = egtSize_;
  int bot = -1;
  for( ; ; )
    {
      if( top <= bot+1 )
	return 0.0;
      int mid = (top+bot)/2;
      Wwegt& midH = pHegt_[mid];
      
      int gt =  greaterThan(midH, es, t);
      if(gt  == 0) return midH.p;
      else if( gt == 1 ) top = mid;
      else bot = mid;
    }
}

int
Bchart::
greaterThan(Wwegt& wwegt, ECString e, int t)
{
  int ans = 0;
  if(wwegt.t < t) ans = -1;
  else if(wwegt.t > t) ans = 1;
  else if(wwegt.e < e) ans = -1;
  else if(wwegt.e > e) ans = 1;
  return ans;
}

float
Bchart::
computepTgT(int t1,int t2)
{
  FullHist fh;
  fh.preTerm = t1;
  fh.term = t2;
  globalGi = NULL;
  return meFHProb(Term::fromInt(t2), fh, TTCALC);
}

float
Bchart::
computeMerit(Edge* edge, int whichDist)
{
  float ans = 0;  //accumulate the sum here;
  const Term* trm = edge->item()->term();
  FullHist fh(edge);
  int denomPos = edge->loc();
  if(whichDist == LMCALC) denomPos = edge->start()-1;
  float denom = 0.1f;  // ??? should be 1, but merit hs a problem with start.
  if(denomPos >= 0) denom = denomProbs[denomPos];
  if(denom <= 0)
    {
      cerr << denomPos << " " << *edge << endl;
      assert(denom > 0);
    }
  /* p(w|nt) = sum_t p(w|t)p(t|nt) where w is the word following item */
  if(denomPos == wrd_count_ || denomPos < 0)
    {
      /* if w is the word preceeding or
	 following the sentence, it is a pretend word
	 with only one part of speech, STOP */
      fh.term = Term::stopTerm->toInt();
      ans = meFHProb(Term::stopTerm, fh, whichDist);
    }
  else
    {
      Wrd& w = sentence_[denomPos];
      list<float>& wpl = wordPlist(&w, denomPos);
      list<float>::iterator wpli = wpl.begin();
      for( ; wpli != wpl.end() ; wpli++)
	{
	  int termInt = (int)(*wpli);
	  const Term* nxtTerm = Term::fromInt(termInt);
	  wpli++;
	  float  pwgt = *wpli;
	  float phtgnt = meFHProb(nxtTerm, fh, whichDist);
	  ans += pwgt * phtgnt;
	}
    }
  ans *= 1.5;
  ans /= denom;
  if(whichDist == LMCALC) ans *= pT(edge->lhs()->toInt());
  if(printDebug(150)) cerr << "cM" << whichDist << " = " << ans << endl;
  return ans;
}

