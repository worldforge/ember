/*
-----------------------------------------------------------------------------
OgreApp.cpp by Miguel Guzman Miranda (Aglanor)
Based on OGRE sample applications:
    OGRE (Object-oriented Graphics Rendering Engine)
For the latest info, see http://ogre.sourceforge.net
Based on the Ember main application by the Ember team

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
 *      Revision 1.75  2005-03-04 01:14:05  erik
 *      2005-03-04  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* Added more functionality and bug fixes to JesusEdit.
 *      	* It's now possible to create new buildings. The interface and the classes are a bit rough though. There's also no way to save the blueprints.
 *
 *      Revision 1.74  2005/02/27 21:08:21  erik
 *      2005-02-27  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* Moved Ember specific stuff of Carpenter into Jesus.
 *      	* Added Adapters to Carpenter.
 *      	* Added a first prototype house. Need to change the way the floors and walls are attached to allow for rounded corners etc.
 *
 *      Revision 1.73  2005/02/22 23:51:32  erik
 *      2005-02-23  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* Change name of main exec to "ember"
 *      	* Initial checking of Carpenter library which would allow dynamic constructions of buildings. This is very much in a protoyping stage right now. Later on the library will be separated from Ember.
 *
 *      Revision 1.72  2005/02/13 15:42:49  erik
 *      2005-02-13  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* Moved to OGRE 1.0 and CEGUI 0.2.
 *      	* Improved terrain LOD calculations. Though it still needs a lot of tweaking.
 *
 *      Revision 1.71  2005/02/10 00:54:03  erik
 *      2005-02-10  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* send event on eris polling start and stop
 *
 *      Revision 1.70  2005/01/22 22:54:29  erik
 *      2005-01-22 Erik Hjortsberg <erik@katastrof.nu>
 *
 *      	* updated INSTALL, TODO and README texts
 *      	* added support to the config file for fog distance and camera movement
 *      	* added some more error checking code. If some media is missing it should only report the error and continue.
 *
 *      Revision 1.69  2005/01/22 02:01:26  erik
 *      2005-01-22  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* Rewrote the input system. I've completely removed Ogre's input system and replaced it.
 *      	Ember now only depends on SDL for input.
 *      	This fixes all of the mouse issues as well as the issues with international characters.
 *
 *      Revision 1.68  2005/01/07 02:42:48  erik
 *      2005-01-07  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* check for, and if not found, copy config files from share to user's home dir
 *
 *      Revision 1.67  2005/01/07 01:07:17  erik
 *      2005-01-07  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* improves resource handling for modeldefinitions
 *      	* added dynamic tree generation
 *      		this is preliminary, to be replaced with a better solution in the future
 *      		it will really bring your GPU to its knees
 *      		it can be turned on and off in the config file
 *      	* added better Makefile handling of modeldefinition files (thanks Al Riddoch)
 *      	* use preloading of resources to prevent stuttering in the gameplay
 *
 *      Revision 1.66  2005/01/04 23:02:36  erik
 *      2005-01-04  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* moved all use of terrain coordinates, both in ogre and in atlas, to the common class TerrainPosition
 *      	* (EmberTerrainSceneManager.cpp) increased the page offset
 *      	* ember now loads all terrain at start, this might be slow on older machines. In the future we'll implement a paging mechanism.
 *      	* removed WF2OGRE_VECTOR3 macros
 *      	* moved to CEGUI 0.1.0, which uses ogre's resource system, thus making it easier to manage resources. This prompted some changes to the paths for gui elements.
 *      	* (DebugWidget)minor modifications
 *      	* (AvatarCamera)added method to get the screen coords for a given entity
 *      	* implement the use of EmberOgrePrerequisites.h
 *      	* (EmberEntity)added support for suggested responses from the server when an entity says something
 *      	* (EmberEntityFactory, EmberOgre)added method for accessing the world entity
 *      	* added IngameChatWidget for displaying "bubbles" over NPC's head when they speak
 *      	* (ServerService)better use of the wield op
 *
 *      Revision 1.65  2004/11/17 21:09:11  erik
 *      2004-11-17  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* added output of the eris log to debug widget
 *      	* fixed Wield and Use ops
 *
 *      Revision 1.64  2004/11/14 18:07:44  erik
 *      2004-11-14  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* fix stupid segfault on exit
 *
 *      Revision 1.63  2004/11/13 21:08:01  erik
 *      2004-11-13  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* Removed some bugs which in various ways stopped the main loop.
 *      	* Finalized on a resource structure.
 *      	* Tried to arrange the inludes in a way that made it easy to compile Ember in debug mode without getting bit by the special debug Ogre memory manager. It's not 100% though. For now, change OgreConfig.h, set OGRE_DEBUG_MEMORY_MANAGER  to 0.
 *      	* Fixed a bug which made Ember not release the mouse upon exit.
 *      	* Changed the gui to use png images instead of tga, since apparently some machines can't load the tga in a correct way.
 *
 *      Revision 1.62  2004/11/13 00:48:22  erik
 *      2004-11-13  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* Changed modeldefintions. Use "Action" instead of "Animation" because an action can contain more than just animations, sounds for example.
 *      	* Fixed wrong references to Ogre::TerrainSceneManager
 *      	* Changed location of config and media files.
 *
 *      Revision 1.61  2004/11/12 01:15:23  erik
 *      2004-11-12  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* Added EmberTerrainSceneManager, which removes the need to link to Ogre's TerrainSceneManager.
 *
 *      Revision 1.60  2004/11/11 23:57:26  erik
 *      2004-11-12  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	*  Added environment classes: Sun, Sky, Water, Foliage
 *      	* Rewrote Model handling to use a subclass of Ogre::ResourceManager
 *      	* Various fixes to get the resouce handling to work better for distributions.
 *      	* Upon startup now only a lone avatar is shown. This is for debugging and shall be removed.
 *      	* Removed bug where images were written to a non-existant directory, resulting in strange resource allocation problems. Ugly one.
 *      	* Various small changes to widgets.
 *
 *      Revision 1.57  2004/11/04 21:40:26  erik
 *      2004-11-04 Erik Hjortsberg <erik@katastrof.nu>
 *      http://erikhjortsberg.blogspot.com/
 *
 *      * Moved from dime to ember. Thus the big checkin.
 *      * Added wield action to inventory
 *
 *      Revision 1.56  2004/11/03 19:38:02  erik
 *      2004-11-03 Erik Hjortsberg <erik@katastrof.nu>
 *      http://erikhjortsberg.blogspot.com/
 *
 *      * Removed references to unused files.
 *      * Various fixes in preparation for distribution.
 *
 *      Revision 1.55  2004/11/03 02:01:14  erik
 *      2004-11-03 Erik Hjortsberg <erik@katastrof.nu>
 *      http://erikhjortsberg.blogspot.com/
 *
 *      * Added configuration through Varconf. All configuration is now done from the directory ~/.ember
 *      * Added distorted fresnel water. One step closer to FarCry scenery :) This requires a GPU with support for Pixel Shader v. 2.0 (GeForceFX and higher for example)
 *      Screens can be found at the usual place, http://purple.worldforge.org/~erik/ember/screens/?C=M;O=D
 *      * Began work to separate environment into own classes (water, sun, sky etc.)
 *
 *      Revision 1.54  2004/11/02 01:19:55  erik
 *      2004-11-02 Erik Hjortsberg <erik@katastrof.nu>
 *      http://erikhjortsberg.blogspot.com/
 *
 *      * Added a better placeholder model than the ogre space ship. Ember now no longer use any media linked to the ogre sample media. In preparation for a release all media will reside under ~/.ember/Media . In the future we'll probably use a shared location, but this is only when we do a real release. For the upcoming release, which is more of a development release, ~/.ember/Media will be best. Media can be found at http://purple.worldforge.org/~erik/ember/media/Media.tar.gz
 *      * Removed a silly bug which made Ember segfault on exit.
 *
 *      Revision 1.53  2004/10/30 17:32:38  erik
 *      2004-10-30 Erik Hjortsberg <erik@hysteriskt.nu>
 *      http://erikhjortsberg.blogspot.com/
 *
 *      * Fixed the issue with entities not getting correct orientation.
 *      * Added a give action to the EntityPickerWidget
 *      * Fixed creation of entities through MakeEntityWidget
 *
 *      Revision 1.52  2004/10/27 23:45:21  erik
 *      2004-10-28 Erik Hjortsberg <erik@hysteriskt.nu>
 *      http://erikhjortsberg.blogspot.com/
 *
 *      * moved to Eris 1.3
 *      This requires some changes to the way enties are handled etc..
 *      There's a big bug which I can't seem to track down. It manifests itself in that entities attached to other entities (i.e. not directly attached to the root node) can't change their orientation. An interim solution for now is to direct all entities attached to the world to the root node. This works for now, but the effect can be seen when the avatar walk into the sty.
 *      *moved to Ogre 0.15.0
 *      Ember now compiles and works against the latest ogre point release. We'll try to keep it this way.
 *      As a result of the move to 0.15.0 I've updated the meshes (Since there's been changes to the binary .mesh format. New meshes can be downloaded from http://purple.worldforge.org/~erik/ember/media/Media.tar.gz.).
 *      *added some more functionality to ServerBrowserWidget and ServerWidget
 *
 *      Revision 1.51  2004/10/14 00:13:14  erik
 *      2004-10-14 Erik Hjortsberg <erik@hysteriskt.nu>
 *
 *      * moved EventCreatedAvatarEntity to EmberOgre class and adjusted inventory
 *
 *      Revision 1.50  2004/10/12 00:49:54  erik
 *      2004-10-12 Erik Hjortsberg <erik@hysteriskt.nu>
 *
 *      * added mouse picker menu for touching and taking entities. Right now it's just a placeholder, but it works, can be seen here: http://purple.worldforge.org/~erik/ember/screens/screenshot_20041012_023002.png
 *      * added a simple inventory. It's just a listbox right now, but it works. The drop button won't do anything yet though. It can be seen here too: http://purple.worldforge.org/~erik/ember/screens/screenshot_20041012_023044.png
 *      http://purple.worldforge.org/~erik/ember/screens/screenshot_20041012_023052.png
 *
 *      Revision 1.49  2004/10/06 22:32:48  erik
 *      2004-10-07 Erik Hjortsberg <erik@hysteriskt.nu>
 *
 *      *Moved completely to CEGUI. We now use no overlays.
 *      *Changed the way the input system and the camera works:
 *      	- Normal mode is gui mode where the mouse can be moved around, windows changed and interacted with. If one clicks on the Ogre render window mouse picking in the 3d world will occur (not implemented yet)
 *      	- By pressing and holding the right mouse button one enters Movement mode. Mouse movement here will change the camera, which is centered on the avatar.
 *      	- Pressing keys will move the avatar, UNLESS we're in Normal mode and a CEGUI widget says otherwise (the console for instance). This is not yet fully implemented.
 *
 *      *Implemented a widget system. Each widget is something like a map, an inventory, avatar stats etc.. These should preferrably be loaded through dynamic libs as plugins in the future. We'll also add lua-scripting (which should be easy since it's almost already in CEGUI).
 *
 *      *Removed references to InputManager since it's obsolete now.
 *
 *      Revision 1.48  2004/09/11 23:50:59  erik
 *      2004-09-12 Erik Hjortsberg <erik@hysteriskt.nu>
 *
 *      /src/components/ogre/:
 *      *added preliminary ground cover through the class GroundCover, this requires some new meshes (small_plant and path_01)
 *      *fixes and additions to the animation system
 *
 *      Revision 1.47  2004/08/22 22:20:01  erik
 *      2004-08-23 Erik Hjortsberg <erik@hysteriskt.nu>
 *
 *      /src/components/ogre/:
 *      *added PersonEmberEntity for taking care of person entities (which can hold stuff in their hands etc.)
 *      *small changes to scene layout, mostly for testing 3d performance
 *      *use better handling of bounding boxes in ogre, there's a call to MeshManager in EmberOgre.cpp which requires a patch to ogre. The call can be safely removed though.
 *
 *      Revision 1.46  2004/08/19 21:23:48  erik
 *      2004-08-19 Erik Hjortsberg <erik@hysteriskt.nu>
 *
 *      /src/components/ogre/:
 *      *added support for models using modeldef.xml format
 *
 *      Revision 1.45  2004/08/04 23:39:43  aglanor
 *      2004-08-05 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *      	* /src/components/ogre/GUIManager.(h|cpp) and others related:
 *      	Added basic GUI support, including display of In-Game chat.
 *
 *      Revision 1.44  2004/07/31 23:42:15  erik
 *      2004-08-01 Erik Hjortsberg <erik@hysteriskt.nu>
 *
 *      /src/components/ogre:
 *      *use skybox instead of skydome
 *
 *      Revision 1.43  2004/07/31 11:53:59  erik
 *      2004-07-31 Erik Hjortsberg <erik@hysteriskt.nu>
 *
 *      /src/components/ogre:
 *      *cleaned up the class layout and structured it, we're not using that many Singletons anymore
 *      *moved to Ogre CVS in order to get a more decent terrainmanager
 *      *added splatting and nice texturing to the terrain
 *      *added classes TerrainShader and EmberTerrainPageSource which works with the new terrainmanager
 *
 *      Revision 1.42  2004/07/21 00:27:30  erik
 *      2004-07-21 Erik Hjortsberg <erik@hysteriskt.nu>
 *
 *      /src/components/ogre:
 *      *removed references to EntityListener, we'll keep the class however as
 *      we'll pobably use it in the future
 *      *put everything under the namespace EmberOgre
 *
 *      Revision 1.41  2004/07/20 22:52:16  erik
 *      2004-07-21 Erik Hjortsberg <erik@hysteriskt.nu>
 *
 *      in src/components/ogre:
 *
 *      MathConverter.h:
 *      *changed scaling
 *
 *      EmberEntity.*:
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
 *      EmberEntityFactory.*:
 *      *added support for AvatarEmberEntity
 *
 *      AvatarEmberEntity.*:
 *      New class for handling the Avatar entity
 *
 *      AvatarCamera.*:
 *      New class for handling the Avatar camera
 *
 *      EmberOgre.*:
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
 *      *Ember now depends on Mercator
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
 *      Revamping of how entities are handled though the new class EmberEntity and changes to EntityListener.
 *      RuEmberntary animation of the avatar.
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
 *              * src/services/server/ServerService.cpp: myView is
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
 *              EmberOgre.cpp: instantiates and configures
 *              PlayerMouseListener.
 *
 *      Revision 1.29  2003/11/05 08:10:56  aglanor
 *      2003-11-04 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *              * src/components/ogre:
 *              MediaDeployer.(h|cpp): added methods for adding media.
 *              EmberOgre.cpp: some cleanup.
 *
 *      Revision 1.28  2003/10/30 07:37:41  aglanor
 *      2003-10-30 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *              * src/services/components/ogre:
 *              Makefile.am, EmberOgre.(h|cpp), ConsoleObjectImpl.(h|cpp):
 *              Moved Ogre response to console commands from the app
 *              to its own class, ConsoleObjectImpl.
 *
 *      Revision 1.27  2003/10/29 18:25:25  aglanor
 *      2003-10-29 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *              * src/services/components/ogre/: EmberOgre.(h|cpp):
 *              Added ConsoleObject inheritance, it runs the Quit command.
 *
 *      Revision 1.26  2003/10/25 21:07:40  aglanor
 *      2003-09-28 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *              * src/services/components/ogre/: Makefile.am,
 *              Console.(h|cpp), InputManager.(h|cpp), EmberOgre.cpp:
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
 *      Removed code and includes from EmberServices.h and put them in
 *      EmberServices.cpp
 *
 *      Fixed other classes and Makefile.am to work again
 *
 *      Make sure you autogen.sh and configure again...
 *
 *      Revision 1.24  2003/10/22 08:42:08  lee
 *      Added include of stdlib.h to EmberOgre to stop errors from not declaring
 *      malloc in OgreStdHeaders.h (why it needs it, i don't know)
 *
 *      Revision 1.23  2003/10/21 20:51:42  aglanor
 *      actually restored it
 *
 *      Revision 1.22  2003/10/21 20:48:37  aglanor
 *      Restored SkyBox
 *
 *      Revision 1.21  2003/10/18 23:18:37  sandalle
 *      * src/components/ogre/EmberOgre.cpp:290: Actually comment it out...
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
 *              * EmberOgre: bound key "5" to character creation
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
 *      	Check screenshot of fully textured pig within Ember here:
 *      	http://purple.worldforge.org/~aglanor/screenshots/Ember_20030623.png
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
 *      	* EmberOgre.h/cpp: added a Water Plane at height 0.
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
 *      EmberOgre app is again linked back from the FrameListener, so entities sighted
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
 *      	more relevant here than Ember or eris. Now it uses the default namesp
 *

-----------------------------------------------------------------------------
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

// Headers to stop compile problems from headers
#include <stdlib.h>
// ------------------------------
// Include Eris header files
// ------------------------------
#include <Eris/Entity.h>
#include <Eris/View.h>

//Ember headers
#include "services/EmberServices.h"
#include "services/logging/LoggingService.h"
#include "services/server/ServerService.h"
#include "services/config/ConfigService.h"
#include "services/metaserver/MetaserverService.h"
#include "framework/ConsoleBackend.h"
#include "framework/ConsoleObject.h" //TODO: this will be included in a different class


// ------------------------------
// Include OGRE Ember client files
// ------------------------------
//#include "OgreGameView.h"
//#include "MathConverter.h"
//#include "EmberTerrainPageSource.h"
#include "TerrainGenerator.h"


#include "ConsoleObjectImpl.h"
#include "Avatar.h"
#include "AvatarController.h"
#include "EntityListener.h"
#include "EmberEntityFactory.h"
#include "MotionManager.h"
#include "AvatarCamera.h"
#include "GUIManager.h"


#include "EmberEntity.h"
#include "WorldEmberEntity.h"

#include "environment/meshtree/TParameters.h"
#include "environment/Tree.h"

#include "carpenter/Carpenter.h"
#include "carpenter/BluePrint.h"



#include "EmberSceneManager/include/EmberTerrainSceneManager.h"
#include "model/ModelDefinitionManager.h"
#include "model/ModelDefinition.h"
#include "model/Model.h"

// ------------------------------
// Include Ember header files
// ------------------------------

#include "framework/ConsoleBackend.h"


#include "EmberOgre.h"

// ------------------------------
// Include Eris header files
// ------------------------------
#if defined( _MSC_VER ) && ( _MSC_VER < 1300 )
// GNDN: MSVC < version 7 is broken
#else
#include <Eris/PollDefault.h>
#include <Eris/Log.h>
#include <Eris/TypeInfo.h>
#include <Eris/Factory.h>
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

#include "EmberOgrePrerequisites.h"
// #include <OgreFontManager.h>
// #include <OgreParticleSystemManager.h>

#ifdef DEBUGTEMP
#undef DEBUGTEMP
#define DEBUG
#endif

#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>


#include "jesus/Jesus.h"

namespace EmberOgre {



// TODO: move CerrLogObserver to its own class (under Logging service, or under Framework)
  class CerrLogObserver: public Ember::LoggingService::Observer
    {
    public:
        CerrLogObserver()
        {
        }

        virtual void onNewMessage(const std::string & message, const std::string & file, const int & line,
                                  const Ember::LoggingService::MessageImportance & importance, const time_t & timeStamp)
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

            if(importance == Ember::LoggingService::CRITICAL)
                {
                    std::cerr << "CRITICAL";
                }
            else  if(importance == Ember::LoggingService::FAILURE)
                {
                    std::cerr << "FAILURE";
                }
            else if(importance == Ember::LoggingService::WARNING)
                {
                    std::cerr << "WARNING";
                }
            else if(importance == Ember::LoggingService::INFO)
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
    
template<> EmberOgre* Ember::Singleton<EmberOgre>::ms_Singleton = 0;

EmberOgre::EmberOgre() :
//mFrameListener(0),
mRoot(0),
mKeepOnRunning(true)
{}

EmberOgre::~EmberOgre()
{
	if (mGUIManager)
		delete mGUIManager;
	if (mTerrainGenerator)
		delete mTerrainGenerator;
	if (mMotionManager)
		delete mMotionManager;
	if (mAvatar)
		delete mAvatar;
	if (mAvatarController)
		delete mAvatarController;
	if (mModelDefinitionManager)
		delete mModelDefinitionManager;
/*	if (mEmberEntityFactory)
		delete mEmberEntityFactory;*/
	if (mRoot)
		delete mRoot;
		
		
		

}


