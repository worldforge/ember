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

namespace CEGUI
{
	class Tree;
	class TreeItem;
}

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
A simple entity creator widget. All entity types are shown in a tree, allowing us to select one and instantiate it in the world.
The tree is populated when the widget is shown, and after that it's updated every time a new type is discovered.

This simplified entity creator will be supplemented by the more advanced one as part of GSoC 2008.
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

	/**
	Use a lookup map for the types and the corresponding tree item.
	*/
	std::map<Eris::TypeInfo*, CEGUI::TreeItem*> mTypes;
	
	CEGUI::Tree* mTypeTree;
	CEGUI::Editbox* mName;
	
	/**
	The main connection to the server.
	*/
	Eris::Connection* mConn;
	
	void gotAvatar(Eris::Avatar* avatar);
	void connectedToServer(Eris::Connection* conn);
	void boundAType(Eris::TypeInfo* typeInfo);
	
	bool createButton_Click(const CEGUI::EventArgs& args);
	
	bool typeTree_ItemSelectionChanged(const CEGUI::EventArgs& args);
	
	/**
	 *    Gets the currently selected type info.
	 * @return 
	 */
	Eris::TypeInfo* getSelectedTypeInfo();
	
	
	/**
	Loads all types into the list
	*/
	void loadAllTypes();
	
	/**
	 *    Adds a type info to the tree.
	 * @param typeInfo The type info to add.
	 * @param parent The parent of the type info, or 0 if we should add to the top.
	 * @param addRecursive If true, all the current children of the type will be added as well.
	 */
	void addToTree(Eris::TypeInfo* typeInfo, CEGUI::TreeItem* parent, bool addRecursive = false);
	
	void createEntityOfType(Eris::TypeInfo* typeinfo);

	/**
	A preview renderer for creating new models.
	*/
	ModelRenderer* mModelPreviewRenderer;

	void createPreviewTexture();
	void updatePreview();
	
	/**
	If true, the type tree has been initialized.
	*/
	bool mIsInitialized;

};
};
};

#endif
