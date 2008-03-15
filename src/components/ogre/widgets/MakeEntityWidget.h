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
#ifndef EMBEROGRE_MAKEENTITYWIDGET_H
#define EMBEROGRE_MAKEENTITYWIDGET_H

#include "Widget.h"



namespace Eris
{
	class TypeInfo;
	class Connection;
	class Avatar;
}

namespace EmberOgre {

class AvatarEmberEntity;
namespace Gui {

class ModelRenderer;
/**
@author Erik Hjortsberg
*/
class MakeEntityWidget : public Widget
{
public:

	MakeEntityWidget();
    virtual ~MakeEntityWidget();
	virtual void buildWidget();

	
	virtual void show();

	const Ember::ConsoleCommandWrapper CreateEntity;
	const Ember::ConsoleCommandWrapper Make;
		
	virtual void runCommand(const std::string &command, const std::string &args);

protected:

	/**
	flag for showing when the widget is ready to be shown
	*/
	bool mIsReady;

	std::map<Eris::TypeInfo*, CEGUI::ListboxItem*> mTypes;
//	std::set<Eris::TypeInfo*> mTypes;
	
	CEGUI::Listbox* mTypeList;
	CEGUI::Editbox* mName;
	
	Eris::Connection* mConn;
	
	void gotAvatar(Eris::Avatar* avatar);
	void connectedToServer(Eris::Connection* conn);
	void boundAType(Eris::TypeInfo* typeInfo);
	
	bool createButton_Click(const CEGUI::EventArgs& args);
	
	bool typeList_ItemSelectionChanged(const CEGUI::EventArgs& args);
	
	Eris::TypeInfo* getSelectedTypeInfo();
	
	
	/**
	Loads all types into the list
	*/
	void loadAllTypes();
	
	void addToList(Eris::TypeInfo* typeInfo, int level);
	
	void createEntityOfType(Eris::TypeInfo* typeinfo);

	/**
	A preview renderer for creating new models.
	*/
	ModelRenderer* mModelPreviewRenderer;

	void createPreviewTexture();
	void updatePreview();

};
};
};

#endif
