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

#ifndef FILERESOURCEPROVIDER_H_
#define FILERESOURCEPROVIDER_H_

#include "IResourceProvider.h"

#include <string>

namespace Ember
{

class FileResourceWrapper: public IResourceWrapper
{
public:
	FileResourceWrapper(std::ifstream& stream);
	virtual ~FileResourceWrapper();

	virtual const char* getDataPtr();
	virtual bool hasData();
	virtual size_t getSize();
private:

	char* mBuffer;
	size_t mSize;
};

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief Loads files relative to a base directory.
 */
class FileResourceProvider: public IResourceProvider
{
public:
	/**
	 * @brief Ctor.
	 * @param baseDirectory The base directory.
	 */
	FileResourceProvider(const std::string& baseDirectory);
	virtual ~FileResourceProvider();

	virtual ResourceWrapper getResource(const std::string& name);

private:
	const std::string mBaseDirectory;
};

}
#endif /* FILERESOURCEPROVIDER_H_ */
