
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

	
	void ServiceManager::stopAll(int code)
	{
	   cout << "one manager to stop them all" << endl;
	}
	
	     
	     
	
	void ServiceManager::listAll()
	{
	   cout << "this should list them all" << endl;
	}
	
	
	
	
	void ServiceManager::deleteAll()
	{
	   for(Iter j = myServiceVector.begin();
	       j != myServiceVector.end(); j++)
	     {	
		delete *j;
	     }
	}
	
	
     }
   // namespace services
}
// namespace dime
