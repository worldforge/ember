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
 *      Revision 1.43  2004-07-31 11:53:59  erik
 *      2004-07-31 Erik Hjortsberg <erik@hysteriskt.nu>
 *
 *      /src/components/ogre:
 *      *cleaned up the class layout and structured it, we're not using that many Singletons anymore
 *      *moved to Ogre CVS in order to get a more decent terrainmanager
 *      *added splatting and nice texturing to the terrain
 *      *added classes TerrainShader and DimeTerrainPageSource which works with the new terrainmanager
 *
 *      Revision 1.42  2004/07/21 00:27:30  erik
 *      2004-07-21 Erik Hjortsberg <erik@hysteriskt.nu>
 *
 *      /src/components/ogre:
 *      *removed references to EntityListener, we'll keep the class however as
 *      we'll pobably use it in the future
 *      *put everything under the namespace DimeOgre
 *
 *      Revision 1.41  2004/07/20 22:52:16  erik
 *      2004-07-21 Erik Hjortsberg <erik@hysteriskt.nu>
 *
 *      in src/components/ogre:
 *
 *      MathConverter.h:
 *      *changed scaling
 *
 *      DimeEntity.*:
 *      *fixed problems with containers
 *      *added more meshes (sty, oak)
 *
 *      Avatar.*:
 *      *moved camera code to AvatarCamera
 *      *better connection to Eris::Avatar and avatar creation
 *
 *      MotionManager.cpp:
 *      *better clean up code
 *
 *      InputManager.cpp:
 *      *removed buggy polling code and replaced it with signals in the ServerService class
 *
 *      DimeEntityFactory.*:
 *      *added support for AvatarDimeEntity
 *
 *      AvatarDimeEntity.*:
 *      New class for handling the Avatar entity
 *
 *      AvatarCamera.*:
 *      New class for handling the Avatar camera
 *
 *      DimeOgre.*:
 *      *cleaned up old code
 *      *moved camera stuff into AvatarCamera
 *      *better connection between Services signals and other objects
 *
 *      EntityListener.cpp:
 *      *removed connection to Avatar, this class is obsolete anyway
 *
 *      AvatarController.*:
 *      *added support for AvatarCamera
 *
 *      Revision 1.40  2004/07/18 23:26:24  erik
 *      2004-07-19 Erik Hjortsberg <erik@hysteriskt.nu>
 *      src/components/ogre:
 *
 *      *implemented containers, though it's not completely solid yet. You'll notice that all avatars are carrying coins through our use of a rather big placeholder.
 *      *added the class MotionManager, this takes care of both motion and animation. This means that both fluid motion of all entites and animation for those that have defined skeletons is implemented
 *
 *      Revision 1.39  2004/07/16 21:21:56  erik
 *      2004-07-16 Erik Hjortsberg <erik@hysteriskt.nu>
 *      /src/components/ogre:
 *      *added support for mercator terrain by using a subclass of Ogre's OctreeSceneManager, it includes some preparation in order to access the Ogre headers, I'll add instructions tomorrow
 *      *dime now depends on Mercator
 *
 *      Revision 1.38  2004/07/13 22:02:24  erik
 *      Erik Hjortsberg  <erik@hysteriskt.nu>
 *      src/components/ogre:
 *      *fixed bug with eris sometimes letting the avatar be a regular Entity
 *      *changed to 3d person view, need to apply spinginess to the camera though
 *      *fixed avatar movement, he/she now moves in the right direction, plus snaps to correct positions
 *
 *      Revision 1.37  2004/07/12 04:05:48  erik
 *      2004-07-12 Erik Hjortsberg erik by hysteriskt speck org
 *      * src/components/ogre:
 *      Changed the way input is handled through the use of an Ogre::EventProcessor
 *      By default mouse is not grabbed, press F9 to grab and ungrab it.
 *      The mouse now shows a cursor, movement happens when the mouse goes near the edges of the screen.
 *      Various code cleanups, forward declarations and movement of code from .h-files to .cpp-files.
 *      Addition of preprocessor declarations in MathConverter.h to easily convert between WF and Ogre units.
 *      Revamping of how entities are handled though the new class DimeEntity and changes to EntityListener.
 *      Rudimentary animation of the avatar.
 *      Addition of a debug layer to show triangles, fps etc. Taken from Ogre.
 *      Usage of ogre.cfg for reading configuration values instead of using the console each time.
 *      Some cleanup of the world. The only thing you see now is a ground plane. But it gets populated when connected to a server.
 *      Had some problem with the top cam so I disabled it.
 *
 *      Revision 1.36  2004/01/28 17:26:18  aglanor
 *      2004-01-28 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *              * Fixed ogre component to get less warnings.
 *
 *      Revision 1.35  2003/12/08 21:25:06  aglanor
 *      2003-11-28 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *              * src/components/ogre: fixed rotation and viewing for the Avatar.
 *
 *      Revision 1.34  2003/11/29 01:25:58  aglanor
 *      2003-11-28 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *              * src/components/ogre: added AvatarController.(h|cpp)
 *      	        and restored local avatar movement.
 *
 *      Revision 1.33  2003/11/26 19:03:43  aglanor
 *      2003-11-26 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *              * src/components/ogre: added Avatar.(h|cpp) to store
 *              the whole scenenode tree for the avatar and to control
 *              its movement, looks, etc.
 *
 *      Revision 1.31  2003/11/18 19:26:27  aglanor
 *      2003-11-18 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *              * src/services/server/ServerService.cpp: myWorld is
 *              not created by this service anymore, instead is
 *              retrieved from myAvatar after taking/creating char.
 *              Modified touch op so we can touch any entity.
 *              * src/components/ogre: overall improvement.
 *              Added EntityListener.(h|cpp) to listen to and act
 *              upon Eris entity-related signals.
 *
 *      Revision 1.30  2003/11/12 07:32:35  aglanor
 *      2003-11-12 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *              * src/components/ogre: added PlayerMouseListener,
 *              which brings back mouselook.
 *              Makefile.am: modified to include PlayerMouseListener.
 *              DimeOgre.cpp: instantiates and configures
 *              PlayerMouseListener.
 *
 *      Revision 1.29  2003/11/05 08:10:56  aglanor
 *      2003-11-04 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *              * src/components/ogre:
 *              MediaDeployer.(h|cpp): added methods for adding media.
 *              DimeOgre.cpp: some cleanup.
 *
 *      Revision 1.28  2003/10/30 07:37:41  aglanor
 *      2003-10-30 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *              * src/services/components/ogre:
 *              Makefile.am, DimeOgre.(h|cpp), ConsoleObjectImpl.(h|cpp):
 *              Moved Ogre response to console commands from the app
 *              to its own class, ConsoleObjectImpl.
 *
 *      Revision 1.27  2003/10/29 18:25:25  aglanor
 *      2003-10-29 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *              * src/services/components/ogre/: DimeOgre.(h|cpp):
 *              Added ConsoleObject inheritance, it runs the Quit command.
 *
 *      Revision 1.26  2003/10/25 21:07:40  aglanor
 *      2003-09-28 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *              * src/services/components/ogre/: Makefile.am,
 *              Console.(h|cpp), InputManager.(h|cpp), DimeOgre.cpp:
 *              Added InputManager and Console from Die (thanks to
 *              wolfman8k).
 *              Press ~ or F12 to show the console. The console is not
 *              funtional yet (to be done ASAP).
 *              As keyboard controls are disabled atm, the program
 *              exits after 600 rendered frames.
 *              * configure.ac: added /user/local/lib/Ogre/Plugin_GuiElements.so
 *              to LDFLAGS. This is harcoded and an ugly hack, but I don't know
 *              if Ogre has a way to add Plugin so's with the autotools.
 *
 *      Revision 1.25  2003/10/23 09:54:45  lee
 *      Removed code and includes from DimeServices.h and put them in
 *      DimeServices.cpp
 *
 *      Fixed other classes and Makefile.am to work again
 *
 *      Make sure you autogen.sh and configure again...
 *
 *      Revision 1.24  2003/10/22 08:42:08  lee
 *      Added include of stdlib.h to DimeOgre to stop errors from not declaring
 *      malloc in OgreStdHeaders.h (why it needs it, i don't know)
 *
 *      Revision 1.23  2003/10/21 20:51:42  aglanor
 *      actually restored it
 *
 *      Revision 1.22  2003/10/21 20:48:37  aglanor
 *      Restored SkyBox
 *
 *      Revision 1.21  2003/10/18 23:18:37  sandalle
 *      * src/components/ogre/DimeOgre.cpp:290: Actually comment it out...
 *
 *      Revision 1.20  2003/09/29 06:28:15  aglanor
 *      changed the texture of the plane to look like ground
 *
 *      Revision 1.19  2003/09/07 12:38:51  aglanor
 *      2003-09-07 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *              * ServerService: added touch operation
 *
 *      Revision 1.18  2003/09/05 15:01:18  aglanor
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
 *      	* Cal3DConverter: converts cal3d meshes to ogre meshes,
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
 *      	There is also a little ogre head marking (0,0,0).
 *      	The more detailed the scene, the easiest it is
 *      	to spot strange behaviors.
 *      	And it looks cool :)
 *
 *      Revision 1.8  2003/04/27 23:46:30  aglanor
 *      2003-04-28 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *      	* MathConverter.h: Point, Vector and Quaternion converter
 *      	for world-centric coordinates (Atlas-wfmath like)
 *      	to/from screen-centric coordinates (Ogre like).
 *      	See file for more info.
 *      	Only point conversion currently, more will come later.
 *      	* Cal3DOgreConverter.h/cpp: model converter.
 *      	Just added files, it is not coded yet.
 *      	* Makefile.am: added Cal3D2Ogre binary file.
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

