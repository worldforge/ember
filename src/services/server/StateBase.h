/*
 Copyright (C) 2010 Erik Ogenvik <erik@ogenvik.org>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef STATEBASE_H_
#define STATEBASE_H_

#include "IState.h"
#include <sigc++/trackable.h>
#include <memory>

namespace Ember {

class ServerServiceSignals;

/**
 * @brief Base core class for most instances of IState. The default behavior of all methods is to delegate to the parent state.
 *
 * This allows subclasses to rely on all methods being propagated upwards, unless they override on of the methods and provide their own implementation.
 */
class StateBaseCore : public IState, public virtual sigc::trackable {
public:
	explicit StateBaseCore(IState& parentState);

	~StateBaseCore() override = default;

	void disconnect() override;

	bool logout() override;

	void takeTransferredCharacter(const Eris::TransferInfo& transferInfo) override;

	void transfer(const Eris::TransferInfo& transferInfo) override;

	void destroyChildState() override;


protected:

	IState& getParentState() const;

	ServerServiceSignals& getSignals() const override;

	virtual StateBaseCore* getCoreChildState();

	virtual void clearChildState();

private:

	IState& mParentState;
};

template<typename TChildState>
class StateBase;

template<>
class StateBase<void> : public StateBaseCore {
public:
	explicit StateBase(IState& parentState) : StateBaseCore(parentState) {}

	~StateBase() override = default;

	IState& getTopState() override {
		return *this;
	}

protected:

	StateBaseCore* getCoreChildState() override { return nullptr; }

	void clearChildState() override {}

};

/**
 * @brief Base class for any states which will contain a parent state (which is pretty much each one).
 */
template<typename TChildState>
class StateBase : public StateBaseCore {
public:
	explicit StateBase(IState& parentState);

	~StateBase() override;

	IState& getTopState() override;

protected:

	void setChildState(std::unique_ptr<TChildState> childState);

	TChildState* getChildState() const;

	StateBaseCore* getCoreChildState() override;

	void clearChildState() override;

private:

	std::unique_ptr<TChildState> mChildState;
};

inline StateBaseCore::StateBaseCore(IState& parentState) :
		mParentState(parentState) {
}

inline IState& StateBaseCore::getParentState() const {
	return mParentState;
}

inline ServerServiceSignals& StateBaseCore::getSignals() const {
	return mParentState.getSignals();
}

inline void StateBaseCore::disconnect() {
	mParentState.disconnect();
}

inline bool StateBaseCore::logout() {
	return mParentState.logout();
}

inline void StateBaseCore::takeTransferredCharacter(const Eris::TransferInfo& transferInfo) {
	mParentState.takeTransferredCharacter(transferInfo);
}

inline void StateBaseCore::transfer(const Eris::TransferInfo& transferInfo) {
	mParentState.transfer(transferInfo);
}

inline void StateBaseCore::destroyChildState() {
	auto child = getCoreChildState();
	if (child) {
		child->destroyChildState();
		clearChildState();
	}
}

inline StateBaseCore* StateBaseCore::getCoreChildState() {
	return nullptr;
}

inline void StateBaseCore::clearChildState() {
}

template<typename TChildState>
inline StateBase<TChildState>::StateBase(IState& parentState) :
		StateBaseCore(parentState) {
}

template<typename TChildState>
inline StateBase<TChildState>::~StateBase() = default;

//template<>
//inline StateBase<void>::~StateBase() = default;
//
//template<>
//inline StateBaseCore* StateBase<void>::getCoreChildState() {
//	return nullptr;
//}

template<typename TChildState>
inline StateBaseCore* StateBase<TChildState>::getCoreChildState() {
	return mChildState.get();
}

template<typename TChildState>
inline void StateBase<TChildState>::clearChildState() {
	mChildState.reset();
}

//template<>
//inline void StateBase<void>::setChildState(std::unique_ptr<void> childState) {
//}

template<typename TChildState>
inline void StateBase<TChildState>::setChildState(std::unique_ptr<TChildState> childState) {
	mChildState = std::move(childState);
}

template<typename TChildState>
inline TChildState* StateBase<TChildState>::getChildState() const {
	return mChildState.get();
}

template<typename TChildState>
inline IState& StateBase<TChildState>::getTopState() {
	if (mChildState) {
		return ((IState*) (mChildState.get()))->getTopState();
	}
	return *this;
}


}

#endif /* STATEBASE_H_ */
