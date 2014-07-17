#include "SmartBodyManager.h"
#include "SmartBodyPathConsts.h"
#include "SmartBodyBehaviors.h"
#include "SmartBodyLocomotion.h"
#include "SmartBodyGestures.h"
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

SmartBodyManager::SmartBodyManager() 
	: mScene(*SmartBody::SBScene::getScene()), mAssetManager(*mScene.getAssetManager()), mSimulation(*mScene.getSimulationManager()),
	  mProcessor(*mScene.getBmlProcessor()), mIsInit(false)
{
	//Initialisation of the animation map.
	for (int i = 0; i < SmartBodyAnimation::ANIMATIONS_COUNT; i ++)
	{
		mAnimations.push_back(SmartBodyAnimation( ((SmartBodyAnimation::Name)i) ));
	}
}

SmartBodyManager::~SmartBodyManager()
{
	mSimulation.stop();

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

	//Deleting the scene carries problems.
	//delete &mScene;
}


//public.
void SmartBodyManager::initialize(double startTime)
{
	//Initialization of SmartBody library.
	mScene.start();

	//Set the media path : where to find the assets.
	mScene.setMediaPath(EMBER_SMARTBODY_MEDIAPATH);

	//Load the different behaviors.
	loadAllBehaviors();

	//Load the skeleton assets at the beginning because it takes too much time to be done during the simulation.
	loadSkeletonAssets();

	//Start the simulation.
	mSimulation.setTime(startTime);
	mSimulation.start();

	//Set init boolean to true.
	mIsInit = true;
}

void SmartBodyManager::loadAllBehaviors()
{
	//Locomotion behavior.
	mBehaviors.push_back(new SmartBodyLocomotion(EMBER_SMARTBODY_ASSETS_LOCOMOTION, EMBER_SMARTBODY_SKELETON_LOCOMOTION, 
		mAssetManager, *mScene.getBlendManager(), *mScene.getRetargetManager()));

	//Gesture behavior.
	mBehaviors.push_back(new SmartBodyGestures(EMBER_SMARTBODY_ASSETS_GESTURES, EMBER_SMARTBODY_SKELETON_GESTURES, 
		mAssetManager, *mScene.getBlendManager(), *mScene.getRetargetManager(), *mScene.getGestureMapManager()));
	
	//Setup all behaviors.
	for (int i = 0, n = mBehaviors.size(); i < n; i ++)
	{
		mBehaviors[i]->setup(*mScene.getJointMapManager());
	}
}

void SmartBodyManager::loadSkeletonAssets() 
{
	//Load the Character skeletons.
	mAssetManager.loadAssetsFromPath(EMBER_SMARTBODY_ASSETS_SKELETONS);

	//For all the loaded assets, try to get the bone mapping.
	std::vector<std::string> skeletonNames = mAssetManager.getSkeletonNames();
	for (size_t sk = 0, nbSk = skeletonNames.size(); sk < nbSk; sk ++)
	{
		//It must concern an Ogre skeleton.
		if (skeletonNames[sk].substr(skeletonNames[sk].rfind('.'), 4) == ".xml")
		{
			//If it exists, then try to get the bone mapping.
			SmartBodySkeletonMap map(skeletonNames[sk]);
			if (map.exists())
			{
				//Finally, map the skeleton.
				map.setMap(mAssetManager, *mScene.getJointMapManager());
			}

			//When the skeleton has been loaded, retarget the behaviors on it.
			for (size_t bh = 0, nbBh = mBehaviors.size(); bh < nbBh; bh ++)
			{
				mBehaviors[bh]->retarget(skeletonNames[sk]);
			}
		}
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
bool SmartBodyManager::hasSkeleton(const std::string& skName)
{
	assert(mIsInit);

	if (mAssetManager.getSkeleton(skName))
	{
		return true;
	}

	return false;
}

//public.
SmartBodyRepresentation* SmartBodyManager::createCharacter(const Ogre::Entity& entity, const std::string& group, const std::string& sbSkName)
{
	assert(mIsInit);

	//Create the representation.
	SmartBodyRepresentation *representation = new SmartBodyRepresentation(mScene, entity, group, sbSkName, mBehaviors);

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
	mScene.update();

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

	mSimulation.pause();

	character.setAnimatedState(true);
	mProcessor.execBML(character.getName(), mAnimations[animation].getBmlRequest());

	mSimulation.resume();
}

//public.
void SmartBodyManager::freeze(SmartBodyRepresentation& character) const
{
	assert(mIsInit);

	character.setAnimatedState(false);
}
	
}