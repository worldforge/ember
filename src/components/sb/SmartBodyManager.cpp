#include "SmartBodyManager.h"
#include "SmartBodyPathConsts.h"

template<> Ember::SmartBodyManager* Ember::Singleton<Ember::SmartBodyManager>::ms_Singleton = 0;

using namespace Ember;
namespace Ember
{

SmartBodyManager::SmartBodyManager()
	: mScene(nullptr), mAssetManager(nullptr), mSimulation(nullptr), mProcessor(nullptr), mIsInit(false)
{
	
}

SmartBodyManager::~SmartBodyManager()
{
	if (mIsInit)
	{
		delete mScene;
	}
}



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

	//Load the different assets.
	loadAllAssets();

	//In order to be able to animate the skeletons through SmartBody, we map them right after they are loaded.
	mapAllSkeletons();

	//Set init boolean to true.
	mIsInit = true;
}

void SmartBodyManager::loadAllAssets()
{
	//Character skeletons.
	mAssetManager->addAssetPath("motion", EMBER_SMARTBODY_ASSETS_SKELETONS);

	//Locomotion assets.
	mAssetManager->addAssetPath("motion", EMBER_SMARTBODY_ASSETS_LOCOMOTION);

	//Loading.
	mAssetManager->loadAssets();
}

void SmartBodyManager::mapAllSkeletons()
{
	std::vector<std::string> names = mAssetManager->getSkeletonNames();

	for (int i = 0; i < names.size(); i ++)
	{
		/* If the skeleton doesn't have .sk extension, then it must be mapped (.sk format are specifical to SmartBody, so Ember shouldn't use this 
		 * format for its characters). */
		std::string extension = names[i].substr(names[i].size() - 3, 3);
		if (extension != ".sk")
		{
			SmartBodySkeletonMap map(names[i]);

			//If the map exists, we then set it to the skeleton.
			if (map.exists())
			{
				map.setMap();
			}
		}
	}
}

	
}