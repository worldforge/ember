#ifndef _SBBEHAVIORSETMANAGER_
#define _SBBEHAVIORSETMANAGER_

#include <sb/SBTypes.h>
#include <map>
#include <vector>
#include "SBBehaviorSet.h"

class SBBehaviorSet;

namespace SmartBody
{

class SBBehaviorSetManager
{
	public:
		SBAPI SBBehaviorSetManager();
		SBAPI ~SBBehaviorSetManager();

		SBAPI SBBehaviorSet* createBehaviorSet(const std::string& name);
		SBAPI int getNumBehaviorSets();
		SBAPI std::map<std::string, SBBehaviorSet*>& getBehaviorSets();
		SBAPI SBBehaviorSet* getBehaviorSet(const std::string& name);
		SBAPI void addBehaviorSet(const std::string& name, SBBehaviorSet* set);
		SBAPI void removeBehaviorSet(const std::string& name);
		SBAPI void removeAllBehaviorSets();

	protected:
		std::map<std::string, SBBehaviorSet*> _behaviorSets;

};

}

#endif