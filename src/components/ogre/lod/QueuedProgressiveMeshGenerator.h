/*
 * Copyright (C) 2012 Peter Szucs <peter.szucs.dev@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef QUEUDPROGRESSIVEMESHGENERATOR_H
#define QUEUDPROGRESSIVEMESHGENERATOR_H

#include "LodConfig.h"
#include "ProgressiveMeshGenerator.h"
#include "EmberOgreMesh.h"

#include "components/ogre/EmberOgrePrerequisites.h"
#include "framework/Singleton.h"

#include <OgreProgressiveMesh.h>
#include <OgreWorkQueue.h>
#include <OgreRoot.h>

#include <string>
#include <stack>

namespace Ember
{
namespace OgreView
{
namespace Lod
{

/**
 * @brief Request data structure.
 */
struct PMGenRequest {
	struct VertexBuffer {
		size_t vertexCount;
		Ogre::Vector3* vertexBuffer;
		VertexBuffer() :
			vertexBuffer(0) { }
	};
	struct IndexBuffer {
		size_t indexSize;
		size_t indexCount;
		unsigned char* indexBuffer; // size in bytes = indexSize * indexCount
		IndexBuffer() :
			indexBuffer(0) { }
	};
	struct SubmeshInfo {
		std::vector<IndexBuffer> genIndexBuffers; // order: lodlevel/generated index buffer
		IndexBuffer indexBuffer;
		VertexBuffer vertexBuffer;
		bool useSharedVertexBuffer;
	};
	std::vector<SubmeshInfo> submesh;
	VertexBuffer sharedVertexBuffer;
	LodConfig config;
	~PMGenRequest();
};

/**
 * @brief Processes requests.
 */
class PMWorker :
	private Ogre::WorkQueue::RequestHandler,
	private ProgressiveMeshGenerator
{
public:
	PMWorker();
	virtual ~PMWorker();
private:
	OGRE_AUTO_MUTEX; // Mutex to force processing one mesh at a time.
	PMGenRequest* mRequest; // This is a copy of the current processed request from stack. This is needed to pass it to overloaded functions like bakeLods().

	Ogre::WorkQueue::Response* handleRequest(const Ogre::WorkQueue::Request* req, const Ogre::WorkQueue* srcQ);
	void buildRequest(LodConfig& lodConfigs);
	void tuneContainerSize();
	void initialize();
	void addVertexBuffer(const PMGenRequest::VertexBuffer& vertexBuffer, bool useSharedVertexLookup);
	void addIndexBuffer(PMGenRequest::IndexBuffer& indexBuffer, bool useSharedVertexLookup, unsigned short submeshID);
	void bakeLods();
};

/**
 * @brief Injects the output of a request to the mesh in a thread safe way.
 */
class PMInjector :
	public Ogre::WorkQueue::ResponseHandler,
	public Ogre::FrameListener
{
public:
	PMInjector();
	virtual ~PMInjector();

	/**
	 * @brief It will add every generated Lod to readyLods for injection, because this is not called on main thread.
	 */
	void handleResponse(const Ogre::WorkQueue::Response* res, const Ogre::WorkQueue* srcQ);
protected:
	OGRE_AUTO_MUTEX; // Mutex for readyLods access.

	// TODO: use thread safe circular queue instead of mutex and stack.
	std::stack<PMGenRequest*> readyLods;

	// Overloaded function, called every frame by Ogre on main thread.
	// This will call inject on each ready Lod.
	bool frameStarted(const Ogre::FrameEvent& evt);

	// Copies every generated Lod level to the mesh.
	void inject(PMGenRequest* request);
};

/**
 * @brief Creates a request for the worker. The interface is compatible with ProgressiveMeshGenerator.
 */
class QueuedProgressiveMeshGenerator
{
public:
	void build(LodConfig& lodConfig);
	virtual ~QueuedProgressiveMeshGenerator();
private:
	void copyVertexBuffer(Ogre::VertexData* data, PMGenRequest::VertexBuffer& out);
	void copyIndexBuffer(Ogre::IndexData* data, PMGenRequest::IndexBuffer& out);
	void copyBuffers(Ogre::Mesh* mesh, PMGenRequest* req);
};

}
}
}
#endif // ifndef QUEUDPROGRESSIVEMESHGENERATOR_H
