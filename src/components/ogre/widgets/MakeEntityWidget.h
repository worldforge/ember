//
// C++ Interface: MakeEntityWidget
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
#ifndef DIMEOGREMAKEENTITYWIDGET_H
#define DIMEOGREMAKEENTITYWIDGET_H


#include <Eris/TypeService.h>
#include <Eris/Connection.h>
#include <Eris/TypeInfo.h>
#include <Eris/Avatar.h>
#include <Eris/View.h>


namespace EmberOgre {

class AvatarEmberEntity;
/**
@author Erik Hjortsberg
*/
class MakeEntityWidget : public Widget
{
public:

    ~MakeEntityWidget();
	void buildWidget();
protected:

	std::set<Eris::TypeInfo*> mTypes;
	
	CEGUI::Listbox* mTypeList;
	CEGUI::Editbox* mName;
	
	Eris::Connection* mConn;
	
	void gotAvatar(Eris::Avatar* avatar);
	void connectedToServer(Eris::Connection* conn);
	void boundAType(Eris::TypeInfo* typeInfo);
	
	bool createButton_Click(const CEGUI::EventArgs& args);
	
	/**
	Loads all types into the list
	*/
	void loadAllTypes();
	
	void addToList(Eris::TypeInfo* typeInfo, int level);


};

};

#endif
