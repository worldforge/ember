#include "TestService.h"

#include <iostream.h>

namespace dime
{
   namespace services
     {
	namespace test
	  {

	/* ctor */
	     TestService::TestService()
	       {
		  myTestMessage = "Hello WorldForge !!!";
		  //setDescription("I am a simple test service");
		  // setActive = 0;	// inactive
					  setStatus(0);	// stopped
	       }

	/* Method for starting the test service 	*/
	     int TestService::start()
	       {
		  cout << "I'm the Test Service Starting" << endl;
		  setStatus(1);
		  return 0;
	       }

	/* Interface method for stopping the test service 	*/
	     void TestService::stop(int code)
	       {
		  cout << "I'm the Test Service Stopping" << endl;
		  setStatus(code);
		  //setActive(0);
	       }

	  }
	// namespace testservice
     }
   // namespace services
}
// namespace dime
//
//
