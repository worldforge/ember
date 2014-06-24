#include "MotionAnalysis.h"
#include "components/sb/sb/SBScene.h"
#include "components/sb/sb/SBAssetManager.h"
#ifdef EMBER_SB_STEER
	#include "components/sb/sb/SBNavigationMesh.h"
#endif
#ifdef EMBER_SB_HEIGHTFIELD
	#include "components/sb/sbm/Heightfield.h" // ?
#endif
/************************************************************************/
/* Locomotion Leg Cycle                                                 */
/************************************************************************/


float LocomotionLegCycle::getNormalizedFlightTime( float motionTime )
{
	float normalizedTime = getNormalizedCycleTime(motionTime);
	if (normalizedTime < normalizedLiftTime) return 0.f;
	if (normalizedTime > normalizedLandTime) return 1.f;
	return (normalizedTime-normalizedLiftTime)/(normalizedLandTime-normalizedLiftTime);
}

float LocomotionLegCycle::getNormalizedCycleTime( float motionTime )
{
	float warpMotionTime = motionTime;
	if (cycleStartTime > cycleEndTime && motionTime < cycleEndTime) // loop back to beginning
		warpMotionTime = motionEndTime + (motionTime-motionStartTime);
	float cycleTime = (warpMotionTime-stanceTime)/cycleDuration; // normalized cycle time
	return cycleTime;
}

SrVec LocomotionLegCycle::getStanceSupportPos( int idx )
{
	return stanceSupportPos[idx];
}

SrVec LocomotionLegCycle::getSupportPos( float motionTime, int idx )
{	
	float cycleTime = getNormalizedCycleTime(motionTime);
// 	int index = (int)(cycleTime*(samples.size()-1));
// 	float weight = cycleTime*(samples.size()-1) - index;
// 
// 	SrVec supportPos = samples[index].supportPos[idx]*(1.f-weight) + samples[(index+1)%samples.size()].supportPos[idx]*weight;
// 	return supportPos;
	return getSupportPosNormalize(cycleTime, idx);
}


SrVec LocomotionLegCycle::getSupportPosNormalize( float normalizeTime, int idx )
{
	float cycleTime = normalizeTime;
	int index = (int)(cycleTime*(samples.size()-1));
	float weight = cycleTime*(samples.size()-1) - index;

	SrVec supportPos = samples[index].supportPos[idx]*(1.f-weight) + samples[(index+1)%samples.size()].supportPos[idx]*weight;
	return supportPos;
}

float LocomotionLegCycle::getTransitionWeight( float motionTime, int& phase )
{
	// normalize the time to [0,1]
	float normalizedMotionTime = getNormalizedCycleTime(motionTime);
	float weight = 1.0;
	if (normalizedMotionTime >= normalizeStrikeTime && normalizedMotionTime <= normalizedLandTime) // transition to landing
	{ 
		weight = 1.0;		
		phase = 0;
	}
	else if (normalizedMotionTime >= normalizedLandTime || normalizedMotionTime <= normalizedLiftTime) // foot step
	{
		weight = 0.0;
		phase = 1;
	}
	else if (normalizedMotionTime >= normalizedLiftTime && normalizedMotionTime <= normalizeLiftoffTime) // transition to lifting
	{
		weight = (normalizedMotionTime-normalizedLiftTime)/(normalizeLiftoffTime-normalizedLiftTime);
		phase = 2;
	}
	else
	{
		weight = 1.0;	
		phase = 3;
	}
	return weight;
}

/************************************************************************/
/* Locomotion Analyzer                                                  */
/************************************************************************/
LocomotionAnalyzer::LocomotionAnalyzer()
{

}

LocomotionAnalyzer::~LocomotionAnalyzer()
{
}


LocomotionLegCycle* LocomotionAnalyzer::getLegCycleByIndex( int iLeg, int cycleIdx )
{
	LegCycleVec& legCycleVec = legCycleMap[iLeg];
	if (cycleIdx >= 0 && cycleIdx < (int)legCycleVec.size())
		return &legCycleVec[cycleIdx];
	return NULL;
}

