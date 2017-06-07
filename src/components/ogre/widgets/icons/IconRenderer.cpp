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
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "IconRenderer.h"
#include "Icon.h"
#include "IconImageStore.h"

#include "../../model/Model.h"
#include "../../model/ModelDefinitionManager.h"
#include "../../SimpleRenderContext.h"
#include <OgreHardwarePixelBuffer.h>
#include <OgreRenderTexture.h>
#include <OgreSceneManager.h>
#include <OgreRoot.h>
#include <sigc++/bind.h>

namespace Ember
{
namespace OgreView
{

namespace Gui
{

namespace Icons
{

IconRenderer::IconRenderer(const std::string& prefix, int pixelWidth) :
	mPixelWidth(pixelWidth), mRenderContext(new SimpleRenderContext(prefix, pixelWidth, pixelWidth)), mWorker(nullptr), mSceneNodeProvider(mRenderContext->getSceneNode(), nullptr, false)
{
	mRenderContext->getSceneManager()->setAmbientLight(Ogre::ColourValue(0.7, 0.7, 0.7));
	mRenderContext->setBackgroundColour(Ogre::ColourValue::ZERO);

}

IconRenderer::~IconRenderer()
{
	delete mWorker;
}

void IconRenderer::setWorker(IconRenderWorker* worker)
{
	mWorker = worker;
}

void IconRenderer::render(const std::string& modelName, Icon* icon)
{
	auto modelDef = Model::ModelDefinitionManager::getSingleton().getByName(modelName);
	auto model = std::shared_ptr<Model::Model>(new Model::Model(*getRenderContext()->getSceneManager(), modelDef, modelName));
	if (model->isLoaded()) {
		render(model, icon);
	} else {
		//If it's being loaded in a background thread, listen for reloading and render it then. The "Reload" signal will be emitted in the main thread.
		model->Reloaded.connect([=]{renderDelayed(model, icon);});
	}
}

void IconRenderer::render(const std::shared_ptr<Model::Model>& model, Icon* icon)
{
	mWorker->render(model, icon, icon->getImageStoreEntry());
}

void IconRenderer::renderDelayed(const std::shared_ptr<Model::Model>& model, Icon* icon)
{
	render(model, icon);
}

void IconRenderer::performRendering(Model::Model* model, Icon*)
{

	if (model) {
		// 		icon->getImageStoreEntry()->getTexture()->get

		model->attachToNode(&mSceneNodeProvider);

		//check for a defined "icon" view and use that if available, else just reposition the camera
		const Model::ViewDefinitionStore::const_iterator I = model->getDefinition()->getViewDefinitions().find("icon");
		if (I != model->getDefinition()->getViewDefinitions().end()) {
			mRenderContext->resetCameraOrientation();
			mRenderContext->repositionCamera();
			mRenderContext->showFull(model->getCombinedBoundingRadius());
			if (I->second->Distance) {
				mRenderContext->setCameraDistance(I->second->Distance);
			}
			// 			Ogre::Camera* camera = mRenderContext->getCamera();
			mRenderContext->getCameraRootNode()->setOrientation(I->second->Rotation);
		} else {
			mRenderContext->resetCameraOrientation();
			mRenderContext->repositionCamera();
			mRenderContext->showFull(model->getCombinedBoundingRadius());
			mRenderContext->setCameraDistance(mRenderContext->getDefaultCameraDistance() * 0.75);
		}

		// 		mRenderContext->setCameraDistance(0.75);

		//the problem with PBuffer and Copy might be that we need to wait a little before we try to blit, since it's not guaranteed that the content will be correctly rendered (since the render ops are queued to the GPU)
		//thus we need to create some kind of frame listener callback mechanism
		if (mRenderContext->getViewport()) {
			try {
				mRenderContext->getViewport()->update();
			} catch (const std::exception& ex) {
				S_LOG_FAILURE("Error when updating render for IconRenderer." << ex);
			}
		}

		// 		SDL_Delay(1000);
		// 		blitRenderToIcon(icon);

		model->attachToNode(nullptr);
	}
}

void IconRenderer::blitRenderToIcon(Icon* icon)
{
	if (!mRenderContext->getTexture().isNull()) {
		Ogre::HardwarePixelBufferSharedPtr srcBuffer = mRenderContext->getTexture()->getBuffer();
		Ogre::HardwarePixelBufferSharedPtr dstBuffer = icon->getImageStoreEntry()->getTexture()->getBuffer();

		Ogre::Box sourceBox(0, 0, mRenderContext->getRenderTexture()->getWidth(), mRenderContext->getRenderTexture()->getHeight());
		// 	Ogre::PixelBox sourceLockedBox = srcBuffer->lock(sourceBox, Ogre::HardwareBuffer::HBL_READ_ONLY);

		Ogre::Box dstBox = icon->getImageStoreEntry()->getBox();
		// 	Ogre::PixelBox dstLockedBox = dstBuffer->lock(dstBox, Ogre::HardwareBuffer::HBL_NORMAL);

		/*	Ogre::MemoryDataStream* dataStream = new Ogre::MemoryDataStream(sourceBox.getWidth() * sourceBox.getHeight() * 4, true);
		 Ogre::DataStreamPtr imageDataStreamPtr(dataStream);*/

		/*	Ogre::Image image;
		 image.loadRawData(imageDataStreamPtr, sourceBox.getWidth(), sourceBox.getHeight(), Ogre::PF_A8R8G8B8);*/

		srcBuffer->blitToMemory(icon->getImageStoreEntry()->getImagePixelBox());
		dstBuffer->blitFromMemory(icon->getImageStoreEntry()->getImagePixelBox(), dstBox);
		//Now that the icon is updated, emit a signal to this effect.
		icon->EventUpdated.emit();
	}
	//  	static int counter(0);
	//  	std::stringstream ss;
	//  	ss << "/home/erik/skit/temp_" << counter++ << ".jpg";
	// 	image.save(ss.str());

	//	dstBuffer->blit(srcBuffer, sourceBox, dstBox);

	// 	srcBuffer->unlock();
	// 	dstBuffer->unlock();
}

// void IconRenderer::showModel(const std::string& modelName)
// {
// 	if (mModel) {
// 		mModel->_getManager()->destroyMovableObject(mModel);
// 		//delete mModel;
// 	}
// 	if (modelName != "") {
// 		mModel = Model::Model::createModel(mTexture->getRenderContext()->getSceneManager(), modelName);
// // 		mModel->create(modelName);
// 		//override the rendering distance from the model; we want to always show it in the preview
// 		mModel->setRenderingDistance(0);
// 		setModel(mModel);
// 		mTexture->getRenderContext()->setActive(true);
// 	} else {
// 		setModel(0);
// 		mTexture->getRenderContext()->setActive(false);
// 	}
// }

// }
SimpleRenderContext* IconRenderer::getRenderContext()
{
	return mRenderContext.get();
}

IconRenderWorker::IconRenderWorker(IconRenderer& renderer) :
	mRenderer(renderer), mImageStoreEntry(0)
{
}

IconRenderWorker::~IconRenderWorker()
{
}

DirectRendererWorker::DirectRendererWorker(IconRenderer& renderer) :
	IconRenderWorker(renderer)
{
}

DirectRendererWorker::~DirectRendererWorker()
{
}

void DirectRendererWorker::render(const std::shared_ptr<Model::Model>& model, Icon* icon, IconImageStoreEntry* imageStoreEntry)
{
	//set the viewport to render into the icon texture
	mRenderer.getRenderContext()->setTexture(imageStoreEntry->getTexture());
	Ogre::TRect<float> iconBox(imageStoreEntry->getRelativeBox());
	mRenderer.getRenderContext()->getViewport()->setDimensions(iconBox.left, iconBox.top, iconBox.right - iconBox.left, iconBox.bottom - iconBox.top);

	mRenderer.performRendering(model.get(), icon);
	// 	mRenderer.blitRenderToIcon(icon);
}

DelayedIconRendererEntry::DelayedIconRendererEntry(DelayedIconRendererWorker& renderer, const std::shared_ptr<Model::Model>& model, Icon* icon) :
	mRenderer(renderer), mModel(model), mIcon(icon), mFrames(0)
{
}

DelayedIconRendererEntry::~DelayedIconRendererEntry()
{
}

Model::Model* DelayedIconRendererEntry::getModel()
{
	return mModel.get();
}

Icon* DelayedIconRendererEntry::getIcon()
{
	return mIcon;
}

void DelayedIconRendererEntry::frameStarted()
{
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
	IconRenderWorker(renderer)
{
	Ogre::Root::getSingleton().addFrameListener(this);
}

DelayedIconRendererWorker::~DelayedIconRendererWorker()
{
	Ogre::Root::getSingleton().removeFrameListener(this);
}

void DelayedIconRendererWorker::render(const std::shared_ptr<Model::Model>& model, Icon* icon, IconImageStoreEntry*)
{
	DelayedIconRendererEntry entry(*this, model, icon);
	entries.push(entry);
}

bool DelayedIconRendererWorker::frameStarted(const Ogre::FrameEvent&)
{
	try {
		if (entries.size()) {
			entries.front().frameStarted();
		}
	} catch (const std::exception& e) {
		S_LOG_FAILURE("Error when rendering icon." << e);
		entries.pop();
	}
	return true;
}

IconRenderer& DelayedIconRendererWorker::getRenderer()
{
	return mRenderer;
}

void DelayedIconRendererWorker::performRendering(DelayedIconRendererEntry& entry)
{
	mRenderer.performRendering(entry.getModel(), entry.getIcon());
}

void DelayedIconRendererWorker::finalizeRendering(DelayedIconRendererEntry& entry)
{
	mRenderer.blitRenderToIcon(entry.getIcon());
	entries.pop();
}

}

}

}
}
