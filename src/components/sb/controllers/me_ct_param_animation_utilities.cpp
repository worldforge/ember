/*
 *  me_ct_param_animation_utilities.cpp - part of Motion Engine and SmartBody-lib
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

#include "controllers/me_ct_param_animation_utilities.h"
#include <sbm/gwiz_math.h>

#include <sr/sr_euler.h>
#include <sb/SBAnimationState.h>
#include <sb/SBAnimationStateManager.h>
#include <sb/SBEvent.h>
#include <sb/SBScene.h>
#include <sb/SBMotion.h>
#include <math.h>

const double timeThreshold = 0.05;

#define NEW_EULER 1

PATimeManager::PATimeManager()
{
}

PATimeManager::PATimeManager(PABlendData* data)
{
	blendData = data;

	if (!blendData->state)
		return;

	if (blendData->state->keys.size() > 0)
	{
		if (blendData->state->keys[0].size() > 0)
			for (size_t i = 0; i < blendData->state->keys.size(); i++)
				//localTimes.push_back(blendData->state->keys[i][0]);
				localTimes.push_back(blendData->getStateKeyTime(i,0));
	}
	else
	{
		SmartBody::SBAnimationBlend0D* state0D = dynamic_cast<SmartBody::SBAnimationBlend0D*>(blendData->state);
		if (state0D)
		{
			//LOG("state 0D, use blend offset");
			localTimes.push_back(blendData->blendStartOffset);			
		}
		else
		{
			LOG("State %s has no keys, setting all local times to zero.", blendData->state->stateName.c_str());
			for (int x = 0; x < blendData->state->getNumMotions(); x++)
			{
				localTimes.push_back(0);
			}

		}
	}
	if (blendData->weights.size() > localTimes.size())
	{
		LOG("Problem: fewer keys (%d) than weights (%d)", blendData->state->keys.size(), blendData->weights.size());
	}
	setMotionTimes();
	setKey();
	setLocalTime();
	loadEvents();
}

PATimeManager::~PATimeManager()
{
}

int PATimeManager::getNumKeys()
{
	return key.size();
}

void PATimeManager::updateLocalTimes(double time)
{
	localTime = time;
	prevLocalTime = localTime;
	getParallelTimes(localTime, localTimes);
	setMotionTimes();
}


bool PATimeManager::step(double timeStep)
{
	bool notReachDuration = true;
	std::vector<double> prevMotionTimes;
	prevMotionTimes.resize(motionTimes.size());
	std::copy(motionTimes.begin(),motionTimes.end(),prevMotionTimes.begin());

	prevLocalTime = localTime;
	double newLocalTime = localTime + timeStep;
	int loopcounter = 0;
	double lastKey = 0;
	//double keyDuration = 0.0;
	if (blendData->state->keys.size() > 0)
	{
		lastKey = key[key.size() - 1];
		//keyDuration = key[key.size()-1] - key[0];
	}
	
	if (blendData->state->keys.size() > 0 && 
	//	(newLocalTime > keyDuration && keyDuration >= 0.0))	
		(newLocalTime > lastKey && lastKey >= 0.0))	
	{	
		// loop back 
		if (blendData->wrapMode == PABlendData::Loop)
		{
			double d = key[key.size() - 1] - key[0];		
			// consider the case when key[0] > 0
			double offsetLocalTime = newLocalTime - key[0];
			int times = (int) (offsetLocalTime / d);
			if (times > 0)
			{
				offsetLocalTime = offsetLocalTime - float(times) * d;
				loadEvents();
			}
			newLocalTime = offsetLocalTime + key[0];
		}		
		notReachDuration = false;
	}

	
	localTime = newLocalTime;
	getParallelTimes(localTime, localTimes);
	if (localTimes.size() == 0)
	{
		LOG("PATimeManager::step ERROR: Miss the timing.");
		return notReachDuration;
	}

	int numMotionTimes = motionTimes.size();
	setMotionTimes();
	checkEvents();
	if (motionTimes.size() == numMotionTimes)
	{
		timeDiffs.clear();
		for (size_t i = 0; i < blendData->weights.size(); i++)
			timeDiffs.push_back(motionTimes[i] - prevMotionTimes[i]);
	}
	else
	{
		LOG("Motion times do not match! Please check!");
	}
	//LOG("newLocal time = %f, lastKey = %f, notReachDuration = %d",newLocalTime, lastKey,notReachDuration);

	return notReachDuration;
}


void PATimeManager::loadEvents()
{
	// add the event instances to this controller
	while (!_events.empty())
		_events.pop();


	std::vector<std::pair<SmartBody::SBMotionEvent*, int> >& events = blendData->state->getEvents();
	for (size_t x = 0; x < events.size(); x++)
	{
		_events.push(events[x]);
	}
}

void PATimeManager::checkEvents()
{
	int motionIndex = 0;
	if (motionIndex >= (int) localTimes.size())
		return;

	while (!_events.empty())
	{
		std::pair<SmartBody::SBMotionEvent*, int>& event = _events.front();
		motionIndex = event.second;
		// localTime is the parameterized time, determine the local time of the event
		if (event.first->isEnabled() && localTimes[motionIndex] >= event.first->getTime())
		{
			SmartBody::SBEventManager* manager = SmartBody::SBScene::getScene()->getEventManager();
			manager->handleEvent(event.first, localTimes[motionIndex]);
			std::string type = event.first->getType();
			std::string params = event.first->getParameters();
			//LOG("EVENT: %f %s %s", time, type.c_str(), params.c_str());
			_events.pop();
		}
		else
		{
			return;
		}
	}
}


void PATimeManager::updateWeights()
{
	setKey();
	setLocalTime();
}

double PATimeManager::getDuration()
{
	return (key[getNumKeys() - 1] - key[0]);
}

double PATimeManager::getNormalizeLocalTime()
{
	return localTime - key[0];
}

double PATimeManager::getPrevNormalizeLocalTime()
{
	return prevLocalTime - key[0];
}

void PATimeManager::setKey()
{
	key.clear();
	int numKeys = 0;
	if (blendData->weights.size() > 0) numKeys = blendData->state->keys[0].size();
	for (int i = 0; i < numKeys; i++)
	{
		double tempK = 0.0;
		for (size_t j = 0; j < blendData->weights.size(); j++)
			tempK += blendData->weights[j] * blendData->getStateKeyTime(j,i);//blendData->state->keys[j][i];
		key.push_back(tempK);
	}
}

void PATimeManager::setLocalTime()
{
	if (localTimes.size() < blendData->weights.size())
	{
		LOG("Problem with number of local times (%d) versus number of weights (%d)", localTimes.size(), blendData->weights.size());
		return;
	}
	localTime = 0.0;
	for (size_t i = 0; i < blendData->weights.size(); i++)
		localTime += blendData->weights[i] * localTimes[i];
	prevLocalTime = localTime;
}

void PATimeManager::setMotionTimes()
{
	motionTimes.clear();
	if (localTimes.size() > 0)
	{
		for (size_t i = 0; i < blendData->weights.size(); i++)
		{	
			// this is just to substract any key times that are larger than motion duration, not to deal with the looping. 
			int d = (int) (localTimes[i] / blendData->state->motions[i]->duration());
			motionTimes.push_back(localTimes[i] - d * blendData->state->motions[i]->duration());			
			//int d = (int) (localTimes[i] / blendData->getStateMotionDuration(i));
			//motionTimes.push_back(localTimes[i] - d * blendData->getStateMotionDuration(i));
			//motionTimes.push_back(localTimes[i]);
		}
	}
	
}

int PATimeManager::getSection(double time)
{
	for (int i = 0; i < getNumKeys() - 1; i++)
	{
		if (key[i] <= time && key[i + 1] > time)
			return i;
	}
	if (time > key[getNumKeys()-1])
		return getNumKeys();
	return -1;
}



void PATimeManager::getParallelTimes(double time, std::vector<double>& times)
{
	times.clear();
	double offsetTime = time;// + blendData->blendStartOffset;
	int section = getSection(offsetTime);
	//LOG("localtime = %f, section = %d",offsetTime,section);
	if (section < 0)
	{
		for (size_t i = 0; i < blendData->weights.size(); i++)
		{
			times.push_back(0);
		}
		return;
	}
	else if (section >= getNumKeys())
	{
		for (size_t i = 0; i < blendData->weights.size(); i++)
		{
			times.push_back(blendData->getStateKeyTime(i,getNumKeys()-1));
		}
		return;
	}
	
	for (size_t i = 0; i < blendData->weights.size(); i++)
	{
		double t = blendData->getStateKeyTime(i,section) + (blendData->getStateKeyTime(i,section+1) - blendData->getStateKeyTime(i,section)) * (offsetTime - key[section]) / (key[section + 1] - key[section]);
		times.push_back(t);
	}
}

std::vector<double>& PATimeManager::getKey()
{
	return key;
}

PAMotions::PAMotions()
{
}

PAMotions::PAMotions(PABlendData* data)
{
	blendData = data;
}

PAMotions::~PAMotions()
{
}


void PAMotions::setMotionContextMaps(MeControllerContext* context)
{
	SkChannelArray& cChannels = context->channels();
	if (!blendData->state)
		return;
	for (size_t mId = 0; mId < blendData->state->motions.size(); mId++)
	{
		SkChannelArray& mChannels = blendData->state->motions[mId]->channels();
		const int size = mChannels.size();
		SrBuffer<int> map;
		map.size(size);
		for (int i = 0; i < size; i++)
		{
			int chanIndex = cChannels.search(mChannels.mappedName(i), mChannels.type(i));
			map[i] = context->toBufferIndex(chanIndex);
		}
		motionContextMaps.push_back(map);
	}
	_context = context;
}

int PAMotions::getNumMotions()
{
	if (!blendData->state)
		return 0;
	return blendData->state->motions.size();
}


void PAMotions::initChanId(MeControllerContext* context, std::string baseJointName)
{
	SkChannelArray& cChannels = context->channels();

	// base joint
	baseChanId.x = cChannels.search(baseJointName.c_str(), SkChannel::XPos);
	baseChanId.y = cChannels.search(baseJointName.c_str(), SkChannel::YPos);
	baseChanId.z = cChannels.search(baseJointName.c_str(), SkChannel::ZPos);
	baseChanId.q = cChannels.search(baseJointName.c_str(), SkChannel::Quat);
	baseBuffId.x = context->toBufferIndex(baseChanId.x);
	baseBuffId.y = context->toBufferIndex(baseChanId.y);
	baseBuffId.z = context->toBufferIndex(baseChanId.z);
	baseBuffId.q = context->toBufferIndex(baseChanId.q);
}

void PAMotions::initPreRotation(const SrQuat& q)
{
	basePrerot.w = q.w;
	basePrerot.x = q.x;
	basePrerot.y = q.y;
	basePrerot.z = q.z;
}


void PAMotions::getBuffer(SkMotion* motion, double t, SrBuffer<int>& map, SrBuffer<float>& buff)
{
	double deltaT = motion->duration() / double(motion->frames() - 1);
	int lastFrame = int (t/deltaT);
	//motion->apply(float(t), &buff[0], &map, SkMotion::Linear, &lastFrame, false, blendData->retarget);
	motion->apply(float(t), &buff[0], &map, SkMotion::Linear, &lastFrame, false);
}


SrMat PAMotions::getBaseMatFromBuffer(SrBuffer<float>& buffer)
{
	SrQuat quat;
	quat.w = buffer[baseBuffId.q + 0];
	quat.x = buffer[baseBuffId.q + 1];
	quat.y = buffer[baseBuffId.q + 2];
	quat.z = buffer[baseBuffId.q + 3];
	SrMat mat;
	quat.get_mat(mat);
	mat.set(12, buffer[baseBuffId.x]);
	mat.set(13, buffer[baseBuffId.y]);
	mat.set(14, buffer[baseBuffId.z]);
	return mat;
}

void PAMotions::setBufferByBaseMat(SrMat& mat, SrBuffer<float>& buffer)
{
	SrQuat quat = SrQuat(mat);
	buffer[baseBuffId.x] = mat.get(12);
	buffer[baseBuffId.y] = mat.get(13);
	buffer[baseBuffId.z] = mat.get(14);
	buffer[baseBuffId.q + 0] = quat.w;
	buffer[baseBuffId.q + 1] = quat.x;
	buffer[baseBuffId.q + 2] = quat.y;
	buffer[baseBuffId.q + 3] = quat.z;	
}


void PAMotions::getUpdateMat(SrMat& dest, SrMat& src)
{
	SrQuat quat = SrQuat(src);
	if (blendData->retarget)
		quat = blendData->retarget->applyRetargetJointRotation(blendData->baseJointName,quat);
	SrMat prerotMat;
//	prerot.get_mat(prerotMat);
	basePrerot.get_mat(prerotMat);

	SrVec vec = quat.axis() * quat.angle();
	SrVec vec1 = vec * basePrerot;//prerotMat.inverse();
	
	quat = SrQuat(vec1);	
#if !NEW_EULER
	SrMat mat;
	quat.get_mat(mat);	
	float rx, ry, rz;
	int eulerRotType = rotType;
	sr_euler_angles(eulerRotType, mat, rx, ry, rz);	
 	if (fabs(rx) > M_PI*0.5f*0.9f || fabs(ry) > M_PI*0.5f*0.9f || fabs(rz) > M_PI*0.5f*0.9f) // swtich rotation order if it's close to singularity
 	{
 		eulerRotType = 312;
 		sr_euler_angles(eulerRotType, mat, rx, ry, rz);
 	}	
	rx = 0.0;
	rz = 0.0;
	sr_euler_mat(eulerRotType, mat, rx, ry, rz);
	//quatP.get_mat(dest);
	dest = mat;
#else
	SrQuat yRotQuat =SrQuat(SrVec(0,vec1.y,0)); // extract only the y-component
	SrMat yRotMat; yRotQuat.get_mat(yRotMat);
	dest = yRotMat;
	/*
	gwiz::euler_t eu = gwiz::euler_t(gwiz::quat_t(quat.w, quat.x,quat.y,quat.z));
	eu.x(0.f);
	eu.z(0.f);
	gwiz::quat_t gw_q = gwiz::quat_t(eu);	
	//vec1.x = 0.0; vec1.z = 0.0;
	SrQuat newQ = SrQuat((float)gw_q.w(),(float)gw_q.x(),(float)gw_q.y(),(float)gw_q.z());	
	dest = newQ.get_mat(dest);
	*/
