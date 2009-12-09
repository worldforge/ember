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


#ifndef EMBEROGRETERRAINWFIMAGE_H_
#define EMBEROGRETERRAINWFIMAGE_H_

#include "Image.h"
namespace EmberOgre
{

namespace Terrain
{

class WFImage : public Image
{
public:
	WFImage(unsigned int width, unsigned int channels);
	WFImage(unsigned int width, unsigned int channels, unsigned char* data);

	void blit(const OgreImage& imageToBlit, unsigned int destinationChannel, unsigned int widthOffset = 0, unsigned int heightOffset = 0);
	void blit(const WFImage& imageToBlit, unsigned int destinationChannel, unsigned int widthOffset = 0, unsigned int heightOffset = 0);
};

}

}

#endif /* EMBEROGRETERRAINWFIMAGE_H_ */
