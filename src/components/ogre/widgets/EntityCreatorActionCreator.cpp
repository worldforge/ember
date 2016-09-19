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
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "EntityCreatorActionCreator.h"
#include "EntityCreatorCreationInstance.h"

#include "EntityCreatorPartAction.h"
#include "EntityCreatorModelAction.h"
#include "EntityCreatorHideModelAction.h"
#include "components/entitymapping/Definitions/CaseDefinition.h"
#include "components/entitymapping/Cases/CaseBase.h"

using namespace Ember::EntityMapping;
namespace Ember {
namespace OgreView {

namespace Gui {

EntityCreatorActionCreator::EntityCreatorActionCreator(EntityCreatorCreationInstance& creationInstance)
		: mCreationInstance(creationInstance)
{
}

EntityCreatorActionCreator::~EntityCreatorActionCreator()
{
}

void EntityCreatorActionCreator::createActions(EntityMapping::EntityMapping& modelMapping, EntityMapping::Cases::CaseBase* aCase, EntityMapping::Definitions::CaseDefinition& caseDefinition)
{
	Definitions::CaseDefinition::ActionStore::iterator endJ = caseDefinition.getActions().end();
	for (Definitions::CaseDefinition::ActionStore::iterator J = caseDefinition.getActions().begin(); J != endJ; ++J) {
		if (J->getType() == "display-part") {
			EntityCreatorPartAction* action = new EntityCreatorPartAction(mCreationInstance, J->getValue());
			aCase->addAction(action);
		} else if (J->getType() == "display-model") {
			EntityCreatorModelAction* action = new EntityCreatorModelAction(mCreationInstance, J->getValue());
			aCase->addAction(action);
		} else if (J->getType() == "hide-model") {
			EntityCreatorHideModelAction* action = new EntityCreatorHideModelAction(mCreationInstance);
			aCase->addAction(action);
		}
	}
}



}

}
}
