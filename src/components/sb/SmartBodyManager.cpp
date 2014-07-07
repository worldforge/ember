#include "SmartBodyManager.h"
#include "SmartBodyPathConsts.h"
#include "SmartBodyBehaviors.h"
#include "SmartBodyLocomotion.h"
#include "SmartBodySkeletonMap.h"
#include "SmartBodyRepresentation.h"
#include "SmartBodyAnimation.h"

#include "sb/SBScene.h"
#include "sb/SBSkeleton.h"
#include "sb/SBAssetManager.h"
#include "sb/SBSimulationManager.h"
#include "sb/SBBmlProcessor.h"

//#define NDEBUG
#include <cassert>


using namespace Ember;
namespace Ember
{


// mScene stays as a pointer and not a reference, because the call the SBScene::getScene() allocates it. Consequently, is has to be deleted at the end.
SmartBodyManager::SmartBodyManager(void) 
	: mScene(SmartBody::SBScene::getScene()), mAssetManager(*mScene->getAssetManager()), mSimulation(*mScene->getSimulationManager()),
	  mProcessor(*mScene->getBmlProcessor()), mIsInit(false)
{
	//Initialisation of the animation map.
	for (int i = 0; i < SmartBodyAnimation::ANIMATIONS_COUNT; i ++)
	{
		mAnimations.push_back(SmartBodyAnimation( ((SmartBodyAnimation::Name)i) ));
	}
}

SmartBodyManager::~SmartBodyManager(void)
{
	//Delete all characters.
	for (int i = 0, n = mCharacters.size(); i < n; i ++)
	{
		delete mCharacters[i];
	}

	//Delete all behaviors.
	for (int i = 0, n = mBehaviors.size(); i < n; i ++)
	{
		delete mBehaviors[i];
	}

	delete mScene;
}


//public.
void SmartBodyManager::initialize(double startTime)
{
	//Initialization of SmartBody library.
	mScene->start();

	//Set the media path : where to find the assets.
	mScene->setMediaPath(EMBER_SMARTBODY_MEDIAPATH);

	//Add the different asset paths.
	addAssetPaths();

	//Load the different behaviors.
	loadAllBehaviors();

	//Start the simulation.
	mSimulation.setTime(startTime);
	mSimulation.start();

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
	mBehaviors.push_back(new SmartBodyLocomotion(EMBER_SMARTBODY_ASSETS_LOCOMOTION, EMBER_SMARTBODY_SKELETON_LOCOMOTION, 
		mAssetManager, *mScene->getBlendManager(), *mScene->getRetargetManager()));
	
	//Setup all behaviors.
	for (int i = 0, n = mBehaviors.size(); i < n; i ++)
	{
		mBehaviors[i]->setup(true);
	}
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
			map.setMap(mAssetManager, *mScene->getJointMapManager());
		}

		//When the skeleton has been loaded, retarget the behaviors on it.
		for (int i = 0, n = mBehaviors.size(); i < n; i ++)
		{
			mBehaviors[i]->retarget(skName);
		}

		return true;
	}

	return false;
}

//public.
SmartBodyRepresentation* SmartBodyManager::createCharacter(const Ogre::Entity& entity, const std::string& group, const std::string& sbSkName)
{
	assert(mIsInit);

	//Create the representation.
	SmartBodyRepresentation *representation = new SmartBodyRepresentation(*mScene, entity, group, sbSkName, mBehaviors);

	//Add it to the character vector.
	mCharacters.push_back(representation);

	return representation;
}

//public.
void SmartBodyManager::removeCharacter(SmartBodyRepresentation *representation)
{
	assert(mIsInit);

	//Find the representation in mCharacters and remove it.
	for (int i = 0, n = mCharacters.size(); i < n; i ++)
	{
		if (mCharacters[i] == representation)
		{
			mCharacters.erase(mCharacters.begin() + i);
		}
	}

	delete representation;
}

//public.
void SmartBodyManager::updateAnimations(double timeSlice)
{
	assert(mIsInit);

	//Update the time of the simulation.
	mSimulation.setTime(mSimulation.getTime() + timeSlice);
	mScene->update();

	//For each character that is animated through SmartBody, set the new bone positions.
	for (int i = 0, n = mCharacters.size(); i < n; i ++)
	{
		if (mCharacters[i]->isAnimated())
			mCharacters[i]->updateBonePositions();
	}
}

//public.
void SmartBodyManager::animate(SmartBodyRepresentation& character, SmartBodyAnimation::Name animation)
{
	assert(mIsInit);

	character.setAnimatedState(true);
	mProcessor.execBML(character.getName(), mAnimations[animation].getBmlRequest());
}

//public.
void SmartBodyManager::freeze(SmartBodyRepresentation& character) const
{
	assert(mIsInit);

	character.setAnimatedState(false);
}
	
}