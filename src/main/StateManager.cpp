#include "StateManager.h"
#include "framework/Exception.h"

using namespace dime;

typedef std::list<State>::iterator stateIter;

State& StateManager::findState(const std::string& state)
{
  for(stateIter i = myStates.begin();i != myStates.end();i++)
    {
      if ((*i).getName() == state)
	return *i;
    }

  // Maybe search for the default version of the state in future?
  // If not found then throw.
  THROW("State not found");
}
