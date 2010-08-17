/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2006 The OGRE Team
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
PagingLandScapeOctreenode.cpp  -  description
-------------------
begin                : Fri Sep 27 2002
copyright            : (C) 2002 by Jon Anderson
email                : janders@users.sf.net

Enhancements 2003 - 2004 (C) The OGRE Team

***************************************************************************/

#include "OgrePagingLandScapePrecompiledHeaders.h"

#include "OgreRoot.h"

#include "OgrePagingLandScapeOctreeNode.h"
#include "OgreOcclusionBoundingBox.h"
#include "OgreWireBoundingBox.h"
#include "OgreRenderQueue.h"

#include "OgreDebugRectangle2D.h"

namespace Ogre
{   
    //-----------------------------------------------------------------------
    PagingLandScapeOctreeNode::PagingLandScapeOctreeNode(SceneManager* creator) : 
        SceneNode(creator),
        OcclusionElement(),

        mOctant(0),
        mOcclusionBoundingBox(0),
        mIsStaticNode(true),
        mIsOccluder(true)
    {
       
    }
    //-----------------------------------------------------------------------
    PagingLandScapeOctreeNode::PagingLandScapeOctreeNode(SceneManager* creator, const String& name) : 
        SceneNode(creator, name),
        OcclusionElement(),

