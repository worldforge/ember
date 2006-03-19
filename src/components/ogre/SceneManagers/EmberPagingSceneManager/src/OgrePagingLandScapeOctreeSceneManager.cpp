/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2005 The OGRE Team
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------
*/
/***************************************************************************
PagingLandScapeOctreescenemanager.cpp  -  description
-------------------
begin                : Fri Sep 27 2002
copyright            : (C) 2002 by Jon Anderson
email                : janders@users.sf.net
 
Enhancements 2003 - 2004 (C) The OGRE Team
 
***************************************************************************/

#include "OgrePagingLandScapeOctreeSceneManager.h"

#include "OgrePagingLandScapeOctreeAxisAlignedBoxSceneQuery.h"
#include "OgrePagingLandScapeOctreeIntersectionSceneQuery.h"
#include "OgrePagingLandScapeOctreePlaneBoundedVolumeListSceneQuery.h"
#include "OgrePagingLandScapeOctreeRaySceneQuery.h"
#include "OgrePagingLandScapeOctreeSphereSceneQuery.h"
#include "OgreRenderSystem.h"
#include "OgreWireBoundingBox.h"
#include "OgreRenderOperation.h"
#include "OgreStringConverter.h"

#include "OgreOverlayManager.h"
#include "OgreOverlayElement.h"

#include "OgrePagingLandScapeOctreeCamera.h"
#include "OgrePagingLandScapeOctree.h"
#include "OgrePagingLandScapeOctreeNode.h"

#include "OgreOcclusionBoundingBox.h"
#include "OgrePagingLandScapeOcclusionSorter.h"
#include "OgrePagingLandScapeOcclusionVisibilityData.h"
#include "OgrePagingLandScapeOcclusionTraversal.h"
#include "OgrePagingLandScapeOcclusionCameraTraversal.h"
#include "OgrePagingLandScapeOcclusionVFTraversal.h"
#include "OgrePagingLandScapeOcclusionSWTraversal.h"
#include "OgrePagingLandScapeOcclusionDebugTraversal.h"



extern "C"
{
    void findNodesInBox(Ogre::SceneManager *sm,
                         const Ogre::AxisAlignedBox &box,
                         std::list < Ogre::SceneNode * > &list,
                         const Ogre::SceneNode * const exclude)
    {
        static_cast<Ogre::PagingLandScapeOctreeSceneManager*>(sm)->findNodesIn(box, list, exclude);
    }
    void findNodesInSphere(Ogre::SceneManager *sm,
                            const Ogre::Sphere &sphere,
                            std::list < Ogre::SceneNode * > &list,
                            const Ogre::SceneNode * const exclude)
    {
        static_cast<Ogre::PagingLandScapeOctreeSceneManager*>(sm)->findNodesIn(sphere, list, exclude);
    }
}

