/*
-----------------------------------------------------------------------------
OgreApp.cpp by Miguel Guzman Miranda (Aglanor)
Based on OGRE sample applications:
    OGRE (Object-oriented Graphics Rendering Engine)
For the latest info, see http://ogre.sourceforge.net
Based on the dime main application by the dime team

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.

 *  Change History (most recent first):
 *
 *      $Log$
 *      Revision 1.3  2003-04-24 20:27:26  aglanor
 *      bughunting
 *
 *      Revision 1.2  2003/04/24 20:02:08  aglanor
 *      Makefile and include changes after the renaming
 *
 *      Revision 1.1  2003/04/24 19:42:09  aglanor
 *      2003-04-24 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *              * components/ogre: big cleanup. Files renamed.
 *
 *      Revision 1.16  2003/04/24 19:20:41  aglanor
 *      Cleanup. All FrameListeners are gone.
 *
 *      Revision 1.15  2003/04/23 21:47:33  aglanor
 *      Added a new FrameListener capable of switching between modes.
 *
 *      Revision 1.14  2003/04/22 22:35:33  nikal
 *      changes for aglanor
 *
 *      Revision 1.13  2003/04/19 21:26:14  nikal
 *      Some changes to get Ogre app to compile with the latest version of OGRE
 *
 *      Revision 1.12  2003/01/06 03:48:20  aglanor
 *      2003-01-06 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *      	* OgreApp.cpp/h: dual mode for the GUI: press 'H' to display
 *      	the GUI and use the mouse to click on it.
 *      	Press 'H' again to hide it and return to fullscreen mouseview.
 *
 *      Revision 1.11  2003/01/05 22:13:46  aglanor
 *      2003-01-05 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *      	* OgreApp.cpp/h: added basic GUI elements.
 *      	Now OgreApp inherits from ogre::ActionListener
 *      	and ogre::MouseListener
 *
 *      Revision 1.10  2002/12/10 00:02:45  aglanor
 *      2002-12-07 Miguel Guzman (Aglanor) <aglanor@telefonica.net>
 *
 *      	* OgreApp.cpp: meshes are placed into the OGRE scene when an entity is created.
 *      	Currently they're all squirrels of doom.
 *
 *      	* OgreApp.cpp/h: removed "using namespace ogre". OGRE should not be
 *      	more relevant here than dime or eris. Now it uses the default namesp
 *

-----------------------------------------------------------------------------
*/


// ------------------------------
// Include OGRE dime client files
// ------------------------------
#include "DimeOgre.h"
#include "PlayerFrameListener.h"
#include "OgreGameView.h"


// ------------------------------
// Include dime header files
// ------------------------------
#include "services/DimeServices.h"
#include "framework/ConsoleBackend.h"

// ------------------------------
// Include Eris header files
// ------------------------------
#if defined( _MSC_VER ) && ( _MSC_VER < 1300 )
// GNDN: MSVC < version 7 is broken
#else
#include <Eris/PollDefault.h>
#include <Eris/Log.h>
#endif

// ------------------------------
// Include WFmath header files
// ------------------------------
#include <wfmath/point.h>
#include <wfmath/vector.h>
#include <wfmath/axisbox.h>
#include <wfmath/quaternion.h>


// ----------------------------------------------------------------------------
// Include the main OGRE header files
// Ogre.h just expands to including lots of individual OGRE header files
// ----------------------------------------------------------------------------

// The DEBUG stuff is a workaround for compile errors this causes in OGRE
#ifdef DEBUG
#define DEBUGTEMP
#undef DEBUG
#endif

#include <Ogre.h>

#ifdef DEBUGTEMP
#undef DEBUGTEMP
#define DEBUG
#endif

// ----------------------------------------------------------------------------
// Include the OGRE example framework
// This includes the classes defined to make getting an OGRE application running
// a lot easier. It automatically sets up all the main objects and allows you to
// just override the bits you want to instead of writing it all from scratch.
// ----------------------------------------------------------------------------



// TODO: move CerrLogObserver to its own class (under Logging service, or under Framework)
  class CerrLogObserver: public dime::LoggingService::Observer
    {
    public:
        CerrLogObserver()
        {
        }

        virtual void onNewMessage(const std::string & message, const std::string & file, const int & line,
                                  const dime::LoggingService::MessageImportance & importance, const time_t & timeStamp)
        {
            tm * ctm = localtime(&timeStamp); //currentLocalTime was too long, sorry
		
	    std::cerr.fill('0');
            std::cerr << "[";
	    std::cerr.width(2);
	    std::cerr << (ctm->tm_year/*+1900*/)%100 << "-";
	    std::cerr.width(2);
	    std::cerr << ctm->tm_mon+1 << "-";
	    std::cerr.width(2);
	    std::cerr << ctm->tm_mday << " ";
	    std::cerr.width(2);
	    std::cerr << ctm->tm_hour << ":";
	    std::cerr.width(2);
	    std::cerr <<  ctm->tm_min << ":";
	    std::cerr.width(2);
	    std::cerr << ctm->tm_sec << "] ";
	    std::cerr  << "[File: " << file << ", Line #:" <<  line << "] (";

            if(importance == dime::LoggingService::CRITICAL)
                {
                    std::cerr << "CRITICAL";
                }
            else  if(importance == dime::LoggingService::FAILURE)
                {
                    std::cerr << "FAILURE";
                }
            else if(importance == dime::LoggingService::WARNING)
                {
                    std::cerr << "WARNING";
                }
            else if(importance == dime::LoggingService::INFO)
                {
                    std::cerr << "INFO";
                }
	    else
                {
                    std::cerr << "VERBOSE";
                }
            std::cerr << ") " <<message << std::endl;
        }

    private:

    };

