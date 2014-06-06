#include "nvbg.h"
#include "vhcl.h"
#include <sb/SBAttribute.h>

namespace SmartBody 
{

Nvbg::Nvbg()
{

}

Nvbg::~Nvbg()
{
}

void Nvbg::objectEvent(std::string character, std::string name, bool isAnimate, SrVec charPosition, SrVec charVelocity, SrVec objPosition, SrVec objVelocity, SrVec relativePosition, SrVec relativeVelocity)
{
	LOG("Object event for %s from %s", character.c_str(), name.c_str());
}

bool Nvbg::execute(std::string character, std::string to, std::string messageId, std::string xml)
{
	LOG("Executing NVBG for %s %s %s %s", character.c_str(), to.c_str(), messageId.c_str(), xml.c_str());
	return true;
}

bool Nvbg::executeEvent(std::string character, std::string messageId, std::string state)
{
	LOG("Executing NVBG action %s's status for %s is %s", messageId.c_str(), character.c_str(), state.c_str());
	return true;
}

bool Nvbg::executeSpeech(std::string character, std::string speechStatus, std::string speechId, std::string speaker)
{
	LOG("Executing NVBG vrSpeech");
	return true;
}

bool Nvbg::executeSpeechRequest(std::vector<std::string> behaviors, std::vector<std::string> types, std::vector<float> times, std::vector<std::string> targets, std::vector<std::string> info)
{
	LOG("Executing NVBG listening feedback from agent speech.");
	return true;
}

void Nvbg::notify(SmartBody::SBSubject* subject)
{
	SmartBody::SBAttribute* attribute = dynamic_cast<SmartBody::SBAttribute*>(subject);
	if (attribute)
	{
		SmartBody::ActionAttribute* actionAttribute = dynamic_cast<SmartBody::ActionAttribute*>(attribute);
		if (actionAttribute)
		{
			notifyAction(actionAttribute->getName());
			return;
		}

		SmartBody::BoolAttribute* boolAttribute = dynamic_cast<SmartBody::BoolAttribute*>(attribute);
		if (boolAttribute)
		{
			notifyBool(boolAttribute->getName(), boolAttribute->getValue());
			return;
		}

		SmartBody::IntAttribute* intAttribute = dynamic_cast<SmartBody::IntAttribute*>(attribute);
		if (intAttribute)
		{
			notifyInt(intAttribute->getName(), intAttribute->getValue());
			return;
		}

		SmartBody::DoubleAttribute* doubleAttribute = dynamic_cast<SmartBody::DoubleAttribute*>(attribute);
		if (doubleAttribute)
		{
			notifyDouble(doubleAttribute->getName(), doubleAttribute->getValue());
			return;
		}

		SmartBody::StringAttribute* stringAttribute = dynamic_cast<SmartBody::StringAttribute*>(attribute);
		if (stringAttribute)
		{
			notifyString(stringAttribute->getName(), stringAttribute->getValue());
			return;
		}

		SmartBody::Vec3Attribute* vec3Attribute = dynamic_cast<SmartBody::Vec3Attribute*>(attribute);
		if (vec3Attribute)
		{
			notifyVec3(vec3Attribute->getName(), vec3Attribute->getValue());
			return;
		}

		SmartBody::MatrixAttribute* matrixAttribute = dynamic_cast<SmartBody::MatrixAttribute*>(attribute);
		if (matrixAttribute)
		{
			notifyMatrix(matrixAttribute->getName(), matrixAttribute->getValue());
			return;
		}
	}
	
	SmartBody::SBObject::notify(subject);
}

void Nvbg::notifyLocal(std::string name)
{
	SmartBody::SBAttribute* attribute = getAttribute(name);
	if (attribute)
		SmartBody::SBObject::notify(attribute);
}

void Nvbg::notifyAction(std::string name)
{
	notifyLocal(name);
}

void Nvbg::notifyBool(std::string name, bool val)
{
	notifyLocal(name);
}

void Nvbg::notifyInt(std::string name, int val)
{
	notifyLocal(name);
}

void Nvbg::notifyDouble(std::string name, double val)
{
	notifyLocal(name);
}

void Nvbg::notifyString(std::string name, std::string val)
{
	notifyLocal(name);
}

void Nvbg::notifyVec3(std::string name, SrVec val)
{
	notifyLocal(name);
}

void Nvbg::notifyMatrix(std::string name, SrMat val)
{
	notifyLocal(name);
}

}






