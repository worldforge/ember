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
PagingLandScapeOctreenode.h  -  description
-------------------
begin                : Fri Sep 27 2002
copyright            : (C) 2002 by Jon Anderson
email                : janders@users.sf.net

Enhancements 2003 - 2004 (C) The OGRE Team
***************************************************************************/
#ifndef __PagingLandScapeOctreeNODE_H
#define __PagingLandScapeOctreeNODE_H

#include "OgreSceneNode.h"
#include "OgrePagingLandScapePrerequisites.h"

#include "OgrePagingLandScapeOctreeSceneManager.h"
#include "OgrePagingLandScapeOctree.h"
#include "OgrePagingLandScapeOcclusionElement.h"

namespace Ogre
{

    /** Specialized SceneNode that is customized for working within an PagingLandScapeOctree. Each node
    * maintains it's own bounding box, rather than merging it with all the children.
    *
    */
    class PagingLandScapeOctreeNode : public SceneNode, public OcclusionElement
    {
        public:
            /** Standard constructor */
            PagingLandScapeOctreeNode(SceneManager* creator);
            /** Standard constructor */
            PagingLandScapeOctreeNode(SceneManager* creator, const String& name);
            /** Standard destructor */
            ~PagingLandScapeOctreeNode(void);

            /** Returns the PagingLandScapeOctree in which this PagingLandScapeOctreeNode resides
            */
            PagingLandScapeOctree* getOctant(void) 
            {
                return mOctant;
            };

            /** Sets the PagingLandScapeOctree in which this PagingLandScapeOctreeNode resides
            */
            void setOctant(PagingLandScapeOctree* o)
            {
                mOctant = o;
            };

            /** Determines if the center of this node is within the given box
            */
            bool _isIn(const AxisAlignedBox& box) const;

            /** Adds all the attached scene nodes to the render queue
            */
//            virtual void _findVisibleObjects(Camera* cam, RenderQueue* queue,
//            			VisibleObjectsBoundsInfo* visibleBounds,
//                        bool includeChildren = true, bool displayNodes = false, bool onlyShadowCasters = false);
            
            MovableObjectList *getVisibleNotifiedNodeObjects(Camera*  cam, 
                                        const bool onlyShadowCasters);
             
            bool notifyNodeObjects(Camera*  cam, 
                                    const bool onlyShadowCasters);
            /** Adds all the attached scene nodes to the render queue
            * with the particularity that it has already been notified by camera.
            */
            void _addAlreadyNotifiedToVisibles();
            void _addToVisibles(Camera*  cam, const bool onlyShadowCasters);
            /** Sets up the LegacyRenderOperation for rendering this scene node as geometry.
            @remarks
            This will render the scene node as a bounding box.
            */
            //virtual void getRenderOperation(RenderOperation& op);

            /** Returns the local bounding box of this PagingLandScapeOctreeNode.
            @remarks
            This is used to render the bounding box, rather then the global.
            */
            AxisAlignedBox& _getLocalAABB(void)
            {
                return mLocalAABB;
            };
            /** Adds a (pre-created) child scene node to this node. If it is attached to another node,
                it must be detached first.
            @param child The Node which is to become a child node of this one
            */
            virtual void addChild(Node* child);
            /** Drops the specified child from this node. 
            @remarks
                Does not delete the node, just detaches it from
                this parent, potentially to be reattached elsewhere. 
                There is also an alternate version which drops a named
                child from this node.
            */
            virtual Node* removeChild(unsigned short index);
            /** Drops the specified child from this node. 
            @remarks
            Does not delete the node, just detaches it from
            this parent, potentially to be reattached elsewhere. 
            There is also an alternate version which drops a named
            child from this node.
            */
            virtual Node* removeChild(Node* child);

            /** Drops the named child from this node. 
            @remarks
                Does not delete the node, just detaches it from
                this parent, potentially to be reattached elsewhere.
            */
            virtual Node* removeChild(const String& name);
            /** Removes all child Nodes attached to this node. Does not delete the nodes, just detaches them from
                this parent, potentially to be reattached elsewhere.
            */
            virtual void removeAllChildren(void);



            virtual OcclusionElement* getParent ()  {return mOctant;};   


            inline const AxisAlignedBox &getCullBoundingBox() const 
            {
                return _getWorldAABB();
            };
          

            virtual const Vector3 &getHalfSize()  const 
            {
                return mHalfSize;
            };
            
            virtual bool isLeaf() const {return true;};

            virtual void traversal(Traversal&tr, VisibleObjectsBoundsInfo * const visibleBounds)
            {   
                tr.onLeaf (*this, visibleBounds);
            };
            virtual void traversal(const TraversalConst &tr, VisibleObjectsBoundsInfo * const visibleBounds)
            {  
                tr.onLeaf(*this, visibleBounds);
            };
            virtual void traversal(const ConstTraversalConst &tr, VisibleObjectsBoundsInfo * const visibleBounds) const
            {   
                tr.onLeaf (*this, visibleBounds);
            };             
		    /** creates the opaque bounding box for this octant
		    */
		    OcclusionBoundingBox* getOcclusionBoundingBox();

            #ifdef _VISIBILITYDEBUG
                void setDebugCorners(PagingLandScapeOctreeSceneManager *scnMgr);
            #endif //_VISIBILITYDEBUG    

             /** Adds an instance of a scene object to this node.
                @remarks
                    Scene objects can include Entity objects, Camera objects, Light objects, 
                    ParticleSystem objects etc. Anything that subclasses from MovableObject.
            */
            virtual void attachObject(MovableObject* obj);   
            /** Detaches the indexed object from this scene node.
            @remarks
                Detaches by index, see the alternate version to detach by name. Object indexes
                may change as other objects are added / removed.
            */
            virtual MovableObject* detachObject(unsigned short index);
            /** Detaches an object by pointer. */
            virtual void detachObject(MovableObject* obj);

            /** Detaches the named object from this node and returns a pointer to it. */
            virtual MovableObject* detachObject(const String& name);

            /** Detaches all objects attached to this node.
            */
            virtual void detachAllObjects(void);

            virtual bool isOccluder () const  { return mIsOccluder; }    

            bool isStaticNode() const { return mIsStaticNode; }

            void setStaticCulling (const bool staticNode) { mIsStaticNode = staticNode; };


        protected:
            /** Internal method for updating the bounds for this PagingLandScapeOctreeNode.
            @remarks
				This method determines the bounds solely from the attached objects, not
				any children. If the node has changed it's bounds, it is removed from its
				current PagingLandScapeOctree, and reinserted into the tree.
            */
            void _updateBounds(void);

            void _removeNodeAndChildren(void);

            ///local bounding box
            AxisAlignedBox          mLocalAABB;

            ///PagingLandScapeOctree this node is attached to.
            PagingLandScapeOctree*  mOctant;
            OcclusionBoundingBox*   mOcclusionBoundingBox;
            Vector3                 mHalfSize;
            /// if node need to in loose octree or normal octree
            bool                    mIsStaticNode;
            /// if node need to be drawn during query phase
            bool                    mIsOccluder;
            // if a pre-notified object is visible.
            MovableObjectList       mNotifiedVisibles;
            
    };

}


#endif //__PagingLandScapeOctreeNODE_H
