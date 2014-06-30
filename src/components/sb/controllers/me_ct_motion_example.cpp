#include <assert.h>
#include <sstream>
#include <algorithm>
#include <time.h>
#include <boost/foreach.hpp>
#include <sr/sr_euler.h>
#include "controllers/me_ct_motion_example.hpp"
#include "controllers/me_ct_motion_parameter.h"
#include "controllers/me_ct_motion_profile.h"
#include "controllers/me_ct_ublas.hpp"
#include <sb/SBScene.h>
#include <sb/SBRetarget.h>
#include <sb/SBRetargetManager.h>

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

using namespace boost;

BodyMotionFrame& BodyMotionFrame::operator=( const BodyMotionFrame& rhs )
{
	jointQuat = rhs.jointQuat;
	rootPos   = rhs.rootPos;
	return *this;
}

void BodyMotionFrame::setMotionPose( float time, SmartBody::SBSkeleton* skel, const vector<SmartBody::SBJoint*>& affectedJoints,SmartBody::SBMotion* motion, bool retarget)
{
	SkJoint* rootJoint = affectedJoints[0];
	motion->connect(skel);	
	motion->apply((float)time);	
	motion->disconnect();

	if (jointQuat.size() != affectedJoints.size())
		jointQuat.resize(affectedJoints.size());

	SmartBody::SBRetarget* sbRetarget = NULL;
	if (retarget)
	{
		SmartBody::SBRetargetManager* retargetManager = SmartBody::SBScene::getScene()->getRetargetManager();
		sbRetarget = retargetManager->getRetarget(motion->getMotionSkeletonName(),skel->getName());		
	}
	for (unsigned int i=0;i<affectedJoints.size();i++)
	{
		SkJoint* joint = affectedJoints[i];
		SrQuat jq = SrQuat();
		if (joint->quat()->active())
			jq = affectedJoints[i]->quat()->rawValue();	

		if (sbRetarget)
			jq = sbRetarget->applyRetargetJointRotation(joint->getMappedJointName(),jq);
		jointQuat[i] = jq;			
	}	
	rootPos.set(rootJoint->pos()->value());
}

void BodyMotionInterface::getMotionParameter( dVector& outPara )
{
	motionParameterFunc->getMotionParameter(this,outPara);
}


/************************************************************************/
/* BodyMotion                                                           */
/************************************************************************/
BodyMotion::BodyMotion()
{
	motion = NULL;
	timeWarp = NULL;
}

BodyMotion::~BodyMotion()
{
	if (timeWarp)
		delete timeWarp;
	timeWarp = NULL;
}

double BodyMotion::getRefDeltaTime( float u, float dt )
{
	return dt/timeWarp->timeSlope(u);	
}

double BodyMotion::motionPercent( float time )
{
	double rt = timeWarp->timeWarp(time);
	return timeWarp->invTimeWarp(rt)/timeWarp->refTimeLength();
}

void BodyMotion::updateRootOffset(SmartBody::SBSkeleton* skel, SmartBody::SBJoint* rootJoint)
{	
	SmartBody::SBRetarget* retarget = NULL;
	if (skel->getName() != motion->getMotionSkeletonName()) // set retarget ?
	{
		SmartBody::SBScene* scene = SmartBody::SBScene::getScene();
		SmartBody::SBRetargetManager* retargetManager = scene->getRetargetManager();
		retarget = retargetManager->getRetarget(motion->getMotionSkeletonName(),skel->getName());
	}
	motion->connect(skel);		
	motion->apply(0.001f,SkMotion::Linear, 0, retarget);
	SrQuat tempQ = rootJoint->quat()->rawValue();
	SrMat src, mat;
	src = tempQ.get_mat(src);
	float rx, ry, rz;
	const int rotType = 132;
	sr_euler_angles(rotType, src, rx, ry, rz);
	rx = 0.0;
	rz = 0.0;
	sr_euler_mat(rotType, mat, rx, ry, rz);
	quatP = SrQuat(mat);
	rootOffset.set(rootJoint->pos()->value());	
}

