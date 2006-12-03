#include "StoredImageSkyColourModel.h"

namespace caelum {

StoredImageSkyColourModel::StoredImageSkyColourModel () {
	mFogColourMap = 0;
	mFogDensity = 0.0005;
}

void StoredImageSkyColourModel::setSkyGradientsImage (const Ogre::String &gradients) {
	if (mSkyGradientsImage == 0)
		mSkyGradientsImage = new Ogre::Image ();

	mSkyGradientsImage->load (gradients, RESOURCE_GROUP_NAME);

	if (mSkyGradientsTextureUnitState) {
		// Dagon and Eihort compatibility
		#if OGRE_VERSION < ((1 << 16) | (3 << 8))
			mSkyGradientsTextureUnitState->setTextureName (gradients, Ogre::TEX_TYPE_2D, -1, true);
		#else
			mSkyGradientsTextureUnitState->setTextureName (gradients, Ogre::TEX_TYPE_2D);
			mSkyGradientsTextureUnitState->setIsAlpha (true);
		#endif			
	}

	setFogColoursImage (gradients);
}

Ogre::ColourValue StoredImageSkyColourModel::getFogColour (float time, const Ogre::Vector3 &sunDir) {
	if (mFogColourMap == 0) {
		return Ogre::ColourValue::White;
	}
	else {
		return getInterpolatedColour (time, 1, mFogColourMap);
	}
}

float StoredImageSkyColourModel::getFogDensity (float time, const Ogre::Vector3 &sunDir) {
	if (mFogColourMap == 0) {
		return mFogDensity;
	}
	else {
		return mFogDensity * (getInterpolatedColour (time, 1, mFogColourMap)).a;
	}
}

void StoredImageSkyColourModel::setFogDensity (float density) {
	mFogDensity = density;
}

void StoredImageSkyColourModel::setFogColoursImage (const Ogre::String &name) {
	if (mFogColourMap == 0)
		mFogColourMap = new Ogre::Image ();

	mFogColourMap->load (name, RESOURCE_GROUP_NAME);
}

Ogre::ColourValue StoredImageSkyColourModel::getSunColour (float time, const Ogre::Vector3 &sunDir) {
	if (mSkyGradientsTextureUnitState == 0) {
		return Ogre::ColourValue::White;
	}
	else {
		return getInterpolatedColour (time, sunDir.y, mSkyGradientsImage);
	}
}

bool StoredImageSkyColourModel::updateMaterial (Ogre::GpuProgramParametersSharedPtr fpp, Ogre::GpuProgramParametersSharedPtr vpp, float time, const Ogre::Vector3 &sunDir) {
	mSkyGradientsTextureUnitState->setTextureUScroll (time);
	if (!fpp.isNull ()) {
		fpp->setNamedConstant ("offset", time);
	}

	return true;
}

Ogre::ColourValue StoredImageSkyColourModel::getInterpolatedColour (float x, float height, Ogre::Image *img, bool wrap) {
	// Get the image width
	int width = img->getWidth ();

	// calculate the height
	int y = (int )(img->getHeight () * height);
	if (y >= img->getHeight ())
		y--;

	// Get the two closest pixels
	int curPix, auxPix;
	float diff;
	curPix = (int )(width * x);
	diff = width * x - curPix;
	if (diff < 0) {
		auxPix = curPix - 1;
	}
	else {
		auxPix = curPix + 1;
	}

	// Calculate the interpolated pixel
	Ogre::ColourValue c1, c2, cf;
	c1 = img->getColourAt (wrap ? curPix % width : curPix > width ? width : curPix, y, 0);
	c2 = img->getColourAt (wrap ? auxPix % width : auxPix > width ? width : auxPix, y, 0);
	cf = c1 * (1 - diff) + c2 * diff;

	return cf;
}

} // namespace caelum
