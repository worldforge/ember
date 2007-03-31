
#include "OgrePagingLandScapePrecompiledHeaders.h"

#include "OgrePagingLandScapeOcclusionTraversal.h"

#include "OgrePagingLandScapeOctreeCamera.h"
#include "OgrePagingLandScapeOctree.h"
#include "OgrePagingLandScapeOctreeNode.h"
#include "OgrePagingLandScapeOcclusionElement.h"
#include "OgrePagingLandScapeOcclusion.h"

namespace Ogre
{
    //-----------------------------------------------------------------------
	void ConstTraversalConst::traverseChildren(const PagingLandScapeOctree& node, VisibleObjectsBoundsInfo * const visibleBounds) const 
    {                
        if (!node.mNodes.empty())
        {
			//Add stuff to be rendered;
			PagingLandScapeOctreeNodeList::const_iterator it = node.mNodes.begin();
			PagingLandScapeOctreeNodeList::const_iterator itEnd = node.mNodes.end();
			while (it != itEnd)
			{
				(*it)->traversal (*this, visibleBounds);
				++it;
			}
        }
       
        PagingLandScapeOctree *n = 
            node.mChildren[ 0 ][ 0 ][ 0 ];
        if (n && n->hasChildren())  
            n->traversal (*this, visibleBounds);
        n = node.mChildren[ 1 ][ 0 ][ 0 ];
        if (n && n->hasChildren())  
            n->traversal (*this, visibleBounds);
        n = node.mChildren[ 0 ][ 1 ][ 0 ];
        if (n && n->hasChildren())  
            n->traversal (*this, visibleBounds);
        n = node.mChildren[ 1 ][ 1 ][ 0 ];
        if (n && n->hasChildren())  
            n->traversal (*this, visibleBounds);
        n = node.mChildren[ 0 ][ 0 ][ 1 ];
        if (n && n->hasChildren())  
            n->traversal (*this, visibleBounds);
        n = node.mChildren[ 1 ][ 0 ][ 1 ];
        if (n && n->hasChildren())  
            n->traversal (*this, visibleBounds);
        n = node.mChildren[ 0 ][ 1 ][ 1 ];
        if (n && n->hasChildren())  
            n->traversal (*this, visibleBounds);
        n = node.mChildren[ 1 ][ 1 ][ 1 ];
        if (n && n->hasChildren())  
            n->traversal (*this, visibleBounds);

	}
    //-----------------------------------------------------------------------
    void TraversalConst::traverseChildren(PagingLandScapeOctree& node, VisibleObjectsBoundsInfo * const visibleBounds) const 
    {                  
        if (!node.mNodes.empty())
        {
            //Add stuff to be rendered;
			PagingLandScapeOctreeNodeList::iterator it = node.mNodes.begin();
			PagingLandScapeOctreeNodeList::iterator itEnd = node.mNodes.end();
			while (it != itEnd)
            {
                (*it)->traversal(*this, visibleBounds);
                ++it;
            }            
        }
        
        PagingLandScapeOctree *n = 
            node.mChildren[ 0 ][ 0 ][ 0 ];
        if (n && n->hasChildren())  
            n->traversal (*this, visibleBounds);
        n = node.mChildren[ 1 ][ 0 ][ 0 ];
        if (n && n->hasChildren())  
            n->traversal (*this, visibleBounds);
        n = node.mChildren[ 0 ][ 1 ][ 0 ];
        if (n && n->hasChildren())  
            n->traversal (*this, visibleBounds);
        n = node.mChildren[ 1 ][ 1 ][ 0 ];
        if (n && n->hasChildren())  
            n->traversal (*this, visibleBounds);
        n = node.mChildren[ 0 ][ 0 ][ 1 ];
        if (n && n->hasChildren())  
            n->traversal (*this, visibleBounds);
        n = node.mChildren[ 1 ][ 0 ][ 1 ];
        if (n && n->hasChildren())  
            n->traversal (*this, visibleBounds);
        n = node.mChildren[ 0 ][ 1 ][ 1 ];
        if (n && n->hasChildren())  
            n->traversal (*this, visibleBounds);
        n = node.mChildren[ 1 ][ 1 ][ 1 ];
        if (n && n->hasChildren())  
            n->traversal (*this, visibleBounds);

	} 
    //-----------------------------------------------------------------------
    void Traversal::traverseChildren(PagingLandScapeOctree& node, VisibleObjectsBoundsInfo * const visibleBounds)
    {                  
        // hasLocalChildren
        if (!node.mNodes.empty())
        {
            //Add stuff to be rendered;
			PagingLandScapeOctreeNodeList::iterator it = node.mNodes.begin();
			PagingLandScapeOctreeNodeList::iterator itEnd = node.mNodes.end();
            while (it != itEnd)
            {
                (*it)->traversal (*this, visibleBounds);
                ++it;
            }
        }
      
        PagingLandScapeOctree *n = 
            node.mChildren[ 0 ][ 0 ][ 0 ];
        if (n && n->hasChildren())  
            n->traversal (*this, visibleBounds);

        n = node.mChildren[ 1 ][ 0 ][ 0 ];
        if (n && n->hasChildren())  
            n->traversal (*this, visibleBounds);

        n = node.mChildren[ 0 ][ 1 ][ 0 ];
        if (n && n->hasChildren())  
            n->traversal (*this, visibleBounds);

        n = node.mChildren[ 1 ][ 1 ][ 0 ];
        if (n && n->hasChildren())  
            n->traversal (*this, visibleBounds);

        n = node.mChildren[ 0 ][ 0 ][ 1 ];
        if (n && n->hasChildren())  
            n->traversal (*this, visibleBounds);

        n = node.mChildren[ 1 ][ 0 ][ 1 ];
        if (n && n->hasChildren())  
            n->traversal (*this, visibleBounds);

        n = node.mChildren[ 0 ][ 1 ][ 1 ];
        if (n && n->hasChildren())  
            n->traversal (*this, visibleBounds);

        n = node.mChildren[ 1 ][ 1 ][ 1 ];
        if (n && n->hasChildren())  
            n->traversal (*this, visibleBounds);

	}
    
}
