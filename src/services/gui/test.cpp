#include "ParaGuiService.h"
#include "ParaGuiApplication.h"
int main () 
{
    dime::ParaGuiService myService;
    dime::Application *myApp = myService.createApplication(640, 480);
    myApp->run();
    
    
}
