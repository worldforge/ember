/*
 *  me_ct_unary.hpp - part of SmartBody-lib's Motion Engine
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

#ifndef ME_CT_UNARY_HPP
#define ME_CT_UNARY_HPP



#include <controllers/me_ct_container.hpp>
#include <controllers/me_controller_context_proxy.hpp>

/**
 *  Virtual controller class for single child operations.
 *
 *  Manages child pointer and overrides MeController
 *  functions to call child as necessary.
 */
class MeCtUnary : public MeCtContainer {
public:
	//////////////////////////////////////////////////////////
	//  Public Constants
	static std::string CONTROLLER_TYPE;

protected:
	//////////////////////////////////////////////////////////
	//  Private Inner Class
	class Context : public MeCtContainer::Context {
	protected:
		//////////////////////////////////////////////////////////
		//  Private Constants
		static std::string CONTEXT_TYPE;

	public:
		Context( MeCtUnary* container, MeControllerContext* context = NULL )
			:	MeCtContainer::Context( container, context )
		{}

		const std::string& context_type() const {	return CONTEXT_TYPE; }

		/**
		 *  Simple implementation MeControllerContext::child_channels_updated(MeController*).
		 *  Copies child's channels and propagates the event to _container's _context.
		 */
		void child_channels_updated( MeController* child );
	};

private:
	//////////////////////////////////////////////////////////
	//  Private Data
	MeController* _child;
	bool          _copy_child_channels;  // Should really be a class template variable


protected:
	//////////////////////////////////////////////////////////
	//  Private Constructor

	//////// Old Constructor comment
	///**
	// *  Constructs a new unary container controller.
	// *
	// *  When subclassing MeCtUnary, do not forget to place the child in the
	// *  local context, and if necessary copy the child's channel array.
	// *  The MeCtUnary constructor does not handles these because the local
	// *  context probably doesn't exist when the superclass constructor is called,
	// *  and any channel handle cannot be easily overrided since the superclass
	// *  constructor must be called.
	// */
	/**
	 *  Constructs a new unary container controller.
	 *
	 *  When subclassing MeCtUnary, do not forget to place the child in the
	 *  local context.  The MeCtUnary constructor does not handles these because
	 *  the local context probably doesn't exist when the superclass constructor
	 *  is called.
	 */
	MeCtUnary( MeCtUnary::Context* sub_context, MeController* child = NULL, bool _copy_child_channels = true );

public:
	//////////////////////////////////////////////////////////
	//  Public Methods
	virtual ~MeCtUnary();

	const std::string& controller_type() const {	return CONTROLLER_TYPE; }

	// Overrides MeController::count_children()
	size_t count_children();

	// Overrides MeController::child(int)
	MeController* child( size_t n );

	// Child controller accessor
	inline MeController* child() {
		return _child;
	}

	// Child controller mutator  (Over-generalizing the init function?)
	virtual void init( MeController* child );

	//! Implements MeCtContainer::remove_child
	virtual bool remove_child( MeController* child );

	/**
	 *  Overrides MeController::controller_channels().
	 *
	 *  Assumes the unary controller's channel set is the same as the child's.
	 */
	SkChannelArray& controller_channels();

protected:
	/**
	 *  Overrides MeController::controller_duration().
	 *
	 *  Assumes that the unary controller will have the same duration as the
	 *  child.  Returns 0 if the child is NULL.
	 */
	virtual double controller_duration();

protected:
	//////////////////////////////////////////////////////////
	//  Private Methods

	/** 
	 *  Implements MeController::controller_map_updated().
	 *
	 *  Call remap on _child, if present.
	 *  Overrides MeCtContainer for efficiency.
	 */
	virtual void controller_map_updated();

	//!  Overrides MeController::print_state since we know there is only at more one child
	virtual void print_children( int tab_count );

	/**
	 *  Copies timing metadata from the child to the unary meta-controller.
	 */
	void update_timing_from_child();
};


#endif // ME_CT_UNARY_HPP
