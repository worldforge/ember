/*
 Copyright (C) 2010 Erik Hjortsberg <erik.hjortsberg@gmail.com>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef EMBEROGRE_WORLD_H_
#define EMBEROGRE_WORLD_H_

#include "GraphicalChangeAdapter.h"

#include "OgreIncludes.h"
#include <sigc++/signal.h>
#include <sigc++/trackable.h>
#include <sigc++/connection.h>
#include <string>
#include <vector>
#include <set>

namespace WFMath
{
template<int>
class AxisBox;
}

namespace Eris
{
class View;
class Connection;
class Entity;
class TimedEvent;
class Calendar;
class EventService;
}

namespace varconf
{
class Variable;
}

namespace Ember
{
class Input;
class ConfigListenerContainer;
class EmberEntity;

namespace EntityMapping {
class EntityMappingManager;
}
namespace OgreView
{

class RenderDistanceManager;

namespace Terrain
{
class TerrainManager;
class TerrainPage;
}

namespace Camera
{
class MainCamera;
}

namespace Authoring
{
class AuthoringManager;
class AuthoringMoverConnector;
class EntityMoveManager;
}

namespace Environment
{
class Environment;
class Foliage;
class FoliageDetailManager;
}

namespace Lod
{
class LodLevelManager;
}

class Avatar;
class MovementController;
class EmberEntityFactory;
class MotionManager;
class Scene;
class EmberOgreSignals;
class ICameraMotionHandler;
class IPageDataProvider;
class EntityWorldPickListener;
class TerrainEntityManager;
class DelayedFoliageInitializer;
class ShaderManager;
class AvatarCameraWarper;

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief Binds together an Eris::View instance with a Scene (and hence Ogre::SceneManager) instance.
 *
 * This is the base unit for representing an Eris::View instance graphically in the Ogre component of EmberOgre.
 * An instance of this is created when an Eris::View instance is available, and destroyed along with it. It's responsibility is to handle the Ogre scene (along with a main camera, movement controllers etc.).
 * It in many ways acts as a hub through which many other subsystems can access both the Ogre and the Eris state of the world.
 */
class World: public virtual sigc::trackable
{
public:

	/**
	 * @brief Ctor.
	 * @param view The View instance which represents the world.
	 * @param renderWindow The main render window used to the render the world.
	 * @param signals An EmberOgre signals instance, used for emitting signals for external components to listen to.
	 * @param input The main input instance.
	 * @param shaderManager The main shader manager.
	 * @param graphicalChangeAdapeter
	 * @param entityMappingManager Manager responsible for creating entity mappings.
	 */
	World(Eris::View& view, Ogre::RenderWindow& renderWindow, EmberOgreSignals& signals, Input& input, ShaderManager& shaderManager, GraphicalChangeAdapter& graphicalChangeAdapter, EntityMapping::EntityMappingManager& entityMappingManager);

	/**
	 * @brief Dtor.
	 * During destruction the Scene along with many other components owned by this class will be deleted.
	 */
	virtual ~World();

	/**
	 * @brief
	 */
	Scene& getScene() const;

	/**
	 * @brief Gets the scene manager which is used to represent this world.
	 * The scene manager is held by a Scene instance and will be destroyed when the World is destroyed.
	 *
	 * @returns The Ogre::SceneManager instance which is used to represent the server side world.
	 */
	Ogre::SceneManager& getSceneManager() const;

	/**
	 * @brief Gets the Eris view of the server side world.
	 *
	 * @returns The Eris view of the server side world.
	 */
	Eris::View& getView() const;

	Eris::EventService& getEventService() const;

	/**
	 * @brief Gets the main avatar instance.
	 *
	 * @return The main avatar instance.
	 */
	Avatar* getAvatar() const;

	/**
	 * @brief Gets the motion manager, which is responsible for handling all motion and animations of entities.
	 *
	 * @return The motion manager.
	 */
	MotionManager& getMotionManager() const;

	/**
	 * @brief Gets the entity factory which is responsible for creating all new entities in the world.
	 *
	 * @return The entity factory.
	 */
	EmberEntityFactory& getEntityFactory() const;

	/**
	 * @brief Gets the main camera.
	 *
	 * @return The main camera.
	 */
	Camera::MainCamera& getMainCamera() const;

	/**
	 * @brief Gets the avatar controller, which receives input and makes sure that the Avatar is moved.
	 *
	 * @return The avatar controller.
	 */
	MovementController* getMovementController() const;

	/**
	 * @brief Gets the entity move manager, which handles movement of entities in the world by the user.
	 *
	 * @return The entity move manager.
	 */
	Authoring::EntityMoveManager& getMoveManager() const;

