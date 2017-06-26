//
// C++ Interface: ModelBackgroundLoader
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2009
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
#include "ModelDefinition.h"

#include <OgreResourceBackgroundQueue.h>

#include <memory>

namespace Eris
{
class EventService;
}

namespace Ember
{
class TimeFrame;
namespace OgreView
{

namespace Model
{

class Model;
class ModelBackgroundLoader;
class ModelDefinition;

/**
 * @brief A background loading listener attached to an instance of ModelBackgroundLoader.
 *
 * It's main purpose is to pass the call to operationCompleted() on to the background loader.
 * @author Erik Ogenvik <erik@ogenvik.org>
 */
class ModelBackgroundLoaderListener: public Ogre::ResourceBackgroundQueue::Listener
{
public:
	/**
	 * @brief Ctor.
	 * @param loader The loader to which this listener is connected.
	 */
	ModelBackgroundLoaderListener(ModelBackgroundLoader& loader);

	virtual ~ModelBackgroundLoaderListener()
	{
	}

	/**
	 * @brief Called in the main thread when the background operation has completed.
	 * Upon completion, the loader, if such an instance exists, will be notified of this. After this has happened, this instance will delete itself.
	 * @param ticket The ticket which was completed.
	 * @param result The result of the background operation.
	 */
	virtual void operationCompleted(Ogre::BackgroundProcessTicket ticket, const Ogre::BackgroundProcessResult& result);


private:

	/**
	 * @brief The loader to which this listener is attached, or null if it is detached.
	 */
	ModelBackgroundLoader& mLoader;

};

/**
 @brief Responsible for loading the resources needed by a Model.
 If thread support is enabled it will be used. You must then call poll() each frame to see if the background thread has finished loading.

 @author Erik Ogenvik <erik@ogenvik.org>
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
		 * @brief Particle systems are being prepared.
		 */
		LS_PARTICLE_SYSTEM_PREPARING,
		/**
		 * @brief The Materials are being prepared.
		 */
		LS_MATERIAL_PREPARING,
		/**
		 * @brief The Materials have been prepared.
		 */
		LS_MATERIAL_PREPARED,
		/**
		 * @brief Textures are being prepared.
		 */
		LS_TEXTURE_PREPARING,
		/**
		 * @brief Textures have been prepared.
		 */
		LS_TEXTURE_PREPARED,
		/**
		 * @brief The Materials are loading.
		 */
		LS_MATERIAL_LOADING,
		/**
		 * @brief Loading is done.
		 */
		LS_DONE
	};
	typedef std::set<Ogre::BackgroundProcessTicket> TicketStore;

	/**
	 * @brief Ctor.
	 * @param model The model which will be loaded.
	 */
	ModelBackgroundLoader(ModelDefinition& modelDefinition);

	/**
	 * @brief Dtor.
	 */
	virtual ~ModelBackgroundLoader();

	/**
	 * @brief Starts the polling loop.
	 *
	 * If the model isn't yet loaded, we're in either of two states.
	 * We're either waiting for background loading tickets to complete. In this case nothing will be done. @see operationCompleted
	 * Or we don't have any background tickets currently, but bailed out early in the loading process in order to allow for interleaving with the event loop.
	 * In the latter case a new call to "poll" will be posted to the event loop.
	 *
	 * The result of this is that this method only should be called externally once.
	 *
	 * @return True if the loading is complete.
	 */
	bool poll();

	/**
	 * @brief Gets the current loading state.
	 * @return The current loading state.
	 */
	LoadingState getState() const;


protected:

	/**
	 * @brief The model definition which will be loaded.
	 */
	ModelDefinition& mModelDefinition;


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
	 * @brief A listener.
	 */
	ModelBackgroundLoaderListener mListener;

	/**
	 * @brief Keeps track of the currently processed submodel.
	 */
	size_t mSubModelLoadingIndex;

	/**
	 * @brief A set of textures that needs to be loaded.
	 */
	std::set<std::string> mTexturesToLoad;

	/**
	 * @brief A set of materials that needs to be loaded.
	 */
	std::set<Ogre::MaterialPtr> mMaterialsToLoad;

	std::shared_ptr<bool> mIsActive;

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
	 * @brief Called when a background operation has completed.
	 * Note that this call will happen in the main thread.
	 */
	virtual void operationCompleted(Ogre::BackgroundProcessTicket ticket, const Ogre::BackgroundProcessResult& result);


	/**
	 * @brief Polls the loading state (which might occur in a background thread).
	 * If the loading state has progressed it will be updated.
	 * @return True if the loading is complete.
	 */
	bool performLoading();

	void prepareMaterialInBackground(const std::string& materialName);

};

}

}

}

#endif
