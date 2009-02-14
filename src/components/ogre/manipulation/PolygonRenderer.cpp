//
// C++ Implementation: PolygonRenderer
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

#include "PolygonRenderer.h"

#include "Polygon.h"
#include "PolygonPoint.h"

#include <Ogre.h>


namespace EmberOgre {

namespace Manipulation {

unsigned int PolygonRenderer::sCounter = 0;

PolygonRenderer::PolygonRenderer(Polygon& polygon)
: mPolygon(polygon), mManualObject(0)
{
	std::stringstream ss;
	ss << "PolygonRenderer" << sCounter++;
	mManualObject = polygon.getBaseNode()->getCreator()->createManualObject(ss.str());
	mManualObject->setDynamic(true); ///We'll be updating this a lot if the use alters the polygon
	mManualObject->setRenderQueueGroup(Ogre::RENDER_QUEUE_SKIES_LATE + 2); ///We want to render the lines on top of everything, so that they aren't hidden by anything
	mPolygon.getBaseNode()->attachObject(mManualObject);
	
}

PolygonRenderer::~PolygonRenderer()
{
	if (mManualObject) {
		mPolygon.getBaseNode()->detachObject(mManualObject);
		mPolygon.getBaseNode()->getCreator()->destroyManualObject(mManualObject);
	}
}

void PolygonRenderer::update()
{
	mManualObject->clear();
	
	if (mPolygon.getPoints().size()) {
		mManualObject->begin("/global/authoring/polygon/line", Ogre::RenderOperation::OT_LINE_STRIP);
		
		for (Polygon::PointStore::const_iterator I = mPolygon.getPoints().begin(); I != mPolygon.getPoints().end(); ++I) {
			const PolygonPoint* point(*I);
			mManualObject->position(point->getNode()->getPosition());
		}
		mManualObject->end();
	}

	
}


}

}
