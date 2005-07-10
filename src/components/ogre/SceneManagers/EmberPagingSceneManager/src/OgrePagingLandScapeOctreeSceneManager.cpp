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

#include <OgrePagingLandScapeOctreeSceneManager.h>
#include <OgrePagingLandScapeOctreeAxisAlignedBoxSceneQuery.h>
#include <OgrePagingLandScapeOctreeIntersectionSceneQuery.h>
#include <OgrePagingLandScapeOctreePlaneBoundedVolumeListSceneQuery.h>
#include <OgrePagingLandScapeOctreeRaySceneQuery.h>
#include <OgrePagingLandScapeOctreeSphereSceneQuery.h>
#include <OgrePagingLandScapeOctreeNode.h>
#include <OgrePagingLandScapeOctreeCamera.h>
#include <OgreRenderSystem.h>


extern "C"
{
    void findNodesInBox( Ogre::SceneManager *sm,
                         const Ogre::AxisAlignedBox &box,
                         std::list < Ogre::SceneNode * > &list,
                         const Ogre::SceneNode * const exclude )
    {
        static_cast<Ogre::PagingLandScapeOctreeSceneManager*>( sm ) -> findNodesIn( box, list, exclude );
    }
    void findNodesInSphere( Ogre::SceneManager *sm,
                            const Ogre::Sphere &sphere,
                            std::list < Ogre::SceneNode * > &list,
                            const Ogre::SceneNode * const exclude )
    {
        static_cast<Ogre::PagingLandScapeOctreeSceneManager*>( sm ) -> findNodesIn( sphere, list, exclude );
    }
}

namespace Ogre
{
enum Intersection
{
    OUTSIDE=0,
    INSIDE=1,
    INTERSECT=2
};
int PagingLandScapeOctreeSceneManager::intersect_call = 0;

Intersection intersect( const Ray &one, const AxisAlignedBox &two )
{
    PagingLandScapeOctreeSceneManager::intersect_call++;

    // Null box?
    if (two.isNull()) 
        return OUTSIDE;


    const Vector3* const pCorners = two.getAllCorners();
    const Vector3 origin = one.getOrigin();
    const Vector3 dir = one.getDirection();

    Vector3 maxT(-1.0f, -1.0f, -1.0f);

    int i = 0;
    bool inside = true;
    for(i=0; i<3; i++ )
    {
        if( origin[i] < pCorners[0][i] )
        {
            inside = false;
            if( dir[i] > 0 )
            {
                maxT[i] = (pCorners[0][i] - origin[i])/ dir[i];
            }
        }
        else if( origin[i] > pCorners[4][i] )
        {
            inside = false;
            if( dir[i] < 0 )
            {
                maxT[i] = (pCorners[4][i] - origin[i]) / dir[i];
            }
        }
    }

    if( inside )
    {
        return INTERSECT;
    }
    int whichPlane = 0;
    if( maxT[1] > maxT[whichPlane])
        whichPlane = 1;
    if( maxT[2] > maxT[whichPlane])
        whichPlane = 2;

    if( ((int)maxT[whichPlane]) & 0x80000000 )
    {
        return OUTSIDE;
    }
    for(i=0; i<3; i++ )
    {
        if( i!= whichPlane )
        {
            const Real f = origin[i] + maxT[whichPlane] * dir[i];
            if ( f < (pCorners[0][i] - 0.00001f) ||
                    f > (pCorners[4][i] +0.00001f ) )
            {
                return OUTSIDE;
            }
        }
    }

    return INTERSECT;

}


/** Checks how the second box intersects with the first.
*/
Intersection intersect( const PlaneBoundedVolume &one, const AxisAlignedBox &two )
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
        for ( unsigned int corner = 0; corner < 8; ++corner )
        {
            const Real distance = plane.getDistance( pCorners[ corners[ corner ] ] );
            all_outside = all_outside && ( distance < 0 );
            all_inside = all_inside && ( distance >= 0 );

            if ( !all_outside && !all_inside )
                break;
        }

        if ( all_outside )
            return OUTSIDE;
    }

    if ( all_inside )
        return INSIDE;
    else
        return INTERSECT;

}


