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
class View;
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
class OutfitCase;
class OutfitMatch;
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
	typedef std::vector<MatchBase*> MatchBaseStore;

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
	inline const MatchBaseStore& getMatches();

	void evaluateChanges(std::vector<CaseBase*>& activateQueue, std::vector<CaseBase*>& deactivateQueue);
	
	virtual void setEntity(Eris::Entity* entity);
	
protected:
	ActionStore mActions;

	CaseBase* mParentCase;
	bool mIsTrue, mIsActive;
	
	inline void setState(bool state);
	
	MatchBaseStore mMatches;
	
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

class EntityBaseCase
{
public:
	bool testMatch(Eris::Entity* entity);
	
	void addEntityType(Eris::TypeInfoPtr typeInfo);
	
protected:
	std::vector<Eris::TypeInfoPtr> mEntityTypes;
	virtual void _setState(bool state) = 0;
};

class EntityTypeCase : public Case<EntityTypeMatch>, public EntityBaseCase
{
public:
// 	bool testMatch(Eris::Entity* entity);
	
// 	void addEntityType(Eris::TypeInfoPtr typeInfo);
	
protected:
// 	std::vector<Eris::TypeInfoPtr> mEntityTypes;
	virtual void _setState(bool state);
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

class OutfitCase : public Case<OutfitMatch>, public EntityBaseCase
{
public:
	OutfitCase() {};
	~OutfitCase() {};
// 	bool testMatch(const Atlas::Message::Element& attribute);
	
// 	void addEntityType(Eris::TypeInfoPtr typeInfo);
protected:
	virtual void _setState(bool state);
// 	std::vector<Eris::TypeInfoPtr> mEntityTypes;
};



class MatchBase
{
public:

	MatchBase() : mParentCase(0) {}
	virtual ~MatchBase() {}
	inline void setParentCase(CaseBase* aCase);
	virtual void setEntity(Eris::Entity* entity) = 0;
	

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
	virtual void setEntity(Eris::Entity* entity);
	
	
protected:
	 std::vector<TCase*> mCases;
};

class AttributeDependentMatch
{
public:
	AttributeDependentMatch();
	virtual ~AttributeDependentMatch();
	virtual void testAttribute(const Atlas::Message::Element& attribute) = 0;
    
    void setAttributeObserver(AttributeObserver* observer);

protected:
	
	std::auto_ptr<AttributeObserver> mAttributeObserver;
};

class EntityTypeMatch : public AbstractMatch<EntityTypeCase>
{
public:

	void testEntity(Eris::Entity* entity);
	virtual void setEntity(Eris::Entity* entity);
	
protected:

};

class AttributeMatch : public AbstractMatch<AttributeCase>, public AttributeDependentMatch
{
public:

	AttributeMatch(const std::string& attributeName);
	AttributeMatch(const std::string& attributeName, const std::string& internalAttributeName);

	virtual void testAttribute(const Atlas::Message::Element& attribute);
	
	inline const std::string& getAttributeName();
	virtual void setEntity(Eris::Entity* entity);

protected:

	std::string mAttributeName, mInternalAttributeName;
};

class OutfitMatch : public AbstractMatch<OutfitCase>, public AttributeDependentMatch
{
public:

	OutfitMatch(const std::string& outfitName, Eris::View* view);

	virtual void testAttribute(const Atlas::Message::Element& attribute);
	
	inline const std::string& getOutfitName();
	virtual void setEntity(Eris::Entity* entity);

protected:

	void testEntity(Eris::Entity* entity);
	std::string mOutfitName;
	Eris::View* mView;
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
    
    
protected:

	EntityTypeMatch mRootEntityMatch;
	
	//std::map<std::string, std::vector<AttributeMatch*> > mAttributeLookupMap;

	Eris::Entity* mEntity;
	
	CaseBaseStore mCases;
};

class AttributeObserver : public sigc::trackable
{
public:

	AttributeObserver(AttributeMatch* match, ModelMapping& modelMapping) ;
	AttributeObserver(AttributeDependentMatch* match, ModelMapping& modelMapping, const std::string& attributeName);
	
	void observeEntity(Eris::Entity* entity);
	
protected:
	Eris::Entity::AttrChangedSlot mSlot;
	
	ModelMapping& mModelMapping;
	
	void attributeChanged(const std::string& attributeName, const Atlas::Message::Element& attributeValue);
	
	AttributeDependentMatch* mMatch;
	
	std::string mAttributeName;
	
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
