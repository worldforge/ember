//
// C++ Interface: ModelBackgroundLoader
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2009
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
#ifndef EMBEROGRE_MODELMODELBACKGROUNDLOADER_H
#define EMBEROGRE_MODELMODELBACKGROUNDLOADER_H

#include <Ogre.h>
namespace EmberOgre {

namespace Model {

class Model;

/**
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class ModelBackgroundLoader
{
public:
	enum LoadingState {
		LS_UNINITIALIZED,
		LS_MESH_PREPARING,
		LS_MESH_PREPARED,
		LS_MATERIAL_PREPARING,
		LS_MATERIAL_PREPARED,
		LS_DONE
	};
	typedef std::list<Ogre::BackgroundProcessTicket> TicketStore;
	
	ModelBackgroundLoader(Model& model);
	
	~ModelBackgroundLoader();
	
	bool poll(bool reloadIfReady = true);
	
	void addTicket(Ogre::BackgroundProcessTicket ticket);
	
// 	void initialize();
	
	
protected:
	Model& mModel;
	
	TicketStore mTickets;
	
	LoadingState mState;
	
};

}

}

#endif
