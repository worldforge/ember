#include "me_ct_reach.hpp"
#include "me_ct_IK_scenario.hpp"
#include <assert.h>

std::string MeCtReach::CONTROLLER_TYPE = "Reach";
const float PI_CONST = 3.14159265358979323846f;

#define LIMB_ARM 1
#if LIMB_ARM
const int  NUM_LIMBS = 5;
// feng : right now I simply hard coded the joint chains and constraints for the left, right arms.
// in the future, I think we should integrate the joint constraints into skeleton, so it is easier to set up an IK chain interactively ( by simply select the start, end joints ? )
// const char limb_chain_r[][20] = {"r_shoulder", "r_elbow", "r_forearm", "r_wrist", "r_middle1"};
// const char limb_chain_l[][20] = {"l_shoulder", "l_elbow", "l_forearm", "l_wrist", "l_middle1"};
const char limb_chain_r[][20] = {"r_shoulder", "r_elbow", "r_forearm", "r_wrist", "r_middle1"};
const char limb_chain_l[][20] = {"l_shoulder", "l_elbow", "l_forearm", "l_wrist", "l_middle1"};

const MeCtIKJointLimit limb_joint_limit_r[] = { 							
	{SrVec2(PI_CONST*0.5f, PI_CONST*0.4f), SrVec2(PI_CONST*0.7f,PI_CONST*0.3f),SrVec2(PI_CONST*0.5f,-PI_CONST*0.5f)},											 
	{SrVec2(PI_CONST*0.01f, PI_CONST*0.01f), SrVec2(PI_CONST*0.8f,PI_CONST*0.05f),SrVec2(PI_CONST*0.1f,-PI_CONST*0.1f)},                                              
	{SrVec2(PI_CONST*0.01f, PI_CONST*0.01f), SrVec2(PI_CONST*0.01f,PI_CONST*0.01f),SrVec2(PI_CONST*0.01f,-PI_CONST*0.01f)},                                              
	{SrVec2(PI_CONST*0.3f, PI_CONST*0.3f), SrVec2(PI_CONST*0.3f,PI_CONST*0.3f),SrVec2(PI_CONST*0.01f,-PI_CONST*0.01f)},                                              
	{SrVec2(PI_CONST*0.3f, PI_CONST*0.3f), SrVec2(PI_CONST*0.3f,PI_CONST*0.3f),SrVec2(PI_CONST*0.01f,-PI_CONST*0.01f)},								    
};

// since the coordinates are flip in the left-arm, all joint limits need to be flipped as well.
const MeCtIKJointLimit limb_joint_limit_l[] = { 							
	{SrVec2(PI_CONST*0.4f, PI_CONST*0.5f), SrVec2(PI_CONST*0.3f,PI_CONST*0.7f),SrVec2(PI_CONST*0.5f,-PI_CONST*0.5f)},											 
	{SrVec2(PI_CONST*0.01f, PI_CONST*0.01f), SrVec2(PI_CONST*0.05f,PI_CONST*0.8f),SrVec2(PI_CONST*0.1f,-PI_CONST*0.1f)},                                              
	{SrVec2(PI_CONST*0.01f, PI_CONST*0.01f), SrVec2(PI_CONST*0.01f,PI_CONST*0.01f),SrVec2(PI_CONST*0.01f,-PI_CONST*0.01f)},                                              
	{SrVec2(PI_CONST*0.3f, PI_CONST*0.3f), SrVec2(PI_CONST*0.3f,PI_CONST*0.3f),SrVec2(PI_CONST*0.01f,-PI_CONST*0.01f)},                                              
	{SrVec2(PI_CONST*0.3f, PI_CONST*0.3f), SrVec2(PI_CONST*0.3f,PI_CONST*0.3f),SrVec2(PI_CONST*0.01f,-PI_CONST*0.01f)},								    
};
#else
const int  NUM_LIMBS = 5;
// feng : right now I simply hard coded the joint chains and constraints for the left, right arms.
// in the future, I think we should integrate the joint constraints into skeleton, so it is easier to set up an IK chain interactively ( by simply select the start, end joints ? )
const char limb_chain_r[][20] = {"r_hip", "r_knee", "r_ankle", "r_forefoot", "r_toe"};
const char limb_chain_l[][20] = {"l_hip", "l_knee", "l_ankle", "l_forefoot", "l_toe"};

const MeCtIKJointLimit limb_joint_limit_r[] = { 							
	{SrVec2(PI_CONST*0.3f, PI_CONST*0.3f), SrVec2(PI_CONST*0.3f,PI_CONST*0.7f),SrVec2(PI_CONST*0.3f,-PI_CONST*0.3f)},											 
	{SrVec2(PI_CONST*0.01f, PI_CONST*0.01f), SrVec2(PI_CONST*0.8f,PI_CONST*0.05f),SrVec2(PI_CONST*0.01f,-PI_CONST*0.01f)},                                              
	{SrVec2(PI_CONST*0.3f, PI_CONST*0.3f), SrVec2(PI_CONST*0.3f,PI_CONST*0.3f),SrVec2(PI_CONST*0.2f,-PI_CONST*0.2f)},                                              
	{SrVec2(PI_CONST*0.3f, PI_CONST*0.3f), SrVec2(PI_CONST*0.01f,PI_CONST*0.01f),SrVec2(PI_CONST*0.01f,-PI_CONST*0.01f)},                                              
	{SrVec2(PI_CONST*0.3f, PI_CONST*0.3f), SrVec2(PI_CONST*0.01f,PI_CONST*0.01f),SrVec2(PI_CONST*0.01f,-PI_CONST*0.01f)},								    
};

