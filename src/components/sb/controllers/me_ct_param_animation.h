/*
 *  me_ct_param_animation.h - part of Motion Engine and SmartBody-lib
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
 *      Yuyu Xu, USC
 */

#ifndef _ME_CT_PARAM_ANIMATION_H_
#define _ME_CT_PARAM_ANIMATION_H_

#include <sb/SBTypes.h>
#include <controllers/me_ct_container.hpp>
#include <sb/SBCharacter.h>
#include <controllers/me_ct_param_animation_utilities.h>
#include <sb/PABlend.h>
#include <controllers/me_ct_motion_example.hpp>
#include <controllers/me_ct_jacobian_IK.hpp>


class PABlend;
class PATransition;
class PABlendData;
class PATransitionManager;
class PAControllerBlending;

struct ScheduleType
{
	PABlendData::WrapMode wrap;
	PABlendData::ScheduleMode schedule;
	PABlendData::BlendMode blend; 
	std::string jName;
	double timeOffset;
	double stateTimeOffset, stateTimeTrim;
	double transitionLen;
	double duration;
	double playSpeed;
	bool   directPlay;
	
	ScheduleType();

};

struct ScheduleUnit
{
	PABlend* data;
	std::vector<double> weights;
	double time;
	float stateTimeOffset, stateTimeTrim;
	float transitionLength;
	float playSpeed;
	float duration;
	bool  directPlay;
	PABlendData::WrapMode wrap;
	PABlendData::ScheduleMode schedule;
	PABlendData::BlendMode blend;
	std::string partialJoint;
};


/*
	MeCtParamAnimation's task:	schedule in states
								controller the transitions between states
								ease in and ease out the states
*/
class MeCtParamAnimation : public MeCtContainer
{
	public:
		static std::string CONTROLLER_TYPE;

		class Context : public MeCtContainer::Context 
		{
		protected:
			static std::string CONTEXT_TYPE;
		public:
			Context( MeCtParamAnimation* container, MeControllerContext* context = NULL )
				:	MeCtContainer::Context( container, context )
			{}

			const std::string& context_type() const {	return CONTEXT_TYPE; }
			void child_channels_updated( MeController* child );
		};

	public:
		MeCtParamAnimation();
		MeCtParamAnimation(SbmCharacter* c, MeCtChannelWriter* wo);
		~MeCtParamAnimation();

		virtual void controller_map_updated();
		virtual SkChannelArray& controller_channels();
		virtual double controller_duration();
		virtual const std::string& controller_type() const {return CONTROLLER_TYPE;}
		virtual bool controller_evaluate( double t, MeFrameData& frame );

		void setBaseJointName(const std::string& name);
		SBAPI const std::string& getBaseJointName();
		
		void schedule(PABlend* state, double x, double y, double z, PABlendData::WrapMode wrap = PABlendData::Loop, PABlendData::ScheduleMode schedule = PABlendData::Queued, PABlendData::BlendMode blend = PABlendData::Overwrite, std::string jName = "", double timeOffset = 0.0, double stateTimeOffset = 0.0, double stateTimeTrim = 0.0, double transitionLen = -1.0, bool directPlay = false);
		void schedule(PABlend* state, const std::vector<double>& weights, PABlendData::WrapMode wrap = PABlendData::Loop, PABlendData::ScheduleMode schedule = PABlendData::Queued, PABlendData::BlendMode blend = PABlendData::Overwrite, std::string jName = "", double timeOffset = 0.0, double stateTimeOffset = 0.0, double stateTimeTrim = 0.0, double transitionLen = -1.0, bool directPlay = false);
		void schedule(PABlend* state, const std::vector<double>& weights, const ScheduleType& scType);
		void unschedule();
		SBAPI void updateWeights(std::vector<double>& w);
		SBAPI void updateWeights();
		
		SBAPI int getNumWeights();
		
		SBAPI const std::string& getNextStateName();
		const std::string& getCurrentStateName();
		SBAPI PABlendData* getCurrentPABlendData();
		bool hasPABlend(const std::string& stateName);
		bool isIdle();
		bool isInTransition();
		void reset();

	private:
		void autoScheduling(double time);
		PABlendData* createStateModule(ScheduleUnit su);		
		void updateWo(SrMat& mat, MeCtChannelWriter* wo, SrBuffer<float>& buffer);
		void updateIK(PABlendData* curBlendData, SrMat& woMat, SrMat& woDeltaMat, float dt, SrBuffer<float>& buff);
		void updateJointTrajectory(PABlendData* blendData);
		SrMat combineMat(SrMat& mat1, SrMat& mat2);
		void updateMotionFrame(BodyMotionFrame& motionFrame, MeCtIKTreeScenario& ikScenario, SrBuffer<float>& buff, bool readData = true);
		void getCharacterJointTrajectory( std::vector<SrVec> &curStatePosList );


	private:

		SkChannelArray	channels;
		SmartBody::SBCharacter*	character;
		std::string baseJointName;
		MeCtChannelWriter* woWriter;
		PATransitionManager* transitionManager;
		double			prevGlobalTime;

		PABlendData*	curStateData;
		PABlendData*	nextStateData;
		std::list<ScheduleUnit> waitingList;
		BodyMotionFrame inputFrame, outputFrame;
		MeCtIKTreeScenario ikScenario;
		SrVec smoothVel;
		float smoothAngVel;
};
#endif