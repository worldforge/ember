#include "Behaviors.h"
#include "Sb.h"
#include "Retarget.h"

/* TODO 

<($_$)> setupBehavior : 
			+ encapsulate the specifical stuff for locomotion 
			+ search the purpose of these buildJointTrajectory
<($_$)> retarget : 
			+ finish to translate Python to C++
			+ search what are IK constraints

*/


Behaviors::Behaviors(std::string const &motionPath /*= ""*/, std::string const &skeletonRef /*= ""*/)
	: mSkelRefName(skeletonRef), mMotionPath(motionPath)
{
	std::cout << "Behaviors :: Creation of a behavior set" << std::endl;

	/* Set the blend manager. */
	mBlendManager = SbManager::getSbScene()->getBlendManager();	
}

Behaviors::~Behaviors()
{

}


/* Set the motion path and the skeleton if they have not been defined. */
void Behaviors::setup(void)
{
	std::cout << "Behaviors::setup" << std::endl;

	/* Where to find the motion assets. */
	if (mMotionPath == "")
	{
		assignDefaultMotionPath();
	}

	/* Name of a skeleton which is bound to the behaviors. The user can specify it, namely if he changes the name of the file. */
	if (mSkelRefName == "")
	{
		assignSkeletonReference();
	}

	/* Before setting up the behaviors, check that there existence. */
	mLoad = checkAssetExistence();
	if (mLoad)
	{		
		setupBehaviors();
	}
}


/* Set up the different motions corresponding to the behavior set. */
void Behaviors::setupBehaviors(void)
{
	std::cout << "Behaviors::setupBehaviors: Setting up the behaviors" << std::endl;

	/* We get the motion paths for the behaviors. */
	std::vector<std::string> motions = getMotions();

	/* For each motion, we need to set the reference of the skeleton and to configurate some of the motion parameters. */ 
	int size = motions.size();
	SmartBody::SBScene *scene = SbManager::getSbScene();
	SmartBody::SBAssetManager *assetManager = scene->getAssetManager();
	for (int i = 0; i < size; i ++)
	{
		SmartBody::SBMotion *motion = assetManager->getMotion(motions[i]);
		motion->setMotionSkeletonName(mSkelRefName);
		setupMotion(motion);
	}		
}

/* Get the name of the file containing the motion code. */
std::string Behaviors::getMotionAsset(std::string motionName)
{
	std::cout << "Behaviors::getMotionAsset" << std::endl;

	return mMotionPath + motionName + ".skm";
}

/* Format the path in order to create a valid asset path. */
std::string Behaviors::createDefaultMotionPath(std::string const &path)
{
	std::cout << "Behaviors::createDefaultMotionPath" << std::endl;

	std::string newPath;

	if (path == "")
	{
		newPath = ("./");
	}

	else if (path[path.size() - 1] != '/')
	{
		newPath = path + "/";
	}

	else
	{
		newPath = path;
	}

	return newPath;
}


/* Retarget the behavior set from the skeleton reference to another character. */
void Behaviors::retarget(std::string characterName)
{
	if (!mLoad)
	{
		std::cout << "The assets have not been loaded correctly. Try setup() again before retargeting." << std::endl;
		return;
	}

	/* We get the corresponding character and the name of its skeleton. */
	SmartBody::SBCharacter *character = SbManager::getSbScene()->getCharacter(characterName);
	if (!character)
		return;

	std::string skelName = character->getSkeleton()->getName();

	Retarget retarget(mSkelRefName, skelName);
	setupRetargeting(character);
}


/* Check that all assets are well loaded. */
bool Behaviors::checkAssetExistence(void)
{
	SmartBody::SBAssetManager *assetManager = SbManager::getSbScene()->getAssetManager();

	if (!assetManager->getSkeleton(mSkelRefName))
	{
		std::cout << "Error : " + mSkelRefName + " does not exists." << std::endl;
		return false;
	}

	std::vector<std::string> motions = getMotions();
	for (int i = 0; i < motions.size(); i ++)
	{
		SmartBody::SBMotion *motion = assetManager->getMotion(motions[i]);
		
		if (!motion)
		{
			assetManager->loadAsset(getMotionAsset(motions[i]));
			motion = assetManager->getMotion(motions[i]);
		}

		if (!motion)
		{
			std::cout << "Error : " + motions[i] + " does not exists." << std::endl;
			return false;
		}
	}

	return true;		
}