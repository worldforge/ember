/*
 *  me_ct_blend.hpp - part of SmartBody-lib's Motion Engine
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

#ifndef ME_CT_BLEND_HPP
#define ME_CT_BLEND_HPP


#include <vector>

#include <controllers/me_ct_unary.hpp>
#include <controllers/me_controller_context_proxy.hpp>

#include <sbm/sr_linear_curve.h>

class MeCtBlend : public MeCtUnary  {
public:
	//////////////////////////////////////////////////////////////////////////
	//  Public Constants
	static std::string CONTROLLER_TYPE;


	//////////////////////////////////////////////////////////////////////////
	//  Local Context
	class Context; // predeclatation

	/**
	 *  FrameData for child evaluation (before interpolation).
	 */
	class FrameData : public MeFrameDataProxy {
		friend class MeCtBlend;
		friend class MeCtBlend::Context;

		///////////////////////////////////////////////////////////////
		//  Private Data
	protected:
		// Owned by these instances.. never NULL
		MeCtBlend&      _blend;
		SrBuffer<float> _buffer;

		///////////////////////////////////////////////////////////////
		//  Constructor
		FrameData( MeCtBlend* blend );
		~FrameData();

	public:
		///////////////////////////////////////////////////////////////
		//  Public Methods implement MeControllerContext
		MeControllerContext* context() const;

		SrBuffer<float>& buffer();

		int toBufferIndex( int chanIndex ) const;

		void channelUpdated( unsigned int n );

		bool isChannelUpdated( unsigned int n ) const;
	};
	friend class MeCtBlend::FrameData;

	/**
	 *  MeControllerContext implementation for child controller.
	 *
	 *  The Context is the heart of the blend controller, performing the actual
	 *  evaluation of the child and interpolation into the parent context.
	 */
	class Context : public MeCtUnary::Context {
		friend class MeCtBlend;
		friend class MeCtBlend::FrameData;
	public:
		///////////////////////////////////////////////////////////////
		//  Public Constants
		static std::string CONTEXT_TYPE;

	protected:
		///////////////////////////////////////////////////////////////
		//  Private Data
		//MeCtBlend& _blend;  /// use static_cast<MeCtBlend&>(_container)
		SkChannelArray _local_channels;
		SrBuffer<int>  _ch_to_buffer_index;
		std::set<int>  _channels_logged;

		FrameData      _local_frame;

		///////////////////////////////////////////////////////////////
		//  Constructor
		Context( MeCtBlend* blend );

		/**
		 *  Loads the intersection of
		 *     _context->channels()
		 *  and
		 *     _container._child->controller_channels()
		 *  into this->_local_channels.
		 *
		 *  Called by MeCtBlend::controller_map_updated()
		 */
		void remap_channels();

	public:
		///////////////////////////////////////////////////////////////
		//  Public Methods
		const std::string& context_type() const
		{	return CONTEXT_TYPE; }

		SkChannelArray& channels();

		int toBufferIndex( int chanIndex );

		void child_channels_updated( MeController* child );

	};
	friend class MeCtBlend::Context;

protected:
	//////////////////////////////////////////////////////////////////////////
	//  MeCtBlend Private Data
	srLinearCurve	_curve;

	MeCtBlend::Context* const _sub_blend_context;

	//  Channel index mappings:
	std::vector<int> _local_ch_to_parent; // Map local channel index to local buffer value index
	std::vector<int> _local_ch_to_buffer; // Map local channel index to parent context index
	

public:
	/** Constructor */
	MeCtBlend( MeController* child = NULL );

	/** Destructor */
	virtual ~MeCtBlend();

	/** Initialize the Blend controller with the given child controller.  */
	void init( MeController* child, SbmPawn* pawn );

	//virtual void set_child( MeController* child );  // Use init( child )

	srLinearCurve& get_curve( void ) { return _curve; }

protected:
	void controller_map_updated();

	bool controller_evaluate( double t, MeFrameData & frame );

	inline int local_ch_to_buffer( unsigned int i )
	{	return i<_local_ch_to_buffer.size()? _local_ch_to_buffer[i] : -1; }

	inline int local_ch_to_parent( unsigned int i )
	{	return i<_local_ch_to_parent.size()? _local_ch_to_parent[i] : -1; }
public:
    const std::string& controller_type() const;
    //SkChannelArray& controller_channels();  // MeCtUnary provides this
	void print_state( int tabCount );

	////// Anm - Rolling back change to prevent crash until further testing
	///**
	// *  Overrides MeController::controller_channels().
	// *
	// *  Returns the intersection of channels available from parent and requested by child.
	// */
	//virtual SkChannelArray& controller_channels();
};
#endif // ME_CT_BLEND_HPP
