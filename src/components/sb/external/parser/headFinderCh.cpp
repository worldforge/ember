#include <list>
#include <vector>
#include <map>
#include "headFinderCh.h"
#include "Term.h"
#include "InputTree.h"
#include <sstream>


typedef map<ECString,list<list<ECString> >,less<ECString> > MapSLL;
typedef map<ECString,list<list<ECString> >,less<ECString> >::iterator MapSLLIter;
typedef list<list<ECString> >::iterator LLIter;
typedef list<ECString>::iterator LIter;
MapSLL hmap;
void printHeadInfo(){
    MapSLLIter miter=hmap.begin();
    for(;miter!=hmap.end();miter++){
		cerr<<"term:"<<(*miter).first<<endl;
		list<list<ECString> > termlist=(*miter).second;
	    LLIter termiter=termlist.begin();
	    for(;termiter!=termlist.end();termiter++){
			list<ECString> hlist=(*termiter);
			LIter hiter=hlist.begin();
			for(;hiter!=hlist.end();hiter++){
				cerr<<*hiter<<" ";
			}
			cerr<<endl;
		}
	}
}

void
readHeadInfoCh(ECString& path)
{
  ECString headStrg(path);
  headStrg += "headInfo.txt";
  ifstream headStrm(headStrg.c_str());
  assert(headStrm);
  ECString term,line,head;
  list<ECString> listofheads;

  int listlen;
  while(getline(headStrm,line)){
	  istringstream oss(line);
	  oss>>term;
	  oss>>listlen;
	  //cerr<<term<<listlen<<endl;
	  assert(Term::get(term));
      list<list<ECString> > termlist;
	  for (int i=0;i<listlen;i++){
		  getline(headStrm,line);
		  istringstream osl(line);
		  list<ECString> listofheads;
		  while(osl>>head){
			  listofheads.push_back(head);
		  }
		  termlist.push_back(listofheads);
	  }
	  hmap[term]=termlist;
	  //cerr<<term<<endl;
	  //cerr<<termlist<<endl;
  }
  //printHeadInfo();

}

int
headPosFromTreeCh(InputTree* tree)
{
  int   ansPriority = 10;
  ECString lhsString(tree->term());
  if(lhsString == "") lhsString = "S1";
  int   pos = -1;
  int   ans = -1;
  int   subsize=0;
  ConstInputTreesIter subTreeIter = tree->subTrees().begin();
  vector<ECString> subvec;
  for( ; subTreeIter != tree->subTrees().end() ; subTreeIter++ ){
	  assert(*subTreeIter);
	  subvec.push_back((*subTreeIter)->term());
	  subsize++;
  }
  assert(subsize==tree->subTrees().size());
  list<list<ECString> > termlist=hmap[lhsString];
  //cerr<<"want to find head for "<<lhsString<<endl;

  LLIter termiter=termlist.begin();
  for(;termiter!=termlist.end();termiter++){
	  list<ECString> hlist=*termiter;

	  LIter hiter=hlist.begin();
	  ECString searchdir=*hiter;
	  hiter++;
	  if (searchdir=="L"){
	      if(hlist.size()==1) return 0;
		  for(int i=0;i<subsize;i++){
		    for(;hiter!=hlist.end();hiter++)
		        if (subvec[i]==(*hiter)){
					ans=i;
					break;
				}
			hiter=hlist.begin();
			hiter++;
	      }
	  }
	  else if(searchdir=="R"){
		  if(hlist.size()==1){
			  return subsize-1;
		  }
		  for(int i=subsize-1;i>=0;i--){
		    for(;hiter!=hlist.end();hiter++)
		        if (subvec[i]==(*hiter)){
					ans=i;
					break;
				}
			if (ans>=0) break;
			hiter=hlist.begin();
			hiter++;
		  }
		}
	  else {cerr<<tree->term()<<" "<<searchdir<<endl; assert(0);}
	  if (ans>=0) break;
  }
  if (ans<0){
	  //cerr<<tree->term()<<endl;
          assert(tree->term()=="S1"); //???;
  	  assert(subsize<=3);
  	  return subsize-1;
  }
  return ans;
}
