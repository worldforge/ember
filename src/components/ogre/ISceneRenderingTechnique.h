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

#ifndef ISCENERENDERINGTECHNIQUE_H_
#define ISCENERENDERINGTECHNIQUE_H_

namespace EmberOgre
{
class EmberEntity;


/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
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
	virtual ~ISceneRenderingTechnique(){}

	/**
	 * @brief Registers an entity with this rendering technique.
	 * @param entity The entity to register.
	 */
	virtual void registerEntity(EmberEntity& entity) = 0;
};

}
#endif /* ISCENERENDERINGTECHNIQUE_H_ */
