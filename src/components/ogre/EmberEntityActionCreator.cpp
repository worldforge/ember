//
// C++ Implementation: EmberEntityActionCreator
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2007
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
#include "EmberEntityPartAction.h"
#include "model/mapping/Cases/CaseBase.h"

using namespace EmberOgre::Model::Mapping;

namespace EmberOgre {

EmberEntityActionCreator::EmberEntityActionCreator(EmberPhysicalEntity& entity)
: mEntity(entity)
{
}


EmberEntityActionCreator::~EmberEntityActionCreator()
{
}

void EmberEntityActionCreator::createActions(ModelMapping& modelMapping, Cases::CaseBase* aCase, Definitions::CaseDefinition& caseDefinition)
{
	Definitions::CaseDefinition::ActionStore::iterator endJ = caseDefinition.getActions().end();
	for (Definitions::CaseDefinition::ActionStore::iterator J = caseDefinition.getActions().begin(); J != endJ; ++J) {
		if (J->getType() == "display-part") {
			EmberEntityPartAction* action = new EmberEntityPartAction(mEntity, J->getValue());
			aCase->addAction(action);
		} else if (J->getType() == "display-model") {
			EmberEntityModelAction* action = new EmberEntityModelAction(mEntity, J->getValue());
			aCase->addAction(action);
		}
	}
	
}

}
