#include <sb/SBPhonemeManager.h>
#include <sb/SBPhoneme.h>
#include <algorithm>
#include <vhcl.h>

namespace SmartBody {

SBDiphoneManager::SBDiphoneManager()
{
}

std::vector<std::string> SBDiphoneManager::getCommonPhonemes()
{
	std::vector<std::string> commonPhonemes;
#if 1
	commonPhonemes.push_back("_");    /// silence
	commonPhonemes.push_back("Ah");   /// Viseme for aa, ae, ah
	commonPhonemes.push_back("Aa");   /// Viseme for aa
	commonPhonemes.push_back("Ao");   /// ao
	commonPhonemes.push_back("Eh");   /// ey, eh, uh
	commonPhonemes.push_back("Er");   /// er
	commonPhonemes.push_back("Ih");   /// y, iy, ih, ix
	commonPhonemes.push_back("W");   /// w, uw
	commonPhonemes.push_back("Ow");   /// ow
	commonPhonemes.push_back("Aw");   /// aw
	commonPhonemes.push_back("Oy");  /// oy
	commonPhonemes.push_back("Ay");  /// ay
	commonPhonemes.push_back("H");  /// h
	commonPhonemes.push_back("R");   /// r
	commonPhonemes.push_back("L");   /// l
	commonPhonemes.push_back("Z");   /// s, z
	commonPhonemes.push_back("Sh");   /// sh, ch, jh, zh
	commonPhonemes.push_back("Th");  /// th, dh
	commonPhonemes.push_back("F");   /// f, v
	commonPhonemes.push_back("D");   /// d, t, n   - also try NG: 2 to 1 against
	commonPhonemes.push_back("KG");  /// k, g, ,ng   - also try NG: 2 to 1 against
	commonPhonemes.push_back("BMP"); /// p, b, m
#else
	commonPhonemes.push_back("_");    /// silence
	commonPhonemes.push_back("Ah");   /// Viseme for aa, ae, ah
	commonPhonemes.push_back("Eh");   /// ey, eh, uh
	commonPhonemes.push_back("Er");   /// er
	commonPhonemes.push_back("Ih");   /// y, iy, ih, ix
	commonPhonemes.push_back("W");   /// w, uw
	commonPhonemes.push_back("Ow");   /// ow
	commonPhonemes.push_back("R");   /// r
	commonPhonemes.push_back("L");   /// l
	commonPhonemes.push_back("Z");   /// s, z
	commonPhonemes.push_back("Sh");   /// sh, ch, jh, zh
	commonPhonemes.push_back("F");   /// f, v
	commonPhonemes.push_back("BMP"); /// p, b, m
#endif

	return commonPhonemes;
}

SBDiphoneManager::~SBDiphoneManager()
{
	std::map<std::string, std::vector<SBDiphone*> >::iterator iter = _diphoneMap.begin();
	for (; iter != _diphoneMap.end(); iter++)
	{
		std::vector<SBDiphone*>& diphones = getDiphones(iter->first);
		for (size_t i = 0; i < diphones.size(); i++)
		{
			delete diphones[i];
			diphones[i] = NULL;
		}
		diphones.clear();
	}
}

void SBDiphoneManager::deleteDiphoneSet(const std::string& name)
{
	if (_diphoneMap.find(name) != _diphoneMap.end())
	{
		_diphoneMap.erase(name);
	}
}

SBDiphone* SBDiphoneManager::createDiphone(const std::string& fromPhoneme, const std::string& toPhoneme, const std::string& name)
{
	std::string lowerCaseFromPhoneme = fromPhoneme;
	std::string lowerCaseToPhoneme = toPhoneme;
	std::transform(fromPhoneme.begin(), fromPhoneme.end(), lowerCaseFromPhoneme.begin(), ::tolower);
	std::transform(toPhoneme.begin(), toPhoneme.end(), lowerCaseToPhoneme.begin(), ::tolower);
	SBDiphone* diphone = getDiphone(lowerCaseFromPhoneme, lowerCaseToPhoneme, name);
	if (diphone)
	{
		;//LOG("Diphone set %s already contain diphone pair %s to %s, return existing one.", name.c_str(), lowerCaseFromPhoneme.c_str(), lowerCaseToPhoneme.c_str());
	}
	else
	{
		diphone = new SBDiphone(lowerCaseFromPhoneme, lowerCaseToPhoneme);
		_diphoneMap[name].push_back(diphone);
	}
	return diphone;
}

std::vector<SBDiphone*>& SBDiphoneManager::getDiphones(const std::string& name)
{
	std::map<std::string, std::vector<SBDiphone*> >::iterator iter = _diphoneMap.find(name);
	if (iter == _diphoneMap.end())
	{
		std::vector<SBDiphone*> newDiphones;
		_diphoneMap.insert(std::make_pair(name, newDiphones));
	}
	return _diphoneMap[name];
}

SBDiphone* SBDiphoneManager::getDiphone(const std::string& fromPhoneme, const std::string& toPhoneme, const std::string& name)
{
	std::string lowerCaseFromPhoneme = fromPhoneme;
	std::string lowerCaseToPhoneme = toPhoneme;
	std::transform(fromPhoneme.begin(), fromPhoneme.end(), lowerCaseFromPhoneme.begin(), ::tolower);
	std::transform(toPhoneme.begin(), toPhoneme.end(), lowerCaseToPhoneme.begin(), ::tolower);
	std::vector<SBDiphone*>& diphones = getDiphones(name);
	for (size_t i = 0; i < diphones.size(); i++)
	{
		if (diphones[i]->getFromPhonemeName() == lowerCaseFromPhoneme && diphones[i]->getToPhonemeName() == lowerCaseToPhoneme)
		{
			return diphones[i];
		}
	}
	return NULL;
}

int SBDiphoneManager::getNumDiphoneMap()
{
	return _diphoneMap.size();
}

int SBDiphoneManager::getNumDiphones(const std::string& name)
{
	std::vector<SBDiphone*>& diphones = getDiphones(name);
	return diphones.size();
}

std::vector<std::string> SBDiphoneManager::getDiphoneMapNames()
{
	std::vector<std::string> diphoneMaps;
	for (std::map<std::string, std::vector<SBDiphone*> >::iterator iter = _diphoneMap.begin();
		 iter != _diphoneMap.end();
		 iter++)
	{
		diphoneMaps.push_back((*iter).first);
		
	}

	return diphoneMaps;

}

void SBDiphoneManager::normalizeCurves(const std::string& name)
{
	std::map<std::string, std::vector<SBDiphone*> >::iterator iter = _diphoneMap.find(name);
	if (iter != _diphoneMap.end())
	{
		// iterate through the phone bigrams
		for (std::map<std::string, std::vector<SBDiphone*> >::iterator iter2 = _diphoneMap.begin();
			 iter2 != _diphoneMap.end();
			 iter2++)
		{
			// get the phone bigram
			std::vector<SBDiphone*>& diphones = getDiphones(iter->first);
			for (std::vector<SBDiphone*>::iterator diphoneIter = diphones.begin();
				 diphoneIter != diphones.end();
				 diphoneIter++)
			{
				SBDiphone* diphone = (*diphoneIter);
				// get the keys and find the largest value in the list
				float maxVal = -1.0f;
				std::vector<std::string> visemes = diphone->getVisemeNames();
				for (std::vector<std::string>::iterator visemeIter = visemes.begin();
					 visemeIter != visemes.end();
					 visemeIter++)
				{
					const std::string& viseme = (*visemeIter);
					std::vector<float>& keys = diphone->getKeys(viseme);
					bool isTime = true;
					for (std::vector<float>::iterator keyIter = keys.begin();
						 keyIter != keys.end();
						 keyIter++)
					{
						float& val = (*keyIter);
						if (isTime)
						{
							isTime = false;
						}
						else
						{
							if (val > maxVal)
								maxVal = val;
							isTime = true;
						}
					}
				}

				if (maxVal <= -0.0f )
				{
					continue;
				}

				// normalize the values according the the scale
				LOG("Normalize scale is %f", maxVal);
				float scale = 1.0f / ((1.0f + maxVal) / 2.0f);
				for (std::vector<std::string>::iterator visemeIter = visemes.begin();
					 visemeIter != visemes.end();
					 visemeIter++)
				{
					const std::string& viseme = (*visemeIter);
					std::vector<float>& keys = diphone->getKeys(viseme);
					bool isTime = true;
					for (std::vector<float>::iterator keyIter = keys.begin();
						 keyIter != keys.end();
						 keyIter++)
					{
						float& val = (*keyIter);
						if (isTime)
						{
							isTime = false;
						}
						else
						{
							val *= scale;
							isTime = true;
						}
					}
				}

			}
		}
	}
}


}