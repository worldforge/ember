//
// C++ Interface: SoundSource
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@iteam.se>, (C) 2008
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
#ifndef EMBERSOUNDSOURCE_H
#define EMBERSOUNDSOURCE_H

#include "SoundGeneral.h"
#include <wfmath/vector.h>
#include <wfmath/quaternion.h>
#include <wfmath/point.h>
#include <AL/al.h>
namespace Ember {

/**
	@brief Represents a sound source in the 3d world.
	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
*/
class SoundSource
{
friend class SoundInstance;
public:
    SoundSource();

    virtual ~SoundSource();
    
    void setPosition(const WFMath::Point<3>& position);
    void setOrientation(const WFMath::Quaternion& orientation);
    void setVelocity(const WFMath::Vector<3>& velocity);
	
	/**
	* @brief Return openAl source within this sample
	 * @return 
	 */
	inline ALuint getALSource() const;
    
protected:

	ALuint mALSource;

};

ALuint SoundSource::getALSource() const
{
	return mALSource;
}

}

#endif
