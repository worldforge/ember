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

#ifndef IATTACHMENTCONTROLDELEGATE_H_
#define IATTACHMENTCONTROLDELEGATE_H_

namespace WFMath
{
template<int> class Point;
class Quaternion;
}

namespace EmberOgre
{

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief Allows control of an IEntityAttachment to be delegated.
 *
 * Under normal operations an instance of IEntityAttachment would get position and orientation data directly from the entity to which it's attached. However, by using an instance of this class it's possible to redirect this.
 *
 */
class IAttachmentControlDelegate
{
public:

	/**
	 * @brief Dtor.
	 */
	virtual ~IAttachmentControlDelegate()
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
};

}

#endif /* IATTACHMENTCONTROLDELEGATE_H_ */
