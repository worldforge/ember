#include "controllers/me_ct_motion_profile.h"

/************************************************************************/
/* Piece-wise Linear Curve                                              */
/************************************************************************/
void LinearProfileCurve::init()
{
	curve.clear();
}

void LinearProfileCurve::addPt( float t, float val )
{
	curve.insert(t,val);
}

float LinearProfileCurve::evaluate( float t )
{
	return (float)curve.evaluate(t);
}

float LinearProfileCurve::start()
{
	return (float)curve.get_head_value();
}

float LinearProfileCurve::end()
{
	return (float)curve.get_tail_value();
}
/************************************************************************/
/* Motion Profile                                                       */
/************************************************************************/

MotionProfile::MotionProfile( SkMotion* m )
{
	motion = m;
}

MotionProfile::~MotionProfile( void )
{

}

void MotionProfile::buildInterpolationProfile( float startTime, float endTime, float timeStep )
{
	cleanUpProfileMap(interpolationProfile);
	ProfileCurveMap::iterator mi;	
	for (mi  = velocityProfile.begin();
		 mi != velocityProfile.end();
		 mi++)
	{
		std::string chanName = mi->first;
		ProfileCurve* velCurve = mi->second;
		interpolationProfile[chanName] = createProfileCurve();
		createNormalizeInterpolationCurve(startTime,endTime,timeStep,velCurve,interpolationProfile[chanName]);		
	}
}

void MotionProfile::buildEulerCurveProfile( float startTime, float endTime, float timeStep )
{
	float motionLength = motion->duration();
	std::vector<float> motionBuffer[2];
	for (int k=0;k<2;k++)
		motionBuffer[k].resize(motion->channels().count_floats()); 
	
	for (int i=0;i<3;i++)
		cleanUpProfileMap(eulerProfile[i]);
	float prevTime = startTime - timeStep;
	if (prevTime < 0.f) prevTime = 0.f;
	motion->apply(prevTime,&motionBuffer[0][0],NULL);
	int curBufferIdx = 1;
	int prevBufferIdx = 0;
	for (float t = startTime; t <= endTime; t+= timeStep)
	{
		// get current motion frame
		motion->apply(t,&motionBuffer[1][0],NULL);
		float normT = (t-startTime)/(endTime-startTime);
		float normTimeStep = timeStep/(endTime-startTime);
		computeEulerVelocity(motion,normT,normTimeStep,motionBuffer[0],motionBuffer[1],eulerProfile);		
	}
}


void MotionProfile::buildVelocityProfile( float startTime, float endTime, float timeStep )
{
	float motionLength = motion->duration();
	std::vector<float> motionBuffer[2];
	for (int k=0;k<2;k++)
		motionBuffer[k].resize(motion->channels().count_floats()); 

	// clean up profile curves
	cleanUpProfileMap(velocityProfile);
	for (int i=0;i<3;i++)
		cleanUpProfileMap(eulerProfile[i]);

	float prevTime = startTime - timeStep;
	if (prevTime < 0.f) prevTime = 0.f;
	motion->apply(prevTime,&motionBuffer[0][0],NULL);
	int curBufferIdx = 1;
	int prevBufferIdx = 0;
	float maxAvgVel = 0.f;
	float minAvgVel = 100000.f;
	std::string velAvgName = "Average";
	for (float t = startTime; t <= endTime; t+= timeStep)
	{
		// get current motion frame
		motion->apply(t,&motionBuffer[curBufferIdx][0],NULL);		
		computeVelocity(motion,t,timeStep,motionBuffer[prevBufferIdx],motionBuffer[curBufferIdx],velocityProfile);
		ProfileCurve* curve = velocityProfile[velAvgName];
		float avgVel = curve->evaluate(t);
		if (avgVel > maxAvgVel)
			maxAvgVel = avgVel;
		if (avgVel < minAvgVel)
			minAvgVel = avgVel;
		//computeEulerVelocity(motion,t,timeStep,motionBuffer[prevBufferIdx],motionBuffer[curBufferIdx],eulerProfile);
		curBufferIdx = 1 - curBufferIdx;
		prevBufferIdx = 1 - prevBufferIdx;
	}
	ProfileCurve* curve = velocityProfile[velAvgName];
	avgVelProfile = createProfileCurve();
	float avgDiv = 1.f/(maxAvgVel-minAvgVel);
	for (float t = startTime; t <= endTime; t+= timeStep)
	{
		float normT = (t-startTime)/(endTime-startTime);
		avgVelProfile->addPt(normT,(curve->evaluate(t)-minAvgVel)*avgDiv);
	}	
}

