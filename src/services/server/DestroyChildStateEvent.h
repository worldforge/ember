/*
 Copyright (C) 2011 Erik Hjortsberg <erik.hjortsberg@gmail.com>

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

#ifndef DESTROYCHILDSTATEEVENT_H_
#define DESTROYCHILDSTATEEVENT_H_
#include <Eris/TimedEventService.h>

namespace Ember
{

class IState;

/**
 * @brief Allows for delayed deletion of a child state.
 *
 * This is useful when you can't delete a state directly
 * (for example inside a callback, where there's code which will be run after the callback has completed
 * which is dependent on instances not being destroyed).
 */
class DestroyChildStateEvent : public Eris::TimedEvent
{
public:

	/**
	 * Ctor.
	 * @param state A state, which will have it's child state destroyed upon expiration.
	 */
	DestroyChildStateEvent(IState& state);

    virtual void expired();

    virtual const WFMath::TimeStamp& due() const;

private:

    /**
     * @brief A state, which will be called upon expiration.
     */
    IState& mState;

    /**
     * @brief A timestamp for when the event is due.
     *
     * This is set to "now" at construction, as we want this event to trigger as soon as possible.
     */
    const WFMath::TimeStamp mDue;
};
}
#endif /* DESTROYCHILDSTATEEVENT_H_ */