void DimeOgre::createScene(void)
{
  mSceneMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));
  // Create a light
  Light* l = mSceneMgr->createLight("MainLight");
  l->setPosition(20,80,50);

  // create a Skydome
  mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);

  // set the world geometry
  mSceneMgr->setWorldGeometry("terrain.cfg");

//#if 0

  //create the entity
  mShip = mSceneMgr->createEntity("blackdog", "squirel_of_doom.mesh");

  // create the node
  mShipNode = dynamic_cast<SceneNode*>(mSceneMgr->getRootSceneNode()->createChild());
  mShipNode->setPosition(10,25,128);
  mShipNode->setScale(0.01,0.01,0.01);

  // attach the node to the entity
  mShipNode->attachObject(mShip);
//#endif

	// a hack from the OGRE GUI sample
	Overlay* o = (Overlay*)OverlayManager::getSingleton().getByName("SS/Setup/HostScreen/Overlay");
	ActionTarget* at = static_cast<BorderButtonGuiElement*>(GuiManager::getSingleton().getGuiElement("SS/Setup/HostScreen/Join"));


	at->addActionListener(this);
	at = static_cast<BorderButtonGuiElement*>(GuiManager::getSingleton().getGuiElement("SS/Setup/HostScreen/Exit"));
	at->addActionListener(this);


	ListChanger* list = static_cast<ListGuiElement*>(GuiManager::getSingleton().getGuiElement("SS/Setup/HostScreen/AvailableGamesList"));

	list->addListItem(new StringResource("test1"));
	list->addListItem(new StringResource("test2"));

	(GuiManager::getSingleton().getGuiElement("Core/CurrFps"))->addMouseListener(this);

	GuiContainer* pCursorGui = OverlayManager::getSingleton().getCursorGui();
	pCursorGui->setMaterialName("Cursor/default");
}

void DimeOgre::createFrameListener(void)
{

	fprintf(stderr, "TRACE - CREATING FRAME LISTENER\n");
	PlayerFrameListener* playerFrameListener = new PlayerFrameListener(mWindow, mCamera, false, false);
	mRoot->addFrameListener(playerFrameListener);
	fprintf(stderr, "TRACE - CREATED FRAME LISTENER\n");


	/*
	MouseFrameListener* mouseFrameListener = new MouseFrameListener(mWindow, mCamera, false);
	mRoot->addFrameListener(mouseFrameListener);
	*/

	/*
	mFrameListener= new TerrainListener(mWindow, mCamera);
	mRoot->addFrameListener(mFrameListener);
	CameraFrameListener* cameraFrameListener = new CameraFrameListener(mWindow, mCamera);
	mRoot->addFrameListener(cameraFrameListener);
	DimeFrameListener* dimeFrameListener = new DimeFrameListener(mWindow, mCamera, this);
	mRoot->addFrameListener(dimeFrameListener);
	*/

#if 0
    CameraRotator* cameraRotator = new CameraRotator(mWindow, mCamera, mShipNode, Vector3(0, 0, 100));
    mRoot->addFrameListener(cameraRotator);
#endif
}

void DimeOgre::createCamera(void)
{
  // Create the camera
  mCamera = mSceneMgr->createCamera("PlayerCam");

  // Position it at 500 in Z direction
  mCamera->setPosition(Vector3(128,25,75));

  // Look back along -Z
  mCamera->lookAt(Vector3(0,0,-300));
  mCamera->setNearClipDistance( 1 );
  // do not clip at far distance
  // so we can see the skydome
  //mCamera->setFarClipDistance( 384 );

}

