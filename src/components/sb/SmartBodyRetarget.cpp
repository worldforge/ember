#include "SmartBodyRetarget.h"
#include "SmartBodyManager.h"

using namespace Ember;
namespace Ember
{

Retarget::Retarget(const std::string& srcSkName, const std::string& tgtSkName, SmartBody::RetargetManager *manager)
	: mSrcName(srcSkName), mTgtName(tgtSkName), mRetarget(0)
{
	SmartBody::Retarget *retarget = manager->getRetarget(srcSkName, tgtSkName);

	//If retarget is NULL, then the instance has never been set and we must create it.
	if (!retarget)
	{
		createInstance(srcSkName, tgtSkName, retargetManager);
	}
}

Retarget::~Retarget(void)
{

}


void Retarget::createInstance(const std::string& srcSk, const std::string& tgtSk, SmartBody::RetargetManager *manager)
{
	//Sets the end joints.
	std::vector<std::string> endJoints;
	endJoints.push_back("l_ankle");
	endJoints.push_back("l_forefoot");
	endJoints.push_back("l_toe");
	endJoints.push_back("l_wrist");
	endJoints.push_back("r_ankle");
	endJoints.push_back("r_forefoot");
	endJoints.push_back("r_toe");
	endJoints.push_back("r_wrist");

	//Sets the relative joints.
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
	
	//Creation of the retarget instance.
	SmartBody::Retarget *retarget = manager->createRetarget(srcSk, tgtSk);
	retarget->initRetarget(endJoints, relJoints);
}

}


