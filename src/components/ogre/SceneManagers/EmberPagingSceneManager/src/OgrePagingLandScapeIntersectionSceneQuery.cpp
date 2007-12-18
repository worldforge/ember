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
OgrePagingLandScapeIntersectionSceneQuery.cpp  -  description
-------------------
begin                : Tues July 20, 2004
copyright            : (C) 2004by Jon Anderson
email                : janders@users.sf.net
 
 
 
***************************************************************************/

#include "OgrePagingLandScapePrecompiledHeaders.h"

#include "OgrePagingLandScapeSceneManager.h"
#include "OgrePagingLandScapeIntersectionSceneQuery.h"

#include "OgrePagingLandScapeOctreeSceneManager.h"
#include "OgrePagingLandScapeOctreeIntersectionSceneQuery.h"

#include "OgreEntity.h"
#include "OgreRoot.h"

namespace Ogre
{

//---------------------------------------------------------------------
PagingLandScapeIntersectionSceneQuery::PagingLandScapeIntersectionSceneQuery(SceneManager* creator)
	: PagingLandScapeOctreeIntersectionSceneQuery(creator)
{
    // Add bounds fragment type
    mSupportedWorldFragments.insert(SceneQuery::WFT_SINGLE_INTERSECTION);
    mSupportedWorldFragments.insert(SceneQuery::WFT_PLANE_BOUNDED_REGION);
}

//---------------------------------------------------------------------
PagingLandScapeIntersectionSceneQuery::~PagingLandScapeIntersectionSceneQuery(void)
{
}

//---------------------------------------------------------------------
void PagingLandScapeIntersectionSceneQuery::execute(IntersectionSceneQueryListener* listener)
{
	if(mWorldFragmentType == SceneQuery::WFT_SINGLE_INTERSECTION)
    {
		PagingLandScapeOctreeIntersectionSceneQuery::execute(listener);
    }
    else if (mWorldFragmentType ==  SceneQuery::WFT_PLANE_BOUNDED_REGION)
    {
        // We want the whole bounded volume
//        assert((*bi)->fragment.fragmentType == SceneQuery::WFT_PLANE_BOUNDED_REGION);
//        if (!listener->queryResult(const_cast<WorldFragment*>(&(brush->fragment)), 
//            result.second + traceDistance))
//			return false; 

    }
}

}
