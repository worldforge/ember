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

#ifndef CAELUMSYSTEM_H
#define CAELUMSYSTEM_H

#include "CaelumPrerequisites.h"

#include "UniversalClock.h"
#include "SkyColourModel.h"
#include "SkyDome.h"
#include "Starfield.h"
#include "LayeredClouds.h"
#include "SolarSystemModel.h"
#include "GroundFog.h"
#include "Sun.h"

namespace caelum {

/** Root of the Caelum system.
    
    Caelum is built from several classes for different sky elements (the sun,
    clouds, etc). Those classes know very little about each other and are 
    connected through the main CaelumSystem class. This class is responsible
    for tracking and updating sub-components. It "owns" all of the components,
    using std::auto_ptr members.

    The constructor will create a standard set of components but you can
    disable some or change others. When you do something like setXxx(new Xxx())
    CaelumSystem takes control of the object's lifetime.

    This class is also reponsible for doing all per-frame and per-RenderTarget
    updates. It's better to keep that logic here instead of coupling components
    together.

    It's difficult to build a CaelumSystem class which will work for any
    combination of sky elements. It might be a good idea to have different
    classes for vastly different sky systems. Alternatively the update logic
    from Caelum could be refactor to only transfer a number of "common sky
    parameter" around; but that is a lot harder and ultimately less flexible.

	@author Jes�s Alonso Abad
 */
class DllExport CaelumSystem : public Ogre::FrameListener, public Ogre::RenderTargetListener {
// Attributes -----------------------------------------------------------------
	private:
		/** Root of the Ogre engine.
		 */
		Ogre::Root *mOgreRoot;

		/** Scene manager.
		 */
		Ogre::SceneManager *mSceneMgr;

		/** Caelum root scene node.
		 */
		Ogre::SceneNode *mCaelumRootNode;

		/** Cleanup requested flag.
		 */
		bool mCleanup;

		/** Flag to let Caelum manage the creation and destruction of the resource group.
		 */
		bool mManageResourceGroup;

		/// Reference to the universal clock.
		UniversalClock *mUniversalClock;

        /// Flag to indicate if Caelum manages standard Ogre::Scene fog.
		bool mManageSceneFog;

        /// Global fog density multiplier.
        double mGlobalFogDensityMultiplier;

        /// Scene fog density multiplier.
        double mSceneFogDensityMultiplier;

        /// Ground fog density multiplier.
        double mGroundFogDensityMultiplier;
		
		/// Reference to the sky colour model, if enabled.
        std::auto_ptr<SkyColourModel> mSkyColourModel;
        
		/// Reference to the sky dome, if enabled.
        std::auto_ptr<SkyDome> mSkyDome;

		/// Reference to the solar system model, if enabled.
        std::auto_ptr<SolarSystemModel> mSolarSystemModel;
        
		/// Reference to the sun, if enabled.
        std::auto_ptr<BaseSun> mSun;

		/// Reference to the starfield, if enabled.
        std::auto_ptr<Starfield> mStarfield;

		/// Reference to the clouds, if enabled.
        std::auto_ptr<LayeredClouds> mClouds;

		/// Reference to ground fog, if enabled.
        std::auto_ptr<GroundFog> mGroundFog;

// Methods --------------------------------------------------------------------
	public:
        /** Flags enumeration for caelum components.
         *  This is an enumeration for the components to create by default in
         *  Caelum's constructor. You can still pass 0 and create everything
         *  by hand.
         * 
         *  CaelumSystem's constructor used to take a number of bools but now
         *  there are too many components and this is nicer.
         * 
         *  CAELUM_COMPONENT_ members are for individual components.
         *  CAELUM_COMPONENTS_ are standard bitmasks.
         *  CAELUM_COMPONENTS_DEFAULT picks elements that don't require
         *  modifications to external materials (right now it excludes ground fog).
         */
        enum CaelumComponent
        {
            CAELUM_COMPONENT_SKY_COLOUR_MODEL   = 1 << 0,
            CAELUM_COMPONENT_SKY_DOME           = 1 << 1,
            CAELUM_COMPONENT_SOLAR_SYSTEM_MODEL = 1 << 2,
            CAELUM_COMPONENT_SUN                = 1 << 3,
            // TODO: CAELUM_COMPONENT_MOON      = 1 << 4,
            CAELUM_COMPONENT_STARFIELD          = 1 << 5,
            CAELUM_COMPONENT_CLOUDS             = 1 << 6,

