
#include "ServiceManager.h"

#include <iostream>

namespace dime
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

    void ServiceManager::startAll()
	{
	   for(Iter j = myServiceVector.begin(); j != myServiceVector.end(); j++)
		{	
			(*j)->start();
		}
	}
		
	void ServiceManager::stopAll(int code)
	{
	   for(Iter j = myServiceVector.begin(); j != myServiceVector.end(); j++)
		{	
			(*j)->stop(0);
		}
	}
		
	void ServiceManager::listAll()
	{
		cout << "listing all services" << endl;
		for(Iter i = myServiceVector.begin(); i != myServiceVector.end(); i++)
		{	
			//HINT: Always use .data() for compatibility to MSVC
			cout << (*i)->getName().data() << endl;
			cout << "TODO!" << endl;
	    }
	}
	
	

	bool ServiceManager::addService( Service *pservice )
	{
		for(Iter j = myServiceVector.begin(); j != myServiceVector.end(); j++)
			{	
				if ( ((pservice)->getName()) == ((*j)->getName()) )
				{
					return false; // services can't be duplicated
				}
			}
		myServiceVector.push_back(pservice);
		return true;
	}
		
	bool ServiceManager::removeService( Service *service )
	{
		for(Iter j = myServiceVector.begin(); j != myServiceVector.end(); j++)
			{	
				if ((service)==*j)
				{
					myServiceVector.erase(j);
					return true;
				}
			}
		return false; // the service was not there
	}	

} // namespace dime
