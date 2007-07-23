#include "CaelumSystem.h"

namespace caelum {

CaelumSystem::CaelumSystem (Ogre::Root *root, 
															Ogre::SceneManager *sceneMgr, 
															bool manageResGroup, 
															const Ogre::String &resGroupName, 
															bool createSkyDome, bool createSun, bool createStarfield) {
	LOG ("Initialising Caelum system...");
	mOgreRoot = root;
	mSceneMgr = sceneMgr;

	RESOURCE_GROUP_NAME = resGroupName;

	// Create resource group
	if (manageResGroup) {
		mManageResourceGroup = true;
		Ogre::ResourceGroupManager::getSingleton ().createResourceGroup (RESOURCE_GROUP_NAME);
		LOG (Ogre::String ("Created Caelum resource group (") + RESOURCE_GROUP_NAME + ")");
	}
	else
		mManageResourceGroup = false;

	// Set-up attributes
	mUpdateRate = 0;
	mTimeSinceLastUpdate = 0;

	mTotalDayTime = 86396.0; // 23h59m56s
	mTimeScale = 1.0f;
	mLocalTime = 0.0f;
	mRelativeTime = 0.0f;

	mManageFog = false;

	mSkyDome = 0;
	mSun = 0;
	mStarfield = 0;
	LOG ("System attributes set up.");

	// Create basic elements
	if (createSkyDome)
		this->createSkyDome ();
	if (createSun)
		this->createSun ();
	if (createStarfield)
		this->createStarfield ();

	// Auto-register itself as a frame listener
	mOgreRoot->addFrameListener (this);

	LOG ("DONE");
}

CaelumSystem::~CaelumSystem () {
	LOG ("Shutting down Caelum system...");
	// Remove itself as a frame listener
	mOgreRoot->removeFrameListener (this);

	// Unregister all the caelum listeners
	mListeners.clear ();

	// Destroy the elements
	destroySkyDome ();
	destroySun ();
	destroyStarfield ();

	// Remove resource group
	if (mManageResourceGroup) {
		Ogre::ResourceGroupManager::getSingleton ().destroyResourceGroup (RESOURCE_GROUP_NAME);
		LOG ("Destroyed Caelum resource group");
	}

	LOG ("DONE");
}

void CaelumSystem::addListener (CaelumListener *listener) {
	mListeners.insert (listener);
}

void CaelumSystem::removeListener (CaelumListener *listener) {
	mListeners.erase (listener);
}

void CaelumSystem::preViewportUpdate (const Ogre::RenderTargetViewportEvent &e) {
	Ogre::Camera *cam = e.source->getCamera ();
	
	if (mSkyDome) {
		mSkyDome->notifyCameraChanged (cam);
	}

	if (mSun) {
		mSun->notifyCameraChanged (cam);
	}

	if (mStarfield) {
		mStarfield->notifyCameraChanged (cam);
	}
}

void CaelumSystem::setUpdateRate (float rate) {
	if (rate < 0)
		rate = 0;

	mUpdateRate = rate;

	// Force an update when changing this
	forceUpdate ();
}

float CaelumSystem::getUpdateRate () {
	return mUpdateRate;
}

void CaelumSystem::setTimeScale (const float scale) {
	mTimeScale = scale;
}

float CaelumSystem::getTimeScale () const {
	return mTimeScale;
}

void CaelumSystem::setLocalTime (const float time) {
	mLocalTime = time;

	// Force an update when changing this
	forceUpdate ();
}

float CaelumSystem::getLocalTime () const {
	return mLocalTime;
}

void CaelumSystem::setTotalDayTime (const float time) {
	mTotalDayTime = time;

	if (mTotalDayTime <= 0)
		mTotalDayTime = 86396;	// 23h59m56s

	// Force an update when changing this
	forceUpdate ();
}

float CaelumSystem::getTotalDayTime () const {
	return mTotalDayTime;
}

bool CaelumSystem::frameStarted (const Ogre::FrameEvent &e) {
	// Update local time
	mLocalTime += e.timeSinceLastFrame * mTimeScale;
	if (mLocalTime > mTotalDayTime) {
		clampLocalTime ();
	}

	// Update only if required
	mTimeSinceLastUpdate += (e.timeSinceLastFrame * Ogre::Math::Abs (mTimeScale)) / mTotalDayTime;
	if (mTimeSinceLastUpdate > mUpdateRate) {
		// Do it this way so we don't lose any seconds and the updates doesn't queue
		if (mUpdateRate != 0)
			mTimeSinceLastUpdate -= mUpdateRate * Ogre::Math::Floor (mTimeSinceLastUpdate / mUpdateRate);
		else
			mTimeSinceLastUpdate = 0;

		mRelativeTime = mLocalTime / mTotalDayTime;

		// Call every listener before doing anything
		if (!fireStartedEvent (e))
			return false;

		if (mSun) {
			mSun->update (mRelativeTime);
		}

		if (mStarfield) {
			mStarfield->update (mRelativeTime);
		}

		if (mSkyDome) {
			mSkyDome->updateSkyDomeMaterialTime (mSkyColourModel, mRelativeTime, mSun);

			mSkyDome->setSunDirection (mSun->getSunDirection ());
		}

		if (mManageFog) {
			// TODO: Fog stuff here!!!
			if (mSkyColourModel) {
				mSceneMgr->setFog( Ogre::FOG_LINEAR, mSkyColourModel->getFogColour (mRelativeTime, mSun ? mSun->getSunDirection() : Ogre::Vector3::UNIT_Y) * 0.7, .001, 10, 600);
/*				mSceneMgr->setFog (Ogre::FOG_EXP2,
													mSkyColourModel->getFogColour (mRelativeTime, mSun ? mSun->getSunDirection() : Ogre::Vector3::UNIT_Y) * 0.7,
													mSkyColourModel->getFogDensity (mRelativeTime, mSun ? mSun->getSunDirection() : Ogre::Vector3::UNIT_Y));*/
			}
		}

		if (mSun) {
			if (mSkyColourModel)
				mSun->setSunColour (mSkyColourModel->getSunColour (mRelativeTime, mSun->getSunDirection ()));
			else
				mSun->setSunColour (mSceneMgr->getFogColour ());
		}

		// Call every listener before quiting
		if (!fireFinishedEvent (e))
			return false;
	}

	return true;
}

SkyDome *CaelumSystem::createSkyDome () {
	if (!mSkyDome) {
		mSkyDome = new SkyDome (mSceneMgr);
		LOG ("Sky Dome created.");
	}

	return mSkyDome;
}

SkyDome *CaelumSystem::getSkyDome () const {
	return mSkyDome;
}

void CaelumSystem::destroySkyDome () {
	if (mSkyDome) {
		delete mSkyDome;
		mSkyDome = 0;
		LOG ("Sky Dome destroyed.");
	}
}

Sun *CaelumSystem::createSun () {
	if (!mSun) {
		mSun = new Sun (mSceneMgr);
		LOG ("Sun created.");
	}

	return mSun;
}

Sun *CaelumSystem::getSun () const {
	return mSun;
}

void CaelumSystem::destroySun () {
	if (mSun) {
		delete mSun;
		mSun = 0;
		LOG ("Sun destroyed.");
	}
}

Starfield *CaelumSystem::createStarfield (const Ogre::String &mapName) {
	if (!mStarfield) {
		mStarfield = new Starfield (mSceneMgr);
		LOG ("Starfield created.");
	}

	mStarfield->updateMaterial (mapName);

	return mStarfield;
}

Starfield *CaelumSystem::getStarfield () const {
	return mStarfield;
}

void CaelumSystem::destroyStarfield () {
	if (mStarfield) {
		delete mStarfield;
		mStarfield = 0;
		LOG ("Starfield destroyed.");
	}
}

void CaelumSystem::setSkyColourModel (SkyColourModel *model) {
	mSkyColourModel = model;
	if (mSkyDome) {
		Ogre::TextureUnitState *temp = mSkyDome->getTextureUnitState ();
		if (temp)
			mSkyColourModel->setSkyGradientsTextureUnitState (temp);
	}
}

void CaelumSystem::setManageFog (bool manage) {
	mManageFog = manage;
}

bool CaelumSystem::isFogManaged () const {
	return mManageFog;
}

bool CaelumSystem::fireStartedEvent (const Ogre::FrameEvent &e) {
	std::set<CaelumListener *>::iterator it, iend = mListeners.end ();
	bool flag = true;

	it = mListeners.begin ();

	while (it != iend && flag) {
		flag &= (*it)->caelumStarted (e, this);

		++it;
	}

	return flag;
}

bool CaelumSystem::fireFinishedEvent (const Ogre::FrameEvent &e) {
	std::set<CaelumListener *>::iterator it, iend = mListeners.end ();
	bool flag = true;

	it = mListeners.begin ();

	while (it != iend && flag) {
		flag &= (*it)->caelumFinished (e, this);

		++it;
	}

	return flag;
}

void CaelumSystem::clampLocalTime () {
	if (mLocalTime < 0) {
		int ratio = (int )(mTotalDayTime / -mLocalTime);
		mLocalTime += mTotalDayTime * ratio;
	}
	if (mLocalTime > mTotalDayTime) {
		int ratio = (int )(mTotalDayTime / mLocalTime);
		mLocalTime -= mTotalDayTime * ratio;
	}
}

void CaelumSystem::forceUpdate () {
	mTimeSinceLastUpdate = mUpdateRate + 1;
}

} // namespace caelum
