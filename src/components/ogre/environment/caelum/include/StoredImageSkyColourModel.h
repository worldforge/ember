#ifndef STOREDIMAGESKYCOLOURMODEL_H
#define STOREDIMAGESKYCOLOURMODEL_H

#include "SkyColourModel.h"
#include "CaelumPrerequisites.h"

namespace caelum {

/** A stored image based sky colour model.
	This particular model takes a stored bitmap as the sky gradients map.
	@author Jesús Alonso Abad
	@version 0.1
 */
class DllExport StoredImageSkyColourModel : public SkyColourModel {
// Attributes -----------------------------------------------------------------
	protected:
		/** The fog colours map.
		 */
		Ogre::Image *mFogColourMap;

		/** The fog density.
		 */
		float mFogDensity;

// Methods --------------------------------------------------------------------
	public:
		/** Constructor method.
		 */
		StoredImageSkyColourModel ();

		/** Destructor.
		 */
		~StoredImageSkyColourModel () {}

		/** Sets the new sky gradients stored image to be used.
			@param gradients The filename of the new sky gradients stored image.
		 */
		void setSkyGradientsImage (const Ogre::String &gradients);

		/** @copydoc SkyColourModel::getFogColour().
		 */
		Ogre::ColourValue getFogColour (float time, const Ogre::Vector3 &sunDir);

		/** @copydoc SkyColourModel::getFogDensity().
		 */
		float getFogDensity (float time, const Ogre::Vector3 &sunDir);

		/** Sets the fog density.
			@param density The new fog density.
		 */
		void setFogDensity (float density);

		/** Sets the fog colours map.
			@note This map will be overriden by the sky gradients map if it's changed later.
			@param name The map filename.
		 */
		void setFogColoursImage (const Ogre::String &name);

		/** @copydoc SkyColourModel::getSunColour().
		 */
		Ogre::ColourValue getSunColour (float time, const Ogre::Vector3 &sunDir);

		/** @copydoc SkyColourModel::updateMaterial().
		 */
		bool updateMaterial (Ogre::GpuProgramParametersSharedPtr fpp, Ogre::GpuProgramParametersSharedPtr vpp, float time, const Ogre::Vector3 &sunDir);

	protected:
		/** Gets the interpolated colour between two pixels from an image.
			@param x The horizontal coordinate in the range [0, 1].
			@param height The height at which the pixels are located in the range [0, 1] (will be converted to integer).
			@param img The lookup image.
			@param wrap If the map wraps horizontally or not.
			@return The interpolated colour.
		 */
		static Ogre::ColourValue getInterpolatedColour (float x, float height, Ogre::Image *img, bool wrap = true);
};

} // namespace caelum

#endif //STOREDIMAGESKYCOLOURMODEL_H
