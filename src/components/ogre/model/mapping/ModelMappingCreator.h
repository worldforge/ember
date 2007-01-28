//
// C++ Interface: ModelMappingCreator
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
#ifndef EMBEROGRE_MODEL_MAPPINGMODELMAPPINGCREATOR_H
#define EMBEROGRE_MODEL_MAPPINGMODELMAPPINGCREATOR_H

#include "ModelMappingDefinition.h"
namespace Eris
{
class Entity;
class TypeService;
}

namespace EmberOgre {

namespace Model {

namespace Mapping {

namespace AttributeComparers {
	class NumericComparer;
}

class ModelMappingDefinition;
class ModelMapping;
class IActionCreator;
class EntityTypeMatch;
class AttributeMatch;
class AttributeCase;
class OutfitMatch;
class OutfitCase;
class CaseBase;
/**
	@author Erik Hjortsberg <erik@katastrof.nu>
*/

class ModelMappingCreator
{
public:
	ModelMappingCreator(ModelMappingDefinition* definition, Eris::Entity* entity, IActionCreator* actionCreator, Eris::TypeService* typeService);
	
	~ModelMappingCreator();
	
	ModelMapping* create();

protected:

	ModelMapping* createMapping();
	
	void addEntityTypeCases(EntityTypeMatch* entityTypeMatch, MatchDefinition& matchDefinition);
	
	void addAttributeCases(AttributeMatch* match, MatchDefinition& matchDefinition);
	
	void addOutfitCases(OutfitMatch* match, MatchDefinition& matchDefinition);
	
	void addMatch(CaseBase* aCase, MatchDefinition& matchDefinition);
	
	void addAttributeMatch(CaseBase* aCase, MatchDefinition& matchDefinition);

	void addEntityTypeMatch(CaseBase* aCase, MatchDefinition& matchDefinition);
	
	void addOutfitMatch(CaseBase* aCase, MatchDefinition& matchDefinition);

	AttributeComparers::NumericComparer* createNumericComparer(CaseDefinition& caseDefinition);
	void setAttributeCaseComparer(AttributeCase* aCase, AttributeMatch* match, MatchDefinition& matchDefinition, CaseDefinition& caseDefinition);


	IActionCreator* mActionCreator;
	Eris::Entity* mEntity;
	ModelMapping* mModelMap;
	ModelMappingDefinition* mDefinition;
	Eris::TypeService* mTypeService;
};


}

}

}

#endif
