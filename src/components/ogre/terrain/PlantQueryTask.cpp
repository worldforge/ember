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
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "PlantQueryTask.h"

#include <utility>
#include "PlantAreaQuery.h"
#include "foliage/PlantPopulator.h"
#include "components/ogre/Convert.h"

namespace Ember {
namespace OgreView {

namespace Terrain {

PlantQueryTask::PlantQueryTask(SegmentRefPtr segmentRef,
							   Foliage::PlantPopulator& plantPopulator,
							   PlantAreaQuery query,
							   const Ogre::ColourValue& defaultShadowColour,
							   sigc::slot<void, const PlantAreaQueryResult&> asyncCallback) :
		mSegmentRef(std::move(segmentRef)),
		mPlantPopulator(plantPopulator),
		mAsyncCallback(std::move(asyncCallback)),
		mQueryResult(std::move(query)) {
	mQueryResult.setDefaultShadowColour(defaultShadowColour);
}

PlantQueryTask::~PlantQueryTask() = default;

void PlantQueryTask::executeTaskInBackgroundThread(Tasks::TaskExecutionContext& context) {
	mPlantPopulator.populate(mQueryResult, mSegmentRef);
	//Release Segment references as soon as we can
	mSegmentRef.reset();
}

bool PlantQueryTask::executeTaskInMainThread() {
	mAsyncCallback(mQueryResult);
	return true;
}
}

}
}
