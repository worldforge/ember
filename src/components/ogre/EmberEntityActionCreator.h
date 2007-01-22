//
// C++ Interface: EmberEntityActionCreator
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
#ifndef EMBEROGREEMBERENTITYACTIONCREATOR_H
#define EMBEROGREEMBERENTITYACTIONCREATOR_H
#include "EmberOgrePrerequisites.h"
#include "EmberPhysicalEntity.h"

#include "model/mapping/ModelMapping.h"
#include "model/mapping/ModelMappingDefinition.h"
#include "model/mapping/ModelMappingManager.h"

namespace EmberOgre {

/**
	@author Erik Hjortsberg <erik@katastrof.nu>
*/
class EmberEntityActionCreator : public Model::Mapping::IActionCreator
{
public:
    EmberEntityActionCreator(EmberPhysicalEntity& entity);

    ~EmberEntityActionCreator();
	virtual void createActions(Model::Mapping::ModelMapping& modelMapping, Model::Mapping::CaseBase* aCase, Model::Mapping::CaseDefinition& caseDefinition);
protected:
	EmberPhysicalEntity& mEntity;


};

}

#endif
