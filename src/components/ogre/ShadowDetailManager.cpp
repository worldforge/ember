/*
 * Copyright (C) 2012 Arjun Kumar <arjun1991@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "ShadowDetailManager.h"

#include "GraphicalChangeAdapter.h"
#include <services/config/ConfigListenerContainer.h>

#include "OgreSceneManager.h"
#include <OgreHardwarePixelBuffer.h>
#include <OgreRenderTexture.h>
#include <OgreViewport.h>
#include <OgreCamera.h>

namespace Ember {
namespace OgreView {

ShadowDetailManager::ShadowDetailManager(GraphicalChangeAdapter& graphicalChangeAdapter, Ogre::SceneManager& sceneManager) :
        mShadowFarDistance(sceneManager.getShadowFarDistance()),
        mShadowCameraLodThreshold(3.0f),
        mShadowDistanceThreshold(3.0f),
        mMaxShadowFarDistance(1000.0f),
        mMinShadowFarDistance(0.0f),
        mDefaultShadowDistanceStep(250),
        mShadowCameraLodBias(1.0f),
        mMaxShadowCameraLodBias(1.0f),
        mMinShadowCameraLodBias(0.1f),
        mDefaultShadowLodStep(0.3f),
        mSceneManager(sceneManager),
        mConfigListenerContainer(std::make_unique<ConfigListenerContainer>()) {
    mChangeRequiredConnection = graphicalChangeAdapter.EventChangeRequired.connect(sigc::mem_fun(*this, &ShadowDetailManager::changeLevel));
    mConfigListenerContainer->registerConfigListener("graphics", "shadowlodbias", sigc::mem_fun(*this, &ShadowDetailManager::Config_ShadowLodBias));
}

ShadowDetailManager::~ShadowDetailManager() {
    if (mChangeRequiredConnection) {
        mChangeRequiredConnection.disconnect();
    }
}

bool ShadowDetailManager::setShadowCameraLodBias(float factor) {
    //do not allow shadow lod bias to be set to 0 or a negative value.
    if (factor <= 0.0f) {
        factor = 0.1f;
    }
    mShadowCameraLodBias = factor;

    size_t textureCount = mSceneManager.getShadowTextureConfigList().size();
    for (size_t i = 0; i < textureCount; ++i) {
        Ogre::TexturePtr texture = mSceneManager.getShadowTexture(i);
        unsigned short viewportCount = texture->getBuffer()->getRenderTarget()->getNumViewports();
        for (unsigned short j = 0; j < viewportCount; ++j) {
            texture->getBuffer()->getRenderTarget()->getViewport(j)->getCamera()->setLodBias(factor);
        }
    }
    return true;
}

bool ShadowDetailManager::setShadowFarDistance(float distance) {
    //do not allow the shadow distance to be set to a negative value.
    if (distance < 0.0f) {
        return false;
    } else {
        mShadowFarDistance = distance;
    }
    mSceneManager.setShadowFarDistance(distance);
    return true;
}

bool ShadowDetailManager::changeLevel(float level) {
    // If the fps change required is less than any threshold, the changeMade boolean will remain false, else it indicates if a step up or down was made.
    bool changeMade = false;

    if (std::abs(level) >= mShadowDistanceThreshold) {
        if (level > 0.0f) {
            changeMade |= stepDownShadowDistance(mDefaultShadowDistanceStep);
        } else {
            changeMade |= stepUpShadowDistance(mDefaultShadowDistanceStep);
        }
    } else if (std::abs(level) >= mShadowCameraLodThreshold) {
        if (level > 0.0f) {
            changeMade |= stepDownShadowCameraLodBias(mDefaultShadowLodStep);
        } else {
            changeMade |= stepUpShadowCameraLodBias(mDefaultShadowLodStep);
        }
    }
    return changeMade;
}

bool ShadowDetailManager::stepDownShadowDistance(float step) {
    if (mShadowFarDistance > step) { //step down only if existing shadow far distance is greater than step
        mShadowFarDistance -= step;
        return setShadowFarDistance(mShadowFarDistance);
    } else if (mShadowFarDistance < step && mShadowFarDistance > mMinShadowFarDistance) { //if there is still some positive distance left which is smaller than step, set it to the minimum distance
        mShadowFarDistance = mMinShadowFarDistance;
        return setShadowFarDistance(mShadowFarDistance);
    } else { //step down not possible
        return false;
    }
}

bool ShadowDetailManager::stepUpShadowDistance(float step) {
    if (mShadowFarDistance + step <= mMaxShadowFarDistance) { //step up only if the step doesn't cause shadow far distance to go over maximum shadow far distance.
        mShadowFarDistance += step;
        return setShadowFarDistance(mShadowFarDistance);
    } else if (mShadowFarDistance < mMaxShadowFarDistance) { //if the shadow far distance is still below maximum shadow far distance but a default step causes it to go over it.
        mShadowFarDistance = mMaxShadowFarDistance;
        return setShadowFarDistance(mShadowFarDistance);
    } else {
        return false; //step up not possible
    }
}

bool ShadowDetailManager::stepDownShadowCameraLodBias(float step) {
    if (mShadowCameraLodBias > step) { //step down only if existing shadow camera lod bias is greater than step
        mShadowCameraLodBias -= step;
        return setShadowCameraLodBias(mShadowCameraLodBias);
    } else if (mShadowCameraLodBias < step && mShadowCameraLodBias > mMinShadowCameraLodBias) { //if there is still some positive lod bias left which is smaller than step, set it to the minimum shadow lod
        mShadowCameraLodBias = mMinShadowCameraLodBias;
        return setShadowCameraLodBias(mShadowCameraLodBias);
    } else { //step down not possible
        return false;
    }
}

bool ShadowDetailManager::stepUpShadowCameraLodBias(float step) {
    if (mShadowCameraLodBias + step <= mMaxShadowCameraLodBias) { //step up only if the step doesn't cause shadow lod bias to go over maximum shadow far distance.
        mShadowCameraLodBias += step;
        return setShadowCameraLodBias(mShadowCameraLodBias);
    } else if (mShadowCameraLodBias < mMaxShadowCameraLodBias) { //if the shadow lod bias is still below maximum shadow lod bias but a default step causes it to go over it.
        mShadowCameraLodBias = mMaxShadowCameraLodBias;
        return setShadowCameraLodBias(mShadowCameraLodBias);
    } else {
        return false; //step up not possible
    }
}

void ShadowDetailManager::Config_ShadowLodBias(const std::string& section, const std::string& key, varconf::Variable& variable) {
    if (variable.is_double()) {
        auto lodBias = static_cast<double>(variable);
        setShadowCameraLodBias((float)lodBias / 100.0f);
    }
}

void ShadowDetailManager::pause() {
    mChangeRequiredConnection.block();
}

void ShadowDetailManager::unpause() {
    mChangeRequiredConnection.unblock();
}

}
}
