/*
 Copyright (C) 2011 Erik Ogenvik

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

#ifndef CURSORINACTIVELISTENER_H_
#define CURSORINACTIVELISTENER_H_

#include <CEGUIEvent.h>
#include <sigc++/trackable.h>

namespace CEGUI
{
class Window;
}

namespace Ember
{
class MainLoopController;

namespace OgreView
{
class MousePicker;
namespace Gui
{

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 *
 *
 */
class CursorInactiveListener : public sigc::trackable
{
public:
	CursorInactiveListener(MainLoopController& mainLoopController, CEGUI::Window& mainWindow, MousePicker& mousePicker);
	virtual ~CursorInactiveListener();

protected:

	CEGUI::Window& mMainWindow;
	MousePicker& mMousePicker;

	bool mHoverEventSent;

	long long mCursorLingerStart;

	CEGUI::Event::Connection mMouseMovesConnection;

	void inputAfterEventProcessing(float timeslice);

	bool windowMouseEnters(const CEGUI::EventArgs& args);
	bool windowMouseLeaves(const CEGUI::EventArgs& args);
	bool windowMouseMoves(const CEGUI::EventArgs& args);

	void sendHoverEvent();
};

}
}
}
#endif /* CURSORINACTIVELISTENER_H_ */
