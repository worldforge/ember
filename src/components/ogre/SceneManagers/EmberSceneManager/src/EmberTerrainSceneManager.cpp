/*
    Copyright (C) 2004  Erik Hjortsberg

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
#include "services/logging/LoggingService.h"

#include "components/ogre/TerrainGenerator.h"
//#include "EmberTerrainRenderable.h"
#include "components/ogre/MathConverter.h"

#include "EmberTerrainPageSource.h"
#include "EmberTerrainSceneManager.h"
namespace EmberOgre {

/*
EmberTerrainSceneManager* EmberTerrainSceneManager::_instance = 0;


EmberTerrainSceneManager & EmberTerrainSceneManager::getSingleton(void)
{
	//fprintf(stderr, "TRACE - ENTITY LISTENER - SINGLETON ENTERING\n");
	if(_instance == 0)
		_instance = new EmberTerrainSceneManager;
	return *_instance;
}
*/

EmberTerrainSceneManager::EmberTerrainSceneManager() 
: Ogre::TerrainSceneManager()
//: mGenerator(0)
{
	mPagingEnabled = true;
	mLivePageMargin = 1;
	mBufferedPageMargin = 20;
	mPageOffset = 10;

}
EmberTerrainSceneManager::~EmberTerrainSceneManager()
{}

int EmberTerrainSceneManager::getPageOffset()
{
	return mPageOffset;	
}

//-------------------------------------------------------------------------
void EmberTerrainSceneManager::setWorldGeometry( const Ogre::String& filename )
{
	mTerrainPages.clear();
	// Load the configuration
	loadConfig(filename);
// 	mOptions = options;
// 	mTerrainPages.clear();

// 	// Resize the octree, allow for 1 page for now
// 	float max_x = mOptions.scale.x * mOptions.pageSize;
// 	float max_y = mOptions.scale.y;
// 	float max_z = mOptions.scale.z * mOptions.pageSize;
// 	resize( Ogre::AxisAlignedBox( 0, 0, 0, max_x, max_y, max_z ) );

//	setupTerrainMaterial();

	setupTerrainPages();

}

//-------------------------------------------------------------------------
void EmberTerrainSceneManager::setWorldGeometry(  Ogre::TerrainOptions& options )
{
	mTerrainPages.clear();
	mOptions = options;
	
	// Load the configuration
//	loadConfig(filename);
// 	mOptions = options;
// 	mTerrainPages.clear();

// 	// Resize the octree, allow for 1 page for now
// 	float max_x = mOptions.scale.x * mOptions.pageSize;
// 	float max_y = mOptions.scale.y;
// 	float max_z = mOptions.scale.z * mOptions.pageSize;
// 	resize( Ogre::AxisAlignedBox( 0, 0, 0, max_x, max_y, max_z ) );

//	setupTerrainMaterial();
	Ogre::TerrainPageSourceOptionList optlist;
	selectPageSource(EmberTerrainPageSource::Name, optlist);
	setupTerrainPages();

}


void EmberTerrainSceneManager::attachPage(Ogre::ushort pageX, Ogre::ushort pageZ, Ogre::TerrainPage* page, float maxY, float minY)
{
	//real size of one page
	Ogre::Real pageSize = mOptions.pageSize - 1;
  //  assert(pageX == 0 && pageZ == 0 && "Multiple pages not yet supported");
	Ogre::ushort adjustedX = pageX + mPageOffset;
	Ogre::ushort adjustedZ = pageZ + mPageOffset;
	short ogreX = (short) pageX;
	short ogreZ = (short) pageZ;
	

    assert(mTerrainPages[adjustedX][adjustedZ] == 0 && "Page at that index not yet expired!");
    // Insert page into list
    mTerrainPages[adjustedX][adjustedZ] = page;
    // Attach page to terrain root
    page->translate(Ogre::Vector3(ogreX * pageSize, 0, ogreZ * pageSize));
    setupPageNeighbors(adjustedX, adjustedZ, page);
    mTerrainRoot->addChild(page->pageSceneNode);

	mMinX = std::min(mMinX, ogreX * pageSize);
	mMaxX = std::max(mMaxX, (ogreX * pageSize) + pageSize);
	mMinY = std::min(mMinY, minY);
	mMaxY = std::max(mMaxY, maxY);
	mMinZ = std::min(mMinZ, ogreZ * pageSize);
	mMaxZ = std::max(mMaxZ, (ogreZ * pageSize) + pageSize);
}

