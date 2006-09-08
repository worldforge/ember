/***************************************************************************
OgrePagingLandScapeTileManager.cpp  -  description
-------------------
begin                : Mon Jun 16 2003
copyright            : (C) 2003-2006 by Jose A. Milan and Tuan Kuranes
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

#include "OgrePagingLandScapePrecompiledHeaders.h"


#include "OgreVector3.h"
#include "OgreColourValue.h"

#include "OgreMovableObject.h"
#include "OgreAxisAlignedBox.h"

#include "OgreCamera.h"

#include "OgrePagingLandScapeSceneManager.h"

#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeCamera.h"
#include "OgrePagingLandScapeTileManager.h"
#include "OgrePagingLandScapeTile.h"

namespace Ogre
{

	//-----------------------------------------------------------------------
	PagingLandScapeTileManager::PagingLandScapeTileManager(PagingLandScapeSceneManager * scnMgr):
mSceneManager(scnMgr),
mOptions(scnMgr->getOptions ())
{	
	mNumTiles = 0;
	// Add the requested initial number
	//_addBatch(mParent->getOptions()->num_tiles);
}
//-----------------------------------------------------------------------
PagingLandScapeTileManager::~PagingLandScapeTileManager()
{
	assert (mTiles.size() == mNumTiles);
	std::for_each(mTiles.begin(), 
		mTiles.end(),  
		delete_object()); 
	// destroy tiles
	mTiles.clear();

}
//-----------------------------------------------------------------------
void PagingLandScapeTileManager::clear()
{
	assert (mTiles.size() == (unsigned int) mQueue.getSize() && 
		mTiles.size() == mNumTiles);
}
//-----------------------------------------------------------------------
void PagingLandScapeTileManager::load()
{
	const unsigned int nTile = mOptions->num_tiles;
	if (mNumTiles < nTile)
	{
		_addBatch (nTile - mNumTiles);
	}
	assert (mOptions->num_tiles <= mTiles.size());
	//    else if (mNumTiles > nTile)
	//    {
	//        for (unsigned int i = nTile; i < mNumTiles; i++)
	//	    {
	//            PagingLandScapeTile  *t = mTiles[i];
	//            mQueue.remove(t);
	//            delete t;
	//	    }
	//        mNumTiles = nTile;
	//        mTiles.resize (nTile);
	//    }
}
//-----------------------------------------------------------------------
PagingLandScapeTile *PagingLandScapeTileManager::getTile()
{
	if (mQueue.empty())
	{
		// We do not have more tiles, so we need to allocate more
		_addBatch(mOptions->num_tiles_increment);
		// Increment the next batch by a 10%
		//mParent->getOptions()->num_tiles_increment += static_cast<unsigned int> (mParent->getOptions()->num_tiles_increment * 0.1f);
	}
	return mQueue.pop();
}
//-----------------------------------------------------------------------
void PagingLandScapeTileManager::freeTile(PagingLandScapeTile *tile)
{
	assert (!tile->isLoaded () && tile->getRenderable () == 0);
	mQueue.push (tile);
}
//-----------------------------------------------------------------------
unsigned int PagingLandScapeTileManager::numTiles(void) const
{
	return mNumTiles;
}
//-----------------------------------------------------------------------
size_t PagingLandScapeTileManager::numFree() const
{
	return mQueue.getSize();
}
//-----------------------------------------------------------------------
void PagingLandScapeTileManager::_addBatch(const unsigned int num)
{
	mNumTiles += num;
	mTiles.reserve (mNumTiles);
	for (unsigned int i = 0; i < num; i++)
	{
		PagingLandScapeTile* tile = new PagingLandScapeTile(this);
		mTiles.push_back (tile);
		mQueue.push (tile);
	}
}
//-----------------------------------------------------------------------
void PagingLandScapeTileManager::updateLoadedTiles()
{
	std::for_each(mTiles.begin(), 
		mTiles.end(),  
		std::mem_fun(&PagingLandScapeTile::touched)); 
}

} //namespace
