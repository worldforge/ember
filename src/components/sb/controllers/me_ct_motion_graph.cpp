#include "controllers/me_ct_motion_graph.hpp"
#include <sb/SBRetargetManager.h>
#include <sb/SBRetarget.h>
#include <sb/SBScene.h>

std::string MeCtMotionGraph::CONTROLLER_TYPE = "MotionGraph";


MeCtMotionGraph::MeCtMotionGraph( SmartBody::SBCharacter* character )
{	
	motionState = NULL;
	sbChar = character;
	baseJointName = "base";

}

MeCtMotionGraph::~MeCtMotionGraph(void)
{
	
}


SmartBody::SBMotionNodeState* MeCtMotionGraph::getMotionNodeState()
{
	return motionState;
}


bool MeCtMotionGraph::controller_evaluate( double t, MeFrameData& frame )
{
	if (!motionState)
	{
		std::vector<std::string> affectedJointNames;
		motionState = new SmartBody::SBMotionNodeState();
		motionState->initState(frame.context()->channels(), affectedJointNames, baseJointName);	
		outMotionBuffer.copyFrameBuffer(motionState->getCurMotionFrameBuffer());
	}	
	
	updateDt((float) t);
	if (dt > 0.05f)
		dt = 0.05f; // cap the update rate		

	if (motionState->isRunning()) // only update the motion state and frame buffer when it is running
	{	
		std::string blendSkel = motionState->getCurrentMotionNode()->getAnimBlend()->getBlendSkeleton();
		std::string charSkel = sbChar->getSkeleton()->getName();
		SmartBody::SBRetargetManager* retargetManager = SmartBody::SBScene::getScene()->getRetargetManager();
		SmartBody::SBRetarget* retarget = retargetManager->getRetarget(blendSkel, charSkel);		
		SrMat curWO = sbChar->get_world_offset();
		float tdt = 0.01f;
		//getJointChannelValues(SbmPawn::WORLD_OFFSET_JOINT_NAME, frame, curWO);		
		SrMat deltaWO = motionState->stepMotionState(dt);
		SrVec deltaTran = deltaWO.get_translation();
		//LOG("delta Tran = %f %f %f", deltaTran[0],deltaTran[1],deltaTran[2]);
		// write the result back to MeFrameBuffer
		SmartBody::SBMotionFrameBuffer& motionBuffer = motionState->getCurMotionFrameBuffer();
		outMotionBuffer.getBuffer() = motionBuffer.getBuffer(); // copy buufer
		if (retarget)
		{
			outMotionBuffer.applyRetarget(retarget);
			SrVec woPos = deltaWO.get_translation();
			for (unsigned int i=0;i<3;i++)
				woPos[i] = retarget->applyRetargetJointTranslation("base",woPos[i]);
			deltaWO.set_translation(woPos);
		}


		assert(frame.buffer().size() == outMotionBuffer.getBuffer().size());
		frame.buffer() = outMotionBuffer.getBuffer();
		updateWO(curWO, deltaWO, frame);
	}

	return true;
}

void MeCtMotionGraph::updateWO( const SrMat& curWO, const SrMat& deltaWO, MeFrameData& frame )
{
	SmartBody::SBJoint* baseJ = sbChar->getSkeleton()->getJointByName(baseJointName);	
	SrVec offset;
	if (baseJ) offset = baseJ->offset(); offset.y = 0.f;
	SrMat negOffset; negOffset.set_translation(-offset);
	SrMat posOffset; posOffset.set_translation(offset);
	SrMat nextMat = negOffset * deltaWO * posOffset * curWO;
	SrVec newTran = nextMat.get_translation();
	SrVec curTran = curWO.get_translation();
	//LOG("curTran = %f %f %f", curTran[0],curTran[1],curTran[2]);
	//LOG("newTran = %f %f %f", newTran[0],newTran[1],newTran[2]);
	SrQuat newRot = SrQuat(nextMat.get_rotation());
	std::string woJointName = SbmPawn::WORLD_OFFSET_JOINT_NAME;	
	setJointChannelPos(woJointName, frame, newTran);
	setJointChannelQuat(woJointName, frame, newRot);
}

void MeCtMotionGraph::controller_map_updated()
{

}

void MeCtMotionGraph::controller_start()
{
	controlRestart();
}
