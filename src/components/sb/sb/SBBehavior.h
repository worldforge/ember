#ifndef _SBBEHAVIOR_H_
#define _SBBEHAVIOR_H_

#include <sb/SBTypes.h>
#include <string>
#include <sr/sr_vec.h>

namespace SmartBody {

class SBBehavior
{
	public:
		SBAPI SBBehavior();
		SBAPI ~SBBehavior();

		SBAPI virtual const std::string& getType();
		SBAPI virtual const std::string& getId();
		SBAPI virtual void setId(const std::string& id);

	protected:
		std::string _type;
		std::string _id;

};


class SpeechBehavior : public SBBehavior
{
	public:
		SBAPI SpeechBehavior();
		SBAPI ~SpeechBehavior();

		SBAPI virtual void setUtterance(const std::string& utterance);
		SBAPI virtual const std::string& getUtterance();

	protected:
		std::string _utterance;


};

class GazeBehavior : public SBBehavior
{
	public:
		SBAPI GazeBehavior();
		SBAPI ~GazeBehavior();

		SBAPI virtual void setGazeTarget(const std::string& target);
		SBAPI virtual const std::string& getGazeTarget();
		SBAPI void setFadingIn(bool val);
		SBAPI void setFadingOut(bool val);
		SBAPI void setFadedOut(bool val);
		SBAPI bool isFadingIn();
		SBAPI bool isFadingOut();
		SBAPI bool isFadedOut();
		SBAPI void setHandle(const std::string& handle);
		SBAPI const std::string& getHandle();

	protected:
		std::string _target;
		bool _fadingIn;
		bool _fadingOut;
		bool _fadedOut;
		std::string _handle;

};

class LocomotionBehavior : public SBBehavior
{
	public:
		SBAPI LocomotionBehavior();
		SBAPI ~LocomotionBehavior();

		SBAPI virtual void setLocomotionTarget(const SrVec& target);
		SBAPI virtual const SrVec& getLocomotionTarget();
		SBAPI void setReachTarget(bool v);
		SBAPI bool getReachTarget();

	protected:
		SrVec _target;
		bool _reachTarget;

};

class PostureBehavior : public SBBehavior
{
	public:
		SBAPI PostureBehavior();
		SBAPI ~PostureBehavior();

		SBAPI virtual void setPosture(const std::string& posture);
		SBAPI virtual const std::string& getPosture();
	
	protected:
		std::string _posture;
};

};

#endif