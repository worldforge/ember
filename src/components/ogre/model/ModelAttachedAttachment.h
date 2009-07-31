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

#ifndef MODELATTACHEDATTACHMENT_H_
#define MODELATTACHEDATTACHMENT_H_

#include "components/ogre/AttachmentBase.h"
#include <string>
#include <sigc++/trackable.h>

namespace EmberOgre
{

class IGraphicalRepresentation;
class EmberEntity;

namespace Model
{

class Model;
class ModelMount;
class ModelRepresentation;
class ModelAttachment;

class ModelAttachedAttachment: public AttachmentBase, public virtual sigc::trackable
{
public:
	ModelAttachedAttachment(EmberEntity& parentEntity, ModelRepresentation& modelRepresentation, Model& rootModel, const std::string& attachPointName);
	ModelAttachedAttachment(ModelAttachedAttachment& source, ModelAttachment& newParentAttachment);

	virtual ~ModelAttachedAttachment();

	virtual IGraphicalRepresentation* getGraphicalRepresentation() const;

	virtual IEntityAttachment* attachEntity(EmberEntity& entity);

protected:


	ModelRepresentation& mModelRepresentation;

	/**
	 * @brief The model mount, which takes care of setting up and handling the rotation and orientation of the model.
	 * This also owns the scale node, which will be destroyed when the mount is destroyed.
	 */
	ModelMount* mModelMount;

	Model& mRootModel;
};

}
}

#endif /* MODELATTACHEDATTACHMENT_H_ */