// Headers to stop compile problems from headers
#include <stdlib.h>

//Dime headers
#include "services/DimeServices.h"
#include "services/logging/LoggingService.h"
#include "services/server/ServerService.h"
#include "services/config/ConfigService.h"
#include "services/metaserver/MetaserverService.h"
#include "framework/ConsoleBackend.h"


// ------------------------------
// Include OGRE dime client files
// ------------------------------
//#include "OgreGameView.h"
//#include "MathConverter.h"
//#include "DimeTerrainPageSource.h"
#include "TerrainGenerator.h"

#include "InputManager.h"
#include "Console.h"
#include "ConsoleObjectImpl.h"
#include "Avatar.h"
#include "AvatarController.h"
#include "PlayerMouseListener.h"
#include "EntityListener.h"
#include "DimeEntityFactory.h"
#include "MotionManager.h"
#include "AvatarCamera.h"
#include "DebugListener.h"


#include "DimeTerrainSceneManager.h"

// ------------------------------
// Include dime header files
// ------------------------------

#include "framework/ConsoleBackend.h"


#include "DimeOgre.h"

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

namespace DimeOgre {



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
    
template<> DimeOgre* dime::Singleton<DimeOgre>::ms_Singleton = 0;
//DimeOgre* DimeOgre::_instance = 0;
/*   
DimeOgre & DimeOgre::getSingleton(void)
{
	//fprintf(stderr, "TRACE - ENTITY LISTENER - SINGLETON ENTERING\n");
	if(_instance == 0)
		_instance = new DimeOgre;
	return *_instance;
}
  
*/  
DimeOgre::DimeOgre() :
//mFrameListener(0),
mRoot(0)
{}

    
// These internal methods package up the stages in the startup process
/** Sets up the application - returns false if the user chooses to abandon configuration. */
bool DimeOgre::setup(void)
{
    mRoot = new Ogre::Root();

    setupResources();

    bool carryOn = configure();
    if (!carryOn) return false;

    chooseSceneManager();
    // Create the scene
    createScene();

	mTerrainGenerator = new TerrainGenerator();
	mMotionManager = new MotionManager();
	mMotionManager->setTerrainGenerator(mTerrainGenerator);
	mInputManager = new InputManager();

	// Avatar
	mAvatar = new Avatar(mSceneMgr);
//	EntityListener::getSingleton().setDimeAvatar(mAvatar);
	
	AvatarController* avatarController = new AvatarController(mAvatar, mInputManager, mWindow);


    createViewports();

    // Set default mipmap level (NB some APIs ignore this)
    Ogre::TextureManager::getSingleton().setDefaultNumMipMaps(5);
    
    // Set default animation mode
	Ogre::Animation::setDefaultInterpolationMode(Ogre::Animation::IM_SPLINE);


    createFrameListener();

/*
    dimeEntityFactory = new DimeEntityFactory(mSceneMgr);
    
    dime::DimeServices::getInstance()->getServerService()->getWorld()->registerFactory(dimeEntityFactory);
*/
    return true;

}
/** Configures the application - returns false if the user chooses to abandon configuration. */
bool DimeOgre::configure(void)
{
    // Show the configuration dialog and initialise the system
    // You can skip this and use root.restoreConfig() to load configuration
    // settings if you were sure there are valid ones saved in ogre.cfg
    if(mRoot->restoreConfig())
    {
        // If returned true, user clicked OK so initialise
        // Here we choose to let the system create a default rendering window by passing 'true'
        mWindow = mRoot->initialise(true);
        //mRoot->showDebugOverlay(true);
        return true;
    }
    else
    {
        return false;
    }
}

void DimeOgre::chooseSceneManager(void)
{
    // We first register our own scenemanager
    DimeTerrainSceneManager* sceneManager = new DimeTerrainSceneManager();
    mRoot->setSceneManager(Ogre::ST_EXTERIOR_FAR, sceneManager);
    //And then request it
    mSceneMgr = static_cast<Ogre::TerrainSceneManager*>(mRoot->getSceneManager(Ogre::ST_EXTERIOR_FAR));
    
    DimeTerrainSceneManager* terr = dynamic_cast<DimeTerrainSceneManager*>(mSceneMgr);
    assert(terr);
 //   const Ogre::String terrainTexture = Ogre::String("terrain_texture.jpg");
 //   const Ogre::String terrainDetail = Ogre::String("terrain_detail.jpg");
    
//    mSceneMgr->setDetailTexture(terrainDetail);
//    mSceneMgr->setWorldTexture(terrainTexture);
//    mSceneMgr->setTileSize(64);
//    mSceneMgr->setPageSize(64 * 3);
//    mSceneMgr->setScale(Ogre::Vector3(1,1,1));
 //   mPageSource = new DimeTerrainPageSource(&TerrainGenerator::getSingleton());
 //   mSceneMgr->registerPageSource("DimeTerrain", mPageSource);
//    Ogre::TerrainPageSourceOptionList option;
 //   mSceneMgr->selectPageSource("DimeSource", option);
    
//    const Ogre::String config = Ogre::String("terrain.cfg");
//    mSceneMgr->setWorldGeometry(config);
    
//    setGenerator(&TerrainGenerator::getSingleton());
    //terr->setShowBoxes(true);
    //DimeOgre::sceneMgr = mSceneMgr;
}

void DimeOgre::createViewports(void)
{

    // Create 1st person viewport, entire window
//    Ogre::Viewport* vp = mWindow->addViewport(mAvatar->getCamera());
//    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

//float left=0.0f, float top=0.0f, float width=1.0f, float height=1.0f)
/*
		Ogre::Viewport* rightvp = mWindow->addViewport(mAvatar.getAvatarTopCamera(),1,0.70,0.05,0.25,0.25);
		rightvp->setBackgroundColour(Ogre::ColourValue(0,0,0));
		rightvp->setOverlaysEnabled(false);
*/
	//Ogre::Viewport* leftvp = mWindow->addViewport(mAvatar.getAvatar1pCamera(),9,0.05,0.05,0.25,0.25);
	//leftvp->setBackgroundColour(Ogre::ColourValue(0,0,0));
	//leftvp->setOverlaysEnabled(false);

	/*
	Ogre::Viewport* spyvp = mWindow->addViewport(mOgreHeadCamera,10,0.05,0.70,0.25,0.25);
	spyvp->setBackgroundColour(Ogre::ColourValue(0,0,0));
	spyvp->setOverlaysEnabled(false);
	*/
}

/// Method which will define the source of resources (other than current folder)
void DimeOgre::setupResources(void)
{
    // Load resource paths from config file
    Ogre::ConfigFile cf;
    cf.load("resources.cfg");

    // Go through all settings in the file
    Ogre::ConfigFile::SettingsIterator i = cf.getSettingsIterator();

    Ogre::String typeName, archName;
    while (i.hasMoreElements())
    {
        typeName = i.peekNextKey();
        archName = i.getNext();
        Ogre::ResourceManager::addCommonArchiveEx( archName, typeName );
    }
}



void DimeOgre::createScene(void)
{

  mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
  // Create a light
  Ogre::Light* l = mSceneMgr->createLight("MainLight");
  l->setPosition(WF2OGRE_VECTOR3(150,150,150));

	//set fog, do this before calling TerrainSceneManager::setWorldGeometry 
	Ogre::ColourValue fadeColour(0.93, 0.86, 0.76);
//	Ogre::ColourValue fadeColour(1,1,1);
	mSceneMgr->setFog( Ogre::FOG_LINEAR, fadeColour, .001, 64, 256);

  // create a Skydome
  mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);

