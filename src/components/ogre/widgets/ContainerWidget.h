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

#include <sigc++/connection.h>
#include "Widget.h"
#include "EntityIconSlot.h"

namespace Ember {
class EmberEntity;
namespace OgreView {
class GUIManager;
namespace Gui {

class ContainerWidget {
public:
	ContainerWidget(GUIManager& guiManager, EmberEntity& entity);

	~ContainerWidget();

	static void registerWidget(GUIManager& guiManager);

	void showEntityContents(const EmberEntity* entity);

protected:
	GUIManager& mGuiManager;
	Widget* mWidget;
	CEGUI::Window* mIconContainer;
	std::vector<std::unique_ptr<EntityIconSlot>> mSlots;
	std::vector<std::unique_ptr<EntityIcon>> mIcons;
	sigc::connection mActionConnection;

	EntityIconSlot* addSlot();

	EntityIconSlot* getFreeSlot();

	EntityIcon* createEntityIcon(EmberEntity* entity);

	void clearShownContent();


};
}
}
}


#endif //EMBER_CONTAINERWIDGET_H
