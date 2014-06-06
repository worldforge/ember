#include "SBBehavior.h"

namespace SmartBody {

SBBehavior::SBBehavior()
{
}

SBBehavior::~SBBehavior()
{
}

const std::string& SBBehavior::getType()
{
	return _type;
}

const std::string& SBBehavior::getId()
{
	return _id;
}

void SBBehavior::setId(const std::string& id)
{
	_id = id;
}

SpeechBehavior::SpeechBehavior()
{
	_type = "speech";
	_utterance = "";
}

SpeechBehavior::~SpeechBehavior()
{
}

const std::string& SpeechBehavior::getUtterance()
{
	return _utterance;
}

void SpeechBehavior::setUtterance(const std::string& utterance)
{
	_utterance = utterance;
}

GazeBehavior::GazeBehavior()
{
	_type = "gaze";
}

GazeBehavior::~GazeBehavior()
{
}

void GazeBehavior::setGazeTarget(const std::string& target)
{
	_target = target;
}

const std::string& GazeBehavior::getGazeTarget()
{
	return _target;
}

void GazeBehavior::setFadingIn(bool val)
{
	_fadingIn = val;
}

void GazeBehavior::setFadingOut(bool val)
{
	_fadingOut = val;
}

void GazeBehavior::setFadedOut(bool val)
{
	_fadedOut = val;
}

bool GazeBehavior::isFadingIn()
{
	return _fadingIn;
}

bool GazeBehavior::isFadingOut()
{
	return _fadingOut;
}

bool GazeBehavior::isFadedOut()
{
	return _fadedOut;
}

void GazeBehavior::setHandle(const std::string& handle)
{
	_handle = handle;
}

const std::string& GazeBehavior::getHandle()
{
	return _handle;
}


LocomotionBehavior::LocomotionBehavior()
{
	_type = "locomotion";
	_reachTarget = false;
}

LocomotionBehavior::~LocomotionBehavior()
{
}

void LocomotionBehavior::setLocomotionTarget(const SrVec& target)
{
	_target = target;
}

const SrVec& LocomotionBehavior::getLocomotionTarget()
{
	return _target;
}

void LocomotionBehavior::setReachTarget(bool v)
{
	_reachTarget = v;
}

bool LocomotionBehavior::getReachTarget()
{
	return _reachTarget;
}

PostureBehavior::PostureBehavior()
{
	_type = "posture";
}

PostureBehavior::~PostureBehavior()
{
}

void PostureBehavior::setPosture(const std::string& posture)
{
	_posture = posture;
}

const std::string& PostureBehavior::getPosture()
{
	return _posture;
}

};

