/*
 *  me_ct_param_animation.cpp - part of Motion Engine and SmartBody-lib
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
 */

#include "controllers/me_ct_param_animation.h"

#include <sb/SBCharacter.h>
#include <sb/SBAnimationState.h>
#include <sb/SBAnimationStateManager.h>
#include <sb/SBAnimationTransition.h>
#include <sb/SBAnimationTransitionRule.h>
#include <sb/SBMotionBlendBase.h>
#include <sb/SBSkeleton.h>
#include <sb/SBSimulationManager.h>
#include <sb/SBScene.h>
#include <sb/SBSceneListener.h>
#include <sb/SBRetargetManager.h>
#include <sb/SBRetarget.h>
#include <sr/sr_euler.h>
#include <sr/sr_mat.h>
#include "controllers/MotionAnalysis.h"

std::string MeCtParamAnimation::Context::CONTEXT_TYPE = "MeCtParamAnimation::Context";
std::string MeCtParamAnimation::CONTROLLER_TYPE = "MeCtParamAnimation";

#define debug 0

ScheduleType::ScheduleType()
{
	wrap = PABlendData::Loop;
	schedule = PABlendData::Queued;
	blend = PABlendData::Overwrite;
	jName = "";
	timeOffset = 0.0;
	stateTimeOffset = 0.0;
	stateTimeTrim = 0.0;
	transitionLen = -1.0;
	directPlay = false;
	playSpeed = 1.0;
	duration = -1.0;
}


void MeCtParamAnimation::Context::child_channels_updated( MeController* child )
{
}

MeCtParamAnimation::MeCtParamAnimation() : MeCtContainer(new MeCtParamAnimation::Context(this))
{
	character = NULL;
	woWriter = NULL;
	smoothAngVel = 0.f;
	smoothVel = SrVec();
}

MeCtParamAnimation::MeCtParamAnimation(SbmCharacter* c, MeCtChannelWriter* wo) : MeCtContainer(new MeCtParamAnimation::Context(this)), woWriter(wo)
{
	character = dynamic_cast<SmartBody::SBCharacter*>(c);
	baseJointName = "base";
	curStateData = NULL;
	nextStateData = NULL;
	transitionManager = NULL;
	waitingList.clear();
	prevGlobalTime = SmartBody::SBScene::getScene()->getSimulationManager()->getTime();
	smoothAngVel = 0.f;
	smoothVel = SrVec();
}

MeCtParamAnimation::~MeCtParamAnimation()
{
	reset();
}

void MeCtParamAnimation::controller_map_updated()
{
}

SkChannelArray& MeCtParamAnimation::controller_channels()
{
	return channels;
}

double MeCtParamAnimation::controller_duration()
{
	return -1;
}


bool MeCtParamAnimation::isInTransition()
{
	if (transitionManager && transitionManager->active)
	{
		return true;
	}
	
	return false;
}

