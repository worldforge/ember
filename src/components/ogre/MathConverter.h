/*
-----------------------------------------------------------------------------
MathConverter.h by Miguel Guzman Miranda (Aglanor)

Copyright © 2003 The Worldforge Project (http://www.worldforge.org)
This file is part of Ember client (http://www.worldforge.org/dev/eng/clients/Ember)

This code is distributed under the GNU GPL (General Public License).
See file COPYING for details.

 *  Change History (most recent first):
 *
 *      $Log$
 *      Revision 1.15  2005-07-10 21:54:28  erik
 *      2005-07-10  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* src/components/ogre/MathConverter.h: added method for converting between Ogre::Vector2 and WFMath
 *
 *      Revision 1.14  2005/05/07 22:24:07  erik
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
 *      Revision 1.13  2005/01/04 23:02:36  erik
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
 *      Revision 1.12  2004/11/13 21:08:01  erik
 *      2004-11-13  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* Removed some bugs which in various ways stopped the main loop.
 *      	* Finalized on a resource structure.
 *      	* Tried to arrange the inludes in a way that made it easy to compile Ember in debug mode without getting bit by the special debug Ogre memory manager. It's not 100% though. For now, change OgreConfig.h, set OGRE_DEBUG_MEMORY_MANAGER  to 0.
 *      	* Fixed a bug which made Ember not release the mouse upon exit.
 *      	* Changed the gui to use png images instead of tga, since apparently some machines can't load the tga in a correct way.
 *
 *      Revision 1.11  2004/11/04 21:40:26  erik
 *      2004-11-04 Erik Hjortsberg <erik@katastrof.nu>
 *      http://erikhjortsberg.blogspot.com/
 *
 *      * Moved from dime to ember. Thus the big checkin.
 *      * Added wield action to inventory
 *
 *      Revision 1.10  2004/10/31 20:26:54  erik
 *      2004-10-31 Erik Hjortsberg <erik@hysteriskt.nu>
 *      http://erikhjortsberg.blogspot.com/
 *
 *      * Implemented give and drop actions, making Mason fully playable now.
 *      * Various gui fixes.
 *      * Updates for the latest Eris cvs.
 *
 *      Revision 1.9  2004/10/30 17:32:38  erik
 *      2004-10-30 Erik Hjortsberg <erik@hysteriskt.nu>
 *      http://erikhjortsberg.blogspot.com/
 *
 *      * Fixed the issue with entities not getting correct orientation.
 *      * Added a give action to the EntityPickerWidget
 *      * Fixed creation of entities through MakeEntityWidget
 *
 *      Revision 1.8  2004/10/12 23:03:01  erik
 *      2004-10-12 Erik Hjortsberg <erik@katastrof.nu>
 *
 *      * fixed mouse picking
 *      * streamlined inventory a bit more
 *
 *      Revision 1.7  2004/07/21 00:27:29  erik
 *      2004-07-21 Erik Hjortsberg <erik@hysteriskt.nu>
 *
 *      /src/components/ogre:
 *      *removed references to EntityListener, we'll keep the class however as
 *      we'll pobably use it in the future
 *      *put everything under the namespace EmberOgre
 *
 *      Revision 1.6  2004/07/20 22:52:16  erik
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
 *      Revision 1.5  2004/07/18 23:26:24  erik
 *      2004-07-19 Erik Hjortsberg <erik@hysteriskt.nu>
 *      src/components/ogre:
 *
 *      *implemented containers, though it's not completely solid yet. You'll notice that all avatars are carrying coins through our use of a rather big placeholder.
 *      *added the class MotionManager, this takes care of both motion and animation. This means that both fluid motion of all entites and animation for those that have defined skeletons is implemented
 *
 *      Revision 1.4  2004/07/12 22:00:14  erik
 *      2004-07-12 Erik Hjortsberg <erik by hysteriskt speck nu>
 *      src/components/ogre:
 *      *added movement on server, although the axes are messed up right now
 *
 *      Revision 1.3  2004/07/12 04:02:14  erik
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
 *      Revision 1.2  2003/04/28 22:42:06  aglanor
 *      Added new viewport for a mini-map and quaternion conversion.
 *      Eris entities are now displayed with orientation.
 *
 *      Revision 1.1  2003/04/27 23:53:06  aglanor
 *      skipped this file in the cvs adds
 *

-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
Filename:    MathConverter.h
Description:	Point, Vector and Quaternion converter
	for world-centric coordinates (Atlas-wfmath like)
	to/from screen-centric coordinates (Ogre like).

	Atlas is World-centric (picture a map):
		x is east
		y is north
		z is up

	Ogre is screen-centric (picture your monitor screen):
		x is right
		y is up
		z is depth (negative towards the screen,
			positive is towards you,
			so the coord system is also dextrogyrous)

	--------------------------------------------------------
	Example of Atlas --> Ogre conversion

	Picture this is the world map, in both cases:
	^ North
	|
	|     East
	(Up)--->


	Atlas			Ogre

	^ a.y			^ -o.z
	|			|
	|     a.x		|   o.x
	(a.z)--->		(o.y)--->

	--------------------------------------------------------
	Example of Ogre --> Atlas conversion

	Picture this is your computer screen, in both cases:
	^ Up
	|
	|      Left
	(You)--->

	Ogre			Atlas

	^ o.y			^ a.z
	|			|
	|     o.x		|      a.x
	(o.z)--->		(-a.y)--->

	The math is:

	Atlas.x = Ogre.x
	Atlas.y = -Ogre.z
	Atlas.z = Ogre.y

	Ogre.x = Atlas.x
	Ogre.y = Atlas.z
	Ogre.z = -Atlas.y

-----------------------------------------------------------------------------
*/

