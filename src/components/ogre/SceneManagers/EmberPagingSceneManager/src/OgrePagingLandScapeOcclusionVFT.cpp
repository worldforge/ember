
#include "OgrePagingLandScapePrecompiledHeaders.h"

#include "OgrePagingLandScapeOcclusionTraversal.h"
#include "OgrePagingLandScapeOcclusionVFT.h"

#include "OgrePagingLandScapeOcclusionVData.h"

#include "OgrePagingLandScapeOctreeCamera.h"
#include "OgrePagingLandScapeOctree.h"
#include "OgrePagingLandScapeOctreeNode.h"
#include "OgrePagingLandScapeOcclusionElement.h"
#include "OgrePagingLandScapeOcclusion.h"

namespace Ogre
{
    //-----------------------------------------------------------------------
    ViewFrustumCullingTraversal::ViewFrustumCullingTraversal  (Occlusion& o):
        occlusion(o), 
        mCurrentVisibility (PagingLandScapeOctreeCamera::NONE)
    {
        
    }
    //-----------------------------------------------------------------------
    void ViewFrustumCullingTraversal::onTree(PagingLandScapeOctree& node, VisibleObjectsBoundsInfo * const visibleBounds)
    {
        PagingLandScapeOctreeCamera::Visibility newVis = mCurrentVisibility;

        if (newVis != PagingLandScapeOctreeCamera::FULL)      
        {
            // box is not hierarchically culled as inside frustum box
            // so we need to check it.
            newVis = occlusion.mCurrentCam->getVisibility(node.getCullBoundingBox ());
             // if the octant is invisible
            if (newVis == PagingLandScapeOctreeCamera::NONE)
            {
                // Culled Octree.
                // outside frustum

                #ifdef _VISIBILITYDEBUG
                    node.getNodeData (occlusion.mCurrentCam)->viewFrustumVisible = false;
                    node.getNodeData (occlusion.mCurrentCam)->queryVisible = false; 
                    occlusion.frustum_culled_nodes_cnt++;
		            occlusion.traversed_nodes_cnt++;
                #endif //_VISIBILITYDEBUG

                return;
            }
        }
  
        #ifdef _VISIBILITYDEBUG
            node.getNodeData (occlusion.mCurrentCam)->viewFrustumVisible = true;
            node.getNodeData (occlusion.mCurrentCam)->queryVisible = true;
		    occlusion.traversed_nodes_cnt++;
        #endif //_VISIBILITYDEBUG

        // if we get there, then it's at least partially visible. 
        // Store parent Vis 
        PagingLandScapeOctreeCamera::Visibility oldVis = mCurrentVisibility;
        mCurrentVisibility = newVis;

        traverseChildren (node, visibleBounds);

        // Restore Parent Vis so that bothers can be pre-culled.
        mCurrentVisibility = oldVis; 
    }
    //-----------------------------------------------------------------------
    void ViewFrustumCullingTraversal::onLeaf(PagingLandScapeOctreeNode &node, VisibleObjectsBoundsInfo * const visibleBounds)
    {
        // if this PagingLandScapeOctree is partially visible, manually cull all
        // scene nodes attached directly to this level.
        PagingLandScapeOctreeCamera::Visibility newVis = mCurrentVisibility;
        if (newVis != PagingLandScapeOctreeCamera::FULL)      
        {
           newVis = occlusion.mCurrentCam->getVisibility(node.getCullBoundingBox ());
           // if the node is invisible
           if (newVis == PagingLandScapeOctreeCamera::NONE)
           {
                // Culled Node.
                // outside frustum

                #ifdef _VISIBILITYDEBUG
                    node.getNodeData (occlusion.mCurrentCam)->viewFrustumVisible = false;
                    node.getNodeData (occlusion.mCurrentCam)->queryVisible = false; 
                    occlusion.frustum_culled_nodes_cnt++;
		            occlusion.traversed_nodes_cnt++;
                #endif //_VISIBILITYDEBUG

                return;
           }
        } 

        #ifdef _VISIBILITYDEBUG
            node.getNodeData (occlusion.mCurrentCam)->viewFrustumVisible = true;
            node.getNodeData (occlusion.mCurrentCam)->queryVisible = true;
		    occlusion.traversed_nodes_cnt++;
            occlusion.object_cnt++;
        #endif //_VISIBILITYDEBUG

        // if we get there, then it's at least partially visible.  
        // Store parent Vis 
        PagingLandScapeOctreeCamera::Visibility oldVis = mCurrentVisibility;
        mCurrentVisibility = newVis;

        node._findVisibleObjects(occlusion.mCurrentCam,
                                occlusion.mCurrentRenderQueue, visibleBounds, true, false,
                                occlusion.mOnlyShadowCaster );


        // Restore Parent Vis so that bothers can be pre-culled.
        mCurrentVisibility = oldVis; 
    }


