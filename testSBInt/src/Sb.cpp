#include "Sb.h"
#include <Ogre.h>


SbManager::SbManager()
	: mScene(0)
{

}

SbManager::~SbManager()
{
	if (mScene != 0)
	{
		delete mScene;
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
	mScene->addAssetPath("motion", ".");

	/* We load the assets. */
	mScene->loadAssets();

	Ogre::LogManager::getSingletonPtr()->logMessage("*** SmartBody initialized ***");
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