LocomotionLegCycle* LocomotionAnalyzer::getLegCycle( int iLeg, float motionTime )
{
	LegCycleVec& legCycleVec = legCycleMap[iLeg];
	for (unsigned int i=0;i<legCycleVec.size();i++)
	{
		int iNext = (i+1)%legCycleVec.size();
		float s1 = legCycleVec[i].stanceTime;
		float s2 = legCycleVec[iNext].stanceTime;
		if (s1 > s2) // loop back to beginning
		{
			if (motionTime >= s1 || motionTime < s2)
				return &legCycleVec[i];
		}
		else // normal interval
		{
			if (motionTime >= s1 && motionTime < s2)
				return &legCycleVec[i];
		}
	}
	return NULL;
}

void LocomotionAnalyzer::initLegCycles( const std::string& name, SmartBody::SBAnimationBlend* locoBlend, KeyTagMap& keyTagMap, SmartBody::SBSkeleton* skelCopy )
{
	KeyTagMap::iterator mi;
	//startTime = (float)locoBlend->getMotionKey(name, 0);
	//endTime = (float)locoBlend->getMotionKey(name, locoBlend->getNumKeys()-1);
	int nSample = 50;
    motionName = name;
	SmartBody::SBAssetManager* assetManager = SmartBody::SBScene::getScene()->getAssetManager();
	SmartBody::SBMotion* sbMotion = assetManager->getMotion(motionName);
	startTime = 0.f;
	endTime = (float)sbMotion->getDuration();
	for ( mi  = keyTagMap.begin(); mi != keyTagMap.end(); mi++)
	{
		KeyTag& keyTag = mi->second;
		if (keyTag.size() == 0) continue;
		legCycleMap[mi->first] = LegCycleVec();
		LegCycleVec& legCycleVec = legCycleMap[mi->first];
		KeyTag::iterator ki = keyTag.begin();
		int nCycles = ki->second.size();
		for (int i=0;i<nCycles;i++)
		{
			legCycleVec.push_back(LocomotionLegCycle());
			LocomotionLegCycle& legCycle = legCycleVec[i];
			legCycle.cycleIdx = i;
			legCycle.stanceTime = (float)getKeyTagTime("stance",i,keyTag);
			legCycle.liftTime = (float)getKeyTagTime("lift",i,keyTag);
			legCycle.liftoffTime = (float)getKeyTagTime("liftoff",i,keyTag);
			legCycle.strikeTime = (float)getKeyTagTime("strike",i,keyTag);
			legCycle.landTime = (float)getKeyTagTime("land",i,keyTag);
			legCycle.motionStartTime = startTime;
			legCycle.motionEndTime = endTime;
		}
		for (int i=0;i<nCycles;i++) // compute cycle duration
		{
			LocomotionLegCycle& c1 = legCycleVec[i];
			LocomotionLegCycle& c2 = legCycleVec[(i+1)%nCycles];
			if (c2.stanceTime > c1.stanceTime)
			{
				c1.cycleDuration = c2.stanceTime - c1.stanceTime;				
			}
			else
			{
				c1.cycleDuration = (endTime-c1.stanceTime) + (c2.stanceTime - startTime);
			}
			// compute cycle time and normalized time
			c1.cycleStartTime = c1.stanceTime;
			c1.cycleEndTime = c2.stanceTime;

			c1.normlizedStanceTime = c1.getNormalizedCycleTime(c1.stanceTime);
			c1.normalizedLiftTime = c1.getNormalizedCycleTime(c1.liftTime);
			c1.normalizeLiftoffTime = c1.getNormalizedCycleTime(c1.liftoffTime);
			c1.normalizeStrikeTime = c1.getNormalizedCycleTime(c1.strikeTime);
			c1.normalizedLandTime = c1.getNormalizedCycleTime(c1.landTime);

			sampleLegCycle(legInfos[mi->first],c1, sbMotion, skelCopy, nSample);			
		}
	}
}

