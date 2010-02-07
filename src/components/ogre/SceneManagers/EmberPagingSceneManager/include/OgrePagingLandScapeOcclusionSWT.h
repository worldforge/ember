#ifndef __PagingLandScapeOcclusionSWTraversal_H
#define __PagingLandScapeOcclusionSWTraversal_H

#include "OgrePagingLandScapePrerequisites.h"
#include "OgrePagingLandScapeOctreeCamera.h"

#include "OgrePagingLandScapeOcclusionTraversal.h"

namespace Ogre
{
    //-----------------------------------------------------------------------
    // TREE TRAVERSAL classes 
    // a OctreeNode with Geometry objects in the leaves 
    // and a TreeNodeData struct at every node
    //-----------------------------------------------------------------------
    // Basic Occlusion query use : stop and wait query result Culling
	class SWTraversal : public Traversal 
    {
	    private:

		    Occlusion& occlusion;
            PagingLandScapeOctreeCamera::Visibility mCurrentVisibility;
            Vector3 camPos;

	    public:

		    SWTraversal (Occlusion& o);

            virtual void onTree(PagingLandScapeOctree&, VisibleObjectsBoundsInfo * const visibleBounds );
            virtual void onLeaf(PagingLandScapeOctreeNode&, VisibleObjectsBoundsInfo * const visibleBounds );
            bool isVisible(OcclusionElement & n);
            void traverseChildren(PagingLandScapeOctree & n, VisibleObjectsBoundsInfo * const visibleBounds );
	};
    
}
#endif //PagingLandScapeOcclusionSWTraversal
