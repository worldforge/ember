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

#ifndef RETARGET_H
#define RETARGET_H

	#include <string>


namespace SmartBody
{
class SBRetargetManager;
}

namespace Ember
{

/**
 @brief Create a retarget instance.

 This class has to be instantiate by any SmartBodyBehaviors to retarget a behavior set on another character.

 @author Céline NOEL <celine.noel.7294@gmail.com>

 */
class SmartBodyRetarget
{
public:

	/**
	 * @brief Ctor.
	 */
	SmartBodyRetarget(const std::string& srcSkName, const std::string& tgtSkName, SmartBody::SBRetargetManager& manager);

	/**
	 * @brief Dtor.
	 */
	~SmartBodyRetarget();

private:

	/**
	 * @brief Create an instance of SBRetarget.
	 */
	void createInstance(const std::string& srcSk, const std::string& tgtSk, SmartBody::SBRetargetManager& manager);
};

}

#endif