bool MeCtParamAnimation::controller_evaluate(double t, MeFrameData& frame)
{	
	double timeStep = t - prevGlobalTime;
	prevGlobalTime = t;

	// auto scheduling
	autoScheduling(t);
	
	// make sure there is always a pseudo idle state running in the system
	if ((!curStateData && !nextStateData) ||
		!curStateData)
	{
		if (!hasPABlend(PseudoIdleState))
		{			
			std::vector<double> weights;
			schedule(NULL, weights);
			return true;
		}
	}	
	if (curStateData)
	{
		if (curStateData->state && // the state exist
			curStateData->state->stateName != PseudoIdleState &&  // not a idle state
			nextStateData == NULL && (curStateData->wrapMode == PABlendData::Once) && // only played once
			curStateData->timeManager->getNormalizeLocalTime() >= (curStateData->timeManager->getDuration() - curStateData->transitionLength)) // about to finish
		{			
			if (!hasPABlend(PseudoIdleState))
			{
				//LOG("no current state, and the state is about to finish, scedule NULL state");
				std::vector<double> weights;
				ScheduleType scType;
				scType.transitionLen = curStateData->transitionLength;
				schedule(NULL, weights, scType);
			}
		}
			
	}
	//------

	// dealing with transition
	
	if (transitionManager)
	{
		if (curStateData == NULL || nextStateData == NULL)
		{
			std::string errorInfo;
			errorInfo = character->getName() + "'s animation state transition warning. ";
			if (curStateData)
				if (curStateData->state)
					errorInfo += "current state: " + curStateData->state->stateName;
			else
				errorInfo += "current state: null ";
			if (nextStateData)
				if (nextStateData->state)
					errorInfo += "next state: " + nextStateData->state->stateName;
			else 
				errorInfo += "next state: null ";
			LOG("%s", errorInfo.c_str());

			if (curStateData == NULL && nextStateData != NULL)
			{
				if (nextStateData->state)
					LOG("would start state %s now", nextStateData->state->stateName.c_str());
				curStateData = nextStateData;
				curStateData->active = true;
				nextStateData = NULL;
				delete transitionManager;
				transitionManager = NULL;
				return true;
			}
			if (curStateData != NULL && nextStateData == NULL)
			{
				LOG("scheduling problem, please check the corresponding time marks for two states.");
				reset();
				return false;
			}
		}

		if (!transitionManager->blendingMode)
		{
			//LOG("Transition align");
			transitionManager->align(curStateData, nextStateData);
		}
		else
		{
			if (transitionManager->active)
			{
				//LOG("Start transition");
				SrBuffer<float> buffer1;
				buffer1.size(frame.buffer().size());
				buffer1 = frame.buffer();
				SrBuffer<float> buffer2;
				buffer2.size(frame.buffer().size());
				buffer2 = frame.buffer();
				SrMat transformMat;		

				SrMat curBaseMat;
				SrMat nextBaseMat;
				double playSpeed = max(curStateData->playSpeed, nextStateData->playSpeed);
				bool directPlay = (curStateData->directPlay || nextStateData->directPlay);
				bool pseudoTransition = (curStateData->getStateName() == PseudoIdleState || nextStateData->getStateName() == PseudoIdleState);				
				if (pseudoTransition && !directPlay && (!curStateData->isPartialBlending() && !nextStateData->isPartialBlending())) // first time && not partial blending case
				{
					bool transitionIn = (curStateData->getStateName() == PseudoIdleState);
					curStateData->evaluateTransition(timeStep * transitionManager->getSlope(), buffer1, transitionIn);
					nextStateData->evaluateTransition(timeStep, buffer2, transitionIn);
					curBaseMat = curStateData->woManager->getBaseTransformMat();
					nextBaseMat = nextStateData->woManager->getBaseTransformMat();

					if (!transitionIn) // update base rotation & translation
					{						
						SrMat origBase = curStateData->woManager->getBaseMatFromBuffer(buffer1);//combineMat(curStateData->woManager->getBaseMatFromBuffer(buffer1),curBaseMat);
						SrMat newBuffBase = origBase;
						newBuffBase = origBase*nextStateData->woManager->getCurrentBaseTransformMat();
						curStateData->woManager->setBufferByBaseMat(newBuffBase,buffer1);	
						curBaseMat = SrMat();//nextBaseMat.inverse();//SrMat();//curBaseMat*nextStateData->woManager->getBaseTransformMat().inverse();
						nextBaseMat = SrMat();//nextBaseMat.inverse();//SrMat();//nextBaseMat.inverse();
					}

					if (!transitionManager->startTransition)
					{
						SrVec pos;
						if (transitionIn)
						{
							// update base transform mat
							curBaseMat = curStateData->woManager->getCurrentBaseTransformMat();
							pos = curBaseMat.get_translation();
							//LOG("startTransistion In, curBase = %f %f %f",pos[0],pos[1],pos[2]);
						}
						else
						{							
							curBaseMat = curBaseMat*nextStateData->woManager->getCurrentBaseTransformMat().inverse();//SrMat();//nbMat.inverse();							
							pos = curBaseMat.get_translation();
							SrVec origPos = nextStateData->woManager->getCurrentBaseTransformMat().get_translation();
						}
						transitionManager->startTransition = true;
					}					
				}
				else // proceed as usual						
				{
					curStateData->evaluate(timeStep * transitionManager->getSlope(), buffer1);
					nextStateData->evaluate(timeStep, buffer2);
					curBaseMat = curStateData->woManager->getBaseTransformMat();
					nextBaseMat = nextStateData->woManager->getBaseTransformMat();
				}			
				
				if (directPlay)
				{
					// clean up base mat if cur or next state is pseudo idle state
					if (curStateData->getStateName() == PseudoIdleState)
					{
						curBaseMat = SrMat::id;												
					}
					else if (nextStateData->getStateName() == PseudoIdleState)
					{
						nextBaseMat = SrMat::id;							
					}
				}
			
				transitionManager->blending(frame.buffer(), buffer1, buffer2, transformMat, curBaseMat, nextBaseMat, timeStep, _context, directPlay);
				transitionManager->step(timeStep*playSpeed);

				// a hack to also transition the IK post-processing results from/to pseudo-idle pose
				float currentTrajBlendWeight = character->getJointTrajBlendWeight();
				bool useJointConstraint = character->getUseJointConstraint();
				if (curStateData->getStateName() == PseudoIdleState && !useJointConstraint) // transition from idle pose, and poseture doesn't provide IK traj
				{	
					character->setJointTrajBlendWeight(1.f - transitionManager->getCurrentTransitionWeight());										
					updateJointTrajectory(nextStateData);	
				}
				else if (nextStateData->getStateName() == PseudoIdleState && !useJointConstraint) // transition to idle pose, and poseture doesn't provide IK traj
				{
					character->setJointTrajBlendWeight(transitionManager->getCurrentTransitionWeight());	
					updateJointTrajectory(curStateData);	
				}		
				else // blend the joint trajectory from current state to next state
				{
					std::vector<SrVec> poseStatePosList; 
					std::vector<SrVec> curStatePosList, nextStatePostList;

					getCharacterJointTrajectory(poseStatePosList);
				
					if (curStateData->getStateName() != PseudoIdleState)
					{
						updateJointTrajectory(curStateData);
						getCharacterJointTrajectory(curStatePosList);
					}										
					else
					{
						curStatePosList = poseStatePosList;
					}
					
					if (nextStateData->getStateName() != PseudoIdleState)
					{
						updateJointTrajectory(nextStateData);
						getCharacterJointTrajectory(nextStatePostList);
					}
					else
					{
						nextStatePostList = poseStatePosList;
					}
					

					std::vector<std::string> jointConsNames = character->getJointConstraintNames();
					float transitionWeight = transitionManager->getCurrentTransitionWeight();
					for (unsigned int i=0;i<jointConsNames.size();i++)
					{
						SmartBody::TrajectoryRecord* traj = character->getJointTrajectoryConstraint(jointConsNames[i]);
						traj->jointTrajGlobalPos = nextStatePostList[i]*(1.f-transitionWeight) + curStatePosList[i]*transitionWeight;
					}
				}


				if (!curStateData->isPartialBlending() && !nextStateData->isPartialBlending())
					updateWo(transformMat, woWriter, frame.buffer());
				return true;
			}
			else // finish transition
			{
				//if (curStateData->getStateName() == PseudoIdleState)
				//	character->setJointTrajBlendWeight(1.f);
				//else if (nextStateData->getStateName() == PseudoIdleState)
				//	character->setJointTrajBlendWeight(0.f);

				delete transitionManager;
				transitionManager = NULL;
				delete curStateData;
				curStateData = nextStateData;
				nextStateData = NULL;
			}			
		}
	} 
	

	// if there's only one state, update current state
	if (curStateData && curStateData->state && curStateData->state->stateName != PseudoIdleState)
	{
		///////////////////////////////////////////////////////////////////////
		// check automatic transitions
		SmartBody::SBScene* scene = SmartBody::SBScene::getScene();
		SmartBody::SBAnimationBlendManager* blendManager = scene->getBlendManager();
		PABlend* blend = curStateData->state;
		SmartBody::SBAnimationBlend* sbstate = dynamic_cast<SmartBody::SBAnimationBlend*>(blend);
		std::vector<std::string> blends = blendManager->getTransitionBlends(blend->stateName);
		for (std::vector<std::string>::iterator iter = blends.begin();
			 iter != blends.end();
			 iter++)
		{
			SmartBody::SBAnimationTransition* transition = blendManager->getTransition(blend->stateName, (*iter));
			if (transition)
			{
				SmartBody::SBAnimationTransitionRule* rule = transition->getTransitionRule();
				if (rule)
				{
					SmartBody::SBCharacter* sbchar = dynamic_cast<SmartBody::SBCharacter*>(character);
					SmartBody::SBAnimationBlend* sbblend = dynamic_cast<SmartBody::SBAnimationBlend*>(blend);
					bool ret = rule->check(sbchar, sbblend);
					if (ret)
					{
						schedule(transition->getDestinationBlend(), 0, 0, 0);
					}
				}

			}
		}
		///////////////////////////////////////////////////////////////////////

		
		if (curStateData->active)
		{
			curStateData->evaluate(timeStep, frame.buffer());	
			if (!curStateData->isPartialBlending())
				updateWo(curStateData->woManager->getBaseTransformMat(), woWriter, frame.buffer());
#if 1 // disable IK post processing for now, until it is stable enough.
			if (character && character->getBoolAttribute("ikPostFix") && curStateData->getStateName().find("Locomotion") != std::string::npos)
			{
				SrMat mat = character->get_world_offset();
				updateIK(curStateData, mat, curStateData->woManager->getBaseTransformMat(), (float)timeStep,frame.buffer());
				
				character->setWorldOffset(ikScenario.ikGlobalMat);
				float woYOffset = ikScenario.ikGlobalMat.get_translation().y;

				JointChannelId baseChanID, baseBuffId;
				baseChanID.y = _context->channels().search(SbmPawn::WORLD_OFFSET_JOINT_NAME, SkChannel::YPos);	
				baseBuffId.y = _context->toBufferIndex(baseChanID.y);	
				frame.buffer()[baseBuffId.y] = woYOffset;	

					
			}

			// update IK trajectory			
			updateJointTrajectory(curStateData);		
#endif				
			return true;
		}
		else
		{
			delete curStateData;
			curStateData = NULL;
		}
	}
	return true;
}

