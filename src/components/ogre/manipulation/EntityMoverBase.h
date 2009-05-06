//
// C++ Interface: EntityMoverBase
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2009
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
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//

#ifndef ENTITYMOVERBASE_H_
#define ENTITYMOVERBASE_H_

class EntityMoverBase : public IMovementBridge
{
public:
    /**
     * @brief Ctor.
     * @param entity The entity which should be moved.
     * @param node The node to which the entity belongs.
     */
	EntityMoverBase(EmberEntity& entity, Ogre::SceneNode* node);
	virtual ~EntityMoverBase();

	virtual const WFMath::Quaternion& getOrientation() const;
	virtual const WFMath::Point<3>& getPosition() const;
	virtual void setPosition(const WFMath::Point<3>& position);
	virtual void move(const WFMath::Vector<3>& directionVector);
	virtual void setRotation (int axis, WFMath::CoordType angle);
	virtual void setOrientation(const WFMath::Quaternion& rotation);
	virtual void yaw(WFMath::CoordType angle);

protected:
	/**
	 * @brief The entity which should be moved.
	 */
	EmberEntity& mEntity;
	/**
	 * @brief The node to which the entity is attached.
	 */
	Ogre::SceneNode* mNode;

	/**
	 * @brief The current orientation.
	 */
	mutable WFMath::Quaternion mOrientation;
	/**
	 * @brief The current position.
	 */
	mutable WFMath::Point<3> mPosition;

	std::auto_ptr<Manipulation::SnapToMovement> mSnapping;

};

#endif /* ENTITYMOVERBASE_H_ */
