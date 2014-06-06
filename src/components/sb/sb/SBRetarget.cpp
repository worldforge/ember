#include "SBRetarget.h"
#include <sb/SBSkeleton.h>
#include <sb/SBJoint.h>
#include <sb/SBScene.h>
#include <sk/sk_motion.h>
#include <sb/SBMotion.h>
#include <queue>

#define ELITE_HACK 0

namespace SmartBody {


SBAPI SBRetarget::SBRetarget()
{
	
}

SBAPI SBRetarget::SBRetarget(std::string srcName, std::string tgtName)
{
	srcSkName = srcName;
	tgtSkName = tgtName;
}

SBAPI SBRetarget::~SBRetarget()
{

}

bool SBRetarget::initRetarget( std::vector<std::string>& endJoints, std::vector<std::string>& relativeJoints )
{
	SmartBody::SBScene* scene = SmartBody::SBScene::getScene();
	SmartBody::SBSkeleton* targetSk = scene->getSkeleton(tgtSkName);
	SmartBody::SBSkeleton* sourceSk = scene->getSkeleton(srcSkName);	
	if (!targetSk || !sourceSk) return false;
	
	SmartBody::SBSkeleton* interSk = new SmartBody::SBSkeleton(targetSk); // copy for an intermediate skeleton
	SmartBody::SBSkeleton* tempSrcSk = new SmartBody::SBSkeleton(sourceSk);
	std::vector<std::string> stopJoints;	
	stopJoints.push_back("skullbase");

	retargetEndJoints = endJoints;
	retargetRelativeJoints = relativeJoints;
	
	tempSrcSk->clearJointValues();
	interSk->clearJointValues();
	tempSrcSk->invalidate_global_matrices();
	tempSrcSk->update_global_matrices();
	interSk->invalidate_global_matrices();
	interSk->update_global_matrices();

#if 0 // don't apply root pre-rotation ( still experimental )
	SrVec srcFaceDir = tempSrcSk->getFacingDirection();
	SrVec tgtFaceDir = interSk->getFacingDirection();
	SrQuat offsetRot = SrQuat(srcFaceDir,tgtFaceDir);
	sr_out << "srcFaceDir = " << srcFaceDir;
	sr_out << "tgtFaceDir = " << tgtFaceDir;
	sr_out << "offset rot = " << offsetRot << srnl;
	tempSrcSk->root()->quat()->prerot(offsetRot);
#endif
	
	std::map<std::string, SrQuat> jointRotationMap;
	std::queue<std::string> jointQueues;
	std::string rootName = "base";
	//jointQueues.push(interSk->root()->name());	
	jointQueues.push(rootName);
	while (!jointQueues.empty())
	{
		std::string pjointName = jointQueues.front();		
		jointQueues.pop();				
		if (std::find(stopJoints.begin(),stopJoints.end(),pjointName) != stopJoints.end())
			continue;

		SkJoint* srcJoint = tempSrcSk->search_joint(pjointName.c_str());
		SkJoint* targetJoint = interSk->search_joint(pjointName.c_str());
		bool isRelativeJoint = false;
		bool isEndJoint = false;
		
		// just copy over joint quat if that is a root or relative joint
		if (pjointName == rootName || std::find(relativeJoints.begin(),relativeJoints.end(), pjointName) != relativeJoints.end())
			isRelativeJoint = true;
		if (std::find(endJoints.begin(),endJoints.end(), pjointName) != endJoints.end())
			isEndJoint = true;

		if ( isRelativeJoint && targetJoint) 
		{
			// don't change the t-pose for these joints
			for (int i=0;i<targetJoint->num_children();i++)
			{					
				jointQueues.push(targetJoint->child(i)->getMappedJointName());
			}
		}	
		else if ( isEndJoint && targetJoint)
		{
			// stop aligning children
		}
		else
		{
			SkMotion::convertBoneOrientation(pjointName, interSk, tempSrcSk, endJoints);
			interSk->invalidate_global_matrices();
			interSk->update_global_matrices();
			SkJoint* pjoint = interSk->search_joint(pjointName.c_str());	
			if (!pjoint)
				continue;			
			for (int i=0; i< pjoint->num_children(); i++)
			{
				//LOG("target joint = %s, child = %s",pjoint->name().c_str(), pjoint->child(i)->name().c_str());
				SkJoint* child = pjoint->child(i);
				jointQueues.push(child->getMappedJointName());
			}
		}				
	}
	heightRatio = (interSk->getBaseHeight("base")/tempSrcSk->getBaseHeight("base"));//*0.99f;
	//LOG("height ratio = %f", heightRatio);

	std::vector<std::string> srcJointNames = tempSrcSk->getJointMappedNames();
	for (unsigned int i=0;i<srcJointNames.size();i++)
	{
		std::string jname = srcJointNames[i];
		SmartBody::SBJoint* srcJoint = tempSrcSk->getJointByName(jname);
		SmartBody::SBJoint* tgtJoint = interSk->getJointByName(jname);		
		bool isEndJoint = false;
		if (std::find(endJoints.begin(),endJoints.end(), jname) != endJoints.end())
			isEndJoint = true;

#if ELITE_HACK
		if (jname == "r_sternoclavicular" || jname == "l_sternoclavicular" || jname == "r_acromioclavicular" || jname == "l_acromioclavicular")
			jointSkipMap[jname] = true;//continue;
#endif

		if (srcJoint && tgtJoint ) //&& std::find(endJoints.begin(),endJoints.end(), jname) == endJoints.end())
		{					
			SrQuat gsrc = SrQuat(srcJoint->gmat());
			SrQuat gdst = SrQuat(tgtJoint->gmat());
			SrQuat protSrc = srcJoint->quat()->rawValue();
			SrQuat protDst = tgtJoint->quat()->rawValue();

			SrQuat finalPreQuat = protDst*gdst.inverse()*gsrc*protSrc.inverse();
			SrQuat finalPostQuat = gsrc.inverse()*gdst;
			
			SrQuat finalRot = finalPreQuat*finalPostQuat;
			jointPrePostRotMap[jname] = QuatPair(finalPreQuat,finalPostQuat);


			//final_q = protDst*gdst.inverse()*gsrc*protSrc.inverse()*q_orig*gsrc.inverse()*gdst;
			//final_q = protDst;//gdst.inverse()*gsrc*protSrc.inverse()*q_orig*gsrc.inverse()*gdst*protDst;
			
			//final_q.normalize();
		}
	}

	// clear intermediate skeletons
	if (interSk)
		delete interSk;
	interSk = NULL;
	if (tempSrcSk)
		delete tempSrcSk;
	tempSrcSk = NULL;

	return true;

#if 0 // not doing motion retargeting here
	for (int i = 0; i < num_f; i++)
	{
		retarget_p->insert_frame(i, this->keytime(i));		
		float *ref_p = this->posture( i );
		float *new_p = retarget_p->posture( i );
		//tempSrcSk.update_global_matrices(); // update global values for the temp source skeleton
		for (int k=0;k<mchan_arr.size();k++)
		{
			SkChannel& chan = mchan_arr[k];			
			const std::string& jointName = mchan_arr.name(k);				
			// map pre-rotation
			int index = mchan_arr.float_position(k);
			if (chan.type == SkChannel::Quat)
			{
				// flip rotation for y,z axis
				SrQuat q_orig = SrQuat( ref_p[ index ], ref_p[ index + 1 ], ref_p[ index + 2 ], ref_p[ index + 3 ] );
				SrQuat final_q = q_orig;
				//SrVec qa = q_orig.axisAngle();
				//SrVec qa_rot = qa*srcToTargetRot;	
				//SrQuat final_q = jointRot;//*SrQuat(qa_rot);	
				SkJoint* srcJoint = tempSrcSk->search_joint(jointName.c_str());
				SkJoint* targetJoint = interSk->search_joint(jointName.c_str());			
				if (srcJoint && targetJoint)
				{					
					SrQuat gsrc = SrQuat(srcJoint->gmat());
					SrQuat gdst = SrQuat(targetJoint->gmat());
					SrQuat protSrc = srcJoint->quat()->rawValue();
					SrQuat protDst = targetJoint->quat()->rawValue();//targetJoint->quat()->prerot();//targetJoint->quat()->rawValue();//targetJoint->quat()->prerot();
					//sr_out << "gsrc = " << gsrc << " gdst = " << gdst << srnl;
					final_q = protDst*gdst.inverse()*gsrc*protSrc.inverse()*q_orig*gsrc.inverse()*gdst;
					//final_q = protDst;//gdst.inverse()*gsrc*protSrc.inverse()*q_orig*gsrc.inverse()*gdst*protDst;
					final_q.normalize();
				}

				new_p[ index + 0 ] = (float)final_q.w;
				new_p[ index + 1 ] = (float)final_q.x;
				new_p[ index + 2 ] = (float)final_q.y;
				new_p[ index + 3 ] = (float)final_q.z;
			}
			else if (chan.type == SkChannel::XPos || chan.type == SkChannel::YPos || chan.type == SkChannel::ZPos)
			{
				// just copy over the translation for now
				float chanValue = ref_p[ index ];				
				new_p[ index ] = chanValue*heightRatio;
				//LOG("jointName = %s, new_p = %f",jointName.c_str(), new_p[index]);
				// 				if (jointPosMap.find(jointName) != jointPosMap.end())
				// 				{
				// 					SrVec pos = jointPosMap[jointName];
				// 					new_p[index] = pos[chan.type - SkChannel::XPos];
				// 				}
			}
		}		
	}	
#endif
}

SrQuat SBRetarget::applyRetargetJointRotation( std::string jointName, SrQuat& inQuat )
{
	SrQuat outQuat ; // identity rotation 	
	if (jointPrePostRotMap.find(jointName) != jointPrePostRotMap.end())
	{
		QuatPair& qpair = jointPrePostRotMap[jointName];		
#if ELITE_HACK
		if (jointName == "l_shoulder" && jointAddRotMap.find("l_sternoclavicular") != jointAddRotMap.end())
			inQuat = jointAddRotMap["l_sternoclavicular"]*inQuat;
		if (jointName == "r_shoulder" && jointAddRotMap.find("r_sternoclavicular") != jointAddRotMap.end())
			inQuat = jointAddRotMap["r_sternoclavicular"]*inQuat;
#endif	
		// add joint rotation offset
		if (jointRotOffsetMap.find(jointName) != jointRotOffsetMap.end())
		{
			inQuat = jointRotOffsetMap[jointName]*inQuat;
		}

		if (jointSkipMap.find(jointName) != jointSkipMap.end())
		{			
			//outQuat = qpair.first*qpair.second;
			outQuat = SrQuat();
#if ELITE_HACK
			SrQuat newInQuat = SrQuat(inQuat.axisAngle()*0.7f);
			jointAddRotMap[jointName] = newInQuat;
#endif
		}
		else
			outQuat = qpair.first*inQuat*qpair.second;	
	}
	return outQuat;
}


SrQuat SBRetarget::applyRetargetJointRotationInverse( std::string jointName, SrQuat& inQuat )
{
	SrQuat outQuat; // identity rotation 
	if (jointPrePostRotMap.find(jointName) != jointPrePostRotMap.end())
	{
		QuatPair& qpair = jointPrePostRotMap[jointName];		
		outQuat = qpair.first.inverse()*inQuat*qpair.second.inverse();
	}
	return outQuat;
}

float SBRetarget::applyRetargetJointTranslation( std::string jointName, float inPos )
{
	float outPos = inPos;
	if (jointPrePostRotMap.find(jointName) != jointPrePostRotMap.end())
	{
		outPos = inPos*heightRatio;
	}
	return outPos;
}


SBAPI SrVec SBRetarget::applyRetargetJointTranslationVec( std::string jointName, SrVec inVec )
{
	SrVec outPos = inVec;
	if (jointPrePostRotMap.find(jointName) != jointPrePostRotMap.end())
	{
		outPos = inVec*heightRatio;
	}
	return outPos;
}

SBAPI SrVec SBRetarget::applyRetargetJointTrajectory( TrajectoryRecord& trajRecord, SrMat& baseGmat )
{
	SrVec outPos = trajRecord.refJointGlobalPos;
	SrVec outOffset = trajRecord.jointTrajLocalOffset*getHeightRatio();
	trajRecord.jointTrajGlobalPos = outPos + outOffset*baseGmat.get_rotation();	
	return trajRecord.jointTrajGlobalPos;
}

SBAPI std::vector<std::string> SBRetarget::getEndJointNames()
{
	return retargetEndJoints;
}

SBAPI std::vector<std::string> SBRetarget::getRelativeJointNames()
{
	return retargetRelativeJoints;
}

SBAPI float SBRetarget::getHeightRatio()
{
	return heightRatio;
}

SBAPI void SBRetarget::addJointRotOffset( std::string jointName, SrQuat& inQuat )
{
	jointRotOffsetMap[jointName] = inQuat;
}

void SBRetarget::computeJointLengthRatio( std::string jointName, std::string refJointName )
{
	SmartBody::SBSkeleton* srcSkel = SmartBody::SBScene::getScene()->getSkeleton(srcSkName);
	SmartBody::SBSkeleton* tgtSkel = SmartBody::SBScene::getScene()->getSkeleton(tgtSkName);
	if (!srcSkel || !tgtSkel)
		return; 
	SmartBody::SBJoint* srcJoint = srcSkel->getJointByName(jointName);
	SmartBody::SBJoint* tgtJoint = tgtSkel->getJointByName(jointName);

	// to-do : compute joint length ratio between source skeleton and target skeleton
}

}