            // These have nasty dependencies on materials.
            CAELUM_COMPONENT_GROUND_FOG         = 1 << (16 + 0),
            // TODO: CAELUM_COMPONENT_HAZE      = 1 << (16 + 1),

            CAELUM_COMPONENTS_NONE              = 0x00000000,
            CAELUM_COMPONENTS_DEFAULT           = 0x0000007F,
            CAELUM_COMPONENTS_ALL               = 0x0002007F,
        };
    
		/** Constructor.
			Registers itself in the Ogre engine and initialises the system.
            It can also initialize a bunch of default components.

			@param root The Ogre root.
			@param scene The Ogre scene manager.
            @param compoment The components to create.
			@param manageResGroup Tells the system if the resource group has
            been created externally (true) or if it's to be managed by the system.
			@param resGroupName The resource group name, if it's desired to
            use an existing one or just a different name.
		 */
		CaelumSystem (
                Ogre::Root *root, 
				Ogre::SceneManager *sceneMgr, 
				CaelumComponent componentsToCreate = CAELUM_COMPONENTS_DEFAULT, 
				bool manageResGroup = true, 
				const Ogre::String &resGroupName = RESOURCE_GROUP_NAME
        );

		/** Destructor.
		 */
		~CaelumSystem ();

		/** Shuts down the system and detaches itself from the Ogre engine.
         *
         *  shutdown(true) is equivalent to deleting CaelumSystem yourself.
         *  shutdown(false) delays destruction to the next time caelum is called as
         *  a frame listener. This makes it safe to shutdown Caelum from inside
         *  another frame listener.
         *
         *  @param cleanup If this is true then detach and destroy the CaelumSystem instantly.
		 */
		void shutdown (bool cleanup);
		
		/** Gets the universal clock.
		 * @return A reference to the universal clock attached to this system.
		 */
		UniversalClock *getUniversalClock () const;
		
		/** Gets caelum root scene node.
		 */
        Ogre::SceneNode* getRootNode(void) const { return mCaelumRootNode; }

        /** Update the whole system manually. You have to call this if you don't
         *  register CaelumSystem as an ogre frame listener.
         *  @param timeSinceLastFrame: Time passed since last frame.
         */
        void updateSubcomponents (double timeSinceLastFrame);

		/** Set the skydome.
         *  @param dome A new dome or null to disable.
		 */
        inline void setSkyDome (SkyDome *dome) {
            mSkyDome.reset(dome);
        }

		/** Returns the current sky dome.
		 */
        SkyDome *getSkyDome () const {
            return mSkyDome.get();
        }

		/** Set the sun.
         *  @param sun A new sun or null to disable.
		 */
        inline void setSun (BaseSun* sun) {
            mSun.reset(sun);
        }

		/** Gets the current sun.
			@return The sun in use.
		 */
        BaseSun* getSun () const {
            return mSun.get();
        }

		/** Set the starfield.
         *  @param starfield A new starfield or null to disable.
         */
        inline void setStarfield (Starfield* starfield) {
            mStarfield.reset(starfield);
        }

		/** Gets the current starfield.
         */
        inline Starfield* getStarfield () const {
            return mStarfield.get();
        }

		/** Set the cloud system
         *  @param clouds A new cloud system or null to disable.
         */
        inline void setClouds (LayeredClouds* clouds) {
            mClouds.reset(clouds);
        }

