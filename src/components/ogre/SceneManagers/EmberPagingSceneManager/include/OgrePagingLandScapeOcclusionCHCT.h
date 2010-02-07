#ifndef __PagingLandScapeOcclusionCHCTraversal_H
#define __PagingLandScapeOcclusionCHCTraversal_H

#include "OgrePagingLandScapePrerequisites.h"
#include "OgrePagingLandScapeOctreeCamera.h"

#include "OgrePagingLandScapeOcclusionTraversal.h"

namespace Ogre
{
    //-----------------------------------------------------------------------
    // TREE TRAVERSAL classes 
    // a OctreeNode with Geometry objects in the leaves 
    // and a TreeNodeData struct at every node
    class CHCTraversal : public TraversalConst 
    {
	    public:
		    virtual void onTree(PagingLandScapeOctree& n, VisibleObjectsBoundsInfo * const visibleBounds ) const;
		    virtual void onLeaf(PagingLandScapeOctreeNode& n, VisibleObjectsBoundsInfo * const visibleBounds ) const ;

		    CHCTraversal(FrontToBackNodeSorterPriorityQueue& vStack, Occlusion& o);

	    private:
		    FrontToBackNodeSorterPriorityQueue& stack;
		    Occlusion& occlusion;
		    const unsigned int frameId;
			const bool mIsNotSolidScene;
    };
}
#endif //PagingLandScapeOcclusionCHCTraversal
