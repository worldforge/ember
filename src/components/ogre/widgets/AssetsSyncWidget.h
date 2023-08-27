/*
 Copyright (C) 2023 Erik Ogenvik

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

#ifndef EMBER_ASSETSSYNCWIDGET_H
#define EMBER_ASSETSSYNCWIDGET_H

#include "Widget.h"
#include "WidgetPlugin.h"
#include "services/server/ServerServiceSignals.h"
#include "framework/AutoCloseConnection.h"

namespace Ember {
class AdminEntityCreator;
namespace OgreView {


class GUIManager;
namespace Gui {
class AssetsSyncWidget : public virtual sigc::trackable, public boost::noncopyable {
public:
	static WidgetPluginCallback registerWidget(Ember::OgreView::GUIManager& guiManager);

	AssetsSyncWidget(GUIManager& guiManager, AssetsSync assetsSync);

	~AssetsSyncWidget();

private:
	Widget* mWidget;

	std::vector<AutoCloseConnection> mConnections;

};

}
}
}

#ifdef WF_USE_WIDGET_PLUGINS
BOOST_DLL_ALIAS(
		Ember::OgreView::Gui::AssetsSyncWidget::registerWidget,
		registerWidget
)
#endif

#endif //EMBER_ASSETSSYNCWIDGET_H
