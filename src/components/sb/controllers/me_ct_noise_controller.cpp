#include "controllers/me_ct_noise_controller.h"
#include <sr/sr_euler.h>

#include <sb/SBSkeleton.h>

std::string MeCtNoiseController::CONTROLLER_TYPE = "NoiseController";

MeCtNoiseController::MeCtNoiseController(SbmCharacter* character) : SmartBody::SBController()
{
	_character = character;
	_valid = true;
	_prevTime = 0.0;
	_duration = 0.0;
	perlinScale = 0.02f;
	perlinFrequency = 0.03f;
	perlinDuration = 1.f/perlinFrequency;
}

MeCtNoiseController::~MeCtNoiseController()
{
}

void MeCtNoiseController::setJointNoise( std::vector<std::string>& jointNames, float scale, float frequency )
{
	perlinMap.clear();
	for (unsigned int i=0;i<jointNames.size();i++)
	{
		std::string jname = jointNames[i];
		SmartBody::SBJoint* joint = _character->getSkeleton()->getJointByName(jname);
		if (joint) // joint actually exist 
		{
			perlinMap[jname] = Perlin();
			perlinMap[jname].init(); // reinitialize perlin noise for the joint
		}
	}
	if (scale > 0.f)
		perlinScale = scale;
	if (frequency > 0.f)
	{
		perlinFrequency = frequency;
		perlinDuration = 1.f/frequency;
	}	
}

float MeCtNoiseController::getNormalizeTime( float t, float offset )
{
	int nT = int(t/perlinDuration);
	float normalizeTime = (t - perlinDuration*nT)/perlinDuration;
	return normalizeTime;
}

bool MeCtNoiseController::controller_evaluate(double t, MeFrameData& frame)
{
	
	if (_prevTime == 0)
		_dt = 0.016;
	else
	{
		_dt = t - _prevTime;
		_prevTime = t;
	}	
	float invDt = 1.f/0.016;
	if (_dt > 1e-6)
		invDt = 1.f/(float)_dt;	
#if 1
	if (_valid && _context )
	{
		SrVec normalizeTime;
		for (int k=0;k<3;k++)
		{			
			normalizeTime[k] = getNormalizeTime((float)t, perlinDuration*0.3f*k);
		}
		
		std::map<std::string, Perlin>::iterator mi;
		for ( mi  = perlinMap.begin();
			  mi != perlinMap.end();
			  mi++)
		{
			SrQuat oldQuat, newQuat;
			SrVec oldPos;
			std::string jname = mi->first;
			Perlin& perlinNoise = mi->second;
			getJointChannelValues(jname, frame, oldQuat, oldPos);
			SrVec noiseVec;
			newQuat = oldQuat;
			for (int k=0;k<3;k++)
			{
				noiseVec[k] = perlinNoise.noise1(normalizeTime[k])*perlinScale + newQuat.getData(k+1);
				newQuat.setData(k+1,noiseVec[k]);
			}
			newQuat.normalize();
			setJointChannelQuat(jname, frame, newQuat);
		}
	}	
#endif
	return true;
}

