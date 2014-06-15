#ifndef RETARGET_H
#define RETARGET_H

	#include <string>
	#include <sb/SmartBody.h>

class Retarget
{
public:
	Retarget(std::string const &srcSkName, std::string const &tgtSkName);
	~Retarget(void);

private:
	SmartBody::SBRetargetManager *mManager;
	SmartBody::SBRetarget *mRetarget;
	std::string mSrcName;
	std::string mTgtName;

	void createInstance(void);
};

#endif