double LocomotionAnalyzer::getKeyTagTime( const std::string& key, int iCycle, KeyTag& tag )
{
	double value = -1.0;
	if (tag.find(key) != tag.end())
	{
		std::vector<double>& valueList = tag[key];
		if (iCycle >=0 && iCycle < (int)valueList.size())
		{
			value = valueList[iCycle];
		}
	}
	return value;
}

void LocomotionAnalyzer::sampleLegCycle(LegInfo* legInfo, LocomotionLegCycle& legCycle, SmartBody::SBMotion* motion, SmartBody::SBSkeleton* skel, int nSample )
{
	legCycle.samples.resize(nSample);
	
	SmartBody::SBJoint* baseJoint = skel->getJointByName(legInfo->base);
	motion->connect(skel);
	double dt = legCycle.cycleDuration/(nSample-1);	
	for (int i=0;i<nSample;i++)
	{
		double motionTime = legCycle.stanceTime + dt*i;
		if (motionTime > motion->duration()) motionTime -= motion->duration();
		motion->apply((float)motionTime);		
		skel->update_global_matrices();
		LegCycleSample& legSample = legCycle.samples[i];
		legSample.supportPos.resize(legInfo->supportJoints.size());
		for (unsigned int sup = 0; sup < legInfo->supportJoints.size(); sup++)
		{
			SmartBody::SBJoint* supJoint = skel->getJointByName(legInfo->supportJoints[sup]);		
			legSample.supportPos[sup] = supJoint->gmat().get_translation()*baseJoint->gmat().inverse();
		}		
	}		
	double nextStanceTime = legCycle.cycleEndTime;
	motion->apply((float)nextStanceTime);
	skel->update_global_matrices();
	legCycle.stanceSupportPos.resize(legInfo->supportJoints.size());
	for (unsigned int sup = 0; sup < legInfo->supportJoints.size(); sup++)
	{
		SmartBody::SBJoint* supJoint = skel->getJointByName(legInfo->supportJoints[sup]);		
		legCycle.stanceSupportPos[sup] = supJoint->gmat().get_translation()*baseJoint->gmat().inverse();
	}		
	motion->disconnect();
}

std::string LocomotionAnalyzer::getMotionName()
{
	return motionName;
}

/************************************************************************/
/* Motion Analysis                                                      */
/************************************************************************/

MotionAnalysis::MotionAnalysis(void)
{
}

MotionAnalysis::~MotionAnalysis(void)
{
	for (size_t i = 0; i < legInfos.size(); ++i)
	{
		if (legInfos[i])
		{
			delete legInfos[i];
			legInfos[i] = NULL;
		}
	}
	legInfos.clear();

	for (size_t i = 0; i < locoAnalyzers.size(); ++i)
	{
		if (locoAnalyzers[i])
		{
			delete locoAnalyzers[i];
			locoAnalyzers[i] = NULL;
		}
	}
	locoAnalyzers.clear();

	if (skelCopy)
	{
		delete skelCopy;
		skelCopy = NULL;
	}
}

void MotionAnalysis::init(std::string skeletonName, std::string baseJoint, SmartBody::SBAnimationBlend* locomotionBlend, const std::vector<std::string>& motions, std::string motionPrefix )
{		
	SmartBody::SBScene* scene = SmartBody::SBScene::getScene();
	SmartBody::SBSkeleton* sbSkel = scene->getSkeleton(skeletonName);
	skelCopy = new SmartBody::SBSkeleton(sbSkel);
	skelCopy->ref();
	initLegInfos();
	int numMotions = motions.size();	
	for (int i=0;i<numMotions;i++)
	{
		const std::string& motionName = motions[i];
		KeyTagMap& keyTagMap = *locomotionBlend->getKeyTagMap(motionPrefix+motionName);
		LocomotionAnalyzer* analyzer = new LocomotionAnalyzer();
		analyzer->legInfos = legInfos;
		analyzer->initLegCycles(motionName,locomotionBlend,keyTagMap,skelCopy);
		locoAnalyzers.push_back(analyzer);
	}	

	for (int i=0;i<2;i++)
	{
		legStates[i].prevMotionCycle.resize(numMotions);
	}
}

