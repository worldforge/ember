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
#include "PointStarfield.h"
#include "GeometryFactory.h"
#include "CaelumExceptions.h"
#include "Astronomy.h"

using namespace Ogre;

namespace caelum
{
	const Ogre::String PointStarfield::BILLBOARD_MATERIAL_NAME = "Caelum/StarPoint";

	PointStarfield::PointStarfield (
			Ogre::SceneManager *sceneMgr,
			Ogre::SceneNode *caelumRootNode,
			bool initWithCatalogue)
	{
		mMag0PixelSize = 16;
		mMinPixelSize = 4;
		mMaxPixelSize = 6;
		mMagnitudeScale = Math::Pow(100, 0.2);
		mObserverLatitude = 45;
		mObserverLongitude = 0;

		mMaterial = Ogre::MaterialManager::getSingleton ().getByName (BILLBOARD_MATERIAL_NAME);
		if (mMaterial.isNull ()) {
			CAELUM_THROW_UNSUPPORED_EXCEPTION ("Can't load point starfield material", "PointStarfield");
		}
		mMaterial->load ();
		if (mMaterial->getBestTechnique () == 0) {
			CAELUM_THROW_UNSUPPORED_EXCEPTION ("Can't load point starfield material", "PointStarfield");
		}

		sceneMgr->getRenderQueue()->getQueueGroup(CAELUM_RENDER_QUEUE_STARFIELD)->setShadowsEnabled (false);

		// We use a separate data source.
		Ogre::String objName = "Caelum/PointStarfield/" + Ogre::StringConverter::toString ((size_t)this);
        mManualObj = sceneMgr->createManualObject (objName);
        mManualObj->setDynamic(false);
		mManualObj->setRenderQueueGroup (CAELUM_RENDER_QUEUE_STARFIELD);
        mManualObj->setCastShadows(false);

		mNode = caelumRootNode->createChildSceneNode ();
		mNode->attachObject (mManualObj);

		if (initWithCatalogue) {
			addBrightStarCatalogue ();
		}
	}

	PointStarfield::~PointStarfield () {
		if (mNode) {
			mNode->getCreator()->destroySceneNode(mNode->getName());
			mNode = 0;
		}

		if (mManualObj) {
			mManualObj->_getManager()->destroyMovableObject(mManualObj);
			mManualObj = 0;
		}
			
		Ogre::MaterialManager::getSingletonPtr()->remove(mMaterial->getHandle());
	}

    void PointStarfield::notifyStarVectorChanged () {
        invalidateGeometry ();
    }

    void PointStarfield::clearAllStars () {
        mStars.clear();
        notifyStarVectorChanged ();
    }

	Real randReal () {
		return rand() / static_cast<float>(RAND_MAX);
	}

	Real randReal (Real min, Real max) {
		Real f = randReal ();
		return min * (1 - f) + max * f;
	}

	void PointStarfield::addRandomStars (int count)
	{
		for (int i = 0; i < count; ++i) {
			// Generate a vector inside a sphere
			Ogre::Vector3 pos;
			do {
				pos.x = randReal(-1, 1);
				pos.y = randReal(-1, 1);
				pos.z = randReal(-1, 1);
			} while (pos.squaredLength () >= 1);

			// Convert to rasc/decl angles.
			LongReal rasc, decl, dist;
			Astronomy::convertRectangularToSpherical(
					pos.x, pos.y, pos.z,
					rasc, decl, dist);

			Star s;
			s.RightAscension = Ogre::Degree (rasc);
			s.Declination = Ogre::Degree (decl);
			// This distribution is wrong.
			s.Magnitude = 6 * pos.squaredLength () + 1.5;
			mStars.push_back(s);
		}
		notifyStarVectorChanged ();
	}

	void PointStarfield::addStar (const BrightStarCatalogueEntry &entry) {
		Star s;
		s.RightAscension = Ogre::Degree(360 / 24.0f * (
				Math::Abs(entry.rasc_hour) +
				entry.rasc_min / 60.0f +
				entry.rasc_sec / 3600.0f));
		s.Declination = Ogre::Degree(Math::Sign(entry.decl_deg) * (
				Math::Abs(entry.decl_deg) +
				entry.decl_min / 60.0f +
				entry.decl_sec / 3600.0f));
		s.Magnitude = entry.magn;
		mStars.push_back(s);

        notifyStarVectorChanged ();
	}

	void PointStarfield::addBrightStarCatalogue (int count) {
		assert(count >= 0);
		if (count < BrightStarCatalogueSize) {
			// Only sort if we don't add everything.
			// It would be lovely if the catalogue was already sorted.
			std::vector<std::pair<Real, int> > vec;
			vec.reserve(BrightStarCatalogueSize);
			for (int i = 0; i < BrightStarCatalogueSize; ++i) {
				vec.push_back(std::make_pair(BrightStarCatalogue[i].magn, i));
			}
			sort(vec.begin(), vec.end());
			for (int i = 0; i < count; ++i) {
				addStar(BrightStarCatalogue[vec[i].second]);
			}
		} else {
			assert(count == BrightStarCatalogueSize);
			for (int i = 0; i < BrightStarCatalogueSize; ++i) {
				addStar(BrightStarCatalogue[i]);
			}
		}
        notifyStarVectorChanged ();
	}

