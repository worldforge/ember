#include "controllers/me_ct_constraint.hpp"
#include <assert.h>
#include <boost/foreach.hpp>
#include <sr/sr_timer.h>
#include <sbm/gwiz_math.h>
using namespace gwiz;

std::string MeCtConstraint::CONTROLLER_TYPE = "Constraint";

bool MeCtConstraint::useIKConstraint = true;

EffectorJointConstraint::EffectorJointConstraint()
{
	rotOffset = SrQuat();
	posOffset = SrVec();
}

EffectorJointConstraint& EffectorJointConstraint::operator=( const EffectorJointConstraint& rhs )
{
	//node = rhs.node;
	efffectorName = rhs.efffectorName;
	rootName    = rhs.rootName;
	targetJoint = rhs.targetJoint;
	rotOffset = rhs.rotOffset;//SrQuat(SrVec(0,1,0),M_PI);
	posOffset = rhs.posOffset;
	return *this;
}

SrQuat EffectorJointConstraint::getRotConstraint()
{
	SrQuat target;		
	if( targetJoint )	
	{
		SrMat sr_M;		
		targetJoint->update_gmat_up();
		sr_M = targetJoint->gmat();			
		target = SrQuat(sr_M)*rotOffset;///rotOffset*SrQuat(sr_M);//SrQuat(sr_M)*rotOffset;//rotOffset*SrQuat(sr_M);		
	}
	return target;
}

SrVec EffectorJointConstraint::getPosConstraint()
{
	SrVec target;		
	if( targetJoint )	
	{
		SrMat sr_M;
		matrix_t M;			
		targetJoint->update_gmat_up();
		sr_M = targetJoint->gmat();			
		target = SrVec(sr_M.get(12),sr_M.get(13),sr_M.get(14)) + posOffset*SrQuat(sr_M);			
	}
	return target;
}

/************************************************************************/
/*  Generic Fading Control                                              */
/************************************************************************/

FadingControl::FadingControl()
{
	fadeMode = FADING_MODE_OFF;
	blendWeight = 0.0;
	prev_time = 0.0;
	restart = false;
}

bool FadingControl::updateFading( float dt )
{
	const float FADE_EPSILON = 0.001f;
	bool finishFadeOut = false;
	if (fadeMode)
	{
		fadeRemainTime -= dt;
		if (fadeRemainTime <= 0.0)
			fadeRemainTime = 0.0;

		if (fadeMode == FADING_MODE_IN)
		{			
			float fadeNormal = 1.f - (float)fadeRemainTime/fadeInterval;
			blendWeight = fadeNormal;
			if (blendWeight > 1.0 - FADE_EPSILON)
			{
				blendWeight = 1.0;
				fadeMode = FADING_MODE_OFF;
			}						
		}
		else
		{
			float fadeNormal = fadeRemainTime/fadeInterval;
			blendWeight = fadeNormal;
			if (blendWeight < FADE_EPSILON)
			{
				blendWeight = 0.0;
				fadeMode = FADING_MODE_OFF;
				finishFadeOut = true;
				//useIKConstraint = false;
			}	
		}
	}
	return finishFadeOut;
}


void FadingControl::setFadeIn( float interval )
{
	if (blendWeight == 1.0 && fadeMode == FADING_MODE_OFF)
		return;

	fadeInterval = interval;
	fadeRemainTime = interval;
	fadeMode = FADING_MODE_IN;
	//useIKConstraint = true;
}

void FadingControl::setFadeOut( float interval )
{
	if (blendWeight == 0.0 && fadeMode == FADING_MODE_OFF)
		return;

	fadeInterval = interval;
	fadeRemainTime = interval;
	fadeMode = FADING_MODE_OUT;
}

void FadingControl::controlRestart()
{
	restart = true;
}

void FadingControl::updateDt( float curTime )
{
	if (restart)
	{
		dt = 0.f;
		restart = false;
	}
	else
	{
		dt = curTime - prev_time;
	}	
	prev_time = curTime;
}


MeCtConstraint::MeCtConstraint( SmartBody::SBSkeleton* skeleton )  : FadingControl()
{
	_skeleton = skeleton;
	prev_time = -1.0;	
	_duration = -1.0f;
}

MeCtConstraint::~MeCtConstraint(void)
{
	
}

