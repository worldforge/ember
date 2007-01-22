//
// C++ Interface: ModelMapping
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
#ifndef EMBEROGRE_MODEL_MAPPINGMODELMAPPING_H
#define EMBEROGRE_MODEL_MAPPINGMODELMAPPING_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


#include <vector>
#include <iostream>

#include <Atlas/Objects/Entity.h>

#include <sigc++/trackable.h>



#include <Eris/TypeInfo.h>
#include <Eris/Entity.h>

namespace Eris
{
class Entity; 
}

namespace EmberOgre {

namespace Model {

namespace Mapping {

class EntityTypeMatch;
class EntityTypeCase;
class AttributeCase;
class AttributeMatch;
class ModelMapping;
class MatchBase;
class Match;
class CaseBase;

template <typename T>
static void cleanVector(T& theVector) 
{
	typename T::iterator I = theVector.begin();
	for (;I != theVector.end();	++I) {
		delete *I;
	}
	theVector.clear();
} 

class Action 
{
public:
	virtual ~Action() {};
	
	virtual void activate() = 0;
	virtual void deactivate() = 0;
	
	void setCase(CaseBase* ownerCase)
	{
		mOwnerCase = ownerCase;
	}

protected:
	CaseBase* mOwnerCase;
};

class DummyAction : public Action
{
public:
	virtual void activate() 
	{
		std::cout << "Dummy action activated." << std::endl;
	};
	virtual void deactivate() 
	{
		std::cout << "Dummy action deactivated." << std::endl;
	};

protected:
};

// class Node
// {
// public:
// 	typedef std::vector<Node*> NodeStore;
// 
// 	virtual ~Node();
// 	
// 	void addChild(Node* node);
// 	const NodeStore& getChildren();
// 
// protected:
// 
// 	void setParent(Node* parentNode);
// 
// 	Node* mParentNode;
// 	std::vector<Node*> mChildren;
// 	
// 	bool mActive;
// 	
// 
// };

class CaseBase 
{
public:
	typedef std::vector<Action*> ActionStore;

	CaseBase() : mParentCase(0), mIsActive(false), mIsTrue(false) {}
	virtual ~CaseBase() {
		cleanVector(mActions);
		cleanVector(mMatches);
	}
	void setParentCase(CaseBase* aCase) {mParentCase = aCase; }

	bool getIsTrue() { return mIsTrue; }
	bool getIsActive() { return mIsActive;}
	bool getIsTrueToRoot() {
		if (!mParentCase) {
			return mIsTrue;
		} else {
			return mIsTrue && mParentCase->getIsTrueToRoot();
		}
	}
	
	void addAction(Action* action) { 
		mActions.push_back(action);
		action->setCase(this);
	}
	const ActionStore& getActions() { return mActions; };
	
	void activateActions()
	{
		for (ActionStore::iterator I = mActions.begin(); I != mActions.end(); ++I) {
			(*I)->activate();
		}
		mIsActive = true;
	}
	void deactivateActions()
	{
		for (ActionStore::iterator I = mActions.begin(); I != mActions.end(); ++I) {
			(*I)->deactivate();
		}
		mIsActive = false;
	}

	void addMatch(MatchBase* match) { 
		mMatches.push_back(match);
		//match->setParentCase(this);
	}
	const std::vector<MatchBase*>& getMatches() { return mMatches; }

protected:
	ActionStore mActions;

	CaseBase* mParentCase;
	bool mIsTrue, mIsActive;
	
	void setState(bool state) 
	{
		mIsTrue = state;
	}
	
	std::vector<MatchBase*> mMatches;
	
};

template <class TMatch>
class Case : public CaseBase
{
public:
	
	Case() : mParentMatch(0) {}
	virtual ~Case() { };
	
 	void setParentMatch(TMatch* aMatch) {mParentMatch = aMatch; }

protected:
	
 	TMatch* mParentMatch;
	
};

class EntityTypeCase : public Case<EntityTypeMatch>
{
public:
	bool testMatch(Eris::Entity* entity)
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
	
