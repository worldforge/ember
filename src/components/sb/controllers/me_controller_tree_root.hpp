/*
 *  me_controller_tree_root.hpp - part of SmartBody-lib's Motion Engine
 *  Copyright (C) 2008  University of Southern California
 *  ( Formerly me_controller_pipeline.hpp )
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
 *      Ed Fast, USC
 */

#ifndef ME_CONTROLLER_TREE_ROOT_HPP
#define ME_CONTROLLER_TREE_ROOT_HPP


#include <string>
#include <limits>
#include <set>

#include <sr/sr_shared_ptr.hpp>  // uses boost
#include <sk/sk_skeleton.h>
#include <sk/sk_channel_array.h>
#include <controllers/me_controller_context.hpp>
#include <controllers/me_evaluation_logger.hpp>

class SbmPawn;



/**
 *  Simple MeEvaluationContext of a series of MeControllers
 *
 *  Implementation requires several little classes, so to avoid polluting the 
 *  namespace, MeControllerTreeRoot is an interface to MeControllerTreeRootImpl.
 *  Create new controller trees using the MeControllerTreeRoot::create() factory method.
 */
class MeControllerTreeRoot
    : public MeControllerContext
{
public:
    ///////////////////////////////////////////////////////////////////////
    // Public Constants
	static std::string CONTEXT_TYPE;

    ///////////////////////////////////////////////////////////////////////
    // Public Methods
	static MeControllerTreeRoot* create();

	const std::string& context_type() const
	{	return CONTEXT_TYPE; }

    /** Implements MeControllerContext::channels() */
    virtual SkChannelArray& channels() = 0;

    /** Implements MeControllerContext:: toBufferIndex(..) */
    virtual int toBufferIndex( int chanIndex ) = 0;

    /** Implements MeControllerContext::childChannelsUpdated(..) */
	virtual void child_channels_updated( MeController* child ) = 0;

    ///** Implements MeEvaluationContext::buffer() */
    //SrBuffer<float>& buffer() = 0;
    //{ return _buffer; }

    ///**
    // *  Implements MeEvaluationContext::invalidate().
    // *
    // *  Clears channels and calls the child controllers setContext(..),
    // *  forcing a remapping.  When children complete their remapping, 
    // *  make sure the buffer is properly sized, then copy data from the
    // *  old buffer.
    // */
    //void invalidate() = 0;

    ///**
    // *  Implements MeEvaluationContext::isValid().
    // *
    // *  MeControllerTreeRoot is always valid (for now).
    // */
    //bool isValid() = 0;
    //{ return true; }

    /**
     *  Sets channel set to skeletons joints.
     *  Currently only supports one entity/skeleton.
     */
	virtual void add_skeleton( const std::string& entityName, SkSkeleton* skeleton ) = 0;

    /**
     *  Removes named entity from the system.
     */
	virtual void remove_skeleton( const std::string& entityName ) = 0;

	/**
	 *  Inserts controller at position, or end if position > countChildren()
	 */
    virtual void add_controller( MeController* ct, unsigned int position = 9999999 ) = 0;

	/**
	 *  Public variant of MeControllerContext::remove_controller(..).
	 */
	virtual void remove_controller( MeController* ct ) = 0;

	/**
	 *  Locates a controller by its handle.
	 */
	virtual MeController* findControllerByHandle(const std::string& handle) = 0;


	/**
	 * Clears the tree of all controller and skeleton references.
	 */
	virtual void clear() = 0;

	/**
	 *  Returns count of controllers in the tree.
	 */
    virtual unsigned int count_controllers() = 0;
    //{ return _controllers.size(); }

    /**
	 *  Returns a pointer to a controller currently in the tree.
	 */
    virtual MeController* controller( unsigned int n ) = 0;
    //{ return (0<=n && n<_controllers.size())? _controllers[n].get(): NULL; }

	/**
	 *  Evaluates all the controllers.
	 */
	virtual void evaluate( double time ) = 0;

	/**
	 *  Returns the most recent frame count.
	 */
	virtual unsigned int getFrameCount() = 0;

	virtual MeFrameData& getLastFrame() = 0;

	virtual void applyBufferToAllSkeletons() = 0;

	virtual void applySkeletonToBuffer() = 0;


	virtual SbmPawn* getPawn() = 0;
	virtual void setPawn(SbmPawn* pawn) = 0;
};


template <class C, class T > // character type, traits
inline std::basic_ostream<C,T>& operator<< ( std::basic_ostream<C,T>& out, const MeControllerTreeRoot& tree ) {
	// Just prints a memory address.
	return out << "MeControllerTreeRoot@0x" << &tree;
}



#endif // ME_CONTROLLER_TREE_ROOT_HPP
