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

#ifndef MODELATTACHMENT_H_
#define MODELATTACHMENT_H_

#include "components/ogre/NodeAttachment.h"
#include <map>
#include <vector>
#include <string>
#include <sigc++/trackable.h>

namespace Atlas
{
namespace Message
{
class Element;
}
}

namespace Ember {
class AttributeObserver;
}

namespace EmberOgre
{

class IGraphicalRepresentation;
class INodeProvider;
class EmberEntity;

namespace Model
{

class ModelMount;
class ModelRepresentation;
class ModelFitting;

/**
 * @brief Attachment for a ModelRepresentation instance.
 *
 * Use this attachment when you have a ModelRepresentation you need to attach to an entity.
 */
class ModelAttachment: public NodeAttachment, public virtual sigc::trackable
{
public:
	typedef std::map<std::string, ModelFitting*> ModelFittingStore;
	ModelAttachment(EmberEntity& parentEntity, ModelRepresentation& modelRepresentation, INodeProvider* nodeProvider);
	ModelAttachment(ModelAttachment& source, NodeAttachment& newParentAttachment);

	virtual ~ModelAttachment();

	virtual IGraphicalRepresentation* getGraphicalRepresentation() const;

	virtual IEntityAttachment* attachEntity(EmberEntity& entity);

	void updateScale();

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

	virtual void getOffsetForContainedNode(const IEntityAttachment& attachment, const WFMath::Point<3>& localPosition, WFMath::Vector<3>& offset);
	/**
	 * @brief Sets the position and orientation of the node.
	 * @param position The position.
	 * @param orientation The orientation.
	 * @param velocity The velocity.
	 */
	virtual void setPosition(const WFMath::Point<3>& position, const WFMath::Quaternion& orientation, const WFMath::Vector<3>& velocity);

protected:

	typedef std::vector<Ember::AttributeObserver*> AttributeObserverStore;

	/**
	 * @brief The Model representation which this attachment is connected to.
	 */
	ModelRepresentation& mModelRepresentation;

	/**
	 * @brief The model mount, which takes care of setting up and handling the rotation and orientation of the model.
	 * This also owns the scale node, which will be destroyed when the mount is destroyed.
	 * The model mount can be connected to either an Ogre::SceneNode or an Ogre::TagPoint. The former is the normal situation, while the latter is the situation if this attachment represents an attachment to a bone on an Ogre::Skeleton instance.
	 */
	ModelMount* mModelMount;

	/**
	 * @brief The fittings, i.e. the connections to child entities which are connected to bones on the Model which this attachment belongs to.
	 */
	ModelFittingStore mFittings;

	/**
	 * @brief A collection of observers to observe entity attribute changes and attach and detach ModelFittings accordingly.
	 */
	AttributeObserverStore mFittingsObservers;

	/**
	 * @brief Sets up the ModelFittings and the observers of the attributes to which they are connected.
	 */
	void setupFittings();

	void entity_AttrChanged(const Atlas::Message::Element& attributeValue, const std::string& fittingName);
	void fittedEntity_BeingDeleted(EmberEntity* entity);

	/**
	 * @brief Detaches a previously attached fitting.
	 * @param entity The entity to attach.
	 */
	void detachFitting(EmberEntity& entity);


	/**
	 * @brief Creates a new ModelFitting instance.
	 * @param fittingName The name of the fitting.
	 * @param entityId The entity id of the attached entity.
	 *
	 * It's safe to call this before the attached entity has appeared in the world. In that case the attachment will be delayed until attachEntity() is later on called.
	 */
	void createFitting(const std::string& fittingName, const std::string& entityId);

	/**
	 * @brief Re-attach entities which previously have been attached.
	 * Call this when the Model is reloaded.
	 */
	void reattachEntities();

	virtual void setVisible(bool visible);

	void model_Reloaded();
};

}
}

#endif /* MODELATTACHMENT_H_ */
