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

#ifndef HEIGHTMAPBUFFER_H_
#define HEIGHTMAPBUFFER_H_

namespace EmberOgre
{

namespace Terrain
{

class HeightMapBufferProvider;

template <typename> class Buffer;

class HeightMapBuffer
{
public:
	typedef Buffer<float> BufferType;
	HeightMapBuffer(HeightMapBufferProvider& provider, BufferType* buffer);
	virtual ~HeightMapBuffer();

	BufferType* getBuffer();

	unsigned int getResolution() const;
private:

	HeightMapBufferProvider& mProvider;
	BufferType* mBuffer;
};

}

}

#endif /* HEIGHTMAPBUFFER_H_ */
