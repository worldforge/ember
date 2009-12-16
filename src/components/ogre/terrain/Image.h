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

#ifndef IMAGE_H_
#define IMAGE_H_

#include "Buffer.h"

namespace EmberOgre
{

namespace Terrain
{

class WFImage;
class OgreImage;

class Image
{
public:
	typedef Buffer<unsigned char> ImageBuffer;
	Image(ImageBuffer* buffer);
	virtual ~Image();

	unsigned char* getData();

	const unsigned char* getData() const;

	unsigned int getChannels() const;

	size_t getSize() const;

	void reset();

	unsigned int getResolution() const;


	virtual void blit(const OgreImage& imageToBlit, unsigned int destinationChannel, unsigned int widthOffset = 0, unsigned int heightOffset = 0) = 0;
	virtual void blit(const WFImage& imageToBlit, unsigned int destinationChannel, unsigned int widthOffset = 0, unsigned int heightOffset = 0) = 0;

protected:

	ImageBuffer* mBuffer;
};

}

}

#endif /* IMAGE_H_ */
