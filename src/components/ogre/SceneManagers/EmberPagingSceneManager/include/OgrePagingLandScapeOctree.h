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
PagingLandScapeOctree.h  -  description
-------------------
begin                : Mon Sep 30 2002
copyright            : (C) 2002 by Jon Anderson
email                : janders@users.sf.net

Enhancements 2003 - 2004 (C) The OGRE Team
***************************************************************************/

#ifndef PagingLandScapeOctree_H
#define PagingLandScapeOctree_H

#include <OgreAxisAlignedBox.h>
#include <OgreWireBoundingBox.h>

#include <list>

namespace Ogre
{

	class PagingLandScapeOctreeNode;

	typedef std::list < PagingLandScapeOctreeNode* > NodeList;


	/** PagingLandScapeOctree data structure for managing scene nodes.
	@remarks
		This is a loose PagingLandScapeOctree implementation, meaning that each
		octant child of the PagingLandScapeOctree actually overlaps it's siblings by a factor
		of .5.  This guarantees that any thing that is half the size of the parent will
		fit completely into a child, with no splitting necessary.
	*/

	class PagingLandScapeOctree
	{
	public:
	    PagingLandScapeOctree( PagingLandScapeOctree* p );
		~PagingLandScapeOctree( void );

		/** Adds an PagingLandScapeOctree scene node to this PagingLandScapeOctree level.
		@remarks
			This is called by the PagingLandScapeOctreeSceneManager after
			it has determined the correct PagingLandScapeOctree to insert the node into.
		*/
		void _addNode( PagingLandScapeOctreeNode* nod );

	    /** Removes an PagingLandScapeOctree scene node to this PagingLandScapeOctree level.
	    */
	    void _removeNode( PagingLandScapeOctreeNode* nod );

		/** Returns the number of scene nodes attached to this PagingLandScapeOctree
		*/
		int numNodes( void ) const
		{
	        return mNumNodes;
	    };

		/** The bounding box of the PagingLandScapeOctree
		@remarks
			This is used for octant index determination and rendering, but not culling
		*/
		AxisAlignedBox mBox;
		WireBoundingBox* mWireBoundingBox;
    
		/** creates the wire frame bounding box for this octant
		*/
		WireBoundingBox* getWireBoundingBox( void );

		/** Vector containing the dimensions of this PagingLandScapeOctree / 2
		*/
		Vector3 mHalfSize;

		/** 3D array of children of this PagingLandScapeOctree.
		@remarks
			Children are dynamically created as needed when nodes are inserted in the PagingLandScapeOctree.
			If, later, the all the nodes are removed from the child, it is still kept around.
		*/
		PagingLandScapeOctree* mChildren[ 2 ][ 2 ][ 2 ];

		/** Determines if this PagingLandScapeOctree is twice as big as the given box.
		@remarks
			This method is used by the PagingLandScapeOctreeSceneManager to determine if the given
			box will fit into a child of this PagingLandScapeOctree.
		*/
		bool _isTwiceSize( const AxisAlignedBox& box ) const;

		/**  Returns the appropriate indexes for the child of this PagingLandScapeOctree into which the box will fit.
		@remarks
			This is used by the PagingLandScapeOctreeSceneManager to determine which child to traverse next when
			finding the appropriate PagingLandScapeOctree to insert the box.  Since it is a loose PagingLandScapeOctree, only the
		center of the box is checked to determine the octant.
		*/
		void _getChildIndexes( const AxisAlignedBox&, int &x, int &y, int &z ) const;

		/** Creates the AxisAlignedBox used for culling this PagingLandScapeOctree.
		@remarks
			Since it's a loose PagingLandScapeOctree, the culling bounds can be different than the actual bounds of the PagingLandScapeOctree.
		*/
		void _getCullBounds( AxisAlignedBox* bound ) const;

		/** Public list of SceneNodes attached to this particular PagingLandScapeOctree
		*/
		NodeList mNodes;

	protected:

		/** Increments the overall node count of this PagingLandScapeOctree and all it's parents
	    */
	    inline void _ref( void )
		{
	        ++mNumNodes;
			if ( mParent != 0 )
			{
				mParent->_ref( );
			}
		};

		/** Decrements the overall node count of this PagingLandScapeOctree and all it's parents
		*/
		inline void _unref( void )
		{
	        --mNumNodes;
			if ( mParent != 0 )
			{
				mParent->_unref( );
			}
		};

		///number of SceneNodes in this PagingLandScapeOctree and all it's children.
		int mNumNodes;

		///parent PagingLandScapeOctree
		PagingLandScapeOctree* mParent;
	};

}

#endif
