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

#ifndef HEIGHTMAPBUFFERPROVIDER_H_
#define HEIGHTMAPBUFFERPROVIDER_H_

#include "HeightMapBuffer.h"
#include <vector>
#include <memory>
#include <mutex>

namespace Ember {
namespace OgreView {

namespace Terrain {

template<typename>
class Buffer;

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief A height map buffer provider, which for performance reasons keeps a pool of buffers which are recycled as new HeightMapBuffer instances are created.
 * To help with performance and to avoid memory fragmentation this class is used to keep a collection of Buffer instances, which are used by HeightMapBuffer instances.
 * The HeightMapBuffer class will at destruction automatically return the Buffer instance to the provider.
 */
class HeightMapBufferProvider {
	friend class HeightMapBuffer;

public:
	/**
	 * @brief Ctor.
	 * @param bufferResolution The resolution of a buffer. This is normally the "size of one segment plus one".
	 * @param desiredBuffers The amount of desired buffers to keep in the pool.
	 * @param desiredBuffersTolerance How much the amount in the pool is allowed to differ from the desired amount before buffers are created or destroyed.
	 */
	explicit HeightMapBufferProvider(unsigned int bufferResolution,
									 unsigned int desiredBuffers = 5,
									 unsigned int desiredBuffersTolerance = 2);

	/**
	 * @brief Dtor.
	 */
	virtual ~HeightMapBufferProvider();


	/**
	 * @brief Checks out a new HeightMapBuffer instance.
	 * Note that the buffer is automatically checked in when the checked out instance is destroyed.
	 */
	std::unique_ptr<HeightMapBuffer> checkout();

private:

	/**
	 * @brief The pool of unused Buffer instances.
	 */
	std::vector<std::unique_ptr<HeightMapBuffer::BufferType>> mPrimitiveBuffers;

	/**
	 * @brief The resolution of one buffer. This is normally the size of one terrain segment plus one (to match Mercator::Segment).
	 */
	unsigned int mBufferResolution;

	/**
	 * @brief The amount of desired buffers to keep in the pool.
	 */
	unsigned int mDesiredBuffers;

	/**
	 * @brief How much the amount in the pool is allowed to differ from the desired amount before buffers are created or destroyed.
	 */
	unsigned int mDesiredBuffersTolerance;

	/**
	 * @brief Returns a previous checked out height map buffer instance.
	 * This will return the Buffer instance to the pool.
	 * @param buffer The height buffer to return to the pool.
	 */
	void checkin(std::unique_ptr<HeightMapBuffer::BufferType> buffer);

	/**
	 * @brief Makes sure that the pool contains the desired amount of buffers.
	 */
	void maintainPool();

	std::mutex mMutex;
};

}

}

}

#endif /* HEIGHTMAPBUFFERPROVIDER_H_ */
