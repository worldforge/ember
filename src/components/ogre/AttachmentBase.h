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

#ifndef ATTACHMENTBASE_H_
#define ATTACHMENTBASE_H_

#include "components/ogre/IEntityAttachment.h"

namespace WFMath
{
class Quaternion;
}

namespace Ogre
{
class SceneNode;
}

namespace EmberOgre
{

class IGraphicalRepresentation;
class EmberEntity;
class IAttachmentControlDelegate;

/**
 * @brief Abstract base class for all attachments which require both a valid parent and child entity.
 *
 * This basically means all attachments except the top world attachment.
 */
class AttachmentBase: public IEntityAttachment
{
public:
	AttachmentBase(EmberEntity& parentEntity, EmberEntity& childEntity);
	virtual ~AttachmentBase();

	virtual IGraphicalRepresentation* getGraphicalRepresentation() const;

	virtual EmberEntity& getAttachedEntity() const;

	virtual EmberEntity* getParentEntity() const;

	virtual void updateScale();

	virtual void updatePosition();

	virtual void getOffsetForContainedNode(const IEntityAttachment& attachment, const WFMath::Point<3>& localPosition, WFMath::Vector<3>& offset);

	virtual void setControlDelegate(IAttachmentControlDelegate* controllerDelegate);

	virtual IAttachmentControlDelegate* getControlDelegate() const;

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

protected:

	EmberEntity& mParentEntity;
	EmberEntity& mChildEntity;

};
}

#endif /* ATTACHMENTBASE_H_ */
