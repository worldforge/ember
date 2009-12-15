/*
 * Image.h
 *
 *  Created on: 15 dec 2009
 *      Author: erik
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
