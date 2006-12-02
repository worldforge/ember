#ifndef SUN_H
#define SUN_H

#include "CaelumPrerequisites.h"

namespace caelum {

/** Class representing the sun.
	It calculates the sun light direction, position, colour...
	@author Jesús Alonso Abad
	@version 0.1
 */
class DllExport Sun : public Ogre::RenderTargetListener {
// Attributes -----------------------------------------------------------------
	public:
		/** Name of the sun material.
		 */
		static const Ogre::String SUN_MATERIAL_NAME;

	private:
		/** The sun trajectory inclination. This is similar to the earth's rotation axis inclination.
		 */
		Ogre::Degree mInclination;

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

// Methods --------------------------------------------------------------------
	public:
		/** Constructor.
			@param sceneMgr The scene manager where the lights will be created.
		 */
		Sun (Ogre::SceneManager *sceneMgr);

		/** Destructor.
		 */
		~Sun ();

		/** Event trigger called just before rendering a viewport in a render target this sun is attached to.
			Useful to make the sun follow every camera that renders a viewport in a certain render target.
			@param e The viewport event, containing the viewport (and camera) to be rendered right now.
		 */
		void preViewportUpdate (const Ogre::RenderTargetViewportEvent &e);

		/** Updates the sun position according to the local time.
			@param time The local day time.
		 */
		void update (const float time);

		/** Changes the sun inclination.
			@param inc The inclination in degrees.
		 */
		void setInclination (Ogre::Degree inc);

		/** Gets the sun inclination.
			@return The sun inclination in degrees.
		 */
		Ogre::Degree getInclination () const;

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

		/** Gets the sun node.
			@return The sun node.
		 */
		inline Ogre::SceneNode* getNode() const;

	private:
		/** Creates the sun material.
		 */
		void createSunMaterial ();

		/** Destroys the sun material.
		 */
		void destroySunMaterial ();
};

Ogre::SceneNode* Sun::getNode() const
{
	return mSunNode;
}

} // namespace caelum

#endif //SUN_H
