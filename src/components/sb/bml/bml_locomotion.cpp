/*
 *  bml_locomotion.cpp - part of SmartBody-lib
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
 *      Andrew n marshall, USC
 *		Yuyu Xu, ICT USC
 */

#include "components/sb/vhcl/vhcl.h"
#include <iostream>
#include <sstream>
#include <string>

#include <xercesc/util/XMLStringTokenizer.hpp>
#include "components/sb/controllers/me_ct_channel_writer.hpp"
#include "components/sb/controllers/me_ct_param_animation.h"
#ifdef EMBER_SB_STEER
	#include "components/sb/sbm/PPRAISteeringAgent.h"
#endif

#include "bml_locomotion.hpp"
#include "bml_event.hpp"


#include "components/sb/sb/SBSimulationManager.h"
#include "components/sb/sb/SBScene.h"

#include "bml_xml_consts.hpp"
#include "components/sb/sbm/xercesc_utils.hpp"
#include "components/sb/sbm/BMLDefs.h"
#include <boost/lexical_cast.hpp>
#ifdef EMBER_SB_STEER
	#include "components/sb/sb/SBSteerManager.h"
#endif
#include "components/sb/sb/SBCommandManager.h"

////// XML ATTRIBUTES

using namespace std;
using namespace BML;
using namespace xml_utils;