// since the coordinates are flip in the left-arm, all joint limits need to be flipped as well.
const MeCtIKJointLimit limb_joint_limit_l[] = { 							
	{SrVec2(PI_CONST*0.3f, PI_CONST*0.3f), SrVec2(PI_CONST*0.7f,PI_CONST*0.3f),SrVec2(PI_CONST*0.3f,-PI_CONST*0.3f)},											 
	{SrVec2(PI_CONST*0.01f, PI_CONST*0.01f), SrVec2(PI_CONST*0.05f,PI_CONST*0.8f),SrVec2(PI_CONST*0.01f,-PI_CONST*0.01f)},                                              
	{SrVec2(PI_CONST*0.3f, PI_CONST*0.3f), SrVec2(PI_CONST*0.3f,PI_CONST*0.3f),SrVec2(PI_CONST*0.2f,-PI_CONST*0.2f)},                                              
	{SrVec2(PI_CONST*0.3f, PI_CONST*0.3f), SrVec2(PI_CONST*0.01f,PI_CONST*0.01f),SrVec2(PI_CONST*0.01f,-PI_CONST*0.01f)},                                              
	{SrVec2(PI_CONST*0.3f, PI_CONST*0.3f), SrVec2(PI_CONST*0.01f,PI_CONST*0.01f),SrVec2(PI_CONST*0.01f,-PI_CONST*0.01f)},								    
};

#endif

MeCtReach::MeCtReach( ) 
{
	MeCtReach(NULL);
}

MeCtReach::MeCtReach( SkSkeleton* skeleton ) 
{
	reach_mode = TARGET_POS;
	target_pos = SrVec(0.f,150.f,0.f);
	_skeleton = skeleton;
	target_joint_ref = NULL;
	limb_length = 0.0;
	prev_time = -1.0;
	//_duration = 10.0;

	reach_arm = REACH_RIGHT_ARM;	
}

MeCtReach::~MeCtReach(void)
{
	
}

SrVec MeCtReach::get_reach_target()
{
	SrVec target;
	if (reach_mode == TARGET_POS)
	{
		target = target_pos;
	}
	else if (reach_mode == TARGET_JOINT_POS)
	{
		assert(target_joint_ref);
		if( target_joint_ref )	
		{
			SrMat sr_M;
//			matrix_t M;			
			target_joint_ref->update_gmat_up();
			sr_M = target_joint_ref->gmat();			
			target = SrVec(sr_M.get(12),sr_M.get(13),sr_M.get(14));			
	    }
	}
	return target;
}

void MeCtReach::set_target_joint(SkJoint* target_joint)
{
	if (target_joint)
	{
		reach_mode = TARGET_JOINT_POS;
		target_joint_ref = target_joint;
	}
	else
	{
		reach_mode = TARGET_POS;
	}
}

void MeCtReach::init(SbmPawn* pawn)
{
	assert(_skeleton);
	joint_name.size(NUM_LIMBS);	
	joint_limit.size(NUM_LIMBS);
	limb_length = 0.f;

	for (int i=0;i<NUM_LIMBS;i++)
	{	
		if (reach_arm == REACH_RIGHT_ARM)
		{
			joint_name[i] = limb_chain_r[i];
			joint_limit[i] = limb_joint_limit_r[i];				
		}
		else if (reach_arm == REACH_LEFT_ARM)
		{
			joint_name[i] = limb_chain_l[i];
			joint_limit[i] = limb_joint_limit_l[i];
		}						
		// feng : add both the left & right arms in the channel buffers
		// I am not sure why, but this avoids popping when adding the second arm
		_channels.add(limb_chain_r[i], SkChannel::Quat);	
		_channels.add(limb_chain_l[i], SkChannel::Quat);
	}		

	limb.init(_skeleton);
	limb.buildJointChain(joint_name,joint_limit);
	limb_length = limb.computeLimbLength();
	root_joint_ref = limb.joint_chain[0];
	MeController::init(pawn);
}

void MeCtReach::controller_map_updated() 
{		
	// set buffer index mapping
	for (int i=0;i<joint_name.size();i++)
	{		
		if (reach_arm == REACH_RIGHT_ARM)
			limb.buf_index[i] = _toContextCh[i*2];
		else 
			limb.buf_index[i] = _toContextCh[i*2+1];
	}	
}

bool MeCtReach::controller_evaluate( double t, MeFrameData& frame )
{			
	float dt = 0.001f;
	if (prev_time == -1.0) // first start
	{
		dt = 0.001f;		
		// for first frame, update from frame buffer to joint quat in the limb
		// any future IK solving will simply use the joint quat from the previous frame.
		limb.updateQuat(frame,true);
	}
	else
	{		
		dt = ((float)(t-prev_time));
	}
	prev_time = (float)t;

	//limb.updateQuat(frame,true);

	MeCtIKScenario* ik_scenario = &limb.ik;
	
	ik_scenario->ik_offset = get_reach_target(); // set the target
	ik_scenario->ik_quat_orientation = SrQuat(0,0,0,1.0); // set to default rotation for testing
	//ik_scenario->joint_quat_list = limb.joint_quat;	
	VecToSrArray(limb.joint_quat,ik_scenario->joint_quat_list);
	
	limb.skeleton->update_global_matrices();
	SkJoint* chain_root = limb.getChainRoot();
	ik_scenario->gmat = chain_root->parent()->gmat();
	
	ik.setDt(dt);
	ik.update(ik_scenario);
	//limb.joint_quat = ik_scenario->joint_quat_list;
	SrArrayToVec(ik_scenario->joint_quat_list,limb.joint_quat);
	// write results from limb to buffer
	limb.updateQuat(frame,false);
	return true;

}

void MeCtReach::controller_start()
{
}

void MeCtReach::print_state(int tabs)
{
}




