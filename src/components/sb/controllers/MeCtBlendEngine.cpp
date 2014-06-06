#include <assert.h>
#include <sstream>
#include <algorithm>
#include <time.h>
#include <boost/foreach.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <sr/sr_timer.h>
#include "MeCtBlendEngine.h"

#include "controllers/me_ct_barycentric_interpolation.h"
#include "controllers/me_ct_inverse_interpolation.h"
#include <sb/SBEvent.h>
#include <sb/SBMotion.h>


/************************************************************************/
/* Blend Engine                                                         */
/************************************************************************/

MeCtBlendEngine::MeCtBlendEngine(SmartBody::SBSkeleton* sk, std::string rootName)
{
	//character = sbmChar;
	skeletonCopy = new SmartBody::SBSkeleton(sk); 
	skeletonCopy->ref();
	skeletonRef  = sk;
	rootJointName = rootName;
	dataInterpolator = NULL;
	refMotion = NULL;
	valid = false;	
	interpMotion = NULL;
	motionParameter = NULL;
	simplexList = NULL;
	
}

void MeCtBlendEngine::init(const std::string& paramFuncType)
{
	SkJoint* rootJoint = skeletonCopy->search_joint(rootJointName.c_str());		
	std::vector<std::string> stopJoints;
	ikScenario.buildIKTreeFromJointRoot(rootJoint,stopJoints);
	affectedJoints.clear();	
	const IKTreeNodeList& nodeList = ikScenario.ikTreeNodes;
	idleMotionFrame.jointQuat.resize(nodeList.size());
	inputMotionFrame.jointQuat.resize(nodeList.size());
	for (unsigned int i=0;i<nodeList.size();i++)
	{
		MeCtIKTreeNode* node = nodeList[i];
		SmartBody::SBJoint* joint = skeletonCopy->getJointByName(node->getNodeName());		
		affectedJoints.push_back(joint);			
	}

	if (paramFuncType == "reach")
	{
		SmartBody::SBJoint* copyEffector = skeletonCopy->getJointByMappedName("r_wrist");
		SmartBody::SBJoint* copyRoot = skeletonCopy->getJointByName("base");
		motionParameter = new ReachMotionParameter(skeletonCopy,affectedJoints,copyEffector,copyRoot);
	}
	else if (paramFuncType == "jump")
	{		
		motionParameter = new JumpParameter(skeletonCopy,affectedJoints,"base");		
	}
	else if (paramFuncType == "kick")
	{
		SmartBody::SBJoint* copyEffector = skeletonCopy->getJointByMappedName("r_ankle");
		SmartBody::SBJoint* copyRoot = skeletonCopy->getJointByMappedName("base");
		motionParameter = new ReachMotionParameter(skeletonCopy,affectedJoints,copyEffector,copyRoot);
	}
	else if (paramFuncType == "punch")
	{

	}
	else if (paramFuncType == "locomotion")
	{
		motionParameter = new LocomotionParameter(skeletonCopy, affectedJoints, "base");
	}
	motionExamples.initMotionExampleSet(motionParameter);	
}

MeCtBlendEngine::~MeCtBlendEngine( void )
{	
	//skeletonCopy->unref();
	if (skeletonCopy)
		delete skeletonCopy;
}


void MeCtBlendEngine::getMotionParameter( const std::string& motion, dVector& outPara )
{
	MotionExample* ex = motionExamples.getMotionExample(motion);	
	if (ex)
		outPara = ex->parameter;
}


void MeCtBlendEngine::updateMotionExamples( const std::vector<SmartBody::SBMotion*>& inMotionSet, std::string interpolatorType )
{
	if (inMotionSet.size() == 0)
		return;

	// set world offset to zero	
	std::string rootName = rootJointName;
	SkJoint* root = skeletonRef->search_joint(rootName.c_str());
	if (root)
	{
		root->quat()->value(SrQuat());
	}
	skeletonCopy->root()->quat()->value(SrQuat());
	for (int i=0;i<3;i++)
	{
		skeletonCopy->root()->pos()->value(i,0.f);
		if (root)
			root->pos()->value(i,0.f);
	}	

	SmartBody::SBJoint* rootJoint = affectedJoints[0];
	for (unsigned int i=0; i < inMotionSet.size(); i++)
	{		
		SmartBody::SBMotion* motion = dynamic_cast<SmartBody::SBMotion*>(inMotionSet[i]);
		if (!motion)
			continue;
		if (std::find(motionData.begin(), motionData.end(), motion) != motionData.end())
			continue; // we do not process example motions that are already used for this controller instance
		if (!refMotion)
			refMotion = motion;

		motionData.push_back(motion);
		MotionExample* ex = new MotionExample();
		ex->motion = motion;		
		ex->timeWarp = new SimpleTimeWarp(refMotion->duration(),motion->duration());
		ex->motionParameterFunc = motionParameter;
		ex->motionProfile = new MotionProfile(motion);
		ex->updateRootOffset(skeletonCopy,rootJoint);
#if 0
		ex->motionProfile->buildVelocityProfile(0.f,motion->duration()*0.999f,0.005f);
		ex->motionProfile->buildInterpolationProfile(0.f,(float)motion->time_stroke_emphasis(),0.005f);
		ex->motionProfile->buildEulerCurveProfile(0.f,(float)motion->time_stroke_emphasis(),0.005f);
#endif
		ex->getMotionParameter(ex->parameter);		
		// set initial index & weight for the motion example
		// by default, the index should be this motion & weight should be 1
		InterpWeight w;
		w.first = motionExamples.getExamples().size();
		w.second = 1.f;
		ex->weight.push_back(w);	
		// add the example parameter for visualization purpose
 		SrVec reachPos;
 		for (int i=0;i<3;i++)
 			reachPos[i] = (float)ex->parameter[i];
		//LOG("reach pos =  %f %f %f",reachPos[0], reachPos[1], reachPos[2]);
// 		examplePts.push_back(reachPos);		
		motionExamples.addMotionExample(ex);
	}	

	if (motionExamples.getExamples().empty()) // no examples
	{
		//valid = false;
		return;
	}

	if (dataInterpolator)
		delete dataInterpolator;

	dataInterpolator = createInterpolator(interpolatorType);
	dataInterpolator->init(&motionExamples);
	dataInterpolator->buildInterpolator();		


	if (resampleData)
	{
		for (unsigned int i=0;i<resampleData->size();i++)
		{
			InterpolationExample* ex = (*resampleData)[i];
			SrVec reachPos;
			for (int k=0;k<3;k++)
				reachPos[k] = (float)ex->parameter[k];
			resamplePts.push_back(reachPos);		
		}
	}


	if (interpMotion)
		delete interpMotion;
	interpMotion = createInterpMotion();	
}