        Ogre::Entity *waterEntity;
        Ogre::Plane waterPlane;


        // create a water plane/scene node
        waterPlane.normal = Ogre::Vector3::UNIT_Y; 
        waterPlane.d = 0; 
        Ogre::MeshManager::getSingleton().createPlane(
            "WaterPlane",
            waterPlane,
            WF2OGRE(1400), WF2OGRE(1400),
            20, 20,
            true, 1, 
            100, 100,
            Ogre::Vector3::UNIT_Z
        );

        waterEntity = mSceneMgr->createEntity("water", "WaterPlane"); 
        waterEntity->setMaterialName("Examples/TextureEffect4"); 
		
		
        Ogre::SceneNode *waterNode = 
            mSceneMgr->getRootSceneNode()->createChildSceneNode("WaterNode"); 
        waterNode->attachObject(waterEntity); 
        waterNode->translate(WF2OGRE(500), 0, WF2OGRE(500));
        
	 
	
}

void DimeOgre::connectWorldSignals(Eris::World* world)
{
    world->registerFactory(mDimeEntityFactory, 10);
	
	EntityListener::getSingleton().connectWorldSignals();
}

void DimeOgre::connectedToServer(Eris::Connection* connection) 
{
	mDimeEntityFactory = new DimeEntityFactory(mSceneMgr,mTerrainGenerator, connection->getTypeService());
	mDimeEntityFactory->CreatedAvatarEntity.connect(SigC::slot(*mAvatar, &Avatar::createdAvatarDimeEntity));
	
}