#endif
	
	
	dest.set(12, src.get(12));
	if (blendData->state->incrementWorldOffsetY)
		dest.set(13, src.get(13));
	dest.set(14, src.get(14));
}

void PAMotions::getProcessedMat(SrMat& dest, SrMat& src)
{
	SrQuat quat = SrQuat(src);
	if (blendData->retarget)
		quat = blendData->retarget->applyRetargetJointRotation(blendData->baseJointName,quat);
	SrMat prerotMat;
//	prerot.get_mat(prerotMat);
	basePrerot.get_mat(prerotMat);

	SrVec vec = quat.axis() * quat.angle();
	SrVec vec1 = vec * prerotMat;//prerotMat.inverse();
	//LOG("quat = %f %f %f %f, axis angle = %f %f %f", quat.w, quat.x, quat.y,quat.z, vec1[0],vec1[1],vec1[2]);
	quat = SrQuat(vec1);	
	SrMat mat;
	quat.get_mat(mat);	
#if !NEW_EULER	
	float rx, ry, rz;
	int eulerRotType = rotType;
	sr_euler_angles(eulerRotType, mat, rx, ry, rz);	 		
	ry = 0.0;
	sr_euler_mat(eulerRotType, mat, rx, ry, rz);
	SrQuat quatP = SrQuat(mat);
#else
	
	SrQuat yRotQuat =SrQuat(SrVec(0,vec1.y,0)); // extract only the y-component
	SrMat yRotMat; yRotQuat.get_mat(yRotMat);
	SrMat xzRotMat = mat*yRotMat.inverse();
	SrQuat quatP = SrQuat(xzRotMat);
	quatP.normalize();
	//LOG("state = %s, y rotation = %f",this->blendData->state->stateName.c_str(), vec1.y);
	//LOG("xzQuat = %f %f %f %f",quatP.x, quatP.y,quatP.z,quatP.w);
	/*	
	gwiz::euler_t eu = gwiz::euler_t(gwiz::quat_t(quat.w, quat.x,quat.y,quat.z));
	//LOG("eu = %f %f %f", eu.x(), eu.y(), eu.z());
	eu.y(0.f);
	gwiz::quat_t gw_q = gwiz::quat_t(eu);
	SrQuat quatP = SrQuat((float)gw_q.w(),(float)gw_q.x(),(float)gw_q.y(),(float)gw_q.z());
	gwiz::euler_t euNew = gwiz::euler_t(gwiz::quat_t(quatP.w, quatP.x,quatP.y,quatP.z));
	//LOG("euNew = %f %f %f", euNew.x(), euNew.y(), euNew.z());
	*/
#endif	
	vec1 = quatP.axis() * quatP.angle();	
	
	//SrQuat quatP;
	//vec1.y = 0.0;
	SrVec vec2 = vec1 * prerotMat.inverse();
	quatP = SrQuat(vec2);
	if (blendData->retarget)
		quatP = blendData->retarget->applyRetargetJointRotationInverse(blendData->baseJointName,quatP);
	quatP.get_mat(dest);

	if (!blendData->state->incrementWorldOffsetY)
		dest.set(13, src.get(13));
}

