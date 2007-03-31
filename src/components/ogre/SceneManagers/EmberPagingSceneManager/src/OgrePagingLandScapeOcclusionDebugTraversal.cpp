
#include "OgrePagingLandScapePrecompiledHeaders.h"

#include "OgrePagingLandScapePrerequisites.h"

#include "OgrePagingLandScapeOctreeCamera.h"
#include "OgrePagingLandScapeOctree.h"
#include "OgrePagingLandScapeOctreeNode.h"
#include "OgrePagingLandScapeOcclusionElement.h"
#include "OgrePagingLandScapeOcclusion.h"

#include "OgrePagingLandScapeOcclusionVisibilityData.h"
#include "OgrePagingLandScapeOcclusionTraversal.h"
#include "OgrePagingLandScapeOcclusionDebugTraversal.h"

#include "OgreDebugRectangle2D.h"

#ifdef _VISIBILITYDEBUG
namespace Ogre
{
    // Debug Info 
    //
        //-----------------------------------------------------------------------
        TreeOverlayDebug::TreeOverlayDebug(Occlusion& o, PagingLandScapeOctreeSceneManager *scnMgr):
                occlusion(o),
                mScnMrg (scnMgr)
        {
        }
        //-----------------------------------------------------------------------
        void TreeOverlayDebug::onTree(PagingLandScapeOctree& n, VisibleObjectsBoundsInfo * const visibleBounds) const 
		{    
			SimpleRenderable * const s = n.getRectangle2d(mScnMrg);
            if (n.getNodeData (occlusion.mCurrentCam)->viewFrustumVisible)
            {
			    if (n.getNodeData (occlusion.mCurrentCam)->queryVisible) 
                {
                    s->setMaterial ("BaseWhiteNoLightNoDepthCheck");
                    occlusion.mCurrentRenderQueue->addRenderable (s, s->getRenderQueueGroup ());

                    traverseChildren (n, visibleBounds);
			    }
			    else 
                {
                    s->setMaterial ("BaseRedNoLightNoDepthCheck");
                    occlusion.mCurrentRenderQueue->addRenderable (s, s->getRenderQueueGroup ());
			    }
		    }
		    else 
			{
                s->setMaterial ("BaseGreenNoLightNoDepthCheck");
                occlusion.mCurrentRenderQueue->addRenderable (s, s->getRenderQueueGroup ());
		    }       
        }
        //-----------------------------------------------------------------------
	    void TreeOverlayDebug::onLeaf(PagingLandScapeOctreeNode& n, VisibleObjectsBoundsInfo * const visibleBounds) const 
		{
			SimpleRenderable * const s = n.getRectangle2d(mScnMrg);
		    if(n.getNodeData (occlusion.mCurrentCam)->viewFrustumVisible)
            {
			    if(n.getNodeData (occlusion.mCurrentCam)->queryVisible) 
                {
                    s->setMaterial ("BaseWhiteNoLightNoDepthCheck");
                    occlusion.mCurrentRenderQueue->addRenderable(s, s->getRenderQueueGroup ());          
			    }
			    else 
                {
                    s->setMaterial ("BaseRedNoLightNoDepthCheck");
                    occlusion.mCurrentRenderQueue->addRenderable(s, s->getRenderQueueGroup ());
			    }
		    }
            else 
            { 
                s->setMaterial ("BaseGreenNoLightNoDepthCheck");
                occlusion.mCurrentRenderQueue->addRenderable(s, s->getRenderQueueGroup ());
		    }
	    }
}
#endif //_VISIBILITYDEBUG   
