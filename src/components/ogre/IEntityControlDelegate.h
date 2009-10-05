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

#ifndef IENTITYCONTROLDELEGATE_H_
#define IENTITYCONTROLDELEGATE_H_

namespace WFMath
{
template<int> class Point;
template<int> class Vector;
class Quaternion;
}

namespace EmberOgre
{

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief Allows control of an entity to be delegated.
 *
 * Under normal operations the position and orientation for an entity used for displaying ingame graphical representations would be obtained directly from the entity. However, by using an instance of this class it's possible to redirect this.
 *
 */
class IEntityControlDelegate
{
public:

	/**
	 * @brief Dtor.
	 */
	virtual ~IEntityControlDelegate()
	{
	}

	/**
	 * @brief Gets the position.
	 * @return The position, in world units and local space.
	 */
	virtual const WFMath::Point<3>& getPosition() const = 0;

	/**
	 * @brief Gets the orientation.
	 * @return The orientation, in local space.
	 */
	virtual const WFMath::Quaternion& getOrientation() const = 0;

	/**
	 * @brief Gets the velocity.
	 * @return The velocity, in world units.
	 */
	virtual const WFMath::Vector<3>& getVelocity() const = 0;
};

}

#endif /* IENTITYCONTROLDELEGATE_H_ */
