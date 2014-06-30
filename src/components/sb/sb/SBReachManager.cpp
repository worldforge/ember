#include "SBReachManager.h"

#include <sb/SBScene.h>
#include <sb/SBReach.h>

namespace SmartBody {

SBReachManager::SBReachManager()
{
}

SBReachManager::~SBReachManager()
{
}

SBReach* SBReachManager::createReach(std::string characterName)
{
	
	SBScene* scene = SmartBody::SBScene::getScene();
	// get the character
	SBCharacter* character = scene->getCharacter(characterName);
	if (!character)
	{
		LOG("Character named %s does not exist.", characterName.c_str());
		return NULL;
	}

	std::map<std::string, SBReach*>::iterator iter = _reaches.find(characterName);
	if (iter != _reaches.end())
	{
		// remove the old reach data
		SBReach* reach = (*iter).second;
		removeReach(reach);
		_reaches.erase(iter);
	}

	SBReach* reach = new SBReach(character);
	//_reaches.insert(pair<std::string, SBReach*>(characterName, reach));
	_reaches[characterName] = reach;
	return reach;
}

void SBReachManager::removeReach(SBReach* reach)
{
	SBCharacter* character = reach->getCharacter();
	 
	// clean up all the reach structures...
	// ...
	// ...
}

int SBReachManager::getNumReaches()
{
	return _reaches.size();
}

SBReach* SBReachManager::getReach(std::string characterName)
{
	std::map<std::string, SBReach*>::iterator iter = _reaches.find(characterName);
	if (iter != _reaches.end())
	{
		return (*iter).second;
	}
	else
	{
		return NULL;
	}
}



}
