//
// C++ Implementation: WidgetDefinitions
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2005
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

#include "WidgetDefinitions.h"

// #include "ServerBrowserWidget.h"
#include "ServerWidget.h"
// #include "ChatWidget.h"
// #include "EntityPickerWidget.h"
#include "InventoryWidget.h"
#include "InspectWidget.h"
#include "MakeEntityWidget.h"
//#include "GiveWidget.h"
#include "DebugWidget.h"
#include "IngameChatWidget.h"
#include "ConsoleWidget.h"
// #include "Performance.h"
#include "JesusEdit.h"
#include "Help.h"
#include "Quit.h"
#include "StatusIconBar.h"
#include "MeshPreview.h"


#include "Widget.h"
namespace EmberOgre
{

WidgetDefinitions::WidgetDefinitions()
{
	WidgetLoader::registerWidget("Widget", &WidgetLoader::createWidgetInstance<Widget>);
// 	WidgetLoader::registerWidget("ServerBrowserWidget", &WidgetLoader::createWidgetInstance<ServerBrowserWidget>);
	WidgetLoader::registerWidget("ServerWidget", &WidgetLoader::createWidgetInstance<ServerWidget>);
// 	WidgetLoader::registerWidget("ChatWidget", &WidgetLoader::createWidgetInstance<ChatWidget>);
// 	WidgetLoader::registerWidget("EntityPickerWidget", &WidgetLoader::createWidgetInstance<EntityPickerWidget>);
	WidgetLoader::registerWidget("InventoryWidget", &WidgetLoader::createWidgetInstance<InventoryWidget>);
	WidgetLoader::registerWidget("InspectWidget", &WidgetLoader::createWidgetInstance<InspectWidget>);
	WidgetLoader::registerWidget("MakeEntityWidget", &WidgetLoader::createWidgetInstance<MakeEntityWidget>);
	//WidgetLoader::registerWidget("GiveWidget", &WidgetLoader::createWidgetInstance<GiveWidget>);
	WidgetLoader::registerWidget("DebugWidget", &WidgetLoader::createWidgetInstance<DebugWidget>);
	WidgetLoader::registerWidget("IngameChatWidget", &WidgetLoader::createWidgetInstance<IngameChatWidget>);
	WidgetLoader::registerWidget("ConsoleWidget", &WidgetLoader::createWidgetInstance<ConsoleWidget>);
// 	WidgetLoader::registerWidget("Performance", &WidgetLoader::createWidgetInstance<Performance>);
	WidgetLoader::registerWidget("JesusEdit", &WidgetLoader::createWidgetInstance<JesusEdit>);
	WidgetLoader::registerWidget("Help", &WidgetLoader::createWidgetInstance<Help>);
	WidgetLoader::registerWidget("Quit", &WidgetLoader::createWidgetInstance<Quit>);
	WidgetLoader::registerWidget("StatusIconBar", &WidgetLoader::createWidgetInstance<StatusIconBar>);
	WidgetLoader::registerWidget("MeshPreview", &WidgetLoader::createWidgetInstance<MeshPreview>);
	


}


WidgetDefinitions::~WidgetDefinitions()
{
}


}
