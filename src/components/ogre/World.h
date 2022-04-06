/*
 Copyright (C) 2010 Erik Ogenvik <erik@ogenvik.org>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
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

namespace WFMath {
template<int>
class AxisBox;
}

namespace Eris {
class View;

class Connection;

class Entity;

class TimedEvent;

class Calendar;

class EventService;
}

namespace varconf {
class Variable;
}

namespace Ember {
class Input;

class ConfigListenerContainer;

class EmberEntity;

namespace EntityMapping {
class EntityMappingManager;
}
namespace OgreView {

class RenderDistanceManager;

namespace Terrain {
class TerrainManager;

class TerrainPage;

class TerrainPageGeometry;
}

namespace Camera {
class MainCamera;
}

namespace Authoring {
class AuthoringManager;

class AuthoringMoverConnector;

class EntityMoveManager;
}

namespace Environment {
class Environment;
}

namespace Lod {
class LodLevelManager;
}

class Avatar;

class MovementController;

class EmberEntityFactory;

class MotionManager;

class Scene;

struct EmberOgreSignals;

struct ICameraMotionHandler;

struct IPageDataProvider;

class EntityWorldPickListener;

class TerrainEntityManager;

class ShaderManager;

class AvatarCameraWarper;

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief Binds together an Eris::View instance with a Scene (and hence Ogre::SceneManager) instance.
 *
 * This is the base unit for representing an Eris::View instance graphically in the Ogre component of EmberOgre.
 * An instance of this is created when an Eris::View instance is available, and destroyed along with it. It's responsibility is to handle the Ogre scene (along with a main camera, movement controllers etc.).
 * It in many ways acts as a hub through which many other subsystems can access both the Ogre and the Eris state of the world.
 */
class World : public virtual sigc::trackable {
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
    World(Eris::View& view,
          Ogre::RenderWindow& renderWindow,
          EmberOgreSignals& signals,
          Input& input,
          ShaderManager& shaderManager,
          GraphicalChangeAdapter& graphicalChangeAdapter,
          EntityMapping::EntityMappingManager& entityMappingManager);

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
     * @brief Represents the server calendar.
     */
    std::unique_ptr<Eris::Calendar> mCalendar;

    /**
     * @brief The scene instance, which handles the Ogre::SceneManager.
     */
    std::unique_ptr<Scene> mScene;

    /**
     * @brief The main viewport, attached to the main RenderWindow.
     */
    Ogre::Viewport* mViewport;

    /**
     * @brief The user avatar
     */
    std::unique_ptr<Avatar> mAvatar;

    /**
     * @brief When connected to a world, handles the avatar and patches mouse and keyboard movement events on the avatar.
     */
    std::unique_ptr<MovementController> mMovementController;

    /**
     * @brief The terrain manager, owned by this instance.
     */
    std::unique_ptr<Terrain::TerrainManager> mTerrainManager;

    /**
     * @brief The main camera, which handles input to be shown in the main viewpoint.
     */
    std::unique_ptr<Camera::MainCamera> mMainCamera;

    /**
     * @brief Responsible for allowing movement of entities in the world by the user.
     */
    std::unique_ptr<Authoring::EntityMoveManager> mMoveManager;

    /**
     * @brief Responsible for updating motions and animations of entities.
     */
    std::unique_ptr<MotionManager> mMotionManager;

    /**
     * @brief The main motion handler for the avatar camera.
     */
    std::unique_ptr<ICameraMotionHandler> mAvatarCameraMotionHandler;

    /**
     * @brief Optional movement adjuster used if the avatar is an admin.
     *
     * This allows the player to let the avatar follow the camera (which helps when authoring).
     */
    std::unique_ptr<AvatarCameraWarper> mAvatarCameraWarper;

    /**
     * @brief The entity world pick listener instance, which handles picking of entities in the world.
     */
    std::unique_ptr<EntityWorldPickListener> mEntityWorldPickListener;

    /**
     * @brief The authoring manager, used for displaying authoring markers.
     */
    std::unique_ptr<Authoring::AuthoringManager> mAuthoringManager;

    /**
     * @brief Connects the authoring manager to the entity mover manager, so that the authoring info is updated as entities are moved.
     */
    std::unique_ptr<Authoring::AuthoringMoverConnector> mAuthoringMoverConnector;

    std::unique_ptr<TerrainEntityManager> mTerrainEntityManager;

    /**
     * @brief The lod level manager, owned by this instance, used to adjust the level of detail of materials and meshes.
     */
    std::unique_ptr<Lod::LodLevelManager> mLodLevelManager;

    /**
     * @brief The page data provider for the terrain rendering system.
     */
    std::unique_ptr<IPageDataProvider> mPageDataProvider;

    /**
     * @brief The main environment object. There should only be one in the system, and it's kept here.
     */
    std::unique_ptr<Environment::Environment> mEnvironment;

    /**
     * @brief The render distance manager, owned by this instance, used to adjust the absolute rendering distance of the world.
     */
    std::unique_ptr<RenderDistanceManager> mRenderDistanceManager;


    /**
     * @brief Listens for configuration changes.
     */
    std::unique_ptr<ConfigListenerContainer> mConfigListenerContainer;

    /**
     * @brief Keeps track of the after terrain update connection.
     *
     * Mainly so that it can be severed during shutdown.
     */
    sigc::connection mAfterTerrainUpdateConnection;

    EmberEntity* mTopLevelEntity;

    /**
	 * @brief Listens to updates to the terrain and makes sure that entities are adjusted.
	 */
    void terrainManager_AfterTerrainUpdate(const std::vector<WFMath::AxisBox<2>>& areas,
										   const std::vector<std::shared_ptr<Terrain::TerrainPageGeometry>>&);

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

    void View_topLevelEntityChanged();

};


inline Authoring::EntityMoveManager& World::getMoveManager() const {
    return *mMoveManager;
}

}
}

#endif /* EMBEROGRE_WORLD_H_ */
