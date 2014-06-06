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

#ifndef FBINARYARRAY_H
#define FBINARYARRAY_H

#include <iostream>
#include <fstream>

class Feat;
class FeatureTree;

class FBinaryArray
{
 public:
  FBinaryArray() : size_(0) {}
  void set(int sz);
  Feat*   find(const int id) const;
  int     size() const { return size_; }
  Feat*   index(int i);
  int size_;
  Feat* array_;
};

class FTreeBinaryArray
{
 public:
  FTreeBinaryArray() : size_(0) {}
  void set(int sz);
  FeatureTree*   find(const int id) const;
  int     size() const { return size_; }
  FeatureTree*   index(int i);
  int size_;
  FeatureTree* array_;
};

#endif /* ! FBINARYARRAY_H */