void MeCtBlendEngine::updateSkeletonCopy()
{
	skeletonCopy->root()->quat()->value(skeletonRef->root()->quat()->value());
	for (int i=0;i<3;i++)
		skeletonCopy->root()->pos()->value(i,skeletonRef->root()->pos()->value(i));
}

DataInterpolator* MeCtBlendEngine::createInterpolator(std::string interpolatorType)
{
	DataInterpolator* interpolator = NULL;
	
	if (interpolatorType == "KNN")
	{
		float sampleRadius = 0.001f;//skeletonCopy->getCurrentHeight()*0.02f;
		KNNInterpolator* knnInterpolator = new KNNInterpolator(100,sampleRadius);
		resampleData = &knnInterpolator->resampleData;	
		interpolator = knnInterpolator;
	}
	else if (interpolatorType == "Inverse")
	{
		interpolator = new InverseInterpolation();
		resampleData = NULL;
	}
	else if (interpolatorType == "RBF")
	{
		interpolator = new RBFInterpolator();
		resampleData = NULL;
	}
	else if (interpolatorType == "Barycentric")
	{
		BarycentricInterpolator* baryInterpolator = new BarycentricInterpolator();
		interpolator = baryInterpolator;
		resampleData = NULL;
		simplexList = &baryInterpolator->simplexList;
	}
	//RBFInterpolator* interpolator = new RBFInterpolator();	 	
	interpExampleData = interpolator->getInterpExamples();
	return interpolator;
}

ResampleMotion* MeCtBlendEngine::createInterpMotion()
{
	ResampleMotion* ex = new ResampleMotion(motionExamples.getMotionData());
	ex->motionParameterFunc = motionParameter;
	ex->motionProfile = NULL;
	return ex;
}

void MeCtBlendEngine::updateBlend(float t, float dt, BodyMotionFrame& inputFrame)
{
	float du = 0.0;
	
	inputMotionFrame = inputFrame;		
	skeletonRef->update_global_matrices();	
	updateSkeletonCopy();	
	// update reach data
	std::string rootName = rootJointName;	
		

	VecOfInterpWeight weight;
	dataInterpolator->predictInterpWeights(targetParameter,weight);	
	interpMotion->getMotionFrame(curRefTime,motionParameter->skeletonRef,motionParameter->affectedJoints,outputMotionFrame);		
	du = (float)interpMotion->getRefDeltaTime(curRefTime,dt);
	curRefTime += du;
}

void MeCtBlendEngine::setBlendParameter( dVector& para, std::vector<double>& outWeight )
{
	targetParameter = para;

	VecOfInterpWeight weight;
	dataInterpolator->predictInterpWeights(targetParameter,weight);	
	outWeight.resize(motionData.size());
	//for (unsigned int i=0;i<weight.size();i++)
	//	LOG("weight %d = %f",weight[i].first, weight[i].second);
	for (unsigned int i=0;i<outWeight.size();i++)
		outWeight[i] = 0.0;
	for (unsigned int i=0;i<weight.size();i++)
		outWeight[weight[i].first] = weight[i].second;

}

void MeCtBlendEngine::getBlendParameterFromWeights( dVector& outPara, std::vector<double>& inWeight )
{
	VecOfInterpWeight weight;
	for (unsigned int i=0;i<inWeight.size();i++)
	{
		if (inWeight[i] > 0.0)
			weight.push_back(InterpWeight(i,(float)inWeight[i]));
	}
	interpMotion->weight = weight;
	// a quick and dirty way to combine the parameters, but are they the right way ?
	for (unsigned int i=0;i<weight.size();i++)
	{
		int index = weight[i].first;
		float w = weight[i].second;
		InterpolationExample* ex = (*interpExampleData)[index];
		dVector tempPara;		
		if (i == 0)
		{			
			outPara = ex->parameter*w;
		}
		else
		{
			outPara = outPara + ex->parameter*w;
		}
	}
	//motionParameter->getMotionParameter(interpMotion, outPara);
}