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

#ifndef FILERESOURCEPROVIDER_H_
#define FILERESOURCEPROVIDER_H_

#include "IResourceProvider.h"
#include <boost/filesystem/path.hpp>
#include <boost/noncopyable.hpp>

#include <string>

namespace Ember {

class FileResourceWrapper : public IResourceWrapper, public boost::noncopyable {
public:
	explicit FileResourceWrapper(std::ifstream& stream);

	~FileResourceWrapper() override;

	const char* getDataPtr() override;

	bool hasData() override;

	size_t getSize() override;

private:

	std::vector<char> mBuffer;
};

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief Loads files relative to a base directory.
 */
class FileResourceProvider : public IResourceProvider, public boost::noncopyable {
public:
	/**
	 * @brief Ctor.
	 * @param baseDirectory The base directory.
	 */
	explicit FileResourceProvider(boost::filesystem::path baseDirectory);

	~FileResourceProvider() override;

	ResourceWrapper getResource(const std::string& name) override;

private:
	const boost::filesystem::path mBaseDirectory;
};

}
#endif /* FILERESOURCEPROVIDER_H_ */