PAInterpolator::PAInterpolator()
{
}

PAInterpolator::PAInterpolator(PABlendData* data) : PAMotions(data)
{
	int numMotions = getNumMotions();
	if (numMotions > 0)
		processedBaseMats.resize(numMotions);
}

PAInterpolator::~PAInterpolator()
{
}

void PAInterpolator::blending(std::vector<double>& times, SrBuffer<float>& buff)
{
	std::vector<int> indices;
	int numMotions = getNumMotions();
	for (int i = 0; i < numMotions; i++)
	{
		if (blendData->weights[i] != 0.0)
			indices.push_back(i);
	}
	SrBuffer<float> buffer;
	buffer.size(buff.size());	
	processedBaseMats.clear();
	if (indices.size() == 0 && blendData->getStateName() == PseudoIdleState)
	{
		handleBaseMatForBuffer(buff);
		return;
	}
	else if (indices.size() == 1)
	{
		buffer = buff;
		int id = indices[0];
		double time = times[id];
		getBuffer(blendData->state->motions[id], time, motionContextMaps[id], buffer);
		//processedBaseMats[0] = handleBaseMatForBuffer(buffer);
		handleBaseMatForBuffer(buffer);
	}
	else
	{
		int numMotions = indices.size();
		if (numMotions == 0)
			return;
		std::vector<SrBuffer<float> > buffers;
		for (int i = 0; i < numMotions; i++)
		{
			buffers.push_back(SrBuffer<float>());
			SrBuffer<float>& b = buffers[buffers.size() - 1];
			b.size(buff.size());
			b = buff;
			getBuffer(blendData->state->motions[indices[i]], times[indices[i]], motionContextMaps[indices[i]], b);
			//processedBaseMats[i] = handleBaseMatForBuffer(b);
			handleBaseMatForBuffer(b);
		}
		buffer = buffers[0];
		SkChannelArray& motionChan = blendData->state->motions[indices[0]]->channels();
		int chanSize = motionChan.size();
		for (int i = 0; i < chanSize; i++)
		{
			const std::string& chanName = motionChan.mappedName(i);
			for (int j = 1; j < numMotions; j++)
			{
				if (blendData->motionIndex.size() != blendData->state->motions.size())
				{
					blendData->updateMotionIndices();
				}
				if (blendData->motionIndex.size() == 0)
					continue;

				if ((int)blendData->motionIndex[indices[j]].size() <= i)
					continue;

				int id = blendData->motionIndex[indices[j]][i];
				//int id = blendData->state->motions[indices[j]]->channels().search(chanName, motionChan[i].type);
				if (id < 0)
					continue;
				int buffId = motionContextMaps[indices[0]].get(i);
				if (buffId >= 0)
				{
					double prevWeight = 0.0;
					for (int k = 0; k < j; k++)
						prevWeight += blendData->weights[indices[k]];
					double w = prevWeight / (blendData->weights[indices[j]] + prevWeight);
					blendData->state->motions[indices[0]]->channels()[i].interp(&buffer[buffId], &buffer[buffId], &buffers[j][buffId], (float)(1 - w));
				}
			}
		}
	}

	if (joints.size() > 0)
	{
		for (size_t i = 0; i < joints.size(); i++)
		{
			JointChannelId chanId;
			JointChannelId buffId;
			chanId.q = _context->channels().search(joints[i].c_str(), SkChannel::Quat);
			if (chanId.q < 0)
				continue;

			buffId.q = _context->toBufferIndex(chanId.q);
			if (buffId.q < 0)
				continue;
			
			SrQuat origQ;
			origQ.w = buff[buffId.q + 0];
			origQ.x = buff[buffId.q + 1];
			origQ.y = buff[buffId.q + 2];
			origQ.z = buff[buffId.q + 3];
			SrQuat currQ;
			currQ.w = buffer[buffId.q + 0];
			currQ.x = buffer[buffId.q + 1];
			currQ.y = buffer[buffId.q + 2];
			currQ.z = buffer[buffId.q + 3];
			SrQuat finalQ;
			if (blendData->blendMode == PABlendData::Additive)
				finalQ = origQ * currQ;
			else
				finalQ = currQ;
			if (blendData->retarget)
				finalQ = blendData->retarget->applyRetargetJointRotation(joints[i].c_str(),finalQ);

			buff[buffId.q + 0] = finalQ.w;
			buff[buffId.q + 1] = finalQ.x;
			buff[buffId.q + 2] = finalQ.y;
			buff[buffId.q + 3] = finalQ.z;
		}
	}
	else
	{
		if (blendData->retarget) // perform a final retarget pass before output the buffer values
		{			
			SkChannelArray& bufChannels = _context->channels();			
			int chanSize = bufChannels.size();
			for (int i = 0; i < chanSize; i++)
			{
				int buffId = _context->toBufferIndex(i);
				if (bufChannels.type(i) == SkChannel::Quat)
				{
					SrQuat origQ;
					origQ.w = buffer[buffId + 0];
					origQ.x = buffer[buffId + 1];
					origQ.y = buffer[buffId + 2];
					origQ.z = buffer[buffId + 3];
					SrQuat finalQ = blendData->retarget->applyRetargetJointRotation(bufChannels.mappedName(i),origQ);
					buffer[buffId + 0] = finalQ.w;
					buffer[buffId + 1] = finalQ.x;
					buffer[buffId + 2] = finalQ.y;
					buffer[buffId + 3] = finalQ.z;					
				}
				else if (bufChannels.type(i) <= SkChannel::ZPos) // xyz channel
				{
					buffer[buffId] = blendData->retarget->applyRetargetJointTranslation(bufChannels.mappedName(i),buffer[buffId]);
				}
			}
		}
		buff = buffer;
	}
}

