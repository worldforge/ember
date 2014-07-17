#include "SmartBodyGestures.h"

#include "sb/SBCharacter.h"
#include "sb/SBGestureMapManager.h"
#include "sb/SBGestureMap.h"


using namespace Ember;
namespace Ember
{

SmartBodyGestures::SmartBodyGestures(const std::string& motionPath, const std::string& skeletonRef, 
		SmartBody::SBAssetManager& assetMng, SmartBody::SBAnimationBlendManager& blendMng, SmartBody::SBRetargetManager& retargetMng, 
		SmartBody::SBGestureMapManager& gestureMapMng)
: 	SmartBodyBehaviors::SmartBodyBehaviors(motionPath, skeletonRef, assetMng, blendMng, retargetMng), 
	mGestureMapMng(gestureMapMng)
{
}

SmartBodyGestures::~SmartBodyGestures()
{

}


bool SmartBodyGestures::setup(SmartBody::SBJointMapManager& jointMapManager)
{
	if (mSetup)
	{
		return true;
	}
	
	if (SmartBodyBehaviors::setupAssets(jointMapManager))
	{
		SmartBodyBehaviors::setupBehaviors();

		mapNeutralGestures();
		mapAngryGestures();
		mapSadGestures();

		mSetup = true;
	}

	return mSetup;
}

void SmartBodyGestures::applyRetargeting(SmartBody::SBCharacter& character)
{
	character.setStringAttribute("gestureMap", "NeutralGestures");
	character.setStringAttribute("gestureMapEmphatic", "AngryGestures");
	character.setStringAttribute("gestureMapMeek", "SadGestures");
}


std::vector<std::string> SmartBodyGestures::getMotions()
{
	std::vector<std::string> motions;

	motions.push_back("ChrBrad@Idle01");
	motions.push_back("ChrBrad@Idle01_ArmStretch01");
	motions.push_back("ChrBrad@Idle01_BeatFistMidLf01");
	motions.push_back("ChrBrad@Idle01_BeatFistMidLf02");
	motions.push_back("ChrBrad@Idle01_BeatHighBt01");
	motions.push_back("ChrBrad@Idle01_BeatHighBt02");
	motions.push_back("ChrBrad@Idle01_BeatHighBt03");
	motions.push_back("ChrBrad@Idle01_BeatHighLf01");
	motions.push_back("ChrBrad@Idle01_BeatLowBt01");
	motions.push_back("ChrBrad@Idle01_BeatLowBt02");
	motions.push_back("ChrBrad@Idle01_BeatLowLf01");
	motions.push_back("ChrBrad@Idle01_BeatLowLf02");
	motions.push_back("ChrBrad@Idle01_BeatLowRt01");
	motions.push_back("ChrBrad@Idle01_BeatMidBt01");
	motions.push_back("ChrBrad@Idle01_BeatMidLf01");
	motions.push_back("ChrBrad@Idle01_BeatPointMidLf01");
	motions.push_back("ChrBrad@Idle01_BesidesLf01");
	motions.push_back("ChrBrad@Idle01_ChopBoth01");
	motions.push_back("ChrBrad@Idle01_ChopLf01");
	motions.push_back("ChrBrad@Idle01_ChopLf02");
	motions.push_back("ChrBrad@Idle01_ChopRt02");
	motions.push_back("ChrBrad@Idle01_Contemplate01");
	motions.push_back("ChrBrad@Idle01_ExampleLf01");
	motions.push_back("ChrBrad@Idle01_Guitar01");
	motions.push_back("ChrBrad@Idle01_HereBt01");
	motions.push_back("ChrBrad@Idle01_HereBt02");
	motions.push_back("ChrBrad@Idle01_HoweverLf01");
	motions.push_back("ChrBrad@Idle01_InclusivityNegBt01");
	motions.push_back("ChrBrad@Idle01_InclusivityNegBt02");
	motions.push_back("ChrBrad@Idle01_InclusivityPosBt01");
	motions.push_back("ChrBrad@Idle01_InclusivityPosBt02");
	motions.push_back("ChrBrad@Idle01_IndicateLeftBt01");
	motions.push_back("ChrBrad@Idle01_IndicateLeftLf01");
	motions.push_back("ChrBrad@Idle01_IndicateRightBt01");
	motions.push_back("ChrBrad@Idle01_IndicateRightRt01");
	motions.push_back("ChrBrad@Idle01_MeLf01");
	motions.push_back("ChrBrad@Idle01_MeLf02");
	motions.push_back("ChrBrad@Idle01_NegativeBt01");
	motions.push_back("ChrBrad@Idle01_NegativeBt02");
	motions.push_back("ChrBrad@Idle01_NegativeBt03");
	motions.push_back("ChrBrad@Idle01_NegativeRt01");
	motions.push_back("ChrBrad@Idle01_OfferBoth01");
	motions.push_back("ChrBrad@Idle01_OfferLf01");
	motions.push_back("ChrBrad@Idle01_PleaBt02");
	motions.push_back("ChrBrad@Idle01_PointLf01");
	motions.push_back("ChrBrad@Idle01_PointLf02");
	motions.push_back("ChrBrad@Idle01_PointRt01");
	motions.push_back("ChrBrad@Idle01_ReceiveRt01");
	motions.push_back("ChrBrad@Idle01_SafeLf01");
	motions.push_back("ChrBrad@Idle01_ScratchChest01");
	motions.push_back("ChrBrad@Idle01_ScratchHeadLf01");
	motions.push_back("ChrBrad@Idle01_ScratchTempleLf01");
	motions.push_back("ChrBrad@Idle01_ShoulderStretch01");
	motions.push_back("ChrBrad@Idle01_Shrug01");
	motions.push_back("ChrBrad@Idle01_Shrug02");
	motions.push_back("ChrBrad@Idle01_SqueezeHand01");
	motions.push_back("ChrBrad@Idle01_StopBt01");
	motions.push_back("ChrBrad@Idle01_StopBt02");
	motions.push_back("ChrBrad@Idle01_SurroundBt01");
	motions.push_back("ChrBrad@Idle01_SurroundBt02");
	motions.push_back("ChrBrad@Idle01_Think01");
	motions.push_back("ChrBrad@Idle01_ToIdle02");
	motions.push_back("ChrBrad@Idle01_ToIdle03");
	motions.push_back("ChrBrad@Idle01_ToLocIdle01");
	motions.push_back("ChrBrad@Idle01_TouchHands01");
	motions.push_back("ChrBrad@Idle01_ToWalk01_Turn180Rt01");
	motions.push_back("ChrBrad@Idle01_ToWalk01_Turn360Rt01");
	motions.push_back("ChrBrad@Idle01_ToWalk01_Turn90Rt01");
	motions.push_back("ChrBrad@Idle01_WeightShift01");
	motions.push_back("ChrBrad@Idle01_WeightShift02");
	motions.push_back("ChrBrad@Idle01_WeRt01");
	motions.push_back("ChrBrad@Idle01_WeRt02");
	motions.push_back("ChrBrad@Idle01_WhenBt01");
	motions.push_back("ChrBrad@Idle01_WhenBt02");
	motions.push_back("ChrBrad@Idle01_YouLf01");
	motions.push_back("ChrBrad@Idle01_YouLf02");
	motions.push_back("ChrBrad@Idle01_YouLf03");
	motions.push_back("ChrBrad@Idle01_YouPointLf01");
	motions.push_back("ChrBrad@Idle01_YouRt01");

	return motions;
}


void SmartBodyGestures::mapNeutralGestures()
{	
	SmartBody::SBGestureMap *map = mGestureMapMng.createGestureMap("NeutralGestures");


	map->addGestureMapping("ChrBrad@Idle01_ChopLf01", "METAPHORIC", "OBLIGATION", "LEFT_HAND", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_ChopRt02", "METAPHORIC", "OBLIGATION", "RIGHT_HAND", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_ChopBoth01", "METAPHORIC", "OBLIGATION", "BOTH_HANDS", "", "ChrBrad@Idle01");

	map->addGestureMapping("ChrBrad@Idle01_BeatLowLf01", "BEAT", "RHYTHM", "LEFT_HAND", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_BeatLowLf02", "BEAT", "RHYTHM", "LEFT_HAND", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_BeatLowRt01", "BEAT", "RHYTHM", "RIGHT_HAND", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_BeatLowBt01", "BEAT", "RHYTHM", "BOTH_HANDS", "", "ChrBrad@Idle01");

	map->addGestureMapping("ChrBrad@Idle01_BeatHighLf01", "DEICTIC", "LEFT", "LEFT_HAND", "high", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_BeatMidLf01", "DEICTIC", "LEFT", "LEFT_HAND", "high", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_HoweverLf01", "DEICTIC", "LEFT", "LEFT_HAND", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_IndicateLeftLf01", "DEICTIC", "LEFT", "LEFT_HAND", "", "ChrBrad@Idle01");

	map->addGestureMapping("ChrBrad@Idle01_BeatLowRt01", "DEICTIC", "RIGHT", "RIGHT_HAND", "high", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_IndicateRightRt01", "DEICTIC", "RIGHT", "RIGHT_HAND", "", "ChrBrad@Idle01");

	//map->addGestureMapping("ChrBrad@Idle01_HereBt02", "DEICTIC", "HERE", "BOTH_HANDS", "high", "ChrBrad@Idle01");
	//map->addGestureMapping("ChrBrad@Idle01_ChopBoth01", "DEICTIC", "HERE", "BOTH_HANDS", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_BeatLowBt01", "DEICTIC", "HERE", "BOTH_HANDS", "", "ChrBrad@Idle01");
	//map->addGestureMapping("ChrBrad@Idle01_Frame", "DEICTIC", "HERE", "BOTH_HANDS", "", "ChrBrad@Idle01");

	map->addGestureMapping("ChrBrad@Idle01_NegativeBt02", "METAPHORIC", "NEGATION", "BOTH_HANDS", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_NegativeRt01", "METAPHORIC", "NEGATION", "RIGHT_HAND", "", "ChrBrad@Idle01");

	map->addGestureMapping("ChrBrad@Idle01_SafeLf01", "METAPHORIC", "ASIDE", "LEFT_HAND", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_BesidesLf01", "METAPHORIC", "ASIDE", "LEFT_HAND", "", "ChrBrad@Idle01");

	map->addGestureMapping("ChrBrad@Idle01_BeatMidBt01", "METAPHORIC", "QUESTION", "BOTH_HANDS", "noheadtilt", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_BeatMidLf01", "METAPHORIC", "QUESTION", "LEFT_HAND", "noheadtilt", "ChrBrad@Idle01");

	map->addGestureMapping("ChrBrad@Idle01_YouLf01", "DEICTIC", "YOU", "LEFT_HAND", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_OfferBoth01", "DEICTIC", "YOU", "BOTH_HANDS", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_OfferLf01", "DEICTIC", "YOU", "LEFT_HAND", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_ReceiveRt01", "DEICTIC", "YOU", "RIGHT_HAND", "", "ChrBrad@Idle01");

	map->addGestureMapping("ChrBrad@Idle01_BeatLowBt01", "METAPHORIC", "INTENSIFIER_POSITIVE", "BOTH_HANDS", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_BeatMidBt01", "METAPHORIC", "INTENSIFIER_POSITIVE", "BOTH_HANDS", "", "ChrBrad@Idle01");

	map->addGestureMapping("ChrBrad@Idle01_MeLf01", "DEICTIC", "ME", "LEFT_HAND", "", "ChrBrad@Idle01");

	map->addGestureMapping("ChrBrad@Idle01_WeRt01", "DEICTIC", "WE", "RIGHT_HAND", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_WeRt02", "DEICTIC", "WE", "RIGHT_HAND", "", "ChrBrad@Idle01");

	map->addGestureMapping("ChrBrad@Idle01_BeatLowLf01", "METAPHORIC", "CONTRAST", "LEFT_HAND", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_BeatLowRt01", "METAPHORIC", "CONTRAST", "RIGHT_HAND", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_BeatLowLf02", "METAPHORIC", "CONTRAST", "LEFT_HAND", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_IndicateRightR01", "METAPHORIC", "CONTRAST", "RIGHT_HAND", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_HoweverLf01", "METAPHORIC", "CONTRAST", "LEFT_HAND", "", "ChrBrad@Idle01");

	map->addGestureMapping("ChrBrad@Idle01_BeatHighBt01", "METAPHORIC", "QUESTION", "BOTH_HANDS", "noheadtilt", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_BeatHighBt02", "METAPHORIC", "QUESTION", "BOTH_HANDS", "noheadtilt", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_BeatLowBt01", "METAPHORIC", "QUESTION", "BOTH_HANDS", "noheadtilt", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_BeatMidBt01", "METAPHORIC", "QUESTION", "BOTH_HANDS", "noheadtilt", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_BeatMidLf01", "METAPHORIC", "QUESTION", "LEFT_HAND", "noheadtilt", "ChrBrad@Idle01");

	map->addGestureMapping("ChrBrad@Idle01_Shrug01", "METAPHORIC", "UNCERTAINTY", "BOTH_HANDS", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_Shrug01", "METAPHORIC", "INDIFFERENCE", "BOTH_HANDS", "", "ChrBrad@Idle01");

	map->addGestureMapping("ChrBrad@Idle01_BeatMidBt01", "METAPHORIC", "INCLUSIVITY", "BOTH_HANDS", "mid", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_BeatLowBt01", "METAPHORIC", "INCLUSIVITY", "BOTH_HANDS", "low", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_InclusivityPosBt01", "METAPHORIC", "INCLUSIVITY", "BOTH_HANDS", "low", "ChrBrad@Idle01");

	map->addGestureMapping("ChrBrad@Idle01_NegativeBt01", "METAPHORIC", "EMPTY", "BOTH_HANDS", "high", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_NegativeBt02", "METAPHORIC", "EMPTY", "BOTH_HANDS", "high", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_NegativeRt01", "METAPHORIC", "EMPTY", "RIGHT_HAND", "high", "ChrBrad@Idle01");

	map->addGestureMapping("ChrBrad@Idle01_SurroundBt01", "METAPHORIC", "SURROUND", "BOTH_HANDS", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_StopBt01", "METAPHORIC", "STOP", "RIGHT_HAND", "", "ChrBrad@Idle02");

	map->addGestureMapping("ChrBrad@Idle01_BeatLowLf01", "EMBLEM", "GREETING", "LEFT_HAND", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_BeatLowRt01", "EMBLEM", "GREETING", "RIGHT_HAND", "", "ChrBrad@Idle01");
}

void SmartBodyGestures::mapAngryGestures()
{	
	SmartBody::SBGestureMap *map = mGestureMapMng.createGestureMap("AngryGestures");


	map->addGestureMapping("ChrBrad@Idle01_YouLf02", "DEICTIC", "YOU", "LEFT_HAND", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_YouPointLf01", "DEICTIC", "YOU", "LEFT_HAND", "", "ChrBrad@Idle01");

	map->addGestureMapping("ChrBrad@Idle01_MeLf02", "DEICTIC", "ME", "LEFT_HAND", "", "ChrBrad@Idle01");

	map->addGestureMapping("ChrBrad@Idle01_WeRt02", "DEICTIC", "WE", "RIGHT_HAND", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_BeatMidBt01", "DEICTIC", "WE", "BOTH_HANDS", "", "ChrBrad@Idle01");

	map->addGestureMapping("ChrBrad@Idle01_PointLf02", "DEICTIC", "LEFT", "LEFT_HAND", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_BeatHighLf01", "DEICTIC", "LEFT", "LEFT_HAND", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_PointRt01", "DEICTIC", "RIGHT", "RIGHT_HAND", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_BeatLowBt01", "DEICTIC", "HERE", "BOTH_HANDS", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_BeatLowBt01", "DEICTIC", "CENTER", "BOTH_HANDS", "", "ChrBrad@Idle01");
	//map->addGestureMapping("ChrBrad@Idle01_HereBt02", "DEICTIC", "HERE", "BOTH_HANDS", "", "ChrBrad@Idle01");
	//map->addGestureMapping("ChrBrad@Idle01_HereBt02", "DEICTIC", "CENTER", "BOTH_HANDS", "", "ChrBrad@Idle01");

	map->addGestureMapping("ChrBrad@Idle01_InclusivityNegBt02", "METAPHORIC", "NEGATION", "BOTH_HANDS", "", "ChrBrad@Idle01");

	map->addGestureMapping("ChrBrad@Idle01_BeatHighBt03", "METAPHORIC", "INTENSIFIER_POSITIVE", "BOTH_HANDS", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_BeatHighBt02", "METAPHORIC", "INTENSIFIER_POSITIVE", "BOTH_HANDS", "", "ChrBrad@Idle01");

	map->addGestureMapping("ChrBrad@Idle01_PointRt01", "METAPHORIC", "CONTRAST", "RIGHT_HAND", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_PointLf02", "METAPHORIC", "CONTRAST", "LEFT_HAND", "", "ChrBrad@Idle01");

	map->addGestureMapping("ChrBrad@Idle01_BeatHighBt03", "METAPHORIC", "QUESTION", "BOTH_HANDS", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_PointRt01", "METAPHORIC", "QUESTION", "RIGHT_HAND", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_PointLf02", "METAPHORIC", "QUESTION", "LEFT_HAND", "", "ChrBrad@Idle01");

	map->addGestureMapping("ChrBrad@Idle01_Shrug02", "METAPHORIC", "UNCERTAINTY", "BOTH_HANDS", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_BeatHighBt03", "METAPHORIC", "UNCERTAINTY", "BOTH_HANDS", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_Shrug02", "METAPHORIC", "INDIFFERENCE", "BOTH_HANDS", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_BeatHighBt03", "METAPHORIC", "INDIFFERENCE", "BOTH_HANDS", "", "ChrBrad@Idle01");

	map->addGestureMapping("ChrBrad@Idle01_ChopLf02", "METAPHORIC", "INCLUSIVITY", "BOTH_HANDS", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_InclusivityNegBt02", "METAPHORIC", "EMPTY", "BOTH_HANDS", "", "ChrBrad@Idle01");

	map->addGestureMapping("ChrBrad@Idle01_BesidesLf01", "METAPHORIC", "ASIDE", "LEFT_HAND", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_BesidesLf01", "METAPHORIC", "SURROUND", "BOTH_HANDS", "", "ChrBrad@Idle01");

	map->addGestureMapping("ChrBrad@Idle01_InclusivityNegBt02", "METAPHORIC", "STOP", "BOTH_HANDS", "", "ChrBrad@Idle01");

	map->addGestureMapping("ChrBrad@Idle01_ChopLf02", "METAPHORIC", "OBLIGATION", "LEFT_HAND", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_ChopRt02", "METAPHORIC", "OBLIGATION", "RIGHT_HAND", "", "ChrBrad@Idle01");

	map->addGestureMapping("ChrBrad@Idle01_BeatLowBt02", "BEAT", "RHYTHM", "BOTH_HANDS", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_BeatPointMidLf01", "BEAT", "RHYTHM", "LEFT_HAND", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_BeatFistMidLf01", "BEAT", "RHYTHM", "LEFT_HAND", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_BeatFistMidLf02", "BEAT", "RHYTHM", "LEFT_HAND", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_ChopLf02", "BEAT", "RHYTHM", "LEFT_HAND", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_YouPointLf01", "BEAT", "RHYTHM", "LEFT_HAND", "", "ChrBrad@Idle01");
}

void SmartBodyGestures::mapSadGestures()
{
	SmartBody::SBGestureMap *map = mGestureMapMng.createGestureMap("SadGestures");


	map->addGestureMapping("ChrBrad@Idle01_YouLf03", "DEICTIC", "YOU", "LEFT_HAND", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_BeatLowLf01", "DEICTIC", "YOU", "LEFT_HAND", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_BeatLowRt01", "DEICTIC", "YOU", "RIGHT_HAND", "", "ChrBrad@Idle01");

	map->addGestureMapping("ChrBrad@Idle01_BeatLowLf01", "DEICTIC", "LEFT", "LEFT_HAND", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_BeatLowRt01", "DEICTIC", "RIGHT", "RIGHT_HAND", "", "ChrBrad@Idle01");

	map->addGestureMapping("ChrBrad@Idle01_BeatLowLf01", "METAPHORIC", "NEGATION", "LEFT_HAND", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_BeatLowRt01", "METAPHORIC", "NEGATION", "RIGHT_HAND", "", "ChrBrad@Idle01");

	map->addGestureMapping("ChrBrad@Idle01_BeatLowLf02", "METAPHORIC", "QUESTION", "LEFT_HAND", "", "ChrBrad@Idle01");

	map->addGestureMapping("ChrBrad@Idle01_BeatLowLf02", "METAPHORIC", "INDIFFERENCE", "LEFT_HAND", "", "ChrBrad@Idle01");
	map->addGestureMapping("ChrBrad@Idle01_BeatLowLf02", "METAPHORIC", "UNCERTAINTY", "LEFT_HAND", "", "ChrBrad@Idle01");
}

}