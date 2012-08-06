/*
 * Copyright (C) 2012 Peter Szucs <peter.szucs.dev@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "LodDefinition.h"
#include "LodDefinitionManager.h"
#include "XMLLodDefinitionSerializer.h"

namespace Ember
{
namespace OgreView
{
namespace Lod
{

LodDistance::LodDistance() :
	mType(LDT_AUTOMATIC_VERTEX_REDUCTION),
	mReductionMethod(ProgressiveMeshGenerator::VRM_PROPORTIONAL),
	mReductionValue(0.5f)
{

}

LodDefinition::LodDefinition(Ogre::ResourceManager* creator,
                             const Ogre::String& name,
                             Ogre::ResourceHandle handle,
                             const Ogre::String& group,
                             bool isManual,
                             Ogre::ManualResourceLoader* loader) :
	Resource(creator, name, handle, group, isManual, loader),
	mUseAutomaticLod(true)
{
	createParamDictionary("LodDefinition");
}

void LodDefinition::addLodDistance(Ogre::Real distVal, const LodDistance& distance)
{
	mManualLod.insert(std::make_pair(distVal, distance));
}

bool LodDefinition::hasLodDistance(Ogre::Real distVal) const
{
	return mManualLod.find(distVal) != mManualLod.end();
}

LodDistance& LodDefinition::getLodDistance( Ogre::Real distVal )
{
	LodDistanceMap::iterator it = mManualLod.find(distVal);
	assert(it != mManualLod.end());
	return it->second;
}

void LodDefinition::removeLodDistance(Ogre::Real distVal)
{
	LodDistanceMap::iterator it = mManualLod.find(distVal);
	assert(it != mManualLod.end());
	mManualLod.erase(it);
}


void LodDefinition::loadImpl(void)
{

}

void LodDefinition::unloadImpl(void)
{

}

size_t LodDefinition::calculateSize(void) const
{
	return 0;
}

LodDefinition::~LodDefinition()
{
	unload();
}

std::vector<float> LodDefinition::createListOfDistances()
{
	std::vector<float> out;
	for(LodDistanceMapIterator it = mManualLod.begin();it != mManualLod.end();it++){
		out.push_back(it->first);
	}
	return out;
}

LodDistance& LodDefinition::createDistance( Ogre::Real distance )
{
	assert(mManualLod.find(distance) == mManualLod.end());
	return mManualLod[distance];
}

LodDefinitionPtr::LodDefinitionPtr(const Ogre::ResourcePtr& r) :
Ogre::SharedPtr<LodDefinition>()
{
	// lock & copy other mutex pointer
	OGRE_MUTEX_CONDITIONAL(r.OGRE_AUTO_MUTEX_NAME) {
		OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
			OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
			pRep = static_cast<LodDefinition*>(r.getPointer());
		pUseCount = r.useCountPointer();
		if (pUseCount) {
			++(*pUseCount);
		}
	}
}

LodDefinitionPtr& LodDefinitionPtr::operator=(const Ogre::ResourcePtr& r)
{
	if (pRep == static_cast<LodDefinition*>(r.getPointer()))
		return *this;
	release();
	// lock & copy other mutex pointer
	OGRE_MUTEX_CONDITIONAL(r.OGRE_AUTO_MUTEX_NAME) {
		OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
			OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
			pRep = static_cast<LodDefinition*>(r.getPointer());
		pUseCount = r.useCountPointer();
		if (pUseCount) {
			++(*pUseCount);
		}
	} else {
		// RHS must be a null pointer
		assert(r.isNull() && "RHS must be null if it has no mutex!");
		setNull();
	}
	return *this;
}

}
}
}
