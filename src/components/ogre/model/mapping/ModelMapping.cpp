//
// C++ Implementation: ModelMapping
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
#include "ModelMapping.h"

namespace EmberOgre {

namespace Model {

namespace Mapping {
	
CaseBase::CaseBase() 
: mParentCase(0), mIsActive(false), mIsTrue(false) 
{
}

CaseBase::~CaseBase() {
	cleanVector(mActions);
	cleanVector(mMatches);
}

void CaseBase::addAction(Action* action) { 
	mActions.push_back(action);
	action->setCase(this);
}

void CaseBase::evaluateChanges() {
	if (getIsTrueToRoot()) {
		if (!getIsActive()) {
			activateActions();
		}
	} else {
		if (getIsActive()) {
			deactivateActions();
		}
	}
}
void CaseBase::activateActions()
{
	for (ActionStore::iterator I = mActions.begin(); I != mActions.end(); ++I) {
		(*I)->activate();
	}
	mIsActive = true;
}
void CaseBase::deactivateActions()
{
	for (ActionStore::iterator I = mActions.begin(); I != mActions.end(); ++I) {
		(*I)->deactivate();
	}
	mIsActive = false;
}

void CaseBase::addMatch(MatchBase* match) { 
	mMatches.push_back(match);
}


bool EntityTypeCase::testMatch(Eris::Entity* entity)
{
	Eris::TypeInfo* type = entity->getType();
	for (std::vector<Eris::TypeInfoPtr>::iterator I = mEntityTypes.begin(); I != mEntityTypes.end(); ++I) {
		if (type->isA(*I)) {
			setState(true);
			return true;
		}
	}
	setState(false);
	return false;
}

void EntityTypeCase::addEntityType(Eris::TypeInfoPtr typeInfo)
{
	mEntityTypes.push_back(typeInfo);
}




bool AttributeCase::testMatch(const Atlas::Message::Element& attribute)
{
	if (mComparerWrapper->testAttribute(attribute)) {
		setState(true);
		return true;
	}
	setState(false);
	return false;
}

void AttributeCase::setAttributeValue(const std::string& value) 
{
	mComparerWrapper = std::auto_ptr<AttributeComparers::AttributeComparerWrapper>(new AttributeComparers::StringComparerWrapper(new AttributeComparers::StringComparer(value)));
}

void AttributeCase::setAttributeComparerWrapper(AttributeComparers::AttributeComparerWrapper* comparerWrapper)
{
	mComparerWrapper = std::auto_ptr<AttributeComparers::AttributeComparerWrapper>(comparerWrapper);
}


template <class TCase>
AbstractMatch<TCase>::AbstractMatch()
{
}

template <class TCase>
AbstractMatch<TCase>::~AbstractMatch() 
{
	cleanVector(mCases);
}




void EntityTypeMatch::testEntity(Eris::Entity* entity)
{
	for (std::vector<EntityTypeCase*>::iterator I = mCases.begin(); I != mCases.end(); ++I) {
		(*I)->testMatch(entity);
	}
}



AttributeMatch::AttributeMatch(const std::string& attributeName) 
: mAttributeName(attributeName) 
{
}

void AttributeMatch::testAttribute(const Atlas::Message::Element& attribute)
{
	for (std::vector<AttributeCase*>::iterator I = mCases.begin(); I != mCases.end(); ++I) {
		(*I)->testMatch(attribute);
	}
}




AttributeObserver::AttributeObserver(Eris::Entity* entity, AttributeMatch* match, ModelMapping& modelMapping,const std::string& attributeName) : mMatch(match), mModelMapping(modelMapping)
{
	mSlot = sigc::mem_fun(*this, &AttributeObserver::attributeChanged);
	entity->observe(attributeName, mSlot);
}

AttributeObserver::AttributeObserver(Eris::Entity* entity, AttributeMatch* match, ModelMapping& modelMapping) :  mMatch(match), mModelMapping(modelMapping)
{
	mSlot = sigc::mem_fun(*this, &AttributeObserver::attributeChanged);
	entity->observe(match->getAttributeName(), mSlot);
}

void AttributeObserver::attributeChanged(const std::string& attributeName, const Atlas::Message::Element& attributeValue)
{
	mMatch->testAttribute( attributeValue);
	mModelMapping.evaluateChanges();
}





ModelMapping::ModelMapping(Eris::Entity* entity): mEntity(entity) 
{
}

ModelMapping::~ModelMapping()
{
	cleanVector(mAttributeObservers);
}

void ModelMapping::evaluateChanges() {
	CaseBaseStore::iterator endI = mCases.end();
	for (CaseBaseStore::iterator I = mCases.begin(); I != endI; ++I) {
		(*I)->evaluateChanges();
	}
}
void ModelMapping::addCase(CaseBase* aCase)
{
	mCases.push_back(aCase);
}

void ModelMapping::addAttributeObserver(AttributeObserver* observer)
{
	mAttributeObservers.push_back(observer);
}

void ModelMapping::initialize()
{
	//mRootEntityMatch.testEntity( mEntity);
	///make sure that we evaluate the changes of the root match first
	std::vector<EntityTypeCase*>::iterator endI = mRootEntityMatch.getCases().end();
	for (std::vector<EntityTypeCase*>::iterator I = mRootEntityMatch.getCases().begin(); I != endI; ++I) {
		(*I)->evaluateChanges();
	}	
	evaluateChanges();
}


}

}

}
