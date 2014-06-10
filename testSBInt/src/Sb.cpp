#include "Sb.h"


void SbManager::SbManager()
{

}


void SbManager::createCharacter(Bonhomme const &bonhomme)
{

}

void SbManager::initialize(std::string const &mediaPath)
{
	/* We get the scene */
	mScene = SmartBody::SBScene::getScene();
	mScene->startFileLogging("testOgre_sb.log");
	mScene->start();

	/* We give the media path */
	mScene->setMediaPath(mediaPath);

	mScene.addAssetPath('motion', '.');
	mScene.loadAssets();
}




	/* Wa create the character */
	mScene->start();

	// sets the media path, or root of all the data to be used
	// other paths will be relative to the media path
	// the file 'OgreSmartBody.py' needs to be placed in the media path directory
	mScene->runScript("ogresmartbody.py");

	mStartTime = Ogre::Root::getSingleton().getTimer()->getMilliseconds() / 1000.0f;
	mRoot->startRendering();


void SbManager::createCharacter()