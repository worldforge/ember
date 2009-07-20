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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "LoadingBar.h"

#include "services/EmberServices.h"
#include "services/wfut/WfutService.h"
#include <OgreOverlay.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreRenderWindow.h>

using namespace Ogre;
namespace EmberOgre {
namespace Gui {

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
	LoadingBar::LoadingBar() :
	mProgress(0) ,
	mWindow(0),
	mLoadOverlay(0),
	mProgressBarScriptSize(0),
	mLoadingBarElement(0),
	mLoadingDescriptionElement(0),
	mLoadingCommentElement(0),
	mVersionElement(0)
	{}

	LoadingBar::~LoadingBar(){}

	/** Show the loading bar and start listening.
	@param window The window to update
	@param numGroupsInit The number of groups you're going to be initialising
	@param numGroupsLoad The number of groups you're going to be loading
	@param initProportion The proportion of the progress which will be taken
		up by initialisation (ie script parsing etc). Defaults to 0.7 since
		script parsing can often take the majority of the time.
	*/
	void LoadingBar::start(RenderWindow* window)
	{
		mWindow = window;
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
			mVersionElement = omgr.getOverlayElement("EmberCore/Splash/VersionInfo");

			//OverlayElement* barContainer = omgr.getOverlayElement("EmberCore/LoadPanel/Bar");
			mProgressBarMaxSize = mLoadingBarElement->getWidth();
	 		mProgressBarMaxLeft = mLoadingBarElement->getLeft();

			//mLoadingBarElement->setWidth(300);

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

		///we won't be needing the bootstrap resources for some while, so unload them
// 		ResourceGroupManager::getSingleton().unloadResourceGroup("Bootstrap");

	}




	void LoadingBar::addSection(LoadingBarSection* section)
	{
		mSections.push_back(section);
	}

	void LoadingBar::activateSection(LoadingBarSection* section)
	{
		SectionStore::iterator I = std::find(mSections.begin(), mSections.end(), section);
		if (I != mSections.end()) {
			mCurrentSection = I;
			SectionStore::iterator J = I;
			float totalSize = 0;
			for (;J != mSections.begin(); --J) {
				totalSize += (*J)->getSize();
			}
			setProgress(totalSize);
		}
	}

	void LoadingBar::setProgress(float progress)
	{
		if (mLoadingBarElement) {
			///make the black blocking block a little bit smaller and move it to the right
			mLoadingBarElement->setWidth(mProgressBarMaxSize * (1 - progress));
			mLoadingBarElement->setLeft(mProgressBarMaxLeft + (mProgressBarMaxSize * progress));
			updateRender();
		}
		mProgress = progress;
	}

	void LoadingBar::increase(float amount)
	{
		setProgress(mProgress + amount);
	}

	void LoadingBar::setCaption(const std::string& caption)
	{
		if (mLoadingCommentElement) {
			mLoadingCommentElement->setCaption(caption);
			updateRender();
		}
	}

	void LoadingBar::setVersionText(const std::string& versionText)
	{
		if (mVersionElement) {
			mVersionElement->setCaption(versionText);
			updateRender();
		}
	}

	void LoadingBar::updateRender()
	{
		try {
			mWindow->update();
		} catch (const std::exception& ex) {
			S_LOG_FAILURE("Error when updating render for loading bar." << ex);
		}
	}





	LoadingBarSection::LoadingBarSection(LoadingBar& loadingBar, float size, const std::string& name)
	: mSize(size), mLoadingBar(loadingBar), mAccumulatedSize(0), mName(name), mActive(false)
	{
	}
	LoadingBarSection::~LoadingBarSection()
	{
	}

	const std::string& LoadingBarSection::getName() const
	{
		return mName;
	}

	void LoadingBarSection::setCaption(const std::string& caption)
	{
		mLoadingBar.setCaption(caption);
	}

	float LoadingBarSection::getSize() const
	{
		return mSize;
	}

	void LoadingBarSection::tick(float tickSize)
	{
		if (mAccumulatedSize < 1.0) {
			mLoadingBar.increase(mSize * tickSize);
			mAccumulatedSize += tickSize;
		}
	}

	void LoadingBarSection::deactivate()
	{
		mActive = false;
	}























	ResourceGroupLoadingBarSection::ResourceGroupLoadingBarSection(LoadingBarSection& section,
		unsigned short numGroupsInit,
		unsigned short numGroupsLoad,
		Ogre::Real initProportion)
	: mInitProportion(initProportion), mNumGroupsInit(numGroupsInit), mNumGroupsLoad(numGroupsLoad), mSection(section), mProgressBarInc(0)
	{
		// self is listener
		ResourceGroupManager::getSingleton().addResourceGroupListener(this);
	}

	ResourceGroupLoadingBarSection::~ResourceGroupLoadingBarSection()
	{
		ResourceGroupManager::getSingleton().removeResourceGroupListener(this);
	}

