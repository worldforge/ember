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
	(o.z)--->		(-a.y)--->http://mandriva.com/en/community/start

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

/**
 * @brief A 2d position in the world.
 */
typedef WFMath::Point<2> TerrainPosition;

/**
 * @brief Converts a 2d terrainposition into a 3d Ogre vector.
 * The vertical position of the resulting vector will be zero.
 * @param p A 2d Worldforge position.
 * @return A 3d Ogre vector.
 */
Ogre::Vector3 Atlas2Ogre(const TerrainPosition& p);

/**
 * @brief Converts a 2d terrainposition into a 2d Ogre vector.
 * @param p A 2d Worldforge position.
 * @return A 2d Ogre vector.
 */
Ogre::Vector2 Atlas2Ogre_Vector2(const TerrainPosition& p);

/**
 * @brief Converts a 2d vector into a 2d Ogre vector.
 * @param p A 2d Worldforge vector.
 * @return A 2d Ogre vector.
 */
Ogre::Vector2 Atlas2Ogre(const WFMath::Vector<2>& p);

/**
 * @brief Converts an Ogre 3d vector into a Worldforge 3d point.
 * @param p A 3d Ogre vector.
 * @return A 3d Worldforge point.
 */
WFMath::Point<3> Ogre2Atlas(const Ogre::Vector3& p);

/**
 * @brief Converts an Ogre 2d vector into a Worldforge 2d point.
 * @param p A 2d Ogre vector.
 * @return A 2d Worldforge point.
 */
TerrainPosition Ogre2Atlas(const Ogre::Vector2& p);

/**
 * @brief Converts an Ogre 3d vector into a Worldforge 2d point.
 * @param p A 3d Ogre vector.
 * @return A 2d Worldforge point.
 */
TerrainPosition Ogre2Atlas_TerrainPosition(const Ogre::Vector3& p);

/**
 * @brief Converts an Ogre 3d vector into a Worldforge 3d vector.
 * @param p A 3d Ogre vector.
 * @return A 3d Worldforge vector.
 */
WFMath::Vector<3> Ogre2Atlas_Vector3(const Ogre::Vector3& p);

/**
 * @brief Converts a Worldforge 3d point into an Ogre 3d vector.
 * @param p A 3d Worldforge point.
 * @return A 3d Ogre vector.
 */
Ogre::Vector3 Atlas2Ogre(const WFMath::Point<3>& p);

/**
 * @brief Converts a Worldforge 3d vector into an Ogre 3d vector.
 * @param v A 3d Worldforge vector.
 * @return A 3d Ogre vector.
 */
Ogre::Vector3 Atlas2Ogre(const WFMath::Vector<3>& v);

/**
 * @brief Converts a Worldforge quaternion into an Ogre quaternion.
 * @param aq A Worldforge quaternion.
 * @return An Ogre quaternion.
 */
Ogre::Quaternion Atlas2Ogre(const WFMath::Quaternion& aq);

/**
 * @brief Converts an Ogre quaternion into a Worldforge quaternion.
 * @param aq An Ogre quaternion.
 * @return A Worldforge quaternion.
 */
WFMath::Quaternion Ogre2Atlas(const Ogre::Quaternion& aq);

/**
 * @brief Converts a Worldforge 3d axisbox into an Ogre 3d axisbox.
 * @param axisBox A Worldforge 3d axisbox.
 * @return An Ogre 3d axisbox.
 */
Ogre::AxisAlignedBox Atlas2Ogre(const WFMath::AxisBox<3>& axisBox);

/**
 * @brief Converts a Worldforge 2d axisbox into an Ogre TRect.
 * @param atlasBox A Worldforge 2d axisbox.
 * @return An Ogre TRect.
 */
Ogre::TRect<Ogre::Real> Atlas2Ogre(const WFMath::AxisBox<2>& atlasBox);


/**
 * @brief Converts an Ogre 3d axisbox into a Worldforge 3d axisbox.
 * @param axisBox An Ogre 3d axisbox.
 * @return A Worldforge 3d axisbox.
 */
WFMath::AxisBox<3> Ogre2Atlas(const Ogre::AxisAlignedBox& axisBox);

/**
 * @brief Converts an Ogre TRect into a Worldforge 2d axisbox.
 * @param bounds An Ogre TRect instance.
 * @return A Worldforge 2d axisbox.
 */
WFMath::AxisBox<2> Ogre2Atlas(const Ogre::TRect<Ogre::Real>& bounds);


///Implementations

inline Ogre::Vector3 Atlas2Ogre(const TerrainPosition& p) {
	return Ogre::Vector3(p.x(),0,-p.y());
}

inline Ogre::Vector2 Atlas2Ogre(const WFMath::Vector<2>& p) {
	return Ogre::Vector2(p.x(),-p.y());
}

inline Ogre::Vector2 Atlas2Ogre_Vector2(const TerrainPosition& p) {
	return Ogre::Vector2(p.x(),-p.y());
}

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

inline Ogre::Vector3 Atlas2Ogre(const WFMath::Point<3>& p){
	return Ogre::Vector3(p.x(),p.z(),-p.y());
}

inline Ogre::Vector3 Atlas2Ogre(const WFMath::Vector<3>& v){
	return Ogre::Vector3(v.x(),v.z(),-v.y());
}

inline Ogre::Quaternion Atlas2Ogre(const WFMath::Quaternion& aq){
	if (!aq.isValid()) {
		return Ogre::Quaternion::IDENTITY;
	}
	return Ogre::Quaternion(aq.scalar(),aq.vector().x(),aq.vector().z(),-aq.vector().y());
}

inline WFMath::Quaternion Ogre2Atlas(const Ogre::Quaternion& aq){
	return WFMath::Quaternion(aq.w,aq.x,-aq.z,aq.y);
}

inline Ogre::AxisAlignedBox Atlas2Ogre(const WFMath::AxisBox<3>& axisBox){
	if (!axisBox.isValid()) {
		return Ogre::AxisAlignedBox();
	}
	return Ogre::AxisAlignedBox(axisBox.lowCorner().x(), axisBox.lowCorner().z(), -axisBox.highCorner().y(), axisBox.highCorner().x(), axisBox.highCorner().z(), -axisBox.lowCorner().y());
}

inline Ogre::TRect<Ogre::Real> Atlas2Ogre(const WFMath::AxisBox<2>& atlasBox) {
	if (!atlasBox.isValid()) {
		return Ogre::TRect<Ogre::Real>();
	}
	return Ogre::TRect<Ogre::Real>(atlasBox.lowCorner().x(), -atlasBox.highCorner().y(), atlasBox.highCorner().x(), -atlasBox.lowCorner().y());
}

inline WFMath::AxisBox<3> Ogre2Atlas(const Ogre::AxisAlignedBox& axisBox){
	if (axisBox.isNull() || axisBox.isInfinite()) {
		return WFMath::AxisBox<3>();
	}
	return WFMath::AxisBox<3>(WFMath::Point<3>(axisBox.getMinimum().x, -axisBox.getMaximum().z, axisBox.getMinimum().y), WFMath::Point<3>(axisBox.getMaximum().x, -axisBox.getMinimum().z, axisBox.getMaximum().y));
}

inline WFMath::AxisBox<2> Ogre2Atlas(const Ogre::TRect<Ogre::Real>& bounds){
	return WFMath::AxisBox<2>(WFMath::Point<2>(bounds.left, -bounds.top), WFMath::Point<2>(bounds.right, -bounds.bottom));
}

}

#endif
