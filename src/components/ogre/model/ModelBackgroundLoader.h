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
@brief Responsible for loading the resources needed by a Model.
If thread support is enabled it will be used. You must then call poll() each frame to see if the background thread has finished loading.

@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class ModelBackgroundLoader
{
public:
	/**
	 * @brief The diffrent loading states of the Model.
	 * Loading normally progresses through these states in order.
	*/
	enum LoadingState {
		/**
		 * @brief The loading hasn't yet begun.
		 */
		LS_UNINITIALIZED,
		/**
		 * @brief The Meshes are being prepared.
		 */
		LS_MESH_PREPARING,
		/**
		 * @brief The Meshes has been prepared.
		 */
		LS_MESH_PREPARED,
		/**
		 * @brief The Materials are being prepared.
		 */
		LS_MATERIAL_PREPARING,
		/**
		 * @brief The Materials have been prepared.
		 */
		LS_MATERIAL_PREPARED,
		/**
		 * @brief Loading is done.
		 */
		LS_DONE
	};
	typedef std::list<Ogre::BackgroundProcessTicket, Ogre::STLAllocator<Ogre::BackgroundProcessTicket, Ogre::CategorisedAlignAllocPolicy<Ogre::MEMCATEGORY_GENERAL> > > TicketStore;
	
	/**
	 * @brief Ctor.
	 * @param model The model which will be loaded. 
	 */
	ModelBackgroundLoader(Model& model);
	
	/**
	 * @brief Dtor.
	 */
	~ModelBackgroundLoader();
	
	/**
	 * @brief Polls the loading state (which might occur in a background thread).
	 * If the loading state has progressed it will be updated.
	 * @return True if the loading is complete.
	 */
	bool poll();
	
	/**
	 * @brief Gets the current loading state.
	 * @return The current loading state.
	 */
	const LoadingState getState() const;
	
	/**
	 * @brief Reloads the model.
	 */
	void reloadModel();
	
protected:
	
	/**
	 * @brief The model which will be loaded.
	 */
	Model& mModel;
	
	/**
	 * @brief The background loading tickets held by this instance.
	 * As the tickets complete they will be removed from the list.
	 */
	TicketStore mTickets;
	
	/**
	 * @brief The current loading state of the instance.
	 */
	LoadingState mState;
	
	/**
	 * @brief Adds a loading ticket.
	 * @param ticket The ticket.
	 */
	void addTicket(Ogre::BackgroundProcessTicket ticket);

};

}

}

#endif