void PAInterpolator::clearBlendingJoints()
{
	joints.clear();
}

void PAInterpolator::setBlendingJoints(std::vector<std::string>& j)
{
	joints = j;
}


SrMat PAInterpolator::handleBaseMatForBuffer(SrBuffer<float>& buffer)
{
	SrMat baseMat = getBaseMatFromBuffer(buffer);
	SrMat processedBaseMat;
	getProcessedMat(processedBaseMat, baseMat);
	setBufferByBaseMat(processedBaseMat, buffer);
	return processedBaseMat;
}

PAWoManager::PAWoManager()
{
}

PAWoManager::PAWoManager(PABlendData* data) : PAMotions(data)
{
	firstTime = true;
	for (int i = 0; i < getNumMotions(); i++)
	{
		SrMat mat;
		baseMats.push_back(mat);
	}
	baseDiffMats.resize(baseMats.size());
	intializeTransition = false;
}

PAWoManager::~PAWoManager()
{
}

void PAWoManager::apply(std::vector<double>& times, std::vector<double>& timeDiffs, SrBuffer<float>& buffer, bool directOverride)
{
	std::vector<SrMat> currentBaseMats;
	getBaseMats(currentBaseMats, times, timeDiffs, buffer.size(), buffer);
	if (directOverride)
	{
		baseTransformMat = currentBaseMats[0];		
		return;
	}

	if (!firstTime)
	{
		std::vector<int> indices;
		for (int i = 0; i < getNumMotions(); i++)
			if (blendData->weights[i] != 0.0)
				indices.push_back(i);

		if (indices.size() == 0 && blendData->state->stateName == PseudoIdleState)
		{
			//baseTransformMat = currentBaseMats[0] * baseMats[0].inverse();
			currentBaseTransformMat = currentBaseMats[0];
		}
		else if (indices.size() == 1)
		{
			int id = indices[0];
			if (timeDiffs[id] > 0)
			{
				//baseTransformMat = currentBaseMats[id] * baseMats[id].inverse();
				baseTransformMat = currentBaseMats[id] * baseMats[id].rigidInverse();
				baseDiffMats[id] = baseTransformMat;
			}
			else
			{
				//LOG("loop back to beginning");
			}
	#if LoopHandle
			else
			{
				SrMat newCurrentBase = currentBaseMats[id] * baseTransitionMats[id];
				baseTransformMat = newCurrentBase * baseMats[id].inverse();				
			}
	#endif
		}
		else
		{
			//std::vector<SrMat> mats;
			int numMotions = indices.size();
			if (numMotions == 0)
				return;
			for (int i = 0; i < numMotions; i++)
			{
				SrMat mat;
				if (timeDiffs[indices[i]] > 0)
					mat = currentBaseMats[indices[i]] * baseMats[indices[i]].inverse();
				else
				{
					//LOG("loop back to beginning");
				}
	#if LoopHandle
				else
				{
					SrMat newCurrentBase = currentBaseMats[indices[i]] * baseTransitionMats[indices[i]];
					mat = newCurrentBase * baseMats[indices[i]].inverse();
				}
	#endif
				//mats.push_back(mat);
				baseDiffMats[i] = mat;
			}
			SrMat tempMat = baseDiffMats[0];
			for (int i = 1; i < numMotions; i++)
			{
				double prevWeight = 0.0;
				for (int j = 0; j < i; j++)
					prevWeight += blendData->weights[indices[j]];
				double w = prevWeight / (blendData->weights[indices[i]] + prevWeight);
				SrMat mat = tempMat;
				matInterp(tempMat, mat, baseDiffMats[i], (float)(w));
			}
			baseTransformMat = tempMat;					
		}
	}
	else
	{
		firstBaseTransformMat = currentBaseMats[0];
		currentBaseTransformMat = currentBaseMats[0];
		firstTime = false;
	}

	if (blendData->retarget)
	{
		SrVec tran = baseTransformMat.get_translation();
		for (unsigned int i=0;i<3;i++)
		{
			tran[i] = blendData->retarget->applyRetargetJointTranslation("base",tran[i]);
		}
		baseTransformMat.set_translation(tran);
	}	

	baseMats = currentBaseMats;
}