BehaviorRequestPtr BML::parse_bml_locomotion( DOMElement* elem, const std::string& unique_id, BehaviorSyncPoints& behav_syncs, bool required, BmlRequestPtr request, SmartBody::SBScene* scene ) {
    const XMLCh* tag      = elem->getTagName();

	DOMElement* child = NULL;
	const XMLCh* attrType = NULL;
	const XMLCh* attrEnable = NULL;
	const XMLCh* attrID = NULL;
	const char * ascii_enable = NULL;
	string enable;
	int type = BML_LOCOMOTION_TARGET_TYPE_UNKNOWN;

	// Viseme transition hack until timing can support multiple sync points

	attrType = elem->getAttribute( BMLDefs::ATTR_TYPE );
	if( attrType && *attrType != 0 ) 
	{
		if( XMLString::compareIString( attrType, BMLDefs::ATTR_TARGET )==0 ) 
		{
			// Locomotion target is a static target
			if(type == BML_LOCOMOTION_TARGET_TYPE_UNKNOWN || type == BML_LOCOMOTION_TARGET_TYPE_TARGET)
			{
				type = BML_LOCOMOTION_TARGET_TYPE_TARGET;
			}
			else 
			{
				std::wstringstream wstrstr;
				wstrstr << "WARNING: BML::parse_bml_locomotion(): <"<<tag<<"> BML target type conflicts with previous setting ";
				LOG(convertWStringToString(wstrstr.str()).c_str());
			}

		} 
		else if( XMLString::compareIString( attrType, BMLDefs::ATTR_DIRECTION )==0 ) 
		{
			// Locomotion target is a direction.
			if(type == BML_LOCOMOTION_TARGET_TYPE_UNKNOWN || type == BML_LOCOMOTION_TARGET_TYPE_DIRECTION)
			{
				type = BML_LOCOMOTION_TARGET_TYPE_DIRECTION;
			}
			else
			{
				std::wstringstream wstrstr;
				wstrstr << "WARNING: BML::parse_bml_locomotion(): <"<<tag<<"> BML target type conflicts with previous setting ";
				LOG(convertWStringToString(wstrstr.str()).c_str());
			}
		} 
	}

	int id = xml_parse_int( BMLDefs::ATTR_ID, elem, -1 );

	child = getFirstChildElement( elem );

//	Locomotion::parse_routine(child, request, type, id);
	//-------------starting  from here, it's BML Spec 1.0
	std::string localId = "locomotion";

#ifdef EMBER_SB_STEER
	if (!SmartBody::SBScene::getScene()->getSteerManager()->getEngineDriver()->isInitialized())
	{
		LOG("Steering Engine not started. Call \"steer start\" first");
		return BehaviorRequestPtr( new EventRequest(unique_id, localId, "", "", behav_syncs, ""));
	}
	if (!SmartBody::SBScene::getScene()->getSteerManager()->getEngineDriver()->_engine)
	{
		LOG("Steering Engine not started. Call \"steer start\" first");
		return BehaviorRequestPtr( new EventRequest(unique_id, localId, "", "", behav_syncs, ""));
	}
	std::stringstream command;
	SmartBody::SBSteerManager* manager = SmartBody::SBScene::getScene()->getSteerManager();
	SmartBody::SBSteerAgent* steerAgent = manager->getSteerAgent(request->actor->getName());
	SmartBody::SBCharacter* c = SmartBody::SBScene::getScene()->getCharacter(request->actor->getName());
	if (!steerAgent)
	{
		LOG("Steering Agent not attached. Check initialization");
		return BehaviorRequestPtr( new EventRequest(unique_id, localId, "", "", behav_syncs, ""));	
	}

	PPRAISteeringAgent* ppraiAgent = dynamic_cast<PPRAISteeringAgent*>(steerAgent);

	// Enable/disable locomotion controller	
	attrEnable = elem->getAttribute( BMLDefs::ATTR_ENABLE );
	if( attrEnable && *attrEnable != 0 ) 
	{
		if( XMLString::compareIString( attrEnable, BMLDefs::ATTR_TRUE )==0 ) 
		{
			
		}
		else if( XMLString::compareIString( attrEnable, BMLDefs::ATTR_FALSE )==0 )
		{
			ppraiAgent->locomotionHalt();
		}
		else if (XMLString::compareIString(attrEnable, BMLDefs::ATTR_RESET) ==0 )
		{
			ppraiAgent->locomotionReset();
		}
	}

	ppraiAgent->steppingMode = false;
	bool stepMode = false;
	bool stepTargetMode = false;
	std::string stepDirection = "";
	float stepTargetX = 0.0f;
	float stepTargetZ = 0.0f;

	std::string locotype = xml_parse_string(BMLDefs::ATTR_TYPE, elem);
	std::string steerTypeCommand = "steer type " + locotype;
	SmartBody::SBScene::getScene()->getCommandManager()->execute((char*) steerTypeCommand.c_str());

	float proximity = xml_parse_float(BMLDefs::ATTR_PROXIMITY, elem, ppraiAgent->distThreshold);
	ppraiAgent->distThreshold = proximity;
	ppraiAgent->acceleration = xml_parse_float(BMLDefs::ATTR_STEERACCEL, elem, ppraiAgent->acceleration);
	ppraiAgent->scootAcceleration = xml_parse_float(BMLDefs::ATTR_STEERSCOOTACCEL, elem, ppraiAgent->scootAcceleration);
	ppraiAgent->angleAcceleration = xml_parse_float(BMLDefs::ATTR_STEERANGLEACCEL, elem, ppraiAgent->angleAcceleration);


	float speed = xml_parse_float( BMLDefs::ATTR_SPEED, elem, -1.0f );
	
	std::string manner = xml_parse_string(BMLDefs::ATTR_MANNER, elem);

	if (!c->hasAttribute("steering.pathMaxSpeed"))
		c->createDoubleAttribute("steering.pathMaxSpeed", 1.5f, true, "steering", 310, false, false, false, "");	

	if (speed > 0.0f)
	{
		ppraiAgent->desiredSpeed = speed;
		//c->setDoubleAttribute("steering.pathMaxSpeed",(double)speed);
		ppraiAgent->currentTargetSpeed = speed;
	}
	else
	{
		//ppraiAgent->currentTargetSpeed = (float)c->getDoubleAttribute("steering.pathMaxSpeed");
		ppraiAgent->desiredSpeed = (float)c->getDoubleAttribute("steering.pathMaxSpeed");
		ppraiAgent->currentTargetSpeed = (float)c->getDoubleAttribute("steering.pathMaxSpeed");
	}

	if (manner != "")
	{
		if (c->locomotion_type == c->Procedural)
		{
			LOG("This mode does not support Procedural Locomotion currently!");
			return BehaviorRequestPtr();
		}		
		

		if (manner == "walk" && speed <= 0.0f)
		{
			ppraiAgent->desiredSpeed = 1.2f;			
			//c->setDoubleAttribute("steering.pathMaxSpeed",1.2);
			ppraiAgent->currentTargetSpeed = 1.2f;
		}
		else if (manner == "jog" && speed <= 0.0f)
		{
			ppraiAgent->desiredSpeed = 2.5f;
			//c->setDoubleAttribute("steering.pathMaxSpeed",2.5);
			ppraiAgent->currentTargetSpeed = 2.5f;
		}
		else if (manner == "run" && speed <= 0.0f)
		{
			ppraiAgent->desiredSpeed = 3.5f;
			//c->setDoubleAttribute("steering.pathMaxSpeed",3.5);
			ppraiAgent->currentTargetSpeed = 3.5f;
		}
		else if (manner == "sbm:step")
			stepMode = true;
		else if (manner == "sbm:jump")
		{
			if (c->param_animation_ct)
			{
				if (c->param_animation_ct->hasPABlend(ppraiAgent->jumpName))
					return BehaviorRequestPtr( new EventRequest(unique_id, localId, command.str().c_str(), "", behav_syncs, ""));
				std::stringstream command1;
				if (c->param_animation_ct->getCurrentPABlendData()->state->stateName == ppraiAgent->locomotionName)
				{
					command1 << "bml char " << c->getName() << " <sbm:states loop=\"false\" name=\"" << ppraiAgent->jumpName << "\" sbm:startnow=\"true\"/>";
					command1 << "<sbm:states loop=\"true\" name=\"" << ppraiAgent->locomotionName << "\" sbm:startnow=\"false\"/>";
				}
				else
				{
					command1 << "bml char " << c->getName() << " <sbm:states loop=\"false\" name=\"" << ppraiAgent->jumpName << "\" sbm:startnow=\"true\"/>";
				}
				SmartBody::SBScene::getScene()->getCommandManager()->execute((char*)command1.str().c_str());
				return BehaviorRequestPtr( new EventRequest(unique_id, localId, command.str().c_str(), "", behav_syncs, ""));
			}
		}
		else 
			return BehaviorRequestPtr();

		// also has to update state weight
	//	PABlend* locoData = SmartBody::SBScene::getScene()->getBlendManager()->getBlend(c->steeringAgent->locomotionName);
	//	if (locoData)
	//		locoData->setWeight(c->steeringAgent->desiredSpeed * 100.0f, 0.0);
	}

	// gain for braking
	const char* brakingFactor = xml_utils::asciiString(elem->getAttribute(BMLDefs::ATTR_BRAKING));
	if (strcmp(brakingFactor, "") != 0)
	{
		std::stringstream command;
		command << "steer braking " << c->getName() << " " << (float)atof(brakingFactor);
		SmartBody::SBScene::getScene()->getCommandManager()->execute((char*)command.str().c_str());	
	}

	// for facing angle, we need to execute with some delay
	const char* facingAngle = xml_utils::asciiString(elem->getAttribute(BMLDefs::ATTR_FACING));
	float facingAngleVal = (float) atof(facingAngle);
	if (strcmp(facingAngle, "") != 0)
	{
		std::string facingAngleStr = facingAngle;
		float facingOffset = 0.f;
		// are there any offsets in the facing angle?
		int posPos = facingAngleStr.find_last_of("+");
		if (posPos != std::string::npos)
		{
			std::string facingOffsetStr = facingAngleStr.substr(posPos + 1);
			facingOffset = (float) atof(facingOffsetStr.c_str());
			facingAngleStr = facingAngleStr.substr(0, posPos);
		}
		else
		{
			int negPos = facingAngleStr.find_last_of("-");
			if (negPos != std::string::npos)
			{
				std::string facingOffsetStr = facingAngleStr.substr(negPos + 1);
				facingOffset = (float) -atof(facingOffsetStr.c_str());
				facingAngleStr = facingAngleStr.substr(0, negPos);
			}
		}

		// does the facing angle match a pawn?
		SbmPawn* facingPawn =  SmartBody::SBScene::getScene()->getPawn(facingAngleStr);
		if (facingPawn)
		{
			float x, y, z, h, p, r;
			facingPawn->get_world_offset(x, y, z, h, p, r);
			facingAngleVal = h + facingOffset;
		}	

		std::stringstream command;

		
		command << "steer facing " << c->getName() << " " << facingAngleVal;
		srCmdSeq *seq = new srCmdSeq();
		seq->insert(float(SmartBody::SBScene::getScene()->getSimulationManager()->getTime() + SmartBody::SBScene::getScene()->getSimulationManager()->getTimeDt()), command.str().c_str());
		SmartBody::SBScene::getScene()->getCommandManager()->execute_seq(seq);
	}
	else
	{
		std::stringstream command;
		command << "steer facing " << c->getName() << " " << "-200"; // set to fabs() > 180 to cancel old facing value
		srCmdSeq *seq = new srCmdSeq();
		seq->insert(float(SmartBody::SBScene::getScene()->getSimulationManager()->getTime() + SmartBody::SBScene::getScene()->getSimulationManager()->getTimeDt()), command.str().c_str());
		SmartBody::SBScene::getScene()->getCommandManager()->execute_seq(seq);
	}
	std::string following = xml_parse_string(BMLDefs::ATTR_FOLLOW, elem);
	SmartBody::SBPawn* followingC = SmartBody::SBScene::getScene()->getPawn(following);
	ppraiAgent->setTargetAgent(followingC);

	// parsing target
	std::string targetString = xml_parse_string(BMLDefs::ATTR_TARGET, elem);
	//LOG("target string = %s",targetString.c_str());
	std::vector<std::string> tokens;
	vhcl::Tokenize(targetString, tokens, " ");
	int tokenSize = tokens.size();
	if (tokenSize == 0)			// wrong
	{
		LOG("Warning: target's format is (X Z) | (X1 Z1 X2 Z2 X3 Z3...) | backward | forward | left | right.");
	}
	else if (tokenSize == 1)	// step mode or pawn name
	{
		std::string token = tokens[0];
		if ((token == "forward" || token == "backward" || token == "left" || token == "right") && stepMode)
		{
			stepTargetMode = false;
			stepDirection = targetString;
		}
		else
		{
			SbmPawn* pawn =  SmartBody::SBScene::getScene()->getPawn(token);
			if (pawn)
			{
				// get the world offset x & z
				float x, y, z, yaw, pitch, roll;
				pawn->get_world_offset(x, y, z, yaw, pitch, roll);
				if (stepMode)
				{
					stepTargetMode = true;
					stepTargetX = x;
					stepTargetZ = z;
				}
				else
				{
					command << "steer move " << c->getName() << " normal " << x << " 0 " << z;
					//SmartBody::SBScene::getScene()->getCommandManager()->execute((char*)command.str().c_str());
				}
			}
		}
	}
	else
	{
		command << "steer move " << c->getName() << " normal ";
#define PAWN_WAY_POINTS 1
#ifndef PAWN_WAY_POINTS
		if (tokens.size() % 2 != 0)
			LOG("Warning: target points are not paired");

		for (size_t i = 0; i < (tokens.size() / 2); i++)
			command << tokens[i * 2 + 0] << " 0 " << tokens[i * 2 + 1] << " ";
#else
		unsigned int icount = 0;
		while (icount < tokens.size())
		{			
			std::string tok1 = tokens[icount];
			SbmPawn* pawn =  SmartBody::SBScene::getScene()->getPawn(tok1);
			if (pawn) // use pawn position as way point
			{				
				float x,y,z,h,p,r;
				pawn->get_world_offset(x,y,z,h,p,r);
				std::string xstr, zstr;
				xstr = boost::lexical_cast<std::string>(x);
				zstr = boost::lexical_cast<std::string>(z);
				command <<  xstr << " 0 " << zstr << " ";
				icount += 1;
			}
			else if (icount + 1 < tokens.size() ) // or use two coordinates
			{
				std::string tok2 = tokens[icount+1];
				command << tok1 << " 0 " << tok2 << " ";				
				icount+= 2;								
			}
			else
			{
				icount++;
			}
		}
#endif
		//SmartBody::SBScene::getScene()->getCommandManager()->execute((char*)command.str().c_str());
	}

	// step mode attributes
	int numSteps = xml_parse_int(BMLDefs::ATTR_NUM_STEPS, elem);
	if (numSteps <= 0) numSteps = 1;

	// execute steps
	if (stepMode)
	{		
		if (!c->param_animation_ct)
		{
			LOG("Parameterized Animation Engine not setup, cannot use step control.");
			return BehaviorRequestPtr();
		}
		if (c->param_animation_ct->hasPABlend(ppraiAgent->stepStateName) || !c->param_animation_ct->isIdle())
			return BehaviorRequestPtr();
		ppraiAgent->stepAdjust = false;
		if (stepTargetMode)		// given target
		{
			ppraiAgent->steppingMode = true;
			ppraiAgent->stepTargetX = stepTargetX;
			ppraiAgent->stepTargetZ = stepTargetZ;
		}
		else					// given direction and number of steps
		{
			for (int i = 0; i < numSteps; i++)
			{
				std::stringstream command1;
				command1 << "panim schedule char " << c->getName();
				command1 << " state " << ppraiAgent->stepStateName << " loop false playnow false additive false joint null ";
				if (stepDirection == "forward")
					command1 << "0 0 1 0 0 0 0";
				if (stepDirection == "backward")
					command1 << "0 1 0 0 0 0 0";
				if (stepDirection == "left")
					command1 << "0 0 0 0 0 0 1";
				if (stepDirection == "right")
					command1 << "0 0 0 1 0 0 0";
				SmartBody::SBScene::getScene()->getCommandManager()->execute((char*)command1.str().c_str());
			}
		}
	}
	//LOG("behavior sync stroke =  %f",behav_syncs.sync_stroke()->time());
	std::stringstream strstr;
	strstr << "scene.command(\"" << command.str() << "\")";
	return BehaviorRequestPtr( new EventRequest(unique_id, localId, "", strstr.str(), behav_syncs, ""));
#else

	return BehaviorRequestPtr( new EventRequest(unique_id, localId, "", "", behav_syncs, ""));

#endif
}

