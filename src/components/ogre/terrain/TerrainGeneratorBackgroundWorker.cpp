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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "TerrainGeneratorBackgroundWorker.h"

#include "TerrainGenerator.h"
#include "TerrainPage.h"


namespace EmberOgre {
namespace Terrain {

void TerrainGeneratorBackgroundWorker::tick()
{
	// join and destroy finished thread
	if (!mIsProcessing && mThread) {
		mThread->join();
		delete mThread;
		mThread = 0;
	}

	// call to create new page from queue, if available
	createPageFromQueue();
}

void TerrainGeneratorBackgroundWorker::pushPageIntoQueue(const TerrainPosition& pos, ITerrainPageBridge* bridge)
{
	// add to queue
	{
		boost::mutex::scoped_lock lock(mMutexPagesQueue);
		mPagesQueue.push_back(std::pair<TerrainPosition, ITerrainPageBridge*>(pos, bridge));
		S_LOG_INFO("Puting TerrainPage at index [" << pos.x() << "," << pos.y() << "] in the queue of background thread");
	}

	// call to create new page from queue, if available
	createPageFromQueue();
}

TerrainPage* TerrainGeneratorBackgroundWorker::popPageReady()
{
	boost::mutex::scoped_lock lock(mMutexPagesReady);
	if (mPagesReady.empty()) {
		return 0;
	} else {
		TerrainPage* page = mPagesReady.front();
		mPagesReady.pop_front();
		return page;
	}
}

void TerrainGeneratorBackgroundWorker::pushPageReady(TerrainPage* page)
{
	// push page ready
	{
		boost::mutex::scoped_lock lock(mMutexPagesReady);
		mPagesReady.push_back(page);
		S_LOG_INFO("TerrainPage at index [" << page->getWFPosition().x() << "," << page->getWFPosition().y() << "] ready, from background thread");
	}

	// set off processing flag
	{
		boost::mutex::scoped_lock lock(mMutexIsProcessing);
		mIsProcessing = false;
	}
}

void TerrainGeneratorBackgroundWorker::createPageFromQueue()
{
	boost::mutex::scoped_lock lock(mMutexIsProcessing);
	if (!mIsProcessing) {
		boost::mutex::scoped_lock lock(mMutexPagesQueue);
		if (!mPagesQueue.empty()) {
			TerrainPosition pos = mPagesQueue.front().first;
			ITerrainPageBridge* bridge = mPagesQueue.front().second;
			S_LOG_INFO("TerrainPage at index [" << pos.x() << "," << pos.y() << "] being created from background thread");
			mThread = new boost::thread(boost::bind(&TerrainGenerator::createPage,
								this, pos, bridge));
			mPagesQueue.pop_front();
			mIsProcessing = true;
		}
	}
}



}
}
