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
 *      Revision 1.121  2006-01-25 21:18:36  erik
 *      2006-01-25  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *          * src/components/ogre/Avatar.cpp: set the walking speed to 2.5 meters per second, to fit with cyphesis
 *          * src/components/ogre/AvatarEmberEntity.h: removed reference to PersonEmberEntity
 *          * src/components/ogre/ConsoleObjectImpl.*, src/components/ogre/EmberOgre.cpp: use the Singleton baseclass
 *          * src/components/ogre/EmberEntity.*, src/components/ogre/WorldEmberEntity.cpp: no need to hold a reference to the SceneManager since it's available through the SceneNode
 *          * src/components/ogre/EmberEntityFactory.*: the creation of PhysicalEntityEntity and AvatarEmberEntity shouldn't be so different
 *
 *      Revision 1.120  2006/01/24 19:33:12  erik
 *      2006-01-24  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *          * src/components/ogre/EmberOgre.cpp, src/components/ogre/GUIManager.cpp,  src/components/ogre/TerrainPage.cpp, src/components/ogre/carpenter/BluePrint.cpp, src/components/ogre/environment/Foliage.cpp,  src/components/ogre/environment/FoliageArea.cpp,  src/components/ogre/model/Model.cpp,  src/components/ogre/model/XMLModelDefinitionSerializer.cpp,  src/components/ogre/widgets/IconBase.cpp,  src/components/ogre/widgets/IngameChatWidget.cpp,  src/components/ogre/widgets/MeshPreview.cpp,  src/services/server/ServerService.cpp: catch exceptions as const
 *          * src/services/scripting/ScriptingService.cpp: better error output
 *
 *      Revision 1.119  2006/01/23 23:36:54  erik
 *      2006-01-24  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *          * src/services/config/ConfigService.cpp: adapt paths so they will work better on win32
 *          * ember.conf: fixed the name of "logginglevel" (not "loggingmode")
 *          * ember.vcproj: updated to work with cvs, though some paths still need to be fixed
 *          * EmberOgre.cpp:
 *              * better exception reporting and handling
 *              * since there was some problems with building the SoundManager on windows, disable calls to it for now on that platform
 *          * src/components/ogre/widgets/EntityCEGUITexture.cpp: added more verbose logging
 *          * src/components/ogre/widgets/GUIManager.cpp: reactivate the quit widget
 *          * src/components/ogre/model/NodelDefinition.h: removed unneeded reference to hash_map
 *          * src/components/ogre/SceneManagers/EmberPagingSceneManager/src/OgrePagingLandScapeIntersectionSceneQuery.cpp: removed code that won't compile and isn't used anyway
 *
 *      Revision 1.118  2006/01/15 15:32:23  aglanor
 *      2006-01-15  Miguel Guzman  <aglanor@gmail.com>
 *
 *      	* configure.ac: added check for freealut 1.0.0
 *      	* src/components/ogre/EmberOgre.cpp: added traces to std::cout since it can't log to the OgreLogObserver until Ogre has started
 *      	* src/services/sound/SoundService.cpp: added playing of a "Hello World" sound when the command "/playsound" is invoked
 *
 *      Revision 1.117  2006/01/15 11:47:44  erik
 *      2006-01-15  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *          * src/components/ogre/modeldefinitions/modeldefinition.xsd: update to bring to sync with the latest ModelDefinion.h
 *          * src/components/ogre/jesus/Jesus.cpp: make sure all scenemanager happens through the referred nodes, since the scene manager involved might not be the system default (for example when used from EntityCEGUITexture
 *          * src/components/ogre/AvatarCamera.cpp: removed unneeded casts
 *          * src/components/ogre/EmberOgre.cpp: typo fix
 *
 *      Revision 1.116  2006/01/11 22:23:09  erik
 *      2006-01-11  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* Ember.kdevelop: updated kdevelop project file
 *      	* ember.conf: added options for setting recursive resource scanning (needed by authoring, but takes some time at startup)
 *      	* src/components/ogre/EmberOgre.cpp: added support for the recursive resource scanning option in ember.conf
 *
 *      Revision 1.115  2006/01/10 01:03:27  erik
 *      2006-01-10  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* src/components/ogre/EmberEntityFactory.*:
 *      		* use getSingletonPtr instead of the deprecated getInstance
 *      		* removed PersonEmberEntity
 *      		* adapt to the model classes being moved into the Model namespace
 *      	* src/components/ogre/EmberEntityUserObject.*: adapt to the model classes being moved into the Model namespace
 *      	* src/components/ogre/EmberOgre.*:
 *      		* added MetaServerService and ScrtiptingService
 *      		* use getSingletonPtr instead of the deprecated getInstance
 *      		* better exception catching
 *      		* added a concept of "user media directory", which will mirror the shared media directory (which is updated from the server), but will allow the user to add it's own media which will override the shared media.
 *      		* load all resources recursive. This adds some overhead to startup, but allows us to search for resources later on
 *      		* adapt to the model classes being moved into the Model namespace
 *
 *      Revision 1.114  2005/10/26 21:44:55  erik
 *      2005-10-26  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* src/components/ogre/WorldEmberEntity.cpp: use the getView method from the Eris::Entity base class instead
 *      	* src/components/ogre/TerrainGenerator.*, src/components/ogre/MousePicker.h, src/components/ogre/GUIManager.*, src/components/ogre/EmberOgre.*, src/components/ogre/Avatar.*, src/components/ogre/AvatarController.*, src/components/ogre/AvatarCamera.*, src/components/ogre/widgets/ChatWidget.cpp, src/components/ogre/widgets/ConsoleWidget.*, src/components/ogre/widgets/EntityPickerWidget.cpp, src/components/ogre/widgets/GiveWidget.cpp, src/components/ogre/widgets/Help.cpp, src/components/ogre/widgets/IngameChatWidget.cpp, src/components/ogre/widgets/InspectWidget.cpp, src/components/ogre/widgets/InventoryWidget.cpp, src/components/ogre/widgets/JesusEdit.cpp, src/components/ogre/widgets/MakeEntityWidget.cpp, src/components/ogre/widgets/MeshPreview.h, src/components/ogre/widgets/Quit.cpp, src/components/ogre/widgets/ServerBrowserWidget.cpp, src/components/ogre/widgets/StatusIconBar.cpp, src/components/ogre/widgets/Widget.h
 *      		*   use the sigc 2.0 methods, which allows us to use the very nice sigc::trackable class and doesn't require all classes to inherit from SigC::Object
 *
 *      Revision 1.113  2005/10/12 23:28:31  erik
 *      2005-10-13  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* acinclude:m4, src/services/config/ConfigService.*, src/services/config/Makefile.am, src/framework/binreloc.*, src/framework/Makefile.am, src/framework/prefix.*, src/components/ogre/EmberOgre.cpp, src/components/ogre/Makefile.am: upgraded to binreloc 2.0
 *
 *      Revision 1.112  2005/10/12 21:08:06  erik
 *      2005-10-12  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* src/components/ogre/AvatarCamera.cpp, src/components/ogre/EmberEntityUserObject.h, src/components/ogre/EmberOgre.cpp, src/components/ogre/EmberPhysicalEntity.cpp: adapt to updated OgreOpcode
 *
 *      Revision 1.111  2005/10/09 23:28:16  erik
 *      2005-10-10  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* src/components/ogre/EmberOgre.*: changed how the request-for-quit functionality works. If a request for quit signal is emitted and not handled, the application will quit instantly.
 *
 *      Revision 1.110  2005/10/04 22:48:56  erik
 *      2005-10-05  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* src/components/ogre/EmberOgre.cpp, src/services/server/ServerService.cpp, src/components/ogre/EmberEntity.cpp. src/components/ogre/EmberEntityFactory.cpp, src/components/ogre/model/Model.cpp: cleaned up the logging calls
 *
 *      Revision 1.109  2005/10/03 19:16:54  erik
 *      2005-10-03  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* src/framework/services/logging/LogginService.h: fixed S_LOG_CRITICAL
 *      	* src/components/ogre/EmberOgre.cpp:
 *      		* log critical errors to the critical log
 *      		* hook the logobserver up to the ConfigService so it reacts on changes to the config
 *      	* ember.conf: added key for the logging level
 *
 *      Revision 1.108  2005/10/01 20:45:41  erik
 *      2005-10-01  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* src/components/ogre/EmbeOgre.cpp,  src/components/ogre/model/XMLModelDefinitionSerializer.cpp: moved some INFO log calls to VERBOSE log calls instead
 *      	* src/components/ogre/input/Input.h: cleaned up SigC includes
 *
 *      Revision 1.107  2005/09/25 16:24:39  erik
 *      2005-09-25  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* src/components/ogre/environment/FoliageArea.*,: don't return bool on frame update
 *      	* src/components/ogre/EmberOgre.cpp: don't delete ModelDefinitionManager. hopefully this will lead to no segfaults at application exit, but it's a mess
 *
 *      Revision 1.106  2005/09/24 16:33:27  erik
 *      2005-09-24  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* src/components/ogre/EmberOgre.cpp: use the new methods in ConfigService for getting the shared config directory
 *
 *      Revision 1.105  2005/09/23 23:45:15  erik
 *      2005-09-24  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* src/services/config/ConfigService.cpp: fixes for Apple and Win32
 *      	* src/components/ogre/TerrainShader.cpp: initialize the material to null, do'h
 *      	* src/components/ogre/EmberOgre.cpp:
 *      		* a lot of changes to make it work on win32 systems. Should not affect linux, but more testing needs to be done.
 *      		* now pass the log output from both ember and eris on to the ogre log system, ending up in Ogre.log
 *
 *      Revision 1.104  2005/09/20 22:43:48  erik
 *      2005-09-21  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* src/components/ogre/EmberOgre.cpp:
 *      		* fixed the setup of Jesus so it now looks in the right places
 *      		* turn the light bright on upon startup, so preview meshes and Carpenter constructions can be seen better
 *
 *      Revision 1.103  2005/09/19 21:38:59  erik
 *      2005-09-19  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	*  src/components/ogre/EmberOgre.*: added a couple of signals for signalling when different sub systems have been created or initialized
 *
 *      Revision 1.102  2005/09/18 23:57:03  erik
 *      2005-09-19  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* src/components/ogre/EmberOgre.*
 *      		* don't support sigc < 2.0 any more
 *      		* changed the name and function of the setting for dynamic tree generation; now assume that pregenerated trees will be used unless else specified
 *
 *      Revision 1.101  2005/09/11 22:08:44  erik
 *      2005-09-12  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* ember: added some debug output and
 *      	* src/components/ogre/EmberOgre.cpp: from resource.cfg, also parse the location of the media
 *      	* services/config/ConfigService.*: added methods for getting the user-specific and shared media
 *      	* src/components/ogre/resources.cfg: added indications where the media might be found (user or shared)
 *
 *      Revision 1.100  2005/09/11 11:43:45  erik
 *      2005-09-11  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* src/components/ogre/EmberOgre.cpp
 *      		* temporalily disable the sound service for now
 *
 *      Revision 1.99  2005/09/08 20:07:52  erik
 *      2005-09-08  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* src/components/ogre/EmberOgre.cpp
 *      		* initialize mEmberEntityFactory to 0
 *      		* in getWorld(), if no mEmberEntityFactory, return the root node of Ogre
 *      	* src/components/ogre/Makefile.am
 *      		* include terrain.cfg when installing
 *
 *      Revision 1.98  2005/07/25 19:07:40  erik
 *      2005-07-25  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* src/component/ogre/GUIManager.*
 *      		* cleaned up code
 *      		* added reference to StatusIconBar
 *      		* added new event for the changing of input modes
 *      	* src/components/ogre/EmberOgre.*
 *      		* added accessor for the AvatarController (this should be renamed to something else really, like just EntityController. Ah, the joys of subversion.)
 *
 *      Revision 1.97  2005/07/21 21:19:25  erik
 *      2005-07-21  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      		* src/components/ogre/EmberOgre.cpp
 *      			* set the icon of the window
 *      			* reactivated jesus
 *      			* changed NULL to 0
 *      		* src/components/ogre/EmberEntity.*
 *      			* changed NULL to 0
 *      			* moved some inline methods to the .cpp part
 *      			* cleaned out some unused ClickMasks
 *      		* src/components/ogre/EmberPhysicalEntity.cpp
 *      			* set the click mask to CM_ENTITY
 *      			* fixed the way the velocity for the entity is checked (to determine whether to show the forward or backwards walking animation)
 *      		* src/components/ogre/GUIManager.cpp
 *      			* changed back to just using one single imageset, since TaharezLook can't handle two different
 *      			* fixed an issue with the main sheet which wasn't raised as it should
 *      		* src/components/ogre/PersonEmberEntity.cpp
 *      			* don't set a click mask, we're trying to move away from the deep nesting of the entity class hierarcy
 *      		* src/components/ogre/TerrainGenerator.cpp
 *      			* correctly calculate the height of the world
 *      		* src/components/ogre/TerrainPage.cpp
 *      			* correct the placement of the terrain and the material, since it was a bit misaligned before
 *      		* src/components/ogre/WorldEmberEntity.cpp
 *      			* changed NULL to 0
 *      		* src/components/ogre/Avatar.*
 *      			* don't send the scenemanager in the constructor since we now get it through EmberOgre instead
 *      			* set more correct walk and run speeds
 *      			* don't create a dummy model on startup, wait for the world connection
 *      			* added some debug output for the current location and velocity
 *      		* src/components/ogre/AvatarController.*
 *      			* changed NULL to 0
 *      			* start with a detached camera, there are still some bugs though
 *      			* made movement with the detached camera a little easier
 *      		* src/components/ogre/AvatarCamera.*
 *      			* started adding functionality for preventing the camera to dip below the terrain, needs a bit more work though
 *      			* fixed the pickObject method which hadn't worked since the switch to the paging scene manager
 *
 *      Revision 1.96  2005/07/13 19:30:33  erik
 *      2005-07-13  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* src/components/ogre/EmberOgre.cpp, src/components/ogre/widgets/JesusEdit.cpp
 *      		* use the new Jesus serializer class
 *
 *      Revision 1.95  2005/07/11 00:02:30  erik
 *      2005-07-11  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* src/components/ogre/EmberOgre.*:
 *      		* use EmberPagingSceneManager
 *      		* initialize some member to 0
 *
 *      Revision 1.94  2005/06/26 21:18:13  erik
 *      2005-06-26  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* src/components/ogre/EmberOgre.cpp
 *      		*added more error logging for errors in the Eris::Poll section
 *      		* try to make the scenemanager shut down cleanly so to avoid a segfault on exit
 *
 *      Revision 1.93  2005/06/25 11:02:46  erik
 *      2005-06-25  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* src/components/ogre/EmberOgre.*
 *      		* added method for getting a random EmberEntity from the world (just like Eris::View::getEntity but with a cast to EmberEntity)
 *
 *      Revision 1.92  2005/06/23 11:29:51  erik
 *      2005-06-23  Erik Hjortsberg  <erik@erik>
 *
 *      	* src/components/ogre/EmberOgre.*
 *      		* added accessor for the main View and made some accessors const
 *
 *      Revision 1.91  2005/06/20 23:22:10  erik
 *      2005-06-21  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	*  src/components/ogre/EmberOgre.cpp
 *      		* create the CollisionManager on init
 *
 *      Revision 1.90  2005/06/16 18:33:22  erik
 *      2005-06-16  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* src/components/ogre/EmberEntity.* src/components/ogre/Acatar.cpp, src/components/ogre/AvatarEmberEntity.cpp, src/components/ogre/EmberPhysicalEntity.*, MotionManager.*, src/components/ogre/EmberOgre.cpp
 *      		* reworked how animations are played and how motion is handled, instead of the MotionManager being tied direclty to Model and automatically updating the animations and motions, they are now more decoupled with EmberEntity and its subclasses acting as mediators. Motion prediction is now fully delegated to Eris (which works splendidly, great work James!), with the MotionManager only keeping track of which entities needs moving.
 *
 *      Revision 1.89  2005/06/12 01:08:19  erik
 *      2005-06-12  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	*  src/components/ogre/EmberOgre.cpp
 *      		* catch exceptions when adding resources
 *
 *      Revision 1.88  2005/06/11 22:57:09  erik
 *      2005-06-12  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* src/components/ogre/EmberOgre.cpp, src/components/ogre/environment/Sky.cpp
 *      		* moved fog settings from EmberOgre to Sky
 *
 *      Revision 1.87  2005/06/01 00:23:35  erik
 *      2005-06-01  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* src/components/ogre/EmberOgre.*
 *      		* added method for random access to entities in the main Eris::View
 *
 *      Revision 1.86  2005/05/16 23:25:15  erik
 *      2005-05-17  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* src/components/ogre/Avatar.cpp, src/components/ogre/EmberEntityFactory.cpp, src/components/ogre/EmberOgre.cpp: updates for new ModelDefinitions system
 *      	* src/components/ogre/Makefile.am : rearrange libs
 *
 *      Revision 1.85  2005/05/09 18:41:22  erik
 *      2005-05-09  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* src/components/ogre/EmberOgre.*
 *      		* allow command line switches, mostly used for defining whether ogre plugins should be loaded through binreloc or ~/.ember/plugins.cfg. Code shamelessly taken from Sear, thanks to all Sear developers.
 *
 *      Revision 1.84  2005/05/08 20:01:17  erik
 *      2005-05-08  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* src/components/ogre/EmberOgre.cpp:
 *      		* make it work with Eris 1.3.5
 *
 *      Revision 1.83  2005/05/07 22:24:07  erik
 *      2005-05-08  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	*src/components/ogre/EmberEntityFactory.cpp, src/components/ogre/EmberEntityFactory.h, src/components/ogre/AvatarCamera.h, src/components/ogre/AvatarCamera.cpp, src/components/ogre/AvatarController.cpp, src/components/ogre/EmberEntity.h, src/components/ogre/EmberOgrePrerequisites.h, src/components/ogre/MathConverter.h, src/components/ogre/model/ModelDefinition.h, src/components/ogre/model/ModelDefinitionManager.cpp, src/components/ogre/model/SubModel.cpp, src/components/ogre/jesus/Jesus.cpp, src/components/ogre/environment/Tree.cpp, src/components/ogre/environment/Water.cpp, src/components/ogre/environment/meshtree/MeshTree.cpp, src/components/ogre/environment/meshtree/TParameters.cpp, src/components/ogre/environment/meshtree/TStem.cpp, src/components/ogre/EmberSceneManager/src/*.cpp, src/components/ogre/EmberSceneManager/include/OgreTerrainRenderable.h:
 *      		* removed Ogre memory macros since we're not using the ogre memory manager anyway
 *      		* cleaned up old unused code
 *
 *      	*src/components/ogre/EmberOgre.cpp:
 *      		* removed references to EntityListener
 *      		* removed references to MetaServerService
 *      		* removed Ogre memory macros since we're not using the ogre memory manager anyway
 *      		* cleaned up include directives
 *
 *      	*src/components/ogre/Avatar.cpp, src/components/ogre/Avatar.h:
 *      		* pass MovementUpdates as references
 *      		* only update the orientation of the avatar when needed
 *      		* removed Ogre memory macros since we're not using the ogre memory manager anyway
 *
 *      Revision 1.82  2005/05/03 19:38:16  erik
 *      2005-05-03  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* src/components/ogre/EmberOgre.cpp:
 *      		* use the S_LOG_* macros
 *      		* cleaned up some code
 *
 *      Revision 1.81  2005/04/24 12:42:32  erik
 *      2005-04-24  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* src/components/ogre/EmberOgre:
 *      		enable loading of the plugin.cfg file for now, we'll have to add a command line switch or something
 *      		check for ".modeldef" when loading modeldefinitions
 *      		Use OgreNoMemoryMacros.h, though this seems to be in vain. One still gets linking errors with the Ogre debug memory manager, forcing one to disable the Ogre MemoryManager in OgreConfig.h.
 *      	* src/components/ogre/EmberEntityFactory.*,  src/components/ogre/EmberOgrePrerequisites.h
 *      		Use OgreNoMemoryMacros.h, though this seems to be in vain. One still gets linking errors with the Ogre debug memory manager, forcing one to disable the Ogre MemoryManager in OgreConfig.h.
 *      		check for config.h
 *
 *      Revision 1.80  2005/04/10 23:11:23  erik
 *      2005-04-11  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* removed unecessary include from GUIManager.h
 *      	* add binreloc support to EmberOgre.cpp
 *
 *      Revision 1.79  2005/03/31 21:17:35  erik
 *      2005-03-31  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* Use the S_LOG* macros.
 *      	* Name the window "Ember"
 *      	* Commented out the sound test where it tries to open /dev/dsp since that made Ember lock up completely on my box when something else was accessing the sound device.
 *
 *      Revision 1.78  2005/03/20 20:21:26  aglanor
 *      2005-03-20 Miguel Guzman (Aglanor)
 *
 *              * added back Sound Service and reenabled the playsound command
 *      	        (type /playsound for a test)
 *
 *      Revision 1.77  2005/03/20 15:40:45  erik
 *      2005-03-20  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* EmberOgre.h: added convenience getRootSceneNode
 *      	* EmberOgre.h: added and cleaned up comments
 *      	* EmberOgre.cpp: setupJesus now loads all blockspecs, building blocks, modeldefinitions and blueprints (both global and user)
 *      	* Jesus added methods for saving blueprints, these will be saved to ~/.ember/carpenter/blueprints/
 *      	* Jesus now loads and holds all blueprints
 *      	* BluePrint.cpp, BluePrint.h: make BuildingBlockBinding all const
 *      	* made sure all blueprints have names
 *      	* JesusEdit: added ability to save and load blueprints. This is very temporary and should use a better method and GUI.
 *      	* MakeEntityWidget: added null checks
 *      	* Widget.h: added comment
 *
 *      Revision 1.76  2005/03/09 21:44:24  erik
 *      2005-03-09  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* TerrainPage.cpp: removed debug image output code
 *      	* TerrainPage.cpp: made MemoryDataStreams pass by pointer instead of reference
 *      	* JesusEdit: show a construction object in the preview instead of just a model (this currently segfaults
 *      	* removed path to shared dirs
 *      	* added path to splatting materials
 *
 *      Revision 1.75  2005/03/04 01:14:05  erik
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
 *      	* Removed bug where images were written to a non-existant directory, resulting in strange extern template allocation problems. Ugly one.
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
#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#ifdef WIN32
	#include <tchar.h>
	#define snprintf _snprintf
    #include <io.h> // for _access, Win32 version of stat()
    #include <direct.h> // for _mkdir
//	#include <sys/stat.h>

	// Necessary to get the Window Handle of the window
	//  Ogre created, so SDL can grab its input.
	#include <windows.h>
	#include <SDL_getenv.h>
	#include <SDL.h>
	#include <SDL_syswm.h>

	#include <iostream>
	#include <fstream>
	#include <ostream>
#else
	#include <dirent.h>
	#include <SDL/SDL_image.h>
#endif

// ------------------------------
// Include Eris header files
// ------------------------------
#include <Eris/PollDefault.h>


#include "EmberOgrePrerequisites.h"

//Ember headers
#include "services/EmberServices.h"
#include "services/logging/LoggingService.h"
#include "services/server/ServerService.h"
#include "services/config/ConfigService.h"
#include "services/metaserver/MetaserverService.h"
//#include "services/sound/SoundService.h"
#include "services/scripting/ScriptingService.h"
#include "framework/ConsoleBackend.h"
#include "framework/ConsoleObject.h" //TODO: this will be included in a different class
#include "framework/binreloc.h" //this is needed for binreloc functionality


// ------------------------------
// Include OGRE Ember client files
// ------------------------------
#include "TerrainGenerator.h"


#include "ConsoleObjectImpl.h"
#include "Avatar.h"
#include "AvatarController.h"
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

#include "ogreopcode/include/OgreCollisionManager.h"

//#include "EmberSceneManager/include/EmberTerrainSceneManager.h"
#include "SceneManagers/EmberPagingSceneManager/include/EmberPagingSceneManager.h"
#include "model/ModelDefinitionManager.h"
#include "model/ModelDefinition.h"

// ------------------------------
// Include Ember header files
// ------------------------------

#include "framework/ConsoleBackend.h"


#include "EmberOgre.h"

#include "jesus/Jesus.h"
#include "jesus/XMLJesusSerializer.h"

#include "framework/osdir.h"

#include "framework/Exception.h"


template<> EmberOgre::EmberOgre* Ember::Singleton<EmberOgre::EmberOgre>::ms_Singleton = 0;

namespace EmberOgre {

	void assureConfigFile(std::string filename, std::string originalConfigFileDir)
	{
		struct stat tagStat;
		int ret = stat( filename.c_str(), &tagStat );
		if (ret == -1) {
			ret = stat( (originalConfigFileDir +filename).c_str(), &tagStat );
			if (ret == 0) {
				//copy conf file from shared
				std::ifstream  instream ((originalConfigFileDir + filename).c_str());
				std::ofstream  outstream (filename.c_str()); 
				outstream <<  instream.rdbuf();
			}
		}

	}


	/**
	A log observer which writes to the Ogre log system.
	*/
	class OgreLogObserver: public Ember::LoggingService::Observer, 
	public sigc::trackable
	{
		public:
			OgreLogObserver()
			{
				Ember::EmberServices::getSingletonPtr()->getConfigService()->EventChangedConfigItem.connect(sigc::mem_fun(*this, &OgreLogObserver::ConfigService_EventChangedConfigItem));
			
			}
			
			~OgreLogObserver()
			{
				
			}

			virtual void onNewMessage(const std::string & message, const std::string & file, const int & line,
									const Ember::LoggingService::MessageImportance & importance, const time_t & timeStamp)
			{
				if (!Ogre::LogManager::getSingletonPtr()) {
					return;
				}
				std::string stringImportance;
				switch (importance) {
					case Ember::LoggingService::CRITICAL:
						stringImportance = "CRITICAL";
						break;
					case Ember::LoggingService::FAILURE:
						stringImportance = "FAILURE";
						break;
					case Ember::LoggingService::WARNING:
						stringImportance = "WARNING";
						break;
					case Ember::LoggingService::INFO:
						stringImportance = "INFO";
						break;
					default:
						stringImportance = "VERBOSE";
						break;
						

				}
				std::stringstream ss;
				ss << "Ember (" << stringImportance << "): " << message << " (in file " << file << ":" << line << ")";
				Ogre::LogManager::getSingleton().logMessage(ss.str());
			}
			
		protected:
			
			/**
			 * Updates from the config. The relevant section is "general" and the key "logginglevel". It can have the values of verbose|info|warning|failure|critical
			 */
			void updateFromConfig()
			{
				if (Ember::EmberServices::getSingletonPtr()->getConfigService()->itemExists("general", "logginglevel")) {
					std::string loggingLevel = static_cast<std::string>(Ember::EmberServices::getSingletonPtr()->getConfigService()->getValue("general", "logginglevel"));
					Ember::LoggingService::MessageImportance importance;
					if (loggingLevel == "verbose") {
						importance = Ember::LoggingService::VERBOSE;
					} else if (loggingLevel == "info") {
						importance = Ember::LoggingService::INFO;
					} else if (loggingLevel == "warning") {
						importance = Ember::LoggingService::WARNING;
					} else if (loggingLevel == "failure") {
						importance = Ember::LoggingService::FAILURE;
					} else if (loggingLevel == "critical") {
						importance = Ember::LoggingService::CRITICAL;
					}
					setFilter(importance);
				}
			}
			
			
			/**
			 *          React on changes to the config.
			 * @param section 
			 * @param key 
			 */
			void ConfigService_EventChangedConfigItem(const std::string& section, const std::string& key)
			{
				if (section == "general") {
					if (key == "logginglevel") {
						updateFromConfig();
					}
				}
			}
			
	};

// // TODO: move CerrLogObserver to its own class (under Logging service, or under Framework)
//   class CerrLogObserver: public Ember::LoggingService::Observer
//     {
//     public:
//         CerrLogObserver()
//         {
//         }
// 
//         virtual void onNewMessage(const std::string & message, const std::string & file, const int & line,
//                                   const Ember::LoggingService::MessageImportance & importance, const time_t & timeStamp)
//         {
//             tm * ctm = localtime(&timeStamp); //currentLocalTime was too long, sorry
// 		
// 	    std::cerr.fill('0');
//             std::cerr << "[";
// 	    std::cerr.width(2);
// 	    std::cerr << (ctm->tm_year/*+1900*/)%100 << "-";
// 	    std::cerr.width(2);
// 	    std::cerr << ctm->tm_mon+1 << "-";
// 	    std::cerr.width(2);
// 	    std::cerr << ctm->tm_mday << " ";
// 	    std::cerr.width(2);
// 	    std::cerr << ctm->tm_hour << ":";
// 	    std::cerr.width(2);
// 	    std::cerr <<  ctm->tm_min << ":";
// 	    std::cerr.width(2);
// 	    std::cerr << ctm->tm_sec << "] ";
// 	    std::cerr  << "[File: " << file << ", Line #:" <<  line << "] (";
// 
//             if(importance == Ember::LoggingService::CRITICAL)
//                 {
//                     std::cerr << "CRITICAL";
//                 }
//             else  if(importance == Ember::LoggingService::FAILURE)
//                 {
//                     std::cerr << "FAILURE";
//                 }
//             else if(importance == Ember::LoggingService::WARNING)
//                 {
//                     std::cerr << "WARNING";
//                 }
//             else if(importance == Ember::LoggingService::INFO)
//                 {
//                     std::cerr << "INFO";
//                 }
// 	    else
//                 {
//                     std::cerr << "VERBOSE";
//                 }
//             std::cerr << ") " <<message << std::endl;
//         }
// 
//     private:
// 
//     };
    

EmberOgre::EmberOgre() :
mRoot(0),
mKeepOnRunning(true),
mWorldView(0),
mGUIManager(0),
mTerrainGenerator(0),
mMotionManager(0),
mAvatarController(0),
mModelDefinitionManager(0),
mEmberEntityFactory(0)
{}

EmberOgre::~EmberOgre()
{
//	mSceneMgr->shutdown();
		delete mWorldView;
		//mSceneMgr->removeAllCameras();
//		mSceneMgr->clearScene();
		delete mGUIManager;
		delete mTerrainGenerator;
		delete mMotionManager;
/*	if (mAvatar)
		delete mAvatar;*/
		delete mAvatarController;
//		delete mModelDefinitionManager;
/*	if (mEmberEntityFactory)
		delete mEmberEntityFactory;*/
//		delete mRoot;
		
		
		

}


bool EmberOgre::frameStarted(const Ogre::FrameEvent & evt)
{
	EventStartErisPoll.emit();
	try {
		Eris::PollDefault::poll(1);
	} catch (const Ember::Exception& ex) {
		S_LOG_CRITICAL(ex.getError());
		throw ex;
	} catch (const Ogre::Exception& ex) {
		S_LOG_CRITICAL(ex.getFullDescription());
		throw ex;
/*	} catch (const CEGUI::Exception& ex) {
		S_LOG_CRITICAL(ex.getMessage());
		throw ex;*/
	} catch (const std::exception& ex)
	{
		S_LOG_CRITICAL("Got exception, shutting down. " << ex.what());
		throw ex;
	} catch (...)
	{
		S_LOG_CRITICAL("Got unknown exception.");
	}
	if (mWorldView)
		mWorldView->update();
	EventEndErisPoll.emit();
	
	
	if (!mKeepOnRunning)
		S_LOG_INFO( "Shutting down Ember.");
	return mKeepOnRunning;
}


void EmberOgre::go(bool loadOgrePluginsThroughBinreloc)
{
	if (!setup(loadOgrePluginsThroughBinreloc))
		return;

// 	try {
	try {
		mRoot->startRendering();
	} catch (const Ember::Exception& ex) {
		S_LOG_CRITICAL(ex.getError());
		throw ex;
	} catch (const Ogre::Exception& ex) {
		S_LOG_CRITICAL(ex.getFullDescription());
		throw ex;
/*	} catch (const CEGUI::Exception& ex) {
		S_LOG_CRITICAL(ex.getMessage());
		throw ex;*/
	} catch (const std::runtime_error& ex)
	{
		S_LOG_CRITICAL("Got exception, shutting down. " << ex.what());
		throw ex;
	} catch (...)
	{
		S_LOG_CRITICAL("Got unknown exception,");
	}
// 	} catch (Ogre::Exception e) {
// 		std::cerr << "Error in Ogre: !\n";
// 	}

}

void EmberOgre::shutdown()
{
	mKeepOnRunning = false;
}

void EmberOgre::requestQuit()
{
	bool handled = false;
	EventRequestQuit.emit(handled);
	//check it was handled
	if (!handled) {
		//it's not handled, quit now
		shutdown();
	}

}


    
// These internal methods package up the stages in the startup process
/** Sets up the application - returns false if the user chooses to abandon configuration. */
bool EmberOgre::setup(bool loadOgrePluginsThroughBinreloc)
{
	
	Ember::ConfigService* configSrv = Ember::EmberServices::getSingletonPtr()->getConfigService();

	checkForConfigFiles();
	
#ifdef __WIN32__
		mRoot = new Ogre::Root("plugins.cfg", "ogre.cfg", "ogre.log");
#else
	if (loadOgrePluginsThroughBinreloc) {
		char* br_libdir = br_find_lib_dir(br_strcat(PREFIX, "/lib"));
		std::string libDir(br_libdir);
		free(br_libdir);
		mRoot = new Ogre::Root("", "ogre.cfg", "ogre.log");
		mRoot->loadPlugin(libDir + "/ember/OGRE/Plugin_CgProgramManager.so");
		mRoot->loadPlugin(libDir + "/ember/OGRE/Plugin_ParticleFX.so");
		mRoot->loadPlugin(libDir + "/ember/OGRE/RenderSystem_GL.so");
	} else {
		mRoot = new Ogre::Root("plugins.cfg", "ogre.cfg", "ogre.log");
	}
#endif

	
    setupResources();

    bool carryOn = configure();
    if (!carryOn) return false;

    chooseSceneManager();
	
	new OgreOpcode::CollisionManager(mSceneMgr);
	
	mModelDefinitionManager->setSceneManager(mSceneMgr);

    // Set default mipmap level (NB some APIs ignore this)
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    
    // Set default animation mode
	Ogre::Animation::setDefaultInterpolationMode(Ogre::Animation::IM_SPLINE);

	//remove padding for bounding boxes
    Ogre::MeshManager::getSingletonPtr()->setBoundsPaddingFactor(0);    
	
// 	//add ourself as a frame listener
	Ogre::Root::getSingleton().addFrameListener(this);

 	S_LOG_INFO( "Begin preload.");
 	preloadMedia();
 	S_LOG_INFO( "End preload.");
	
	mGUIManager = new GUIManager(mWindow, mSceneMgr);
	EventGUIManagerCreated.emit(*mGUIManager);
    
	

	chdir(configSrv->getHomeDirectory().c_str());
	
	// Avatar
	mAvatar = new Avatar();
	
	mAvatarController = new AvatarController(mAvatar, mWindow, mGUIManager);
	EventAvatarControllerCreated.emit(*mAvatarController);
	
	mTerrainGenerator = new TerrainGenerator();
	EventTerrainGeneratorCreated.emit(*mTerrainGenerator);
	mMotionManager = new MotionManager();
	mMotionManager->setTerrainGenerator(mTerrainGenerator);
	EventMotionManagerCreated.emit(*mMotionManager);
	
//	mSceneMgr->setPrimaryCamera(mAvatar->getAvatarCamera()->getCamera());





	mRoot->addFrameListener(mMotionManager);
	new ConsoleObjectImpl();


	mGUIManager->initialize();
	EventGUIManagerInitialized.emit(*mGUIManager);
	
	// Create the scene
    createScene();
	EventSceneCreated.emit();
	
	//this should be in a separate class, a separate plugin even
	setupJesus();
    
	return true;
  
}
/** Configures the application - returns false if the user chooses to abandon configuration. */
bool EmberOgre::configure(void)
{
#ifndef __WIN32__
	if (dlopen("libSDL_image-1.2.so.0", RTLD_NOW)) {
		//set the icon of the window
		char* br_datadir = br_find_data_dir(br_strcat(PREFIX, "/share"));
		
		const char* iconPath = br_strcat(br_datadir,"/icons/worldforge/ember.png");
		free(br_datadir);
		SDL_WM_SetIcon(IMG_Load(iconPath), 0);
	} else {
		std::cerr << dlerror() << "\n";
	}
#endif


//for non-windows systems don't show any config option
#ifndef __WIN32__
	bool success = mRoot->restoreConfig();
#else
	//but do for windows. We need a better way to do this though
	bool success = mRoot->showConfigDialog();
#endif    
	if(success)
    {
		//this will only apply on DirectX
		//it will force DirectX _not_ to set the FPU to single precision mode (since this will mess with mercator amongst others)
		try {
			mRoot->getRenderSystem()->setConfigOption("Video Mode", "1024 x 768 @ 32-bit colour");
			mRoot->getRenderSystem()->setConfigOption("Floating-point mode", "Consistent");
			
		} catch (const Ogre::Exception&) 
		{
			//we don't know what kind of render system is used, so we'll just swallow the error since it doesn't affect anything else than DirectX
		}

        // If returned true, user clicked OK so initialise
        // Here we choose to let the system create a default rendering window by passing 'true'
        mWindow = mRoot->initialise(true, "Ember");
#if __WIN32__
   //do some FPU fiddling, since we need the correct settings for stuff like mercator (which uses fractals etc.) to work
   	_fpreset();
	_controlfp(_PC_64, _MCW_PC);
	_controlfp(_RC_NEAR , _MCW_RC);

   // Allow SDL to use the window Ogre just created

   // Old method: do not use this, because it only works
   //  when there is 1 (one) window with this name!
   // HWND hWnd = FindWindow(tmp, 0);

   // New method: As proposed by Sinbad.
   //  This method always works.
   HWND hWnd;
   mWindow->getCustomAttribute("HWND", &hWnd);
	
   char tmp[64];
   // Set the SDL_WINDOWID environment variable
   sprintf(tmp, "SDL_WINDOWID=%d", hWnd);
   putenv(tmp);

   if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE) < 0)
    {
      S_LOG_FAILURE("Couldn't initialize SDL:\n\t\t");
      S_LOG_FAILURE(SDL_GetError());
    }

      // if width = 0 and height = 0, the window is fullscreen

      // This is necessary to allow the window to move
      //  on WIN32 systems. Without this, the window resets
      //  to the smallest possible size after moving.
      SDL_SetVideoMode(mWindow->getWidth(), mWindow->getHeight(), 0, 0); // first 0: BitPerPixel, 
                                             // second 0: flags (fullscreen/...)
                                             // neither are needed as Ogre sets these

   static SDL_SysWMinfo pInfo;
   SDL_VERSION(&pInfo.version);
   SDL_GetWMInfo(&pInfo);

   // Also, SDL keeps an internal record of the window size
   //  and position. Because SDL does not own the window, it
   //  missed the WM_POSCHANGED message and has no record of
   //  either size or position. It defaults to {0, 0, 0, 0},
   //  which is then used to trap the mouse "inside the 
   //  window". We have to fake a window-move to allow SDL
   //  to catch up, after which we can safely grab input.
   RECT r;
   GetWindowRect(pInfo.window, &r);
   SetWindowPos(pInfo.window, 0, r.left, r.top, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

   //do some FPU fiddling, since we need the correct settings for stuff like mercator (which uses fractals etc.) to work
   	_fpreset();
	_controlfp(_PC_64, _MCW_PC);
	_controlfp(_RC_NEAR , _MCW_RC);

#endif


		
		return true;
    }
    else
    {
        return false;
    }
}

