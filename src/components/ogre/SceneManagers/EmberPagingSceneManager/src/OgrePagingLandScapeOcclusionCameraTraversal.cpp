
#include "OgrePagingLandScapePrecompiledHeaders.h"

#include "OgrePagingLandScapeOcclusionTraversal.h"

#include "OgrePagingLandScapeOctreeCamera.h"
#include "OgrePagingLandScapeOctree.h"
#include "OgrePagingLandScapeOctreeNode.h"
#include "OgrePagingLandScapeOcclusionElement.h"
#include "OgrePagingLandScapeOcclusion.h"

#include "OgrePagingLandScapeOcclusionTraversal.h"
#include "OgrePagingLandScapeOcclusionCameraTraversal.h"

namespace Ogre
{
    //-----------------------------------------------------------------------
    void RegisterCameraTraversal::onTree(PagingLandScapeOctree& n, VisibleObjectsBoundsInfo * const visibleBounds) const 
    {
        n.addCamNodeData (cam);
        traverseChildren (n, visibleBounds);
    }
    //-----------------------------------------------------------------------
    void RegisterCameraTraversal::onLeaf(PagingLandScapeOctreeNode& n, VisibleObjectsBoundsInfo * const visibleBounds) const 
    {
        n.addCamNodeData (cam);
    }
    //-----------------------------------------------------------------------
    void UnregisterCameraTraversal::onTree(PagingLandScapeOctree& n, VisibleObjectsBoundsInfo * const visibleBounds) const 
    {
        n.removeCamNodeData (cam);
        traverseChildren (n, visibleBounds);
    }
    //-----------------------------------------------------------------------
    void UnregisterCameraTraversal::onLeaf(PagingLandScapeOctreeNode& n, VisibleObjectsBoundsInfo * const visibleBounds) const 
    {
        n.removeCamNodeData (cam);
    } 
}
