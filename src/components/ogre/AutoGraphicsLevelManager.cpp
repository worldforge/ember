#include <components/ogre/AutoGraphicsLevelManager.h>

#include <services/config/ConfigListenerContainer.h>

#include <components/ogre/EmberOgre.h>
#include <framework/Time.h>

#include <Ogre.h>

#include <cmath>

namespace Ember
{
namespace OgreView
{

FpsUpdater::FpsUpdater(Ogre::RenderWindow& renderWindow) :
		mCurrentFps(0.0), mTimeAtLastUpdate(Time::currentTimeMillis()), mRenderWindow(renderWindow)
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
		mCurrentFps = mRenderWindow.getLastFPS();
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
	float translatedChangeRequired = changeSize / 1.0f;

	bool furtherChangePossible = changeRequired.emit(translatedChangeRequired);
	return furtherChangePossible;
}

AutomaticGraphicsLevelManager::AutomaticGraphicsLevelManager(Ogre::RenderWindow& renderWindow) :
		mEnabled(false), mDefaultFps(60), mFpsUpdater(renderWindow), mConfigListenerContainer(new ConfigListenerContainer())
{
	mFpsUpdater.fpsUpdated.connect(sigc::mem_fun(*this, &AutomaticGraphicsLevelManager::checkFps));
	mConfigListenerContainer->registerConfigListener("graphics", "desiredfps", sigc::mem_fun(*this, &AutomaticGraphicsLevelManager::Config_DefaultFps));
}

AutomaticGraphicsLevelManager::~AutomaticGraphicsLevelManager()
{
	delete mConfigListenerContainer;
}

void AutomaticGraphicsLevelManager::setFps(float fps)
{
	mDefaultFps = fps;
}

void AutomaticGraphicsLevelManager::checkFps(float currentFps)
{
	float changeRequired = mDefaultFps - currentFps;

	if (std::abs(changeRequired) >= 1.0f) {
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

void AutomaticGraphicsLevelManager::Config_DefaultFps(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	if (variable.is_double()) {
		float fps = static_cast<double>(variable);
		//If set to 0 the FPS the manager tries to achieve is 60
		if (fps == 0.0f) {
			fps = 60.0f;
		}
		setFps(fps);
	}
}

}
}
