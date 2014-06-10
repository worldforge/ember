#include "App.h"
#include "Bonhomme.h"

static App *application = NULL; 


App::App() : mRoot(0)
{
	application = this;
}

App::~App()
{
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

	/* Scene creation. */
	mSceneMgr = mRoot->createSceneManager("DefaultSceneManager", "myscmng");
	mSceneMgr->setAmbientLight(Ogre::ColourValue(1.0f, 1.0f, 1.0f));
	createScene();

	/* FrameListener creation. */
	createFrameListener();

	/* Infinite loop. */
	bool loop;
	Ogre::FrameEvent event;

	while(true)
	{
		Ogre::WindowEventUtilities::messagePump();
		Bonhomme bonhomme("monBonhomme");
		bonhomme.updateAnimation("trns_walk", 0.001);

		bool loop = mInputListener->frameRenderingQueued(event);
		if (!loop)
			return true;

		if(!mRoot->renderOneFrame())
			return false;
	}

	return true;
}


void App::createScene()
{
	/* Camera. */
	mCamera = mSceneMgr->createCamera("PlayerCam");
	mCamera->setPosition(Ogre::Vector3(0,1,3));
	mCamera->lookAt(Ogre::Vector3(0,1,-20));
	mCamera->setNearClipDistance(1);

	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
	mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

	/* Bonhomme. */
	Bonhomme bonhomme("monBonhomme");
	bonhomme.addToScene();
	bonhomme.animate("trns_walk");
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

SbManager* App::getSbManager()
{
	return application->mSbManager;
}