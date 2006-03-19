
#include "OgrePagingLandScapeOcclusionTraversal.h"

#include "OgrePagingLandScapeOctreeCamera.h"
#include "OgrePagingLandScapeOctree.h"
#include "OgrePagingLandScapeOctreeNode.h"
#include "OgrePagingLandScapeOcclusionElement.h"
#include "OgrePagingLandScapeOcclusion.h"

namespace Ogre
{
    //-----------------------------------------------------------------------
    void ConstTraversalConst::traverseChildren(const PagingLandScapeOctree& node) const 
    {                
        if (!node.mNodes.empty())
        {
            //Add stuff to be rendered;
            PagingLandScapeOctreeNodeList::const_iterator it = node.mNodes.begin();
        }
       
        PagingLandScapeOctree *n = 
            node.mChildren[ 0 ][ 0 ][ 0 ];
        if (n && n->hasChildren())  
            n->traversal (*this);
        n = node.mChildren[ 1 ][ 0 ][ 0 ];
        if (n && n->hasChildren())  
            n->traversal (*this);
        n = node.mChildren[ 0 ][ 1 ][ 0 ];
        if (n && n->hasChildren())  
            n->traversal (*this);
        n = node.mChildren[ 1 ][ 1 ][ 0 ];
        if (n && n->hasChildren())  
            n->traversal (*this);
        n = node.mChildren[ 0 ][ 0 ][ 1 ];
        if (n && n->hasChildren())  
            n->traversal (*this);
        n = node.mChildren[ 1 ][ 0 ][ 1 ];
        if (n && n->hasChildren())  
            n->traversal (*this);
        n = node.mChildren[ 0 ][ 1 ][ 1 ];
        if (n && n->hasChildren())  
            n->traversal (*this);
        n = node.mChildren[ 1 ][ 1 ][ 1 ];
        if (n && n->hasChildren())  
            n->traversal (*this);

	}
    //-----------------------------------------------------------------------
    void TraversalConst::traverseChildren(PagingLandScapeOctree& node) const 
    {                  
        if (!node.mNodes.empty())
        {
            //Add stuff to be rendered;
            PagingLandScapeOctreeNodeList::iterator it = node.mNodes.begin();
            while (it != node.mNodes.end())
            {
                (*it)->traversal(*this);
                ++it;
            }            
        }
        
        PagingLandScapeOctree *n = 
            node.mChildren[ 0 ][ 0 ][ 0 ];
        if (n && n->hasChildren())  
            n->traversal (*this);
        n = node.mChildren[ 1 ][ 0 ][ 0 ];
        if (n && n->hasChildren())  
            n->traversal (*this);
        n = node.mChildren[ 0 ][ 1 ][ 0 ];
        if (n && n->hasChildren())  
            n->traversal (*this);
        n = node.mChildren[ 1 ][ 1 ][ 0 ];
        if (n && n->hasChildren())  
            n->traversal (*this);
        n = node.mChildren[ 0 ][ 0 ][ 1 ];
        if (n && n->hasChildren())  
            n->traversal (*this);
        n = node.mChildren[ 1 ][ 0 ][ 1 ];
        if (n && n->hasChildren())  
            n->traversal (*this);
        n = node.mChildren[ 0 ][ 1 ][ 1 ];
        if (n && n->hasChildren())  
            n->traversal (*this);
        n = node.mChildren[ 1 ][ 1 ][ 1 ];
        if (n && n->hasChildren())  
            n->traversal (*this);

	} 
    //-----------------------------------------------------------------------
    void Traversal::traverseChildren(PagingLandScapeOctree& node)
    {                  
        if (!node.mNodes.empty())
        {
            //Add stuff to be rendered;
            PagingLandScapeOctreeNodeList::iterator it = node.mNodes.begin();
            while (it != node.mNodes.end())
            {
                (*it)->traversal(*this);
                ++it;
            }
        }
      
        PagingLandScapeOctree *n = 
            node.mChildren[ 0 ][ 0 ][ 0 ];
        if (n && n->hasChildren())  
            n->traversal (*this);
        n = node.mChildren[ 1 ][ 0 ][ 0 ];
        if (n && n->hasChildren())  
            n->traversal (*this);
        n = node.mChildren[ 0 ][ 1 ][ 0 ];
        if (n && n->hasChildren())  
            n->traversal (*this);
        n = node.mChildren[ 1 ][ 1 ][ 0 ];
        if (n && n->hasChildren())  
            n->traversal (*this);
        n = node.mChildren[ 0 ][ 0 ][ 1 ];
        if (n && n->hasChildren())  
            n->traversal (*this);
        n = node.mChildren[ 1 ][ 0 ][ 1 ];
        if (n && n->hasChildren())  
            n->traversal (*this);
        n = node.mChildren[ 0 ][ 1 ][ 1 ];
        if (n && n->hasChildren())  
            n->traversal (*this);
        n = node.mChildren[ 1 ][ 1 ][ 1 ];
        if (n && n->hasChildren())  
            n->traversal (*this);

	}
    
}