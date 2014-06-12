#ifndef APP_H
#define APP_H

	#include <Ogre.h>
    #include "Sb.h"
	#include "Input.h"
    
    #define MEDIA_PATH "media/"

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
    static SbManager* getSbManagerSingleton();


private:
    Ogre::Root *mRoot;
    Ogre::RenderWindow *mWindow;
    Ogre::SceneManager *mSceneMgr;
    Ogre::Camera *mCamera;
    InputListener *mInputListener;
    double mITime;
    SbManager *mSbManager;
};

#endif