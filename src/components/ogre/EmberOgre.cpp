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
 *      Revision 1.18  2003-09-05 15:01:18  aglanor
 *      2003-09-04 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *              * configure.ac: pumped up atlas version req to 0.5
 *      	* ServerService: added support for character creation
 *              * DimeOgre: bound key "5" to character creation
 *
 *      Revision 1.17  2003/08/28 17:12:08  aglanor
 *      fixed typo
 *
 *      Revision 1.16  2003/07/12 11:39:59  alriddoch
 *      <aglanor from alistair's machine>
 *      - fixed pig movement, and on-game chat is now shown on screen
 *
 *      Revision 1.15  2003/07/03 20:12:25  aglanor
 *      added sample animation to demo mesh
 *
 *      Revision 1.14  2003/06/23 01:20:34  aglanor
 *      2003-06-23 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *              * Cal3DConverter: converts Cal3D materials to Ogre
 *              materials, and assigns material and texture mapping
 *              to the Ogre mesh accordingly.
 *
 *      	Check screenshot of fully textured pig within dime here:
 *      	http://purple.worldforge.org/~aglanor/screenshots/dime_20030623.png
 *
 *      Revision 1.13  2003/05/07 23:28:43  aglanor
 *      Added a little more variety to the media. Now there's different media
 *      for settlers (malebuilder), pig (skeleton), merchant (robot) and
 *      everything else.
 *      The camera is set to the Avatar position/orientation when entering the game world.
 *
 *      Revision 1.12  2003/05/06 22:16:48  aglanor
 *      added directory and filenames management to the cal3d converter.
 *
 *      Revision 1.11  2003/05/05 01:41:06  aglanor
 *      2003-05-05 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *              * Cal3DConverter: converts cal3d meshes to ogre meshes,
 *      	without material, textures or animations yet. Does the
 *      	appropiate Atlas->Ogre rotation on the mesh.
 *
 *      Revision 1.10  2003/04/28 22:42:06  aglanor
 *      Added new viewport for a mini-map and quaternion conversion.
 *      Eris entities are now displayed with orientation.
 *
 *      Revision 1.9  2003/04/28 00:55:06  aglanor
 *      2003-04-28 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *      	* DimeOgre.h/cpp: added a Water Plane at height 0.
 *      		There is also a little ogre head marking (0,0,0).
 *      			The more detailed the scene, the easiest it is
 *      				to spot strange behaviors.
 *      					And it looks cool :)
 *
 *      Revision 1.8  2003/04/27 23:46:30  aglanor
 *      2003-04-28 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *      	* MathConverter.h: Point, Vector and Quaternion converter
 *      		for world-centric coordinates (Atlas-wfmath like)
 *      			to/from screen-centric coordinates (Ogre like).
 *      				See file for more info.
 *      					Only point conversion currently, more will come later.
 *      						* Cal3DOgreConverter.h/cpp: model converter.
 *      							Just added files, it is not coded yet.
 *      								* Makefile.am: added Cal3D2Ogre binary file.
 *
 *      Revision 1.7  2003/04/26 14:57:26  aglanor
 *      quick coordinates hack to fix position of entities when entering the world.
 *      I'm in the process of writing a proper Atlas<-->Ogre MathConverter.
 *
 *      Revision 1.6  2003/04/25 00:48:14  aglanor
 *      basic media selection:
 *          - if Eris Entity is a settler, it will look like a robot
 *          - else, will look like the good old squirrel
 *
 *      heh, we're short of OGRE models around here ;)
 *
 *      Revision 1.5  2003/04/24 23:21:09  aglanor
 *      DimeOgre app is again linked back from the FrameListener, so entities sighted
 *      are created in the Ogre SceneManager (as squirrels, of course).
 *      I've also made the sample entity the ogrehead.mesh, you need to have it
 *      on your media dir too.
 *
 *      Revision 1.4  2003/04/24 21:21:07  aglanor
 *      more namespace removal
 *
 *      Revision 1.3  2003/04/24 20:27:26  aglanor
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
#include "MathConverter.h"


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
#include <Eris/TypeInfo.h>
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
  mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
  // Create a light
  Ogre::Light* l = mSceneMgr->createLight("MainLight");
  l->setPosition(150,150,150);

  // create a Skydome
  mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);

  // set the world geometry
  mSceneMgr->setWorldGeometry("terrain.cfg");

