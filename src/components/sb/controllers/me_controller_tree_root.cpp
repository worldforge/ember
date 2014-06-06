/*
 *  me_controller_tree_root.cpp - part of SmartBody-lib's Motion Engine
 *  Copyright (C) 2008  University of Southern California
 *  ( Formerly me_controller_pipeline.cpp )
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

#include "vhcl.h"

#include <list>
#include <vector>
#include <iostream>
#include <algorithm>

#include <controllers/me_controller_tree_root.hpp>
#include <controllers/me_ct_container.hpp>

#include <sr/sr.h>
#include <sk/sk_channel_array.h>

#include <sb/sbm_pawn.hpp>
#include <sb/SBSimulationManager.h>
#include <sb/SBScene.h>


using namespace std;



std::string MeControllerTreeRoot::CONTEXT_TYPE = "MeControllerTreeRoot";

class MeControllerTreeRootImpl;


///////////////////////////////////////////////////////////////////////////
//  TreeRootFrameData
class TreeRootFrameData : public MeFrameData {
	MeControllerTreeRootImpl* _context;
	unsigned int              _count_cur;
	vector<unsigned int>      _updated;   // channel[n] was last updated on frame _updated[n]
	SrBuffer<int>             _map;       // channel[n] data is located at buffer[ map[n] ]
	SrBuffer<float>           _buffer;

	//  Debugging data
	double _last_time;    // Time for last frame evaluation
	double _debug_until;  // Enable debug-hooks until this time

	
public:
	TreeRootFrameData( MeControllerTreeRootImpl* tree_root )
		:	_context( tree_root ),
			_updated(), _buffer(),
			_count_cur( 0 ),
			_last_time( 0 ),
			_debug_until( 4 )
	{}

	MeControllerContext* context() const;
		// defined at bottom of this file (attempt to downcast _context)

	int toBufferIndex( int chanIndex ) const {
		if( chanIndex < 0 || _map.size() <= chanIndex )
			return -1;
		return _map[ chanIndex ];
	}

	SrBuffer<float>& buffer() {
		return _buffer;
	}

	void channelUpdated( unsigned int n ) {
		SR_ASSERT( n < _updated.size() );
		_updated[n] = _count_cur;
	}


	bool isChannelUpdated( unsigned int n ) const {
		return ( n < _updated.size() ) &&     // valid n?
			( _updated[n] == _count_cur ); // actual check
	}

	///**
	// *  Implement runtime debugging hook called at the beginning of MeController::evaluate(..)
	// *
	// *  Prints the data selected channels.
	// */
	//void ct_pre_evaluate( MeController* ct ) {
	//	if( _last_time < _debug_until ) {
	//		// TODO: implement TreeRootFrameData::ct_pre_evaluate
	//		std::cout << "DEBUG: pre_evaluate: " << ct->controller_type() << " \"" << ct->name() << '\"' << std::endl;
	//	}
	//}

	///**
	// *  Implement runtime debugging hook called at the end of MeController::evaluate(..)
	// *
	// *  Prints the data selected channels.
	// */
	//void ct_post_evaluate( MeController* ct ) {
	//	if( _last_time < _debug_until ) {
	//		// TODO: implement TreeRootFrameData::ct_post_evaluate
	//		std::cout << "DEBUG: post_evaluate: " << ct->controller_type() << " \"" << ct->name() << '\"' << std::endl;
	//	}
	//}


private:
	// Private classes for MeControllerTreeRoot
	friend class MeControllerTreeRootImpl;

protected:
	void incrementFrameCount( double time );
		// defined at bottom of this file (refers to _context members)

	void remapBuffers( SkChannelArray& cur, SkChannelArray& prev );
		// defined at bottom of this file (refers to _context members)
};

///////////////////////////////////////////////////////////////////////////
//  Helper algorithms
struct Evaluate_Func {
	const double  time;
	MeFrameData&  frame;

	Evaluate_Func( double time, MeFrameData& frame )
		:	time(time), frame(frame) {}

	void operator() ( controller_ptr& ct )
	{
		ct->evaluate( time, frame );
	}
};

// Since std::mem_fun( &MeController::remap ) doesn't seem to work with controller_ptr
struct RemapAndActivate_Func {
	void operator() ( controller_ptr& ct ){
		ct->remap();
		if( !ct->active() ) {
			ct->start(SmartBody::SBScene::getScene()->getSimulationManager()->getTime());
		}
	}
};