SrVec BodyMotion::getMotionBaseTranslation( float time, const std::string& baseName )
{
	double rt = timeWarp->timeWarp(time);
	int idxFrame = (int)(rt/motion->duration()*motion->frames());
	if (idxFrame >= motion->frames()) idxFrame = motion->frames()-1;
	float *fp = motion->posture(idxFrame);
	SkChannelArray& chans = motion->channels();
	SrVec pos;
	for (int i=0;i<3;i++)
	{
		int chanIdx = chans.search(baseName, SkChannel::Type(SkChannel::XPos + i));
		int floatIdx = chans.float_position(chanIdx);
		if (floatIdx >= 0)
		{
			pos[i] = fp[floatIdx];
		}
	}		
	return pos;	
}

SrQuat BodyMotion::getMotionBaseRotation( float time, const std::string& baseName )
{
	double rt = timeWarp->timeWarp(time);
	int idxFrame = (int)(rt/motion->duration()*motion->frames());
	if (idxFrame >= motion->frames()) idxFrame = motion->frames()-1;
	float *fp = motion->posture(idxFrame);
	SkChannelArray& chans = motion->channels();
	SrQuat rot;
	int chanIdx = chans.search(baseName, SkChannel::Quat);
	int floatIdx = chans.float_position(chanIdx);
	if (floatIdx >= 0)
	{
		rot.w = fp[floatIdx+0];
		rot.x = fp[floatIdx+1];
		rot.y = fp[floatIdx+2];
		rot.z = fp[floatIdx+3];
	}			
	return rot;	
}


double BodyMotion::getMotionFrame( float time, SmartBody::SBSkeleton* skel, const vector<SmartBody::SBJoint*>& affectedJoints, BodyMotionFrame& outMotionFrame )
{
	// Because the SkMotion stored its joint quats in an indirect way, it is not straightforward to grab corresponding quats we need.
	// This is a hack to apply the motion on a skeleton, and then get the quat values directly.
	if (affectedJoints.size() == 0)
		return -1.0;

	//SrVec rootOffset = SrVec();
	
	SkJoint* rootJoint = affectedJoints[0];

	SmartBody::SBRetarget* retarget = NULL;
	if (skel->getName() != motion->getMotionSkeletonName()) // set retarget ?
	{
		SmartBody::SBScene* scene = SmartBody::SBScene::getScene();
		SmartBody::SBRetargetManager* retargetManager = scene->getRetargetManager();
		retarget = retargetManager->getRetarget(motion->getMotionSkeletonName(),skel->getName());
	}
	
	motion->connect(skel);	
	double rt = timeWarp->timeWarp(time);
	// get root position
	/*
	motion->apply(0.f);
	SrQuat tempQ = rootJoint->quat()->value();
	SrMat src, mat;
	src = tempQ.get_mat(src);
	float rx, ry, rz;
	const int rotType = 132;
	sr_euler_angles(rotType, src, rx, ry, rz);
	rx = 0.0;
	rz = 0.0;
	sr_euler_mat(rotType, mat, rx, ry, rz);
	SrQuat quatP = SrQuat(mat);
	rootOffset.set(rootJoint->pos()->value());	
	*/

	motion->apply((float)rt,SkMotion::Linear, 0, retarget);	
	motion->disconnect();

	if (outMotionFrame.jointQuat.size() != affectedJoints.size())
		outMotionFrame.jointQuat.resize(affectedJoints.size());

	for (unsigned int i=0;i<affectedJoints.size();i++)
	{
		SkJoint* joint = affectedJoints[i];
		SrQuat jointQuat = SrQuat();
		if (joint->quat()->active())
			jointQuat = affectedJoints[i]->quat()->rawValue();	

		outMotionFrame.jointQuat[i] = jointQuat;			
	}

	// root orientation
	outMotionFrame.jointQuat[0] = quatP.inverse()*outMotionFrame.jointQuat[0];	//outMotionFrame.jointQuat[0];//
	outMotionFrame.rootPos.set(rootJoint->pos()->value());
	outMotionFrame.rootPos = (outMotionFrame.rootPos - rootOffset)*quatP.inverse(); // outMotionFrame.rootPos;//
	return timeWarp->invTimeWarp(rt);
}

