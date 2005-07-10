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
PagingLandScapeOctree.cpp  -  description
-------------------
begin                : Mon Sep 30 2002
copyright            : (C) 2002 by Jon Anderson
email                : janders@users.sf.net

Enhancements 2003 - 2004 (C) The OGRE Team

***************************************************************************/

#include "OgrePagingLandScapeOctree.h"
#include "OgrePagingLandScapeOctreeNode.h"

namespace Ogre
{

/** Returns true is the box will fit in a child.
*/
bool PagingLandScapeOctree::_isTwiceSize( const AxisAlignedBox &box ) const
{
    const Vector3 * const pts1 = mBox.getAllCorners();
    const Vector3 * const pts2 = box.getAllCorners();

    return ( ( pts2[ 4 ].x - pts2[ 0 ].x ) <= ( pts1[ 4 ].x - pts1[ 0 ].x ) * 0.5f ) &&
           ( ( pts2[ 4 ].y - pts2[ 0 ].y ) <= ( pts1[ 4 ].y - pts1[ 0 ].y ) * 0.5f ) &&
           ( ( pts2[ 4 ].z - pts2[ 0 ].z ) <= ( pts1[ 4 ].z - pts1[ 0 ].z ) * 0.5f ) ;

}

/** It's assumed the the given box has already been proven to fit into
* a child.  Since it's a loose PagingLandScapeOctree, only the centers need to be
* compared to find the appropriate node.
*/
void PagingLandScapeOctree::_getChildIndexes( const AxisAlignedBox &box, int &x, int &y, int &z ) const
{
    const Vector3 center = mBox.getMaximum().midPoint( mBox.getMinimum() );

    const Vector3 ncenter = box.getMaximum().midPoint( box.getMinimum() );

    if ( ncenter.x > center.x )
        x = 1;
    else
        x = 0;

    if ( ncenter.y > center.y )
        y = 1;
    else
        y = 0;

    if ( ncenter.z > center.z )
        z = 1;
    else
        z = 0;

}

PagingLandScapeOctree::PagingLandScapeOctree( PagingLandScapeOctree * parent ) : 
        mWireBoundingBox(0),
        mHalfSize( 0.0f,  0.0f,  0.0f ),
        mNumNodes (0),
        mParent (parent)
{
    //initialize all children to null.
    for ( uint i = 0; i < 2; i++ )
    {
        for ( uint j = 0; j < 2; j++ )
        {
            for ( uint k = 0; k < 2; k++ )
            {
                mChildren[ i ][ j ][ k ] = 0;
            }
        }
    }

}

PagingLandScapeOctree::~PagingLandScapeOctree()
{
    //initialize all children to null.
    for ( uint i = 0; i < 2; i++ )
    {
        for ( uint j = 0; j < 2; j++ )
        {
            for ( uint k = 0; k < 2; k++ )
            {
                if ( mChildren[ i ][ j ][ k ] != 0 )
                    delete mChildren[ i ][ j ][ k ];
            }
        }
    }

    if(mWireBoundingBox)
        delete mWireBoundingBox;

    mParent = 0;
}

void PagingLandScapeOctree::_addNode( PagingLandScapeOctreeNode * n )
{
    mNodes.push_back( n );
    n -> setOctant( this );

    //update total counts.
    _ref();

}

void PagingLandScapeOctree::_removeNode( PagingLandScapeOctreeNode * n )
{
    mNodes.erase( std::find( mNodes.begin(), mNodes.end(), n ) );
    n -> setOctant( 0 );

    //update total counts.
    _unref();
}

void PagingLandScapeOctree::_getCullBounds( AxisAlignedBox *b ) const
{
    const Vector3 * const corners = mBox.getAllCorners();
    b -> setExtents( corners[ 0 ] - mHalfSize, corners[ 4 ] + mHalfSize );
}

WireBoundingBox* PagingLandScapeOctree::getWireBoundingBox()
{
    // Create a WireBoundingBox if needed
    if(mWireBoundingBox == 0)
        mWireBoundingBox = new WireBoundingBox();

    mWireBoundingBox->setupBoundingBox(mBox);
    return mWireBoundingBox;
}

}
