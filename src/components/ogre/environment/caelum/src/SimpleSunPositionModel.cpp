#include "SimpleSunPositionModel.h"

namespace caelum {

SimpleSunPositionModel::SimpleSunPositionModel (Ogre::Degree inc) {
	setInclination (inc);
}

void SimpleSunPositionModel::setInclination (Ogre::Degree inc) {
	mInclination = inc;
}

Ogre::Degree SimpleSunPositionModel::getInclination () const {
	return mInclination;
}

Ogre::Vector3 SimpleSunPositionModel::update (const float time) {
	// Get the inclinated axis
	Ogre::Vector3 axis = Ogre::Vector3::UNIT_Z;
	axis = Ogre::Quaternion (mInclination, Ogre::Vector3::UNIT_X) * axis;

	// Get the inclinated light direction, according to the day time
	Ogre::Vector3 dir = Ogre::Vector3::UNIT_Y;
	dir = Ogre::Quaternion (Ogre::Radian (time * 2 * Ogre::Math::PI), axis) * dir;

	mSunPosition = dir.normalisedCopy () * -1;

	return mSunPosition * -1;
}

} // namespace caelum
