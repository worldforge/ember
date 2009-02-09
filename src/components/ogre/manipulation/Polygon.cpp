//
// C++ Implementation: Polygon
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2009
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
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "Polygon.h"
#include "PolygonPoint.h"
#include "PolygonPointPickListener.h"


#include <Ogre.h>
#include <wfmath/vector.h>
#include <wfmath/polygon.h>

#include "../MathConverter.h"
#include "../EmberOgre.h"
#include "../AvatarCamera.h"

#include "services/logging/LoggingService.h"



namespace EmberOgre {

namespace Manipulation {

Polygon::Polygon(Ogre::SceneNode* baseNode, IPolygonPositionProvider* positionProvider)
: mBaseNode(baseNode), mPositionProvider(positionProvider), mRenderer(*this), mListener(new PolygonPointPickListener(*this))
{
	EmberOgre::getSingleton().getMainCamera()->pushWorldPickListener(mListener.get());

}


Polygon::~Polygon()
{
	EmberOgre::getSingleton().getMainCamera()->removeWorldPickListener(mListener.get());
	clear();
}

Ogre::SceneNode* Polygon::getBaseNode()
{
	return mBaseNode;
}

void Polygon::loadFromShape(const WFMath::Polygon<2>& shape)
{
	clear();
	for(int i = 0; i < shape.numCorners(); ++i) {
		const WFMath::Point<2>& position = shape[i];
		PolygonPoint* point = new PolygonPoint(*this, position);
		mPoints.push_back(point);
	}
	mRenderer.update();
}

void Polygon::clear()
{
	for (PointStore::iterator I = mPoints.begin(); I != mPoints.end(); ++I) {
		delete *I;
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


}

}