void DimeOgre::createFrameListener(void)
{

	fprintf(stderr, "TRACE - CREATING FRAME LISTENERS\n");
	//PlayerFrameListener* playerFrameListener = new PlayerFrameListener(mWindow, mCamera, this, false, false);
	//mRoot->addFrameListener(playerFrameListener);
	fprintf(stderr, "TRACE - CREATING INPUT MANAGER\n");
	
	mInputManager->addKeyListener(&(Console::getSingleton()));
	fprintf(stderr, "TRACE - INPUT MANAGER ADDED - NOW GONNA ADD CONSOLE FRAME LISTENER\n");
	mRoot->addFrameListener(&(Console::getSingleton()));
	mRoot->addFrameListener(mMotionManager);
	mRoot->addFrameListener(&(DebugListener::getSingleton()));
	//AvatarCamera* camera = new AvatarCamera(mAvatar.getAvatar1pCamera(), &mAvatar, mSceneMgr);
	//mRoot->addFrameListener(camera);
	ConsoleObjectImpl::getSingleton();
	
//	Ogre::OverlayManager::getSingleton().addMouseMotionListener(&(DebugListener::getSingleton()));
	
//	mInputManager->addMouseListener(&(AvatarController::getSingleton()));
	//PlayerMouseListener::getSingleton().setCamera(mAvatar.getAvatar1pCamera());
	//PlayerMouseListener::getSingleton().setAvatar(&mAvatar);
	//EntityListener::getSingleton().setSceneManager(mSceneMgr);
//	AvatarController::getSingleton().setSceneManager(mSceneMgr);
//	AvatarController::getSingleton().setAvatar(mAvatar);
	
	mSceneMgr->setPrimaryCamera(mAvatar->getAvatarCamera()->getCamera());

	//Console::getSingleton().write("Welcome to Dime / Ember!\n");
	fprintf(stderr, "TRACE - CREATED FRAME LISTENERS\n");
}

Avatar* DimeOgre::getAvatar() {
	return mAvatar;
}


Ogre::TerrainSceneManager* DimeOgre::getSceneManager()
{
	return mSceneMgr;
}

TerrainGenerator* DimeOgre::getTerrainGenerator()
{
	return mTerrainGenerator;
}

MotionManager* DimeOgre::getMotionManager()
{
	return mMotionManager;
}

Ogre::Root* DimeOgre::getOgreRoot()
{
	assert(mRoot);
	return mRoot;
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
	dime::DimeServices::getInstance()->getServerService()->GotConnection.connect(SigC::slot(*this, &DimeOgre::connectedToServer));
	dime::DimeServices::getInstance()->getServerService()->GotWorld.connect(SigC::slot(*this, &DimeOgre::connectWorldSignals));
	
	dime::DimeServices::getInstance()->getServerService()->start();
#endif

}

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
    DimeOgre::DimeOgre* app = new DimeOgre::DimeOgre();

	// Initialize all dime services needed for this application
	app->initializeDimeServices();

    try {
        app->go();
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


