/*
 Copyright (C) 2010 Erik Hjortsberg <erik.hjortsberg@gmail.com>

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

#ifndef EMBEROGREILIGHTNING_H_
#define EMBEROGREILIGHTNING_H_

namespace WFMath
{
	template<int> class Vector;
}

namespace EmberOgre {

/**
 * @brief Provider of main light (like a sun or moon) lightning information.
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 */
class ILightning
{
public:
	virtual ~ILightning(){}

	/**
	 * @brief Gets the direction of the main light, in world space.
	 * @returns The direction of the main light, in world space.
	 */
	virtual WFMath::Vector<3> getMainLightDirection() const = 0;

};
}

#endif /* EMBEROGREILIGHTNING_H_ */
