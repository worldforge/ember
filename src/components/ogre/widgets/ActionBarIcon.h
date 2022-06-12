//
// C++ Interface: ActionBarIcon
//
// Description:
//
//	Author Tiberiu Paunescu <tpa12@sfu.ca>, (C) 2010
//	Based on the EntityIcon class by Erik Ogenvik <erik@ogenvik.org>, (C) 2007
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
#ifndef EMBEROGRE_GUIACTIONBARICON_H
#define EMBEROGRE_GUIACTIONBARICON_H


#include "icons/Icon.h"
#include "ActionBarIconManager.h"
#include "ActionBarIconDragDropTarget.h"
#include "ActionBarIconSlot.h"
#include "GenericIconUserData.h"
#include "CEGUIUtils.h"
#include <sigc++/trackable.h>
#include <boost/any.hpp>

namespace CEGUI {
class DragContainer;

class Window;

class EventArgs;
}

namespace Ember {
class EmberEntity;
namespace OgreView {
namespace Gui {

class ActionBarIconSlot;

class ActionBarIcon;

/**
	@brief An action bar icon, representing a generic icon that exists on the action bars.
	An action icon is a draggable element, which can be dragged and dropped onto different slots.
	The most common slot on which it can be dragged and dropped would be an instance of ActionIconSlot. An action bar icon doesn't necessarily have to be connected to a slot, but in most cases they are.

	Additionally, another action bar icon can be dragged and dropped onto an instance of this class. When this happens the EventIcon* events are emitted. This allows you to implement functionality for combining different entities.
	@author Erik Ogenvik <erik@ogenvik.org>
*/
class ActionBarIcon : public ActionBarIconDragDropTarget, public virtual sigc::trackable {
	friend class ActionBarIconManager;

public:
	/**
	 * @brief Ctor. This is normally only called from the EntityIconManager.
	 * @param manager The icon manager to which this entity belongs.
	 * @param dragContainer The CEGUI drag container instance which provides drag and drop behavior for this entity icon. Ownership will not be transferred.
	 * @param image The image which represents the entity. In many cases this will be the same image as provided by the icon parameter. Ownership is not transferred.
	 * @param icon The Icon instance responsible for providing the image. Ownership is not transferred.
	 */
	ActionBarIcon(ActionBarIconManager& manager, UniqueWindowPtr<CEGUI::DragContainer> dragContainer, UniqueWindowPtr<CEGUI::Window> image, Gui::Icons::Icon* icon);

	/**
	 * @brief Dtor.
	 *
	 * If the ícon is attached to a slot it will be detached at destruction.
	 */
	~ActionBarIcon() override;


	virtual void defaultAction();

	/**
	 * @brief Gets the CEGUI image which shows the object represented. This can in some cases be a 3d render of the icon, or a prerendered texture. The actual handling of this is often taken care of by the mIcon instance.
	 * @return The image displaying the entity in the CEGUI system.
	 */
	CEGUI::Window* getImage();

	/**
	 * @brief Gets the CEGUI drag container which allows this entity to be dragged and dropped.
	 * @return The CEGUI drag container instance for this entity icon.
	 */
	CEGUI::DragContainer* getDragContainer();

	/**
	 * @brief Gets the icon instance which is responsible for providing the graphical representation (for example a live render, or a prerendered texture).
	 * @return The icon instance responsible for providing the graphical representation of the entity.
	 */
	Gui::Icons::Icon* getIcon();

	/**
	 * @brief Sets the slot to which this action bar icon is attached. This will make the action bar icon appear within the slot.
	 * Calling this on an action bar icon which is already connected to a slot will make the icon first disconnect from that slot.
	 * @param slot The slot to which the entity icon should be connected.
	 */
	void setSlot(ActionBarIconSlot* slot);

	/**
	 * @brief Gets the icon action bar slot to which this entity currently is attached to.
	 * @return The action bar slot to which this action bar icon is attached, or null if it's not connected to any slot.
	 */
	ActionBarIconSlot* getSlot();

	/**
	 * @brief Sets the tool tip for this action bar icon.
	 * The tooltip will be displayed when the user hovers the mouse over the icon.
	 * @param text A text to show when the user hovers the mouse over the icon.
	 */
	void setTooltipText(const std::string& text);

	/**
	 * @brief Emitted when another ActionBarIcon instance is dragged and enters this icon.
	 */
	sigc::signal<void, ActionBarIcon*> EventIconEntered;

	/**
	 * @brief Emitted when another action instance is dragged and leaves this icon.
	 */
	sigc::signal<void, ActionBarIcon*> EventIconLeaves;

	/**
	 * @brief Emitted when another action instance is dragged and dropped on this icon.
	 */
	sigc::signal<void, ActionBarIcon*> EventIconDropped;

	/**
	 * @brief The main reason for having this is an issue in the lua bindings where comparisons of two ActionBar instances always generated a lua error.
	 * @param value Another object to compare to.
	 * @return True if the objects are identical.
	 */
	bool operator==(const ActionBarIcon& value) { return this == &value; }

	/**
	 * @brief Handler for CEGUI drag start event.
	 * @param args Event args.
	 */
	bool dragContainer_DragStarted(const CEGUI::EventArgs& args);

	/**
	 * @brief Handler for CEGUI drag stop event.
	 * @param args Event args.
	 */
	bool dragContainer_DragStopped(const CEGUI::EventArgs& args);

	/**
	 * @brief Handler for CEGUI drag enter event.
	 * @param args Event args.
	 */
	bool handleDragEnter(const CEGUI::EventArgs& args, ActionBarIcon* icon) override;

	/**
	 * @brief Handler for CEGUI drag enter event.
	 * @param args Event args.
	 */
	bool handleDragLeave(const CEGUI::EventArgs& args, ActionBarIcon* icon) override;

	/**
	 * @brief Handler for dropping an existing action bar icon onto this icon.
	 * @param args Event args.
	 * @param icon The action bar icon.
	 * @note Event is passed to the slot the icon belongs to.
	 */
	bool handleDragActionBarIconDropped(const CEGUI::EventArgs& args, ActionBarIcon* icon) override;

	/**
	 * @brief Handler for dropping an existing entity icon onto this icon.
	 * @param args Event args.
	 * @param icon The entity icon.
	 * @note Event is passed to the slot the icon belongs to.
	 */
	bool handleDragEntityIconDropped(const CEGUI::EventArgs& args, EntityIcon* icon) override;

	void icon_Updated();

protected:


	/**
	 * @brief The main entity icon manager.
	 */
	ActionBarIconManager& mManager;

	/**
	 * @brief The CEGUI drag container instance which provides drag and drop behavior for this entity icon.
	 */
	UniqueWindowPtr<CEGUI::DragContainer> mDragContainer;

	/**
	 * @brief The image which represents the entity. In many cases this will be the same image as provided by the icon parameter.
	 */
	UniqueWindowPtr<CEGUI::Window> mImage;

	/**
	 * @brief The Icon instance responsible for providing the image.
	 */
	Gui::Icons::Icon* mIcon;

	/**
	 * @brief The user data attached to the window which binds it to this instance.
	 */
	GenericIconUserData<ActionBarIcon> mUserData;

	/**
	 * @brief Holds the mUserData and is attached to the CEGUI::DragContainer.
	 *
	 * This allows us to better query for type, as the boost::any class stores the type id of the wrapped object.
	 */
	boost::any mUserDataWrapper;

	/**
	 * @brief If the icon is attached to any slot it's stored here.
	 */
	ActionBarIconSlot* mCurrentSlot;

};

}

}

}

#endif
