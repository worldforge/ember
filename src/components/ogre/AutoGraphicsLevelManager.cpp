#include <components/ogre/AutoGraphicsLevelManager.h>

#include <components/ogre/EmberOgre.h>
#include <framework/Time.h>

#include <Ogre.h>

#include <cmath>

namespace Ember
{
namespace OgreView
{

FpsUpdater::FpsUpdater() :
		mCurrentFps(0.0), mTimeAtLastUpdate(Time::currentTimeMillis())
{
	Ogre::Root::getSingleton().addFrameListener(this);
}

FpsUpdater::~FpsUpdater()
{
	Ogre::Root::getSingleton().removeFrameListener(this);
}

bool FpsUpdater::frameStarted(const Ogre::FrameEvent& event)
{
	long currentTime = Time::currentTimeMillis();
	if (currentTime - mTimeAtLastUpdate >= 1500) {
		mTimeAtLastUpdate = currentTime;
		mCurrentFps = EmberOgre::getSingleton().getRenderWindow()->getLastFPS();
		fpsUpdated.emit(mCurrentFps);
	}
	return true;
}

float FpsUpdater::getCurrentFPS()
{
	return mCurrentFps;
}

IGraphicalChangeAdapter::IGraphicalChangeAdapter()
{
}

IGraphicalChangeAdapter::~IGraphicalChangeAdapter()
{
}

bool IGraphicalChangeAdapter::fpsChangeRequired(float changeSize)
{
	//for now leaving it at this, need to update later with better callibrated values
	float translatedChangeRequired = changeSize / 10;

	bool furtherChangePossible = changeRequired.emit(translatedChangeRequired);
	return furtherChangePossible;
}

AutomaticGraphicsLevelManager::AutomaticGraphicsLevelManager() :
		mEnabled(false), mDefaultFps(60)
{
	mFpsUpdater.fpsUpdated.connect(sigc::mem_fun(*this, &AutomaticGraphicsLevelManager::checkFps));
}

AutomaticGraphicsLevelManager::~AutomaticGraphicsLevelManager()
{

}

void AutomaticGraphicsLevelManager::checkFps(float currentFps)
{
	float changeRequired = mDefaultFps - currentFps;

	if (std::abs(changeRequired) >= 10) {
		changeGraphicsLevel(changeRequired);
	}
}

void AutomaticGraphicsLevelManager::changeGraphicsLevel(float changeInFpsRequired)
{
	//TODO: Need to implement functionality for if further change is not possible
	mGraphicalChangeAdapter.fpsChangeRequired(changeInFpsRequired);
}

IGraphicalChangeAdapter& AutomaticGraphicsLevelManager::getGraphicalAdapter()
{
	return mGraphicalChangeAdapter;
}

void AutomaticGraphicsLevelManager::setEnabled(bool newEnabled)
{
	mEnabled = newEnabled;
}

bool AutomaticGraphicsLevelManager::isEnabled()
{
	return mEnabled;
}

}
}
