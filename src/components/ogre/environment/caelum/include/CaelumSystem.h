#ifndef CAELUMSYSTEM_H
#define CAELUMSYSTEM_H

#include "CaelumPrerequisites.h"

#include "CaelumListener.h"
#include "SkyColourModel.h"
#include "SkyDome.h"
#include "Starfield.h"
#include "Sun.h"

namespace caelum {

/** Root of the Caelum system.
	This class is the root of the Caelum system.
	@author Jesús Alonso Abad
	@version 0.1
 */
class DllExport CaelumSystem : public Ogre::FrameListener {
// Attributes -----------------------------------------------------------------
	public:
		/** Name of the dome material.
		 */
		static const Ogre::String SKY_DOME_MATERIAL_NAME;

		/** Name of the starfield material.
		 */
		static const Ogre::String STARFIELD_MATERIAL_NAME;

	private:
		/** Root of the Ogre engine.
		 */
		Ogre::Root *mOgreRoot;

		/** Scene manager.
		 */
		Ogre::SceneManager *mSceneMgr;

		/** List of listeners registered.
		 */
		std::set<CaelumListener *> mListeners;

		/** Updating rate.
		 */
		float mUpdateRate;

		/** Time since the last update.
		 */
		float mTimeSinceLastUpdate;

		/** The time scale factor.
		 */
		float mTimeScale;

		/** The local time in seconds.
		 */
		float mLocalTime;

		/** The total day time in seconds.
		 */
		float mTotalDayTime;

		/** The list of sky domes.
		 */
		std::set<SkyDome *> mSkyDomes;

		/** Reference to the sky dome material.
		 */
		Ogre::MaterialPtr mSkyDomeMaterial;

		/** Reference to the starfield material.
		 */
		Ogre::MaterialPtr mStarfieldMaterial;

		/** Reference to the sky colour model in use.
		 */
		SkyColourModel *mSkyColourModel;

		/** Flag to indicate if Caelum should manage the fog or not.
		 */
		bool mManageFog;

		/** Reference to the sun.
		 */
		Sun *mSun;

		/** Reference to the starfield.
		 */
		Starfield *mStarfield;

// Methods --------------------------------------------------------------------
	public:
		/** Constructor.
			Registers itself in the Ogre engine and initialises the system.
			@param root The Ogre rool.
			@param sceneMgr The Ogre scene manager.
		 */
		CaelumSystem (Ogre::Root *root, Ogre::SceneManager *sceneMgr);

		/** Destructor.
			Shuts down the system and detaches itself from the Ogre engine.
			@remarks The model used in this system <u>won't be deleted here</u>.
		 */
		~CaelumSystem ();

		/** Registers a listener in the system.
			This listener will be called each frame, before and after Caelum does its work.
			@param listener The listener to register.
		 */
		void addListener (CaelumListener *listener);

		/** Unregisters a listener in the system.
			@note The listener must be destroyed out of Caelum.
			@param listener The listener to be removed.
		 */
		void removeListener (CaelumListener *listener);

		/** Registers all the RenderTarget listeners to the given target.
			@param target The render target.
		 */
		void registerAllToTarget (Ogre::RenderTarget *target);

		/** Unregisters all the RenderTarget listeners from the given target.
			@param target The render target.
		 */
		void unregisterAllFromTarget (Ogre::RenderTarget *target);

		/** Sets the updating rate.
			Allows to tell how much <b>relative daytime [0, 1]</b> can pass without being updated. 
			For instance, in a 24hrs day, setting an update rate of 1/24 means that Caelum will update its state each hour. 
			@param rate The updating rate.
		 */
		void setUpdateRate (float rate);

		/** Gets the updating rate.
			@return The updating rate.
		 */
		float getUpdateRate ();

		/** Sets the time scale.
			This scale will determine at which speed the time will advance, relative to the real time.
			@param scale The time scale.
		 */
		void setTimeScale (const float scale);

		/** Gets the time scale.
			@return The time scale.
		 */
		float getTimeScale () const;

		/** Sets the local time.
			@param time The local time in seconds.
		 */
		void setLocalTime (const float time);

		/** Gets the local time.
			@return The local time in seconds.
		 */
		float getLocalTime () const;

		/** Sets the total day time.
			By default the total day time is stablished as 23h 59m 56s.
			@param time The total day time in seconds (values lesser or equal than 0 will be set to the
				default day time).
		 */
		void setTotalDayTime (const float time);

		/** Gets the total day time.
			@return The total day time in seconds.
		 */
		float getTotalDayTime () const;

		/** Sets the sun direction.
			@param dir The sun light direction.
		 */
		void setSunDirection (Ogre::Vector3 dir);

		/** Updates the system.
			@param e The frame event (contains the elapsed time since the last update).
		 */
		bool frameStarted (const Ogre::FrameEvent &e);

		/** Creates a sky dome.
		 */
		SkyDome *createSkyDome ();

		/** Destroys a sky dome.
			@remarks Remember to detach the sky dome from every viewport it is attached to!
			@param skyDome The sky dome to be destroyed.
		 */
		void destroySkyDome (SkyDome *skyDome);

		/** Sets the sky colour model to be used.
			@param model The sky colour model.
		 */
		void setSkyColourModel (SkyColourModel *model);

		/** Enables/disables the Caelum fog management.
			@param manage True if you want Caelum to manage the fog for you.
		 */
		void setManageFog (bool manage);

		/** Tells if Caelum is managing the fog or not.
			@return True if Caelum manages the fog.
		 */
		bool isFogManaged () const;

		/** Gets the current sun.
			@return The sun in use.
		 */
		Sun *getSun () const;

		/** Create the starfield.
			@note Overwrites the previous starfield if there's one.
			@param mapName Name of the starfield texture bitmap.
		 */
		Starfield *createStarfield (const Ogre::String &mapName);

		/** Gets the starfield.
			@return The starfield.
		 */
		Starfield *getStarfield () const;

		/** Destroy the starfield.
		 */
		void destroyStarfield ();

	private:
		/** Fires the start event to all the registered listeners.
			@param e The Ogre FrameEvent object passed this frame.
			@return True if all the listeners returned true.
		 */
		bool fireStartedEvent (const Ogre::FrameEvent &e);

		/** Fires the finish event to all the registered listeners.
			@param e The Ogre FrameEvent object passed this frame.
			@return True if all the listeners returned true.
		 */
		bool fireFinishedEvent (const Ogre::FrameEvent &e);

		/** Collects all the render target listeners in a list.
			@return A list containing all the render target listeners.
		 */
		std::set<Ogre::RenderTargetListener *> *collectAllRenderTargetListeners ();

		/** Clamps the local day time to the total day time range.
		 */
		void clampLocalTime ();

		/** Internal method to create the sky dome material.
		 */
		void createSkyDomeMaterial ();

		/** Internal method to create the starfield material.
		 */
		void createStarfieldMaterial ();

		/** Updates the sky dome material to match the local time.
		 */
		void updateSkyDomeMaterialTime ();
};

} // namespace caelum

#endif //CAELUMSYSTEM_H