void EmberTerrainSceneManager::setupPageNeighbors(Ogre::ushort pageX, Ogre::ushort pageZ, Ogre::TerrainPage* page) 
{

//begin with the northern end
	if (pageZ - 1 >= 0) {
		if (mTerrainPages[pageX][pageZ - 1]) {
			Ogre::TerrainPage* northPage = mTerrainPages[pageX][pageZ - 1];
			for ( size_t i = 0; i < page->tilesPerPage; i++ ) {
           		page->tiles[i][0]->_setNeighbor( Ogre::TerrainRenderable::NORTH, northPage->tiles[ i ][ northPage->tilesPerPage - 1 ] );
           		northPage->tiles[i][northPage->tilesPerPage - 1]->_setNeighbor( Ogre::TerrainRenderable::SOUTH, page->tiles[ i ][ 0 ] );
			}
           
 		}
	}
	
//southern end
	if (pageZ + 1 < mBufferedPageMargin) {
		if (mTerrainPages[pageX][pageZ + 1]) {
			Ogre::TerrainPage* northPage = mTerrainPages[pageX][pageZ + 1];
			for ( size_t i = 0; i < page->tilesPerPage; i++ ) {
           		northPage->tiles[i][0]->_setNeighbor( Ogre::TerrainRenderable::NORTH, page->tiles[ i ][ page->tilesPerPage - 1 ] );
           		page->tiles[i][page->tilesPerPage - 1]->_setNeighbor( Ogre::TerrainRenderable::SOUTH, northPage->tiles[ i ][ 0 ] );
			}
           
 		}
	}
	
	
//west end
	if (pageX - 1 >= 0) {
		if (mTerrainPages[pageX - 1][pageZ]) {
			Ogre::TerrainPage* northPage = mTerrainPages[pageX - 1][pageZ];
			for ( size_t i = 0; i < page->tilesPerPage; i++ ) {
           		page->tiles[0][i]->_setNeighbor( Ogre::TerrainRenderable::WEST, northPage->tiles[ northPage->tilesPerPage - 1 ][ i ] );
           		northPage->tiles[northPage->tilesPerPage - 1][i]->_setNeighbor( Ogre::TerrainRenderable::EAST, page->tiles[ 0 ][ i ] );
			}
           
 		}
	}
		
	
//east end
	if (pageX + 1 < mBufferedPageMargin) {
		if (mTerrainPages[pageX + 1][pageZ]) {
			Ogre::TerrainPage* northPage = mTerrainPages[pageX + 1][pageZ];
			for ( size_t i = 0; i < page->tilesPerPage; i++ ) {
           		northPage->tiles[0][i]->_setNeighbor( Ogre::TerrainRenderable::WEST, page->tiles[ page->tilesPerPage - 1 ][ i ] );
           		page->tiles[page->tilesPerPage - 1][i]->_setNeighbor( Ogre::TerrainRenderable::EAST, northPage->tiles[ 0 ][ i ] );
			}
           
 		}
	}
		
}



Ogre::TerrainPage* EmberTerrainSceneManager::getTerrainPage( const Ogre::Vector3 & pt )
{
    if (mPagingEnabled)
    {
        int ix = (int)(floor(pt.x / mOptions.pageSize)) + mPageOffset;
    	int iz = (int)(floor(pt.z / mOptions.pageSize)) + mPageOffset;    
    	return mTerrainPages[ix][iz];
    }
    else
    {
        // Single page
        return mTerrainPages[0][0];
    }
}

void EmberTerrainSceneManager::doResize()
{
	resize( Ogre::AxisAlignedBox( mMinX, mMinY, mMinZ, mMaxX, mMaxY, mMaxZ ) );
	
}
}


