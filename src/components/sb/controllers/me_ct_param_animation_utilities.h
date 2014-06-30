/*
 *  me_ct_param_animation_utilities.h - part of Motion Engine and SmartBody-lib
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

#ifndef _ME_CT_PARAM_ANIMATION_UTILITIES_H_
#define _ME_CT_PARAM_ANIMATION_UTILITIES_H_

#include <sb/SBTypes.h>
#include <sk/sk_motion.h>
#include <controllers/me_controller.h>
#include <controllers/me_ct_channel_writer.hpp>
#include <sbm/sr_linear_curve.h>
#include <sb/sbm_pawn.hpp>
#include <sb/PABlend.h>
#include <sb/SBAnimationTransition.h>
#include <sb/SBRetarget.h>

#define LoopHandle 0
const int rotType = 132;
//const int rotType = 312;
const double defaultTransition = 0.3;

struct JointChannelId 
{
	int x, y, z, q;
};

class PABlendData;

class PATimeManager
{
	public:
		PATimeManager();
		PATimeManager(PABlendData* data);
		~PATimeManager();

		int getNumKeys();
		void updateLocalTimes(double time);
		bool step(double timeStep);
		SBAPI void updateWeights();
		double getDuration();		
		double getNormalizeLocalTime();
		double getPrevNormalizeLocalTime();
		SBAPI void getParallelTimes(double time, std::vector<double>& times);
		std::vector<double>& getKey();

		void loadEvents();
		void checkEvents();


		std::vector<double> localTimes;			//always in ascending order
		std::vector<double> motionTimes;		//actual motion times get from localTimes
		std::vector<double> timeDiffs;			//time steps from last evaluation to this evaluation, get from motionTimes
		std::vector<double> key;				//key get from keys and current weights
	protected:
		double localTime;
		double prevLocalTime;		

		PABlendData* blendData;

	public:
	protected:
		void setKey();
		void setLocalTime();
		void setMotionTimes();
		int getSection(double time);
		

		std::queue<std::pair<SmartBody::SBMotionEvent*, int> > _events;
};

class PAMotions
{
	public:
		MeControllerContext* _context;
	
	protected:
		std::vector<SrBuffer<int> > motionContextMaps;
		JointChannelId baseChanId;
		JointChannelId baseBuffId;
		SrQuat basePrerot;

	public:
		PAMotions();
		PAMotions(PABlendData* data);
		~PAMotions();

		int getNumMotions();
		void setMotionContextMaps(MeControllerContext* context);
		void initChanId(MeControllerContext* context, std::string baseJointName);
		void initPreRotation(const SrQuat& q);
		SrMat getBaseMatFromBuffer(SrBuffer<float>& buffer);
		void setBufferByBaseMat(SrMat& mat, SrBuffer<float>& buffer);

	protected:
		void getBuffer(SkMotion* motion, double t, SrBuffer<int>& map, SrBuffer<float>& buff);				
		void getUpdateMat(SrMat& dest, SrMat& src);
		void getProcessedMat(SrMat& dest, SrMat& src);
		PABlendData* blendData;
};

class PAWoManager : public PAMotions
{
	private:
		bool firstTime;
		std::vector<SrMat>	baseMats;
		std::vector<SrMat>	baseTransitionMats;
		std::vector<SrMat>  baseDiffMats;
		bool intializeTransition;
		SrMat baseTransformMat;
		SrMat firstBaseTransformMat;
		SrMat currentBaseTransformMat;
	public:
		PAWoManager();
		PAWoManager(PABlendData* data);
		~PAWoManager();

		void apply(std::vector<double>& times, std::vector<double>& timeDiffs, SrBuffer<float>& buffer, bool directOverride = false);
		SrMat& getBaseTransformMat();
		SrMat& getFirstBaseTransformMat();
		SrMat& getCurrentBaseTransformMat();
		static void matInterp(SrMat& ret, SrMat& mat1, SrMat& mat2, float w);

	private:
		void getBaseMats(std::vector<SrMat>& mats, std::vector<double>& times, std::vector<double>& timeDiffs, int bufferSize, SrBuffer<float>& inBuff);
};	

class PAInterpolator : public PAMotions
{
	public:
		PAInterpolator();
		PAInterpolator(PABlendData* data);
		~PAInterpolator();

		std::vector<std::string> joints;	// joints to be blended, if this is defined which means partial, world offset would be ignored
		std::vector<SrMat> processedBaseMats;

	public:
		void blending(std::vector<double>& times, SrBuffer<float>& buff);
		void clearBlendingJoints();
		void setBlendingJoints(std::vector<std::string>& j);

	private:
		SrMat handleBaseMatForBuffer(SrBuffer<float>& buff);
};

class PABlend;
class PABlendData
{
	public:
		enum WrapMode { Loop, Once };
		enum BlendMode { Overwrite, Additive };
		enum ScheduleMode { Now, Queued };

	public:
		SBAPI PABlendData();
		SBAPI PABlendData(const std::string& stateName, std::vector<double>& w, BlendMode blend = Overwrite, WrapMode wrap = Loop, ScheduleMode schedule = Queued, double blendOffset = 0.0, double blendTrim = 0.0, bool dplay = false);
		SBAPI PABlendData(PABlend* state, std::vector<double>& w, BlendMode blend = Overwrite, WrapMode wrap = Loop, ScheduleMode schedule = Queued, double blendOffset = 0.0, double blendTrim = 0.0, bool dplay = false);
		SBAPI ~PABlendData();
		virtual void evaluate(double timeStep, SrBuffer<float>& buffer);
		virtual void evaluateTransition(double timeStep, SrBuffer<float>& buffer, bool tranIn);

		std::string getStateName();		
		
		float getStateKeyTime(int motionIdx, int sectionIdx);
		float getStateMotionDuration(int motionIdx);

		bool isPartialBlending();
		bool isZeroDState();
		SBAPI bool getTrajPosition(std::string effectorName, float time, SrVec& outPos);

		std::vector<double> weights;

		PATimeManager* timeManager;
		PAWoManager* woManager;
		PAInterpolator* interpolator;
		SmartBody::SBRetarget* retarget;
		WrapMode wrapMode;
		BlendMode blendMode;
		ScheduleMode scheduleMode;
		PABlend* state;
		float blendStartOffset, blendEndTrim;
		float transitionLength;
		float playSpeed;
		bool active;	
		bool directPlay;
		std::string baseJointName;
		std::vector<std::vector<int> > motionIndex;
		void updateMotionIndices();
};

class PATransitionManager
{
	public:
		
		PATransitionManager(float transitionLen);
		PATransitionManager(double easeOutStart, double duration);
		PATransitionManager(SmartBody::SBAnimationTransition* transition, PABlendData* from, PABlendData* to);
		~PATransitionManager();

		void align(PABlendData* current, PABlendData* next);
		void blending(SrBuffer<float>& buffer, SrBuffer<float>&buffer1, SrBuffer<float>&buffer2, SrMat& mat, SrMat& mat1, SrMat& mat2, double timeStep, MeControllerContext* context, bool directCombine = false);

		void step( double timeStep );
		void update();
		double getSlope();
		int getNumEaseOut();
		float getCurrentTransitionWeight();
		bool blendingMode;
		bool active;
		bool startTransition;

		PABlendData* from;
		PABlendData* to;
		SmartBody::SBAnimationTransition* transition;
		srLinearCurve* curve;
		double duration;
		double localTime;
		std::vector<double> easeOutStarts;
		std::vector<double> easeOutEnds;
		double s1;
		double e1;
		double s2;
		double e2;
		


		static void bufferBlending(SrBuffer<float>& buffer, SrBuffer<float>& buffer1, SrBuffer<float>& buffer2, double w, MeControllerContext* context);

	private:
		double getTime(double time, const std::vector<double>& key, const std::vector<std::vector<double> >& keys, const std::vector<double>& w);
};

#endif
