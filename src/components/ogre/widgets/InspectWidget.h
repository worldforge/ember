//
// C++ Interface: InspectWidget
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
#ifndef DIMEOGREINSPECTWIDGET_H
#define DIMEOGREINSPECTWIDGET_H


namespace EmberOgre {

class GUIManager;
class Widget;
/**
@author Erik Hjortsberg
*/
class InspectWidget : public Widget
{
public:
    InspectWidget(GUIManager* guiManager);

    ~InspectWidget();
	void buildWidget();

protected:
	CEGUI::Listbox* mChildList;
	CEGUI::StaticText* mInfo;
	void showEntityInfo(EmberEntity* entity);
	
	void handleAction(const std::string& action, EmberEntity* entity);


};

};

#endif
