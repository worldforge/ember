//
// C++ Implementation: EmberPagingLandScapeData2D_HeightField
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2005
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//

#include "EmberOgrePrerequisites.h"
#include "OgrePagingLandScapeOptions.h"

#include "OgrePagingLandScapeData2DManager.h"

#include "EmberPagingLandScapeData2D_HeightField.h"
#include "EmberPagingSceneManager.h"

#include "EmberOgre.h"
#include "TerrainPage.h"
#include "TerrainGenerator.h"


using namespace Ogre;
namespace EmberOgre
{
	
EmberPagingLandScapeData2D_HeightField::EmberPagingLandScapeData2D_HeightField(Ogre::PagingLandScapeData2DManager *pageMgr) 
: Ogre::PagingLandScapeData2D(pageMgr), mTerrainPage(0)
{
	//set it to something, so it doesn't default to a crazy number (like 5.79555e+022) since that will break stuff later on
	//in regards to calculating the distance to the tile (especially in PagingLandScapeTile::_Notify)
	mMaxheight = 1;
}
	
bool EmberPagingLandScapeData2D_HeightField::_load( const Ogre::uint x, const Ogre::uint z )
{
/*	mMaxArrayPos = static_cast <uint> (mXDimension * mZDimension);
	mMax = static_cast <uint> (mMaxArrayPos * mBpp);
	mHeightData = new Real[mMaxArrayPos];*/
	assert(!mTerrainPage);
	TerrainGenerator* terrainGenerator = EmberOgre::getSingleton().getTerrainGenerator();
/*	if (!mTerrainPage) 
	{*/
		mMaxArrayPos = terrainGenerator->getPageSize() * terrainGenerator->getPageSize();
		mHeightData = new Real[mMaxArrayPos];
		mTerrainPage = terrainGenerator->getTerrainPage(Ogre::Vector2(x,z));
		//should always return a TerrainPage*
		assert(mTerrainPage);
		
		mTerrainPage->createHeightData(mHeightData);
	
		
		///make sure it's not 0
		mMaxheight = std::max<float>(mTerrainPage->getMaxHeight(), 1.0f);
		mMax = static_cast <unsigned int> (mSize * mTerrainPage->getMaxHeight());
		mXDimension = terrainGenerator->getPageSize();
		mZDimension = terrainGenerator->getPageSize();
		return true;
// 	}

	
// 	return true;
		   
}

PagingLandScapeData2D* EmberPagingLandScapeData2D_HeightField::newPage( )
{
	return new EmberPagingLandScapeData2D_HeightField(mParent);
}
	
	
const ColourValue EmberPagingLandScapeData2D_HeightField::getBase (const Real mX, const Real mZ)
{

	return ColourValue::White;

}

    //-----------------------------------------------------------------------
const ColourValue EmberPagingLandScapeData2D_HeightField::getCoverage (const Real mX, const Real mZ)
{

	return ColourValue::Blue;
}

    //-----------------------------------------------------------------------
const Real EmberPagingLandScapeData2D_HeightField::getShadow (const Real mX, const Real mZ,
			const bool &positive)
{

	return 0.0f;

}

    //-----------------------------------------------------------------------
// const Vector3 EmberPagingLandScapeData2D_HeightField::getNormal (const Real x, const Real z)
// {
// 	return 
// 	return Vector3::UNIT_Y;
// }
    //-----------------------------------------------------------------------
void EmberPagingLandScapeData2D_HeightField::_save()
{
	S_LOG_VERBOSE("Saving terrain page.");
}
    //-----------------------------------------------------------------------
    //-----------------------------------------------------------------------
void EmberPagingLandScapeData2D_HeightField::_load()
{
	S_LOG_VERBOSE("Loading terrain page.");
}
    //-----------------------------------------------------------------------
void EmberPagingLandScapeData2D_HeightField::_unload()
{
	S_LOG_VERBOSE("Unloading terrain page.");
	mTerrainPage = 0;
}

const Ogre::Real EmberPagingLandScapeData2D_HeightField::getMaxAbsoluteHeight(void) const
{
	return mMaxheight;
}



};
