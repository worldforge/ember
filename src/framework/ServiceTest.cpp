/* copyleft and stuff */

#include "Service.h"
#include "ServiceManager.h"
#include "AbstractService.h"
#include "services/test/TestService.h"
//#include "LoggingService.h"

#include <iostream.h>


int main()
{

	cout << "Dime Service Test running" << endl;

	/* Test the Test Service */
	cout << "***TESTING TEST SERVICE***" << endl;
	dime::services::test::TestService myTestService;
	myTestService.start();
	cout << "My name is: " << myTestService.getName() << endl;
	cout << myTestService.getDescription() << endl;
	myTestService.stop(0);
	cout << "***END OF TESTING TEST SERVICE***" << endl;
	cout << "***TESTING SERVICE MANAGER***" << endl;
	cout << "***adding and listing one service***" << endl;
	dime::services::ServiceManager myServiceManager;
	myServiceManager.addService(&myTestService);
	myServiceManager.listAll();
	cout << "***adding and listing the second service***" << endl;
	dime::services::test::TestService myTestService2;
	myServiceManager.addService(&myTestService2);
	myServiceManager.listAll();
	cout << "***removing the first service, listing again***" << endl;
	myServiceManager.removeService(&myTestService);
	myServiceManager.listAll();
	cout << "***END OF TESTING SERVICE MANAGER***" << endl;
	cout << "Dime Service Test ends" << endl;
	//dime::services::LoggingService ls;
   
	return 0;
   
}