/** Checks how the second box intersects with the first.
*/
Intersection intersect( const AxisAlignedBox &one, const AxisAlignedBox &two )
{
    PagingLandScapeOctreeSceneManager::intersect_call++;

    // Null box?
    if (one.isNull() || two.isNull()) 
        return OUTSIDE;

    const Vector3 * outside = one.getAllCorners();
    const Vector3 *inside = two.getAllCorners();

    if ( inside[ 4 ].x < outside[ 0 ].x ||
            inside[ 4 ].y < outside[ 0 ].y ||
            inside[ 4 ].z < outside[ 0 ].z ||
            inside[ 0 ].x > outside[ 4 ].x ||
            inside[ 0 ].y > outside[ 4 ].y ||
            inside[ 0 ].z > outside[ 4 ].z )
    {
        return OUTSIDE;
    }

    const bool full = ( inside[ 0 ].x > outside[ 0 ].x &&
                  inside[ 0 ].y > outside[ 0 ].y &&
                  inside[ 0 ].z > outside[ 0 ].z &&
                  inside[ 4 ].x < outside[ 4 ].x &&
                  inside[ 4 ].y < outside[ 4 ].y &&
                  inside[ 4 ].z < outside[ 4 ].z );

    if ( full )
        return INSIDE;
    else
        return INTERSECT;

}

/** Checks how the box intersects with the sphere.
*/
Intersection intersect( const Sphere &one, const AxisAlignedBox &two )
{
    PagingLandScapeOctreeSceneManager::intersect_call++;

    // Null box?
    if (two.isNull()) 
        return OUTSIDE;


    const Real sradius = Math::Sqr (one.getRadius());
    const Vector3 scenter = one.getCenter();
    const Vector3 * const corners = two.getAllCorners();

    const Vector3 mndistance = ( corners[ 0 ] - scenter );
    const Vector3 mxdistance = ( corners[ 4 ] - scenter );


    if ( mndistance.squaredLength() < sradius &&
            mxdistance.squaredLength() < sradius )
    {
        return INSIDE;
    }

    //find the square of the distance
    //from the sphere to the box
    Real d = 0;
    for ( uint i = 0 ; i < 3 ; i++ )
    {
        const Real sCenteri = scenter[ i ];
        if ( sCenteri < corners[ 0 ][ i ] )
        {
            const Real s = sCenteri - corners[ 0 ][ i ];
            d += s * s;
        }
        else if ( sCenteri > corners[ 4 ][ i ] )
        {
            const Real s = sCenteri - corners[ 4 ][ i ];
            d += s * s;
        }

    }

    const bool partial = ( d <= sradius );

    if ( !partial )
    {
        return OUTSIDE;
    }

    else
    {
        return INTERSECT;
    }


}

unsigned long white = 0xFFFFFFFF;

unsigned short PagingLandScapeOctreeSceneManager::mIndexes[ 24 ] = 
       {0, 1, 1, 2, 2, 3, 3, 0,       //back
        0, 6, 6, 5, 5, 1,             //left
        3, 7, 7, 4, 4, 2,             //right
        6, 7, 5, 4 };          //front

unsigned long PagingLandScapeOctreeSceneManager::mColors[ 8 ] = {white, white, 
                                                white, white, 
                                                white, white, 
                                                white, white };


PagingLandScapeOctreeSceneManager::PagingLandScapeOctreeSceneManager( ) : SceneManager(),
    mPagingLandScapeOctree(0)
{
    AxisAlignedBox b( -10000, -10000, -10000, 10000, 10000, 10000 );
    int depth = 8; 
    init( b, depth );
}

PagingLandScapeOctreeSceneManager::PagingLandScapeOctreeSceneManager( AxisAlignedBox &box, int max_depth ) : SceneManager(),
    mPagingLandScapeOctree(0)
{
    init( box, max_depth );
}

void PagingLandScapeOctreeSceneManager::init( const AxisAlignedBox &box, int depth )
{
    delete mSceneRoot; //get rid of old root.

    // -- Changes by Steve
    // Don't do it this way, it will add it to the mSceneNodes which we don't want
    //mSceneRoot = createSceneNode( "SceneRoot" );
    mSceneRoot = new PagingLandScapeOctreeNode( this, "SceneRoot" );
	mSceneRoot->_notifyRootNode();
    // -- End changes by Steve

    delete mPagingLandScapeOctree;

    mPagingLandScapeOctree = new PagingLandScapeOctree( 0 );

    mMaxDepth = depth;
    mBox = box;

    mPagingLandScapeOctree -> mBox = box;

    const Vector3 min = box.getMinimum();

    const Vector3 max = box.getMaximum();

    mPagingLandScapeOctree -> mHalfSize = ( max - min ) * 0.5f;

/////////////////////////
    mShowBoxes = false;//true;
    mCullCamera = false;//true;

    mNumObjects = 0;

    const Vector3 v( 1.5f, 1.5f, 1.5f );

    mScaleFactor.setScale( v );



//     setDisplaySceneNodes( true );
//     setShowBoxes( true );

    //
    //setUseCullCamera( true );

    //mSceneRoot isn't put into the PagingLandScapeOctree since it has no volume.

}

