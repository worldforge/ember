
#include "ServiceManager.h"

#include <iostream.h>

namespace dime
{
   namespace services
     {

     /** Typedef for the iterator */
	typedef std::vector<Service*> Container;
	typedef Container::iterator Iter;

	
	/* ctor */
	ServiceManager::ServiceManager()
	{
	}

	/* dtor */
	ServiceManager::~ServiceManager()
	{
	}

	
	void ServiceManager::stopAll(int code)
	{
	   cout << "one manager to stop them all" << endl;
	}
		
	void ServiceManager::listAll()
	{
		cout << "this should list them all" << endl;
		for(Iter i = myServiceVector.begin(); i != myServiceVector.end(); i++)
		{	
			cout << (*i)->getName() << endl;
	    }
	}
	
	

	bool ServiceManager::addService( Service *service )
	{
		myServiceVector.push_back(service);
		return true; // TODO: make boolean false if service couldn't be inserted
	}
		
	bool ServiceManager::removeService( Service *service )
	{
		for(Iter j = myServiceVector.begin(); j != myServiceVector.end(); j++)
			{	
				if ((service)==*j)
				{
					myServiceVector.erase(j);
				}
			}
		return true; // TODO: make it return proper value
	}	
	
	void ServiceManager::deleteAll()
	{
	   for(Iter j = myServiceVector.begin(); j != myServiceVector.end(); j++)
	     {	
		delete *j;
	     }
	}
	
	
     }
   // namespace services
}
// namespace dime
