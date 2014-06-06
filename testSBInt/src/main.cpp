#include <Ogre.h>
#include "App.h"



int main(int argc, char **argv)
{
	App application;

    try {

    	application.start();

	} catch(Ogre::Exception& e) {
		std::cerr << "An exception has occured: " << e.getFullDescription().c_str() << std::endl;
    }

    return 0;
}