void MeCtParamAnimation::setBaseJointName(const std::string& name)
{
	baseJointName = name;
}

const std::string& MeCtParamAnimation::getBaseJointName()
{
	return baseJointName;
}

void MeCtParamAnimation::schedule(PABlend* state, double x, double y, double z, PABlendData::WrapMode wrap, PABlendData::ScheduleMode schedule, PABlendData::BlendMode blend, std::string jName, double timeOffset, double stateTimeOffset, double stateTimeTrim, double transitionLen, bool directPlay )
{
	std::vector<double> weights;
	weights.resize(state->getNumMotions());
	SmartBody::SBAnimationBlend0D* state0D = dynamic_cast<SmartBody::SBAnimationBlend0D*>(state);
	if (state0D)
	{
		if (state->getNumMotions() > 0)
			weights[0] = 1.0f;
	}
	else
	{
		SmartBody::SBAnimationBlend1D* state1D = dynamic_cast<SmartBody::SBAnimationBlend1D*>(state);
		if (state1D)
		{
			state->getWeightsFromParameters(x, weights);
		}
		else
		{
			SmartBody::SBAnimationBlend2D* state2D = dynamic_cast<SmartBody::SBAnimationBlend2D*>(state);
			if (state2D)
			{
				state->getWeightsFromParameters(x, y, weights);
			}
			else
			{
				SmartBody::SBAnimationBlend3D* state3D = dynamic_cast<SmartBody::SBAnimationBlend3D*>(state);
				if (state3D)
				{
					state->getWeightsFromParameters(x, y, z, weights);
				}
				else
				{
					SmartBody::SBMotionBlendBase* blendBase = dynamic_cast<SmartBody::SBMotionBlendBase*>(state);					
					if (blendBase)
					{
						state->getWeightsFromParameters(x, y, z, weights);
					}
					else
					{
						LOG("Unknown state type. What is this?");
					}
				}
			}
		}
	}
	this->schedule(state, weights, wrap, schedule, blend, jName, timeOffset, stateTimeOffset, stateTimeTrim, transitionLen, directPlay);
}

