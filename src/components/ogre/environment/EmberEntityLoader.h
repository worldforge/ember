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

//if set to 1 batches will be used
//this will speed up the process of getting trees, but will add overhead to adding or removing
#define USEBATCH 0

namespace EmberOgre {

class EmberPhysicalEntity;

namespace Environment {


/**
	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
*/
class EmberEntityLoader : public ::PagedGeometry::PageLoader
{
public:
	typedef std::map<std::string, EmberPhysicalEntity*> EntityMap;
	typedef std::map<int, EntityMap> EntityColumn;
	typedef std::map<int, EntityColumn> EntityStore;
	typedef std::map<EmberPhysicalEntity*, std::pair<int, int> > EntityLookup;
	
    EmberEntityLoader(::PagedGeometry::PagedGeometry *geom, unsigned int batchSize);

    virtual ~EmberEntityLoader();
    
	void addEmberEntity(EmberPhysicalEntity* entity);
	void removeEmberEntity(EmberPhysicalEntity* entity);
	
	virtual void loadPage(::PagedGeometry::PageInfo &page);
   
protected:
#if USEBATCH
	EntityStore mEntities;
	EntityLookup mEntityLookup;
#else
	EntityMap mEntities;
#endif
	
	::PagedGeometry::PagedGeometry *mGeom;

	unsigned int mBatchSize;
};

}

}

#endif
