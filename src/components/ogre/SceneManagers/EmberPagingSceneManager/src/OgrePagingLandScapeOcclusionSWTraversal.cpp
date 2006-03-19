
#include "OgrePagingLandScapeOcclusionTraversal.h"
#include "OgrePagingLandScapeOcclusionSWTraversal.h"

#include "OgrePagingLandScapeOctreeCamera.h"
#include "OgrePagingLandScapeOctree.h"
#include "OgrePagingLandScapeOctreeNode.h"

#include "OgrePagingLandScapeOcclusionSorter.h"
#include "OgrePagingLandScapeOcclusionElement.h"
#include "OgrePagingLandScapeOcclusion.h"
#include "OgrePagingLandScapeOcclusionVisibilityData.h"

namespace Ogre
{
    //-----------------------------------------------------------------------
    SWTraversal::SWTraversal  (Occlusion& o):
        occlusion(o), 
        mCurrentVisibility (PagingLandScapeOctreeCamera::NONE),
        camPos(occlusion.mCurrentCam->getPosition ())
    {
        
    }
    //-----------------------------------------------------------------------
    void SWTraversal::traverseChildren(PagingLandScapeOctree & node)
    {
	    FrontToBackNodeSorterPriorityQueue myStack = FrontToBackNodeSorterPriorityQueue(FrontToBackNodeSorterOperator (camPos));
       
        //assert (node.getNodeData (occlusion.mCurrentCam)->viewFrustumVisible);
        if (!node.mNodes.empty())
        {
            PagingLandScapeOctreeNodeList::const_iterator it =  node.mNodes.begin();
            while (it != node.mNodes.end())
            {
                 myStack.push(*it);
                ++it;
            }
        }

        PagingLandScapeOctree *n = 
            node.mChildren[ 0 ][ 0 ][ 0 ];
        if (n && n->hasChildren())  
            myStack.push (n);
        n = node.mChildren[ 1 ][ 0 ][ 0 ];
        if (n && n->hasChildren())  
            myStack.push (n);
        n = node.mChildren[ 0 ][ 1 ][ 0 ];
        if (n && n->hasChildren())  
            myStack.push (n);
        n = node.mChildren[ 1 ][ 1 ][ 0 ];
        if (n && n->hasChildren())  
            myStack.push (n);
        n = node.mChildren[ 0 ][ 0 ][ 1 ];
        if (n && n->hasChildren())  
            myStack.push (n);
        n = node.mChildren[ 1 ][ 0 ][ 1 ];
        if (n && n->hasChildren())  
            myStack.push (n);
        n = node.mChildren[ 0 ][ 1 ][ 1 ];
        if (n && n->hasChildren())  
            myStack.push (n);
        n = node.mChildren[ 1 ][ 1 ][ 1 ];
        if (n && n->hasChildren())  
            myStack.push (n);

        //traverse nearer child first
        while (!myStack.empty ())
        {
	        OcclusionElement& nstacked = *myStack.top ();
	        myStack.pop ();
            nstacked.traversal(*this);
        }
    }
    //-----------------------------------------------------------------------
    bool SWTraversal::isVisible(OcclusionElement & n) 
    {
        // issue query
        if (n.isLeaf () && n.isOccluder() )
        {
            if (occlusion.issueDrawQuery (static_cast <PagingLandScapeOctreeNode &> (n)))
            {
                // we just wait for result
                return occlusion.isQueryResultIsVisible (n);
            }
            else
            {
                return false;
            }
        }
        else
        { 
            occlusion.issueBboxQuery (n); 
            // we just wait for result
	        return occlusion.isQueryResultIsVisible (n);
        }
	       
    }
    //-----------------------------------------------------------------------
    void SWTraversal::onLeaf(PagingLandScapeOctreeNode &n)
    {
        if(occlusion.mCurrentCam->isVisible (n.getCullBoundingBox ()))
        {

		    #ifdef _VISIBILITYDEBUG
	            occlusion.traversed_nodes_cnt++;
                n.getNodeData (occlusion.mCurrentCam)->viewFrustumVisible = true;
		    #endif //_VISIBILITYDEBUG   

            const bool isitVisible = isVisible (n);
		    if (isitVisible) 
            {
                #ifdef _VISIBILITYDEBUG    
			        occlusion.object_cnt++;
                #endif //_VISIBILITYDEBUG   
                occlusion.queueDraw (n);
            } 
		    #ifdef _VISIBILITYDEBUG
		        n.getNodeData (occlusion.mCurrentCam)->queryVisible = isitVisible;
		    #endif //_VISIBILITYDEBUG   

        }

		#ifdef _VISIBILITYDEBUG
	        else 
	        {
	            occlusion.traversed_nodes_cnt++;
                n.getNodeData (occlusion.mCurrentCam)->viewFrustumVisible = false;
		        occlusion.frustum_culled_nodes_cnt++;
	        }
		#endif //_VISIBILITYDEBUG    
    }
    //-----------------------------------------------------------------------
    void SWTraversal::onTree(PagingLandScapeOctree & n)
    {
        if(occlusion.mCurrentCam->isVisible(n.getCullBoundingBox ())) 
        {

		    #ifdef _VISIBILITYDEBUG
	            occlusion.traversed_nodes_cnt++; 
                n.getNodeData (occlusion.mCurrentCam)->viewFrustumVisible = true;
		    #endif //_VISIBILITYDEBUG    

            const bool isitVisible = isVisible(n);

		    if (isitVisible) 
            {
                traverseChildren (n);
		    }

		    #ifdef _VISIBILITYDEBUG
		        n.getNodeData (occlusion.mCurrentCam)->queryVisible = isitVisible;
		    #endif //_VISIBILITYDEBUG    

	    }

		#ifdef _VISIBILITYDEBUG
	        else 
            {
	            occlusion.traversed_nodes_cnt++; 
                n.getNodeData (occlusion.mCurrentCam)->viewFrustumVisible = false;
		        occlusion.frustum_culled_nodes_cnt++;
	        }
		#endif //_VISIBILITYDEBUG    
    }
}
