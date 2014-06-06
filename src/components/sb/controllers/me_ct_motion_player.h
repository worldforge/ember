/*
 *  me_ct_motion_player.h - part of Motion Engine and SmartBody-lib
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
 *      Yuyu Xu, USC
 */

#ifndef _ME_CT_MOTION_PLAYER_H
#define _ME_CT_MOTION_PLAYER_H

#include <controllers/me_ct_container.hpp>
#include <controllers/me_ct_motion.h>
#include <sb/sbm_character.hpp>

#include <string>

class MeCtMotionPlayer : public MeCtContainer
{
public:
	static std::string CONTROLLER_TYPE;

	class Context : public MeCtContainer::Context 
	{
	protected:
		static std::string CONTEXT_TYPE;
	public:
		Context( MeCtMotionPlayer* container, MeControllerContext* context = NULL )
			:	MeCtContainer::Context( container, context )
		{}

		const std::string& context_type() const {	return CONTEXT_TYPE; }
		void child_channels_updated( MeController* child );
	};

public:
	MeCtMotionPlayer(SbmCharacter* c);
	~MeCtMotionPlayer();

	void init(SbmPawn* pawn, std::string motionName, double n);

	void setFrameNum(double n);
	double getFrameNum();

	void setMotionName(const std::string& name);
	const std::string& getMotionName();

	void setActive(bool a);
	bool getActive();

	virtual void controller_map_updated();
    virtual SkChannelArray& controller_channels();
    virtual double controller_duration();
	virtual const std::string& controller_type() const {return CONTROLLER_TYPE;}
	virtual bool controller_evaluate( double t, MeFrameData& frame );

private:
	SbmCharacter*					character;
	MeController*					controller;		
	SkChannelArray					channels;

	double							frameNum;
	std::string						motionName;
	bool							isActive;
};

#endif
