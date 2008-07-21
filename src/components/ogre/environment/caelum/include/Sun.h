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

#ifndef SUN_H
#define SUN_H

#include "CaelumPrerequisites.h"
#include "CameraBoundElement.h"
#include "SkyLight.h"

namespace caelum {

class BaseSkyLight;		// abstract
class SphereSun;	// deprecated
class SpriteSun;

typedef SpriteSun Sun;
//========================================================================================================================

/** Class representing the sun as sphere with emissive color on it.
	@author Jes˙s Alonso Abad
 */
class CAELUM_EXPORT SphereSun : public BaseSkyLight {
// Attributes -----------------------------------------------------------------
	public:
		/// Name of the sun material.
		static const Ogre::String SUN_MATERIAL_NAME;

	private:
		/// The sun entity.
		Ogre::Entity *mSunEntity;

		/// Reference to the sun material.
		Ogre::MaterialPtr mSunMaterial;

// Methods --------------------------------------------------------------------
	public:
		/** Constructor.
			@param sceneMgr The scene manager where the lights will be created.
		 */
		SphereSun (Ogre::SceneManager *sceneMgr,
			Ogre::SceneNode *caelumRootNode,
			const Ogre::String &meshName = "sphere.mesh");

		/** Destructor.
			@note If a sun position model is in use, it will be deleted.
		 */
		virtual ~SphereSun ();

		/** Sets the sun sphere colour.
			@param colour The colour used to draw the sun
		 */
		void setBodyColour (const Ogre::ColourValue &colour);

    public:
		/// Handle camera change.
		virtual void notifyCameraChanged (Ogre::Camera *cam);
};

//========================================================================================================================

/** Class representing the sun as billboard with texture on it.
	@author Eugene Golushkov
 */
class CAELUM_EXPORT SpriteSun : public BaseSkyLight {
// Attributes -----------------------------------------------------------------
	public:
		/// Name of the sun material.
		static const Ogre::String SUN_MATERIAL_NAME;

	protected:
		/// The sun sprite.
		Ogre::BillboardSet* mSunBillboardSet;
		
		/// Reference to the sun material.
		Ogre::MaterialPtr mSunMaterial;

		/// The sun sprite visible angle
		Ogre::Degree mSunTextureAngularSize;

// Methods --------------------------------------------------------------------
	public:
		/** Constructor.
			@param sceneMgr The scene manager where the lights will be created.
		 */
		SpriteSun (Ogre::SceneManager *sceneMgr,
			Ogre::SceneNode *caelumRootNode,
			const Ogre::String& sunTextureName = "sun_disc.png", 
			const Ogre::Degree& sunTextureAngularSize = Ogre::Degree(3.77f)); // 0.53f is real angular size of Sun and Moon, 3.77f is compatible with SphereSun

		/** Destructor.
			@note If a sun position model is in use, it will be deleted.
		 */
		virtual ~SpriteSun ();

		/** Updates the sun material.
			@param textureName The new sun texture name.
		 */
		void setSunTexture (const Ogre::String &textureName);
		
		/** Updates the sun size.
			@param sunTextureAngularSize The new sun texture angular size.
		 */
		void setSunTextureAngularSize(const Ogre::Degree& sunTextureAngularSize);

		/** Sets the sun sphere colour.
			@param colour The colour used to draw the sun
		 */
		void setBodyColour (const Ogre::ColourValue &colour);

    public:
		/// Handle camera change.
		virtual void notifyCameraChanged (Ogre::Camera *cam);
};
}

#endif //SUN_H