	/**
	 * @brief Finds and returns the entity with the given id, if it's available.
	 *
	 * @param id The unique id for the entity.
	 * @return An instance of EmberEntity or null if no entity with the specified id could be found.
	 */
	EmberEntity* getEmberEntity(const std::string& eid) const;

	/**
	 * @brief Gets the world entity pick listener, which listens to pick events for entities.
	 *
	 * @returns The main entity pick listener.
	 */
	EntityWorldPickListener& getEntityPickListener() const;

	/**
	 * @brief Accessor for the authoring manager.
	 * @return The authoring manager.
	 */
	Authoring::AuthoringManager& getAuthoringManager() const;

	/**
	 * @brief Gets the main Environment object of the world.
	 * @return An environment object, or null if none has been created yet.
	 */
	Environment::Environment* getEnvironment() const;

	/**
	 * @brief Gets the main Foliage object of the world. Note that the initialization of the foliage might be delayed.
	 * @return A foliage object, or null if none created.
	 */
	Environment::Foliage* getFoliage() const;

	/**
	 * @brief Accessor for the terrain manager.
	 * @returns The terrain manager.
	 */
	Terrain::TerrainManager& getTerrainManager() const;

	/**
	 * @brief Gets the level of detail manager.
	 * @returns The LOD level manager.
	 */
	Lod::LodLevelManager& getLodLevelManager() const;

	/**
	 * @brief Gets the render distance manager.
	 * @return The render distance manager.
	 * @see RenderDistanceManager
	 */
	RenderDistanceManager& getRenderDistanceManager() const;

	/**
	 * @brief Accessor for the main server calendar.
	 * @return The main server calendar.
	 */
	Eris::Calendar& getCalendar() const;

	/**
	 * @brief Emitted when the world has received the avatar entity.
	 */
	sigc::signal<void> EventGotAvatar;

	/**
	 * @brief Emitted when the foliage has been created.
	 */
	sigc::signal<void> EventFoliageCreated;

protected:

	/**
	 * @brief The Eris View instance which represents the world.
	 */
	Eris::View& mView;

	/**
	 * @brief The main render window instance.
	 * The client will have many different render targets, but this is the main render window.
	 */
	Ogre::RenderWindow& mRenderWindow;

	/**
	 * @brief An EmberOgre signals instance, used for emitting signals for external components to listen to.
	 */
	EmberOgreSignals& mSignals;

	/**
	 * @brief The scene instance, which handles the Ogre::SceneManager.
	 */
	Scene* mScene;

	/**
	 * @brief The main viewport, attached to the main RenderWindow.
	 */
	Ogre::Viewport* mViewport;

	/**
	 * @brief The user avatar
	 */
	Avatar* mAvatar;

	/**
	 * @brief When connected to a world, handles the avatar and patches mouse and keyboard movement events on the avatar.
	 */
	MovementController* mMovementController;

	/**
	 * @brief The terrain manager, owned by this instance.
	 */
	Terrain::TerrainManager* mTerrainManager;

	/**
	 * @brief The main camera, which handles input to be shown in the main viewpoint.
	 */
	Camera::MainCamera* mMainCamera;

	/**
	 * @brief Responsible for allowing movement of entities in the world by the user.
	 */
	Authoring::EntityMoveManager* mMoveManager;

	/**
	 * @brief Main factory for all entities created in the world.
	 */
	EmberEntityFactory* mEmberEntityFactory;

	/**
	 * @brief Responsible for updating motions and animations of entities.
	 */
	MotionManager* mMotionManager;

	/**
	 * @brief The main motion handler for the avatar camera.
	 */
	ICameraMotionHandler* mAvatarCameraMotionHandler;

	/**
	 * @brief Optional movement adjuster used if the avatar is an admin.
	 *
	 * This allows the player to let the avatar follow the camera (which helps when authoring).
	 */
	AvatarCameraWarper* mAvatarCameraWarper;

	/**
	 * @brief The entity world pick listener instance, which handles picking of entities in the world.
	 */
	EntityWorldPickListener* mEntityWorldPickListener;

	/**
	 * @brief The authoring manager, used for displaying authoring markers.
	 */
	Authoring::AuthoringManager* mAuthoringManager;

	/**
	 * @brief Connects the authoring manager to the entity mover manager, so that the authoring info is updated as entities are moved.
	 */
	Authoring::AuthoringMoverConnector* mAuthoringMoverConnector;

	TerrainEntityManager* mTerrainEntityManager;

	/**
	 * @brief The lod level manager, owned by this instance, used to adjust the level of detail of materials and meshes.
	 */
	Lod::LodLevelManager* mLodLevelManager;

	/**
	 * @brief The render distance manager, owned by this instance, used to adjust the absolute rendering distance of the world.
	 */
	RenderDistanceManager* mRenderDistanceManager;

