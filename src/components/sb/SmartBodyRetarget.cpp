/*
 Copyright (C) 2014 Céline NOËL <celine.noel.7294@gmail.com>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "SmartBodyRetarget.h"
#include "SmartBodyManager.h"

#include <sb/SBRetargetManager.h>
#include <sb/SBRetarget.h>

#include <vector>


using namespace Ember;
namespace Ember
{

SmartBodyRetarget::SmartBodyRetarget(const std::string& srcSkName, const std::string& tgtSkName, SmartBody::SBRetargetManager& manager)
{
	SmartBody::SBRetarget *retarget = manager.getRetarget(srcSkName, tgtSkName);

	//If retarget is NULL, then the instance has never been set and we must create it.
	if (!retarget)
	{
		createInstance(srcSkName, tgtSkName, manager);
	}
}

SmartBodyRetarget::~SmartBodyRetarget()
{

}


void SmartBodyRetarget::createInstance(const std::string& srcSk, const std::string& tgtSk, SmartBody::SBRetargetManager& manager)
{
	//Sets the end joints.
	std::vector<std::string> endJoints;
	endJoints.push_back("l_ankle");
	endJoints.push_back("l_forefoot");
	endJoints.push_back("l_toe");
	endJoints.push_back("l_wrist");
	endJoints.push_back("r_ankle");
	endJoints.push_back("r_forefoot");
	endJoints.push_back("r_toe");
	endJoints.push_back("r_wrist");

	//Sets the relative joints.
	std::vector<std::string> relJoints;
	relJoints.push_back("spine1");
	relJoints.push_back("spine2");
	relJoints.push_back("spine3");
	relJoints.push_back("spine4");
	relJoints.push_back("spine5");
	relJoints.push_back("r_sternoclavicular");
	relJoints.push_back("l_sternoclavicular");
	relJoints.push_back("r_acromioclavicular");
	relJoints.push_back("l_acromioclavicular");
	
	//Creation of the retarget instance.
	SmartBody::SBRetarget *retarget = manager.createRetarget(srcSk, tgtSk);
	retarget->initRetarget(endJoints, relJoints);
}

}


