#include "SBBehaviorSetManager.h"


namespace SmartBody {

SBBehaviorSetManager::SBBehaviorSetManager()
{

}

SBBehaviorSetManager::~SBBehaviorSetManager()
{
	removeAllBehaviorSets();
}

SBBehaviorSet* SBBehaviorSetManager::createBehaviorSet(const std::string& name)
{
	removeBehaviorSet(name);
	SBBehaviorSet* b = new SBBehaviorSet();
	b->setName(name);
	addBehaviorSet(name, b);
	return b;
}

int SBBehaviorSetManager::getNumBehaviorSets()
{
	return _behaviorSets.size();
}

std::map<std::string, SBBehaviorSet*>& SBBehaviorSetManager::getBehaviorSets()
{
	return _behaviorSets;
}

SBBehaviorSet* SBBehaviorSetManager::getBehaviorSet(const std::string& name)
{
	std::map<std::string, SBBehaviorSet*>::iterator iter = _behaviorSets.find(name);
	if (iter == _behaviorSets.end())
		return NULL;
	return (*iter).second;
}

void SBBehaviorSetManager::addBehaviorSet(const std::string& name, SBBehaviorSet* set)
{
	std::map<std::string, SBBehaviorSet*>::iterator iter = _behaviorSets.find(name);
	if (iter != _behaviorSets.end())
	{
		delete (*iter).second;
	}
	_behaviorSets[name] = set;

}

void SBBehaviorSetManager::removeBehaviorSet(const std::string& name)
{
	std::map<std::string, SBBehaviorSet*>::iterator iter = _behaviorSets.find(name);
	if (iter != _behaviorSets.end())
	{
		delete (*iter).second;
		_behaviorSets.erase(iter);
	}
	

}

void SBBehaviorSetManager::removeAllBehaviorSets()
{
	for (std::map<std::string, SBBehaviorSet*>::iterator iter = _behaviorSets.begin();
		 iter != _behaviorSets.end();
		 iter++)
	{
		delete (*iter).second;
	}
	_behaviorSets.clear();

}

}

