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

#ifndef HEIGHTMAPBUFFERPROVIDER_H_
#define HEIGHTMAPBUFFERPROVIDER_H_

#include <vector>

namespace EmberOgre
{

namespace Terrain
{

template <typename> class Buffer;
class HeightMapBuffer;

class HeightMapBufferProvider
{
public:
	HeightMapBufferProvider(unsigned int bufferResolution);
	virtual ~HeightMapBufferProvider();

	void checkin(HeightMapBuffer* heightMapBuffer);

	HeightMapBuffer* checkout();

private:

	typedef std::vector<Buffer<float>* > BufferStore;
	BufferStore mPrimitiveBuffers;
	unsigned int mDesiredBuffers;
	unsigned int mBufferResolution;

};

}

}

#endif /* HEIGHTMAPBUFFERPROVIDER_H_ */
