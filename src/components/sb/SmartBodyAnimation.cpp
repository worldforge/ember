/*
 Copyright (C) 2014 Céline NOËL <celine.noel.7294@gmail.com>

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

 #include "SmartBodyAnimation.h"


namespace Ember
{

SmartBodyAnimation::SmartBodyAnimation(SmartBodyAnimation::Name name)
{
	switch (name)
	{
		case Name::IDLE:
			mBmlRequest = std::string("<body posture=\"ChrUtah_Idle001\"/>");
			break;

		case Name::WALK:
			mBmlRequest = std::string("<body posture=\"ChrUtah_Walk001\"/>");
			break;

		case Name::RUN:
			mBmlRequest = std::string("<body posture=\"ChrUtah_Run001\"/>");
			break;

		case Name::NOD:
			mBmlRequest = std::string("<head type=\"NOD\" repeats=\"2\"/>");
			break;
	}
}

SmartBodyAnimation::~SmartBodyAnimation()
{
}


const std::string& SmartBodyAnimation::getBmlRequest() const
{
	return mBmlRequest;
}

}