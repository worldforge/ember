/*
 * Copyright (c) 2013 Peter Szucs <peter.szucs.dev@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
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
	mReductionMethod(Ogre::LodLevel::VRM_PROPORTIONAL),
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
	mUseAutomaticLod(true),
	mType(LT_AUTOMATIC_VERTEX_REDUCTION),
	mStrategy(LS_DISTANCE)
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

LodDistance& LodDefinition::getLodDistance(Ogre::Real distVal)
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
	for (LodDistanceMap::iterator it = mManualLod.begin(); it != mManualLod.end(); it++) {
		out.push_back(it->first);
	}
	return out;
}

LodDistance& LodDefinition::createDistance(Ogre::Real distance)
{
	assert(mManualLod.find(distance) == mManualLod.end());
	return mManualLod[distance];
}

LodDefinitionPtr::LodDefinitionPtr(const Ogre::ResourcePtr& r) :
	Ogre::SharedPtr<LodDefinition>()
{
	// lock & copy other mutex pointer
	OGRE_MUTEX_CONDITIONAL(r.OGRE_AUTO_MUTEX_NAME)
	{
		OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME);
		OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME);
		pRep = static_cast<LodDefinition*>(r.getPointer());
		pUseCount = r.useCountPointer();
		if (pUseCount) {
			++(*pUseCount);
		}
	}
}

LodDefinitionPtr& LodDefinitionPtr::operator= (const Ogre::ResourcePtr& r)
{
	if (pRep == static_cast<LodDefinition*>(r.getPointer())) {
		return *this;
	}
	release();
	// lock & copy other mutex pointer
	OGRE_MUTEX_CONDITIONAL(r.OGRE_AUTO_MUTEX_NAME)
	{
		OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME);
		OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME);
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
