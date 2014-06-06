#ifndef _SBBEHAVIORSET_
#define _SBBEHAVIORSET_

#include <sb/SBTypes.h>
#include <string>

namespace SmartBody {

class SBBehaviorSet
{
	public:
		SBAPI SBBehaviorSet();
		SBAPI ~SBBehaviorSet();

		SBAPI void setName(const std::string& name);
		SBAPI const std::string& getName();
		SBAPI void setScript(const std::string& name);
		SBAPI const std::string& getScript();
	
	protected:
		std::string _name;
		std::string _script;

};

}

#endif