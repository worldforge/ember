//
// C++ Interface: EmberEntityModelAction
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2007
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifndef EMBEROGREEMBERENTITYMODELACTION_H
#define EMBEROGREEMBERENTITYMODELACTION_H

#include "components/entitymapping/Actions/Action.h"
#include <string>

namespace Ember
{
class EmberEntity;
namespace EntityMapping
{
class EntityMapping;
}
namespace OgreView
{

class Scene;

/**
 * @brief Show a certain Model.
 * @author Erik Ogenvik <erik@ogenvik.org>
 */
class EmberEntityModelAction: public EntityMapping::Actions::Action
{
public:
	EmberEntityModelAction(EmberEntity& entity, const std::string& modelName, Scene& scene, EntityMapping::EntityMapping& mMapping);
	virtual ~EmberEntityModelAction();

	virtual void activate(EntityMapping::ChangeContext& context);
	virtual void deactivate(EntityMapping::ChangeContext& context);

protected:
	EmberEntity& mEntity;

	std::string mModelName;

	Scene& mScene;

	/**
	 * @brief The mapping this action is bound to.
	 */
	EntityMapping::EntityMapping& mMapping;

	void ChangeContext_ContextComplete();

};

}

}

#endif
