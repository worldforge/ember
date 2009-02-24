//
// C++ Implementation: EntityCreatorActionCreator
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2009
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
#include "EntityCreator.h"

#include "EntityCreatorPartAction.h"
#include "EntityCreatorModelAction.h"
#include "EntityCreatorHideModelAction.h"
#include "components/ogre/model/mapping/Definitions/CaseDefinition.h"
#include "components/ogre/model/mapping/Cases/CaseBase.h"

namespace EmberOgre {

namespace Gui {

EntityCreatorActionCreator::EntityCreatorActionCreator(EntityCreator& entityCreator)
		: mEntityCreator(entityCreator)
{
}

EntityCreatorActionCreator::~EntityCreatorActionCreator()
{
}

void EntityCreatorActionCreator::createActions(Model::Mapping::ModelMapping& modelMapping, Model::Mapping::Cases::CaseBase* aCase, Model::Mapping::Definitions::CaseDefinition& caseDefinition)
{
	::EmberOgre::Model::Mapping::Definitions::CaseDefinition::ActionStore::iterator endJ = caseDefinition.getActions().end();
	for (::EmberOgre::Model::Mapping::Definitions::CaseDefinition::ActionStore::iterator J = caseDefinition.getActions().begin(); J != endJ; ++J) {
		if (J->getType() == "display-part") {
			EntityCreatorPartAction* action = new EntityCreatorPartAction(mEntityCreator, J->getValue());
			aCase->addAction(action);
		} else if (J->getType() == "display-model") {
			EntityCreatorModelAction* action = new EntityCreatorModelAction(mEntityCreator, J->getValue());
			aCase->addAction(action);
		} else if (J->getType() == "hide-model") {
			EntityCreatorHideModelAction* action = new EntityCreatorHideModelAction(mEntityCreator);
			aCase->addAction(action);
		}
	}
}



}

}
