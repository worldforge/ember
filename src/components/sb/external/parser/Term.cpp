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

#include "Term.h"
#include "utils.h"

Term*  Term::array_[MAXNUMNTTS];
map<ECString, Term*,less<ECString> > Term::termMap_;
int    Term::lastTagInt_ = 0;
int    Term::lastNTInt_ = 0;
const Term*  Term::stopTerm;
const Term*  Term::startTerm;
const Term*  Term::rootTerm;
ECStrings Term::Colons;
ECStrings Term::Finals;
ECString Term::Language = "En";

bool
Term::
isS() const 
{
  if(Term::Language == "Ch") return name_== "IP";
  else return name_ == "S";
}

Term::
Term()
: num_( -1 ), terminal_p_( 0 )
{}

Term::
Term(const ECString s, int terminal, int num )
: name_( s ), num_( num ), terminal_p_( terminal )
{}

Term::
Term( const Term& src )
: name_( src.name() ), 
  //num_(src.toInt()),
  terminal_p_( src.terminal_p() )
{}

ostream&
operator<< ( ostream& os, const Term& t )
{
    os << t.name();
    return os;
}

int
Term::
operator== ( const Term& rhs ) const
{
    if( this == &rhs || name_ == rhs.name() )
	return 1;
    return 0;
}


void
Term::
init(ECString & prefix)
{
  ECString          fileName(prefix);
  fileName += "terms.txt";
  ifstream           stream(fileName.c_str(), ios::in);
  if (!stream)
    {
      cerr << "Can't open terms file " << fileName << endl;;
      return;
    }
  
  ECString          termName;
  int ind, n;
  n = 0;
  bool seenNTs = false;
  while (stream >> termName)
    {
      stream >> ind;
      Term* nextTerm = new Term(termName, ind, n);
      termMap_[nextTerm->name()] = nextTerm;
      if(termName == "STOP") Term::stopTerm = nextTerm;
      else if(termName == "G4") Term::startTerm = nextTerm;
      else if(termName == "S1") Term::rootTerm = nextTerm;
      array_[n] = nextTerm;
      if(!ind && !seenNTs)
	{
	  assert(n > 0);
	  lastTagInt_ = n-1;
	  seenNTs = true;
	}
      n++;
      assert(n < 400);
    }
  assert(!ind);
  lastNTInt_ = n-1;
  //lastNTInt_ = n-4;  //??? hack to ignore G1 and G2 and G3;
  stream.close();
}

Const_Term_p
Term::
get(const ECString& nm)
{
  TermMap::iterator ti = termMap_.find(nm);
  if( ti == termMap_.end()) return NULL;
  return (*ti).second;
}