double BodyMotion::motionDuration(DurationType durType)
{
	double motionTime = 0.0;
	if (durType == DURATION_REF)
		motionTime = timeWarp->refTimeLength();
	else if (durType == DURATION_ACTUAL)
		motionTime = timeWarp->actualTimeLength();		

	return motionTime;
}

double BodyMotion::strokeEmphasisTime()
{	
	//double emphTime = (motion->time_stroke_end()+motion->time_stroke_emphasis())*0.5f;//motion->time_stroke_emphasis();//motion->time_stroke_emphasis();//(motion->time_stroke_end()+motion->time_stroke_emphasis())*0.5f;	
	double emphTime = motion->time_stroke_emphasis();
	return timeWarp->invTimeWarp(emphTime);
}

/************************************************************************/
/* Motion Example                                                       */
/************************************************************************/

void MotionExample::getExampleParameter( dVector& outPara )
{
	this->getMotionParameter(outPara);
}



/************************************************************************/
/* ResampleMotion                                                       */
/************************************************************************/
ResampleMotion::ResampleMotion( VecOfBodyMotionPtr* motionRef )
{
	motionDataRef = motionRef;
	weight.resize(1);
	InterpWeight& w = this->weight[0];
	w.first = 0;
	w.second = 1.f;
}


double ResampleMotion::getMotionFrame( float time, SmartBody::SBSkeleton* skel, const vector<SmartBody::SBJoint*>& affectedJoints, BodyMotionFrame& outMotionFrame )
{
	// use blended weights to get the motion frame
	return MotionExampleSet::blendMotionFunc(time,skel,affectedJoints,*motionDataRef,weight,outMotionFrame);	
}

SrVec ResampleMotion::getMotionBaseTranslation( float time, const std::string& baseName )
{
	SrVec basePos = SrVec(0.f,0.f,0.f);
	VecOfBodyMotionPtr& motions = *motionDataRef;
	for (unsigned int i=0;i<weight.size();i++)
	{
		int idx = weight[i].first;
		float w = weight[i].second;
		BodyMotionInterface* motion = motions[idx];
		basePos += motion->getMotionBaseTranslation(time, baseName)*w;
	}
	return basePos;		
}

SrQuat ResampleMotion::getMotionBaseRotation( float time, const std::string& baseName )
{
	// 	SrQuat baseRot = SrQuat(0.f,0.f,0.f,0.f);
	// 	VecOfBodyMotionPtr& motions = *motionDataRef;
	// 	for (unsigned int i=0;i<weight.size();i++)
	// 	{
	// 		int idx = weight[i].first;
	// 		float w = weight[i].second;
	// 		BodyMotionInterface* motion = motions[idx];
	// 		//baseRot = baseRot + motion->getMotionBaseRotation(time, baseName)*w;
	// 	}
	// 	baseRot.normalize();

	VecOfBodyMotionPtr& motions = *motionDataRef;
	int idx = weight[0].first;
	double newTime = 0.0;
	float weightSum = weight[0].second;
	BodyMotionInterface* ex = motions[idx];
	float w1,w2;	
	SrQuat outQuat = ex->getMotionBaseRotation(time,baseName);
	for (unsigned int i=1;i<weight.size();i++)
	{
		w1 = weightSum;
		w2 = weight[i].second;
		weightSum += w2;
		idx = weight[i].first;
		if (weightSum == 0.f)
			continue;

		float weight = w2/weightSum;
		float oneMinusWeight = 1.f - weight;
		BodyMotionInterface* motion = motions[idx];
		outQuat = slerp(outQuat,motion->getMotionBaseRotation(time, baseName), weight);
		outQuat.normalize();
	}	
	return outQuat;	
}

double ResampleMotion::motionDuration(DurationType durType)
{
	double motionTime = 0.0;
	VecOfBodyMotionPtr& motions = *motionDataRef;
	for (unsigned int i=0;i<weight.size();i++)
	{
		int idx = weight[i].first;
		float w = weight[i].second;
		BodyMotionInterface* motion = motions[idx];
		motionTime += motion->motionDuration(durType)*w;
	}
	return motionTime;
}


