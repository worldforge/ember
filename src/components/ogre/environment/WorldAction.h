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

#ifndef WORLDACTION_H_
#define WORLDACTION_H_

#include "components/entitymapping/Actions/Action.h"

namespace Ember
{
namespace OgreView
{
class EmberEntity;
namespace Environment
{

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief Creates a WorldRepresentation.
 *
 * This is mainly used to represent the world. It will show the firmament and other features only visible when the world itself is visible.
 */
class WorldAction: public EntityMapping::Actions::Action
{
public:

	/**
	 * @brief Ctor.
	 * @param entity The entity the action is applied to.
	 */
	WorldAction(EmberEntity& entity);
	virtual ~WorldAction();

	/**
	 * @copydoc EntityMapping::Actions::Action::activate()
	 */
	virtual void activate(EntityMapping::ChangeContext& context);

	/**
	 * @copydoc EntityMapping::Actions::Action::deactivate()
	 */
	virtual void deactivate(EntityMapping::ChangeContext& context);

protected:

	/**
	 * @brief The entity the action is applied to.
	 */
	EmberEntity& mEntity;

};

}
}
}
#endif /* WORLDACTION_H_ */
