#ifndef _SBPHONEMEMANAGER_H_
#define _SBPHONEMEMANAGER_H_

#include <sb/SBTypes.h>
#include <vector>
#include <string>
#include <map>

namespace SmartBody{

class SBDiphone;

class SBDiphoneManager
{
	public:
		SBAPI SBDiphoneManager();
		SBAPI ~SBDiphoneManager();

		SBAPI std::vector<std::string> getCommonPhonemes();
	
		SBAPI SBDiphone* createDiphone(const std::string& fromPhoneme, const std::string& toPhoneme, const std::string& name);
		SBAPI std::vector<SBDiphone*>& getDiphones(const std::string& name);
		SBAPI SBDiphone* getDiphone(const std::string& fromPhoneme, const std::string& toPhoneme, const std::string& name);
		SBAPI std::vector<std::string> getDiphoneMapNames();
		SBAPI int getNumDiphoneMap();
		SBAPI int getNumDiphones(const std::string& name);
		SBAPI void normalizeCurves(const std::string& name);
		SBAPI void deleteDiphoneSet(const std::string& name);

	protected:
		std::map<std::string, std::vector<SBDiphone*> > _diphoneMap;
};

}
#endif
