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

#ifndef EMBEROGRE_SCENE_H_
#define EMBEROGRE_SCENE_H_

#include "OgreIncludes.h"
#include <map>
#include <string>

namespace EmberOgre
{
class EmberEntity;
class ISceneRenderingTechnique;

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief Represents the main scene of the world, where entities are placed.
 */
class Scene
{
public:
	/**
	 * @brief Ctor.
	 * @param sceneManager The scene manager which represents the world.
	 */
	Scene();

	/**
	 * @brief Dtor.
	 */
	virtual ~Scene();

	/**
	 * @brief Accessor for the scene manager which handles the world.
	 * @returns The scene manager for the world.
	 */
	Ogre::SceneManager& getSceneManager() const;

	/**
	 * @brief Registers an entity with a named technique.
	 * If no technique exists, nothing will happen.
	 * @param entity The entity we want to register with a specified technique.
	 * @param technique The name of the rendering technique.
	 */
	void registerEntityWithTechnique(EmberEntity& entity, const std::string& technique);

	/**
	 * @brief Adds a new named technique.
	 * @param name The name of the technique.
	 * @param technique The rendering technique.
	 */
	void addRenderingTechnique(const std::string& name, ISceneRenderingTechnique* technique);

	/**
	 * @brief Removes a technique.
	 * @param name The name of the technique to remove.
	 * @returns A pointer to the technique if one such is found, else a null pointer.
	 */
	ISceneRenderingTechnique* removeRenderingTechnique(const std::string& name);

	/**
	 * @brief Gets the main camera of the scene.
	 *
	 * Each scene has one main camera used to render it. There can of course be many more cameras.
	 * @returns The main camera.
	 */
	Ogre::Camera& getMainCamera() const;

protected:
	typedef std::map<std::string, ISceneRenderingTechnique*> RenderingTechniqueStore;

	/**
	 * @brief The scene manager which manages the main world.
	 */
	Ogre::SceneManager* mSceneManager;

	/**
	 * @brief The main camera used to render the world.
	 * There can be many different cameras in the world, but this is the main one used by most components.
	 */
	Ogre::Camera* mMainCamera;

	/**
	 * @brief A store of rendering techniques.
	 */
	RenderingTechniqueStore mTechniques;

};

}

#endif /* SCENE_H_ */