	void addEntityType(Eris::TypeInfoPtr typeInfo)
	{
		mEntityTypes.push_back(typeInfo);
	}
protected:
	std::vector<Eris::TypeInfoPtr> mEntityTypes;
};

class AttributeCase : public Case<AttributeMatch>
{
public:
	bool testMatch(const Atlas::Message::Element& attribute)
	{
		///TODO: add an implementation abstract class for doing checks for different types of values
		if (attribute == mValue) {
			setState(true);
			return true;
		}
		setState(false);
		return false;
	}
	
	void setAttributeValue(const std::string& value) 
	{
		mValue = value;
	}
protected:
	std::string mValue;
};




class MatchBase
{
public:

	MatchBase() : mParentCase(0) {}
	virtual ~MatchBase() {}
	void setParentCase(CaseBase* aCase) { mParentCase = aCase;}

protected:
	 CaseBase* mParentCase;
};

template <class TCase>
class AbstractMatch : public MatchBase
{
public:

	AbstractMatch()  {}
	virtual ~AbstractMatch() { cleanVector(mCases); }
	void addCase(TCase* aCase) {
		mCases.push_back(aCase); 
//		aCase->setParentMatch(this);
		aCase->setParentCase(mParentCase);
	}

protected:
	 std::vector<TCase*> mCases;
};

class EntityTypeMatch : public AbstractMatch<EntityTypeCase>
{
public:

	void testEntity(Eris::Entity* entity)
	{
		for (std::vector<EntityTypeCase*>::iterator I = mCases.begin(); I != mCases.end(); ++I) {
			(*I)->testMatch(entity);
		}
	}
	
protected:

};

class AttributeMatch : public AbstractMatch<AttributeCase>
{
public:

	AttributeMatch(const std::string& attributeName) : mAttributeName(attributeName) {}

	void testAttribute(const Atlas::Message::Element& attribute)
	{
		for (std::vector<AttributeCase*>::iterator I = mCases.begin(); I != mCases.end(); ++I) {
			(*I)->testMatch(attribute);
		}
	}
	
	const std::string& getAttributeName();

protected:

	std::string mAttributeName;
};


/**
	@author Erik Hjortsberg <erik@katastrof.nu>
*/
class ModelMapping
{
public:
	typedef std::vector<CaseBase*> CaseBaseStore;
    ModelMapping(Eris::Entity* entity): mEntity(entity) {
    }

    ~ModelMapping();
    
    EntityTypeMatch& getRootEntityMatch() { return mRootEntityMatch;}
        
    void evaluateChanges() {
    	CaseBaseStore::iterator endI = mCases.end();
    	for (CaseBaseStore::iterator I = mCases.begin(); I != endI; ++I) {
    		if ((*I)->getIsTrueToRoot()) {
    			if (!(*I)->getIsActive()) {
    				(*I)->activateActions();
    			}
    		} else {
    			if ((*I)->getIsActive()) {
    				(*I)->deactivateActions();
    			}
    		}
    	}
    }
    
    void addCase(CaseBase* aCase)
    {
    	mCases.push_back(aCase);
    }
    
    void initialize()
    {
		mRootEntityMatch.testEntity( mEntity);
		evaluateChanges();
    }
    
protected:

	EntityTypeMatch mRootEntityMatch;
	
	std::map<std::string, std::vector<AttributeMatch*> > mAttributeLookupMap;

	Eris::Entity* mEntity;
	
	CaseBaseStore mCases;
};

class AttributeObserver : public sigc::trackable
{
public:

	AttributeObserver(Eris::Entity* entity, AttributeMatch* match, ModelMapping& modelMapping) : mMatch(match), mModelMapping(modelMapping)
	{
		mSlot = sigc::mem_fun(*this, &AttributeObserver::attributeChanged);
		entity->observe(match->getAttributeName(), mSlot);
	}

protected:
	Eris::Entity::AttrChangedSlot mSlot;
	
	ModelMapping& mModelMapping;
	
	void attributeChanged(const std::string& attributeName, const Atlas::Message::Element& attributeValue)
	{
		mMatch->testAttribute( attributeValue);
		mModelMapping.evaluateChanges();
	}
	AttributeMatch* mMatch;
	
};


}

}

}

#endif
