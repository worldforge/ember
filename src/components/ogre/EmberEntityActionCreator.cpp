//
// C++ Implementation: EmberEntityActionCreator
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2007
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
#include "EmberEntityActionCreator.h"
#include "EmberEntityModelAction.h"
#include "EmberEntityHideModelAction.h"
#include "EmberEntityPartAction.h"
#include "components/ogre/environment/OceanAction.h"
#include "components/entitymapping/Cases/CaseBase.h"
#include "EmberEntity.h"

using namespace Ember::EntityMapping;

namespace EmberOgre {

EmberEntityActionCreator::EmberEntityActionCreator(EmberEntity& entity)
: mEntity(entity)
{
}


EmberEntityActionCreator::~EmberEntityActionCreator()
{
}

void EmberEntityActionCreator::createActions(EntityMapping& modelMapping, Cases::CaseBase* aCase, Definitions::CaseDefinition& caseDefinition)
{
	Definitions::CaseDefinition::ActionStore::iterator endJ = caseDefinition.getActions().end();
	for (Definitions::CaseDefinition::ActionStore::iterator J = caseDefinition.getActions().begin(); J != endJ; ++J) {
		if (J->getType() == "display-part") {
			EmberEntityPartAction* action = new EmberEntityPartAction(mEntity, J->getValue());
			aCase->addAction(action);
		} else if (J->getType() == "display-model") {
			EmberEntityModelAction* action = new EmberEntityModelAction(mEntity, J->getValue());
			aCase->addAction(action);
		} else if (J->getType() == "hide-model") {
			EmberEntityHideModelAction* action = new EmberEntityHideModelAction(mEntity);
			aCase->addAction(action);
		} else if (J->getType() == "display-ocean") {
			Environment::OceanAction* action = new Environment::OceanAction(mEntity);
			aCase->addAction(action);
		}
	}

}

}
