/*
    Copyright (C) 2004  Miguel Guzman (Aglanor)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef GUIMANAGER_H
#define GUIMANAGER_H

#include "CEGUI/CEGUIWindow.h"

#include <Ogre.h>
//#include <OgrePredefinedControllers.h> 
#include "framework/Singleton.h"

namespace DimeOgre {

class DimeEntity;
class TerrainGenerator;
class CEGUI::Window;

/*
 * This class will be responsible for all the GUI related things
 */
class GUIManager : public dime::Singleton<GUIManager> {
public:

	GUIManager(Ogre::RenderWindow* window, Ogre::SceneManager* sceneMgr);
	virtual ~GUIManager();
	//static MotionManager & getSingleton(void);
	
	void appendIGChatLine(std::string line);
	void appendOOGChatLine(std::string line);
	
	/*
	 * Adds a DimeEntity to the movement list.
	 * That means that until removeEntity is called for the specific entity
	 * new positions for the entity will be calculated for each frame.
	 */
	//void addEntity(DimeEntity* entity) {mMotionSet.insert(entity);}
	//void removeEntity(DimeEntity* entity) {mMotionSet.erase(entity);}
	
	/*
	 * Registers an animationState. After registration it will be enough to use
	 * AnimationState::enabled(bool) to toggle the animation on and off
	 */
	//void addAnimation(Ogre::AnimationState* animationState);
	/*
	 * Deregisters an animationState
	 */
	//void removeAnimation(Ogre::AnimationState* animationState);
	/*
	 * Pauses the supplies animationState
	 */
	//void pauseAnimation(Ogre::AnimationState* animationState);
	/*
	 * Unpauses (starts) an already paused animationState
	 */
	//void unpauseAnimation(Ogre::AnimationState* animationState);
	


	/*
	 * Methods from Ogre::FrameListener
	 */
	//bool frameStarted(const Ogre::FrameEvent& event);
	//bool frameEnded(const Ogre::FrameEvent& event);
	
	/*
	 * Adjusts the height of the supplied node
	 */
	//void adjustHeightPositionForNode(Ogre::SceneNode* sceneNode);

	//void setTerrainGenerator(TerrainGenerator* generator);
	
	
private:

	CEGUI::Window* mChat;
	
	/*
	 * A pointer to the applications ControllerManager. This will take care of 
	 * keeping tabs on all controllers.
	 */
	//Ogre::ControllerManager* mControllerManager;
	//typedef Ogre::Controller<Ogre::Real> animationControllerType;
	//typedef std::map<Ogre::AnimationState*, animationControllerType*> animationStateMap;
	/*
	 * A map of AnimationState's and their corresponding Controllers
	 */
	//animationStateMap mAnimations;


	//static MotionManager* _instance;

	/* This method will iterate over all registered moving entities and update
	 * their positions.
	 */
	//void doMotionUpdate(Ogre::Real timeSlice);

	/* 
	 * Update the motion for a single DimeEntity
	 */
	//void updateMotionForEntity(DimeEntity* entity, Ogre::Real timeSlice);

	/*
	 * This contains all of the entities that will be moved each frame
	 */
	//std::set<DimeEntity*> mMotionSet;
	
	//TerrainGenerator* mTerrainGenerator;
	
};
}


#endif // MOTIONMANAGER_H
