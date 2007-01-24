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

#include "AttributeComparers.h"


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


#include <vector>
#include <iostream>
#include <memory>

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

namespace AttributeComparers {
class AttributeComparerWrapper;
}

class EntityTypeMatch;
class EntityTypeCase;
class AttributeCase;
class AttributeMatch;
class ModelMapping;
class MatchBase;
class Match;
class CaseBase;
class AttributeObserver;

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
	
	inline void setCase(CaseBase* ownerCase);

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

class CaseBase 
{
public:
	typedef std::vector<Action*> ActionStore;

	CaseBase();
	virtual ~CaseBase();
	
	inline void setParentCase(CaseBase* aCase);

	inline bool getIsTrue();
	inline bool getIsActive();
	inline bool getIsTrueToRoot();
	
	void addAction(Action* action);
	inline const ActionStore& getActions();
	
	void activateActions();
	void deactivateActions();

	void addMatch(MatchBase* match);
	inline const std::vector<MatchBase*>& getMatches();

	void evaluateChanges();
protected:
	ActionStore mActions;

	CaseBase* mParentCase;
	bool mIsTrue, mIsActive;
	
	inline void setState(bool state);
	
	std::vector<MatchBase*> mMatches;
	
};

template <class TMatch>
class Case : public CaseBase
{
public:
	
	Case() : mParentMatch(0) {}
	virtual ~Case() { };
	
 	inline void setParentMatch(TMatch* aMatch);

protected:
	
 	TMatch* mParentMatch;
	
};

class EntityTypeCase : public Case<EntityTypeMatch>
{
public:
	bool testMatch(Eris::Entity* entity);
	
	void addEntityType(Eris::TypeInfoPtr typeInfo);
	
protected:
	std::vector<Eris::TypeInfoPtr> mEntityTypes;
};



class AttributeCase : public Case<AttributeMatch>
{
public:
	AttributeCase() {};
	~AttributeCase() {};
	bool testMatch(const Atlas::Message::Element& attribute);
	
	/**
	Default to a string comparer.
	*/
	void setAttributeValue(const std::string& value);
	void setAttributeComparerWrapper(AttributeComparers::AttributeComparerWrapper* comparerWrapper);
protected:
	//std::string mValue;
	std::auto_ptr<AttributeComparers::AttributeComparerWrapper> mComparerWrapper;
};





class MatchBase
{
public:

	MatchBase() : mParentCase(0) {}
	virtual ~MatchBase() {}
	inline void setParentCase(CaseBase* aCase);

protected:
	 CaseBase* mParentCase;
};

template <class TCase>
class AbstractMatch : public MatchBase
{
public:

	AbstractMatch();
	virtual ~AbstractMatch();
	void addCase(TCase* aCase) {
		mCases.push_back(aCase); 
		aCase->setParentCase(mParentCase);
	}
	std::vector<TCase*>& getCases() { return mCases;}

protected:
	 std::vector<TCase*> mCases;
};

class EntityTypeMatch : public AbstractMatch<EntityTypeCase>
{
public:

	void testEntity(Eris::Entity* entity);
	
protected:

};

class AttributeMatch : public AbstractMatch<AttributeCase>
{
public:

	AttributeMatch(const std::string& attributeName);

	void testAttribute(const Atlas::Message::Element& attribute);
	
	inline const std::string& getAttributeName();

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
	typedef std::vector<AttributeObserver*> AttributeObserverStore;
    ModelMapping(Eris::Entity* entity);

    ~ModelMapping();
    
    EntityTypeMatch& getRootEntityMatch() { return mRootEntityMatch;}
        
    void evaluateChanges();
    
    void addCase(CaseBase* aCase);
    
    void initialize();
    
    void addAttributeObserver(AttributeObserver* observer);
    
protected:

	EntityTypeMatch mRootEntityMatch;
	
	//std::map<std::string, std::vector<AttributeMatch*> > mAttributeLookupMap;

	Eris::Entity* mEntity;
	
	CaseBaseStore mCases;
	AttributeObserverStore mAttributeObservers;
};

class AttributeObserver : public sigc::trackable
{
public:

	AttributeObserver(Eris::Entity* entity, AttributeMatch* match, ModelMapping& modelMapping) ;
	AttributeObserver(Eris::Entity* entity, AttributeMatch* match, ModelMapping& modelMapping,const std::string& attributeName);
	
protected:
	Eris::Entity::AttrChangedSlot mSlot;
	
	ModelMapping& mModelMapping;
	
	void attributeChanged(const std::string& attributeName, const Atlas::Message::Element& attributeValue);
	
	AttributeMatch* mMatch;
	
};


void MatchBase::setParentCase(CaseBase* aCase) 
{ 
	mParentCase = aCase;
}

const std::string& AttributeMatch::getAttributeName()
{
	return mAttributeName;
}


template <class TMatch>
void Case<TMatch>::setParentMatch(TMatch* aMatch) 
{
	mParentMatch = aMatch;
}

void CaseBase::setState(bool state) 
{
	mIsTrue = state;
}

const std::vector<MatchBase*>& CaseBase::getMatches() 
{
	return mMatches;
}
	
const CaseBase::ActionStore& CaseBase::getActions() 
{ 
	return mActions; 
};

void CaseBase::setParentCase(CaseBase* aCase) {
	mParentCase = aCase; 
}

bool CaseBase::getIsTrue() 
{ 
	return mIsTrue; 
}

bool CaseBase::getIsActive() 
{ 
	return mIsActive;
}

bool CaseBase::getIsTrueToRoot() {
	if (!mParentCase) {
		return mIsTrue;
	} else {
		return mIsTrue && mParentCase->getIsTrueToRoot();
	}
}
	
void Action::setCase(CaseBase* ownerCase)
{
	mOwnerCase = ownerCase;
}

}

}

}

#endif
