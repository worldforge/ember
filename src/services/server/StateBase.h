/*
 Copyright (C) 2010 erik

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef STATEBASE_H_
#define STATEBASE_H_

#include "IState.h"
#include "NonConnectedAdapter.h"
#include <sigc++/trackable.h>

namespace Ember {

class ServerServiceSignals;

class StateBaseCore : public IState, public virtual sigc::trackable
{
public:
	StateBaseCore(IState& parentState);
	virtual ~StateBaseCore();

	virtual void disconnect();

	virtual bool logout();

	virtual void takeCharacter(const std::string &id);

	virtual bool createCharacter(const std::string& name, const std::string& sex, const std::string& type, const std::string& description, const std::string& spawnName);

	virtual IServerAdapter& getServerAdapter();

protected:

	IState& getParentState() const;

	ServerServiceSignals& getSignals() const;

private:

	IState& mParentState;
};

template <typename TChildState>
class StateBase : public StateBaseCore
{
public:
	StateBase(IState& parentState);

	virtual ~StateBase();

	virtual void destroyChildState();

	virtual IState& getTopState();

protected:

	virtual void setChildState(TChildState* childState);

	TChildState* getChildState() const;

private:

	TChildState* mChildState;
};

inline StateBaseCore::StateBaseCore(IState& parentState)
: mParentState(parentState)
{
}

inline StateBaseCore::~StateBaseCore()
{
}


inline IState& StateBaseCore::getParentState() const
{
	return mParentState;
}

inline ServerServiceSignals& StateBaseCore::getSignals() const
{
	return mParentState.getSignals();
}
inline void StateBaseCore::disconnect()
{
	mParentState.disconnect();
}

inline bool StateBaseCore::logout()
{
	return mParentState.logout();
}

inline void StateBaseCore::takeCharacter(const std::string &id)
{
	mParentState.takeCharacter(id);
}

inline bool StateBaseCore::createCharacter(const std::string& name, const std::string& sex, const std::string& type, const std::string& description, const std::string& spawnName)
{
	return mParentState.createCharacter(name, sex, type, description, spawnName);
}

inline IServerAdapter& StateBaseCore::getServerAdapter()
{
	static NonConnectedAdapter sNonConnectedAdapter;
	return sNonConnectedAdapter;
}


template <typename TChildState>
inline StateBase<TChildState>::StateBase(IState& parentState)
: StateBaseCore::StateBaseCore(parentState), mChildState(0)
{
}

template <typename TChildState>
inline StateBase<TChildState>::~StateBase()
{
	delete mChildState;
}


template <typename TChildState>
inline void StateBase<TChildState>::destroyChildState()
{
	delete mChildState;
	mChildState = 0;
}

template <>
inline void StateBase<void>::destroyChildState()
{
}

template <typename TChildState>
inline void StateBase<TChildState>::setChildState(TChildState* childState)
{
	delete mChildState;
	mChildState = childState;
}

template <typename TChildState>
inline TChildState* StateBase<TChildState>::getChildState() const
{
	return mChildState;
}

template <typename TChildState>
inline IState& StateBase<TChildState>::getTopState()
{
	if (mChildState) {
		return ((IState*)(mChildState))->getTopState();
	}
	return *this;
}



}

#endif /* STATEBASE_H_ */