void MotionProfile::createNormalizeInterpolationCurve( float startTime, float endTime, float timeStep, ProfileCurve* velCurve, ProfileCurve* interpCurve )
{
	float integration = 0.0f;
	float normStep = timeStep/(endTime-startTime);
	for (float t = startTime; t <= endTime; t+= timeStep)
	{		
		float r = velCurve->evaluate(t);
		integration += r*normStep;
	}

	float intSum = 0.f;
	if (integration == 0.f) // just produce a linear curve
	{
		for (float t = startTime; t <= endTime; t+= timeStep)
		{	
			float normT = (t-startTime)/(endTime-startTime);
			intSum += normT;
			interpCurve->addPt(normT,intSum);
		}
		return;
	}
	float intDiv = 1.f/integration;
	
	for (float t = startTime; t <= endTime; t+= timeStep)
	{	
		float normT = (t-startTime)/(endTime-startTime);
		intSum += velCurve->evaluate(t)*normStep*intDiv;
		interpCurve->addPt(normT,intSum);
	}
}

void MotionProfile::computeVelocity( SkMotion* m, float t, float dt, std::vector<float>& prevFrame, std::vector<float>& curFrame, ProfileCurveMap& outProfile )
{
	SkChannelArray& channels = m->channels();
	float aveargeVelocity = 0.f;
	for (int i=0;i<channels.size();i++)
	{
		const std::string& chanName = channels.name(i);
		int chanType = channels.type(i);
		if (chanType != SkChannel::Quat)
			continue;
		
		int floatIdx = channels.float_position(i);
		SrQuat pq = SrQuat(&prevFrame[floatIdx]);
		SrQuat cq = SrQuat(&curFrame[floatIdx]);
		
		SrQuat diff = cq*pq.inverse();
		diff.normalize();
		ProfileCurveMap::iterator mi = outProfile.find(chanName);
		
		if (mi == outProfile.end())
			outProfile[chanName] = createProfileCurve();

		ProfileCurve* curve = outProfile[chanName];
		float velAngle = diff.angle();	
		aveargeVelocity += velAngle;
		curve->addPt(t,diff.angle());
	}	
	std::string velAvgChanName = "Average";
	ProfileCurveMap::iterator mi = outProfile.find(velAvgChanName);
	if (mi == outProfile.end())
		outProfile[velAvgChanName]  = createProfileCurve();

	ProfileCurve* curve = outProfile[velAvgChanName];
	curve->addPt(t,aveargeVelocity);
}



void MotionProfile::computeEulerVelocity( SkMotion* m, float t, float dt, std::vector<float>& prevFrame, std::vector<float>& curFrame, ProfileCurveMap outProfile[3] )
{
	SkChannelArray& channels = m->channels();
	for (int i=0;i<channels.size();i++)
	{
		const std::string& chanName = channels.name(i);
		int chanType = channels.type(i);
		if (chanType != SkChannel::Quat)
			continue;

		int floatIdx = channels.float_position(i);
		SrVec pe = SrQuat(&prevFrame[floatIdx]).getEuler();
		SrVec ce = SrQuat(&curFrame[floatIdx]).getEuler();

		for (int k=0;k<3;k++)
		{
			ProfileCurveMap::iterator mi = outProfile[k].find(chanName);
			if (mi == outProfile[k].end())
				outProfile[k][chanName] = createProfileCurve();

			ProfileCurve* curve = outProfile[k][chanName];
			float eulerVel = (ce[k] - pe[k]);	
			curve->addPt(t,eulerVel);
		}			
		//ProfileCurveMap::iterator mi = outProfile.find(chanName);	
	}
}

void MotionProfile::cleanUpProfileMap( ProfileCurveMap& curveMap )
{
	ProfileCurveMap::iterator mi;	
	for (mi  = curveMap.begin();
		 mi != curveMap.end();
		 mi++)
	{
		ProfileCurve* curve = mi->second;
		delete curve;
	}
	curveMap.clear();
}

ProfileCurve* MotionProfile::createProfileCurve()
{
	return new LinearProfileCurve();
}