void EmberOgre::chooseSceneManager(void)
{
	
	EmberPagingSceneManager* sceneManager = new EmberPagingSceneManager();
	mRoot->setSceneManager(Ogre::ST_EXTERIOR_REAL_FAR, sceneManager);
	mSceneMgr = static_cast<EmberPagingSceneManager*>(mRoot->getSceneManager(Ogre::ST_EXTERIOR_REAL_FAR));
	sceneManager->InitScene();
	
 
//     // We first register our own scenemanager
//     EmberTerrainSceneManager* sceneManager = new EmberTerrainSceneManager();
//     mRoot->setSceneManager(Ogre::ST_EXTERIOR_FAR, sceneManager);
//     //And then request it
//     mSceneMgr = static_cast<EmberTerrainSceneManager*>(mRoot->getSceneManager(Ogre::ST_EXTERIOR_FAR));
//     
//     EmberTerrainSceneManager* mEmberTerr = dynamic_cast<EmberTerrainSceneManager*>(mSceneMgr);
//     assert(mEmberTerr);

   //mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
   
    
}

EmberEntity* EmberOgre::getEmberEntity(const std::string & eid) const
{
	assert(mWorldView);
	return static_cast<EmberEntity*>(mWorldView->getEntity(eid));
}


void EmberOgre::checkForConfigFiles()
{
	chdir(Ember::EmberServices::getSingletonPtr()->getConfigService()->getHomeDirectory().c_str());

	std::string sharePath(Ember::EmberServices::getSingletonPtr()->getConfigService()->getSharedConfigDirectory());

	//make sure that there are files 
	assureConfigFile("ogre.cfg", sharePath);
	assureConfigFile("resources.cfg", sharePath);
	assureConfigFile("plugins.cfg", sharePath);
	assureConfigFile("terrain.cfg", sharePath);
}