PagingLandScapeOctreeSceneManager::~PagingLandScapeOctreeSceneManager()
{
    // -- Changed by Steve
    // Don't do this here, SceneManager will do it
    /*
    if( mSceneRoot )
    delete mSceneRoot;
    */ 
    // --End Changes by Steve

    delete mPagingLandScapeOctree;
}

Camera * PagingLandScapeOctreeSceneManager::createCamera( const String &name )
{
    Camera * c = new PagingLandScapeOctreeCamera( name, this );
    mCameras.insert( CameraList::value_type( name, c ) );
    return c;
}

void PagingLandScapeOctreeSceneManager::destroySceneNode( const String &name )
{
    PagingLandScapeOctreeNode * on = static_cast < PagingLandScapeOctreeNode* > ( SceneManager::getSceneNode( name ) );

    if ( on != 0 )
        _removePagingLandScapeOctreeNode( on );

    SceneManager::destroySceneNode( name );
}

bool PagingLandScapeOctreeSceneManager::getOptionValues( const String & key, StringVector  &refValueList )
{
    return SceneManager::getOptionValues( key, refValueList );
}

bool PagingLandScapeOctreeSceneManager::getOptionKeys( StringVector & refKeys )
{
    SceneManager::getOptionKeys( refKeys );

    refKeys.push_back( "CullCamera" );
    refKeys.push_back( "Size" );
    refKeys.push_back( "ShowPagingLandScapeOctree" );
    refKeys.push_back( "Depth" );

    return true;
}


void PagingLandScapeOctreeSceneManager::_updatePagingLandScapeOctreeNode( PagingLandScapeOctreeNode * onode )
{
    const AxisAlignedBox box = onode -> _getWorldAABB();

    if ( box.isNull() )
        return ;

    assert (onode && mPagingLandScapeOctree);

    if ( onode -> getOctant() == 0 )
    {
        //if outside the PagingLandScapeOctree, force into the root node.
        if ( ! onode -> _isIn( mPagingLandScapeOctree -> mBox ) )
            mPagingLandScapeOctree->_addNode( onode );
        else
            _addPagingLandScapeOctreeNode( onode, mPagingLandScapeOctree );
        return ;
    }

    if ( ! onode -> _isIn( onode -> getOctant() -> mBox ) )
    {
        _removePagingLandScapeOctreeNode( onode );

        //if outside the PagingLandScapeOctree, force into the root node.
        if ( ! onode -> _isIn( mPagingLandScapeOctree -> mBox ) )
            mPagingLandScapeOctree->_addNode( onode );
        else
            _addPagingLandScapeOctreeNode( onode, mPagingLandScapeOctree );
    }
}

/** Only removes the node from the PagingLandScapeOctree.  It leaves the PagingLandScapeOctree, even if it's empty.
*/
void PagingLandScapeOctreeSceneManager::_removePagingLandScapeOctreeNode( PagingLandScapeOctreeNode * n ) const 
{
    assert (n);
    PagingLandScapeOctree * oct = n -> getOctant();

    if ( oct )
    {
        oct -> _removeNode( n );
    }

    n->setOctant(0);
}


void PagingLandScapeOctreeSceneManager::_addPagingLandScapeOctreeNode( PagingLandScapeOctreeNode * n, PagingLandScapeOctree *octant, int depth )
{
    assert (n && octant);

    const AxisAlignedBox bx = n -> _getWorldAABB();


    //if the PagingLandScapeOctree is twice as big as the scene node,
    //we will add it to a child.
    if ( ( depth < mMaxDepth ) && octant -> _isTwiceSize( bx ) )
    {
        int x, y, z;
        octant -> _getChildIndexes( bx, x, y, z );

        if ( octant -> mChildren[ x ][ y ][ z ] == 0 )
        {
            octant -> mChildren[ x ][ y ][ z ] = new PagingLandScapeOctree( octant );

            const Vector3 *corners = octant -> mBox.getAllCorners();
            Vector3 min, max;

            if ( x == 0 )
            {
                min.x = corners[ 0 ].x;
                max.x = ( corners[ 0 ].x + corners[ 4 ].x ) * 0.5f;
            }

            else
            {
                min.x = ( corners[ 0 ].x + corners[ 4 ].x ) * 0.5f;
                max.x = corners[ 4 ].x;
            }

            if ( y == 0 )
            {
                min.y = corners[ 0 ].y;
                max.y = ( corners[ 0 ].y + corners[ 4 ].y ) * 0.5f;
            }

            else
            {
                min.y = ( corners[ 0 ].y + corners[ 4 ].y ) * 0.5f;
                max.y = corners[ 4 ].y;
            }

            if ( z == 0 )
            {
                min.z = corners[ 0 ].z;
                max.z = ( corners[ 0 ].z + corners[ 4 ].z ) * 0.5f;
            }

            else
            {
                min.z = ( corners[ 0 ].z + corners[ 4 ].z ) * 0.5f;
                max.z = corners[ 4 ].z;
            }

            octant -> mChildren[ x ][ y ][ z ] -> mBox.setExtents( min, max );
            octant -> mChildren[ x ][ y ][ z ] -> mHalfSize = ( max - min ) * 0.5f;
        }

        _addPagingLandScapeOctreeNode( n, octant -> mChildren[ x ][ y ][ z ], ++depth );

    }

    else
    {
        octant -> _addNode( n );
    }
}


