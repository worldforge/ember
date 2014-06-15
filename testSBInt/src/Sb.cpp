#include "Sb.h"
#include "App.h"
#include "Bonhomme.h"
#include <Ogre.h>


SbManager::SbManager()
	: mScene(0), mSimulation(0), mProcessor(0)
{

}

SbManager::~SbManager()
{
	if (mScene != 0)
	{
		delete mScene;
		mScene = 0;
	}
}


void SbManager::initialize(std::string const &mediaPath)
{
	Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing SmartBody ***");

	/* We get the scene */
	mScene = SmartBody::SBScene::getScene();
	mScene->startFileLogging("testOgre_sb.log");
	mScene->start();

	/* We give the media path */
	mScene->setMediaPath(mediaPath);

	/* We load the assets. */
	mScene->getAssetManager()->addAssetPath("motion", "references/locomotion");
	mScene->getAssetManager()->addAssetPath("motion", "sinbad");
	mScene->getAssetManager()->loadAssets();

	/* We get the simulation manager and the bml processor. */
	mSimulation = mScene->getSimulationManager();
	mProcessor = mScene->getBmlProcessor();

	Ogre::LogManager::getSingletonPtr()->logMessage("*** SmartBody initialized ***");
}

void SbManager::startSimulation()
{
	mSimulation->setTime(0.0);

	if (MOVE != MOVE_NONE)
	{
		std::string bml(std::string("<body posture=\"") + std::string(MOVE) + std::string("\"/>"));
		mProcessor->execBML("monBonhomme", bml);
	}
	
	mSimulation->start();
	mSimulation->resume();
}

void SbManager::updateSimulation()
{
	mScene->update();
	mSimulation->setTime(mSimulation->getTime() + 0.001);

	Bonhomme bonhomme("monBonhomme");
	bonhomme.updateBonePositions();
}

SmartBody::SBScene* SbManager::getScene(void)
{
	return mScene;
}


/* Static method that allows to get the SBScene singleton. */
SmartBody::SBScene* SbManager::getSbScene(void)
{
	return App::getSbManagerSingleton()->getScene();
}
