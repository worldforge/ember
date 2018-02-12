/*
 Copyright (C) 2018 Erik Ogenvik

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

#ifndef EMBER_MODELACTIONBASE_H
#define EMBER_MODELACTIONBASE_H
#include "components/entitymapping/Actions/Action.h"

#include <string>

namespace Ember {
class EmberEntity;
namespace EntityMapping {
class EntityMapping;
}

namespace OgreView {

class Scene;


/**
 * Abstract base for actions which shows Models.
 */
class ModelActionBase : public EntityMapping::Actions::Action {
public:
	explicit ModelActionBase(EmberEntity& entity, Scene& scene, EntityMapping::EntityMapping& mapping);

	void deactivate(EntityMapping::ChangeContext& context) override;

protected:

	/**
	 * @brief The entity being affected.
	 */
	EmberEntity& mEntity;

	Scene& mScene;

	/**
	 * @brief The mapping this action is bound to.
	 */
	EntityMapping::EntityMapping& mMapping;

	void showModel(const std::string& modelName);

};
}
}


#endif //EMBER_MODELACTIONBASE_H
