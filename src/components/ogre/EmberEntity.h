/*
    Copyright (C) 2004  Erik Hjortsberg

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

#ifndef DIMEENTITY_H
#define DIMEENTITY_H

#include "EmberOgrePrerequisites.h"


#include <Atlas/Objects/Entity.h>
#include <Atlas/Objects/Operation.h>

#include <Eris/Entity.h>

namespace Ogre
{
	class OOBBWireBoundingBox;
}

namespace Mercator
{
	class Area;
}

namespace Eris
{
	class View;
}

namespace EmberOgre {

namespace Model {
	class Model;
}

namespace Terrain
{
	class TerrainArea;
}

class EmberEntityFactory;
/**
 * A representation of an Eris::Entity, ie. a world entity.
 * Note that most entities in the game world will be of type EmberPhysicalEntity
 * as they will have some sort of physical representation.
 * For things such as boundaries and weather, this is a nice class.
 */
class EmberEntity : public Eris::Entity {
	friend class EmberEntityFactory;
public:

	
	enum MovementMode
	{
		MM_DEFAULT = 0,
		MM_STANDING = 1,
		MM_FLOATING = 2,
		MM_PROJECTILE = 3,
		MM_SWIMMING = 4,
		MM_WALKING = 5,
		MM_RUNNING = 6,
		MM_FIXED = 7  
	};

	static const std::string MODE_STANDING;
	static const std::string MODE_RUNNING;
	static const std::string MODE_WALKING;
	static const std::string MODE_SWIMMING;
	static const std::string MODE_FLOATING;
	static const std::string MODE_FIXED;
	
	/**
	The material used for showing the eris bbox.
	*/
	static const std::string BboxMaterialName;

	
	
	EmberEntity(const std::string& id, Eris::TypeInfo* ty, Eris::View* vw,Ogre::SceneManager* sceneManager);
	virtual ~EmberEntity();
	

	/**
	 * Called by contained entites to determine how they should be adjusted, for example snap to the ground.
	 * For instance a house entitiy containing a player entity.
	 * This should of course be extended to a more dynamic physics simulation
	 * in the future
	 */
	virtual void adjustPositionForContainedNode(EmberEntity* const entity, const Ogre::Vector3& position);


	/**
	 * Adjust the height of the entity so that it "snaps" to the ground.
	 * This is most often done by making a call to the containing node's
	 * adjustPositionForContainedNode method.
	 */
	virtual void adjustPosition(); 
	virtual void adjustPosition(const Ogre::Vector3& position);

	/**
	 * return the scenenode to which this entity belongs
	 */
	Ogre::SceneNode* getSceneNode() const;
	
	/**
	 * Called by a contained member to see if the member is allowed to be shown.
	 * This can be reimplemented in a subclass such as AvatarEmberEntity to 
	 * disallow things that belongs to a characters inventory to be shown.
	 */
	virtual bool allowVisibilityOfMember(EmberEntity* entity);

	
	/**
	*return true if the entity has a list of suggested responses
	*/
	bool hasSuggestedResponses() const;
	
	/**
	* returns a list of all suggested responses
	*/
	const std::vector< std::string >& getSuggestedResponses() const;
	
	
	/**
	 *    Sets the visibity of the Entity
	 * @param visible 
	 */
	virtual void setVisible(bool visible);
	

	/**
	 *    gets the location as cast to an EmberEntity
	 * @return 
	 */
	EmberEntity* getEmberLocation() const;
	
	/**
	attaches the entity to another entity (or in reality another Model)
	*/
	virtual void attachToPointOnModel(const std::string& point, Model::Model* model) {};
	
	/**
	detaches the entity from another entity (or in reality another Model)
	*/
	virtual void detachFromModel() {};
	
	/**
	if this is true, init(...) has been called and the entity been set up
	*/
	inline bool isInitialized() const;

	/**
	the mode the entity is in, like walking, running, swimming etc.
	*/
	inline MovementMode getMovementMode() const;
	
	/**
	Call this method once per frame to update the motion of the entity
	*/
	virtual void updateMotion(Ogre::Real timeSlice);
	
	/**
	For debugging.
	Shows the Ogre bounding box.
	*/
	virtual void showOgreBoundingBox(bool show);
	

	/**
	For debugging.
	Shows the eris/atlas bounding box.
	@see mErisEntityBoundingBox
	*/
	virtual void showErisBoundingBox(bool show);

	/**
	returns whether the ogre bounding box is shown
	*/
	virtual bool getShowOgreBoundingBox() const;
	
	/**
	returns whether the eris/atlas bounding box is shown
	@see mErisEntityBoundingBox
	*/
	virtual bool getShowErisBoundingBox() const;

	/**
	 *    returns the world bounding box of the entity
	 * @param derive whether to derive from attached objects too
	 * @return 
	 */
	virtual const Ogre::AxisAlignedBox& getWorldBoundingBox(bool derive = true) const;
	
	
	virtual const Ogre::Sphere & getWorldBoundingSphere (bool derive=true) const;
	
