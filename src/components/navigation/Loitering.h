/*
 Copyright (C) 2014 Erik Ogenvik

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

#ifndef LOITERING_H_
#define LOITERING_H_

#include <wfmath/vector.h>
#include <wfmath/axisbox.h>
#include <wfmath/point.h>


#include <sigc++/connection.h>

namespace Eris
{
class Avatar;
}

namespace Ember
{
namespace Navigation
{

class Awareness;

/**
 * @brief Keeps awareness around the avatar until it has moved away from the area.
 *
 * The idea behind this is that in many cases the avatar will loiter around a bit until issuing a new movement command.
 * Therefore we should keep the surrounding area in the awareness until we've moved out from it.
 */
class Loitering
{
public:
	/**
	 * @brief Ctor.
	 * @param awareness Awareness instance.
	 * @param avatar The main avatar.
	 * @param areaSize The size of the loitering area.
	 */
	Loitering(Awareness& awareness, Eris::Avatar& avatar, const WFMath::Vector<2>& areaSize);

	~Loitering();

protected:

	Awareness& mAwareness;
	Eris::Avatar& mAvatar;

	/**
	 * @brief The loiter area.
	 */
	WFMath::AxisBox<2> mLoiterArea;

	/**
	 * @brief The connection to the Eris::Entity::Moving signal.
	 *
	 * This is severed when we move away from the area.
	 */
	sigc::connection mMovedConnection;

	/**
	 * @brief When the entity moves we check if we're still in the loitering area.
	 *
	 * If not we'll cancel the awareness and sever the Eris::Entity::Moving signal connection.
	 * @param isMoving
	 */
	void entity_Moving(bool isMoving);
};

}
}
#endif /* LOITERING_H_ */