double ResampleMotion::strokeEmphasisTime()
{
	double motionTime = 0.0;
	VecOfBodyMotionPtr& motions = *motionDataRef;
	for (unsigned int i=0;i<weight.size();i++)
	{
		int idx = weight[i].first;
		float w = weight[i].second;
		BodyMotionInterface* motion = motions[idx];
		motionTime += motion->strokeEmphasisTime()*w;
	}
	return motionTime;
}

void ResampleMotion::getExampleParameter( dVector& outPara )
{
	this->getMotionParameter(outPara);
}

double ResampleMotion::motionPercent( float time )
{
	double timePercent = 0.0;
	for (unsigned int i=0;i<weight.size();i++)
	{		
		float w = weight[i].second;		
		int idx = weight[i].first;
		BodyMotionInterface* motion = (*motionDataRef)[idx];
		timePercent += motion->motionPercent(time)*w;		
	}		
	return timePercent;
}

double ResampleMotion::getRefDeltaTime( float u, float dt )
{
	double du = 0.0;
	for (unsigned int i=0;i<weight.size();i++)
	{		
		float w = weight[i].second;		
		int idx = weight[i].first;
		BodyMotionInterface* motion = (*motionDataRef)[idx];
		du += motion->getRefDeltaTime(u,dt)*w;	
	}		
	return du;
	
}

MotionProfile* ResampleMotion::getValidMotionProfile()
{
	float maxWeight = -1.f;
	MotionProfile* validProfile = NULL;
	for (unsigned int i=0;i<weight.size();i++)
	{		
		int idx = weight[i].first;
		float w = weight[i].second;	
		BodyMotionInterface* motion = (*motionDataRef)[idx];
		if (motion->motionProfile && w > maxWeight)
		{
			validProfile = motion->motionProfile;
			maxWeight = w;
		}
	}
	return validProfile;
}

/************************************************************************/
/* Grid Box                                                             */
/************************************************************************/
ParameterBoundingBox::ParameterBoundingBox( int nD )
{
	initBBox(nD);	
}

ParameterBoundingBox::ParameterBoundingBox()
{
	isInit = false;
}

ParameterBoundingBox::ParameterBoundingBox( ParameterBoundingBox& bBox )
{
	numDim = bBox.numDim;
	minParameter = bBox.minParameter;
	maxParameter = bBox.maxParameter;
	isInit = true;
}


void ParameterBoundingBox::initBBox( int nD )
{
	numDim = nD;
	minParameter.resize(numDim);
	maxParameter.resize(numDim);
	for (int i=0;i<nD;i++)
	{
		// initialize to invalid box
		minParameter[i] = 1e30;
		maxParameter[i] = -1e30;
	}

	isInit = true;	
}

void ParameterBoundingBox::extendBBox( const dVector& inPara )
{
	// check for dimension
	if (!isInit)
		initBBox(inPara.size());

	if (inPara.size() != numDim)
		return;

	for (int i=0;i<numDim;i++)
	{
		SR_UPDMIN ( minParameter[i], inPara[i] ); 
		SR_UPDMAX ( maxParameter[i], inPara[i] );	
	}	
}

void ParameterBoundingBox::randomPointInBox( dVector& outPara )
{
	outPara.resize(numDim);
	for (int i=0;i<numDim;i++)
	{
		double coord = minParameter[i] + (maxParameter[i] - minParameter[i])*MeCtMath::Random(0.f,1.f);
		outPara[i] = coord;
	}
}

int ParameterBoundingBox::gridHashing( const dVector& inPara, double cellSize, VecOfInt& adjHash )
{
	int nHash = 0;

	dVector localPara(numDim);
	VecOfDouble gridSize(numDim);
	VecOfInt    intPara(numDim);

	for (int i=0;i<numDim;i++)
	{
		localPara[i] = inPara[i] - minParameter[i];
		gridSize[i] = floor((maxParameter[i]-minParameter[i])/cellSize) + 1;
		intPara[i] = static_cast<int>(localPara[i]/cellSize);
		nHash *= (int)gridSize[i];
		nHash += intPara[i];		
	}

	int nAdjCells = static_cast<int>(pow(3.0,numDim));
	adjHash.resize(nAdjCells);
	adjHash.assign(nAdjCells,0);
	for (int k=0;k<nAdjCells;k++)
	{		
		int powNum = static_cast<int>(pow(3.0,numDim-1));
		int idx = k;
		for (int l=0;l<numDim;l++)
		{
			int tempPara = intPara[l];
			int bit = idx/powNum -1;

			adjHash[k] *= (int)gridSize[l];			
			tempPara += bit;
			idx = idx%powNum;
			powNum /= 3;				
			adjHash[k] += tempPara;
		}		
	}	
	return nHash;	
}