	/**
	 *    Returns a list of the default use operators that can be used with this entity.
	 For example, an axe would have a list of operators such as "chop" and "sharpen".
	 * @return 
	 */
	std::vector<std::string> getDefaultUseOperators();
	
	
	/**
	 *    Synchronizes the position and orientation of the entity with the server.
	 */
	void synchronizeWithServer();


	/**
	Dumps all of this entity's attributes to the supplied outstream.
	*/
	void dumpAttributes(std::iostream& outstream, std::ostream& logOutstream) const;
	
	/**
	 * General method for turning on and off debug visualizations. Subclasses might support more types of visualizations than the ones defined here.
	 * @param visualization The type of visualization. Currently supports "OgreBBox" and "ErisBBox".
	 * @param visualize Whether to visualize or not.
	 */
	virtual void setVisualize(const std::string& visualization, bool visualize);
	
	
	/**
	 *    Gets whether a certain visualization is turned on or off.
	 * @param visualization The type of visualization. Currently supports "OgreBBox" and "ErisBBox".
	 * @return true if visualization is turned on, else false
	 */
	virtual bool getVisualize(const std::string& visualization) const;

protected: 


	/**
	 *    Gets the position of a contained node.
	 * @param position 
	 * @param entity 
	 * @return 
	 */
	virtual const Ogre::Vector3& getOffsetForContainedNode(const Ogre::Vector3& position, EmberEntity* const entity);
	

	/**
	if this is true, init(...) has been called and the entity been set up
	*/
	bool mIsInitialized;
	
	/**
	if true, the entity is already registered with the motion manager, so we don't need to add it again (which can be expensive since
	the motionmanager holds all entities needing updated motions in a std::set)
	*/
	bool mIsInMotionManager;
	
	/**
	 * Overridden from Eris::Entity
	 * @see Eris::Entity
	 */
	virtual void onMoved();
	virtual void setMoving(bool moving);
	virtual void onTalk(const Atlas::Objects::Operation::RootOperation& talk);
//	virtual void setContainer(Entity *pr);
	virtual void onVisibilityChanged(bool vis);
	virtual void onLocationChanged(Eris::Entity *oldLocation);
    virtual void onAction(const Atlas::Objects::Operation::RootOperation& act);
    virtual void onImaginary(const Atlas::Objects::Root& act);
	virtual void onSoundAction(const Atlas::Objects::Operation::RootOperation& op);
	
	virtual void addArea(Terrain::TerrainArea* area);
	virtual void onAttrChanged(const std::string& str, const Atlas::Message::Element& v);
	
	
	virtual void onModeChanged(MovementMode newMode);
	
	/**
	 *    Called when the bounding box has changed.
	 */
	virtual void onBboxChanged();

	/**
	
	For debugging purposes. This holds a bounding box of how the entity appears in the eris/atlas world.
	This is often different from the Ogre bounding box.
	*/
	Ogre::OOBBWireBoundingBox* mErisEntityBoundingBox;
	
	/**
	 *    Creates the material used for the eris bboxes, if not already created.
	 */
	void createErisBboxMaterial();
	
	
	
	/**
	 * Creates the main scene node which holds the entity.
	 */
	void createSceneNode(Ogre::SceneManager* sceneManager);
	
	/**
	override from eris
	this is called by eris just after the entity has been put into the world
	*/
    virtual void init(const Atlas::Objects::Entity::RootEntity &ge, bool fromCreateOp);

	virtual void checkVisibility(bool vis);

	/**
	Sometimes when talking to an entity, the server will provide suggested responses. These are stored here.
	*/
	std::vector<std::string> mSuggestedResponses;

	/**
	 * The main SceneNode which holds the entity in the ogre world space.
	 */
	Ogre::SceneNode* mOgreNode;
	
	/**
	Gets the scene manager that manages the Ogre scene node held by this.
	*/
	Ogre::SceneManager* getSceneManager();
	
	/**
	If there's a terrainarea belonging to this entity, that's stored here.
	*/
	std::auto_ptr<Terrain::TerrainArea> mTerrainArea;

		
	/**
	the mode the entity is in, like walking, running, swimming etc.
	*/
	MovementMode mMovementMode;


	/**
	 *    parses the current mode from the submitted element, which should be taken from the "mode" attribute
	 *    this method will in turn call onModeChanged if the mode is changed
	 * @param v 
	 */
	void parseModeChange(const Atlas::Message::Element& v);

};


///inline implementations
	bool EmberEntity::isInitialized() const 
	{ 
		return mIsInitialized; 
	}

	EmberEntity::MovementMode EmberEntity::getMovementMode() const 
	{ 
		return mMovementMode; 
	}


}

#endif // DIMEENTITY_H