void MeCtParamAnimation::schedule( PABlend* state, const std::vector<double>& weights, const ScheduleType& scType )
{
	//schedule(state,weights,scType.wrap,scType.schedule,scType.blend,scType.jName, scType.timeOffset,scType.stateTimeOffset, scType.stateTimeTrim, scType.transitionLen, scType.directPlay);
	PABlendData::WrapMode wrap = scType.wrap;
	PABlendData::ScheduleMode scheduleMode = scType.schedule;
	PABlendData::BlendMode blend = scType.blend;
	std::string jName = scType.jName;
	double timeOffset = scType.timeOffset;
	double stateTimeOffset = scType.stateTimeOffset;
	double stateTimeTrim = scType.stateTimeTrim;
	double transitionLen = scType.transitionLen;
	double playSpeed = scType.playSpeed;
	bool directPlay = scType.directPlay;
	double desireDuration = scType.duration;

	

	ScheduleUnit unit;
	SmartBody::SBAnimationBlend* animState = dynamic_cast<SmartBody::SBAnimationBlend*>(state);
	if (animState)
	{
		animState->validateState(); // to make sure the animaion state is valid before schedule it
	}

	

	// schedule
	unit.weights = weights;
	unit.data = state;
	unit.wrap = wrap;
	unit.schedule = scheduleMode;
	unit.blend = blend;
	unit.partialJoint = jName;
	unit.time = SmartBody::SBScene::getScene()->getSimulationManager()->getTime() + timeOffset;
	unit.stateTimeOffset = (float)stateTimeOffset;
	unit.stateTimeTrim = (float)stateTimeTrim;
	unit.directPlay = directPlay;
	unit.playSpeed = (float)playSpeed;
	unit.duration = (float)desireDuration;

	//LOG("unit schedule mode = %d",unit.schedule);

	float unitTransitionLen = (float)defaultTransition; // set this to initial default	
	// override if the default attribute exists
	if (character->hasAttribute("defaultTransition") && character->getDoubleAttribute("defaultTransition") >= 0)
	{
		unitTransitionLen = (float)character->getDoubleAttribute("defaultTransition");
	}
	// override if specified in the blend scheduling
	if (transitionLen >= 0)
	{
		unitTransitionLen = (float)transitionLen;
	}
	unit.transitionLength = unitTransitionLen;
	// make sure the weights are valid for non-pseudoidle state
	if (unit.weights.size() == 0 && unit.data != NULL)
	{
		if (unit.data->stateName != PseudoIdleState)
		{
			LOG("MeCtParamAnimation::schedule Warning: state %s has no weights assigned.", unit.data->stateName.c_str());
			unit.weights.resize(unit.data->getNumMotions());
			for (int i = 0; i < unit.data->getNumMotions(); i++)
			{
				if (i == 0)
					unit.weights[i] = 1.0;
				else
					unit.weights[i] = 0.0;
			}
		}
	}
	// make sure there's motion for non-pseudoidle state
	if (unit.data != NULL)
	{
		if (unit.data->stateName != PseudoIdleState && unit.data->getNumMotions() == 0)
		{
			LOG("MeCtParamAnimation::schedule ERR: state %s has no motions attached.", unit.data->stateName.c_str());
			return;
		}
	}
	waitingList.push_back(unit);
}

