/*
 Copyright (C) 2009 Erik Hjortsberg <erik.hjortsberg@gmail.com>

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

#ifndef ModelFitting_H_
#define ModelFitting_H_

#include <string>

namespace EmberOgre
{
class EmberEntity;

namespace Model
{
/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief Represents a "model fitting", i.e. an attachment of a child entity to a bone on a model.
 * An instance of this is used to keep track of which entities are attached to which attach points on a model.
 * Note that it's not until attachChild() has been called that there's an entity attached.
 * This is because it's not uncommon for the attribute data which tells that a certain entity should be attached to be available before the actual entity is available.
 */
class ModelFitting
{
public:
	/**
	 * @brief Ctor.
	 * @param parent The parent entity to which other entities can be attached.
	 * @param mountPoint The name of the mount point.
	 * @param childEntityId The id of the child entity which should be attached.
	 * Note that we don't submit the child entity in the constructor. This is done in attachChild() instead. The reason is that it's not certain that the child entity is available at the time of creation.
	 */
	ModelFitting(EmberEntity& parent, const std::string& mountPoint, const std::string& childEntityId);

	/**
	 * @brief Dtor.
	 */
	virtual ~ModelFitting();

	/**
	 * @brief Attaches a child entity to this fitting.
	 * @param entity The child entity to attach.
	 */
	void attachChild(EmberEntity& entity);

	/**
	 * @brief Gets the parent entity.
	 * @return The parent entity.
	 */
	EmberEntity& getParent() const;

	/**
	 * @brief Gets the attached entity.
	 * @return The attached entity, or null if no entity yet is attached.
	 */
	EmberEntity* getChild() const;

	/**
	 * @brief Gets the name of the mount point.
	 * @return The name of the mount point.
	 */
	const std::string& getMountPoint() const;

	/**
	 * @brief Gets the id of the attached entity.
	 * @return The id of the attached entity.
	 */
	const std::string& getChildEntityId() const;

protected:

	EmberEntity& mParent;
	EmberEntity* mChild;
	std::string mMountPoint;
	std::string mChildEntityId;
};
}
}
#endif /* ModelFitting_H_ */