void MeCtConstraint::updateChannelBuffer(MeFrameData& frame, std::vector<SrQuat>& quatList, bool bRead)
{
	SrBuffer<float>& buffer = frame.buffer();
	int count = 0;

#if 0
	BOOST_FOREACH(SrQuat& quat, quatList)
	{
		int index = frame.toBufferIndex(_toContextCh[count++]);	
		if (index == -1)
		{
			if (bRead)
			{
				quat = SrQuat();
			}
		}
		else
		{
			if (bRead)
			{
				quat.w = buffer[index] ;
				quat.x = buffer[index + 1] ;
				quat.y = buffer[index + 2] ;
				quat.z = buffer[index + 3] ;			
			}
			else
			{
				buffer[index] = quat.w;
				buffer[index + 1] = quat.x;
				buffer[index + 2] = quat.y;
				buffer[index + 3] = quat.z;
			}
		}				
	}	
#else
	const IKTreeNodeList& nodeList = ik_scenario.ikTreeNodes;		
	BOOST_FOREACH(SrQuat& quat, quatList)
	{
		int chanId;
		int index;
		SkJoint* joint = nodeList[count++]->joint;
		chanId = _context->channels().search(joint->getMappedJointName(), SkChannel::Quat);
		if (chanId < 0)
			continue;

		index = _context->toBufferIndex(chanId);
		if (index < 0 )
		{
			if (bRead)
			{
				quat = SrQuat();
			}
		}
		else
		{
			if (bRead)
			{
				quat.w = buffer[index] ;
				quat.x = buffer[index + 1] ;
				quat.y = buffer[index + 2] ;
				quat.z = buffer[index + 3] ;			
			}
			else
			{
				buffer[index] = quat.w;
				buffer[index + 1] = quat.x;
				buffer[index + 2] = quat.y;
				buffer[index + 3] = quat.z;
			}
		}			
	}
#endif
}


void MeCtConstraint::init(SmartBody::SBCharacter* sbChar, const char* rootJointName)
{
	assert(_skeleton);	
	// root is "world_offset", so we use root->child to get the base joint.
	SmartBody::SBJoint* rootJoint = _skeleton->getJointByName(rootJointName);//_skeleton->root()->child(0);//_skeleton->search_joint("l_acromioclavicular");//_skeleton->root()->child(0);//_skeleton->search_joint("l_acromioclavicular");//_skeleton->root()->child(0);//_skeleton->search_joint("base"); // test for now
	if (!rootJoint)
		rootJoint = dynamic_cast<SmartBody::SBJoint*>(_skeleton->root()->child(0)); // use base joint by default

	_sbChar = sbChar;
	std::vector<std::string> stopJoints;
	ik_scenario.buildIKTreeFromJointRoot(rootJoint,stopJoints);
	ik_scenario.ikPosEffectors = &posConstraint;
	ik_scenario.ikRotEffectors = &rotConstraint;

	const IKTreeNodeList& nodeList = ik_scenario.ikTreeNodes;		
// 	for (int i=0;i<3;i++)
// 		_channels.add(rootJoint->name().get_string(), (SkChannel::Type)(SkChannel::XPos+i));
	for (unsigned int i=0;i<nodeList.size();i++)
	{
		SkJoint* joint = nodeList[i]->joint;
		_channels.add(joint->getMappedJointName(), SkChannel::Quat);	
	}	

	double ikReachRegion = _sbChar->getHeight()*0.02f;		
	ikDamp        = ikReachRegion*ikReachRegion*14.0;
	MeController::init(sbChar);
}

void MeCtConstraint::controller_map_updated() 
{		
	
}

