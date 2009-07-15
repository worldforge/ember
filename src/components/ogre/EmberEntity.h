/*
    Copyright (C) 2004  Erik Hjortsberg <erik.hjortsberg@gmail.com>

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

#include <OgreVector3.h>
#include <OgreQuaternion.h>

namespace Ogre
{
	class OOBBWireBoundingBox;
}

namespace Eris
{
	class View;
}

namespace Ember
{
namespace EntityMapping
{
class EntityMapping;
}
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
class IGraphicalRepresentation;
/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief A representation of an Eris::Entity, ie. a world entity.
 *
 * This is the base class for any entities in the world. It's therefore safe to case any instance of Eris::Entity into this class.
 * Any entity which has a graphical representation in the world, and under normal circumstances that's the most of them, will have mGraphicalRepresentation set to a subclass of IGraphicalRepresentation.
 */
class EmberEntity : public Eris::Entity {
	friend class EmberEntityFactory;
public:


	/**
	 * @brief The movement modes the entity can be in.
	 */
	enum MovementMode
	{
		/**
		 * @brief The default movement mode, when the entity is idle and not moving.
		 */
		MM_DEFAULT = 0,

		/**
		 * @brief Swimming through water.
		 */
		MM_SWIMMING = 1,

		/**
		 * @brief Walking at a normal pace.
		 */
		MM_WALKING = 2,

		/**
		 * @brief Running.
		 */
		MM_RUNNING = 3
	};

	/**
	 * @brief The different positioning modes the entity can be in.
	 * The positioning mode determines how the entity can be adjusted in the world.
	 */
	enum PositioningMode
	{
		/**
		 * @brief The default mode, where the entity is affected by gravity.
		 * In most circumstances this means that it will snap to the ground, or any object below it.
		 */
		PM_DEFAULT = 0,

		/**
		 * @brief Floating on top of an entity. Usually a mass of water.
		 */
		PM_FLOATING = 1,

		/**
		 * @brief Moving as a projectile through the world.
		 */
		PM_PROJECTILE = 2,

		/**
		 * @brief Fixed in the world. This means that the position of the entity won't be adjusted (i.e. not affected by gravity etc.).
		 */
		PM_FIXED = 3
	};

	/**
	 * @brief The name of the "floating" positioning mode.
	 */
	static const std::string MODE_FLOATING;

	/**
	 * @brief The name of the "fixed" positioning mode.
	 */
	static const std::string MODE_FIXED;

	/**
	 * @brief The name of the "projectile" positioning mode.
	 */
	static const std::string MODE_PROJECTILE;

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
	 *    Creates the model mapping for this entity. Call this once when initializing the entity.
	 */
	void createEntityMapping();

	/**
	 * @brief Called by contained entites to determine how they should be adjusted, for example snap to the ground.
	 * For instance a house entitiy containing a player entity.
	 * This should of course be extended to a more dynamic physics simulation in the future
	 * @param entity The entity to adjust.
	 * @param position The current position of the entity.
	 */
	virtual void adjustPositionForContainedNode(const EmberEntity& entity, const Ogre::Vector3& position);


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
	const std::vector<std::string>& getSuggestedResponses() const;


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
	virtual void attachToPointOnModel(const std::string& point, Model::Model* model, Ogre::Quaternion orientation = Ogre::Quaternion::IDENTITY, Ogre::Vector3 offset = Ogre::Vector3::ZERO) {};

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
	bool isInitialized() const;

	/**
	 * @brief The movement mode the entity is in, like walking, running, swimming etc.
	 * @return The current movement mode of the entity.
	 */
	MovementMode getMovementMode() const;

	/**
	 * @brief The movement mode the entity is in, like walking, running, swimming etc.
	 * @return The current movement mode of the entity.
	 */
	PositioningMode getPositioningMode() const;

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
	void setVisualize(const std::string& visualization, bool visualize);


	/**
	 * @brief Gets whether a certain visualization is turned on or off.
	 * @param visualization The type of visualization. Currently supports "OgreBBox" and "ErisBBox".
	 * @return true if visualization is turned on, else false
	 */
	bool getVisualize(const std::string& visualization) const;

	/**
	 * @brief Gets the graphical representation of this entity.
	 * Not all entities will have graphical representations, so you must always make sure to check for null on the result.
	 * Most graphical representation classes will also contain methods for safely casting into the concrete subclass.
	 * @returns The graphical representation, or null if there are none.
	 */
	IGraphicalRepresentation* getGraphicalRepresentation() const;

	/**
	 * @brief Sets the graphical representation to be used.
	 * If any previous graphical representation has been set, it will be deleted.
	 * @param graphicalRepresentation The new graphical representation. Ownership will be transferred to this class.
	 */
	void setGraphicalRepresentation(IGraphicalRepresentation* graphicalRepresentation);

	/**
	 * @brief Emitted when the visibility of the entity has changed.
	 * This differs from the visibility set by the server, as found in Eris::Entity. The main reason for this is that we sometimes don't want to show all entities, even if the server says they're visible. An example would be items held as inventory by an entity, which we in most cases don't want to show as entitied directly.
	 */
	sigc::signal<void, bool> EventClientVisibilityChanged;


	/**
	 * @brief Emitted when the movement mode has changed.
	 * As the entity moves the "movement mode" changes. An entity which isn't moving should in most cases be in the "default" movement mode, whereas a moving one could be in the "walking", "running" or any other mode.
	 * The parameter sent is the new movement mode.
	 * This event will be emitted before the actual mode is changed, so you can call getMovementMode() to get the current movement mode, before the new one is in effect.
	 */
	sigc::signal<void, MovementMode> EventMovementModeChanged;

