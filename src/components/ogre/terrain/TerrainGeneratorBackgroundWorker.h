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
 * @brief Creates terrain pages in a background thread.
 *
 * This class has to be instantiated by the TerrainGenerator, and it spawns a thread (at most one at the same time) for every terrain page that is requested to create (by providing the position of the terrain page and the bridge to PLSM by pushPageIntoQueue()).
 *
 * When the thread spawns, it calls back to TerrainGenerator::createPage() (now static), which in turn performs the typical steps that were until now used to create a page (only the ones which are thread-safe though), and that method calls back to pushPageReady() to store the newly created page in a safe way (destroying the threaded path while at it).  The TerrainGenerator checks once every frame (in frameEnded() method) if there's a new page ready, and then post-processes it (mostly performing non-thread-safe operations), and passes it over to PLSM as before.
 *
 * This somewhat complicate scheme of things was devised so the threaded operations were separate and safe (the methods in this class are protected by locks), and the code was minimally altered (the process of creating a page requires functionality of a lot of TerrainGenerator's facilities).  Probably an overhaul/redesign of this and TerrainGenerator classes, and maybe even all EmberOgre::Terrain hierarchy, would be desirable to end with a better overall design, with mistake-proof implementations, taking more advantages of multi-threading, and in general simplifying things.
 *
 * @author Manuel A. Fernandez Montecelo <manuel.montezelo@gmail.com>
 *
 */
class TerrainGeneratorBackgroundWorker
{
public:
	/** @brief Ctor. */
	TerrainGeneratorBackgroundWorker() : mIsProcessing(false), mThread(0) {}

	/** @brief Tick be called periodically (e.g. after frame ended) to continue processing page creation requests */
	void tick();

	/** @brief Push a page into the queue, to be loaded in the background */
	void pushPageIntoQueue(const TerrainPosition& pos, ITerrainPageBridge* bridge);

	/** @brief Get one of the ready pages.
	 *
	 * @return 0 if not available, the first in the set of created pages otherwise
	 */
	TerrainPage* popPageReady();

	/** @brief Push one of ready page, called when the page creator completes the operation.
	 */
	void pushPageReady(TerrainPage* page);

private:
	/** @brief Set of pages ready */
	std::list<TerrainPage*> mPagesReady;
	/** @brief Queue of pages to be loaded */
	std::list<std::pair<TerrainPosition, ITerrainPageBridge*> > mPagesQueue;
	/** @brief Flag to know when a thread is already processing a request (only one request at a time, pages access other pages' data so they cannot be created in parallel) */
	bool mIsProcessing;
	/** @brief Thread, a pointer to free the thread after finishes */
	boost::thread* mThread;

	/** @brief Mutex for shared variable */
	boost::mutex mMutexPagesReady;
	/** @brief Mutex for shared variable */
	boost::mutex mMutexPagesQueue;
	/** @brief Mutex for shared variable */
	boost::mutex mMutexIsProcessing;


	/** @brief Helper function to peek at the queue and create a page if requested */
	void createPageFromQueue();
};

}
}

#endif // TERRAINGENERATORBACKGROUNDWORKER_H