void ParameterBoundingBox::scaleBBox( double scaleRatio )
{
	double extendRatio = scaleRatio-1.0;
	for (int i=0;i<numDim;i++)
	{
		double length = maxParameter[i] - minParameter[i];
		minParameter[i] -= length*0.5*extendRatio;
		maxParameter[i] += length*0.5*extendRatio;
	}
}

/************************************************************************/
/* MotionExampleSet                                                     */
/************************************************************************/
void ExampleSet::addExample( InterpolationExample* ex )
{
	// initialize the parameter size
	if (paraSize == 0)
		paraSize = ex->parameter.size();

	interpExamples.push_back(ex);
	parameterBBox.extendBBox(ex->parameter);
}

bool MotionExampleSet::addMotionExample(MotionExample* ex )
{
	motionData.push_back(ex);
	motionExamples.push_back(ex);
	addExample(ex);
	return true;
}

double MotionExampleSet::blendMotion( float time, const VecOfInterpWeight& blendWeight, BodyMotionFrame& outMotionFrame )
{
	return MotionExampleSet::blendMotionFunc(time,skeletonRef,affectedJoints,motionData,blendWeight,outMotionFrame);
}

MotionExampleSet::~MotionExampleSet()
{
	for (unsigned int i=0;i<motionExamples.size();i++)
	{
		// feng : need to figure out why we can not delete the base pointer "BodyMotionInterface".
		MotionExample* ex = motionExamples[i];
		delete ex;
	}
	motionExamples.clear();
}

MotionExample* MotionExampleSet::getMotionExample( const std::string& motionName )
{
	for (unsigned int i=0;i<motionExamples.size();i++)
	{
		MotionExample* ex = motionExamples[i];
		if (ex->motion->getName() == motionName)
			return ex;
	}
	return NULL;
}



InterpolationExample* MotionExampleSet::createPseudoExample()
{
	ResampleMotion* ex = new ResampleMotion(&motionData);
	ex->motionParameterFunc = motionParameterFunc;
	ex->motionProfile = NULL;
	return ex;
}

void MotionExampleSet::initMotionExampleSet(MotionParameter* parameterFunc)
{
	motionParameterFunc = parameterFunc;
	skeletonRef = parameterFunc->skeletonRef;
	affectedJoints = parameterFunc->affectedJoints;
}

double MotionExampleSet::blendMotionFunc( float time, SmartBody::SBSkeleton* skel, const vector<SmartBody::SBJoint*>& joints, const VecOfBodyMotionPtr& motions, 
									    const VecOfInterpWeight& blendWeight, BodyMotionFrame& outMotionFrame )
{
	int idx = blendWeight[0].first;
	double newTime = 0.0;
	float weightSum = blendWeight[0].second;
	BodyMotionInterface* ex = motions[idx];

	newTime = ex->getMotionFrame(time,skel,joints,outMotionFrame);			
	BodyMotionFrame tempFrame;
	float w1,w2;	
	for (unsigned int i=1;i<blendWeight.size();i++)
	{
		w1 = weightSum;
		w2 = blendWeight[i].second;
		weightSum += w2;
		idx = blendWeight[i].first;
		if (weightSum == 0.f)
			continue;
				
		float weight = w2/weightSum;
		float oneMinusWeight = 1.f - weight;

		newTime = motions[idx]->getMotionFrame(time,skel,joints,tempFrame)*weight + newTime*oneMinusWeight;

		for (unsigned int k=0;k<outMotionFrame.jointQuat.size();k++)
		{
			outMotionFrame.jointQuat[k] = slerp( outMotionFrame.jointQuat[k], tempFrame.jointQuat[k], weight );
			outMotionFrame.jointQuat[k].normalize();
		}		
		outMotionFrame.rootPos = outMotionFrame.rootPos*oneMinusWeight + tempFrame.rootPos*weight;
	}	
	return newTime;
}

