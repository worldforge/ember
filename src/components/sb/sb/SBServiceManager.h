#ifndef _SBSERVICEMANAGER_H_
#define _SBSERVICEMANAGER_H_

#include <sb/SBTypes.h>
#include <vhcl.h>
#include <sb/SBObject.h>
#include <string>
#include <map>

namespace SmartBody {

class SBService;

class SBServiceManager : public SBObject
{
	public:
		SBAPI SBServiceManager();
		SBAPI ~SBServiceManager();

		SBAPI int getNumServices();
		SBAPI std::vector<std::string> getServiceNames();

		SBAPI SBService* getService(const std::string& name);
		SBAPI void addService(SBService* service);
		SBAPI void removeService(const std::string& name);

		SBAPI std::map<std::string, SBService*>& getServices();
		
	protected:
		std::map<std::string, SBService*> _services;

};

}


#endif