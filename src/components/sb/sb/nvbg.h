#ifndef _NVBG_H
#define _NVBG_H

#include <sb/SBObject.h>
#include <string>

namespace SmartBody 
{

class Nvbg : public SmartBody::SBObject
{
public:
		Nvbg();
		virtual ~Nvbg();

		virtual void objectEvent(std::string character, std::string name, bool isAnimate, SrVec charPosition, SrVec charVelocity, SrVec objPosition, SrVec objVelocity, SrVec relativePosition, SrVec relativeVelocity);
		virtual bool execute(std::string character, std::string to, std::string messageId, std::string xml);
		virtual bool executeEvent(std::string character, std::string messageId, std::string state);
		virtual bool executeSpeech(std::string character, std::string speechStatus, std::string speechId, std::string speaker);
		virtual bool executeSpeechRequest(std::vector<std::string> behaviors, std::vector<std::string> types, std::vector<float> times, std::vector<std::string> targets, std::vector<std::string> info);
		virtual void notify(SmartBody::SBSubject* subject);
		virtual void notifyLocal(std::string name);

		virtual void notifyAction(std::string name);
		virtual void notifyBool(std::string name, bool val);
		virtual void notifyInt(std::string name, int val);
		virtual void notifyDouble(std::string name, double val);
		virtual void notifyString(std::string name, std::string val);
		virtual void notifyVec3(std::string name, SrVec val);
		virtual void notifyMatrix(std::string name, SrMat val);

};

}

#endif