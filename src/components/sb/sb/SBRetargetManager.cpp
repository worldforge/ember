#include "SBRetargetManager.h"
#include <sb/SBRetarget.h>


namespace SmartBody {

SBAPI SBRetargetManager::SBRetargetManager()
{

}

SBAPI SBRetargetManager::~SBRetargetManager()
{

}

SBAPI SBRetarget* SBRetargetManager::createRetarget( std::string sourceSk, std::string targetSk )
{	
	if (getRetarget(sourceSk, targetSk))
	{
		LOG("Retarget for skeleton pair (%s,%s) already exist.", sourceSk.c_str(), targetSk.c_str());
		return NULL;			
	}

	SmartBody::SBRetarget* retarget = new SmartBody::SBRetarget(sourceSk,targetSk);
	StringPair skNamePair = StringPair(sourceSk,targetSk);
	_retargets[skNamePair] = retarget;
	return retarget;
}

SBAPI SBRetarget* SBRetargetManager::getRetarget( std::string sourceSk, std::string targetSk )
{
	StringPair skNamePair = StringPair(sourceSk,targetSk);
	SmartBody::SBRetarget* retarget = NULL;
	if (_retargets.find(skNamePair) != _retargets.end())
	{
		retarget = _retargets[skNamePair];
	}
	return retarget;
}

SBAPI std::vector<StringPair> SBRetargetManager::getRetargetNames()
{
	std::vector<StringPair> retargetNames;
	std::map<StringPair, SBRetarget*>::iterator ri;
	for ( ri  = _retargets.begin();
		  ri != _retargets.end();
		  ri++)
	{
		retargetNames.push_back(ri->first);
	}
	return retargetNames;
}

}
