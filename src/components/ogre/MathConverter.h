/*
-----------------------------------------------------------------------------
MathConverter.h by Miguel Guzman Miranda (Aglanor)

Copyright © 2003 The Worldforge Project (http://www.worldforge.org)
This file is part of Dime client (http://www.worldforge.org/dev/eng/clients/dime)

This code is distributed under the GNU GPL (General Public License).
See file COPYING for details.

 *  Change History (most recent first):
 *
 *      $Log$
 *      Revision 1.4  2004-07-12 22:00:14  erik
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
 *      Revamping of how entities are handled though the new class DimeEntity and changes to EntityListener.
 *      Rudimentary animation of the avatar.
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
*/
/*
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
// Include Ogre header files
// ------------------------------
#include <Ogre.h>

// ------------------------------
// Include WFmath header files
// ------------------------------
#include <wfmath/point.h>
#include <wfmath/vector.h>
#include <wfmath/axisbox.h>
#include <wfmath/quaternion.h>

inline WFMath::Point<3> Ogre2Atlas(Ogre::Vector3 p) {
	return WFMath::Point<3>(p.x,-p.z,p.y);
}

inline WFMath::Vector<3> Ogre2Atlas_Vector3(Ogre::Vector3 p) {
	return WFMath::Vector<3>(p.x,-p.z,p.y);
}
inline Ogre::Vector3 Atlas2Ogre(WFMath::Point<3> p){
	return Ogre::Vector3(p.x(),p.z(),-p.y());
}

inline Ogre::Quaternion Atlas2Ogre(WFMath::Quaternion aq){
	return Ogre::Quaternion(aq.scalar(),aq.vector().x(),aq.vector().z(),-aq.vector().y());
}

//is this correct?
inline WFMath::Quaternion Ogre2Atlas(Ogre::Quaternion aq){
	return WFMath::Quaternion(aq.w,aq.x,-aq.z,aq.y);
}

/*
 * used when scaling Ogre models to the units used ny WF, i.e. 1 unit == 1 meter
 * Ogre seems to use 1 unit == 1 centimeter
 */
#define OGRE2WF(x) (x * 0.01)
#define WF2OGRE(x) (x * 100.0)
#define OGRE2WF_VECTOR3(x,y,z) (Ogre::Vector3(x * 0.01, y * 0.01, z * 0.01))
#define WF2OGRE_VECTOR3(x,y,z) (Ogre::Vector3(x * 100.0, y * 100.0, z * 100.0))
#define OGRESCALER Ogre::Vector3(1,1,1)


#endif
