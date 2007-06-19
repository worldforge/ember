//
// C++ Interface: IWorldPickListener
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2006
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
#ifndef EMBEROGREIWORLDPICKLISTENER_H
#define EMBEROGREIWORLDPICKLISTENER_H

#include "EmberOgrePrerequisites.h"

namespace EmberOgre {

/**
	@author Erik Hjortsberg <erik@katastrof.nu>
*/

/**
The kind of mouse click operation.
*/
enum MousePickType
{
	///Simple click
	MPT_CLICK = 1,
	///Double click
	MPT_DOUBLECLICK = 2

};

/**
Mouse picking info from the windowing system.
*/
struct MousePickerArgs
{
	/**
	The x and y coords in local window space.
	*/
	float windowX, windowY;
	
	MousePickType pickType;
};


class IWorldPickListener
{
public:

virtual ~IWorldPickListener() {}

/**
Called at the start of a picking context. This allows the pickers to be reset and to keep state for each picking.
*/
virtual void initializePickingContext() {}

/**
Called when the picking is over, either because one of the processPickResult calls set continuePicking to false, or because there are no more objects to pick.
*/
virtual void endPickingContext(const MousePickerArgs& mousePickerArgs) {}

/**
 * Processes the pick result.
 * This will be called for each picked object.
 * @param continuePicking set this to false if you don't want to process any more pick results, or don't want any other IWorldPickListener to process the pick any more
 * @param entry The actual pick entry.
 * @param cameraRay The ray which resulted in the pick.
 * @param mousePickerArgs The original mouse picker arguments.
 */
virtual void processPickResult(bool& continuePicking, Ogre::RaySceneQueryResultEntry& entry, Ogre::Ray& cameraRay, const MousePickerArgs& mousePickerArgs) = 0;


};

}

#endif