void EmberOgre::getResourceArchiveFromVarconf(Ogre::ResourceManager* manager, std::string variableName, std::string section, std::string type)
{
// 	if (Ember::EmberServices::getSingletonPtr()->getConfigService()->itemExists(section, variableName)) {
// 		std::string value =  Ember::EmberServices::getSingletonPtr()->getConfigService()->getHomeDirectory() + std::string(Ember::EmberServices::getSingletonPtr()->getConfigService()->getValue(section, variableName)) + "/";
// 		manager->addArchiveEx(value, type);
// 	} else {
// 		//throw new Exception(std::string("Could not find setting: ") + variableName + " in section " + section + ".");
// 	}

} 

/// Method which will define the source of resources (other than current folder)
void EmberOgre::setupResources(void)
{
 	Ember::ConfigService* configSrv = Ember::EmberServices::getSingletonPtr()->getConfigService();
	bool loadRecursive = false;
	
	///check from the config if we should load media recursively
	///this is needed for most authoring, since it allows us to find all meshes before they are loaded
	if (configSrv->itemExists("general", "loadmediarecursive")) { 
			loadRecursive = (bool)configSrv->getValue("general", "loadmediarecursive");
	}
	

	chdir(Ember::EmberServices::getSingletonPtr()->getConfigService()->getHomeDirectory().c_str());
		

	
	std::string userMediaPath = Ember::EmberServices::getSingletonPtr()->getConfigService()->getUserMediaDirectory();
	std::string sharedMediaPath = Ember::EmberServices::getSingletonPtr()->getConfigService()->getSharedMediaDirectory();
	
	mModelDefinitionManager = new Model::ModelDefinitionManager();
	
	//Ogre::ResourceGroupManager::getSingleton().addResourceLocation(mediaHomePath + "modeldefinitions", "FileSystem", "modeldefinitions");
	try {
	
		if (configSrv->itemExists("tree", "usedynamictrees") && ((bool)configSrv->getValue("tree", "usedynamictrees"))) { 
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(userMediaPath + "modeldefinitions/trees/dynamic", "FileSystem", "ModelDefinitions");
		} else {
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(userMediaPath + "modeldefinitions/trees/pregenerated", "FileSystem", "ModelDefinitions");
		}
	} catch (const Ogre::Exception& ) {
		S_LOG_FAILURE("Couldn't load trees. Continuing as if nothing happened.");
	}
	
// /*	std::string modeldefspath = "modeldefinitions/";
// 	if (Ember::EmberServices::getSingletonPtr()->getConfigService()->itemExists("ogre", "modeldefinitionpath")) {
// 		modeldefspath =  Ember::EmberServices::getSingletonPtr()->getConfigService()->getHomeDirectory() + std::string(Ember::EmberServices::getSingletonPtr()->getConfigService()->getValue("ogre", "modeldefinitionpath")) + "/";
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
	//first do user media, then ember media, allowing the user to override ember-media
	{
		std::string userOwnMediaPath = Ember::EmberServices::getSingletonPtr()->getConfigService()->getHomeDirectory() + "/user-media/";
	
		Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
		
		Ogre::String secName, typeName, archName, fullResourcePath;
		std::string finalTypename;
		while (seci.hasMoreElements())
		{
			secName = seci.peekNextKey();
			Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
			Ogre::ConfigFile::SettingsMultiMap::iterator i;
			for (i = settings->begin(); i != settings->end(); ++i)
			{
				typeName = i->first;
				archName = i->second;
				if (Ogre::StringUtil::endsWith(typeName, "[shared]")) {
					//skip shared
				} else {
					fullResourcePath = userOwnMediaPath + archName;
					finalTypename = typeName.substr(0, typeName.find("["));
					try {
						Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
							fullResourcePath, finalTypename, secName, loadRecursive);
					} catch (const Ogre::Exception&) {
						S_LOG_FAILURE("Couldn't load " + fullResourcePath + ". Continuing as if nothing happened.");
					}
				}
			}
		}
	}
	
	{
		Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
		
		Ogre::String secName, typeName, archName, fullResourcePath;
		std::string finalTypename;
		while (seci.hasMoreElements())
		{
			secName = seci.peekNextKey();
			Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
			Ogre::ConfigFile::SettingsMultiMap::iterator i;
			for (i = settings->begin(); i != settings->end(); ++i)
			{
				typeName = i->first;
				archName = i->second;
				if (Ogre::StringUtil::endsWith(typeName, "[shared]")) {
					fullResourcePath = sharedMediaPath + archName;
				} else {
					fullResourcePath = userMediaPath + archName;
				}
				finalTypename = typeName.substr(0, typeName.find("["));
				try {
					Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
						fullResourcePath, finalTypename, secName, loadRecursive);
				} catch (const Ogre::Exception&) {
					S_LOG_FAILURE("Couldn't load " + fullResourcePath + ". Continuing as if nothing happened.");
				}
			}
		}
	}
	
	
