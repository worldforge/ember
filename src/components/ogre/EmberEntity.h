/*
    Copyright (C) 2004  Erik Hjortsberg <erik.hjortsberg@iteam.se>

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

#ifndef EMBEROGRE_EMBERENTITY_H
#define EMBEROGRE_EMBERENTITY_H

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
	class TerrainMod;
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
	class TerrainMod;
}

class EmberEntityFactory;
/**
 * @author Erik Hjortsberg <erik.hjortsberg@iteam.se>
 * @brief A representation of an Eris::Entity, ie. a world entity.
 * Note that most entities in the game world will be of type EmberPhysicalEntity as they will have some sort of physical representation.
 * This is however the base class for all entities in the world. All entities, even those that are standard instances of this and therefore doesn't have any Model attached to them, are however represented in the Ogre scenegraph through an Ogre::SceneNode, as accessed through getSceneNode().
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

	
	/**
	 * @brief Ctor.
	 * This isn't normally called by something else than the EmberEntityFactory, but we'll keep it public for testing sake.
	 * @param id The unique id of the entity.
	 * @param ty The type info which specified the type for this entity.
	 * @param vw The world view to which this entity belongs.
	 * @param sceneManager The scene manager to which this entity belongs.
	 */
	EmberEntity(const std::string& id, Eris::TypeInfo* ty, Eris::View* vw,Ogre::SceneManager* sceneManager);
	
	/**
	 * @brief Dtor.
	 */
	virtual ~EmberEntity();
	

	/**
	 * @brief Called by contained entites to determine how they should be adjusted, for example snap to the ground.
	 * For instance a house entitiy containing a player entity.
	 * This should of course be extended to a more dynamic physics simulation in the future
	 * @param entity The entity to adjust.
	 * @param position The current position of the entity.
	 */
	virtual void adjustPositionForContainedNode(EmberEntity* const entity, const Ogre::Vector3& position);


	/**
	 * @brief Adjust the height of the entity so that it "snaps" to the ground or is otherwise adjusted, depending on the current movement mode.
	 * This is most often done by making a call to the containing node's adjustPositionForContainedNode method.
	 * @see adjustPositionForContainedNode()
	 */
	virtual void adjustPosition(); 
	
	/**
	 * @brief Adjust the height of the entity so that it "snaps" to the ground or is otherwise adjusted, depending on the current movement mode.
	 * This is most often done by making a call to the containing node's
	 * adjustPositionForContainedNode method.
	 * @see adjustPositionForContainedNode()
	 * @param position The current position.
	 */
	virtual void adjustPosition(const Ogre::Vector3& position);

	/**
	 * @brief Return the scenenode to which this entity belongs.
	 * All entities are always attached to an Ogre::SceneNode. Some subclasses, such as EmberPhysicalEntity, uses additional scene nodes for attaching the graphical representation, but true for all entities, even those that are just basic instances of EmberEntities, are that they will be represented in the Ogre world through a SceneNode.
	 * @return The SceneNode to which this entity is attached.
	 */
	Ogre::SceneNode* getSceneNode() const;
	
	/**
	 * @brief Called by a contained member to see if the member is allowed to be shown.
	 * This can be reimplemented in a subclass such as AvatarEmberEntity to disallow things that belongs to a characters inventory to be shown.
	 * @param entity The entity which we want to check whether we should show or not.
	 * @see setClientVisible()
	 * @see checkClientVisibility()
	 * @return True if the entity should be shown, false if it should be hidden.
	 */
	virtual bool allowVisibilityOfMember(EmberEntity* entity);

	
	/**
	 * @brief Returns true if the entity has a list of suggested responses to something this entity has said in the world.
	 * @see getSuggestedResponses()
	 * @return True if there's a collection of suggested responses to something that this entity has just said.
	 */
	bool hasSuggestedResponses() const;
	
	/**
	 * @brief Returns a list of all suggested responses when conversing with this entity.
	 * When talking to entities in the world (for example NPCs) the can sometimes be a specified list of suggested responses.
	 * @return A vector of the suggested responses.
	 */
	const std::vector< std::string >& getSuggestedResponses() const;
	
	
	/**
	 * @brief Sets the visiblity of the graphical visibility of the entity.
	 * This differs from the visibility set by the server, as found in Eris::Entity. The main reason for this is that we sometimes don't want to show all entities, even if the server says they're visible. An example would be items held as inventory by an entity, which we in most cases don't want to show as entitied directly.
	 * @see Eris::Entity::setVisible()
	 * @param visible True if the graphical representation should be shown, else false.
	 */
	virtual void setClientVisible(bool visible);
	

	/**
	 * @brief Gets the location (i.e. parent entity) as cast to an EmberEntity.
	 * This behaves just like getLocation, but will always return an instance of EmberEntity. We can do this since we're guaranteed to only ever deal with instances of EmberEntity in the world, thanks to the EmberEntityFactory class.
	 * @see Eris::Entity::getLocation
	 * @see EmberOgre::EmberEntityFactory
	 * @return The location of this entity, i.e. the parent entity. If there's no parent entity available, null will be returned (i.e. if the entity is in limbo, or if it's the topmost entity).
	 */
	EmberEntity* getEmberLocation() const;
	
	/**
	 * @brief Attaches the entity to another entity (or in reality another Model).
	 * @param point The point on the other entity to attach to.
	 * @param model The model to which we should attach ourself.
	 */
	virtual void attachToPointOnModel(const std::string& point, Model::Model* model) {};
	
	/**
	 * @brief Detaches the entity from another entity (or in reality another Model).
	 * While the implementation here is missing, it's expected that subclasses that deals with Models should implement this.
	 */
	virtual void detachFromModel() {};
	
	/**
	 * @brief Returns true if init(...) has been called and the entity been set up.
	 * This can be useful when handling the entity, and not knowing whether it has been properly initialized yet. After the entity has been created, there's a small delay until init() is called. During this time the entity is in an incomplete state, and code that accesses it during that time might instead want to wait until it has been properly initialized.
	 * @see init()
	 * @return True if init() already has been called, else false.
	 */
	inline bool isInitialized() const;

	/**
	 * @brief The mode the entity is in, like walking, running, swimming etc.
	 * @return The current movement mode of the entity.
	 */
	inline MovementMode getMovementMode() const;
	
	/**
	 * @brief Call this method once per frame to update the motion of the entity
	 * @param timeSlice Elapsed time since we last updated, in seconds.
	 */
	virtual void updateMotion(Ogre::Real timeSlice);
	
	/**
	 * @brief Shows the Ogre bounding box.
	 * This is mainly useful for debugging or authoring.
	 * @param show If true, the bounding box will be shown. If false, it will be hidden.
	 */
	virtual void showOgreBoundingBox(bool show);
	

	/**
	 * @brief Shows the eris/atlas bounding box.
	 * This is mainly useful for debugging or authoring.
	 * @see mErisEntityBoundingBox
	 * @param show If true, the bounding box will be shown. If false, it will be hidden.
	 */
	virtual void showErisBoundingBox(bool show);

	/**
	 * @brief Returns whether the ogre bounding box is shown.
	 * @return True if the ogre bounding box is shown.
	 */
	virtual bool getShowOgreBoundingBox() const;
	
	/**
	 * @brief Returns whether the eris/atlas bounding box is shown.
	 * @see mErisEntityBoundingBox
	 * @return True if the eris bounding box is shown.
	 */
	virtual bool getShowErisBoundingBox() const;

	/**
	 * @brief Accesses the world bounding box of the entity.
	 * @param derive whether to derive from attached objects too
	 * @return The world bounding box for this entity, in world units.
	 */
	virtual const Ogre::AxisAlignedBox& getWorldBoundingBox(bool derive = true) const;
	
	
	/**
	 * @brief Accesses the world bounding sphere for this entity.
	 * @param derive 
	 * @return The world bounding sphere, in world units, for this entity.
	 */
	virtual const Ogre::Sphere & getWorldBoundingSphere (bool derive=true) const;
	
	/**
	 *    Returns a list of the default use operators that can be used with this entity.
	 For example, an axe would have a list of operators such as "chop" and "sharpen".
	 * @return 
	 */
	std::vector<std::string> getDefaultUseOperators();
	
	
	/**
	 * @brief Returns a list of actions that can be performed on this entity, if any.
	 * @return A vector of actions, as strings.
	 */
	std::vector<std::string> getActions();
	
	/**
	 * @brief Synchronizes the position and orientation of the entity with the server.
	 */
	void synchronizeWithServer();


	/**
	 * @brief Dumps all of this entity's attributes to the supplied outstream.
	 * By default the entity will be exported using XML Atlas format.
	 * @param outstream The stream to write to.
	 * @param logOutstream The stream to which log messages will be written.
	 */
	void dumpAttributes(std::iostream& outstream, std::ostream& logOutstream) const;
	
	/**
	 * General method for turning on and off debug visualizations. Subclasses might support more types of visualizations than the ones defined here.
	 * @param visualization The type of visualization. Currently supports "OgreBBox" and "ErisBBox".
	 * @param visualize Whether to visualize or not.
	 */
	virtual void setVisualize(const std::string& visualization, bool visualize);
	
	
	/**
	 * @brief Gets whether a certain visualization is turned on or off.
	 * @param visualization The type of visualization. Currently supports "OgreBBox" and "ErisBBox".
	 * @return true if visualization is turned on, else false
	 */
	virtual bool getVisualize(const std::string& visualization) const;
	
	
	/**
	 * @brief Emitted when the visibility of the entity has changed.
	 * This differs from the visibility set by the server, as found in Eris::Entity. The main reason for this is that we sometimes don't want to show all entities, even if the server says they're visible. An example would be items held as inventory by an entity, which we in most cases don't want to show as entitied directly.
	 */
	sigc::signal<void, bool> EventClientVisibilityChanged;

