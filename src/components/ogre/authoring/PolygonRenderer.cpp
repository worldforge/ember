//
// C++ Implementation: PolygonRenderer
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "PolygonRenderer.h"

#include "Polygon.h"
#include "PolygonPoint.h"

#include <Ogre.h>

namespace Ember
{
namespace OgreView
{

namespace Authoring
{

unsigned int PolygonRenderer::sCounter = 0;

PolygonRenderer::PolygonRenderer(Ogre::SceneNode& sceneNode, const std::list<PolygonPoint*>& points, bool closed) :
	mNode(sceneNode), mPoints(points), mManualObject(nullptr), mClosed(closed)
{
	std::stringstream ss;
	ss << "PolygonRenderer" << sCounter++;
	mManualObject = mNode.getCreator()->createManualObject(ss.str());
	mManualObject->setDynamic(true); //We'll be updating this a lot if the use alters the polygon
	mManualObject->setRenderQueueGroup(Ogre::RENDER_QUEUE_SKIES_LATE - 1); //We want to render the lines on top of everything, so that they aren't hidden by anything
	mNode.attachObject(mManualObject);

}

PolygonRenderer::~PolygonRenderer()
{
	if (mManualObject) {
		mNode.detachObject(mManualObject);
		mNode.getCreator()->destroyManualObject(mManualObject);
	}
}

void PolygonRenderer::update()
{
	mManualObject->clear();

	if (mPoints.size() > 1) {
		mManualObject->begin("/common/base/authoring/polygon/line", Ogre::RenderOperation::OT_LINE_STRIP);

		for (auto point : mPoints) {
			mManualObject->position(point->getNode()->getPosition());
		}
		//if there are more than two points we should close the polygon (if mClosed is true)
		if (mPoints.size() > 2 && mClosed) {
			const PolygonPoint* point(*mPoints.begin());
			mManualObject->position(point->getNode()->getPosition());
		}
		mManualObject->end();
	}

}

}

}
}
