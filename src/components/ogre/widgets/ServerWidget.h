//
// C++ Interface: ServerWidget
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2004
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
#ifndef DIMEOGRESERVERWIDGET_H
#define DIMEOGRESERVERWIDGET_H

#include <Eris/ServerInfo.h>
#include <Eris/Utils.h>
#include <Eris/Connection.h>
#include <Eris/Avatar.h>
#include <Eris/Player.h>

#include <Atlas/Objects/Entity/GameEntity.h>
#include <Atlas/Message/Element.h>



namespace DimeOgre {

class GUIManager;
/**
@author Erik Hjortsberg
*/
class ServerWidget : public Widget
{
public:
    ServerWidget(GUIManager* guiManager);

    ~ServerWidget();
	void buildWidget();

protected:

	Eris::Player* mPlayer;
	CEGUI::Listbox* mCharacterList;

	void createdPlayer(Eris::Player* player);
	void gotAvatar(Eris::Avatar* avatar);
	
	bool Login_Click(const CEGUI::EventArgs& args);
	bool Choose_Click(const CEGUI::EventArgs& args);
	
	void loginSuccess(Eris::Player* player);
	void gotAllCharacters(Eris::Player* player);



};

};

#endif
