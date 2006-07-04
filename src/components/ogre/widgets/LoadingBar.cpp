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
#include "LoadingBar.h"

using namespace Ogre;
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
	This progress bar relies on you having the OgreCore.zip and EmberCore.zip package already 
	added to a resource group called 'Bootstrap' - this provides the basic 
	resources required for the progress bar and will be loaded automatically.
*/
	LoadingBar::LoadingBar() {}
	LoadingBar::~LoadingBar(){}

	/** Show the loading bar and start listening.
	@param window The window to update 
	@param numGroupsInit The number of groups you're going to be initialising
	@param numGroupsLoad The number of groups you're going to be loading
	@param initProportion The proportion of the progress which will be taken
		up by initialisation (ie script parsing etc). Defaults to 0.7 since
		script parsing can often take the majority of the time.
	*/
	void LoadingBar::start(RenderWindow* window, 
		unsigned short numGroupsInit, 
		unsigned short numGroupsLoad, 
		Real initProportion)
	{
		mWindow = window;
		mNumGroupsInit = numGroupsInit;
		mNumGroupsLoad = numGroupsLoad;
		mInitProportion = initProportion;
		// We need to pre-initialise the 'Bootstrap' group so we can use
		// the basic contents in the loading screen
		ResourceGroupManager::getSingleton().initialiseResourceGroup("Bootstrap");

		try {
			OverlayManager& omgr = OverlayManager::getSingleton();
			mLoadOverlay = (Overlay*)omgr.getByName("EmberCore/LoadOverlay");
			if (!mLoadOverlay)
			{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
				"Cannot find loading overlay", "LoadingBar::start");
			}
			mLoadOverlay->show();
	
			// Save links to the bar and to the loading text, for updates as we go
			mLoadingBarElement = omgr.getOverlayElement("EmberCore/LoadPanel/Bar/Progress");
			mLoadingCommentElement = omgr.getOverlayElement("EmberCore/LoadPanel/Comment");
			mLoadingDescriptionElement = omgr.getOverlayElement("EmberCore/LoadPanel/Description");
	
			OverlayElement* barContainer = omgr.getOverlayElement("EmberCore/LoadPanel/Bar");
			mProgressBarMaxSize = barContainer->getWidth();
	// 		mProgressBarMaxLeft = barContainer->getLeft();
			
			//mLoadingBarElement->setWidth(300);
	
			// self is listener
			ResourceGroupManager::getSingleton().addResourceGroupListener(this);
		} catch (const Ogre::Exception& ex) {
			S_LOG_FAILURE("Error when creating loading bar. Message: \n" << ex.getFullDescription());
		}


	}

	/** Hide the loading bar and stop listening. 
	*/
	void LoadingBar::finish(void)
	{
		if (mLoadOverlay) {
			/// hide loading screen
			mLoadOverlay->hide();
		}

		/// Unregister listener
		ResourceGroupManager::getSingleton().removeResourceGroupListener(this);
		
		///we won't be needing the bootstrap resources for some while, so unload them
		ResourceGroupManager::getSingleton().unloadResourceGroup("Bootstrap");

	}


	/// ResourceGroupListener callbacks
	void LoadingBar::resourceGroupScriptingStarted(const String& groupName, size_t scriptCount)
	{
		assert(mNumGroupsInit > 0 && "You stated you were not going to init "
			"any groups, but you did! Divide by zero would follow...");
		mProgressBarInc = mProgressBarMaxSize * mInitProportion / (Real)scriptCount;
		mProgressBarInc /= mNumGroupsInit;
		mLoadingDescriptionElement->setCaption("Parsing scripts...");
		mWindow->update();
	}
	void LoadingBar::scriptParseStarted(const String& scriptName)
	{
		mLoadingCommentElement->setCaption(scriptName);
		mWindow->update();
	}
	void LoadingBar::scriptParseEnded(void)
	{
		///make the black blocking block a little bit smaller and move it to the right
		mLoadingBarElement->setWidth(
			mLoadingBarElement->getWidth() - mProgressBarInc);
		mLoadingBarElement->setLeft(mLoadingBarElement->getLeft() + mProgressBarInc);
		mWindow->update();
	}
	void LoadingBar::resourceGroupScriptingEnded(const String& groupName)
	{
	}
	void LoadingBar::resourceGroupLoadStarted(const String& groupName, size_t resourceCount)
	{
		assert(mNumGroupsLoad > 0 && "You stated you were not going to load "
			"any groups, but you did! Divide by zero would follow...");
		mProgressBarInc = mProgressBarMaxSize * (1-mInitProportion) / 
			(Real)resourceCount;
		mProgressBarInc /= mNumGroupsLoad;
		mLoadingDescriptionElement->setCaption("Loading resources...");
		mWindow->update();
	}
	void LoadingBar::resourceLoadStarted(const ResourcePtr& resource)
	{
		mLoadingCommentElement->setCaption(resource->getName());
		mWindow->update();
	}
	void LoadingBar::resourceLoadEnded(void)
	{
		///make the black blocking block a little bit smaller and move it to the right
		mLoadingBarElement->setWidth(
			mLoadingBarElement->getWidth() - mProgressBarInc);
		mLoadingBarElement->setLeft(mLoadingBarElement->getLeft() + mProgressBarInc);
		mWindow->update();
	}
	void LoadingBar::worldGeometryStageStarted(const String& description)
	{
		mLoadingCommentElement->setCaption(description);
		mWindow->update();
	}
	void LoadingBar::worldGeometryStageEnded(void)
	{
		///make the black blocking block a little bit smaller and move it to the right
		mLoadingBarElement->setWidth(
			mLoadingBarElement->getWidth() - mProgressBarInc);
		mLoadingBarElement->setLeft(mLoadingBarElement->getLeft() + mProgressBarInc);
		mWindow->update();
	}
	void LoadingBar::resourceGroupLoadEnded(const String& groupName)
	{
	}
}

