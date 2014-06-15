#include "App.h"
#include "Bonhomme.h"

static App *application = NULL; 


App::App()
	: mRoot(0)
{
	mSbManager = new SbManager();
	application = this;
}

App::~App()
{
	delete mSbManager;
	delete mInputListener;
    delete mRoot;

    application = NULL;
}

bool App::start()
{
	/* Root creation. */
	mRoot = new Ogre::Root("plugins.cfg", "ogre.cfg", "Ogre.log");


	/* Ressource loading. */

	Ogre::ConfigFile configFile;
	configFile.load("resources.cfg");

	Ogre::ConfigFile::SectionIterator seci = configFile.getSectionIterator();

	Ogre::String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
		}
	}

	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	/* System configuration. */
	if(!(mRoot->restoreConfig() || mRoot->showConfigDialog()))
	{
		return false;
	}

	/* Window getting. */
	mWindow = mRoot->initialise(true, "Ogre test");

	/* FrameListener creation. */
	createFrameListener();

	/* SbManager initialization. */
	mSbManager->initialize(MEDIA_PATH);

	/* Scene creation. */
	mSceneMgr = mRoot->createSceneManager("DefaultSceneManager", "myscmng");
	mSceneMgr->setAmbientLight(Ogre::ColourValue(1.0f, 1.0f, 1.0f));
	createScene();

	/* Infinite loop. */
	bool loop;
	Ogre::FrameEvent event;
	mITime = Ogre::Root::getSingleton().getTimer()->getMilliseconds() / 1000.0f;
	mSbManager->startSimulation();

	while(true)
	{
		Ogre::WindowEventUtilities::messagePump();

//		bonhomme.updateAnimation("actn_chop_down_0", 0.001);

		bool loop = mInputListener->frameRenderingQueued(event);
		if (!loop)
			return true;

		mSbManager->updateSimulation();

		if(!mRoot->renderOneFrame())
			return false;
	}

	return true;
}


void App::createScene()
{
	/* Camera. */
	mCamera = mSceneMgr->createCamera("PlayerCam"); 
	mCamera->setPosition(Ogre::Vector3(0, 0, 20));
	mCamera->lookAt(Ogre::Vector3(0, 0, -300));
	mCamera->setNearClipDistance(5);

	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
	mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

	/* Bonhomme. */
	Bonhomme bonhomme("monBonhomme");
	bonhomme.addToScene();
//	bonhomme.animate("actn_chop_down_0");
}

void App::createFrameListener()
{
    mInputListener = new InputListener(mWindow, mCamera);
    mRoot->addFrameListener(mInputListener);
}

Ogre::Root* App::getRootSingleton()
{
	return application->mRoot;
}

Ogre::SceneManager* App::getSceneManagerSingleton()
{
	return application->mSceneMgr;
}

SbManager* App::getSbManagerSingleton()
{
	return application->mSbManager;
}