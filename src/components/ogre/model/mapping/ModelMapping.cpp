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
#include <Eris/View.h>

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

void CaseBase::evaluateChanges(std::vector<CaseBase*>& activateQueue, std::vector<CaseBase*>& deactivateQueue) 
{
	if (getIsTrueToRoot()) {
		if (!getIsActive()) {
			activateQueue.push_back(this);
// 			activateActions();
		}
	} else {
		if (getIsActive()) {
			deactivateQueue.push_back(this);
// 			deactivateActions();
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

void CaseBase::setEntity(Eris::Entity* entity)
{
	MatchBaseStore::iterator I = mMatches.begin();
	for ( ; I != mMatches.end(); ++I) {
		(*I)->setEntity(entity);
	}
}


bool EntityBaseCase::testMatch(Eris::Entity* entity)
{
	if (entity) {
		Eris::TypeInfo* type = entity->getType();
		for (std::vector<Eris::TypeInfoPtr>::iterator I = mEntityTypes.begin(); I != mEntityTypes.end(); ++I) {
			if (type->isA(*I)) {
				_setState(true);
				return true;
			}
		}
	}
	_setState(false);
	return false;
}

void EntityBaseCase::addEntityType(Eris::TypeInfoPtr typeInfo)
{
	mEntityTypes.push_back(typeInfo);
}

void EntityTypeCase::_setState(bool state)
{
	setState(state);
}

void OutfitCase::_setState(bool state)
{
	setState(state);
}



bool AttributeCase::testMatch(const Atlas::Message::Element& attribute)
{
	if (mComparerWrapper.get()) {
		if (mComparerWrapper->testAttribute(attribute)) {
			setState(true);
			return true;
		}
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


template <class TCase>
void AbstractMatch<TCase>::setEntity(Eris::Entity* entity)
{
	typename std::vector< TCase* >::iterator I = mCases.begin();
	for ( ; I != mCases.end(); ++I) {
		(*I)->setEntity(entity);
	}
}


void EntityTypeMatch::setEntity(Eris::Entity* entity)
{
	AbstractMatch<EntityTypeCase>::setEntity( entity);
	testEntity(entity);
}

void EntityTypeMatch::testEntity(Eris::Entity* entity)
{
	for (std::vector<EntityTypeCase*>::iterator I = mCases.begin(); I != mCases.end(); ++I) {
		(*I)->testMatch(entity);
	}
}

OutfitMatch::OutfitMatch(const std::string& outfitName, Eris::View* view)
: mOutfitName(outfitName), mView(view)
{
}

void OutfitMatch::testAttribute(const Atlas::Message::Element& attribute)
{
	if (attribute.isMap()) {
		Eris::Entity* entity(0);
		const Atlas::Message::MapType & tmap = attribute.asMap();
		Atlas::Message::MapType::const_iterator I = tmap.find(mOutfitName);
		if (I != tmap.end() && I->second.isString()) {
			entity = mView->getEntity(I->second.asString()); 
		}
		testEntity(entity);
	}
}

void OutfitMatch::setEntity(Eris::Entity* entity)
{
	///observe the attribute by the use of an AttributeObserver
	mAttributeObserver->observeEntity(entity);
}

void OutfitMatch::testEntity(Eris::Entity* entity)
{
	AbstractMatch<OutfitCase>::setEntity(entity);	
	
	for (std::vector<OutfitCase*>::iterator I = mCases.begin(); I != mCases.end(); ++I) {
		(*I)->testMatch(entity);
	}
}


AttributeMatch::AttributeMatch(const std::string& attributeName) 
: mAttributeName(attributeName), mInternalAttributeName(attributeName) 
{
}
	
AttributeMatch::AttributeMatch(const std::string& attributeName, const std::string& internalAttributeName)
: mAttributeName(attributeName), mInternalAttributeName(internalAttributeName) 
{
}


void AttributeMatch::testAttribute(const Atlas::Message::Element& attribute)
{
	for (std::vector<AttributeCase*>::iterator I = mCases.begin(); I != mCases.end(); ++I) {
		(*I)->testMatch(attribute);
	}
}

void AttributeMatch::setEntity(Eris::Entity* entity)
{
	///observe the attribute by the use of an AttributeObserver
	mAttributeObserver->observeEntity(entity);
	if (entity) {
		if (entity->hasAttr(mInternalAttributeName)) {
			testAttribute(entity->valueOfAttr(mInternalAttributeName));
		}
	}
}

AttributeDependentMatch::AttributeDependentMatch()
: mAttributeObserver(0)
{
}
	
AttributeDependentMatch::~AttributeDependentMatch()
{
}

void AttributeDependentMatch::setAttributeObserver(AttributeObserver* observer)
{
	mAttributeObserver = std::auto_ptr<AttributeObserver>(observer);
}




AttributeObserver::AttributeObserver(AttributeDependentMatch* match, ModelMapping& modelMapping,const std::string& attributeName) : mMatch(match), mModelMapping(modelMapping), mAttributeName(attributeName)
{
}

AttributeObserver::AttributeObserver(AttributeMatch* match, ModelMapping& modelMapping) :  mMatch(match), mModelMapping(modelMapping), mAttributeName(match->getAttributeName())
{
}

void AttributeObserver::attributeChanged(const std::string& attributeName, const Atlas::Message::Element& attributeValue)
{
	mMatch->testAttribute( attributeValue);
	mModelMapping.evaluateChanges();
}

void AttributeObserver::observeEntity(Eris::Entity* entity)
{
	mSlot.disconnect();
	if (entity) {
		mSlot = sigc::mem_fun(*this, &AttributeObserver::attributeChanged);
		entity->observe(mAttributeName, mSlot);
	}
}




ModelMapping::ModelMapping(Eris::Entity* entity): mEntity(entity) 
{
}

ModelMapping::~ModelMapping()
{
}

void ModelMapping::evaluateChanges() {

	///we want to make sure that we start with deactivating actions, and then after that activate those that should be activated

	std::vector<CaseBase*> activateQueue;
	std::vector<CaseBase*> deactivateQueue;
	CaseBaseStore::iterator endI = mCases.end();
	for (CaseBaseStore::iterator I = mCases.begin(); I != endI; ++I) {
		(*I)->evaluateChanges(activateQueue, deactivateQueue);
	}
	
	for (std::vector<CaseBase*>::iterator I = deactivateQueue.begin(); I != deactivateQueue.end(); ++I) {
		(*I)->deactivateActions();
	} 
	for (std::vector<CaseBase*>::iterator I = activateQueue.begin(); I != activateQueue.end(); ++I) {
		(*I)->activateActions();
	} 
}
void ModelMapping::addCase(CaseBase* aCase)
{
	mCases.push_back(aCase);
}


void ModelMapping::initialize()
{
	///make sure that we evaluate the changes of the root match first
	
// 	std::vector<CaseBase*> activateQueue;
// 	std::vector<CaseBase*> deactivateQueue;
// 	
// 	std::vector<EntityTypeCase*>::iterator endI = mRootEntityMatch.getCases().end();
// 	for (std::vector<EntityTypeCase*>::iterator I = mRootEntityMatch.getCases().begin(); I != endI; ++I) {
// 		(*I)->evaluateChanges(activateQueue, deactivateQueue);
// 	}	
// 	
// 	for (std::vector<CaseBase*>::iterator I = activateQueue.begin(); I != activateQueue.end(); ++I) {
// 		(*I)->activateActions();
// 	} 
	
	evaluateChanges();
}


}

}

}
