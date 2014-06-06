#include "SBBehaviorSet.h"

namespace SmartBody {

SBBehaviorSet::SBBehaviorSet()
{
}

SBBehaviorSet::~SBBehaviorSet()
{
}

void SBBehaviorSet::setName(const std::string& name)
{
	_name = name;
}

const std::string& SBBehaviorSet::getName()
{
	return _name;
}

void SBBehaviorSet::setScript(const std::string& name)
{
	_script = name;
}

const std::string& SBBehaviorSet::getScript()
{
	return _script;
}

}



