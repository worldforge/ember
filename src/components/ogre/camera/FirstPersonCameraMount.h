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

#ifndef FIRSTPERSONCAMERAMOUNT_H_
#define FIRSTPERSONCAMERAMOUNT_H_

#include "CameraMountBase.h"
#include <OgreMath.h>
namespace EmberOgre
{
namespace Camera
{
class FirstPersonCameraMount : CameraMountBase
{
public:

private:
	Ogre::Degree mDegreeOfPitchPerSecond;
	Ogre::Degree mDegreeOfYawPerSecond;

};

}
}

#endif /* FIRSTPERSONCAMERAMOUNT_H_ */
