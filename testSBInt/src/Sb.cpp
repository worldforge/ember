

void Sb::Sb(std::string const &mediaPath)
{
	mScene = SmartBody::SBScene::getScene();

	mScene->startFileLogging("smartbody.log");
	OgreSmartBodyListener* listener = new OgreSmartBodyListener(????);
	mScene->addSceneListener(listener);
	mScene->start();

	// sets the media path, or root of all the data to be used
	// other paths will be relative to the media path
	mScene->setMediaPath(mediaPath);
	mScene->addAssetPath("script", ".");
	// the file 'OgreSmartBody.py' needs to be placed in the media path directory
	mScene->runScript("ogresmartbody.py");

	mStartTime = Ogre::Root::getSingleton().getTimer()->getMilliseconds() / 1000.0f;
	mRoot->startRendering();
}