void MotionAnalysis::initLegInfos()
{
	const std::string lFootName[] = {"l_forefoot", "l_ankle" };
	const std::string rFootName[] = {"r_forefoot", "r_ankle" };

	LegInfo* lLeg = new LegInfo();	
	lLeg->supportOffset.resize(2);
	lLeg->supportJoints.resize(2);
	lLeg->supportJoints[1] = lFootName[1];
	lLeg->supportJoints[0] = lFootName[0];
	lLeg->base = "base";
	lLeg->hip = "l_hip";	

	LegInfo* rLeg = new LegInfo();
	rLeg->supportOffset.resize(2);
	rLeg->supportJoints.resize(2);
	rLeg->supportJoints[1] = rFootName[1];
	rLeg->supportJoints[0] = rFootName[0];
	rLeg->base = "base";
	rLeg->hip = "r_hip";

	legInfos.push_back(lLeg);
	legInfos.push_back(rLeg);
	legStates.resize(2);

	for (int i=0;i<2;i++)
	{
		EffectorConstantConstraint* lFoot = new EffectorConstantConstraint();
		lFoot->efffectorName = lFootName[i];
		lFoot->rootName = "";
		lLeg->ikConstraint[lFoot->efffectorName] = lFoot;

		EffectorConstantConstraint* rFoot = new EffectorConstantConstraint();
		rFoot->efffectorName = rFootName[i];
		rFoot->rootName = "";
		rLeg->ikConstraint[rFoot->efffectorName] = rFoot;

		legStates[i].curSupportPos.resize(2);
		legStates[i].globalSupportPos.resize(2);
		legStates[i].stanceSupportPos.resize(2);		
	}	
	//ikScenario.buildIKTreeFromJointRoot(skelCopy->getJointByMappedName("base"),stopJoint);
}

#ifdef EMBER_SB_HEIGHTFIELD
float MotionAnalysis::getTerrainYOffset( LegCycleState& state, float flightTime )
{
	Heightfield* heightField = SmartBody::SBScene::getScene()->getHeightfield();	
	if (!heightField) return 0.f;
	
	SrVec startPt = state.curStep, endPt = state.nextStep;
	SrVec startN, endN;
	SrVec up = SrVec(0,1,0);
	startPt.y = heightField->get_elevation(startPt.x, startPt.z, (float*)&startN);
	endPt.y = heightField->get_elevation(endPt.x, endPt.z, (float*)&endN);

	SrVec midPt = (startPt+endPt)*0.5f;
	SrVec a = startPt - midPt;
	SrVec b = endPt - midPt;
	float startH = dot(a,startN)/dot(startN,up);
	float endH = dot(b,endN)/dot(endN,up);	
	//return max(startH,endH);
	float hmax = max(startH,endH);
	float flightTimeLift = (float)sin(flightTime*M_PI);
	//return 2.f*hmax/M_PI*flightTimeLift + startPt.y*(1.f-flightTime) + endPt.y*flightTime;
	return  startPt.y*(1.f-flightTime) + endPt.y*flightTime;
}
#endif