namespace Ogre
{
    //---------------------------------------------------------------------
    enum Intersection
    {
        OUTSIDE=0,
        INSIDE=1,
        INTERSECT=2
    };
    int PagingLandScapeOctreeSceneManager::intersect_call = 0;
    //---------------------------------------------------------------------
    Intersection intersect(const Ray &one, const AxisAlignedBox &two)
    {
        PagingLandScapeOctreeSceneManager::intersect_call++;

        // Null box?
        if (two.isNull()) 
            return OUTSIDE;


        const Vector3* const pCorners = two.getAllCorners();
        const Vector3 origin = one.getOrigin();
        const Vector3 dir = one.getDirection();

        Vector3 maxT (-1.0f, -1.0f, -1.0f);

        int i = 0;
        bool inside = true;
        for (i = 0; i < 3; i++)
        {
            if(origin[i] < pCorners[0][i])
            {
                inside = false;
                if(dir[i] > 0)
                {
                    maxT[i] = (pCorners[0][i] - origin[i]) / dir[i];
                }
            }
            else if(origin[i] > pCorners[4][i])
            {
                inside = false;
                if(dir[i] < 0)
                {
                    maxT[i] = (pCorners[4][i] - origin[i]) / dir[i];
                }
            }
        }

        if(inside)
        {
            return INTERSECT;
        }
        int whichPlane = 0;
        if(maxT[1] > maxT[whichPlane])
            whichPlane = 1;
        if(maxT[2] > maxT[whichPlane])
            whichPlane = 2;

        if(((int)maxT[whichPlane]) & 0x80000000)
        {
            return OUTSIDE;
        }
        const Real maxTThisPlane = maxT[whichPlane];
        for(i = 0; i < 3; i++)
        {
            if(i != whichPlane)
            {
                const Real f = origin[i] + maxTThisPlane * dir[i];
                if (f < (pCorners[0][i] - 0.00001f) ||
                        f > (pCorners[4][i] + 0.00001f))
                {
                    return OUTSIDE;
                }
            }
        }

        return INTERSECT;

    }
    //---------------------------------------------------------------------
    /** Checks how the second box intersects with the first.
    */
    Intersection intersect(const PlaneBoundedVolume &one, const AxisAlignedBox &two)
    {
        PagingLandScapeOctreeSceneManager::intersect_call++;
        // Null box?
        if (two.isNull()) 
            return OUTSIDE;

        // Get corners of the box
        const Vector3 * const pCorners = two.getAllCorners();

        // For each plane, see if all points are on the negative side
        // If so, object is not visible.
        // If one or more are, it's partial.
        // If all aren't, full
        static const int corners[ 8 ] = {0, 4, 3, 5, 2, 6, 1, 7};

        bool all_inside = true;
        PlaneList::const_iterator i, iend = one.planes.end();
        for (i = one.planes.begin(); i != iend; ++i)
        {
            const Plane& plane = *i;
            bool all_outside = true;
            for (unsigned int corner = 0; corner < 8; ++corner)
            {
                const Real distance = plane.getDistance(pCorners[ corners[ corner ] ]);
                all_outside = all_outside && (distance < 0);
                all_inside = all_inside && (distance >= 0);

                if (!all_outside && !all_inside)
                    break;
            }

            if (all_outside)
                return OUTSIDE;
        }

        if (all_inside)
            return INSIDE;
        else
            return INTERSECT;

    }
    //---------------------------------------------------------------------
    /** Checks how the second box intersects with the first.
    */
    Intersection intersect(const AxisAlignedBox &one, const AxisAlignedBox &two)
    {
        PagingLandScapeOctreeSceneManager::intersect_call++;

        // Null box?
        if (one.isNull() || two.isNull()) 
            return OUTSIDE;

        const Vector3 * outside = one.getAllCorners();
        const Vector3 *inside = two.getAllCorners();

        if (inside[ 4 ].x < outside[ 0 ].x ||
                inside[ 4 ].y < outside[ 0 ].y ||
                inside[ 4 ].z < outside[ 0 ].z ||
                inside[ 0 ].x > outside[ 4 ].x ||
                inside[ 0 ].y > outside[ 4 ].y ||
                inside[ 0 ].z > outside[ 4 ].z)
        {
            return OUTSIDE;
        }

        const bool full = (inside[ 0 ].x > outside[ 0 ].x &&
                    inside[ 0 ].y > outside[ 0 ].y &&
                    inside[ 0 ].z > outside[ 0 ].z &&
                    inside[ 4 ].x < outside[ 4 ].x &&
                    inside[ 4 ].y < outside[ 4 ].y &&
                    inside[ 4 ].z < outside[ 4 ].z);

        if (full)
            return INSIDE;
        else
            return INTERSECT;

    }
    //---------------------------------------------------------------------
    /** Checks how the box intersects with the sphere.
    */
    Intersection intersect(const Sphere &one, const AxisAlignedBox &two)
    {
        PagingLandScapeOctreeSceneManager::intersect_call++;

        // Null box?
        if (two.isNull()) 
            return OUTSIDE;


        const Real sradius = Math::Sqr (one.getRadius());
        const Vector3 scenter = one.getCenter();
        const Vector3 * const corners = two.getAllCorners();

        const Vector3 mndistance = (corners[ 0 ] - scenter);
        const Vector3 mxdistance = (corners[ 4 ] - scenter);


        if (mndistance.squaredLength() < sradius &&
                mxdistance.squaredLength() < sradius)
        {
            return INSIDE;
        }

        //find the square of the distance
        //from the sphere to the box
        Real d = 0;
        for (uint i = 0 ; i < 3 ; i++)
        {
            const Real sCenteri = scenter[ i ];
            if (sCenteri < corners[ 0 ][ i ])
            {
                const Real s = sCenteri - corners[ 0 ][ i ];
                d += s * s;
            }
            else if (sCenteri > corners[ 4 ][ i ])
            {
                const Real s = sCenteri - corners[ 4 ][ i ];
                d += s * s;
            }

        }

        const bool partial = (d <= sradius);

        if (!partial)
        {
            return OUTSIDE;
        }

        else
        {
            return INTERSECT;
        }


    }
    //-----------------------------------------------------------------------
    //-----------------------------------------------------------------------
    //-----------------------------------------------------------------------
    const String PagingLandScapeOctreeSceneManagerFactory::FACTORY_TYPE_NAME = "PagingLandScapeOctreeSceneManager";
    //-----------------------------------------------------------------------
    void PagingLandScapeOctreeSceneManagerFactory::initMetaData(void) const
    {
       mMetaData.typeName = FACTORY_TYPE_NAME;
       mMetaData.description = "Scene manager organising the scene on the basis of an octree, possibly using occlusion culling.";
       mMetaData.sceneTypeMask = ST_EXTERIOR_REAL_FAR; // support all types
       mMetaData.worldGeometrySupported = false;
    }
    //-----------------------------------------------------------------------
    SceneManager* PagingLandScapeOctreeSceneManagerFactory::createInstance(
        const String& instanceName)
    {
        return new PagingLandScapeOctreeSceneManager(instanceName);
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeOctreeSceneManagerFactory::destroyInstance(SceneManager* instance)
    {
        delete instance;
    }
    //---------------------------------------------------------------------
    PagingLandScapeOctreeSceneManager::PagingLandScapeOctreeSceneManager(const String& name): 
        SceneManager(name),
		mPagingLandScapeOctree(0),
		mCurrentOptionCamera(0)
    {
        AxisAlignedBox b(-10000, -10000, -10000, 10000, 10000, 10000);
        int depth = 8; 
        init(b, depth);
    }
    //---------------------------------------------------------------------
    PagingLandScapeOctreeSceneManager::PagingLandScapeOctreeSceneManager(const String& name, AxisAlignedBox &box, int max_depth) : 
        SceneManager(name),
        mPagingLandScapeOctree(0)
    {
        init(box, max_depth);
    }
    //---------------------------------------------------------------------
    const String& PagingLandScapeOctreeSceneManager::getTypeName(void) const
    {
     	return PagingLandScapeOctreeSceneManagerFactory::FACTORY_TYPE_NAME;
    }
    //---------------------------------------------------------------------
    void PagingLandScapeOctreeSceneManager::init(const AxisAlignedBox &box, int depth)
    {
        delete mSceneRoot; //get rid of old root.

        // -- Changes by Steve
        // Don't do it this way, it will add it to the mSceneNodes which we don't want
        //mSceneRoot = createSceneNode("SceneRoot");
        mSceneRoot = new PagingLandScapeOctreeNode(this, "SceneRoot");
	    mSceneRoot->_notifyRootNode();
        // -- End changes by Steve

        delete mPagingLandScapeOctree;

        mPagingLandScapeOctree = new PagingLandScapeOctree(0);


        mMaxDepth = depth;
        mBox = box;

        //mSceneRoot isn't put into the PagingLandScapeOctree since it has no volume.   
        mPagingLandScapeOctree->setBoundingBox (box.getMinimum(), box.getMaximum());

        #ifdef _VISIBILITYDEBUG
            mShowBoxes = false;
            mCullCamera = false;
            //     setDisplaySceneNodes(true);
        #endif //_VISIBILITYDEBUG
        
     
        mOcclusion.init (this);
        mOcclusionDepth = 0;

    }
    //---------------------------------------------------------------------
    PagingLandScapeOctreeSceneManager::~PagingLandScapeOctreeSceneManager()
    {
        // -- Changed by Steve
        // Don't do this here, SceneManager will do it
        /*
        if(mSceneRoot)
        delete mSceneRoot;
        */ 
        // --End Changes by Steve

        delete mPagingLandScapeOctree;

        VisiblesPerCam::iterator i = mVisibles.begin ();
        while (i != mVisibles.end())
        {            
           delete (i->second);
           ++i;
        }
        mVisibles.clear();
    }
    //---------------------------------------------------------------------
    Camera * PagingLandScapeOctreeSceneManager::createCamera(const String &name)
    {
        if (mCameras.find(name) != mCameras.end())
        {
            OGRE_EXCEPT(
                Exception::ERR_DUPLICATE_ITEM,
                "A camera with the name " + name + " already exists",
                "PagingLandScapeSceneManager::createCamera");
        }
        Camera * c = new PagingLandScapeOctreeCamera(name, this);
        mCameras.insert(CameraList::value_type(name, c));
		addCamera (c);
        return c;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeOctreeSceneManager::addCamera(Camera *cam)
	{
		PagingLandScapeOctreeCamera *c = static_cast <PagingLandScapeOctreeCamera *> (cam);
		mVisibles[c->getId ()] = new MovableObjectList();
        if (!c->isRegisteredInOcclusionSystem()
			&& c->needRegistrationInOcclusionSystem())
        {
            mPagingLandScapeOctree->traversal(RegisterCameraTraversal(c));
            c->setRegisteredInOcclusionSystem(true);
        }
		if (!mCurrentOptionCamera)
			mCurrentOptionCamera = c;
	}
	//-----------------------------------------------------------------------
	void PagingLandScapeOctreeSceneManager::unregisterCamera(PagingLandScapeOctreeCamera *c)
	{
		VisiblesPerCam::iterator i = mVisibles.find (c->getId ());
		if (i != mVisibles.end())
		{            
			delete (i->second);
			mVisibles.erase(i);
		}
		if (c->isRegisteredInOcclusionSystem ())
		{
			mPagingLandScapeOctree->traversal (UnregisterCameraTraversal(c));
			c->setRegisteredInOcclusionSystem (false);                   
		} 
		if (mCurrentOptionCamera == c)
			c = 0;
	}
    //-----------------------------------------------------------------------
    void PagingLandScapeOctreeSceneManager::destroyCamera(Camera *cam)
    {
        unregisterCamera (static_cast <PagingLandScapeOctreeCamera *> (cam));
        SceneManager::destroyCamera(cam);
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeOctreeSceneManager::destroyCamera(const String& name)
    {
		// Find in list
       CameraList::iterator i = mCameras.find(name);
       if (i != mCameras.end())
	   {
          destroyCamera (i->second);
       }                  
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeOctreeSceneManager::destroyAllCameras(void)
    {
		CameraList::iterator i = mCameras.begin();
		for (; i != mCameras.end(); ++i)
		{
			unregisterCamera (static_cast <PagingLandScapeOctreeCamera *> (i->second));
			// Notify render system
			mDestRenderSystem->_notifyCameraRemoved(i->second);
			delete i->second;
		}
        mCameras.clear ();
    } 
    //---------------------------------------------------------------------
    void PagingLandScapeOctreeSceneManager::destroySceneNode(const String &name)
    {
        PagingLandScapeOctreeNode * on = static_cast < PagingLandScapeOctreeNode* > (SceneManager::getSceneNode(name));

        if (on != 0)
            _removePagingLandScapeOctreeNode(on);

        SceneManager::destroySceneNode(name);
    }
    //---------------------------------------------------------------------
    bool PagingLandScapeOctreeSceneManager::getOptionValues(const String & key, StringVector  &refValueList)
    {
        return SceneManager::getOptionValues(key, refValueList);
    }
    //---------------------------------------------------------------------
    bool PagingLandScapeOctreeSceneManager::getOptionKeys(StringVector & refKeys)
    {
        SceneManager::getOptionKeys(refKeys);

        refKeys.push_back("CullCamera");
        refKeys.push_back("Size");
        refKeys.push_back("ShowPagingLandScapeOctree");
        refKeys.push_back("Depth");

        return true;
    }
    //---------------------------------------------------------------------
    void PagingLandScapeOctreeSceneManager::_updatePagingLandScapeOctreeNode(PagingLandScapeOctreeNode * onode)
    {
        const AxisAlignedBox box = onode->_getWorldAABB();

        if (box.isNull())
            return ;

        assert (onode && mPagingLandScapeOctree);

        if (onode->getOctant() == 0)
        {
            //if outside the PagingLandScapeOctree, force into the root node.
            if (! onode->_isIn(mPagingLandScapeOctree->getBoundingBox()))
                mPagingLandScapeOctree->_addNode(onode);
            else
                _addPagingLandScapeOctreeNode(onode, mPagingLandScapeOctree);
            return ;
        }

        if (! onode->_isIn(onode->getOctant()->getBoundingBox()))
        {
            _removePagingLandScapeOctreeNode(onode);

            //if outside the PagingLandScapeOctree, force into the root node.
            if (! onode->_isIn(mPagingLandScapeOctree->getBoundingBox()))
                mPagingLandScapeOctree->_addNode(onode);
            else
                _addPagingLandScapeOctreeNode(onode, mPagingLandScapeOctree);
        }
    }
    //---------------------------------------------------------------------
    /** Only removes the node from the PagingLandScapeOctree.  It leaves the PagingLandScapeOctree, even if it's empty.
    */
    void PagingLandScapeOctreeSceneManager::_removePagingLandScapeOctreeNode(PagingLandScapeOctreeNode * n)
    {
		assert (n);
		PagingLandScapeOctree * oct = n->getOctant();
		if (oct)
			oct->_removeNode (n);
		n->setOctant (0);
		unregisteredNodeInCamera (n);
    }
    //---------------------------------------------------------------------
    void PagingLandScapeOctreeSceneManager::_addPagingLandScapeOctreeMovableNode(PagingLandScapeOctreeNode * n, PagingLandScapeOctree *octant, int depth)
    {
        const AxisAlignedBox bx = n->_getWorldAABB();

        //if the PagingLandScapeOctree is twice as big as the scene node,
        //we will add it to a child.
        if ((depth < mMaxDepth) && octant->_isTwiceSize (bx))
        {
            _addPagingLandScapeOctreeMovableNode(n, 
                                          octant->_getChildWhereBoxFits(bx, this), 
                                          ++depth);
        }
        else
        {  
            #ifdef _VISIBILITYDEBUG   
                n ->setDebugCorners(this);
            #endif //_VISIBILITYDEBUG    
            octant->_addNode(n);
        }
    }
    //---------------------------------------------------------------------
    void PagingLandScapeOctreeSceneManager::_addPagingLandScapeOctreeStaticNode(PagingLandScapeOctreeNode * n, PagingLandScapeOctree *octant, int depth)
    {
        const AxisAlignedBox bx = n->_getWorldAABB();

        //if the PagingLandScapeOctree is twice as big as the scene node,
        //we will add it to a child.
        if ((depth < mMaxDepth) && octant->_isTwiceCullSize (bx))
        {
            if (octant->_isNotCrossingAxes(bx))
			{
                _addPagingLandScapeOctreeStaticNode(n, 
                                                    octant->_getCullChildWhereBoxFits(bx, this), 
                                                    ++depth);
			}
            else
            {
                // re-insert it as a moving node, therefore avoiding crossing problem.
                n->setStaticCulling (false);
                _addPagingLandScapeOctreeMovableNode(n, 
                                                    mPagingLandScapeOctree, 
                                                    0);
            }
        }
        else
        {  
            #ifdef _VISIBILITYDEBUG   
                n ->setDebugCorners(this);
            #endif //_VISIBILITYDEBUG    
            octant->_addNode(n);
        }
    }
    //---------------------------------------------------------------------
    void PagingLandScapeOctreeSceneManager::_addPagingLandScapeOctreeNode(PagingLandScapeOctreeNode * n, PagingLandScapeOctree *octant, int depth)
    {
        assert (n && octant);
        if (n->isStaticNode ())
            _addPagingLandScapeOctreeStaticNode(n, octant, depth);
        else
			_addPagingLandScapeOctreeMovableNode(n, octant, depth);
		registeredNodeInCamera (n);
    }
    //---------------------------------------------------------------------
    SceneNode * PagingLandScapeOctreeSceneManager::createSceneNode(void)
	{
        PagingLandScapeOctreeNode * on = new PagingLandScapeOctreeNode(this);
		mSceneNodes[ on->getName() ] = on; 
        return on;
    }
    //---------------------------------------------------------------------
    SceneNode * PagingLandScapeOctreeSceneManager::createSceneNode(const String &name)
    {
		// Check name not used
	    if (mSceneNodes.find(name) != mSceneNodes.end())
	    {
		    OGRE_EXCEPT(
			        Exception::ERR_DUPLICATE_ITEM,
			        "A scene node with the name " + name + " already exists",
			        "PagingLandScapeOctreeSceneManager::createSceneNode" );
		}
        PagingLandScapeOctreeNode * on = new PagingLandScapeOctreeNode(this, name);
		mSceneNodes[ on->getName() ] = on; 
        return on;
    }
    //---------------------------------------------------------------------
    void PagingLandScapeOctreeSceneManager::registeredNodeInCamera(OcclusionElement *on)
    {
         // register existing camera in nodes
        bool isRegistered = false;
        for (CameraList::iterator it = mCameras.begin(); it != mCameras.end(); ++it)
        {
            // Notify occlusion system
            PagingLandScapeOctreeCamera *c = static_cast <PagingLandScapeOctreeCamera *> (it->second);
            assert (c);
            if (c->isRegisteredInOcclusionSystem ())
            {
                on->traversal (RegisterCameraTraversal(c));
                isRegistered = true;
            }
        }
        on->setRegisteredtoCam (isRegistered);      
	}
    //---------------------------------------------------------------------
    void PagingLandScapeOctreeSceneManager::unregisteredNodeInCamera(OcclusionElement *on)
    {
         // register existing camera in nodes
        for (CameraList::iterator it = mCameras.begin();it != mCameras.end(); ++it)
        {
            // Notify occlusion system
            PagingLandScapeOctreeCamera *c = static_cast <PagingLandScapeOctreeCamera *> (it->second);
            assert (c);
            if (c->isRegisteredInOcclusionSystem ())
                on->traversal (UnregisterCameraTraversal(c));
        }   
        on->setRegisteredtoCam (false);               
	}
    //---------------------------------------------------------------------
    void PagingLandScapeOctreeSceneManager::_updateSceneGraph(Camera * cam)
    {
        SceneManager::_updateSceneGraph(cam);
    }
    //---------------------------------------------------------------------
    void PagingLandScapeOctreeSceneManager::enableHardwareOcclusionTests()
    { 
        Camera * const c_cam = mCameraInProgress;

        // make sure we always occlude in SOLID MODE
		mCamDetail =  c_cam->getPolygonMode ();
		if (mCamDetail != PM_SOLID)
		{			
			mOcclusion.setIfNotSolidScene	(true);
			c_cam->setPolygonMode (PM_SOLID);
		}
		else 
		{
			mOcclusion.setIfNotSolidScene	(false);
		}

        //#define HWOCCLUSIONRTT
        #ifdef HWOCCLUSIONRTT

			const Viewport * const c_camViewPort = c_cam->getViewport();
			assert (c_camViewPort);
            if (mOcclusionDepth == 0)
            {
                mOcclusionDepth = mDestRenderSystem->
                    createRenderTexture(
                    "OcclusionDepthTexture", 
                    c_camViewPort->getActualWidth() / 8, 
                    c_camViewPort->getActualHeight() / 8, 
                    TEX_TYPE_2D, 
                    PF_DEPTH);


                mOcclusionDepth->setAutoUpdated (false);
                mOcclusionDepth->setActive (false);

                mOcclusionCamera = createCamera ("OcclusionDepthTextureCamera");
                Viewport * const v = mOcclusionDepth->addViewport(mOcclusionCamera);
				assert (v);
                v->setOverlaysEnabled (false);
                v->setClearEveryFrame(false);
                v->setBackgroundColour(ColourValue::Black);

                #define RENDERCOLOR
                #ifdef RENDERCOLOR
                    MaterialPtr mat = MaterialManager::getSingleton().create (
                                        "OcclusionDepthTextureMat",
                                        ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
                    mat->getTechnique(0)->getPass(0)->createTextureUnitState("OcclusionDepthTexture");

                    Overlay *OcclusionDepthOverlay = OverlayManager::getSingleton ().create("OcclusionDepthOverlay"); 
                    OverlayElement *OcclusionDepthPanel = 
                    OverlayManager::getSingleton ().createOverlayElement ("Panel", 
                                                                            "OcclusionDepthPanel", 
                                                                            false); 
                    OcclusionDepthPanel->setMaterialName ("OcclusionDepthTextureMat"); 
                    OcclusionDepthOverlay->add2D ((OverlayContainer *)OcclusionDepthPanel); 
                    OcclusionDepthOverlay->show ();

                    //OcclusionDepthOverlay->setScale (0.5f, 0.5f);
                    OcclusionDepthPanel->setPosition (0.8, 0.8);
                #endif //RENDERCOLOR

                mOcclusionCamera->setNearClipDistance(c_cam->getNearClipDistance());
                mOcclusionCamera->setFarClipDistance(c_cam->getFarClipDistance());
                mOcclusionCamera->setAspectRatio(
                    (Real)c_camViewPort->getActualWidth() /  c_camViewPort->getActualHeight());
            }

            // copy camera characteristics to occlusion camera.
            mOcclusionCamera->setPosition (c_cam->getWorldPosition ());
            mOcclusionCamera->setOrientation (c_cam->getWorldOrientation ());
            mOcclusionCamera->setNearClipDistance(c_cam->getNearClipDistance());
            mOcclusionCamera->setFarClipDistance(c_cam->getFarClipDistance());  


            mDestRenderSystem->_setViewport(mOcclusionCamera->getViewport ());
            mDestRenderSystem->_setWorldMatrix(Matrix4::IDENTITY);
            mDestRenderSystem->_setViewMatrix(c_cam->getViewMatrix());
            mDestRenderSystem->_setProjectionMatrix(c_cam->getProjectionMatrix());

            mDestRenderSystem->_beginFrame();

            mDestRenderSystem->clearFrameBuffer (FBT_DEPTH | FBT_COLOUR, 
            mOcclusionCamera->getViewport ()->getBackgroundColour ());

        #else //HWOCCLUSIONRTT
			assert (c_cam->getViewport());
            mDestRenderSystem->_setViewport(c_cam->getViewport());
            mDestRenderSystem->clearFrameBuffer (FBT_DEPTH | FBT_COLOUR, 
                                                c_cam->getViewport ()->getBackgroundColour (),//color
                                                1.0f,//depth
                                                0);//stencil
            mDestRenderSystem->_beginFrame();
            assert (c_cam->getViewport()->getClearEveryFrame () == false);
        #endif //HWOCCLUSIONRTT

		mDestRenderSystem->_beginGeometryCount();

    }
    //---------------------------------------------------------------------
    void PagingLandScapeOctreeSceneManager::disableHardwareOcclusionTests()
    {
        Camera * const c_cam = mCameraInProgress;

		// make sure we restore previous detail level.
		if (mCamDetail != PM_SOLID)
			c_cam->setPolygonMode (mCamDetail);

		assert (c_cam->getViewport ());
        #ifdef HWOCCLUSIONRTT

                mDestRenderSystem->_endFrame();
                mDestRenderSystem->_setViewport(c_cam->getViewport ());

        #else //HWOCCLUSIONRTT
                mDestRenderSystem->_endFrame();
		#endif //HWOCCLUSIONRTT

		if (mCamDetail != PM_SOLID)
		{
			mDestRenderSystem->clearFrameBuffer (FBT_DEPTH | FBT_COLOUR, 
												c_cam->getViewport ()->getBackgroundColour (),//color
												1.0f,//depth
												0);//stencil
		}

		// Notify camera or vis faces
		PagingLandScapeOctreeCamera * const octreeCam = static_cast <PagingLandScapeOctreeCamera *> (c_cam);
		octreeCam->_addCHCRenderedFaces(mDestRenderSystem->_getFaceCount());
	}
    //---------------------------------------------------------------------
    void PagingLandScapeOctreeSceneManager::_findVisibleObjects(Camera * cam, bool onlyShadowCasters)
    {
        RenderQueue *q = SceneManager::getRenderQueue();
       

		PagingLandScapeOctreeCamera * const octreeCam = static_cast <PagingLandScapeOctreeCamera *> (cam);
		
        assert (mVisibles.find(octreeCam->getId()) != mVisibles.end());
        mCamInProgressVisibles = mVisibles[octreeCam->getId()];
        assert (mCamInProgressVisibles);

        #ifdef _VISIBILITYDEBUG
            //if (!octreeCam->isRegisteredInOcclusionSystem())
            //{
            //    mPagingLandScapeOctree->traversal(RegisterCameraTraversal(octreeCam));
            //    octreeCam->setRegisteredInOcclusionSystem(true);
            //}
            mBoxes.clear();
            if (mCullCamera)
            {
                if (getCamera("CullCamera"))
                    cam = getCamera("CullCamera");
            }
        #endif // _VISIBILITYDEBUG

        if (mPagingLandScapeOctree->hasChildren ())
        {
            mNumObjects = 0;

            // should find a way to be sure that 
            // it's a new frame, and not just a camera change or something.
            if (mOcclusion.nextFrame (static_cast < PagingLandScapeOctreeCamera * > (cam),
                                      mCamInProgressVisibles,
                                      onlyShadowCasters, 
                                      q))
            {
                switch (octreeCam->getOcclusionMode())
                {
                case CHC:
                    {                     
                        enableHardwareOcclusionTests();
                        mOcclusion.CHCtraversal (mPagingLandScapeOctree);
						disableHardwareOcclusionTests();
						q->clear();

						#ifdef _VISIBILITYDEBUG
						if (mCullDebug && cam->getViewport ())
						{
                            cam->getViewport ()->getTarget ()->setDebugText(
                                "CHC = " + StringConverter::toString(mOcclusion.object_cnt) + ", t=" +
                                StringConverter::toString(mOcclusion.traversed_nodes_cnt) + ", f=" +
                                StringConverter::toString(mOcclusion.frustum_culled_nodes_cnt) + ", q=" +
                                StringConverter::toString(mOcclusion.query_cnt)
                                            );
						}
                        #endif //_VISIBILITYDEBUG
                    }
                    break;
                case CHC_CONSERVATIVE:
                    {                     
                        enableHardwareOcclusionTests();
                        mOcclusion.CHCtraversalConservative (mPagingLandScapeOctree);
						disableHardwareOcclusionTests();
						q->clear();

                        #ifdef _VISIBILITYDEBUG
							if (mCullDebug && cam->getViewport ())
							{
							cam->getViewport ()->getTarget ()->setDebugText(
									"CHC Conservative = " + StringConverter::toString(mOcclusion.object_cnt) + ", t=" +
									StringConverter::toString(mOcclusion.traversed_nodes_cnt) + ", f=" +
									StringConverter::toString(mOcclusion.frustum_culled_nodes_cnt) + ", q=" +
									StringConverter::toString(mOcclusion.query_cnt)
												);
							}
						#endif //_VISIBILITYDEBUG

                    }
                    break;
//                    case STOP_AND_WAIT:
//                        {     
//                            enableHardwareOcclusionTests();
//                            mOcclusion.initQueryPool ();
//                            mPagingLandScapeOctree->traversal (SWTraversal (mOcclusion));
//                            disableHardwareOcclusionTests(); q->clear();
//    
//                           #ifdef _VISIBILITYDEBUG
//                               if (mCullDebug && cam->getViewport ()) {cam->getViewport ()->getTarget ()->setDebugText(
//                                        "SnW = " + StringConverter::toString(mOcclusion.object_cnt) + ", t=" +
//                                        StringConverter::toString(mOcclusion.traversed_nodes_cnt) + ", f=" +
//                                        StringConverter::toString(mOcclusion.frustum_culled_nodes_cnt) + ", q=" +
//                                        StringConverter::toString(mOcclusion.query_cnt)
//                                                      );}
//                                
//                                #endif //_VISIBILITYDEBUG
//        
//                        }
//                         break;
//                case VIEW_FRUSTUM:
//                        {
//                            mPagingLandScapeOctree->traversal (ViewFrustumCullingTraversal (mOcclusion));
//							q->clear();
//                            #ifdef _VISIBILITYDEBUG
//								if (mCullDebug && cam->getViewport ())
//                              {  cam->getViewport ()->getTarget ()->setDebugText(
//                                    "VF = " +  StringConverter::toString(mOcclusion.object_cnt) + ", t=" +
//                                StringConverter::toString(mOcclusion.traversed_nodes_cnt) + ", f=" +
//                                StringConverter::toString(mOcclusion.frustum_culled_nodes_cnt) + ", q=" +
//                                StringConverter::toString(mOcclusion.query_cnt)
//                                              );}
//                             
//                            #endif //_VISIBILITYDEBUG
//                        }
//		            break;  
                case VIEW_FRUSTUM_DIRECT:
                {
					ViewFrustumCullingTraversalDirect temp (mOcclusion);
                    mPagingLandScapeOctree->traversal (temp);

                    #ifdef _VISIBILITYDEBUG
						if (mCullDebug && cam->getViewport ())
						{ 
							cam->getViewport ()->getTarget ()->setDebugText(
								"VFD = " +  StringConverter::toString(mOcclusion.object_cnt) + ", t=" +
							StringConverter::toString(mOcclusion.traversed_nodes_cnt) + ", f=" +
							StringConverter::toString(mOcclusion.frustum_culled_nodes_cnt) + ", q=" +
							StringConverter::toString(mOcclusion.query_cnt)
                                    ); 
						}           
                    #endif // _VISIBILITYDEBUG
                }
			        break;
                    
//                case STANDARD_WALK:
//                    {    
//                        // recursive heap eater old thing
//                        walkPagingLandScapeOctree (static_cast < PagingLandScapeOctreeCamera * > (cam),
//                                                    q,
//                                                    mPagingLandScapeOctree, 
//                                                    false,
//                                                    onlyShadowCasters);
//                         
//                        #ifdef _VISIBILITYDEBUG
//                        if (mCullDebug && cam->getViewport ()) 
//									cam->getViewport ()->getTarget ()->setDebugText (
//                                "OldWalk :" + 
//                                StringConverter::toString(mNumObjects)
//                                              );                        
//                            // Show the PagingLandScapeOctree boxes & cull camera if required        
//                            if (mShowBoxes)
//                            {
//                                for (MovableObjectList::iterator it = mBoxes.begin(); it != mBoxes.end(); ++it)
//                                {
//                                    (*it)->_updateRenderQueue(q);
//                                }
//                            }
//
//                            if (mCullCamera)
//                            {
//                                PagingLandScapeOctreeCamera * c = static_cast<PagingLandScapeOctreeCamera*>(SceneManager::getCamera("CullCamera"));
//                                if (c != 0)
//                                {
//                                    c->_updateRenderQueue(q);               
//                                }
//                            }           
//                        #endif // _VISIBILITYDEBUG
//                    }
//			        break;
		        default:
                    assert (0);
			        break;
            }
            }

            if (!mCamInProgressVisibles->empty())
            {
                MovableObjectList::iterator movableIt = mCamInProgressVisibles->begin();
                MovableObjectList::iterator mitend = mCamInProgressVisibles->end(); 
                while (movableIt != mitend)
                {
                    (*movableIt)->_updateRenderQueue (q);
                    ++movableIt;
                }   
            }

            #ifdef _VISIBILITYDEBUG
                if (mCullDebug)
                {   
                    mPagingLandScapeOctree->traversal (TreeOverlayDebug (mOcclusion, this));
                }      
            #endif // _VISIBILITYDEBUG
        }
    }
    //---------------------------------------------------------------------
    void PagingLandScapeOctreeSceneManager::addVisible(MovableObject *mo)
    {   
        mCamInProgressVisibles->push_back (mo);
    }
    //---------------------------------------------------------------------
    void PagingLandScapeOctreeSceneManager::directRenderSingleQueue(RenderQueue *queue)
    {  
        RenderQueue *oldQueue = mRenderQueue;
        mRenderQueue = queue;

        SceneManager::_renderVisibleObjects ();

        queue->clear ();
        mRenderQueue = oldQueue;
    }    
    //---------------------------------------------------------------------
    void PagingLandScapeOctreeSceneManager::directRenderSingleObject(Renderable *r)
    {   
          const Camera * const c_cam = mCameraInProgress;
          mDestRenderSystem->_setWorldMatrix(Matrix4::IDENTITY);
//        mDestRenderSystem->_setViewMatrix(c_cam->getViewMatrix());
//        mDestRenderSystem->_setProjectionMatrix(c_cam->getProjectionMatrix());

		mDestRenderSystem->setCurrentPassIterationCount(1);
        _setPass(r->getMaterial()->getBestTechnique()->getPass(0));
        useRenderableViewProjMode (r);

        RenderOperation ro;
        r->getRenderOperation (ro);
        mDestRenderSystem->_render(ro);

    } 
    //---------------------------------------------------------------------
    void PagingLandScapeOctreeSceneManager::registerCamera (PagingLandScapeOctreeCamera *c)
    {        
       mPagingLandScapeOctree->traversal(RegisterCameraTraversal(c));
    }
    //---------------------------------------------------------------------
    void PagingLandScapeOctreeSceneManager::walkPagingLandScapeOctree(PagingLandScapeOctreeCamera * camera, RenderQueue * const queue,
                                        PagingLandScapeOctree * const octant, const bool foundvisible, const bool onlyShadowCasters)
    {
        assert (octant);
        PagingLandScapeOctreeCamera::Visibility v = PagingLandScapeOctreeCamera::NONE;

        if (foundvisible)
        {
            v = PagingLandScapeOctreeCamera::FULL;
        }
        else if (octant == mPagingLandScapeOctree)
        {
            v = PagingLandScapeOctreeCamera::PARTIAL;
        }
        else
        {
            AxisAlignedBox box;
            octant->_getCullBounds(&box);
            v = camera->getVisibility(box);
        }


        // if the octant is visible, or if it's the root node...
        if (v != PagingLandScapeOctreeCamera::NONE)
        {
            #ifdef _VISIBILITYDEBUG
                    octant->getNodeData (camera)->viewFrustumVisible = true;
                    octant->getNodeData (camera)->queryVisible = true;

                    if (mShowBoxes)
                    {
                        mBoxes.push_back(octant->getWireBoundingBox());
                        //mBoxes.push_back(octant->getOcclusionBoundingBox());
                    }
            #endif //_VISIBILITYDEBUG

            bool vis = true;
            //Add stuff to be rendered;
            NodeList::iterator it = octant->mNodes.begin();
            while (it != octant->mNodes.end())
            {
                PagingLandScapeOctreeNode * const sn = *it;

                // if this PagingLandScapeOctree is partially visible, manually cull all
                // scene nodes attached directly to this level.

                if (v == PagingLandScapeOctreeCamera::PARTIAL)
                    vis = camera->isVisible(sn->_getWorldAABB());

                if (vis)
                {
                    #ifdef _VISIBILITYDEBUG
                        sn->getNodeData (camera)->viewFrustumVisible = true;
                        sn->getNodeData (camera)->queryVisible = true;
                    #endif //_VISIBILITYDEBUG

                    mNumObjects++;
                    sn->_addToRenderQueue(camera, queue, onlyShadowCasters);

                    //mVisible.push_back(sn);

                    if (mDisplayNodes)
                        queue->addRenderable(sn);

                    // check if the scene manager or this node wants the bounding box shown.
                    if (sn->getShowBoundingBox() || mShowBoundingBoxes)
                        sn->_addBoundingBoxToQueue(queue);
                }
                #ifdef _VISIBILITYDEBUG
                    else
                    {  
                        sn->getNodeData (camera)->viewFrustumVisible = false;
                        sn->getNodeData (camera)->queryVisible = false;
                    }
                #endif //_VISIBILITYDEBUG

                ++it;
            } // while (it != octant->mNodes.end())

            const bool isFullVis =  (v == PagingLandScapeOctreeCamera::FULL);
            if (octant->mChildren[ 0 ][ 0 ][ 0 ] != 0)
                walkPagingLandScapeOctree(camera, queue, octant->mChildren[ 0 ][ 0 ][ 0 ], 
                            isFullVis, onlyShadowCasters);

            if (octant->mChildren[ 1 ][ 0 ][ 0 ] != 0)
                walkPagingLandScapeOctree(camera, queue, octant->mChildren[ 1 ][ 0 ][ 0 ], 
                            isFullVis, onlyShadowCasters);

            if (octant->mChildren[ 0 ][ 1 ][ 0 ] != 0)
                walkPagingLandScapeOctree(camera, queue, octant->mChildren[ 0 ][ 1 ][ 0 ], 
                            isFullVis, onlyShadowCasters);

            if (octant->mChildren[ 1 ][ 1 ][ 0 ] != 0)
                walkPagingLandScapeOctree(camera, queue, octant->mChildren[ 1 ][ 1 ][ 0 ], 
                            isFullVis, onlyShadowCasters);

            if (octant->mChildren[ 0 ][ 0 ][ 1 ] != 0)
                walkPagingLandScapeOctree(camera, queue, octant->mChildren[ 0 ][ 0 ][ 1 ], 
                            isFullVis, onlyShadowCasters);

            if (octant->mChildren[ 1 ][ 0 ][ 1 ] != 0)
                walkPagingLandScapeOctree(camera, queue, octant->mChildren[ 1 ][ 0 ][ 1 ], 
                            isFullVis, onlyShadowCasters);

            if (octant->mChildren[ 0 ][ 1 ][ 1 ] != 0)
                walkPagingLandScapeOctree(camera, queue, octant->mChildren[ 0 ][ 1 ][ 1 ], 
                            isFullVis, onlyShadowCasters);

            if (octant->mChildren[ 1 ][ 1 ][ 1 ] != 0)
                walkPagingLandScapeOctree(camera, queue, octant->mChildren[ 1 ][ 1 ][ 1 ], 
                            isFullVis, onlyShadowCasters);

        }
        #ifdef _VISIBILITYDEBUG
            else
            {  
                octant->getNodeData (camera)->viewFrustumVisible = false;
                octant->getNodeData (camera)->queryVisible = false;
            }
        #endif //_VISIBILITYDEBUG

    }
    //---------------------------------------------------------------------
     // --- non template versions
    void _findNodes(const AxisAlignedBox &t, std::list < SceneNode * > &list, 
                    const Ogre::SceneNode * const exclude, const bool full, PagingLandScapeOctree *octant)
    {

        bool isFull = full;
	    if (!full)
	    {
		    AxisAlignedBox obox;
		    octant->_getCullBounds(&obox);

		    const Intersection isect = intersect(t, obox);

		    if (isect == OUTSIDE)
			    return ;

		    isFull = (isect == INSIDE);
	    }


        const bool b_full = isFull;
	    NodeList::iterator it = octant->mNodes.begin();

	    while (it != octant->mNodes.end())
	    {
		    PagingLandScapeOctreeNode * const on = (*it);

		    if (on != exclude)
		    {
			    if (b_full)
			    {
				    list.push_back(on);
			    }

			    else
			    {
				    const Intersection nsect = intersect(t, on->_getWorldAABB());

				    if (nsect != OUTSIDE)
				    {
					    list.push_back(on);
				    }
			    }

		    }

		    ++it;
	    }



	    if (octant->mChildren[ 0 ][ 0 ][ 0 ] != 0)
		    _findNodes(t, list, exclude, b_full, octant->mChildren[ 0 ][ 0 ][ 0 ]);

	    if (octant->mChildren[ 1 ][ 0 ][ 0 ] != 0)
		    _findNodes(t, list, exclude, b_full, octant->mChildren[ 1 ][ 0 ][ 0 ]);

	    if (octant->mChildren[ 0 ][ 1 ][ 0 ] != 0)
		    _findNodes(t, list, exclude, b_full, octant->mChildren[ 0 ][ 1 ][ 0 ]);

	    if (octant->mChildren[ 1 ][ 1 ][ 0 ] != 0)
		    _findNodes(t, list, exclude, b_full, octant->mChildren[ 1 ][ 1 ][ 0 ]);

	    if (octant->mChildren[ 0 ][ 0 ][ 1 ] != 0)
		    _findNodes(t, list, exclude, b_full, octant->mChildren[ 0 ][ 0 ][ 1 ]);

	    if (octant->mChildren[ 1 ][ 0 ][ 1 ] != 0)
		    _findNodes(t, list, exclude, b_full, octant->mChildren[ 1 ][ 0 ][ 1 ]);

	    if (octant->mChildren[ 0 ][ 1 ][ 1 ] != 0)
		    _findNodes(t, list, exclude, b_full, octant->mChildren[ 0 ][ 1 ][ 1 ]);

	    if (octant->mChildren[ 1 ][ 1 ][ 1 ] != 0)
		    _findNodes(t, list, exclude, b_full, octant->mChildren[ 1 ][ 1 ][ 1 ]);

    }
    //---------------------------------------------------------------------
    void _findNodes(const Sphere &t, std::list < SceneNode * > &list, 
                    const SceneNode * const exclude, 
                    const bool full, PagingLandScapeOctree *octant)
    {
        bool isFull = full;
	    if (!full)
	    {
		    AxisAlignedBox obox;
		    octant->_getCullBounds(&obox);

		    const Intersection isect = intersect(t, obox);

		    if (isect == OUTSIDE)
			    return ;

		    isFull = (isect == INSIDE);
	    }

        const bool b_full = isFull;
	    NodeList::iterator it = octant->mNodes.begin();

	    while (it != octant->mNodes.end())
	    {
		    PagingLandScapeOctreeNode * const on = (*it);

		    if (on != exclude)
		    {
			    if (b_full)
			    {
				    list.push_back(on);
			    }

			    else
			    {
				    const Intersection nsect = intersect(t, on->_getWorldAABB());

				    if (nsect != OUTSIDE)
				    {
					    list.push_back(on);
				    }
			    }

		    }

		    ++it;
	    }



	    if (octant->mChildren[ 0 ][ 0 ][ 0 ] != 0)
		    _findNodes(t, list, exclude, b_full, octant->mChildren[ 0 ][ 0 ][ 0 ]);

	    if (octant->mChildren[ 1 ][ 0 ][ 0 ] != 0)
		    _findNodes(t, list, exclude, b_full, octant->mChildren[ 1 ][ 0 ][ 0 ]);

	    if (octant->mChildren[ 0 ][ 1 ][ 0 ] != 0)
		    _findNodes(t, list, exclude, b_full, octant->mChildren[ 0 ][ 1 ][ 0 ]);

	    if (octant->mChildren[ 1 ][ 1 ][ 0 ] != 0)
		    _findNodes(t, list, exclude, b_full, octant->mChildren[ 1 ][ 1 ][ 0 ]);

	    if (octant->mChildren[ 0 ][ 0 ][ 1 ] != 0)
		    _findNodes(t, list, exclude, b_full, octant->mChildren[ 0 ][ 0 ][ 1 ]);

	    if (octant->mChildren[ 1 ][ 0 ][ 1 ] != 0)
		    _findNodes(t, list, exclude, b_full, octant->mChildren[ 1 ][ 0 ][ 1 ]);

	    if (octant->mChildren[ 0 ][ 1 ][ 1 ] != 0)
		    _findNodes(t, list, exclude, b_full, octant->mChildren[ 0 ][ 1 ][ 1 ]);

	    if (octant->mChildren[ 1 ][ 1 ][ 1 ] != 0)
		    _findNodes(t, list, exclude, b_full, octant->mChildren[ 1 ][ 1 ][ 1 ]);

    }
    //---------------------------------------------------------------------
    void _findNodes(const PlaneBoundedVolume &t, std::list < SceneNode * > &list, 
                const Ogre::SceneNode * const exclude, const bool full, PagingLandScapeOctree *octant)
    {

        bool isFull = full;
	    if (!full)
	    {
		    AxisAlignedBox obox;
		    octant->_getCullBounds(&obox);

		    const Intersection isect = intersect(t, obox);

		    if (isect == OUTSIDE)
			    return ;

		    isFull = (isect == INSIDE);
	    }


        const bool b_full = isFull;
	    NodeList::iterator it = octant->mNodes.begin();

	    while (it != octant->mNodes.end())
	    {
		    PagingLandScapeOctreeNode * const on = (*it);

		    if (on != exclude)
		    {
			    if (b_full)
			    {
				    list.push_back(on);
			    }

			    else
			    {
				    const Intersection nsect = intersect(t, on->_getWorldAABB());

				    if (nsect != OUTSIDE)
				    {
					    list.push_back(on);
				    }
			    }

		    }

		    ++it;
	    }



	    if (octant->mChildren[ 0 ][ 0 ][ 0 ] != 0)
		    _findNodes(t, list, exclude, b_full, octant->mChildren[ 0 ][ 0 ][ 0 ]);

	    if (octant->mChildren[ 1 ][ 0 ][ 0 ] != 0)
		    _findNodes(t, list, exclude, b_full, octant->mChildren[ 1 ][ 0 ][ 0 ]);

	    if (octant->mChildren[ 0 ][ 1 ][ 0 ] != 0)
		    _findNodes(t, list, exclude, b_full, octant->mChildren[ 0 ][ 1 ][ 0 ]);

	    if (octant->mChildren[ 1 ][ 1 ][ 0 ] != 0)
		    _findNodes(t, list, exclude, b_full, octant->mChildren[ 1 ][ 1 ][ 0 ]);

	    if (octant->mChildren[ 0 ][ 0 ][ 1 ] != 0)
		    _findNodes(t, list, exclude, b_full, octant->mChildren[ 0 ][ 0 ][ 1 ]);

	    if (octant->mChildren[ 1 ][ 0 ][ 1 ] != 0)
		    _findNodes(t, list, exclude, b_full, octant->mChildren[ 1 ][ 0 ][ 1 ]);

	    if (octant->mChildren[ 0 ][ 1 ][ 1 ] != 0)
		    _findNodes(t, list, exclude, b_full, octant->mChildren[ 0 ][ 1 ][ 1 ]);

	    if (octant->mChildren[ 1 ][ 1 ][ 1 ] != 0)
		    _findNodes(t, list, exclude, b_full, octant->mChildren[ 1 ][ 1 ][ 1 ]);

    }
    //---------------------------------------------------------------------
    void _findNodes(const Ray &t, std::list < SceneNode * > &list, 
                    const Ogre::SceneNode * const exclude, 
                    const bool full, PagingLandScapeOctree *octant)
    {

        bool isFull = full;
	    if (!full)
	    {
		    AxisAlignedBox obox;
		    octant->_getCullBounds(&obox);

		    const Intersection isect = intersect(t, obox);

		    if (isect == OUTSIDE)
			    return ;

		    isFull = (isect == INSIDE);
	    }


        const bool b_full = isFull;
	    NodeList::iterator it = octant->mNodes.begin();

	    while (it != octant->mNodes.end())
	    {
		    PagingLandScapeOctreeNode * const on = (*it);

		    if (on != exclude)
		    {
			    if (b_full)
			    {
				    list.push_back(on);
			    }

			    else
			    {
				    const Intersection nsect = intersect(t, on->_getWorldAABB());

				    if (nsect != OUTSIDE)
				    {
					    list.push_back(on);
				    }
			    }

		    }

		    ++it;
	    }



	    if (octant->mChildren[ 0 ][ 0 ][ 0 ] != 0)
		    _findNodes(t, list, exclude, b_full, octant->mChildren[ 0 ][ 0 ][ 0 ]);

	    if (octant->mChildren[ 1 ][ 0 ][ 0 ] != 0)
		    _findNodes(t, list, exclude, b_full, octant->mChildren[ 1 ][ 0 ][ 0 ]);

	    if (octant->mChildren[ 0 ][ 1 ][ 0 ] != 0)
		    _findNodes(t, list, exclude, b_full, octant->mChildren[ 0 ][ 1 ][ 0 ]);

	    if (octant->mChildren[ 1 ][ 1 ][ 0 ] != 0)
		    _findNodes(t, list, exclude, b_full, octant->mChildren[ 1 ][ 1 ][ 0 ]);

	    if (octant->mChildren[ 0 ][ 0 ][ 1 ] != 0)
		    _findNodes(t, list, exclude, b_full, octant->mChildren[ 0 ][ 0 ][ 1 ]);

	    if (octant->mChildren[ 1 ][ 0 ][ 1 ] != 0)
		    _findNodes(t, list, exclude, b_full, octant->mChildren[ 1 ][ 0 ][ 1 ]);

	    if (octant->mChildren[ 0 ][ 1 ][ 1 ] != 0)
		    _findNodes(t, list, exclude, b_full, octant->mChildren[ 0 ][ 1 ][ 1 ]);

	    if (octant->mChildren[ 1 ][ 1 ][ 1 ] != 0)
		    _findNodes(t, list, exclude, b_full, octant->mChildren[ 1 ][ 1 ][ 1 ]);

    }
    //---------------------------------------------------------------------
    void PagingLandScapeOctreeSceneManager::findNodesIn(const AxisAlignedBox &box, std::list < SceneNode * > &list, const SceneNode* const exclude)
    {
        _findNodes(box, list, exclude, false, mPagingLandScapeOctree);
    }
    //---------------------------------------------------------------------
    void PagingLandScapeOctreeSceneManager::findNodesIn(const Sphere &sphere, std::list < SceneNode * > &list, const SceneNode* const exclude)
    {
        _findNodes(sphere, list, exclude, false, mPagingLandScapeOctree);
    }
    //---------------------------------------------------------------------
    void PagingLandScapeOctreeSceneManager::findNodesIn(const PlaneBoundedVolume &volume, std::list < SceneNode * > &list, const SceneNode* const exclude)
    {
        _findNodes(volume, list, exclude, false, mPagingLandScapeOctree);
    }
    //---------------------------------------------------------------------
    void PagingLandScapeOctreeSceneManager::findNodesIn(const Ray &r, std::list < SceneNode * > &list, const SceneNode* const exclude)
    {
        _findNodes(r, list, exclude, false, mPagingLandScapeOctree);
    }
    //---------------------------------------------------------------------
    void PagingLandScapeOctreeSceneManager::resize(const AxisAlignedBox &box, const int depth)
    {
        mMaxDepth = depth;
        resize(box);
    }
    //---------------------------------------------------------------------
    void PagingLandScapeOctreeSceneManager::resize(const AxisAlignedBox &box)
    {
        std::list < SceneNode * > nodes;

        _findNodes(mPagingLandScapeOctree->getBoundingBox (), nodes, 0, true, mPagingLandScapeOctree);

        delete mPagingLandScapeOctree;

        mBox = box;
        mPagingLandScapeOctree = new PagingLandScapeOctree(0);       
        mPagingLandScapeOctree ->setBoundingBox (box.getMinimum(), box.getMaximum());       
		registeredNodeInCamera (mPagingLandScapeOctree); 
        
        if (!nodes.empty())
        {
            std::list < SceneNode * > ::iterator it;
            it = nodes.begin();

            while (it != nodes.end())
            {
                PagingLandScapeOctreeNode * const on = static_cast < PagingLandScapeOctreeNode * > (*it);
                on->setOctant(0);
                _updatePagingLandScapeOctreeNode(on);
                ++it;
            } // while (it != nodes.end())
        }  
    }
    //---------------------------------------------------------------------
    bool PagingLandScapeOctreeSceneManager::setOption(const String & key, const void * val)
    {
        if (key == "Size")
        {
            resize(* static_cast < const AxisAlignedBox * > (val));
            return true;
        }

        else if (key == "Depth")
        {
            assert (mPagingLandScapeOctree);
            mMaxDepth = * static_cast < const int * > (val);
            resize(mPagingLandScapeOctree->getBoundingBox());
            return true;
        }
        else if (key == "NextCullMode")
        {
            if (val)
            {
                CameraList::iterator it = mCameras.find (static_cast < const PagingLandScapeOctreeCamera * > (val)->getName());
                if (it != mCameras.end())
				{
                    static_cast < PagingLandScapeOctreeCamera * > (it->second)->setNextOcclusionMode();
                    return true;
                }
            }
            return false;
		}
		else if (key == "setNumberOfConservativeFrames")
		{
			mOcclusion.setNumberOfConservativeFrames(*(static_cast < const unsigned int * > (val)));
		}
		else if (key == "CurrentOptionCamera")
		{ 
			if (val)
			{
				CameraList::iterator it = mCameras.find (static_cast < const PagingLandScapeOctreeCamera * > (val)->getName());
				if (it != mCameras.end())
				{
					mCurrentOptionCamera = static_cast < PagingLandScapeOctreeCamera * > (it->second);
				}
				return true;
			}	
		}
		else if (key == "setCullingMode")
		{
			assert (mCurrentOptionCamera);
			mCurrentOptionCamera->setOcclusionModeAsString(* static_cast < const String * > (val));			
			return true;		
		}
        #ifdef _VISIBILITYDEBUG
            else if (key == "ShowPagingLandScapeOctree")
            {   
                    mShowBoxes = * static_cast < const bool * > (val);
                return true;
            }
            else if (key == "CullDebug")
            {
                mCullDebug = * static_cast < const bool * > (val);
                return true;
            }
            else if (key == "CullCamera")
            {
                mCullCamera = * static_cast < const bool * > (val);
                return true;
            }
        #endif //_VISIBILITYDEBUG    
        return SceneManager::setOption(key, val);


    }
    //---------------------------------------------------------------------
    bool PagingLandScapeOctreeSceneManager::getOption(const String & key, void *val)
    {
        if (key == "Size")
        {
            AxisAlignedBox * b = static_cast < AxisAlignedBox * > (val);
            b->setExtents(mPagingLandScapeOctree->getBoundingBox ().getMinimum(), mPagingLandScapeOctree->getBoundingBox().getMaximum());
            return true;
        }

        else if (key == "Depth")
        {
            * static_cast < int * > (val) = mMaxDepth;
            return true;
		}
		else if (key == "setNumberOfConservativeFrames")
		{
			* static_cast < unsigned int * > (val) = mOcclusion.getNumberOfConservativeFrames();
			return true;
		}  
		else if (key == "CurrentOptionCamera")
		{
			*static_cast < String * > (val) = mCurrentOptionCamera->getName(); 
			return true;	
		}
		else if (key == "getCullingMode")
		{
			assert (mCurrentOptionCamera);
			*static_cast < String * > (val) = mCurrentOptionCamera->getOcclusionModeAsString();
			return true;		
		}
		
        #ifdef _VISIBILITYDEBUG
            else if (key == "ShowPagingLandScapeOctree")
            {

                * static_cast < bool * > (val) = mShowBoxes;
                return true;
            }
            else if (key == "CullCamera")
            {
                * static_cast < bool * > (val) = mCullCamera;
                return true;
            } 
            else if (key == "CullDebug")
            {
                * static_cast < bool * > (val) = mCullDebug;
                return true;
            }
        #endif //_VISIBILITYDEBUG
    

        return SceneManager::getOption(key, val);
    }
    //---------------------------------------------------------------------
    void PagingLandScapeOctreeSceneManager::clearScene(void)
    {
        SceneManager::clearScene();

         // reset cameras
        //CameraList::iterator iCam = mCameras.begin();
        //for (; iCam != mCameras.end(); ++iCam)
        //{
        //    PagingLandScapeOctreeCamera *cam = static_cast <PagingLandScapeOctreeCamera *> (iCam->second);
        //    if (cam->isRegisteredInOcclusionSystem ())
        //    {
        //        mPagingLandScapeOctree->traversal (UnregisterCameraTraversal(cam));                   
        //        cam->setRegisteredInOcclusionSystem (false);
       //     } 
        //}            

        init(mBox, mMaxDepth);

    }
    //---------------------------------------------------------------------
    AxisAlignedBoxSceneQuery*
    PagingLandScapeOctreeSceneManager::createAABBQuery(const AxisAlignedBox& box, unsigned long mask)
    {
        PagingLandScapeOctreeAxisAlignedBoxSceneQuery* q = new PagingLandScapeOctreeAxisAlignedBoxSceneQuery(this);
        q->setBox(box);
        q->setQueryMask(mask);
        return q;
    }
    //---------------------------------------------------------------------
    SphereSceneQuery*
    PagingLandScapeOctreeSceneManager::createSphereQuery(const Sphere& sphere, unsigned long mask)
    {
        PagingLandScapeOctreeSphereSceneQuery* q = new PagingLandScapeOctreeSphereSceneQuery(this);
        q->setSphere(sphere);
        q->setQueryMask(mask);
        return q;
    }
    //---------------------------------------------------------------------
    PlaneBoundedVolumeListSceneQuery*
    PagingLandScapeOctreeSceneManager::createPlaneBoundedVolumeQuery(const PlaneBoundedVolumeList& volumes,
            unsigned long mask)
    {
        PagingLandScapeOctreePlaneBoundedVolumeListSceneQuery* q = new PagingLandScapeOctreePlaneBoundedVolumeListSceneQuery(this);
        q->setVolumes(volumes);
        q->setQueryMask(mask);
        return q;
    }

    //---------------------------------------------------------------------
    RaySceneQuery*
    PagingLandScapeOctreeSceneManager::createRayQuery(const Ray& ray, unsigned long mask)
    {
        PagingLandScapeOctreeRaySceneQuery* q = new PagingLandScapeOctreeRaySceneQuery(this);
        q->setRay(ray);
        q->setQueryMask(mask);
        return q;
    }
    //---------------------------------------------------------------------
    IntersectionSceneQuery*
    PagingLandScapeOctreeSceneManager::createIntersectionQuery(unsigned long mask)
   {

    // PagingLandScapeOctree implementation performs WORSE for < 500 objects
    // TODO: optimize it so it's better in all cases
    //PagingLandScapeOctreeIntersectionSceneQuery* q = new PagingLandScapeOctreeIntersectionSceneQuery(this);
    DefaultIntersectionSceneQuery* q;
    if (mPagingLandScapeOctree->numNodes () > 500)
        q = new PagingLandScapeOctreeIntersectionSceneQuery(this);
    else
        q = new DefaultIntersectionSceneQuery(this);
    q->setQueryMask(mask);
    return q;
   }
}
