//
// C++ Interface: EntityIconDragDropTarget
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
#ifndef EMBEROGRE_GUIENTITYICONDRAGDROPTARGET_H
#define EMBEROGRE_GUIENTITYICONDRAGDROPTARGET_H

#include <sigc++/signal.h>

namespace CEGUI
{
	class Window;
	class EventArgs;
}

namespace EmberOgre {

namespace Gui {

class EntityIcon;

/**
@author Erik Hjortsberg <erik.hjortsberg@gmail.com>

@brief Attaches to a CEGUI::Window and allows that window to have EntityIcons dragged and dropped on it.

An instance of this class will attach to a CEGUI::Window and listen to drag and drop events. When an entity icon is dragged onto it events are signalled.
Use this as an easy way to activate entity icon dragging onto arbitrary CEGUI windows.

@note Subclasses of this which want to implement their own behaviour should override the handle* methods.
*/
class EntityIconDragDropTarget
{
public:
	/**
	 * @brief Ctor.
	 * @param container The window which will be able to have entity icons dragged and dropped on it.
	 *
	 */
	EntityIconDragDropTarget(CEGUI::Window* container);

	/**
	 * @brief Dtor.
	 */
	virtual ~EntityIconDragDropTarget();

	/**
	 * @brief Emitted when an entity icon is dragged over the window.
	 */
	sigc::signal<void, EntityIcon*> EventIconEntered;

	/**
	 * @brief Emitted when an entity icon is dragged off the window.
	 */
	sigc::signal<void, EntityIcon*> EventIconLeaves;

	/**
	 * @brief Emitted when an entity icon is dropped on window.
	 */
	sigc::signal<void, EntityIcon*> EventIconDropped;

protected:

	/**
	 * @brief Handle an entity icon being dragged onto the window.
	 * @param args Event args.
	 * @param icon The entity icon being dragged.
	 */
	virtual bool handleDragEnter(const CEGUI::EventArgs& args, EntityIcon* icon);

	/**
	 * @brief Handle an entity icon being dragged off the window.
	 * @param args Event args.
	 * @param icon The entity icon being dragged.
	 */
	virtual bool handleDragLeave(const CEGUI::EventArgs& args, EntityIcon* icon);

	/**
	 * @brief Handle an entity icon being dropped onto the window.
	 * @param args Event args.
	 * @param icon The entity icon being dropped.
	 */
	virtual bool handleDragDropped(const CEGUI::EventArgs& args, EntityIcon* icon);

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
	EntityIcon* parseIcon(const CEGUI::EventArgs& args);

};

}

}

#endif
