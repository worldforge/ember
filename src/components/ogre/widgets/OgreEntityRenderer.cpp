//
// C++ Implementation: OgreEntityRenderer
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2006
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

#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include <OgrePixelCountLodStrategy.h>
#include <CEGUI/Image.h>
#include <CEGUI/Window.h>

namespace Ember
{
namespace OgreView
{
namespace Gui
{

/**
 * @brief Stops the animation when the observed resource is unloaded.
 *
 * This prevents crashes, since the animation state will be incorrect when the skeleton has been unloaded (which happens when it's reloaded).
 */
class OgreEntityRendererResourceListener : public Ogre::Resource::Listener
{
private:
	OgreEntityRenderer& mRenderer;
	Ogre::AnimationState** mActiveAnimation;

public:
	OgreEntityRendererResourceListener(OgreEntityRenderer& renderer, Ogre::AnimationState** activeAnimation) : mRenderer(renderer), mActiveAnimation(activeAnimation)
	{

	}

	virtual void unloadingComplete(Ogre::Resource*) {
		*mActiveAnimation = nullptr;
	}

};

OgreEntityRenderer::OgreEntityRenderer(CEGUI::Window* image) :
		MovableObjectRenderer(image, image->getName().c_str()), mEntity(nullptr), mActiveAnimation(nullptr), mMeshListener(new OgreEntityRendererResourceListener(*this, &mActiveAnimation)), mSkeletonListener(new OgreEntityRendererResourceListener(*this, &mActiveAnimation))
{
}

OgreEntityRenderer::~OgreEntityRenderer()
{
	setEntity(nullptr);
	delete mMeshListener;
	delete mSkeletonListener;
}

Ogre::Entity* OgreEntityRenderer::getEntity()
{
	return mEntity;
}

void OgreEntityRenderer::showEntity(const std::string& mesh)
{
	unloadEntity();
	try {
		std::string entityName(mTexture->getImage()->getName().c_str());
		entityName += "_entity";
		mEntity = mTexture->getRenderContext()->getSceneNode()->getCreator()->createEntity(entityName, mesh);
		setEntity(mEntity);
		mTexture->getRenderContext()->setActive(true);
	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Error when creating entity." << ex);
	}
}

float OgreEntityRenderer::getMovableBoundingRadius()
{
	return mEntity->getBoundingRadius();
}

void OgreEntityRenderer::setEntity(Ogre::Entity* entity)
{
	mActiveAnimation = nullptr;
	Ogre::SceneNode* node = mTexture->getRenderContext()->getSceneNode();

	node->detachAllObjects();
	if (mEntity) {
		if (mMeshListener) {
			mEntity->getMesh()->removeListener(mMeshListener);
		}
		if (mEntity->hasSkeleton()) {
			mEntity->getSkeleton()->removeListener(mSkeletonListener);
		}
	}


	if (entity) {
		node->attachObject(entity);
		mTexture->getRenderContext()->repositionCamera();
		if (mAutoShowFull) {
			showFull();
		}

		entity->getMesh()->addListener(mMeshListener);
		if (entity->hasSkeleton()) {
			entity->getSkeleton()->addListener(mSkeletonListener);
		}
	}
}

Ogre::SceneManager* OgreEntityRenderer::getSceneManager()
{
	return mTexture->getRenderContext()->getSceneManager();
}

void OgreEntityRenderer::unloadEntity()
{
	setEntity(nullptr);
	if (mEntity) {
		Ogre::SceneManager* scenemgr = mTexture->getRenderContext()->getSceneManager();
		scenemgr->destroyEntity(mEntity);
		mEntity = nullptr;
	}
}

bool OgreEntityRenderer::getWireframeMode()
{
	return (mTexture->getRenderContext()->getCamera()->getPolygonMode() == Ogre::PM_WIREFRAME);
}

void OgreEntityRenderer::setWireframeMode(bool enabled)
{
	Ogre::PolygonMode mode = enabled ? Ogre::PM_WIREFRAME : Ogre::PM_SOLID;
	mTexture->getRenderContext()->getCamera()->setPolygonMode(mode);
}

void OgreEntityRenderer::setForcedLodLevel(int lodLevel)
{
	if (mEntity) {
		mEntity->setMeshLodBias(1, lodLevel, lodLevel);
	}
}

void OgreEntityRenderer::clearForcedLodLevel()
{
	if (mEntity) {
		mEntity->setMeshLodBias(1.0, 0, std::numeric_limits<unsigned short>::max());
	}
}

void OgreEntityRenderer::enableAnimation(const std::string& animationName)
{
	if (mEntity && mEntity->getAllAnimationStates()) {
		auto I = mEntity->getAllAnimationStates()->getAnimationStateIterator();
		while (I.hasMoreElements()) {
			auto state = I.getNext();
			if (state->getAnimationName() == animationName) {
				mActiveAnimation = state;
				state->setEnabled(true);
			} else {
				state->setEnabled(false);
			}
		}
	}
}

std::vector<std::string> OgreEntityRenderer::getEntityAnimationNames() const
{
	std::vector<std::string> names;

	if (mEntity && mEntity->getAllAnimationStates()) {
		auto I = mEntity->getAllAnimationStates()->getAnimationStateIterator();
		while (I.hasMoreElements()) {
			auto state = I.getNext();
			names.push_back(state->getAnimationName());
		}
	}
	return names;
}

bool OgreEntityRenderer::frameStarted(const Ogre::FrameEvent& event)
{
	if (mActiveAnimation) {
		mActiveAnimation->addTime(event.timeSinceLastFrame);
	}
	return MovableObjectRenderer::frameStarted(event);
}


}
}
}
