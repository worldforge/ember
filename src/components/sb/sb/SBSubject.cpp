#include "SBSubject.h"
#include "SBObserver.h"

namespace SmartBody {

SBSubject::SBSubject()
{
}

SBSubject::~SBSubject()
{
	for (std::set<SBObserver*>::iterator iter = m_observers.begin();
		 iter !=  m_observers.end();
		 iter++)
	{
		(*iter)->removeDependency(this);
	}
}

void SBSubject::registerObserver(SBObserver* observer)
{
	std::set<SBObserver*>::iterator iter = m_observers.find(observer);
	if (iter == m_observers.end())
	{
		m_observers.insert(observer);
		observer->addDependency(this);
	}
}

void SBSubject::unregisterObserver(SBObserver* observer)
{
	std::set<SBObserver*>::iterator iter = m_observers.find(observer);
	if (iter != m_observers.end())
	{
		m_observers.erase(iter);
		observer->removeDependency(this);
	}
}

void SBSubject::notifyObservers()
{
	for (std::set<SBObserver*>::iterator iter = m_observers.begin();
		 iter != m_observers.end();
		 iter++)
	{
		(*iter)->notify(this);
	}
}

};