void MeCtParamAnimation::schedule(PABlend* blendData, const std::vector<double>& weights, PABlendData::WrapMode wrap, PABlendData::ScheduleMode scheduleMode, PABlendData::BlendMode blend, std::string jName, double timeOffset, double stateTimeOffset, double stateTimeTrim, double transitionLen, bool directPlay)
{
	// Feng : rewrite this part to use the object scType so we don't need to indefinitely change this schedule function everytime we add another paramter
	ScheduleType scType;
	scType.wrap = wrap;
	scType.schedule = scheduleMode;
	scType.blend = blend;
	scType.jName = jName;
	scType.timeOffset = timeOffset;
	scType.stateTimeOffset = stateTimeOffset;
	scType.stateTimeTrim = stateTimeTrim;
	scType.transitionLen = transitionLen;
	scType.directPlay = directPlay;

	schedule(blendData,weights,scType);

#if 0
	ScheduleUnit unit;
	SmartBody::SBAnimationBlend* animState = dynamic_cast<SmartBody::SBAnimationBlend*>(blendData);
	if (animState)
	{
		animState->validateState(); // to make sure the animaion state is valid before schedule it
	}

	// schedule
	unit.weights = weights;
	unit.data = blendData;
	unit.wrap = wrap;
	unit.schedule = scheduleMode;
	unit.blend = blend;
	unit.partialJoint = jName;
	unit.time = mcuCBHandle::singleton().time + timeOffset;
	unit.stateTimeOffset = (float)stateTimeOffset;
	unit.stateTimeTrim = (float)stateTimeTrim;
	unit.directPlay = directPlay;

	//LOG("unit schedule mode = %d",unit.schedule);
	
	float unitTransitionLen = (float)defaultTransition; // set this to initial default	
	// override if the default attribute exists
	if (character->hasAttribute("defaultTransition") && character->getDoubleAttribute("defaultTransition") >= 0)
	{
		unitTransitionLen = (float)character->getDoubleAttribute("defaultTransition");
	}
	// override if specified in the blend scheduling
	if (transitionLen >= 0)
	{
		unitTransitionLen = (float)transitionLen;
	}
	unit.transitionLength = unitTransitionLen;
	
	
	
	

	// make sure the weights are valid for non-pseudoidle state
	if (unit.weights.size() == 0 && unit.data != NULL)
	{
		if (unit.data->stateName != PseudoIdleState)
		{
			LOG("MeCtParamAnimation::schedule Warning: state %s has no weights assigned.", unit.data->stateName.c_str());
			unit.weights.resize(unit.data->getNumMotions());
			for (int i = 0; i < unit.data->getNumMotions(); i++)
			{
				if (i == 0)
					unit.weights[i] = 1.0;
				else
					unit.weights[i] = 0.0;
			}
		}
	}

	// make sure there's motion for non-pseudoidle state
	if (unit.data != NULL)
	{
		if (unit.data->stateName != PseudoIdleState && unit.data->getNumMotions() == 0)
		{
			LOG("MeCtParamAnimation::schedule ERR: state %s has no motions attached.", unit.data->stateName.c_str());
			return;
		}
	}

	waitingList.push_back(unit);
#endif
}

void MeCtParamAnimation::unschedule()
{
	reset();
}

void MeCtParamAnimation::updateWeights(std::vector<double>& w)
{
	if (curStateData == NULL)
		return;
	if (curStateData->state->getNumMotions() != w.size())
		return;

	double wCheck = 0.0;
	for (size_t i = 0; i < w.size(); i++)
		wCheck += w[i];
	if (fabs(wCheck - 1.0) > 0.1)
	{
		for (size_t i = 0; i < w.size(); i++)
			if (i == 0) w[i] = 1.0;
			else		w[i] = 0.0;
	}

	curStateData->weights = w;
	curStateData->timeManager->updateWeights();
	
	if (transitionManager)
		transitionManager->update();
}

void MeCtParamAnimation::updateWeights()
{
	if (curStateData == NULL)
		return;
	std::vector<double>& w = curStateData->weights;
	updateWeights(w);
}

int MeCtParamAnimation::getNumWeights()
{
	if (curStateData)
		return curStateData->interpolator->getNumMotions();
	else
		return 0;
}

const std::string& MeCtParamAnimation::getNextStateName()
{
	if (nextStateData)
		return nextStateData->state->stateName;
	else
		return m_emptyString;
}

const std::string& MeCtParamAnimation::getCurrentStateName()
{
	if (curStateData)
		if (curStateData->state)
			return curStateData->state->stateName;
		else
			return m_emptyString;
	else
		return m_emptyString;
}

PABlendData* MeCtParamAnimation::getCurrentPABlendData()
{
	if (curStateData)
		return curStateData;
	else
		return NULL;
}

bool MeCtParamAnimation::hasPABlend(const std::string& name)
{
	if (getCurrentStateName() == name)
		return true;
	if (getNextStateName() == name)
		return true;
	std::list<ScheduleUnit>::iterator iter = waitingList.begin();
	for (; iter!= waitingList.end(); iter++)
	{
		if (iter->data)
		{
			if (iter->data->stateName == name)
				return true;
		}
		else
		{
			if (name == PseudoIdleState)
				return true;
		}

	}
	return false;
}

bool MeCtParamAnimation::isIdle()
{
	if (getCurrentPABlendData() && 
		getCurrentPABlendData()->state &&
		getCurrentPABlendData()->state->stateName != PseudoIdleState)
		return false;
	if (getNextStateName() != "")
		return false;
	if (waitingList.size() != 0)
		return false;
	return true;
}

