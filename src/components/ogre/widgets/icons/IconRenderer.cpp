//
// C++ Implementation: IconRenderer
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2007
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
#include "IconRenderer.h"
#include "Icon.h"
#include "IconImageStore.h"

#include "../../model/Model.h"
#include "../../model/ModelDefinitionManager.h"
#include "../../SimpleRenderContext.h"
#include <OgreHardwarePixelBuffer.h>
#include <OgreRenderTexture.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <sigc++/bind.h>

#include <utility>
#include <sigc++/connection.h>

namespace Ember {
namespace OgreView {

namespace Gui {

namespace Icons {

IconRenderer::IconRenderer(const std::string& prefix, int pixelWidth) :
		mPixelWidth(pixelWidth),
		mRenderContext(std::make_unique<SimpleRenderContext>(prefix, pixelWidth, pixelWidth)),
		mWorker(nullptr),
		mSceneNodeProvider(mRenderContext->getSceneNode(), nullptr, false) {
	mRenderContext->getSceneManager()->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));
	mRenderContext->setBackgroundColour(Ogre::ColourValue::ZERO);

}

IconRenderer::~IconRenderer() = default;

void IconRenderer::setWorker(std::unique_ptr<IconRenderWorker> worker) {
	mWorker = std::move(worker);
}

void IconRenderer::render(const std::string& modelName, Icon* icon) {
	auto modelDef = Model::ModelDefinitionManager::getSingleton().getByName(modelName);
	if (modelDef) {
		auto model = std::make_shared<Model::Model>(*getRenderContext()->getSceneManager(), modelDef);
		//No need to use instancing, and it seems that there are bugs (at least in Ogre 1.9) which will make some models linger, polluting other icons.
		model->setUseInstancing(false);
		model->load();
		if (model->isLoaded()) {
			mWorker->render(model, icon, icon->getImageStoreEntry());
		} else {
			auto connection = std::make_shared<sigc::connection>();
			//If it's being loaded in a background thread, listen for reloading and render it then. The "Reload" signal will be emitted in the main thread.
			*connection = model->Reloaded.connect([=] {
				mWorker->render(model, icon, icon->getImageStoreEntry());
				connection->disconnect();
			});
		}
	}
}


void IconRenderer::performRendering(Model::Model* model, Icon*) {

	if (model) {

		model->attachToNode(&mSceneNodeProvider);

		//check for a defined "icon" view and use that if available, else just reposition the camera
		auto I = model->getDefinition()->getViewDefinitions().find("icon");
		if (I != model->getDefinition()->getViewDefinitions().end()) {
			mSceneNodeProvider.setPositionAndOrientation(Ogre::Vector3::ZERO, Ogre::Quaternion::IDENTITY);
			mRenderContext->resetCameraOrientation();
			mRenderContext->repositionCamera();
			mRenderContext->showFull(model->getCombinedBoundingRadius());
			if (I->second.Distance != 0) {
				mRenderContext->setCameraDistance(I->second.Distance);
			}
			mRenderContext->getCameraRootNode()->setOrientation(I->second.Rotation);
		} else {

			auto size = model->getBoundingBox().getSize();
			auto depth_to_height_ratio = size.z / size.y;
			auto radius = std::max(size.x, size.y);
			//If the ratio between the height and the depth of the item is larger than 3 it's probably being rendered from the side. Let's flip it to show it better. This ought to work in most cases.
			if (depth_to_height_ratio > 3) {
				mSceneNodeProvider.setPositionAndOrientation(Ogre::Vector3::ZERO, Ogre::Quaternion(Ogre::Degree(-90), Ogre::Vector3::UNIT_X));
				radius = std::max(size.x, size.z);
			} else {
				mSceneNodeProvider.setPositionAndOrientation(Ogre::Vector3::ZERO, Ogre::Quaternion::IDENTITY);
			}

			mRenderContext->resetCameraOrientation();
			mRenderContext->repositionCamera();
			mRenderContext->showFull(radius);
			mRenderContext->setCameraDistance(mRenderContext->getDefaultCameraDistance() * 0.75f);
		}

		if (mRenderContext->getViewport()) {
			try {
				mRenderContext->getViewport()->update();
			} catch (const std::exception& ex) {
				S_LOG_FAILURE("Error when updating render for IconRenderer." << ex);
			}
		}


		model->attachToNode(nullptr);
	}
}

void IconRenderer::blitRenderToIcon(Icon* icon) {
	if (mRenderContext->getTexture()) {
		Ogre::HardwarePixelBufferSharedPtr srcBuffer = mRenderContext->getTexture()->getBuffer();
		Ogre::HardwarePixelBufferSharedPtr dstBuffer = icon->getImageStoreEntry()->getTexture()->getBuffer();

		Ogre::Box sourceBox(0, 0, mRenderContext->getRenderTexture()->getWidth(), mRenderContext->getRenderTexture()->getHeight());

		Ogre::Box dstBox = icon->getImageStoreEntry()->getBox();

		try {
			if (srcBuffer->isLocked()) {
				srcBuffer->unlock();
			}

			if (dstBuffer->isLocked()) {
				dstBuffer->unlock();
			}

			dstBuffer->blit(srcBuffer, sourceBox, dstBox);

		} catch (...) {
			//Catch possible exceptions and ignore them
			S_LOG_WARNING("Got exception when trying to lock buffers. This will lead to some corrupt icons.");
		}

		//Now that the icon is updated, emit a signal to this effect.
		icon->EventUpdated.emit();
	}
}

SimpleRenderContext* IconRenderer::getRenderContext() {
	return mRenderContext.get();
}

IconRenderWorker::IconRenderWorker(IconRenderer& renderer) :
		mRenderer(renderer), mImageStoreEntry(nullptr) {
}

IconRenderWorker::~IconRenderWorker() = default;

DirectRendererWorker::DirectRendererWorker(IconRenderer& renderer) :
		IconRenderWorker(renderer) {
}

DirectRendererWorker::~DirectRendererWorker() = default;

void DirectRendererWorker::render(const std::shared_ptr<Model::Model>& model, Icon* icon, IconImageStoreEntry* imageStoreEntry) {
	//set the viewport to render into the icon texture
	mRenderer.getRenderContext()->setTexture(imageStoreEntry->getTexture());
	Ogre::TRect<float> iconBox(imageStoreEntry->getRelativeBox());
	mRenderer.getRenderContext()->getViewport()->setDimensions(iconBox.left, iconBox.top, iconBox.right - iconBox.left, iconBox.bottom - iconBox.top);

	mRenderer.performRendering(model.get(), icon);
}

DelayedIconRendererEntry::DelayedIconRendererEntry(DelayedIconRendererWorker& renderer, std::shared_ptr<Model::Model> model, Icon* icon)
		: mRenderer(renderer),
		  mModel(std::move(model)),
		  mIcon(icon),
		  mFrames(0) {
}

DelayedIconRendererEntry::~DelayedIconRendererEntry() = default;

Model::Model* DelayedIconRendererEntry::getModel() {
	return mModel.get();
}

Icon* DelayedIconRendererEntry::getIcon() {
	return mIcon;
}

void DelayedIconRendererEntry::frameStarted() {
	//on the first frame we'll do the rendering, and then do the blitting on the second frame
	if (mFrames == 0) {
		//do render
		mRenderer.performRendering(*this);
		mFrames++;
	} else {
		//do blit
		mRenderer.finalizeRendering(*this);
		//we can't do anything here, since the call to finalizeRendering will delete this instance
	}

}

DelayedIconRendererWorker::DelayedIconRendererWorker(IconRenderer& renderer) :
		IconRenderWorker(renderer) {
	Ogre::Root::getSingleton().addFrameListener(this);
}

DelayedIconRendererWorker::~DelayedIconRendererWorker() {
	Ogre::Root::getSingleton().removeFrameListener(this);
}

void DelayedIconRendererWorker::render(const std::shared_ptr<Model::Model>& model, Icon* icon, IconImageStoreEntry*) {
	DelayedIconRendererEntry entry(*this, model, icon);
	entries.push(entry);
}

bool DelayedIconRendererWorker::frameStarted(const Ogre::FrameEvent&) {
	try {
		if (!entries.empty()) {
			entries.front().frameStarted();
		}
	} catch (const std::exception& e) {
		S_LOG_FAILURE("Error when rendering icon." << e);
		entries.pop();
	}
	return true;
}

IconRenderer& DelayedIconRendererWorker::getRenderer() {
	return mRenderer;
}

void DelayedIconRendererWorker::performRendering(DelayedIconRendererEntry& entry) {
	mRenderer.performRendering(entry.getModel(), entry.getIcon());
}

void DelayedIconRendererWorker::finalizeRendering(DelayedIconRendererEntry& entry) {
	mRenderer.blitRenderToIcon(entry.getIcon());
	entries.pop();
}

}

}

}
}