///////////////////////////////////////////////////////////////////////////
//  MeControllerTreeRootImpl
class MeControllerTreeRootImpl
    : public MeControllerTreeRoot
{
protected:
    ///////////////////////////////////////////////////////////////////////
    // Protected Fields

	// TODO: handle more than one entity/skeleton
	std::string   _skeletonName;
    SkSkeleton*    _skeleton;
	State          _state;   // Need to be lockable, even for single threading

	//  We keep two copies of channels arround to allow comparisons
	//  after adding/removing skeletons.
	//  Also reduces memory allocation on every such update.
	SkChannelArray _channels[2];
	int            _channels_cur; // index to current channel

	std::vector< controller_ptr >  _controllers;
	TreeRootFrameData              _frame_data;

	//SrBuffer<unsigned int> _updated;   //  Channel n was last updated on frame _updated[n]
    //SrBuffer<float>        _buffer;    // the output of the controller

	MeEvaluationLogger* _logger;
	set<string>         _logged_joints;
	set<int>            _logged_channel_indices;
	SbmPawn*			_pawn;


public:
    ///////////////////////////////////////////////////////////////////////
    // Public Methods

	// Constructor
	MeControllerTreeRootImpl()
	:	_skeletonName(""),
		_skeleton(NULL),
		_state(VALID),
		_channels_cur(0),
		_controllers(),
		_frame_data(this),  // Ignore this warning... no member access
		_logger( NULL )
	{
		// Make sure the ChannelArray members are zeroed out
		_channels[0].init();
		_channels[1].init();
	}

	// Destructor
	~MeControllerTreeRootImpl() {
		/*
		vector< controller_ptr >::iterator ct_iter = _controllers.begin();
		vector< controller_ptr >::iterator ct_end = _controllers.end();
		while( ct_iter != ct_end ) {
			(*ct_iter)->unref();
			(*ct_iter) = NULL;

			++ct_iter;
		}*/
		//printf("after remove controllers\n");

		if( _skeletonName != "" )
			remove_skeleton( _skeletonName );
		if( _logger )
			_logger->unref();
	}


	// Implement MeControllerContext
    SkChannelArray& channels() {
#if 0
		if( _state!=State::VALID ) 
			LOG("ERROR: MeControllerTreeRoot::channels() called while invalid");
#endif
		if( _state==INVALID )
			remapFrameData();

		return _channels[_channels_cur];
	}

	// Implement MeControllerContext
	int toBufferIndex( int chanIndex ) {
		return _frame_data.toBufferIndex( chanIndex );
	}

	void child_channels_updated( MeController* child ) {
		// Force a revalidation on next evaluate.
		_state = INVALID;
	}

    //bool isValid() {
    //	return true;
	//}
	//
	//void MeControllerTreeRootImpl::invalidate() {
	//	// reset the channels
	//	_channels.init();
	//
	//	// remap channels
	//	int cChild = countChildren();
	//	for( int i=0; i<cChild; i++ ) {
	//		MeController* child = _children[i];
	//		if( child ) {
	//			child->eval_context( this );
	//		}
	//	}
	//
	//	//  Set buffer size
	//	_buffer.size( _channels.floats() );
	//}

    /**
     *  Sets channel set to skeletons joints.
     *  Currently only supports one entity/skeleton.
     */
	void add_skeleton( const std::string& entityName, SkSkeleton* skeleton ) {		
		SR_ASSERT( _state!=REMAPPING );  // simple lock

		if( skeleton==NULL ) {
			LOG("MeEvaluationContextSimple::addSkeleton(..): skeleton is NULL");
			return;
		}
		if( _skeleton!=NULL && _skeleton!=skeleton ) {
			LOG("MeEvaluationContextSimple::addSkeleton(..): Unimplemented: Multiple Skeletons.");
			return;
		}		
		_skeletonName = entityName;		
		_skeleton = skeleton;		
		_skeleton->ref();

		_state = INVALID;
		
	}

    /**
     *  Removes named entity from the system.
     */
	void remove_skeleton( const std::string& entityName ) {
		SR_ASSERT( _state!=REMAPPING );  // simple lock
		if( _skeletonName==entityName ) {
			_skeletonName = "";
			//_skeleton->unref();
			_skeleton = NULL;
		}
	}

	void clear() {
		// Remove controllers
		// commented out by Shapiro 5/4/11 - need to investigate why this causes a crash in release mode
		//_controllers.clear(); // yay for smart pointers


		// Remove skeletons
		//if( _skeleton )  // only one in this implementation
		//	remove_skeleton( _skeleton->getName() );
	}

	SbmPawn* getPawn()
	{
		return _pawn;
	}

	void setPawn(SbmPawn* pawn)
	{
		_pawn = pawn;
	}


	/**
	 *  Inserts controller at position, or end if position > countChildren()
	 */
    void add_controller( MeController* ct,
                        unsigned int position = std::numeric_limits<unsigned int>::max() )
	{
		SR_ASSERT( _state!=REMAPPING );  // simple lock
		// Check bounds
		unsigned int count = count_controllers();
		if( position > count )
			position = count;

		// Insert
		_controllers.insert( _controllers.begin()+position, ct );
		MeControllerContext::add_controller( ct );

		if( _state==VALID ) {
			ct->remap();
			ct->start(SmartBody::SBScene::getScene()->getSimulationManager()->getTime());
		}
	}

	/**
	 *	Removes a controller from the controller tree.
	 */
	void remove_controller( MeController* ct )
	{	
		MeControllerContext::remove_controller( ct );
	}

	MeController* findControllerByHandle(const std::string& handle)
	{
		for (size_t x = 0; x < _controllers.size(); x++)
		{
			if (_controllers[x]->handle() == handle)
			{
				return _controllers[x];
			}
			else
			{
				MeCtContainer* container = dynamic_cast<MeCtContainer*>(_controllers[x]);
				if (container)
				{
					MeController* controller = container->findControllerByHandle(handle);
					if (controller)
						return controller;
				}
			}
		}
		return NULL;
	}

	/**
	 *  Returns count of controllers in the tree.
	 */
    unsigned int count_controllers() 
    { return ( _controllers.size() ); }

    /**
	 *  Returns a pointer to a controller currently in the tree.
	 */
    MeController* controller( unsigned int n ) 
    { return (0<=n && n<_controllers.size())? _controllers[n]: NULL; }

	/**
	 *  Evaluates all the controllers.
	 */
	void evaluate( double time ) {
		SR_ASSERT( _state!=REMAPPING );  // simple lock
		if( _state==INVALID ) {
			remapFrameData();
		}
		SR_ASSERT( _state==VALID );

		_frame_data.incrementFrameCount( time );

		// reset the frame to zero
		SrBuffer<float>& buffer = _frame_data.buffer();
//		buffer.setall(0); // also need to accomodate quaternions

		if( _logger )
			_logger->context_pre_evaluate( time, *this, _frame_data );
		//for_each( _controllers.begin(), 
		//          _controllers.end(),
		//          Evaluate_Func( time, _frame_data ) );
		for (unsigned int i=0;i<_controllers.size();i++)
			_controllers[i]->evaluate(time,_frame_data);

		if( _logger )
			_logger->context_post_evaluate( time, *this, _frame_data );
	}

	/**
	 *  Returns the most recent frame count.
	 */
	unsigned int getFrameCount() {
		return _frame_data._count_cur;
	}

	/**
	 *  Returns the MeFrameData of the last frame evaluated.
	 */
	MeFrameData& getLastFrame() {
		return _frame_data;
	}

	///**
	// *  Returns a map of channel index to the index of 
	// *  the channel's first float in the buffer.
	// */
	//SrBuffer<int>& getLastFrameMap() {
	//	if( !_valid )
	//		remapFrameData();
	//	return _frame_data.map();
	//}
	//
	///**
	// *  Returns the results of the last frame.
	// */
	//SrBuffer<float>& getLastFrameBuffer() {
	//	if( !_valid )
	//		remapFrameData();
	//	return _frame_data.buffer();
	//}

	/**
	 *  Copies channel buffer data from the last frame's buffer.
	 *
	 *  This should be considered a tempory replacement for MeController::apply().
	 *  A better solution is to build a controller or controller context capable 
	 *  of copying the data directly into the scene graph.
	 */
	void applyBufferToAllSkeletons() {
		SR_ASSERT( _state!=REMAPPING );
		if( _skeleton ) {
			if( _state==INVALID )
				remapFrameData();

			SkChannelArray& channels = _channels[_channels_cur];
			SrBuffer<float>& buffer = _frame_data.buffer();
			int size = channels.size();
			const int BEGIN =0;
			const int END = size;

			//// Debug pointer range
			//float* fp_min = &( buffer.get( _frame_data.toBufferIndex( 0 ) ) );
			//float* fp_max = fp_min;
			for( int i=BEGIN; i<END; ++i ) {
				SkChannel& channel = channels[i];
				if( channel.joint ) {
					if( channel.joint->skeleton() == _skeleton ) {
						// Line by line temp variables for the debugger
						int buffer_index = _frame_data.toBufferIndex( i );
						float* fp = &( buffer.get( buffer_index ) );  // pointer hack copied from Marcelo's SkChannelArray::set_values
						//fp_min = std::min( fp_min, fp );
						//fp_max = std::max( fp_max, fp );
						channel.set( fp );
					} else {
						LOG("ERROR: applyBufferToAllSkeletons(): channels[%d].joint.skeleton() != _skeleton", i);
					}
				} else {
					LOG("ERROR: applyBufferToAllSkeletons(): channels[%d].joint == NULL", i);
				}
			}
		}
	}

	/*
	Applys the skeleton data to the frame buffer
	*/
	void applySkeletonToBuffer()
	{
		SR_ASSERT( _state!=REMAPPING );
		if( _skeleton ) {
			if( _state==INVALID )
				remapFrameData();

			SkChannelArray& channels = _channels[_channels_cur];
			SrBuffer<float>& buffer = _frame_data.buffer();
			int size = channels.size();
			const int BEGIN =0;
			const int END = size;

			//// Debug pointer range
			//float* fp_min = &( buffer.get( _frame_data.toBufferIndex( 0 ) ) );
			//float* fp_max = fp_min;
			for( int i=BEGIN; i<END; ++i ) {
				SkChannel& channel = channels[i];
				if( channel.joint ) {
					if( channel.joint->skeleton() == _skeleton ) {
						// Line by line temp variables for the debugger
						int buffer_index = _frame_data.toBufferIndex( i );
						float* fp = &( buffer.get( buffer_index ) );  // pointer hack copied from Marcelo's SkChannelArray::set_values
						//fp_min = std::min( fp_min, fp );
						//fp_max = std::max( fp_max, fp );
						channel.get( fp );
					} else {
						LOG("ERROR: applySkeletonToBuffer(): channels[%d].joint.skeleton() != _skeleton", i);
					}
				} else {
					LOG("ERROR: applySkeletonToBuffer(): channels[%d].joint == NULL", i);
				}
			}
		}
	}
	void set_evaluation_logger( MeEvaluationLogger* logger ) {
		if( _logger ) {
			_logger->unref();
			_logger = NULL;
		}
		_logger = logger;
		if( _logger )
			_logger->ref();
	}

	MeEvaluationLogger* get_evaluation_logger() const {
		return _logger;
	}

	void set_logged_joints( set<string>& joint_names ) {
		_logged_joints = joint_names;
		set<string>::iterator set_end = _logged_joints.end();

		// Update channel indices
		_logged_channel_indices.clear();
		SkChannelArray& channels = _channels[_channels_cur];
		const int MAX = channels.size();
		for( int i=0; i<MAX; ++i ) {
			// MeControllerTreeRoot only takes skeletons,
			// so all channels should be connected.
			// Avoiding the check.
			const std::string& name = channels.name(i);
			if( _logged_joints.find(name)!=set_end )
				_logged_channel_indices.insert(i);
		}
		_state=INVALID;
	}

	const set<string>& get_logged_joints() const {
		return _logged_joints;
	}

	const set<int>& get_logged_channel_indices() const {
		return _logged_channel_indices;
	}

private:
	/**
	 *  Remap frame data to reflect new skeleton set.
	 */
	void remapFrameData() {
		_state = REMAPPING;

		// Swap channels and store convience references
		SkChannelArray& prev = _channels[_channels_cur];
		_channels_cur = _channels_cur? 0: 1;
		SkChannelArray& cur = _channels[_channels_cur];

		// TODO: Handle multiple skeletons
		if( _skeleton ) {
			cur.get_active_channels( _skeleton, true );  // Get connected channels
		} else {
			cur.init();
		}

		//  Do actual remap
		_frame_data.remapBuffers( cur, prev );
		prev.init();  // clear old channel references

		RemapAndActivate_Func remapAndActivate;
		for_each( _controllers.begin(), 
		          _controllers.end(),
		          remapAndActivate );

		_state = VALID;
	}
};




