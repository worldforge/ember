//
// C++ Implementation: MeshSerializerListener
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2009
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "MeshSerializerListener.h"
#include "components/ogre/lod/LodManager.h"

namespace Ember {
namespace OgreView {

MeshSerializerListener::MeshSerializerListener(bool requireTangents)
: mRequireTangents(true)
{
}


MeshSerializerListener::~MeshSerializerListener()
{
}

void MeshSerializerListener::processMaterialName(Ogre::Mesh* mesh, Ogre::String* name)
{
	
}

void MeshSerializerListener::processSkeletonName(Ogre::Mesh *mesh, Ogre::String *name)
{
	//If the skeleton path starts with ".." it's a relative path and we need to use the path of the current mesh to create a new skeleton path
	if (Ogre::StringUtil::startsWith(*name, "../") || Ogre::StringUtil::startsWith(*name, "./")) {
		std::string meshPath = mesh->getName();
		std::string path;
		std::string baseName;
		Ogre::StringUtil::splitFilename(meshPath, baseName, path);
		
		Ogre::vector<std::string>::type skeletonPathSegments = Ogre::StringUtil::split(*name, "/");
		Ogre::vector<std::string>::type pathSegments = Ogre::StringUtil::split(path, "/");
		
		//For every ".." part in the skeleton path, walk upwards in the directory hierarcy
		while (*skeletonPathSegments.begin() == ".." || *skeletonPathSegments.begin() == ".") {
			if (*skeletonPathSegments.begin() == "..") {
				pathSegments.pop_back();
			}
			skeletonPathSegments.erase(skeletonPathSegments.begin());
		}
		
		std::stringstream ss;
		for (Ogre::vector<std::string>::type::const_iterator I = pathSegments.begin(); I != pathSegments.end(); ++I) {
			if (I != pathSegments.begin()) {
				ss << "/";
			}
			ss << *I;
		}
		for (Ogre::vector<std::string>::type::const_iterator I = skeletonPathSegments.begin(); I != skeletonPathSegments.end(); ++I) {
			ss << "/" << *I;
		}
		name->assign(ss.str());
	}
}

void MeshSerializerListener::processMeshCompleted(Ogre::Mesh* mesh)
{
	if (mRequireTangents) {
		//Ḿake sure that the mesh has tangents. This takes time, but is ok during development.
		unsigned short outSourceCoordSet;
		unsigned short outIndex;
		try {
			if (!mesh->suggestTangentVectorBuildParams(Ogre::VES_TANGENT, outSourceCoordSet, outIndex)) {
		#if DEBUG
				S_LOG_VERBOSE("No tangents available for " << mesh->getName() << " mesh; generating new ones now.");
		#else
				S_LOG_WARNING("No tangents available for " << mesh->getName() << " mesh; generating new ones now. You should instead make sure that all meshes have tangents pregenerated.");
		#endif
				mesh->buildTangentVectors(Ogre::VES_TANGENT, outSourceCoordSet, outIndex);
			}

		} catch (const Ogre::Exception& e) {
			//Just swallow exceptions, since this just means that we couldn't generate any tangents.
		}
	}
}

}
}
