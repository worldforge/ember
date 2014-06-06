/* can copy into npharser05/*/
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

#ifndef TERM_H
#define TERM_H

#include "ECString.h"
#ifdef WIN32       // temporarily disable warning on windows
#pragma warning( push )
#pragma warning( disable : 4800 )  // forcing value to bool 'true' or 'false' (performance warning)
#endif
#include <list>
#ifdef WIN32
#pragma warning( pop )
#endif
#include <map>
#include <assert.h>
#include <iostream>
#include <fstream>
#include "Feature.h"
#include "utils.h"


class Term;

typedef Term *		Term_p;
typedef const Term *	Const_Term_p;
typedef const Term     ConstTerm;

#define Terms list<ConstTerm*>
#define ConstTerms const list<ConstTerm*>
#define TermsIter list<Term*>::iterator
#define ConstTermsIter list<ConstTerm*>::const_iterator
typedef map<ECString, Term*, less<ECString> >  TermMap;

#define FINAL 3
#define COLON 8



class Term 
{
public:
    Term();			// provided only for maps.
    Term( const ECString s, int terminal, int n );
    Term( const Term& src );
    int              toInt() const { return num_; }
    const ECString& name() const {  return name_;  }
    friend ostream& operator<< ( ostream& os, const Term& t );
    friend ostream& operator>> ( istream& os, const Term& t );
    int		operator== (const Term& rhs ) const;

    int	  terminal_p() const { return terminal_p_; }
    bool   isPunc() const { return (terminal_p_ > 2) ? true : false ; }
    bool   openClass() const { return (terminal_p_ == 2) ? true : false ; }
    bool   isColon() const { return vECfind(name(), Colons);}
    bool   isFinal() const { return vECfind(name(), Finals);}
    bool   isComma() const { return terminal_p_ == 4; }
    bool   isCC() const { return (name() == "CC" || name() == "CONJP"); }
    bool   isRoot() const { return (name() == "S1"); }
    bool   isS() const ;
    bool   isParen() const {return terminal_p_ == 7;}
    bool   isNP() const {return name() == "NP";}
    bool   isVP() const {return name() == "VP";}
    bool   isOpen() const { return terminal_p_ == 5; }
    bool   isClosed() const { return terminal_p_ == 6; }

    static Const_Term_p get(const ECString& getname);
    static void  init(ECString & prefix);
    static const Term* fromInt(int i) 
      { assert(i < MAXNUMNTTS); return array_[i]; }
    static int  lastTagInt() { return lastTagInt_; }
    static int  lastNTInt() { return lastNTInt_; }
    static const Term* stopTerm;
    static const Term* startTerm;
    static const Term* rootTerm;
    static ECStrings Finals;
    static ECStrings Colons;
    static ECString Language;
private:
    ECString* namePtr() { return (ECString*)&name_; }
    int    	terminal_p_;
    int		num_;
    const ECString name_;
    static Term*  array_[MAXNUMNTTS];
    static TermMap termMap_ ;
    static int    lastTagInt_;
    static int    lastNTInt_;
};
  

#endif /* ! TERM_H */
