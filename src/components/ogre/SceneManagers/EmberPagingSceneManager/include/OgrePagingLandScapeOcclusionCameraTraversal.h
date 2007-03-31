#ifndef __PagingLandScapeOcclusionCameraTraversal_H
#define __PagingLandScapeOcclusionCameraTraversal_H

#include "OgrePagingLandScapePrerequisites.h"
#include "OgrePagingLandScapeOctreeCamera.h"

namespace Ogre
{
    //-----------------------------------------------------------------------
    // 
    class RegisterCameraTraversal : public TraversalConst
    {
	    private:
		    PagingLandScapeOctreeCamera *cam;

	    public:
		    RegisterCameraTraversal(PagingLandScapeOctreeCamera *c) : cam (c) {};
		    virtual void onTree(PagingLandScapeOctree& n, VisibleObjectsBoundsInfo * const visibleBounds ) const ;
		    virtual void onLeaf(PagingLandScapeOctreeNode& n, VisibleObjectsBoundsInfo * const visibleBounds ) const ;
	};
    //-----------------------------------------------------------------------
    // 
    class UnregisterCameraTraversal : public TraversalConst
    {
	    private:
		    PagingLandScapeOctreeCamera *cam;

	    public:
            UnregisterCameraTraversal(PagingLandScapeOctreeCamera *c) : cam (c) {};
		    virtual void onTree(PagingLandScapeOctree& n, VisibleObjectsBoundsInfo * const visibleBounds ) const ;
		    virtual void onLeaf(PagingLandScapeOctreeNode& n, VisibleObjectsBoundsInfo * const visibleBounds ) const ;
	};
  
}
#endif //PagingLandScapeOcclusionCameraTraversal
