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
    EmberEntityLoader(::PagedGeometry::PagedGeometry *geom);

    virtual ~EmberEntityLoader();
    
	void addEmberEntity(EmberPhysicalEntity* entity);
	void removeEmberEntity(EmberPhysicalEntity* entity);
	
	virtual void loadPage(::PagedGeometry::PageInfo &page);
   
protected:
	::PagedGeometry::PagedGeometry *mGeom;
	EntityMap mEntities;

};

}

}

#endif
