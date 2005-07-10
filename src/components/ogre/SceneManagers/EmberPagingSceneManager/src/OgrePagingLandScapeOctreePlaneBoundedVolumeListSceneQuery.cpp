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
OgrePagingLandScapeOctreePlaneBoundedVolumeListSceneQuery.cpp  -  description
-------------------
begin                : Tues July 20, 2004
copyright            : (C) 2004by Jon Anderson
email                : janders@users.sf.net
 
 
 
***************************************************************************/

#include <OgrePagingLandScapeOctreePlaneBoundedVolumeListSceneQuery.h>
#include <OgrePagingLandScapeOctreeSceneManager.h>
#include <OgreEntity.h>

namespace Ogre
{

//---------------------------------------------------------------------
PagingLandScapeOctreePlaneBoundedVolumeListSceneQuery::PagingLandScapeOctreePlaneBoundedVolumeListSceneQuery( SceneManager* creator )
	: DefaultPlaneBoundedVolumeListSceneQuery( creator )
{
}

//---------------------------------------------------------------------
PagingLandScapeOctreePlaneBoundedVolumeListSceneQuery::~PagingLandScapeOctreePlaneBoundedVolumeListSceneQuery( void )
{
}

//---------------------------------------------------------------------
void PagingLandScapeOctreePlaneBoundedVolumeListSceneQuery::execute( SceneQueryListener* listener )
{
    PlaneBoundedVolumeList::iterator pi, piend;
    piend = mVolumes.end( );
    for ( pi = mVolumes.begin( ); pi != piend; ++pi )
    {
        std::list < SceneNode* > list;
        //find the nodes that intersect the AAB
        static_cast< PagingLandScapeOctreeSceneManager* >( mParentSceneMgr )->findNodesIn( *pi, list, 0 );

        //grab all moveables from the node that intersect...
        std::list < SceneNode* >::iterator it = list.begin( );
        while ( it != list.end( ) )
        {
            SceneNode::ObjectIterator oit = ( *it )->getAttachedObjectIterator( );
            while ( oit.hasMoreElements( ) )
            {
                MovableObject* m = oit.getNext( );
                if ( ( m->getQueryFlags( ) & mQueryMask ) && m->isInScene( ) &&	( *pi ).intersects( m->getWorldBoundingBox( ) ) )
                {
                    listener->queryResult( m );
                }
            }
            ++it;
        }
    }
}

}
