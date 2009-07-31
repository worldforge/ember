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
#include <string>
#include <sigc++/trackable.h>

namespace Atlas
{
namespace Message
{
class Element;
}
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

protected:

	ModelRepresentation& mModelRepresentation;

	/**
	 * @brief The model mount, which takes care of setting up and handling the rotation and orientation of the model.
	 * This also owns the scale node, which will be destroyed when the mount is destroyed.
	 */
	ModelMount* mModelMount;

	ModelFittingStore mFittings;

	void setupFittings();

	void entity_AttrChanged(const Atlas::Message::Element& attributeValue, const std::string& fittingName);
	void fittedEntity_BeingDeleted(EmberEntity* entity);

	void detachFitting(EmberEntity& entity);
	void createFitting(const std::string& fittingName, const std::string& entityId);
};

}
}

#endif /* MODELATTACHMENT_H_ */
