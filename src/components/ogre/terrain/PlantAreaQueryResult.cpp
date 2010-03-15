/*
 Copyright (C) 2010 Erik Hjortsberg <erik.hjortsberg@gmail.com>

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

#include "PlantAreaQueryResult.h"
#include "PlantAreaQuery.h"
#include "PlantInstance.h"
#include "Buffer.h"
#include <OgreColourValue.h>

namespace EmberOgre
{

namespace Terrain
{

PlantAreaQueryResult::PlantAreaQueryResult(const PlantAreaQuery& query) :
	mQuery(new PlantAreaQuery(query)), mShadow(0)
{

}

PlantAreaQueryResult::~PlantAreaQueryResult()
{
	delete mShadow;
	delete mQuery;
}

PlantAreaQueryResult::PlantStore& PlantAreaQueryResult::getStore()
{
	return mStore;
}

const PlantAreaQueryResult::PlantStore& PlantAreaQueryResult::getStore() const
{
	return mStore;
}

const PlantAreaQuery& PlantAreaQueryResult::getQuery() const
{
	return *mQuery;
}

PlantAreaQueryResult::ShadowBuffer* PlantAreaQueryResult::getShadow() const
{
	return mShadow;
}

void PlantAreaQueryResult::setShadow(PlantAreaQueryResult::ShadowBuffer* shadow)
{
	delete mShadow;
	mShadow = shadow;
}

bool PlantAreaQueryResult::hasShadow() const
{
	return mShadow != 0;
}
void PlantAreaQueryResult::getShadowColourAtWorldPosition(const Ogre::Vector2& position, Ogre::uint32& colour) const
{
	Ogre::uint8* aVal((Ogre::uint8*)&colour);
	if (mShadow) {
		//first translate world position to local coords
		Ogre::Vector2 localPos = position - Ogre::Vector2(mQuery->getArea().left, mQuery->getArea().bottom);
		if (localPos.x >= 0 && localPos.x < mQuery->getArea().width() && localPos.y >= 0 && localPos.y < mQuery->getArea().height()) {
			unsigned char val = mShadow->getData()[static_cast<size_t> ((localPos.y * mShadow->getResolution()) + localPos.x)];
			aVal[0] = aVal[1] = aVal[2] = val;
			aVal[3] = 0xFF;
			return;
		}
	}
	aVal[0] = aVal[1] = aVal[2] = aVal[3] = 0xFF;
}

void PlantAreaQueryResult::getShadowColourAtWorldPosition(const Ogre::Vector2& position, Ogre::ColourValue& colour) const
{
	if (mShadow) {
		//first translate world position to local coords
		Ogre::Vector2 localPos = position - Ogre::Vector2(mQuery->getArea().left, mQuery->getArea().bottom);
		if (localPos.x >= 0 && localPos.x < mQuery->getArea().width() && localPos.y >= 0 && localPos.y < mQuery->getArea().height()) {
			unsigned char val = mShadow->getData()[static_cast<size_t> ((localPos.y * mShadow->getResolution()) + localPos.x)];
			Ogre::uint8* aVal((Ogre::uint8*)&colour);
			colour.r = colour.g = colour.b = val;
			colour.a = 1.0f;
			return;
		}
	}
	colour.r = colour.g = colour.b = colour.a = 1.0f;
}

}

}
