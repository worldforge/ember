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
	class AttributeComparerWrapper;
}

class AttributeCase;
class OutfitCase;
class CaseBase;
}
class ModelMapping;
class IActionCreator;
/**
	Creates a ModelMapping instances from the supplied definition.

	@author Erik Hjortsberg <erik@katastrof.nu>
*/

class ModelMappingCreator
{
public:
	/**
	 *    Default constructor.
	 * @param definition The definition to use.
	 * @param entity Entity to attach to.
	 * @param actionCreator Client supplied action creator.
	 * @param typeService A valid typeservice instance.
	 */
	ModelMappingCreator( Definitions::ModelMappingDefinition* definition, Eris::Entity* entity, IActionCreator* actionCreator, Eris::TypeService* typeService);
	
	~ModelMappingCreator();
	
	
	/**
	 *    Creates a new ModelMapping instance. 
	 */
	ModelMapping* create();

protected:

	/**
	Main entry point for mapping creation.
	*/
	ModelMapping* createMapping();
	
	/**
	 * Adds EntityTypeCases to the supplied match. 
	 * @param entityTypeMatch
	 * @param matchDefinition 
	 */
	void addEntityTypeCases(Matches::EntityTypeMatch* entityTypeMatch, Definitions::MatchDefinition& matchDefinition);
	
	/**
	 * Adds AttributeCases to the supplied match. 
	 * @param match 
	 * @param matchDefinition 
	 */
	void addAttributeCases(Matches::AttributeMatch* match, Definitions::MatchDefinition& matchDefinition);
	
	/**
	 * Adds OutfitCases to the supplied match. 
	 * @param match 
	 * @param matchDefinition 
	 */
	void addOutfitCases(Matches::OutfitMatch* match, Definitions::MatchDefinition& matchDefinition);
	
	/**
	 * Adds matches to the supplied case.
	 * @param aCase 
	 * @param matchDefinition 
	 */
	void addMatch(Cases::CaseBase* aCase, Definitions::MatchDefinition& matchDefinition);
	
	/**
	 * Adds attribute matches to the supplied case.
	 * @param aCase 
	 * @param matchDefinition 
	 */
	void addAttributeMatch(Cases::CaseBase* aCase, Definitions::MatchDefinition& matchDefinition);

	/**
	 * Adds entity type matches to the supplied case.
	 * @param aCase 
	 * @param matchDefinition 
	 */
	void addEntityTypeMatch(Cases::CaseBase* aCase, Definitions::MatchDefinition& matchDefinition);
	
	/**
	 * Adds outfit matches to the supplied case.
	 * @param aCase 
	 * @param matchDefinition 
	 */
	void addOutfitMatch(Cases::CaseBase* aCase, Definitions::MatchDefinition& matchDefinition);

	/**
	 * Creates and returns a numeric comparer for the supplied case.
	 * @param caseDefinition 
	 */
	Cases::AttributeComparers::NumericComparer* createNumericComparer(Definitions::CaseDefinition& caseDefinition);
	
	/**
	 * Creates and returns suitable attribute comparer for the supplied attribute case.
	 * @param match 
	 * @param matchDefinition 
	 * @param caseDefinition 
	 * @returns An AttributeComparers::AttributeComparerWrapper instance or null.
	*/
	Cases::AttributeComparers::AttributeComparerWrapper* getAttributeCaseComparer(Matches::AttributeMatch* match, Definitions::MatchDefinition& matchDefinition, Definitions::CaseDefinition& caseDefinition);

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
