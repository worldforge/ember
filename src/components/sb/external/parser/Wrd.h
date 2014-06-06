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

#ifndef WRD_H 
#define WRD_H

#include "ECString.h"
#include <iostream>
#include "ewDciTokStrm.h"

class Wrd;
class Wrd
{
public:
  friend class SentRep;
  Wrd() : lexeme_(""), loc_(-1) {}
  Wrd(const Wrd& wrd) : lexeme_(wrd.lexeme()),loc_(wrd.loc()),wInt_(wrd.toInt()) {}
  Wrd(ECString lx, int ps) : lexeme_(lx), loc_(ps) {}
  const ECString&  lexeme() const { return lexeme_; }
  ECString&  lexeme() { return lexeme_; }
  friend ewDciTokStrm& operator>>(ewDciTokStrm& is, Wrd& w)
    {
      w.lexeme_ = is.read();
      return is;
    }
  friend int operator<(const Wrd& w1, const Wrd& w2)
    { return w1.lexeme_ < w2.lexeme_; }
  friend istream& operator>>(istream& is, Wrd& w)
    {
      ECString lx;
      is >> lx;
      if(lx == "(") lx = "-LRB-";
      else if(lx == ")") lx = "-RRB-";
      else if(lx == "{") lx = "-LCB-";
      else if(lx == "}") lx = "-RCB-";
      else if(lx == "[") lx = "-LSB-";
      else if(lx == "]") lx = "-RSB-";
      w.lexeme_ = lx;
      return is;
    }
  friend ostream& operator<<(ostream& os, const Wrd& w)
    {
      os << w.lexeme_;
      return os;
    }
  void operator=(const Wrd& wr)
    {
      lexeme_ = wr.lexeme();
      loc_ = wr.loc();
    }
  int loc() const { return loc_; }
  int toInt() const { return wInt_; }
  int& toInt() { return wInt_; }
private:
  int loc_;
  int wInt_;
  ECString lexeme_;
};

#endif
