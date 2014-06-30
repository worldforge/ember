#ifndef DOBSERVER_H
#define DOBSERVER_H

#include <sb/SBTypes.h>
#include <set>

namespace SmartBody {

class SBSubject;

class SBObserver
{
	public:
		SBAPI SBObserver();
		SBAPI ~SBObserver();

		SBAPI virtual void addDependency(SBSubject* subject);
		SBAPI virtual void removeDependency(SBSubject* subject);
		SBAPI bool hasDependency(SBSubject* subject);

		SBAPI virtual void notify(SBSubject* subject);

	protected:
		std::set<SBSubject*> _subjects;
};

};

#endif