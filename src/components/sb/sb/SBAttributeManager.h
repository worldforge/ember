#ifndef DATTRIBUTEMANAGER_H
#define DATTRIBUTEMANAGER_H

#include <sb/SBTypes.h>
#include "SBSubject.h"
#include <map>
#include <vector>
#include <string>

namespace SmartBody {

class SBAttribute;
class SBAttributeGroup;

class SBAttributeManager : public SBSubject
{
	public:
		SBAPI SBAttributeManager();
		SBAPI ~SBAttributeManager();

		SBAPI void addGroup(const std::string& name);
		SBAPI SBAttributeGroup* getGroup(const std::string& name, bool createIfNotFound = false);
		SBAPI std::map<std::string, SBAttributeGroup*>& getGroups();
		SBAPI std::vector<SBAttributeGroup*>& getAttributeGroups();

		SBAPI void notifyCreateAttribute(SBAttribute* attr);
		SBAPI void notifyRemoveAttribute(SBAttribute* attr);
		SBAPI void notifyPrioritizeAttribute(SBAttribute* attr);
		SBAPI void notifyHideAttribute(SBAttribute* attr);

	protected:
		void resortGroups();

		std::map<std::string, SBAttributeGroup*> m_groups;
		std::vector<SBAttributeGroup*> m_groupsByPriority;
};

};

#endif
