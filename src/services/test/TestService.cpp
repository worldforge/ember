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
		  setStatus(0);	// stopped

        setName("Test Service");
        setDescription("Basic test service for testing and debugging.");
// TODO(zzorn, 2002-01-19): Set the status of the service to OK.
//        setStatus( Service::Status::OK );


	       }

	/* Method for starting the test service 	*/
	     int TestService::start()
	       {
		  cout << "I'm the Test Service Starting" << endl;
		  setStatus(1);
          setRunning( true );
		  return 0;
	       }

	/* Interface method for stopping the test service 	*/
	     void TestService::stop(int code)
	       {
		  cout << "I'm the Test Service Stopping" << endl;
		  setStatus(code);
		  //setActive(0);
          setRunning( false );
	       }

} // namespace dime
