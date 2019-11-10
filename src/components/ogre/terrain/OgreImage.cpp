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


#include "OgreImage.h"
#include "WFImage.h"
#include <cassert>

namespace Ember {
namespace OgreView {

namespace Terrain {

OgreImage::OgreImage(std::unique_ptr<Image::ImageBuffer> buffer) :
		Image(std::move(buffer)) {

}

void OgreImage::blit(const WFImage& imageToBlit, unsigned int destinationChannel, int widthOffset, int heightOffset) {
	if (imageToBlit.getChannels() > 1) {
		return;
	}

	const unsigned int destChannels = getChannels();
	const unsigned int sourceChannels = imageToBlit.getChannels();

	unsigned int sourceWidth = imageToBlit.getResolution();
	size_t wfSegmentWidth = sourceWidth * sourceChannels;
	size_t ogreImageWidth = getResolution() * destChannels;
	const unsigned char* sourcePtr = imageToBlit.getData();
	unsigned char* destStart = getData() + destinationChannel;

	// copy points line-by line
	unsigned char* destPtr = destStart + (ogreImageWidth * (getResolution() - heightOffset - 1)) + (widthOffset * destChannels);
	sourcePtr = sourcePtr + (wfSegmentWidth * wfSegmentWidth) - wfSegmentWidth;

	for (unsigned int i = 0; i < sourceWidth; ++i) {
		for (unsigned int j = 0; j < sourceWidth; ++j) {
			*(destPtr + (j * destChannels)) = *(sourcePtr + (j * sourceChannels));
		}
		destPtr -= ogreImageWidth;
		sourcePtr -= wfSegmentWidth;
	}
}

void OgreImage::blit(const OgreImage& imageToBlit, unsigned int destinationChannel, int widthOffset, int heightOffset) {

}


}

}
}
