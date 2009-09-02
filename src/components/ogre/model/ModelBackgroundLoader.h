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

#include "components/ogre/EmberOgrePrerequisites.h"

#include <OgreResourceBackgroundQueue.h>

namespace EmberOgre
{

namespace Model
{

class Model;
class ModelBackgroundLoader;

/**
 * @brief A background loading listener attached to an instance ModelBackgroundLoader.
 *
 * An instance of this is self contained and will destroy itself when the operation is complete.
 * It's main purpose is to pass the call to operationCompleted() on to the background loader.
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 */
class ModelBackgroundLoaderListener: public Ogre::ResourceBackgroundQueue::Listener
{
public:
	/**
	 * @brief Ctor.
	 * @param loader The loader to which this listener is connected.
	 */
	ModelBackgroundLoaderListener(ModelBackgroundLoader& loader);

	/**
	 * @brief Called in the main thread when the background operation has completed.
	 * Upon completion, the loader, if such an instance exists, will be notified of this. After this has happened, this instance will delete itself.
	 * @param ticket The ticket which was completed.
	 * @param result The result of the background operation.
	 */
	virtual void operationCompleted(Ogre::BackgroundProcessTicket ticket, const Ogre::BackgroundProcessResult& result);

	/**
	 * @brief Detaches the listener from the loader.
	 * Be sure to call this method on any existing listeners if the loader to which they belong to is deleted.
	 */
	void detachFromLoader();

private:

	/**
	 * @brief The loader to which this listener is attached, or null if it is detached.
	 */
	ModelBackgroundLoader* mLoader;

	/**
	 * @brief Dtor.
	 * This is private to make sure that an instance of this only can be deleted by itself (through a call to operationCompleted()).
	 */
	virtual ~ModelBackgroundLoaderListener()
	{
	}
	;
};

/**
 @brief Responsible for loading the resources needed by a Model.
 If thread support is enabled it will be used. You must then call poll() each frame to see if the background thread has finished loading.

 @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 */
class ModelBackgroundLoader
{
	friend class ModelBackgroundLoaderListener;
public:
	/**
	 * @brief The different loading states of the Model.
	 * Loading normally progresses through these states in order.
	 */
	enum LoadingState
	{
		/**
		 * @brief The loading hasn't yet begun.
		 */
		LS_UNINITIALIZED,
		/**
		 * @brief The Meshes are being prepared.
		 */
		LS_MESH_PREPARING,
		/**
		 * @brief The Meshes have been prepared.
		 */
		LS_MESH_PREPARED,
		/**
		 * @brief The Meshes are loading.
		 */
		LS_MESH_LOADING,
		/**
		 * @brief The Meshes have been loaded.
		 */
		LS_MESH_LOADED,
		/**
		 * @brief The Materials are being prepared.
		 */
		LS_MATERIAL_PREPARING,
		/**
		 * @brief The Materials have been prepared.
		 */
		LS_MATERIAL_PREPARED,
		/**
		 * @brief The Materials are loading.
		 */
		LS_MATERIAL_LOADING,
		/**
		 * @brief The Materials have been loaded.
		 */
		LS_MATERIAL_LOADED,
		/**
		 * @brief The Textures are being prepared.
		 */
		LS_TEXTURES_PREPARING,
		/**
		 * @brief The Texture have been prepared.
		 */
		LS_TEXTURES_PREPARED,
		/**
		 * @brief The Textures are loading.
		 */
		LS_TEXTURES_LOADING,
		/**
		 * @brief Loading is done.
		 */
		LS_DONE
	};
	typedef std::list<Ogre::BackgroundProcessTicket, Ogre::STLAllocator<Ogre::BackgroundProcessTicket, Ogre::CategorisedAlignAllocPolicy<Ogre::MEMCATEGORY_GENERAL> > > TicketStore;
	typedef std::vector<ModelBackgroundLoaderListener*> ListenerStore;

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
	 * @brief A store of all listeners created by this instance.
	 * At destruction time, if there are any listeners left, these must be detached (they cannot be deleted by this class).
	 */
	ListenerStore mListeners;

	/**
	 * @brief Adds a loading ticket.
	 * @param ticket The ticket.
	 */
	void addTicket(Ogre::BackgroundProcessTicket ticket);

	/**
	 * @brief Checks to see if there are any tickets left.
	 * @return True if there aren't any tickets left.
	 */
	bool areAllTicketsProcessed();

	/**
	 * @brief Creates a new listener and registers it with this class.
	 * @return A new listener instance. This instance isn't owned by anything and will delete itself when the task it listens for is complete.
	 */
	ModelBackgroundLoaderListener* createListener();

	/**
	 * @brief Called when a background operation has completed.
	 * Note that this call will happen in the main thread.
	 */
	virtual void operationCompleted(Ogre::BackgroundProcessTicket ticket, const Ogre::BackgroundProcessResult& result, ModelBackgroundLoaderListener* listener);

};

}

}

#endif
