#ifndef SKYDOME_H
#define SKYDOME_H

#include "CaelumPrerequisites.h"

namespace caelum {

/** A sky dome element.
	@author Jesús Alonso Abad
	@version 0.1
 */
class DllExport SkyDome  : public Ogre::RenderTargetListener {
// Attributes -----------------------------------------------------------------
	private:
		/** Control scene node.
		 */
		Ogre::SceneNode *mNode;

		/** Name of the spheric dome resource.
		 */
		static const Ogre::String mSphericDomeResourceName;

		/** Defines if the sky dome radius is automatically set or not.
		 */
		bool mAutoRadius;

// Methods --------------------------------------------------------------------
	public:
		/** Constructor (DO NOT USE! Use CaelumSystem::createSkyDome instead).
			@param sceneMgr The scene manager where this sky dome will be created.
		 */
		SkyDome (Ogre::SceneManager *sceneMgr);

		/** Destructor (DO NOT USE! Use CaelumSystem::destroySkyDome instead).
		 */
		~SkyDome ();

		/** Event trigger called just before rendering a viewport in a render target this dome is attached to.
			Useful to make the dome follow every camera that renders a viewport in a certain render target.
			@param e The viewport event, containing the viewport (and camera) to be rendered right now.
		 */
		void preViewportUpdate (const Ogre::RenderTargetViewportEvent &e);

		/** Forces the size of the dome to a specific radius.
			If the parameter is negative or zero, the radius is set automatically.
			@param radius The positive radius of the sky dome, or a negative/zero value to let Caelum to resize it.
		 */
		void setSize (float radius);
};

} // namespace caelum

#endif //SKYDOME_H
