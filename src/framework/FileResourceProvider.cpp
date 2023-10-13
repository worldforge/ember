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
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "FileResourceProvider.h"
#include <sstream>
#include <fstream>

namespace Ember {

FileResourceWrapper::FileResourceWrapper(std::ifstream& stream) {
	if (stream.is_open()) {

		stream.seekg(0, std::ios::end);
		auto size = stream.tellg();
		stream.seekg(0, std::ios::beg);

		mBuffer.resize(size);

		stream.read(mBuffer.data(), size);
		stream.close();

	}

}

FileResourceWrapper::~FileResourceWrapper() = default;

const char* FileResourceWrapper::getDataPtr() {
	return mBuffer.data();
}

bool FileResourceWrapper::hasData() {
	return !mBuffer.empty();
}

size_t FileResourceWrapper::getSize() {
	return mBuffer.size();
}

FileResourceProvider::FileResourceProvider(boost::filesystem::path baseDirectory) :
		mBaseDirectory(std::move(baseDirectory)) {
}

FileResourceProvider::~FileResourceProvider() = default;

ResourceWrapper FileResourceProvider::getResource(const std::string& name) {
	std::ifstream stream((mBaseDirectory / name).string(), std::ios::binary | std::ios::in);
	return ResourceWrapper(std::make_shared<FileResourceWrapper>(stream), name);
}

}
