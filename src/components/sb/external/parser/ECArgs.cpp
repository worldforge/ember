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

#include "ECArgs.h"
#include <assert.h>
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include "utils.h"

ECArgs::
ECArgs(int argc, char *argv[])
{
  nargs_ = 0;
  for(int i = 1 ; i < argc ; i++)
    {
      ECString arg(argv[i]);
      if(arg[0] != '-')
	{
	  argList[nargs_] = arg;
	  nargs_++;
	}
      else
	{
	  nopts_++;
	  int l = (int)arg.length();
	  assert(l > 1);
	  ECString opt(1,arg[1]);
	  optList.push_back(opt);
	  if(l == 2) optList.push_back("");
	  else
	    {
	      ECString v(arg,2,l-2);
	      optList.push_back(v);
	    }
	}
    }
}

bool
ECArgs::
isset(char c)
{
  ECString sig = "";
  sig += c;
  list<ECString>::iterator
    oIter = find(optList.begin(), optList.end(), sig);
  bool found = (oIter != optList.end());
  return found;
}


ECString
ECArgs::
value(char c)
{
  ECString sig;
  sig += c;

  list<ECString>::iterator oIter = find(optList.begin(), optList.end(), sig);
  bool found = (oIter != optList.end());
  if(!found)
    {
      cerr << "Looking for value of on-line argument " << sig << endl;
      error("could not find value");
    }
  ++oIter;
  return *oIter;
}


