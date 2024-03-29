/*
 Copyright (C) 2009 Erik Ogenvik <erik@ogenvik.org>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef HEIGHTMAPBUFFER_H_
#define HEIGHTMAPBUFFER_H_

#include <memory>

namespace Ember {
namespace OgreView {

namespace Terrain {

class HeightMapBufferProvider;

template<typename>
class Buffer;

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief Wraps a buffer instance and presents it to the height map system.
 * An instance of this will be created by the HeightMapBufferProvider, and at destruction it will automatically return the underlying Buffer instance to the provider.
 */
class HeightMapBuffer {
	friend class HeightMapBufferProvider;

public:

	/**
	 * @brief The buffer type for a height map.
	 */
	typedef Buffer<float> BufferType;

	/**
	 * @brief Ctor.
	 * @param provider The provider to which this instance belongs.
	 * @param buffer The buffer instance which will hold the actual data.
	 */
	HeightMapBuffer(HeightMapBufferProvider& provider, std::unique_ptr<BufferType> buffer);


	/**
	 * @brief Dtor.
	 * At destruction time the underlying buffer will be automatically returned to the provider.
	 */
	virtual ~HeightMapBuffer();

	/**
	 * @brief Gets the underlying buffer instance.
	 * @returns The underlying buffer instance.
	 */
	BufferType& getBuffer();

	/**
	 * @brief Gets the resolution of the buffer, i.e. the size of one side of the buffer.
	 * @returns The resolution of the buffer.
	 */
	unsigned int getResolution() const;


private:

	/**
	 * @brief The provider to which this buffer belongs.
	 */
	HeightMapBufferProvider& mProvider;

	/**
	 * @brief The underlying buffer which contains the data.
	 * This isn't destroyed along with this instance, rather it's recycled back to the provider.
	 */
	std::unique_ptr<BufferType> mBuffer;

};

}

}

}

#endif /* HEIGHTMAPBUFFER_H_ */
