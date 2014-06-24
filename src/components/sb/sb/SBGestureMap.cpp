#include "SBGestureMap.h"

#include "SBCharacter.h"
#include "SBScene.h"
#include "SBMotion.h"
#include <cstdlib>
#include <ctime> 
#include <math.h>
#include <strings.h>

#ifndef WIN32
#define _stricmp strcasecmp
#endif

namespace SmartBody {

SBGestureMap::SBGestureMap() : SBObject()
{
	defaultGestureInfo._animation = "";
	defaultGestureInfo._lexeme = "";
	defaultGestureInfo._type = "";
	defaultGestureInfo._hand = "";
	defaultGestureInfo._style = "";
	defaultGestureInfo._posture = "";

	srand((unsigned int)time(0));
	//srand(clock());
}

SBGestureMap::SBGestureMap(const std::string& name)
{
	setName(name);

	defaultGestureInfo._animation = "";
	defaultGestureInfo._lexeme = "";
	defaultGestureInfo._type = "";
	defaultGestureInfo._hand = "";
	defaultGestureInfo._style = "";
	defaultGestureInfo._posture = "";

	srand((unsigned int)time(0));
	//srand(clock());
}

SBGestureMap::~SBGestureMap()
{
	_gestureMaps.clear();
}

SBGestureMap* SBGestureMap::copy()
{
	// TODO:
	return NULL;
}

void SBGestureMap::addGestureMapping(const std::string& name, const std::string& lexeme, const std::string& type, const std::string& hand, const std::string& style, const std::string& posture)
{
	GestureInfo gInfo;
	gInfo._animation = name;
	gInfo._lexeme = lexeme;
	gInfo._type = type;
	gInfo._hand = hand;
	gInfo._style = style;
	gInfo._posture = posture;

	_gestureMaps.push_back(gInfo);

	SBMotion* motion = SmartBody::SBScene::getScene()->getMotion(name);
	if (!motion)
		return;
	motion->setMotionType(SBMotion::Gesture);
}

bool SBGestureMap::gestureInfoCompare(	const std::string& glexeme, const std::string& gtype, const std::string& ghand, const std::string& gstyle, const std::string& gposture,
										const std::string& lexeme, const std::string& type, const std::string& hand, const std::string& style, const std::string& posture, 
										std::string compType)
{
	if (compType == "lexeme")
		return (_stricmp(glexeme.c_str(), lexeme.c_str()) == 0);
	if (compType == "type")
		return (_stricmp(gtype.c_str(), type.c_str()) == 0);
	if (compType == "hand")
		return (_stricmp(ghand.c_str(), hand.c_str()) == 0);
	if (compType == "style")
		return (_stricmp(gstyle.c_str(), style.c_str()) == 0);
	if (compType == "posture")
		return (_stricmp(gposture.c_str(), posture.c_str()) == 0);
	return true;
}


std::vector<std::string> SBGestureMap::getGestureListByInfo(const std::string& lexeme, const std::string& type, const std::string& hand, const std::string& style, const std::string& posture)
{
	std::vector<std::string> compTypes;
	if (lexeme != "")
		compTypes.push_back("lexeme");
	if (type != "")
		compTypes.push_back("type");
	if (hand != "")
		compTypes.push_back("hand");
	if (style != "")
		compTypes.push_back("style");
	if (posture != "")
		compTypes.push_back("posture");


	std::vector<std::string> retAnimations;
	for (std::vector<GestureInfo>::iterator iter = _gestureMaps.begin(); 
		iter != _gestureMaps.end(); 
		iter++)
	{
		bool hasMatching = true;
		for (size_t i = 0; i < compTypes.size(); ++i)
		{
			if (!gestureInfoCompare(iter->_lexeme, iter->_type, iter->_hand, iter->_style, iter->_posture, lexeme, type, hand, style, posture, compTypes[i]))
			{
				hasMatching = false;
				break;
			}
		}

		if (hasMatching)
			retAnimations.push_back(iter->_animation);
	}

	return retAnimations;
}

/* 
 *	policy can be "random" | "first"
 *  user can only input type and get the result
 *	posture always has to match
 */
std::string SBGestureMap::getGestureByInfo(const std::string& lexeme, const std::string& type, const std::string& hand, const std::string& style, const std::string& posture, const std::string& policy)
{
	const std::vector<std::string>& retAnimations = getGestureListByInfo(lexeme, type, hand, style, posture);
	if (retAnimations.size() == 0)
	{
		LOG("Gesture %s cannot find gesture with type %s, posture %s, hand %s.", getName().c_str(), type.c_str(), posture.c_str(), hand.c_str());
		return "";
	}
	else
	{
		if (policy == "random")
		{
			int id = rand() % retAnimations.size();
			if (id == retAnimations.size())
				id = id - 1;
			return retAnimations[id];
		}
		else if (policy == "first")
			return retAnimations[0];
		else
			return retAnimations[0];
	}
}

SBGestureMap::GestureInfo& SBGestureMap::getGestureByIndex(int i)
{
	if (i < 0 || i >= int(_gestureMaps.size()))
	{
		LOG("Index %d out of range of gesture map.", i);
		return defaultGestureInfo;
	}

	return _gestureMaps[i];
}


SBAPI std::string SBGestureMap::getGestureAnimation( int i )
{
	GestureInfo& gesture = getGestureByIndex(i);
	return gesture._animation;
}

int SBGestureMap::getNumMappings()
{
	return _gestureMaps.size();
}

void SBGestureMap::validate()
{
	SBScene* scene = SBScene::getScene();
	std::vector<GestureInfo>::iterator iter = _gestureMaps.begin();
	for (; iter != _gestureMaps.end(); iter++)
	{
		LOG("Gesture: motion='%s' idle='%s' lexeme='%s', type='%s', hand='%s', style='%s'", 
			 iter->_animation.c_str(), iter->_posture.c_str(), iter->_lexeme.c_str(), iter->_type.c_str(), iter->_hand.c_str(), iter->_style.c_str());
		SBMotion* animation = scene->getMotion(iter->_animation);
		if (!animation)
		{
			LOG("WARNING: Animation '%s' is not loaded.", iter->_animation.c_str());
		}
		SBMotion* idle = scene->getMotion(iter->_posture);
		if (!idle)
		{
			LOG("WARNING: Idle '%s' is not loaded.", iter->_posture.c_str());
		}
	}

}


}