SrMat& PAWoManager::getBaseTransformMat()
{
	return baseTransformMat;
}


SrMat& PAWoManager::getFirstBaseTransformMat()
{
	return firstBaseTransformMat;
}

SrMat& PAWoManager::getCurrentBaseTransformMat()
{
	return currentBaseTransformMat;
}

void PAWoManager::matInterp(SrMat& ret, SrMat& mat1, SrMat& mat2, float w)
{
	SrQuat quat1 = SrQuat(mat1);
	SrQuat quat2 = SrQuat(mat2);
	SrQuat quat = slerp(quat1, quat2, 1 - w);
	quat.get_mat(ret);
	float posX = mat1.get(12) * w + mat2.get(12) * (1 - w);
	float posY = mat1.get(13) * w + mat2.get(13) * (1 - w);
	float posZ = mat1.get(14) * w + mat2.get(14) * (1 - w);
	ret.set(12, posX);
	ret.set(13, posY);
	ret.set(14, posZ);
}

void PAWoManager::getBaseMats(std::vector<SrMat>& mats, std::vector<double>& times, std::vector<double>& timeDiffs, int bufferSize, SrBuffer<float>& inBuff)
{
	if (!intializeTransition)
	{
		SrBuffer<float> buffer;
		buffer.size(bufferSize);
		for (int i = 0; i < getNumMotions(); i++)
		{	
			SrMat src;
			getBuffer(blendData->state->motions[i], 0.0, motionContextMaps[i], buffer);
			src = getBaseMatFromBuffer(buffer);
			SrMat dest;
			getBuffer(blendData->state->motions[i], blendData->state->motions[i]->duration(), motionContextMaps[i], buffer);
			dest = getBaseMatFromBuffer(buffer);
			SrMat transition = src.inverse() * dest;
			baseTransitionMats.push_back(SrMat());
			SrMat& updateTransitionMat = baseTransitionMats[baseTransitionMats.size() - 1];
			getUpdateMat(updateTransitionMat, transition);
		}
		intializeTransition = true;
	}

	SrBuffer<float> buffer;
	buffer.size(bufferSize);
	int numMotions = getNumMotions();
	if ( getNumMotions() == 0 && blendData->state->stateName == PseudoIdleState)
	{
		SrMat baseMat;			
		baseMat = getBaseMatFromBuffer(inBuff);
		mats.push_back(SrMat());
		SrMat& updateBaseMat = mats[mats.size() - 1];
		getUpdateMat(updateBaseMat, baseMat);		
	}	
	else if (numMotions == times.size())
	{
		for (int i = 0; i < getNumMotions(); i++)
		{
			
			double time = times[i];
			SrMat baseMat;
			getBuffer(blendData->state->motions[i], time, motionContextMaps[i], buffer);
			baseMat = getBaseMatFromBuffer(buffer);
			mats.push_back(SrMat());
			SrMat& updateBaseMat = mats[mats.size() - 1];
			getUpdateMat(updateBaseMat, baseMat);
		}
	}
	
}

