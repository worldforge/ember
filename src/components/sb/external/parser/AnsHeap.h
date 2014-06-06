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

#ifndef ANSHEAP_H 
#define ANSHEAP_H

#define AHeapSize 100
#include "AnswerTree.h"

class           AnsTreeHeap
{
public:
  ~AnsTreeHeap();
  AnsTreeHeap() : nth(NTH), unusedPos_(0){}
  void         insert(AnsTreePair* ans);
  AnsTreePair*   pop();
  int          size() { return unusedPos_; }
  AnsTreePair*   front() { return array[0]; }
  static bool print;
private:
  void  del_(int pos);
  void  downHeap(int pos);
  bool  upheap(int pos);
  int   left_child(int par) const { return (par*2) + 1; }
  int   right_child(int par) const { return ((par*2) + 2); }
  int   parent(int child) const { return ((child-1)/2); }
  int   unusedPos_;
  int   nth;
  AnsTreePair*  array[AHeapSize];
};



#endif /* !ANSHEAP_H */
