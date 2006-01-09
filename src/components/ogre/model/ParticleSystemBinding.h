//
// C++ Interface: ParticleSystemBinding
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
#ifndef EMBEROGREPARTICLESYSTEMBINDING_H
#define EMBEROGREPARTICLESYSTEMBINDING_H

#include <Ogre.h>
#include <string>

namespace EmberOgre {
namespace Model {

class ParticleSystem;
/**
@author Erik Hjortsberg
*/
class ParticleSystemBinding
{
public:
	ParticleSystemBinding(ParticleSystem* parentSystem, const std::string& emitterVal, const std::string& variableName);
	void scaleValue(Ogre::Real scaler);
private:
	std::string mEmitterVal;
	std::string mVariableName;
	ParticleSystem* mParticleSystem;
	Ogre::Real mOriginalValue;
public:
	inline const std::string& getEmitterVal() const {return mEmitterVal;}
	inline const std::string& getVariableName() const {return mVariableName;}

};
}
}
#endif
