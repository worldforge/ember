/*
    Copyright (C) 2002  Miguel Guzman Miranda [Aglanor]

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

//#include "ServiceManager.h"
#include "test/TestService.h"
#include "config/ConfigService.h"
#include "metaserver/MetaserverService.h"
#include "Eris/PollDefault.h"
//#include "LoggingService.h"

#include <iostream.h>


int main()
{

	cout << "Dime Service Test running" << endl;

	cout << "***TESTING TEST SERVICE***" << endl;
	dime::TestService myTestService;
	myTestService.start();
	cout << "My name is: " << myTestService.getName() << endl;
	cout << myTestService.getDescription() << endl;
	myTestService.stop(0);
	cout << "***END TEST OF TEST SERVICE***" << endl;
	
	cout << "***TESTING CONFIG SERVICE***" << endl;
	dime::ConfigService myConfigService;
	myConfigService.start();
	cout << "My name is: " << myConfigService.getName() << endl;
	cout << myTestService.getDescription() << endl;
	myConfigService.stop(0);
	cout << "***END TEST OF CONFIG SERVICE***" << endl;	
	
	cout << "***TESTING METASERVER SERVICE***" << endl;
	dime::MetaserverService myMsService;
	myMsService.start();
	Eris::PollDefault::Poll()
	while(true)
	{
//		myMsService.poll();
	}
	cout << "My name is: " << myMsService.getName() << endl;
	cout << myMsService.getDescription() << endl;
	myMsService.stop(0);
	//delete myMsService;
	cout << "***END TEST OF METASERVER SERVICE***" << endl;		
	
	//cout << "***TESTING SERVICE MANAGER***" << endl;
	//cout << "***adding and listing one service***" << endl;
	//dime::services::ServiceManager myServiceManager;
	//myServiceManager.addService(&myTestService);
	//myServiceManager.listAll();
	//cout << "***adding and listing the second service***" << endl;
	//dime::services::test::TestService myTestService2;
	//myServiceManager.addService(&myTestService2);
	//myServiceManager.listAll();
	//cout << "***removing the first service, listing again***" << endl;
	//myServiceManager.removeService(&myTestService);
	//myServiceManager.listAll();
	//cout << "***END OF TESTING SERVICE MANAGER***" << endl;
	cout << "Dime Service Test ends" << endl;
	//dime::services::LoggingService ls;
   
	return 0;
   
}
