#include "SmartBodyManager.h"
#include "SmartBodyPathConsts.h"
#include "SmartBodyLocomotion.h"
#include "SmartBodySkeletonMap.h"

#include <sb/SBScene.h>
#include <sb/SBCharacter.h>
#include <sb/SBSkeleton.h>
#include <sb/SBAssetManager.h>
#include <sb/SBSimulationManager.h>
#include <sb/SBBmlProcessor.h>

#include <OgreSkeletonInstance.h>
#include <OgreEntity.h>
#include <OgreBone.h>

//#define NDEBUG
#include <cassert>


using namespace Ember;
namespace Ember
{


// mScene stays as a pointer and not a reference, because the call the SBScene::getScene() allocates it. Consequently, is has to be deleted at the end.
SmartBodyManager::SmartBodyManager(void) 
	: mScene(SmartBody::SBScene::getScene()), mAssetManager(*mScene->getAssetManager()), mSimulation(*mScene->getSimulationManager()),
	  mProcessor(*mScene->getBmlProcessor()), mIsInit(false), mLocomotion(nullptr)
{
}

SmartBodyManager::~SmartBodyManager(void)
{
	delete mLocomotion;
	delete mScene;
}


//public.
void SmartBodyManager::initialize(void)
{
	//Initialization of SmartBody library.
	//TODO : suppress ? I think this line do nothing X.X
	mScene->start();

	//Set the media path : where to find the assets.
	mScene->setMediaPath(EMBER_SMARTBODY_MEDIAPATH);
	//Add the different asset paths.
	addAssetPaths();

	//Load the different behaviors.
	loadAllBehaviors();

	//Set init boolean to true.
	mIsInit = true;
}

void SmartBodyManager::addAssetPaths(void) 
{
	//Character skeletons.
	mAssetManager.addAssetPath("motion", EMBER_SMARTBODY_ASSETS_SKELETONS);
}

void SmartBodyManager::loadAllBehaviors(void)
{
	//Locomotion behavior.
	mLocomotion = new SmartBodyLocomotion(EMBER_SMARTBODY_ASSETS_LOCOMOTION, EMBER_SMARTBODY_SKELETON_LOCOMOTION, *this);
	mLocomotion->setup();
}

void SmartBodyManager::retargetAllBehaviors(SmartBody::SBCharacter& character)
{
	//Retarget locomotion behavior set.
	mLocomotion->retarget(character);
}

//public.
bool SmartBodyManager::setCorrespondingSkeletonName(std::string& sbName, const std::string& ogreName)
{
	size_t lastSlash = ogreName.rfind('/');
	if (lastSlash + 1 >= ogreName.size())
		return false;

 	sbName = ogreName.substr(lastSlash + 1) + std::string(EMBER_SMARTBODY_SKELETON_EXTENSION);	
 	return true;
}

//public.
bool SmartBodyManager::hasSkeleton(const std::string& skName, bool load /*= false */)
{
	assert(mIsInit);

	if (mAssetManager.getSkeleton(skName))
	{
		return true;
	}

	else if (load)
	{
		//Try to load the skeleton.
		mAssetManager.loadAsset(EMBER_SMARTBODY_ASSETS_SKELETONS + skName);
		if (!mAssetManager.getSkeleton(skName))
		{
			return false;
		}

		//If it exists, then try to get the bone mapping.
		SmartBodySkeletonMap map(skName);
		if (map.exists())
		{
			//Finally, map the skeleton.
			map.setMap(*this);
		}

		return true;
	}

	return false;
}

//public.
SmartBody::SBCharacter* SmartBodyManager::createCharacter(const Ogre::Entity& entity, const std::string& group, const std::string& sbSkName)
{
	assert(mIsInit);

	//Create SB character.
	SmartBody::SBCharacter *character = mScene->createCharacter(entity.getName(), group);

	//Create SB skeleton.
	SmartBody::SBSkeleton *skeleton = mScene->createSkeleton(sbSkName);

	//Associate the skeleton to the character.
	character->setSkeleton(skeleton);

	//Set manually controlled mode on the Ogre skeleton.
	// TODO : verify that the manual control doesn't prevent the character from using classical animations 
	// edit : it seems that it makes the character convulse :X
	setManualControl(entity);

	//Add behaviors and controllers.
	character->createStandardControllers();
	retargetAllBehaviors(*character);

	return character;
}

//public.
void SmartBodyManager::removeCharacter(SmartBody::SBCharacter *character)
{
	mScene->removeCharacter(character->getName());
}

void SmartBodyManager::setManualControl(const Ogre::Entity& entity, bool mode)
{
	Ogre::SkeletonInstance *skeleton = entity.getSkeleton();
	Ogre::Skeleton::BoneIterator it = skeleton->getBoneIterator(); 

	while (it.hasMoreElements()) 
	{ 
		it.getNext()->setManuallyControlled(mode);
	}
}

//public.
SmartBody::SBScene& SmartBodyManager::getScene(void) const
{
	return *mScene;
}

//public.
SmartBody::SBAssetManager& SmartBodyManager::getAssetManager(void) const
{
	return mAssetManager;
}
	
}