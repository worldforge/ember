//
// C++ Interface: IResourceProvider
//
// Description: 
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2007
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//

#ifndef EMBERIRESOURCEWRAPPER_H
#define EMBERIRESOURCEWRAPPER_H

#include <memory>
#include <string>
#include <utility>

namespace Ember {

/**
An internal interface to be implemented by data providers.
*/
struct IResourceWrapper {
public:
	virtual ~IResourceWrapper() = default;

	/**
	Gets raw data pointer.
	*/
	virtual const char* getDataPtr() = 0;

	/**
	True if the wrapper has data.
	*/
	virtual bool hasData() = 0;

	/**
	Gets the size of the data.
	*/
	virtual size_t getSize() = 0;
};

/**
A simple wrapper for general data.
A wrapper does not need to contain any data: use the hasData method to check before accessing.
*/
class ResourceWrapper {
public:
	ResourceWrapper(const ResourceWrapper& wrapper) = default;

	ResourceWrapper(IResourceWrapper* internalWrapper, std::string name) :
			mInternalWrapper(internalWrapper),
			mName(std::move(name)) {}

	/**
	Gets raw data pointer.
	*/
	const char* getDataPtr() const;

	/**
	True if the wrapper has data.
	*/
	bool hasData() const;

	/**
	Gets the size of the data.
	*/
	size_t getSize() const;

	/**
	Gets the name of the resource.
	*/
	const std::string& getName() const;

private:
	mutable std::shared_ptr<IResourceWrapper> mInternalWrapper;
	std::string mName;
};

/**
Interface implemented by resource providers.
*/
struct IResourceProvider {
public:
	virtual ~IResourceProvider() = default;

	/**
	Returns a resource by the name.
	*/
	virtual ResourceWrapper getResource(const std::string& name) = 0;
};

inline const char* ResourceWrapper::getDataPtr() const { return mInternalWrapper->getDataPtr(); }

inline bool ResourceWrapper::hasData() const { return mInternalWrapper->hasData(); }

inline size_t ResourceWrapper::getSize() const { return mInternalWrapper->getSize(); }

inline const std::string& ResourceWrapper::getName() const { return mName; }

}
#endif //EMBERIRESOURCEWRAPPER_H
