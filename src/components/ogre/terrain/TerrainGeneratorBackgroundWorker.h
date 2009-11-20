/*
 * Copyright (C) 2009 Manuel A. Fernandez Montecelo <manuel.montezelo@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 675 Mass
 * Ave, Cambridge, MA 02139, USA.
 */

#ifndef TERRAINGENERATORBACKGROUNDWORKER_H
#define TERRAINGENERATORBACKGROUNDWORKER_H

#include "components/ogre/Types.h"
#include "components/ogre/terrain/TerrainInfo.h"
#include "framework/ConsoleObject.h"
#include "services/config/ConfigListenerContainer.h"

#include <wfmath/point.h> //needed for the terrain position

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

#include <list>

namespace EmberOgre {
namespace Terrain {

class TerrainPage;
class ITerrainPageBridge;

class TerrainGenerator;

/**
 * @brief Creates pages in a background thread.
 *
 * @author Manuel A. Fernandez Montecelo <manuel.montezelo@gmail.com>
 *
 */
class TerrainGeneratorBackgroundWorker
{
public:
	/** Ctor. */
	TerrainGeneratorBackgroundWorker() : mIsProcessing(false), mThread(0) {}

	/** Tick be called periodically (e.g. after frame ended) to continue processing page creation requests */
	void tick();

	/** Push a page into the queue, to be loaded in the background */
	void pushPageIntoQueue(const TerrainPosition& pos, ITerrainPageBridge* bridge);

	/** Get one of the ready pages.
	 *
	 * @return 0 if not available, the first in the set of created pages otherwise
	 */
	TerrainPage* popPageReady();

	/** Push one of ready page, called when the page creator completes the operation.
	 */
	void pushPageReady(TerrainPage* page);

private:
	/** Set of pages ready */
	std::list<TerrainPage*> mPagesReady;
	/** Queue of pages to be loaded */
	std::list<std::pair<TerrainPosition, ITerrainPageBridge*> > mPagesQueue;
	/** Flag to know when a thread is already processing a request (only one request at a time, pages access other pages' data so they cannot be created in parallel) */
	bool mIsProcessing;
	/** Thread, a pointer to free the thread after finishes */
	boost::thread* mThread;

	/** Mutex for shared variable */
	boost::mutex mMutexPagesReady;
	/** Mutex for shared variable */
	boost::mutex mMutexPagesQueue;
	/** Mutex for shared variable */
	boost::mutex mMutexIsProcessing;

	/** Helper function to peek at the queue and create a page if requested */
	void createPageFromQueue();
};

}
}

#endif // TERRAINGENERATORBACKGROUNDWORKER_H
