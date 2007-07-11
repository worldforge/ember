#ifndef SUN_H
#define SUN_H

#include "CaelumPrerequisites.h"
#include "CameraBoundElement.h"
#include "SunPositionModel.h"

namespace caelum {

/** Class representing the sun.
	It calculates the sun light direction, position, colour...
	@author Jesús Alonso Abad
 */
class DllExport Sun : public CameraBoundElement {
// Attributes -----------------------------------------------------------------
	public:
		/** Name of the sun material.
		 */
		static const Ogre::String SUN_MATERIAL_NAME;

	private:
		/** The sun positioning model.
		 */
		SunPositionModel *mSunPositionModel;

		/** The main directional light.
		 */
		Ogre::Light *mMainLight;

		/** The latest normalised sun direction.
		 */
		Ogre::Vector3 mSunDirection;
		
		/** The latest calculated sun colour.
		 */
		Ogre::ColourValue mSunColour;

		/** The sun scene node.
		 */
		Ogre::SceneNode *mSunNode;

		/** The sun entity.
		 */
		Ogre::Entity *mSunEntity;

		/** Reference to the sun material.
		 */
		Ogre::MaterialPtr mSunMaterial;

		/** Base distance of the sun.
		 */
		float mRadius;

// Methods --------------------------------------------------------------------
	public:
		/** Constructor.
			@param sceneMgr The scene manager where the lights will be created.
		 */
		Sun (Ogre::SceneManager *sceneMgr);

		/** Destructor.
			@note If a sun position model is in use, it will be deleted.
		 */
		virtual ~Sun ();

		/** @copydoc CameraBoundElement::notifyCameraChanged().
		 */
		void notifyCameraChanged (Ogre::Camera *cam);

		/** @copydoc CameraBoundElement::setFarRadius().
		 */
		void setFarRadius (float radius);

		/** Updates the sun position according to the local time.
			@param time The local day time.
		 */
		void update (const float time);

		/** Sets the new sun positioning model.
			If already exists one here, it will be returned so that it can be deleted.
			@param model The new sun positioning model to use.
			@return The existing model or 0 if there wasn't any.
		 */
		SunPositionModel *setSunPositionModel (SunPositionModel *model);

		/** Returns the sun position model in use.
			@return The current sun position model.
		 */
		SunPositionModel *getSunPositionModel () const;

		/** Retrieves the latest sun direction.
			@return The sun direction.
		 */
		Ogre::Vector3 getSunDirection () const;

		/** Sets the sun colour.
			@param colour The sun colour.
		 */
		void setSunColour (Ogre::ColourValue colour);
		
		/** Gets the sun colour.
			@return The latest sun colour.
		 */
		Ogre::ColourValue getSunColour ();

	private:
		/** Creates the sun material.
		 */
		void createSunMaterial ();

		/** Destroys the sun material.
		 */
		void destroySunMaterial ();
};

} // namespace caelum

#endif //SUN_H
