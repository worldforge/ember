/*
 Copyright (C) 2011 Erik Ogenvik

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

#include "AvatarTransferInfo.h"

#include <utility>

namespace Ember {
AvatarTransferInfo::AvatarTransferInfo(std::string avatarName, WFMath::TimeStamp timestamp, Eris::TransferInfo transferInfo) :
		mAvatarName(std::move(avatarName)),
		mTimestamp(timestamp),
		mTransferInfo(std::move(transferInfo)) {
}

const std::string& AvatarTransferInfo::getAvatarName() const {
	return mAvatarName;
}

const WFMath::TimeStamp& AvatarTransferInfo::getTimestamp() const {
	return mTimestamp;
}

const Eris::TransferInfo& AvatarTransferInfo::getTransferInfo() const {
	return mTransferInfo;
}

}
