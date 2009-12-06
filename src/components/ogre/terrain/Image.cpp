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

#include "Image.h"

#include "framework/Exception.h"

#include <string.h>
#include <cassert>

namespace EmberOgre
{

namespace Terrain
{

Image::Image(unsigned int width, unsigned int channels) :
	mWidth(width), mChannels(channels), mData(new unsigned char[width * width * channels]), mDataOwned(true)
{
}

Image::Image(unsigned int width, unsigned int channels, unsigned char* data) :
	mWidth(width), mChannels(channels), mData(data), mDataOwned(false)
{
}

Image::~Image()
{
	if (mDataOwned) {
		delete[] mData;
	}
}

unsigned char* Image::getData()
{
	return mData;
}

const unsigned char* Image::getData() const
{
	return mData;
}

unsigned int Image::getChannels() const
{
	return mChannels;
}

size_t Image::getSize() const
{
	return mWidth * mWidth * mChannels;
}

unsigned int Image::getWidth() const
{
	return mWidth;
}

void Image::reset()
{
	memset(mData, '\0', getSize());
}

void Image::blit(const Image& imageToBlit, unsigned int destinationChannel, unsigned int widthOffset, unsigned int heightOffset)
{
	if (imageToBlit.getChannels() > 1) {
		return;
	}

//	if (imageToBlit.getWidth() == getWidth() && widthOffset == 0 && heightOffset == 0) {
//		const unsigned char* sourcePtr = imageToBlit.getData();
//		unsigned char* destPtr = getData() + destinationChannel;
//
//		for (unsigned int i = 0; i < imageToBlit.getSize(); ++i) {
//			*destPtr = *sourcePtr;
//			destPtr += mChannels;
//			sourcePtr++;
//		}
//	} else {
		//	unsigned int width = imageToBlit.getSize();
		unsigned int width = 64;
		long i, j;

		const unsigned char* sourcePtr = imageToBlit.getData();
		unsigned char* destPtr = getData() + destinationChannel;

		unsigned char* dataEnd = getData() + getSize();

		unsigned char* end = destPtr + (mChannels * mWidth * ((width - 1) + heightOffset)) + (((width - 1) + widthOffset) * mChannels);
		assert(end >= mData && end <= dataEnd);
		///we need to do this to get the alignment correct
		//		sourcePtr += width + 1;

//		unsigned char* tempPtr = end + destinationChannel + mChannels;
		unsigned char* tempPtr = end;
		for (i = 0; i < width; ++i) {
			tempPtr -= (width * mChannels);
			for (j = 0; j < width; ++j) {
				if (tempPtr >= mData && tempPtr < dataEnd) {
					assert(tempPtr >= mData && tempPtr < dataEnd);
					*(tempPtr) = *(sourcePtr + j);
				}
				///advance the number of channels
				tempPtr += mChannels;

			}
			tempPtr -= (mWidth * mChannels);
			sourcePtr += 65;
		}
//	}
}

}

}