void MeCtParamAnimation::autoScheduling(double time)
{
	if (waitingList.size() == 0)
		return;	

	ScheduleUnit nextUnit = waitingList.front();

	if (time < nextUnit.time)
		return;
	
	if (transitionManager)
		return;

	// if current state is pseudo idle & next state is also pseudo idle, ignore
	if (curStateData != NULL)
	{
		if (curStateData->getStateName() == PseudoIdleState)
		{
			if (nextUnit.data == NULL)
			{
				waitingList.pop_front();
				return;
			}
			else
				if (nextUnit.data->stateName == PseudoIdleState)
				{
					waitingList.pop_front();
					return;
				}
		}
	}

	if (curStateData == NULL)
	{
		if (nextStateData)
			delete nextStateData;
		nextStateData = NULL;
		curStateData = createStateModule(nextUnit);
		if (!curStateData)
		{
			return;
		}
		curStateData->active = true;
		waitingList.pop_front();
	}
	else
	{
		if (transitionManager)
			delete transitionManager;
		SmartBody::SBAnimationTransition* data = NULL;
		if (nextUnit.data)
			data = SmartBody::SBScene::getScene()->getBlendManager()->getTransition(curStateData->state->stateName, nextUnit.data->stateName);
		nextStateData = createStateModule(nextUnit);
		nextStateData->active = false;
		if (!data) // no existing transition data
		{
			if (curStateData->state->stateName == PseudoIdleState)
				transitionManager = new PATransitionManager(nextStateData->transitionLength);
			else
			{
				double transitionLen = nextStateData->transitionLength;				
				if (nextStateData->scheduleMode == PABlendData::Now)
				{
					transitionManager = new PATransitionManager((float)transitionLen);					
				}
				else
				{
					// check to see if the current local time cannot afford the defaultTransition time					
					double actualTransitionTime = transitionLen;
					if (curStateData->timeManager->getNormalizeLocalTime() >= (curStateData->timeManager->getDuration() - transitionLen))
						actualTransitionTime = curStateData->timeManager->getDuration() - curStateData->timeManager->getNormalizeLocalTime();
					double easeOutStart = curStateData->timeManager->getDuration() - actualTransitionTime;
					double easeOutDur = actualTransitionTime;
					//LOG("easeOutStart = %f, duration = %f",easeOutStart, easeOutDur);
					transitionManager = new PATransitionManager(easeOutStart, easeOutDur);					
					//transitionManager = new PATransitionManager(curStateData->timeManager->getDuration(), 0.0);
				}
			}
		}
		else
		{
			//LOG("state transition from = %s, to = %s, easeOut = %f, easeIn = %f",curStateData->getStateName().c_str(), nextStateData->getStateName().c_str(),data->easeOutStart[0], data->easeInStart);
			transitionManager = new PATransitionManager(data, curStateData, nextStateData);			
			nextStateData->timeManager->updateLocalTimes(transitionManager->s2);
		}
		waitingList.pop_front();
	}
}

PABlendData* MeCtParamAnimation::createStateModule(ScheduleUnit su)
{
	PABlendData* module = NULL;
	if (su.data)
	{
		module = new PABlendData(su.data, su.weights, su.blend, su.wrap, su.schedule, su.stateTimeOffset, su.stateTimeTrim, su.directPlay);		
		SmartBody::SBAnimationBlend* animBlend = dynamic_cast<SmartBody::SBAnimationBlend*>(su.data);
		if (animBlend) // set retarget information if it is available
		{
			SmartBody::SBRetargetManager* retargetManager = SmartBody::SBScene::getScene()->getRetargetManager();			
			SmartBody::SBRetarget* retarget = retargetManager->getRetarget(animBlend->getBlendSkeleton(),character->getSkeleton()->getName());
			if (retarget && character->getBoolAttribute("onlineRetarget"))
				module->retarget = retarget;
			else
				module->retarget = NULL;
		}

		module->blendStartOffset = su.stateTimeOffset;
		module->blendEndTrim = su.stateTimeTrim;
		module->transitionLength = su.transitionLength;
		module->playSpeed = su.playSpeed;
		module->baseJointName = baseJointName;
		if (su.duration > 0.f) // the user set a state duration, adjut playSpeed
		{
			module->playSpeed = float(module->timeManager->getDuration()/su.duration);
		}
		std::vector<std::string> joints;
		SkJoint* j = character->getSkeleton()->search_joint(su.partialJoint.c_str());
		if (j)
		{
			std::vector<SkJoint*> jVec;
			SkJoint::recursive_children(jVec, j);
			for (size_t i = 0; i < jVec.size(); i++)
			{
				joints.push_back(jVec[i]->jointName());
			}
		}
		module->interpolator->setBlendingJoints(joints);
	}
	else
	{
		module = new PABlendData(PseudoIdleState, su.weights, su.blend, su.wrap, su.schedule, su.stateTimeOffset, su.stateTimeTrim, su.directPlay);
		module->transitionLength = su.transitionLength;
	}
	if (_context)
	{
		module->interpolator->setMotionContextMaps(_context);
		module->interpolator->initChanId(_context, baseJointName);
		SkJoint* baseJoint = character->getSkeleton()->search_joint(baseJointName.c_str());
		if (!baseJoint)
			return NULL;		
		module->interpolator->initPreRotation(baseJoint->quat()->orientation()*baseJoint->quat()->prerot());
		module->woManager->setMotionContextMaps(_context);
		module->woManager->initChanId(_context, baseJointName);		
		//SrQuat preRot = character->getSkeleton()->search_joint(baseJointName.c_str())->quat()->prerot();
		SrQuat preRot = baseJoint->quat()->orientation()*baseJoint->quat()->prerot();
		module->woManager->initPreRotation(preRot);
	}
	else
		return NULL;
	return module;
}

void MeCtParamAnimation::reset()
{
	if (curStateData)
		delete curStateData;
	curStateData = NULL;
	if (nextStateData)
		delete nextStateData;
	nextStateData = NULL;
	if (transitionManager)
		delete transitionManager;
	transitionManager = NULL;
	waitingList.clear();
}

