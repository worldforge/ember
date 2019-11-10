/*
 Copyright (C) 2010 Erik Ogenvik <erik@ogenvik.org>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "PlantAreaQueryResult.h"
#include "PlantAreaQuery.h"
#include "PlantInstance.h"
#include "Buffer.h"

namespace Ember {
namespace OgreView {

namespace Terrain {

PlantAreaQueryResult::PlantAreaQueryResult(PlantAreaQuery query) :
		mQuery(std::move(query)),
		mShadow(nullptr),
		mDefaultShadowColourLong(0) {
	setDefaultShadowColour(Ogre::ColourValue(1, 1, 1, 1));
}

PlantAreaQueryResult::~PlantAreaQueryResult() = default;

PlantAreaQueryResult::PlantStore& PlantAreaQueryResult::getStore() {
	return mStore;
}

const PlantAreaQueryResult::PlantStore& PlantAreaQueryResult::getStore() const {
	return mStore;
}

const PlantAreaQuery& PlantAreaQueryResult::getQuery() const {
	return mQuery;
}

PlantAreaQueryResult::ShadowBuffer* PlantAreaQueryResult::getShadow() const {
	return mShadow.get();
}

void PlantAreaQueryResult::setShadow(std::unique_ptr<PlantAreaQueryResult::ShadowBuffer> shadow) {
	mShadow = std::move(shadow);
}

bool PlantAreaQueryResult::hasShadow() const {
	return mShadow.get();
}

void PlantAreaQueryResult::getShadowColourAtWorldPosition(const Ogre::Vector2& position, Ogre::uint32& colour) const {
	if (mShadow) {
		Ogre::uint8* aVal = ((Ogre::uint8*) &colour);
		//first translate world position to local coords
		Ogre::Vector2 localPos = position - Ogre::Vector2(mQuery.mArea.left, mQuery.mArea.bottom);
		if (localPos.x >= 0 && localPos.x < mQuery.mArea.width() && localPos.y >= 0 && localPos.y < mQuery.mArea.height()) {
			unsigned char val = mShadow->getData()[static_cast<size_t> ((localPos.y * mShadow->getResolution()) + localPos.x)];
			aVal[0] = aVal[1] = aVal[2] = val;
			aVal[3] = 0xFF;
			return;
		}
	}
	colour = mDefaultShadowColourLong;
}

void PlantAreaQueryResult::getShadowColourAtWorldPosition(const Ogre::Vector2& position, Ogre::ColourValue& colour) const {
	if (mShadow) {
		//first translate world position to local coords
		Ogre::Vector2 localPos = position - Ogre::Vector2(mQuery.mArea.left, mQuery.mArea.bottom);
		if (localPos.x >= 0 && localPos.x < mQuery.mArea.width() && localPos.y >= 0 && localPos.y < mQuery.mArea.height()) {
			unsigned char val = mShadow->getValueAt(localPos.x, localPos.y, 0);
			colour.r = colour.g = colour.b = val;
			colour.a = 1.0f;
			return;
		}
	}
	colour = mDefaultShadowColourValue;
}

void PlantAreaQueryResult::setDefaultShadowColour(const Ogre::ColourValue& colour) {
	//Make sure to saturate so the values are clamped to [0..1]
	mDefaultShadowColourValue = colour.saturateCopy();
	mDefaultShadowColourLong = mDefaultShadowColourValue.getAsARGB();
}

}

}
}