bool EmberOgre::frameStarted(const Ogre::FrameEvent & evt)
{
	EventStartErisPoll.emit();
	Eris::PollDefault::poll(1);
	EventEndErisPoll.emit();
	
	
	if (!mKeepOnRunning)
		fprintf(stderr, "Shutting down EmberOgre.\n");
	return mKeepOnRunning;
}


void EmberOgre::go(void)
{
	if (!setup())
		return;

	mRoot->startRendering();
}

void EmberOgre::shutdown()
{
	mKeepOnRunning = false;
}

    
// These internal methods package up the stages in the startup process
/** Sets up the application - returns false if the user chooses to abandon configuration. */
bool EmberOgre::setup(void)
{
	
	Ember::ConfigService* configSrv = Ember::EmberServices::getInstance()->getConfigService();

	checkForConfigFiles();
	
    mRoot = new Ogre::Root();
	
    setupResources();

    bool carryOn = configure();
    if (!carryOn) return false;

    chooseSceneManager();

    // Set default mipmap level (NB some APIs ignore this)
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    
    // Set default animation mode
	Ogre::Animation::setDefaultInterpolationMode(Ogre::Animation::IM_SPLINE);

	//remove padding for bounding boxes
    Ogre::MeshManager::getSingletonPtr()->setBoundsPaddingFactor(0);    
	
// 	//add ourself as a frame listener
	Ogre::Root::getSingleton().addFrameListener(this);

 	fprintf(stderr, "TRACE - BEGIN PRELOAD\n");
 	preloadMedia();
 	fprintf(stderr, "TRACE - END PRELOAD\n");
	
	
	mGUIManager = new GUIManager(mWindow, mSceneMgr);
    
	

	chdir(configSrv->getHomeDirectory().c_str());
	
	mTerrainGenerator = new TerrainGenerator();
	mMotionManager = new MotionManager();
	mMotionManager->setTerrainGenerator(mTerrainGenerator);
	
	// Avatar
	mAvatar = new Avatar(mSceneMgr);
	
	mAvatarController = new AvatarController(mAvatar, mWindow, mGUIManager);





	mRoot->addFrameListener(mMotionManager);
	ConsoleObjectImpl::getSingleton();
	mSceneMgr->setPrimaryCamera(mAvatar->getAvatarCamera()->getCamera());


	mGUIManager->initialize();
	
	// Create the scene
    createScene();

	setupJesus();
/*
    EmberEntityFactory = new EmberEntityFactory(mSceneMgr);
    
    Ember::EmberServices::getInstance()->getServerService()->getView()->registerFactory(EmberEntityFactory);
*/
    return true;

}
/** Configures the application - returns false if the user chooses to abandon configuration. */
bool EmberOgre::configure(void)
{
    // Show the configuration dialog and initialise the system
    // You can skip this and use root.restoreConfig() to load configuration
    // settings if you were sure there are valid ones saved in ogre.cfg
	bool success = mRoot->restoreConfig();
    if(success)
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

void EmberOgre::chooseSceneManager(void)
{
	
    // We first register our own scenemanager
    EmberTerrainSceneManager* sceneManager = new EmberTerrainSceneManager();
    mRoot->setSceneManager(Ogre::ST_EXTERIOR_FAR, sceneManager);
    //And then request it
    mSceneMgr = static_cast<EmberTerrainSceneManager*>(mRoot->getSceneManager(Ogre::ST_EXTERIOR_FAR));
    
    EmberTerrainSceneManager* mEmberTerr = dynamic_cast<EmberTerrainSceneManager*>(mSceneMgr);
    assert(mEmberTerr);

    
    
}

void EmberOgre::checkForConfigFiles()
{
 	Ember::ConfigService* configSrv = Ember::EmberServices::getInstance()->getConfigService();

	chdir(Ember::EmberServices::getInstance()->getConfigService()->getHomeDirectory().c_str());
		
	//make sure that there are files 
	struct stat tagStat;
    int ret;
	
	ret = stat( "ogre.cfg", &tagStat );
	if (ret == -1) {
		//copy conf file from shared
		std::string sharePath(ETCDIR);
		sharePath = sharePath + "/ember/ogre.cfg";
		std::ifstream  IN (sharePath.c_str());
		std::ofstream  OUT ("ogre.cfg"); 
		OUT << IN.rdbuf();
	}
	ret = stat( "resources.cfg", &tagStat );
	if (ret == -1) {
		//copy conf file from shared
		std::string sharePath(ETCDIR);
		sharePath = sharePath + "/ember/resources.cfg";
		std::ifstream  IN (sharePath.c_str());
		std::ofstream  OUT ("resources.cfg"); 
		OUT << IN.rdbuf();
	}
	ret = stat( "plugins.cfg", &tagStat );
	if (ret == -1) {
		//copy conf file from shared
		std::string sharePath(ETCDIR);
		sharePath = sharePath + "/ember/plugins.cfg";
		std::ifstream  IN (sharePath.c_str());
		std::ofstream  OUT ("plugins.cfg"); 
		OUT << IN.rdbuf();
	}
}

void EmberOgre::getResourceArchiveFromVarconf(Ogre::ResourceManager* manager, std::string variableName, std::string section, std::string type)
{
// 	if (Ember::EmberServices::getInstance()->getConfigService()->itemExists(section, variableName)) {
// 		std::string value =  Ember::EmberServices::getInstance()->getConfigService()->getHomeDirectory() + std::string(Ember::EmberServices::getInstance()->getConfigService()->getValue(section, variableName)) + "/";
// 		manager->addArchiveEx(value, type);
// 	} else {
// 		//throw new Exception(std::string("Could not find setting: ") + variableName + " in section " + section + ".");
// 	}

} 

/// Method which will define the source of resources (other than current folder)
void EmberOgre::setupResources(void)
{
 	Ember::ConfigService* configSrv = Ember::EmberServices::getInstance()->getConfigService();

	chdir(Ember::EmberServices::getInstance()->getConfigService()->getHomeDirectory().c_str());
		

	
	std::string mediaHomePath = Ember::EmberServices::getInstance()->getConfigService()->getEmberDataDirectory() + "media/";
	
	mModelDefinitionManager = new ModelDefinitionManager();
	
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(mediaHomePath + "modeldefinitions", "FileSystem", "modeldefinitions");
 	
	if (!(configSrv->itemExists("tree", "usepregeneratedtrees") && ((bool)configSrv->getValue("tree", "usepregeneratedtrees")))) { 
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(mediaHomePath + "modeldefinitions/trees/dynamic", "FileSystem", "modeldefinitions");
 	} else {
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(mediaHomePath + "modeldefinitions/trees/pregenerated", "FileSystem", "modeldefinitions");
	}
	
// /*	std::string modeldefspath = "modeldefinitions/";
// 	if (Ember::EmberServices::getInstance()->getConfigService()->itemExists("ogre", "modeldefinitionpath")) {
// 		modeldefspath =  Ember::EmberServices::getInstance()->getConfigService()->getHomeDirectory() + std::string(Ember::EmberServices::getInstance()->getConfigService()->getValue("ogre", "modeldefinitionpath")) + "/";
// 	} 
// 	mModelDefinitionManager->addArchiveEx(modeldefspath, "FileSystem");*/
// 	
// 	getResourceArchiveFromVarconf(mModelDefinitionManager, "modeldefinitions");
// 	getResourceArchiveFromVarconf(Ogre::TextureManager::getSingletonPtr(), "common");
// 	getResourceArchiveFromVarconf(Ogre::GpuProgramManager::getSingletonPtr(), "programs");
// 	getResourceArchiveFromVarconf(Ogre::FontManager::getSingletonPtr(), "fonts");
// 	getResourceArchiveFromVarconf(Ogre::MaterialManager::getSingletonPtr(), "materials");
// 	getResourceArchiveFromVarconf(Ogre::TextureManager::getSingletonPtr(), "textures");
// 	getResourceArchiveFromVarconf(Ogre::ParticleSystemManager::getSingletonPtr(), "particles");

		    
	// Load resource paths from config file
    Ogre::ConfigFile cf;
    cf.load("resources.cfg");

    
	// Go through all sections & settings in the file
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
	
	Ogre::String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
				mediaHomePath + archName, typeName, secName);
		}
	}
	
	
	