	void PointStarfield::invalidateGeometry () {
		mValidGeometry = false;
	}

	void PointStarfield::ensureGeometry ()
	{
		if (mValidGeometry) {
			return;
		}

		Ogre::LogManager::getSingleton ().logMessage ("Caelum: Recomputing starfield geometry.");

        mManualObj->clear();
        mManualObj->estimateVertexCount(6 * mStars.size());
        mManualObj->begin(mMaterial->getName (), Ogre::RenderOperation::OT_TRIANGLE_LIST);
        for (uint i = 0; i < mStars.size(); ++i) {
			// Determine position at J2000
			LongReal azm, alt;
			Astronomy::convertEquatorialToHorizontal(
					Astronomy::J2000,
					mObserverLatitude.valueDegrees(),
					mObserverLongitude.valueDegrees(),
					mStars[i].RightAscension.valueDegrees(), mStars[i].Declination.valueDegrees(),
					azm, alt);

    		Ogre::Vector3 pos;
		    pos.z = -Math::Cos (Ogre::Degree(azm)) * Math::Cos (Ogre::Degree(alt));
		    pos.x =  Math::Sin (Ogre::Degree(azm)) * Math::Cos (Ogre::Degree(alt));
		    pos.y = -Math::Sin (Ogre::Degree(alt));

            mManualObj->colour (Ogre::ColourValue::White);
            mManualObj->position (pos);
            mManualObj->textureCoord (+1, -1, mStars[i].Magnitude);
            mManualObj->position (pos);
            mManualObj->textureCoord (+1, +1, mStars[i].Magnitude);
            mManualObj->position (pos);
            mManualObj->textureCoord (-1, -1, mStars[i].Magnitude);

            mManualObj->position (pos);
            mManualObj->textureCoord (-1, -1, mStars[i].Magnitude);
            mManualObj->position (pos);
            mManualObj->textureCoord (+1, +1, mStars[i].Magnitude);
            mManualObj->position (pos);
            mManualObj->textureCoord (-1, +1, mStars[i].Magnitude);
        }
        mManualObj->end();

        // Set infinite bounds on the starfield.
        AxisAlignedBox box;
        box.setInfinite ();
        mManualObj->setBoundingBox (box);

		mValidGeometry = true;
	}

	void PointStarfield::notifyCameraChanged (Ogre::Camera *cam) {
		CameraBoundElement::notifyCameraChanged (cam);

        // Shader params are changed for every camera.
        Pass* pass = mMaterial->getBestTechnique ()->getPass (0);
        GpuProgramParametersSharedPtr fpParams = pass->getFragmentProgramParameters ();
        GpuProgramParametersSharedPtr vpParams = pass->getVertexProgramParameters ();

        int height = cam->getViewport ()-> getActualHeight ();
        int width = cam->getViewport ()-> getActualWidth ();
        Real pixFactor = 1.0f / width;
        Real magScale = -Math::Log (mMagnitudeScale) / 2;
        Real mag0Size = mMag0PixelSize * pixFactor;
        Real minSize = mMinPixelSize * pixFactor;
        Real maxSize = mMaxPixelSize * pixFactor;
        Real aspectRatio = static_cast<Real>(width) / height;

        // These params are relative to the size of the screen.
        vpParams->setIgnoreMissingParams (true);
        vpParams->setNamedConstant ("mag_scale", magScale);
        vpParams->setNamedConstant ("mag0_size", mag0Size);
        vpParams->setNamedConstant ("min_size", minSize);
        vpParams->setNamedConstant ("max_size", maxSize);
        vpParams->setNamedConstant ("aspect_ratio", aspectRatio);
	}

	void PointStarfield::setFarRadius (Ogre::Real radius) {
        CameraBoundElement::setFarRadius(radius);
	    mNode->setScale (Ogre::Vector3::UNIT_SCALE * radius);
	}

	void PointStarfield::_update (const float time) {
		// This is probably wrong.
		Ogre::Quaternion orientation = Ogre::Quaternion::IDENTITY;
		orientation = orientation * Ogre::Quaternion (Ogre::Radian (-mObserverLatitude + Ogre::Degree (90)), Ogre::Vector3::UNIT_X);
		orientation = orientation * Ogre::Quaternion (Ogre::Radian (-time * 2 * Ogre::Math::PI), Ogre::Vector3::UNIT_Y);
		mNode->setOrientation (orientation);
        ensureGeometry ();
	}

	void PointStarfield::setObserverLatitude (Ogre::Degree value) {
		if (!Math::RealEqual (mObserverLatitude.valueDegrees (), value.valueDegrees (), 0.001)) {
			mObserverLatitude = value;
			invalidateGeometry ();
		}
	}

	void PointStarfield::setObserverLongitude (Ogre::Degree value) {
		if (!Math::RealEqual (mObserverLongitude.valueDegrees (), value.valueDegrees (), 0.001)) {
			mObserverLongitude = value;
			invalidateGeometry ();
		}
	}
}
