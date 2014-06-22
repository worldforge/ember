#include <vhcl/vhcl.h>

#include <iostream>
#include <sstream>
#include <string>

#include "bml_gesture.hpp"

#include "sb/SBScene.h"
#include "bml_xml_consts.hpp"
#include <sb/SBGestureMap.h>
#include <sb/SBAssetManager.h>
#include <sb/SBGestureMapManager.h>
#ifdef EMBER_SB_STEER
	#include <sb/SBSteerManager.h>
	#include <sb/SBSteerAgent.h>
#endif
#include <sb/SBBehavior.h>
#include <sb/SBSkeleton.h>
#include <controllers/me_ct_motion.h>
#include <controllers/me_ct_scheduler2.h>
#ifdef EMBER_SB_STEER
	#include <sbm/PPRAISteeringAgent.h>
#endif
#include <sb/SBMotion.h>

using namespace std;
using namespace BML;
using namespace xml_utils;

/*	Usage of bml gestures
 *	- if stroke and stroke_end is defined, poststroke_hold time would be calculated automatically even if there's poststroke_hold given by user
 *	- you can specify idle animation used for gesture holding period
 *	- you can specify joints, scale, frequency for perlin noise added to gesture holding period. premise is no idle animation is specified.
 */

BML::BehaviorRequestPtr BML::parse_bml_gesture( DOMElement* elem, const std::string& unique_id, BehaviorSyncPoints& behav_syncs, bool required, BmlRequestPtr request, SmartBody::SBScene* scene ) 
{
	const XMLCh* animName = elem->getAttribute( BMLDefs::ATTR_NAME );
	const XMLCh* id = elem->getAttribute(BMLDefs::ATTR_ID);
	const XMLCh* lexemeAttr = elem->getAttribute(BMLDefs::ATTR_LEXEME);
	const XMLCh* typeAttr = elem->getAttribute(BMLDefs::ATTR_TYPE);
	const XMLCh* modeAttr = elem->getAttribute(BMLDefs::ATTR_MODE);
	const XMLCh* styleAttr = elem->getAttribute(BMLDefs::ATTR_STYLE);
	const XMLCh* priorityAttr = elem->getAttribute(BMLDefs::ATTR_PRIORITY_JOINT);
	std::string animationName;
	std::string localId;
	std::string lexeme;
	std::string type;
	std::string mode;
	std::string style;
	xml_utils::xml_translate(&localId, id);
	xml_utils::xml_translate(&animationName, animName);
	xml_utils::xml_translate(&lexeme, lexemeAttr);
	xml_utils::xml_translate(&type, typeAttr);
	xml_utils::xml_translate(&mode, modeAttr);
	xml_utils::xml_translate(&style, styleAttr);

	bool isAdditive = false;
	std::string additiveStr = xml_parse_string( BMLDefs::ATTR_ADDITIVE, elem, "false" );	
	if( stringICompare(additiveStr,"false") ) 
	{			
		isAdditive = false;
	}
	else //if (stringICompare(attrFootIK,"false"))
	{			
		isAdditive = true;
	}

	std::string emotion = xml_utils::xml_parse_string(BMLDefs::ATTR_EMOTION, elem, "neutral");
	GestureRequest::EmotionTag tag = GestureRequest::NEUTRAL;
	if (emotion == "sad")
		tag = GestureRequest::MEEK;
	else if (emotion == "angry")
		tag = GestureRequest::EMPHATIC;
	else if (emotion == "furious")
		tag = GestureRequest::FURIOUS;
	else
		tag = GestureRequest::NEUTRAL;

	SmartBody::SBCharacter* character = dynamic_cast<SmartBody::SBCharacter*>(request->actor);
	std::string gestureMapName = "";
	if (tag == GestureRequest::EMPHATIC)
		gestureMapName = character->getStringAttribute("gestureMapEmphatic");
	if (tag == GestureRequest::MEEK)
		gestureMapName = character->getStringAttribute("gestureMapMeek");
	if (tag == GestureRequest::FURIOUS)
		gestureMapName = character->getStringAttribute("gestureMapFurious");
	if (gestureMapName == "")
		gestureMapName = character->getStringAttribute("gestureMap");

	std::vector<std::string> animationList;
	if (animationName == "")	// If you have assigned the animation name, do not look for the map
	{
		SmartBody::SBGestureMap* gestureMap = SmartBody::SBScene::getScene()->getGestureMapManager()->getGestureMap(gestureMapName);
		if (!gestureMap)
		{
			LOG("WARNING: BML::parse_bml_gesture(): gesture map for character %s doesn't exist.", request->actor->getName().c_str());
			return BehaviorRequestPtr();		
		}

		// Get current posture
		std::string posture = "";
		if (request->actor->posture_sched_p)
		{
			MeCtScheduler2::VecOfTrack tracks = request->actor->posture_sched_p->tracks();
			for (size_t t = 0; t < tracks.size(); t++)
			{
				MeCtMotion* motionCt = dynamic_cast<MeCtMotion*>(tracks[t]->animation_ct());
				if (motionCt)
				{
					posture = motionCt->motion()->getName();
				}
			}
		}
		if (isAdditive)			// if is additive, do not take in consideration of current posture.
		{
			posture = "";
		}
		animationName = gestureMap->getGestureByInfo(lexeme, type, mode, style, posture, request->actor->getStringAttribute("gesturePolicy"));
		animationList = gestureMap->getGestureListByInfo(lexeme, type, mode, style, posture);
		if (animationName == "")
		{
			LOG("Could not find animation for: %s %s %s %s %s", lexeme.c_str(), type.c_str(), mode.c_str(), style.c_str(), posture.c_str());
			return BehaviorRequestPtr();
		}
		else
			LOG("bml_gesture gesture retrieval: %s", animationName.c_str());
	}

	if (animationName == "")
	{
		LOG("WARNING: BML::parse_bml_gesture(): invalid animation name");
		return BehaviorRequestPtr();
	}

	// play the animation
	if (!request->actor->motion_sched_p)
	{
		LOG("Character %s does not have a motion scheduler, so cannot schedule motion.", request->actor->getName().c_str());
		return BehaviorRequestPtr();
	}
	SmartBody::SBMotion* motion = SmartBody::SBScene::getScene()->getAssetManager()->getMotion(animationName);

	if (motion)
	{
		SkMotion* mForCt = motion;
		MeCtMotion* motionCt = new MeCtMotion();
		if (isAdditive)
		{
			mForCt = motion->getOffset();
			motionCt->isAdditive(true);
		}
		else
			mForCt = motion;

		// Name controller with behavior unique_id
		ostringstream name;
		name << unique_id << ' ' << motion->getName();
		motionCt->setName(name.str().c_str());  // TODO: include BML act and behavior ids
		
		// pre stroke hold
		float prestrokehold = (float)xml_utils::xml_parse_double(BMLDefs::ATTR_PRESTROKE_HOLD, elem, -1.0);
		std::string prestrokehold_idlemotion = xml_utils::xml_parse_string(BMLDefs::ATTR_PRESTROKE_HOLD_IDLEMOTION, elem);
		SkMotion* preIdleMotion = (SkMotion*)SmartBody::SBScene::getScene()->getMotion(prestrokehold_idlemotion);
		if (prestrokehold > 0)
			mForCt = motion->buildPrestrokeHoldMotion(prestrokehold, preIdleMotion);
		float poststrokehold = (float)xml_utils::xml_parse_double(BMLDefs::ATTR_POSTSTROKE_HOLD, elem, -1.0);
		std::string poststrokehold_idlemotion = xml_utils::xml_parse_string(BMLDefs::ATTR_POSTSTROKE_HOLD_IDLEMOTION, elem);
		SkMotion* postIdleMotion = (SkMotion*)SmartBody::SBScene::getScene()->getMotion(poststrokehold_idlemotion);
		
		// post stroke hold (it's alternative to setting stroke and relax time)
		std::string joints = xml_utils::xml_parse_string(BMLDefs::ATTR_JOINT_RANGE, elem);
		float scale = (float)xml_utils::xml_parse_double(BMLDefs::ATTR_SCALE, elem, 0.02f);
		float freq = (float)xml_utils::xml_parse_double(BMLDefs::ATTR_FREQUENCY, elem, 0.03f);
		std::string strokeString = xml_utils::xml_parse_string(BMLDefs::ATTR_STROKE, elem);
		std::string relaxString = xml_utils::xml_parse_string(BMLDefs::ATTR_RELAX, elem);
		if (poststrokehold > 0 && (strokeString == "" || relaxString == ""))
		{
			std::vector<std::string> jointVec;
			vhcl::Tokenize(joints, jointVec);
			mForCt = mForCt->buildPoststrokeHoldMotion(poststrokehold, jointVec, scale, freq, postIdleMotion);
			SmartBody::SBMotion* sbMForCT = dynamic_cast<SmartBody::SBMotion*>(mForCt);
			if (sbMForCT)
				sbMForCT->setMotionSkeletonName(motion->getMotionSkeletonName());
		}

		// gesture + locomotion
		SmartBody::SBCharacter* sbCharacter = dynamic_cast<SmartBody::SBCharacter*>(request->actor);
		bool isInLocomotion = false;
	#ifdef EMBER_SB_STEER
		SmartBody::SBSteerManager* steerManager = SmartBody::SBScene::getScene()->getSteerManager();
		SmartBody::SBSteerAgent* steerAgent = steerManager->getSteerAgent(request->actor->getName());
		if (steerAgent)
		{
			for (size_t i = 0; i < request->behaviors.size(); ++i)
			{
				if ((request->behaviors)[i]->local_id == "locomotion")
				{
					isInLocomotion = true;
					break;
				}
			}
			PPRAISteeringAgent* ppraiAgent = dynamic_cast<PPRAISteeringAgent*>(steerAgent);
			if (ppraiAgent->isInLocomotion())
				isInLocomotion = true;
		}
	#else
		for (size_t i = 0; i < request->behaviors.size(); ++i)
		{
			if ((request->behaviors)[i]->local_id == "locomotion")
			{
				isInLocomotion = true;
				break;
			}
		}
	#endif
		
		if (isInLocomotion)
		{
			std::vector<std::string> jointNames = sbCharacter->getSkeleton()->getUpperBodyJointNames();
			motionCt->init( const_cast<SbmCharacter*>(request->actor), mForCt, jointNames);
		}
		else
		{
			motionCt->init( const_cast<SbmCharacter*>(request->actor), mForCt, 0.0, 1.0);
		}

		int priority = xml_utils::xml_parse_int(BMLDefs::ATTR_PRIORITY, elem, 0);
		BehaviorRequestPtr behavPtr(new GestureRequest( unique_id, localId, motionCt, request->actor->motion_sched_p, behav_syncs, animationList, joints, scale, freq, priority, tag) );
		return behavPtr; 
	} 
	else 
	{
		LOG("WARNING: BML::parse_bml_gesture(): behavior \"%s\": name=\"%s\" not loaded; ignoring behavior.", unique_id.c_str(), animationName.c_str());
		return BehaviorRequestPtr();
	}
}
