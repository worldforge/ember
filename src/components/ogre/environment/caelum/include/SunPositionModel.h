#ifndef SUNPOSITIONMODEL_H
#define SUNPOSITIONMODEL_H

#include "CaelumPrerequisites.h"

namespace caelum {

/** Interface for all the sun positioning models. 
		A sun position model is a configurable utility/model that will return sun's sky coordinates (direction/azimuth in the 
		form of a normalised vector) depending on various parameters. 
		These models can range from a simple circular (sin/cos) oscillation, to complex equations for accurate positioning.
		@author Jesús Alonso Abad.
 */
class DllExport SunPositionModel {
// Attributes -----------------------------------------------------------------
	protected:
		/** The last calculated sun position.
		 */
		Ogre::Vector3 mSunPosition;

// Methods --------------------------------------------------------------------
	public:
		virtual ~SunPositionModel() {}
		
		/** Updates the sun position.
				@param time Local time.
				@return The sun light direction (or the negated sun position).
		 */
		virtual Ogre::Vector3 update (const float time) = 0;
};

} // namespace caelum

#endif // SUNPOSITIONMODEL_H
