
#ifndef CLASSRULE_H
#define CLASSRULE_H

#include "FullHist.h"
#include <fstream>
#include <iostream>
#include <vector>
#include "AnswerTree.h"
#include "Bst.h"

#define MCALCRULES 10

class ClassRule
{
 public:
  ClassRule(int dd, int mm, int rr, int tt)
    : t_(tt), m_(mm), rel_(rr), d_(dd) {}
  ClassRule(const ClassRule& cr)
    : t_(cr.t()), m_(cr.m()), rel_(cr.rel()), d_(cr.d()) {}
  Val* apply(FullHist* treeh);
  static void readCRules(ECString str);
  static vector<ClassRule>& getCRules(FullHist* treeh, int wh);
  friend ostream& operator<<(ostream& os, const ClassRule& cr)
    {
      os << "{"<< cr.d() << "," << cr.m() << "," << cr.rel() << "," << cr.t() << "}";
      return os;
    }
  int d() const { return d_; }
  int m() const { return m_; }
  int t() const { return t_; }
  int rel() const { return rel_; }
 private:
  int d_;
  int m_;
  int t_;
  int rel_;
  static vector<ClassRule>  rBundles2_[MAXNUMNTTS][MAXNUMNTS];
  static vector<ClassRule>  rBundles3_[MAXNUMNTTS][MAXNUMNTS];
  static vector<ClassRule>  rBundlesm_[MAXNUMNTTS][MAXNUMNTS];
};
    
typedef vector<ClassRule> CRuleBundle;

#endif /* ! CLASSRULE_H */
