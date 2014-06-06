#include "me_ct_limb.hpp"
#include <assert.h>

MeCtLimb::MeCtLimb(void)
{
	skeleton = NULL;
}

MeCtLimb::~MeCtLimb(void)
{
}

void MeCtLimb::init(SkSkeleton* skel)
{
	skeleton = skel;
}

SkJoint* MeCtLimb::getChainRoot()
{
	if (joint_chain.size() > 0)
		return joint_chain[0];
	else
		return NULL;
}


SkJoint* MeCtLimb::getChainEndEffector()
{
	if (joint_chain.size() > 0)
		return joint_chain[joint_chain.size()-1];
	else
		return NULL;
}

void MeCtLimb::updateQuatToJointChain( SrArray<SrQuat>& joint_quat )
{
	assert(joint_quat.size() == joint_chain.size());

	for (unsigned int i=0;i<joint_chain.size();i++)
	{
		joint_chain[i]->quat()->value(joint_quat[i]);
	}
	skeleton->invalidate_global_matrices();
}

void MeCtLimb::updateQuat(MeFrameData& frame, bool bRead)
{
	SrBuffer<float>& buffer = frame.buffer();

	for (unsigned int i=0;i<buf_index.size();i++)
	{
		int index = frame.toBufferIndex(buf_index[i]);
		if (bRead)
		{
			joint_quat[i].w = buffer[index];
			joint_quat[i].x = buffer[index+1];
			joint_quat[i].y = buffer[index+2];
			joint_quat[i].z = buffer[index+3];
		}
		else // write current joint quat to the buffer
		{
			SrQuat prevQuat = SrQuat(buffer[index],buffer[index+1],buffer[index+2],buffer[index+3]);
			SrQuat curQuat  = joint_quat[i];

			buffer[index] = joint_quat[i].w;
			buffer[index+1] = joint_quat[i].x;
			buffer[index+2] = joint_quat[i].y;
			buffer[index+3] = joint_quat[i].z;			
		}		
	}	
}

bool MeCtLimb::buildJointChain(SrArray<const char*>& joint_names, SrArray<MeCtIKJointLimit>& joint_limit)
{
	// initialize joint chain list
	joint_chain_limit.clear();
	joint_chain.clear();
	joint_quat.clear();
	buf_index.clear();
	for (int i=0;i<joint_names.size();i++)
	{
		SkJoint* joint = skeleton->search_joint(joint_names[i]);
		if (!joint)
		{					
			//return false;
			continue;
		}
		//joint->rot_type(SkJoint::TypeSwingTwist);
		joint_chain.push_back(joint);
		joint_chain_limit.push_back(joint_limit[i]);
		//joint_chain[i] = joint;		
	}	
	buf_index.resize(joint_chain.size());
	joint_quat.resize(joint_chain.size());

	// initialize MeCtIKScenario
	ik.joint_info_list.size(joint_chain.size());
	for (unsigned int i=0;i<joint_chain.size();i++)
	{	
		ik.joint_info_list[i] = MeCtIKScenarioJointInfo();
		MeCtIKScenarioJointInfo& joint_info = ik.joint_info_list[i];
		joint_info.sk_joint = joint_chain[i];
		joint_info.type = JOINT_TYPE_BALL;			
		joint_quat[i] = SrQuat(0,0,0,1.0);			
		joint_info.is_support_joint = 0;	
		joint_info.joint_limit = joint_chain_limit[i];
	}
	ik.start_joint = &ik.joint_info_list[0];
	ik.end_joint   = &ik.joint_info_list[joint_chain.size()-1];
	//ik.joint_quat_list = joint_quat;
	VecToSrArray(joint_quat,ik.joint_quat_list);

	return true;	
}

float MeCtLimb::computeLimbLength()
{
	float fLength = 0.f;
	for (unsigned int i=0;i<joint_chain.size()-1;i++)
	{
		fLength += joint_chain[i]->offset().len();
	}
	return fLength;
}

void MeCtLimb::updateMotionFrameToJointChain( SkMotion* motion, int frame )
{
	const float *fp = motion->posture(frame);
	SkChannelArray& channels = motion->channels();
	unsigned int inserted;
	unsigned int size = channels.size();

	for (unsigned int i=0; i<size; i++ ) {
		if ( channels[i].joint && hasJoint(channels[i].joint) )
		{
			inserted = channels[i].set ( fp );
		}
		else
			inserted = channels[i].size ();
		fp += inserted;
	}
	skeleton->invalidate_global_matrices();
	
}

bool MeCtLimb::hasJoint( SkJoint* joint )
{
	if (!joint)
		return false;

	for (unsigned int i=0;i<joint_chain.size();i++)
	{
		if (joint == joint_chain[i])
			return true;
	}
	return false;
}