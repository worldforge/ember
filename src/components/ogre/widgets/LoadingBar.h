/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2005 The OGRE Team
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
LGPL like the rest of the engine.
-----------------------------------------------------------------------------
*/
/*
-----------------------------------------------------------------------------
Filename:    ExampleLoadingBar.h
Description: Defines an example loading progress bar which you can use during 
startup, level changes etc to display loading progress. 
IMPORTANT: Note that this progress bar relies on you having the OgreCore.zip 
package already added to a resource group called 'Bootstrap' - this provides
the basic resources required for the progress bar and will be loaded automatically.
*/

#ifndef EMBEROGRELOADERBAR_H
#define EMBEROGRELOADERBAR_H

#include <Ogre.h>

namespace EmberOgre {

/** Defines an example loading progress bar which you can use during 
	startup, level changes etc to display loading progress. 
@remarks
	Basically you just need to create an instance of this class, call start()
	before loading and finish() afterwards. You may also need to stop areas of
	your scene rendering in between since this method will call 
	RenderWindow::update() to update the display of the bar - we advise using
	SceneManager's 'special case render queues' for this, see
	SceneManager::addSpecialCaseRenderQueue for details.
@note 
	This progress bar relies on you having the OgreCore.zip package already 
	added to a resource group called 'Bootstrap' - this provides the basic 
	resources required for the progress bar and will be loaded automatically.
*/
class LoadingBar : public Ogre::ResourceGroupListener
{
protected:
	Ogre::RenderWindow* mWindow;
	Ogre::Overlay* mLoadOverlay;
	Ogre::Real mInitProportion;
	unsigned short mNumGroupsInit;
	unsigned short mNumGroupsLoad;
	Ogre::Real mProgressBarMaxSize;
	Ogre::Real mProgressBarScriptSize;
	Ogre::Real mProgressBarInc;
	Ogre::OverlayElement* mLoadingBarElement;
	Ogre::OverlayElement* mLoadingDescriptionElement;
	Ogre::OverlayElement* mLoadingCommentElement;

public:
	LoadingBar();
	virtual ~LoadingBar();

	/** Show the loading bar and start listening.
	@param window The window to update 
	@param numGroupsInit The number of groups you're going to be initialising
	@param numGroupsLoad The number of groups you're going to be loading
	@param initProportion The proportion of the progress which will be taken
		up by initialisation (ie script parsing etc). Defaults to 0.7 since
		script parsing can often take the majority of the time.
	*/
	virtual void start(Ogre::RenderWindow* window, 
		unsigned short numGroupsInit = 1, 
		unsigned short numGroupsLoad = 1, 
		Ogre::Real initProportion = 0.70f);

	/** Hide the loading bar and stop listening. 
	*/
	virtual void finish(void);


	// ResourceGroupListener callbacks
	void resourceGroupScriptingStarted(const Ogre::String & groupName, size_t scriptCount);
	void scriptParseStarted(const Ogre::String & scriptName);
	void scriptParseEnded(void);
	void resourceGroupScriptingEnded(const Ogre::String & groupName);
	void resourceGroupLoadStarted(const Ogre::String & groupName, size_t resourceCount);
	void resourceLoadStarted(const Ogre::ResourcePtr& resource);
	void resourceLoadEnded(void);
	void worldGeometryStageStarted(const Ogre::String & description);
	void worldGeometryStageEnded(void);
	void resourceGroupLoadEnded(const Ogre::String & groupName);

};

}

#endif