//     Ogre::ResourceManager::addCommonArchiveEx( Ember::EmberServices::getInstance()->getConfigService()->getSharedDataDirectory() + "media/", "FileSystem");
	
	fprintf(stderr, "TRACE - ADDED MEDIA PATHS\n");
	
}



void EmberOgre::preloadMedia(void)
{
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	Ember::ConfigService* configSrv = Ember::EmberServices::getInstance()->getConfigService();
	

	std::vector<std::string> shaderTextures;
	
	shaderTextures.push_back(std::string(configSrv->getValue("shadertextures", "rock")));
	shaderTextures.push_back(std::string(configSrv->getValue("shadertextures", "sand")));
	shaderTextures.push_back(std::string(configSrv->getValue("shadertextures", "grass")));
	
	for (std::vector<std::string>::iterator I = shaderTextures.begin(); I != shaderTextures.end(); ++I) {
		try {
			Ogre::TextureManager::getSingleton().load(*I, "General");
		} catch (Ogre::Exception e) {
			std::cerr << "Error when loading texture " << *I << ".\n";
		}
	}	
	  
	
	//TODO: use C++ io methods
	DIR *dp;
	struct dirent *ep;
	
	std::string modeldefDir = Ember::EmberServices::getInstance()->getConfigService()->getEmberDataDirectory() + "media/modeldefinitions";
	dp = opendir (modeldefDir.c_str());
	if (dp != NULL)
	{
		//Ogre::SceneNode* mNode = dynamic_cast<Ogre::SceneNode*>(mSceneMgr->getRootSceneNode()->createChild());
		while (ep = readdir (dp)) {
			if (ep->d_name != "." && ep->d_name != "..") {
				try {
					fprintf(stderr, (std::string("TRACE - PRELOADING: ") + ep->d_name + "\n").c_str());
					ModelDefinitionPtr modeldef = mModelDefinitionManager->load(ep->d_name, "modeldefinitions");
/*					if (modeldef->isValid()) {
						Model* model = Model::Create(ep->d_name, ep->d_name);
						mNode->attachObject(model);
					}*/
				} catch (Ogre::Exception ex)
				{
					fprintf(stderr, (std::string("TRACE - ERROR PRELOADING: ") + ep->d_name + "\n").c_str());
				}
			}
		}
		(void) closedir (dp);
/*		mSceneMgr->destroySceneNode(mNode->getName());*/
	}
	
	//only autogenerate trees if we're not using the pregenerated ones
 	if (!(configSrv->itemExists("tree", "usepregeneratedtrees") && ((bool)configSrv->getValue("tree", "usepregeneratedtrees")))) { 
		Tree tree;
		tree.makeMesh("GeneratedTrees/European_Larch", Ogre::TParameters::European_Larch);
		tree.makeMesh("GeneratedTrees/Fir", Ogre::TParameters::Fir);
 	}	

	
	
// 	mModelDefinitionManager->load("malebuilder.modeldef.xml");
// 	mModelDefinitionManager->load("settler.modeldef.xml");
// 	mModelDefinitionManager->load("merchant.modeldef.xml");
// 	mModelDefinitionManager->load("pig.modeldef.xml");
// 	mModelDefinitionManager->load("fir.modeldef.xml");
// 	mModelDefinitionManager->load("oak.modeldef.xml");


}

