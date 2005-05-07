/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/
 
Copyright  2000-2005 The OGRE Team
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
OgreOctreeSceneQuery.cpp  -  description
-------------------
begin                : Tues July 20, 2004
copyright            : (C) 2004by Jon Anderson
email                : janders@users.sf.net
 
 
 
***************************************************************************/

#include <OgreOctreeSceneQuery.h>
#include <OgreOctreeSceneManager.h>
#include <OgreEntity.h>

namespace Ogre
{

//---------------------------------------------------------------------
OctreeIntersectionSceneQuery::OctreeIntersectionSceneQuery(SceneManager* creator)
        : DefaultIntersectionSceneQuery(creator)
{

}
//---------------------------------------------------------------------
OctreeIntersectionSceneQuery::~OctreeIntersectionSceneQuery()
{}
//---------------------------------------------------------------------
void OctreeIntersectionSceneQuery::execute(IntersectionSceneQueryListener* listener)
{
    typedef std::pair<MovableObject *, MovableObject *> MovablePair;
    typedef std::set
        < std::pair<MovableObject *, MovableObject *> > MovableSet;

    MovableSet set;

    SceneManager::EntityIterator it = mParentSceneMgr->getEntityIterator();
    while( it.hasMoreElements() )
    {

        Entity * e = it.getNext();

        std::list < SceneNode * > list;
        //find the nodes that intersect the AAB
        static_cast<OctreeSceneManager*>( mParentSceneMgr ) -> findNodesIn( e->getWorldBoundingBox(), list, 0 );
        //grab all moveables from the node that intersect...
        std::list < SceneNode * >::iterator it = list.begin();
        while( it != list.end() )
        {
            SceneNode::ObjectIterator oit = (*it) -> getAttachedObjectIterator();
            while( oit.hasMoreElements() )
            {
                MovableObject * m = oit.getNext();

                if( m != e &&
                        set.find( MovablePair(e,m)) == set.end() &&
                        set.find( MovablePair(m,e)) == set.end() &&
                        (m->getQueryFlags() & mQueryMask) &&
						m->isInScene() && 
						e->getWorldBoundingBox().intersects( m->getWorldBoundingBox() ) )
                {
                    listener -> queryResult( e, m );
                }
                set.insert( MovablePair(e,m) );

            }
            ++it;
        }

    }
}
/** Creates a custom Octree AAB query */
OctreeAxisAlignedBoxSceneQuery::OctreeAxisAlignedBoxSceneQuery(SceneManager* creator)
        : DefaultAxisAlignedBoxSceneQuery(creator)
{
}
/** Deletes the custom Octree query */
OctreeAxisAlignedBoxSceneQuery::~OctreeAxisAlignedBoxSceneQuery()
{}

/** Finds any entities that intersect the AAB for the query. */
void OctreeAxisAlignedBoxSceneQuery::execute(SceneQueryListener* listener)
{
    std::list < SceneNode * > list;
    //find the nodes that intersect the AAB
    static_cast<OctreeSceneManager*>( mParentSceneMgr ) -> findNodesIn( mAABB, list, 0 );

    //grab all moveables from the node that intersect...
    std::list < SceneNode * >::iterator it = list.begin();
    while( it != list.end() )
    {
        SceneNode::ObjectIterator oit = (*it) -> getAttachedObjectIterator();
        while( oit.hasMoreElements() )
        {
            MovableObject * m = oit.getNext();
            if( (m->getQueryFlags() & mQueryMask) && 
				m->isInScene() &&
				mAABB.intersects( m->getWorldBoundingBox() ) )
            {
                listener -> queryResult( m );
            }
        }

        ++it;
    }

}
//---------------------------------------------------------------------
OctreeRaySceneQuery::
OctreeRaySceneQuery(SceneManager* creator) : DefaultRaySceneQuery(creator)
{
}
//---------------------------------------------------------------------
OctreeRaySceneQuery::~OctreeRaySceneQuery()
{}
//---------------------------------------------------------------------
void OctreeRaySceneQuery::execute(RaySceneQueryListener* listener)
{
    std::list < SceneNode * > list;
    //find the nodes that intersect the AAB
    static_cast<OctreeSceneManager*>( mParentSceneMgr ) -> findNodesIn( mRay, list, 0 );

    //grab all moveables from the node that intersect...
    std::list < SceneNode * >::iterator it = list.begin();
    while( it != list.end() )
    {
        SceneNode::ObjectIterator oit = (*it) -> getAttachedObjectIterator();
        while( oit.hasMoreElements() )
        {
            MovableObject * m = oit.getNext();
            if( (m->getQueryFlags() & mQueryMask) && m->isInScene() )
            {
                std::pair<bool, Real> result = mRay.intersects(m->getWorldBoundingBox());

                if( result.first )
                {
                    listener -> queryResult( m, result.second );
                }
            }
        }

        ++it;
    }

}


//---------------------------------------------------------------------
OctreeSphereSceneQuery::
OctreeSphereSceneQuery(SceneManager* creator) : DefaultSphereSceneQuery(creator)
{
}
//---------------------------------------------------------------------
OctreeSphereSceneQuery::~OctreeSphereSceneQuery()
{}
//---------------------------------------------------------------------
void OctreeSphereSceneQuery::execute(SceneQueryListener* listener)
{
    std::list < SceneNode * > list;
    //find the nodes that intersect the AAB
    static_cast<OctreeSceneManager*>( mParentSceneMgr ) -> findNodesIn( mSphere, list, 0 );

    //grab all moveables from the node that intersect...
    std::list < SceneNode * >::iterator it = list.begin();
    while( it != list.end() )
    {
        SceneNode::ObjectIterator oit = (*it) -> getAttachedObjectIterator();
        while( oit.hasMoreElements() )
        {
            MovableObject * m = oit.getNext();
            if( (m->getQueryFlags() & mQueryMask) && 
				m->isInScene() && 
				mSphere.intersects( m->getWorldBoundingBox() ) )
            {
                listener -> queryResult( m );
            }
        }

        ++it;
    }
}
//---------------------------------------------------------------------
OctreePlaneBoundedVolumeListSceneQuery::
OctreePlaneBoundedVolumeListSceneQuery(SceneManager* creator)
        : DefaultPlaneBoundedVolumeListSceneQuery(creator)
{

}
//---------------------------------------------------------------------
OctreePlaneBoundedVolumeListSceneQuery::~OctreePlaneBoundedVolumeListSceneQuery()
{}
//---------------------------------------------------------------------
void OctreePlaneBoundedVolumeListSceneQuery::execute(SceneQueryListener* listener)
{
    PlaneBoundedVolumeList::iterator pi, piend;
    piend = mVolumes.end();
    for (pi = mVolumes.begin(); pi != piend; ++pi)
    {
        std::list < SceneNode * > list;
        //find the nodes that intersect the AAB
        static_cast<OctreeSceneManager*>( mParentSceneMgr ) -> findNodesIn( *pi, list, 0 );

        //grab all moveables from the node that intersect...
        std::list < SceneNode * >::iterator it = list.begin();
        while( it != list.end() )
        {
            SceneNode::ObjectIterator oit = (*it) -> getAttachedObjectIterator();
            while( oit.hasMoreElements() )
            {
                MovableObject * m = oit.getNext();
                if( (m->getQueryFlags() & mQueryMask) && 
					m->isInScene() &&
					(*pi).intersects( m->getWorldBoundingBox() ) )
                {
                    listener -> queryResult( m );
                }
            }
            ++it;
        }
    }//for
}


}