void MotionExampleSet::blendMotionFrame( BodyMotionFrame& startFrame, BodyMotionFrame& endFrame, float weight, BodyMotionFrame& outFrame )
{
	BodyMotionFrame tempFrame;
	float oneMinusWeight = 1.f - weight;
	tempFrame = startFrame;
	tempFrame.rootPos = startFrame.rootPos*oneMinusWeight + endFrame.rootPos*weight;
	for (unsigned int i=0;i<tempFrame.jointQuat.size();i++)
	{
		tempFrame.jointQuat[i] = slerp(startFrame.jointQuat[i],endFrame.jointQuat[i],weight);
		tempFrame.jointQuat[i].normalize();
	}
	outFrame = tempFrame;	
}

// faster version, not using slerp for Quat but just do weighted sum then normalize
double MotionExampleSet::blendMotionFuncFast( float time, SmartBody::SBSkeleton* skel, const vector<SmartBody::SBJoint*>& joints, const VecOfBodyMotionPtr& motions, 
											 const VecOfInterpWeight& blendWeight, BodyMotionFrame& outMotionFrame )
{
	// assuming the weight array has been normalized !!!
	int idx;
	float weight;
	double newTime = 0.0;
	BodyMotionFrame tempFrame;

	// first init outMotionFrame
	idx = blendWeight[0].first;
	weight = blendWeight[0].second;
	BodyMotionInterface* ex = motions[idx];
	newTime = ex->getMotionFrame(time,skel,joints,outMotionFrame);
	unsigned int jntQuatSize = outMotionFrame.jointQuat.size();
	for(unsigned int k=0; k<jntQuatSize; k++)
	{
		outMotionFrame.jointQuat[k].w = outMotionFrame.jointQuat[k].w * weight;
		outMotionFrame.jointQuat[k].x = outMotionFrame.jointQuat[k].x * weight;
		outMotionFrame.jointQuat[k].y = outMotionFrame.jointQuat[k].y * weight;
		outMotionFrame.jointQuat[k].z = outMotionFrame.jointQuat[k].z * weight;
	}
	outMotionFrame.rootPos = outMotionFrame.rootPos * weight;
	newTime = newTime * weight;

	for(unsigned int i=1;i<blendWeight.size();i++)
	{
		idx = blendWeight[i].first;
		weight = blendWeight[i].second;

		// avg frame time
		newTime += motions[idx]->getMotionFrame(time,skel,joints,tempFrame) * weight;

		// avg Quat
		for (unsigned int k=0;k<jntQuatSize;k++)
		{
			outMotionFrame.jointQuat[k].w += tempFrame.jointQuat[k].w * weight;
			outMotionFrame.jointQuat[k].x += tempFrame.jointQuat[k].x * weight;
			outMotionFrame.jointQuat[k].y += tempFrame.jointQuat[k].y * weight;
			outMotionFrame.jointQuat[k].z += tempFrame.jointQuat[k].z * weight;
		}
		// avg root pos
		outMotionFrame.rootPos += tempFrame.rootPos * weight;
	}	

	// Quat (from  weighted sum) must be normalized !!!
	for (unsigned int k=0;k<jntQuatSize;k++)
		outMotionFrame.jointQuat[k].normalize();

	return newTime;
}

