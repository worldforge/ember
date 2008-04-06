/*
This file is part of Caelum.
See http://www.ogre3d.org/wiki/index.php/Caelum 

Copyright (c) 2006-2008 Caelum team. See Contributors.txt for details.

Caelum is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Caelum is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with Caelum. If not, see <http://www.gnu.org/licenses/>.
*/

#include "CaelumPrecompiled.h"
#include "CaelumSystem.h"

namespace caelum {

// Default resource group name is Caelum
Ogre::String RESOURCE_GROUP_NAME = "Caelum";
				
CaelumSystem::CaelumSystem
(
    Ogre::Root *root, 
	Ogre::SceneManager *sceneMgr, 
	CaelumComponent componentsToCreate/* = CAELUM_COMPONENTS_DEFAULT*/, 
	bool manageResGroup/* = true*/, 
	const Ogre::String &resGroupName/* = RESOURCE_GROUP_NAME*/
):
    mCleanup(false)
{
	LOG ("Initialising Caelum system...");
	mOgreRoot = root;
	mSceneMgr = sceneMgr;
	mCaelumRootNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("CaelumRoot");

	mCleanup = false;
	RESOURCE_GROUP_NAME = resGroupName;

	// Create resource group
	if (manageResGroup) {
		// Search for the resource group
		Ogre::StringVector resGroups = Ogre::ResourceGroupManager::getSingleton ().getResourceGroups ();
		Ogre::StringVector::iterator it = resGroups.begin (), iend = resGroups.end ();
        while (it != iend && *it != resGroupName) {
			++it;
        }

		if (it == iend) {
			Ogre::ResourceGroupManager::getSingleton ().createResourceGroup (RESOURCE_GROUP_NAME);
			mManageResourceGroup = true;
			LOG (Ogre::String ("Created Caelum resource group (") + RESOURCE_GROUP_NAME + ")");
		} else {
			mManageResourceGroup = false;
		}
	} else {
		mManageResourceGroup = false;
	}

	// Clock
	mUniversalClock = new UniversalClock ();

	// Set-up attributes
	mManageSceneFog = false;
	mGlobalFogDensityMultiplier = 1;
	mSceneFogDensityMultiplier = 1;
	mGroundFogDensityMultiplier = 1;

	LOG ("System attributes set up.");

	// Create default components; as requested.
    if (componentsToCreate & CAELUM_COMPONENT_SKY_COLOUR_MODEL) {
        this->setSkyColourModel (new SkyColourModel ());
    }
    if (componentsToCreate & CAELUM_COMPONENT_SKY_DOME) {
		this->setSkyDome (new SkyDome (mSceneMgr, mCaelumRootNode));
    }
    if (componentsToCreate & CAELUM_COMPONENT_SOLAR_SYSTEM_MODEL) {
        this->setSolarSystemModel (new SolarSystemModel ());
    }
    if (componentsToCreate & CAELUM_COMPONENT_SUN) {
		this->setSun (new SpriteSun (mSceneMgr, mCaelumRootNode));
    }
    if (componentsToCreate & CAELUM_COMPONENT_STARFIELD) {
    	try {
		this->setStarfield (new Starfield (mSceneMgr, mCaelumRootNode));
		} catch (const Ogre::Exception& ex) {}
    }
    if (componentsToCreate & CAELUM_COMPONENT_CLOUDS) {
    	try {
		this->setClouds (new LayeredClouds (mSceneMgr, mCaelumRootNode));
		} catch (const Ogre::Exception& ex) {}
    }
    if (componentsToCreate & CAELUM_COMPONENT_GROUND_FOG) {
    	try {
		this->setGroundFog (new GroundFog (mSceneMgr, mCaelumRootNode));
		} catch (const Ogre::Exception& ex) {}
    }

	LOG ("DONE");
}

CaelumSystem::~CaelumSystem () {
    destroySubcomponents();
	LOG ("Caelum system destroyed.");
}

void CaelumSystem::destroySubcomponents () {
	// Destroy the elements
	setSkyDome (0);
	setSun (0);
	setStarfield (0);
	setClouds (0);
    setGroundFog (0);
    setSkyColourModel (0);

	// Destroy the clock
	if (mUniversalClock) {
		delete mUniversalClock;
		mUniversalClock = 0;
	}

	// Remove resource group
	if (mManageResourceGroup) {
		Ogre::ResourceGroupManager::getSingleton ().destroyResourceGroup (RESOURCE_GROUP_NAME);
		LOG ("Destroyed Caelum resource group");
        mManageResourceGroup = false;
	}

    if (mCaelumRootNode) {
        static_cast<Ogre::SceneNode*>(mCaelumRootNode->getParent())->
                removeAndDestroyChild(mCaelumRootNode->getName());
        mCaelumRootNode = 0;
    }
}

void CaelumSystem::shutdown (const bool cleanup) {
	LOG ("Shutting down Caelum system...");

    destroySubcomponents();

    if (cleanup) {
        mOgreRoot->removeFrameListener (this);
		delete this;
    } else {
        // We'll delete later. Make sure we're registered as a frame listener, or we'd leak.
        mOgreRoot->addFrameListener(this);
		mCleanup = true;
    }
}

void CaelumSystem::preViewportUpdate (const Ogre::RenderTargetViewportEvent &e) {
	Ogre::Camera *cam = e.source->getCamera ();
	
	// Move root node.
	mCaelumRootNode->setPosition(cam->getRealPosition());

	if (getSkyDome ()) {
		getSkyDome ()->notifyCameraChanged (cam);
	}

	if (getSun ()) {
		getSun ()->notifyCameraChanged (cam);
	}

	if (getStarfield ()) {
		getStarfield ()->notifyCameraChanged (cam);
	}

	if (getClouds ()) {
		getClouds ()->notifyCameraChanged (cam);
	}

	if (getGroundFog ()) {
		getGroundFog ()->notifyCameraChanged (cam);
    }
}

UniversalClock *CaelumSystem::getUniversalClock () const {
	return mUniversalClock;
}
				
bool CaelumSystem::frameStarted (const Ogre::FrameEvent &e) {
	if (mCleanup) {
        // Delayed destruction.
        mOgreRoot->removeFrameListener (this);
		delete this;
		return true;
	}

    updateSubcomponents(e.timeSinceLastFrame);

    return true;
}

void CaelumSystem::updateSubcomponents (double timeSinceLastFrame) {
	if (!mUniversalClock->update (timeSinceLastFrame)) {
        return;
    }

    // Get current julian day.
    LongReal julDay = mUniversalClock->getJulianDay ();
    LongReal relDayTime = fmod(julDay, 1);

    // Get the sun's direction.
    Ogre::Vector3 sunDir;
    if (getSolarSystemModel ()) {
        sunDir = getSolarSystemModel ()->getSunDirection(julDay);
    } else {
        sunDir = Ogre::Vector3::UNIT_Y;
    }

    // Update starfield
    if (getStarfield ()) {
        getStarfield ()->update (relDayTime);
    }

    // Update skydome.
    if (getSkyDome ()) {
        getSkyDome ()->setSunDirection (sunDir);
    }

    // Init various properties from sky colour model.
    double fogDensity;
    Ogre::ColourValue fogColour;
    Ogre::ColourValue sunLightColour;
    Ogre::ColourValue sunSphereColour;
    if (getSkyColourModel ()) {
        fogDensity = getSkyColourModel ()->getFogDensity (relDayTime, sunDir);
        fogDensity *= mGlobalFogDensityMultiplier;
        fogColour = mSkyColourModel->getFogColour (relDayTime, sunDir);
        sunLightColour = getSkyColourModel ()->getSunLightColour (relDayTime, sunDir);
        sunSphereColour = getSkyColourModel ()->getSunSphereColour (relDayTime, sunDir);
    } else {
        fogDensity = 0;
        fogColour = Ogre::ColourValue::Black;
        sunLightColour = sunSphereColour = Ogre::ColourValue::White;
    }

    // Update scene fog.
    if (getManageSceneFog ()) {
        mSceneMgr->setFog (Ogre::FOG_EXP2,
                fogColour * 0.7,
                fogDensity * mSceneFogDensityMultiplier);
    }

    // Update ground fog.
    if (getGroundFog ()) {
        getGroundFog ()->setColour (fogColour);
        getGroundFog ()->setDensity (fogDensity * mGroundFogDensityMultiplier);
    }

    // Update sun
    if (getSun () && getSkyColourModel ()) {
        mSun->update (sunDir, sunLightColour, sunSphereColour);
    }

    // Update clouds
    if (getClouds()) {
        mClouds->update (mUniversalClock->getJulianSecondDifference(),
                sunDir, sunLightColour, fogColour);
    }
}

void CaelumSystem::setManageSceneFog (bool value) {
	mManageSceneFog = value;
    // Prevent having some stale values around.
    if (!value) {
        mSceneMgr->setFog (Ogre::FOG_NONE);
    }
}

bool CaelumSystem::getManageSceneFog () const {
	return mManageSceneFog;
}

void CaelumSystem::setSceneFogDensityMultiplier (double value) {
    mSceneFogDensityMultiplier = value;
}

double  CaelumSystem::getSceneFogDensityMultiplier () const {
    return mSceneFogDensityMultiplier;
}

void CaelumSystem::setGroundFogDensityMultiplier (double value) {
    mGroundFogDensityMultiplier = value;
}

double  CaelumSystem::getGroundFogDensityMultiplier () const {
    return mGroundFogDensityMultiplier;
}

void CaelumSystem::setGlobalFogDensityMultiplier (double value) {
    mGlobalFogDensityMultiplier = value;
}

double  CaelumSystem::getGlobalFogDensityMultiplier () const {
    return mGlobalFogDensityMultiplier;
}
} // namespace caelum
