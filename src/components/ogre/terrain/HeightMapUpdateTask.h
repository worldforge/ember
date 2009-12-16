/*
 Copyright (C) 2009 Erik Hjortsberg <erik.hjortsberg@gmail.com>

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

#ifndef HEIGHTMAPUPDATETASK_H_
#define HEIGHTMAPUPDATETASK_H_

#include "framework/tasks/ITask.h"

#include <vector>

namespace WFMath
{
	template<int> class Point;
}

namespace Mercator
{
	class Segment;
}

namespace EmberOgre
{

namespace Terrain
{
class HeightMap;
class HeightMapBufferProvider;
class HeightMapSegment;

class HeightMapUpdateTask : public Ember::Tasks::ITask
{
public:
	typedef std::vector<Mercator::Segment*> SegmentStore;
	HeightMapUpdateTask(HeightMapBufferProvider& provider, HeightMap& heightMap, const SegmentStore& segments);
	virtual ~HeightMapUpdateTask();

	virtual void executeTaskInBackgroundThread(Ember::Tasks::TaskExecutionContext& context);

	virtual void executeTaskInMainThread();

private:

	typedef std::vector<std::pair<WFMath::Point<2>, HeightMapSegment*> > HeightMapSegmentStore;
	HeightMapBufferProvider& mProvider;
	HeightMap& mHeightMap;
	SegmentStore mSegments;
	HeightMapSegmentStore mHeightMapSegments;


	void createHeightMapSegments();
	void injectHeightMapSegmentsIntoHeightMap();


};

}

}

#endif /* HEIGHTMAPUPDATETASK_H_ */
