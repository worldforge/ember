#include "SBMotionGraph.h"
#include "SBScene.h"
#include "SBAssetManager.h"
#include "SBSkeleton.h"
#include "SBRetarget.h"
#include "SBRetargetManager.h"
#include "components/sb/controllers/me_ct_motion_timewarp.hpp"
#include "components/sb/controllers/me_ct_param_animation_utilities.h"
#include "components/sb/controllers/me_ct_jacobian_IK.hpp"
#include "components/sb/sr/sr_random.h"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/lexical_cast.hpp>
#ifdef EMBER_SB_SOIL
	#include "components/sb/external/SOIL/SOIL.h"
#endif

namespace SmartBody {

/************************************************************************/
/* SBMotionFrameBuffer                                                  */
/************************************************************************/


SBMotionFrameBuffer::SBMotionFrameBuffer()
{

}

SBMotionFrameBuffer::~SBMotionFrameBuffer()
{

}

void SBMotionFrameBuffer::initFrameBuffer( SkChannelArray& channelArray, std::vector<std::string>& affectedChannelList )
{
	motionFrameBuffer.size(channelArray.floats());
	std::vector<SkChannel::Type> typeList; 
	typeList.push_back(SkChannel::Quat); typeList.push_back(SkChannel::XPos); typeList.push_back(SkChannel::YPos); typeList.push_back(SkChannel::ZPos);
	channelIdxMap.clear();
	quatChannelNames.clear();
	posChannelNames.clear();

	std::map<std::string,int> posChannelMap;
	if (affectedChannelList.size() != 0) // use subset of all channels
	{
		for (unsigned int i=0;i<affectedChannelList.size();i++)
		{
			const std::string& chanName = affectedChannelList[i];
			for (unsigned int k=0;k<typeList.size();k++)
			{
				int chanIdx = channelArray.search(chanName, typeList[k]);
				if (chanIdx < 0)
					continue;
				int floatIdx = channelArray.float_position(chanIdx);
				if (floatIdx <0)
					continue;
				std::pair<std::string,SkChannel::Type> key(chanName,typeList[k]);
				channelIdxMap[key] = floatIdx;

				// maintain a list of names for all quat channel & position channel
				if (typeList[k] == SkChannel::Quat)
				{
					quatChannelNames.push_back(chanName);
				}
				else if (typeList[k] <= SkChannel::ZPos)
				{
					if (posChannelMap.find(chanName) == posChannelMap.end())
					{
						posChannelMap[chanName] = 1;
						posChannelNames.push_back(chanName);
					}
				}
			}
		}		
	}
	else // use all channels
	{			
		for (unsigned int i=0;i< (unsigned int) channelArray.size();i++)
		{
			const std::string& chanName = channelArray.mappedName(i);
			SkChannel::Type chanType = channelArray.type(i);

			int chanIdx = channelArray.search(chanName, chanType);
			if (chanIdx < 0)
				continue;
			int floatIdx = channelArray.float_position(chanIdx);
			if (floatIdx <0)
				continue;
			std::pair<std::string,SkChannel::Type> key(chanName,chanType);
			channelIdxMap[key] = floatIdx;
			// maintain a list of names for all quat channel & position channel
			if (chanType == SkChannel::Quat)
			{
				quatChannelNames.push_back(chanName);
			}
			else if (chanType <= SkChannel::ZPos)
			{
				if (posChannelMap.find(chanName) == posChannelMap.end())
				{
					posChannelMap[chanName] = 1;
					posChannelNames.push_back(chanName);
				}
			}
		}
	}
}

int SBMotionFrameBuffer::findChannelIndex( const std::string& chanName, SkChannel::Type chanType )
{
	int idx = -1;
	std::pair<std::string,SkChannel::Type> key(chanName,chanType);
	std::map<std::pair<std::string,SkChannel::Type>, int>::iterator mi;
	mi = channelIdxMap.find(key);
	if (mi != channelIdxMap.end())
		idx = mi->second;
	return idx;
}

SrBuffer<float>& SBMotionFrameBuffer::getBuffer()
{
	return motionFrameBuffer;
}

void SBMotionFrameBuffer::setQuat( const std::string& chanName, const SrQuat& q )
{
	int bufIdx = findChannelIndex(chanName, SkChannel::Quat);
	if (bufIdx < 0)
		return;

	motionFrameBuffer[bufIdx + 0] = q.w;
	motionFrameBuffer[bufIdx + 1] = q.x;
	motionFrameBuffer[bufIdx + 2] = q.y;
	motionFrameBuffer[bufIdx + 3] = q.z;		
}

void SBMotionFrameBuffer::setPos( const std::string& chanName, const SrVec& p )
{
	for (int i=0;i<3;i++)
	{
		int chanType = SkChannel::XPos + i;
		int bufIdx = findChannelIndex(chanName, (SkChannel::Type)chanType);
		if (bufIdx < 0)
			continue;
		motionFrameBuffer[bufIdx] = p[i];
	}
}

SrQuat SBMotionFrameBuffer::getQuat( const std::string& chanName )
{
	SrQuat q;
	int bufIdx = findChannelIndex(chanName, SkChannel::Quat);
	if (bufIdx < 0)
		return q;

	q.w = motionFrameBuffer[bufIdx + 0];
	q.x = motionFrameBuffer[bufIdx + 1];
	q.y = motionFrameBuffer[bufIdx + 2];
	q.z = motionFrameBuffer[bufIdx + 3];
	return q;
}

SrVec SBMotionFrameBuffer::getPos( const std::string& chanName )
{
	SrVec p;
	for (int i=0;i<3;i++)
	{
		int chanType = SkChannel::XPos + i;
		int bufIdx = findChannelIndex(chanName, (SkChannel::Type)chanType);
		if (bufIdx < 0)
			continue;
		p[i] = motionFrameBuffer[bufIdx] ;
	}
	return p;
}


int SBMotionFrameBuffer::getNumQuatChannels()
{
	return quatChannelNames.size();
}

int SBMotionFrameBuffer::getNumPosChannels()
{
	return posChannelNames.size();
}

const std::map<std::pair<std::string,SkChannel::Type>, int>& SBMotionFrameBuffer::getChannelIdxMap()
{
	return channelIdxMap;
}

const std::vector<std::string>& SBMotionFrameBuffer::getQuatChannelNames()
{
	return quatChannelNames;
}

const std::vector<std::string>& SBMotionFrameBuffer::getPosChannelNames()
{
	return posChannelNames;
}

void SBMotionFrameBuffer::interpMotionFrameBuffer( SBMotionFrameBuffer& frameA, SBMotionFrameBuffer& frameB, float weight, SBMotionFrameBuffer& outFrame )
{
	if (frameA.getNumQuatChannels() != frameB.getNumQuatChannels() || frameA.getNumPosChannels() != frameB.getNumPosChannels())
		return;

	if (outFrame.getNumQuatChannels() != frameA.getNumQuatChannels() || outFrame.getNumPosChannels() != frameA.getNumPosChannels())
	{
		outFrame.copyFrameBuffer(frameA);
	}

	const std::vector<std::string>& quatChannelNames = frameA.getQuatChannelNames();
	for (unsigned int i=0;i<quatChannelNames.size();i++)
	{
		const std::string& quatName = quatChannelNames[i];
		SrQuat qa,qb;
		qa = frameA.getQuat(quatName);
		qb = frameB.getQuat(quatName);
		SrQuat qout = slerp(qa,qb,weight);
		outFrame.setQuat(quatName,qout);
	}
	const std::vector<std::string>& posChannelNames = frameA.getPosChannelNames();
	for (unsigned int i=0;i<posChannelNames.size();i++)
	{
		const std::string& posName = posChannelNames[i];
		SrVec pa,pb;
		pa = frameA.getPos(posName);
		pb = frameB.getPos(posName);
		SrVec pout = pa*(1-weight) + pb*weight;
		outFrame.setPos(posName,pout);
	}
}


void SBMotionFrameBuffer::applyRetarget( SBRetarget* retarget )
{
	const std::vector<std::string>& quatChannelNames = getQuatChannelNames();
	for (unsigned int i=0;i<quatChannelNames.size();i++)
	{
		const std::string& quatName = quatChannelNames[i];
		SrQuat qa = getQuat(quatName);
		SrQuat qout = retarget->applyRetargetJointRotation(quatName,qa);
		setQuat(quatName,qout);
	}
	const std::vector<std::string>& posChannelNames = getPosChannelNames();
	for (unsigned int i=0;i<posChannelNames.size();i++)
	{
		const std::string& posName = posChannelNames[i];
		SrVec pa = getPos(posName);
		for (int k=0;k<3;k++)
			pa[k] = retarget->applyRetargetJointTranslation(posName, pa[k]);
		setPos(posName, pa);
	}
		 

}

void SBMotionFrameBuffer::copyFrameBuffer( SBMotionFrameBuffer& inBuffer )
{
	channelIdxMap = inBuffer.getChannelIdxMap();
	motionFrameBuffer.size(inBuffer.getBuffer().size());
	quatChannelNames = inBuffer.getQuatChannelNames();
	posChannelNames  = inBuffer.getPosChannelNames();
}

void SBMotionFrameBuffer::setMat( const std::string& chanName, const SrMat& mat )
{
	SrQuat newQuat = SrQuat(mat);
	SrVec newPos = mat.get_translation();
	setQuat(chanName, newQuat);
	setPos(chanName,newPos);
}

void SBMotionFrameBuffer::setAllQuats( const std::vector<std::string>& quatNames, const std::vector<SrQuat>& quatList )
{
	for (unsigned int i=0;i<quatNames.size();i++)
	{
		setQuat(quatNames[i],quatList[i]);
	}
}

void SBMotionFrameBuffer::setAllPos( const std::vector<std::string>& posNames, const std::vector<SrVec>& posList )
{
	for (unsigned int i=0;i<posNames.size();i++)
	{
		setPos(posNames[i],posList[i]);
	}
}

SrMat SBMotionFrameBuffer::getMat( const std::string& chanName )
{
	SrQuat q = getQuat(chanName);
	SrVec  pos = getPos(chanName);
	SrMat tranMat; q.get_mat(tranMat);
	tranMat.set_translation(pos);
	return tranMat;
}
/************************************************************************/
/* SBMotionNode                                                         */
/************************************************************************/
SBMotionNode::SBMotionNode( SBAnimationBlend* blend, int idx )
{
	baseJointName = "base";
	basePreRot = SrMat::id;
	animBlend = blend;
	index = idx;
	initTimeWarp();
}

SBMotionNode::SBMotionNode()
{

}
SBMotionNode::~SBMotionNode()
{

}


void SBMotionNode::initTimeWarp()
{
	if (!animBlend) return;
	std::vector<double> refKey;
	float step = 1.f/(animBlend->getNumKeys()-1.f);
	for (unsigned int i=0;i<animBlend->getNumKeys();i++)
		refKey.push_back(step*i); // normalize the reference time to [0,1]
	for (unsigned int i=0;i<animBlend->getNumMotions();i++)
	{
		std::string motionName = animBlend->getMotionName(i);
		std::vector<double> motionKeys;
		for (unsigned int k=0;k<animBlend->getNumKeys(); k++)
		{
			motionKeys.push_back(animBlend->getMotionKey(motionName,k));
		}
		MotionTimeWarpFunc* timeWarp = new MultiLinearTimeWarp(refKey, motionKeys);
		timeWarpFuncs.push_back(timeWarp);
	}
}


void SBMotionNode::getDeltaAlignTransform( float u, float du , const std::vector<float>& weights,  SrMat& outAlignDiff, SrMat& outBaseXZFrame)
{
	// only use motions with non-zero weights
	std::vector<int> moIndex;
	for (unsigned int i=0;i<weights.size();i++)
	{
		if (weights[i] == 0.f)
			continue;
		moIndex.push_back(i);
	}
	
	SrMat tempWoMat, tempBaseMat;
	float prevWeight = 0.f;
	for (unsigned int i=0;i<moIndex.size();i++)
	{
		int idx = moIndex[i];
		MotionTimeWarpFunc* timeWarp = timeWarpFuncs[idx];
		SBMotion* motion = animBlend->motions[idx];
		float t = timeWarp->timeWarp(u);
		float tdt = timeWarp->timeWarp(u+du);
		t = SimpleTimeWarp::floatMod(t,motion->getDuration());
		tdt = SimpleTimeWarp::floatMod(tdt,motion->getDuration());
		if (tdt < t) tdt = t;
		SrMat mat1 = motion->getChannelMat(baseJointName,t);
		SrMat mat2 = motion->getChannelMat(baseJointName,tdt);
		
		SrMat mat1Y, mat1XZ, mat2Y, mat2XZ;
		decomposeBaseMat(mat1,basePreRot,mat1Y,mat1XZ);
		decomposeBaseMat(mat2,basePreRot,mat2Y,mat2XZ);
		SrMat matYDiff = mat2Y*mat1Y.rigidInverse();
		if (i==0)
		{
			tempWoMat = matYDiff;
			tempBaseMat = mat2XZ;			
		}
		else
		{
			SrMat mat = tempWoMat;
			SrMat baseMat = tempBaseMat;
			float w = prevWeight/(prevWeight + weights[idx]);
			PAWoManager::matInterp(tempWoMat,mat,matYDiff,w);	
			PAWoManager::matInterp(tempBaseMat,baseMat,mat2XZ,w);		
		}
		prevWeight += weights[idx];
	}
	outAlignDiff = tempWoMat;
	outBaseXZFrame = tempBaseMat;
}

void SBMotionNode::decomposeBaseMat( SrMat& inMat, SrMat& inPreRot, SrMat& outYWoMat, SrMat& outXZBaseMat, SBRetarget* retarget )
{
	SrQuat quat = SrQuat(inMat);
	//if (retarget)
	//	quat = retarget->applyRetargetJointRotation(baseJointName,quat);

	SrVec vec = quat.axis() * quat.angle();
	SrVec vec1 = vec * inPreRot;//prerotMat.inverse();
	quat = SrQuat(vec1);	
	quat.normalize();
	SrMat mat;
	quat.get_mat(mat);	

	SrQuat yRotQuat =SrQuat(SrVec(0,vec1.y,0)); // extract only the y-component
	SrMat yRotMat; yRotQuat.get_mat(yRotMat);
	SrMat xzRotMat = mat*yRotMat.inverse();
	
	SrQuat quatXZ = SrQuat(xzRotMat);
	quatXZ.normalize();

	SrVec vecXZ = quatXZ.axis() * quatXZ.angle();			
	SrVec vecXZ2 = vecXZ * inPreRot.inverse();
	quatXZ = SrQuat(vecXZ2);
	//if (retarget)
	//	quatXZ = retarget->applyRetargetJointRotationInverse(baseJointName,quatXZ);
	quatXZ.get_mat(outXZBaseMat);
	outXZBaseMat.set(13, inMat.get(13));

	outYWoMat = yRotMat;
	outYWoMat.set(12, inMat.get(12));
	outYWoMat.set(14, inMat.get(14));
}

void SBMotionNode::getBlendedMotionFrame( float u, const std::vector<float>& weights, SBMotionFrameBuffer& outFrame )
{
	std::vector<int> moIndex;
	for (unsigned int i=0;i<weights.size();i++)
	{
		if (weights[i] == 0.f)
			continue;
		moIndex.push_back(i);
	}

	const std::vector<std::string>& quatChannelNames = outFrame.getQuatChannelNames();
	const std::vector<std::string>& posChannelNames = outFrame.getPosChannelNames();

	float prevWeight = 0.f;
	std::vector<SrQuat> outQuatList, tempQuatList;
	std::vector<SrVec>  outPosList, tempPosList;
	for (unsigned int i=0;i<moIndex.size();i++)
	{
		int idx = moIndex[i];
		MotionTimeWarpFunc* timeWarp = timeWarpFuncs[idx];
		SBMotion* motion = animBlend->motions[idx];
		float t = timeWarp->timeWarp(u);
		t = SimpleTimeWarp::floatMod(t,motion->getDuration());	

		if (i==0)
		{
			motion->getAllChannelQuat(quatChannelNames, t, outQuatList);
			motion->getAllChannelPos(posChannelNames, t, outPosList);			
		}	
		else
		{
			float weight = prevWeight/(prevWeight+weights[idx]);
			motion->getAllChannelQuat(quatChannelNames, t, tempQuatList);
			motion->getAllChannelPos(posChannelNames, t, tempPosList);
			SBMotionNode::interpQuatList(weight, tempQuatList, outQuatList, outQuatList);
			SBMotionNode::interpPosList(weight, tempPosList, outPosList, outPosList);
		}
		prevWeight += weights[idx];
	}	

	// write the blending results back to the motion frame buffer
	for (unsigned int i=0;i<outQuatList.size();i++)
		outFrame.setQuat(quatChannelNames[i], outQuatList[i]);
	for (unsigned int i=0;i<outPosList.size();i++)
		outFrame.setPos(posChannelNames[i], outPosList[i]);
}

void SBMotionNode::interpQuatList(float w,  std::vector<SrQuat>& quatListA, std::vector<SrQuat>& quatListB, std::vector<SrQuat>& outList )
{
	if (quatListA.size() != quatListB.size())
		return;
	if (outList.size() != quatListA.size())
		outList.resize(quatListA.size());

	for (unsigned int i=0;i<outList.size();i++)
	{
		SrQuat& qA = quatListA[i];
		SrQuat& qB = quatListB[i];
		outList[i] = slerp(qA, qB, w);		
	}
}

void SBMotionNode::interpPosList( float w, std::vector<SrVec>& posListA, std::vector<SrVec>& posListB, std::vector<SrVec>& outPosList )
{
	if (posListA.size() != posListB.size())
		return;
	if (outPosList.size() != posListA.size())
		outPosList.resize(posListA.size());

	for (unsigned int i=0;i<outPosList.size();i++)
	{
		SrVec& pA = posListA[i];
		SrVec& pB = posListB[i];
		outPosList[i] = pA*(1.f-w) + pB*w;		
	}
}

int SBMotionNode::getIndex()
{
	return index;
}

std::string SBMotionNode::getName()
{
	return animBlend->getName();
}

float SBMotionNode::getRefDuration()
{
	float refDuration = 0.f;
	if (timeWarpFuncs.size() > 0)
	{
		refDuration = timeWarpFuncs[0]->refTimeLength();
	}
	return refDuration;
}

void SBMotionNode::addOutEdge( SBMotionTransitionEdge* edge )
{
	outEdges.push_back(edge);
}

const std::vector<SBMotionTransitionEdge*>& SBMotionNode::getOutEdges()
{
	return outEdges;
}

float SBMotionNode::getRefDeltaTime( float u, float dt, const std::vector<float>& weights )
{
	std::vector<int> moIndex;
	for (unsigned int i=0;i<weights.size();i++)
	{
		if (weights[i] == 0.f)
			continue;
		moIndex.push_back(i);
	}

	float du = 0.f;
	for (unsigned int i=0;i<moIndex.size();i++)
	{
		int idx = moIndex[i];
		MotionTimeWarpFunc* timeWarp = timeWarpFuncs[idx];
		du += dt/timeWarp->timeSlope(u)*weights[idx];
	}
	return du;
}

float SBMotionNode::getActualTime( float u, const std::vector<float>& weights  )
{
	std::vector<int> moIndex;
	for (unsigned int i=0;i<weights.size();i++)
	{
		if (weights[i] == 0.f)
			continue;
		moIndex.push_back(i);
	}

	float t = 0.f;
	for (unsigned int i=0;i<moIndex.size();i++)
	{
		int idx = moIndex[i];
		MotionTimeWarpFunc* timeWarp = timeWarpFuncs[idx];
		t += (timeWarp->timeWarp(u)-timeWarp->timeWarp(0.f))*weights[idx];
	}
	return t;
}

float SBMotionNode::getActualDuration( const std::vector<float>& weights )
{
	std::vector<int> moIndex;
	for (unsigned int i=0;i<weights.size();i++)
	{
		if (weights[i] == 0.f)
			continue;
		moIndex.push_back(i);
	}

	float t = 0.f;
	for (unsigned int i=0;i<moIndex.size();i++)
	{
		int idx = moIndex[i];
		MotionTimeWarpFunc* timeWarp = timeWarpFuncs[idx];
		t += timeWarp->actualTimeLength()*weights[idx];
	}
	return t;
}

SBAnimationBlend* SBMotionNode::getAnimBlend()
{
	return animBlend;
}

void SBMotionNode::getRandomBlendWeights( std::vector<float>& outWeights )
{
	SrBox paraBox = animBlend->getParameterBoundBox();
	SrVec parameter;
	SrRandom random;		
	for (int i=0;i<animBlend->getParameterDimension();i++)
	{
		random.limits(paraBox.a[i],paraBox.b[i]);
		parameter[i] = random.getf();
	}
	animBlend->getWeightsFromParameters(parameter,outWeights);
}

/************************************************************************/
/* SBMotionGraph                                                        */
/************************************************************************/

SBAPI SBMotionGraph::SBMotionGraph()
{
	useTransitionInterpolation = true;		
}

SBAPI SBMotionGraph::~SBMotionGraph()
{
}

SBAPI SBMotionNode* SBMotionGraph::addMotionNodeFromBlend( SBAnimationBlend* blend )
{
	SBMotionNode* node = getMotionNode(blend->getName());
	if (node)
	{
		LOG("Motion Node '%s' already exists.", blend->getName().c_str());
		return NULL;
	}
	int nodeIdx = motionNodes.size();
	node = new SBMotionNode(blend, nodeIdx);
	motionNodes.push_back(node);
	motionNodeMap[blend->getName()] = node;
	return node;
}

SBAPI SBMotionNode* SBMotionGraph::addMotionNodeFromMotion( const std::string& nodeName, const std::string& motionName, int startFrame /*= -1*/, int endFrame /*= -1*/ )
{	
	SmartBody::SBAssetManager* assetManager = SmartBody::SBScene::getScene()->getAssetManager();
	SmartBody::SBMotion* sbMotion = assetManager->getMotion(motionName);	
	return addMotionNodeFromMotionRef(nodeName, sbMotion, startFrame, endFrame);
}


SBAPI SBMotionNode* SBMotionGraph::addMotionNodeFromMotionRef( const std::string& nodeName, SmartBody::SBMotion* sbMotion, int startFrame, int endFrame )
{
	if (!sbMotion)
		return NULL; // no motion available
	SmartBody::SBAnimationBlend1D* animBlend = new SBAnimationBlend1D(nodeName);
	if (startFrame < 0 || startFrame >= sbMotion->getNumFrames())
		startFrame = 0;
	if (endFrame < 0 || endFrame >= sbMotion->getNumFrames())
		endFrame = sbMotion->getNumFrames()-1;

	std::string motionName = sbMotion->getName();
	// set a dummy parameter since we are only using one motion
	//animBlend->addMotion(motionName, 0.f);
	animBlend->addMotionFromRef(sbMotion, 0.f);		
	animBlend->setBlendSkeleton(sbMotion->getMotionSkeletonName());
	std::vector<std::string> moNames;
	moNames.push_back(motionName);
	std::vector<double> animationKey(1);
	animationKey[0] = sbMotion->keytime(startFrame);	
	animBlend->addCorrespondencePoints(moNames, animationKey);
	animationKey[0] = sbMotion->keytime(endFrame);
	animBlend->addCorrespondencePoints(moNames, animationKey);
	motioAnimBlends.push_back(animBlend); // maintain the list of animBlends created this way inside the Motion Graph
	return addMotionNodeFromBlend(animBlend);
}

SBMotionNode* SBMotionGraph::addMotionNodeFromMotionTransition( const std::string& nodeName, const std::string& motionName1, std::string& motionName2, int mo1EndFrme, int mo2StartFrame, int transitionLength )
{
	SBMotionNode* motionNode = NULL;
	SmartBody::SBAssetManager* assetManager = SmartBody::SBScene::getScene()->getAssetManager();
	SmartBody::SBMotion* motion1 = assetManager->getMotion(motionName1);
	SmartBody::SBMotion* motion2 = assetManager->getMotion(motionName2);	

	SmartBody::SBMotion* transMotion = new SmartBody::SBMotion();
	SBMotionFrameBuffer frame1, frame2, outFrame;
	std::vector<std::string> affectedJointNames;
	frame1.initFrameBuffer(motion1->channels(), affectedJointNames);
	frame2.initFrameBuffer(motion1->channels(), affectedJointNames);
	outFrame.initFrameBuffer(motion1->channels(), affectedJointNames);
	transMotion->channels() = motion1->channels();
	
	// add transition frames
	std::vector<std::string> allQuatNames = frame1.getQuatChannelNames();
	std::vector<std::string> allPosNames = frame1.getPosChannelNames();
	std::vector<SrQuat> quatList1, quatList2;
	std::vector<SrVec>  posList1, posList2;
	SrMat yMat1, yMat2; 
	float frameStep = motion1->getFrameRate();
	//LOG("motion1 = %s, motion2 = %s, frame1 = %d, frame2 = %d", motionName1.c_str(), motionName2.c_str(), mo1EndFrme, mo2StartFrame);
	for (int i=0;i<transitionLength;i++)
	{
		int keyFrame1 = i+mo1EndFrme;
		if (keyFrame1 >= motion1->getNumFrames())
			keyFrame1 = motion1->getNumFrames()-1;

		int keyFrame2 = mo2StartFrame - transitionLength + 1 + i;
		if (keyFrame2 < 0)
			keyFrame2 = 0;

		float key1 = motion1->keytime(keyFrame1);
		motion1->getAllChannelPos(allPosNames, key1, posList1 );
		motion1->getAllChannelQuat(allQuatNames, key1, quatList1);
		frame1.setAllPos(allPosNames, posList1);
		frame1.setAllQuats(allQuatNames, quatList1);

		float key2 = motion2->keytime(keyFrame2);
		motion2->getAllChannelPos(allPosNames, key2, posList2 );
		motion2->getAllChannelQuat(allQuatNames, key2, quatList2);
		frame2.setAllPos(allPosNames, posList2);
		frame2.setAllQuats(allQuatNames, quatList2);

		SrMat xzMat, baseMat1, baseMat2, preRot;	
		baseMat1 = frame1.getMat("base");
		baseMat2 = frame2.getMat("base");
		if (i==0)
		{
			SBMotionNode::decomposeBaseMat(baseMat1, preRot, yMat1, xzMat);
			SBMotionNode::decomposeBaseMat(baseMat2, preRot, yMat2, xzMat);
			yMat1 = yMat1.inverse();
			yMat2 = yMat2.inverse();
		}
		baseMat1 = baseMat1*yMat1;
		baseMat2 = baseMat2*yMat2;
		SrVec baseTran1 = baseMat1.get_translation();
		SrVec baseTran2 = baseMat2.get_translation();
		//LOG("node %s, baseTran1 = %f %f %f",nodeName.c_str(), baseTran1[0],baseTran1[1],baseTran1[2]);
		//LOG("node %s, baseTran2 = %f %f %f",nodeName.c_str(), baseTran2[0],baseTran2[1],baseTran2[2]);

		frame1.setMat("base",baseMat1);
		frame2.setMat("base",baseMat2);
		float ratio = (float)(i+1)/transitionLength;
		float alphaWeight = 2*ratio*ratio*ratio - 3*ratio*ratio + 1;
		SBMotionFrameBuffer::interpMotionFrameBuffer(frame1,frame2, 1.f-alphaWeight, outFrame);

		std::vector<float> tempFrame;
		SrBuffer<float>& buffer = outFrame.getBuffer();
		for (int m=0;m<buffer.size();m++)
			tempFrame.push_back(buffer[m]);
		transMotion->addFrame(frameStep*i, tempFrame);	
	}
	transMotion->setName(nodeName);
	transMotion->setMotionSkeletonName(motion1->getMotionSkeletonName());
	motionNode = addMotionNodeFromMotionRef(nodeName, transMotion, 0, transitionLength-1);		
	return motionNode;
}

SBAPI SBMotionNode* SBMotionGraph::getMotionNode( const std::string& nodeName )
{
	std::map<std::string, SBMotionNode*>::iterator mi;
	mi = motionNodeMap.find(nodeName);
	if (mi != motionNodeMap.end())
	{
		return mi->second;
	}
	return NULL;
}


SBAPI SBMotionTransitionEdge* SBMotionGraph::addMotionEdgeByIndex( int fromNodeIdx, int toNodeIdx )
{
	if (fromNodeIdx < 0 || fromNodeIdx >= motionNodes.size() || toNodeIdx < 0 || toNodeIdx >= motionNodes.size())
		return NULL; // not valid edge
	SBMotionNode* fromNode = motionNodes[fromNodeIdx];
	SBMotionNode* toNode = motionNodes[toNodeIdx];
	return addMotionEdge(fromNode->getName(), toNode->getName());
}

SBAPI SBMotionTransitionEdge* SBMotionGraph::addMotionEdge( const std::string& fromNodeName, const std::string& toNodeName )
{
	SBMotionTransitionEdge* edge = getMotionEdge(fromNodeName, toNodeName);
	if (edge)
	{
		LOG("Motion Edge '%s'->'%s' already exists.",fromNodeName.c_str(), toNodeName.c_str());
		return NULL;
	}

	SBMotionNode *fromNode = NULL, *toNode = NULL;
	fromNode = getMotionNode(fromNodeName);
	toNode   = getMotionNode(toNodeName);

	if (!fromNode || !toNode)
	{
		LOG("Can not create edge. Motion node '%s' or '%s' could not be found. ", fromNodeName.c_str(), toNodeName.c_str());
		return NULL;
	}

	std::string edgeName = fromNodeName + "_" + toNodeName;
	int idx = motionEdges.size();
	edge = new SBMotionTransitionEdge(idx);
	// by default, the transition is from the end of first node to the beginning of second node
	edge->initTransitionEdge(fromNode, toNode, fromNode->getRefDuration(), 0.f);
	fromNode->addOutEdge(edge);
	motionEdges.push_back(edge);
	motionEdgeMap[edgeName] = edge;
	return edge;
}

SBAPI SBMotionTransitionEdge* SBMotionGraph::getMotionEdge( const std::string& srcNode, const std::string& dstNode )
{
	std::string edgeName = srcNode + "_" + dstNode;
	std::map<std::string, SBMotionTransitionEdge*>::iterator mi;
	mi = motionEdgeMap.find(edgeName);
	if (mi != motionEdgeMap.end())
		return mi->second;
	return NULL;
}

/*
SBAPI void SBMotionGraph::synthesizePath( SteerPath& desiredPath, const std::string& skeletonName, std::vector<std::pair<std::string,std::string> >& graphTraverseEdges )
{
	SBRetargetManager* retargetManager = SmartBody::SBScene::getScene()->getRetargetManager();
	// build the delta transform cache to quickly compute the path cost	
	std::map<std::string, MotionNodeCache> pathDeltaTransformCache;	
	for (unsigned int i=0;i<motionNodes.size();i++)
	{
		SBMotionNode* node = motionNodes[i];
		std::string nodeSkelName = node->getAnimBlend()->getBlendSkeleton();
		SmartBody::SBRetarget* retarget = retargetManager->getRetarget(nodeSkelName, skeletonName);

		pathDeltaTransformCache[node->getName()] = MotionNodeCache();
		MotionNodeCache& nodeCache = pathDeltaTransformCache[node->getName()];

		std::vector<float> weights;
		SrMat deltaMat, baseXZMat;
		node->getRandomBlendWeights(weights);		
		node->getDeltaAlignTransform(0.f, 0.999f, weights, deltaMat, baseXZMat);
		
		int numFrames = node->getActualDuration(weights)/0.033f;
		if (numFrames < 1) numFrames = 1; // minimum one frames
		
		float duStep = 1.f/numFrames;
		SrMat dMat, xzMat;
		SrVec prevPos;
		nodeCache.totalArcLength = 0.f;
		for (int f=1;f<=numFrames;f++)
		{
			float uStep = duStep*f*0.999f;
			if (uStep > 0.999f ) uStep = 0.999f;
			node->getDeltaAlignTransform(0.f, uStep, weights, dMat, xzMat);
			SrVec curPos = dMat.get_translation();
			if (retarget)
				curPos = retarget->applyRetargetJointTranslationVec("base", curPos);
			nodeCache.pathLocalPts.push_back(curPos);
			float dist = (curPos - prevPos).norm();
			nodeCache.pathDists.push_back(dist);
			nodeCache.totalArcLength += dist;
		}

		// rescale the mat translation to account for retargeting
		if (retarget)
		{
			SrVec deltaTrans = retarget->applyRetargetJointTranslationVec("base", deltaMat.get_translation());
			deltaMat.set_translation(deltaTrans);
		}
		nodeCache.deltaTransform = deltaMat;
	}	
	SrMat curTransform;
	// start the translation at the beginning of path
	curTransform.set_translation(desiredPath.pathPoint(0.f));
	SBMotionNode* curNode = motionNodes[0]; // start with the first node
	MotionGraphTraverse curGraphTraverse, bestGraphTraverse, finalGraphTraverse;
	float bestTraverseError = 1e30;
	curGraphTraverse.curNodeIdx = curNode->getIndex();
	curGraphTraverse.curTransform.set_translation(desiredPath.pathPoint(0.f)); // start from the beginning of the path

	MotionNodeCache& nodeCache = pathDeltaTransformCache[curNode->getName()];
	SrMat& deltaT = nodeCache.deltaTransform;
	curGraphTraverse.curTransform = deltaT*curGraphTraverse.curTransform;

	curGraphTraverse.traverseError = 0.f;
	curGraphTraverse.traversePathDist = 0.f;
	curGraphTraverse.traverseTime = 0.f;
	bestGraphTraverse.traversePathDist = 0.f;

	finalGraphTraverse = curGraphTraverse;

	float timeThreshold = 2.f; // restart traverse for every 2 seconds of graph traversal
	std::vector<std::pair<int,int> > bestEdgeList;
	bool reachTarget = false;
	while (!reachTarget && bestGraphTraverse.traversePathDist < desiredPath.pathLength()*1.2f) 
	{		

		float curError = traverseGraph(desiredPath, curGraphTraverse, bestGraphTraverse, pathDeltaTransformCache, timeThreshold, bestTraverseError);

		// pick only the first half of the best graph traverse
		float accumTime = 0.f;		
		for (unsigned int i=0;i<bestGraphTraverse.graphEdges.size();i++)
		{
			std::pair<int,int> edge = bestGraphTraverse.graphEdges[i];
			std::vector<float> weights;			
			SBMotionNode* node = motionNodes[edge.second];	
			node->getRandomBlendWeights(weights);
			MotionNodeCache& nodeCache = pathDeltaTransformCache[node->getName()];
			SrMat& deltaT = nodeCache.deltaTransform;

			float nodeError = pathError(desiredPath, node, finalGraphTraverse.curTransform, pathDeltaTransformCache[node->getName()], finalGraphTraverse.traversePathDist);
			finalGraphTraverse.curTransform = deltaT*finalGraphTraverse.curTransform;			
			finalGraphTraverse.graphEdges.push_back(edge);
			finalGraphTraverse.traversePathDist += deltaT.get_translation().norm();		
			finalGraphTraverse.traverseError += nodeError;
			finalGraphTraverse.distToTarget = (finalGraphTraverse.curTransform.get_translation() - desiredPath.pathPoint(desiredPath.pathLength()*0.99f)).norm();
			finalGraphTraverse.curNodeIdx = node->getIndex();
			accumTime += node->getActualDuration(weights);			

			
			reachTarget = finalGraphTraverse.traversePathDist >= desiredPath.pathLength() && finalGraphTraverse.distToTarget < 0.5f;			
			if (reachTarget || accumTime > 1.f)
			{			
				
				break;
			}
		}

		// attach current path to the edge list
		//bestEdgeList.insert(bestEdgeList.end(), bestGraphTraverse.graphEdges.begin(), bestGraphTraverse.graphEdges.end());		
		//LOG("best graph traverse, Node Idx = %d",bestGraphTraverse.curNodeIdx, bestGraphTraverse.graphEdges.size());
		
		LOG("curError = %f, traverseDit = %f, total edge size = %d", finalGraphTraverse.traverseError, finalGraphTraverse.traversePathDist, finalGraphTraverse.graphEdges.size());
		curGraphTraverse = finalGraphTraverse;		
		// reset the best path, since we will restart the traverse from previous result.
		curGraphTraverse.graphEdges.clear(); 
		curGraphTraverse.traverseTime = 0.f;
		bestTraverseError = 1e30; 

		//finalGraphTraverse.distToTarget = (finalGraphTraverse.curTransform.get_translation() - desiredPath.pathPoint(desiredPath.pathLength()*0.99f)).norm();
		//reachTarget = finalGraphTraverse.traversePathDist >= desiredPath.pathLength() && finalGraphTraverse.distToTarget < 0.5f;
	}	

	graphTraverseEdges.clear();
	bestEdgeList = finalGraphTraverse.graphEdges;
	LOG("Finish traverse graph, best edge size = %d", bestEdgeList.size());
	for (unsigned int i=0;i<bestEdgeList.size();i++)
	{
		std::pair<int,int>& edge = bestEdgeList[i];
		//LOG("Edge %d, node %s to node %s", i, motionNodes[edge.first]->getName().c_str(), motionNodes[edge.second]->getName().c_str() );
		graphTraverseEdges.push_back(std::pair<std::string,std::string>(motionNodes[edge.first]->getName(), motionNodes[edge.second]->getName()));				
	}

}

*/


SBMotionGraph::MotionGraphTraverse& SBMotionGraph::MotionGraphTraverse::operator=( const MotionGraphTraverse& rtIn )
{
	graphEdges = rtIn.graphEdges;
	curNodeIdx = rtIn.curNodeIdx;
	curTransform = rtIn.curTransform;
	traverseError = rtIn.traverseError;
	traversePathDist = rtIn.traversePathDist;
	traverseTime = rtIn.traverseTime;
	distToTarget = rtIn.distToTarget;
	return *this;
}

/*
float SBMotionGraph::traverseGraph(SteerPath& curPath, MotionGraphTraverse& curGraphTraverse, MotionGraphTraverse& bestGraphTraverse, std::map<std::string, MotionNodeCache>& deltaTransformMap, float timeThreshold, float& bestTraverseError )
{
	// either the path is final now, or the path error is already larger than the previous best path
	bool reachTarget = false; //curGraphTraverse.traversePathDist >= curPath.pathLength() && curGraphTraverse.distToTarget < 0.5f;

	if (reachTarget || curGraphTraverse.traverseTime >= timeThreshold || curGraphTraverse.traverseError > bestTraverseError)
	{
		if (curGraphTraverse.traverseError < bestTraverseError) // current best path 
		{	
			bestTraverseError = curGraphTraverse.traverseError;
			bestGraphTraverse = curGraphTraverse;
			LOG("Best error = %f, node idx = %d, traverseDist = %f, traverseTime = %f, num nodes = %d", bestTraverseError, curGraphTraverse.curNodeIdx, curGraphTraverse.traversePathDist, curGraphTraverse.traverseTime, curGraphTraverse.graphEdges.size());			
		}
		
		return curGraphTraverse.traverseError;
	}

	SBMotionNode* node = motionNodes[curGraphTraverse.curNodeIdx];
	std::map<float,int> nodeErrorMap;
	const std::vector<SBMotionTransitionEdge*>& outEdges = node->getOutEdges();
	for (unsigned int i=0;i<outEdges.size();i++)
	{
		SBMotionTransitionEdge* edge = outEdges[i];
		SBMotionNode* tgtNode = edge->getTgtNode();
		float nodeError = pathError(curPath, tgtNode, curGraphTraverse.curTransform, deltaTransformMap[tgtNode->getName()], curGraphTraverse.traversePathDist);
		nodeErrorMap[nodeError] = tgtNode->getIndex();
	}	

	// doing the depth-first search in the graph, start from the node with lowest error	
	float bestEdgeError = 1e30;
	std::map<float,int>::iterator mi;	
	for ( mi  = nodeErrorMap.begin();
		  mi != nodeErrorMap.end();
		  mi++ )
	{
		SBMotionNode* node = motionNodes[mi->second];		
		MotionGraphTraverse tempTraverse = curGraphTraverse;		
		tempTraverse.graphEdges.push_back(std::pair<int,int>(curGraphTraverse.curNodeIdx, node->getIndex()));
		tempTraverse.curNodeIdx = node->getIndex();
		MotionNodeCache& nodeCache = deltaTransformMap[node->getName()];
		SrMat& deltaT = nodeCache.deltaTransform;
		tempTraverse.curTransform = deltaT*curGraphTraverse.curTransform;
		tempTraverse.traversePathDist += deltaT.get_translation().norm();//nodeCache.totalArcLength;//deltaT.get_translation().norm();
		std::vector<float> weights;
		node->getRandomBlendWeights(weights);
		tempTraverse.traverseTime += node->getActualDuration(weights);
		tempTraverse.traverseError += mi->first ;
		tempTraverse.distToTarget = (tempTraverse.curTransform.get_translation() - curPath.pathPoint(curPath.pathLength()*0.99f)).norm();
		float newError = traverseGraph(curPath, tempTraverse, bestGraphTraverse, deltaTransformMap, timeThreshold, bestTraverseError);
		if (bestEdgeError > newError)
		{
			bestEdgeError = newError;			
		}		
	}	

	return bestEdgeError;
}
*/

/*
float SBMotionGraph::pathError( SteerPath& curPath, const SBMotionNode* curNode, const SrMat& curTransform,  MotionNodeCache& nodeCache, float pathLength )
{
#if 0
	SrMat& deltaTransform = nodeCache.deltaTransform;
	SrMat nextMat = deltaTransform * curTransform;
	SrVec offset = deltaTransform.get_translation();
	SrVec nextPos = nextMat.get_translation();
	float newPathLength = pathLength + offset.norm();
	SrVec newPathPt = curPath.pathPoint(newPathLength);
	newPathPt.y = 0.f; // remove the y component in path point
	float error = (newPathPt - nextPos).norm();
#else
	float error = 0.f;
	float pathAccumDist = pathLength;
	for (unsigned int i=0;i<nodeCache.pathLocalPts.size();i++)
	{
		pathAccumDist += nodeCache.pathDists[i];
		SrVec newPathPt = curPath.pathPoint(pathAccumDist);
		SrVec nextFramePos = nodeCache.pathLocalPts[i]*curTransform;
		error += (newPathPt-nextFramePos).norm2();
	}
#endif
	return error;
}
*/


struct MotionGraphNode
{
	int nodeIdx;
	int moIndex;
	int startFrame, endFrame;
};

SBAPI void SBMotionGraph::buildAutomaticMotionGraph( const std::vector<std::string>& motionNames, const std::string& skelName, const std::vector<std::string>& endJointNames )
{
	SmartBody::SBAssetManager* assetManager = SmartBody::SBScene::getScene()->getAssetManager();
	MeCtIKTreeScenario ikTree;
	SmartBody::SBSkeleton* skel = assetManager->getSkeleton(skelName);
	if (!skel)
		return;

	SrBox bbox = skel->getBoundingBox();
	float threshold = bbox.getSize().norm()*0.05f;
	std::vector<std::string> stopJointNames = endJointNames;
	ikTree.buildIKTreeFromJointRoot(skel->getJointByName("base"),stopJointNames);
	std::vector<std::string> affectedJointNames;
	for (unsigned int i=0;i<ikTree.ikTreeNodes.size();i++)
	{
		affectedJointNames.push_back(ikTree.ikTreeNodes[i]->joint->getMappedJointName());
	}

	std::vector<std::vector<int> > fromToList; // store all splited frames by transition
	//std::vector<std::map<int,bool> > fromToList;
	fromToList.resize(motionNames.size());
	typedef std::pair<int,int> IntPair;
	std::map<IntPair, std::vector<IntPair> > transitionMap;
	int minFrameMergeDist = 3;
	int minFrameRejectDist = 8;
	for (unsigned int i=0;i<motionNames.size();i++)
	{
		for (unsigned int j=0;j<motionNames.size();j++)
		{
			std::vector<std::pair<int,int> > outTransition;
			//LOG("Building motion transition from '%s' to '%s'",motionNames[i].c_str(),motionNames[j].c_str());
			//computeMotionTransition(motionNames[i],motionNames[j], skelName, affectedJointNames, threshold, outTransition);	
			computeMotionTransitionFast(motionNames[i],motionNames[j], skelName, affectedJointNames, threshold, outTransition);
			std::vector<int>& motion1FromToList = fromToList[i];
			std::vector<int>& motion2FromToList = fromToList[j];
			for (unsigned int k=0;k<outTransition.size();k++)
			{
				std::pair<int,int>& elem = outTransition[k];
				// cluster nearby transition point to avoid very short animation clips
// 				int moElem1 = findClosestElement(motion1FromToList, elem.first);
// 				if (moElem1 != -1 && abs(elem.first - moElem1) < minFrameRejectDist)
// 				{
// 					if (abs(elem.first - moElem1) < minFrameMergeDist)
// 						elem.first = moElem1;
// 					else
// 						continue;
// 				}
// 				int moElem2 = findClosestElement(motion2FromToList, elem.second);
// 				if (moElem2 != -1 && abs(elem.second - moElem2) < minFrameRejectDist)
// 				{
// 					if (abs(elem.first - moElem2) < minFrameMergeDist)
// 						elem.first = moElem2;
// 					else
// 						continue;
// 				}		
				motion1FromToList.push_back(elem.first);
				motion2FromToList.push_back(elem.second);
			}
			//LOG("Num of transitions = %d", outTransition.size());
			transitionMap[IntPair(i,j)] = outTransition;
		}
	}

	// analyze all the transition to build the raw graph
	
	std::vector<std::pair<int,int> > forwardEdgeList;
	std::vector<MotionGraphNode> motionNodeList;
	std::map<IntPair,int> startFrameNodeMap;
	std::map<IntPair,int> endFrameNodeMap;
	int numNodes = 0;
	for (unsigned int i=0;i<fromToList.size();i++)
	{
		std::vector<int>& frameList = fromToList[i];
		frameList.erase(std::unique(frameList.begin(),frameList.end()),frameList.end());
		std::sort(frameList.begin(),frameList.end());
		int prevFrame = 0;				
		bool firstNode = true;
		for (unsigned j=0;j<frameList.size();j++)
		{
			int curFrame = frameList[j];
			//LOG("curFrame = %d", curFrame);
			if (curFrame == prevFrame)
				continue;
			MotionGraphNode node;
			node.nodeIdx = motionNodeList.size();
			node.moIndex = i;
			node.startFrame = prevFrame;
			node.endFrame = curFrame;			
			if (!firstNode)
			{
				// add a default edge to forward the animation
				forwardEdgeList.push_back(IntPair(motionNodeList.size()-1, motionNodeList.size()));
			}
			motionNodeList.push_back(node);
			startFrameNodeMap[IntPair(node.moIndex,node.startFrame)] = node.nodeIdx;
			endFrameNodeMap[IntPair(node.moIndex,node.endFrame)] = node.nodeIdx;
			if (firstNode)
				firstNode = false;
			prevFrame = curFrame;
		}		
	}
	// build a temporary boost graph to compute strong connected components
	typedef boost::adjacency_list<boost::vecS,boost::vecS,boost::directedS> BoostMotionGraph;
	BoostMotionGraph tempMotionGraph(motionNodeList.size());
	// add all forward playback edges
	for (unsigned int i=0;i<forwardEdgeList.size();i++)
	{
		IntPair& edge = forwardEdgeList[i];
		boost::add_edge(edge.first, edge.second, tempMotionGraph);
	}
	std::map<IntPair, std::vector<IntPair> >::iterator mi;
	for ( mi  = transitionMap.begin();
		  mi != transitionMap.end();
		  mi++)
	{
		IntPair mapKey = mi->first;
		int fromMotion = mapKey.first;
		int toMotion = mapKey.second;
		std::vector<IntPair> transitionPoints = mi->second;
		for (unsigned int i=0;i<transitionPoints.size();i++)
		{
			IntPair tPoint = transitionPoints[i];
			std::map<IntPair,int>::iterator fromIter, toIter;
			fromIter = endFrameNodeMap.find(IntPair(fromMotion, tPoint.first));
			toIter   = startFrameNodeMap.find(IntPair(toMotion, tPoint.second));
			if (fromIter == endFrameNodeMap.end() || toIter == startFrameNodeMap.end())
			{
				// This should not happens, some errors ? 
				continue;
			}			
			boost::add_edge(fromIter->second, toIter->second, tempMotionGraph);
		}
	}

	// compute strong connected components
	std::vector<int> compOut(motionNodeList.size());
	int numComps = boost::strong_components(tempMotionGraph, boost::make_iterator_property_map(compOut.begin(),boost::get(boost::vertex_index, tempMotionGraph)));
	int numMaxCompVerts = -1;
	int maxCompIdx = -1;
	// find the largest connected components
	for (int i=0;i<numComps;i++)
	{
		int elemCount = std::count(compOut.begin(),compOut.end(), i);
		if (elemCount > numMaxCompVerts)
		{
			numMaxCompVerts = elemCount;
			maxCompIdx = i;
		}
	}

	// convert the largest connected components into actual motion graph
	std::vector<bool> validNodeList;	
	std::map<int,int> validNodeIdxMap;
	validNodeList.assign(motionNodeList.size(), false);	

	// add all valid motion nodes
	int totalValidFrames = 0;
	float avgTime = 0.f;
	for (unsigned int i=0;i<compOut.size();i++) 
	{
		if (compOut[i] == maxCompIdx)
		{
			int nodeIdx = validNodeIdxMap.size();
			validNodeList[i] = true;
			MotionGraphNode& node = motionNodeList[i];
			std::string motionName = motionNames[node.moIndex];
			std::string nodeName = motionName + "_" + boost::lexical_cast<std::string>(nodeIdx);
			//LOG("Node %s, frame length = %d", nodeName.c_str(), node.endFrame - node.startFrame);
			SBMotionNode* moNode = addMotionNodeFromMotion(nodeName, motionName, node.startFrame, node.endFrame);
			totalValidFrames += node.endFrame-node.startFrame;
			std::vector<float> weights;
			moNode->getRandomBlendWeights(weights);
			avgTime += moNode->getActualDuration(weights);
			validNodeIdxMap[i] = nodeIdx;
		}
	}	
	LOG("Total valid frames in motion graph = %d, avgFrames per node = %f, avgTime per node = %f", totalValidFrames, (float)totalValidFrames/validNodeIdxMap.size(), avgTime/validNodeIdxMap.size());
	// add all valid motion edges
	for (unsigned int i=0;i<forwardEdgeList.size();i++)
	{
		IntPair& edge = forwardEdgeList[i];
		if (!validNodeList[edge.first] || !validNodeList[edge.second])
			continue;
		int mapFromIdx, mapToIdx;
		mapFromIdx = validNodeIdxMap[edge.first];
		mapToIdx = validNodeIdxMap[edge.second];	

		SBMotionNode* fromNode = motionNodes[mapFromIdx];
		SBMotionNode* toNode = motionNodes[mapToIdx];
		SBAnimationBlend* blendFrom = fromNode->getAnimBlend();
		SBAnimationBlend* blendTo = toNode->getAnimBlend();
		//LOG("fromNode frame = %f, toNode frame = %f", blendFrom->getMotionKey(blendFrom->getMotionName(0), 1), blendTo->getMotionKey(blendTo->getMotionName(0), 0));

		SBMotionTransitionEdge* transEdge = addMotionEdgeByIndex(mapFromIdx, mapToIdx);
		transEdge->forwardEdge = true;
	}

	int transitionCount = 0;
	int transitionLength = 10;
	for ( mi  = transitionMap.begin();
		  mi != transitionMap.end();
		  mi++)
	{
		IntPair mapKey = mi->first;
		int fromMotion = mapKey.first;
		int toMotion = mapKey.second;
		std::vector<IntPair> transitionPoints = mi->second;
		for (unsigned int i=0;i<transitionPoints.size();i++)
		{
			IntPair tPoint = transitionPoints[i];
			std::map<IntPair,int>::iterator fromIter, toIter;
			fromIter = endFrameNodeMap.find(IntPair(fromMotion, tPoint.first));
			toIter   = startFrameNodeMap.find(IntPair(toMotion, tPoint.second));
			

			if (fromIter == endFrameNodeMap.end() || toIter == startFrameNodeMap.end())
			{
				// This should not happens, some errors ? 
				continue;
			}			
			int fromIdx, toIdx;
			fromIdx = fromIter->second; 
			toIdx = toIter->second;
			if (!validNodeList[fromIdx] || !validNodeList[toIdx])
				continue;
			int mapFromIdx, mapToIdx;

			if (fromMotion != toMotion || tPoint.first != tPoint.second)
			//if (false)
			{
				std::string moName1,moName2;
				moName1 = motionNames[fromMotion];
				moName2 = motionNames[toMotion];
				std::string nodeName = "transition_";
				nodeName += boost::lexical_cast<std::string>(transitionCount);				
				SBMotionNode* transitionNode = addMotionNodeFromMotionTransition(nodeName, moName1, moName2, tPoint.first, tPoint.second, transitionLength);
				mapFromIdx = validNodeIdxMap[fromIdx];
				mapToIdx = validNodeIdxMap[toIdx];
				addMotionEdgeByIndex(mapFromIdx, transitionNode->getIndex());
				addMotionEdgeByIndex(transitionNode->getIndex(), mapToIdx);
				transitionCount++;
			}
			else // simply connect the edge
			{
				//LOG("forward edge, fromMotion = %d, toMotion = %d, fromFrame = %d, toFrame = %d", fromMotion, toMotion, tPoint.first, tPoint.second);
				mapFromIdx = validNodeIdxMap[fromIdx];
				mapToIdx = validNodeIdxMap[toIdx];
				addMotionEdgeByIndex(mapFromIdx, mapToIdx);
			}			
		}
	}
	LOG("total number of graph nodes = %d, graph edges = %d", motionNodes.size(), motionEdges.size());
	// finish building the motion graph.
}


void SBMotionGraph::computeMotionTransitionFast( const std::string& moName1, const std::string& moName2, const std::string& skelName, const std::vector<std::string>& affectedJointNames, float threshold, std::vector<std::pair<int,int> >& outTransition )
{
	SmartBody::SBAssetManager* assetManager = SmartBody::SBScene::getScene()->getAssetManager();
	SmartBody::SBMotion* motion1 = assetManager->getMotion(moName1);
	SmartBody::SBMotion* motion2 = assetManager->getMotion(moName2);
	SmartBody::SBSkeleton* moSkel = assetManager->getSkeleton(skelName);
	if (!motion1 || !motion2 || !moSkel) return;
	SmartBody::SBSkeleton* skelCopy1 = new SBSkeleton(moSkel);
	SmartBody::SBSkeleton* skelCopy2 = NULL;
	if (motion1 == motion2)
		skelCopy2 = skelCopy1;
	else
		skelCopy2 = new SBSkeleton(moSkel);
	motion1->connect(skelCopy1);
	motion2->connect(skelCopy2);
	int mo1Frames = motion1->getNumFrames();
	int mo2Frames = motion2->getNumFrames();
	//mo1Frames = 1000;
	//mo2Frames = 1000;

	// precompute all frame pair data
	MotionCoordCache m;
	m.avgX1.resize(mo1Frames); m.avgZ1.resize(mo1Frames); m.avgF1.resize(mo1Frames);
	m.avgX2.resize(mo2Frames); m.avgZ2.resize(mo2Frames); m.avgF2.resize(mo2Frames);
	std::vector<SrVec> pointCloud1, pointCloud2;
	pointCloud1.resize(affectedJointNames.size());
	pointCloud2.resize(affectedJointNames.size());	
	std::vector<std::vector<SrVec> > pointCloudList1, pointCloudList2;
	pointCloudList1.resize(mo1Frames);
	pointCloudList2.resize(mo2Frames);
	float invNumPoints = 1.f/affectedJointNames.size();
	for (unsigned int i=0;i<mo1Frames;i++)
	{
		motion1->apply_frame(i); 
		skelCopy1->update_global_matrices();
		skelCopy1->getJointPositions(affectedJointNames, pointCloud1, 0);
		float x1 = 0.f, z1 = 0.f, f1 = 0.f;
		for (unsigned int p=0;p<pointCloud1.size();p++)
		{
			x1 += pointCloud1[p].x;
			z1 += pointCloud1[p].z;
			f1 += pointCloud1[p].norm2();
		}	
		m.avgX1[i] = x1*invNumPoints;
		m.avgZ1[i] = z1*invNumPoints;
		m.avgF1[i] = f1*invNumPoints;			
		pointCloudList1[i] = pointCloud1;
	}
	for (unsigned int i=0;i<mo2Frames;i++)
	{
		motion2->apply_frame(i); 
		skelCopy2->update_global_matrices();
		skelCopy2->getJointPositions(affectedJointNames, pointCloud2, 0);
		float x2 = 0.f, z2 = 0.f, f2 = 0.f;
		for (unsigned int p=0;p<pointCloud2.size();p++)
		{
			x2 += pointCloud2[p].x;
			z2 += pointCloud2[p].z;
			f2 += pointCloud2[p].norm2();
		}	
		m.avgX2[i] = x2*invNumPoints;
		m.avgZ2[i] = z2*invNumPoints;
		m.avgF2[i] = f2*invNumPoints;	
		pointCloudList2[i] = pointCloud2;
	}
	m.xxzzMat.resize(mo1Frames,mo2Frames);
	m.xzzxMat.resize(mo1Frames,mo2Frames);
	m.yyMat.resize(mo1Frames,mo2Frames);
	for (unsigned int i=0;i<mo1Frames;i++)
	{
		std::vector<SrVec>& pcloud1 = pointCloudList1[i];
		for (unsigned int j=0;j<mo2Frames;j++)
		{
			std::vector<SrVec>& pcloud2 = pointCloudList2[j];
			float xxzz = 0.f, xzzx = 0.f, yy = 0.f;
			for (unsigned int p=0;p<pcloud1.size();p++)
			{
				xxzz += pcloud1[p].x*pcloud2[p].x + pcloud1[p].z*pcloud2[p].z;
				xzzx += pcloud1[p].x*pcloud2[p].z - pcloud1[p].z*pcloud2[p].x;
				yy   += pcloud1[p].y*pcloud2[p].y;
			}
			m.xxzzMat(i,j) = xxzz*invNumPoints;
			m.xzzxMat(i,j) = xzzx*invNumPoints;
			m.yyMat(i,j) = yy*invNumPoints;
		}
	}

	dMatrix transitionMat;
	transitionMat.resize(mo1Frames,mo2Frames);
	int windowSize = 7;
	for (int i=0;i<mo1Frames;i++)
	{
		for (int j=0;j<mo2Frames;j++)
		{
			float transError = computeTransitionErrorFast(m,i,j,windowSize);
			//LOG("transition error (%d, %d) = %f", i,j, transError);
			transitionMat(i,j) = transError;
		}
	}

	for (int i=0;i<mo1Frames;i++)
	{
		for (int j=0;j<mo2Frames;j++)
		{			
			if (motion1 == motion2 && abs(i-j) <= 20) // skip the diagonal area of the motion
				continue;
			// determine if i,j is a local minimum
			bool isLocalMinimum = findLocalMinimum(transitionMat,i,j);
			if (isLocalMinimum && transitionMat(i,j) <= threshold)
				outTransition.push_back(std::pair<int,int>(i,j));
		}
	}

	std::string outImgName = moName1 + "_to_" + moName2 + ".bmp";
	writeOutTransitionMap(outImgName, transitionMat, outTransition);
}


float SBMotionGraph::computeTransitionErrorFast( MotionCoordCache& cache, int frame1, int frame2, int windowSize )
{
	float transError = 0.f;

	int numFrames1 = cache.avgF1.size();
	int numFrames2 = cache.avgF2.size();
	float divSum = 1.f/(windowSize*2+1);
	float sumX1 = 0.f, sumX2 = 0.f, sumZ1 = 0.f, sumZ2 = 0.f, sumXXZZ = 0.f, sumXZZX = 0.f, sumYY = 0.f, sumF1 = 0.f, sumF2 = 0.f;
	//for (int i=-windowSize;i<=windowSize;i++)
	for (int i=0;i<windowSize*2+1;i++)
	{
		int idx1 = frame1 + i;
		int idx2 = frame2 - (windowSize*2 - i);
		if (idx1 < 0) idx1 = 0;
		if (idx1 >= numFrames1) idx1 = numFrames1-1;
		if (idx2 < 0) idx2 = 0;
		if (idx2 >= numFrames2) idx2 = numFrames2-1;

		sumX1 += cache.avgX1[idx1];
		sumX2 += cache.avgX2[idx2];

		sumZ1 += cache.avgZ1[idx1];
		sumZ2 += cache.avgZ2[idx2];

		sumF1 += cache.avgF1[idx1];
		sumF2 += cache.avgF2[idx2];

		sumXXZZ += cache.xxzzMat(idx1,idx2);
		sumXZZX += cache.xzzxMat(idx1,idx2);
		sumYY += cache.yyMat(idx1,idx2);
	}
	sumX1 *= divSum;
	sumX2 *= divSum;
	sumZ1 *= divSum;
	sumZ2 *= divSum;
	sumF1 *= divSum;
	sumF2 *= divSum;

	sumXXZZ *= divSum;
	sumXZZX *= divSum;
	sumYY   *= divSum;
	
	float yNumerator = sumXZZX - (sumX1*sumZ2 - sumZ1*sumX2);
	float yDenominator = sumXXZZ - (sumX1*sumX2 + sumZ1*sumZ2);
	float thetaY = atan2(yNumerator, yDenominator);
	float x0 = sumX1 - sumX2*cos(thetaY) - sumZ2*sin(thetaY);
	float z0 = sumZ1 + sumX2*sin(thetaY) - sumZ2*cos(thetaY);
	float a = cos(thetaY)*(-sumXXZZ + x0*sumX2 + z0*sumZ2);
	float b = sin(thetaY)*(-sumXZZX + x0*sumZ2 - z0*sumX2);
	float c = -sumYY - x0*sumX1 - z0*sumZ1;
	transError = sumF1 + sumF2 + 2.f*(a+b+c) + x0*x0 + z0*z0;

	return transError;
}


#if 0
void SBMotionGraph::computeMotionTransition( const std::string& moName1, const std::string& moName2, const std::string& skelName, const std::vector<std::string>& affectedJointNames, float threshold, std::vector<std::pair<int,int> >& outTransition )
{
	SmartBody::SBAssetManager* assetManager = SmartBody::SBScene::getScene()->getAssetManager();
	SmartBody::SBMotion* motion1 = assetManager->getMotion(moName1);
	SmartBody::SBMotion* motion2 = assetManager->getMotion(moName2);
	SmartBody::SBSkeleton* moSkel = assetManager->getSkeleton(skelName);
	if (!motion1 || !motion2 || !moSkel) return;
	SmartBody::SBSkeleton* skelCopy1 = new SBSkeleton(moSkel);
	SmartBody::SBSkeleton* skelCopy2 = NULL;
	if (motion1 == motion2)
		skelCopy2 = skelCopy1;
	else
		skelCopy2 = new SBSkeleton(moSkel);
	motion1->connect(skelCopy1);
	motion2->connect(skelCopy2);
	int mo1Frames = motion1->getNumFrames();
	int mo2Frames = motion2->getNumFrames();
	std::vector<SrVec> pointCloud1, pointCloud2;
	std::vector<SrVec> curCloud1, curCloud2;
	int windowSize = 3;
	pointCloud1.resize(windowSize*affectedJointNames.size());
	pointCloud2.resize(windowSize*affectedJointNames.size());
	for (int i=0;i<windowSize;i++)
	{
		motion1->apply_frame(0); 
		motion2->apply_frame(0); 
		skelCopy1->update_global_matrices();
		skelCopy2->update_global_matrices();
		skelCopy1->getJointPositions(affectedJointNames, pointCloud1, i*affectedJointNames.size());
		skelCopy2->getJointPositions(affectedJointNames, pointCloud2, i*affectedJointNames.size());
	}
	curCloud1 = pointCloud1;
	curCloud2 = pointCloud2;

	dMatrix transitionMat;
	transitionMat.resize(mo1Frames,mo2Frames);
	int posIdx1 = (windowSize-1), posIdx2 = (windowSize-1);
	for (int i=0;i<mo1Frames;i++)
	{
		motion1->apply_frame(i);
		skelCopy1->update_global_matrices();
		skelCopy1->getJointPositions(affectedJointNames, curCloud1, posIdx1*affectedJointNames.size());
		curCloud2 = pointCloud2;
		posIdx2 = (windowSize-1);
		if (i%100 == 0)
			LOG("iteration : Frame %d", i);
		for (int j=0;j<mo2Frames;j++)
		{
			motion2->apply_frame(j);
			skelCopy2->update_global_matrices();
			skelCopy2->getJointPositions(affectedJointNames, curCloud2, posIdx2*affectedJointNames.size());
			float transError = computeTransitionError(curCloud1, curCloud2, posIdx1*affectedJointNames.size(), posIdx2*affectedJointNames.size());
			//LOG("transition error (%d, %d) = %f", i,j, transError);
			transitionMat(i,j) = transError;
			posIdx2 = (posIdx2+1)%(windowSize);
		}
		posIdx1 = (posIdx1+1)%(windowSize);
	}

	for (int i=0;i<mo1Frames;i++)
	{
		for (int j=0;j<mo2Frames;j++)
		{			
			if (motion1 == motion2 && abs(i-j) <= 10) // skip the diagonal area of the motion
				continue;
			// determine if i,j is a local minimum
			bool isLocalMinimum = findLocalMinimum(transitionMat,i,j);
			if (isLocalMinimum && transitionMat(i,j) <= threshold)
				outTransition.push_back(std::pair<int,int>(i,j));
		}
	}

	std::string outImgName = moName1 + "_to_" + moName2 + ".bmp";
	writeOutTransitionMap(outImgName, transitionMat, outTransition);
}
#endif

#if 0
float SBMotionGraph::computeTransitionError( const std::vector<SrVec>& pos1, const std::vector<SrVec>& pos2, int idx1, int idx2 )
{
	if (pos1.size() == 0 || pos1.size() != pos2.size())
		return -1.f;

	float thetaY = 0.f;
	float x0, z0;
	SrVec sumPos1, sumPos2;
	for (unsigned int i=0;i<pos1.size();i++)
	{
		sumPos1 += pos1[i];
		sumPos2 += pos2[i];
	}
	float divSum = 1.f/pos1.size();
	float xzMinusxz = 0.f, xxPluszz = 0.f;
	for (unsigned int i=0;i<pos1.size();i++)
	{
		int pos1Idx = (idx1+i)%pos1.size();
		int pos2Idx = (idx2+i)%pos2.size();
		xzMinusxz += pos1[pos1Idx].x*pos2[pos2Idx].z - pos1[pos1Idx].z*pos2[pos2Idx].x;
		xxPluszz  += pos1[pos1Idx].x*pos2[pos2Idx].x + pos1[pos1Idx].z*pos2[pos2Idx].z;
	}
	float yNumerator = xzMinusxz-divSum*(sumPos1.x*sumPos2.z-sumPos2.x*sumPos1.z);
	float yDenominator = xxPluszz - divSum*(sumPos1.x*sumPos2.x + sumPos1.z*sumPos2.z);
	//thetaY = atan(yNumerator/yDenominator);
	thetaY = atan2(yNumerator, yDenominator);
	x0 = divSum*(sumPos1.x - sumPos2.x*cos(thetaY) - sumPos2.z*sin(thetaY));
	z0 = divSum*(sumPos1.z + sumPos2.x*sin(thetaY) - sumPos2.z*cos(thetaY));
	SrVec yAxis = SrVec(0,1,0);
	SrQuat yRot = SrQuat(yAxis,thetaY);
	yRot.normalize();
	SrMat alignTransform;
	alignTransform = yRot.get_mat(alignTransform);
	alignTransform.set_translation(SrVec(x0,0,z0));

	float transitionError = 0.f;
	for (unsigned int i=0;i<pos1.size();i++)
	{
		int pos1Idx = (idx1+i)%pos1.size();
		int pos2Idx = (idx2+i)%pos2.size();
		SrVec diffVec = pos1[pos1Idx] - pos2[pos2Idx] *alignTransform;
		transitionError += diffVec.norm2();
	}
	transitionError *= divSum;
	return sqrt(transitionError); // the average distance between two joints
}
#endif

bool SBMotionGraph::findLocalMinimum( dMatrix& mat, int i, int j )
{
	int windowSize = 1;
	if (i < 0 || i >= mat.size1() || j < 0 || j >= mat.size1())
		return false;
	float centerVal = mat(i,j);
	for (int m=-windowSize;m<=windowSize;m++)
	{
		for (int n=-windowSize;n<=windowSize;n++)
		{
			if (m==0 && n==0)
				continue;
			int idx1,idx2;
			idx1 = m+i;
			idx2 = n+j;
			if (idx1 < 0 || idx1 >= mat.size1() || idx2 < 0 || idx2 >= mat.size2())
				continue;
			if (centerVal > mat(idx1,idx2))
				return false;
		}
	}
	return true;
}

SBAPI std::vector<std::string> SBMotionGraph::getMotionNodeNames()
{
	std::vector<std::string> nodeNames;
	for (unsigned int i=0;i<motionNodes.size();i++)
	{
		nodeNames.push_back(motionNodes[i]->getName());
	}
	return nodeNames;
}

// temp function for debugging
void SBMotionGraph::writeOutTransitionMap( const std::string& outfilename, const dMatrix& mat, const std::vector<std::pair<int,int> >& transitions )
{
	double maxVal = -1.f;
	for (unsigned int i=0;i<mat.size1();i++)
		for (unsigned int j=0;j<mat.size2();j++)
		{
			if (maxVal < mat(i,j))
				maxVal = mat(i,j);
		}
	
	unsigned char* imgBuf = NULL;
	imgBuf = new unsigned char[mat.size1()*mat.size2()*3]; // create a color img
	for (unsigned int i=0;i<mat.size1();i++)
	{
		for (unsigned int j=0;j<mat.size2();j++)
		{
			float imgVal = mat(i,j)/maxVal;
			unsigned char imgByte = imgVal*255;
			imgBuf[i*mat.size2()*3 + j*3 ] = imgByte;
			imgBuf[i*mat.size2()*3 + j*3 + 1] = imgByte;
			imgBuf[i*mat.size2()*3 + j*3 + 2] = imgByte;
		}
	}

	for (unsigned int i=0;i<transitions.size();i++)
	{
		std::pair<int,int> trans = transitions[i];
		int x,y; 
		x = trans.first;
		y = trans.second;
		imgBuf[x*mat.size2()*3 + y*3 ] = 0;
		imgBuf[x*mat.size2()*3 + y*3 + 1] = 255;
		imgBuf[x*mat.size2()*3 + y*3 + 2] = 0;
	}

#ifdef EMBER_SB_SOIL
	SOIL_save_image(
		outfilename.c_str(),
		SOIL_SAVE_TYPE_BMP,
		mat.size1(),mat.size2(), 3,
		imgBuf
		);
#endif
}

int SBMotionGraph::findClosestElement( const std::vector<int>& intList, int val )
{
	int minDist = 1e30;
	int minDistElem = -1;
	for (unsigned int i=0;i<intList.size();i++)
	{
		int dist = abs(intList[i]-val);
		if (dist < minDist)
		{
			minDist = dist;
			minDistElem = intList[i];
		}
	}
	return minDistElem;
}


/************************************************************************/
/* SBMotionNodeState                                                    */
/************************************************************************/

SBMotionNodeState::SBMotionNodeState()
{
	motionNode = NULL;
	prevNode = NULL;
	nextNode = NULL;
	currentRefTime = 0.f;
	transitionLength = 0.5f;
	graphWalkType = RANDOM_WALK;
	useTransitionInterpolation = true;
}

SBMotionNodeState::~SBMotionNodeState()
{

}

void SBMotionNodeState::initState( SkChannelArray& channelArray, std::vector<std::string>& affectedChannelList, const std::string& baseName )
{
	prevBuffer.initFrameBuffer(channelArray, affectedChannelList);
	curBuffer.initFrameBuffer(channelArray, affectedChannelList);
	nextBuffer.initFrameBuffer(channelArray, affectedChannelList);
	baseJointName = baseName;
}

float SBMotionNodeState::getCurrentRefTime()
{
	return currentRefTime;
}

SBMotionNode* SBMotionNodeState::getCurrentMotionNode()
{
	return motionNode;
}

SBMotionFrameBuffer& SBMotionNodeState::getCurMotionFrameBuffer()
{
	return curBuffer;
}

SmartBody::SBMotionNodeState::GraphWalkType SBMotionNodeState::getGraphWalkType() const
{
	return graphWalkType;
}

void SBMotionNodeState::setGraphWalkType( GraphWalkType val )
{
	graphWalkType = val;	
}

bool SBMotionNodeState::addNextTransition( SBMotionTransitionEdge* edge, std::vector<float>& transitionWeight )
{
	if (transitionQueue.empty() && edge->getSrcNode() == motionNode) // valid transition edge	
	{
		transitionQueue.push(SBTransitionRecord(edge, transitionWeight));
		updateNextStateBufferCache();
		return true;
	}
	else if (transitionQueue.back().first->getTgtNode() == edge->getSrcNode()) // the tgt node of last transition is the same as the src node of this transition edge
	{
		transitionQueue.push(SBTransitionRecord(edge, transitionWeight));		
		return true;
	}
	else // error
	{
		LOG("Error, edge '%s' is not valid for current state.", edge->getName().c_str());
		return false;		
	}
}


float SBMotionNodeState::applyTransition( float dt, float curDu )
{
	float newDu = -1.f;
	if (transitionQueue.empty()) // no available transition
		return newDu;
	bool hasTransition = false;
	SBMotionTransitionEdge* transEdge = NULL;
	std::vector<float> transWeight;
	while (!hasTransition && !transitionQueue.empty())
	{
		SBTransitionRecord record = transitionQueue.front();
		SBMotionTransitionEdge* edge = record.first;
		if (edge->getSrcNode() == motionNode) // a valid transition
		{
			transEdge = edge;
			transWeight = record.second;
			hasTransition = true;
		}
		transitionQueue.pop();
	}

	if (!hasTransition) 
		return newDu;
	
	// cache the last frame of the current motion node
	SrMat woMat, baseXZ;
	motionNode->getBlendedMotionFrame(currentRefTime, blendWeights, prevBuffer); 
	motionNode->getDeltaAlignTransform(currentRefTime, 0.f, blendWeights, woMat, baseXZ);
	//prevBuffer = curBuffer;
	prevBuffer.setMat(baseJointName, baseXZ);

	// handle the transition
	SBMotionNode* targetNode = transEdge->getTgtNode();

	float dtLeft = motionNode->getActualDuration(blendWeights) - motionNode->getActualTime(currentRefTime, blendWeights);
	float dtNext = dt - dtLeft;
	float duRatio = 1.f - (motionNode->getRefDuration() - currentRefTime)/curDu;
	//if (transEdge->forwardEdge)
	//	LOG("Edge is forward edge");
	//LOG("dtLeft = %f, motionDuration = %f, motionActualTime = %f", dtLeft, motionNode->getActualDuration(blendWeights), motionNode->getActualTime(currentRefTime, blendWeights));
	//LOG("duRatio = %f, newDt = %f, dtNext = %f",duRatio, dt*duRatio, dtNext);
	currentRefTime = 0.f; // a simple hack for now. later should handle multiple transition points
	newDu = targetNode->getRefDeltaTime(currentRefTime, dtNext, transWeight);

	

	// set the state to the next node
	prevNode = motionNode;
	motionNode = targetNode;
	blendWeights = transWeight;

	//LOG("transition from node %s to node %s", prevNode->getName().c_str(), motionNode->getName().c_str());
	// also update the cache for the next transition if available
	// caveat : should we do it here, or just-in-time when necessary ?
	updateNextStateBufferCache(); 
	return newDu;
}


void SBMotionNodeState::updateNextStateBufferCache()
{	
	if (transitionQueue.empty())
		return;
	SBTransitionRecord record = transitionQueue.front();
	SBMotionTransitionEdge* edge = record.first;
	if (edge->getSrcNode() == motionNode) // a valid next node
	{
		SrMat woMat, baseXZ;
		float refTime = 0.f;
		SBMotionNode* tgtNode = edge->getTgtNode();
		tgtNode->getBlendedMotionFrame(refTime, record.second, nextBuffer); 
		tgtNode->getDeltaAlignTransform(refTime, 0.f, record.second, woMat, baseXZ);
		nextBuffer.setMat(baseJointName, baseXZ);
		nextNode = edge->getTgtNode();
	}
	else
	{
		nextNode = NULL; // don't have a valid next node ?
	}
}

SrMat SBMotionNodeState::stepMotionState( float dt )
{
	SrMat deltaWO;
	if (!motionNode)
		return deltaWO; // no state to advance

	// a hack to add a random transition
	if (transitionQueue.empty() && getGraphWalkType() == RANDOM_WALK)
	{		
		const std::vector<SBMotionTransitionEdge*>& outEdges = motionNode->getOutEdges();
		SrRandom random(0, outEdges.size()-1);		
		// pick an random edge
		int iEdge = random.geti();
		SBMotionTransitionEdge* edge = outEdges[iEdge];
		// generate random weights
		SBMotionNode* tgtNode = edge->getTgtNode();	
		std::vector<float> weights;
		tgtNode->getRandomBlendWeights(weights);
		addNextTransition(edge, weights);
	}

	float du = motionNode->getRefDeltaTime(currentRefTime, dt, blendWeights);
	float newRefTime = currentRefTime + du;	

	if (newRefTime >= motionNode->getRefDuration()) // need to go to the next state
	{
		// handle transition here
		du = applyTransition(dt,du);
		newRefTime = currentRefTime + du;				
		if (du < 0.f) // no available transition, freeze the current time
			return deltaWO;
	}		
	//float actualTime = motionNode->getActualTime(newRefTime, blendWeights);	
	float actualDuration = motionNode->getActualDuration(blendWeights);
	float actualTime = actualDuration*newRefTime; // since reftime is [0,1]
	if (actualTime > actualDuration) actualTime = actualDuration;

	float activeTransitionLength = transitionLength;
	if (activeTransitionLength > actualDuration*0.5f)
		activeTransitionLength = actualDuration*0.5f;
	// get the current frame
	motionNode->getBlendedMotionFrame(newRefTime, blendWeights, curBuffer);
	SrMat baseXZ;
	motionNode->getDeltaAlignTransform(currentRefTime, du, blendWeights, deltaWO, baseXZ );
	SrMat tempMat = SrMat::id;
	SrVec p;
	curBuffer.setMat(baseJointName, baseXZ);

	// smooth-out the discontinuity during transition
// 	if (useTransitionInterpolation)
// 	{
// 		if (actualTime <= activeTransitionLength && prevNode) // transition in
// 		{
// 			float ratio = (actualTime+activeTransitionLength)/activeTransitionLength;
// 			float yf =  fabs(-0.5f*ratio*ratio + 2.f*ratio - 2.f); // transition weight
// 			//LOG("trans In, yf = %f",yf);
// 			SBMotionFrameBuffer::interpMotionFrameBuffer(curBuffer,prevBuffer,yf, curBuffer);
// 
// 		}
// 		else if (actualTime > actualDuration - activeTransitionLength && nextNode) // transition out
// 		{
// 			float ratio = (actualTime+activeTransitionLength-actualDuration)/activeTransitionLength;
// 			float yf =  fabs(0.5f*ratio*ratio); // transition weight
// 			//LOG("trans Out, yf = %f", yf);
// 			SBMotionFrameBuffer::interpMotionFrameBuffer(curBuffer,nextBuffer,yf, curBuffer);
// 		}
// 	}	
	currentRefTime = newRefTime;
	return deltaWO;
}

void SBMotionNodeState::startMotionState( SBMotionNode* startNode, std::vector<float>& startWeights )
{
	motionNode = startNode;
	currentRefTime = 0.f;
	blendWeights = startWeights;	
}

bool SBMotionNodeState::isRunning()
{
	if (motionNode)
		return true;
	else
		return false;
}

/************************************************************************/
/* SBMotionTransitionEdge                                               */
/************************************************************************/
SBMotionTransitionEdge::SBMotionTransitionEdge(int idx)
{
	srcNode = NULL;
	tgtNode = NULL;
	srcRefTime = 0.f;
	tgtRefTime = 0.f;
	index = idx;
	forwardEdge = false;
}

SBMotionTransitionEdge::SBMotionTransitionEdge()
{
	forwardEdge = false;
}
SBMotionTransitionEdge::~SBMotionTransitionEdge()
{

}

void SBMotionTransitionEdge::initTransitionEdge( SBMotionNode* nodeFrom, SBMotionNode* nodeTo, float srcTime, float dstTime )
{
	srcNode = nodeFrom;
	tgtNode = nodeTo;
	srcRefTime = srcTime;
	tgtRefTime = dstTime;
}

void SBMotionTransitionEdge::getSimpleTransition( float desiredSourceRefTime, float& uSrc, float& uTgt )
{
	uSrc = srcRefTime;
	uTgt = tgtRefTime;
}

void SBMotionTransitionEdge::getParametricTransition( float desiredSourceRefTime, const std::vector<float>& srcParameter, float& uSrc, float& uTgt, std::vector<float>& tgtParameter )
{
	// not implemented for now
}

int SBMotionTransitionEdge::getIndex()
{
	return index;
}

std::string SBMotionTransitionEdge::getName()
{
	return srcNode->getName() + "_" + tgtNode->getName();
}

SBMotionNode* SBMotionTransitionEdge::getSrcNode()
{
	return srcNode;
}

SBMotionNode* SBMotionTransitionEdge::getTgtNode()
{
	return tgtNode;
}

/************************************************************************/
/* SBMotionGraph Manager                                                */
/************************************************************************/
SBAPI SBMotionGraphManager::SBMotionGraphManager()
{

}

SBAPI SBMotionGraphManager::~SBMotionGraphManager()
{

}

SBAPI SBMotionGraph* SBMotionGraphManager::createMotionGraph( const std::string& moGraphName )
{
	SBMotionGraph* moGraph = getMotionGraph(moGraphName);
	if (moGraph)
	{
		LOG("Warning, Can't Create Motion Graph '%s', already exists.");
		return NULL;
	}

	moGraph = new SBMotionGraph();
	_motionGraphMap[moGraphName] = moGraph;
	return moGraph;
}

SBAPI SBMotionGraph* SBMotionGraphManager::getMotionGraph( const std::string& moGraphName )
{
	std::map<std::string, SBMotionGraph*>::iterator mi;
	mi = _motionGraphMap.find(moGraphName);
	if (mi != _motionGraphMap.end())
		return mi->second;
	return NULL;
}

SBAPI std::vector<std::string> SBMotionGraphManager::getMotionGraphNames()
{
	std::vector<std::string> moGraphNames;
	std::map<std::string, SBMotionGraph*>::iterator mi;
	for ( mi  = _motionGraphMap.begin();
		  mi != _motionGraphMap.end();
		  mi++)
	{
		moGraphNames.push_back(mi->first);
	}
	return moGraphNames;
}


} // namespace