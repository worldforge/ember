//
// C++ Interface: EntityMappingCreator
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
#ifndef EMBEROGRE_MODEL_MAPPINGMODELMAPPINGCREATOR_H
#define EMBEROGRE_MODEL_MAPPINGMODELMAPPINGCREATOR_H

#include "Definitions/EntityMappingDefinition.h"
namespace Eris
{
class Entity;
class TypeService;
class View;
}

namespace Ember {



namespace EntityMapping {

namespace Definitions {
class EntityMappingDefinition;
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
class EntityMapping;
class IActionCreator;
/**
	Creates a EntityMapping instances from the supplied definition.

	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/

class EntityMappingCreator
{
public:
	/**
	 *    Default constructor.
	 * @param definition The definition to use.
	 * @param entity Entity to attach to.
	 * @param actionCreator Client supplied action creator.
	 * @param typeService A valid typeservice instance.
	 * @param view An optional View instance.
	 */
	EntityMappingCreator(Definitions::EntityMappingDefinition& definition, Eris::Entity& entity, IActionCreator* actionCreator, Eris::TypeService& typeService, Eris::View* view);

	~EntityMappingCreator();


	/**
	 *    Creates a new EntityMapping instance.
	 */
	EntityMapping* create();

protected:

	/**
	Main entry point for mapping creation.
	*/
	EntityMapping* createMapping();

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
	Eris::Entity& mEntity;
	EntityMapping* mModelMap;
	Definitions::EntityMappingDefinition& mDefinition;
	Eris::TypeService& mTypeService;
	Eris::View* mView;
};


}

}

#endif
