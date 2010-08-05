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

#include "OgreIncludes.h"
#include <string>

namespace Eris
{
class View;
class Connection;
class Entity;
}

namespace EmberOgre {

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

class Avatar;
class MovementController;
class EmberEntityFactory;
class MotionManager;
class Scene;
class EmberOgreSignals;
class EmberEntity;
class ICameraMotionHandler;
class EntityWorldPickListener;

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief Binds together an Eris::View instance with a Scene (and hence Ogre::SceneManager) instance.
 *
 * This is the base unit for representing an Eris::View instance graphically in the Ogre component of EmberOgre.
 * An instance of this is created when an Eris::View instance is available, and destroyed along with it. It's responsibility is to handle the Ogre scene (along with a main camera, movement controllers etc.).
 * It in many ways acts as a hub through which many other subsystems can access both the Ogre and the Eris state of the world.
 */
class World
{
public:

	/**
	 * @brief Ctor.
	 * @param view The View instance which represents the world.
	 * @param renderWindow The main render window used to the render the world.
	 * @signals An EmberOgre signals instance, used for emitting signals for external components to listen to.
	 */
	World(Eris::View& view, Ogre::RenderWindow& renderWindow, EmberOgreSignals& signals);

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
};


inline Authoring::EntityMoveManager& World::getMoveManager() const
{
	return *mMoveManager;
}


}
#endif /* EMBEROGRE_WORLD_H_ */
