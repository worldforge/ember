#ifndef LOCOMOTIONBEHAV_H
#define LOCOMOTIONBEHAV_H

	#include <string>
	#include <vector>
	#include "Behaviors.h"


class LocomotionBehav : public Behaviors
{

public:

	LocomotionBehav(std::string const &motionPath = "", std::string const &skeletonRef = "");
	~LocomotionBehav(void);

	static void setDefaultMotionPath(std::string const &path);

private:

	void setupMotion(SmartBody::SBMotion *motion);
	void setupRetargeting(SmartBody::SBCharacter *character);

	std::vector<std::string> getMotions(void);
	std::vector<std::string> getMotionsForLocomotionSetup(void);

	void assignDefaultMotionPath(void);
	void assignSkeletonReference(void);

	void locomotionSetup();
	void startingSetup();
	void idleTurnSetup();
	void stepSetup();
	void transitionSetup();
};

#endif