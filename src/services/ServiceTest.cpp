/* copyleft and stuff */

#include "Service.h"
#include "AbstractService.h"
#include "test/TestService.h"

#include <iostream.h>


int main()
{

   cout << "Dime Service Test running" << endl;

   /* Test the Test Service */
   dime::services::test::TestService myTestService;
   myTestService.start();
   myTestService.setName("Test Test Service");
   cout << "my name is " << myTestService.getName() << endl;
   myTestService.setDescription("I am the Test for the Test Service");
   cout << myTestService.getDescription() << endl;
   myTestService.stop(0);

   cout << "Dime Service Test ends" << endl;
   
   return 0;
   
}
