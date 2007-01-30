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

#include "Definitions/ModelMappingDefinition.h"
namespace Eris
{
class Entity;
class TypeService;
}

namespace EmberOgre {

namespace Model {

namespace Mapping {

namespace Definitions {
class ModelMappingDefinition;
}

namespace Matches {
class EntityTypeMatch;
class AttributeMatch;
class OutfitMatch;
}
namespace Cases {

namespace AttributeComparers {
	class NumericComparer;
}

class AttributeCase;
class OutfitCase;
class CaseBase;
}
class ModelMapping;
class IActionCreator;
/**
	@author Erik Hjortsberg <erik@katastrof.nu>
*/

class ModelMappingCreator
{
public:
	ModelMappingCreator( Definitions::ModelMappingDefinition* definition, Eris::Entity* entity, IActionCreator* actionCreator, Eris::TypeService* typeService);
	
	~ModelMappingCreator();
	
	ModelMapping* create();

protected:

	ModelMapping* createMapping();
	
	void addEntityTypeCases(Matches::EntityTypeMatch* entityTypeMatch, Definitions::MatchDefinition& matchDefinition);
	
	void addAttributeCases(Matches::AttributeMatch* match, Definitions::MatchDefinition& matchDefinition);
	
	void addOutfitCases(Matches::OutfitMatch* match, Definitions::MatchDefinition& matchDefinition);
	
	void addMatch(Cases::CaseBase* aCase, Definitions::MatchDefinition& matchDefinition);
	
	void addAttributeMatch(Cases::CaseBase* aCase, Definitions::MatchDefinition& matchDefinition);

	void addEntityTypeMatch(Cases::CaseBase* aCase, Definitions::MatchDefinition& matchDefinition);
	
	void addOutfitMatch(Cases::CaseBase* aCase, Definitions::MatchDefinition& matchDefinition);

	Cases::AttributeComparers::NumericComparer* createNumericComparer(Definitions::CaseDefinition& caseDefinition);
	void setAttributeCaseComparer(Cases::AttributeCase* aCase, Matches::AttributeMatch* match, Definitions::MatchDefinition& matchDefinition, Definitions::CaseDefinition& caseDefinition);


	IActionCreator* mActionCreator;
	Eris::Entity* mEntity;
	ModelMapping* mModelMap;
	Definitions::ModelMappingDefinition* mDefinition;
	Eris::TypeService* mTypeService;
};


}

}

}

#endif
