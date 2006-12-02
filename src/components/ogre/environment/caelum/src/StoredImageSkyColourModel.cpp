#include "StoredImageSkyColourModel.h"

namespace caelum {

StoredImageSkyColourModel::StoredImageSkyColourModel () {
	mFogColourMap = 0;
	mFogDensity = 0.0005;
}

void StoredImageSkyColourModel::setSkyGradientsImage (const Ogre::String &gradients) {
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

Ogre::ColourValue StoredImageSkyColourModel::getFogColour (float time) {
	if (mFogColourMap == 0) {
		return Ogre::ColourValue::White;
	}
	else {
		return getInterpolatedColour (time, 1, mFogColourMap);
	}
}

float StoredImageSkyColourModel::getFogDensity (float time) {
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

	mFogColourMap->load (name, "Caelum");
}

Ogre::ColourValue StoredImageSkyColourModel::getInterpolatedColour (float x, float height, Ogre::Image *img) {
	// calculate the height
	int y = (int )(img->getHeight () * height);
	if (y >= img->getHeight ())
		y--;

	// Get the two closest pixels
	int curPix, auxPix;
	float diff;
	curPix = (int )(img->getWidth () * x);
	diff = img->getWidth () * x - curPix;
	if (diff < 0) {
		auxPix = curPix - 1;
	}
	else {
		auxPix = curPix + 1;
	}

	// Calculate the interpolated pixel
	Ogre::ColourValue c1, c2, cf;
	c1 = img->getColourAt (curPix % img->getWidth (), y, 0);
	c2 = img->getColourAt (auxPix % img->getWidth (), y, 0);
	cf = c1 * (1 - diff) + c2 * diff;

	return cf;
}

} // namespace caelum
