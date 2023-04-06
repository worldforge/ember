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
#include "CEGUIUtils.h"
#include <sigc++/trackable.h>

#include <OgreResourceGroupManager.h>
#include <OgreTimer.h>
#include <components/cegui/CEGUISetup.h>

namespace Ember {
class MainLoopController;
namespace OgreView {
namespace Gui {

class LoadingBar;

class LoadingBarSection {
	friend class LoadingBar;

public:
	LoadingBarSection(LoadingBar& loadingBar, float size, std::string name);

	virtual ~LoadingBarSection() = default;

	float getSize() const;

	const std::string& getName() const;

	void tick(float tickSize);

	void setCaption(const std::string& caption);

	void setProgress(float progress);

private:
	void deactivate();

	float mSize;
	LoadingBar& mLoadingBar;
	float mAccumulatedSize;
	std::string mName;
	bool mActive;
};

class ResourceGroupLoadingBarSection : public Ogre::ResourceGroupListener {
public:
	explicit ResourceGroupLoadingBarSection(LoadingBarSection& section,
											size_t numGroupsInit = 1,
											size_t numGroupsLoad = 1,
											Ogre::Real initProportion = 0.70f);

	~ResourceGroupLoadingBarSection() override;

	// ResourceGroupListener callbacks
	void resourceGroupScriptingStarted(const Ogre::String& groupName, size_t scriptCount) override;

	void scriptParseStarted(const Ogre::String& scriptName, bool& skipThisScript) override;

	void scriptParseEnded(const Ogre::String& scriptName, bool skipped) override;

	void resourceGroupScriptingEnded(const Ogre::String& groupName) override;

	void resourceGroupLoadStarted(const Ogre::String& groupName, size_t resourceCount) override;

	void resourceLoadStarted(const Ogre::ResourcePtr& resource) override;

	void resourceLoadEnded() override;

	void resourceGroupLoadEnded(const Ogre::String& groupName) override;

private:
	Ogre::Real mInitProportion;
	size_t mNumGroupsInit;
	size_t mNumGroupsLoad;
	LoadingBarSection& mSection;
	Ogre::Real mProgressBarInc;
	size_t mCompletedSections;

};


/** Defines an example loading progress bar which you can use during
	startup, level changes etc to display loading progress.
*/
class LoadingBar {
protected:
	const Cegui::CEGUISetup& mGuiSetup;
	std::vector<LoadingBarSection*> mSections;
	float mProgress;
	Ogre::Real mProgressBarMaxSize, mProgressBarMaxLeft;

	Ogre::Real mProgressBarScriptSize;
	Gui::UniqueWindowPtr<CEGUI::Window> mSheet;
	CEGUI::Window* mVersionText;
	CEGUI::Window* mDescriptionText;
	CEGUI::Window* mProgressWindow;

	Ogre::Timer mTimer;
	MainLoopController& mMainLoopController;

public:
	/**
	 * @brief Ctor.
	 * @param window The window to update
	 * @param mainLoopController Allows the loading bar to see if the application should shut down.
	 */
	LoadingBar(const Cegui::CEGUISetup& ceguiSetup, MainLoopController& mainLoopController);

	virtual ~LoadingBar();

	void addSection(LoadingBarSection* section);

	void increase(float amount);

	void setProgress(float progress);

	void setCaption(const std::string& caption);

	void setVersionText(const std::string& versionText);

	/**
	 * @brief Updates the render of the loading bar.
	 *
	 * Be sure to call this after each change, else nothing will be updated to the user.
	 */
	void updateRender(bool forceUpdate = false);

};
}
}

}

#endif
