#pragma once
#include "controllers/me_ct_motion_example.hpp"
#include <sbm/sr_linear_curve.h>
#include <sbm/sr_spline_curve.h>

class ProfileCurve
{
public:
	float startTime, endTime;	
	ProfileCurve() {}
	virtual ~ProfileCurve() {}
public:
	virtual void init() = 0;
	virtual void addPt(float t, float val) = 0;
	virtual float evaluate(float t) = 0;
	virtual float start() = 0;
	virtual float end() = 0;
};

class LinearProfileCurve : public ProfileCurve
{
protected:
	srLinearCurve curve;	
public:
	LinearProfileCurve() {}
	virtual ~LinearProfileCurve() {}
public:
	virtual void init();
	virtual void addPt(float t, float val);
	virtual float evaluate(float t);	
	virtual float start();
	virtual float end();
};

typedef std::map<std::string,ProfileCurve*> ProfileCurveMap;

class MotionProfile
{
public:
	enum { PROFILE_VELOCITY = 0, NUM_PROFILE_TYPES };
public:
	SkMotion* motion;	
	ProfileCurveMap eulerProfile[3]; // euler angles X,Y,Z
	ProfileCurveMap velocityProfile; 
	ProfileCurveMap interpolationProfile;	
	ProfileCurve*   avgVelProfile;
public:
	MotionProfile(SkMotion* m);
	virtual ~MotionProfile(void);	
	void buildEulerCurveProfile(float startTime, float endTime, float timeStep);
	void buildVelocityProfile(float startTime, float endTime, float timeStep);
	void buildInterpolationProfile(float startTime, float endTime, float timeStep);
protected:
	void computeVelocity(SkMotion* m, float t, float dt, std::vector<float>& prevFrame, std::vector<float>& curFrame, ProfileCurveMap& outProfile);
	void computeEulerVelocity(SkMotion* m, float t, float dt, std::vector<float>& prevFrame, std::vector<float>& curFrame, ProfileCurveMap outProfile[3]);
	void createNormalizeInterpolationCurve(float startTime, float endTime, float timeStep, ProfileCurve* velCurve, ProfileCurve* interpCurve);
	static void cleanUpProfileMap(ProfileCurveMap& curveMap);
	ProfileCurve* createProfileCurve();
};
