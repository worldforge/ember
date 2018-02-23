/*
 Copyright (C) 2009 Erik Ogenvik

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

#ifndef IENTITYVISITOR_H_
#define IENTITYVISITOR_H_

namespace Ember
{

class EmberEntity;

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief A visitor interface for traversing the entity hierarchy.
 */
class IEntityVisitor
{
public:
	/**
	 * @brief Dtor.
	 */
	virtual ~IEntityVisitor() = default;;

	/**
	 * @brief Called for each entity in the hierarchy that's visited.
	 * @param entity The currently visited entity.
	 */
	virtual void visit(EmberEntity& entity) = 0;
};

}

#endif /* IENTITYVISITOR_H_ */