void MotionExampleSet::blendMotionFrameProfile( ResampleMotion* motion, BodyMotionFrame& startFrame, BodyMotionFrame& endFrame, float weight, BodyMotionFrame& outFrame )
{
	std::vector<SmartBody::SBJoint*>& affectedJoints = motion->motionParameterFunc->affectedJoints;
	MotionProfile* profile = motion->getValidMotionProfile();
	if (!profile)
	{
		// no existing profile, just use the basic linear blend between motion frames
		MotionExampleSet::blendMotionFrame(startFrame,endFrame,weight,outFrame);
		return;
	}

	BodyMotionFrame tempFrame;
	float oneMinusWeight = 1.f - weight;
	tempFrame = startFrame;
	tempFrame.rootPos = startFrame.rootPos*oneMinusWeight + endFrame.rootPos*weight; // interpolate the position using linear blend

	ProfileCurveMap& interpProfileMap = profile->interpolationProfile;
	//LOG("profile curve map size = %d\n",interpProfileMap.size());
	//LOG("num affected joints = %d\n",affectedJoints.size());
	for (unsigned int i=0;i<affectedJoints.size();i++)
	{
		SkJoint* joint = affectedJoints[i];
 		std::string chanName = joint->jointName();
 		float interpW = weight;
		if (interpProfileMap.find(chanName) != interpProfileMap.end())
		{
			ProfileCurve* curve = interpProfileMap[chanName];
			interpW = curve->evaluate(weight);
			//LOG("interp W = %f\n",interpW);
		}
		if (interpW < 0)
			interpW = 0;
		if (interpW > 1.f)
			interpW = 1.f;
		
		tempFrame.jointQuat[i] = slerp(startFrame.jointQuat[i],endFrame.jointQuat[i],interpW);
		tempFrame.jointQuat[i].normalize();		
	}	
	outFrame = tempFrame;
}

float MotionExampleSet::blendMotionFrameEulerProfile( ResampleMotion* motion, BodyMotionFrame& startFrame, BodyMotionFrame& endFrame, float scaleFactor, float weight, BodyMotionFrame& outFrame )
{
	float retimingScale = 0.f;
	std::vector<SmartBody::SBJoint*>& affectedJoints = motion->motionParameterFunc->affectedJoints;
	MotionProfile* profile = motion->getValidMotionProfile();
	if (!profile)
	{
		// no existing profile, just use the basic linear blend between motion frames
		MotionExampleSet::blendMotionFrame(startFrame,endFrame,weight,outFrame);
		return retimingScale;
	}

	float scale = scaleFactor;///motion->motionDuration(MotionExample::DURATION_ACTUAL);

	BodyMotionFrame tempFrame;
	float oneMinusWeight = 1.f - weight;
	tempFrame = startFrame;
	tempFrame.rootPos = startFrame.rootPos*oneMinusWeight + endFrame.rootPos*weight; // interpolate the position using linear blend

	ProfileCurveMap& eulerProfileMap = profile->eulerProfile[0];
	//LOG("profile curve map size = %d\n",interpProfileMap.size());
	//LOG("num affected joints = %d\n",affectedJoints.size());
	for (unsigned int i=0;i<affectedJoints.size();i++)
	{
		SkJoint* joint = affectedJoints[i];
		std::string chanName = joint->jointName();
		float interpW = weight;
		SrVec startEuler = startFrame.jointQuat[i].getEuler();
		SrVec endEuler = endFrame.jointQuat[i].getEuler();
		
		if (eulerProfileMap.find(chanName) != eulerProfileMap.end())
		{
			SrVec outEuler;
			for (int k=0;k<3;k++)
			{	
				ProfileCurveMap& profileMap = profile->eulerProfile[k];
				ProfileCurve* curve = profileMap[chanName];		
				float curveDiff = (curve->end() - curve->start());
				float interpDiff = endEuler[k] - startEuler[k];
				float modulateWeight = 1.f;
// 				if (curveDiff*interpDiff < 0.f)
// 				{
// 					modulateWeight = -1.f;																						
// 				}
				float modulateDiff = interpDiff - curveDiff*modulateWeight;	
				float linearEstimate = curveDiff*weight;
				float curveLinearDiff = curve->evaluate(weight) - linearEstimate;
				outEuler[k] = startEuler[k] + linearEstimate + curveLinearDiff*modulateWeight*scale + modulateDiff*weight;				
			}
			//tempFrame.jointQuat[i].set(outEuler[0],outEuler[1],outEuler[2]);
			tempFrame.jointQuat[i].set(outEuler);
		}
		else
		{
			tempFrame.jointQuat[i] = slerp(startFrame.jointQuat[i],endFrame.jointQuat[i],interpW);
		}
		tempFrame.jointQuat[i].normalize();				
	}	
	outFrame = tempFrame;

	
	if (profile->avgVelProfile)
	{
		retimingScale = profile->avgVelProfile->evaluate(weight);		
	}

	return retimingScale;
}