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

#ifndef LAYEREDCLOUDS_H
#define LAYEREDCLOUDS_H

#include "CaelumPrerequisites.h"
#include "CameraBoundElement.h"

namespace caelum {

/** Class for layered clouds.
 *  @note This is tighly integrated with LayeredCloud.cg and LayeredClouds.material.
 *  There are two "cloud mass" layers blended to create animating clouds and an extra
 *  detailLayer.
 *  Most of the parameters in this class are direct wrappers from GPU shader params.
 *
 *  Cloud offsets and speeds are not in any meaningful world units. Maybe they should
 *  be in radians or something?
 */
class DllExport LayeredClouds: public CameraBoundElement {
	public:
		/** Standard constructor.
		 *  @param scene: Scene to add cloud dome to. This class takes control
		 *		of the scene's skydome.
		 *  @param material: Name of the material to load. It would be a lot
		 *		more consistent to generate the material in code.
		 */
		LayeredClouds (
                Ogre::SceneManager* scene,
				Ogre::SceneNode *caelumRootNode,
				const Ogre::String &resourceGroupName="Caelum",
				const Ogre::String &materialName="CaelumLayeredClouds",
				const Ogre::String &meshName="CaelumLayeredClouds",
				const Ogre::String &entityName="CaelumLayeredClouds");

		~LayeredClouds ();

		/** Update cloud material.
		 *  @param timePassed Time passed since last update.
		 *  @param sunDirection Vector towards the sun.
		 *  @param sunColour Colour of the sun.
		 */
		void update (
                Ogre::Real timePassed,
				const Ogre::Vector3& sunDirection,
				const Ogre::ColourValue& sunColour,
				const Ogre::ColourValue& fogColour);

		/** Sets cloud cover, between 0 (completely clear) and 1 (completely covered)
		 *  @param cloudCover Cloud cover between 0 and 1
		 */
		void setCloudCover (const Ogre::Real cloudCover);

		/** Gets the current cloud cover.
		 *  @return Cloud cover, between 0 and 1
		 */
		Ogre::Real getCloudCover () const;

        /** Set the image used to lookup the cloud coverage threshold.
         *  This image is used to calculate the cloud coverage threshold
         *  based on the desired cloud cover.
         *
         *  The cloud coverage threshold is substracted from cloud intensity
         *  at any point; to generate fewer or more clouds. That threshold is
         *  not linear, a lookup is required to ensure that setCloudCover(0.1)
         *  will actually have 10% the clouds at setCloudCover(1).
         *
         *  The lookup is the inverse of the sum on the histogram, and was
         *  calculated with a small hacky tool.
         */
        void setCloudCoverLookup (const Ogre::String& fileName);

        /** Disable any cloud cover lookup.
         *  @see setCloudCoverLookup.
         */
        void disableCloudCoverLookup ();


		/** Sets blending factor between the two cloud mass layers.
		 */
		void setCloudMassBlend (const Ogre::Real cloudCover);

		/** Gets the current blending factor between the two cloud mass layers
		 */
		Ogre::Real getCloudMassBlend () const;

		/** Sets current offset(translation) of the cloud mass on the sky.
		 */
		void setCloudMassOffset (const Ogre::Vector2 &cloudMassOffset);

		/** Gets current offset(translation) of the cloud mass on the sky.
		 */
		Ogre::Vector2 getCloudMassOffset () const;

		/** Sets current offset(translation) of cloud details on the sky.
		 */
		void setCloudDetailOffset (const Ogre::Vector2 &cloudMassOffset);

		/** Gets current offset(translation) of cloud details on the sky.
		 */
		Ogre::Vector2 getCloudDetailOffset () const;


		/** If the cloud layer is animating itself.
		 *  @note If this is true then parameters like cloudMassBlend and offsets
		 *  are set every update().
		 */
		bool isAnimating () const;

		/** Switch internal animation on/off
		 */
		void setAnimating (bool animating);

		/** Sets cloud movement speed.
		 *  @param cloudSpeed Cloud movement speed.
		 */
		void setCloudSpeed (const Ogre::Vector2 &cloudSpeed);

		/** Gets cloud movement speed.
		 *  @param cloudSpeed Cloud movement speed.
		 */
		Ogre::Vector2 getCloudSpeed () const;

		/** Sets the time it takes to blend two cloud shaped together, in seconds.
		 *  @param cloudCover Cloud shape blend time in seconds
		 */
		void setCloudBlendTime (const Ogre::Real cloudBlendTime);

		/** Gets the time it takes to blend two cloud shaped together, in seconds.
		 *  @return Cloud shape blend time in seconds
		 */
		Ogre::Real getCloudBlendTime () const;

	private:
		/// Shortcut function for fragment program parameters
		Ogre::GpuProgramParametersSharedPtr getFpParams();

		/// Shortcut function for fragment program parameters
		Ogre::GpuProgramParametersSharedPtr getVpParams();

		/// Shortcut function for texture unit state parameters
		Ogre::TextureUnitState* getTUS(unsigned short num);
		
		/// Set the sunDirection parameter in the shaders.
		void setSunDirection(const Ogre::Vector3 &sunDirection);

		/// Set the sunColour parameter in the shaders.
		void setSunColour(const Ogre::ColourValue &sunColour);

		/// Set the fogColour parameter in the shaders.
		void setFogColour(const Ogre::ColourValue &fogColour);

		/// Sets inverse Haze height.
		void setInvHazeHeight (const Ogre::Real invHazeHeight);

		/// Current cloud coverage
		Ogre::Real mCloudCover;

		/// Current cloud blend factor
		Ogre::Real mCloudMassBlend;

		/// Current cloud layer offset.
		Ogre::Vector2 mCloudMassOffset;

		/// Current cloud detail layer offset.
		Ogre::Vector2 mCloudDetailOffset;

        /// Lookup used for cloud coverage.
        /// see setCloudCoverLookup.
        std::auto_ptr<Ogre::Image> mCloudCoverLookup;


		/// If this class controls animation for itself.
		bool mAnimating;

		/// Current cloud blend time.
		Ogre::Real mCloudBlendTime;

		/** If true then CloudMassBlend is increased in animation; otherwise it's
		 *  decreased. This is changed each time it reaches the end.
		 */
		bool mCloudBlendAnimationDirection;

		/// Current cloud speed
		Ogre::Vector2 mCloudSpeed;


		/// Cloud material
		Ogre::MaterialPtr mMaterial;

		/// True if selected technique has shaders.
		bool mShadersEnabled;

		/// Cloud plane mesh
		Ogre::SceneNode *mNode;

		/// Cloud plane entity
		Ogre::Entity *mEntity;

		/// Reference to scene manager.
		Ogre::SceneManager *mSceneMgr;

    public:
		/// Handle camera change.
		virtual void notifyCameraChanged (Ogre::Camera *cam);

    protected:
        /// Handle far radius.
	    virtual void setFarRadius (Ogre::Real radius);
};
}  // namespace caelum

#endif // LAYEREDCLOUDS_H
