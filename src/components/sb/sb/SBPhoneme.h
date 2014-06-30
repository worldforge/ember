#ifndef _SBPHONEME_H_
#define _SBPHONEME_H_

#include <sb/SBTypes.h>
#include <vector>
#include <string>
#include <map>

namespace SmartBody {

class SBDiphone
{
public:
	SBAPI SBDiphone();
	SBAPI SBDiphone(const std::string& fromPhoneme, const std::string& toPhoneme);
	SBAPI ~SBDiphone();

	SBAPI const std::string& getFromPhonemeName();
	SBAPI const std::string& getToPhonemeName();

	SBAPI void addKey(const std::string& viseme, float time, float weight);
	SBAPI void setKey(const std::string& viseme, std::vector<float>& keys);
	SBAPI std::vector<float>& getKeys(const std::string& viseme);
	SBAPI std::vector<std::string> getVisemeNames();
	SBAPI int getNumVisemes();
	SBAPI void clean();

private:
	std::string _fromPhoneme;
	std::string _toPhoneme;
	std::map<std::string, std::vector<float> > _visemeKeysMap;
};

}

#endif