void EmberOgre::setupJesus()
{
	const std::string datadir = Ember::EmberServices::getInstance()->getConfigService()->getEmberDataDirectory();

	Carpenter::Carpenter* carpenter = new Carpenter::Carpenter();
	mJesus = new Jesus(carpenter);

	mJesus->loadBlockSpec(datadir + "/carpenter/blockspec/floors.blockspec.xml");
	mJesus->loadBlockSpec(datadir + "/carpenter/blockspec/walls.blockspec.xml");
	mJesus->loadBlockSpec(datadir + "/carpenter/blockspec/roofs.blockspec.xml");
	mJesus->loadBlockSpec(datadir + "/carpenter/blockspec/slopewalls.blockspec.xml");
	mJesus->loadBlockSpec(datadir + "/carpenter/blockspec/adapters.blockspec.xml");
	mJesus->loadBuildingBlockSpecDefinition(datadir + "/carpenter/modelblockspecs/general.modelblocks.xml");
	
	mJesus->loadModelBlockMapping(datadir + "/jesus/modelmappings/general.modelmapping.xml");
	
	
//	Carpenter::BluePrint* housePrint = mJesus->loadBlueprint(datadir + "/carpenter/blueprints/house.blueprint.xml");
	Carpenter::BluePrint* blueprint = mJesus->loadBlueprint(datadir + "/carpenter/blueprints/empty.blueprint.xml");
	
    Ogre::SceneNode* node;
    node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	blueprint->compile();
	Construction* construction = new Construction(blueprint, mJesus, node);
	
	EventCreatedJesus.emit(mJesus);

}

