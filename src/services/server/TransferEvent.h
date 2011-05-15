/*
 Copyright (C) 2011 Erik Hjortsberg

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

#ifndef TRANSFEREVENT_H_
#define TRANSFEREVENT_H_

#include <Eris/TimedEventService.h>
#include <Eris/TransferInfo.h>

namespace Ember
{

class IState;

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief A transfer timed event, which transfers the client to a new server.
 *
 * This timed event is used to transfer a client to a new server, as part of a "teleport" operation.
 * It's implemented as a separate timed event to better fit with the life cycle of server states and Eris instances.
 *
 * Upon expiration the event will issue a call to IState::transfer().
 */
class TransferEvent : public Eris::TimedEvent
{
public:

	/**
	 * Ctor.
	 * @param state A state, which will be called upon expiration.
	 * @param transferInfo The information about the transfer.
	 */
	TransferEvent(IState& state, const Eris::TransferInfo& transferInfo);

    virtual void expired();

    virtual const WFMath::TimeStamp& due() const;

private:

    /**
     * @brief A state, which will be called upon expiration.
     */
    IState& mState;

    /**
     * @brief The information about the transfer.
     */
    const Eris::TransferInfo mTransferInfo;

    /**
     * @brief A timestamp for when the event is due.
     *
     * This is set to "now" at construction, as we want this event to trigger as soon as possible.
     */
    const WFMath::TimeStamp mDue;
};

}
#endif /* TRANSFEREVENT_H_ */