SceneNode * PagingLandScapeOctreeSceneManager::createSceneNode( void )
{
    PagingLandScapeOctreeNode * on = new PagingLandScapeOctreeNode( this );
    mSceneNodes[ on->getName() ] = on;
    return on;
}

SceneNode * PagingLandScapeOctreeSceneManager::createSceneNode( const String &name )
{
    PagingLandScapeOctreeNode * on = new PagingLandScapeOctreeNode( this, name );
    mSceneNodes[ on->getName() ] = on;
    return on;
}

void PagingLandScapeOctreeSceneManager::_updateSceneGraph( Camera * cam )
{
    SceneManager::_updateSceneGraph( cam );
}

void PagingLandScapeOctreeSceneManager::_alertVisibleObjects( void )
{
//    NodeList::iterator it = mVisible.begin();
//
//    while ( it != mVisible.end() )
//    {
//        PagingLandScapeOctreeNode * node = *it;
//        node->alert ()??
//        ++it;
//    }
}

void PagingLandScapeOctreeSceneManager::_findVisibleObjects( Camera * cam, bool onlyShadowCasters )
{

    SceneManager::getRenderQueue()->clear();
    mBoxes.clear();
    mVisible.clear();

    if ( mCullCamera )
    {
        Camera * c = getCamera( "CullCamera" );

        if ( c != 0 )
            cam = getCamera( "CullCamera" );
    }

    mNumObjects = 0;

    //walk the PagingLandScapeOctree, adding all visible PagingLandScapeOctree nodes nodes to the render queue.
    walkPagingLandScapeOctree( static_cast < PagingLandScapeOctreeCamera * > ( cam ), getRenderQueue(), mPagingLandScapeOctree, 
                false, onlyShadowCasters );


    // Show the PagingLandScapeOctree boxes & cull camera if required
    if ( mShowBoxes || mCullCamera )
    {
        if ( mShowBoxes )
        {
            for ( BoxList::iterator it = mBoxes.begin(); it != mBoxes.end(); ++it )
            {
                SceneManager::getRenderQueue()->addRenderable(*it);
            }
        }

        if ( mCullCamera )
        {
            PagingLandScapeOctreeCamera * c = static_cast<PagingLandScapeOctreeCamera*>(SceneManager::getCamera( "CullCamera" ));

            if ( c != 0 )
            {
                SceneManager::getRenderQueue()->addRenderable(c);
            }
        }

    }



}

