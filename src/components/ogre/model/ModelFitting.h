/*
 Copyright (C) 2009 Erik Ogenvik <erik@ogenvik.org>

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

#ifndef ModelFitting_H_
#define ModelFitting_H_

#include <string>

namespace Ember
{
class EmberEntity;
namespace OgreView
{
namespace Model
{
/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief Represents a "model fitting", i.e. an attachment of a child entity to a bone on a model.
 * An instance of this is used to keep track of which entities are attached to which attach points on a model.
 * Note that it's not until attachChild() has been called that there's an entity attached.
 * This is because it's not uncommon for the attribute data which tells that a certain entity should be attached to be available before the actual entity is available.
 */
struct ModelFitting
{
	EmberEntity& mParent;
	std::string mMountPoint;
	std::string mChildEntityId;
	EmberEntity* mChild = nullptr;
};
}
}
}
#endif /* ModelFitting_H_ */
