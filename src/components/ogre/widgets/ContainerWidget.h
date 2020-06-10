/*
 Copyright (C) 2020 Erik Ogenvik

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

#ifndef EMBER_CONTAINERWIDGET_H
#define EMBER_CONTAINERWIDGET_H

#include "Widget.h"
#include "WidgetPlugin.h"
#include "EntityIconSlot.h"
#include "ContainerView.h"

#include <sigc++/connection.h>

namespace Ember {
class EmberEntity;
namespace OgreView {
class GUIManager;
namespace Gui {


class ContainerWidget {
public:
	ContainerWidget(GUIManager& guiManager, EmberEntity& entity, int slotSize = 32);

	~ContainerWidget();
	static WidgetPluginCallback registerWidget(Ember::OgreView::GUIManager& guiManager);

protected:
	GUIManager& mGuiManager;
	int mSlotSize;
	Widget* mWidget;
	std::unique_ptr<ContainerView> mContainerView;


};
}
}
}
#ifdef WF_USE_WIDGET_PLUGINS

BOOST_DLL_ALIAS(
		Ember::OgreView::Gui::ContainerWidget::registerWidget,
		registerWidget
)
#endif

#endif //EMBER_CONTAINERWIDGET_H
