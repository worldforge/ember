//
// C++ Interface: EntityMover
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2006
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
#ifndef EMBEROGREENTITYMOVER_H
#define EMBEROGREENTITYMOVER_H

#include "EntityMoverBase.h"
#include "components/ogre/EmberOgrePrerequisites.h"
#include "domain/IEntityControlDelegate.h"

#include <wfmath/point.h>

namespace Ember
{
namespace OgreView
{
class NodeAttachment;

namespace Authoring
{

class SnapToMovement;

class EntityMoveManager;

/**
 @author Erik Ogenvik <erik@ogenvik.org>

 @brief An implementation of IEntityMoveBridge which allows for EmberEntity instances to be moved.
 */
class EntityMover: public Authoring::EntityMoverBase
{
public:

	EntityMover(NodeAttachment& NodeAttachment, EntityMoveManager& manager);

	~EntityMover() override;

	void finalizeMovement() override;

	void cancelMovement() override;

	const IEntityControlDelegate& getControlDelegate() const;

protected:

	void newEntityPosition(const Ogre::Vector3& position) override;

	void cleanup();

private:

	NodeAttachment& mNodeAttachment;
	EntityMoveManager& mManager;

	IEntityControlDelegate* mPreviousControlDelegate;
	IEntityControlDelegate* mControlDelegate;

};

class EntityMoverControlDelegate: public IEntityControlDelegate
{
public:

	explicit EntityMoverControlDelegate(EntityMover& entityMover);

	const WFMath::Point<3>& getPosition() const override;

	const WFMath::Quaternion& getOrientation() const override;

	const WFMath::Vector<3>& getVelocity() const override;

protected:
	EntityMover& mEntityMover;

};

}
}

}

#endif
