#include "MetaserverService.h"

#include <iostream.h>

namespace dime
{

	/* ctor */
	MetaserverService::MetaserverService()
	{
	    setName("Metaserver Service");
		setDescription("Service for Metaserver session");
		// TODO(zzorn, 2002-01-19): Set the status of the service to OK.
		//        setStatus( Service::Status::OK );
	}

	/* Method for starting this service 	*/
	int MetaserverService::start()
	{
		setStatus(1);
        setRunning( true );

		return 0;
	
	}

	/* Interface method for stopping this service 	*/
	void MetaserverService::stop(int code)
	{
		// TODO: stop the service
	}

} // namespace dime

