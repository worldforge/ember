#include "controllers/me_ct_data_receiver.h"
#include <sr/sr_euler.h>


std::string MeCtDataReceiver::CONTROLLER_TYPE = "DataReceiver";

MeCtDataReceiver::MeCtDataReceiver(SkSkeleton* skel) : SmartBody::SBController()
{
	_skeleton = skel;
	//skel->ref();

	_valid = true;
	_prevTime = 0.0;
	_duration = 0.0;
}

MeCtDataReceiver::~MeCtDataReceiver()
{
	//_skeleton->unref();
	delete _skeleton;
	_posMap.clear();
	_startingPos.clear();
	_quatMap.clear();
}

void MeCtDataReceiver::setGlobalPosition(const std::string& jName, SrVec& pos)
{
	std::map<std::string, SrVec>::iterator iter = _posMap.find(jName);
	if (iter == _posMap.end())
	{
		_posMap.insert(std::make_pair(jName, pos));
		_startingPos.insert(std::make_pair(jName, pos));
	}
	else
		iter->second = pos;
}


void MeCtDataReceiver::setLocalPosition(const std::string& jName, SrVec& pos)
{
	std::map<std::string, SrVec>::iterator iter = _localPosMap.find(jName);
	if (iter == _localPosMap.end())
	{
		_localPosMap.insert(std::make_pair(jName, pos));
	}
	else
		iter->second = pos;
}

void MeCtDataReceiver::setLocalRotation(const std::string& jName, SrQuat& q)
{
	std::map<std::string, SrQuat>::iterator iter = _quatMap.find(jName);
	if (iter == _quatMap.end())
		_quatMap.insert(std::make_pair(jName, q));
	else
		iter->second = q;
}

void MeCtDataReceiver::removeLocalRotation(const std::string& jName)
{
	std::map<std::string, SrQuat>::iterator iter = _quatMap.find(jName);
	if (iter != _quatMap.end())
		_quatMap.erase(iter);
}

void MeCtDataReceiver::removeLocalPosition(const std::string& jName)
{
	std::map<std::string, SrVec>::iterator iter = _localPosMap.find(jName);
	if (iter != _localPosMap.end())
		_localPosMap.erase(iter);
}

bool MeCtDataReceiver::controller_evaluate(double t, MeFrameData& frame)
{
	
	if (_prevTime == 0)
		_dt = 0.016;
	else
	{
		_dt = t - _prevTime;
		_prevTime = t;
	}
	if (_valid && _context)
	{
		// set local rotation directly
		std::map<std::string, SrQuat>::iterator iter;
		for (iter = _quatMap.begin(); iter != _quatMap.end(); iter++)
		{
			std::string jName = iter->first;
			SkJoint* joint = _skeleton->search_joint(jName.c_str());
			if (!joint)	continue;
			int channelId = _context->channels().search(jName, SkChannel::Quat);
			if (channelId < 0)	continue;
			int bufferId = frame.toBufferIndex(channelId);
			if (bufferId < 0)	continue;
			SrQuat quat = iter->second;
			//SrQuat quat_p = joint->quat()->prerot().inverse() * (quat * joint->gmatZero().inverse());
			//SrQuat quat_p = (quat * joint->gmatZero().inverse()) * joint->quat()->prerot().inverse();

			//SrQuat quat_p =  joint->gmatZero().inverse() * quat * joint->gmatZero();
			SrQuat quat_p = quat;
			//SrQuat quat_p = quat * joint->quat()->prerot().inverse();
			//SrQuat quat_p = quat;
			frame.buffer()[bufferId + 0] = quat_p.w;
			frame.buffer()[bufferId + 1] = quat_p.x;
			frame.buffer()[bufferId + 2] = quat_p.y;
			frame.buffer()[bufferId + 3] = quat_p.z;
		}

		// set local position directly
		std::map<std::string, SrVec>::iterator iter2;
		for (iter2 = _localPosMap.begin(); iter2 != _localPosMap.end(); iter2++)
		{
			std::string jName = iter2->first;
			SkJoint* joint = _skeleton->search_joint(jName.c_str());
			if (!joint)	continue;
			int channelId = _context->channels().search(jName, SkChannel::XPos);
			if (channelId < 0)	continue;
			int bufferId = frame.toBufferIndex(channelId);
			if (bufferId < 0)	continue;
			SrVec curr = iter2->second;

			frame.buffer()[bufferId + 0] = curr.x;
			frame.buffer()[bufferId + 1] = curr.y;
			frame.buffer()[bufferId + 2] = curr.z;
		}

		// get difference of global position and add up to frame buffer
		std::map<std::string, SrVec>::iterator iter1;
		for (iter1 = _posMap.begin(); iter1 != _posMap.end(); iter1++)
		{
			std::string jName = iter1->first;
			SkJoint* joint = _skeleton->search_joint(jName.c_str());
			if (!joint)	continue;
			int channelId = _context->channels().search(jName, SkChannel::XPos);
			if (channelId < 0)	continue;
			int bufferId = frame.toBufferIndex(channelId);
			if (bufferId < 0)	continue;
			SrVec curr = iter1->second;
			SrVec start = _startingPos[jName];
			SrVec diff = curr - start;

			frame.buffer()[bufferId + 0] = -diff.x;
			frame.buffer()[bufferId + 1] = diff.y;
			frame.buffer()[bufferId + 2] = -diff.z;
		}
	}
	return true;
}
