//
// C++ Interface: InspectWidget
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2004
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
#ifndef EMBEROGREINSPECTWIDGET_H
#define EMBEROGREINSPECTWIDGET_H

#include "Widget.h"
#include "WidgetPlugin.h"
#include <Eris/Types.h>
#include <sigc++/connection.h>

namespace Ember {
class EmberEntity;
namespace OgreView {
class GUIManager;
namespace Gui {


class Widget;

/**
@author Erik Ogenvik
*/
class InspectWidget : public Widget {
public:
	static WidgetPluginCallback registerWidget(Ember::OgreView::GUIManager& guiManager);

	explicit InspectWidget(GUIManager& guiManager);

	~InspectWidget() override;

	void frameStarted(float timeSinceLastFrame);

	const ConsoleCommandWrapper Inspect;


	/**
	 *    reimplement ConsoleObject::runCommand to catch the "inspect $entityid" command
	 * @param command
	 * @param args
	 */
	void runCommand(const std::string& command, const std::string& args) override;

protected:


	void fillChildrenList();

	void entity_ChildAdded(Eris::Entity* entity);

	void entity_ChildRemoved(Eris::Entity* entity);

	void addChildToList(Eris::Entity* child);

	void disconnectFromEntity();

	/**
	 *    Starts inspecting an entity
	 * @param entity
	 */
	void startInspecting(EmberEntity* entity);

	CEGUI::Listbox* mChildList;
	CEGUI::Window* mInfo;

	/**
	 * Updated the entity information text
	 * @param entity
	 */
	void showEntityInfo(EmberEntity* entity);

	void handleAction(const std::string& action, EmberEntity* entity);

	/**
	 * @brief Listen to double clicks in the child list, and inspect the child if so.
	 * @param args
	 * @return
	 */
	bool ChildList_MouseDoubleClick(const CEGUI::EventArgs& args);

	EmberEntity* mCurrentEntity;

	void entity_BeingDeleted();

	void updateAttributeString();

	void entity_Changed(const std::set<std::string>& attributes);

	std::string mAttributesString;

	sigc::connection mChangedConnection;
	sigc::connection mChildAddedConnection;
	sigc::connection mChildRemovedConnection;
	sigc::connection mBeingDeletedConnection;

	/**
	 * @brief True if the entity has changed this frame, and the info should therefore be updated.
	 */
	bool mChangedThisFrame;

};
}
}
}

#ifdef WF_USE_WIDGET_PLUGINS

BOOST_DLL_ALIAS(
		Ember::OgreView::Gui::InspectWidget::registerWidget,
		registerWidget
)
#endif
#endif
