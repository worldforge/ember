#ifndef BEHAVIORS_H
#define BEHAVIORS_H

	#include <string>
	#include <vector>
	#include <sb/SmartBody.h>


class Behaviors
{

public:

	Behaviors(std::string const &motionPath = "", std::string const &skeletonRef = "");
	~Behaviors(void);

	void setup(void);
	void retarget(std::string characterName);

protected:

	std::string mSkelRefName;
	std::string mMotionPath;
	SmartBody::SBAnimationBlendManager *mBlendManager;
	bool mLoad;

	void setupBehaviors(void);
	virtual void setupMotion(SmartBody::SBMotion *motion) = 0;
	virtual void setupRetargeting(SmartBody::SBCharacter *character) = 0;

	std::string getMotionAsset(std::string motionName);
	static std::string createDefaultMotionPath(std::string const &path);

	virtual std::vector<std::string> getMotions(void) = 0;

	virtual void assignDefaultMotionPath(void) = 0;
	virtual void assignSkeletonReference(void) = 0;

	bool checkAssetExistence(void);

};

#endif