#ifdef EMBER_SB_STEER
void MotionAnalysis::applyIKFix(MeCtIKTreeScenario& ikScenario, SmartBody::SBCharacter* sbChar, std::vector<double>& weights, PATimeManager* timeManager, SrMat worldOffsetMat, SrVec velocity, float angSpeed, BodyMotionFrame& inputFrame, BodyMotionFrame& outFrame )
{
	static SrQuat prevQuat;
	SmartBody::SBSkeleton* charSk = sbChar->getSkeleton();
	ikScenario.ikGlobalMat = worldOffsetMat;
	ikScenario.ikTreeRootPos = inputFrame.rootPos;
	ikScenario.setTreeNodeQuat(inputFrame.jointQuat, QUAT_INIT);
	ikScenario.copyTreeNodeQuat(QUAT_INIT,QUAT_CUR);
	std::string baseName = "base";	
	MeCtIKTreeNode* rootNode = ikScenario.findIKTreeNode(baseName.c_str());

	SmartBody::SBScene* scene = SmartBody::SBScene::getScene();
	SmartBody::SBAssetManager* assetManager = scene->getAssetManager();
	SmartBody::SBSkeleton* srcSk = assetManager->getSkeleton(skelCopy->getName());
	SmartBody::SBSkeleton* tgtSk = assetManager->getSkeleton(charSk->getName());
	SmartBody::SBJoint* srcBase = srcSk->getJointByName(baseName);
	SmartBody::SBJoint* tgtBase = tgtSk->getJointByName(baseName);

	float tgtBaseHeight = tgtSk->getBaseHeight(baseName); // to decide the terrain compensation offset
	float scaleRatio = tgtSk->getBaseHeight(baseName)/srcSk->getBaseHeight(baseName);

	for (unsigned int i=0;i<legInfos.size();i++)
	{
		LegInfo* info = legInfos[i];
		for (unsigned int k=0;k<info->supportJoints.size();k++)
		{
			MeCtIKTreeNode* supNode = ikScenario.findIKTreeNode(info->supportJoints[k].c_str());
			supNode->active = false;
			SmartBody::SBJoint* srcSupJoint = srcSk->getJointByName(info->supportJoints[k]);
			SmartBody::SBJoint* tgtSupJoint = tgtSk->getJointByName(info->supportJoints[k]);
			SrVec srcOffset = (srcSupJoint->getPosition() - srcBase->getPosition())*scaleRatio;
			SrVec tgtOffset = tgtSupJoint->getPosition() - tgtBase->getPosition();
			info->supportOffset[k] = tgtOffset - srcOffset;
		}
	}	

	ikScenario.updateNodeGlobalMat(rootNode, QUAT_INIT);
	SrQuat prerot = rootNode->joint->quat()->prerot();
	SrMat prerotMat; prerot.get_mat(prerotMat);
	SrMat gmatBase = prerotMat.inverse()*rootNode->gmat;
	for (unsigned int k=0;k<legStates.size();k++)
	{
		std::vector<SrVec>& supPos = legStates[k].curSupportPos;
		std::vector<SrVec>& stancePos = legStates[k].stanceSupportPos;
		std::fill(supPos.begin(),supPos.end(),SrVec()); // clear the support position	
		std::fill(stancePos.begin(),stancePos.end(),SrVec()); // clear the support position	
		legStates[k].cycleTime = 0.f;
		legStates[k].timeToNextCycle = 0.f;
		legStates[k].newCycle = false;
		
	}
	float transitionWeight = 0.0;
	int maxWeightIndex = -1;
	double maxBlendWeight = -1;	
	for (unsigned int i=0;i<weights.size();i++)
	{
		if (weights[i] > maxBlendWeight)
		{
			maxBlendWeight = weights[i];
			maxWeightIndex = i;
		}
	}	
	// get normalize cycle from maxWeight locomotion
	LocomotionAnalyzer* maxWeightAnalyzer = locoAnalyzers[maxWeightIndex];
	for (unsigned int k=0;k<legStates.size();k++)
	{
		LegCycleState& legState = legStates[k];		
		double maxMoTime = timeManager->motionTimes[maxWeightIndex];
		LocomotionLegCycle* maxCycle = maxWeightAnalyzer->getLegCycle(k,(float)maxMoTime);		
		float normalizeCycle = maxCycle->getNormalizedCycleTime((float)maxMoTime);	
		legState.flightTime = maxCycle->getNormalizedFlightTime((float)maxMoTime);
		sbChar->flightTime[k] = legState.flightTime;
		for (unsigned int i=0;i<locoAnalyzers.size();i++) // weighted sum of local foot base
		{
			LocomotionAnalyzer* analyzer = locoAnalyzers[i];		
			double moTime = timeManager->motionTimes[i];
			LocomotionLegCycle* legCycle = analyzer->getLegCycle(k,(float)moTime);		
			//LocomotionLegCycle* legCycle = analyzer->getLegCycleByIndex(k,maxCycle->cycleIdx);		
			legState.cycleTime += normalizeCycle*legCycle->cycleDuration*(float)weights[i];
			legState.timeToNextCycle += (1.f-normalizeCycle)*legCycle->cycleDuration*(float)weights[i];
 			if ( i == maxWeightIndex ) //( weights[i] > 0.f ) // only test the motion with maximum weight
			{			
				if (legState.prevCycle != legCycle->cycleIdx)
				{
					//if (k==1)
					//    LOG("dominant motion %s, weight = %f, time = %f, prevCycle = %d, nextCylce = %d",analyzer->getMotionName().c_str(), weights[i], timeManager->getNormalizeLocalTime(), legState.prevCycle, legCycle->cycleIdx);
					legState.newCycle = true;	
					legState.curStep = legState.nextStep;
				}
				legState.prevCycle = legCycle->cycleIdx;
			}

			//legState.prevMotionCycle[i] = legCycle->cycleIdx;

			for (unsigned int m=0;m<legState.curSupportPos.size();m++)
			{
				// get weighted sum 
				legState.curSupportPos[m] += legCycle->getSupportPos((float)moTime,m)*(float)weights[i];	
				//legState.curSupportPos[m] += legCycle->getSupportPosNormalize(normalizeCycle,m)*(float)weights[i];	
				legState.stanceSupportPos[m] += legCycle->getStanceSupportPos(m)*(float)weights[i];
			}			
		}
// 		if (weights[i] > maxBlendWeight)
// 		{
// 			maxWeightIndex = i;
// 			maxBlendWeight = weights[i];
// 		}
	}


	#ifdef EMBER_SB_HEIGHTFIELD
	Heightfield* heightField = SmartBody::SBScene::getScene()->getHeightfield();	
	#endif
	SrVec vUp = SrVec(0,1,0);
	SrVec rotCenter = cross(vUp,velocity)/(angSpeed*(float)M_PI/180.f); // rotation center in character local frame
	for (unsigned int k=0;k<legStates.size();k++)
	{
		LegCycleState& legState = legStates[k];
		float nextStepAngle = angSpeed*legState.timeToNextCycle*(float)M_PI/180.f;
		SrQuat nextStepRotation = SrQuat(vUp,nextStepAngle)*SrQuat(gmatBase.get_rotation());
		SrMat nextStepRotMat; nextStepRotation.get_mat(nextStepRotMat);
		SrVec nextStepCharPos =  velocity*legState.timeToNextCycle;
		if (angSpeed > 20.0)
			nextStepCharPos = rotCenter - rotCenter*SrQuat(vUp,nextStepAngle); //
		//SrVec nextStepCharPos =  rotCenter - rotCenter*SrQuat(vUp,nextStepAngle); //
		SrVec nextStepFootPos = gmatBase.get_translation() + nextStepCharPos + (legState.stanceSupportPos[0]*scaleRatio + legInfos[k]->supportOffset[0])*nextStepRotMat;
		//SrVec nextStepFootPos = gmatBase.get_translation() + nextStepCharPos + (legState.stanceSupportPos[0]*scaleRatio + legInfos[k]->supportOffset[0])*gmatBase.get_rotation(); //
		//SrVec nextStepFootPos = gmatBase.get_translation() + nextStepCharPos + (legState.stanceSupportPos[0]*scaleRatio + legInfos[k]->supportOffset[0])*gmatBase.get_rotation(); //
		// 		
// 		if (k==0 && legState.timeToNextCycle < 0.1f)
// 		{
// 			SrVec sf = legState.stanceSupportPos[0];
// 			SrVec cf = legState.curSupportPos[0];			
// 			LOG("stance Foot = %f %f %f, curFoot = %f %f %f",sf.x,sf.y,sf.z, cf.x, cf.y, cf.z);
// 		}
		//if (k==0) LOG("time to next cycle = %f", legState.timeToNextCycle);
		//SrVec nextStepFootPos = (legState.curSupportPos[0]*scaleRatio + legInfos[k]->supportOffset[0])*gmatBase;
		legState.nextStep = nextStepFootPos;
		// // set to ground height by default
		#ifdef EMBER_SB_HEIGHTFIELD
		if (heightField && sbChar->getBoolAttribute("terrainWalk"))
		{
			float tnormal[3];
			nextStepFootPos.y = SmartBody::SBScene::getScene()->queryTerrain(nextStepFootPos.x, nextStepFootPos.z, tnormal);
		}
		else
		#endif
			nextStepFootPos.y = 0.f;

		sbChar->addFootStep(k,nextStepFootPos, !legState.newCycle);		
	}
	
	


	LocomotionAnalyzer* dominantAnalyzer = locoAnalyzers[maxWeightIndex];
	double dominantMotime = timeManager->motionTimes[maxWeightIndex];

	for (unsigned int k=0;k<legStates.size();k++)
	{
		LegCycleState& legState = legStates[k];
		for (unsigned int m=0;m<legState.curSupportPos.size();m++)
		{
			legState.globalSupportPos[m] = (legState.curSupportPos[m]*scaleRatio + legInfos[k]->supportOffset[m])*gmatBase; // transform to global space	
			if (k==0 && m==0)
			{
				SrVec spos;
				spos = legState.curSupportPos[m];
				//LOG("leg support pos = %f %f %f",spos.x, spos.y, spos.z);
			}
		}
// 		if (legState.stanceSupportPos.size() != legState.curSupportPos.size()) // if stance pos is not initialized yet
// 			legState.stanceSupportPos = legState.curSupportPos;
// 		int phase = 3;		
// 		LocomotionLegCycle* legCycle = dominantAnalyzer->getLegCycle(k,(float)dominantMotime);
// 		transitionWeight = legCycle->getTransitionWeight((float)dominantMotime,phase);
// 		if (phase == 0)
// 		{
// 			legState.stanceSupportPos = legState.curSupportPos;
// 		}
// 		// blend in/out of stance leg position
// 		for (unsigned int m=0;m<legState.curSupportPos.size();m++)
// 		{
// 			legState.curSupportPos[m] = legState.curSupportPos[m]*transitionWeight + legState.stanceSupportPos[m]*(1.f-transitionWeight); 
// 		}
	}

	// get the height offset 
 // use height field

	SmartBody::SBNavigationMesh* navMesh = SmartBody::SBScene::getScene()->getNavigationMesh();	
	#ifdef EMBER_SB_HEIGHTFIELD
	heightField = SmartBody::SBScene::getScene()->getHeightfield();	
	if (heightField && !navMesh)
	{
		float tnormal[3];
		float terrainHeight = 0.0f;
		float footOffset = 0.05f;	
		float minHeight = 1e30f;
		float maxHeight = -1e30f;	
		if (heightField && sbChar->getBoolAttribute("terrainWalk"))
		{
			//terrainHeight = SmartBody::SBScene::getScene()->queryTerrain(x, z, tnormal);
			for (unsigned int k=0;k<legStates.size();k++)
			{
				LegCycleState& legState = legStates[k];
#define NAIVE_IKFIX 1
#if NAIVE_IKFIX
				for (unsigned int m=0;m<legState.curSupportPos.size();m++)
				{
					SrVec supPos = legState.globalSupportPos[m];
					float height = SmartBody::SBScene::getScene()->queryTerrain(supPos.x, supPos.z, tnormal);
					float supHeightOffset = (height + tgtBaseHeight) - gmatBase.get_translation().y; 
					legState.globalSupportPos[m].y += supHeightOffset + footOffset;
					if (minHeight > height)
						minHeight = height;
					if (maxHeight < height)
						maxHeight = height;
				}
#else			
				float hoffset = getTerrainYOffset(legState, legState.flightTime);
				float supHeightOffset = (hoffset + tgtBaseHeight) - gmatBase.get_translation().y; 
				for (unsigned int m=0;m<legState.curSupportPos.size();m++)
				{				
					SrVec supPos = legState.globalSupportPos[m];
					float height = SmartBody::SBScene::getScene()->queryTerrain(supPos.x, supPos.z, tnormal);
					if (minHeight > height)
						minHeight = height;
					if (maxHeight < height)
						maxHeight = height;

					legState.globalSupportPos[m].y += supHeightOffset + footOffset;

				}
				sbChar->curFootIKPos[k] = legState.globalSupportPos[0];
#endif
			}
			float heightOffset = (minHeight + tgtBaseHeight) - gmatBase.get_translation().y; 
			ikScenario.ikGlobalMat.set(13, ikScenario.ikGlobalMat.get(13) + heightOffset + footOffset);
		}
	}

	else if (navMesh && !heightField)
	#else
	if (navMesh)
	#endif
	{
		// use navigation mesh
		//float tnormal[3];
		float terrainHeight = 0.0f;
		float footOffset = 0.05f;
		float minHeight = 1e30f;
		float maxHeight = -1e30f;
		SrVec searchSize = SrVec(sbChar->getHeight()*0.3f, sbChar->getHeight(), sbChar->getHeight()*0.3f);	
		if (navMesh && sbChar->getBoolAttribute("terrainWalk"))
		{
			//terrainHeight = SmartBody::SBScene::getScene()->queryTerrain(x, z, tnormal);
			for (unsigned int k=0;k<legStates.size();k++)
			{
				LegCycleState& legState = legStates[k];
				for (unsigned int m=0;m<legState.curSupportPos.size();m++)
				{
					SrVec supPos = legState.globalSupportPos[m];
					//float height = SmartBody::SBScene::getScene()->queryTerrain(supPos.x, supPos.z, tnormal);
					float height = navMesh->queryFloorHeight(supPos,searchSize);
					float supHeightOffset = (height + tgtBaseHeight) - gmatBase.get_translation().y; 
					legState.globalSupportPos[m].y += supHeightOffset + footOffset;
					if (minHeight > height)
						minHeight = height;
					if (maxHeight < height)
						maxHeight = height;
				}
			}
			float heightOffset = (minHeight + tgtBaseHeight) - gmatBase.get_translation().y; 
			ikScenario.ikGlobalMat.set(13, ikScenario.ikGlobalMat.get(13) + heightOffset + footOffset);
		}
	}	
	// set ik constraint
	for (unsigned int i=0;i<legInfos.size();i++)
	{
		LegInfo* leg = legInfos[i];
		LegCycleState& legState = legStates[i];
		ConstraintMap& constraint = leg->ikConstraint;
		ConstraintMap::iterator mi;
		for (unsigned int k=0;k<leg->supportJoints.size();k++)
		{			
			EffectorConstantConstraint* foot = dynamic_cast<EffectorConstantConstraint*>(constraint[leg->supportJoints[k]]);
			foot->targetPos = legState.globalSupportPos[k];
		}		
		ikScenario.ikPosEffectors = &constraint;
		ikCCD.update(&ikScenario);		
	}
// 	SrQuat curQuat = ikScenario.findIKTreeNode("r_knee")->getQuat();
// 	SrQuat deltaQ = curQuat*prevQuat.inverse();
// 	if (fabs(deltaQ.angle()) > 0.05)
// 	{
// 		LOG("sudden change in joint angle !");
// 	}
// 	prevQuat = curQuat;
	outFrame = inputFrame;
	// write the result to output frame
	ikScenario.getTreeNodeQuat(outFrame.jointQuat,QUAT_CUR); 		
}

#endif