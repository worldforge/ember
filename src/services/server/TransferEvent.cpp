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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "TransferEvent.h"
#include "IState.h"
namespace Ember
{

TransferEvent::TransferEvent(IState& state, const Eris::TransferInfo& transferInfo)
: mState(state), mTransferInfo(transferInfo), mDue(WFMath::TimeStamp::now())
{

}

TransferEvent::~TransferEvent()
{
}

void TransferEvent::expired()
{
	mState.transfer(mTransferInfo);
}

const WFMath::TimeStamp& TransferEvent::due() const
{
	return mDue;
}

}