	/**
	 * @brief The page data provider for the terrain rendering system.
	 */
	IPageDataProvider* mPageDataProvider;

	/**
	 * @brief The foliage system which provides different foliage functions.
	 */
	Environment::Foliage* mFoliage;

	/**
	 * Utility object that can be used to manage detail level of foliage.
	 */
	Environment::FoliageDetailManager* mFoliageDetailManager;

	/**
	 * @brief A foliage initializer which acts a bit delayed.
	 */
	DelayedFoliageInitializer* mFoliageInitializer;

	/**
	 * @brief The main environment object. There should only be one in the system, and it's kept here.
	 */
	Environment::Environment* mEnvironment;

	/**
	 * @brief Listens for configuration changes.
	 */
	ConfigListenerContainer* mConfigListenerContainer;

	/**
	 * @brief Represents the server calendar.
	 */
	Eris::Calendar* mCalendar;

	/**
	 * @brief Keeps track of the after terrain update connection.
	 *
	 * Mainly so that it can be severed during shutdown.
	 */
	sigc::connection mAfterTerrainUpdateConnection;

	/**
	 * @brief Listens to updates to the terrain and makes sure that entities are adjusted.
	 * @param areas The areas that were changed.
	 * @param pages The pages that were changed.
	 */
	void terrainManager_AfterTerrainUpdate(const std::vector<WFMath::AxisBox<2>>& areas, const std::set<Terrain::TerrainPage*>& pages);

	/**
	 * @brief Sent from the view when an avatar entity has been created.
	 *
	 * We'll listen to this and setup the cameras etc. when this happens.
	 * @param entity The avatar entity.
	 */
	void View_gotAvatarCharacter(Eris::Entity* entity);

	/**
	 * @brief Called when the avatar entity is being deleted.
	 *
	 * This means that we should clean up our avatar objects.
	 */
	void avatarEntity_BeingDeleted();

	/**
	 * @brief Updates the position of the entity, and all it's children, as long as it's within the changed areas.
	 * @param entity The entity to update.
	 * @param areas The areas which have been updated.
	 */
	void updateEntityPosition(EmberEntity* entity, const std::vector<WFMath::AxisBox<2>>& areas);

	/**
	 * @brief Listen to changes the "graphics:foliage" config element, and create or destroy the foliage accordingly.
	 * @param section
	 * @param key
	 * @param variable
	 */
	void Config_Foliage(const std::string& section, const std::string& key, varconf::Variable& variable, GraphicalChangeAdapter& graphicalChangeAdapter);

	/**
	 * @brief Initializes foliage.
	 * @param graphicalChangeAdapter An adapter for graphical change events.
	 */
	void initializeFoliage(GraphicalChangeAdapter& graphicalChangeAdapter);

};

/**
 @brief Allows for a delayed initialization of the foliage.

 The initialization will occur when either the sight queue is empty, or a certain time has elapsed.
 The main reason for doing this is that whenever a new area is added to the world, the foliage is invalidated and reloaded.
 As a result when the user first enters the world and is getting sent all the surrounding entities, there's a great chance that some of these entities will be areas. If the foliage then already has been initialized it will lead to the foliage being reloaded a couple of time.
 By delaying the loading of the foliage we can avoid this.

 @author Erik Hjortsberg <erik@worldforge.org>

 */
class DelayedFoliageInitializer
{
public:
	/**
	 * @brief Ctor.
	 * @param callback The callback to call when the foliage should be initialized.
	 * @param view The Eris::View object of the world. This will be used for querying about the size of the Sight queue.
	 * @param intervalMs In milliseconds how often to check if the queue is empty or time has elapsed. Defaults to 1 second.
	 * @param maxTimeMs In milliseconds the max time to wait until we initialize the foliage anyway.
	 */
	DelayedFoliageInitializer(sigc::slot<void> callback, Eris::View& view, unsigned int intervalMs = 1000, unsigned int maxTimeMs = 15000);

	/**
	 * @brief Dtor.
	 */
	virtual ~DelayedFoliageInitializer();

protected:
	sigc::slot<void> mCallback;
	Eris::View& mView;
	unsigned int mIntervalMs;
	unsigned int mMaxTimeMs;

	Eris::TimedEvent* mTimeout;
	unsigned int mTotalElapsedTime;

	/**
	 * @brief Called when the time out has expired. We'll check for if either the set max time has elapsed, or if there's no more entities in the sight queue, and if so initialize the foliage. If not we'll just extend the waiting time.
	 */
	void timout_Expired();

};

inline Authoring::EntityMoveManager& World::getMoveManager() const
{
	return *mMoveManager;
}

}
}

#endif /* EMBEROGRE_WORLD_H_ */
