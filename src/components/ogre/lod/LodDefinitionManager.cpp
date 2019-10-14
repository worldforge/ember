/*
 * Copyright (c) 2013 Peter Szucs <peter.szucs.dev@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "LodDefinitionManager.h"
#include "LodManager.h"

namespace Ember {
namespace OgreView {
namespace Lod {

LodDefinitionManager::LodDefinitionManager(const boost::filesystem::path& exportDirectory) :
		mLodDefinitionSerializer(exportDirectory) {
	// MeshManager has a load order of 350, so this should be bigger then that.
	mLoadOrder = 400.0f;
	mResourceType = "LodDefinition";
	mScriptPatterns.push_back("*.loddef");
	Ogre::ResourceGroupManager::getSingleton()._registerScriptLoader(this);
}

LodDefinitionManager::~LodDefinitionManager() {
	Ogre::ResourceGroupManager::getSingleton()._unregisterScriptLoader(this);
}

LodDefinitionPtr LodDefinitionManager::create(const Ogre::String& name, const Ogre::String& group,
											  bool isManual, Ogre::ManualResourceLoader* loader,
											  const Ogre::NameValuePairList* createParams) {
	return Ogre::static_pointer_cast<LodDefinition>(createResource(name, group, isManual, loader, createParams));
}

LodDefinitionPtr LodDefinitionManager::getByName(const Ogre::String& name, const Ogre::String& groupName) {
	return Ogre::static_pointer_cast<LodDefinition>(getResourceByName(name, groupName));
}

Ogre::Resource* LodDefinitionManager::createImpl(const Ogre::String& name,
												 Ogre::ResourceHandle handle,
												 const Ogre::String& group,
												 bool isManual,
												 Ogre::ManualResourceLoader* loader,
												 const Ogre::NameValuePairList* createParams) {
	return OGRE_NEW LodDefinition(this, name, handle, group, isManual, loader);
}

void LodDefinitionManager::parseScript(Ogre::DataStreamPtr& stream, const Ogre::String& groupName) {
	std::string lodName = stream->getName();

	// Extract file name from path.
	size_t start = lodName.find_last_of("/\\");
	if (start != std::string::npos) {
		lodName = lodName.substr(start + 1);
	}
	// Create resource
	LodDefinitionPtr resource = Ogre::static_pointer_cast<LodDefinition>(create(lodName, groupName));

	// Set origin of resource.
	resource->_notifyOrigin(stream->getName());

	mLodDefinitionSerializer.importLodDefinition(stream, *resource.get());
}

void LodDefinitionManager::exportScript(std::string meshName, LodDefinitionPtr definition) {
	std::string lodName = LodManager::getSingleton().convertMeshNameToLodName(meshName);
	mLodDefinitionSerializer.exportScript(definition, lodName);
}

}
}
}