PABlendData::PABlendData(const std::string& stateName, std::vector<double>& w, BlendMode blend, WrapMode wrap, ScheduleMode schedule, double blendOffset, double blendTrim, bool dplay)
{
	
	blendStartOffset = (float)blendOffset;
	blendEndTrim = (float)blendEndTrim;
	directPlay = dplay;
	playSpeed = 1.f;
	retarget = NULL;
	SmartBody::SBAnimationBlend* s = SmartBody::SBScene::getScene()->getBlendManager()->getBlend(stateName);
	state = s;
	if (state)
	{
		weights.resize(s->getNumMotions());
		for (size_t x = 0; x < w.size(); x++)
			weights[x] = w[x];
		timeManager = new PATimeManager(this);
		interpolator = new PAInterpolator(this);
		woManager = new PAWoManager(this);
	}
	else
	{
		std::vector<SkMotion*> motions;
		std::vector<std::vector<double> > keys;
		timeManager = new PATimeManager(this);
		interpolator = new PAInterpolator(this);
		woManager = new PAWoManager(this);
	}
	
	blendMode = blend;
	wrapMode = wrap;
	scheduleMode = schedule;
	active = false;
}

PABlendData::PABlendData(PABlend* s, std::vector<double>& w, BlendMode blend, WrapMode wrap, ScheduleMode schedule, double blendOffset, double blendTrim, bool dplay)
{
	blendStartOffset = (float)blendOffset;
	blendEndTrim = (float)blendTrim;
	directPlay = dplay;
	playSpeed = 1.f;
	retarget = NULL;
	state = s;
	weights.resize(s->getNumMotions());
	for (size_t x = 0; x < w.size(); x++)
		weights[x] = w[x];
	timeManager = new PATimeManager(this);
	interpolator = new PAInterpolator(this);
	woManager = new PAWoManager(this);

	blendMode = blend;
	wrapMode = wrap;
	scheduleMode = schedule;
	

	active = false;
}

PABlendData::~PABlendData()
{
	if (timeManager)
		delete timeManager;
	timeManager = NULL;
	if (interpolator)
		delete interpolator;
	interpolator = NULL;
	if (woManager)
		delete woManager;
	woManager = NULL;
}


SBAPI bool PABlendData::getTrajPosition( std::string effectorName, float time, SrVec& outPos )
{
	SmartBody::SBScene* scene = SmartBody::SBScene::getScene();
	outPos = SrVec(0,0,0);	
	for (unsigned int i=0;i<weights.size();i++)
	{
		SmartBody::SBMotion* sbMotion = scene->getMotion(state->getMotionName(i));
		SrVec moPos;
		bool hasTraj = sbMotion->getTrajPosition(effectorName,(float)timeManager->motionTimes[i],moPos);
		if (!hasTraj)
			return false;
		//moPos = moPo;
		outPos += moPos*(float)weights[i];				
	}	
	return true;
}

void PABlendData::evaluateTransition( double timeStep, SrBuffer<float>& buffer, bool tranIn )
{
	if (state && getStateName() == PseudoIdleState) // transition 
	{
		SrBuffer<float> buffCopy = buffer;		
		if (tranIn) // PseudoIdle ----> Animation State
		{
			interpolator->blending(timeManager->motionTimes, buffer); // remove (x,z) & ry from buffer
			woManager->apply(timeManager->motionTimes,timeManager->timeDiffs, buffCopy); // add (x,z) & ry to world offset
			active = true;
		}
		else // Animation State ----> PseudoIdle		
		{				
			woManager->apply(timeManager->motionTimes,timeManager->timeDiffs,buffCopy); // add (x,z) & ry to world offset
			active = true;
		}
	}
	else // if not Pseudo Idle, just proceed as usual
	{
		evaluate(timeStep,buffer);
	}
}

void PABlendData::evaluate(double timeStep, SrBuffer<float>& buffer)
{
	if (state && state->stateName == PseudoIdleState)
	{
		if (!active)
			active = true;		
		return;
	}

	bool notReachCycle = true;
	notReachCycle = timeManager->step(timeStep*playSpeed);
	SrBuffer<float> buffCopy = buffer;
	bool isZeroD = isZeroDState();
	bool OnceAndReachCycle = (wrapMode != Loop && !notReachCycle);
	{		
		if (wrapMode == Loop || ((wrapMode != Loop) && notReachCycle))
		{
			//LOG("motion time = %f",timeManager->motionTimes[0]);			
			interpolator->blending(timeManager->motionTimes, buffer);
			woManager->apply(timeManager->motionTimes, timeManager->timeDiffs, buffCopy);
			
			active = true;
		}
		else
			active = false;	
	}	
}

