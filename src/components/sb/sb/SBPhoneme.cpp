#include "SBPhoneme.h"
#include <vhcl.h>

namespace SmartBody{


SBDiphone::SBDiphone()
{
	_fromPhoneme = "";
	_toPhoneme = "";
}

SBDiphone::SBDiphone(const std::string& fromPhoneme, const std::string& toPhoneme)
{
	_fromPhoneme = fromPhoneme;
	_toPhoneme = toPhoneme;
}

SBDiphone::~SBDiphone()
{
	clean();
}

void SBDiphone::addKey(const std::string& viseme, float time, float weight)
{
	// insert the viseme if not existing
	std::map<std::string, std::vector<float> >::iterator iter = _visemeKeysMap.find(viseme);
	if (iter == _visemeKeysMap.end())
	{
		std::vector<float> newFloat;
		_visemeKeysMap.insert(std::make_pair(viseme, newFloat));
	}

	std::vector<float>& curveData = _visemeKeysMap[viseme];
	// insert the data ascendingly
	if (curveData.size() == 0)
	{
		curveData.push_back(time);
		curveData.push_back(weight);
	}
	else
	{
		for (size_t i = 0; i < curveData.size(); i++)
		{
			if ((i % 2) == 0)
			{
				if (curveData[i] > time)
				{
					curveData.insert(curveData.begin() + i, time);
					curveData.insert(curveData.begin() + i + 1, weight);
					return;
				}
			}
		}
		curveData.push_back(time);
		curveData.push_back(weight);
	}
}

void SBDiphone::setKey(const std::string& viseme, std::vector<float>& keys)
{
	std::map<std::string, std::vector<float> >::iterator iter = _visemeKeysMap.find(viseme);
	if (iter == _visemeKeysMap.end())
	{
		std::vector<float> newFloat;
		_visemeKeysMap.insert(std::make_pair(viseme, newFloat));
	}

	_visemeKeysMap[viseme] = keys;
}

std::vector<float>& SBDiphone::getKeys(const std::string& viseme)
{
	std::map<std::string, std::vector<float> >::iterator iter = _visemeKeysMap.find(viseme);
	if (iter == _visemeKeysMap.end())
	{
		std::vector<float> newFloat;
		_visemeKeysMap.insert(std::make_pair(viseme, newFloat));
	}

	return _visemeKeysMap[viseme];
}

std::vector<std::string> SBDiphone::getVisemeNames()
{
	std::vector<std::string> visemeNames;
	std::map<std::string, std::vector<float> >::iterator iter;
	for (iter = _visemeKeysMap.begin(); iter != _visemeKeysMap.end(); iter++)
	{
		visemeNames.push_back(iter->first);
	}
	return visemeNames;
}


int SBDiphone::getNumVisemes()
{
	return _visemeKeysMap.size();
}

void SBDiphone::clean()
{
	std::map<std::string, std::vector<float> >::iterator iter = _visemeKeysMap.begin();
	for (; iter != _visemeKeysMap.end(); iter++)
	{
		iter->second.clear();
	}
	_visemeKeysMap.clear();
}

const std::string& SBDiphone::getFromPhonemeName()
{
	return _fromPhoneme;
}

const std::string& SBDiphone::getToPhonemeName()
{
	return _toPhoneme;
}

}