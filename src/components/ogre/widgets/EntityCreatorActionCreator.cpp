//
// C++ Implementation: EntityCreatorActionCreator
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2009
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "EntityCreatorActionCreator.h"
#include "EntityCreatorCreationInstance.h"

#include "EntityCreatorPartAction.h"
#include "EntityCreatorModelAction.h"
#include "EntityCreatorHideModelAction.h"
#include "EntityCreatorPresentModelAction.h"
#include "EntityCreatorPresentMeshAction.h"
#include "components/entitymapping/Cases/CaseBase.h"

using namespace Ember::EntityMapping;
namespace Ember {
namespace OgreView {

namespace Gui {

EntityCreatorActionCreator::EntityCreatorActionCreator(EntityCreatorCreationInstance& creationInstance)
		: mCreationInstance(creationInstance)
{
}

void EntityCreatorActionCreator::createActions(EntityMapping::EntityMapping& modelMapping, EntityMapping::Cases::CaseBase* aCase, EntityMapping::Definitions::CaseDefinition& caseDefinition)
{
	for (auto& actionDef : caseDefinition.getActions()) {
		if (actionDef.getType() == "display-part") {
			aCase->addAction(new EntityCreatorPartAction(mCreationInstance, actionDef.getValue()));
		} else if (actionDef.getType() == "display-model") {
			aCase->addAction(new EntityCreatorModelAction(mCreationInstance, actionDef.getValue()));
		} else if (actionDef.getType() == "hide-model") {
			aCase->addAction(new EntityCreatorHideModelAction(mCreationInstance));
		} else if (actionDef.getType() == "present-model") {
			aCase->addAction(new EntityCreatorPresentModelAction(mCreationInstance));
		} else if (actionDef.getType() == "present-mesh") {
			aCase->addAction(new EntityCreatorPresentMeshAction(mCreationInstance));
		}
	}
}



}

}
}
