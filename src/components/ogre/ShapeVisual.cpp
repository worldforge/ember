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

#include "ShapeVisual.h"

#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

namespace Ember
{
namespace OgreView
{

ShapeVisual::ShapeVisual(Ogre::SceneNode& baseNode, bool closed) :
		mBaseNode(baseNode), mClosed(closed), mManualObject(nullptr)
{
	mManualObject = mBaseNode.getCreator()->createManualObject();
	mManualObject->setDynamic(true); //We'll be updating this a lot if the user alters the polygon
	mManualObject->setRenderQueueGroup(Ogre::RENDER_QUEUE_SKIES_LATE - 1); //We want to render the lines on top of everything, so that they aren't hidden by anything
	baseNode.attachObject(mManualObject);

}

ShapeVisual::~ShapeVisual()
{
	if (mManualObject) {
		mBaseNode.detachObject(mManualObject);
		mBaseNode.getCreator()->destroyManualObject(mManualObject);
	}
}

}

}
