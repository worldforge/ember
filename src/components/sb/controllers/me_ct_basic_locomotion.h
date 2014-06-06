/*
 *  me_ct_basic_locomotion.h - part of Motion Engine and SmartBody-lib
 *  Copyright (C) 2011  University of Southern California
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
 *      Yuyu Xu, ICT USC
 */

#ifndef _ME_CT_BASIC_LOCOMOTION_H_
#define _ME_CT_BASIC_LOCOMOTION_H_

#include <controllers/me_controller.h>
#include <sb/sbm_character.hpp>
#include <sb/SBController.h>

class MeCtBasicLocomotion : public SmartBody::SBController
{
	public:
		MeCtBasicLocomotion(SbmCharacter* c);
		~MeCtBasicLocomotion();
		void init(SbmPawn* pawn);
		
		virtual bool controller_evaluate(double t, MeFrameData& frame);		
		virtual SkChannelArray& controller_channels()	{return(_channels);}
		virtual double controller_duration()			{return -1;}
		virtual const std::string& controller_type() const		{return(_type_name);}
		
	public:
		static std::string _type_name;
		void setScootSpd(float v) {scootSpd = v;}
		float getScootSpd() {return scootSpd;}
		void setMovingSpd(float v) {movingSpd = v;}
		float getMovingSpd() {return movingSpd;}
		void setTurningSpd(float v) {turningSpd = v;}
		float getTurningSpd() {return turningSpd;}
		void setValid(bool v) {_valid = v;}
		void setDesiredHeading(float v) {desiredHeading = v;}
		float getDesiredHeading() {return desiredHeading;}

	private:
		bool _valid;
		float scootSpd;
		float movingSpd;
		float turningSpd;
		SkChannelArray _channels;
		SbmCharacter* character;
		double _lastTime;
		float desiredHeading;
};

#endif
