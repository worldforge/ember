#include "SBGestureMapManager.h"
#include <sb/SBScene.h>
#include <sb/SBGestureMap.h>

namespace SmartBody {

SBGestureMapManager::SBGestureMapManager()
{
}

SBGestureMapManager::~SBGestureMapManager()
{
	std::map<std::string, SBGestureMap*>::iterator iter = _gestureMaps.begin();
	for (; iter != _gestureMaps.end(); iter++)
	{
		delete iter->second;
	}
	_gestureMaps.clear();
}

SBGestureMap* SBGestureMapManager::createGestureMap(std::string gestureName)
{
	
	SBScene* scene = SmartBody::SBScene::getScene();

	std::map<std::string, SBGestureMap*>::iterator iter = _gestureMaps.find(gestureName);
	if (iter != _gestureMaps.end())
	{
		delete iter->second;
		_gestureMaps.erase(iter);
	}
	
	SBGestureMap* map = new SBGestureMap(gestureName);
	_gestureMaps.insert(std::make_pair(gestureName, map));
	return map;
}

void SBGestureMapManager::removeGestureMap(std::string gestureName)
{
	std::map<std::string, SBGestureMap*>::iterator iter = _gestureMaps.find(gestureName);
	if (iter != _gestureMaps.end())
	{
		delete iter->second;
		_gestureMaps.erase(iter);
	}
	else
		LOG("Gesture map %s doesn't exist!", gestureName.c_str());
}

int SBGestureMapManager::getNumGestureMaps()
{
	return _gestureMaps.size();
}

std::vector<std::string> SBGestureMapManager::getGestureMapNames()
{
	std::vector<std::string> gestureMapNames;
	for (std::map<std::string, SBGestureMap*>::iterator iter = _gestureMaps.begin();
		 iter != _gestureMaps.end();
		 iter++)
	{
		gestureMapNames.push_back((*iter).first);
	}

	return gestureMapNames;
}



SBGestureMap* SBGestureMapManager::getGestureMap(std::string gestureName)
{
	std::map<std::string, SBGestureMap*>::iterator iter = _gestureMaps.find(gestureName);
	if (iter != _gestureMaps.end())
	{
		return (*iter).second;
	}
	else
	{
		return NULL;
	}
}

}
