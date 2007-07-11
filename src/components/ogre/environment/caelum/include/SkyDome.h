#ifndef SKYDOME_H
#define SKYDOME_H

#include "CaelumPrerequisites.h"
#include "CameraBoundElement.h"
#include "SkyColourModel.h"
#include "Sun.h"

namespace caelum {

/** A sky dome element.
	@author Jesús Alonso Abad
 */
class DllExport SkyDome : public CameraBoundElement {
// Attributes -----------------------------------------------------------------
	protected:
		/** Control scene node.
		 */
		Ogre::SceneNode *mNode;

		/** Name of the spheric dome resource.
		 */
		static const Ogre::String mSphericDomeResourceName;

		/** Name of the dome material.
		 */
		static const Ogre::String SKY_DOME_MATERIAL_NAME;

		/** Reference to the sky dome material.
		 */
		Ogre::MaterialPtr mSkyDomeMaterial;

// Methods --------------------------------------------------------------------
	public:
		/** Constructor (DO NOT USE! Use CaelumSystem::createSkyDome instead).
			@param sceneMgr The scene manager where this sky dome will be created.
		 */
		SkyDome (Ogre::SceneManager *sceneMgr);

		/** Destructor (DO NOT USE! Use CaelumSystem::destroySkyDome instead).
		 */
		virtual ~SkyDome ();

		/** @copydoc CameraBoundElement::notifyCameraChanged().
		 */
		void notifyCameraChanged (Ogre::Camera *cam);

		/** @copydoc CameraBoundElement::setFarRadius().
		 */
		void setFarRadius (float radius);
	
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

		/** Updates the sky dome material to match the local time.
			@param skyColourModel The sky colour model in use.
			@param time The local time.
			@param sun The sun.
		 */
		void updateSkyDomeMaterialTime (SkyColourModel *skyColourModel, float time, Sun *sun);

		/** Returns the sky dome texture unit state.
			@return The texture unit state of the sky dome material.
		 */
		Ogre::TextureUnitState *getTextureUnitState ();

	private:
		/** Internal method to create the sky dome material.
		 */
		void createSkyDomeMaterial ();

		/** Internal method to destroy the sky dome material.
		 */
		void destroySkyDomeMaterial ();
};

} // namespace caelum

#endif //SKYDOME_H
