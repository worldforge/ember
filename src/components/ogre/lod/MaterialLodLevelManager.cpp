#include "MaterialLodLevelManager.h"

#include "components/ogre/AutoGraphicsLevelManager.h"

#include "OgreMaterialManager.h"
#include "OgreLodStrategy.h"
#include "OgreMath.h"

#include "sigc++/signal.h"

namespace Ember
{
namespace OgreView
{
namespace Lod
{

MaterialLodLevelManager::MaterialLodLevelManager(AutomaticGraphicsLevelManager& automaticGraphicsLevelManager) :
		mThresholdLevel(1.0f), mLodFactor(1.0f), mDefaultStep(0.3f), mMinLodFactor(0.2f), mMaxLodFactor(2.0f), mAutomaticGraphicsLevelManager(automaticGraphicsLevelManager)
{
}

MaterialLodLevelManager::~MaterialLodLevelManager()
{
	mChangeRequiredConnection.disconnect();
}

bool MaterialLodLevelManager::setLodBiasAll(float factor)
{
	//additional check if factor is set from outside
	if (factor > mMaxLodFactor || factor < mMinLodFactor) {
		return false;
	}

	Ogre::ResourceManager::ResourceMapIterator resources = Ogre::MaterialManager::getSingleton().getResourceIterator();
	while (resources.hasMoreElements()) {
		resources.moveNext();
		Ogre::MaterialPtr material = static_cast<Ogre::MaterialPtr>(resources.peekNextValue());

		//implementation for distance strategy
		if (material->getLodStrategy()->getName() == "Distance") {
			Ogre::Material::LodValueList lodValues;
			Ogre::Material::LodValueIterator I = material->getLodValueIterator();

			// Ignoring the first 0 lod value that getLodValueIterator returns since setLodLevels needs values from level 1 onwards.
			I.moveNext();

			while (I.hasMoreElements()) {
				I.moveNext();

				//The lod values are stored as squares of user provided values, but expected in unsquared values.
				lodValues.push_back(Ogre::Math::Sqrt(I.peekNext() * factor));
			}
			material->setLodLevels(lodValues);
		} else {
			//TODO: Implementation for Pixel Count lod strategy.
		}
	}
	return true;
}

bool MaterialLodLevelManager::changeLevel(float level)
{
	if (std::abs(level) < mThresholdLevel) {
		return false;
	} else {
		bool changeMade = false;
		if (level > 1.0f) {
			changeMade |= stepDownLodBias(mDefaultStep);
		} else {
			changeMade |= stepUpLodBias(mDefaultStep);
		}
		return changeMade;
	}
}

bool MaterialLodLevelManager::stepDownLodBias(float step)
{
	if (mLodFactor > step) { //step down only if existing lod is greater than step
		mLodFactor -= step;
		return setLodBiasAll(mLodFactor);
	} else if (mLodFactor < step && mLodFactor > mMinLodFactor) { //if there is still some positive lod left which is smaller than step, set it to the minimum lod
		mLodFactor = mMinLodFactor;
		return setLodBiasAll(mLodFactor);
	} else { //step down not possible
		return false;
	}
}

bool MaterialLodLevelManager::stepUpLodBias(float step)
{
	if (mLodFactor + step <= mMaxLodFactor) { //step up only if the step doesn't cause lod to go over maximum lod.
		mLodFactor += step;
		return setLodBiasAll(mLodFactor);
	} else if (mLodFactor < mMaxLodFactor) { //if the lod is still below maximum lod but a default step causes it to go over it.
		mLodFactor = mMaxLodFactor;
		return setLodBiasAll(mLodFactor);
	} else {
		return false; //step up not possible
	}
}

void MaterialLodLevelManager::initialize()
{
	mChangeRequiredConnection = mAutomaticGraphicsLevelManager.getGraphicalAdapter().changeRequired.connect(&MaterialLodLevelManager::changeLevel);
}

void MaterialLodLevelManager::pause()
{
	if (mChangeRequiredConnection) {
		mChangeRequiredConnection.disconnect();
	}
}

void MaterialLodLevelManager::unpause()
{
	if (!mChangeRequiredConnection) {
		mChangeRequiredConnection = mAutomaticGraphicsLevelManager.getGraphicalAdapter().changeRequired.connect(&MaterialLodLevelManager::changeLevel);
	}
}

}
}
}
