
#include "Link.h"
#include "InputTree.h"
#include "Term.h"

Link*
Link::
do_link(int tint, bool& ans)
{
  LinksIter li = links_.begin();
  for( ; li != links_.end() ; li++)
    {
      Link* slink = (*li);
      if(slink->key() == tint)
	{
	  ans = false;
	  return slink;
	}
    }
  ans = true;
  Link* nlink = new Link(tint);
  //cerr << "LN " << tint << endl;
  links_.push_back(nlink);
  return nlink;
}

Link*
Link::
is_unique(InputTree* tree, bool& ans)
{
  //cerr << "IU " << *tree << endl;
  Link* nlink;
  const Term* trm = Term::get(tree->term());
  nlink = do_link(trm->toInt(), ans);
  if(trm->terminal_p()) return nlink;
  InputTreesIter iti = tree->subTrees().begin();
  for( ; iti != tree->subTrees().end() ; iti++)
    {
      nlink = nlink->is_unique((*iti), ans);
    }
  nlink = nlink->do_link(DUMMYVAL, ans);
  return nlink;
}
    
      
      