void PagingLandScapeOctreeSceneManager::walkPagingLandScapeOctree( PagingLandScapeOctreeCamera * camera, RenderQueue * const queue,
                                     PagingLandScapeOctree * const octant, const bool foundvisible, const bool onlyShadowCasters )
{
    assert (octant && mPagingLandScapeOctree);

    //return immediately if nothing is in the node.
    if ( octant -> numNodes() == 0 )
        return ;

    PagingLandScapeOctreeCamera::Visibility v = PagingLandScapeOctreeCamera::NONE;

    if ( foundvisible )
    {
        v = PagingLandScapeOctreeCamera::FULL;
    }
    else if ( octant == mPagingLandScapeOctree )
    {
        v = PagingLandScapeOctreeCamera::PARTIAL;
    }
    else
    {
        AxisAlignedBox box;
        octant -> _getCullBounds( &box );
        v = camera -> getVisibility( box );
    }


    // if the octant is visible, or if it's the root node...
    if ( v != PagingLandScapeOctreeCamera::NONE )
    {

        //Add stuff to be rendered;
        NodeList::iterator it = octant -> mNodes.begin();

        if ( mShowBoxes )
        {
            mBoxes.push_back( octant->getWireBoundingBox() );
        }

        bool vis = true;

        while ( it != octant -> mNodes.end() )
        {
            PagingLandScapeOctreeNode * const sn = *it;

            // if this PagingLandScapeOctree is partially visible, manually cull all
            // scene nodes attached directly to this level.

            if ( v == PagingLandScapeOctreeCamera::PARTIAL )
                vis = camera -> isVisible( sn -> _getWorldAABB() );

            if ( vis )
            {
                mNumObjects++;
                sn -> _addToRenderQueue(camera, queue, onlyShadowCasters );

                mVisible.push_back( sn );

                if ( mDisplayNodes )
                    queue -> addRenderable( sn );

                // check if the scene manager or this node wants the bounding box shown.
                if (sn->getShowBoundingBox() || mShowBoundingBoxes)
                    sn->_addBoundingBoxToQueue(queue);
            }

            ++it;
        } // while ( it != octant -> mNodes.end() )

        const bool isFullVis =  (v == PagingLandScapeOctreeCamera::FULL);
        if ( octant -> mChildren[ 0 ][ 0 ][ 0 ] != 0 )
            walkPagingLandScapeOctree( camera, queue, octant -> mChildren[ 0 ][ 0 ][ 0 ], 
                        isFullVis, onlyShadowCasters );

        if ( octant -> mChildren[ 1 ][ 0 ][ 0 ] != 0 )
            walkPagingLandScapeOctree( camera, queue, octant -> mChildren[ 1 ][ 0 ][ 0 ], 
                        isFullVis, onlyShadowCasters );

        if ( octant -> mChildren[ 0 ][ 1 ][ 0 ] != 0 )
            walkPagingLandScapeOctree( camera, queue, octant -> mChildren[ 0 ][ 1 ][ 0 ], 
                        isFullVis, onlyShadowCasters );

        if ( octant -> mChildren[ 1 ][ 1 ][ 0 ] != 0 )
            walkPagingLandScapeOctree( camera, queue, octant -> mChildren[ 1 ][ 1 ][ 0 ], 
                        isFullVis, onlyShadowCasters );

        if ( octant -> mChildren[ 0 ][ 0 ][ 1 ] != 0 )
            walkPagingLandScapeOctree( camera, queue, octant -> mChildren[ 0 ][ 0 ][ 1 ], 
                        isFullVis, onlyShadowCasters );

        if ( octant -> mChildren[ 1 ][ 0 ][ 1 ] != 0 )
            walkPagingLandScapeOctree( camera, queue, octant -> mChildren[ 1 ][ 0 ][ 1 ], 
                        isFullVis, onlyShadowCasters );

        if ( octant -> mChildren[ 0 ][ 1 ][ 1 ] != 0 )
            walkPagingLandScapeOctree( camera, queue, octant -> mChildren[ 0 ][ 1 ][ 1 ], 
                        isFullVis, onlyShadowCasters );

        if ( octant -> mChildren[ 1 ][ 1 ][ 1 ] != 0 )
            walkPagingLandScapeOctree( camera, queue, octant -> mChildren[ 1 ][ 1 ][ 1 ], 
                        isFullVis, onlyShadowCasters );

    }

}

