/*
This file is part of Caelum.
See http://www.ogre3d.org/wiki/index.php/Caelum 

Copyright (c) 2006-2007 Caelum team. See Contributors.txt for details.

Caelum is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Caelum is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with Caelum. If not, see <http://www.gnu.org/licenses/>.
*/

#include "CaelumPrecompiled.h"
#include "GeometryFactory.h"

namespace caelum {

void GeometryFactory::generateSphericDome (const Ogre::String &name, int segments, DomeType type) {
	// Return now if already exists
	if (Ogre::MeshManager::getSingleton ().resourceExists (name))
		return;

	LOG ("Creating " + name + " sphere mesh resource...");

	// Use the mesh manager to create the mesh
	Ogre::MeshPtr msh = Ogre::MeshManager::getSingleton ().createManual (name, RESOURCE_GROUP_NAME);
	// Create a submesh
	Ogre::SubMesh *sub = msh->createSubMesh ();

	// Create the shared vertex data
	Ogre::VertexData *vertexData = new Ogre::VertexData ();
	msh->sharedVertexData = vertexData;

	// Define the vertices' format
	Ogre::VertexDeclaration *vertexDecl = vertexData->vertexDeclaration;
	size_t currOffset = 0;
	// Position
	vertexDecl->addElement (0, currOffset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
	currOffset += Ogre::VertexElement::getTypeSize (Ogre::VET_FLOAT3);
	// Normal
	vertexDecl->addElement (0, currOffset, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
	currOffset += Ogre::VertexElement::getTypeSize (Ogre::VET_FLOAT3);
	// Texture coordinates
	vertexDecl->addElement (0, currOffset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, 0);
	currOffset += Ogre::VertexElement::getTypeSize (Ogre::VET_FLOAT2);

	// Allocate the vertex buffer
	switch (type) {
		case DT_GRADIENTS:
			vertexData->vertexCount = segments * (segments - 1) + 2;
			break;
		case DT_STARFIELD:
			vertexData->vertexCount = (segments + 1) * (segments + 1);
			break;
	};
	Ogre::HardwareVertexBufferSharedPtr vBuf = Ogre::HardwareBufferManager::getSingleton ().createVertexBuffer (vertexDecl->getVertexSize (0), vertexData->vertexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
	Ogre::VertexBufferBinding *binding = vertexData->vertexBufferBinding;
	binding->setBinding (0, vBuf);

	float *pVertex = static_cast<float *>(vBuf->lock (Ogre::HardwareBuffer::HBL_DISCARD));

	// Allocate the index buffer
	switch (type) {
		case DT_GRADIENTS:
			sub->indexData->indexCount = 2 * segments * (segments - 1) * 3;
			break;
		case DT_STARFIELD:
			// TODO
			sub->indexData->indexCount = 2 * (segments + 1) * segments * 3;
			break;
	};
	sub->indexData->indexBuffer = Ogre::HardwareBufferManager::getSingleton ().createIndexBuffer (Ogre::HardwareIndexBuffer::IT_16BIT, sub->indexData->indexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
	Ogre::HardwareIndexBufferSharedPtr iBuf = sub->indexData->indexBuffer;
	unsigned short *pIndices = static_cast<unsigned short *>(iBuf->lock (Ogre::HardwareBuffer::HBL_DISCARD));

	// Fill the buffers
	switch (type) {
		case DT_GRADIENTS:
			fillGradientsDomeBuffers (pVertex, pIndices, segments);
			break;
		case DT_STARFIELD:
			fillStarfieldDomeBuffers (pVertex, pIndices, segments);
			break;
	};

	// Close the vertex buffer
	vBuf->unlock ();

	// Close the index buffer
	iBuf->unlock ();

	// Finishing it...
	sub->useSharedVertices = true;
	msh->_setBounds (Ogre::AxisAlignedBox (-1, -1, -1, 1, 1, 1), false);
	msh->_setBoundingSphereRadius (1);
	msh->load ();

	LOG ("DONE");
}

void GeometryFactory::fillGradientsDomeBuffers (float *pVertex, unsigned short *pIndices, int segments) {
	const float deltaLatitude = Ogre::Math::PI / (float )segments;
	const float deltaLongitude = Ogre::Math::PI * 2.0 / (float )segments;

	// Generate the rings
	for (int i = 1; i < segments; i++) {
		float r0 = Ogre::Math::Sin (Ogre::Radian (i * deltaLatitude));
		float y0 = Ogre::Math::Cos (Ogre::Radian (i * deltaLatitude));

		for (int j = 0; j < segments; j++) {
			float x0 = r0 * Ogre::Math::Sin (Ogre::Radian (j * deltaLongitude));
			float z0 = r0 * Ogre::Math::Cos (Ogre::Radian (j * deltaLongitude));

			*pVertex++ = x0;
			*pVertex++ = y0;
			*pVertex++ = z0;

			*pVertex++ = -x0;
			*pVertex++ = -y0;
			*pVertex++ = -z0;

			*pVertex++ = 0;
			*pVertex++ = 1 - y0;
		}
	}

	// Generate the "north pole"
	*pVertex++ = 0;	// Position
	*pVertex++ = 1;
	*pVertex++ = 0;
	*pVertex++ = 0;	// Normal
	*pVertex++ = -1;
	*pVertex++ = 0;
	*pVertex++ = 0;	// UV
	*pVertex++ = 0;

	// Generate the "south pole"
	*pVertex++ = 0;	// Position
	*pVertex++ = -1;
	*pVertex++ = 0;
	*pVertex++ = 0;	// Normal
	*pVertex++ = 1;
	*pVertex++ = 0;
	*pVertex++ = 0;	// UV
	*pVertex++ = 2;

	// Generate the mid segments
	for (int i = 0; i < segments - 2; i++) {
		for (int j = 0; j < segments; j++) {
			*pIndices++ = segments * i + j;
			*pIndices++ = segments * i + (j + 1) % segments;
			*pIndices++ = segments * (i + 1) + (j + 1) % segments;
			*pIndices++ = segments * i + j;
			*pIndices++ = segments * (i + 1) + (j + 1) % segments;
			*pIndices++ = segments * (i + 1) + j;
		}
	}

	// Generate the upper cap
	for (int i = 0; i < segments; i++) {
		*pIndices++ = segments * (segments - 1);
		*pIndices++ = (i + 1) % segments;
		*pIndices++ = i;
	}

	// Generate the lower cap
	for (int i = 0; i < segments; i++) {
		*pIndices++ = segments * (segments - 1) + 1;
		*pIndices++ = segments * (segments - 2) + i;
		*pIndices++ = segments * (segments - 2) + (i + 1) % segments;
	}
}

void GeometryFactory::fillStarfieldDomeBuffers (float *pVertex, unsigned short *pIndices, int segments) {
	const float deltaLatitude = Ogre::Math::PI / (float )segments;
	const float deltaLongitude = Ogre::Math::PI * 2.0 / (float )segments;

	// Generate the rings
	for (int i = 0; i <= segments; i++) {
		float r0 = Ogre::Math::Sin (Ogre::Radian (i * deltaLatitude));
		float y0 = Ogre::Math::Cos (Ogre::Radian (i * deltaLatitude));

		for (int j = 0; j <= segments; j++) {
			float x0 = r0 * Ogre::Math::Sin (Ogre::Radian (j * deltaLongitude));
			float z0 = r0 * Ogre::Math::Cos (Ogre::Radian (j * deltaLongitude));

			*pVertex++ = x0;
			*pVertex++ = y0;
			*pVertex++ = z0;

			*pVertex++ = -x0;
			*pVertex++ = -y0;
			*pVertex++ = -z0;

			*pVertex++ = (float )j / (float )segments;
			*pVertex++ = 1 - (y0 * 0.5 + 0.5);
		}
	}

	// Generate the mid segments
	for (int i = 0; i <= segments; i++) {
		for (int j = 0; j < segments; j++) {
			*pIndices++ = segments * i + j;
			*pIndices++ = segments * i + (j + 1) % segments;
			*pIndices++ = segments * (i + 1) + (j + 1) % segments;
			*pIndices++ = segments * i + j;
			*pIndices++ = segments * (i + 1) + (j + 1) % segments;
			*pIndices++ = segments * (i + 1) + j;
		}
	}
}

} // namespace caelum
