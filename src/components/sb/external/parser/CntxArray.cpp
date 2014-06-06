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

#include "CntxArray.h"

//int  CntxArray::sz = 6;
int  CntxArray::sz = 5;

CntxArray::
CntxArray(int* data)
{
  int i;
  for(i = 0 ; i < sz ; i++)
    //d[i] = data[i+1];
    d[i] = data[i];
}

CntxArray::
CntxArray()
{
  int i;
  for(i = 0 ; i < sz ; i++)
    d[i] = -1;
}
    
int
operator<(CntxArray a1, CntxArray a2)
{
  int i;
  for(i = 0 ; i < CntxArray::sz ; i++)
    {
      if(a1.d[i] > a2.d[i]) return 0;
      else if(a1.d[i] < a2.d[i]) return 1;
      else if(a1.d[i] < 0) return 0;
    }
  return 0;
}

ostream&
operator<< ( ostream& os, const CntxArray& ca )
{
  int i;
  int sz = ca.sz;
  for(i = 0 ; i < sz ; i++)
    {
      int val = ca.d[i];
      if(val == -1) os << ".";
      else os << val;
      if(i != sz-1) os << "/";
    }
  return os;
}
