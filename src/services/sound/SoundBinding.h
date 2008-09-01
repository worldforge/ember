//
// C++ Interface: SoundBinding
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
#ifndef EMBERSOUNDBINDING_H
#define EMBERSOUNDBINDING_H

namespace Ember {
class SoundSource;
/**
@brief Acts as a binding between a sound source and one or many sound data buffers.

An instance of this is responsible for binding a sound source and one or many sound data buffers together. The buffers will contain the actual sound data to be played, and without this binding no sound can be played.
Normally the binding occurs in the constructor, but some sounds needs to be streamed, and thus further binding alterations needs to occur in the update() method. The update() method will be called once each frame when the binding is in use.

@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
*/
class SoundBinding
{
public:

/**
 * @brief Ctor.
 * In most subclasses you want to perform the initial, if not all, bind operations in the constructor.
 * @param source The sound source to which this binding should bind any sound data buffers.
 */
SoundBinding(SoundSource& source);
/**
 * @brief Dtor.
 * Normally nothing should happen here, unless there's a need to unqueue any buffers.
 */
virtual ~SoundBinding();

/**
 * @brief Called each frame to allow the binding to do any dynamic updates if so required.
 * This is especially true for streaming sounds, where the buffers needs to be updated as OpenAL plays through them.
 */
virtual void update() = 0;

protected:
/**
 * @brief The SoundSource to which this binding is attached.
 */
SoundSource& mSource;
};

}

#endif