void EmberOgre::createScene(void)
{
/*  mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_MODULATIVE);
 mSceneMgr->setAmbientLight(Ogre::ColourValue(0, 0, 0));
  mSceneMgr->setShadowColour(Ogre::ColourValue(0.5, 0.5, 0.5));
  mSceneMgr->setShowDebugShadows(true);
//  mSceneMgr->setShadowFarDistance(2000);
 	mSceneMgr->showBoundingBoxes(true);
 */
  // Create a light
  


/*  
        Ogre::SceneNode* node;
        node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        Ogre::Entity* mAthene = mSceneMgr->createEntity( "athene", "athene.mesh" );
        //mAnimState = pEnt->getAnimationState("Walk");
        //mAnimState->setEnabled(true);
        node->attachObject( mAthene );
        node->translate(0, 10, 0);
        node->scale(0.1,0.1,0.1);
        node->yaw(90);
*/
	//set fog, do this before calling TerrainSceneManager::setViewGeometry 
//	Ogre::ColourValue fadeColour(0.93, 0.86, 0.76);
mSceneMgr->setAmbientLight(Ogre::ColourValue(1, 1, 1));
	Ogre::ColourValue fadeColour(1,1,1);
	double fogstartDistance = 192; //default for fog
	if (Ember::EmberServices::getInstance()->getConfigService()->itemExists("graphics", "fogstart")) {
		fogstartDistance = (double)Ember::EmberServices::getInstance()->getConfigService()->getValue("graphics", "fogstart");
	}
	mSceneMgr->setFog( Ogre::FOG_LINEAR, fadeColour, .001, fogstartDistance, 256);
	
	
	
	

  // create a Skydome
//  mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);

  
         
	 
	
}

