#include "SmartBodyBehaviors.h"
#include "SmartBodyRetarget.h"
#include "SmartBodySkeletonMap.h"

#include <sb/SBAssetManager.h>
#include <sb/SBCharacter.h>
#include <sb/SBSkeleton.h>
#include <sb/SBScene.h>

//#define NDEBUG
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
		//Try to get the skeleton map.
		SmartBodySkeletonMap map(mSkelRefName);
		if (map.exists())
		{
			//Finally, map the skeleton.
			map.setMap(mAssetManager, jointMapMng);
		}

		return true;
	}

	return false;
}


void SmartBodyBehaviors::setupBehaviors(void)
{
	//We get the motion paths for this behavior set. 
	std::vector<std::string> motions = getMotions();

	//For each motion, we need to set the skeleton and to configurate some of the motion parameters. 
	int size = motions.size();
	for (int i = 0; i < size; i ++)
	{
		SmartBody::SBMotion *motion = mAssetManager.getMotion(motions[i]);
		motion->setMotionSkeletonName(mSkelRefName);
		setupMotion(*motion);
	}	
}

bool SmartBodyBehaviors::assetsExist(void)
{
	if (!mAssetManager.getSkeleton(mSkelRefName))
	{
		//std::cout << "Error : " + mSkelRefName + " does not exists." << std::endl;
		return false;
	}

	std::vector<std::string> motions = getMotions();
	for (int i = 0; i < motions.size(); i ++)
	{
		SmartBody::SBMotion *motion = mAssetManager.getMotion(motions[i]);

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