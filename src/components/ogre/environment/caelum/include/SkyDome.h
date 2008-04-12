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

#ifndef SKYDOME_H
#define SKYDOME_H

#include "CaelumPrerequisites.h"
#include "CameraBoundElement.h"
#include "Sun.h"

namespace caelum {

/** A sky dome element.
	@author Jes�s Alonso Abad
 */
class DllExport SkyDome : public CameraBoundElement {
// Attributes -----------------------------------------------------------------
	private:
		/** Control scene node.
		 */
		Ogre::SceneNode *mNode;

		/** Name of the spheric dome resource.
		 */
		static const Ogre::String SPHERIC_DOME_NAME;

		/** Name of the dome material.
		 */
		static const Ogre::String SKY_DOME_MATERIAL_NAME;

		/// Reference to the sky dome material.
		Ogre::MaterialPtr mMaterial;

        /// If haze is enabled.
        bool mHazeEnabled;

// Methods --------------------------------------------------------------------
	public:
		/** Constructor
         *  This will setup some nice defaults.
		 *  @param sceneMgr The scene manager where this sky dome will be created.
		 */
		SkyDome (Ogre::SceneManager *sceneMgr, Ogre::SceneNode *caelumRootNode);

		/** Destructor
		 */
		virtual ~SkyDome ();

		/** Sets the sun direction.
			@param dir The sun light direction.
		 */
		void setSunDirection (Ogre::Vector3 dir);

		/** Sets the new light absorption factor.
			@param absorption The light absorption factor; a number in the range [0, 1], the lower, the less light the atmosphere will absorb.
		 */
		void setLightAbsorption (float absorption) const;

		/** Sets the light scattering factor. 
			@param scattering The light scattering factor; a number major than zero.
		 */
		void setLightScattering (float scattering) const;

		/** Sets the atmosphere height factor. 
			@param height The atmosphere height factor; a number in the range (0, 1].
		 */
		void setAtmosphereHeight (float height) const;

        /// Set the sky color gradients image.
        void setSkyGradientsImage (const Ogre::String& gradients);

        /// Set the atmosphere depthh gradient image.
        void setAtmosphereDepthImage (const Ogre::String& gradients);

        /// If skydome haze is enabled.
        bool getHazeEnabled () const;

        /// Enable or disable skydome haze. This makes the sky darker.
        void setHazeEnabled (bool value);

    public:
		/// Handle camera change.
		virtual void notifyCameraChanged (Ogre::Camera *cam);

    protected:
        /// Handle far radius.
	    virtual void setFarRadius (Ogre::Real radius);
};

} // namespace caelum

#endif //SKYDOME_H