std::string PABlendData::getStateName()
{
	return state->stateName;	
}

float PABlendData::getStateKeyTime( int motionIdx, int sectionIdx )
{
	float stateKeyTime = (float)state->keys[motionIdx][sectionIdx];	
	if (sectionIdx == 0) stateKeyTime += blendStartOffset;
	if (sectionIdx == state->getNumKeys()-1) stateKeyTime -= blendEndTrim;
	
	return stateKeyTime;
}

float PABlendData::getStateMotionDuration( int motionIdx )
{
	return (getStateKeyTime(motionIdx,state->getNumKeys()-1) - getStateKeyTime(motionIdx, 0));
}


bool PABlendData::isPartialBlending()
{
	if (interpolator->joints.size() != 0)
		return true;
	else
		return false;
}

void PABlendData::updateMotionIndices()
{
	motionIndex.clear();

	if (!state)
		return;

	for (size_t m = 0; m < state->motions.size(); m++)
	{
		motionIndex.push_back(std::vector<int>());
		SkMotion* motion = state->motions[m];
		SkChannelArray& motionChan = state->motions[m]->channels();
		int chanSize = motionChan.size();
		for (int c = 0; c < chanSize; c++)
		{
			const std::string& chanName = motionChan.mappedName(c);
			motionIndex[m].push_back(motionChan.search(chanName, motionChan[c].type));
		}
	}
}

bool PABlendData::isZeroDState()
{
	SmartBody::SBAnimationBlend0D* zeroBlend = dynamic_cast<SmartBody::SBAnimationBlend0D*>(state);
	return (zeroBlend != NULL);
}


PATransitionManager::PATransitionManager(float transitionLen)
{
	from = NULL;
	to = NULL;
	startTransition = false;
	blendingMode = true;
	active = true;
	transition = NULL;
	curve = new srLinearCurve();
	curve->insert(0.0, 1.0);
	float tranLen = transitionLen;
#if 1
	curve->insert(tranLen, 0.0);
	duration = tranLen;
	localTime = 0.0;
	s1 = 0.0;
	e1 = tranLen;
	s2 = 0.0;
	e2 = tranLen;	
#endif
}

PATransitionManager::PATransitionManager(double easeOutStart, double dur)
{
	startTransition = false;
	blendingMode = false;
	active = true;
	transition = NULL;
	curve = new srLinearCurve();
	duration = dur;
	localTime = 0.0;
	s1 = easeOutStart;
	e1 = s1 + dur;
	s2 = 0.0;
	// possible bug spot, although for now, the result seems better
#if 1
	e2 = s2 + dur;
#else
	e2 += dur;
#endif
}

PATransitionManager::PATransitionManager(SmartBody::SBAnimationTransition* trans, PABlendData* f, PABlendData* t)
{
	from = f;
	to = t;
	//transition = trans;
	transition = new SmartBody::SBAnimationTransition(trans, f->state, to->state);
	update();
	localTime = 0.0;
	startTransition = false;
	blendingMode = false;
	active = true;
	curve = new srLinearCurve();
	s1 = -1.0;
	e1 = -1.0;
}

PATransitionManager::~PATransitionManager()
{
}


/* 
	This function activate next state module according to current module.
	PATransitionManager has two mode: align mode and blending mode, it starts with align mode, after aligning, switch to blending mode
*/
void PATransitionManager::align(PABlendData* current, PABlendData* next)
{

	// possible bug spot, although for now, the result seems better
#if 0
	int numEaseOut = getNumEaseOut();
	for (int i = 0; i < numEaseOut; i++)
	{
		if (fabs(current->timeManager->localTime - easeOutStarts[i]) < timeThreshold)
		{
			s1 = easeOutStarts[i];
			e1 = easeOutEnds[i];
		}
	}

	if (fabs(current->timeManager->localTime - s1) < timeThreshold)
	{
		next->active = true;
		blendingMode = true;
		curve->insert(0.0, 1.0);
		curve->insert(duration, 0.0);
#if PrintPADebugInfo
		LOG("State %s being scheduled.[ACTIVE]", next->data->stateName.c_str());
#endif
	}
#else	
	int numEaseOut = getNumEaseOut();
	for (int i = 0; i < numEaseOut; i++)
	{
		if (current->timeManager->getPrevNormalizeLocalTime() <= easeOutStarts[i] && current->timeManager->getNormalizeLocalTime() >= easeOutStarts[i]) 
		{
			s1 = easeOutStarts[i];
			e1 = easeOutEnds[i];			
			break;
		}
	}
	if (current->timeManager->getPrevNormalizeLocalTime() <= s1 && current->timeManager->getNormalizeLocalTime() >= s1)
	//if (current->timeManager->getNormalizeLocalTime() >= s1)
	{
		next->active = true;
		blendingMode = true;
		// Important: adjust for the duration (is this enough?)
		//LOG("start transition, s1 = %f, local time = %f",s1,current->timeManager->getNormalizeLocalTime());
		duration += (current->timeManager->getPrevNormalizeLocalTime() - current->timeManager->getNormalizeLocalTime());				
		curve->insert(0.0, 1.0);
		curve->insert(duration, 0.0);
	}
#endif
}

void PATransitionManager::blending( SrBuffer<float>& buffer, SrBuffer<float>&buffer1, SrBuffer<float>&buffer2, SrMat& mat, SrMat& mat1, SrMat& mat2, double timeStep, MeControllerContext* context, bool directCombine)
{
	double w = curve->evaluate(localTime);
	bufferBlending(buffer, buffer1, buffer2, w, context);
	if (directCombine)
		//PAWoManager::matInterp(mat, mat1, mat2, (float)woBlendWeight);	
		mat = mat1*mat2;
	else
		PAWoManager::matInterp(mat, mat1, mat2, (float)w);		
}