protected: 


	/**
	 * @brief Gets the position of a contained node.
	 * The main reason for us wanting to offset contained nodes is that sometimes we want contained nodes to be adjusted to better fit the current mode. An example would be a table, where we want nodes that are contained by the table entity to appear to lie flat on the table. The server of course has some ideas about this, but depending on the actual model used this might look strange. We therefore adjust the contained nodes to better fit the actual model.
	 * @param position The current position of the contained node.
	 * @param entity The entity to get an adjusted position for.
	 * @return The offset by which to adjust the contained node.
	 */
	virtual const Ogre::Vector3& getOffsetForContainedNode(const Ogre::Vector3& position, EmberEntity* const entity);
	

	/**
	* @brief If this is true, init(...) has been called and the entity been set up.
	* @see isInitialized().
	*/
	bool mIsInitialized;
	
	/**
	@brief If true, the entity is already registered with the motion manager, so we don't need to add it again (which can be expensive since the motionmanager holds all entities needing updated motions in a std::set).
	*/
	bool mIsInMotionManager;
	
	/**
	 *    @copydoc Eris::Entity::onMoved()
	 */
	virtual void onMoved();
	/**
	 *    @copydoc Eris::Entity::setMoving()
	 */
	virtual void setMoving(bool moving);
	/**
	 *    @copydoc Eris::Entity::onTalk()
	 */
	virtual void onTalk(const Atlas::Objects::Operation::RootOperation& talk);