void DimeOgre::initializeDimeServices(void)
{
	// Initialize dime services

	// Initialize the Logging service and an error observer
	dime::LoggingService *logging = dime::DimeServices::getInstance()->getLoggingService();
	CerrLogObserver* obs = new CerrLogObserver();
	obs->setFilter(dime::LoggingService::VERBOSE);
	logging->addObserver(obs);


	// Initialize the Configuration Service
	dime::DimeServices::getInstance()->getConfigService()->start();

	// Initialize the Sound Service
#ifndef WIN32
	// Test that /dev/dsp is availible
	FILE *temp = fopen("/dev/dsp","w");
	if (temp) {
	  fclose(temp);
#endif
	// Initialize the SoundService
	dime::DimeServices::getInstance()->getSoundService()->start();
#ifndef WIN32
	}
#endif

	// Initialize and start the Metaserver Service.
#if defined( _MSC_VER ) && ( _MSC_VER < 1300 )
	// GNDN: MSVC < version 7 is broken
#else
	// Set Eris Logging Level
	Eris::setLogLevel(Eris::LOG_DEBUG);

	dime::DimeServices::getInstance()->getMetaserverService()->start();

	// Initialize the Server Service
	dime::DimeServices::getInstance()->getServerService()->start();
#endif



}


void DimeOgre::connectWorldSignals(void) {

    /* Find out where the Eris world instance resides... */
    Eris::World *w = dime::DimeServices::getInstance()->getServerService()->getWorld();

    /* Connect to the relevant World signals */
    w->EntityCreate.connect( SigC::slot( *this, &DimeOgre::entityCreate ) );

    w->EntityDelete.connect( SigC::slot( *this, &DimeOgre::entityDelete ) );

    w->Entered.connect( SigC::slot( *this, &DimeOgre::entered ) );

    w->Appearance.connect( SigC::slot( *this, &DimeOgre::appearance ) );

    w->Disappearance.connect( SigC::slot( *this, &DimeOgre::disappearance ) );

}

/* Eris::World entity signals */

void DimeOgre::entityCreate( Eris::Entity *e )
{

	// create the ogre entity
	Entity* ogreEntity = mSceneMgr->createEntity(e->getID(), "squirel_of_doom.mesh");

	// create the ogre node
	// TODO: use Eris entity hierarchy for the node hierarchy !!
	SceneNode* ogreNode = dynamic_cast<SceneNode*>(mSceneMgr->getRootSceneNode()->createChild());

	// set the node position based on eris entity position
	WFMath::Point<3> position = e->getPosition();
	ogreNode->setPosition(position.x(),position.y(),position.z());
	ogreNode->setScale(0.05,0.05,0.05);

	// attach the node to the entity
	ogreNode->attachObject(ogreEntity);

	fprintf(stderr, "TRACE - ENTITY ADDED TO THE GAMEVIEW\n");

    /* Whenever a new entity is created, make sure to connect to those signals
       too */

    // Xmp's Notes: hmm need to work out how to connect these
    e->AddedMember.connect( SigC::slot( *this, &DimeOgre::addedMember ) );

    e->RemovedMember.connect( SigC::slot( *this, &DimeOgre::removedMember ) );

    e->Recontainered.connect( SigC::slot( *this, &DimeOgre::recontainered ) );

    e->Changed.connect( SigC::bind( SigC::slot( *this, &DimeOgre::changed ), e ) );

    e->Moved.connect( SigC::bind( SigC::slot( *this, &DimeOgre::moved ), e ) );

    e->Say.connect( SigC::bind( SigC::slot( *this, &DimeOgre::say ), e ) );
}


void DimeOgre::entityDelete( Eris::Entity *e )
{}

void DimeOgre::entered( Eris::Entity *e )
{}

void DimeOgre::appearance( Eris::Entity *e )
{
	fprintf(stderr, "TRACE - APPEARANCE\n");
}

void DimeOgre::disappearance( Eris::Entity *e )
{}

/* Eris::Entity signals */

void DimeOgre::recontainered( Eris::Entity *e, Eris::Entity *c )
{}

void DimeOgre::changed( const Eris::StringSet &s, Eris::Entity *e  )
{}

void DimeOgre::moved( const WFMath::Point< 3 > &p, Eris::Entity *e )
{}

void DimeOgre::say( const std::string &s, Eris::Entity *e )
{
	// TODO: fix this one
	/*
    dime::LoggingService::getInstance()->slog(__FILE__, __LINE__, dime::LoggingService::VERBOSE) << e->getName() << " says: "<< s<< ENDM;
	*/
}

void DimeOgre::addedMember(Eris::Entity *e)
{}

void DimeOgre::removedMember(Eris::Entity *e)
{}

void DimeOgre::runCommand(const std::string &command, const std::string &args)
{
}




// ----------------------------------------------------------------------------
// Main function, just boots the application object
// ----------------------------------------------------------------------------
#if OGRE_PLATFORM == PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"


INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
int main(int argc, char **argv)
#endif
{
    // Create application object
    DimeOgre app;

	// Initialize all dime services needed for this application
	app.initializeDimeServices();

    try {
        app.go();
    } catch( Exception& e ) {
#if OGRE_PLATFORM == PLATFORM_WIN32
        MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        fprintf(stderr, "An exception has occured: %s\n",
                e.getFullDescription().c_str());
#endif
    }


    return 0;
}
