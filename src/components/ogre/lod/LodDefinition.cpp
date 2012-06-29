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
	mReductionMethod(Ogre::ProgressiveMesh::VRQ_PROPORTIONAL),
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

void LodDefinition::addLodDistance(int distVal, const LodDistance& distance)
{
	mManualLod.insert(std::make_pair(distVal, distance));
}

bool LodDefinition::hasLodDistance(int distVal) const
{
	return mManualLod.find(distVal) != mManualLod.end();
}

const LodDistance& LodDefinition::getLodDistance(int distVal) const
{
	std::map<int, LodDistance>::const_iterator it = mManualLod.find(distVal);
	assert(it != mManualLod.end());
	return it->second;
}

void LodDefinition::removeLodDistance(int distVal)
{
	std::map<int, LodDistance>::const_iterator it = mManualLod.find(distVal);
	assert(it != mManualLod.end());
	mManualLod.erase(it);
}


void LodDefinition::loadImpl(void)
{
	Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(mName, mGroup, true, this);
	const XMLLodDefinitionSerializer& serializer = LodDefinitionManager::getSingleton().getSerializer();
	serializer.importLodDefinition(stream, *this);
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

}
}
}
