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
octreescenemanager.cpp  -  description
-------------------
begin                : Fri Sep 27 2002
copyright            : (C) 2002 by Jon Anderson
email                : janders@users.sf.net
 
Enhancements 2003 - 2004 (C) The OGRE Team
 
***************************************************************************/

#include <OgreOctreeSceneManager.h>
#include <OgreOctreeSceneQuery.h>
#include <OgreOctreeNode.h>
#include <OgreOctreeCamera.h>
#include <OgreRenderSystem.h>
#include <OgreNoMemoryMacros.h>


extern "C"
{
    void findNodesInBox( Ogre::SceneManager *sm,
                         const Ogre::AxisAlignedBox &box,
                         std::list < Ogre::SceneNode * > &list,
                         Ogre::SceneNode *exclude )
    {
        static_cast<Ogre::OctreeSceneManager*>( sm ) -> findNodesIn( box, list, exclude );
    }
    void findNodesInSphere( Ogre::SceneManager *sm,
                            const Ogre::Sphere &sphere,
                            std::list < Ogre::SceneNode * > &list,
                            Ogre::SceneNode *exclude )
    {
        static_cast<Ogre::OctreeSceneManager*>( sm ) -> findNodesIn( sphere, list, exclude );
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
int OctreeSceneManager::intersect_call = 0;

Intersection intersect( const Ray &one, const AxisAlignedBox &two )
{
    OctreeSceneManager::intersect_call++;
    // Null box?
    if (two.isNull()) return OUTSIDE;

    bool inside = true;
    const Vector3* pCorners = two.getAllCorners();
    Vector3 origin = one.getOrigin();
    Vector3 dir = one.getDirection();

    Vector3 maxT(-1, -1, -1);

    int i = 0;
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
            float f = origin[i] + maxT[whichPlane] * dir[i];
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
    OctreeSceneManager::intersect_call++;
    // Null box?
    if (two.isNull()) return OUTSIDE;

    // Get corners of the box
    const Vector3* pCorners = two.getAllCorners();

    // For each plane, see if all points are on the negative side
    // If so, object is not visible.
    // If one or more are, it's partial.
    // If all aren't, full
    int corners[ 8 ] = {0, 4, 3, 5, 2, 6, 1, 7};
    bool all_inside = true;
    PlaneList::const_iterator i, iend;
    iend = one.planes.end();
    for (i = one.planes.begin(); i != iend; ++i)
    {
        const Plane& plane = *i;
        bool all_outside = true;

        float distance = 0;

        for ( int corner = 0; corner < 8; ++corner )
        {
            distance = plane.getDistance( pCorners[ corners[ corner ] ] );
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
    OctreeSceneManager::intersect_call++;
    // Null box?
    if (one.isNull() || two.isNull()) return OUTSIDE;

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

    bool full = ( inside[ 0 ].x > outside[ 0 ].x &&
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
    OctreeSceneManager::intersect_call++;
    // Null box?
    if (two.isNull()) return OUTSIDE;

    float sradius = one.getRadius();

    sradius *= sradius;

    Vector3 scenter = one.getCenter();

    const Vector3 *corners = two.getAllCorners();

    float s, d = 0;

    Vector3 mndistance = ( corners[ 0 ] - scenter );
    Vector3 mxdistance = ( corners[ 4 ] - scenter );

    if ( mndistance.squaredLength() < sradius &&
            mxdistance.squaredLength() < sradius )
    {
        return INSIDE;
    }

    //find the square of the distance
    //from the sphere to the box
    for ( int i = 0 ; i < 3 ; i++ )
    {
        if ( scenter[ i ] < corners[ 0 ][ i ] )
        {
            s = scenter[ i ] - corners[ 0 ][ i ];
            d += s * s;
        }

        else if ( scenter[ i ] > corners[ 4 ][ i ] )
        {
            s = scenter[ i ] - corners[ 4 ][ i ];
            d += s * s;
        }

    }

    bool partial = ( d <= sradius );

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

unsigned short OctreeSceneManager::mIndexes[ 24 ] = {0, 1, 1, 2, 2, 3, 3, 0,       //back
        0, 6, 6, 5, 5, 1,             //left
        3, 7, 7, 4, 4, 2,             //right
        6, 7, 5, 4 };          //front
unsigned long OctreeSceneManager::mColors[ 8 ] = {white, white, white, white, white, white, white, white };


OctreeSceneManager::OctreeSceneManager( ) : SceneManager()
{
    AxisAlignedBox b( -10000, -10000, -10000, 10000, 10000, 10000 );
    int depth = 8; 
    mOctree = 0;
    init( b, depth );
}

OctreeSceneManager::OctreeSceneManager( AxisAlignedBox &box, int max_depth ) : SceneManager()
{
    mOctree = 0;
    init( box, max_depth );
}

void OctreeSceneManager::init( AxisAlignedBox &box, int depth )
{
    delete mSceneRoot; //get rid of old root.

    // -- Changes by Steve
    // Don't do it this way, it will add it to the mSceneNodes which we don't want
    //mSceneRoot = createSceneNode( "SceneRoot" );
    mSceneRoot = new OctreeNode( this, "SceneRoot" );
	mSceneRoot->_notifyRootNode();
    // -- End changes by Steve

    if ( mOctree != 0 )
        delete mOctree;

    mOctree = new Octree( 0 );

    mMaxDepth = depth;
    mBox = box;

    mOctree -> mBox = box;

    Vector3 min = box.getMinimum();

    Vector3 max = box.getMaximum();

    mOctree -> mHalfSize = ( max - min ) / 2;


    mShowBoxes = false;

    mCullCamera = false;

    mNumObjects = 0;

    Vector3 v( 1.5, 1.5, 1.5 );

    mScaleFactor.setScale( v );



    // setDisplaySceneNodes( true );
    // setShowBoxes( true );

    //
    //setUseCullCamera( true );
    //mSceneRoot isn't put into the octree since it has no volume.

}

OctreeSceneManager::~OctreeSceneManager()
{
    // -- Changed by Steve
    // Don't do this here, SceneManager will do it
    /*
    if( mSceneRoot )
    delete mSceneRoot;
    */ 
    // --End Changes by Steve

    if ( mOctree )
        delete mOctree;
}

Camera * OctreeSceneManager::createCamera( const String &name )
{
    Camera * c = new OctreeCamera( name, this );
    mCameras.insert( CameraList::value_type( name, c ) );
    return c;
}

void OctreeSceneManager::destroySceneNode( const String &name )
{
    OctreeNode * on = static_cast < OctreeNode* > ( getSceneNode( name ) );

    if ( on != 0 )
        _removeOctreeNode( on );

    SceneManager::destroySceneNode( name );
}

bool OctreeSceneManager::getOptionValues( const String & key, StringVector  &refValueList )
{
    return SceneManager::getOptionValues( key, refValueList );
}

bool OctreeSceneManager::getOptionKeys( StringVector & refKeys )
{
    SceneManager::getOptionKeys( refKeys );
    refKeys.push_back( "CullCamera" );
    refKeys.push_back( "Size" );
    refKeys.push_back( "ShowOctree" );
    refKeys.push_back( "Depth" );

    return true;
}


void OctreeSceneManager::_updateOctreeNode( OctreeNode * onode )
{
    AxisAlignedBox box = onode -> _getWorldAABB();

    if ( box.isNull() )
        return ;


    if ( onode -> getOctant() == 0 )
    {
        //if outside the octree, force into the root node.
        if ( ! onode -> _isIn( mOctree -> mBox ) )
            mOctree->_addNode( onode );
        else
            _addOctreeNode( onode, mOctree );
        return ;
    }

    if ( ! onode -> _isIn( onode -> getOctant() -> mBox ) )
    {
        _removeOctreeNode( onode );

        //if outside the octree, force into the root node.
        if ( ! onode -> _isIn( mOctree -> mBox ) )
            mOctree->_addNode( onode );
        else
            _addOctreeNode( onode, mOctree );
    }
}

/** Only removes the node from the octree.  It leaves the octree, even if it's empty.
*/
void OctreeSceneManager::_removeOctreeNode( OctreeNode * n )
{
    Octree * oct = n -> getOctant();

    if ( oct )
    {
        oct -> _removeNode( n );
    }

    n->setOctant(0);
}


void OctreeSceneManager::_addOctreeNode( OctreeNode * n, Octree *octant, int depth )
{

    AxisAlignedBox bx = n -> _getWorldAABB();


    //if the octree is twice as big as the scene node,
    //we will add it to a child.
    if ( ( depth < mMaxDepth ) && octant -> _isTwiceSize( bx ) )
    {
        int x, y, z;
        octant -> _getChildIndexes( bx, &x, &y, &z );

        if ( octant -> mChildren[ x ][ y ][ z ] == 0 )
        {
            octant -> mChildren[ x ][ y ][ z ] = new Octree( octant );

            const Vector3 *corners = octant -> mBox.getAllCorners();
            Vector3 min, max;

            if ( x == 0 )
            {
                min.x = corners[ 0 ].x;
                max.x = ( corners[ 0 ].x + corners[ 4 ].x ) / 2;
            }

            else
            {
                min.x = ( corners[ 0 ].x + corners[ 4 ].x ) / 2;
                max.x = corners[ 4 ].x;
            }

            if ( y == 0 )
            {
                min.y = corners[ 0 ].y;
                max.y = ( corners[ 0 ].y + corners[ 4 ].y ) / 2;
            }

            else
            {
                min.y = ( corners[ 0 ].y + corners[ 4 ].y ) / 2;
                max.y = corners[ 4 ].y;
            }

            if ( z == 0 )
            {
                min.z = corners[ 0 ].z;
                max.z = ( corners[ 0 ].z + corners[ 4 ].z ) / 2;
            }

            else
            {
                min.z = ( corners[ 0 ].z + corners[ 4 ].z ) / 2;
                max.z = corners[ 4 ].z;
            }

            octant -> mChildren[ x ][ y ][ z ] -> mBox.setExtents( min, max );
            octant -> mChildren[ x ][ y ][ z ] -> mHalfSize = ( max - min ) / 2;
        }

        _addOctreeNode( n, octant -> mChildren[ x ][ y ][ z ], ++depth );

    }

    else
    {
        octant -> _addNode( n );
    }
}


SceneNode * OctreeSceneManager::createSceneNode( void )
{
    OctreeNode * on = new OctreeNode( this );
    mSceneNodes[ on->getName() ] = on;
    return on;
}

SceneNode * OctreeSceneManager::createSceneNode( const String &name )
{
    OctreeNode * on = new OctreeNode( this, name );
    mSceneNodes[ on->getName() ] = on;
    return on;
}

void OctreeSceneManager::_updateSceneGraph( Camera * cam )
{
    SceneManager::_updateSceneGraph( cam );
}

void OctreeSceneManager::_alertVisibleObjects( void )
{
    NodeList::iterator it = mVisible.begin();

    while ( it != mVisible.end() )
    {
        OctreeNode * node = *it;

        ++it;
    }
}

void OctreeSceneManager::_findVisibleObjects( Camera * cam, bool onlyShadowCasters )
{

    getRenderQueue()->clear();
    mBoxes.clear();
    mVisible.clear();

    if ( mCullCamera )
    {
        Camera * c = getCamera( "CullCamera" );

        if ( c != 0 )
            cam = getCamera( "CullCamera" );
    }

    mNumObjects = 0;

    //walk the octree, adding all visible Octreenodes nodes to the render queue.
    walkOctree( static_cast < OctreeCamera * > ( cam ), getRenderQueue(), mOctree, false, onlyShadowCasters );


    // Show the octree boxes & cull camera if required
    if ( mShowBoxes || mCullCamera )
    {



        if ( mShowBoxes )
        {
            for ( BoxList::iterator it = mBoxes.begin(); it != mBoxes.end(); ++it )
            {
                getRenderQueue()->addRenderable(*it);
            }
        }

        if ( mCullCamera )
        {
            OctreeCamera * c = static_cast<OctreeCamera*>(getCamera( "CullCamera" ));

            if ( c != 0 )
            {
                getRenderQueue()->addRenderable(c);
            }
        }

    }



}

void OctreeSceneManager::walkOctree( OctreeCamera *camera, RenderQueue *queue,
                                     Octree *octant, bool foundvisible, bool onlyShadowCasters )
{

    //return immediately if nothing is in the node.
    if ( octant -> numNodes() == 0 )
        return ;

    OctreeCamera::Visibility v = OctreeCamera::NONE;

    if ( foundvisible )
    {
        v = OctreeCamera::FULL;
    }

    else if ( octant == mOctree )
    {
        v = OctreeCamera::PARTIAL;
    }

    else
    {
        AxisAlignedBox box;
        octant -> _getCullBounds( &box );
        v = camera -> getVisibility( box );
    }


    // if the octant is visible, or if it's the root node...
    if ( v != OctreeCamera::NONE )
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
            OctreeNode * sn = *it;

            // if this octree is partially visible, manually cull all
            // scene nodes attached directly to this level.

            if ( v == OctreeCamera::PARTIAL )
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
        }

        if ( octant -> mChildren[ 0 ][ 0 ][ 0 ] != 0 )
            walkOctree( camera, queue, octant -> mChildren[ 0 ][ 0 ][ 0 ], ( v == OctreeCamera::FULL ), onlyShadowCasters );

        if ( octant -> mChildren[ 1 ][ 0 ][ 0 ] != 0 )
            walkOctree( camera, queue, octant -> mChildren[ 1 ][ 0 ][ 0 ], ( v == OctreeCamera::FULL ), onlyShadowCasters );

        if ( octant -> mChildren[ 0 ][ 1 ][ 0 ] != 0 )
            walkOctree( camera, queue, octant -> mChildren[ 0 ][ 1 ][ 0 ], ( v == OctreeCamera::FULL ), onlyShadowCasters );

        if ( octant -> mChildren[ 1 ][ 1 ][ 0 ] != 0 )
            walkOctree( camera, queue, octant -> mChildren[ 1 ][ 1 ][ 0 ], ( v == OctreeCamera::FULL ), onlyShadowCasters );

        if ( octant -> mChildren[ 0 ][ 0 ][ 1 ] != 0 )
            walkOctree( camera, queue, octant -> mChildren[ 0 ][ 0 ][ 1 ], ( v == OctreeCamera::FULL ), onlyShadowCasters );

        if ( octant -> mChildren[ 1 ][ 0 ][ 1 ] != 0 )
            walkOctree( camera, queue, octant -> mChildren[ 1 ][ 0 ][ 1 ], ( v == OctreeCamera::FULL ), onlyShadowCasters );

        if ( octant -> mChildren[ 0 ][ 1 ][ 1 ] != 0 )
            walkOctree( camera, queue, octant -> mChildren[ 0 ][ 1 ][ 1 ], ( v == OctreeCamera::FULL ), onlyShadowCasters );

        if ( octant -> mChildren[ 1 ][ 1 ][ 1 ] != 0 )
            walkOctree( camera, queue, octant -> mChildren[ 1 ][ 1 ][ 1 ], ( v == OctreeCamera::FULL ), onlyShadowCasters );

    }

}

// --- non template versions
void _findNodes( const AxisAlignedBox &t, std::list < SceneNode * > &list, SceneNode *exclude, bool full, Octree *octant )
{

	if ( !full )
	{
		AxisAlignedBox obox;
		octant -> _getCullBounds( &obox );

		Intersection isect = intersect( t, obox );

		if ( isect == OUTSIDE )
			return ;

		full = ( isect == INSIDE );
	}


	NodeList::iterator it = octant -> mNodes.begin();

	while ( it != octant -> mNodes.end() )
	{
		OctreeNode * on = ( *it );

		if ( on != exclude )
		{
			if ( full )
			{
				list.push_back( on );
			}

			else
			{
				Intersection nsect = intersect( t, on -> _getWorldAABB() );

				if ( nsect != OUTSIDE )
				{
					list.push_back( on );
				}
			}

		}

		++it;
	}



	if ( octant -> mChildren[ 0 ][ 0 ][ 0 ] != 0 )
		_findNodes( t, list, exclude, full, octant -> mChildren[ 0 ][ 0 ][ 0 ] );

	if ( octant -> mChildren[ 1 ][ 0 ][ 0 ] != 0 )
		_findNodes( t, list, exclude, full, octant -> mChildren[ 1 ][ 0 ][ 0 ] );

	if ( octant -> mChildren[ 0 ][ 1 ][ 0 ] != 0 )
		_findNodes( t, list, exclude, full, octant -> mChildren[ 0 ][ 1 ][ 0 ] );

	if ( octant -> mChildren[ 1 ][ 1 ][ 0 ] != 0 )
		_findNodes( t, list, exclude, full, octant -> mChildren[ 1 ][ 1 ][ 0 ] );

	if ( octant -> mChildren[ 0 ][ 0 ][ 1 ] != 0 )
		_findNodes( t, list, exclude, full, octant -> mChildren[ 0 ][ 0 ][ 1 ] );

	if ( octant -> mChildren[ 1 ][ 0 ][ 1 ] != 0 )
		_findNodes( t, list, exclude, full, octant -> mChildren[ 1 ][ 0 ][ 1 ] );

	if ( octant -> mChildren[ 0 ][ 1 ][ 1 ] != 0 )
		_findNodes( t, list, exclude, full, octant -> mChildren[ 0 ][ 1 ][ 1 ] );

	if ( octant -> mChildren[ 1 ][ 1 ][ 1 ] != 0 )
		_findNodes( t, list, exclude, full, octant -> mChildren[ 1 ][ 1 ][ 1 ] );

}

void _findNodes( const Sphere &t, std::list < SceneNode * > &list, SceneNode *exclude, bool full, Octree *octant )
{

	if ( !full )
	{
		AxisAlignedBox obox;
		octant -> _getCullBounds( &obox );

		Intersection isect = intersect( t, obox );

		if ( isect == OUTSIDE )
			return ;

		full = ( isect == INSIDE );
	}


	NodeList::iterator it = octant -> mNodes.begin();

	while ( it != octant -> mNodes.end() )
	{
		OctreeNode * on = ( *it );

		if ( on != exclude )
		{
			if ( full )
			{
				list.push_back( on );
			}

			else
			{
				Intersection nsect = intersect( t, on -> _getWorldAABB() );

				if ( nsect != OUTSIDE )
				{
					list.push_back( on );
				}
			}

		}

		++it;
	}



	if ( octant -> mChildren[ 0 ][ 0 ][ 0 ] != 0 )
		_findNodes( t, list, exclude, full, octant -> mChildren[ 0 ][ 0 ][ 0 ] );

	if ( octant -> mChildren[ 1 ][ 0 ][ 0 ] != 0 )
		_findNodes( t, list, exclude, full, octant -> mChildren[ 1 ][ 0 ][ 0 ] );

	if ( octant -> mChildren[ 0 ][ 1 ][ 0 ] != 0 )
		_findNodes( t, list, exclude, full, octant -> mChildren[ 0 ][ 1 ][ 0 ] );

	if ( octant -> mChildren[ 1 ][ 1 ][ 0 ] != 0 )
		_findNodes( t, list, exclude, full, octant -> mChildren[ 1 ][ 1 ][ 0 ] );

	if ( octant -> mChildren[ 0 ][ 0 ][ 1 ] != 0 )
		_findNodes( t, list, exclude, full, octant -> mChildren[ 0 ][ 0 ][ 1 ] );

	if ( octant -> mChildren[ 1 ][ 0 ][ 1 ] != 0 )
		_findNodes( t, list, exclude, full, octant -> mChildren[ 1 ][ 0 ][ 1 ] );

	if ( octant -> mChildren[ 0 ][ 1 ][ 1 ] != 0 )
		_findNodes( t, list, exclude, full, octant -> mChildren[ 0 ][ 1 ][ 1 ] );

	if ( octant -> mChildren[ 1 ][ 1 ][ 1 ] != 0 )
		_findNodes( t, list, exclude, full, octant -> mChildren[ 1 ][ 1 ][ 1 ] );

}


void _findNodes( const PlaneBoundedVolume &t, std::list < SceneNode * > &list, SceneNode *exclude, bool full, Octree *octant )
{

	if ( !full )
	{
		AxisAlignedBox obox;
		octant -> _getCullBounds( &obox );

		Intersection isect = intersect( t, obox );

		if ( isect == OUTSIDE )
			return ;

		full = ( isect == INSIDE );
	}


	NodeList::iterator it = octant -> mNodes.begin();

	while ( it != octant -> mNodes.end() )
	{
		OctreeNode * on = ( *it );

		if ( on != exclude )
		{
			if ( full )
			{
				list.push_back( on );
			}

			else
			{
				Intersection nsect = intersect( t, on -> _getWorldAABB() );

				if ( nsect != OUTSIDE )
				{
					list.push_back( on );
				}
			}

		}

		++it;
	}



	if ( octant -> mChildren[ 0 ][ 0 ][ 0 ] != 0 )
		_findNodes( t, list, exclude, full, octant -> mChildren[ 0 ][ 0 ][ 0 ] );

	if ( octant -> mChildren[ 1 ][ 0 ][ 0 ] != 0 )
		_findNodes( t, list, exclude, full, octant -> mChildren[ 1 ][ 0 ][ 0 ] );

	if ( octant -> mChildren[ 0 ][ 1 ][ 0 ] != 0 )
		_findNodes( t, list, exclude, full, octant -> mChildren[ 0 ][ 1 ][ 0 ] );

	if ( octant -> mChildren[ 1 ][ 1 ][ 0 ] != 0 )
		_findNodes( t, list, exclude, full, octant -> mChildren[ 1 ][ 1 ][ 0 ] );

	if ( octant -> mChildren[ 0 ][ 0 ][ 1 ] != 0 )
		_findNodes( t, list, exclude, full, octant -> mChildren[ 0 ][ 0 ][ 1 ] );

	if ( octant -> mChildren[ 1 ][ 0 ][ 1 ] != 0 )
		_findNodes( t, list, exclude, full, octant -> mChildren[ 1 ][ 0 ][ 1 ] );

	if ( octant -> mChildren[ 0 ][ 1 ][ 1 ] != 0 )
		_findNodes( t, list, exclude, full, octant -> mChildren[ 0 ][ 1 ][ 1 ] );

	if ( octant -> mChildren[ 1 ][ 1 ][ 1 ] != 0 )
		_findNodes( t, list, exclude, full, octant -> mChildren[ 1 ][ 1 ][ 1 ] );

}

void _findNodes( const Ray &t, std::list < SceneNode * > &list, SceneNode *exclude, bool full, Octree *octant )
{

	if ( !full )
	{
		AxisAlignedBox obox;
		octant -> _getCullBounds( &obox );

		Intersection isect = intersect( t, obox );

		if ( isect == OUTSIDE )
			return ;

		full = ( isect == INSIDE );
	}


	NodeList::iterator it = octant -> mNodes.begin();

	while ( it != octant -> mNodes.end() )
	{
		OctreeNode * on = ( *it );

		if ( on != exclude )
		{
			if ( full )
			{
				list.push_back( on );
			}

			else
			{
				Intersection nsect = intersect( t, on -> _getWorldAABB() );

				if ( nsect != OUTSIDE )
				{
					list.push_back( on );
				}
			}

		}

		++it;
	}



	if ( octant -> mChildren[ 0 ][ 0 ][ 0 ] != 0 )
		_findNodes( t, list, exclude, full, octant -> mChildren[ 0 ][ 0 ][ 0 ] );

	if ( octant -> mChildren[ 1 ][ 0 ][ 0 ] != 0 )
		_findNodes( t, list, exclude, full, octant -> mChildren[ 1 ][ 0 ][ 0 ] );

	if ( octant -> mChildren[ 0 ][ 1 ][ 0 ] != 0 )
		_findNodes( t, list, exclude, full, octant -> mChildren[ 0 ][ 1 ][ 0 ] );

	if ( octant -> mChildren[ 1 ][ 1 ][ 0 ] != 0 )
		_findNodes( t, list, exclude, full, octant -> mChildren[ 1 ][ 1 ][ 0 ] );

	if ( octant -> mChildren[ 0 ][ 0 ][ 1 ] != 0 )
		_findNodes( t, list, exclude, full, octant -> mChildren[ 0 ][ 0 ][ 1 ] );

	if ( octant -> mChildren[ 1 ][ 0 ][ 1 ] != 0 )
		_findNodes( t, list, exclude, full, octant -> mChildren[ 1 ][ 0 ][ 1 ] );

	if ( octant -> mChildren[ 0 ][ 1 ][ 1 ] != 0 )
		_findNodes( t, list, exclude, full, octant -> mChildren[ 0 ][ 1 ][ 1 ] );

	if ( octant -> mChildren[ 1 ][ 1 ][ 1 ] != 0 )
		_findNodes( t, list, exclude, full, octant -> mChildren[ 1 ][ 1 ][ 1 ] );

}

void OctreeSceneManager::findNodesIn( const AxisAlignedBox &box, std::list < SceneNode * > &list, SceneNode *exclude )
{
    _findNodes( box, list, exclude, false, mOctree );
}

void OctreeSceneManager::findNodesIn( const Sphere &sphere, std::list < SceneNode * > &list, SceneNode *exclude )
{
    _findNodes( sphere, list, exclude, false, mOctree );
}

void OctreeSceneManager::findNodesIn( const PlaneBoundedVolume &volume, std::list < SceneNode * > &list, SceneNode *exclude )
{
    _findNodes( volume, list, exclude, false, mOctree );
}

void OctreeSceneManager::findNodesIn( const Ray &r, std::list < SceneNode * > &list, SceneNode *exclude )
{
    _findNodes( r, list, exclude, false, mOctree );
}

void OctreeSceneManager::resize( const AxisAlignedBox &box )
{
    std::list < SceneNode * > nodes;
    std::list < SceneNode * > ::iterator it;

    _findNodes( mOctree->mBox, nodes, 0, true, mOctree );

    delete mOctree;

    mOctree = new Octree( 0 );
    mOctree->mBox = box;

    it = nodes.begin();

    while ( it != nodes.end() )
    {
        OctreeNode * on = static_cast < OctreeNode * > ( *it );
        on -> setOctant( 0 );
        _updateOctreeNode( on );
        ++it;
    }

}

bool OctreeSceneManager::setOption( const String & key, const void * val )
{
    if ( key == "Size" )
    {
        resize( * static_cast < const AxisAlignedBox * > ( val ) );
        return true;
    }

    else if ( key == "Depth" )
    {
        mMaxDepth = * static_cast < const int * > ( val );
        resize( mOctree->mBox );
        return true;
    }

    else if ( key == "ShowOctree" )
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

bool OctreeSceneManager::getOption( const String & key, void *val )
{
    if ( key == "Size" )
    {
        AxisAlignedBox * b = static_cast < AxisAlignedBox * > ( val );
        b -> setExtents( mOctree->mBox.getMinimum(), mOctree->mBox.getMaximum() );
        return true;
    }

    else if ( key == "Depth" )
    {
        * static_cast < int * > ( val ) = mMaxDepth;
        return true;
    }

    else if ( key == "ShowOctree" )
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

void OctreeSceneManager::clearScene(void)
{
    SceneManager::clearScene();
    init(mBox, mMaxDepth);

}

//---------------------------------------------------------------------
AxisAlignedBoxSceneQuery*
OctreeSceneManager::createAABBQuery(const AxisAlignedBox& box, unsigned long mask)
{
    OctreeAxisAlignedBoxSceneQuery* q = new OctreeAxisAlignedBoxSceneQuery(this);
    q->setBox(box);
    q->setQueryMask(mask);
    return q;
}
//---------------------------------------------------------------------
SphereSceneQuery*
OctreeSceneManager::createSphereQuery(const Sphere& sphere, unsigned long mask)
{
    OctreeSphereSceneQuery* q = new OctreeSphereSceneQuery(this);
    q->setSphere(sphere);
    q->setQueryMask(mask);
    return q;
}
//---------------------------------------------------------------------
PlaneBoundedVolumeListSceneQuery*
OctreeSceneManager::createPlaneBoundedVolumeQuery(const PlaneBoundedVolumeList& volumes,
        unsigned long mask)
{
    OctreePlaneBoundedVolumeListSceneQuery* q = new OctreePlaneBoundedVolumeListSceneQuery(this);
    q->setVolumes(volumes);
    q->setQueryMask(mask);
    return q;
}

//---------------------------------------------------------------------
RaySceneQuery*
OctreeSceneManager::createRayQuery(const Ray& ray, unsigned long mask)
{
    OctreeRaySceneQuery* q = new OctreeRaySceneQuery(this);
    q->setRay(ray);
    q->setQueryMask(mask);
    return q;
}
//---------------------------------------------------------------------
IntersectionSceneQuery*
OctreeSceneManager::createIntersectionQuery(unsigned long mask)
{

    // Octree implementation performs WORSE for < 500 objects
    // TODO: optimise it so it's better in all cases
    //OctreeIntersectionSceneQuery* q = new OctreeIntersectionSceneQuery(this);
    DefaultIntersectionSceneQuery* q = new DefaultIntersectionSceneQuery(this);
    q->setQueryMask(mask);
    return q;
}


}
