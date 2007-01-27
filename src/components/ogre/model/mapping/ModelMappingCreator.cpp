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
	
	///since we already have the entity, we can perform a check right away
	mModelMap->getRootEntityMatch().testEntity(mEntity);
	return mModelMap;
}

void ModelMappingCreator::addEntityTypeCases(EntityTypeMatch* entityTypeMatch, MatchDefinition& matchDefinition) {
	MatchDefinition::CaseStore::iterator endI = matchDefinition.getCases().end();
	for (MatchDefinition::CaseStore::iterator I = matchDefinition.getCases().begin(); I != endI; ++I) {
		if (mTypeService) {
			EntityTypeCase* aCase = new EntityTypeCase();
			const std::string& entityName = I->getProperties()["equals"];
			std::vector<std::string> splitNames = ModelMappingManager::splitString(entityName, ",", 100);
			for (std::vector<std::string>::const_iterator J = splitNames.begin(); J != splitNames.end(); ++J) {
				aCase->addEntityType(mTypeService->getTypeByName(*J));
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

void ModelMappingCreator::setAttributeCaseComparer(AttributeCase* aCase, AttributeMatch* match, MatchDefinition& matchDefinition, CaseDefinition& caseDefinition)
{
	const std::string& matchType = matchDefinition.getProperties()["type"];
	
	if ((matchType == "") || (matchType == "string")) {
		///default is string comparison
		const std::string& attributeValue = caseDefinition.getProperties()["equals"];
		aCase->setAttributeValue(attributeValue);
	} else if (matchType == "numeric") {
		aCase->setAttributeComparerWrapper(new AttributeComparers::NumericComparerWrapper(createNumericComparer(caseDefinition)));
	} else if (matchType == "function") {
		if (match->getAttributeName() == "height") {
			aCase->setAttributeComparerWrapper(new AttributeComparers::HeightComparerWrapper(createNumericComparer(caseDefinition), mEntity));
		}
	}
	
}

AttributeComparers::NumericComparer* ModelMappingCreator::createNumericComparer(CaseDefinition& caseDefinition)
{
	DefinitionBase::PropertiesMap::const_iterator value;
	DefinitionBase::PropertiesMap::const_iterator end = caseDefinition.getProperties().end();
	value = caseDefinition.getProperties().find("equals");
	
	if (value != end) {
		return new AttributeComparers::NumericEqualsComparer(atof(value->second.c_str()));
	}
	
	///If both a min and max value is set, it's a range comparer
	AttributeComparers::NumericComparer *mMin(0), *mMax(0);
	if ((value = caseDefinition.getProperties().find("lesser")) != end) {
		mMin = new AttributeComparers::NumericLesserComparer(atof(value->second.c_str()));
	} else if ((value = caseDefinition.getProperties().find("lesserequals")) != end) {
		mMin = new AttributeComparers::NumericEqualsOrLesserComparer(atof(value->second.c_str()));
	}
	
	if ((value = caseDefinition.getProperties().find("greater")) != end) {
		mMax = new AttributeComparers::NumericGreaterComparer(atof(value->second.c_str()));
	} else if ((value = caseDefinition.getProperties().find("greaterequals")) != end) {
		mMax = new AttributeComparers::NumericEqualsOrGreaterComparer(atof(value->second.c_str()));
	}
	
	///check if we have both min and max set, and if so we should use a range comparer
	if (mMin && mMax) {
		return new AttributeComparers::NumericRangeComparer(mMin, mMax);
	} else if (!mMax && mMin) {
		return mMin;
	} else if (mMax && !mMin) {
		return mMax;
	}
	///invalid, could not find anything to compare against
	return 0;
}


void ModelMappingCreator::addAttributeCases(AttributeMatch* match, MatchDefinition& matchDefinition) {
	MatchDefinition::CaseStore::iterator endI = matchDefinition.getCases().end();
	for (MatchDefinition::CaseStore::iterator I = matchDefinition.getCases().begin(); I != endI; ++I) {
		AttributeCase* aCase = new AttributeCase();
		setAttributeCaseComparer(aCase, match, matchDefinition, *I);		
		//aCase->setAttributeValue( attributeValue);
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
		
		///observe the attribute by the use of an AttributeObserver
		AttributeObserver* observer(0);
		const std::string& matchType = matchDefinition.getProperties()["type"];
		///TODO: make this check better
		if (matchType == "function") {
			if (match->getAttributeName() == "height") {
				observer = new AttributeObserver(mEntity, match, *mModelMap, "bbox");
			}
		}
		if (!observer) {
			observer = new AttributeObserver(mEntity, match, *mModelMap);
		}
		mModelMap->addAttributeObserver(observer);
	
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
	const std::string& attributeName = matchDefinition.getProperties()["attribute"];
	AttributeMatch* match = new AttributeMatch(attributeName);
	aCase->addMatch( match);
	addAttributeCases(match, matchDefinition);
}

void ModelMappingCreator::addEntityTypeMatch(CaseBase* aCase, MatchDefinition& matchDefinition) {
	EntityTypeMatch* match = new EntityTypeMatch();
	aCase->addMatch( match);
	addEntityTypeCases(match, matchDefinition);
	
	///since we already have the entity, we can perform a check right away
	match->testEntity(mEntity);
}
}

}

}
