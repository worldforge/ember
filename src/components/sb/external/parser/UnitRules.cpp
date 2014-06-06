/*
 * Copyright 2005 Brown University, Providence, RI.
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

#include "UnitRules.h"
#include "Term.h"

/* only needed for sel */
void
UnitRules::
init()
{
}

void
UnitRules::
printData(ECString path)
{
  ECString fl(path);
  fl += "unitRules.txt";
  ofstream data(fl.c_str());
  assert(data);
  int lim = Term::lastNTInt() - Term::lastTagInt();
  for(int i = 0 ; i < lim ; i++)
    {
      assert(data);
      data  << unitRules[i] << "\n";
    }
  ECString tmp;
  data << tmp;
}
     

void
UnitRules::
readData(ECString path)
{
  int p, c;
  for(p = 0 ; p < MAXNUMNTS ; p++)
    for(c = 0 ; c < MAXNUMNTS ; c++) bef_[p][c] = false;
  
  ECString fl(path);
  fl += "unitRules.txt";
  ifstream data(fl.c_str());
  assert(data);
  for( ; ; )
    {
      data >> p;
      if(!data) break;
      data >> c;
      bef_[p][c] = true;
      //cerr << "PCT " << p << " " << c << endl;
    }
}

bool
UnitRules::
badPair(int par, int chi)
{
  int parInt = par-Term::lastTagInt()-1;
  int chiInt = chi-Term::lastTagInt()-1;
  //cerr << "BP " << parInt << " " << chiInt << endl;
  return !bef_[parInt][chiInt];
}


void
recMark(int p, int c, int bef[MAXNUMNTS][MAXNUMNTS], int lim)
{
  assert(bef[p][c] != 0);
  if(bef[p][c] >= 1) return;
  bef[p][c] = 1;
  bef[c][p] = 0;
  for(int k = 0 ; k < lim ; k++)
    {
      if(bef[c][k] > 0) recMark(p, k, bef, lim);
      if(bef[k][p] > 0) recMark(k, c, bef, lim);
    }
}
