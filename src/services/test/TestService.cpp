#include "TestService.h"

#include <iostream.h>

namespace dime
{

	/* ctor */
	     TestService::TestService()
	       {
		  myTestMessage = "Hello WorldForge !!!";
		  //setDescription("I am a simple test service");
		  // setActive = 0;	// inactive
		  //setStatus(Service::OK);	// stopped

        setName("Test Service");
        setDescription("Basic test service for testing and debugging.");
// TODO(zzorn, 2002-01-19): Set the status of the service to OK.
//        setStatus( Service::OK );


	       }

	/* Method for starting the test service 	*/
	    Service::Status TestService::start()
	       {
		  cout << "I'm the Test Service Starting" << endl;
		  setStatus(Service::OK);
          setRunning( true );
		  return Service::OK;
	       }

	/* Interface method for stopping the test service 	*/
	     void TestService::stop(int code)
	       {
		  cout << "I'm the Test Service Stopping" << endl;
		  //setStatus(code);
		  //setActive(0);
          setRunning( false );
	       }

} // namespace dime
