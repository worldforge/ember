/*
 *  me_evaluation_logger.hpp - part of SmartBody-lib's Motion Engine
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

#ifndef ME_EVALUATION_LOGGER
#define ME_EVALUATION_LOGGER

#include <sr/sr_shared_class.h>
#include <sk/sk_channel_array.h>

// Predeclare class because of circular reference:
class MeEvaluationLogger;

#include <controllers/me_controller.h>
#include <controllers/me_controller_context.hpp>



/**
 *  Interface for logging SkChannelArray data during controller and context evaluation.
 */
class MeEvaluationLogger
	: public SrSharedClass
{
public:
	/**
	 *  Logs channel and/or buffer data before a context's evaluation.
	 */
	virtual void context_pre_evaluate(
		double time,
		MeControllerContext& context,
		MeFrameData& frame
	) = 0;

	/**
	 *  Logs channel and/or buffer data after a context's evaluation.
	 */
	virtual void context_post_evaluate(
		double time,
		MeControllerContext& context,
		MeFrameData& frame
	) = 0;

	/**
	 *  Logs channel and/or buffer data before a controller's evaluation.
	 */
	virtual void controller_pre_evaluate(
		double time,
		MeControllerContext& context,
		MeController& ct,
		MeFrameData& frame
	) = 0;

	/**
	 *  Logs channel and/or buffer data after a controller's evaluation.
	 */
	virtual void controller_post_evaluate(
		double time,
		MeControllerContext& context,
		MeController& ct,
		MeFrameData& frame
	) = 0;
};




#endif  // ME_EVALUATION_LOGGER
