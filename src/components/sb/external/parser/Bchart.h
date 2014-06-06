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

#ifndef BCHART_H
#define BCHART_H

#include "ChartBase.h"
#include "EdgeHeap.h"
#include "Wrd.h"
#include "FullHist.h"
#include "UnitRules.h"

#define Termstar const Term*

struct Wwegt  
{
  int t;
  ECString e;
  float p;
};

class Bchart;

class           Bchart : public ChartBase
{
public:
    Bchart(SentRep& sentence);
    virtual ~Bchart();
    virtual double  parse();
    static int&      printDebug() { return printDebug_; }
    static bool      printDebug(int val) { return val < printDebug_; }
    static void     readTermProbs(ECString& path);
    static void     makepUgT(ECString path);
    static void     readpUgT(ECString path);
    static int      wtoInt(ECString& str);
    int     extraTime; //if no parse is found on regular time;
    static  Item*    dummyItem;
    static float timeFactor;
    static float    denomProbs[MAXSENTLEN];  
    void            check();
    static void     setPosStarts();
    static ECString intToW(int n);
    bool prned();
    bool issprn(Edge* e);
    static map< ECString, int, less<ECString> > wordMap;
    static ECString invWordMap[MAXNUMWORDS];
  static int lastKnownWord;
  static int lastWord;
  static map<ECString, int> newWordMap;
  static vector<ECString> newWords;
  static UnitRules*  unitRules; 
  static bool caseInsensitive;
  static bool tokenize;
  static int Nth;
  static bool prettyPrint;
  static bool silent;
  Item*   edgesFromTree(InputTree* tree);
  void  set_Betas();

 protected:
  /* this block of functions are only used/defined in rParse */
    Wrd*  add_word(const Term* trm, int st, ECString wrdStr);
    Item* add_item(int b, const Term* trmNm, int wrd);
    Item* add_item2(int b, const Term* trm, int wInt, ECString wrdstr);
    Item* addToChart(const Term* trm);;
    Item* in_chart(int b, const Term* trm, bool& wasThere);
    Item* in_chartT(int b, const Term* trm);
    Edge* add_edge(Item* lhs, Items& rhs);
    void   computeEdgeBeta(Item* itm, Edge* edge);
    void   propagateItemBeta(Item* itm, double quant);
    int   headPosFromItems(Item* lhs, Items& rhs);
    void  readItem(istream& str, int& b, const Term*& trm);
    void  store_word(Wrd* wrd);
    Wrd*  find_word(int wint, int st);
    void  assignRProb(Edge* edge);
    double compute_Betas();
    bool  compute_Beta(Item* itm);
    double compute_EdgeBeta(Edge* edge);
    InputTree* lookUpPhrase(Item* lhs, ECString phrase);
    void  newWord(ECString wrdstr, int wInt, Item* ans);
    Edge*  procPhrasal(Item* lhs, ECString phrase);
    bool   procPhrase(Item* lhs, InputTree* tree);
    void   rPendFactor();
    /* end of block */

    void            add_reg_item(Item * itm);
    void            addFinishedEdge(Edge* newEdge);
    void            add_starter_edges(Item* itm);
    float           meEdgeProb(const Term* trm, Edge* edge, int whichInt);
    float           meFHProb(const Term* trm, FullHist& fh, int whichInt);
    static int printDebug_;

    void            extend_rule(Edge* rule, Item * itm, int right);
    void            already_there_extention(int i, int start, int right,
					    Edge* edge);
    void            add_edge(Edge* rli, int left);
    void            put_in_reg(Item * item);
    void            addWordsToKeylist( );
    Item           *in_chart(const Wrd* hd, const Term * trm,
			     int start, int finish);
    bool            repeatRule(Edge* edge);

    void            redoP(Edge* edge, double probRatio);
    void            redoP(Item *item, double probDiff);

    float           computeMerit(Edge* edge, int whichCalc);

    void  initDenom();
    list<float>& wordPlist(Wrd* word, int word_num);
    double  psktt(Wrd* shU, int t);
    double  pCapgt(const Wrd* shU, int t);
    float   pHst(int w, int t);
    double  psutt(const Wrd* shU, int t);
    float   pegt(ECString& sh, int t);
    void    getpHst(const ECString& hd, int t);
    double pHypgt(const ECString& shU, int t);
    static float&  pHcapgt(int i) { return pHcapgt_[i]; }
    static float&  pHhypgt(int i) { return pHhypgt_[i]; }
    static float&  pHugt(int i) { return pHugt_[i]; }
    static float& pT(int tInt)
      {
	int val = tInt-Term::lastTagInt()-1;
	if(!(val >= 0 && val < MAXNUMTS))
	  {
	    cerr << "Bad val = " << val << endl;
	    assert(val >= 0 && val < MAXNUMTS);
	  }
	return pT_[val];
      }
    int     bucket(float val, int whichInt, int whichFt);
    int     bucket(float val);
    int    greaterThan(Wwegt& wwegt, ECString e, int t);
    float  pHegt(ECString& es, int t);
    float  computepTgT(int t1,int t2);
    void   addToDemerits(Edge* edge);
    static Item*    stops[MAXSENTLEN];
    EdgeHeap*       heap;
    int             alreadyPopedNum;
    Edge*           alreadyPoped[450000]; //was 350000;
    static int&     posStarts(int i, int j);
    static int      posStarts_[MAXNUMNTTS][MAXNUMNTS];
    static int     curDemerits_[MAXSENTLEN][MAXSENTLEN];

  static int egtSize_;
  static float bucketLims[14];
  static float pT_[MAXNUMTS];
  static float pHcapgt_[MAXNUMTS];
  static float pHhypgt_[MAXNUMTS];
  static float pHugt_[MAXNUMTS];

  static Wwegt* pHegt_;
  list<float> wordPlists[MAXSENTLEN];
};

#endif	/* ! BCHART_H */

