/*
 *  me_ct_container.hpp - part of SmartBody-lib's Motion Engine
 *  Copyright (C) 2008  University of Southern California
 *
 *  SmartBody-lib is free software: you can redistribute it and/or
 *  modify it under the terms of the Lesser GNU General Public License
 *  as published by the Free Software Foundation, version 3 of the
 *  license.
 *
 *  SmartBody-lib is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Lesser GNU General Public License for more details.
 *
 *  You should have received a copy of the Lesser GNU General Public
 *  License along with SmartBody-lib.  If not, see:
 *      http://www.gnu.org/licenses/lgpl-3.0.txt
 *
 *  CONTRIBUTORS:
 *      Andrew n marshall, USC
 */

#ifndef ME_CT_CONTAINER_HPP
#define ME_CT_CONTAINER_HPP

#include <sb/SBController.h>
#include <controllers/me_controller_context_proxy.hpp>


/**
 *  Base class for controllers that parent other controllers.
 */
class MeCtContainer : public SmartBody::SBController {
public:
	///////////////////////////////////////////////////////////////////////
	//  Public Inner Classes
	class Context : public MeControllerContextProxy
	{
	friend class MeCtContainer;

	protected:
		///////////////////////////////////////////////////////////////
		//  Private Data
		MeCtContainer* _container;

	public:
		///////////////////////////////////////////////////////////////
		//  Constructors
		Context( MeCtContainer* container, MeControllerContext* context = NULL );

		/**
		 *  Adds/Registers a child with the container context.
		 *  Should only be called from owner container.
		 *
		 *  Public (unlike overridden method) to provide container controller access.
		 */
		void add_controller( MeController* child );

		/**
		 *  Removes/Unregisters from the child with the container context.
		 *
		 *  Public (unlike overridden method) to provide container controller access.
		 */
		void remove_controller( MeController* child );

	};

public:
	MeController* findControllerByHandle(std::string handle);

protected:
	//////////////////////////////////////////////////////////
	//  Private Data
	MeCtContainer::Context* const _sub_context;

	//////////////////////////////////////////////////////////
	//  Private Methods
	MeCtContainer( MeCtContainer::Context* sub_context );


	/** Destructor removes all children before deletion */
	virtual ~MeCtContainer();


	//////// Removed to use pointer
	///**
	// *  Provides a reference to the context to be used by children of this controller.
	// */
	//virtual MeCtContainer::Context* local_context() = 0;

	/** 
	 *  Implements MeController::controller_map_updated().
	 *
	 *  Call remap on all children.
	 */
	virtual void controller_map_updated();

	// Called immeidately after a context has been set.
	virtual void context_updated();


protected:
	// Overrides MeController::controller_start() to call start() on each child.
	virtual void controller_start();

	// Overrides MeController::controller_stop() to call stop() on each child.
	virtual void controller_stop();
};


#endif // ME_CT_CONTAINER_HPP
