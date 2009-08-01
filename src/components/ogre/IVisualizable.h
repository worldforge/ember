/*
 Copyright (C) 2009 Erik Hjortsberg <erik.hjortsberg@gmail.com>

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

#ifndef IVISUALIZABLE_H_
#define IVISUALIZABLE_H_

#include <string>

namespace EmberOgre
{

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief An interface for graphical components which can provide simple late binding visualization features.
 *
 * Typical features would be debug visualizations.
 *
 */
class IVisualizable
{
public:

	/**
	 * @brief Dtor.
	 */
	virtual ~IVisualizable()
	{
	}

	/**
	 * General method for turning on and off debug visualizations. Subclasses might support more types of visualizations than the ones defined here.
	 * @param visualization The type of visualization. Currently supports "OgreBBox" and "ErisBBox".
	 * @param visualize Whether to visualize or not.
	 */
	virtual void setVisualize(const std::string& visualization, bool visualize) = 0;

	/**
	 * @brief Gets whether a certain visualization is turned on or off.
	 * @param visualization The type of visualization. Currently supports "OgreBBox" and "ErisBBox".
	 * @return true if visualization is turned on, else false
	 */
	virtual bool getVisualize(const std::string& visualization) const = 0;
};
}

#endif /* IVISUALIZABLE_H_ */
