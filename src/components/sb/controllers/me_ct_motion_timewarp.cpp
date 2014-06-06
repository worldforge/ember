#include <assert.h>
#include <sstream>
#include <algorithm>
#include <time.h>
#include <boost/foreach.hpp>
#include "me_ct_motion_timewarp.hpp"

using namespace boost;


SimpleTimeWarp::SimpleTimeWarp( double refLen, double targetLen )
{
	refDuration = refLen;
	targetDuration = targetLen;
	slope = targetDuration/refDuration;
	invSlope = refDuration/targetDuration;
}

double SimpleTimeWarp::timeWarp( double u )
{

	if (u > refDuration)
		u = refDuration - gwiz::epsilon10();
// 	u = floatMod(u,refDuration*2.0);
// 	if (u > refDuration)
// 		u = refDuration*2.0 - u;

	if (u <= 0.0)
		u = 0.0;

	double mu = u;//floatMod(u,refDuration);
	return mu*slope;
}

double SimpleTimeWarp::invTimeWarp( double t )
{
	if (t > targetDuration)
		t = targetDuration - gwiz::epsilon10();
	if (t < 0.0)
		t = 0.0;
// 	t = floatMod(t,targetDuration*2.0);
// 	if (t > targetDuration)
// 		t = targetDuration*2.0 - t;

	double mt = t;//floatMod(t,targetDuration);
	return mt*invSlope;
}

double SimpleTimeWarp::floatMod( double a, double b )
{
	int result = static_cast<int>( a / b );
	return a - static_cast<double>( result ) * b;
}

/************************************************************************/
/* Multi-Linear Time Warp                                               */
/************************************************************************/
MultiLinearTimeWarp::MultiLinearTimeWarp( std::vector<double>& inRefKeys, std::vector<double>& inTargetKeys )
{
	refKeyTimes = inRefKeys;
	targetKeyTimes = inTargetKeys;	
}

double MultiLinearTimeWarp::refTimeLength()
{
	if (refKeyTimes.size() >= 2)
	{
		return refKeyTimes[refKeyTimes.size()-1] - refKeyTimes[0];
	}
	else
	{
		return 0.0;
	}
}


double MultiLinearTimeWarp::timeWarp( double u )
{
	if (u > refTimeLength())
		u = refTimeLength() - gwiz::epsilon10();
	if (u <= 0.0)
		u = 0.0;
	int section = getSection(refKeyTimes,u);
	//LOG("section = %d, u = %f, time slope = %f, refKeyTime = %f, targetKeyTime = %f", section, u, timeSlope(u), refKeyTimes[section], targetKeyTimes[section]);
	return targetKeyTimes[section] + (u-refKeyTimes[section])*timeSlope(u);
}

double MultiLinearTimeWarp::invTimeWarp( double t )
{
	if (t > actualTimeLength())
		t = actualTimeLength() - gwiz::epsilon10();
	if (t <= 0.0)
		t = 0.0;
	int section = getSection(targetKeyTimes,t);
	return refKeyTimes[section] + (t-targetKeyTimes[section])*invTimeSlope(t);
}

double MultiLinearTimeWarp::actualTimeLength()
{
	if (targetKeyTimes.size() >= 2)
	{
		return targetKeyTimes[refKeyTimes.size()-1] - targetKeyTimes[0];
	}
	else
	{
		return 0.0;
	}
}

double MultiLinearTimeWarp::timeSlope( double u )
{
	int idx = getSection(refKeyTimes, u);
	if (idx < 0) idx = 0;
	if (idx >= (int) refKeyTimes.size()-1)
		idx = refKeyTimes.size()-2; 
	
	return (targetKeyTimes[idx+1]-targetKeyTimes[idx])/(refKeyTimes[idx+1]-refKeyTimes[idx]);
}


double MultiLinearTimeWarp::invTimeSlope( double t )
{
	int idx = getSection(targetKeyTimes, t);
	if (idx < 0) idx = 0;
	if (idx >= (int) targetKeyTimes.size()-1)
		idx = targetKeyTimes.size()-2; 
	return (refKeyTimes[idx+1]-refKeyTimes[idx])/(targetKeyTimes[idx+1]-targetKeyTimes[idx]);
}

int MultiLinearTimeWarp::getSection( std::vector<double>& keys, double time )
{
	for (unsigned int i = 0; i < keys.size() - 1; i++)
	{
		if (keys[i] <= time && keys[i + 1] >= time)
			return i;
	}
	if (time > keys[keys.size()-1])
		return keys.size();
	return -1;
}
