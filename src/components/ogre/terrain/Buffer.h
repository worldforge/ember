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


#ifndef EMBEROGRETERRAINBUFFER_H_
#define EMBEROGRETERRAINBUFFER_H_

#include <stdlib.h>
#include <string.h>

namespace EmberOgre
{

namespace Terrain
{

template<typename DataType>
class Buffer
{
public:

	Buffer(unsigned int resolution, unsigned int channels);
	Buffer(unsigned int resolution, unsigned int channels, DataType* data, bool transferOwnership = false);

	virtual ~Buffer();

	DataType* getData();

	const DataType* getData() const;

	unsigned int getChannels() const;

	size_t getSize() const;

	void reset();

	unsigned int getResolution() const;

protected:

	const unsigned int mResolution;
	const unsigned int mChannels;

	DataType* mData;
	bool mDataOwned;

};

template<typename DataType>
Buffer<DataType>::Buffer(unsigned int width, unsigned int channels) :
	mResolution(width), mChannels(channels), mData(new DataType[width * width * channels]), mDataOwned(true)
{
}

template<typename DataType>
Buffer<DataType>::Buffer(unsigned int width, unsigned int channels, DataType* data, bool transferOwnership) :
	mResolution(width), mChannels(channels), mData(data), mDataOwned(transferOwnership)
{
}

template<typename DataType>
Buffer<DataType>::~Buffer()
{
	if (mDataOwned) {
		delete[] mData;
	}
}

template<typename DataType>
DataType* Buffer<DataType>::getData()
{
	return mData;
}

template<typename DataType>
const DataType* Buffer<DataType>::getData() const
{
	return mData;
}

template<typename DataType>
unsigned int Buffer<DataType>::getChannels() const
{
	return mChannels;
}

template<typename DataType>
size_t Buffer<DataType>::getSize() const
{
	return mResolution * mResolution * mChannels;
}

template<typename DataType>
unsigned int Buffer<DataType>::getResolution() const
{
	return mResolution;
}

template<typename DataType>
void Buffer<DataType>::reset()
{
	memset(mData, '\0', getSize());
}

}

}

#endif /* EMBEROGRETERRAINBUFFER_H_ */