void MeCtParamAnimation::updateWo(SrMat& mat, MeCtChannelWriter* woWriter, SrBuffer<float>& buffer)
{
	// get current woMat
	SrBuffer<float>& woValue = woWriter->get_data();
	SrMat currentWoMat;
	SrQuat quat;
	quat.w = woValue[3];
	quat.x = woValue[4];
	quat.y = woValue[5];
	quat.z = woValue[6];
	quat.get_mat(currentWoMat);
	currentWoMat.set(12, woValue[0]);
	currentWoMat.set(13, woValue[1]);
	currentWoMat.set(14, woValue[2]);	

	// separate transform matrix to rotation and transltion matrix
	SrQuat q = SrQuat(mat);
	SrMat rot;
	q.get_mat(rot);
	SrMat translate;
	translate.set(12, mat.get(12));
	translate.set(14, mat.get(14));

	// apply rotation and transition matrix perspectively
	SrQuat woQuat = SrQuat(currentWoMat);
	SrMat currentRot;
	woQuat.get_mat(currentRot);		

	SkJoint* baseJ = character->_skeleton->search_joint(baseJointName.c_str());
	SrVec offset;
	if (baseJ) offset = baseJ->offset(); offset.y = 0.f;
	SrMat negOffset; negOffset.set_translation(-offset);
	SrMat posOffset; posOffset.set_translation(offset);
	//SrMat newWoMat =  rot * currentRot;
	//SrMat newTranslate = translate * currentRot;	
	//SrMat newTranslate = translate * newWoMat;	
	SrMat nextMat = negOffset * mat * posOffset * currentWoMat;
	SrVec newTran = nextMat.get_translation();

	// set new wo mat back to skeleton and woWriter
	SrVec bufferTran = newTran;
	quat = SrQuat(nextMat);
	woValue[3] = quat.w;
	woValue[4] = quat.x;
	woValue[5] = quat.y;
	woValue[6] = quat.z;
	woValue[0] = bufferTran[0];
	woValue[1] = bufferTran[1];
	woValue[2] = bufferTran[2];
	gwiz::quat_t skelQ = gwiz::quat_t(woValue[3], woValue[4], woValue[5], woValue[6]);
	gwiz::euler_t skelE = gwiz::euler_t(skelQ);	
	character->set_world_offset(woValue[0], woValue[1], woValue[2], (float)skelE.h(), (float)skelE.p(), (float)skelE.r());

	JointChannelId baseChanID, baseBuffId;
	baseChanID.x = _context->channels().search(SbmPawn::WORLD_OFFSET_JOINT_NAME, SkChannel::XPos);
	baseChanID.y = _context->channels().search(SbmPawn::WORLD_OFFSET_JOINT_NAME, SkChannel::YPos);
	baseChanID.z = _context->channels().search(SbmPawn::WORLD_OFFSET_JOINT_NAME, SkChannel::ZPos);
	baseChanID.q = _context->channels().search(SbmPawn::WORLD_OFFSET_JOINT_NAME, SkChannel::Quat);

	baseBuffId.x = _context->toBufferIndex(baseChanID.x);
	baseBuffId.y = _context->toBufferIndex(baseChanID.y);
	baseBuffId.z = _context->toBufferIndex(baseChanID.z);	
	baseBuffId.q = _context->toBufferIndex(baseChanID.q);	

	buffer[baseBuffId.x] = bufferTran[0];
	buffer[baseBuffId.y] = bufferTran[1];
	buffer[baseBuffId.z] = bufferTran[2];	
	for (int k = 0; k < 4; k++)
		buffer[baseBuffId.q + k] = quat.getData(k);
}

void MeCtParamAnimation::updateIK( PABlendData* curBlendData, SrMat& woMat, SrMat& woDeltaMat, float dt, SrBuffer<float>& buff )
{
	SmartBody::SBAnimationBlend* blend = dynamic_cast<SmartBody::SBAnimationBlend*>(curBlendData->state);
	if (!blend) return;
	MotionAnalysis* moAnalysis = blend->getMotionAnalysis();
	if (!moAnalysis) return;
	
	if (ikScenario.ikTreeNodes.size() == 0) // initialize ik nodes
	{
		std::vector<std::string> stopJoint;
		stopJoint.push_back("spine1");
		ikScenario.buildIKTreeFromJointRoot(character->getSkeleton()->getJointByMappedName("base"),stopJoint);
	}	
	
	float rotSpeed = SrQuat(woDeltaMat.get_rotation()).axisAngle().y*180.f/(dt*(float)M_PI);
	SrVec deltaTrans = woDeltaMat.get_translation()*woMat.get_rotation(); // rotate translation to global frame
	deltaTrans.y = 0.f; 
	SrVec velocity = deltaTrans/dt;		
	float smoothWeight = dt*10.f;
	smoothAngVel = smoothAngVel*(1.f-smoothWeight) + rotSpeed*smoothWeight;
	smoothVel = smoothVel*(1.f-smoothWeight) + velocity*smoothWeight;

	updateMotionFrame(inputFrame, ikScenario, buff, true); // read data from frame

#ifdef EMBER_SB_STEER
	//moAnalysis->applyIKFix(ikScenario, character, curBlendData->weights, curBlendData->timeManager, woMat, velocity, rotSpeed, inputFrame, outputFrame);
	moAnalysis->applyIKFix(ikScenario, character, curBlendData->weights, curBlendData->timeManager, woMat, smoothVel, smoothAngVel, inputFrame, outputFrame);

	updateMotionFrame(outputFrame, ikScenario, buff, false);	
#endif
}

