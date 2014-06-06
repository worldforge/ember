/*
*  me_ct_constraint.h - part of Motion Engine and SmartBody-lib
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
*      Wei-Wen Feng, USC
*/

#pragma once
#include <sb/SBTypes.h>
#include <map>
#include <sb/SBSkeleton.h>
#include <sb/SBJoint.h>
#include <sb/SBController.h>
#include <sb/SBCharacter.h>
#include "me_ct_limb.hpp"
#include "me_ct_jacobian_IK.hpp"
#include "sbm/gwiz_math.h"

class EffectorJointConstraint : public EffectorConstraint
{
public:	
	SmartBody::SBJoint*        targetJoint;	
	SrQuat					   rotOffset;
	SrVec					   posOffset;	
public:
	EffectorJointConstraint();
	~EffectorJointConstraint() {}

	EffectorJointConstraint& operator=(const EffectorJointConstraint& rhs);

	virtual SrVec getPosConstraint();
	virtual SrQuat getRotConstraint();
};

typedef std::vector<EffectorJointConstraint> ConstraintList;

class FadingControl
{
public:
	enum FadingControlMode	{
		FADING_MODE_OFF = 0,
		FADING_MODE_IN,
		FADING_MODE_OUT
	};
protected:
	float             fadeRemainTime, fadeInterval;
	float             blendWeight;
	float             prev_time, cur_time, dt;
	bool              restart;	
	FadingControlMode fadeMode;
public:
	FadingControl();
	virtual ~FadingControl() {}	
	SBAPI void setFadeIn(float interval);
	SBAPI void setFadeOut(float interval);
	bool updateFading(float dt);

	void controlRestart();
	void updateDt(float curTime);
};


class MeCtConstraint : public SmartBody::SBController, public FadingControl
{
private:
	static std::string CONTROLLER_TYPE;
public:	
	SBAPI static bool useIKConstraint;

	
	enum ConstraintType
	{
		CONSTRAINT_POS = 0,
		CONSTRAINT_ROT,
		NUM_OF_CONSTRAINT
	};

public:	
	MeCtConstraint(SmartBody::SBSkeleton* skeleton);
	~MeCtConstraint(void);

protected:	
	MeCtJacobianIK       ik;
	MeCtIKTreeScenario   ik_scenario;
	SmartBody::SBSkeleton*     _skeleton;
	SmartBody::SBCharacter*    _sbChar;
	float 			_duration;
	SkChannelArray	_channels;	

	ConstraintMap   rotConstraint;
	ConstraintMap   posConstraint;

public:	
	double          ikDamp;
public:			
	void init (SmartBody::SBCharacter* pawn, const char* rootName);
	bool addEffectorJointPair(SmartBody::SBJoint* targetJoint, const char* effectorName, const char* effectorRootName, const SrVec& posOffset , const SrQuat& rotOffset , ConstraintType cType = CONSTRAINT_POS);
	virtual void controller_map_updated();
	virtual void controller_start();	
	virtual bool controller_evaluate( double t, MeFrameData& frame );
	virtual SkChannelArray& controller_channels()	{ return( _channels ); }
	virtual double controller_duration()			{ return( (double)_duration ); }
	void set_duration(float duration) { _duration = duration; }
	virtual const std::string& controller_type() const		{ return( CONTROLLER_TYPE ); }
	virtual void print_state( int tabs );	

protected:	
	void  updateChannelBuffer(MeFrameData& frame, std::vector<SrQuat>& quatList, bool bRead = false);
};