// --- non template versions
static void _findNodes( const AxisAlignedBox &t, std::list < SceneNode * > &list, 
                const Ogre::SceneNode * const exclude, const bool full, PagingLandScapeOctree *octant )
{

    bool isFull = full;
	if ( !full )
	{
		AxisAlignedBox obox;
		octant -> _getCullBounds( &obox );

		const Intersection isect = intersect( t, obox );

		if ( isect == OUTSIDE )
			return ;

		isFull = ( isect == INSIDE );
	}


    const bool b_full = isFull;
	NodeList::iterator it = octant -> mNodes.begin();

	while ( it != octant -> mNodes.end() )
	{
		PagingLandScapeOctreeNode * const on = ( *it );

		if ( on != exclude )
		{
			if ( b_full )
			{
				list.push_back( on );
			}

			else
			{
				const Intersection nsect = intersect( t, on -> _getWorldAABB() );

				if ( nsect != OUTSIDE )
				{
					list.push_back( on );
				}
			}

		}

		++it;
	}



	if ( octant -> mChildren[ 0 ][ 0 ][ 0 ] != 0 )
		_findNodes( t, list, exclude, b_full, octant -> mChildren[ 0 ][ 0 ][ 0 ] );

	if ( octant -> mChildren[ 1 ][ 0 ][ 0 ] != 0 )
		_findNodes( t, list, exclude, b_full, octant -> mChildren[ 1 ][ 0 ][ 0 ] );

	if ( octant -> mChildren[ 0 ][ 1 ][ 0 ] != 0 )
		_findNodes( t, list, exclude, b_full, octant -> mChildren[ 0 ][ 1 ][ 0 ] );

	if ( octant -> mChildren[ 1 ][ 1 ][ 0 ] != 0 )
		_findNodes( t, list, exclude, b_full, octant -> mChildren[ 1 ][ 1 ][ 0 ] );

	if ( octant -> mChildren[ 0 ][ 0 ][ 1 ] != 0 )
		_findNodes( t, list, exclude, b_full, octant -> mChildren[ 0 ][ 0 ][ 1 ] );

	if ( octant -> mChildren[ 1 ][ 0 ][ 1 ] != 0 )
		_findNodes( t, list, exclude, b_full, octant -> mChildren[ 1 ][ 0 ][ 1 ] );

	if ( octant -> mChildren[ 0 ][ 1 ][ 1 ] != 0 )
		_findNodes( t, list, exclude, b_full, octant -> mChildren[ 0 ][ 1 ][ 1 ] );

	if ( octant -> mChildren[ 1 ][ 1 ][ 1 ] != 0 )
		_findNodes( t, list, exclude, b_full, octant -> mChildren[ 1 ][ 1 ][ 1 ] );

}

static void _findNodes( const Sphere &t, std::list < SceneNode * > &list, 
                const SceneNode * const exclude, 
                const bool full, PagingLandScapeOctree *octant )
{
    bool isFull = full;
	if ( !full )
	{
		AxisAlignedBox obox;
		octant -> _getCullBounds( &obox );

		const Intersection isect = intersect( t, obox );

		if ( isect == OUTSIDE )
			return ;

		isFull = ( isect == INSIDE );
	}

    const bool b_full = isFull;
	NodeList::iterator it = octant -> mNodes.begin();

	while ( it != octant -> mNodes.end() )
	{
		PagingLandScapeOctreeNode * const on = ( *it );

		if ( on != exclude )
		{
			if ( b_full )
			{
				list.push_back( on );
			}

			else
			{
				const Intersection nsect = intersect( t, on -> _getWorldAABB() );

				if ( nsect != OUTSIDE )
				{
					list.push_back( on );
				}
			}

		}

		++it;
	}



	if ( octant -> mChildren[ 0 ][ 0 ][ 0 ] != 0 )
		_findNodes( t, list, exclude, b_full, octant -> mChildren[ 0 ][ 0 ][ 0 ] );

	if ( octant -> mChildren[ 1 ][ 0 ][ 0 ] != 0 )
		_findNodes( t, list, exclude, b_full, octant -> mChildren[ 1 ][ 0 ][ 0 ] );

	if ( octant -> mChildren[ 0 ][ 1 ][ 0 ] != 0 )
		_findNodes( t, list, exclude, b_full, octant -> mChildren[ 0 ][ 1 ][ 0 ] );

	if ( octant -> mChildren[ 1 ][ 1 ][ 0 ] != 0 )
		_findNodes( t, list, exclude, b_full, octant -> mChildren[ 1 ][ 1 ][ 0 ] );

	if ( octant -> mChildren[ 0 ][ 0 ][ 1 ] != 0 )
		_findNodes( t, list, exclude, b_full, octant -> mChildren[ 0 ][ 0 ][ 1 ] );

	if ( octant -> mChildren[ 1 ][ 0 ][ 1 ] != 0 )
		_findNodes( t, list, exclude, b_full, octant -> mChildren[ 1 ][ 0 ][ 1 ] );

	if ( octant -> mChildren[ 0 ][ 1 ][ 1 ] != 0 )
		_findNodes( t, list, exclude, b_full, octant -> mChildren[ 0 ][ 1 ][ 1 ] );

	if ( octant -> mChildren[ 1 ][ 1 ][ 1 ] != 0 )
		_findNodes( t, list, exclude, b_full, octant -> mChildren[ 1 ][ 1 ][ 1 ] );

}


