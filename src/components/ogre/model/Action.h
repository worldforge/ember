//
// C++ Interface: Action
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2005
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifndef EMBEROGREACTION_H
#define EMBEROGREACTION_H
#include "AnimationSet.h"
namespace EmberOgre {
namespace Model {

/**
@author Erik Hjortsberg
*/
class Action
{
public:
	inline AnimationSet& getAnimations() { return mAnimations; }
	inline void setName(const std::string& name) { mName = name; }
	inline const std::string& getName() { return mName; }
	

protected:
	std::string mName;
	AnimationSet mAnimations;
};

}
}
#endif
