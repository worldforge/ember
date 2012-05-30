
#include <components/ogre/AutoGraphicsLevelManager.h>

#include <Ogre.h>

#include <cmath>

namespace Ember {
namespace OgreView {

FpsUpdater::FpsUpdater() : 
	mCurrentFps(0.0)
{
	Ogre::Root::getSingleton().addFrameListener(this);
}

FpsUpdater::~FpsUpdater()
{
	Ogre::Root::getSingleton().removeFrameListener(this);
}

bool FpsUpdater::frameStarted(const Ogre::FrameEvent& event)
{
	mCurrentFps=Ogre::RenderTarget::getLastFPS();
	fpsUpdated.emit(mCurrentFps);
	return true;
}

float FpsUpdater::getCurrentFPS()
{
	return mCurrentFps;
}

template<typename T_iterator>
result_type FurtherChangePossibleAccumulater::operator()(T_iterator first, T_iterator last) const
{
	result_type endResult=false;
	for (; first != last; ++first)
	{
		endResult= endResult || first; 
	}
	return true;
}


bool IGraphicalChangeAdapter::fpsChangeRequired(float changeSize)
{
	//for now leaving it at this, need to update later with better callibrated values
	float translatedChangeRequired = changeSize/10;
	
	bool furtherChangePossible;
	furtherChangePossible = changeRequired.emit(translatedChangeRequired);
	return furtherChangePossible;
}

AutomaticGraphicsLevelManager::AutomaticGraphicsLevelManager() : 
	mEnabled(false)
{
	mFpsUpdater.fpsUpdated.connect(sigc::ptr_fun(checkFps));
}

AutomaticGraphicsLevelManager::~AutomaticGraphicsLevelManager()
{
	
}

void AutomaticGraphicsLevelManager::checkFps(float currentFps)
{
	float changeRequired;
	changeRequired = mDefaultFps - currentFps;
	
	if(abs(changeRequired) >= 10)
	{
			changeGraphicsLevel(changeRequired);
	}
}


void AutomaticGraphicsLevelManager::changeGraphicsLevel(float changeInFpsRequired)
{
	//TODO: Need to implement functionality for if further change is not possible
	mGraphicalChangeAdapter.fpsChangeRequired(changeInFpsRequired);
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