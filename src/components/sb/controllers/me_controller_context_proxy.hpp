/*
 *  me_controller_context_proxy.hpp - part of SmartBody-lib's Motion Engine
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

#ifndef ME_CONTROLLER_CONTEXT_PROXY_HPP
#define ME_CONTROLLER_CONTEXT_PROXY_HPP


#include "controllers/me_controller_context.hpp"


/**
 *  Convience class for controllers that wish to reimplement or intercept 
 *  calls between their child controllers and the current MeControllerContext.
 */
class MeControllerContextProxy
	: public MeControllerContext
{
public:
	//////////////////////////////////////////////////////////////////////////
	//  Public Constants
	static std::string CONTEXT_TYPE;

protected:
	//////////////////////////////////////////////////////////////////////////
	//  Private Data
	MeControllerContext* _context;

	//////////////////////////////////////////////////////////////////////////
	//  Private Constructors

	MeControllerContextProxy( MeControllerContext* context = NULL );

public:
	virtual ~MeControllerContextProxy();

	//!  Implements MeControllerContext::context_type()
	const std::string& context_type() const {	return CONTEXT_TYPE; }

	//!  Implements MeControllerContext::channels()
	virtual SkChannelArray& channels();

	/**
	 *  Update the inner context.
	 */
	virtual void context( MeControllerContext* context );

	/**
	 *  Returns the float buffer index coresponding 
	 *  to the given channel index.
     */
    virtual int toBufferIndex( int chanIndex );

};


/**
 *  Convience class for controllers that wish to reimplement or intercept 
 *  calls between their child controllers and the current MeFrameData.
 */
class MeFrameDataProxy
	: public MeFrameData
{
protected:
	/** Owned by this MeControllerContext instance...
	 *  Don't ref back (would cause a cyclical reference).
	 */
	MeControllerContextProxy* const _context;
	/**  Proxied frame data  */
	MeFrameData*                    _frame;

public:
	MeFrameDataProxy( MeControllerContextProxy* context );

	/**
	 *  Sets the proxied MeFrameData.
	 *  MUST be called before this MeFrameDataProxy is a valid MeFrameData.
	 */
	virtual void set_proxied_frame( MeFrameData* frame );

	//!  Implements MeFrameData
	virtual MeControllerContext* context() const;

	//!  Implements MeFrameData
	virtual int toBufferIndex( int chanIndex ) const;

	//!  Implements MeFrameData
	virtual SrBuffer<float>& buffer();

	//!  Implements MeFrameData
	virtual void channelUpdated( unsigned int n );

	//!  Implements MeFrameData
	virtual bool isChannelUpdated( unsigned int n ) const;


	///**
	// *  Implement runtime debugging hook called at the beginning of MeController::evaluate(..)
	// */
	//virtual void ct_pre_evaluate( MeController* ct, double time ) const;

	///**
	// *  Implement runtime debugging hook called at the end of MeController::evaluate(..)
	// */
	//virtual void ct_post_evaluate( MeController* ct, double time ) const;
};

#endif // ME_CONTROLLER_CONTEXT_PROXY_HPP
