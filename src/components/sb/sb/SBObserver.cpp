#include "SBObserver.h"
#include "SBSubject.h"

#include <vector>

namespace SmartBody {

SBObserver::SBObserver()
{
}

SBObserver::~SBObserver()
{
	std::vector<SBSubject*> tmpSubjects;
	for (std::set<SBSubject*>::iterator iter = _subjects.begin(); 
		 iter != _subjects.end();
		 iter++)
	{
		tmpSubjects.push_back(*iter);
	}
	
	for (std::vector<SBSubject*>::iterator iter = tmpSubjects.begin(); 
		 iter != tmpSubjects.end();
		 iter++)
	{
		(*iter)->unregisterObserver(this);
	}
}

void SBObserver::addDependency(SBSubject* subject)
{
	if (_subjects.find(subject) == _subjects.end())
	{
		_subjects.insert(subject);
	}
}

void SBObserver::removeDependency(SBSubject* subject)
{
	std::set<SBSubject*>::iterator iter = _subjects.find(subject);
	if (iter != _subjects.end())
	{
		_subjects.erase(iter);
	}
}

void SBObserver::notify(SBSubject* subject)
{
}

bool SBObserver::hasDependency(SBSubject* subject)
{
	return (_subjects.find(subject) != _subjects.end());
}

};