	/**
	 * @brief Emitted when the positioning mode has changed.
	 * The entity is placed in the world differently depending on the "positioning mode". The default mode is to be gravity affected, i.e. fall to the ground.
	 * In a simplified simulation this will in most cases mean that the entity is snapped to the terrain. However, there are other options, such as "fixed" or "floating".
	 * The parameter sent is the new positioning mode.
	 * This event will be emitted before the actual mode is changed, so you can call getPositioningMode() to get the current positioning mode, before the new one is in effect.
	 */
	sigc::signal<void, PositioningMode> EventPositioningModeChanged;

protected:


	/**
	 * @brief Gets the position of a contained node.
	 * The main reason for us wanting to offset contained nodes is that sometimes we want contained nodes to be adjusted to better fit the current mode. An example would be a table, where we want nodes that are contained by the table entity to appear to lie flat on the table. The server of course has some ideas about this, but depending on the actual model used this might look strange. We therefore adjust the contained nodes to better fit the actual model.
	 * @param position The current position of the contained node.
	 * @param entity The entity to get an adjusted position for.
	 * @return The offset by which to adjust the contained node.
	 */
	virtual const Ogre::Vector3& getOffsetForContainedNode(const Ogre::Vector3& position, const EmberEntity& entity);


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
	 * @param newMode The new movement mode.
	 */
	virtual void onMovementModeChanged(MovementMode newMode);

	/**
	 * @brief Called when the positioning mode of the entity changes.
	 * Depending on the kind of positioning mode the entity needs to be adjusted in the world accordinly. For example, when the entity is gravity affected (the default) it should snap to the terrain, in contrast to when the mode is "fixed" where it wouldn't snap to the terrain.
	 * @param newMode The new movement mode.
	 */
	virtual void onPositioningModeChanged(PositioningMode newMode);

	/**
	 * @brief Called when the bounding box has changed.
	 */
	virtual void onBboxChanged();

	/**
	 * @brief Adds a terrain area to the entity.
	 * Areas are a terrain feature, and cannot be handled by a standard implementation of EmberEntity. The default implementation of this method in this class will therefore only pass it on to the parent entity, if any such is available. The idea is that somewhere along the way, as the method "walks" the entity hierarcy upwards it will come across an implementation of this method that does indeed know how to handle the terrain area.
	 * @see EmberOgre::WorldEmberEntity::addArea()
	 * @param area The area which has been added.
	 */
	virtual void addArea(Terrain::TerrainArea* area);

	/**
	 * @brief Adds a terrain mod to the entity.
	 * Mods are a terrain feature, and cannot be handled by a standard implementation of EmberEntity. The default implementation of this method in this class will therefore only pass it on to the parent entity, if any such is available. The idea is that somewhere along the way, as the method "walks" the entity hierarcy upwards it will come across an implementation of this method that does indeed know how to handle the terrain mod.
	 * @see EmberOgre::WorldEmberEntity::addTerrainMod()
	 * @param mod The mod which has been added.
	 */
	virtual void addTerrainMod(Terrain::TerrainMod* mod);

	/**
	 * @brief Checks if we should create any dependent objects, such as TerrainArea or TerrainMod.
	 * Call this only when the entity has been propely initialized, since we then have all the correct data regarding positions, bounding boxes etc.
	 * @see init()
	 * @see onAttrChanged()
	 * @param attributeName The attribute which we should try to create something for.
	 * @return True if any dependent object was created.
	 */
	virtual bool createDependentObject(const std::string& attributeName);

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
	 * @brief If there's a terrain area belonging to this entity, that's stored here.
	 * The terrain area instance will take care of all required terrain area functionality once it's been created, offloading this from the EmberEntity. Most entities won't however have any terrain areas, for which this will be null.
	 */
	std::auto_ptr<Terrain::TerrainArea> mTerrainArea;

	/**
	 * @brief If a terrainmod belongs to this entity, it's stored here.
	 * The terrain mod instance will take care of all required terrain mod functionality once it's been created, offloading this from the EmberEntity. Most entities won't however have any terrain mods, for which this will be null.
	 */
	std::auto_ptr<Terrain::TerrainMod> mTerrainMod;

	/**
	* @brief The movement mode the entity is in, like walking, running, swimming etc.
	*/
	MovementMode mMovementMode;

	/**
	* @brief The positioning mode the entity is in, like gravity affected, fixed or floating.
	*/
	PositioningMode mPositioningMode;

	/**
	 * @brief The graphical representation used for representing this entity.
	 * Some entities won't have any graphical representation, and this will in those cases be null.
	 */
	IGraphicalRepresentation* mGraphicalRepresentation;

	/**
	 * @brief The model mapping used for this entity.
	 */
	Ember::EntityMapping::EntityMapping* mEntityMapping;

	/**
	 * @brief Parses the current positioning mode from the submitted element, which should be taken from the "mode" attribute.
	 * This method will in turn call onPositioningModeChanged if the mode is changed
	 * @param v The element that contains the mode attribute, should be the "mode" attribute in the root attributes map.
	 */
	void parsePositioningModeChange(const Atlas::Message::Element& v);

	/**
	 * @brief Parses and sets the movement mode.
	 * The movement mode is determined mainly from whether the entity is moving or not. The speed of the movement also affects the mode.
	 */
	virtual void parseMovementMode();

};


inline bool EmberEntity::isInitialized() const
{
	return mIsInitialized;
}

inline EmberEntity::MovementMode EmberEntity::getMovementMode() const
{
	return mMovementMode;
}

inline EmberEntity::PositioningMode EmberEntity::getPositioningMode() const
{
	return mPositioningMode;
}

}

#endif // EMBEROGRE_EMBERENTITY_H