//	virtual void setContainer(Entity *pr);
	/**
	 *    @copydoc Eris::Entity::onVisibilityChanged()
	 */
	virtual void onVisibilityChanged(bool vis);
	/**
	 *    @copydoc Eris::Entity::onLocationChanged()
	 */
	virtual void onLocationChanged(Eris::Entity *oldLocation);
	/**
	 *    @copydoc Eris::Entity::onAction()
	 */
    virtual void onAction(const Atlas::Objects::Operation::RootOperation& act);
	/**
	 *    @copydoc Eris::Entity::onImaginary()
	 */
    virtual void onImaginary(const Atlas::Objects::Root& act);
	/**
	 *    @copydoc Eris::Entity::onSoundAction()
	 */
	virtual void onSoundAction(const Atlas::Objects::Operation::RootOperation& op);
	
	/**
	 *    @copydoc Eris::Entity::onAttrChanged()
	 */
	virtual void onAttrChanged(const std::string& str, const Atlas::Message::Element& v);
	
	/**
	 * @brief Called when the movement mode of the entity changes.
	 * For example when the entity changes from standing to walking.
	 * Depending on the kind of movement mode the entity needs to be adjusted in the world accordinly. For example, when the entity is walking it should snap to the terrain, in contrast to when the mode is "fixed" where it wouldn't snap to the terrain.
	 * @param newMode The new movement mode.
	 */
	virtual void onModeChanged(MovementMode newMode);
	
	/**
	 *    Called when the bounding box has changed.
	 */
	virtual void onBboxChanged();

	/**
	 * @brief Adds a terrain area to the entity.
	 * Areas are a terrain feature, and cannot be handled by a standard implementation of EmberEntity. The default implementation of this method in this class will therefore only pass it on to the parent entity, if any such is available. The idea is that somewhere along the way, as the method "walks" the entity hierarcy upwards it will come across an implementation of this method that does indeed know how to handle the terrain area.
	 * @see EmberOgre::WorldEmberEntity::addArea()
	 * @param area The area which has been added.
	 */
	virtual void addArea(Terrain::TerrainArea* area);
	
	virtual void addTerrainMod(Terrain::TerrainMod* mod);
	
	/**
	* @brief For debugging purposes. This holds a bounding box of how the entity appears in the eris/atlas world.
	* This is often different from the Ogre bounding box.
	*/
	Ogre::OOBBWireBoundingBox* mErisEntityBoundingBox;
	
	/**
	 * @brief Creates the material used for the eris bboxes, if not already created.
	 */
	void createErisBboxMaterial();

	/**
	 * @brief Creates the main scene node which holds the entity.
	 * @param sceneManager The scene manager which should be used for creating the scene node.
	 */
	void createSceneNode(Ogre::SceneManager* sceneManager);
	
	/**
	 * @brief Called by eris just after the entity has been put into the world.
	 * @param ge The root entity which contains all atlas data that define this entity.
	 * @param fromCreateOp 
	 */
	virtual void init(const Atlas::Objects::Entity::RootEntity &ge, bool fromCreateOp);

	/**
	 * @brief Checks whether the client represenation should be visible or not.
	 * This differs from the visibility set by the server, as found in Eris::Entity. The main reason for this is that we sometimes don't want to show all entities, even if the server says they're visible. An example would be items held as inventory by an entity, which we in most cases don't want to show as entitied directly.
	 * @param vis The original visiblity, as Eris::Entity sees it.
	 */
	virtual void checkClientVisibility(bool vis);

	/**
	@brief Sometimes when talking to an entity, the server will provide suggested responses. These are stored here.
	*/
	std::vector<std::string> mSuggestedResponses;

	/**
	 * @brief The main SceneNode which holds the entity in the ogre world space.
	 */
	Ogre::SceneNode* mOgreNode;
	
	/**
	 * @brief Gets the scene manager that manages the Ogre scene node held by this.
	 */
	Ogre::SceneManager* getSceneManager();
	
	/**
	 * @brief If there's a terrainarea belonging to this entity, that's stored here.
	 */
	std::auto_ptr<Terrain::TerrainArea> mTerrainArea;
		
	/**
	 * @brief If a terrainmod belongs to this entity, it's stored here.
	 */
	std::auto_ptr<Terrain::TerrainMod> mTerrainMod;
	
	/**
	* @brief The mode the entity is in, like walking, running, swimming etc.
	*/
	MovementMode mMovementMode;


	/**
	 * @brief Parses the current mode from the submitted element, which should be taken from the "mode" attribute.
	 * This method will in turn call onModeChanged if the mode is changed
	 * @param v The element that contains the mode attribute, should be the "mode" attribute in the root attributes map.
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

#endif // EMBEROGRE_EMBERENTITY_H
