
#include "OgrePagingLandScapePrecompiledHeaders.h"

#include <stack>
#include <queue>

#include "OgrePagingLandScapeOcclusionElement.h"

#include "OgrePagingLandScapePrerequisites.h"
#include "OgrePagingLandScapeOctreeCamera.h"
#include "OgrePagingLandScapeOcclusionVData.h"
#include "OgreDebugRectangle2D.h"


namespace Ogre
{
    //---------------------------------------------------------------------
    OcclusionElement::OcclusionElement() :
    	#ifdef _VISIBILITYDEBUG
            mDebugRectangle2d(0),
        #endif //_VISIBILITYDEBUG  
        mIsRegisteredToCam (false)
    {
    } 
    //---------------------------------------------------------------------
    OcclusionElement::~OcclusionElement() 
    {
    	#ifdef _VISIBILITYDEBUG
            delete mDebugRectangle2d;
        #endif //_VISIBILITYDEBUG  
        
        NodeDataPerCamMapiterator i = nodeDataPerCam.begin ();
        while (i != nodeDataPerCam.end())
        {
            delete i->second;
            ++i;
        }
        nodeDataPerCam.clear();
    }  
    //---------------------------------------------------------------------
    VisibilityData *OcclusionElement::getNodeData(PagingLandScapeOctreeCamera *cam)
    {
        if (nodeDataPerCam.find(cam->getId()) == nodeDataPerCam.end())
            nodeDataPerCam[cam->getId()] = new VisibilityData();
        //assert (nodeDataPerCam.find(cam->getId()) != nodeDataPerCam.end());
        return nodeDataPerCam[cam->getId()];
    }
    //---------------------------------------------------------------------
    void OcclusionElement::addCamNodeData (PagingLandScapeOctreeCamera *cam)
    {
        if (nodeDataPerCam.find(cam->getId()) == nodeDataPerCam.end())
            nodeDataPerCam[cam->getId()] = new VisibilityData();
    }
    //---------------------------------------------------------------------
    void OcclusionElement::removeCamNodeData (PagingLandScapeOctreeCamera *cam)
    {
        NodeDataPerCamMapiterator i = nodeDataPerCam.find (cam->getId());
        if (i != nodeDataPerCam.end())
        {
            delete i->second;
	        nodeDataPerCam.erase (i);
        }
    }
    #ifdef _VISIBILITYDEBUG
        //---------------------------------------------------------------------
        DebugRectangle2D *OcclusionElement::getRectangle2d(SceneManager *scnMgr)
        {
            if (!mDebugRectangle2d)
            {
                mDebugRectangle2d = new DebugRectangle2D();
                //scnMgr->getRootSceneNode ()->createChildSceneNode ()->attachObject (mDebugRectangle2d);
            }
            return mDebugRectangle2d;
        }
    #endif //_VISIBILITYDEBUG   
}//namespace Ogre
