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

#ifndef AVATARTRANSFERINFO_H_
#define AVATARTRANSFERINFO_H_

#include <Eris/TransferInfo.h>
#include <wfmath/timestamp.h>

namespace Ember
{
/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief Envelopes an Eris::TransferInfo object with time stamp and avatar name.
 *
 * So that we can present a little more informative message to the user.
 */
class AvatarTransferInfo
{
public:
	/**
	 * @brief Ctor.
	 * @param avatarName The name of the avatar.
	 * @param timestamp A timestamp for when the transfer info was created.
	 * @param transferInfo A transfer info object.
	 */
	AvatarTransferInfo(const std::string& avatarName, const WFMath::TimeStamp timestamp, const Eris::TransferInfo& transferInfo);

	/**
	 * @brief Gets the name of the avatar which was transferred.
	 * @return The name of the avatar.
	 */
	const std::string& getAvatarName() const;

	/**
	 * @brief Gets the time stamp for when the transfer info was created.
	 * @return A time stamp.
	 */
	const WFMath::TimeStamp& getTimestamp() const;

	/**
	 * @brief Gets the transfer info instance.
	 * @return The transfer info instance.
	 */
	const Eris::TransferInfo& getTransferInfo() const;

private:

	/**
	 * @brief The name of the avatar.
	 */
	std::string mAvatarName;

	/**
	 * @brief A time stamp for when the transfer info was created.
	 */
	WFMath::TimeStamp mTimestamp;

	/**
	 * @brief The transfer info instance.
	 */
	Eris::TransferInfo mTransferInfo;
};
}
#endif /* AVATARTRANSFERINFO_H_ */
