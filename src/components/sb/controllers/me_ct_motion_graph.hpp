/*
*  me_ct_motion_graph.h - part of Motion Engine and SmartBody-lib
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
*     Andrew Feng, USC
*/

#pragma once
#include <sb/SBTypes.h>
#include <map>
#include <sb/SBSkeleton.h>
#include <sb/SBCharacter.h>
#include <sb/SBController.h>
#include <sb/SBMotionGraph.h>
#include "me_ct_constraint.hpp"


class MeCtMotionGraph : public SmartBody::SBController, public FadingControl
{
private:
	static std::string CONTROLLER_TYPE;
public:	
	MeCtMotionGraph(SmartBody::SBCharacter* character);
	~MeCtMotionGraph(void);
	SmartBody::SBMotionNodeState* getMotionNodeState();
	virtual void controller_map_updated();
	virtual void controller_start();	
	virtual bool controller_evaluate( double t, MeFrameData& frame );	
	void updateWO(const SrMat& curWO, const SrMat& deltaWO, MeFrameData& frame);
protected:	
	SmartBody::SBCharacter* sbChar;
	SmartBody::SBMotionNodeState* motionState;
	SmartBody::SBMotionFrameBuffer outMotionBuffer;
	std::string baseJointName;	
};
