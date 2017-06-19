/*
 Copyright (C) 2010 Erik Ogenvik <erik@ogenvik.org>

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
#include "PlantAreaQuery.h"
#include "foliage/PlantPopulator.h"
#include "components/ogre/Convert.h"

namespace Ember
{
namespace OgreView
{

namespace Terrain
{

PlantQueryTask::PlantQueryTask(const SegmentRefPtr& segmentRef, Foliage::PlantPopulator& plantPopulator, const PlantAreaQuery& query, const Ogre::ColourValue& defaultShadowColour, sigc::slot<void, const PlantAreaQueryResult&> asyncCallback) :
	mSegmentRef(segmentRef), mPlantPopulator(plantPopulator), mAsyncCallback(asyncCallback), mQueryResult(query)
{
	mQueryResult.setDefaultShadowColour(defaultShadowColour);
}

PlantQueryTask::~PlantQueryTask()
{
}

void PlantQueryTask::executeTaskInBackgroundThread(Tasks::TaskExecutionContext& context)
{
	mPlantPopulator.populate(mQueryResult, mSegmentRef);
	//Release Segment references as soon as we can
	mSegmentRef.reset();
}

bool PlantQueryTask::executeTaskInMainThread()
{
	mAsyncCallback(mQueryResult);
	return true;
}
}

}
}
