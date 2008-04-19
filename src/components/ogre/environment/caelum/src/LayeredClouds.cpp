/*
This file is part of Caelum.
See http://www.ogre3d.org/wiki/index.php/Caelum 

Copyright (c) 2006-2007 Caelum team. See Contributors.txt for details.

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
#include "LayeredClouds.h"
#include "CaelumExceptions.h"
#include "ImageHelper.h"

namespace caelum
{
	/** Extend a Vector2 to a Vector3 by appending a 0
	 */
	inline Ogre::Vector3 extend(Ogre::Vector2 vec)
	{
		return Ogre::Vector3(vec.x, vec.y, 0);
	}
}

namespace caelum
{
	LayeredClouds::LayeredClouds
	(
			Ogre::SceneManager* scene,
			Ogre::SceneNode *caelumRootNode,
			const Ogre::String &resourceGroupName,
			const Ogre::String &materialName,
			const Ogre::String &meshName,
			const Ogre::String &entityName
	):
            mCloudCoverLookup(0),
			mSceneMgr(scene)
	{
		// Create cloud plane mesh if it doesn't exist.
		if (Ogre::MeshManager::getSingleton ().getByName (meshName).isNull ()) {
			Ogre::Plane plane = Ogre::Plane(Ogre::Vector3::NEGATIVE_UNIT_Y, -0.1);
			Ogre::MeshManager::getSingleton ().createCurvedPlane(
					meshName, resourceGroupName, plane, 2, 2, .4, 64, 64,
					false, 1, 1, 1, Ogre::Vector3::UNIT_Z);
		}

		mSceneMgr->getRenderQueue()->getQueueGroup(CAELUM_RENDER_QUEUE_CLOUDS)->setShadowsEnabled(false);

		// Create cloud plane entity.
		mEntity = mSceneMgr->createEntity(entityName, meshName);
        mEntity->setMaterialName(materialName);
        mEntity->setCastShadows(false);
		mEntity->setRenderQueueGroup (CAELUM_RENDER_QUEUE_CLOUDS);

		mNode = caelumRootNode->createChildSceneNode ();
		mNode->attachObject (mEntity);

		mMaterial = static_cast<Ogre::MaterialPtr>(
				Ogre::MaterialManager::getSingleton().getByName(materialName));
		mMaterial->load();
		if (mMaterial->getBestTechnique() == 0) {
			throw UnsupportedException (0, "Layered cloud material not supported.",
					"LayeredClouds", "LayeredClouds.cpp", -1);
		}
		mShadersEnabled = mMaterial->getBestTechnique()->getPass(0)->isProgrammable();
		
		if(mShadersEnabled) {
			getFpParams()->setIgnoreMissingParams(true);
			getVpParams()->setIgnoreMissingParams(true);
		}

		// Default parameter values
        assert(mCloudCoverLookup.get() == 0);
        setCloudCoverLookup("CloudCoverLookup.png");
		setCloudCover(0.5);
		setCloudMassOffset(Ogre::Vector2(0, 0));
		setCloudDetailOffset(Ogre::Vector2(0, 0));

		setAnimating(true);
		setCloudMassBlend(0.5);
		setCloudBlendTime(100);
		setCloudSpeed(Ogre::Vector2(0, 0));
	}

	LayeredClouds::~LayeredClouds() {
		mNode->detachObject(mEntity);
		mSceneMgr->destroyEntity(mEntity);
		mEntity = 0;
		static_cast<Ogre::SceneNode*>(mNode->getParent())->
				removeAndDestroyChild(mNode->getName());
		mNode = 0;
		mSceneMgr = 0;
	}

	void LayeredClouds::notifyCameraChanged (Ogre::Camera *cam) {
        CameraBoundElement::notifyCameraChanged (cam);
    }

	void LayeredClouds::setFarRadius (Ogre::Real radius) {
        CameraBoundElement::setFarRadius (radius);
		mNode->setScale (Ogre::Vector3::UNIT_SCALE * radius);
	}

	Ogre::GpuProgramParametersSharedPtr LayeredClouds::getVpParams() {
		return mMaterial->getBestTechnique()->getPass(0)->getVertexProgramParameters();
	}

	Ogre::GpuProgramParametersSharedPtr LayeredClouds::getFpParams() {
		return mMaterial->getBestTechnique()->getPass(0)->getFragmentProgramParameters();
	}

	Ogre::TextureUnitState* LayeredClouds::getTUS(unsigned short num) {
		return mMaterial->getBestTechnique()->getPass(0)->getTextureUnitState(num);
	}
	
	void LayeredClouds::update(Ogre::Real timePassed,
			const Ogre::Vector3 &sunDirection,
			const Ogre::ColourValue &sunColour,
			const Ogre::ColourValue &fogColour)
	{
		// Set sun parameters.
		setSunDirection(sunDirection);
		setSunColour(sunColour);
		setFogColour(fogColour);

		if (isAnimating()) {
			// Move clouds.
			setCloudMassOffset(getCloudMassOffset() + timePassed * getCloudSpeed());
			setCloudDetailOffset(getCloudDetailOffset() - timePassed * getCloudSpeed());

			// Animate cloud blending.
			double blend = getCloudMassBlend();
			blend += timePassed / getCloudBlendTime();
			blend = fmod(blend, 1);
			if (blend < 0) {
				blend = 1 - blend;
			}
			setCloudMassBlend(blend);
		}
	}