float PATransitionManager::getCurrentTransitionWeight()
{
	return (float)curve->evaluate(localTime);
}

void PATransitionManager::update()
{
	if (!transition)
		return;
	std::vector<double> fromKey;
	int id;
	

	for (int i = 0; i < transition->getSourceBlend()->getNumMotions(); i++)
	{
		std::string motionName = transition->getSourceBlend()->motions[i]->getName();
		if (motionName == transition->getSourceMotionName())
		{
			fromKey = transition->getSourceBlend()->keys[i];
			id = i;
		}
	}
	
	if (fromKey.size() > 0)
	{
		for (int i = 0; i < transition->getNumEaseOutIntervals(); i++)
		{
			if (transition->getEaseOutStart()[i] < fromKey[0])
			{
				transition->getEaseOutStart()[i] += from->getStateMotionDuration(id);//transition->getSourceBlend()->motions[id]->duration();
				transition->getEaseOutEnd()[i] += from->getStateMotionDuration(id);//transition->getSourceBlend()->motions[id]->duration();
				//transition->getEaseOutStart()[i] += transition->getSourceBlend()->

			}
			easeOutStarts.push_back(getTime(transition->getEaseOutStart()[i], fromKey, transition->getSourceBlend()->keys, from->weights));
			easeOutEnds.push_back(getTime(transition->getEaseOutEnd()[i], fromKey, transition->getSourceBlend()->keys, from->weights));
		}
	}
	

	std::vector<double> toKey;
	for (int i = 0; i < transition->getDestinationBlend()->getNumMotions(); i++)
	{
		std::string motionName = transition->getDestinationBlend()->motions[i]->getName();
		if (motionName == transition->getDestinationMotionName())
		{
			toKey = transition->getDestinationBlend()->keys[i];
			id = i;
		}
	}
	if (toKey.size() > 0)
	{
		if (transition->getEaseInStart() < toKey[0])
		{
			//transition->setEaseInStart(transition->getEaseInStart() + transition->getDestinationBlend()->motions[id]->duration());
			//transition->setEaseInEnd(transition->getEaseInEnd() + transition->getDestinationBlend()->motions[id]->duration());
			transition->setEaseInStart(transition->getEaseInStart() + to->getStateMotionDuration(id));
			transition->setEaseInEnd(transition->getEaseInEnd() + to->getStateMotionDuration(id));
		}
	}
	s2 = getTime(transition->getEaseInStart(), toKey, transition->getDestinationBlend()->keys, to->weights);
	e2 = getTime(transition->getEaseInEnd(), toKey, transition->getDestinationBlend()->keys, to->weights);

	duration = e2 - s2;
}

double PATransitionManager::getSlope()
{
	double slope = (s1 - e1) / (s2 - e2);
	return slope;
}

int PATransitionManager::getNumEaseOut()
{
	if (easeOutStarts.size() != easeOutEnds.size())
		return -1;
	else
		return easeOutStarts.size();
}

void PATransitionManager::bufferBlending(SrBuffer<float>& buffer, SrBuffer<float>& buffer1, SrBuffer<float>& buffer2, double w, MeControllerContext* _context)
{
	if (!_context)
		return;
	SkChannelArray& channels = _context->channels();
	for (int i = 0; i < channels.size(); i++)
	{
		const std::string& chanName = channels.mappedName(i);
		if (chanName == SbmPawn::WORLD_OFFSET_JOINT_NAME)
			continue;

		if (channels[i].type == SkChannel::XPos || channels[i].type == SkChannel::YPos || channels[i].type == SkChannel::ZPos)
		{
			float v1 = buffer1[_context->toBufferIndex(i)];
			float v2 = buffer2[_context->toBufferIndex(i)];
			float v;
			SkChannel::interp(channels[i].type, &v, &v1, &v2, float(1 - w));
			buffer[_context->toBufferIndex(i)] = v;
		}
		if (channels[i].type == SkChannel::Quat)
		{
			float v1[4];
			float v2[4];
			float v[4];
			for (int j = 0; j < 4; j++)
			{
				v1[j] = buffer1[_context->toBufferIndex(i) + j];
				v2[j] = buffer2[_context->toBufferIndex(i) + j];
			}
			SkChannel::interp(channels[i].type, v, v1, v2, float(1 - w)); 
			for (int j = 0; j < 4; j++)
				buffer[_context->toBufferIndex(i) + j] = v[j];

// 			if (channels.mappedName(i) == "base")
// 			{
// 				LOG("quat1 = %.3f %.3f %.3f %.3f, quat2 = %.3f %.3f %.3f %.3f, combine quat = %.3f %.3f %.3f %.3f", v1[0],v1[1],v1[2],v1[3],v2[0],v2[1],v2[2],v2[3], v[0],v[1],v[2],v[3]);
// 			}

		}
	}	
}


double PATransitionManager::getTime(double time, const std::vector<double>& key, const std::vector<std::vector<double> >& keys, const std::vector<double>& w)
{
	double ret = 0.0;
	int section = -1;
	if (key.size() > 0)
	{
		for (size_t i = 0; i < key.size() - 1; i++)
		{
			if (key[i] <= time && key[i + 1] >= time)
				section = i;
		}
	}

	if (section < 0)
	{
		LOG("PATransitionManager::getTime ERR!");
		return -1.0;
	}
	for (size_t i = 0; i < w.size(); i++)
	{
		double t = keys[i][section] + (keys[i][section + 1] - keys[i][section]) * (time - key[section]) / (key[section + 1] - key[section]);
		ret += t * w[i];
	}
	return ret;
}

void PATransitionManager::step( double timeStep )
{
	localTime += timeStep;
	if (localTime > duration) 
		active = false;
}
