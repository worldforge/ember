/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/
 
Copyright  2000-2006 The OGRE Team
Also see acknowledgements in Readme.html
 
This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.
 
This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
You should have received a copy of the GNU General Public License along with
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

#include "OgrePagingLandScapePrecompiledHeaders.h"

#include "OgrePagingLandScapeOctreeIntersectionSceneQuery.h"
#include "OgrePagingLandScapeOctreeSceneManager.h"
#include "OgreEntity.h"
#include "OgreRoot.h"

namespace Ogre
{

//---------------------------------------------------------------------
PagingLandScapeOctreeIntersectionSceneQuery::PagingLandScapeOctreeIntersectionSceneQuery(SceneManager* creator)
	: DefaultIntersectionSceneQuery(creator)
{

}

//---------------------------------------------------------------------
PagingLandScapeOctreeIntersectionSceneQuery::~PagingLandScapeOctreeIntersectionSceneQuery(void)
{
}

//---------------------------------------------------------------------
void PagingLandScapeOctreeIntersectionSceneQuery::execute(IntersectionSceneQueryListener* listener)
{
	typedef std::pair<MovableObject *, MovableObject *> MovablePair;
	typedef std::set
		< std::pair<MovableObject *, MovableObject *> > MovableSet;

	MovableSet set;

	// Iterate over all movable types
	Root::MovableObjectFactoryIterator factIt = 
		Root::getSingleton().getMovableObjectFactoryIterator();
	while(factIt.hasMoreElements())
	{
		SceneManager::MovableObjectIterator it = 
			mParentSceneMgr->getMovableObjectIterator(
			factIt.getNext()->getType());
		while( it.hasMoreElements() )
		{

			MovableObject * e = it.getNext();

			std::list < SceneNode * > list;
			//find the nodes that intersect the AAB
			static_cast<PagingLandScapeOctreeSceneManager*>( mParentSceneMgr ) -> findNodesIn( e->getWorldBoundingBox(), list, 0 );
			//grab all movables from the node that intersect...
			std::list < SceneNode * >::iterator itscn = list.begin();
			while( itscn != list.end() )
			{
				SceneNode::ObjectIterator oit = (*itscn) -> getAttachedObjectIterator();
				while( oit.hasMoreElements() )
				{
					MovableObject * m = oit.getNext();

					if( m != e &&
						set.find( MovablePair(e,m)) == set.end() &&
						set.find( MovablePair(m,e)) == set.end() &&
						(m->getQueryFlags() & mQueryMask) &&
						(m->getTypeFlags() & mQueryTypeMask) &&
						m->isInScene() && 
						e->getWorldBoundingBox().intersects( m->getWorldBoundingBox() ) )
					{
						listener -> queryResult( e, m );
						// deal with attached objects, since they are not directly attached to nodes
						if (m->getMovableType() == "Entity")
						{
							Entity* e2 = static_cast<Entity*>(m);
							Entity::ChildObjectListIterator childIt = e2->getAttachedObjectIterator();
							while(childIt.hasMoreElements())
							{
								MovableObject* c = childIt.getNext();
								if (c->getQueryFlags() & mQueryMask && 
									e->getWorldBoundingBox().intersects( c->getWorldBoundingBox() ))
								{
									listener->queryResult(e, c);
								}
							}
						}
					}
					set.insert( MovablePair(e,m) );

				}
				++itscn;
			}

		}
	}
}

}
