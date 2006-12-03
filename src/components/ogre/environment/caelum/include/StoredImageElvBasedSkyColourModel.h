#ifndef STOREDIMAGEELVBASEDSKYCOLOURMODEL_H
#define STOREDIMAGEELVBASEDSKYCOLOURMODEL_H

#include "StoredImageSkyColourModel.h"
#include "CaelumPrerequisites.h"

namespace caelum {

/** A stored image (sun elevation) based sky colour model.
	This particular model takes a stored bitmap as the sky gradients map, and the sun elevation as the 
	localisation parameter.
	@author Jesús Alonso Abad
	@version 0.1
 */
class DllExport StoredImageElvBasedSkyColourModel : public StoredImageSkyColourModel {
// Attributes -----------------------------------------------------------------

// Methods --------------------------------------------------------------------
	public:
		/** Constructor method.
		 */
		StoredImageElvBasedSkyColourModel () : StoredImageSkyColourModel () {}

		/** Destructor.
		 */
		~StoredImageElvBasedSkyColourModel () {}

		/** @copydoc StoredImageSkyColourModel::getFogColour().
		 */
		Ogre::ColourValue getFogColour (float time, const Ogre::Vector3 &sunDir);

		/** @copydoc StoredImageSkyColourModel::getFogDensity().
		 */
		float getFogDensity (float time, const Ogre::Vector3 &sunDir);

		/** @copydoc StoredImageSkyColourModel::setSkyGradientsImage().
		 */
		void setSkyGradientsImage (const Ogre::String &gradients);

		/** @copydoc StoredImageSkyColourModel::getSunColour().
		 */
		Ogre::ColourValue getSunColour (float time, const Ogre::Vector3 &sunDir);

		/** @copydoc StoredImageSkyColourModel::updateMaterial().
		 */
		bool updateMaterial (Ogre::GpuProgramParametersSharedPtr fpp, Ogre::GpuProgramParametersSharedPtr vpp, float time, const Ogre::Vector3 &sunDir);
};

} // namespace caelum

#endif //STOREDIMAGEELVBASEDSKYCOLOURMODEL_H
