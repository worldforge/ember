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
	/* Création de l'objet Root */
	mRoot = new Ogre::Root("plugins.cfg", "ogre.cfg", "Ogre.log");


	/* Chargement des ressources */

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

	/* Configuration du système */
	if(!(mRoot->restoreConfig() || mRoot->showConfigDialog()))
	{
		return false;
	}

	/* Récupération de la fenêtre */
	mWindow = mRoot->initialise(true, "Une application de zér0");

	/* Création de la scène */
	mSceneMgr = mRoot->createSceneManager("DefaultSceneManager", "Mon Scene Manager");
	mSceneMgr->setAmbientLight(Ogre::ColourValue(1.0f, 1.0f, 1.0f));
	createScene();

	/* Création du FrameListener */
	createFrameListener();

	/* Lancement de la boucle infinie */
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
	/* Ajout d'une caméra */
	mCamera = mSceneMgr->createCamera("PlayerCam");
	mCamera->setPosition(Ogre::Vector3(0,1,3));
	mCamera->lookAt(Ogre::Vector3(0,1,-20));
	mCamera->setNearClipDistance(1);

	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
	mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

	/* Création d'un bonhomme */
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