void _findNodes( const PlaneBoundedVolume &t, std::list < SceneNode * > &list, 
               const Ogre::SceneNode * const exclude, const bool full, PagingLandScapeOctree *octant )
{

    bool isFull = full;
	if ( !full )
	{
		AxisAlignedBox obox;
		octant -> _getCullBounds( &obox );

		const Intersection isect = intersect( t, obox );

		if ( isect == OUTSIDE )
			return ;

		isFull = ( isect == INSIDE );
	}


    const bool b_full = isFull;
	NodeList::iterator it = octant -> mNodes.begin();

	while ( it != octant -> mNodes.end() )
	{
		PagingLandScapeOctreeNode * const on = ( *it );

		if ( on != exclude )
		{
			if ( b_full )
			{
				list.push_back( on );
			}

			else
			{
				const Intersection nsect = intersect( t, on -> _getWorldAABB() );

				if ( nsect != OUTSIDE )
				{
					list.push_back( on );
				}
			}

		}

		++it;
	}



	if ( octant -> mChildren[ 0 ][ 0 ][ 0 ] != 0 )
		_findNodes( t, list, exclude, b_full, octant -> mChildren[ 0 ][ 0 ][ 0 ] );

	if ( octant -> mChildren[ 1 ][ 0 ][ 0 ] != 0 )
		_findNodes( t, list, exclude, b_full, octant -> mChildren[ 1 ][ 0 ][ 0 ] );

	if ( octant -> mChildren[ 0 ][ 1 ][ 0 ] != 0 )
		_findNodes( t, list, exclude, b_full, octant -> mChildren[ 0 ][ 1 ][ 0 ] );

	if ( octant -> mChildren[ 1 ][ 1 ][ 0 ] != 0 )
		_findNodes( t, list, exclude, b_full, octant -> mChildren[ 1 ][ 1 ][ 0 ] );

	if ( octant -> mChildren[ 0 ][ 0 ][ 1 ] != 0 )
		_findNodes( t, list, exclude, b_full, octant -> mChildren[ 0 ][ 0 ][ 1 ] );

	if ( octant -> mChildren[ 1 ][ 0 ][ 1 ] != 0 )
		_findNodes( t, list, exclude, b_full, octant -> mChildren[ 1 ][ 0 ][ 1 ] );

	if ( octant -> mChildren[ 0 ][ 1 ][ 1 ] != 0 )
		_findNodes( t, list, exclude, b_full, octant -> mChildren[ 0 ][ 1 ][ 1 ] );

	if ( octant -> mChildren[ 1 ][ 1 ][ 1 ] != 0 )
		_findNodes( t, list, exclude, b_full, octant -> mChildren[ 1 ][ 1 ][ 1 ] );

}

void _findNodes( const Ray &t, std::list < SceneNode * > &list, 
                const Ogre::SceneNode * const exclude, 
                const bool full, PagingLandScapeOctree *octant )
{

    bool isFull = full;
	if ( !full )
	{
		AxisAlignedBox obox;
		octant -> _getCullBounds( &obox );

		const Intersection isect = intersect( t, obox );

		if ( isect == OUTSIDE )
			return ;

		isFull = ( isect == INSIDE );
	}


    const bool b_full = isFull;
	NodeList::iterator it = octant -> mNodes.begin();

	while ( it != octant -> mNodes.end() )
	{
		PagingLandScapeOctreeNode * const on = ( *it );

		if ( on != exclude )
		{
			if ( b_full )
			{
				list.push_back( on );
			}

			else
			{
				const Intersection nsect = intersect( t, on -> _getWorldAABB() );

				if ( nsect != OUTSIDE )
				{
					list.push_back( on );
				}
			}

		}

		++it;
	}



	if ( octant -> mChildren[ 0 ][ 0 ][ 0 ] != 0 )
		_findNodes( t, list, exclude, b_full, octant -> mChildren[ 0 ][ 0 ][ 0 ] );

	if ( octant -> mChildren[ 1 ][ 0 ][ 0 ] != 0 )
		_findNodes( t, list, exclude, b_full, octant -> mChildren[ 1 ][ 0 ][ 0 ] );

	if ( octant -> mChildren[ 0 ][ 1 ][ 0 ] != 0 )
		_findNodes( t, list, exclude, b_full, octant -> mChildren[ 0 ][ 1 ][ 0 ] );

	if ( octant -> mChildren[ 1 ][ 1 ][ 0 ] != 0 )
		_findNodes( t, list, exclude, b_full, octant -> mChildren[ 1 ][ 1 ][ 0 ] );

	if ( octant -> mChildren[ 0 ][ 0 ][ 1 ] != 0 )
		_findNodes( t, list, exclude, b_full, octant -> mChildren[ 0 ][ 0 ][ 1 ] );

	if ( octant -> mChildren[ 1 ][ 0 ][ 1 ] != 0 )
		_findNodes( t, list, exclude, b_full, octant -> mChildren[ 1 ][ 0 ][ 1 ] );

	if ( octant -> mChildren[ 0 ][ 1 ][ 1 ] != 0 )
		_findNodes( t, list, exclude, b_full, octant -> mChildren[ 0 ][ 1 ][ 1 ] );

	if ( octant -> mChildren[ 1 ][ 1 ][ 1 ] != 0 )
		_findNodes( t, list, exclude, b_full, octant -> mChildren[ 1 ][ 1 ][ 1 ] );

}