	bool LayeredClouds::isAnimating () const {
		return mAnimating;
	}

	void LayeredClouds::setAnimating (bool animating) {
		mAnimating = animating;
	}

	void LayeredClouds::setCloudSpeed(const Ogre::Vector2 &cloudSpeed) {
		mCloudSpeed = cloudSpeed;
	}

	Ogre::Vector2 LayeredClouds::getCloudSpeed() const {
		return mCloudSpeed;
	}

	void LayeredClouds::setCloudBlendTime(const Ogre::Real cloudBlendTime) {
		static const Ogre::String animTextures1[4] = {
			"noise1.png", "noise2.png", "noise3.png", "noise4.png"
		};
		static const Ogre::String animTextures2[4] = {
			"noise2.png", "noise3.png", "noise4.png", "noise1.png"
		};

		Ogre::Pass* pass = mMaterial->getBestTechnique()->getPass(0);
		pass->getTextureUnitState(0)->setAnimatedTextureName(animTextures1, 4, cloudBlendTime * 4);
		pass->getTextureUnitState(1)->setAnimatedTextureName(animTextures2, 4, cloudBlendTime * 4);
		setCloudMassBlend(0);
		mCloudBlendTime = cloudBlendTime;
	}

	Ogre::Real LayeredClouds::getCloudBlendTime() const {
		return mCloudBlendTime;
	}

	void LayeredClouds::setSunDirection(const Ogre::Vector3 &sunDirection) {
		if(mShadersEnabled) {
			getVpParams()->setNamedConstant("sunDirection", sunDirection);
		}
	}

	void LayeredClouds::setSunColour(const Ogre::ColourValue &sunColour) {
		if(mShadersEnabled) {
			getFpParams()->setNamedConstant("sunColour", sunColour);
		}
	}

	void LayeredClouds::setFogColour(const Ogre::ColourValue &fogColour) {
		if(mShadersEnabled) {
			getFpParams()->setNamedConstant("fogColour", fogColour);
		}
	}

	void LayeredClouds::setCloudMassOffset(const Ogre::Vector2 &cloudMassOffset) {
		mCloudMassOffset = cloudMassOffset;
		if(mShadersEnabled) {
			getFpParams()->setNamedConstant("cloudMassOffset", extend(cloudMassOffset));
		} else {
			getTUS(0)->setTextureScroll(cloudMassOffset.x, cloudMassOffset.y);
			getTUS(1)->setTextureScroll(cloudMassOffset.x, cloudMassOffset.y);
		}
	}

	Ogre::Vector2 LayeredClouds::getCloudMassOffset() const {
		return mCloudMassOffset;
	}

	void LayeredClouds::setCloudMassBlend(const Ogre::Real cloudMassBlend) {
		mCloudMassBlend = cloudMassBlend;
		if(mShadersEnabled) {
			getFpParams()->setNamedConstant("cloudMassBlend", cloudMassBlend);
		} else {
			getTUS(1)->setColourOperationEx(Ogre::LBX_BLEND_MANUAL, Ogre::LBS_TEXTURE, Ogre::LBS_CURRENT, Ogre::ColourValue::White, Ogre::ColourValue::White, cloudMassBlend);
		}
	}

	Ogre::Real LayeredClouds::getCloudMassBlend() const {
		return mCloudMassBlend;
	}

	void LayeredClouds::setCloudDetailOffset(const Ogre::Vector2 &cloudDetailOffset) {
		mCloudDetailOffset = cloudDetailOffset;
		if(mShadersEnabled) {
			getFpParams()->setNamedConstant("cloudDetailOffset", extend(cloudDetailOffset));
		}
	}

	Ogre::Vector2 LayeredClouds::getCloudDetailOffset() const {
		return mCloudDetailOffset;
	}

	void LayeredClouds::setCloudCover(const Ogre::Real cloudCover) {
        mCloudCover = cloudCover;

		if(mShadersEnabled)	{
			float cloudCoverageThreshold = 0;
			if (mCloudCoverLookup.get() != 0) {
				cloudCoverageThreshold = getInterpolatedColour(cloudCover, 1, mCloudCoverLookup.get(), false).r;
			} else {
				cloudCoverageThreshold = 1 - cloudCover;
			}
			getFpParams()->setNamedConstant("cloudCoverageThreshold", cloudCoverageThreshold);
		}
		else {
			getTUS(2)->setColourOperationEx(Ogre::LBX_MODULATE, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, Ogre::ColourValue(cloudCover, cloudCover, cloudCover));
		}
	}

	Ogre::Real LayeredClouds::getCloudCover() const {
		return mCloudCover;
	}

    void LayeredClouds::setCloudCoverLookup (const Ogre::String& fileName) {
        mCloudCoverLookup.reset(0);
        mCloudCoverLookup.reset(new Ogre::Image());
        mCloudCoverLookup->load (fileName, RESOURCE_GROUP_NAME);
    }

    void LayeredClouds::disableCloudCoverLookup () {
        mCloudCoverLookup.reset(0);
    }
} // namespace caelum