//     Ogre::ResourceManager::addCommonArchiveEx( Ember::EmberServices::getSingletonPtr()->getConfigService()->getSharedDataDirectory() + "media/", "FileSystem");
	
	S_LOG_INFO(  "Added media paths.");
	
}



void EmberOgre::preloadMedia(void)
{
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	Ember::ConfigService* configSrv = Ember::EmberServices::getSingletonPtr()->getConfigService();
	

	std::vector<std::string> shaderTextures;
	
	shaderTextures.push_back(std::string(configSrv->getValue("shadertextures", "rock")));
	shaderTextures.push_back(std::string(configSrv->getValue("shadertextures", "sand")));
	shaderTextures.push_back(std::string(configSrv->getValue("shadertextures", "grass")));
	
	for (std::vector<std::string>::iterator I = shaderTextures.begin(); I != shaderTextures.end(); ++I) {
		try {
			Ogre::TextureManager::getSingleton().load(*I, "General");
		} catch (const Ogre::Exception& e) {
			S_LOG_FAILURE( "Error when loading texture " << *I )
		}
	}	
	  
	
	//TODO: use C++ io methods
// 	DIR *dp;
// 	struct dirent *ep;
// 	
// 	std::string modeldefDir = Ember::EmberServices::getSingletonPtr()->getConfigService()->getEmberDataDirectory() + "media/modeldefinitions";
// 	dp = opendir (modeldefDir.c_str());
// 	if (dp != NULL)
// 	{
// 		while (ep = readdir (dp)) {
// 			std::string filename(ep->d_name);
// 			if (ep->d_name != "." && ep->d_name != ".." && filename.find(".modeldef")) {
// 				try {
// 					S_LOG_INFO(  "TRACE - PRELOADING: "<< ep->d_name);
// 					ModelDefinitionPtr modeldef = mModelDefinitionManager->load(ep->d_name, "modeldefinitions");
// 				} catch (Ogre::Exception ex)
// 				{
// 					S_LOG_FAILURE( "TRACE - ERROR PRELOADING: " <<ep->d_name );
// 				}
// 			}
// 		}
// 		(void) closedir (dp);
// 	}
	
	//only autogenerate trees if we're not using the pregenerated ones
 	if (configSrv->itemExists("tree", "usedynamictrees") && ((bool)configSrv->getValue("tree", "usedynamictrees"))) { 
		Tree tree;
		tree.makeMesh("GeneratedTrees/European_Larch", Ogre::TParameters::European_Larch);
		tree.makeMesh("GeneratedTrees/Fir", Ogre::TParameters::Fir);
 	}	




}

