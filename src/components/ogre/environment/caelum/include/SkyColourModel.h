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

// Methods --------------------------------------------------------------------
	public:
		/** Constructor method.
		 */
		SkyColourModel () {
			mSkyGradientsTextureUnitState = 0;
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
			@return The fog colour.
		 */
		virtual Ogre::ColourValue getFogColour (float time) = 0;

		/** Gets the fog density for a certain daytime.
			@param time The current time.
			@return The fog density.
		 */
		virtual float getFogDensity (float time) = 0;

};

} // namespace caelum

#endif //SKYCOLOURMODEL_H
