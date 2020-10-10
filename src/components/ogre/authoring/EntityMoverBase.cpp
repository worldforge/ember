#include <memory>

//
// C++ Class: EntityMoverBase
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2009
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//

#include "EntityMoverBase.h"

#include "domain/EmberEntity.h"
#include "components/ogre/Convert.h"
#include "components/ogre/EntityCollisionInfo.h"

#include <Eris/TypeInfo.h>

#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreManualObject.h>

namespace Ember {
namespace OgreView {
namespace Authoring {

SnapListener* EntityMoverBase::msSnapListener(nullptr);

SnapListener::SnapListener()
		: SnapTo("+snaptomovement", this, "Activates the 'snap to' behavior when moving an entity.", true),
		  mSnappingEnabled(false) {

}

void SnapListener::runCommand(const std::string& command, const std::string& args) {
	if (SnapTo == command) {
		setSnapToEnabled(true);
	} else if (SnapTo.getInverseCommand() == command) {
		setSnapToEnabled(false);
	}
}

void SnapListener::setSnapToEnabled(bool snapTo) {
	mSnappingEnabled = snapTo;
	EventSnappingChanged.emit(snapTo);
}

bool SnapListener::getSnappingEnabled() const {
	return mSnappingEnabled;
}

EntityMoverBase::EntityMoverBase(Eris::Entity* entity, Ogre::Node* node, Ogre::SceneManager& sceneManager) :
		mEntity(entity),
		mNode(node),
		mSceneManager(sceneManager),
		mSnapping(nullptr),
		mOffsetMarker(nullptr)
{

	SnapListener& snapListener = getSnapListener();
	if (snapListener.getSnappingEnabled()) {
		setSnapToEnabled(true);
	}
	snapListener.EventSnappingChanged.connect(sigc::mem_fun(*this, &EntityMoverBase::snapListener_SnappingChanged));

	mOffsetMarker = sceneManager.createManualObject();
	auto sceneNode = dynamic_cast<Ogre::SceneNode*>(node);
	if (sceneNode) {
		sceneNode->attachObject(mOffsetMarker);
	}

}

EntityMoverBase::~EntityMoverBase() {
	mSceneManager.destroyManualObject(mOffsetMarker);
}

const WFMath::Quaternion& EntityMoverBase::getOrientation() const {
	mOrientation = Convert::toWF(mNode->getOrientation());
	return mOrientation;
}

const WFMath::Point<3>& EntityMoverBase::getPosition() const {
	mPosition = Convert::toWF<WFMath::Point<3>>(mNode->_getDerivedPosition());
	return mPosition;
}

void EntityMoverBase::setPosition(const WFMath::Point<3>& position) {
	WFMath::Point<3> finalPosition = position;
	if (position.isValid()) {
		WFMath::Vector<3> adjustment;
		EmberEntity* entity = nullptr;
		if (mSnapping.get() && mSnapping->testSnapTo(position, getOrientation(), adjustment, &entity) && adjustment.isValid()) {
			finalPosition = finalPosition.shift(adjustment);
		}

		if (mOffset) {
			finalPosition.y() += mOffset.get();
		}

		//We need to offset into local space.
		Ogre::Vector3 posOffset = Ogre::Vector3::ZERO;
		if (mNode->getParent()) {
			posOffset = mNode->getParent()->_getDerivedPosition();
		}
		mNode->setPosition(Convert::toOgre(finalPosition) - posOffset);
		newEntityPosition(mNode->getPosition());
		Moved.emit();
	}
}

void EntityMoverBase::move(const WFMath::Vector<3>& directionVector) {
	if (directionVector.isValid()) {
		mNode->translate(Convert::toOgre(directionVector));
		newEntityPosition(mNode->getPosition());
		Moved.emit();
	}
}

void EntityMoverBase::setRotation(int /*axis*/, WFMath::CoordType /*angle*/) {
	//not implemented yet
}

void EntityMoverBase::yaw(WFMath::CoordType angle) {
	mNode->yaw(Ogre::Degree(angle));
	Moved.emit();
}

void EntityMoverBase::setOrientation(const WFMath::Quaternion& rotation) {
	if (rotation.isValid()) {
		//We need to offset into local space.
		Ogre::Quaternion rotOffset = Ogre::Quaternion::IDENTITY;
		if (mNode->getParent()) {
			rotOffset = mNode->getParent()->_getDerivedOrientation();
		}
		mNode->setOrientation(Convert::toOgre(rotation) - rotOffset);
		Moved.emit();
	}
}

void EntityMoverBase::newEntityPosition(const Ogre::Vector3& /*position*/) {
}


void EntityMoverBase::setSnapToEnabled(bool snapTo) {
	if (snapTo) {
		if (!mSnapping && mEntity) {
			mSnapping = std::make_unique<Authoring::SnapToMovement>(*mEntity, *mNode, 2.0f, mSceneManager, true);
			setPosition(Convert::toWF<WFMath::Point<3>>(mNode->getPosition()));
		}
	} else {
		mSnapping.reset();
	}
}

SnapListener& EntityMoverBase::getSnapListener() const {
	if (!msSnapListener) {
		msSnapListener = new SnapListener();
	}
	return *msSnapListener;
}

void EntityMoverBase::snapListener_SnappingChanged(bool snapTo) {
	setSnapToEnabled(snapTo);
}

void EntityMoverBase::setOffset(boost::optional<float> offset) {
	auto existingOffset = mOffset;
	auto position = getPosition();
	mOffset = offset;
	if (existingOffset && position.isValid()) {
		position.y() -= existingOffset.get();
	}
	setPosition(position);

	mOffsetMarker->clear();
	if (offset && offset.get() > 0) {
		mOffsetMarker->begin("BaseWhite", Ogre::RenderOperation::OT_LINE_LIST);
		mOffsetMarker->position(Ogre::Vector3::ZERO);
		mOffsetMarker->position(0, -offset.get(), 0);
		mOffsetMarker->end();
	}

}

boost::optional<float> EntityMoverBase::getOffset() const {
	return mOffset;
}

bool EntityMoverBase::isCollisionResultValid(Ember::OgreView::PickResult& result) {
	if (mEntity) {
		if (result.collisionInfo.type() == typeid(EntityCollisionInfo)) {
			auto& entityCollisionInfo = boost::any_cast<EntityCollisionInfo&>(result.collisionInfo);
			//It's a valid entry if it's not transparent and not the entity which is being moved itself.
			if (!entityCollisionInfo.isTransparent && entityCollisionInfo.entity != mEntity.get()) {
				return true;
			}
		}
	}
	return false;
}


}
}
}
