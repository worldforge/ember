#ifndef _SBJOINTMAPMANAGER_
#define _SBJOINTMAPMANAGER_

#include <sb/SBTypes.h>
#include <string>
#include <vector>
#include <map>

namespace SmartBody {

class SBJointMap;

class SBJointMapManager 
{
	public:
		SBAPI SBJointMapManager();
		SBAPI ~SBJointMapManager();
	
		SBAPI SmartBody::SBJointMap* createJointMap(const std::string& name);
		SBAPI SmartBody::SBJointMap* getJointMap(const std::string& name);
		SBAPI std::vector<std::string> getJointMapNames();
		SBAPI void removeJointMap(const std::string& name);
		SBAPI void removeAllJointMaps();

	private:
		std::map<std::string, SmartBody::SBJointMap*> _jointMaps;

};


}

#endif
