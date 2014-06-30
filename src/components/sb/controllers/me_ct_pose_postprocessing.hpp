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
#include <sb/SBController.h>
#include "me_ct_constraint.hpp"
#include "me_ct_limb.hpp"
#include "me_ct_jacobian_IK.hpp"
#include "me_ct_ccd_IK.hpp"
#include "sbm/gwiz_math.h"


class MeCtPosePostProcessing : public MeCtConstraint
{
private:
	static std::string CONTROLLER_TYPE;
public:	
	MeCtPosePostProcessing(SmartBody::SBSkeleton* skeleton);
	~MeCtPosePostProcessing(void);
	virtual bool controller_evaluate( double t, MeFrameData& frame );	
protected:
	void updatePoseConstraint();
	MeCtCCDIK ikCCD;
	bool firstIKSolve;
};