		/** Get the current cloud system.
         */
        inline LayeredClouds* getClouds () const {
            return mClouds.get();
        }

		/** Sets the sky colour model to be used.
		 *	@param model The sky colour model, or null to disable
		 */
        inline void setSkyColourModel (SkyColourModel *model) {
            mSkyColourModel.reset(model);
        }

		/** Get the current sky colour model.
		 */
        inline SkyColourModel* getSkyColourModel () const {
            return mSkyColourModel.get();
        }

		/// Set the solar system model to use, or null to disable.
        inline void setSolarSystemModel (SolarSystemModel *model) {
            mSolarSystemModel.reset(model);
        }

		/// Set the current solar system model.
        inline SolarSystemModel* getSolarSystemModel () const {
            return mSolarSystemModel.get();
        }

		/** Sets ground fog system.
		 *	@param model The sky colour model, or null to disable
		 */
        inline void setGroundFog (GroundFog *model) {
            mGroundFog.reset(model);
        }

		/** Get ground fog; if any.
		 */
        inline GroundFog* getGroundFog () const {
            return mGroundFog.get();
        }

		/** Enables/disables Caelum managing standard Ogre::Scene fog.
            This makes CaelumSystem control standard Ogre::Scene fogging. It
            will use EXP2 fog with density from SkyColourModel.

            Fog density multipliers are used; final scene fog density is:
            SceneMultiplier * GlobalMultiplier * SkyColourModel.GetFogDensity

            When this is set to false it also disables all scene fog (but you
            control it afterwards).

            @param value New value
		 */
		void setManageSceneFog (bool value);

		/** Tells if Caelum is managing the fog or not.
			@return The value set in setManageSceneFog.
		 */
		bool getManageSceneFog () const;

        /** Multiplier for scene fog density (default 1).
            This is an additional multiplier for Ogre::Scene fog density.
            This has no effect if getManagerSceneFog is false.

            Final scene fog density is:
            SceneMultiplier * GlobalMultiplier * SkyColourModel.GetFogDensity
         */
        void setSceneFogDensityMultiplier (double value);

        /** Get the value set by setSceneFogDensityMultiplier.
         */
        double getSceneFogDensityMultiplier () const;

        /** Multiplier for ground fog density (default 1).
            This is an additional multiplier for Caelum::GroundFog density.
            This has no effect if GroundFog is not used.

            Final ground fog density is:
            GroundFogMultipler * GlobalMultiplier * SkyColourModel.GetFogDensity
         */
        void setGroundFogDensityMultiplier (double value);

        /** Get the value set by setGroundFogDensityMultiplier.
         */
        double getGroundFogDensityMultiplier () const;

        /** Multiplier for global fog density (default 1).
            This is an additional multiplier for fog density as received from
            SkyColourModel. There are other multipliers you can tweak for
            individual kinds of fog; but this is what you should change from
            whatever "game logic" you might have.
         */
        void setGlobalFogDensityMultiplier (double value);

        /** Get the value set by setSceneFogDensityMultiplier.
         */
        double getGlobalFogDensityMultiplier () const;

    protected:
		/** Handle FrameListener::frameStarted to call updateSubcomponents every frame.
         *  If you don't register CaelumSystem as a an ogre frame listener you have to
         *  call updateSubcomponents yourself.
		 */
		virtual bool frameStarted (const Ogre::FrameEvent &e);

		/** Event trigger called just before rendering a viewport in a render target Caelum is attached to.
			Useful to make objects follow every camera that renders a viewport in a certain render target.
			@param e The viewport event, containing the viewport (and camera) to be rendered right now.
		 */
		virtual void preViewportUpdate (const Ogre::RenderTargetViewportEvent &e);

    private:
        /// Free all subcomponents, but not CaelumSystem itself. Can be called multiple times.
        void destroySubcomponents();
};
} // namespace caelum

#endif //CAELUMSYSTEM_H
