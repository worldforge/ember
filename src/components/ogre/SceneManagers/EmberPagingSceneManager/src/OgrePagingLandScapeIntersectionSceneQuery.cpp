/***************************************************************************
	OgrePagingLandScapeIntersectionSceneQuery.cpp  -  description
  -------------------
  begin                : Fri Aug 08 2003
  copyright            : (C) 2003-2005 by Jose A Milan && Tuan Kuranes
  email                : spoke2@supercable.es && tuan.kuranes@free.fr
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU Lesser General Public License as        *
*   published by the Free Software Foundation; either version 2 of the    *
*   License, or (at your option) any later version.                       *
*                                                                         *
***************************************************************************/

#include "OgreEntity.h"

#include "OgreVector3.h"
#include "OgreColourValue.h"

#include "OgreSceneManager.h"
#include "OgreCamera.h"

#include "OgrePagingLandScapeOptions.h"

#include "OgrePagingLandScapeSceneManager.h"

#include "OgrePagingLandScapeData2DManager.h"

#include "OgrePagingLandScapeRaySceneQuery.h"
#include "OgrePagingLandScapeIntersectionSceneQuery.h"
#include "OgrePagingLandScapeCamera.h"
#include "OgrePagingLandScapePageManager.h"
#include "OgrePagingLandScapePage.h"

namespace Ogre
{

//----------------------------------------------------------------------------
void PagingLandScapeIntersectionSceneQuery::execute(IntersectionSceneQueryListener* listener)
{
	// Do movables to movables as before
	DefaultIntersectionSceneQuery::execute(listener);
	SceneQuery::WorldFragment frag;

	// Do entities to world
	SceneManager::EntityList::const_iterator a, theEnd;
	PagingLandScapeSceneManager *sceneMgr = static_cast<PagingLandScapeSceneManager*>(mParentSceneMgr);
	theEnd = sceneMgr->getEntities().end();
	for (a = sceneMgr->getEntities().begin();
        a != theEnd; 
        ++a)
	{
		// Apply mask 
		if ( a->second->getQueryFlags() & mQueryMask)
		{
			const AxisAlignedBox& box = a->second->getWorldBoundingBox();
			std::list<RenderOperation> opList;

            			//this does not compile, had to disable it
            //PagingLandScapePageManager::getSingleton().getPagingLandScapeRenderOpsInBox (box, opList);
					

			std::list<RenderOperation>::iterator i, iend;
			iend = opList.end();
			for (i = opList.begin(); i != iend; ++i)
			{
				frag.fragmentType = SceneQuery::WFT_RENDER_OPERATION;
				frag.renderOp = &(*i);
				listener->queryResult(a->second, &frag);
			}
		}
	}
}

} // namespace Ogre

