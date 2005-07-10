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
OgrePagingLandScapeOctreeIntersectionSceneQuery.cpp  -  description
-------------------
begin                : Tues July 20, 2004
copyright            : (C) 2004by Jon Anderson
email                : janders@users.sf.net
 
 
 
***************************************************************************/

#include <OgrePagingLandScapeOctreeIntersectionSceneQuery.h>
#include <OgrePagingLandScapeOctreeSceneManager.h>
#include <OgreEntity.h>

namespace Ogre
{

//---------------------------------------------------------------------
PagingLandScapeOctreeIntersectionSceneQuery::PagingLandScapeOctreeIntersectionSceneQuery( SceneManager* creator )
	: DefaultIntersectionSceneQuery(creator)
{

}

//---------------------------------------------------------------------
PagingLandScapeOctreeIntersectionSceneQuery::~PagingLandScapeOctreeIntersectionSceneQuery( void )
{
}

//---------------------------------------------------------------------
void PagingLandScapeOctreeIntersectionSceneQuery::execute( IntersectionSceneQueryListener* listener )
{
    typedef std::pair< MovableObject*, MovableObject* > MovablePair;
    typedef std::set< std::pair< MovableObject*, MovableObject* > > MovableSet;

    MovableSet set;

    SceneManager::EntityIterator ite = mParentSceneMgr->getEntityIterator( );
    while ( ite.hasMoreElements( ) )
    {
        Entity* e = ite.getNext( );

        std::list < SceneNode* > list;
        //find the nodes that intersect the AAB
        static_cast< PagingLandScapeOctreeSceneManager* >( mParentSceneMgr )->findNodesIn( e->getWorldBoundingBox( ), list, 0 );
        //grab all movables from the node that intersect...
        std::list < SceneNode* >::iterator it = list.begin( );
        while ( it != list.end( ) )
        {
            SceneNode::ObjectIterator oit = ( *it )->getAttachedObjectIterator( );
            while ( oit.hasMoreElements( ) )
            {
                MovableObject* const m = oit.getNext( );

                if ( m != e 
                    && ( m->getQueryFlags( ) & mQueryMask ) 
                    && m->isInScene( ) 
                    && set.find( MovablePair( e, m ) ) == set.end( ) 
                    && set.find( MovablePair( m, e ) ) == set.end( ) 
                    && e->getWorldBoundingBox( ).intersects( m->getWorldBoundingBox( ) ) )
                {
                    listener->queryResult( e, m );
                }
                set.insert( MovablePair( e, m ) );
            }
            ++it;
        }
    }
}

}
