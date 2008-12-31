/*
This file is part of Caelum.
See http://www.ogre3d.org/wiki/index.php/Caelum 

Copyright (c) 2006-2008 Caelum team. See Contributors.txt for details.

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

#ifndef STARFIELD_H
#define STARFIELD_H

#include "CaelumPrerequisites.h"
#include "CameraBoundElement.h"

namespace caelum {

/** ImageStarfield dome class.
	@author Jesús Alonso Abad
 */
class CAELUM_EXPORT ImageStarfield : public CameraBoundElement {
// Attributes -----------------------------------------------------------------
	protected:
		/** Reference to the dome node.
		 */
		Ogre::SceneNode *mNode;

		/** Name of the spheric dome resource.
		 */
		static const Ogre::String STARFIELD_DOME_NAME;

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
		ImageStarfield (
                Ogre::SceneManager *sceneMgr,
				Ogre::SceneNode *caelumRootNode,
                const Ogre::String &textureName = "Starfield.jpg");

		/** Destructor.
		 */
		virtual ~ImageStarfield ();

		/** Sets the starfield inclination. This inclination is the angle between the starfield rotation axis and the horizon plane.
			@param inc The starfield inclination in degrees. It`s equal to observer latitude taken with opposite sign.
		 */
		void setInclination (Ogre::Degree inc);

		/** Updates the starfield position/orientation.
			@param time Local time in [0, 1] range.
		 */
		void update (const float time);

		/** Updates the starfield material.
			@param mapName The new starfield texture map name.
		 */
		void setTexture (const Ogre::String &mapName);

    public:
		/// Handle camera change.
		virtual void notifyCameraChanged (Ogre::Camera *cam);

    protected:
        /// Handle far radius.
	    virtual void setFarRadius (Ogre::Real radius);
};

} // namespace caelum;

#endif //STARFIELD_H
