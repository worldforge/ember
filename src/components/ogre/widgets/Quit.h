//
// C++ Interface: Quit
//
// Description: 
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2005
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
#ifndef EMBEROGREQUIT_H
#define EMBEROGREQUIT_H

#include "Widget.h"
#include "WidgetPlugin.h"

namespace Ember {
namespace OgreView {
namespace Gui {

/**
@author Erik Ogenvik
*/
class Quit : public Widget {
public:
	explicit Quit(GUIManager& guiManager);

	~Quit() override;

	void runCommand(const std::string& command, const std::string& args) override;

	const ConsoleCommandWrapper SoftQuit;


	/**
	 *    overloaded to remove the modal state
	 */
	void hide() override;

protected:

	bool Shutdown_Click(const CEGUI::EventArgs& args);

	bool Logout_Click(const CEGUI::EventArgs& args);

	bool Cancel_Click(const CEGUI::EventArgs& args);

	void EmberOgre_RequestQuit(bool& handled);

	void softquit();

};
}
}
}

PLUGIN_API WidgetPluginCallback registerWidget(Ember::OgreView::GUIManager& guiManager);

#endif
