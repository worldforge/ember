#ifndef		_ME_CT_PHYSICS_CONTROLLER_H_
#define _ME_CT_PHYSICS_CONTROLLER_H_

#include <sb/SBController.h>
#include <sb/sbm_character.hpp>
#include <map>

class MeCtPhysicsController : public SmartBody::SBController
{
	public:
		static std::string CONTROLLER_TYPE;

	private:
		double			_prevTime;
		double			_dt;
		float 			_duration;
		SkChannelArray	_channels;
		SbmCharacter* 	_character;
		SmartBody::SBSkeleton*     _skeletonCopy;
		bool			_valid;	
	public:
		MeCtPhysicsController(SbmCharacter* character);
		~MeCtPhysicsController();

		bool getValid()						{return _valid;}
		void setValid(bool v)				{_valid = v;}		
	protected:
		void getJointChannelValues(const std::string& jointName, MeFrameData& frame, SrQuat& outQuat, SrVec& outPos);
	private:
		virtual bool controller_evaluate(double t, MeFrameData& frame);
		virtual SkChannelArray& controller_channels()	{ return(_channels); }
		virtual double controller_duration()			{ return((double)_duration); }
		virtual const std::string& controller_type() const		{ return(CONTROLLER_TYPE); }			
};

#endif