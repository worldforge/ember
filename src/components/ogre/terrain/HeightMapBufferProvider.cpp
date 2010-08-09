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

#include "HeightMapBufferProvider.h"
#include "HeightMapBuffer.h"
#include "Buffer.h"

namespace EmberOgre
{

namespace Terrain
{

HeightMapBufferProvider::HeightMapBufferProvider(unsigned int bufferResolution, unsigned int desiredBuffers, unsigned int desiredBuffersTolerance) :
	mBufferResolution(bufferResolution), mDesiredBuffers(desiredBuffers), mDesiredBuffersTolerance(desiredBuffersTolerance)
{
	while (mPrimitiveBuffers.size() < mDesiredBuffers) {
		mPrimitiveBuffers.push_back(new Buffer<float> (mBufferResolution, 1));
	}
}

HeightMapBufferProvider::~HeightMapBufferProvider()
{
	for (BufferStore::const_iterator I = mPrimitiveBuffers.begin(); I != mPrimitiveBuffers.end(); ++I) {
		delete *I;
	}
}

void HeightMapBufferProvider::checkin(HeightMapBuffer& heightMapBuffer)
{
	Buffer<float>* buffer = heightMapBuffer.getBuffer();
	mPrimitiveBuffers.push_back(buffer);
}

HeightMapBuffer* HeightMapBufferProvider::checkout()
{
	if (mPrimitiveBuffers.size() == 0) {
		while (mPrimitiveBuffers.size() < mDesiredBuffers) {
			mPrimitiveBuffers.push_back(new Buffer<float> (mBufferResolution, 1));
		}
	}
	Buffer<float>* buffer = mPrimitiveBuffers.back();
	mPrimitiveBuffers.pop_back();
	return new HeightMapBuffer(*this, buffer);
}

void HeightMapBufferProvider::maintainPool()
{

	if (mPrimitiveBuffers.size() <= mDesiredBuffers - mDesiredBuffersTolerance) {
		while (mPrimitiveBuffers.size() < mDesiredBuffers) {
			mPrimitiveBuffers.push_back(new Buffer<float> (mBufferResolution, 1));
		}
	}

	if (mPrimitiveBuffers.size() >= mDesiredBuffers + mDesiredBuffersTolerance) {
		while (mPrimitiveBuffers.size() > mDesiredBuffers) {
			Buffer<float>* buffer = mPrimitiveBuffers.back();
			mPrimitiveBuffers.pop_back();
			delete buffer;
		}
	}
}

}

}
