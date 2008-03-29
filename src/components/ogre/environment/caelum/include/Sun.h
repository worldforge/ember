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

/** Class representing the sun.
	It calculates the sun light direction, position, colour...
	@author Jes�s Alonso Abad
 */
class DllExport Sun : public CameraBoundElement {
// Attributes -----------------------------------------------------------------
	public:
		/// Name of the sun material.
		static const Ogre::String SUN_MATERIAL_NAME;

	private:
		/// Pointer to scene.
		Ogre::SceneManager *mScene;

		/// The main directional light.
		Ogre::Light *mMainLight;

		/// The sun scene node.
		Ogre::SceneNode *mSunNode;

		/// The sun entity.
		Ogre::Entity *mSunEntity;

		/// Reference to the sun material.
		Ogre::MaterialPtr mSunMaterial;

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
		Sun (Ogre::SceneManager *sceneMgr,
			Ogre::SceneNode *caelumRootNode,
			const Ogre::String &meshName = "sphere.mesh");

		/** Destructor.
			@note If a sun position model is in use, it will be deleted.
		 */
		virtual ~Sun ();

		/** Updates sun parameters.
			@param sunDirection Sun direction.
			@param sunLightColour Color for the sun's light source
			@param sunSphereColour Color to draw the sun sphere itself.
		 */
        void update (
                const Ogre::Vector3& sunDirection,
                const Ogre::ColourValue &sunLightColour,
                const Ogre::ColourValue &sunSphereColour);

		/** Retrieves the latest sun direction.
			@return The sun direction.
		 */
		Ogre::Vector3 getSunDirection () const;

		/** Set the sun direction.
			@param dir The sun direction.
		 */
		void setSunDirection (const Ogre::Vector3 &dir);

		/** Sets the sun sphere colour.
			@param colour The colour used to draw the sun
		 */
		void setSunSphereColour (const Ogre::ColourValue &colour);

		/// Get current sun sphere colour, as set in setSunSphereColour.
		Ogre::ColourValue getSunSphereColour ();

		/** Sets the sunlight colour.
			@param colour The colour used to illuminate the scene.
		 */
		void setSunLightColour (const Ogre::ColourValue &colour);

		/// Get current sun sphere colour, as set in setSunLightColour.
		Ogre::ColourValue getSunLightColour ();

		/// Set diffuse multiplier for light colour
		void setDiffuseMultiplier (const Ogre::ColourValue &diffuse);

		/// Set diffuse multiplier for light colour
		Ogre::ColourValue getDiffuseMultiplier ();

		/// Set specular multiplier for light colour
		void setSpecularMultiplier (const Ogre::ColourValue &specular);

		/// Set specular multiplier for light colour
		Ogre::ColourValue getSpecularMultiplier ();

		/// Set ambient multiplier for light colour
		/// This only works if isManaging
		void setAmbientMultiplier (const Ogre::ColourValue &ambient);

		/// Set ambient multiplier for light colour
		Ogre::ColourValue getAmbientMultiplier ();

		/// This can make this class manage the ambient light of the scene.
		void setManageAmbientLight (bool manage);

		/// If the class is managing the scene's ambient light.
		bool isManagingAmbientLight ();

	private:
		/// Creates the sun material.
		void createSunMaterial ();

		/// Destroys the sun material.
		void destroySunMaterial ();

    public:
		/// Handle camera change.
		virtual void notifyCameraChanged (Ogre::Camera *cam);

    protected:
        /// Handle far radius.
	    virtual void setFarRadius (Ogre::Real radius);
};

} // namespace caelum

#endif //SUN_H
