//
// C++ Interface: StackableContainer
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
#ifndef EMBEROGRE_GUISTACKABLECONTAINER_H
#define EMBEROGRE_GUISTACKABLECONTAINER_H

#include <CEGUI.h>
#include <list>

namespace CEGUI {
class Window;
}

namespace EmberOgre {

namespace Gui {

/**
	Use this class with CEGUI windows to allow for automatic layout of child window.
	An instance of this will attach itself to a window and listen for children being added and removed, as well as resized.
	It will rearrange these child windows so that they are stacked, either vertically or horizontally.

	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class StackableContainer{
public:
	/**
	The flow direction of the child window.
	*/
	enum FlowDirection {
		Horizontal = 0,
		Vertical = 1
	};

	/**
	Default constructor.
	@param A valid window, which will have its children automatically arranged.
	*/
    StackableContainer(CEGUI::Window* window);

    virtual ~StackableContainer();
	
	/**
	Manually sets the inner container window. In some cases, such as with a ScrollablePane, this will differ from the main window.
	*/
	void setInnerContainerWindow(CEGUI::Window* window);
	
	/**
	Gets the window that this instance listens to.
	*/
	CEGUI::Window* getWindow();
	
	/**
	Sets how much to pad between each child, in pixels.
	*/
	void setPadding(int padding);
	/**
	Gets how much to pad between each child, in pixels.
	*/
	int getPadding() const;

	/**
	Utility method for getting the absolute height of the window.
	*/
	float getAbsoluteHeight();
	/**
	Utility method for getting the absolute width of the window.
	*/
	float getAbsoluteWidth();
	
	/**
	Sets the direction that children will flow.
	*/
	void setFlowDirection(FlowDirection flowDirection);
	/**
	Gets the direction that children will flow.
	*/
	FlowDirection getFlowDirection() const;
	
	/**
	Repositions all windows as they are added and removed to the list.
	*/
	void repositionWindows();
	
	/**
	Disconnects the listener, in effect deactivating the instance.
	*/
	void disconnect();

protected:

	/**
	Hold references to all event bindings, so we can properly clean them up.
	*/
	typedef std::map<CEGUI::Window*, CEGUI::Event::Connection> ConnectorStore;
	CEGUI::Event::Connection mChildRemovedConnection, mChildAddedConnection, mWindowDestructionConnection;
	ConnectorStore mChildConnections;
	
	/**
	We keep a reference to both the mainwindow and an inner container window. In most cases these will be the same, but when for example using the ScrollablePane, they might be different.
	*/
	CEGUI::Window* mWindow;
	CEGUI::Window* mInnerContainerWindow;
	
	
	/**
	In pixels, how much to pad between each window.
	*/
	int mPadding;
	
	/**
	The direction the windows will flow.
	*/
	FlowDirection mFlowDirection;
	
	bool window_ChildAdded(const CEGUI::EventArgs& e);
	bool window_ChildRemoved(const CEGUI::EventArgs& e);
	bool childwindow_Sized(const CEGUI::EventArgs& e);
	bool window_DestructionStarted(const CEGUI::EventArgs& e);
	
	/**
	Clean up all bindings.
	*/
	void cleanup();
	
};

}

}

#endif
