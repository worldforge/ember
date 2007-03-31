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

#include <sigc++/signal.h>

namespace EmberOgre {

typedef std::vector<Ogre::Entity*> EntityStore;


class MeshPreviewHandler;
class MeshPreviewMeshInstance;

typedef std::vector<MeshPreviewMeshInstance> InstanceStore;

/**
Class for handling the preview of meshes. This will keep track of all the meshes that are previewed.
*/
class MeshPreviewHandler 
{
public:
	MeshPreviewHandler();
	
	
	/**
	 *    removes an instance of MeshPreviewMeshInstance
	 * @param index 
	 */
	void removeInstance(size_t index);
	
	/**
	 *    creates a new instance of MeshPreviewMeshInstance, return the internal position of it. Use getInstance(size_t) to access it.
	 * @param meshName a valid name of a mesh
	 * @return the index of the new instance, to be used with getInstance(size_t)
	 */
	size_t createInstance(const std::string& meshName);
	
	
	/**
	 *    gets the MeshPreviewMeshInstance at the specified position
	 *    If there is no instance at the position, an Ember::Exception will be thrown.
	 * @param position 
	 * @return 
	 */
	MeshPreviewMeshInstance& getInstance(size_t position);
	
	/**
	Emitted when an instance has been created.
	*/
 	sigc::signal<void, size_t> EventCreatedInstance;
 	
 	/**
 	Emitted when an instance has been removed.
 	*/
 	sigc::signal<void, size_t> EventRemoveInstance;
 	
 	/**
 	Call this each fram to update the animations of all meshes.
 	*/
 	void updateAnimation(Ogre::Real elapsedTime);

private:
	/**
	When creating new entities, they need an unique name, hence this counter.
	*/
	size_t mEntityCounter;
	
	/**
	A store of all instances.
	*/
	InstanceStore mInstances;
};


typedef std::map<std::string, Ogre::AnimationState*> AnimationStore;

/**
Basically a container for an Ogre::Entity with some nice methods. 
*/
class MeshPreviewMeshInstance
{
public:
	MeshPreviewMeshInstance(Ogre::Entity* entity);

	
	/**
	 *    Accessor for the Entity.
	 * @return 
	 */
	Ogre::Entity* getEntity() const;
	
	/**
	 *    Accessor for the SceneNode.
	 * @return 
	 */
	Ogre::SceneNode* getSceneNode() const;
	
	/**
	 *    start the animation with the given name
	 * @param name 
	 */
	void startAnimation(const std::string& name);
	
	
	/**
	 *    stops the animation with the given name
	 * @param name 
	 */
	void stopAnimation(const std::string& name);
	
	
	/**
	 *    Toggles the status of the animation with the given name on and off.
	 * @param name 
	 */
	void toggleAnimation(const std::string& name);
	
	
	/**
	 *    Resets all animations and return the Entity to the initial state.
	 */
	void resetAnimations();
	
	/**
	 *    returns true if the animation with the given name is currently playing
	 * @param name 
	 * @return 
	 */
	bool isAnimationPlaying(const std::string& name);
	
	
	/**
	 *    returns true if the animation with the given name is enabled
	 * @param name 
	 * @return 
	 */
	bool isAnimationEnabled(const std::string& name);
	
 	
 	/**
 	 *   Call this method each frame to update the animations.
 	 * @param elapsedTime 
 	 */
 	void updateAnimation(Ogre::Real elapsedTime);
private:

	Ogre::Entity* mEntity;
	AnimationStore mActiveAnimations;
	//Ogre::SceneNode* sceneNode;
	
};


typedef std::vector<std::string> StringStore;

/**
@author Erik Hjortsberg
*/
class MeshPreview : public Widget {
public:
    MeshPreview();

    virtual ~MeshPreview();
	
	virtual void buildWidget();
	
	const Ember::ConsoleCommandWrapper CreateMesh;
		
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
	
	CEGUI::PushButton* mPlayAnimation;
	
//  	void loadAllAvailableMeshes();

	void createdNewEntity(size_t index);
	void removedEntity(size_t index);
	
	bool createButton_Click(const CEGUI::EventArgs& args);
	bool removeButton_Click(const CEGUI::EventArgs& args);
	bool playAnimation_MouseClick(const CEGUI::EventArgs& args);
	bool resetAnimation_MouseClick(const CEGUI::EventArgs& args);
	
// 	void addMeshToAvailableMeshesList(const std::string& name);

	bool createdMeshes_EventSelectionChanged(const CEGUI::EventArgs& args);
	
	bool Scale_ValueChanged(const CEGUI::EventArgs& args);

	Ogre::SceneNode* getActiveSceneNode();
	MeshPreviewMeshInstance& getActiveInstance();
	
	MeshPreviewHandler mHandler;
	
	void fillAnimationList(MeshPreviewMeshInstance& instance );
	
//	const std::string getAnimationNameAtPosition();
	StringStore mAnimationNames;

};

}

#endif
