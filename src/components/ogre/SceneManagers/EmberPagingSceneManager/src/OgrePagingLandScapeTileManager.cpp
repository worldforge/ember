/***************************************************************************
	OgrePagingLandScapeTileManager.cpp  -  description
	-------------------
	begin                : Mon Jun 16 2003
	copyright            : (C) 2003-2005 by Jose A. Milan and Tuan Kuranes
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


#include "OgreVector3.h"
#include "OgreColourValue.h"

#include "OgreMovableObject.h"
#include "OgreAxisAlignedBox.h"

#include "OgreCamera.h"


#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeCamera.h"
#include "OgrePagingLandScapeTileManager.h"
#include "OgrePagingLandScapeTile.h"

namespace Ogre
{

//-----------------------------------------------------------------------
template<> PagingLandScapeTileManager* Singleton<PagingLandScapeTileManager>::ms_Singleton = 0;
PagingLandScapeTileManager* PagingLandScapeTileManager::getSingletonPtr(void)
{
	return ms_Singleton;
}
PagingLandScapeTileManager& PagingLandScapeTileManager::getSingleton(void)
{  
	assert( ms_Singleton );  return ( *ms_Singleton );  
}

//-----------------------------------------------------------------------
PagingLandScapeTileManager::PagingLandScapeTileManager( )
{	
	mNumTiles = 0;
	// Add the requested initial number
	_addBatch(PagingLandScapeOptions::getSingleton().num_tiles);
}
//-----------------------------------------------------------------------
PagingLandScapeTileManager::~PagingLandScapeTileManager( )
{
    assert (mTiles.size() == mNumTiles);
    std::for_each( mTiles.begin(), 
             mTiles.end(),  
              delete_object()); 
	// destroy tiles
	mTiles.clear();

}
//-----------------------------------------------------------------------
void PagingLandScapeTileManager::clear()
{
    assert ( mTiles.size() == (uint) mQueue.getSize() && 
             mTiles.size() == mNumTiles);
}
//-----------------------------------------------------------------------
void PagingLandScapeTileManager::load()
{
    const uint nTile = PagingLandScapeOptions::getSingleton().num_tiles;
    if (mNumTiles < nTile)
    {
        _addBatch (nTile - mNumTiles);
    }
//    else if (mNumTiles > nTile)
//    {
//        for (uint i = nTile; i < mNumTiles; i++)
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
PagingLandScapeTile *PagingLandScapeTileManager::getTile( )
{
	if ( mQueue.empty() )
	{
		// We don´t have more renderables, so we are going to add more
		_addBatch(PagingLandScapeOptions::getSingleton().num_tiles_increment);
		// Increment the next batch by a 10%
		//PagingLandScapeOptions::getSingleton().num_tiles_increment += static_cast<uint> (PagingLandScapeOptions::getSingleton().num_tiles_increment * 0.1f);
	}
	return mQueue.pop( );
}

//-----------------------------------------------------------------------
void PagingLandScapeTileManager::freeTile( PagingLandScapeTile *tile )
{
	mQueue.push( tile );
}

//-----------------------------------------------------------------------
uint PagingLandScapeTileManager::numTiles( void ) const
{
	return mNumTiles;
}

//-----------------------------------------------------------------------
int PagingLandScapeTileManager::numFree( ) const
{
	return mQueue.getSize( );
}

//-----------------------------------------------------------------------
void PagingLandScapeTileManager::_addBatch(const uint num)
{
	mNumTiles += num;
    mTiles.reserve (mNumTiles);
	for ( uint i = 0; i < num; i++ )
	{
		PagingLandScapeTile* tile = new PagingLandScapeTile( );
		mTiles.push_back(tile);
		mQueue.push( tile );
	}
}

} //namespace
