#include "SBServiceManager.h"
#include <sb/SBService.h>

namespace SmartBody {

SBServiceManager::SBServiceManager() : SBObject()
{
}

SBServiceManager::~SBServiceManager()
{
	_services.clear();
}

int SBServiceManager::getNumServices()
{
	return _services.size();
}

std::vector<std::string> SBServiceManager::getServiceNames()
{
	std::vector<std::string> serviceNames;

	for (std::map<std::string, SBService*>::iterator iter = _services.begin();
		 iter != _services.end();
		 iter++)
	{
		serviceNames.push_back((*iter).first);
	}

	return serviceNames;
}

void SBServiceManager::addService(SBService* service)
{
	std::map<std::string, SBService*>::iterator iter = _services.find(service->getName());
	if (iter != _services.end())
	{
		LOG("Service named %s already exists. Service not added.", service->getName().c_str());
		return;
	}

	_services.insert(std::pair<std::string, SBService*>(service->getName(), service));
}

void SBServiceManager::removeService(const std::string& name)
{
	std::map<std::string, SBService*>::iterator iter = _services.find(name);
	if (iter == _services.end())
	{
		LOG("Service named %s does not exist. Service not removed.", name.c_str());
		return;
	}

	_services.erase(iter);
}

SBService* SBServiceManager::getService(const std::string& name)
{
	std::map<std::string, SBService*>::iterator iter = _services.find(name);
	if (iter == _services.end())
	{
		LOG("Service named %s does not exist.", name.c_str());
		return NULL;
	}

	return (*iter).second;
}


std::map<std::string, SBService*>& SBServiceManager::getServices()
{
	return _services;
}


}