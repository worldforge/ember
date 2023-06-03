//
// C++ Interface: ActionBarIconDragDropTarget
//
// Description:
//
//	Author Tiberiu Paunescu <tpa12@sfu.ca>, (C) 2010
//	Based on the EntityIconDragDropTarget class by Erik Ogenvik <erik@ogenvik.org>, (C) 2007
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
#ifndef EMBEROGRE_GUIACTIONBARICONDRAGDROPTARGET_H
#define EMBEROGRE_GUIACTIONBARICONDRAGDROPTARGET_H

#include <sigc++/signal.h>

namespace boost
{
	class any;
}

namespace CEGUI
{
	class Window;
	class EventArgs;
}

namespace Ember {
namespace OgreView {

namespace Gui {

class ActionBarIcon;
class EntityIcon;

/**
@author Erik Ogenvik <erik@ogenvik.org>

@brief Attaches to a CEGUI::Window and allows that window to have EntityIcons and ActionBarIcons dragged and dropped on it.

An instance of this class will attach to a CEGUI::Window and listen to drag and drop events. When an entity icon or action bar icon is dragged onto it events are signalled.
Use this as an easy way to activate entity icon or action bar icon dragging onto arbitrary CEGUI windows.

@note Subclasses of this which want to implement their own behaviour should override the handle* methods.
*/
class ActionBarIconDragDropTarget
{
public:
	/**
	 * @brief Ctor.
	 * @param container The window which will be able to have entity icons and action bar icons dragged and dropped on it.
	 *
	 */
	explicit ActionBarIconDragDropTarget(CEGUI::Window* container);

	/**
	 * @brief Dtor.
	 */
	virtual ~ActionBarIconDragDropTarget();

	/**
	 * @brief Emitted when an action bar icon is dragged over the window.
	 */
	sigc::signal<void(ActionBarIcon*)> EventIconEntered;

	/**
	 * @brief Emitted when an action bar icon is dragged off the window.
	 */
	sigc::signal<void(ActionBarIcon*)> EventIconLeaves;

	/**
	 * @brief Emitted when an action bar icon is dropped on window.
	 */
	sigc::signal<void(ActionBarIcon*)> EventActionBarIconDropped;

	/**
	 * @brief Emitted when an entity icon is dropped on window.
	 */
	sigc::signal<void(EntityIcon*)> EventEntityIconDropped;

protected:

	/**
	 * @brief Handle an action bar icon being dragged onto the window.
	 * @param args Event args.
	 * @param icon The action bar icon being dragged.
	 */
	virtual bool handleDragEnter(const CEGUI::EventArgs& args, ActionBarIcon* icon);

	/**
	 * @brief Handle an action bar icon being dragged off the window.
	 * @param args Event args.
	 * @param icon The action bar icon being dragged.
	 */
	virtual bool handleDragLeave(const CEGUI::EventArgs& args, ActionBarIcon* icon);

	/**
	 * @brief Handle an action bar icon being dropped onto the window.
	 * @param args Event args.
	 * @param icon The action bar icon being dropped.
	 */
	virtual bool handleDragActionBarIconDropped(const CEGUI::EventArgs& args, ActionBarIcon* icon);

	/**
	 * @brief Handle an entity icon being dropped onto the window.
	 * @param args Event args.
	 * @param icon The entity icon being dropped.
	 */
	virtual bool handleDragEntityIconDropped(const CEGUI::EventArgs& args, EntityIcon* icon);

private:

	/**
	 * @brief Listener for CEGUI event.
	 * @param args Event args.
	 */
	bool dragContainer_DragEnter(const CEGUI::EventArgs& args);

	/**
	 * @brief Listener for CEGUI event.
	 * @param args Event args.
	 */
	bool dragContainer_DragLeave(const CEGUI::EventArgs& args);

	/**
	 * @brief Listener for CEGUI event.
	 * @param args Event args.
	 */
	bool dragContainer_DragDropped(const CEGUI::EventArgs& args);

	/**
	 * @brief Inspect the event args and extract the entity icon instance.
	 * @param args Event args.
	 * @returns An entity icon instance, or null if none could be found.
	 */
	ActionBarIcon* parseIcon(const CEGUI::EventArgs& args);

	/**
	 * @brief Based on the events args, extract the user data
	 * @param args Event args.
	 * @returns User data attached to the window in the form of a boost::any class.
	 * @note boost::any allows us to determine the type of icon dropped
	 */
	const boost::any* getUserData(const CEGUI::EventArgs& args) const;

};

}

}

}

#endif
