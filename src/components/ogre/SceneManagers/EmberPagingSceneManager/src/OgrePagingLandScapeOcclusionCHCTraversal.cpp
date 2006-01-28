#include "OgrePagingLandScapeOctreeCamera.h"
#include "OgrePagingLandScapeOctree.h"
#include "OgrePagingLandScapeOctreeNode.h"
#include "OgrePagingLandScapeOcclusionElement.h"
#include "OgrePagingLandScapeOcclusion.h"

#include "OgrePagingLandScapeOcclusionSorter.h"
#include "OgrePagingLandScapeOcclusionTraversal.h"
#include "OgrePagingLandScapeOcclusionCHCTraversal.h"
#include "OgrePagingLandScapeOcclusionVisibilityData.h"

namespace Ogre
{

    //-----------------------------------------------------------------------
    void CHCTraversal::onTree(PagingLandScapeOctree& node) const 
    {
    	#ifdef _VISIBILITYDEBUG
            assert (node.getNodeData (occlusion.mCurrentCam)->viewFrustumVisible);
		    occlusion.traversed_nodes_cnt++;       
        #endif //_VISIBILITYDEBUG    
                
        if (!node.mNodes.empty())
        {	
            PagingLandScapeOctreeNodeList::iterator it =  node.mNodes.begin();  
            while (it != node.mNodes.end())
            { 
                stack.push (*it);
                ++it;
            }
        }

        PagingLandScapeOctree *n = 
            node.mChildren[ 0 ][ 0 ][ 0 ];
        if (n && n->hasChildren())  
           stack.push(n);
        n = node.mChildren[ 1 ][ 0 ][ 0 ];
        if (n && n->hasChildren())  
            stack.push(n);
        n = node.mChildren[ 0 ][ 1 ][ 0 ];
        if (n && n->hasChildren())  
            stack.push(n);
        n = node.mChildren[ 1 ][ 1 ][ 0 ];
        if (n && n->hasChildren())  
            stack.push(n);
        n = node.mChildren[ 0 ][ 0 ][ 1 ];
        if (n && n->hasChildren())  
            stack.push(n);
        n = node.mChildren[ 1 ][ 0 ][ 1 ];
        if (n && n->hasChildren())  
            stack.push(n);
        n = node.mChildren[ 0 ][ 1 ][ 1 ];
        if (n && n->hasChildren())  
            stack.push(n);
        n = node.mChildren[ 1 ][ 1 ][ 1 ];
        if (n && n->hasChildren())  
            stack.push(n);
	}
    //-----------------------------------------------------------------------
    void CHCTraversal::onLeaf(PagingLandScapeOctreeNode& n) const 
    {
		#ifdef _VISIBILITYDEBUG
            assert (n.getNodeData (occlusion.mCurrentCam)->viewFrustumVisible);
            assert (n.getNodeData (occlusion.mCurrentCam)->queryVisible);
		    occlusion.traversed_nodes_cnt++;
        #endif //_VISIBILITYDEBUG    
        if (!n.isOccluder() || mIsNotSolidScene)
			occlusion.queueDraw (n);
	}
    //-----------------------------------------------------------------------
    CHCTraversal::CHCTraversal(FrontToBackNodeSorterPriorityQueue& vStack, Occlusion& o): 
        stack(vStack), 
        occlusion(o), 
        frameId(o.getFrame()),
		mIsNotSolidScene(occlusion.isNotSolidScene())
    { 
    }
}