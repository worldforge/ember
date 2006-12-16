#ifndef SKYCOLOURMODEL_H
#define SKYCOLOURMODEL_H

#include "CaelumPrerequisites.h"

namespace caelum {

/** Sky colour model interface.
	@author Jesús Alonso Abad
	@version 0.1
 */
class DllExport SkyColourModel {
// Attributes -----------------------------------------------------------------
	protected:
		/** A reference to the sky gradients texture unit state.
		 */
		Ogre::TextureUnitState *mSkyGradientsTextureUnitState;

		/** The sky gradients image (for lookups).
		 */
		Ogre::Image *mSkyGradientsImage;

// Methods --------------------------------------------------------------------
	public:
		/** Constructor method.
		 */
		SkyColourModel () {
			mSkyGradientsTextureUnitState = 0;
			mSkyGradientsImage = 0;
		}

		/** Destructor.
		 */
		virtual ~SkyColourModel () {}

		/** Sets the sky gradients texture unit in use.
			@param tus The TextureUnitState to be modified.
		 */
		void setSkyGradientsTextureUnitState (Ogre::TextureUnitState *tus) {
			mSkyGradientsTextureUnitState = tus;
		}

		/** Gets the fog colour for a certain daytime.
			@param time The current time.
			@param sunDir The sun direction.
			@return The fog colour.
		 */
		virtual Ogre::ColourValue getFogColour (float time, const Ogre::Vector3 &sunDir) = 0;

		/** Gets the fog density for a certain daytime.
			@param time The current time.
			@param sunDir The sun direction.
			@return The fog density.
		 */
		virtual float getFogDensity (float time, const Ogre::Vector3 &sunDir) = 0;

		/** Gets the sun colour for a certain daytime.
			@param time The current time.
			@param sunDir The sun direction.
			@return The sun colour.
		 */
		virtual Ogre::ColourValue getSunColour (float time, const Ogre::Vector3 &sunDir) = 0;

		/** Updates the sky colour material, according to various factors.
			@param fpp The material fragment program parameters (null if it's not supported)
			@param vpp The material vertex program parameters (null if it's not supported)
			@param time The local daytime, ranging [0, 1]
			@param sunDir The sun light direction.
			@return True if everything went fine.
		 */
		virtual bool updateMaterial (Ogre::GpuProgramParametersSharedPtr fpp, Ogre::GpuProgramParametersSharedPtr vpp, float time, const Ogre::Vector3 &sunDir) = 0;
};

} // namespace caelum

#endif //SKYCOLOURMODEL_H