void EmberOgre::connectViewSignals(Eris::View* world)
{
    Eris::Factory::registerFactory(mEmberEntityFactory);
	//world->registerFactory(mEmberEntityFactory, 10);
	
	EntityListener::getSingleton().connectViewSignals();
}

void EmberOgre::connectedToServer(Eris::Connection* connection) 
{
	mEmberEntityFactory = new EmberEntityFactory(mSceneMgr,mTerrainGenerator, connection->getTypeService());
	EventCreatedAvatarEntity.connect(SigC::slot(*mAvatar, &Avatar::createdAvatarEmberEntity));
	EventCreatedEmberEntityFactory.emit(mEmberEntityFactory);
}



Avatar* EmberOgre::getAvatar() {
	return mAvatar;
}


EmberTerrainSceneManager* EmberOgre::getSceneManager()
{
	return mSceneMgr;
}

TerrainGenerator* EmberOgre::getTerrainGenerator()
{
	return mTerrainGenerator;
}

MotionManager* EmberOgre::getMotionManager()
{
	return mMotionManager;
}

Ogre::Root* EmberOgre::getOgreRoot()
{
	assert(mRoot);
	return mRoot;
}

Ogre::SceneNode * EmberOgre::getWorldSceneNode( )
{
	return mEmberEntityFactory->getWorld()->getSceneNode();
/*	Ogre::SceneNode* node = mSceneMgr->getSceneNode("0");
	//TODO: implement better exception handling
	if (node == 0)
		throw Exception();
	return node;*/
}

