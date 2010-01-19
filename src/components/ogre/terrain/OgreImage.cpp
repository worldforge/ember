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


#include "OgreImage.h"
#include "WFImage.h"
#include <cassert>
namespace EmberOgre
{

namespace Terrain
{

OgreImage::OgreImage(Image::ImageBuffer* buffer) :
	Image::Image(buffer)
{

}

void OgreImage::blit(const WFImage& imageToBlit, unsigned int destinationChannel, int widthOffset, int heightOffset)
{
	if (imageToBlit.getChannels() > 1) {
		return;
	}

	//	if (imageToBlit.getResolution() == getResolution() && widthOffset == 0 && heightOffset == 0) {
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
	size_t i, j;

	size_t wfSegmentWidth = width * getChannels();
	size_t ogreImageWidth = getResolution() * getChannels();

	const unsigned char* sourcePtr = imageToBlit.getData();
	unsigned char* destPtr = getData() + destinationChannel;

	unsigned char* dataEnd = getData() + getSize();

	unsigned char* end = destPtr + (getChannels() * getResolution() * ((width - 1) + heightOffset)) + (((width - 1) + widthOffset) * getChannels());

	unsigned char* tempPtr = end;
	for (i = 0; i < width; ++i) {
		tempPtr -= wfSegmentWidth;
		for (j = 0; j < width; ++j) {
			if (tempPtr >= getData() && tempPtr < dataEnd) {
				*(tempPtr) = *(sourcePtr + j);
			}
			///advance the number of channels
			tempPtr += getChannels();
		}
		tempPtr -= ogreImageWidth;
		sourcePtr += imageToBlit.getResolution();
	}
	//	}
}

void OgreImage::blit(const OgreImage& imageToBlit, unsigned int destinationChannel, int widthOffset, int heightOffset)
{

}


}

}
