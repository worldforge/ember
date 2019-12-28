/*
 Copyright (C) 2009 Erik Ogenvik <erik@ogenvik.org>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "AttachmentBase.h"

#include "domain/EmberEntity.h"
#include "domain/IGraphicalRepresentation.h"

namespace Ember {
namespace OgreView {

AttachmentBase::AttachmentBase(EmberEntity* parentEntity, EmberEntity& childEntity) :
		mParentEntity(parentEntity), mChildEntity(childEntity) {
}

AttachmentBase::~AttachmentBase() = default;

IGraphicalRepresentation* AttachmentBase::getGraphicalRepresentation() const {
	return nullptr;
}

EmberEntity& AttachmentBase::getAttachedEntity() const {
	return mChildEntity;
}

EmberEntity* AttachmentBase::getParentEntity() const {
	return mParentEntity;
}

void AttachmentBase::setControlDelegate(IEntityControlDelegate*) {
}

IEntityControlDelegate* AttachmentBase::getControlDelegate() const {
	return nullptr;
}

void AttachmentBase::updateScale() {
}

void AttachmentBase::updatePosition() {
}

void AttachmentBase::getOffsetForContainedNode(const IEntityAttachment& attachment, const WFMath::Point<3>& localPosition, WFMath::Vector<3>& offset) {
	if (mParentEntity->getAttachment()) {
		WFMath::Vector<3> localPositionShift(mChildEntity.getPredictedPos());
		if (!localPositionShift.isValid()) {
			localPositionShift = WFMath::Vector<3>::ZERO();
		}
		WFMath::Point<3> adjustedLocalPosition = localPosition + localPositionShift;
		mParentEntity->getAttachment()->getOffsetForContainedNode(attachment, adjustedLocalPosition, offset);
	}
}

void AttachmentBase::setVisualize(const std::string& visualization, bool visualize) {
	if (getGraphicalRepresentation()) {
		getGraphicalRepresentation()->setVisualize(visualization, visualize);
	}
}

bool AttachmentBase::getVisualize(const std::string& visualization) const {
	if (getGraphicalRepresentation()) {
		return getGraphicalRepresentation()->getVisualize(visualization);
	}
	return false;
}

}
}
