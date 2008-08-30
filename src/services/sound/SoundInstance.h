//
// C++ Interface: SoundInstance
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
#ifndef EMBERSOUNDINSTANCE_H
#define EMBERSOUNDINSTANCE_H

#include <memory>

namespace Ember {

class SoundSource;
class BaseSoundSample;
class SoundBinding;
class SoundService;

class ISoundMotionProvider
{
public:
	virtual ~ISoundMotionProvider() {}
	virtual void update(SoundSource& soundSource) = 0;
};

/**
	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
*/
class SoundInstance
{
friend class SoundService;
public:
    SoundInstance();

    ~SoundInstance();
    
    bool play();
    bool stop();
    
    SoundSource& getSource();
    
	void bind(SoundBinding* binding);
	
	inline void setMotionProvider(ISoundMotionProvider* motionProvider);
	
protected:

	void update();
	
	std::auto_ptr<SoundSource> mSource;
	
	SoundBinding* mBinding;
	
	ISoundMotionProvider* mMotionProvider;

};

void SoundInstance::setMotionProvider(ISoundMotionProvider* motionProvider)
{
	mMotionProvider = motionProvider;
}

}

#endif
