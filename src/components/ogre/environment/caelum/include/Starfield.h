#ifndef STARFIELD_H
#define STARFIELD_H

#include "CaelumPrerequisites.h"
#include "CameraBoundElement.h"

namespace caelum {

/** Starfield dome class.
	@author Jesús Alonso Abad
 */
class DllExport Starfield : public CameraBoundElement {
// Attributes -----------------------------------------------------------------
	protected:
		/** Reference to the dome node.
		 */
		Ogre::SceneNode *mNode;

		/** Name of the spheric dome resource.
		 */
		static const Ogre::String mStarfieldDomeResourceName;

		/** Name of the starfield material.
		 */
		static const Ogre::String STARFIELD_MATERIAL_NAME;

		/** Reference to the starfield material.
		 */
		Ogre::MaterialPtr mStarfieldMaterial;

		/** Inclination of the starfield.
		 */
		Ogre::Degree mInclination;

// Methods --------------------------------------------------------------------
	public:
		/** Constructor.
			@param sceneMgr The scene manager this dome will belong to.
		 */
		Starfield (Ogre::SceneManager *sceneMgr);

		/** Destructor.
		 */
		virtual ~Starfield ();

		/** @copydoc CameraBoundElement::notifyCameraChanged().
		 */
		void notifyCameraChanged (Ogre::Camera *cam);

		/** @copydoc CameraBoundElement::setFarRadius().
		 */
		void setFarRadius (float radius);

		/** Sets the starfield inclination.
			@param inc The starfield inclination in degrees.
		 */
		void setInclination (Ogre::Degree inc);

		/** Updates the starfield position/orientation.
			@param time Local time in [0, 1] range.
		 */
		void update (const float time);

		/** Updates the starfield material.
			@param mapName The new starfield texture map name.
		 */
		void updateMaterial (const Ogre::String &mapName);

	private:
		/** Internal method to create the starfield material.
		 */
		void createStarfieldMaterial ();

		/** Internal method to destroy the starfield material.
		 */
		void destroyStarfieldMaterial ();
};

} // namespace caelum;

#endif //STARFIELD_H