	/// ResourceGroupListener callbacks
	void ResourceGroupLoadingBarSection::resourceGroupScriptingStarted(const String& groupName, size_t scriptCount)
	{
		if (mNumGroupsInit == 0) {
			return; ///avoid divide-by-zero
		}
		if (mNumGroupsLoad != 0) {
			mProgressBarInc = mInitProportion / mNumGroupsInit;
		} else {
			mProgressBarInc = 1.0 / mNumGroupsInit;
		}

		if (scriptCount == 0) {
			///no scripts will be loaded, so we'll have to conclude this group here and now
			mSection.tick(mProgressBarInc);
		}

		mProgressBarInc /= (Real)scriptCount;
		mSection.setCaption("Parsing scripts...");
	}
	void ResourceGroupLoadingBarSection::scriptParseStarted(const String& scriptName, bool& skipThisScript)
	{
		mSection.setCaption(scriptName);
	}
	void ResourceGroupLoadingBarSection::scriptParseEnded(const Ogre::String& scriptName, bool skipped)
	{
		///make the black blocking block a little bit smaller and move it to the right
		mSection.tick(mProgressBarInc);
	}
	void ResourceGroupLoadingBarSection::resourceGroupLoadStarted(const String& groupName, size_t resourceCount)
	{
		if (mNumGroupsLoad == 0) {
			return; ///avoid divide-by-zero
		}
		if (mNumGroupsInit) {
			mProgressBarInc = (1.0-mInitProportion) /
				mNumGroupsLoad;
		} else {
			mProgressBarInc = 1.0 / mNumGroupsLoad;
		}

		if (resourceCount == 0) {
			///no resources will be loaded, so we'll have to conclude this group here and now
			mSection.tick(mProgressBarInc);
		}

		mProgressBarInc /= (Real)resourceCount;
		mSection.setCaption("Loading resources...");
	}
	void ResourceGroupLoadingBarSection::resourceLoadStarted(const ResourcePtr& resource)
	{
		mSection.setCaption(resource->getName());
	}
	void ResourceGroupLoadingBarSection::resourceLoadEnded(void)
	{
		mSection.tick(mProgressBarInc);
	}
/*	void ResourceGroupLoadingBarSection::worldGeometryStageStarted(const String& description)
	{
		mSection.setCaption(description);
	}*/
	void ResourceGroupLoadingBarSection::resourceGroupLoadEnded(const String& groupName)
	{
	}
















	WfutLoadingBarSection::WfutLoadingBarSection(LoadingBarSection& section)
	: mSection(section), mNumberOfFilesToUpdate(0), mDownloadedSoFar(0)
	{
		Ember::WfutService* wfutSrv = Ember::EmberServices::getSingleton().getWfutService();
		wfutSrv->DownloadComplete.connect(sigc::mem_fun(*this, &WfutLoadingBarSection::wfutService_DownloadComplete));
		wfutSrv->DownloadFailed.connect(sigc::mem_fun(*this, &WfutLoadingBarSection::wfutService_DownloadFailed));
		wfutSrv->AllDownloadsComplete.connect(sigc::mem_fun(*this, &WfutLoadingBarSection::wfutService_AllDownloadsComplete));
		wfutSrv->DownloadingServerList.connect(sigc::mem_fun(*this, &WfutLoadingBarSection::wfutService_DownloadingServerList));
		wfutSrv->UpdatesCalculated.connect(sigc::mem_fun(*this, &WfutLoadingBarSection::wfutService_UpdatesCalculated));
	}
	WfutLoadingBarSection::~WfutLoadingBarSection()
	{}

	void WfutLoadingBarSection::wfutService_DownloadComplete(const std::string& url, const std::string& filename)
	{
		mDownloadedSoFar++;
		std::stringstream ss;
		ss << "Downloaded " << filename << " (" <<mDownloadedSoFar << " of "<< mNumberOfFilesToUpdate << ")";
		mSection.setCaption(ss.str());
		if (mNumberOfFilesToUpdate) {
			mSection.tick(1.0 / mNumberOfFilesToUpdate);
		}
	}

	void WfutLoadingBarSection::wfutService_DownloadFailed(const std::string& url, const std::string& filename, const std::string& reason)
	{
		mDownloadedSoFar++;
		std::stringstream ss;
		ss << "Failed to download " << filename << " (" << mDownloadedSoFar << " of " << mNumberOfFilesToUpdate << ")";
		mSection.setCaption(ss.str());
		if (mNumberOfFilesToUpdate) {
			mSection.tick(1.0 / mNumberOfFilesToUpdate);
		}
	}

	void WfutLoadingBarSection::wfutService_AllDownloadsComplete()
	{
	}

	void WfutLoadingBarSection::wfutService_DownloadingServerList(const std::string& url)
	{
		mSection.setCaption("Getting server list from " + url);
	}

	void WfutLoadingBarSection::wfutService_UpdatesCalculated(unsigned int numberOfFilesToUpdate)
	{
		mNumberOfFilesToUpdate = numberOfFilesToUpdate;
	}

}
}

