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
#include "SkyColourModel.h"
#include "ImageHelper.h"

namespace caelum {

SkyColourModel::SkyColourModel
(
        const Ogre::String &gradients,
        const Ogre::String &sunColours
):
        mSkyGradientsImage(0),
        mSunColoursImage(0)
{
	mSkyGradientsImage = 0;
    mFogDensityMultiplier = 1;
    setSkyGradientsImage(gradients);
    setSunColoursImage(sunColours);
}

SkyColourModel::~SkyColourModel () {
	if (mSkyGradientsImage != 0) {
		delete mSkyGradientsImage;
		mSkyGradientsImage = 0;
	}
	if (mSunColoursImage != 0) {
		delete mSunColoursImage;
		mSunColoursImage = 0;
	}
}

void SkyColourModel::setSkyGradientsImage (const Ogre::String &filename) {
	if (mSkyGradientsImage != 0) {
		delete mSkyGradientsImage;
		mSkyGradientsImage = 0;
	}

	mSkyGradientsImage = new Ogre::Image ();
	mSkyGradientsImage->load (filename, RESOURCE_GROUP_NAME);
}

void SkyColourModel::setSunColoursImage (const Ogre::String &filename) {
	if (mSunColoursImage != 0) {
		delete mSunColoursImage;
		mSunColoursImage = 0;
	}

	mSunColoursImage = new Ogre::Image ();
	mSunColoursImage->load (filename, RESOURCE_GROUP_NAME);
}

Ogre::ColourValue SkyColourModel::getFogColour (float time, const Ogre::Vector3 &sunDir) {
	float elevation = sunDir.dotProduct (Ogre::Vector3::UNIT_Y) * 0.5 + 0.5;
	Ogre::ColourValue col = getInterpolatedColour (elevation, 1, mSkyGradientsImage, false);
    return col;
}

float SkyColourModel::getFogDensity (float time, const Ogre::Vector3 &sunDir) {
	float elevation = sunDir.dotProduct (Ogre::Vector3::UNIT_Y) * 0.5 + 0.5;
	Ogre::ColourValue col = getInterpolatedColour (elevation, 1, mSkyGradientsImage, false);
    return col.a;
}

Ogre::ColourValue SkyColourModel::getSunSphereColour (float time, const Ogre::Vector3 &sunDir) {
    float elevation = sunDir.dotProduct (Ogre::Vector3::UNIT_Y);
    elevation = elevation * 2 + 0.4;

    if (mSunColoursImage == 0) {
        return Ogre::ColourValue::White;
    } else {
        return getInterpolatedColour (elevation, 1, mSunColoursImage, false);
    }
}

Ogre::ColourValue SkyColourModel::getSunLightColour (float time, const Ogre::Vector3 &sunDir) {
	float elevation = sunDir.dotProduct (Ogre::Vector3::UNIT_Y) * 0.5 + 0.5;

	// Hack: return averaged sky colours.
    // Don't use an alpha value for lights, this can can nasty problems.
	Ogre::ColourValue col = getInterpolatedColour (elevation, elevation, mSkyGradientsImage, false);
    double val = (col.r + col.g + col.b) / 3;
    col = Ogre::ColourValue(val, val, val, 1.0);
    assert(Ogre::Math::RealEqual(col.a, 1));
    return col;
}

} // namespace caelum