void PagingLandScapeOctreeSceneManager::findNodesIn( const AxisAlignedBox &box, std::list < SceneNode * > &list, const SceneNode* const exclude )
{
    _findNodes( box, list, exclude, false, mPagingLandScapeOctree );
}

void PagingLandScapeOctreeSceneManager::findNodesIn( const Sphere &sphere, std::list < SceneNode * > &list, const SceneNode* const exclude )
{
    _findNodes( sphere, list, exclude, false, mPagingLandScapeOctree );
}

void PagingLandScapeOctreeSceneManager::findNodesIn( const PlaneBoundedVolume &volume, std::list < SceneNode * > &list, const SceneNode* const exclude )
{
    _findNodes( volume, list, exclude, false, mPagingLandScapeOctree );
}

void PagingLandScapeOctreeSceneManager::findNodesIn( const Ray &r, std::list < SceneNode * > &list, const SceneNode* const exclude )
{
    _findNodes( r, list, exclude, false, mPagingLandScapeOctree );
}

void PagingLandScapeOctreeSceneManager::resize( const AxisAlignedBox &box, const int depth )
{
    mMaxDepth = depth;
    resize( box );
}
void PagingLandScapeOctreeSceneManager::resize( const AxisAlignedBox &box )
{
    std::list < SceneNode * > nodes;

    _findNodes( mPagingLandScapeOctree->mBox, nodes, 0, true, mPagingLandScapeOctree );

    delete mPagingLandScapeOctree;

    mPagingLandScapeOctree = new PagingLandScapeOctree( 0 );
    mPagingLandScapeOctree->mBox = box;

    if (!nodes.empty())
    {
        std::list < SceneNode * > ::iterator it;
        it = nodes.begin();

        while ( it != nodes.end() )
        {
            PagingLandScapeOctreeNode * const on = static_cast < PagingLandScapeOctreeNode * > ( *it );
            on -> setOctant( 0 );
            _updatePagingLandScapeOctreeNode( on );
            ++it;
        } // while ( it != nodes.end() )
    }

}

bool PagingLandScapeOctreeSceneManager::setOption( const String & key, const void * val )
{
    if ( key == "Size" )
    {
        resize( * static_cast < const AxisAlignedBox * > ( val ) );
        return true;
    }

    else if ( key == "Depth" )
    {
        assert (mPagingLandScapeOctree);
        mMaxDepth = * static_cast < const int * > ( val );
        resize( mPagingLandScapeOctree->mBox );
        return true;
    }

    else if ( key == "ShowPagingLandScapeOctree" )
    {
        mShowBoxes = * static_cast < const bool * > ( val );
        return true;
    }

    else if ( key == "CullCamera" )
    {
        mCullCamera = * static_cast < const bool * > ( val );
        return true;
    }

    return SceneManager::setOption( key, val );


}

bool PagingLandScapeOctreeSceneManager::getOption( const String & key, void *val )
{
    if ( key == "Size" )
    {
        AxisAlignedBox * b = static_cast < AxisAlignedBox * > ( val );
        b -> setExtents( mPagingLandScapeOctree->mBox.getMinimum(), mPagingLandScapeOctree->mBox.getMaximum() );
        return true;
    }

    else if ( key == "Depth" )
    {
        * static_cast < int * > ( val ) = mMaxDepth;
        return true;
    }

    else if ( key == "ShowPagingLandScapeOctree" )
    {

        * static_cast < bool * > ( val ) = mShowBoxes;
        return true;
    }

    else if ( key == "CullCamera" )
    {
        * static_cast < bool * > ( val ) = mCullCamera;
        return true;
    }

    return SceneManager::getOption( key, val );

}

void PagingLandScapeOctreeSceneManager::clearScene(void)
{
    SceneManager::clearScene();
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
    DefaultIntersectionSceneQuery* q = new DefaultIntersectionSceneQuery(this);
    q->setQueryMask(mask);
    return q;
}


}
