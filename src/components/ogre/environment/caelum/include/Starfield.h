#ifndef STARFIELD_H
#define STARFIELD_H

#include "CaelumPrerequisites.h"

namespace caelum {

/** Starfield dome class.
	@author Jesús Alonso Abad
	@version 0.1
 */
class DllExport Starfield : public Ogre::RenderTargetListener {
// Attributes -----------------------------------------------------------------
	protected:
		/** Reference to the dome node.
		 */
		Ogre::SceneNode *mNode;

		/** Name of the spheric dome resource.
		 */
		static const Ogre::String mStarfieldDomeResourceName;

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
		~Starfield ();

		/** Event trigger called just before rendering a viewport in a render target this dome is attached to.
			Useful to make the dome follow every camera that renders a viewport in a certain render target.
			@param e The viewport event, containing the viewport (and camera) to be rendered right now.
		 */
		void preViewportUpdate (const Ogre::RenderTargetViewportEvent &e);

		/** Sets the starfield inclination.
			@param inc The starfield inclination in degrees.
		 */
		void setInclination (Ogre::Degree inc);

		/** Updates the starfield position/orientation.
			@param time Local time in [0, 1] range.
		 */
		void update (const float time);
};

} // namespace caelum;

#endif //STARFIELD_H
