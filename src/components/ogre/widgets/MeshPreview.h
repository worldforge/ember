//
// C++ Interface: MeshPreview
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
#ifndef EMBEROGREMESHPREVIEW_H
#define EMBEROGREMESHPREVIEW_H

#include "Widget.h"

namespace CEGUI
{
	class Listbox;
	class Slider;
	class Editbox;
}

namespace EmberOgre {

typedef std::vector<Ogre::Entity*> EntityStore;

/**
@author Erik Hjortsberg
*/
class MeshPreview : public Widget {
public:
    MeshPreview();

    ~MeshPreview();
	
	virtual void buildWidget();
	
	static const std::string CREATEMESH;
		
	virtual void runCommand(const std::string &command, const std::string &args);
	
private:
	
	int mEntityCounter;
	
	void removeMesh(size_t index);
	void createMesh(const std::string& meshName);
	

	EntityStore mEntities;
	
	CEGUI::Editbox* mNameOfMesh;
	CEGUI::Listbox* mCreatedMeshes;
	
	CEGUI::Slider* mScaleSlider;
	
// 	void loadAllAvailableMeshes();

	void createdNewEntity(Ogre::Entity* entity, Ogre::SceneNode* sceneNode);
	void removedEntity(size_t index);
	
	bool createButton_Click(const CEGUI::EventArgs& args);
	bool removeButton_Click(const CEGUI::EventArgs& args);
/*	void addMeshToAvailableMeshesList(const std::string& name);*/
	
	bool Scale_ValueChanged(const CEGUI::EventArgs& args);

	Ogre::SceneNode* getActiveSceneNode();
	
};

}

#endif