     //-----------------------------------------------------------------------
    ViewFrustumCullingTraversalDirect::ViewFrustumCullingTraversalDirect  (Occlusion& o):
        occlusion(o)
    {
        
    }
    //-----------------------------------------------------------------------
    void ViewFrustumCullingTraversalDirect::onTree(PagingLandScapeOctree& node, VisibleObjectsBoundsInfo * const visibleBounds)
    {
        if (occlusion.mCurrentCam->isVisible(node.getCullBoundingBox ()))      
        {          

            #ifdef _VISIBILITYDEBUG
		        occlusion.traversed_nodes_cnt++;
                node.getNodeData (occlusion.mCurrentCam)->viewFrustumVisible = true;
                node.getNodeData (occlusion.mCurrentCam)->queryVisible = true;
            #endif //_VISIBILITYDEBUG

            // if we get there, then it's at least partially visible.   
            traverseChildren (node, visibleBounds);     
        } 
        #ifdef _VISIBILITYDEBUG
            else
            {
                // Culled Octree.
                // outside frustum
		        occlusion.traversed_nodes_cnt++;
                occlusion.frustum_culled_nodes_cnt++;
                node.getNodeData (occlusion.mCurrentCam)->viewFrustumVisible = false;
                node.getNodeData (occlusion.mCurrentCam)->queryVisible = false;     
            }   
        #endif //_VISIBILITYDEBUG     
    }
    //-----------------------------------------------------------------------
    void ViewFrustumCullingTraversalDirect::onLeaf(PagingLandScapeOctreeNode &node, VisibleObjectsBoundsInfo * const visibleBounds)
    {
        // if this PagingLandScapeOctree is partially visible, manually cull all
        // scene nodes attached directly to this level.
        if (occlusion.mCurrentCam->isVisible(node.getCullBoundingBox ()))      
        {          
            #ifdef _VISIBILITYDEBUG
		        occlusion.traversed_nodes_cnt++;
                node.getNodeData (occlusion.mCurrentCam)->viewFrustumVisible = true;
                node.getNodeData (occlusion.mCurrentCam)->queryVisible = true;
                occlusion.object_cnt++;
            #endif //_VISIBILITYDEBUG

            // if we get there, then it's at least partially visible.  
            node._findVisibleObjects(occlusion.mCurrentCam,
                                occlusion.mCurrentRenderQueue, visibleBounds, true, false,
                                occlusion.mOnlyShadowCaster );

        } 
        #ifdef _VISIBILITYDEBUG
            else
            {
                // Culled Octree.
                // outside frustum
		            occlusion.traversed_nodes_cnt++;
                    occlusion.frustum_culled_nodes_cnt++;
                    node.getNodeData (occlusion.mCurrentCam)->viewFrustumVisible = false;
                    node.getNodeData (occlusion.mCurrentCam)->queryVisible = false; 
            }
        #endif //_VISIBILITYDEBUG    
    }

}
