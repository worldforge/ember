/*
 Copyright (C) 2009 Erik Ogenvik

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

#include "DelegatingNodeController.h"

#include "domain/IEntityControlDelegate.h"
#include "components/ogre/NodeAttachment.h"
#include <wfmath/quaternion.h>
#include <wfmath/const.h>

namespace Ember {
namespace OgreView {

DelegatingNodeController::DelegatingNodeController(NodeAttachment& attachment, IEntityControlDelegate& attachmentControlDelegate)
		: NodeController(attachment), mAttachmentControlDelegate(attachmentControlDelegate) {

}

DelegatingNodeController::~DelegatingNodeController() {
}

void DelegatingNodeController::updatePosition() {
	mAttachment.setPosition(mAttachmentControlDelegate.getPosition(), mAttachmentControlDelegate.getOrientation() * WFMath::Quaternion(2, WFMath::numeric_constants<float>::pi() / 2), mAttachmentControlDelegate.getVelocity());
}

IEntityControlDelegate* DelegatingNodeController::getControlDelegate() const {
	return &mAttachmentControlDelegate;
}

}
}