void MeCtParamAnimation::updateJointTrajectory( PABlendData* blendData )
{	
	std::vector<std::string> jointConsNames = character->getJointConstraintNames();
	SmartBody::SBJoint* baseJoint = character->getSkeleton()->getJointByMappedName("base");
	SrMat baseGmat;
	character->getSkeleton()->update_global_matrices();
	if (baseJoint && baseJoint->getParent())
	{		
		baseGmat = baseJoint->gmat();//baseJoint->getParent()->gmat();		
	}
	SmartBody::SBScene* scene = SmartBody::SBScene::getScene();

	SmartBody::SBAnimationBlend* animBlend = dynamic_cast<SmartBody::SBAnimationBlend*>(blendData->state);

	SmartBody::SBRetarget* retarget = NULL;
	if (animBlend)
	{
		SmartBody::SBRetargetManager* retargetManager = SmartBody::SBScene::getScene()->getRetargetManager();			
		retarget = retargetManager->getRetarget(animBlend->getBlendSkeleton(),character->getSkeleton()->getName());
	}
	
	if (!retarget)
		return;

	for (unsigned int i=0;i<jointConsNames.size();i++)
	{
		SmartBody::TrajectoryRecord* trajRecord = character->getJointTrajectoryConstraint(jointConsNames[i]);
		if (!trajRecord)
			continue;

		SmartBody::SBJoint* refJoint = character->getSkeleton()->getJointByName(trajRecord->refJointName);
		if (!refJoint)
			continue;			
		SrVec trajOffset;
		bool hasTraj = blendData->getTrajPosition(jointConsNames[i],0.f,trajOffset);
		if (!hasTraj)
		{	
			trajRecord->isEnable = false;
			continue;
		}
		trajRecord->isEnable = true;
		trajRecord->jointTrajLocalOffset = trajOffset;
		trajRecord->refJointGlobalPos = baseGmat.get_translation();//refJoint->gmat().get_translation(); 
		if (retarget)
			retarget->applyRetargetJointTrajectory(*trajRecord,baseGmat);			
	}
}

void MeCtParamAnimation::updateMotionFrame( BodyMotionFrame& motionFrame, MeCtIKTreeScenario& ikScenario, SrBuffer<float>& buff, bool readData /*= true*/ )
{
	IKTreeNodeList& nodeList = ikScenario.ikTreeNodes;
	if (motionFrame.jointQuat.size() != nodeList.size())
		motionFrame.jointQuat.resize(nodeList.size());

	MeCtIKTreeNode* rootNode = ikScenario.ikTreeRoot;
	if (rootNode)
	{
		int chanId[3];
		int buffId[3];
		chanId[0] = _context->channels().search(rootNode->getNodeName().c_str(), SkChannel::XPos);
		chanId[1] = _context->channels().search(rootNode->getNodeName().c_str(), SkChannel::YPos);
		chanId[2] = _context->channels().search(rootNode->getNodeName().c_str(), SkChannel::ZPos);
		for (int k=0;k<3;k++)
		{
			if (chanId[k] < 0)
				continue;
			buffId[k] = _context->toBufferIndex(chanId[k]);
			if (buffId[k] < 0)
				continue;
			if (readData)
				motionFrame.rootPos[k] = buff[buffId[k]];
			else
				buff[buffId[k]] = motionFrame.rootPos[k];
		}		
	}

	for (unsigned int i=0;i<nodeList.size();i++)
	{
		MeCtIKTreeNode* node = nodeList[i];
		JointChannelId chanId;
		JointChannelId buffId;
		chanId.q = _context->channels().search(node->getNodeName().c_str(), SkChannel::Quat);
		if (chanId.q < 0)
			continue;
		buffId.q = _context->toBufferIndex(chanId.q);
		if (buffId.q < 0)
			continue;
		SrQuat& origQ = motionFrame.jointQuat[i];
		if (readData)
		{
			origQ.w = buff[buffId.q + 0];
			origQ.x = buff[buffId.q + 1];
			origQ.y = buff[buffId.q + 2];
			origQ.z = buff[buffId.q + 3];
		}
		else
		{
			buff[buffId.q + 0] = origQ.w ;
			buff[buffId.q + 1] = origQ.x;
			buff[buffId.q + 2] = origQ.y;
			buff[buffId.q + 3] = origQ.z;
		}
	}	
}


SrMat MeCtParamAnimation::combineMat( SrMat& mat1, SrMat& mat2 )
{
	SrMat newMat = mat1*mat2;
	SrVec newTranslation = mat1.get_translation() + mat2.get_translation();
	newMat.set_translation(newTranslation);
	return newMat;	
}

void MeCtParamAnimation::getCharacterJointTrajectory( std::vector<SrVec> &curStatePosList )
{
	std::vector<std::string> jointConsNames = character->getJointConstraintNames();					
	for (unsigned int i=0;i<jointConsNames.size();i++)
	{
		SmartBody::TrajectoryRecord* traj = character->getJointTrajectoryConstraint(jointConsNames[i]);
		curStatePosList.push_back(traj->jointTrajGlobalPos);
	}
}