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
};

} // namespace caelum

#endif //SKYDOME_H
