//
// C++ Implementation: ModelMappingCreator
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
#include "ModelMappingCreator.h"
#include "ModelMapping.h"
#include "ModelMappingManager.h"
namespace EmberOgre {

namespace Model {

namespace Mapping {




ModelMappingCreator::ModelMappingCreator(ModelMappingDefinition* definition, Eris::Entity* entity, IActionCreator* actionCreator, Eris::TypeService* typeService) : mActionCreator(actionCreator), mEntity(entity), mModelMap(0), mDefinition(definition), mTypeService(typeService)
{
}


ModelMappingCreator::~ModelMappingCreator()
{
}

ModelMapping* ModelMappingCreator::create() 
{
	return createMapping();
}


ModelMapping* ModelMappingCreator::createMapping() {
	mModelMap = new ModelMapping(mEntity);
	addEntityTypeCases(&mModelMap->getRootEntityMatch(), mDefinition->getRoot());
	return mModelMap;
}

void ModelMappingCreator::addEntityTypeCases(EntityTypeMatch* entityTypeMatch, MatchDefinition& matchDefinition) {
	MatchDefinition::CaseStore::iterator endI = matchDefinition.getCases().end();
	for (MatchDefinition::CaseStore::iterator I = matchDefinition.getCases().begin(); I != endI; ++I) {
		if (mTypeService) {
			EntityTypeCase* aCase = new EntityTypeCase();
			const std::string& entityName = I->getProperties()["equals"];
			std::vector<std::string> splitNames = ModelMappingManager::splitString(entityName, ",", 100);
			for (std::vector<std::string>::const_iterator I = splitNames.begin(); I != splitNames.end(); ++I) {
				aCase->addEntityType(mTypeService->getTypeByName(*I));
			}
			if (mActionCreator) {
				mActionCreator->createActions(*mModelMap, aCase, *I);
			}

			CaseDefinition::MatchStore::iterator endJ = I->getMatches().end();
			for (CaseDefinition::MatchStore::iterator J = I->getMatches().begin(); J != endJ; ++J) {
				addMatch(aCase, *J);
			}
			entityTypeMatch->addCase( aCase);
			///also add the case to the model map for quick lookup
			mModelMap->addCase(aCase);
			aCase->setParentMatch( entityTypeMatch);
		}
	}
}

void ModelMappingCreator::addAttributeCases(AttributeMatch* match, MatchDefinition& matchDefinition) {
	MatchDefinition::CaseStore::iterator endI = matchDefinition.getCases().end();
	for (MatchDefinition::CaseStore::iterator I = matchDefinition.getCases().begin(); I != endI; ++I) {
		const std::string& attributeValue = I->getProperties()["equals"];
		AttributeCase* aCase = new AttributeCase();
		aCase->setAttributeValue( attributeValue);
		if (mActionCreator) {
			mActionCreator->createActions(*mModelMap, aCase, *I);
		}

		CaseDefinition::MatchStore::iterator endJ = I->getMatches().end();
		for (CaseDefinition::MatchStore::iterator J = I->getMatches().begin(); J != endJ; ++J) {
			addMatch(aCase, *J);
		}
		match->addCase( aCase);
		///also add the case to the model map for quick lookup
		mModelMap->addCase(aCase);
		aCase->setParentMatch( match);
	}
}

void ModelMappingCreator::addMatch(CaseBase* aCase, MatchDefinition& matchDefinition) {
	if (matchDefinition.getType() == "attribute") {
		addAttributeMatch(aCase, matchDefinition);
	} else if (matchDefinition.getType() == "entitytype") {
		addEntityTypeMatch(aCase, matchDefinition);
	}
}

void ModelMappingCreator::addAttributeMatch(CaseBase* aCase, MatchDefinition& matchDefinition) {
	const std::string& attributeName = matchDefinition.getProperties()["attributename"];
	AttributeMatch* match = new AttributeMatch(attributeName);
	aCase->addMatch( match);
	addAttributeCases(match, matchDefinition);
}

void ModelMappingCreator::addEntityTypeMatch(CaseBase* aCase, MatchDefinition& matchDefinition) {
	EntityTypeMatch* match = new EntityTypeMatch();
	aCase->addMatch( match);
	addEntityTypeCases(match, matchDefinition);
}	
}

}

}
