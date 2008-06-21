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

namespace caelum {

class BaseSun;		// abstract
class SphereSun;	// deprecated
class SpriteSun;

typedef SpriteSun Sun;

/** Abstract class representing sun.
	Can be registered in CaelumSystem and receive update notifications.
	Contains scene node and main light, that are properly placed on the sky by update method.
	@author Jes˙s Alonso Abad
 */
class DllExport BaseSun : public CameraBoundElement {
// Attributes -----------------------------------------------------------------
	public:
	protected:
		/// Pointer to scene.
		Ogre::SceneManager *mScene;

		/// The main directional light.
		Ogre::Light *mMainLight;

		/// The sun scene node.
		Ogre::SceneNode *mSunNode;

		/// Base distance of the sun.
		float mRadius;

		/// The latest normalised sun direction.
		Ogre::Vector3 mSunDirection;
		
		/// Sun sphere colour, as set by setSunSphereColour
		Ogre::ColourValue mSunSphereColour;

		/// Sun light colour, as set by setSunLightColour
		Ogre::ColourValue mSunLightColour;

		/// Colour multiplier for light diffuse colour.
		Ogre::ColourValue mDiffuseMultiplier;

		/// Colour multiplier for light specular colour.
		Ogre::ColourValue mSpecularMultiplier;

		/** Colour multiplier for ambient light colour.
		 *  No effect unless isManagingAmbientLight is true.
		 */
		Ogre::ColourValue mAmbientMultiplier;

		/// If true then this class also manages ambient lighting.
		bool mManageAmbientLight;

// Methods --------------------------------------------------------------------
	public:
		/** Constructor.
			@param sceneMgr The scene manager where the lights will be created.
		 */
		BaseSun (Ogre::SceneManager *sceneMgr,
			Ogre::SceneNode *caelumRootNode);

		/** Destructor.
			@note You must use one of descendant, since base sun is invisible.
		 */
		virtual ~BaseSun () = 0;

		/** Updates sun parameters.
			@param sunDirection Sun direction.
			@param sunLightColour Color for the sun's light source
			@param sunSphereColour Color to draw the sun sphere itself.
		 */
        virtual void update (
                const Ogre::Vector3& sunDirection,
                const Ogre::ColourValue &sunLightColour,
                const Ogre::ColourValue &sunSphereColour);

		/** Retrieves the latest sun direction.
			@return The sun direction.
		 */
		const Ogre::Vector3& getSunDirection () const;

		/** Set the sun direction.
			@param dir The sun direction.
		 */
		virtual void setSunDirection (const Ogre::Vector3 &dir);

		/// Get current sun sphere colour, as set in setSunSphereColour.
		Ogre::ColourValue getSunSphereColour () const;

		/** Sets the sun sphere colour.
			@param colour The colour used to draw the sun
		 */
		virtual void setSunSphereColour (const Ogre::ColourValue &colour);

		/// Get current sunlight colour, as set in setSunLightColour.
		Ogre::ColourValue getSunLightColour () const;

		/** Sets the sunlight colour.
			@param colour The colour used to illuminate the scene.
		 */
		virtual void setSunLightColour (const Ogre::ColourValue &colour);

		/// Set diffuse multiplier for light colour
		void setDiffuseMultiplier (const Ogre::ColourValue &diffuse);

		/// Set diffuse multiplier for light colour
		Ogre::ColourValue getDiffuseMultiplier () const;

		/// Set specular multiplier for light colour
		void setSpecularMultiplier (const Ogre::ColourValue &specular);

		/// Set specular multiplier for light colour
		Ogre::ColourValue getSpecularMultiplier () const;

		/// Set ambient multiplier for light colour
		/// This only works if isManaging
		void setAmbientMultiplier (const Ogre::ColourValue &ambient);

		/// Set ambient multiplier for light colour
		Ogre::ColourValue getAmbientMultiplier () const;

		/// This can make this class manage the ambient light of the scene.
		void setManageAmbientLight (bool manage);

		/// If the class is managing the scene's ambient light.
		bool isManagingAmbientLight () const;

		/// Allow access to main light, to disable it duaring eclipse, for example.
		Ogre::Light* getMainLight() const;

    protected:
        /// Handle far radius.
	    virtual void setFarRadius (Ogre::Real radius);
		
		/// Temporary change main light color
		void setMainLightColour(const Ogre::ColourValue &colour);
};

//========================================================================================================================

/** Class representing the sun as sphere with emissive color on it.
	@author Jes˙s Alonso Abad
 */
class DllExport SphereSun : public BaseSun {
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
		void setSunSphereColour (const Ogre::ColourValue &colour);

    public:
		/// Handle camera change.
		virtual void notifyCameraChanged (Ogre::Camera *cam);
};

//========================================================================================================================

/** Class representing the sun as billboard with texture on it.
	@author Eugene Golushkov
 */
class DllExport SpriteSun : public BaseSun {
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
		void setSunSphereColour (const Ogre::ColourValue &colour);

    public:
		/// Handle camera change.
		virtual void notifyCameraChanged (Ogre::Camera *cam);
};

} // namespace caelum

#endif //SUN_H
