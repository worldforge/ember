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

#ifndef SKYCOLOURMODEL_H
#define SKYCOLOURMODEL_H

#include "CaelumPrerequisites.h"

namespace caelum {

/** Class which returns various sky colours.
 *  This is based on the same gradients as the skydome.
 *  It would be nice to split it into multiple components.
 *  Functions take a "time" parameter but only elevation is actually used.
 */
class DllExport SkyColourModel {
// Attributes -----------------------------------------------------------------
	private:
		/** The sky gradients image (for lookups).
		 */
		Ogre::Image *mSkyGradientsImage;

        /** The sun gradients image (for lookups).
         */
        Ogre::Image *mSunColoursImage;

        /** Multiplier for fog densities obtained from the sky gradients image.
         */
        float mFogDensityMultiplier;

// Methods --------------------------------------------------------------------
	public:
	    /** Constructor method.
		 */
		SkyColourModel (
                const Ogre::String &skyGradients = "EarthClearSky2.png",
                const Ogre::String &sunColours = "SunGradient.png"
                );

		/** Destructor.
		 */
		virtual ~SkyColourModel ();

		/** Gets the fog colour for a certain daytime.
			@param time The current time.
			@param sunDir The sun direction.
			@return The fog colour.
		 */
		Ogre::ColourValue getFogColour (float time, const Ogre::Vector3 &sunDir);

		/** Gets the fog density for a certain daytime.
			@param time The current time.
			@param sunDir The sun direction.
			@return The fog density.
		 */
		float getFogDensity (float time, const Ogre::Vector3 &sunDir);

		/** Get the colour of the sun sphere.
         *  This colour is used to draw the sun sphere in the sky.
		 *  @return The colour of the sun.
		 */
		Ogre::ColourValue getSunSphereColour (float time, const Ogre::Vector3 &sunDir);

		/** Gets the colour of sun light.
         *  This color is used to illuminate the scene.
         *  @return The colour of the sun's light
		 */
		Ogre::ColourValue getSunLightColour (float time, const Ogre::Vector3 &sunDir);

		/// Set the sun gradients image.
		/// resources/EarthClearSky2.png is the default image; but you can supply another
		void setSkyGradientsImage (const Ogre::String &filename);

		/// Set the sun colours image.
		/// Sun colour is taken from this image.
		/// 'resources/SunGradient.png' is the default image; but you can supply another
		void setSunColoursImage (const Ogre::String &filename);
};

} // namespace caelum

#endif //SKYCOLOURMODEL_H