void EmberOgre::setupJesus()
{
	const std::string datadir = Ember::EmberServices::getSingletonPtr()->getConfigService()->getSharedDataDirectory();

	Carpenter::Carpenter* carpenter = new Carpenter::Carpenter();
	mJesus = new Jesus(carpenter);
	XMLJesusSerializer serializer(mJesus);

	std::string dir;
	dir = Ember::EmberServices::getSingletonPtr()->getConfigService()->getSharedDataDirectory() + "carpenter/blockspec";

	std::string filename;

	//oslink::directory needs to be destroyed before a new one can be used, regular copy constructor doesn't seem to work
	//we could also use new/delete, but scopes works as well
	{
		oslink::directory osdir(dir);
		while (osdir) {
			filename = osdir.next();
			S_LOG_VERBOSE( "Loading blockspec: " << filename );
			serializer.loadBlockSpec(dir + "/" + filename);
		}
	}
	//load all buildingblockspecs
	dir = Ember::EmberServices::getSingletonPtr()->getConfigService()->getSharedDataDirectory() + "carpenter/modelblockspecs";
		{
		oslink::directory osdir(dir);
		while (osdir) {
			filename = osdir.next();
			S_LOG_VERBOSE( "Loading buildingblockspecC: " << filename);
			serializer.loadBuildingBlockSpecDefinition(dir + "/" + filename);
		}
	}
	//load all modelmappings
	dir = Ember::EmberServices::getSingletonPtr()->getConfigService()->getSharedDataDirectory() + "jesus/modelmappings";
	{
		oslink::directory osdir(dir);
		while (osdir) {
			filename = osdir.next();
			S_LOG_VERBOSE( "Loading modelmapping: " <<  filename );
			serializer.loadModelBlockMapping(dir + "/" + filename);
		}
	}
	
	//load all global blueprints
	dir = Ember::EmberServices::getSingletonPtr()->getConfigService()->getSharedDataDirectory() + "carpenter/blueprints";
	{
		oslink::directory osdir(dir);
		while (osdir) {
			filename = osdir.next();
			S_LOG_VERBOSE(  "Loading blueprint: " << filename );
			Carpenter::BluePrint* blueprint = serializer.loadBlueprint(dir + "/" + filename);
			if (blueprint) {
				blueprint->compile();
				bool result = mJesus->addBluePrint(blueprint);
				if (!result)
				{
					S_LOG_FAILURE( "Could not add blueprint: " << filename);
				}
			}
		}
	}
	//load all local blueprints
	dir = Ember::EmberServices::getSingletonPtr()->getConfigService()->getHomeDirectory() + "carpenter/blueprints";
	{
		oslink::directory osdir(dir);
		while (osdir) {
			filename = osdir.next();
			S_LOG_VERBOSE( "Loading local blueprint: " << filename );
			Carpenter::BluePrint* blueprint = serializer.loadBlueprint(dir + "/" + filename);
			if (blueprint) {
				blueprint->compile();
				bool result = mJesus->addBluePrint(blueprint);
				if (!result)
				{
					S_LOG_FAILURE(  "Could not add blueprint: " << filename );
				}
			}
		}
	}


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
	mSceneMgr->setAmbientLight(Ogre::ColourValue(1, 1, 1));
	
	
	
	

  // create a Skydome
//  mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);

  
         
	 
	
}

