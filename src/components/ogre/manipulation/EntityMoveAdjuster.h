//
// C++ Interface: EntityMoveAdjuster
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
#ifndef EMBEROGREENTITYMOVEADJUSTER_H
#define EMBEROGREENTITYMOVEADJUSTER_H

#include <Eris/Timeout.h>

namespace EmberOgre {

class EmberEntity;
class EntityMoveAdjuster;
class EntityMoveManager;

/**
	@see EntityMoveAdjuster
	An instance of a adjustment operation. After time out, the entity encapsuled by this instance will be synchronized with the server.
*/
class EntityMoveAdjustmentInstance
{
public: 
	/**
	 *    Default ctor.
	 * @param moveAdjuster 
	 * @param entity 
	 * @return 
	 */
	EntityMoveAdjustmentInstance(EntityMoveAdjuster* moveAdjuster, EmberEntity* entity);
	
private:
	/**
	The actual entity.
	*/
	EmberEntity* mEntity;
	
	/**
	The timeout object which provides timeout functionality.
	*/
	Eris::Timeout mTimeout;
	
	/**
	 *    Called when the time has expired.
	 */
	void timout_Expired();
	
	/**
	A reference to the owner object.
	*/
	EntityMoveAdjuster* mMoveAdjuster;
};

/**
This class is responsible for adjusting moved entities to their correct place.
Basically, when an entity is moved the client sends the updates to the server, but it's not clear at that time whether the movement is allowed. This can only be seen by waiting to see whether the movement went through, i.e. if the entity was updated.
So what this class does, together with EntityMoveAdjustmentInstance, waiting a couple of milleseconds and then telling the entity that  was moved to synchronize with the server. If the movement didn't go through, this will lead to the entity "snapping" back to the original position. If it did go through nothing will happen.
*/
class EntityMoveAdjuster
{
friend class EntityMoveAdjustmentInstance;
public:
	/**
	 *    Default ctor.
	 * @param manager 
	 * @return 
	 */
	EntityMoveAdjuster(EntityMoveManager* manager);
private:
	typedef std::vector<EntityMoveAdjustmentInstance*> MoveAdjustmentInstanceStore;
	
	/**
	Holds all instances of EntityMoveAdjustmentInstance.
	*/
	MoveAdjustmentInstanceStore mInstances;
	
	/**
	 *    Removes the supplied instance from the list of instances.
	 * @param  
	 */
	void removeInstance(EntityMoveAdjustmentInstance*);
	
	/**
	 *    When movement has finished, i.e. something has been moved, create an EntityMoveAdjustmentInstance which will update the entity at expiration.
	 */
	void EntityMoveManager_FinishedMoving();
	
	/**
	 *    When movement has been cancelled, there's no need to create any EntityMoveAdjustmentInstance, but we want to clean up.
	 */
	void EntityMoveManager_CancelledMoving();
	
	/**
	 *    When movement starts, register the entity that is being moved.
	 * @param entity 
	 */
	void EntityMoveManager_StartMoving(EmberEntity* entity);
	
	/**
	The entity that is being moved.
	*/
	EmberEntity* mActiveEntity;
	
	/**
	A reference to the manager.
	*/
	EntityMoveManager* mManager;
};

}

#endif
