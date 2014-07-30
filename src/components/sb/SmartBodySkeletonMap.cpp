/*
 Copyright (C) 2014 Céline NOËL <celine.noel.7294@gmail.com>

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

#include "SmartBodySkeletonMap.h"
#include "SmartBodyPathConsts.h"

#include <sb/SBSkeleton.h>
#include <sb/SBMotion.h>
#include <sb/SBAssetManager.h>
#include <sb/SBJointMapManager.h>
#include <sb/SBJointMap.h>
#include <sb/SBScene.h>


using namespace Ember;
namespace Ember
{

SmartBodySkeletonMap::SmartBodySkeletonMap(const std::string& skeleton, std::vector<std::string> const *motions /*= nullptr*/)
	: mName(skeleton), mMotions(motions)
{
	std::ifstream mapFile(std::string(EMBER_SMARTBODY_ASSETS_MAPS + skeleton + EMBER_SMARTBODY_MAP_EXTENSION).c_str(), std::ios::in);
	mHasMap = mapFile.is_open();

	if (mHasMap)
	{
		mHasMap = parseMapFile(mapFile);
		mapFile.close();
	}
}

SmartBodySkeletonMap::~SmartBodySkeletonMap()
{
}

bool SmartBodySkeletonMap::exists() const
{
	return mHasMap;
}


void SmartBodySkeletonMap::setMap(SmartBody::SBAssetManager& assetMng, SmartBody::SBJointMapManager& mapMng) const
{
	SmartBody::SBSkeleton *skeleton = assetMng.getSkeleton(mName);
	SmartBody::SBJointMap *jointMap = mapMng.createJointMap(mName);

	//Creation of the joint map.
	for (auto& bone : mBones)
	{
		jointMap->setMapping(bone.first, bone.second);
	}

	//Application of the map to the skeleton.
	jointMap->applySkeleton(skeleton);

	//And now to the motions.
	if (mMotions)
	{
		for (auto& motionName : *mMotions)
		{
			SmartBody::SBMotion *motion = assetMng.getMotion(motionName);
			jointMap->applyMotion(motion);
		}
	}
}

bool SmartBodySkeletonMap::parseMapFile(std::ifstream& mapFile)
{
	while (!mapFile.eof())
	{
		std::string line;
		mapFile >> line;

		//The delimiter between the two names is the arrow '->'.
		size_t delim = line.find("->");
		if (delim == 0 || delim + 1 > line.size() || delim == std::string::npos)
		{
			return false;
		}

		std::string ogreBone = line.substr(0, delim);
		std::string sbBone = line.substr(delim + 2, line.size() + 1);

		mBones[ogreBone] = sbBone;
	}

	return true;
}

}
