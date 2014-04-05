/*
 Copyright (C) 2012 Erik Ogenvik

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

#ifndef COMPOSITIONACTION_H_
#define COMPOSITIONACTION_H_

#include "components/entitymapping/Actions/Action.h"
#include <string>

namespace Ember
{
class EmberEntity;
namespace OgreView
{

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief Enables or disables composition mode of entities.
 *
 * Some entities can be "composition" entities. That means that they and their children make up one single conceptual entity.
 * An example might be a table, where each table leg is a separate entity. Each leg is then part of the "table composition".
 */
class CompositionAction: public EntityMapping::Actions::Action
{
public:
	/**
	 * @brief Ctor.
	 * @param entity The entity being affected.
	 * @param mode The mode of the composition. Either "none", "composition" or "exclusive". This corresponds to EmberEntity::CompositionMode.
	 */
	CompositionAction(EmberEntity& entity, const std::string& mode);

	virtual void activate(EntityMapping::ChangeContext& context);
	virtual void deactivate(EntityMapping::ChangeContext& context);

protected:

	/**
	 * @brief The entity being affected.
	 */
	EmberEntity& mEntity;

	/**
	 * @brief The mode of the composition.
	 *
	 * Either "none", "composition" or "exclusive". This corresponds to EmberEntity::CompositionMode.
	 */
	const std::string mMode;

};

}
}
#endif /* COMPOSITIONACTION_H_ */
