#include "StoredImageElvBasedSkyColourModel.h"

namespace caelum {

Ogre::ColourValue StoredImageElvBasedSkyColourModel::getFogColour (float time, const Ogre::Vector3 &sunDir) {
	float elevation = sunDir.dotProduct (Ogre::Vector3::UNIT_Y);
	elevation = elevation * 0.5 + 0.5;

	if (mFogColourMap == 0) {
		return Ogre::ColourValue::White;
	}
	else {
		return getInterpolatedColour (elevation, 1, mFogColourMap, false);
	}
}

float StoredImageElvBasedSkyColourModel::getFogDensity (float time, const Ogre::Vector3 &sunDir) {
	float elevation = sunDir.dotProduct (Ogre::Vector3::UNIT_Y);
	elevation = elevation * 0.5 + 0.5;

	if (mFogColourMap == 0) {
		return mFogDensity;
	}
	else {
		return mFogDensity * (getInterpolatedColour (elevation, 1, mFogColourMap, false)).a;
	}
}

void StoredImageElvBasedSkyColourModel::setSkyGradientsImage (const Ogre::String &gradients) {
	StoredImageSkyColourModel::setSkyGradientsImage (gradients);

	mSkyGradientsTextureUnitState->setTextureAddressingMode (Ogre::TextureUnitState::TAM_CLAMP);	
}

Ogre::ColourValue StoredImageElvBasedSkyColourModel::getSunColour (float time, const Ogre::Vector3 &sunDir) {
	float elevation = sunDir.dotProduct (Ogre::Vector3::UNIT_Y);
	elevation = elevation * 0.5 + 0.5;

	if (mSkyGradientsImage == 0) {
		return Ogre::ColourValue::White;
	}
	else {
		return getInterpolatedColour (elevation, elevation, mSkyGradientsImage, false);
	}
}

bool StoredImageElvBasedSkyColourModel::updateMaterial (Ogre::GpuProgramParametersSharedPtr fpp, Ogre::GpuProgramParametersSharedPtr vpp, float time, const Ogre::Vector3 &sunDir) {
	float elevation = sunDir.dotProduct (Ogre::Vector3::UNIT_Y);
	elevation = elevation * 0.5 + 0.5;

	mSkyGradientsTextureUnitState->setTextureUScroll (elevation);
	if (!fpp.isNull ()) {
		fpp->setNamedConstant ("offset", elevation);
	}

	return true;
}

} // namespace caelum
