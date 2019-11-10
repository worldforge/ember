/*
 Copyright (C) 2010 Erik Ogenvik <erik@ogenvik.org>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "ShadowUpdateTask.h"
#include "TerrainPage.h"
#include "TerrainPageSurface.h"
#include "TerrainPageShadow.h"
#include "TerrainPageGeometry.h"

#include <OgreTextureManager.h>
#include <OgreRoot.h>
#include <OgreHardwarePixelBuffer.h>

namespace Ember {
namespace OgreView {

namespace Terrain {

ShadowUpdateTask::ShadowUpdateTask(GeometryPtrVector pageGeometries, const WFMath::Vector<3>& lightDirection) :
		mPageGeometries(std::move(pageGeometries)),
		mLightDirection(lightDirection) {

}

ShadowUpdateTask::~ShadowUpdateTask() = default;

void ShadowUpdateTask::executeTaskInBackgroundThread(Tasks::TaskExecutionContext& context) {
	for (auto& pageGeometry : mPageGeometries) {
		auto& page = pageGeometry->getPage();
		if (page.getSurface()) {
			auto shadow = page.getSurface()->getShadow();
			if (shadow) {
				auto& shadowTextureName = shadow->getShadowTextureName();
				if (!shadowTextureName.empty()) {
					pageGeometry->repopulate(true);
					shadow->setLightDirection(mLightDirection);
					shadow->updateShadow(*pageGeometry.get());
				}
			}
		}
	}
}

bool ShadowUpdateTask::executeTaskInMainThread() {
	if (!mPageGeometries.empty()) {
		auto pageGeometry = mPageGeometries.back();
		mPageGeometries.pop_back();
		auto& page = pageGeometry->getPage();
		if (page.getSurface()) {
			auto shadow = page.getSurface()->getShadow();
			if (shadow) {
				auto& shadowTextureName = shadow->getShadowTextureName();
				if (!shadowTextureName.empty()) {
					Ogre::TexturePtr texture = Ogre::Root::getSingletonPtr()->getTextureManager()->getByName(shadowTextureName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
					if (texture) {
						Ogre::Image ogreImage;
						shadow->loadIntoImage(ogreImage);

						texture->loadImage(ogreImage);

						//blit the whole image to the hardware buffer
						Ogre::PixelBox sourceBox(ogreImage.getPixelBox());
						//blit for each mipmap
						for (unsigned int i = 0; i <= texture->getNumMipmaps(); ++i) {
							Ogre::HardwarePixelBufferSharedPtr hardwareBuffer(texture->getBuffer(0, i));
							hardwareBuffer->blitFromMemory(sourceBox);
						}
					}
				}
			}
		}
	}
	return mPageGeometries.empty();
}

}

}
}