///////////////////////////////////////////////////////////////////////////
// TreeRootFrameData Method Definitons
MeControllerContext* TreeRootFrameData::context() const {
	return _context;
}

void TreeRootFrameData::incrementFrameCount( double time ) {
	_count_cur++;

	// Check for overflow
	if( _count_cur==0 ) {
		// zero out all update date
		_updated.assign( _context->channels().size(), 0 );
		// skip the 0 frame
		_count_cur = 1;
	}

	_last_time = time;
}


void TreeRootFrameData::remapBuffers( SkChannelArray& cur, SkChannelArray& prev ) {
	//cout << "TreeRootFrameData::remapBuffers(..):" << endl
	//     << "    cur.size(): " << cur.size() << "      cur.floats():  " << cur.floats() << endl
	//     << "   prev.size(): " << prev.size() << "     prev.floats(): " << prev.floats() << endl;

	const int cur_size = cur.size();

	vector<unsigned int> newUpdated( cur_size, 0 );
	SrBuffer<float>      newBuffer( cur.floats() );
	SrBuffer<int>        newMap( cur_size );

	int bufIndex=0;
	for( int i=0; i<cur_size; ++i ) {
		SkChannel::Type type = cur.type(i);
		int size = SkChannel::size( type );
		int oldChannel = prev.search( cur.name(i), type );
		if( oldChannel==-1 ) {
			// New Channel
			// new Updated was already zeroed out
			SkJoint* joint = cur.joint(i);
			newMap[i]     = bufIndex; // Index of first float
			if( joint ) {
				// Copy data from joint
				switch( type ) {
					case SkChannel::XPos:
						newBuffer[bufIndex] = joint->pos()->value(0);
						break;
					case SkChannel::YPos:
						newBuffer[bufIndex] = joint->pos()->value(1);
						break;
					case SkChannel::ZPos:
						newBuffer[bufIndex] = joint->pos()->value(2);
						break;
					case SkChannel::XRot:
						newBuffer[bufIndex] = joint->euler()->xRot();
						break;
					case SkChannel::YRot:
						newBuffer[bufIndex] = joint->euler()->yRot();
						break;
					case SkChannel::ZRot:
						newBuffer[bufIndex] = joint->euler()->zRot();
						break;
					case SkChannel::Quat: {
						const SrQuat& quat = joint->quat()->value();
						newBuffer[bufIndex+0] = quat.w;
						newBuffer[bufIndex+1] = quat.x;
						newBuffer[bufIndex+2] = quat.y;
						newBuffer[bufIndex+3] = quat.z;
						break;
					}
					case SkChannel::Swing: {
						SR_ASSERT( false ); // Unimplemented
						break;
					}
					case SkChannel::Twist: {
						SR_ASSERT( false ); // Unimplemented
						break;
					}
					default:
						SR_ASSERT( false );  // Invalid type
				}
			} else {
				for( int j=0; j<size; ++j ) {
					newBuffer[bufIndex+j] = 0;
				}
			}
		} else {
			int oldData   = _map[oldChannel];     // Find old data
			newUpdated[i] = _updated[oldChannel]; // Copy update frame
			newMap[i]     = bufIndex;             // Remap
			for( int j=0; j<size; ++j )
				newBuffer[ bufIndex+j ] = _buffer[ oldData+j ]; // Copy old data
		}
		bufIndex+=size;
	}
	SR_ASSERT( bufIndex==cur.floats() );

	swap( _updated, newUpdated );
	_buffer.swap( newBuffer );
	_map.swap( newMap );
}




///////////////////////////////////////////////////////////////////////////
//  MeControllerTreeRoot
MeControllerTreeRoot* MeControllerTreeRoot::create() {
	return new MeControllerTreeRootImpl();
}