bool MeCtConstraint::controller_evaluate( double t, MeFrameData& frame )
{	
	//SrTimer time;
	//time.start();
	//float dt = 0.001f;
	std::vector<SrQuat> tempQuatList; tempQuatList.resize(ik_scenario.ikTreeNodes.size());
	if (prev_time == -1.0) // first start
	{		
		// for first frame, update from frame buffer to joint quat in the limb
		// any future IK solving will simply use the joint quat from the previous frame.		
		updateChannelBuffer(frame,tempQuatList,true);			
		ik_scenario.setTreeNodeQuat(tempQuatList,QUAT_INIT);
		ik_scenario.setTreeNodeQuat(tempQuatList,QUAT_PREVREF);
		ik_scenario.setTreeNodeQuat(tempQuatList,QUAT_CUR);
	}
	updateDt((float)t);
// 	else
// 	{		
// 		dt = ((float)(t-prev_time));
// 	}

	updateChannelBuffer(frame,tempQuatList,true);
	ik_scenario.setTreeNodeQuat(tempQuatList,QUAT_REF);
			
	prev_time = (float)t;

	//assert(ik_scenario.ikEndEffectors.size() == targetJointList.size());
// 	for (int i=0;i<targetJointList.size();i++)
// 	{
// 		ik_scenario.ikEndEffectors[i]->targetPos = get_reach_target(targetJointList[i]);
// 	}

	ConstraintMap::iterator ci;

	//for (unsigned int i=0;i<rotConstraint.size();i++)
	for (ci = rotConstraint.begin(); ci != rotConstraint.end(); ci++)
	{
		EffectorConstraint* cons = ci->second;//rotConstraint[i];
		MeCtIKTreeNode* node = ik_scenario.findIKTreeNode(cons->efffectorName.c_str());
		node->targetDir = cons->getRotConstraint();
	}

	for (ci = posConstraint.begin(); ci != posConstraint.end(); ci++)
	{
		EffectorConstraint* cons = ci->second;//posConstraint[i];
		MeCtIKTreeNode* node = ik_scenario.findIKTreeNode(cons->efffectorName.c_str());
		node->targetPos = cons->getPosConstraint();
	}	


	_skeleton->update_global_matrices();
	ik_scenario.ikTreeRoot->joint->parent()->update_gmat_up(); // update world offset global transformation	
	ik_scenario.ikGlobalMat = ik_scenario.ikTreeRoot->joint->parent()->gmat();
	for (int i=0;i<3;i++)
		ik_scenario.ikTreeRootPos[i] = ik_scenario.ikTreeRoot->joint->pos()->value(i);

	
	ik.setDt(dt);
	if (fadeMode == FADING_MODE_IN)
		useIKConstraint = true;

	bool finishFadeOut = updateFading(dt);	
	if (finishFadeOut)
		useIKConstraint = false;

	if (useIKConstraint) //&& ik_scenario.ikEndEffectors.size() != 0)	
	{
		{
			//sr_out << "global offset mat = " << ik_scenario.ikGlobalMat << srnl;
			ik.dampJ = ikDamp;
			ik.refDampRatio = 0.01;
			for (int i=0;i<1;i++)
			{
				//printf("IK Iteration %d\n",i);
				ik.update(&ik_scenario);
				//ik_scenario.updateGlobalMat();
				ik_scenario.copyTreeNodeQuat(QUAT_CUR,QUAT_INIT);
			}
			
			//ik_scenario.ikInitQuatList = ik_scenario.ikQuatList;
			ik_scenario.getTreeNodeQuat(tempQuatList,QUAT_CUR);			
			for (unsigned int i=0;i<ik_scenario.ikTreeNodes.size();i++)
			{
				MeCtIKTreeNode* node = ik_scenario.ikTreeNodes[i];
				SrQuat qEval = node->getQuat(QUAT_CUR);//ik_scenario.ikQuatList[i];
				SrQuat qInit = node->getQuat(QUAT_REF);
				qEval.normalize();
				qInit.normalize();
				tempQuatList[i] = slerp(qInit,qEval,blendWeight);	
				//tempQuatList[i].normalize();
				//node->setQuat(tempQuatList[i],QUAT_INIT);
			}			
		}
		updateChannelBuffer(frame,tempQuatList);
		//ik_scenario.copyTreeNodeQuat(QUAT_REF,QUAT_PREVREF);
	}
	//printf("Time = %f\n",time.t());	
	return true;
}

void MeCtConstraint::controller_start()
{
	controlRestart();
}

void MeCtConstraint::print_state(int tabs)
{
}

bool MeCtConstraint::addEffectorJointPair( SmartBody::SBJoint* targetJoint, const char* effectorName, const char* effectorRootName, const SrVec& posOffset, const SrQuat& rotOffset, ConstraintType cType )
{
	MeCtIKTreeNode* node = ik_scenario.findIKTreeNode(effectorName);
	MeCtIKTreeNode* rootNode = ik_scenario.findIKTreeNode(effectorRootName);	

	if (!node)
		return false;

	std::string rootName = effectorRootName;

	if (!rootNode)
		rootName = ik_scenario.ikTreeRoot->getNodeName();

	// separate position & rotation constraint
	//ConstraintList& jEffectorList = (cType == CONSTRAINT_ROT) ? rotConstraint : posConstraint;
	ConstraintMap& jEffectorMap = (cType == CONSTRAINT_ROT) ? rotConstraint : posConstraint;
	//VecOfString& effectorList  = (cType == CONSTRAINT_ROT) ? ik_scenario.ikRotEffectors : ik_scenario.ikPosEffectors;

	std::string str = effectorName;
	//assert(jEffectorList.size() == effectorList.size());
	//std::find()

	//int idx = distance(effectorList.begin(), find(effectorList.begin(),effectorList.end(),str));
	//int idx = //MeCtIKTreeScenario::findIKTreeNodeInList(effectorName,effectorList);
	ConstraintMap::iterator ci = jEffectorMap.find(str);
	if (ci != jEffectorMap.end())//idx != effectorList.size())
	{
		//jEffectorList[idx].targetJoint = targetJoint;	
		//EffectorJointConstraint& cons = jEffectorList[idx];
		EffectorJointConstraint* cons = dynamic_cast<EffectorJointConstraint*>((*ci).second);
		cons->targetJoint = targetJoint;
		cons->rootName = rootName;//effectorRootName;
		cons->posOffset = posOffset;
		cons->rotOffset = rotOffset;
		
	}
	else // add effector-joint pair
	{
		// initialize constraint
		EffectorJointConstraint* cons = new EffectorJointConstraint();
		//constraint.node = node;
		cons->efffectorName = effectorName;
		cons->targetJoint = targetJoint;
		cons->rootName = rootName;//effectorRootName;
		cons->posOffset = posOffset;
		cons->rotOffset = rotOffset;
		jEffectorMap[str] = cons;
		//effectorList.push_back(effectorName);
		//jEffectorList.push_back(cons);		
	}
	return true;
}




