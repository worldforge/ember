//
// C++ Implementation: OgreEntityRenderer
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2006
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

#include "OgreEntityRenderer.h"
#include "EntityCEGUITexture.h"
#include "../SimpleRenderContext.h"

#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include <CEGUIImage.h>
#include <elements/CEGUIGUISheet.h>

namespace EmberOgre {
namespace Gui {

OgreEntityRenderer::OgreEntityRenderer(CEGUI::Window* image) : MovableObjectRenderer(image), mEntity(0)
{
}


OgreEntityRenderer::~OgreEntityRenderer()
{
}

Ogre::Entity* OgreEntityRenderer::getEntity()
{
	return mEntity;
}

void OgreEntityRenderer::showEntity(const std::string& mesh)
{
	if (mEntity) {
		mTexture->getRenderContext()->getSceneNode()->getCreator()->destroyEntity(mEntity);
	}
	std::string meshName(mTexture->getImage()->getName().c_str());
	meshName += "_entity";
	try {
		mEntity =  mTexture->getRenderContext()->getSceneNode()->getCreator()->createEntity(meshName , mesh);
		setEntity(mEntity);
		mTexture->getRenderContext()->setActive(true);
	} catch (const Ogre::Exception& ex) {
		S_LOG_FAILURE("Error when creating entity. Message: " << ex.getFullDescription());
	}
}


Ogre::MovableObject* OgreEntityRenderer::getMovableObject()
{
	return mEntity;
}

void OgreEntityRenderer::setEntity(Ogre::Entity* entity)
{
	Ogre::SceneNode* node = mTexture->getRenderContext()->getSceneNode();

	node->detachAllObjects();
	node->attachObject(entity);
	mTexture->getRenderContext()->repositionCamera();
	if (mAutoShowFull) {
		showFull();
	}

}
}
}
