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
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */


#include "OgreImage.h"
#include "WFImage.h"
#include <cassert>
namespace Ember
{
namespace OgreView
{

namespace Terrain
{

OgreImage::OgreImage(Image::ImageBuffer* buffer) :
	Image(buffer)
{

}

void OgreImage::blit(const WFImage& imageToBlit, unsigned int destinationChannel, int widthOffset, int heightOffset)
{
	if (imageToBlit.getChannels() > 1) {
		return;
	}

	unsigned int width = imageToBlit.getResolution();
	size_t i, j;

	size_t wfSegmentWidth = width * getChannels();
	size_t ogreImageWidth = getResolution() * getChannels();

	const unsigned char* sourcePtr = imageToBlit.getData();

	unsigned char* destStart = getData() + destinationChannel;

	unsigned char* destEnd = getData() + getSize();
	const unsigned int channels = getChannels();

	//Consider the bitmap with a coord system where (0,0) is in the upper left and dataEnd is in the lower right.
	//Since Ogre uses a different coord system than WF we need to first place the write pointer at the lower right of
	//where we want to blit. We will the work our ways upwards to the left (while advancing normally on the WF bitmap).
	//The math is thus:
	//start with the beginning of the bitmap:
	// destStart
	//then adjust it vertically using the offset
	// destStart + (channels * getResolution() * (heightOffset
	//however, we also need to adjust it more so that it's actually on on the lower line of where we want to blit
	// destStart + (channels * getResolution() * (heightOffset + (width - 1)))
	//now add with width offset
	// destStart + (channels * getResolution() * (heightOffset + (width - 1))) + (widthOffset * channels)
	//and then finally make sure that we're positioned at the lower right
	// destStart + (channels * getResolution() * (heightOffset + (width - 1))) + (widthOffset * channels) + wfSegmentWidth
	unsigned char* writePtr = destStart + (channels * getResolution() * (heightOffset + (width - 1))) + (widthOffset * channels) + wfSegmentWidth;


	for (i = 0; i < width; ++i) {
		writePtr -= wfSegmentWidth;
		for (j = 0; j < width; ++j) {
			assert(writePtr >= destStart && writePtr < destEnd);
			*(writePtr) = *(sourcePtr + j);
			//advance the number of channels
			writePtr += channels;
		}
		writePtr -= ogreImageWidth;
		sourcePtr += imageToBlit.getResolution();
	}
}

void OgreImage::blit(const OgreImage& imageToBlit, unsigned int destinationChannel, int widthOffset, int heightOffset)
{

}


}

}
}
