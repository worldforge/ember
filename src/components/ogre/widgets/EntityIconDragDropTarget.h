//
// C++ Interface: EntityIconDragDropTarget
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@iteam.se>, (C) 2007
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
	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
*/
class EntityIconDragDropTarget
{
public:
	EntityIconDragDropTarget(CEGUI::Window* container);
	virtual ~EntityIconDragDropTarget();

	sigc::signal<void, EntityIcon*> EventIconEntered;
	sigc::signal<void, EntityIcon*> EventIconLeaves;
	sigc::signal<void, EntityIcon*> EventIconDropped;
protected:
	virtual bool handleDragEnter(const CEGUI::EventArgs& args, EntityIcon* icon);
	virtual bool handleDragLeave(const CEGUI::EventArgs& args, EntityIcon* icon);
	virtual bool handleDragDropped(const CEGUI::EventArgs& args, EntityIcon* icon);

private:
	bool dragContainer_DragEnter(const CEGUI::EventArgs& args);
	bool dragContainer_DragLeave(const CEGUI::EventArgs& args);
	bool dragContainer_DragDropped(const CEGUI::EventArgs& args);
	
	EntityIcon* parseIcon(const CEGUI::EventArgs& args);

};

}

}

#endif
