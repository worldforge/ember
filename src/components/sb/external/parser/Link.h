#ifndef LINKH
#define LINKH

#include <vector>
#include "ECString.h"

class InputTree;

#define DUMMYVAL 999

class Link;
typedef  vector<Link*> Links;
typedef  Links::iterator LinksIter;

class Link
{
 public:
  Link(short key): key_(key){}
  ~Link()
    {
      LinksIter li = links_.begin();
      for( ; li != links_.end() ; li++) delete (*li);
    }
  Link* is_unique(InputTree* tree, bool& ans);
  short key() const { return key_; }
 private:
  Link* do_link(int tint, bool& ans);
  short key_;
  Links links_;
};

#endif
