//
// C++ Implementation: Polygon
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2009
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#include "Polygon.h"
#include "PolygonPoint.h"

#include "../Convert.h"
#include "../EmberOgre.h"
#include <OgreSceneNode.h>

namespace Ember
{
namespace OgreView
{

namespace Authoring
{

Polygon::Polygon(Ogre::SceneNode* baseNode, IPolygonPositionProvider* positionProvider, bool isClosed) :
		mBaseNode(baseNode->createChildSceneNode()),
		mPositionProvider(positionProvider),
		mRenderer(*baseNode, mPoints, isClosed),
		mBulletWorld(nullptr)
{
	mBaseNode->setInheritScale(false);
	mBaseNode->setScale(1.0f, 1.0f, 1.0f);
}

Polygon::~Polygon()
{
	clear();
	mBaseNode->getParentSceneNode()->removeAndDestroyChild(mBaseNode);
}

Ogre::SceneNode* Polygon::getBaseNode()
{
	return mBaseNode;
}

WFMath::Polygon<2> Polygon::getShape() const
{
	WFMath::Polygon<2> poly;
	unsigned int i = 0;
	for (auto& point : mPoints) {
		poly.addCorner(i++, point->getLocalPosition());
	}
	return poly;
}

void Polygon::loadFromShape(const WFMath::Polygon<2>& shape)
{
	clear();
	for (size_t i = 0; i < shape.numCorners(); ++i) {
		const WFMath::Point<2>& position = shape[i];
		PolygonPoint* point = new PolygonPoint(*getBaseNode(), getPositionProvider(), 0.25, position);
		point->makeInteractive(mBulletWorld);
		mPoints.push_back(point);
	}
	mRenderer.update();
}

void Polygon::clear()
{
	for (auto& point : mPoints) {
		delete point;
	}
	mPoints.clear();
	mRenderer.update();
}

const Polygon::PointStore& Polygon::getPoints() const
{
	return mPoints;
}

IPolygonPositionProvider* Polygon::getPositionProvider() const
{
	return mPositionProvider;
}

void Polygon::updateRender()
{
	mRenderer.update();
}

PolygonPoint* Polygon::appendPoint()
{
	PolygonPoint* newPoint = new PolygonPoint(*getBaseNode(), getPositionProvider(), 0.25);
	newPoint->makeInteractive(mBulletWorld);
	mPoints.push_back(newPoint);
	return newPoint;
}

PolygonPoint* Polygon::insertPointBefore(PolygonPoint& point)
{
	if (!mPoints.empty()) {
		auto I = std::find(mPoints.begin(), mPoints.end(), &point);
		if (I != mPoints.end()) {
			PolygonPoint* newPoint = new PolygonPoint(*getBaseNode(), getPositionProvider(), 0.25);
			newPoint->makeInteractive(mBulletWorld);
			mPoints.insert(I, newPoint);
			return newPoint;
		}
	}
	return nullptr;
}

bool Polygon::reInsertPointBefore(PolygonPoint& point, PolygonPoint& existingPoint)
{
	if (!mPoints.empty()) {
		auto I = std::find(mPoints.begin(), mPoints.end(), &point);
		if (I != mPoints.end()) {
			mPoints.insert(I, &existingPoint);
			return true;
		}
	}
	return false;
}

bool Polygon::reInsertPoint(size_t index, PolygonPoint& point)
{

	size_t i = 0;
	auto I = mPoints.begin();
	while (I != mPoints.end() && i < index) {
		++i;
		++I;
	}
	mPoints.insert(I, &point);
	return true;
}

PolygonPoint* Polygon::getPointBefore(PolygonPoint& point)
{
	if (!mPoints.empty()) {
		auto I = std::find(mPoints.begin(), mPoints.end(), &point);
		if (I != mPoints.end()) {
			if (I == mPoints.begin()) {
				return *(mPoints.rbegin());
			} else {
				return *(--I);
			}
		}
	}
	return nullptr;
}

PolygonPoint* Polygon::getPointAfter(PolygonPoint& point)
{
	if (!mPoints.empty()) {
		auto I = std::find(mPoints.begin(), mPoints.end(), &point);
		if (I != mPoints.end()) {
			if (I == (--mPoints.end())) {
				return *(mPoints.begin());
			} else {
				return *(++I);
			}
		}
	}
	return nullptr;
}

bool Polygon::removePoint(PolygonPoint& point)
{
	if (!mPoints.empty()) {
		auto I = std::find(mPoints.begin(), mPoints.end(), &point);
		if (I != mPoints.end()) {
			mPoints.erase(I);
			return true;
		}
	}
	return false;
}

void Polygon::makeInteractive(BulletWorld* bulletWorld) {
	mBulletWorld = bulletWorld;
	for (auto& point : mPoints) {
		point->makeInteractive(bulletWorld);
	}
}

}

}
}
