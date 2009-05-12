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

#include "../EmberOgrePrerequisites.h"
#include <sigc++/trackable.h>

#include <OgreResourceGroupManager.h>
namespace EmberOgre {
namespace Gui {

class LoadingBar;

class LoadingBarSection
{
friend class LoadingBar;
public:
	LoadingBarSection(LoadingBar& loadingBar, float size, const std::string& name);
	virtual ~LoadingBarSection();

	void activate(int steps);
	float getSize() const;
	const std::string& getName() const;
	void tick(float tickSize);
	void setCaption(const std::string& caption);

private:
	void deactivate();
	float mSize;
	LoadingBar& mLoadingBar;
	float mAccumulatedSize;
	std::string mName;
	bool mActive;
};

class WfutLoadingBarSection : public sigc::trackable
{
public:
	WfutLoadingBarSection(LoadingBarSection& section);
	virtual ~WfutLoadingBarSection();
private:

	void wfutService_DownloadComplete(const std::string& url, const std::string& filename);

	void wfutService_DownloadFailed(const std::string& url, const std::string& filename, const std::string& reason);

	void wfutService_AllDownloadsComplete();

	void wfutService_DownloadingServerList(const std::string& url);

	void wfutService_UpdatesCalculated(unsigned int numberOfFilesToUpdate);


	LoadingBarSection& mSection;
	unsigned int mNumberOfFilesToUpdate, mDownloadedSoFar;
};

class ResourceGroupLoadingBarSection : public Ogre::ResourceGroupListener
{
public:
	ResourceGroupLoadingBarSection(LoadingBarSection& section, unsigned short numGroupsInit = 1,
		unsigned short numGroupsLoad = 1,
		Ogre::Real initProportion = 0.70f);
	virtual ~ResourceGroupLoadingBarSection();

	// ResourceGroupListener callbacks
	void resourceGroupScriptingStarted(const Ogre::String & groupName, size_t scriptCount);
	void scriptParseStarted(const Ogre::String & scriptName, bool& skipThisScript);
	void scriptParseEnded(const Ogre::String& scriptName, bool skipped);
	void resourceGroupScriptingEnded(const Ogre::String & groupName) {}
	void resourceGroupLoadStarted(const Ogre::String & groupName, size_t resourceCount);
	void resourceLoadStarted(const Ogre::ResourcePtr& resource);
	void resourceLoadEnded(void);
	void worldGeometryStageStarted(const Ogre::String & description) {}
	void worldGeometryStageEnded(void) {}
	void resourceGroupLoadEnded(const Ogre::String & groupName);

private:
	Ogre::Real mInitProportion;
	unsigned short mNumGroupsInit;
	unsigned short mNumGroupsLoad;
	LoadingBarSection& mSection;
	Ogre::Real mProgressBarInc;

};


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
class LoadingBar
{
protected:
	typedef std::vector<LoadingBarSection*> SectionStore;
	SectionStore mSections;
	SectionStore::iterator mCurrentSection;
	float mProgress;
	Ogre::Real mProgressBarMaxSize, mProgressBarMaxLeft;

	Ogre::RenderWindow* mWindow;
	Ogre::Overlay* mLoadOverlay;
	Ogre::Real mProgressBarScriptSize;
	Ogre::OverlayElement* mLoadingBarElement;
	Ogre::OverlayElement* mLoadingDescriptionElement;
	Ogre::OverlayElement* mLoadingCommentElement;
	Ogre::OverlayElement* mVersionElement;

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
	virtual void start(Ogre::RenderWindow* window);

	/** Hide the loading bar and stop listening.
	*/
	virtual void finish(void);

	void addSection(LoadingBarSection* section);

	void activateSection(LoadingBarSection* section);
	void increase(float amount);
	void setProgress(float progress);
	void setDescription(const std::string& description);
	void setCaption(const std::string& caption);
	void setVersionText(const std::string& versionText);


};
}
}

#endif
