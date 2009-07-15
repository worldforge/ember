/*
 Copyright (C) 2009 Erik Hjortsberg <erik.hjortsberg@gmail.com>

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

#include "components/ogre/IGraphicalRepresentation.h"
#include "components/ogre/OgreIncludes.h"
#include "components/ogre/EmberEntity.h"
#include "components/ogre/MotionManager.h"
#include <vector>
#include <list>
#include <OgreMath.h>
#include <Eris/Types.h>
#include <sigc++/trackable.h>

#ifndef MODELREPRESENTATION_H_
#define MODELREPRESENTATION_H_

namespace Eris {
class Entity;
}

namespace EmberOgre {
class EmberEntity;
class SoundEntity;


namespace Model {

class ActionDefinition;
class SoundDefinition;
class Model;
class Action;
class ModelMount;

typedef std::list<Action*> ActionStore;
typedef std::vector<ActionDefinition*> ActionDefinitionsStore;
typedef std::vector<SoundDefinition*> SoundDefinitionsStore;
typedef std::vector<Ogre::SceneNode*> SceneNodeStore;

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 *
 * @brief Represents an entity through a Model instance.
 *
 * An instance of this binds an EmberEntity instance and a Model instance together. It's normally not meant to be created externally.
 * You can access instances of it attached to an entity through the ModelRepresentationManager class.
 */
class ModelRepresentation : public IGraphicalRepresentation, public virtual sigc::trackable, public IAnimated
{
public:

	/**
	 * @brief The name of the normal standing action.
	 */
	static const char * const ACTION_STAND;

	/**
	 * @brief The name of the running action.
	 */
	static const char * const ACTION_RUN;
	/**
	 * @brief The name of the walking action.
	 */
	static const char * const ACTION_WALK;
	/**
	 * @brief The name of the swimming action.
	 */
	static const char * const ACTION_SWIM;
	/**
	 * @brief The name of the float action.
	 */
	static const char * const ACTION_FLOAT;

	/**
	 * @brief Ctor.
	 * @param entity The entity instance to bind to.
	 * @param model The model instance used.
	 */
	ModelRepresentation(EmberEntity& entity, Model& model);

	/**
	 * @brief Dtor.
	 */
    virtual ~ModelRepresentation();


    /**
     * @copydoc EmberOgre::IGraphicalRepresentation::getType()
     */
	virtual const std::string& getType() const;

	/**
	 * @brief Gets the shared class type name, which is the same one returned through getType().
	 * This can be used to do lookups to see whether any instance of IGraphicalRepresentation if an instance of this class.
	 * @returns The string "ModelRepresentation".
	 */
	static const std::string& getTypeNameForClass();

	/**
	 * @brief Gets the entity which this representation is connected to.
	 */
    EmberEntity & getEntity() const;

    /**
     * @brief Gets the model which this representation shows.
     */
    Model & getModel() const;

	/**
	 * @brief Returns the "scale node", which is the Ogre::SceneNode to which the Model instance is attached. This is separate from the Ogre::SceneNode in EmberEntity since we want to be able to scale the node without also scaling the attached nodes (such as any wielded entity).
	 * @return An Ogre::SceneNode, to which the Model::Model instance is attached.
	 */
	Ogre::SceneNode* getScaleNode() const;

	void attachToPointOnModel(const std::string& point, Model* model, Ogre::Quaternion orientation = Ogre::Quaternion::IDENTITY, Ogre::Vector3 offset = Ogre::Vector3::ZERO);
	void detachFromModel();

	/**
	 * @brief Updates the animation. This is normally called by MotionManager.
	 * @param timeSlice time to update with.
	 */
	void updateAnimation(Ogre::Real timeSlice);

	/**
	 * @brief Sets whether the ogre axis aligned bounding box should be shown or not.
	 * @param show whether to show the ogre bounding box
	 */
	void showOgreBoundingBox(bool show);

	/**
	 * @brief Gets whether the ogre axis aligned bounding box should be shown or not.
	 * @return true if the bounding box is shown
	 */
	bool getShowOgreBoundingBox() const;

	/**
	 * @brief Returns the entity that's attached to the specified point, if there is such
	 * @param attachPoint
	 * @return a pointer to the EmberEntity, or 0 if none found
	 */
	EmberEntity* getEntityAttachedToPoint(const std::string& attachPoint);

	/**
	 * @brief Accesses the world bounding box of the model.
	 * @param derive Whether to derive from attached objects too.
	 * @return The world bounding box for this model, in world units.
	 */
	virtual const Ogre::AxisAlignedBox& getWorldBoundingBox(bool derive = true) const;


	/**
	 * @brief Accesses the world bounding sphere for this model.
	 * @param derive
	 * @return The world bounding sphere, in world units, for this model.
	 */
	virtual const Ogre::Sphere & getWorldBoundingSphere (bool derive=true) const;

	/**
	 * @brief Called by a contained member to see if the member is allowed to be shown.
	 * This can be reimplemented in a subclass such as AvatarEmberEntity to
	 * disallow things that belongs to a characters inventory to be shown.
	 */
	bool allowVisibilityOfMember(EmberEntity* entity);

	/**
	 * @brief General method for turning on and off debug visualizations. Subclasses might support more types of visualizations than the ones defined here.
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
	 * @brief Shows/hides a certain part of the model.
	 * This method is normally called by an instance of EmberEntityPartAction.
	 * @param partName The part to show.
	 * @param visible Whether to have it visible or not.
	 */
	void setModelPartShown(const std::string& partName, bool visible);

protected:

