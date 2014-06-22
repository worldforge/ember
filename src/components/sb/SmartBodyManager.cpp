#include "SmartBodyManager.h"
#include "SmartBodyPathConsts.h"

//#define NDEBUG
#include <cassert>


using namespace Ember;
namespace Ember
{

SmartBodyManager::SmartBodyManager()
	: mScene(nullptr), mAssetManager(nullptr), mSimulation(nullptr), mProcessor(nullptr), mIsInit(false), mLocomotion(nullptr)
{
	
}

SmartBodyManager::~SmartBodyManager()
{
	delete mLocomotion;
	delete mScene;
}


//public.
void SmartBodyManager::initialize()
{
	//Initialization of SmartBody library.
	mScene = SmartBody::getScene();
	mScene->start();

	//Set the member parameters.
	mAssetLoader = mScene->getAssetManager();
	mSimulation = mScene->getSimulationManager();
	mProcessor = mScene->getBmlProcessor();

	//Set the media path : where to find the assets.
	mScene->setMediaPath(EMBER_SMARTBODY_MEDIAPATH);

	//Add the different asset paths.
	addAssetPaths();

	//Load the different behaviors.
	loadAllBehaviors();

	//Set init boolean to true.
	mIsInit = true;
}

void SmartBodyManager::addAssetPaths()
{
	//Character skeletons.
	mAssetManager->addAssetPath("motion", EMBER_SMARTBODY_ASSETS_SKELETONS);
}

void SmartBodyManager::loadAllBehaviors()
{
	/* Should we load the behaviors at the beginning or when they are used for the first time... 
	I think it is better at the beginning because it can take time, and this is not good to have lagging during the simulation.
	*/

	//Locomotion behavior.
	mLocomotion = new SmartBodyLocomotion(EMBER_SMARTBODY_ASSETS_LOCOMOTION, EMBER_SMARTBODY_SKELETON_LOCOMOTION, this);
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
bool SmartBodyManager::hasSkeleton(const std::string& skName, bool load)
{
	assert(mIsInit);

	if (mAssetManager->getSkeleton(skName))
	{
		return true;
	}

	else if (load)
	{
		//Try to load the skeleton.
		mAssetManager->loadAsset(skName);
		if (!mAssetManager->getSkeleton(skName))
		{
			return false;
		}

		//If it exists, then try to get the bone mapping.
		SmartBodySkeletonMap map(skName);
		if (map.exists())
		{
			//Finally, map the skeleton.
			map.setMap();
		}

		return true;
	}

	return false;
}

//public.
void SmartBodyManager::createCharacter(const std::string& entityName, const std::string& group, const std::string& ogreSkName)
{
	assert(mIsInit);

	//Create SB character.
	SmartBody::SBCharacter *character = mScene->createCharacter(entityName, group);

	//Create SB skeleton.
	std::string sbSkName(ogreName.substr(ogreName.rfind('/') + 1) + std::string(EMBER_SMARTBODY_SKELETON_EXTENSION));
	SmartBody::SBSkeleton *skeleton = mScene->createSkeleton(sbSkName);

	//Associate the skeleton to the character.
	character->setSkeleton(skeleton);

	//Set manually controlled mode on the Ogre skeleton.
	// TODO : verify that the manual control doesn't prevent the character from using classical animations.
	setManualControl(entityName);

	//Add behaviors and controllers.
	character->createStandardControllers();
	retargetAllBehaviors(*character);
}

void SmartBodyManager::setManualControl(const std::string& entityName, bool mode)
{
	Ogre::SceneManager* sceneManager = _getManager()
	Ogre::Skeleton* skeleton = sceneManager->getEntity(entityName)->getSkeleton();
	Ogre::Skeleton::BoneIterator it = skeleton->getBoneIterator(); 

	while (it.hasMoreElements()) 
	{ 
		it.getNext()->setManuallyControlled(mode);
	}
}

//public.
SmartBody::SBScene* SmartBodyManager::getScene()
{
	return mScene;
}

//public.
SmartBody::SBAssetManager* SmartBodyManager::getAssetManager()
{
	return mAssetManager;
}
	
}