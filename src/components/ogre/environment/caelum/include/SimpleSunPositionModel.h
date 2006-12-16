#ifndef SIMPLESUNPOSITIONMODEL_H
#define SIMPLESUNPOSITIONMODEL_H

#include "CaelumPrerequisites.h"
#include "SunPositionModel.h"

namespace caelum {

/** A simple circular sun position model. 
	It just creates an inclinated circular orbit.
	@author Jesús Alonso Abad.
 */
class DllExport SimpleSunPositionModel : public SunPositionModel {
// Attributes -----------------------------------------------------------------
	protected:
		/** The sun trajectory inclination. This is similar to the earth's rotation axis inclination.
		 */
		Ogre::Degree mInclination;

// Methods --------------------------------------------------------------------
	public:
		/** Basic constructor.
			@param inc The orbit inclination.
		 */
		SimpleSunPositionModel (Ogre::Degree inc);
		
		/** Changes the orbit inclination.
			@param inc The inclination in degrees.
		 */
		void setInclination (Ogre::Degree inc);

		/** Gets the orbit inclination.
			@return The orbit inclination in degrees.
		 */
		Ogre::Degree getInclination () const;

		/** @copydoc SunPositionModel::update().
		 */
		Ogre::Vector3 update (const float time);
};

} // namespace caelum

#endif // SIMPLESUNPOSITIONMODEL_H
