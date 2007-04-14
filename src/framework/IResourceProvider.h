//
// C++ Interface: IResourceProvider
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@iteam.se>, (C) 2007
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
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//


namespace Ember {

/**
An internal interface to be implemented by data providers.
*/
class IResourceWrapper
{
public:
	virtual ~IResourceWrapper() {}
	
	/**
	Gets raw data pointer.
	*/
	virtual char* getDataPtr() = 0;
	
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
class ResourceWrapper
{
public:
	ResourceWrapper(const ResourceWrapper& wrapper) : mInternalWrapper(wrapper.mInternalWrapper), mName(wrapper.mName) { }
	ResourceWrapper(IResourceWrapper* internalWrapper, const std::string& name) : mInternalWrapper(internalWrapper), mName(name) {}
	
	/**
	Gets raw data pointer.
	*/
	inline char* getDataPtr();
	/**
	True if the wrapper has data.
	*/
	inline bool hasData();
	/**
	Gets the size of the data.
	*/	
	inline size_t getSize();
	/**
	Gets the name of the resource.
	*/	
	inline const std::string& getName();

private:
	mutable std::auto_ptr<IResourceWrapper> mInternalWrapper;
	std::string mName;
};

/**
Interface implemented by resource providers.
*/
class IResourceProvider
{
public:
	virtual ~IResourceProvider() {}
	
	/**
	Returns a resource by the name.
	*/
	virtual ResourceWrapper getResource(const std::string& name) = 0;
};

char* ResourceWrapper::getDataPtr() { return mInternalWrapper->getDataPtr();}
bool ResourceWrapper::hasData() { return mInternalWrapper->hasData();}
size_t ResourceWrapper::getSize() { return mInternalWrapper->getSize();}
const std::string& ResourceWrapper::getName() {return mName;}

}
