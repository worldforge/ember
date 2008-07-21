/*
This file is part of Caelum.
See http://www.ogre3d.org/wiki/index.php/Caelum 

Copyright (c) 2008 Caelum team. See Contributors.txt for details.

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
#include "CaelumExceptions.h"
#include "Moon.h"

namespace caelum
{
    const Ogre::String Moon::MOON_MATERIAL_NAME = "Caelum/PhaseMoon";

    const Ogre::String Moon::MOON_BACKROUND_MATERIAL_NAME = "Caelum/MoonBackground";

    Moon::Moon (
		    Ogre::SceneManager *sceneMgr,
		    Ogre::SceneNode *caelumRootNode, 
		    const Ogre::String& moonTextureName,
		    Ogre::Degree angularSize
    ):
		    BaseSkyLight(sceneMgr, caelumRootNode),
		    mAngularSize(angularSize)
    {
        // Clone moon face material.
        mMoonMaterial = Ogre::MaterialManager::getSingletonPtr()->getByName(MOON_MATERIAL_NAME);
        if (mMoonMaterial.isNull()) {
            CAELUM_THROW_UNSUPPORED_EXCEPTION("Can't find moon material", "Moon");
        }
	    mMoonMaterial = mMoonMaterial->clone(mMoonMaterial->getName() + Ogre::StringConverter::toString((size_t)this));
        mMoonMaterial->load();
        if (mMoonMaterial->getBestTechnique() == NULL) {
            CAELUM_THROW_UNSUPPORED_EXCEPTION("Moon material not supported", "Moon");
        }

        // Clone moon back material.
        mBackMaterial = Ogre::MaterialManager::getSingletonPtr()->getByName(MOON_BACKROUND_MATERIAL_NAME);
        if (mBackMaterial.isNull()) {
            CAELUM_THROW_UNSUPPORED_EXCEPTION("Can't find moon background material", "Moon");
        }
	    mBackMaterial = mBackMaterial->clone(mBackMaterial->getName() + Ogre::StringConverter::toString((size_t)this));
        mBackMaterial->load();
        if (mBackMaterial->getBestTechnique() == NULL) {
            CAELUM_THROW_UNSUPPORED_EXCEPTION("Moon background material not supported", "Moon");
        }

	    getFpParams()->setIgnoreMissingParams(true);
        setMoonTexture(moonTextureName);

	    mMoonBB = sceneMgr->createBillboardSet(
                "Caelum/Moon/MoonBB/" + Ogre::StringConverter::toString((size_t)this), 1);
	    mMoonBB->setMaterialName (mMoonMaterial->getName());
	    mMoonBB->setCastShadows (false);
	    mMoonBB->setRenderQueueGroup (CAELUM_RENDER_QUEUE_MOON);
	    mMoonBB->setDefaultDimensions (1.0f, 1.0f);
	    mMoonBB->createBillboard (Ogre::Vector3::ZERO);

	    mBackBB = sceneMgr->createBillboardSet(
                "Caelum/Moon/BackBB/" + Ogre::StringConverter::toString((size_t)this), 1);
	    mBackBB->setMaterialName (mBackMaterial->getName());
	    mBackBB->setCastShadows (false);
	    mBackBB->setRenderQueueGroup (CAELUM_RENDER_QUEUE_MOON_BACKGROUND);
	    mBackBB->setDefaultDimensions (1.0f, 1.0f);
	    mBackBB->createBillboard (Ogre::Vector3::ZERO);

	    mNode->attachObject (mMoonBB);
	    mNode->attachObject (mBackBB);
    }

    Moon::~Moon () {
	    if (mMoonBB) {
		    mMoonBB->_getManager ()->destroyBillboardSet (mMoonBB);
		    mMoonBB = 0;
	    }
	    if (mBackBB) {
		    mBackBB->_getManager ()->destroyBillboardSet (mBackBB);
		    mBackBB = 0;
	    }
	    Ogre::MaterialManager::getSingletonPtr()->remove(mMoonMaterial->getHandle());
	    Ogre::MaterialManager::getSingletonPtr()->remove(mBackMaterial->getHandle());
    }

    void Moon::setBodyColour (const Ogre::ColourValue &colour) {
	    BaseSkyLight::setBodyColour(colour);

	    // Set moon material colour.
	    mMoonBB->getBillboard(0)->setColour(colour);
    }

    void Moon::setMoonTexture (const Ogre::String &textureName)
    {
	    // Update the moon material
	    assert(mMoonMaterial->getBestTechnique ());
	    assert(mMoonMaterial->getBestTechnique ()->getPass (0));
	    assert(mMoonMaterial->getBestTechnique ()->getPass (0)->getTextureUnitState (0));
	    mMoonMaterial->getBestTechnique ()->getPass (0)->getTextureUnitState (0)->setTextureName (textureName);
	    mBackMaterial->getBestTechnique ()->getPass (0)->getTextureUnitState (0)->setTextureName (textureName);
    }

    void Moon::setMoonTextureAngularSize(const Ogre::Degree& moonTextureAngularSize){
	    mAngularSize = moonTextureAngularSize;
    }

    void Moon::notifyCameraChanged (Ogre::Camera *cam) {
        // This calls setFarRadius
        BaseSkyLight::notifyCameraChanged(cam);

        // Set moon position.
        Ogre::Real moonDistance = mRadius - mRadius * Ogre::Math::Tan(mAngularSize);
	    mNode->setPosition(-mDirection * moonDistance);

        // Set moon scaling in [1.0 ~ 1.2] range
        float factor = 1.2f - mBodyColour.b * 0.2f;
        float scale = factor * moonDistance * Ogre::Math::Tan(mAngularSize);
        mNode->setScale (Ogre::Vector3::UNIT_SCALE * scale);
    }

    Ogre::GpuProgramParametersSharedPtr Moon::getFpParams () {
        assert (!mMoonMaterial.isNull ());
        assert (mMoonMaterial->getBestTechnique ());
        assert (mMoonMaterial->getBestTechnique ()->getPass (0));
        assert (mMoonMaterial->getBestTechnique ()->getPass( 0)->hasFragmentProgram ());
	    return mMoonMaterial->getBestTechnique ()->getPass (0)->getFragmentProgramParameters ();
    }

    void Moon::setPhase (Ogre::Real phase) {
	    getFpParams ()->setNamedConstant ("phase", phase);	
    }
}
