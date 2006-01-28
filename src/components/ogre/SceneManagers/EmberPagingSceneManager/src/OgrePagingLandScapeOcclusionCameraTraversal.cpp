
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
    void RegisterCameraTraversal::onTree(PagingLandScapeOctree& n) const 
    {
        n.addCamNodeData (cam);
        traverseChildren (n);
    }
    //-----------------------------------------------------------------------
    void RegisterCameraTraversal::onLeaf(PagingLandScapeOctreeNode& n) const 
    {
        n.addCamNodeData (cam);
    }
    //-----------------------------------------------------------------------
    void UnregisterCameraTraversal::onTree(PagingLandScapeOctree& n) const 
    {
        n.removeCamNodeData (cam);
        traverseChildren (n);
    }
    //-----------------------------------------------------------------------
    void UnregisterCameraTraversal::onLeaf(PagingLandScapeOctreeNode& n) const 
    {
        n.removeCamNodeData (cam);
    } 
}