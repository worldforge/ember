//
// C++ Interface: EntityIcon
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2007
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
#ifndef EMBEROGRE_GUIENTITYICON_H
#define EMBEROGRE_GUIENTITYICON_H


#include "icons/Icon.h"
#include "EntityIconManager.h"
#include "EntityIconDragDropTarget.h"

namespace CEGUI {
class DragContainer;
class Window;
class EventArgs;
}

namespace EmberOgre {
class EmberEntity;
namespace Gui {

class EntityIconSlot;
class EntityIcon;

/**
	@brief A very lightweight class which can be used as a "user data" instance in CEGUI.
	CEGUI allows for "user data" to be attached to windows. The user data can be anything, and this class is used as a means to bind an EntityIcon to a CEGUI::Window. Whenever it's used it must be correctly casted (since the user data stored by CEGUI::Window is just a void pointer.
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class EntityIconUserData
{
friend class EntityIcon;
public:
	
	/**
	 * @brief Gets the entity icon instance to which this user data instance belongs.
	 * @return The entity icon to which to which this user data belongs.
	 */
	EntityIcon& getEntityIcon();
private:
	/**
	 * @brief Ctor.
	 * @param entityIcon The entity icon to which to which this user data belongs.
	 */
	EntityIconUserData(EntityIcon& entityIcon);
	
	/**
	@brief The entity icon to which to which this user data belongs.
	*/
	EntityIcon& mEntityIcon;
};


/**
	@brief An entity icon, representing a specific entity.
	An entity icon is a draggable element, which can be dragged and dropped onto different slots.
	The most common slot on which it can be dragged and dropped would be an instance of EntityIconSlot. An entity icon doesn't necessarily have to be connected to a slot, but in most cases they are.
	
	Additionally, another entity icon can be dragged and dropped onto an instance of this class. When this happens the EvenIcon* events are emitted. This allows you to implement functionality for combining different entities.
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class EntityIcon : public EntityIconDragDropTarget
{
friend class EntityIconManager;
public:
	
	/**
	 * @brief Gets the CEGUI image which shows the entity. This can in some cases be a 3d render of the icon, or a prerendered texture. The actual handling of this is often taken care of by the mIcon instance.
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
	 * @brief Sets the slot to which this entity icon is attached. This will make the entity icon appear within the slot.
	 * Calling this on an entity which is already connected to a slot will make the icon first disconnect from that slot.
	 * @param slot The slot to which the entity icon should be connected.
	 */
	void setSlot(EntityIconSlot* slot);
	
	/**
	 * @brief Gets the icon entity slot to which this entity currently is attached to.
	 * @return The entity slot to which this entity icon is attached, or null if it's not connected to any slot.
	 */
	EntityIconSlot* getSlot();
	
	/**
	 * @brief Sets the tool tip for this entity icon.
	 * The tooltip will be displayed when the user hovers the mouse over the icon.
	 * @param text A text to show when the user hovers the mouse over the icon.
	 */
	void setTooltipText(const std::string& text);
	
	/**
	 * @brief Gets the entity which this icon represents.
	 * @return The entity which this icon represents.
	 */
	EmberEntity* getEntity();

	/**
	 * @brief Emitted when another EntityIcon instance is dragged and enters this icon.
	 */
	sigc::signal<void, EntityIcon*> EventIconEntered;
	
	/**
	 * @brief Emitted when another EntityIcon instance is dragged and leaves this icon.
	 */
	sigc::signal<void, EntityIcon*> EventIconLeaves;
	
	/**
	 * @brief Emitted when another EntityIcon instance is dragged and dropped on this icon.
	 */
	sigc::signal<void, EntityIcon*> EventIconDropped;
	
	/**
	 * @brief The main reason for having this is an issue in the lua bindings where comparisons of two EntityIcon instances always generated a lua error.
	 * @param value Another object to compare to.
	 * @return True if the objects are identical.
	 */
	bool operator==(const EntityIcon& value) {return this == &value;}
	
protected:
	/**
	 * @brief Ctor. This is normally only called from the EntityIconManager.
	 * @param manager The icon manager to which this entity belongs.
	 * @param dragContainer The CEGUI drag container instance which provides drag and drop behavior for this entity icon. Ownership will not be transferred.
	 * @param image The image which represents the entity. In many cases this will be the same image as provided by the icon parameter. Ownership is not transferred.
	 * @param icon The Icon instance responsible for providing the image. Ownership is not transferred.
	 * @param entity The entity to which this icon belongs. Ownership is not transferred.
	 */
	EntityIcon(EntityIconManager& manager, CEGUI::DragContainer* dragContainer, CEGUI::Window* image, Gui::Icons::Icon* icon, EmberEntity* entity);

	/**
	 * @brief Dtor.
	 *
	 * If the ícon is attached to a slot it will be detached at destruction.
	 */
	virtual ~EntityIcon();
	
	/**
	 * @brief The main entity icon manager.
	 */
	EntityIconManager& mManager;

	/**
	 * @brief The CEGUI drag container instance which provides drag and drop behavior for this entity icon.
	 */
	CEGUI::DragContainer* mDragContainer;

	/**
	 * @brief The image which represents the entity. In many cases this will be the same image as provided by the icon parameter.
	 */
	CEGUI::Window* mImage;

	/**
	 * @brief The Icon instance responsible for providing the image.
	 */
	Gui::Icons::Icon* mIcon;

	/**
	 * @brief The user data attached to the window which binds it to this instance.
	 */
	EntityIconUserData mUserData;

	/**
	 * @brief If the icon is attached to any slot it's stored here.
	 */
	EntityIconSlot* mCurrentSlot;

	/**
	 * @brief The entity which the icon represents.
	 */
	EmberEntity* mEntity;
	
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
	
	virtual bool handleDragEnter(const CEGUI::EventArgs& args, EntityIcon* icon);
	virtual bool handleDragLeave(const CEGUI::EventArgs& args, EntityIcon* icon);
	virtual bool handleDragDropped(const CEGUI::EventArgs& args, EntityIcon* icon);

};

}

}

#endif
