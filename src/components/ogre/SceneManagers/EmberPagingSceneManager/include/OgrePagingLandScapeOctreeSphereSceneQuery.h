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
OgrePagingLandScapeOctreeSphereSceneQuery.h  -  description
-------------------
begin                : Tues July 20, 2004
copyright            : (C) 2004 by Jon Anderson
email                : janders@users.sf.net
***************************************************************************/

#ifndef PagingLandScapeOctreeSphereSCENEQUERY_H
#define PagingLandScapeOctreeSphereSCENEQUERY_H

#include "OgrePagingLandScapePrerequisites.h"
#include <OgreSceneManager.h>

namespace Ogre
{

/** PagingLandScapeOctree implementation of SphereSceneQuery. */
class _OgrePagingLandScapeExport PagingLandScapeOctreeSphereSceneQuery : public DefaultSphereSceneQuery
{
public:
    PagingLandScapeOctreeSphereSceneQuery( SceneManager* creator );
    virtual ~PagingLandScapeOctreeSphereSceneQuery( void );

    /** See SceneQuery. */
    void execute( SceneQueryListener* listener );
};

}

#endif
