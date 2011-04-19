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

#include "AvatarTransferInfo.h"
namespace Ember
{
AvatarTransferInfo::AvatarTransferInfo(const std::string& avatarName, const WFMath::TimeStamp timestamp, const Eris::TransferInfo& transferInfo) :
	mAvatarName(avatarName), mTimestamp(timestamp), mTranferInfo(transferInfo)
{
}

const std::string& AvatarTransferInfo::getAvatarName() const
{
	return mAvatarName;
}

const WFMath::TimeStamp& AvatarTransferInfo::getTimestamp() const
{
	return mTimestamp;
}

const Eris::TransferInfo& AvatarTransferInfo::getTransferInfo() const
{
	return mTransferInfo;
}

}
