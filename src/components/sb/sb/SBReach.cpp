#include "SBReach.h"
#include <sb/SBCharacter.h>
#include <sb/SBMotion.h>
#include <controllers/MeCtBodyReachState.h>

namespace SmartBody {

SBReach::SBReach()
{
	_character = NULL;
	interpolatorType = "KNN";	
}

SBReach::SBReach(SBCharacter* character)
{
	_character = character;
	interpolatorType = "KNN";	
}

SBReach::~SBReach()
{
}

SBCharacter* SBReach::getCharacter()
{
	return _character;
}

SBReach* SBReach::copy()
{
	// ?????
	// ...
	// ...
	return NULL;
}

void SBReach::addMotion(std::string type, SBMotion* motion)
{
	if (!_character)
	{
		LOG("No character present, motion %s was not added to reach.", motion->getName().c_str());
		return;
	}

	int reachType = MeCtReachEngine::getReachType(type);
	if (reachType != -1)
	{
		//_character->addReachMotion(reachType,motion);
		TagMotion tagMotion = TagMotion(reachType, motion);
		if (reachMotionData.find(tagMotion) == reachMotionData.end()) 
		{
			reachMotionData.insert(tagMotion);			
		}		
	}
	else
	{
		LOG("Please use 'LEFT' or 'RIGHT'");
		return;
	}
}

void SBReach::removeMotion(std::string type, SBMotion* motion)
{
	if (!_character)
	{
		LOG("No character present, motion %s was not removed from reach.", motion->getName().c_str());
		return;
	}

	int reachType = MeCtReachEngine::getReachType(type);
	if (reachType != -1)
	{
		TagMotion tagMotion = TagMotion(reachType, motion);
		if (reachMotionData.find(tagMotion) != reachMotionData.end()) 
		{
			reachMotionData.erase(tagMotion);			
		}		
	}	
	else
	{
		LOG("Please use 'LEFT' or 'RIGHT'");
		return;
	}
}

int SBReach::getNumMotions()
{
	if (!_character)
		return 0;

	return reachMotionData.size();
}

std::vector<std::string> SBReach::getMotionNames(std::string type)
{
	std::vector<std::string> motionNames;
	//MotionDataSet& motionSet = const_cast<MotionDataSet&>(_character->getReachMotionDataSet());
	MotionDataSet& motionSet = reachMotionData;
	MotionDataSet::iterator vi;
	for ( vi  = motionSet.begin();
		  vi != motionSet.end();
		  vi++)
	{
		SkMotion* motion = (*vi).second;
		motionNames.push_back(motion->getName());
	}	
	return motionNames;

}
void SBReach::build(SBCharacter* character)
{
	if (!character)
		return;

	// create the reach engine if the character does not have one.
	character->createReachEngine();
	character->setReach(this);

	for (ReachEngineMap::iterator mi = character->getReachEngineMap().begin();
		mi != character->getReachEngineMap().end();
		mi++)
	{
		MeCtReachEngine* re = mi->second;
		if (re)
		{
			re->updateMotionExamples(reachMotionData, interpolatorType);
		}
	}
}


bool SBReach::isPawnAttached( std::string pawnName )
{	
	if (!_character)
		return false;

	for (ReachEngineMap::iterator mi = _character->getReachEngineMap().begin();
		mi != _character->getReachEngineMap().end();
		mi++)
	{
		MeCtReachEngine* re = mi->second;
		if (re)
		{
			if (re->getReachData()->effectorState.attachedPawnName == pawnName)
			{
				return true;
			}
		}
	}
	return false;
}

void SBReach::setPointHandMotion( std::string type, SBMotion* pointMotion )
{
	if (!_character)
		return;
	MotionDataSet& pointMotionSet = pointHandData;
	int reachType = MeCtReachEngine::getReachType(type);
	if (reachType != -1)
	{
		TagMotion motion = TagMotion(reachType,pointMotion);
		pointMotionSet.insert(motion);
	}
	else
	{
		LOG("Please use 'LEFT' or 'RIGHT'");
		return;
	}	

}

SBMotion* SBReach::getPointHandMotion( std::string type )
{
	MotionDataSet& pointMotionSet = pointHandData;
	int reachType = MeCtReachEngine::getReachType(type);
	SBMotion* sbMotion = SBReach::findTagMotion(reachType,pointMotionSet);	
	return sbMotion;
}

void SBReach::setGrabHandMotion(std::string type, SBMotion* grabMotion)
{
	if (!_character)
		return;
	MotionDataSet& grabMotionSet = grabHandData;
	int reachType = MeCtReachEngine::getReachType(type);
	if (reachType != -1)
	{
		TagMotion motion = TagMotion(reachType,grabMotion);
		grabMotionSet.insert(motion);
	}
	else
	{
		LOG("Please use 'LEFT' or 'RIGHT'");
		return;
	}	
}

SBMotion* SBReach::getGrabHandMotion(std::string type)
{
	MotionDataSet& grabMotionSet = grabHandData;
	int reachType = MeCtReachEngine::getReachType(type);
	SBMotion* sbMotion = SBReach::findTagMotion(reachType,grabMotionSet);	
	return sbMotion;
}

void SBReach::setReleaseHandMotion(std::string type, SBMotion* releasebMotion)
{
	if (!_character)
		return;
	MotionDataSet& releaseMotionSet = releaseHandData;
	int reachType = MeCtReachEngine::getReachType(type);
	if (reachType != -1)
	{
		TagMotion motion = TagMotion(reachType,releasebMotion);
		releaseMotionSet.insert(motion);
	}
	else
	{
		LOG("Please use 'LEFT' or 'RIGHT'");
		return;
	}	
}

SBMotion* SBReach::getReleaseHandMotion(std::string type)
{
	MotionDataSet& releaseMotionSet = releaseHandData;
	int reachType = MeCtReachEngine::getReachType(type);
	SBMotion* sbMotion = SBReach::findTagMotion(reachType,releaseMotionSet);	
	return sbMotion;
}

void SBReach::setReachHandMotion(std::string type, SBMotion* reachMotion)
{
	if (!_character)
		return;
	MotionDataSet& reachMotionSet = reachHandData;
	int reachType = MeCtReachEngine::getReachType(type);
	if (reachType != -1)
	{
		TagMotion motion = TagMotion(reachType,reachMotion);
		reachMotionSet.insert(motion);
	}
	else
	{
		LOG("Please use 'LEFT' or 'RIGHT'");
		return;
	}	
}

SBMotion* SBReach::getReachHandMotion(std::string type)
{
	MotionDataSet& reachMotionSet = reachHandData;
	int reachType = MeCtReachEngine::getReachType(type);
	SBMotion* sbMotion  = SBReach::findTagMotion(reachType,reachMotionSet);	
	return sbMotion;
}

void SBReach::setInterpolatorType( std::string type )
{
	interpolatorType = type;
}

std::string& SBReach::getInterpolatorType()
{
	return interpolatorType;
}

SBMotion* SBReach::findTagMotion( int tag, const MotionDataSet& motionSet )
{
	MotionDataSet::const_iterator vi;
	for ( vi  = motionSet.begin();
		vi != motionSet.end();
		vi++)
	{
		TagMotion tagMotion = *vi;
		if (tagMotion.first == tag)
			return tagMotion.second;
	}
	return NULL;
}

}