#ifndef __MATH_CONVERTER_H__
#define __MATH_CONVERTER_H__

// ------------------------------
// Include WFmath header files
// ------------------------------
#include <wfmath/point.h>
#include <wfmath/vector.h>
#include <wfmath/axisbox.h>
#include <wfmath/quaternion.h>
#include <wfmath/const.h>

namespace EmberOgre {

typedef WFMath::Point<2> TerrainPosition;

// //note that this will ignore the y value of the supplied ogre vector
// inline TerrainPosition Ogre2Atlas(Ogre::Vector3& p) {
// 	return WFMath::Point<2>(p.x,-p.z);
// }

/*inline Ogre::Vector3 Atlas2Ogre(TerrainPosition& p) {
	return Ogre::Vector3(p.x(),0,-p.y());
}*/
inline Ogre::Vector3 Atlas2Ogre(TerrainPosition p) {
	return Ogre::Vector3(p.x(),0,-p.y());
}

// inline WFMath::Point<3> Ogre2Atlas(Ogre::Vector3& p) {
// 	return WFMath::Point<3>(p.x,-p.z,p.y);
// }

/*inline WFMath::Vector<3> Ogre2Atlas_Vector3(Ogre::Vector3& p) {
	return WFMath::Vector<3>(p.x,-p.z,p.y);
}*/
inline WFMath::Point<3> Ogre2Atlas(Ogre::Vector3 p) {
	return WFMath::Point<3>(p.x,-p.z,p.y);
}

inline TerrainPosition Ogre2Atlas(Ogre::Vector2 p) {
	return TerrainPosition(p.x,-p.y);
}

inline TerrainPosition Ogre2Atlas_TerrainPosition(Ogre::Vector3 p) {
	return TerrainPosition(p.x,-p.z);
}

inline WFMath::Vector<3> Ogre2Atlas_Vector3(Ogre::Vector3 p) {
	return WFMath::Vector<3>(p.x,-p.z,p.y);
}

// inline Ogre::Vector3 Atlas2Ogre(WFMath::Point<3>& p){
// 	return Ogre::Vector3(p.x(),p.z(),-p.y());
// }

inline Ogre::Vector3 Atlas2Ogre(WFMath::Point<3> p){
	return Ogre::Vector3(p.x(),p.z(),-p.y());
}

// inline Ogre::Vector3 Atlas2Ogre(WFMath::Vector<3>& v){
// 	return Ogre::Vector3(v.x(),v.z(),-v.y());
// }

inline Ogre::Vector3 Atlas2Ogre(WFMath::Vector<3> v){
	return Ogre::Vector3(v.x(),v.z(),-v.y());
}

// inline Ogre::Quaternion Atlas2Ogre(WFMath::Quaternion& aq){
// 	if (!aq.isValid()) {
// 		return Ogre::Quaternion::IDENTITY;
// 	}
// 	return Ogre::Quaternion(aq.scalar(),aq.vector().x(),aq.vector().z(),-aq.vector().y());
// }

inline Ogre::Quaternion Atlas2Ogre(WFMath::Quaternion aq){
	if (!aq.isValid()) {
		return Ogre::Quaternion::IDENTITY;
	}
	return Ogre::Quaternion(aq.scalar(),aq.vector().x(),aq.vector().z(),-aq.vector().y());
}

//is this correct?
// inline WFMath::Quaternion Ogre2Atlas(Ogre::Quaternion& aq){
// 	return WFMath::Quaternion(aq.w,aq.x,-aq.z,aq.y);
// }
inline WFMath::Quaternion Ogre2Atlas(Ogre::Quaternion aq){
	return WFMath::Quaternion(aq.w,aq.x,-aq.z,aq.y);
}


//these methods are obsolete
// #define OGRE2WF(x) x 
// #define WF2OGRE(x) x
// #define OGRE2WF_VECTOR3(x,y,z) (Ogre::Vector3(x, y, z))
// #define WF2OGRE_VECTOR3(x,y,z) (Ogre::Vector3(x, y, z))
// #define OGRESCALER Ogre::Vector3(1,1,1)

}



/*
Ogre::Vector3 Ogre::Vector3(WFMath::Vector<3> v) {
	return Ogre::Vector3(v.x(),v.z(),-v.y());
}


WFMath::Point<3>::operator Ogre::Vector3() const{
	return Ogre::Vector3(this.x(),this.z(),-this.y());
}
*/

#endif