void BML::Locomotion::parse_routine(DOMElement* elem, BmlRequestPtr request, int type, int id)
{
	float speed = 0.0f;
	float g_angular_speed = 0.0f;
	float l_angular_speed = 0.0f;
	float pos[3];
	memset(pos, 0, sizeof(float)*3);

	const XMLCh* tag = NULL;
	DOMElement* child = elem;

	while(child)
	{
		tag = child->getTagName();
		if(XMLString::compareIString( tag, BMLDefs::ATTR_TARGET )==0)
		{
			if(type != BML_LOCOMOTION_TARGET_TYPE_TARGET)
			{
				LOG("WARNING: BML::parse_routine(): locomotion routine type unmatched");
				return;  // a.k.a., NULL
			}

			pos[ 0 ] = xml_parse_float( BMLDefs::ATTR_X, child );
			pos[ 1 ] = xml_parse_float( BMLDefs::ATTR_Y, child );
			pos[ 2 ] = xml_parse_float( BMLDefs::ATTR_Z, child );
			speed = xml_parse_float( BMLDefs::ATTR_SPEED, child );
		}
		else if(XMLString::compareIString( tag, BMLDefs::ATTR_DIRECTION) == 0)
		{
			if(type != BML_LOCOMOTION_TARGET_TYPE_DIRECTION)
			{
				LOG("WARNING: BML::parse_routine(): locomotion routine type unmatched");
				return;  // a.k.a., NULL
			}

			pos[ 0 ] = xml_parse_float( BMLDefs::ATTR_X, child );
			pos[ 1 ] = xml_parse_float( BMLDefs::ATTR_Y, child );
			pos[ 2 ] = xml_parse_float( BMLDefs::ATTR_Z, child );
			speed = xml_parse_float( BMLDefs::ATTR_SPEED, child );

			SrVec dir(pos[0], pos[1], pos[2]);
			dir.normalize();
			dir *= speed;
			pos[0] = dir.x;
			pos[1] = dir.y;
			pos[2] = dir.z;
		}
		else if(XMLString::compareIString( tag, BMLDefs::ATTR_VELOCITY) == 0)
		{
			if(type != BML_LOCOMOTION_TARGET_TYPE_DIRECTION)
			{
				LOG("WARNING: BML::parse_routine(): locomotion routine type unmatched.");
				return;  // a.k.a., NULL
			}

			pos[ 0 ] = xml_parse_float( BMLDefs::ATTR_X, child );
			pos[ 1 ] = xml_parse_float( BMLDefs::ATTR_Y, child );
			pos[ 2 ] = xml_parse_float( BMLDefs::ATTR_Z, child );
		}

		else if(XMLString::compareIString( tag, BMLDefs::ATTR_ROTATION) == 0)
		{
			const XMLCh* attrType = child->getAttribute( BMLDefs::ATTR_TYPE );
			
			float rotation_speed = xml_parse_float( BMLDefs::ATTR_SPEED, child );
			
			if(XMLString::compareIString( attrType, BMLDefs::ATTR_LRPS) == 0)
			{
				l_angular_speed = rotation_speed;
			}

			else if(XMLString::compareIString( attrType, BMLDefs::ATTR_GRPS) == 0)
			{
				g_angular_speed = rotation_speed;
			}

			else if(XMLString::compareIString( attrType, BMLDefs::ATTR_RPS) == 0)
			{
				l_angular_speed = rotation_speed;
				g_angular_speed = rotation_speed;
			}
			
		}

		child = getNextElement( child );
	}
}

