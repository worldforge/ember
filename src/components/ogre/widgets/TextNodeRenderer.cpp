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

#include "TextNodeRenderer.h"
#include "components/ogre/camera/MainCamera.h"

namespace Ember {
namespace OgreView {
namespace Gui {

TextNodeRenderer::TextNodeRenderer(CEGUI::Window& mainSheet, const UniqueWindowPtr <CEGUI::Window>& windowTemplate) : mMainSheet(mainSheet) {

	for (int i = 0; i < 20; ++i) {
		UniqueWindowPtr <CEGUI::Window> window(windowTemplate->clone(true));
		window->setMousePassThroughEnabled(true);
		window->setRiseOnClickEnabled(false);
		window->setName("TextNode_" + std::to_string(i));

		TextNode textNode(std::move(window));
		mFreeTextNodes.emplace_back(std::move(textNode));
	}
}

void TextNodeRenderer::attachTextToNode(Ogre::SceneNode* node, const std::string& text, std::function<void(Ogre::Vector2 & )> adjustFn) {
	if (!mFreeTextNodes.empty()) {
		auto textNode = std::move(mFreeTextNodes.back());
		mFreeTextNodes.pop_back();
		textNode.mNode = node;
		textNode.mAdjustFn = std::move(adjustFn);
		textNode.mWindow->setText(text);
		mMainSheet.addChild(textNode.mWindow.get());
        textNode.mWindow->moveToBack();
		mActiveTextNodes.emplace_back(std::move(textNode));
	}
};

void TextNodeRenderer::detachTextNode(Ogre::SceneNode* node) {
	auto I = std::find_if(mActiveTextNodes.begin(), mActiveTextNodes.end(), [&](const TextNode& textNode) -> bool { return node == textNode.mNode; });
	if (I != mActiveTextNodes.end()) {
		mMainSheet.removeChild(I->mWindow.get());
		mFreeTextNodes.emplace_back(std::move(*I));
		mActiveTextNodes.erase(I);
	}
}

void TextNodeRenderer::cameraPreRenderScene(Ogre::Camera* cam) {

	for (const auto& textNode : mActiveTextNodes) {
		renderTextNode(cam, textNode);
	}

};

void TextNodeRenderer::renderTextNode(Ogre::Camera* camera, const TextNode& textNode) {
	auto worldCoords = textNode.mNode->_getDerivedPosition();
	//check what the new position is in screen coords
	auto screenCoords = Camera::MainCamera::worldToScreen(*camera, worldCoords);

	if (screenCoords.isNaN()) {
		textNode.mWindow->setVisible(false);
	} else {
		if (textNode.mAdjustFn) {
			textNode.mAdjustFn(screenCoords);
		}
		textNode.mWindow->setVisible(true);
		textNode.mWindow->setPosition(CEGUI::UVector2(
				CEGUI::UDim(screenCoords.x, -(textNode.mWindow->getPixelSize().d_width * 0.5f)),
				CEGUI::UDim(screenCoords.y, -(textNode.mWindow->getPixelSize().d_height * 0.5f))));
	}
}

}
}
}