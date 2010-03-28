//
// C++ Implementation: EmberPagingLandScapeData2D_HeightField
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2005
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

#include "EmberPagingLandScapeData2D_HeightField.h"

#include "EmberOgrePrerequisites.h"
#include "OgrePagingLandScapeOptions.h"

#include "OgrePagingLandScapeData2DManager.h"
#include "OgrePagingLandScapePageManager.h"

#include "EmberPagingSceneManager.h"
#include "EmberTerrainPageBridge.h"

#include "Convert.h"
#include "terrain/TerrainPage.h"

//#include <OgreCodec.h>
//#include <OgreImage.h>
//#include <OgreImageCodec.h>
using namespace Ogre;

namespace EmberOgre
{

EmberPagingLandScapeData2D_HeightField::EmberPagingLandScapeData2D_HeightField(Ogre::PagingLandScapeData2DManager* pageMgr) :
	Ogre::PagingLandScapeData2D(pageMgr)
{
	///set it to something, so it doesn't default to a crazy number (like 5.79555e+022) since that will break stuff later on
	///in regards to calculating the distance to the tile (especially in PagingLandScapeTile::_Notify)
	mMaxheight = 1;
}

bool EmberPagingLandScapeData2D_HeightField::_load(unsigned int x, unsigned int z)
{
	// 	assert(!mTerrainPage);
	assert(!mHeightData);

	EmberPagingSceneManager* emberPagingSceneManager = static_cast<EmberPagingSceneManager*> (mParent->getSceneManager());
	IPageDataProvider* provider = emberPagingSceneManager->getProvider();
	if (provider) {
		mXDimension = mZDimension = provider->getPageIndexSize();

		mMaxArrayPos = mSize * mSize;
		mHeightData = new Ogre::Real[mMaxArrayPos];
		mHeightDataPtr = boost::shared_array<Ogre::Real>(mHeightData);
		float* heightDataPtr = mHeightData;
		for (size_t i = 0; i < mMaxArrayPos; ++i) {
			*(heightDataPtr++) = 0.0f;
		}

		EmberTerrainPageBridge* bridge = new EmberTerrainPageBridge(*emberPagingSceneManager->getData2DManager(), mHeightDataPtr, std::pair<unsigned int, unsigned int>(x, z));
		provider->setUpTerrainPageAtIndex(Ogre::Vector2(x, z), bridge);

		return true;
	}
	return false;
}

EmberPagingLandScapeData2D_HeightField::~EmberPagingLandScapeData2D_HeightField()
{

}
;

void EmberPagingLandScapeData2D_HeightField::setMaxHeight(float maxHeight)
{
	///make sure it's not 0
	mMaxheight = std::max<float>(maxHeight, 1.0f);
	mMax = static_cast<unsigned int> (mSize * maxHeight);
}

PagingLandScapeData2D* EmberPagingLandScapeData2D_HeightField::newPage()
{
	return new EmberPagingLandScapeData2D_HeightField(mParent);
}

Ogre::ColourValue EmberPagingLandScapeData2D_HeightField::getBase(const Ogre::Real& mX, const Ogre::Real& mZ) const
{
	return ColourValue::White;
}

Ogre::ColourValue EmberPagingLandScapeData2D_HeightField::getCoverage(const Ogre::Real& mX, const Ogre::Real& mZ) const
{
	return ColourValue::Blue;
}

Ogre::Real EmberPagingLandScapeData2D_HeightField::getShadow(const Ogre::Real& mX, const Ogre::Real& mZ, bool positive) const
{
	return 0.0f;
}

Ogre::Vector3 EmberPagingLandScapeData2D_HeightField::getNormal(const Ogre::Real& localPageX, const Ogre::Real& localPageZ) const
{

	return PagingLandScapeData2D::getNormal(localPageX, localPageZ);
//	///Use the bridge for quicker lookup.
//	if (mBridge) {
//		const Terrain::TerrainPage* terrainPage(mBridge->getTerrainPage());
//		if (terrainPage) {
//			WFMath::Vector<3> normal;
//			if (terrainPage->getNormal(TerrainPosition(localPageX, 512 - localPageZ), normal)) {
//				return Convert::toOgre(normal);
//			}
//		}
//	}
//	return Vector3::ZERO;
}

void EmberPagingLandScapeData2D_HeightField::_save()
{
	S_LOG_VERBOSE("Saving terrain page at x: " << mPageX << " z:" << mPageZ << ".");
}

void EmberPagingLandScapeData2D_HeightField::_load()
{
	S_LOG_VERBOSE("Loading (_load()) terrain page at x: " << mPageX << " z:" << mPageZ << ".");
}

void EmberPagingLandScapeData2D_HeightField::_unload()
{
	S_LOG_VERBOSE("Unloading terrain page at x: " << mPageX << " z:" << mPageZ << ".");
	EmberPagingSceneManager* emberPagingSceneManager = static_cast<EmberPagingSceneManager*> (mParent->getSceneManager());
	IPageDataProvider* provider = emberPagingSceneManager->getProvider();
	if (provider) {
		provider->removeBridge(Ogre::Vector2(mPageX, mPageZ));
	}
}

Ogre::Real EmberPagingLandScapeData2D_HeightField::getMaxAbsoluteHeight() const
{
	///return a totally arbitrary high enough value
	return 250.0f;
	//return mMaxheight;
}

void EmberPagingLandScapeData2D_HeightField::eventTerrainPageLoaded()
{
	S_LOG_VERBOSE("Terrain page at (" << mPageX << ", " << mPageZ << ") got TerrainPageLoaded event");

	// notify that terrain data has been loaded
	mParent->getSceneManager()->getPageManager()->getPage(mPageX, mPageZ, false)->eventData2DLoaded(true);
}

}
