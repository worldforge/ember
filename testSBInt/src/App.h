#ifndef APP_H
#define APP_H

	#include <Ogre.h>
	#include "Input.h"

class App
{
public:
    App();
    ~App();

    bool start();
    void createScene();
    void createFrameListener();
    Ogre::Entity* createEntityWithSkeleton(Ogre::String const& name, Ogre::String const& meshFile, Ogre::String const& skeletonFile);
    void printEntityAnimations(Ogre::String const& name);

    static Ogre::Root* getRootSingleton();
    static Ogre::SceneManager* getSceneManagerSingleton();


private:
    Ogre::Root *mRoot;
    Ogre::RenderWindow *mWindow;
    Ogre::SceneManager *mSceneMgr;
    Ogre::Camera *mCamera;
    InputListener *mInputListener;
};

#endif