void EmberOgre::connectViewSignals(Eris::View* world)
{
	mWorldView = world;
    world->registerFactory(mEmberEntityFactory);
}

EmberEntity* EmberOgre::getEntity(const std::string & id) const
{
	return static_cast<EmberEntity*>(mWorldView->getEntity(id));
}


void EmberOgre::connectedToServer(Eris::Connection* connection) 
{
	mEmberEntityFactory = new EmberEntityFactory(mTerrainGenerator, connection->getTypeService());
	//EventCreatedAvatarEntity.connect(sigc::mem_fun(*mAvatar, &Avatar::createdAvatarEmberEntity));
	EventCreatedEmberEntityFactory.emit(mEmberEntityFactory);
}



Avatar* EmberOgre::getAvatar() const {
	return mAvatar;
}


Ogre::SceneManager* EmberOgre::getSceneManager() const
{
	return mSceneMgr;
}

TerrainGenerator* EmberOgre::getTerrainGenerator() const
{
	return mTerrainGenerator;
}

MotionManager* EmberOgre::getMotionManager() const
{
	return mMotionManager;
}

Ogre::Root* EmberOgre::getOgreRoot() const
{
	assert(mRoot);
	return mRoot;
}

Ogre::SceneNode * EmberOgre::getWorldSceneNode( ) const
{
	if (mEmberEntityFactory && mEmberEntityFactory->getWorld()) {
		return mEmberEntityFactory->getWorld()->getSceneNode();
	} else {
		return mSceneMgr->getRootSceneNode();
	}
/*	Ogre::SceneNode* node = mSceneMgr->getSceneNode("0");
	//TODO: implement better exception handling
	if (node == 0)
		throw Exception();
	return node;*/
}

