/*
 *  me_controller_context.hpp - part of SmartBody-lib's Motion Engine
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

#ifndef ME_CONTROLLER_CONTEXT_HPP
#define ME_CONTROLLER_CONTEXT_HPP


#include <iostream>
#include <string>
#include <set>


#include <sr/sr_shared_class.h>
#include <sr/sr_buffer.h>
#include <sk/sk_channel_array.h>
#include <controllers/me_controller_context.hpp>


// Predeclare class because of circular reference:
class MeController;
class MeFrameData;

class MeEvaluationLogger;



/**
 *  MeControllerContext interface (pure virtual methods only)
 *  - Abstracts the context of controller evaluation context
 *    (SrScene, SkSkeleton, or parent MeController)
 *  - Lists channels in the order they exist within the controller tree
 */
class MeControllerContext
	: public SrSharedClass
{
	///////////////////////////////////////////////////////////////////////
	//  Private Data
	std::string _name;


public:
	enum State { INVALID, REMAPPING, VALID };

	/**
	 *  Constructor
	 */
	MeControllerContext();

	/**
	 *  Returns a string naming the type of the controller.
	 *
	 *  The char* should be identical for all instances of a type, and thus can
	 *  be used by numerical comparison for runtime type identification.
	 */
	virtual const std::string& context_type() const = 0;

	/**
	 *  Sets the name for this context.
	 */
	virtual void name( const std::string& name );

	/**
	 *  Returns the name for this context.
	 */
    virtual const std::string& name() const;

    /**
     *  Exposes channels' type & order
     *  - Combining controllers will likely return thier own
     *    _context->channels()
     */
    virtual SkChannelArray& channels() = 0;

    /**
	 *  Returns a mapping of channel index to the index of 
	 *  the channel's first float in the buffer.
     */
    virtual int toBufferIndex( int chanIndex ) = 0;

    /**
	 *  Notifies context that a child registered to this context
	 *  has updated its channels().
     */
	virtual void child_channels_updated( MeController* child ) = 0;

	///**
    // *  Checks if the channels set valid (i.e., it will not change before
    // *  the next evaluation).
    // *  - Needed by combining controllers that may have children before
    // *    their own context is set.
    // */
    //virtual bool isValid() = 0;

    ///////////////////////////////////////////////////////////////////////
    // Controller tree traversal methods
    // - Just for a common interface, not part of frame evaluation

    /** Return number of children */
    //virtual int countChildren() = 0;

    /**
     *  Get Nth child 
     *  - may return NULL
     */
    //virtual MeController* child( int n ) = 0;

protected:
	/**
	 *  Sets controller's _context to this.
	 */
	virtual void add_controller( MeController* ct );

	/**
	 *  Sets controller's _context to this.
	 */
	virtual void remove_controller( MeController* ct );
};

/**
 *  MeFrameContext interface (pure virtual methods only)
 *  - Contains the buffer from which to read the previously calculated
 *    joint values and write the new joint values into.  This eleviates
 *    a constant fetch and copy of buffer values for better effiency
 *    with smaller controllers.
 *  - SkScene and Skeleton can now copy values out of this buffer after
 *    each evaluation, rather than root controller.apply() or
 *    scene.update()
 */
class MeFrameData
{
public:
	/**
	 *  Returns the associated controller context.
	 */
	virtual MeControllerContext* context() const = 0;

    /**
	 *  Returns a map of channel index to the index of 
	 *  the channel's first float in the buffer.
     */
    virtual int toBufferIndex( int chanIndex ) const = 0;

	/**
     *  Exposes channel data for read and write.
     */
    virtual SrBuffer<float>& buffer() = 0;

	/**
	 *  Marks channel n as having been updated during the current frame.
	 *  Does nothing if n is an invalid channel index.
	 */
	virtual void channelUpdated( unsigned int n ) = 0;

	/**
	 *  Returns whether this channel has been updated during the current frame.
	 *  Returns false for invalid n.
	 */
	virtual bool isChannelUpdated( unsigned int n ) const = 0;

    ///** 
    // *  Called by children when their channel set changes.
    // */
    //virtual void invalidate() = 0;

    ///**
    // *  Checks if the channels set valid (i.e., it will not change before
    // *  the next evaluation).
    // *  - Needed by combining controllers that may have children before
    // *    their own context is set.
    // */
    //virtual bool isValid() = 0;


	////  The following methods are debugging hooks for derived classes.
	////  Some of these methods are called directly from MeController
	////  during key points in execution.

	///**
	// *  Runtime debugging hook called at the beginning of MeController::evaluate(..)
	// */
	//void controller_pre_evaluate( MeController* ct, double time ) const;

	///**
	// *  Runtime debugging hook called at the end of MeController::evaluate(..)
	// */
	//void controller_post_evaluate( MeController* ct, double time ) const;
};


inline bool operator== ( const MeControllerContext& a, const MeControllerContext& b ) {
	return &a == &b; // identity by memory address?
}

inline bool operator== ( const MeFrameData& a, const MeFrameData& b ) {
	return &a == &b; // identity by memory address?
}

template <class C, class T > // character type, traits
inline std::basic_ostream<C,T>& operator<< ( std::basic_ostream<C,T>& out, const MeControllerContext& context ) {
	// Just prints a memory address.
	return out << "MeControllerContext@0x" << &context;
}


#endif // ME_CONTROLLER_CONTEXT_HPP
