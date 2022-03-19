/*
 Copyright (C) 2019 Erik Ogenvik

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef EMBER_TEXTNODERENDERER_H
#define EMBER_TEXTNODERENDERER_H

#include "CEGUIUtils.h"

#include <Ogre.h>
#include <boost/noncopyable.hpp>
#include <functional>

namespace Ember {
namespace OgreView {

namespace Gui {

/**
 * Renders a CEGUI window on top of a SceneNode.
 */
class TextNodeRenderer : public Ogre::Camera::Listener, public boost::noncopyable {
public:
	explicit TextNodeRenderer(CEGUI::Window& mainSheet, const UniqueWindowPtr<CEGUI::Window>& windowTemplate);

	void cameraPreRenderScene(Ogre::Camera* cam) override;

	void attachTextToNode(Ogre::SceneNode* node, const std::string& text, std::function<void(Ogre::Vector2&)> adjustFn);

	void detachTextNode(Ogre::SceneNode* node);

protected:

	CEGUI::Window& mMainSheet;

	/**
	 * A SceneNode attached text window.
	 *
	 * The text window will be attached to the scene node, so that when either it or the camera moves the
	 * window is adjusted accordingly.
	 */
	struct TextNode : boost::noncopyable {
		UniqueWindowPtr<CEGUI::Window> mWindow;
		Ogre::SceneNode* mNode;
		std::function<void(Ogre::Vector2&)> mAdjustFn;

		explicit TextNode(UniqueWindowPtr<CEGUI::Window> window) :
				mWindow(std::move(window)),
				mNode(nullptr) {
		}

		TextNode(TextNode&& rhs) noexcept
				: mWindow(std::move(rhs.mWindow)),
				  mNode(rhs.mNode),
				  mAdjustFn(std::move(rhs.mAdjustFn)) {
			rhs.mNode = nullptr;
		}

		TextNode& operator=(TextNode&& rhs) noexcept {
			mNode = rhs.mNode;
			rhs.mNode = nullptr;
			mWindow = std::move(rhs.mWindow);
			mAdjustFn = std::move(rhs.mAdjustFn);
			return *this;
		}
	};

	void renderTextNode(Ogre::Camera* camera, const TextNode& textNode);

	std::vector<TextNode> mFreeTextNodes;
	std::vector<TextNode> mActiveTextNodes;


};
}
}
}


#endif //EMBER_TEXTNODERENDERER_H