//#if 0


	// A little Ogre head to mark the center of the scene
	// ----------------------------------------

	//create the entity
	mOgreHead = mSceneMgr->createEntity("test", "robot.mesh");

	// create the node
	mOgreHeadNode = dynamic_cast<Ogre::SceneNode*>(mSceneMgr->getRootSceneNode()->createChild());
	mOgreHeadNode->setPosition(0,0,0);
	mOgreHeadNode->setScale(0.001,0.001,0.001);

	// attach the node to the entity
	mOgreHeadNode->attachObject(mOgreHead);

	// end little Ogre Head
	// ----------------------------------------



	// A Water Plane for the fancyness of it ;)
	// ----------------------------------------
        Ogre::Entity *waterEntity;
        Ogre::Plane waterPlane;

        // create a water plane/scene node
        waterPlane.normal = Ogre::Vector3::UNIT_Y;	// normal points up
        waterPlane.d = 0;				// sea level is at 0 ;)
        Ogre::MeshManager::getSingleton().createPlane(
            "WaterPlane",
            waterPlane,
            2800, 2800,
            20, 20,
            true, 1,
            10, 10,
            Ogre::Vector3::UNIT_Z
        );

        waterEntity = mSceneMgr->createEntity("water", "WaterPlane");
        waterEntity->setMaterialName("Examples/TextureEffect4");

        Ogre::SceneNode *waterNode = static_cast<Ogre::SceneNode*>(
            mSceneMgr->getRootSceneNode()->createChild("WaterNode"));
        waterNode->attachObject(waterEntity);
        waterNode->translate(1000, 0, 1000);

	// end Water Plane
	// ----------------------------------------





	// a hack from the OGRE GUI sample
	/*
	Ogre::Overlay* o = (Ogre::Overlay*)Ogre::OverlayManager::getSingleton().getByName("SS/Setup/HostScreen/Overlay");
	Ogre::ActionTarget* at =
		static_cast<Ogre::BorderButtonGuiElement*>(Ogre::GuiManager::getSingleton().getGuiElement("SS/Setup/HostScreen/Join"));


	at->addActionListener(this);
	at = static_cast<Ogre::BorderButtonGuiElement*>(Ogre::GuiManager::getSingleton().getGuiElement("SS/Setup/HostScreen/Exit"));
	at->addActionListener(this);


	Ogre::ListChanger* list = static_cast<Ogre::ListGuiElement*>(Ogre::GuiManager::getSingleton().getGuiElement("SS/Setup/HostScreen/AvailableGamesList"));

	list->addListItem(new Ogre::StringResource("test1"));
	list->addListItem(new Ogre::StringResource("test2"));

	(Ogre::GuiManager::getSingleton().getGuiElement("Core/CurrFps"))->addMouseListener(this);

	Ogre::GuiContainer* pCursorGui = Ogre::OverlayManager::getSingleton().getCursorGui();
	pCursorGui->setMaterialName("Cursor/default");
	*/
}

void DimeOgre::createFrameListener(void)
{

	fprintf(stderr, "TRACE - CREATING FRAME LISTENER\n");
	PlayerFrameListener* playerFrameListener = new PlayerFrameListener(mWindow, mCamera, this, false, false);
	mRoot->addFrameListener(playerFrameListener);
	fprintf(stderr, "TRACE - CREATED FRAME LISTENER\n");

}

void DimeOgre::createCamera(void)
{
  // Create the camera
  mCamera = mSceneMgr->createCamera("PlayerCam");

  // Position it at 500 in Z direction
  mCamera->setPosition(Ogre::Vector3(10,10,10));

  // Look back along -Z
  mCamera->lookAt(Ogre::Vector3(0,10,0));
  mCamera->setNearClipDistance(1);
  // do not clip at far distance
  // so we can see the skydome
  //mCamera->setFarClipDistance( 384 );

  // Create the camera
  mPlayerMapCamera = mSceneMgr->createCamera("PlayerMapCamera");

  // Position it at 500 in Z direction
  mPlayerMapCamera->setPosition(Ogre::Vector3(0,50,0));
  // Look back along -Z
  mPlayerMapCamera->lookAt(Ogre::Vector3(0,0,0));
  //mPlayerMapCamera->setDirection(Ogre::Vector3(0,-1,0));
  mPlayerMapCamera->setNearClipDistance(1);


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


/*
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
*/

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
	// create the ogre node and the
	// TODO: use Eris entity hierarchy for the node hierarchy !!
	Ogre::SceneNode* ogreNode = dynamic_cast<Ogre::SceneNode*>(mSceneMgr->getRootSceneNode()->createChild());
	Ogre::Entity* ogreEntity;

	// create the ogre entity
        if(!strcmp(e->getType()->getName().c_str(),"settler")) {	// 0 if strings are equal
		fprintf(stderr, "TRACE - FOUND A SETTLER - MALEBUILDER MESH\n");
		ogreEntity = mSceneMgr->createEntity(e->getID(), "malebuilder.mesh");
		//ogreNode->setScale(1,1,1);
		ogreNode->setScale(0.01,0.01,0.01);
	}
	else if(!strcmp(e->getType()->getName().c_str(),"merchant"))
	{
		fprintf(stderr, "TRACE - FOUND A MERCHANT - ROBOT MESH\n");
		ogreEntity = mSceneMgr->createEntity(e->getID(), "robot.mesh");
		//ogreNode->setScale(1,1,1);
		ogreNode->setScale(0.025,0.025,0.025);
	}
	else if(!strcmp(e->getType()->getName().c_str(),"pig"))
	{
		fprintf(stderr, "TRACE - FOUND A PIG - PIG MESH\n");
		ogreEntity = mSceneMgr->createEntity(e->getID(), "pig.mesh");
		ogreNode->setScale(1,1,1);
	}
	else
	{
		// TODO: razor should be a coin
		fprintf(stderr, "TRACE - FOUND ANYTHING ELSE - RAZOR MESH\n");
		ogreEntity = mSceneMgr->createEntity(e->getID(), "razor.mesh");
		//ogreNode->setScale(1,1,1);
		ogreNode->setScale(0.001,0.001,0.001);
	}

	// set the Ogre node position and orientation based on Atlas data
	ogreNode->setPosition(Atlas2Ogre(e->getPosition()));
	std::cout << "Entity placed at (" << e->getPosition().x() << "," << e->getPosition().y() << "," << e->getPosition().x() << ")" << std::endl;
	//WFMath::Quaternion wq = e->getOrientation();
	//WFMath::Vector<3> wv = wq.vector();
	//Ogre::Quaternion oq = Ogre::Quaternion(Atlas2Ogre(e-getOrientation()));
	ogreNode->setOrientation(Atlas2Ogre(e->getOrientation()));

	// old code, just for the record
	/** WFMath::Point<3> position = e->getPosition();
	ogreNode->setPosition(position.x(),position.z(),-position.y());*/

	// scale HACK. This is very hacky. Fix this.
        if(!strcmp(e->getType()->getName().c_str(),"settler")) {	// 0 if strings are equal
				// robots are bigger :P
	}
	else {

	}

	// attach the node to the entity
	ogreNode->attachObject(ogreEntity);

	fprintf(stderr, "TRACE - ENTITY ADDED TO THE GAMEVIEW\n");

    // Whenever a new entity is created, make sure to connect to those signals too

    // Xmp's Notes: hmm need to work out how to connect these
    e->AddedMember.connect( SigC::slot( *this, &DimeOgre::addedMember ) );

    e->RemovedMember.connect( SigC::slot( *this, &DimeOgre::removedMember ) );

    e->Recontainered.connect( SigC::slot( *this, &DimeOgre::recontainered ) );

    e->Changed.connect( SigC::bind( SigC::slot( *this, &DimeOgre::changed ), e ) );

    e->Moved.connect( SigC::bind( SigC::slot( *this, &DimeOgre::moved ), e ) );

    e->Say.connect( SigC::bind( SigC::slot( *this, &DimeOgre::say ), e ) );
}


