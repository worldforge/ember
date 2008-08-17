//
// C++ Interface: EmberEntityLoader
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@iteam.se>, (C) 2008
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
#ifndef EMBEROGRE_ENVIRONMENTEMBERENTITYLOADER_H
#define EMBEROGRE_ENVIRONMENTEMBERENTITYLOADER_H

#include "pagedgeometry/include/PagedGeometry.h"
#include <sigc++/connection.h>

//If set to 1 batches will be used.
//This will speed up the process of getting trees, but will add overhead to adding or removing.
#define EMBERENTITYLOADER_USEBATCH 0

namespace EmberOgre {

class EmberPhysicalEntity;

namespace Environment {

/**
@author Erik Hjortsber <erik.hjortsberg@iteam.se>
@brief A simple struct for storing an entity inst1ance.
* This allows us to keep track of both the latest position of the entity, as well as the connections that have been made.
* The position is needed for updating the paged geometry when the position of the entity changes, since we need to update the page both at the entity's previous position as well as the page in the new position.
* The connections are useful for disconnecting the listeners when we're not listening to the entity anymore (i.e. when it's been removed from the world).
*/
struct EntityInstance
{
	/**
	@brief The entity instance.
	*/
	EmberPhysicalEntity* entity;
	
	/**
	@brief The last known position of the entity.
	When the entity has been moved, we need to update the paged geometry page both at the previous and the new position. Keeping track of the last known position of the entity helps with that.
	*/
	Ogre::Vector3 lastPosition;
	
	/**
	@brief The connection for listening to the Moved event on the entity. When removing the entity, make sure to disconnect this.
	*/
	sigc::connection movedConnection;
	
	/**
	@brief The connection for listening to the BeingDeleted event on the entity. When removing the entity, make sure to disconnect this.
	*/
	sigc::connection beingDeletedConnection;

	/**
	@brief The connection for listening to the VisiblityChanged event on the entity. When removing the entity, make sure to disconnect this.
	*/
	sigc::connection visibilityChangedConnection;

};


/**
	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
	@brief A specialized implementation of PagedGeometry::PageLoader used for positioning instances of EmberPhysicalEntity 
	
	This acts like a bridge between the Ember and the PagedGeometry systems, allowing EmberPhysicalEntity instances from the former being represented in the latter. Entities that are handled by this will be graphically represented through the PagedGeometry engine, rather than by using instances of Model. The class will take care of listening to movement updates and deletions from the entity, automatically updating the paged geometry when the entity changes.
	
	
	Use addEmberEntity to add entities, and removeEmberEntity to remove them.
	
	The storage of entities can either use a straight up map, or use batches, where each batch corresponds to a page in the paged geometry. The latter is more efficient when generating the pages needed by the paged geometry engine, but requires more memory and is less effective when adding or removing entities. The batching can be turned on and off through the EMBERENTITYLOADER_USEBATCH define.
*/
class EmberEntityLoader : public ::PagedGeometry::PageLoader
{
public:
	typedef std::map<std::string, EntityInstance> EntityMap;
	typedef std::map<int, EntityMap> EntityColumn;
	typedef std::map<int, EntityColumn> EntityStore;
	typedef std::map<EmberPhysicalEntity*, std::pair<int, int> > EntityLookup;
	
    /**
     * @brief Ctor.
     * @param geom The geometry for which this class will provide entity loading.
     * @param batchSize The size of each batch. Only relevant if batching is used.
     */
    EmberEntityLoader(::PagedGeometry::PagedGeometry &geom, unsigned int batchSize);

    /**
     * Dtor.
     */
    virtual ~EmberEntityLoader();
    
	/**
	 * @brief Adds an entity to the loader.
	 * This will register an entity with the loader, delegating all rendering to the paged geometry engine.
	 * After an entity has been added, the loader will take care of updating the paged geometry when the entity changes (is moved, deleted etc.)
	 * @param entity The entity which should be added.
	 */
	void addEmberEntity(EmberPhysicalEntity* entity);
	
	/**
	 * @brief Removes an entity from the loader, thus also removing it from the paged geometry.
	 * @param entity The entity which should be removed.
	 */
	void removeEmberEntity(EmberPhysicalEntity* entity);
	
	/**
	 * @copydoc PagedGeometry::PageLoader::loadPage() 
	 */
	virtual void loadPage(::PagedGeometry::PageInfo &page);
   
protected:
#if EMBERENTITYLOADER_USEBATCH
	/**
	@brief The batched store in which the EntityInstance instances are stored.
	*/
	EntityStore mEntities;
	
	/**
	@brief A lookup map, used for looking up which column and segment any EntityInstance is stored. Use this in combination with mEntities to access the EntityMap where a specific entity can be found.
	*/
	EntityLookup mEntityLookup;
#else
	/**
	@brief The main store where we keep our EntityInstance instances.
	*/
	EntityMap mEntities;
#endif
	
	/**
	@brief The main paged geometry instance which will handle all rendering.
	*/
	::PagedGeometry::PagedGeometry &mGeom;

	/**
	@brief The size, in world units, for each batch. Only relevant if batching is used.
	*/
	unsigned int mBatchSize;
	
	
	/**
	 * @brief Listen for movements of the entity and update the paged geometry accordingly.
	 * @param entity The entity which was moved.
	 */
	void EmberEntity_Moved(EmberPhysicalEntity* entity);
	
	/**
	 * @brief Listen for deletion of the entity and update the paged geometry accordingly.
	 * @param entity The entity which will be deleted.
	 */
	void EmberEntity_BeingDeleted(EmberPhysicalEntity* entity);
	
	/**
	 * @brief Listen for visiblity changes and update the paged geometry accordingly.
	 * @param visible Whether the entity is visible or not.
	 * @param entity The entity for which the visibility was changed.
	 */
	void EmberEntity_VisibilityChanged(bool visible, EmberPhysicalEntity* entity);
	
	/**
	 * @brief Utility method for getting the EntityMap a certain entity belongs to.
	 * Since we support batching, getting the EntityMap where an entity is located can require some further lookups if batching is enabled. This method should be used by any other method that needs to do such a lookup.
	 * @param entity The which we should lookup.
	 * @return A pointer to the EntityMap where the entity is stored, or null if no entity map can be found.
	 */
	EntityMap* getStoreForEntity(EmberPhysicalEntity * entity);
};

}

}

#endif
