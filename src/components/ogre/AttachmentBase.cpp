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

#include "AttachmentBase.h"

#include "components/ogre/EmberEntity.h"
#include "components/ogre/IGraphicalRepresentation.h"

namespace EmberOgre
{

AttachmentBase::AttachmentBase(EmberEntity& parentEntity, EmberEntity& childEntity) :
	mParentEntity(parentEntity), mChildEntity(childEntity)
{
}

AttachmentBase::~AttachmentBase()
{
}

IGraphicalRepresentation* AttachmentBase::getGraphicalRepresentation() const
{
	return 0;
}

EmberEntity& AttachmentBase::getAttachedEntity() const
{
	return mChildEntity;
}

EmberEntity* AttachmentBase::getParentEntity() const
{
	return &mParentEntity;
}

void AttachmentBase::setControlDelegate(IEntityControlDelegate* controllerDelegate)
{
}

IEntityControlDelegate* AttachmentBase::getControlDelegate() const
{
	return 0;
}

void AttachmentBase::updateScale()
{
}

void AttachmentBase::updatePosition()
{
}

void AttachmentBase::getOffsetForContainedNode(const IEntityAttachment& attachment, const WFMath::Point<3>& localPosition, WFMath::Vector<3>& offset)
{
	if (mParentEntity.getAttachment()) {
		WFMath::Vector<3> localPositionShift(mChildEntity.getPredictedPos());
		WFMath::Point<3> adjustedLocalPosition = localPosition + localPositionShift;
		mParentEntity.getAttachment()->getOffsetForContainedNode(attachment, adjustedLocalPosition, offset);
	}
}

void AttachmentBase::setVisualize(const std::string& visualization, bool visualize)
{
	if (getGraphicalRepresentation()) {
		getGraphicalRepresentation()->setVisualize(visualization, visualize);
	}
}

bool AttachmentBase::getVisualize(const std::string& visualization) const
{
	if (getGraphicalRepresentation()) {
		return getGraphicalRepresentation()->getVisualize(visualization);
	}
	return false;
}

}
