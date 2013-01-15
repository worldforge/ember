/*
 Copyright (C) 2013 Erik Ogenvik

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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "FileResourceProvider.h"
#include <sstream>
#include <fstream>

namespace Ember
{

FileResourceWrapper::FileResourceWrapper(std::ifstream& stream) :
		mBuffer(nullptr), mSize(0)
{
	if (stream.is_open()) {

		stream.seekg(0, std::ios::end);
		mSize = stream.tellg();
		stream.seekg(0, std::ios::beg);

		mBuffer = new char[mSize];

		stream.read(mBuffer, mSize);
		stream.close();

	}

}

FileResourceWrapper::~FileResourceWrapper()
{
	delete[] mBuffer;
}

const char* FileResourceWrapper::getDataPtr()
{
	return mBuffer;
}

bool FileResourceWrapper::hasData()
{
	return mSize != 0;
}

size_t FileResourceWrapper::getSize()
{
	return mSize;
}

FileResourceProvider::FileResourceProvider(const std::string& baseDirectory) :
		mBaseDirectory(baseDirectory)
{
}

FileResourceProvider::~FileResourceProvider()
{
}

ResourceWrapper FileResourceProvider::getResource(const std::string& name)
{
	std::ifstream stream(mBaseDirectory + name, std::ios::binary | std::ios::in);
	return ResourceWrapper(new FileResourceWrapper(stream), name);
}

}
