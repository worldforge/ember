#include "ConfigService.h"

#include <iostream.h>

using namespace dime::framework;

namespace dime
{
   namespace services
     {
	namespace config
	  {

	/* ctor */
	ConfigService::ConfigService()
	{
		//myTestMessage = "Hello WorldForge !!!";
		  //setDescription("I am a simple test service");
		  // setActive = 0;	// inactive
		  //setStatus(0);	// stopped

        //setName("Test Service");
        //setDescription("Basic test service for testing and debugging.");
// TODO(zzorn, 2002-01-19): Set the status of the service to OK.
//        setStatus( Service::Status::OK );

	}

	/* Method for starting the test service 	*/
	int ConfigService::start()
	{
		setStatus(1);
        setRunning( true );
		// TODO: has to call the Persistance Service and load user configuration data into the hashtable (agla)
		return 0;
	}

	/* Interface method for stopping the test service 	*/
	void ConfigService::stop(int code)
	{
		// TODO: stop the service, save any data that might be unsaved (agla)
	}

	void getConfig(std::string StringKey)   // what the hell should this return?
	{
		// TODO: implement. Easier said than done :)  (agla)
	}
	
	bool ConfigService::saveConfig()
	{
		// TODO: call the Persistance Service and save the data.
		return true;  // or false, if couldn't write to disk
	}
	
		} // namespace testservice
	} // namespace services
} // namespace dime

