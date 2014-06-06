#include "SBAttributeManager.h"
#include <algorithm>
#include "SBAttribute.h"

namespace SmartBody {

SBAttributeManager::SBAttributeManager() : SBSubject()
{
}

SBAttributeManager::~SBAttributeManager()
{
	for (std::map<std::string, SBAttributeGroup*>::iterator iter = m_groups.begin();
		 iter != m_groups.end();
		 iter++)
	{
		delete (*iter).second;
	}
}

void SBAttributeManager::addGroup(const std::string& name)
{
	for (std::map<std::string, SBAttributeGroup*>::iterator iter = m_groups.begin();
		 iter != m_groups.end();
		 iter++)
	{
		if (name == (*iter).second->getName())
			return;
	}

	SBAttributeGroup* group = new SBAttributeGroup(name);
	m_groups[name] = group;
	resortGroups();
}

SBAttributeGroup* SBAttributeManager::getGroup(const std::string& name, bool createIfNotFound)
{
	for (std::map<std::string, SBAttributeGroup*>::iterator iter = m_groups.begin();
		 iter != m_groups.end();
		 iter++)
	{
		if (name == (*iter).second->getName())
		{
			return (*iter).second;
		}
	}

	if (createIfNotFound)
	{
		SBAttributeGroup* group = new SBAttributeGroup(name);
		m_groups[name] = group;
		resortGroups();
		return group;
	}
	else
	{
		return NULL;
	}
}

struct prioritySort
{
  bool operator()(SBAttributeGroup* a, SBAttributeGroup* b)
  {
	  return a->getPriority() < b->getPriority();
  }
};

void SBAttributeManager::resortGroups()
{
	m_groupsByPriority.clear();
	for (std::map<std::string, SBAttributeGroup*>::iterator iter = m_groups.begin();
		iter != m_groups.end();
		iter++)
	{
		m_groupsByPriority.push_back((*iter).second);
	}

	std::sort(m_groupsByPriority.begin(), m_groupsByPriority.end(), prioritySort());
	
}

std::map<std::string, SBAttributeGroup*>& SBAttributeManager::getGroups()
{
	return m_groups;
}

std::vector<SBAttributeGroup*>& SBAttributeManager::getAttributeGroups()
{
	return m_groupsByPriority;
}

void SBAttributeManager::notifyCreateAttribute(SBAttribute* attr)
{
	notifyObservers();
}

void SBAttributeManager::notifyRemoveAttribute(SBAttribute* attr)
{
	notifyObservers();
}

void SBAttributeManager::notifyPrioritizeAttribute(SBAttribute* attr)
{
	notifyObservers();
}

void SBAttributeManager::notifyHideAttribute(SBAttribute* attr)
{
	notifyObservers();
}

};
