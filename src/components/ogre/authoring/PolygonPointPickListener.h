//
// C++ Interface: PolygonPointPickListener
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
#ifndef EMBEROGRE_MANIPULATIONPOLYGONPOINTPICKLISTENER_H
#define EMBEROGRE_MANIPULATIONPOLYGONPOINTPICKLISTENER_H

#include "../IWorldPickListener.h"
#include <sigc++/signal.h>

namespace Ember
{
namespace OgreView
{

namespace Authoring
{

class PolygonPointUserObject;
class Polygon;
class PolygonPoint;

/**
 @brief A listener which knows how to handle PolygonPoints.
 The Polygon class allows for easy editing of polygon through clicking and dragging of the polygon points.
 This listener class will take care of recognizing mouse clicking and initializing movements of the point.
 @author Erik Ogenvik <erik@ogenvik.org>
 */
class PolygonPointPickListener: public IWorldPickListener
{
public:
	/**
	 * @brief Ctor.
	 * @param polygon The polygon to which this listener belongs to.
	 */
	explicit PolygonPointPickListener(Polygon& polygon);

	/**
	 * @brief Dtor.
	 */
	~PolygonPointPickListener() override = default;

	void processPickResult(bool& continuePicking, PickResult& result, Ogre::Ray& cameraRay, const MousePickerArgs& mousePickerArgs) override;

	void initializePickingContext(bool& willParticipate, const MousePickerArgs& pickArgs) override;

	void endPickingContext(const MousePickerArgs& mousePickerArgs, const std::vector<PickResult>& results) override;

	void processDelayedPick(const MousePickerArgs& mousePickerArgs) override;

	sigc::signal<void, PolygonPoint&> EventPickedPoint;

private:
	/**
	 * @brief The polygon to which this listener belongs to.
	 */
	Polygon& mPolygon;

	/**
	 * @brief The picked user object, if any has been picked.
	 */
	PolygonPointUserObject* mPickedUserObject;
};

}

}

}

#endif
