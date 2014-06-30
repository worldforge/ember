#ifndef DSUBJECT_H
#define DSUBJECT_H

#include <set>
#include <sb/SBTypes.h>

namespace SmartBody {

class SBObserver;

class SBSubject
{
	public:
		SBAPI SBSubject();
		SBAPI ~SBSubject();
		SBAPI virtual void registerObserver(SBObserver* observer);
		SBAPI virtual void unregisterObserver(SBObserver* observer);
		SBAPI virtual void notifyObservers();	
	protected:
		std::set<SBObserver*> m_observers;

};

};

#endif