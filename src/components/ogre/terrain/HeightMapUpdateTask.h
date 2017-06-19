/*
 Copyright (C) 2009 Erik Ogenvik <erik@ogenvik.org>

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

#include "framework/tasks/TemplateNamedTask.h"

#include <vector>

namespace WFMath
{
	template<int> class Point;
}

namespace Mercator
{
	class Segment;
}

namespace Ember
{
namespace OgreView
{

namespace Terrain
{
class HeightMap;
class HeightMapBufferProvider;
class IHeightMapSegment;

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief Builds new HeightMapSegments in the background and then inserts these into the HeightMap.
 * This is the main task for making sure that the HeightMap is kept up to date with changes to the Mercator terrain.
 */
class HeightMapUpdateTask : public Tasks::TemplateNamedTask<HeightMapUpdateTask>
{
public:
	typedef std::vector<Mercator::Segment*> SegmentStore;

	/**
	 * @brief Ctor.
	 * @param provider The provider which is tasked to create the HeightMapBuffer instances.
	 * @param heightMap The main HeightMap instance, which holds the whole height map.
	 * @param segments The Mercator::Segments for which we'll be creating HeightMapSegments.
	 */
	HeightMapUpdateTask(HeightMapBufferProvider& provider, HeightMap& heightMap, const SegmentStore& segments);
	virtual ~HeightMapUpdateTask();

	virtual void executeTaskInBackgroundThread(Tasks::TaskExecutionContext& context);

	virtual bool executeTaskInMainThread();

private:

	typedef std::vector<std::pair<WFMath::Point<2>, IHeightMapSegment*>> HeightMapSegmentStore;

	/**
	 * @brief The provider which is tasked to create the HeightMapBuffer instances.
	 */
	HeightMapBufferProvider& mProvider;

	/**
	 * @brief The main HeightMap instance, which holds the whole height map.
	 */
	HeightMap& mHeightMap;

	/**
	 * @brief The Mercator::Segments for which we'll be creating HeightMapSegments.
	 */
	SegmentStore mSegments;

	/**
	 * @brief The HeightMapSegment instances created. These will be injected into the HeightMap.
	 */
	HeightMapSegmentStore mHeightMapSegments;

	/**
	 * @brief Creates the HeightMapSegment instances.
	 */
	void createHeightMapSegments();

	/**
	 * @brief Injects the previously created HeightMapSegment instances into the HeightMap.
	 */
	void injectHeightMapSegmentsIntoHeightMap();


};

}

}

}

#endif /* HEIGHTMAPUPDATETASK_H_ */
