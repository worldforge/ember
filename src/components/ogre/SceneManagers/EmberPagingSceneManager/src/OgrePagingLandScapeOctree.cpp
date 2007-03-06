/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2006 The OGRE Team
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later__add
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
PagingLandScapeOctree.cpp  -  description
-------------------
begin                : Mon Sep 30 2002
copyright            : (C) 2002 by Jon Anderson
email                : janders@users.sf.net

Enhancements 2003 - 2004 (C) The OGRE Team

***************************************************************************/

#include "OgrePagingLandScapePrecompiledHeaders.h"

#include "OgreWireBoundingBox.h"
#include "OgreOcclusionBoundingBox.h"

#include "OgrePagingLandScapeOctreeSceneManager.h"
#include "OgrePagingLandScapeOctree.h"
#include "OgrePagingLandScapeOctreeNode.h"
#include "OgreDebugRectangle2D.h"

namespace Ogre
{
    //-----------------------------------------------------------------------
    PagingLandScapeOctree::PagingLandScapeOctree() : 
        OcclusionElement(),

            mWireBoundingBox(0),
            mOcclusionBoundingBox(0),
            mHalfSize(Vector3::ZERO),
            mNumNodes (0),
            mParent (0)
    {
        //initialize all children to null.
        for (unsigned int i = 0; i < 2; i++)
        {
            for (unsigned int j = 0; j < 2; j++)
            {
                for (unsigned int k = 0; k < 2; k++)
                {
                    mChildren[ i ][ j ][ k ] = 0;
                }
            }
        }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeOctree::reset()
    {
        //initialize all children to null.
        PagingLandScapeOctree *o;
        for (unsigned int i = 0; i < 2; i++)
        {
            for (unsigned int j = 0; j < 2; j++)
            {
                for (unsigned int k = 0; k < 2; k++)
                {
                    o =  mChildren[ i ][ j ][ k ];
                    if (o)
                    {
                        mSceneMgr->deleteOctree (mChildren[ i ][ j ][ k ]);
                        mChildren[ i ][ j ][ k ] = 0;
                    }
                }
            }
        }
        mSceneMgr = 0;
        mParent = 0;  
        mNumNodes = 0;
    }
    //-----------------------------------------------------------------------
    PagingLandScapeOctree::~PagingLandScapeOctree()
    {
        //initialize all children to null.
#ifdef _DEBUG
        for (unsigned int i = 0; i < 2; i++)
        {
            for (unsigned int j = 0; j < 2; j++)
            {
                for (unsigned int k = 0; k < 2; k++)
                {
                    assert (mChildren[ i ][ j ][ k ] == 0);
                }
            }
        }
#endif //_DEBUG

        delete mWireBoundingBox;
        delete mOcclusionBoundingBox;

        mParent = 0;
    }
    //-----------------------------------------------------------------------
    /** Returns true is the box will fit in a child.
    */
    bool PagingLandScapeOctree::_isTwiceSize(const AxisAlignedBox &box) const
    {
        const Vector3 &boxMin = box.getMinimum();
        const Vector3 &boxMax = box.getMaximum();
        return((boxMax.x - boxMin.x) <= mHalfSize.x) &&
              ((boxMax.y - boxMin.y) <= mHalfSize.y) &&
              ((boxMax.z - boxMin.z) <= mHalfSize.z) ;

    }
    //-----------------------------------------------------------------------
    /** Returns true is the box will fit in a child.
    */
    bool PagingLandScapeOctree::_isTwiceCullSize(const AxisAlignedBox &box) const
    {
        const Vector3 &boxMin = box.getMinimum();
        const Vector3 &boxMax = box.getMaximum();
        return((boxMax.x - boxMin.x) <= mCullHalfSize.x) &&
              ((boxMax.y - boxMin.y) <= mCullHalfSize.y) &&
              ((boxMax.z - boxMin.z) <= mCullHalfSize.z) ;

    }
     //-----------------------------------------------------------------------
    /** Returns true is the box will fit in a child.
    */
    bool PagingLandScapeOctree::_isNotCrossingAxes(const AxisAlignedBox &box) const
    { 
        const Vector3 &boxMin = box.getMinimum();
        const Vector3 &boxMax = box.getMaximum();

        const Vector3 octCullCenter = mCullBox.getMinimum() + mCullHalfSize;   

        return !((boxMin.x <= octCullCenter.x && octCullCenter.x <= boxMax.x) ||
                 (boxMin.y <= octCullCenter.y && octCullCenter.y <= boxMax.y) ||
                 (boxMin.z <= octCullCenter.z && octCullCenter.z <= boxMax.z));

    }
    //-----------------------------------------------------------------------
    PagingLandScapeOctree *PagingLandScapeOctree::_getCullChildWhereBoxFits(const AxisAlignedBox &box, 
                                                                            PagingLandScapeOctreeSceneManager *scn)
    { 
        /** It's assumed the the given box has already been proven to fit into
        * a child.  Since it's a loose PagingLandScapeOctree, only the centers need to be
        * compared to find the appropriate node.
        */
        const Vector3 octCenter = mCullBox.getMinimum() + mCullHalfSize;   
        const Vector3 ncenter = box.getMaximum().midPoint(box.getMinimum());
        
        unsigned int x;
        if (ncenter.x > octCenter.x)
            x = 1;
        else
            x = 0;

        unsigned int y;
        if (ncenter.y > octCenter.y)
            y = 1;
        else
            y = 0;

        unsigned int z;
        if (ncenter.z > octCenter.z)
            z = 1;
        else
            z = 0;

        if (mChildren[ x ][ y ][ z ] == 0)
        {
            const Vector3 &octantMax = mBox.getMaximum();
            const Vector3 &octantMin = mBox.getMinimum();

            Vector3 min, max;

            if (x == 0)
            {
                min.x = octantMin.x;
                max.x = octCenter.x;
            }
            else
            {
                min.x = octCenter.x;
                max.x = octantMax.x;
            }

            if (y == 0)
            {
                min.y = octantMin.y;
                max.y = octCenter.y;
            }
            else
            {
                min.y = octCenter.y;
                max.y = octantMax.y;
            }

            if (z == 0)
            {
                min.z = octantMin.z;
                max.z = octCenter.z;
            }
            else
            {
                min.z = octCenter.z;
                max.z = octantMax.z;
            }

            PagingLandScapeOctree *newChild = scn->getNewOctree();
            newChild->setParent (this);
            newChild->setSceneManager(scn);
            newChild ->setBoundingBox(min, max);
            scn->registeredNodeInCamera (newChild);
            #ifdef _VISIBILITYDEBUG   
                newChild ->setDebugCorners(scn);
            #endif //_VISIBILITYDEBUG    
            mChildren[x][y][z] = newChild;
        }
        return mChildren[x][y][z];
            
    }
    //-----------------------------------------------------------------------
    PagingLandScapeOctree *PagingLandScapeOctree::_getChildWhereBoxFits(const AxisAlignedBox &box, 
                                                                        PagingLandScapeOctreeSceneManager *scn)
    { 
        /** It's assumed the the given box has already been proven to fit into
        * a child.  Since it's a loose PagingLandScapeOctree, only the centers need to be
        * compared to find the appropriate node.
        */
        const Vector3 &octantMax = mBox.getMaximum();
        const Vector3 &octantMin = mBox.getMinimum();
        const Vector3 octCenter = octantMin + mHalfSize;   

        const Vector3 ncenter = box.getMaximum().midPoint(box.getMinimum());

        assert (octantMax.x >= ncenter.x && octantMax.y >= ncenter.y && octantMax.z >= ncenter.z &&
                octantMin.x <= ncenter.x && octantMin.y <= ncenter.y && octantMin.z <= ncenter.z);

        unsigned int x;
        if (ncenter.x > octCenter.x)
            x = 1;
        else
            x = 0;

        unsigned int y;
        if (ncenter.y > octCenter.y)
            y = 1;
        else
            y = 0;

        unsigned int z;
        if (ncenter.z > octCenter.z)
            z = 1;
        else
            z = 0;

        if (mChildren[ x ][ y ][ z ] == 0)
        {
            Vector3 min, max;

            if (x == 0)
            {
                min.x = octantMin.x;
                max.x = octCenter.x;
            }
            else
            {
                min.x = octCenter.x;
                max.x = octantMax.x;
            }

            if (y == 0)
            {
                min.y = octantMin.y;
                max.y = octCenter.y;
            }
            else
            {
                min.y = octCenter.y;
                max.y = octantMax.y;
            }

            if (z == 0)
            {
                min.z = octantMin.z;
                max.z = octCenter.z;
            }
            else
            {
                min.z = octCenter.z;
                max.z = octantMax.z;
            }

#ifdef _DEBUG
            std::cout << "new Child\n";
#endif

            PagingLandScapeOctree *newChild = scn->getNewOctree();
            newChild->setParent (this);
            newChild->setSceneManager(scn);
            newChild->setBoundingBox(min, max);

            assert (max.x >= ncenter.x && max.y >= ncenter.y && max.z >= ncenter.z &&
                    min.x <= ncenter.x && min.y <= ncenter.y && min.z <= ncenter.z);
          

            scn->registeredNodeInCamera (newChild);
            #ifdef _VISIBILITYDEBUG   
                newChild ->setDebugCorners(scn);
            #endif //_VISIBILITYDEBUG    
            mChildren[x][y][z] = newChild;
        }
        return mChildren[x][y][z];
            
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeOctree::setBoundingBox(const Vector3 &min, 
                                                const Vector3 &max)
    {
        mBox.setExtents(min, max);
        mCullHalfSize = max - min;
        const Vector3 halfSize =  mCullHalfSize * 0.5f;
        mHalfSize = halfSize;
        mCullBox.setExtents(min - halfSize, max + halfSize);
        if(mWireBoundingBox != 0)
            mWireBoundingBox->setupBoundingBox(mBox);
        if(mOcclusionBoundingBox != 0)
            mOcclusionBoundingBox->setupBoundingBox(mBox);
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeOctree::_addNode(PagingLandScapeOctreeNode * n)
    {
        if (n->isStaticNode ())
            mStaticNodes.push_back (n);
        else
            mMovingNodes.push_back (n);
        mNodes.push_back (n);
        n->setOctant(this);
        //update total counts.
        _ref();
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeOctree::_removeNode(PagingLandScapeOctreeNode * n)
    {  
        assert (!mNodes.empty());

		NodeList::iterator it;
       
		it = std::find (mNodes.begin (), mNodes.end (), n); 
		if (it != mNodes.end ()) 
			mNodes.erase (it);
			
        if (n->isStaticNode ())
		{
			it = std::find(mStaticNodes.begin (), mStaticNodes.end (), n); 
			if (it != mStaticNodes.end ()) 
				mStaticNodes.erase (it);
        }
        else
        {
			it = std::find(mMovingNodes.begin (), mMovingNodes.end (), n); 
			if (it != mMovingNodes.end ()) 
				mMovingNodes.erase (it);
        }
        n->setOctant(0);
        //update total counts.
        _unref(mNumNodes == 1);
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeOctree::_unref(const bool removeChildren)
    {
        --mNumNodes;
        if (removeChildren)
        {
            //remove null children
            PagingLandScapeOctree *child;
            for (unsigned int i = 0; i < 2; i++)
            {
                for (unsigned int j = 0; j < 2; j++)
                {
                    for (unsigned int k = 0; k < 2; k++)
                    {
                        child = mChildren[ i ][ j ][ k ];
                        if (child && !child->hasChildren())
                        {
                            mSceneMgr->deleteOctree(child);
                            mChildren[ i ][ j ][ k ] = 0;
                        }
                    }
                }
            }
        }
        if (mParent != 0)
        {
            mParent->_unref(mNumNodes == 0);
        }
    };
    //-----------------------------------------------------------------------
    void PagingLandScapeOctree::_getCullBounds(AxisAlignedBox *b) const
    {
       // const Vector3 * const corners = mBox.getAllCorners();
       // b->setExtents(corners[ 0 ] - mHalfSize, corners[ 4 ] + mHalfSize);
        *b = mCullBox;
    }
    //-----------------------------------------------------------------------
    WireBoundingBox* PagingLandScapeOctree::getWireBoundingBox()
    {
        // Create a WireBoundingBox if needed
        if (mWireBoundingBox == 0)
        {
            mWireBoundingBox = new WireBoundingBox();
            //mWireBoundingBox->setRenderQueueGroup (RENDER_QUEUE_WORLD_GEOMETRY_2);
            mWireBoundingBox->setupBoundingBox(mBox);
        }

        return mWireBoundingBox;
    }
    //-----------------------------------------------------------------------
    OcclusionBoundingBox* PagingLandScapeOctree::getOcclusionBoundingBox()
    {
        // Create an OcclusionBoundingBox only if needed
        if (mOcclusionBoundingBox == 0)
        {
            mOcclusionBoundingBox = new OcclusionBoundingBox();
            //mOcclusionBoundingBox->setRenderQueueGroup (RENDER_QUEUE_WORLD_GEOMETRY_2);
            mOcclusionBoundingBox->setupBoundingBox (mBox);
        }
        return mOcclusionBoundingBox;     
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeOctree::setParent(PagingLandScapeOctree * parent)
    {
        mParent = parent;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeOctree::setSceneManager(PagingLandScapeOctreeSceneManager * scn)
    {
        mSceneMgr = scn;
    }
#ifdef _VISIBILITYDEBUG
        //-----------------------------------------------------------------------
        void PagingLandScapeOctree::setDebugCorners(PagingLandScapeOctreeSceneManager *scnMgr)
        {
            // use the ratio (OCtreeAAB / thisAAB) / 8
            const Vector3 &max =  mBox.getMaximum ();
            const Vector3 &min =  mBox.getMinimum ();
            
            const Vector3 &sceneMax = scnMgr->getBoundingBox ().getMaximum ();
            const Vector3 &sceneMin = scnMgr->getBoundingBox ().getMinimum ();

            const Real width = sceneMax.x - sceneMin.x;
            const Real height = sceneMax.z - sceneMin.z;
            if (Math::Abs(width) > 1e-08 && Math::Abs(height)  > 1e-08)
            {    
                Real left =  0.0f, top =  0.0f, right =  0.0f, bottom =  0.0f;
          
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