        mOctant(0),
        mOcclusionBoundingBox(0),
        mIsStaticNode(true),
        mIsOccluder(true)
    {
        
    }
    //-----------------------------------------------------------------------
    PagingLandScapeOctreeNode::~PagingLandScapeOctreeNode()
	{
		removeAllChildren();
		assert (!mParent);
        delete mOcclusionBoundingBox;
    }
    //-----------------------------------------------------------------------
    //same as SceneNode, only it doesn't care about children...
    void PagingLandScapeOctreeNode::_updateBounds(void)
    {
        mWorldAABB.setNull();
        mLocalAABB.setNull();

        // Update bounds from own attached objects
        ObjectMap::const_iterator i = mObjectsByName.begin();        
        while (i != mObjectsByName.end())
        {
            const MovableObject * const mo = i->second;
            // Get local bounds of object
            mLocalAABB.merge(mo->getBoundingBox());
            // Get World bounds of object
            mWorldAABB.merge(mo->getWorldBoundingBox(true));

            ++i;
        }

        //update the PagingLandScapeOctreeSceneManager that things might have moved.
        // if it hasn't been added to the PagingLandScapeOctree, add it, and if has moved
        // enough to leave it's current node, we'll update it.

        if (! mWorldAABB.isNull())
        { 
            assert (mCreator);
            static_cast < PagingLandScapeOctreeSceneManager * > (mCreator)->_updatePagingLandScapeOctreeNode(this);
        }
        mHalfSize = (mWorldAABB.getMaximum () - mWorldAABB.getMinimum ()) * 0.5;
        if (mOcclusionBoundingBox)
            mOcclusionBoundingBox->setupBoundingBox(mWorldAABB);
        if (mWireBoundingBox)
            mWireBoundingBox->setupBoundingBox(mWorldAABB);
        if (!mIsRegisteredToCam)
            static_cast< PagingLandScapeOctreeSceneManager * > (mCreator)->
            registeredNodeInCamera (this);
    }
    //-----------------------------------------------------------------------
    /** Since we are loose, only check the center.
    */
    bool PagingLandScapeOctreeNode::_isIn(const AxisAlignedBox &box) const
    {
	    // Always fail if not in the scene graph or box is null
		if (!mIsInSceneGraph || box.isNull()) 
			return false;
		// Always succeed if AABB is infinite
		if (box.isInfinite())
			return true;

        // Object Bounding Box Center
        const Vector3 center = mWorldAABB.getMaximum().midPoint(mWorldAABB.getMinimum());

        // Min and Max of Octree BBox
        const Vector3 &bmin = box.getMinimum();
        const Vector3 &bmax = box.getMaximum();

        // Object Bbox center is IN Octree BBox ?

        const bool centre  = bmax.x >= center.x && bmax.y >= center.y && bmax.z >= center.z &&
                             bmin.x <= center.x && bmin.y <= center.y && bmin.z <= center.z;
        if (!centre)
          		return false;
        
         // Even if covering the centre line, need to make sure this BB is not large
        // enough to require being moved up into parent. When added, bboxes would
        // end up in parent due to cascade but when updating need to deal with
        // bbox growing too large for this child
        const Vector3 octreeSize (bmax - bmin);
        const Vector3 nodeSize (mWorldAABB.getMaximum() - mWorldAABB.getMinimum());

        return (nodeSize < octreeSize);

    }
//    //-----------------------------------------------------------------------
//     /** Adds the attached objects of this PagingLandScapeOctreeScene node into the queue. */
//    void PagingLandScapeOctreeNode::_findVisibleObjects(Camera* cam, RenderQueue* queue,
//			VisibleObjectsBoundsInfo* visibleBounds,
//            bool includeChildren, bool displayNodes, bool onlyShadowCasters)
//    {
//        ObjectMap::iterator mit = mObjectsByName.begin();
//
//        while (mit != mObjectsByName.end())
//        {
//            MovableObject * const mo = mit->second;
//
//            mo->_notifyCurrentCamera(cam);
//
//            if (mo->isVisible() &&
//                (!onlyShadowCasters || mo->getCastShadows()))
//            {
//				mo->_updateRenderQueue(queue);
//
//				if (visibleBounds)
//				{
//					visibleBounds->merge(mo->getWorldBoundingBox(true),
//										 mo->getWorldBoundingSphere(true), cam);
//				}
//			}
//            ++mit;
//        }
//
//        // check if the scene manager or this node wants the bounding box shown.
//        if (getShowBoundingBox() || mCreator->getShowBoundingBoxes())
//            _addBoundingBoxToQueue(queue);
//
//        if (displayNodes)
//        {
//            // Include self in the render queue
//            queue->addRenderable(getDebugRenderable());
//        }
//
//    }
    //-----------------------------------------------------------------------
    void PagingLandScapeOctreeNode::attachObject(MovableObject* obj)
    {    
        if (mIsStaticNode)
        {
            const String movableType = obj->getMovableType();
            if (movableType == "Entity" || movableType == "StaticGeometry")
            {
                if (StringUtil::startsWith(obj->getName(), "Static", false))
                {
                     if (mIsOccluder && !StringUtil::startsWith(obj->getName(), "StaticOccluder", false))
                     {  
                        mIsOccluder = false;
                     }
                }
                else
                {
                    mIsStaticNode = false;
                    mIsOccluder = false;
                }
            }
            else if (movableType != "PagingLandScapeRenderable" 
                    && movableType != "TerrainMipMap")
            {
                 mIsStaticNode = false;
                 mIsOccluder = false;
            }
        }
        SceneNode::attachObject (obj);
    }
    //-----------------------------------------------------------------------
    MovableObject* PagingLandScapeOctreeNode::detachObject(unsigned short index)
    {
		// if we detach 1 object, after the return, 
		// there's none left so we reset the thing.
        if (!mObjectsByName.empty() && 1 == mObjectsByName.size())
        {
            mIsStaticNode = true;
            mIsOccluder = true;
        }
        return SceneNode::detachObject (index);

    }
    //-----------------------------------------------------------------------
    MovableObject* PagingLandScapeOctreeNode::detachObject(const String& name)
    {        
        if (!mObjectsByName.empty() && 1 == mObjectsByName.size())
        {
            mIsStaticNode = true;
            mIsOccluder = true;
        }
        return SceneNode::detachObject (name);
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeOctreeNode::detachObject(MovableObject* obj)
    {
        if (!mObjectsByName.empty() && 1 == mObjectsByName.size())
        {
            mIsStaticNode = true;
            mIsOccluder = true;
        }
        SceneNode::detachObject (obj);
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeOctreeNode::detachAllObjects(void)
    {
		SceneNode::detachAllObjects ();
        mIsStaticNode = true;
        mIsOccluder = true;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeOctreeNode::_addAlreadyNotifiedToVisibles()
    {             
        if (!mNotifiedVisibles.empty())
        {
            PagingLandScapeOctreeSceneManager * const osm = static_cast < PagingLandScapeOctreeSceneManager * > (mCreator);
            
            // create the render Queue.
            MovableObjectList::iterator it = mNotifiedVisibles.begin(),
                                        itend = mNotifiedVisibles.end();
            while (it != itend)
			{
				assert ((*it)->isVisible ());
                osm->addVisible (*it);
                ++it;
            }
            mNotifiedVisibles.clear();
        }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeOctreeNode::_addToVisibles(Camera*  cam, 
                                        const bool onlyShadowCasters)
    {             
        if (!mNotifiedVisibles.empty())
        {
            PagingLandScapeOctreeSceneManager * const osm = static_cast < PagingLandScapeOctreeSceneManager * > (mCreator);
            
            // create the render Queue.
           ObjectMap::iterator mit = mObjectsByName.begin ();
            while (mit != mObjectsByName.end ())
            {
                MovableObject * const mo = mit->second;  
                mo->_notifyCurrentCamera (cam); 
                if (mo->isVisible () &&
                    (!onlyShadowCasters || mo->getCastShadows ()))
                {
                    osm->addVisible (mo);
                }
                ++mit;
            }         
            mNotifiedVisibles.clear ();
        }
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeOctreeNode::notifyNodeObjects(Camera*  cam, 
                                        const bool onlyShadowCasters)
    {       
        // if at least one is visible.
        bool isVisible = false;
        mNotifiedVisibles.clear();
        ObjectMap::iterator mit = mObjectsByName.begin ();
        while (mit != mObjectsByName.end ())
        {
            MovableObject * const mo = mit->second;  
            mo->_notifyCurrentCamera (cam); 
            if (mo->isVisible () &&
                (!onlyShadowCasters || mo->getCastShadows ()))
            {
                mNotifiedVisibles.push_back (mo);
                isVisible = true;
            }
            ++mit;
        }         
        return isVisible;
    }
    //-----------------------------------------------------------------------
    MovableObjectList *PagingLandScapeOctreeNode::getVisibleNotifiedNodeObjects(Camera*  cam, 
                                                                                const bool onlyShadowCasters)
    {            
        notifyNodeObjects(cam, onlyShadowCasters);
        return &mNotifiedVisibles;
    }
    //-----------------------------------------------------------------------
    OcclusionBoundingBox* PagingLandScapeOctreeNode::getOcclusionBoundingBox()
    {
        // Create an OcclusionBoundingBox if needed
        if(mOcclusionBoundingBox == 0)
        {
            mOcclusionBoundingBox = new OcclusionBoundingBox();
            //mOcclusionBoundingBox->setRenderQueueGroup (RENDER_QUEUE_WORLD_GEOMETRY_2);
            mOcclusionBoundingBox->setupBoundingBox(mWorldAABB);
        }
        return mOcclusionBoundingBox;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeOctreeNode::addChild(Node* child)
    {
        mIsRegisteredToCam = false;
		static_cast< PagingLandScapeOctreeSceneManager * > (mCreator)->registeredNodeInCamera(this);
        // call superclass method
        Node::addChild (child);
    } 
    //-----------------------------------------------------------------------
    void PagingLandScapeOctreeNode::_removeNodeAndChildren()
    { 
        assert (mCreator);
		static_cast< PagingLandScapeOctreeSceneManager * > (mCreator)->unregisteredNodeInCamera (this);
        mIsRegisteredToCam = false;

        PagingLandScapeOctree * oct = getOctant();
        if (oct)
          oct->_removeNode(this);
       setOctant (0);

        //remove all the children nodes as well from the PagingLandScapeOctree.
        ChildNodeMap::iterator it = mChildren.begin();
        while(it != mChildren.end())
        {
            static_cast<PagingLandScapeOctreeNode *>(it->second)->_removeNodeAndChildren();
            ++it;
        }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeOctreeNode::removeAllChildren(void)
    {
        ChildNodeMap::iterator i, iend = mChildren.end();
        for (i = mChildren.begin(); i != iend; ++i)
        {	
            static_cast <PagingLandScapeOctreeNode* > (i->second)->_removeNodeAndChildren();	
		}
        Node::removeAllChildren();
    }
    //-----------------------------------------------------------------------
    Node * PagingLandScapeOctreeNode::removeChild(unsigned short index)
    {
        PagingLandScapeOctreeNode *on = static_cast<PagingLandScapeOctreeNode* >(SceneNode::removeChild(index));
        on->_removeNodeAndChildren(); 
        return on; 
    }
    //-----------------------------------------------------------------------
    Node * PagingLandScapeOctreeNode::removeChild(Node* child)
    {
        PagingLandScapeOctreeNode *on = static_cast<PagingLandScapeOctreeNode* >(SceneNode::removeChild(child));
        on->_removeNodeAndChildren(); 
        return on; 
    }
    //-----------------------------------------------------------------------
    Node * PagingLandScapeOctreeNode::removeChild(const String & name)
    {
        PagingLandScapeOctreeNode *on = static_cast< PagingLandScapeOctreeNode * >(SceneNode::removeChild(name));
        on->_removeNodeAndChildren(); 
        return on; 
    }
    #ifdef _VISIBILITYDEBUG
        //-----------------------------------------------------------------------
        void PagingLandScapeOctreeNode::setDebugCorners(PagingLandScapeOctreeSceneManager *scnMgr)
        {
            // use the ratio (OCtreeAAB / thisAAB) / 8
            const Vector3 &max =  mWorldAABB.getMaximum ();
            const Vector3 &min =  mWorldAABB.getMinimum ();
            
            Real left = 0, top = 0, right = 0, bottom = 0;


            const Vector3 &sceneMax = scnMgr->getBoundingBox ().getMaximum ();
            const Vector3 &sceneMin = scnMgr->getBoundingBox ().getMinimum ();

            const Real width = sceneMax.x - sceneMin.x;
            const Real height = sceneMax.z - sceneMin.z;
            if (Math::Abs(width) > 1e-08 && Math::Abs(height)  > 1e-08)
            {
                const Real toScreenDivider = 1.0f / 3;

                const Real xleft = min.x - sceneMin.x;
                if (Math::Abs(xleft) > 1e-08)
                    left = (xleft /  width) * toScreenDivider;   

                const Real ztop = min.z - sceneMin.z;
                if (Math::Abs(ztop) > 1e-08)
                    top = (ztop /  height) * toScreenDivider;

                const Real xright = max.x - sceneMin.x;
                if (Math::Abs(xright) > 1e-08)
                    right = (xright /  width) * toScreenDivider;        

                const Real zbottom = max.z - sceneMin.z;
                if (Math::Abs(zbottom) > 1e-08)
                    bottom =  (zbottom /  height) * toScreenDivider;

                getRectangle2d (scnMgr)->setCorners ((1.0f - toScreenDivider) + left, 
                                                        (1.0f - toScreenDivider) + top, 
                                                        (1.0f - toScreenDivider) + right, 
                                                        (1.0f - toScreenDivider) + bottom);
            }
        }  
    #endif //_VISIBILITYDEBUG   
}
