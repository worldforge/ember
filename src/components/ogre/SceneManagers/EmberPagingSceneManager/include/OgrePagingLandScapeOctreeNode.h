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
PagingLandScapeOctreenode.h  -  description
-------------------
begin                : Fri Sep 27 2002
copyright            : (C) 2002 by Jon Anderson
email                : janders@users.sf.net

Enhancements 2003 - 2004 (C) The OGRE Team
***************************************************************************/

#ifndef PagingLandScapeOctreeNODE_H
#define PagingLandScapeOctreeNODE_H

#include <OgreSceneNode.h>

#include <OgrePagingLandScapeOctreeSceneManager.h>

namespace Ogre
{

    /** Specialized SceneNode that is customized for working within an PagingLandScapeOctree. Each node
    * maintains it's own bounding box, rather than merging it with all the children.
    *
    */
    class PagingLandScapeOctreeNode : public SceneNode
    {
        public:
            /** Standard constructor */
            PagingLandScapeOctreeNode( SceneManager* creator );
            /** Standard constructor */
            PagingLandScapeOctreeNode( SceneManager* creator, const String& name );
            /** Standard destructor */
            virtual ~PagingLandScapeOctreeNode( void );

            /** Overridden from Node to remove any reference to octants */
            Node* removeChild( unsigned short index );
            
            /** Overridden from Node to remove any reference to octants */
            Node* removeChild( const String& name );

            /** Overridden from Node to remove any reference to octants */
            Node* removeChild( Node* child);

            /** Returns the PagingLandScapeOctree in which this PagingLandScapeOctreeNode resides
            */
            PagingLandScapeOctree* getOctant( void ) 
            {
                return mOctant;
            };

            /** Sets the PagingLandScapeOctree in which this PagingLandScapeOctreeNode resides
            */
            void setOctant( PagingLandScapeOctree* o )
            {
                mOctant = o;
            };

            /** Determines if the center of this node is within the given box
            */
            bool _isIn( const AxisAlignedBox& box ) const;

            /** Adds all the attached scene nodes to the render queue
            */
            virtual void _addToRenderQueue( Camera* cam, RenderQueue* const q, const bool onlyShadowCasters );

            /** Sets up the LegacyRenderOperation for rendering this scene node as geometry.
            @remarks
            This will render the scene node as a bounding box.
            */
            virtual void getRenderOperation( RenderOperation& op );

            /** Returns the local bounding box of this PagingLandScapeOctreeNode.
            @remarks
            This is used to render the bounding box, rather then the global.
            */
            AxisAlignedBox& _getLocalAABB( void )
            {
                return mLocalAABB;
            };

        protected:
            /** Internal method for updating the bounds for this PagingLandScapeOctreeNode.
            @remarks
				This method determines the bounds solely from the attached objects, not
				any children. If the node has changed it's bounds, it is removed from its
				current PagingLandScapeOctree, and reinserted into the tree.
            */
            void _updateBounds( void );

            void _removeNodeAndChildren( void );

            ///local bounding box
            AxisAlignedBox mLocalAABB;

            ///PagingLandScapeOctree this node is attached to.
            PagingLandScapeOctree* mOctant;

            ///preallocated corners for rendering
            Real mCorners[ 24 ];

            ///shared colors for rendering
            static unsigned long mColors[ 8 ];
            ///shared indexes for rendering
            static unsigned short mIndexes[ 24 ];

    };

}


#endif
