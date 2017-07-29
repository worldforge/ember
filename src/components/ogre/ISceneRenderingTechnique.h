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

#ifndef ISCENERENDERINGTECHNIQUE_H_
#define ISCENERENDERINGTECHNIQUE_H_

namespace Ember
{
class EmberEntity;
namespace OgreView
{

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief A technique for rending an entity in a scene.
 *
 * The main method for rending entities in a scene is as a Model, but for some entities we want to use alternate rending methods (imposters for example).
 */
class ISceneRenderingTechnique
{
public:
	/**
	 * @brief Dtor.
	 */
	virtual ~ISceneRenderingTechnique()
	{
	}

	/**
	 * @brief Registers an entity with this rendering technique.
	 * @param entity The entity to register.
	 */
	virtual void registerEntity(EmberEntity& entity) = 0;

	/**
	 * @brief Deregisters an entity with this rendering technique.
	 * @param entity The entity to deregister.
	 */
	virtual void deregisterEntity(EmberEntity& entity) = 0;
};

}
}
#endif /* ISCENERENDERINGTECHNIQUE_H_ */
