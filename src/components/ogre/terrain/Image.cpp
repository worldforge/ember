/*
 * Image.cpp
 *
 *  Created on: 15 dec 2009
 *      Author: erik
 */

#include "Image.h"

#include "Buffer.h"

namespace EmberOgre
{

namespace Terrain
{

Image::Image(ImageBuffer* buffer) :
	mBuffer(buffer)
{

}

Image::~Image()
{
	delete mBuffer;
}

unsigned char* Image::getData()
{
	return mBuffer->getData();
}

const unsigned char* Image::getData() const
{
	return mBuffer->getData();

}

unsigned int Image::getChannels() const
{
	return mBuffer->getChannels();
}

size_t Image::getSize() const
{
	return mBuffer->getSize();
}

void Image::reset()
{
	mBuffer->reset();
}

unsigned int Image::getResolution() const
{
	return mBuffer->getResolution();
}

}

}
