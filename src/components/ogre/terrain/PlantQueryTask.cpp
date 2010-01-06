/*
 Copyright (C) 2010 Erik Hjortsberg <erik.hjortsberg@gmail.com>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "PlantQueryTask.h"
#include "TerrainPage.h"
#include "PlantAreaQuery.h"
#include "components/ogre/Convert.h"

namespace EmberOgre
{

namespace Terrain
{

PlantQueryTask::PlantQueryTask(const TerrainPage& page, const PlantAreaQuery& query, sigc::slot<void, const PlantAreaQueryResult&> asyncCallback) :
	mPage(page), mQueryResult(query), mAsyncCallback(asyncCallback)
{
}

PlantQueryTask::~PlantQueryTask()
{
}

void PlantQueryTask::executeTaskInBackgroundThread(Ember::Tasks::TaskExecutionContext& context)
{
	Ogre::TRect<float> ogrePageExtent = Convert::toOgre(mPage.getExtent());
	Ogre::TRect<float> adjustedBounds = Ogre::TRect<float>(mQueryResult.getQuery().getArea().left - ogrePageExtent.left, mQueryResult.getQuery().getArea().top - ogrePageExtent.top, mQueryResult.getQuery().getArea().right - ogrePageExtent.left, mQueryResult.getQuery().getArea().bottom - ogrePageExtent.top);
	mPage.getPlantsForArea(mQueryResult);
}

void PlantQueryTask::executeTaskInMainThread()
{
	mAsyncCallback(mQueryResult);
}
}

}
