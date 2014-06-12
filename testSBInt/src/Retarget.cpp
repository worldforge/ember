#include "Retarget.h"

Retarget::Retarget(std::string const &srcSkName, std::string const &tgtSkName)
	: mSrcName(srcSkName), mTgtName(tgtSkName), mRetarget(0);
{
	mManager = App::getSbManagerSingleton()->getScene()->getRetargetManager();
	mRetarget = mManager->getRetarget(srcSkName, tgtSkName);

	/* If mRetarget is NULL, then the instance has never been set and we must create it. */
	if (!mRetarget)
	{
		createInstance();
	}
}

Retarget::~Retarget(void)
{

}


/* Create an instance of SBRetarget. */
void Retarget::createInstance(void)
{
	/* Set of the end joints (?). */
	std::vector<std::string> endJoints;
	endJoints.push_back("l_ankle");
	endJoints.push_back("l_forefoot");
	endJoints.push_back("l_toe");
	endJoints.push_back("l_wrist");
	endJoints.push_back("r_ankle");
	endJoints.push_back("r_forefoot");
	endJoints.push_back("r_toe");
	endJoints.push_back("r_wrist");

	/* Set of the relative joints (?). */
	std::vector<std::string> relJoints;
	relJoints.push_back("spine1");
	relJoints.push_back("spine2");
	relJoints.push_back("spine3");
	relJoints.push_back("spine4");
	relJoints.push_back("spine5");
	relJoints.push_back("r_sternoclavicular");
	relJoints.push_back("l_sternoclavicular");
	relJoints.push_back("r_acromioclavicular");
	relJoints.push_back("l_acromioclavicular");
	
	/* Creation of the retarget instance. */
	mRetarget = mManager->createRetarget(mSrcName, mTgtName);
	mRetarget.initRetarget(endJoints, relJoints);
}