	struct ModelAttachment
	{
		Model* model;
		std::string attachPoint;
		Ogre::Vector3 offset;
		Ogre::Quaternion orientation;
	};

	/**
	 * Tells the entity to retrieve it sound actions from
	 * the model definition manager
	 */
	void setSounds();
	bool needSoundEntity();



	void setClientVisible(bool visible);

	const Ogre::Vector3& getOffsetForContainedNode(const Ogre::Vector3& position, const EmberEntity& entity);

	/**
	 *   creates EmberEntityUserObjects, connects them and sets up the collision detection system
	 * @return
	 */
	void connectEntities();

	/**
	 *    Called when the bounding box has changed.
	 */
	void bboxChanged();

	/**
	 * @brief Called when the movement mode has changed. We might want to update the animation of the entity, for example if it's a human.
	 * @param newMode
	 */
	void entity_MovementModeChanged(EmberEntity::MovementMode newMode);

	/**
	 The current movement action of the entity, for example a walk action or a run action.
	 */
	Action* mCurrentMovementAction;

	/**
	 All the active actions, except the movement action (since it's stored in mCurrentMovementAction).
	 These actions will be updated each frame.
	 NOTE: we currently don't allow for multiple actions playing at the same time
	 */
	//ActionStore mActiveActions;
	Action* mActiveAction;

	/**
	 If the entity is attached to another entity, this is the model to which it is attached to.
	 This will be 0 if the entity isn't attached.
	 */
	ModelAttachment* mModelAttachedTo;

	/**
	 We can't do attachments until the entity has been properly initialized, so sometimes we need to do delayed attachments. This will then hold the model to which this entity should be attached. Once the entity has been initialized we'll use mModelAttachedTo instead.
	 */
	ModelAttachment* mModelMarkedToAttachTo;

	virtual void entity_ChildAdded(Eris::Entity *e);
	virtual void entity_ChildRemoved(Eris::Entity *e);
	virtual void entity_LocationChanged(Eris::Entity *oldLocation);

	/**
	 * @brief Detaches an entity which is already wielded.
	 * @param entityId
	 */
	void detachEntity(const std::string & entityId);

	/**
	 * @brief Attaches an entity to a certain attach point
	 * @param attachPoint the name of the attachpoint to attach to
	 * @param entityId the id of the entity to attach to
	 */
	void attachEntity(const std::string & attachPoint, const std::string & entityId);

	/**
	 * @brief Detaches all currently attached entities. Call this before the Model is resetted.
	 */
	void detachAllEntities();

	/**
	 * @brief Attaches all entities that aren't currently attached.
	 */
	void attachAllEntities();

	/**
	 * @brief Process wield ops, which means wielding and unwielding entities. This methos will in turn call the appropriate attachEntity and detachEntity methods.
	 * @param wieldName the attachpoint to update
	 * @param idElement the id of the entity to wield
	 */
	void processWield(const std::string& wieldName, const Atlas::Message::Element& idElement);

	/**
	 * @brief Processes the outfit map and updates the appearance.
	 * @param outfitMap
	 */
	void processOutfit(const Atlas::Message::MapType & outfitMap);

	void entity_Changed(const Eris::StringSet& attributeIds);


	/**
	 *    Overridden from Eris::Entity
	 * @param str
	 * @param v
	 */
	virtual void attrChanged(const std::string& str, const Atlas::Message::Element& v);
	/**
	 *    Overridden from Eris::Entity
	 * @param act
	 */
	virtual void entity_Acted(const Atlas::Objects::Operation::RootOperation& act);

	typedef std::map<std::string, std::string> AttachedEntitiesStore;
	/**
	 * @brief A store of all attached entities, indexed by their id.
	 */
	AttachedEntitiesStore mAttachedEntities;

	/**
	 *    Overridden from Eris::Entity
	 * @param moving
	 */
//	void setMoving(bool moving);

	/**
	 * @brief Scales the Ogre::SceneNode to the size of the AxisBox defined by Eris::Entity.
	 * Note that this method in its default implementation will just call ModelMount::rescale
	 */
	void scaleNode();

	/**
	 * @brief The model of the entity.
	 * This is the main graphical representation of this entity.
	 * Note that the Model won't be directly connected to the main scene node, instead the mModelMount instance will take care of setting everything up to use an intermediary "scale node".
	 */
	Model& mModel;

	/**
	 * @brief The model mount, which takes care of setting up and handling the rotation and orientation of the model.
	 * This also owns the scale node, which will be destroyed when the mount is destroyed.
	 */
	ModelMount* mModelMount;

	/**
	 * @brief The sound entity this entitiy is connected to.
	 */
	SoundEntity* mSoundEntity;

	/**
	 * @brief When the Model is reloaded we need to update with the new values.
	 */
	void model_Reloaded();

	/**
	 * @brief When the Model is reset we need to clean up and remove all attachments from it.
	 */
	void model_Resetting();

	/**
	 * @brief Initialize position and scaling of the scale node with values from the Model, as well as set up any alternative rendering techniques.
	 */
	void initFromModel();

	/**
	 * @brief Keep track of the light nodes.
	 */
	SceneNodeStore mLightNodes;

	/**
	 * @brief The entity which this representation is bound to.
	 */
    EmberEntity & mEntity;

    /**
     * @brief The type name for the class.
     */
    static std::string sTypeName;
};

}
}
#endif /* MODELREPRESENTATION_H_ */
