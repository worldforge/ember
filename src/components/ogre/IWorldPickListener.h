//
// C++ Interface: IWorldPickListener
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2006
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

namespace Ogre {
class RaySceneQueryResultEntry;
class Ray;
}

namespace EmberOgre {



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
	The x coords in local window space.
	*/
	float windowX;
	/**
	The y coords in local window space.
	*/
	float windowY;
	
	/**
	The type of picking (either singe or double click).
	*/
	MousePickType pickType;
};


/**
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
	
	When a pick operation is performed in the world, i.e. when the user clicks the mouse on something in the world, the AvatarCamera will first check what was picked, which will most often result in a list of Ogre::MovableObjects, and then for each of these objects as its internal list of IWorldPickListener instances whether any of those knows how to handle the object picked.
	This allows us a great deal of flexibility when we want to add support for different types of things that can be interacted with in the world.
	
	The first stage of the picking action is that the initializePickingContext() method is called. This allows the listeners to do some cleanup before the actual processing. Then processPickResult(...) will be called for each object, in order of the nearness to the camera (nearer objects being handled first). If any of the listeners sets the argument continuePicking to false, the picking action will then end, and endPickingContext() will be called.
	
	Normal operation for an implementation of this is then, if it can handle the object picked, to mark it, but wait with performing the action action until endPickingContext(...) is called (as opposed to performing the action directly when processPickResult(...) is called).
	
*/
class IWorldPickListener
{
public:

virtual ~IWorldPickListener() {}

/**
@brief Called at the start of a picking context.
This allows the pickers to be reset and to keep state for each picking.
*/
virtual void initializePickingContext() {}

/**
@brief Called when the picking is over, either because one of the processPickResult calls set continuePicking to false, or because there are no more objects to pick.
 * @param mousePickerArgs The original mouse picking arguments.
 */
virtual void endPickingContext(const MousePickerArgs& mousePickerArgs) {}

/**
 * @brief Processes the pick result.
 * This will be called for each picked object.
 * This is where you need to put your logic checking whether the picked object is something that your implementation knows how to handle. If you don't want any other listeners to then act on the object (which you in most cases don't want) you need to set the continuePicking argument to false.
 * @param continuePicking set this to false if you don't want to process any more pick results, or don't want any other IWorldPickListener to process the pick any more
 * @param entry The actual pick entry.
 * @param cameraRay The ray which resulted in the pick.
 * @param mousePickerArgs The original mouse picker arguments.
 */
virtual void processPickResult(bool& continuePicking, Ogre::RaySceneQueryResultEntry& entry, Ogre::Ray& cameraRay, const MousePickerArgs& mousePickerArgs) = 0;


};

}

#endif
