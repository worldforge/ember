#ifndef BEHAVIORS_H
#define BEHAVIORS_H

	#include <string>
	#include <vector>

typedef enum {

	BEHAVIOR_LOCOMOTION
	
} BehaviorType;

class Behaviors
{
public:
	Behaviors::Behaviors(BehaviorType behaviors, std::string motionPath = "", std::string skeletonRef = "");
	~Behaviors(void);

	void setDefaultMotionPath(void);

private:
	BehaviorType mType;
	std::string mSkelRefName;
	std::string mMotionPath;

	void setupBehaviors(void);
	std::vector<std::string> getMotions(void);
	std::vector<std::string> getLocomotionMotions(void);
	std::string getMotionAsset(std::string motionName);

	void assignDefaultMotionPath(void);
	void assignSkeletonReference(void);


};

#endif