void DimeOgre::entityDelete( Eris::Entity *e )
{
	fprintf(stderr, "TRACE - ENTITY DELETED\n");
}

void DimeOgre::entered( Eris::Entity *e )
{
	fprintf(stderr, "TRACE - PLAYER ENTERED THE WORLD\n");
	std::cout << "THE PLAYER IS ENTITY " <<  e->getID() << std::endl;
	// Set the Player camera accordingly
	// TODO: do this when the avatar moves too
	mCamera->setPosition(Atlas2Ogre(e->getPosition()));
	Ogre::Vector3 height(0,1.75,0);
	mCamera->move(height);
	mCamera->setOrientation(Atlas2Ogre(e->getOrientation()));
        mWindow->setDebugText("Avatar entered the world");

}

void DimeOgre::appearance( Eris::Entity *e )
{
	fprintf(stderr, "TRACE - ENTITY APPEARS\n");
}

void DimeOgre::disappearance( Eris::Entity *e )
{
	fprintf(stderr, "TRACE - ENTITY DISAPPPEARS\n");
}

/* Eris::Entity signals */

void DimeOgre::recontainered( Eris::Entity *e, Eris::Entity *c )
{
	fprintf(stderr, "TRACE - ENTITY RECONTAINERED\n");
}

void DimeOgre::changed( const Eris::StringSet &s, Eris::Entity *e  )
{
	std::cout << "TRACE - ENTITY CHANGED - PROCESSING CHANGES FOR "  << e->getID() << std::endl;
	/*
        TODO: ENTITY CHANGES HERE
	*/
}

void DimeOgre::moved( const WFMath::Point< 3 > &p, Eris::Entity *e )
{
	fprintf(stderr, "TRACE - ENTITY MOVED\n");

	Ogre::Entity* o = mSceneMgr->getEntity(e->getID());
	o->getParentNode()->setPosition(Atlas2Ogre(e->getPosition()));
	o->getParentNode()->setOrientation(Atlas2Ogre(e->getOrientation()));
}

void DimeOgre::say( const std::string &s, Eris::Entity *e )
{
        std::string message = "<";
        message.append(e->getName());
        message.append("> ");
        message.append(s);
	std::cout << "TRACE - ENTITY SAYS: [" << message << "]\n" << std::endl;
        mWindow->setDebugText(message);

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

void DimeOgre::updateAnimations(Ogre::Real time) {

	Ogre::AnimationState* animState;
	animState=mOgreHead->getAnimationState("Walk");
	animState->setEnabled(true);
	animState->addTime(time);

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
    } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == PLATFORM_WIN32
        MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        fprintf(stderr, "An exception has occured: %s\n",
                e.getFullDescription().c_str());
#endif
    }


    return 0;
}
