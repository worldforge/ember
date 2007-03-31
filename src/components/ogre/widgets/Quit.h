//
// C++ Interface: Quit
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2005
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
#ifndef EMBEROGREQUIT_H
#define EMBEROGREQUIT_H

#include "Widget.h"

namespace EmberOgre {

/**
@author Erik Hjortsberg
*/
class Quit : public Widget
{
public:
    Quit();

    virtual ~Quit();
	
	virtual void buildWidget();
	virtual void runCommand(const std::string &command, const std::string &args);

	const Ember::ConsoleCommandWrapper SoftQuit;
	
	
	/**
	 *    overloaded to remove the modal state
	 */
	virtual void hide();
	
protected:
	
	bool Yes_Click(const CEGUI::EventArgs& args);
	bool No_Click(const CEGUI::EventArgs& args);
	
	void EmberOgre_RequestQuit(bool& handled);
	
	void softquit();

};

};

#endif