Ogre::SceneNode* EmberOgre::getRootSceneNode() const
{
	return mSceneMgr->getRootSceneNode();
}


AvatarCamera* EmberOgre::getMainCamera() const
{
	return mAvatar->getAvatarCamera();
}

EmberEntityFactory* EmberOgre::getEntityFactory() const
{
	return mEmberEntityFactory;
}

AvatarController* EmberOgre::getAvatarController() const
{
	return mAvatarController;
}



void EmberOgre::initializeEmberServices(void)
{
	// Initialize Ember services
	S_LOG_INFO( "Initializing Ember Services");

	// Initialize the Logging service and an error observer
	new Ember::EmberServices();
	Ember::LoggingService *logging = Ember::EmberServices::getSingletonPtr()->getLoggingService();
	OgreLogObserver* obs = new OgreLogObserver();
	//default to INFO, though this can be changed by the config file
 	obs->setFilter(Ember::LoggingService::INFO);
 	logging->addObserver(obs);


	// Initialize the Configuration Service
	Ember::EmberServices::getSingletonPtr()->getConfigService()->start();
	// Change working directory
	struct stat tagStat;
	int ret;
	ret = stat( Ember::EmberServices::getSingletonPtr()->getConfigService()->getHomeDirectory().c_str(), &tagStat );
	if (ret == -1) {
#ifdef WIN32
		mkdir(Ember::EmberServices::getSingletonPtr()->getConfigService()->getHomeDirectory().c_str());
#else
		mkdir(Ember::EmberServices::getSingletonPtr()->getConfigService()->getHomeDirectory().c_str(), S_IRWXU);
#endif
	}
	
	
	chdir(Ember::EmberServices::getSingletonPtr()->getConfigService()->getHomeDirectory().c_str());

	std::string sharePath(Ember::EmberServices::getSingletonPtr()->getConfigService()->getSharedConfigDirectory());

	//make sure that there are files 
	assureConfigFile("ember.conf", sharePath);

	Ember::EmberServices::getSingletonPtr()->getConfigService()->loadSavedConfig("ember.conf");



	//Initialize the Sound Service
	S_LOG_INFO( "Initializing Sound Service");

	std::cout << "************************************" << std::endl;
	std::cout << "TRACE --- CHECKING SOUND SERVICE " << std::endl;
	std::cout << "************************************" << std::endl;
#ifndef WIN32
	// Test that /dev/dsp is availible
	FILE *temp = fopen("/dev/dsp","w");
	if (temp) {
	  fclose(temp);
#endif
#ifndef WIN32
	// Initialize the SoundService
	std::cout << "************************************" << std::endl;
	std::cout << "TRACE --- INITIALIZING SOUND SERVICE" << std::endl;
	std::cout << "************************************" << std::endl;
	S_LOG_INFO("************** TEST LOG ****************");
	//Ember::EmberServices::getSingletonPtr()->getSoundService()->start();
	std::cout << "************************************" << std::endl;
	std::cout << "TRACE --- SOUND SERVICE INITIALIZED" << std::endl;
	std::cout << "************************************" << std::endl;
#endif
#ifndef WIN32
	}
#endif


	// Initialize and start the Metaserver Service.
	// Set Eris Logging Level
	S_LOG_INFO("Initializing MetaServer Service");

	Eris::setLogLevel(Eris::LOG_DEBUG);

 	Ember::EmberServices::getSingletonPtr()->getMetaserverService()->start();
	//hoho, we get linking errors if we don't do some calls to the service
	Ember::EmberServices::getSingletonPtr()->getMetaserverService()->getMetaServer();
	
	// Initialize the Server Service
	S_LOG_INFO("Initializing Server Service");

	Ember::EmberServices::getSingletonPtr()->getServerService()->GotConnection.connect(sigc::mem_fun(*this, &EmberOgre::connectedToServer));
	Ember::EmberServices::getSingletonPtr()->getServerService()->GotView.connect(sigc::mem_fun(*this, &EmberOgre::connectViewSignals));
	
	Ember::EmberServices::getSingletonPtr()->getServerService()->start();

 	Ember::EmberServices::getSingletonPtr()->getScriptingService()->start();


}

}

