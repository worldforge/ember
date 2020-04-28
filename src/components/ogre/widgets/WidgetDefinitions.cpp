//
// C++ Implementation: WidgetDefinitions
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2005
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


#include "WidgetDefinitions.h"

#include "ServerWidget.h"
#include "InspectWidget.h"
#include "IngameChatWidget.h"
#include "Help.h"
#include "Quit.h"
#include "MeshPreview.h"
#include "ContainerWidget.h"

namespace Ember
{
namespace OgreView
{
namespace Gui {

WidgetDefinitions::WidgetDefinitions()
{
	WidgetLoader::registerWidgetFactory("Widget", &WidgetLoader::createWidgetInstance<Widget>);
	WidgetLoader::registerWidgetFactory("ServerWidget", &WidgetLoader::createWidgetInstance<ServerWidget>);
	WidgetLoader::registerWidgetFactory("InspectWidget", &WidgetLoader::createWidgetInstance<InspectWidget>);
	WidgetLoader::registerWidgetFactory("IngameChatWidget", &WidgetLoader::createWidgetInstance<Gui::IngameChatWidget>);
	WidgetLoader::registerWidgetFactory("Help", &WidgetLoader::createWidgetInstance<Help>);
	WidgetLoader::registerWidgetFactory("Quit", &WidgetLoader::createWidgetInstance<Quit>);
	WidgetLoader::registerWidgetFactory("MeshPreview", &WidgetLoader::createWidgetInstance<MeshPreview>);



}

void WidgetDefinitions::registerWidgets(GUIManager& guiManager) {
	ContainerWidget::registerWidget(guiManager);
}

}
}
}
