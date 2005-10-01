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

/*#include <sigc++/object.h>
#include <sigc++/signal.h>*/
#include <sigc++/slot.h>
// #include <sigc++/bind.h>
// #include <sigc++/object_slot.h>
namespace CEGUI
{
	class Listbox;
	class Slider;
	class Editbox;
}

namespace EmberOgre {

typedef std::vector<Ogre::Entity*> EntityStore;


class MeshPreviewHandler;
class MeshPreviewMeshInstance;

typedef std::vector<MeshPreviewMeshInstance> InstanceStore;

class MeshPreviewHandler : SigC::Object
{
public:
	MeshPreviewHandler();
	void removeInstance(size_t index);
	size_t createInstance(const std::string& meshName);
	MeshPreviewMeshInstance& getInstance(size_t position);
	
 	SigC::Signal1<void, size_t> EventCreatedInstance;
 	SigC::Signal1<void, size_t> EventRemoveInstance;
 	
 	void updateAnimation(Ogre::Real elapsedTime);

private:
	size_t mEntityCounter;
	InstanceStore mInstances;
};


typedef std::map<std::string, Ogre::AnimationState*> AnimationStore;

class MeshPreviewMeshInstance
{
public:
	MeshPreviewMeshInstance(Ogre::Entity* entity);

	Ogre::Entity* getEntity() const;
	Ogre::SceneNode* getSceneNode() const;
	void startAnimation(std::string name);
	void stopAnimation(std::string name);
 	
 	void updateAnimation(Ogre::Real elapsedTime);
private:

	Ogre::Entity* mEntity;
	AnimationStore mActiveAnimations;
	//Ogre::SceneNode* sceneNode;
};

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
	
	/**
	 *    Called each frame.
	 * @param evt 
	 */
	virtual void frameStarted(const Ogre::FrameEvent& evt);

private:
	
	
	

	EntityStore mEntities;
	
	CEGUI::Editbox* mNameOfMesh;
	CEGUI::Listbox* mCreatedMeshes;
	CEGUI::Listbox* mAnimations;
	
	CEGUI::Slider* mScaleSlider;
	
//  	void loadAllAvailableMeshes();

	void createdNewEntity(size_t index);
	void removedEntity(size_t index);
	
	bool createButton_Click(const CEGUI::EventArgs& args);
	bool removeButton_Click(const CEGUI::EventArgs& args);
// 	void addMeshToAvailableMeshesList(const std::string& name);

	bool createdMeshes_EventSelectionChanged(const CEGUI::EventArgs& args);
	
	bool Scale_ValueChanged(const CEGUI::EventArgs& args);

	Ogre::SceneNode* getActiveSceneNode();
	MeshPreviewMeshInstance& getActiveInstance();
	
	MeshPreviewHandler mHandler;
	
	void fillAnimationList(MeshPreviewMeshInstance& instance );
	
};

}

#endif