// ----------------------------------------------------------------------------
// Main function, just boots the application object
// ----------------------------------------------------------------------------
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"


INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
int main(int argc, char **argv)
#endif
{
	bool exit_program = false;
	bool useBinrelocPluginsLoading = false;
#ifndef __WIN32__
	if (argc > 1) {
		std::string invoked = std::string((char *)argv[0]);
		(argv)++;
		argc--;
		while (argc > 0)  {
			std::string arg = std::string((char *)argv[0]);
			argv++;
			argc--;
			if (arg == "-v" || arg == "--version") {
				//std::cout << "Ember version: " << VERSION << std::endl;
				exit_program = true;
			} else if (arg == "-b" || arg == "--binrelocloading") {
				useBinrelocPluginsLoading = true;
			} else if (arg == "-h" || arg == "--help") {
				std::cout << invoked << " {options}" << std::endl;
				std::cout << "-h, --help    - display this message" << std::endl;
				std::cout << "-v, --version - display version info" << std::endl;
				std::cout << "-b, --binrelocloading - loads ogre plugins through binreloc instead of ~/.ember/plugins.cfg" << std::endl;
				exit_program = true;
			} else {
		//        std::cout << "Unknown arument: " << arg << std::endl;
			}
		}
	}

	if (exit_program) {
		return 0;
	}
#else 
 //  char tmp[64];

 //  unsigned int floatSetting = _controlfp( 0, 0 );
	//sprintf(tmp, "Original: 0x%.4x\n", floatSetting );
 //   MessageBox( 0, tmp, "floating point control", MB_OK | MB_ICONERROR | MB_TASKMODAL);
	//_fpreset();
	//_controlfp(_PC_64, _MCW_PC);
	//_controlfp(_RC_NEAR , _MCW_RC);
	//floatSetting = _controlfp( 0, 0 );
	//sprintf(tmp, "New: 0x%.4x\n", floatSetting );
 //   MessageBox( 0, tmp, "floating point control", MB_OK | MB_ICONERROR | MB_TASKMODAL);

#endif

    // Create application object
    EmberOgre::EmberOgre app;

	// Initialize all Ember services needed for this application
	std::cout << "*************************************" << std::endl;
	std::cout << "TRACE --- INITIALIZING EMBER SERVICES" << std::endl;
	std::cout << "*************************************" << std::endl;
	app.initializeEmberServices();
	std::cout << "************************************" << std::endl;
	std::cout << "TRACE --- EMBER SERVICES INITIALIZED" << std::endl;
	std::cout << "************************************" << std::endl;

    try {
        app.go(useBinrelocPluginsLoading);
    } catch(const Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        MessageBox( 0, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        fprintf(stderr, "An exception has occured: %s\n",
                e.getFullDescription().c_str());
#endif
    }


    return 0;
}



