#include "ConfigService.h"

#include <iostream.h>

namespace dime
{
	namespace services
	{

	/* ctor */
	ConfigService::ConfigService()
	{
	    setName("Configuration Service");
		setDescription("Service for management of dime user-defined configuration");
		// TODO(zzorn, 2002-01-19): Set the status of the service to OK.
		//        setStatus( Service::Status::OK );
	}

	/* Method for starting the test service 	*/
	int ConfigService::start()
	{
		setStatus(1);
        setRunning( true );
		// TODO: has to call the Persistance Service and load user configuration data into the hashtable (agla)


		inputMap["KEY_UP"]="move_forward";		
		inputMap["KEY_DOWN"]="move_backwards";
			
		return 0;
	
	}

	/* Interface method for stopping the test service 	*/
	void ConfigService::stop(int code)
	{
		// TODO: stop the service, save any data that might be unsaved (agla)
	}

	const char* ConfigService::getParam(const char* StringKey)   // what the hell should this return?
	{
		return inputMap[StringKey];
	}
	
	bool ConfigService::saveConfig()
	{
		// TODO: call the Persistance Service and save the data.
		return true;  // or false, if couldn't write to disk
	}
	
	} // namespace services
} // namespace dime

