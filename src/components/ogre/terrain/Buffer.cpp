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

#include "Buffer.h"

namespace EmberOgre
{

namespace Terrain
{

template<>
void Buffer<unsigned char>::reset()
{
	memset(mData, '\0', getSize());
}

template<>
void Buffer<unsigned int>::reset()
{
	memset(mData, '\0', getSize());
}

template<>
void Buffer<float>::reset()
{
	size_t size = getSize();
	for (size_t i = 0; i < size; ++i) {
		mData[i] = 0;
	}
}

}

}
