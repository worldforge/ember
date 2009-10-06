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
#include "components/ogre/IAnimated.h"
#include <vector>
#include <list>
#include <OgreMath.h>
#include <OgreVector3.h>
#include <OgreQuaternion.h>
#include <sigc++/trackable.h>

#ifndef MODELREPRESENTATION_H_
#define MODELREPRESENTATION_H_

namespace Eris
{
class Entity;
}

namespace EmberOgre
{
class EmberEntity;
class SoundEntity;

namespace Model
{

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
class ModelRepresentation: public IGraphicalRepresentation, public virtual sigc::trackable, public IAnimated
{
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
		MM_RUNNING = 3,

		/**
		 * @brief Walking backwards. Note that there is no "running backwards" mode.
		 */
		MM_WALKING_BACKWARDS = 4
	};

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

	//	void attachToPointOnModel(const std::string& point, Model* model, Ogre::Quaternion orientation = Ogre::Quaternion::IDENTITY, Ogre::Vector3 offset = Ogre::Vector3::ZERO);
	//	void detachFromModel();

	/**
	 * @brief Updates the animation. This is normally called by MotionManager.
	 * @param timeSlice time to update with.
	 */
	void updateAnimation(Ogre::Real timeSlice);

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
	virtual const Ogre::Sphere & getWorldBoundingSphere(bool derive = true) const;

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

	Ogre::Vector3 getScale() const;

	/**
	 * @brief The movement mode the entity is in, like walking, running, swimming etc.
	 * @return The current movement mode of the entity.
	 */
	MovementMode getMovementMode() const;

	/**
	 * @brief Emitted when the movement mode has changed.
	 * As the entity moves the "movement mode" changes. An entity which isn't moving should in most cases be in the "default" movement mode, whereas a moving one could be in the "walking", "running" or any other mode.
	 * The parameter sent is the new movement mode.
	 * This event will be emitted before the actual mode is changed, so you can call getMovementMode() to get the current movement mode, before the new one is in effect.
	 */
	sigc::signal<void, MovementMode> EventMovementModeChanged;

	/**
	 * @brief Sets the velocity, in local units. This means relative to the way the representation is facing, so that x>1 means a forward looking direction.
	 */
	void setLocalVelocity(const WFMath::Vector<3>& velocity);

protected:

	/**
	 * @brief The entity which this representation is bound to.
	 */
	EmberEntity& mEntity;

	/**
	 * @brief The model of the entity.
	 * This is the main graphical representation of this entity.
	 * Note that the Model won't be directly connected to the main scene node, instead the mModelMount instance will take care of setting everything up to use an intermediary "scale node".
	 */
	Model& mModel;

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
	 * @brief The sound entity this entity is connected to.
	 */
	SoundEntity* mSoundEntity;

	/**
	 * @brief Keep track of the light nodes.
	 */
	SceneNodeStore mLightNodes;

	/**
	 * @brief The type name for the class.
	 */
	static std::string sTypeName;

	/**
	 * @brief The movement mode the entity is in, like walking, running, swimming etc.
	 */
	MovementMode mMovementMode;

	/**
	 * Tells the entity to retrieve it sound actions from
	 * the model definition manager
	 */
	void setSounds();

	bool needSoundEntity();

	void setClientVisible(bool visible);

	/**
	 *   creates EmberEntityUserObjects, connects them and sets up the collision detection system
	 * @return
	 */
	void connectEntities();

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
	void attrChanged(const std::string& str, const Atlas::Message::Element& v);
	/**
	 *    Overridden from Eris::Entity
	 * @param act
	 */
	void entity_Acted(const Atlas::Objects::Operation::RootOperation& act);

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
	 * @brief Called when the movement mode of the entity changes.
	 * For example when the entity changes from standing to walking.
	 * @param newMode The new movement mode.
	 */
	virtual void onMovementModeChanged(MovementMode newMode);

	/**
	 * @brief Parses and sets the movement mode.
	 * The movement mode is determined mainly from whether the entity is moving or not. The speed of the movement also affects the mode.
	 * @param velocity The velocity of the entity.
	 */
	virtual void parseMovementMode(const WFMath::Vector<3>& velocity);

};

}
}
#endif /* MODELREPRESENTATION_H_ */
