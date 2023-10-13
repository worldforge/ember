/*
 Copyright (C) 2010  Erik Ogenvik <erik@ogenvik.org>

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

#include "EntityTooltip.h"
#include "components/ogre/World.h"
#include "domain/EmberEntity.h"
#include "components/ogre/widgets/icons/IconManager.h"
#include "components/ogre/widgets/icons/Icon.h"

#include <Eris/TypeInfo.h>

#include <CEGUI/WindowManager.h>
#include <CEGUI/WindowFactoryManager.h>
#include <CEGUI/RenderedStringWordWrapper.h>
#include <CEGUI/LeftAlignedRenderedString.h>

namespace Ember {
namespace OgreView {
namespace Gui {

EmberEntityTooltipWidget::EmberEntityTooltipWidget(const CEGUI::String& type, const CEGUI::String& name) :
		CEGUI::Tooltip(type, name) {
}

CEGUI::Sizef EmberEntityTooltipWidget::getTextSize_impl() const {
	//The first window is the text window.
	//We need to measure how much vertical space the text window would take, and then use that (or 80 pixels) as the height of the tooltip window.
	auto textWindow = getChildAtIdx(1);
	CEGUI::RenderedStringWordWrapper<CEGUI::LeftAlignedRenderedString> wordWrapper(textWindow->getRenderedString());
	auto pixelSize = textWindow->getPixelSize();
	wordWrapper.format(textWindow, CEGUI::Sizef(pixelSize.d_width, 1000));
	return CEGUI::Sizef(180, std::max(wordWrapper.getVerticalExtent(textWindow), 80.0f));
}

const CEGUI::String EmberEntityTooltipWidget::WidgetTypeName("Ember/EntityTooltip");

void EntityTooltip::registerFactory() {
	CEGUI::WindowFactoryManager::addFactory<CEGUI::TplWindowFactory<EmberEntityTooltipWidget>>();
}

void EntityTooltip::deregisterFactory() {
	CEGUI::WindowFactoryManager::getSingleton().removeFactory(EmberEntityTooltipWidget::WidgetTypeName);
}

EntityTooltip::EntityTooltip(World& world, UniqueWindowPtr<EmberEntityTooltipWidget> tooltip, Icons::IconManager& iconManager) :
		mWorld(world),
		mTooltip(std::move(tooltip)),
		mIconManager(iconManager),
		isUpdatingOurselves(false),
		mImageWindow(nullptr),
		mTextWindow(nullptr) {
	mTooltip->setDestroyedByParent(false);
	mImageWindow = CEGUI::WindowManager::getSingleton().createWindow("EmberLook/StaticImage");
	mImageWindow->setSize(CEGUI::USize(CEGUI::UDim(0, 64), CEGUI::UDim(0, 64)));
	mImageWindow->setProperty("FrameEnabled", "false");
	mImageWindow->setProperty("BackgroundEnabled", "false");
	mTooltip->addChild(mImageWindow);

	mTextWindow = CEGUI::WindowManager::getSingleton().createWindow("EmberLook/StaticText");
	mTextWindow->setSize(CEGUI::USize(CEGUI::UDim(1, -64), CEGUI::UDim(1, 0)));
	mTextWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 64), CEGUI::UDim(0, 0)));
	mTextWindow->setProperty("HorzFormatting", "WordWrapLeftAligned");
	mTextWindow->setProperty("VertFormatting", "TopAligned");
	mTextWindow->setProperty("FrameEnabled", "false");
	mTextWindow->setProperty("BackgroundEnabled", "false");
	mTooltip->addChild(mTextWindow);

	//listen to the text being changed, since that indicates that the tooltip has been activated for a new window (alas there's no signal when the tooltip is attached to a target window, so this is the best we can do)
	mTooltip->subscribeEvent(CEGUI::Window::EventTextChanged, CEGUI::Event::Subscriber(&EntityTooltip::tooltip_TextChanged, this));
}

EntityTooltip::~EntityTooltip()
= default;

CEGUI::Tooltip& EntityTooltip::getTooltipWindow() const {
	return *mTooltip;
}

bool EntityTooltip::tooltip_TextChanged(const CEGUI::EventArgs& e) {
	//Check the guard so we don't end up in an infinite loop, as we'll be resetting the text of the tooltip.
	if (!isUpdatingOurselves) {
		EmberEntity* entity = getActiveEntity();
		if (entity) {
			isUpdatingOurselves = true;
			mTextWindow->setText(composeEntityInfoText(*entity));
			Icons::Icon* icon = mIconManager.getIcon(64, entity);
			if (icon) {
				mImageWindow->setProperty("Image", CEGUI::PropertyHelper<CEGUI::Image*>::toString(icon->getImage()));
			} else {
				mImageWindow->setProperty("Image", "");
			}
			mTooltip->positionSelf();
		}
		mTooltip->setText(""); //The text has contained the id of the entity and should now be removed.
	}
	isUpdatingOurselves = false;
	return true;
}

std::string EntityTooltip::composeEntityInfoText(EmberEntity& entity) {
	std::stringstream ss;
	if (!entity.getName().empty()) {
		ss << entity.getName() << " (of type " << entity.getType()->getName() << ")";
	} else {
		ss << entity.getType()->getName();
	}
	auto amountPtr = entity.ptrOfProperty("amount");
	if (amountPtr && amountPtr->isInt() && amountPtr->Int() > 1) {
		ss << std::endl << "Amount: " << amountPtr->Int();
	}
	if (entity.hasProperty("worn")) {
		const Atlas::Message::Element& element = entity.valueOfProperty("worn");
		if (element.isString()) {
			ss << std::endl << "Worn on the " << element.asString();
		}
	}
	for (auto& entry : entity.getUsages()) {
		ss << std::endl << "Can be used to " << entry.first;
	}
	auto description = entity.ptrOfProperty("description");
	if (description && description->isString()) {
		ss << std::endl << description->String();
	}

	return ss.str();
}

EmberEntity* EntityTooltip::getActiveEntity() {
	if (mTooltip->getTargetWindow()) {
		return mWorld.getEmberEntity(mTooltip->getTargetWindow()->getTooltipText().c_str());
	}
	return nullptr;
}

}
}
}
