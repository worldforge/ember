//
// C++ Implementation: ModelBackgroundLoader
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ModelBackgroundLoader.h"
#include "Model.h"

#include <OgreResourceBackgroundQueue.h>

namespace EmberOgre {

namespace Model {

ModelBackgroundLoader::ModelBackgroundLoader(Model& model)
: mModel(model)
{
}


ModelBackgroundLoader::~ModelBackgroundLoader()
{
}


bool ModelBackgroundLoader::checkModelReady(bool reloadIfReady)
{
	if (mTickets.size() > 0) {
		for (TicketStore::iterator I = mTickets.begin(); I != mTickets.end(); ) {
			TicketStore::iterator I_copy = I;
			++I;
			if (Ogre::ResourceBackgroundQueue::getSingleton().isProcessComplete(*I_copy)) {
				mTickets.erase(I_copy);
			}
		}
		if (mTickets.size() == 0) {
			if (reloadIfReady) {
				mModel.reload();
			}
			return true;
		}
		return false;
	}
	return true;
}

void ModelBackgroundLoader::addTicket(Ogre::BackgroundProcessTicket ticket)
{
	mTickets.push_back(ticket);
}

}

}
