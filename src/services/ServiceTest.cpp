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

#include <iostream>


int main()
{

	std::cout << "Dime Service Test running" << std::endl;

	std::cout << "***TESTING TEST SERVICE***" << std::endl;
	dime::TestService myTestService;
	myTestService.start();
	std::cout << "My name is: " << myTestService.getName() << std::endl;
	std::cout << myTestService.getDescription() << std::endl;
	myTestService.stop(0);
	std::cout << "***END TEST OF TEST SERVICE***" << std::endl;
	
	std::cout << "***TESTING CONFIG SERVICE***" << std::endl;
	dime::ConfigService myConfigService;
	myConfigService.start();
	std::cout << "My name is: " << myConfigService.getName() << std::endl;
	std::cout << myTestService.getDescription() << std::endl;
	myConfigService.stop(0);
	std::cout << "***END TEST OF CONFIG SERVICE***" << std::endl;	
	
	std::cout << "***TESTING METASERVER SERVICE***" << std::endl;
	dime::MetaserverService myMsService;
	myMsService.start();
	Eris::PollDefault::poll();
	while(true)
	{
//		myMsService.poll();
	}
	std::cout << "My name is: " << myMsService.getName() << std::endl;
	std::cout << myMsService.getDescription() << std::endl;
	myMsService.stop(0);
	//delete myMsService;
	std::cout << "***END TEST OF METASERVER SERVICE***" << std::endl;		
	
	//std::cout << "***TESTING SERVICE MANAGER***" << std::endl;
	//std::cout << "***adding and listing one service***" << std::endl;
	//dime::services::ServiceManager myServiceManager;
	//myServiceManager.addService(&myTestService);
	//myServiceManager.listAll();
	//std::cout << "***adding and listing the second service***" << std::endl;
	//dime::services::test::TestService myTestService2;
	//myServiceManager.addService(&myTestService2);
	//myServiceManager.listAll();
	//std::cout << "***removing the first service, listing again***" << std::endl;
	//myServiceManager.removeService(&myTestService);
	//myServiceManager.listAll();
	//std::cout << "***END OF TESTING SERVICE MANAGER***" << std::endl;
	std::cout << "Dime Service Test ends" << std::endl;
	//dime::services::LoggingService ls;
   
	return 0;
   
}
