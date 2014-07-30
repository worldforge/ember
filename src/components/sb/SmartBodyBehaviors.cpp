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

#include "SmartBodyBehaviors.h"
#include "SmartBodyRetarget.h"
#include "SmartBodySkeletonMap.h"

#include <sb/SBAssetManager.h>
#include <sb/SBCharacter.h>
#include <sb/SBSkeleton.h>
#include <sb/SBScene.h>

#include <cassert>


using namespace Ember;
namespace Ember
{

SmartBodyBehaviors::SmartBodyBehaviors(const std::string& motionPath, const std::string& skeletonRef, SmartBody::SBAssetManager& assetMng,
		SmartBody::SBAnimationBlendManager& blendMng, SmartBody::SBRetargetManager& retargetMng)
: 	mSkelRefName(skeletonRef), mAssetManager(assetMng), mBlendManager(blendMng), mRetargetManager(retargetMng), mSetup(false)
{
	//Loads the assets.
	mAssetManager.loadAssetsFromPath(motionPath);	
}

SmartBodyBehaviors::~SmartBodyBehaviors()
{

}


bool SmartBodyBehaviors::setupAssets(SmartBody::SBJointMapManager& jointMapMng)
{
	if (assetsExist())
	{
		std::vector<std::string> motions = getMotions();

		//Try to get the skeleton map.
		SmartBodySkeletonMap map(mSkelRefName, &motions);
		if (map.exists())
		{
			//Finally, map the skeleton and the motions.
			map.setMap(mAssetManager, jointMapMng);
		}

		return true;
	}

	return false;
}


void SmartBodyBehaviors::setupBehaviors()
{
	//We get the motion paths for this behavior set. 
	std::vector<std::string> motions = getMotions();

	//For each motion, we need to set the skeleton and to configurate some of the motion parameters. 
	for (auto& motionName : motions)
	{
		SmartBody::SBMotion *motion = mAssetManager.getMotion(motionName);
		motion->setMotionSkeletonName(mSkelRefName);
	}	
}

bool SmartBodyBehaviors::assetsExist()
{
	if (!mAssetManager.getSkeleton(mSkelRefName))
	{
		//std::cout << "Error : " + mSkelRefName + " does not exists." << std::endl;
		return false;
	}

	std::vector<std::string> motions = getMotions();
	for (auto& motionName : motions)
	{
		SmartBody::SBMotion *motion = mAssetManager.getMotion(motionName);

		if (!motion)
		{
			//std::cout << "Error : " + motions[i] + " does not exists." << std::endl;
			return false;
		}
	}

	return true;		
}

void SmartBodyBehaviors::retarget(const std::string& skName)
{
	//Cannot retarget if setup has not been called previously.
	assert(mSetup);

	//Creation of the retargeting object.
	SmartBodyRetarget retarget(mSkelRefName, skName, mRetargetManager);
}

}