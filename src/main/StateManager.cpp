#include "StateManager.h"

using namespace dime;

typedef std::list<State>::iterator stateIter;

State& StateManager::findState(const std::string& state)
{
  for(stateIter i = myStates.begin();i != myStates.end();i++)
    {
      if ((*i).getName() == state)
	return *i;
    }
}
