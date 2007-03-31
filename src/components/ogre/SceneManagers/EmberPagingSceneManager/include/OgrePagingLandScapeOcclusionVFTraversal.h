#ifndef __PagingLandScapeOcclusionViewFrustumCullingTraversal_H
#define __PagingLandScapeOcclusionViewFrustumCullingTraversal_H

#include "OgrePagingLandScapePrerequisites.h"
#include "OgrePagingLandScapeOctreeCamera.h"

#include "OgrePagingLandScapeOcclusionTraversal.h"

namespace Ogre
{
    //-----------------------------------------------------------------------
    // Basic View Frustum Culling using hierarchy info 
    // as if an octree is fully in frustum, all children are for sure visible
	class ViewFrustumCullingTraversal : public Traversal 
    {
	    public:

		    ViewFrustumCullingTraversal (Occlusion& o);

            virtual void onTree(PagingLandScapeOctree&, VisibleObjectsBoundsInfo * const visibleBounds );
            virtual void onLeaf(PagingLandScapeOctreeNode&, VisibleObjectsBoundsInfo * const visibleBounds );
	    private:

		    Occlusion& occlusion;
            PagingLandScapeOctreeCamera::Visibility mCurrentVisibility;

	};
  
    //-----------------------------------------------------------------------
    // Basic View Frustum Culling
	class ViewFrustumCullingTraversalDirect : public Traversal 
    {

	    public:

		    ViewFrustumCullingTraversalDirect (Occlusion& o);

            virtual void onTree(PagingLandScapeOctree&, VisibleObjectsBoundsInfo * const visibleBounds );
            virtual void onLeaf(PagingLandScapeOctreeNode&, VisibleObjectsBoundsInfo * const visibleBounds );
	    private:

		    Occlusion& occlusion;
	};
}
#endif //__PagingLandScapeOcclusionViewFrustumCullingTraversal_H
