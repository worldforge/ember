/*
-----------------------------------------------------------------------------
MathConverter.h by Miguel Guzman Miranda (Aglanor)
Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2005

Copyright © 2003 The Worldforge Project (http://www.worldforge.org)
This file is part of Ember client (http://www.worldforge.org/dev/eng/clients/Ember)

This code is distributed under the GNU GPL (General Public License).
See file COPYING for details.

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

#include <math.h>

namespace EmberOgre {

typedef WFMath::Point<2> TerrainPosition;

// //note that this will ignore the y value of the supplied ogre vector
// inline TerrainPosition Ogre2Atlas(Ogre::Vector3& p) {
// 	return WFMath::Point<2>(p.x,-p.z);
// }

/*inline Ogre::Vector3 Atlas2Ogre(TerrainPosition& p) {
	return Ogre::Vector3(p.x(),0,-p.y());
}*/
inline Ogre::Vector3 Atlas2Ogre(const TerrainPosition& p) {
	return Ogre::Vector3(p.x(),0,-p.y());
}

inline Ogre::Vector2 Atlas2Ogre_Vector2(const TerrainPosition& p) {
	return Ogre::Vector2(p.x(),-p.y());
}

// inline WFMath::Point<3> Ogre2Atlas(Ogre::Vector3& p) {
// 	return WFMath::Point<3>(p.x,-p.z,p.y);
// }

/*inline WFMath::Vector<3> Ogre2Atlas_Vector3(Ogre::Vector3& p) {
	return WFMath::Vector<3>(p.x,-p.z,p.y);
}*/
inline WFMath::Point<3> Ogre2Atlas(const Ogre::Vector3& p) {
	return WFMath::Point<3>(p.x,-p.z,p.y);
}

inline TerrainPosition Ogre2Atlas(const Ogre::Vector2& p) {
	return TerrainPosition(p.x,-p.y);
}

inline TerrainPosition Ogre2Atlas_TerrainPosition(const Ogre::Vector3& p) {
	return TerrainPosition(p.x,-p.z);
}

inline WFMath::Vector<3> Ogre2Atlas_Vector3(const Ogre::Vector3& p) {
	return WFMath::Vector<3>(p.x,-p.z,p.y);
}

// inline Ogre::Vector3 Atlas2Ogre(WFMath::Point<3>& p){
// 	return Ogre::Vector3(p.x(),p.z(),-p.y());
// }

inline Ogre::Vector3 Atlas2Ogre(const WFMath::Point<3>& p){
	return Ogre::Vector3(p.x(),p.z(),-p.y());
}

// inline Ogre::Vector3 Atlas2Ogre(WFMath::Vector<3>& v){
// 	return Ogre::Vector3(v.x(),v.z(),-v.y());
// }

inline Ogre::Vector3 Atlas2Ogre(const WFMath::Vector<3>& v){
	return Ogre::Vector3(v.x(),v.z(),-v.y());
}

// inline Ogre::Quaternion Atlas2Ogre(WFMath::Quaternion& aq){
// 	if (!aq.isValid()) {
// 		return Ogre::Quaternion::IDENTITY;
// 	}
// 	return Ogre::Quaternion(aq.scalar(),aq.vector().x(),aq.vector().z(),-aq.vector().y());
// }

inline Ogre::Quaternion Atlas2Ogre(const WFMath::Quaternion& aq){
	if (!aq.isValid()) {
		return Ogre::Quaternion::IDENTITY;
	}
	return Ogre::Quaternion(aq.scalar(),aq.vector().x(),aq.vector().z(),-aq.vector().y());
}

//is this correct?
// inline WFMath::Quaternion Ogre2Atlas(Ogre::Quaternion& aq){
// 	return WFMath::Quaternion(aq.w,aq.x,-aq.z,aq.y);
// }
inline WFMath::Quaternion Ogre2Atlas(const Ogre::Quaternion& aq){
	return WFMath::Quaternion(aq.w,aq.x,-aq.z,aq.y);
}



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