AvatarCamera* EmberOgre::getMainCamera()
{
	return mAvatar->getAvatarCamera();
}

EmberEntityFactory* EmberOgre::getEntityFactory()
{
	return mEmberEntityFactory;
}

void EmberOgre::initializeEmberServices(void)
{
	// Initialize Ember services

	// Initialize the Logging service and an error observer
	Ember::LoggingService *logging = Ember::EmberServices::getInstance()->getLoggingService();
	CerrLogObserver* obs = new CerrLogObserver();
	obs->setFilter(Ember::LoggingService::VERBOSE);
	logging->addObserver(obs);


	// Initialize the Configuration Service
	Ember::EmberServices::getInstance()->getConfigService()->start();
	// Change working directory
	struct stat tagStat;
    int ret;
	ret = stat( Ember::EmberServices::getInstance()->getConfigService()->getHomeDirectory().c_str(), &tagStat );
	if (ret == -1) {
		mkdir(Ember::EmberServices::getInstance()->getConfigService()->getHomeDirectory().c_str(), S_IRWXU);
	}
	
	
	chdir(Ember::EmberServices::getInstance()->getConfigService()->getHomeDirectory().c_str());
	
	ret = stat( "ember.conf", &tagStat );
	if (ret == -1) {
		//copy conf file from shared
		std::string sharePath(ETCDIR);
		sharePath = sharePath + "/ember/ember.conf";
		std::ifstream  IN (sharePath.c_str());
		std::ofstream  OUT ("ember.conf"); 
		OUT << IN.rdbuf();
	}
	Ember::EmberServices::getInstance()->getConfigService()->loadSavedConfig("ember.conf");


/*
	// Initialize the Sound Service
#ifndef WIN32
	// Test that /dev/dsp is availible
	FILE *temp = fopen("/dev/dsp","w");
	if (temp) {
	  fclose(temp);
#endif
	// Initialize the SoundService
	Ember::EmberServices::getInstance()->getSoundService()->start();
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

	Ember::EmberServices::getInstance()->getMetaserverService()->start();

	// Initialize the Server Service
	Ember::EmberServices::getInstance()->getServerService()->GotConnection.connect(SigC::slot(*this, &EmberOgre::connectedToServer));
	Ember::EmberServices::getInstance()->getServerService()->GotView.connect(SigC::slot(*this, &EmberOgre::connectViewSignals));
	
	Ember::EmberServices::getInstance()->getServerService()->start();
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
    EmberOgre::EmberOgre app;

	// Initialize all Ember services needed for this application
	app.initializeEmberServices();

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



