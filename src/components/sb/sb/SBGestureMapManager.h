#ifndef _SBGESTUREMAPMANAGER_H_
#define _SBGESTUREMAPMANAGER_H_

#include <sb/SBTypes.h>
#include <string>
#include <map>
#include <vector>

namespace SmartBody {

class SBGestureMap;

class SBGestureMapManager
{
	public:
		SBAPI SBGestureMapManager();
		SBAPI ~SBGestureMapManager();

		SBAPI SBGestureMap* createGestureMap(std::string gestureName);
		SBAPI void removeGestureMap(std::string gestureName);
		SBAPI int getNumGestureMaps();
		SBAPI std::vector<std::string> getGestureMapNames();
		SBAPI SBGestureMap* getGestureMap(std::string gestureName);

	protected:
		std::map<std::string, SBGestureMap*> _gestureMaps;
};

}
#endif