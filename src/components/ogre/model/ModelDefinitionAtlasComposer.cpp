//
// C++ Implementation: ModelDefinitionAtlasComposer
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2008
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

#include "ModelDefinitionAtlasComposer.h"
#include "Model.h"
#include "SubModel.h"

#include "../Convert.h"
#include "services/config/ConfigService.h"

#include "framework/osdir.h"

#include <Atlas/MultiLineListFormatter.h>
#include <Atlas/Message/Element.h>
#include <Atlas/Objects/Decoder.h>
#include <Atlas/Codecs/XML.h>
#include <Atlas/Message/MEncoder.h>
#include <Atlas/Message/QueuedDecoder.h>
#include <Ogre.h>
#include <wfmath/axisbox.h>
#include <wfmath/atlasconv.h>
#include <wfmath/stream.h>

#ifdef _WIN32
#include "platform/platform_windows.h"
#else
#include <dirent.h>
#endif

using namespace Atlas::Message;
namespace Ember
{
namespace OgreView
{

namespace Model
{

Atlas::Message::MapType ModelDefinitionAtlasComposer::compose(Model* model, const std::string& typeName, const std::string& parentTypeName, float scale, const std::string& collisionType)
{
	MapType mainMap;
	if (!model) {
		return mainMap;
	}
	MapType attributesMap;

	MapType bboxMap;

	model->getParentNode()->rotate(Ogre::Vector3::UNIT_Y, Ogre::Degree(90));

	Ogre::AxisAlignedBox aabb(model->getWorldBoundingBox(true));
	if (scale != .0f && scale != 1.0f) {
		aabb.scale(Ogre::Vector3(scale, scale, scale));
	}
	WFMath::AxisBox<3> wfmathAabb = Convert::toWF(aabb);

	bboxMap["default"] = wfmathAabb.toAtlas();
	bboxMap["visibility"] = StringType("public");
	attributesMap["bbox"] = bboxMap;

	Atlas::Message::MapType geometryMap;
	geometryMap["default"] = composeGeometry(model, collisionType, wfmathAabb);
	geometryMap["visibility"] = "public";
	attributesMap["geometry"] = geometryMap;

	mainMap["attributes"] = attributesMap;

	mainMap["objtype"] = StringType("class");
	mainMap["id"] = StringType(typeName);

	ListType parents;
	parents.push_back(StringType(parentTypeName));
	mainMap["parents"] = parents;

	model->getParentNode()->rotate(Ogre::Vector3::UNIT_Y, Ogre::Degree(-90));

	return mainMap;
}

Atlas::Message::Element ModelDefinitionAtlasComposer::composeGeometry(Model* model, const std::string& collisionType, const WFMath::AxisBox<3>& bbox) const
{
	Atlas::Message::MapType geometryMap;
	geometryMap["shape"] = collisionType;
	if (collisionType == "mesh") {

		std::vector<Atlas::Message::Element> vertices;
		std::vector<Atlas::Message::Element> indices;

		for (SubModel* submodel : model->getSubmodels()) {
			auto mesh = submodel->getEntity()->getMesh();
			if (mesh->sharedVertexData) {
				copyVertexData(vertices, *mesh->sharedVertexData, bbox);
			}
			for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i) {
				Ogre::SubMesh* submesh = mesh->getSubMesh(i);
				size_t offset = 0;
				if (!submesh->useSharedVertices) {
					offset = vertices.size();
					copyVertexData(vertices, *submesh->vertexData, bbox);
				}

				auto indexBuffer = submesh->indexData->indexBuffer;
				unsigned short* pIndex = static_cast<unsigned short*>(indexBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
				for (size_t j = 0; j < indexBuffer->getNumIndexes(); j += 3) {
					indices.push_back(pIndex[j] + (unsigned short)offset);
					indices.push_back(pIndex[j + 1] + (unsigned short)offset);
					indices.push_back(pIndex[j + 2] + (unsigned short)offset);
				}
				indexBuffer->unlock();
			}
		}
		geometryMap["vertices"] = std::move(vertices);
		geometryMap["indices"] = std::move(indices);
	}
	return std::move(geometryMap);
}

void ModelDefinitionAtlasComposer::copyVertexData(std::vector<Atlas::Message::Element>& vertices, Ogre::VertexData& vertexData, const WFMath::AxisBox<3>& bbox) const
{
	// Locate position element and the buffer to go with it.
	const Ogre::VertexElement* posElem = vertexData.vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
	Ogre::HardwareVertexBufferSharedPtr vbuf = vertexData.vertexBufferBinding->getBuffer(posElem->getSource());

	// Lock the buffer for reading.
	unsigned char* pVertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
	int vSize = vbuf->getVertexSize();
	size_t numVertices = vertexData.vertexCount;
	vertices.reserve(numVertices * 3);
	Ogre::Quaternion q(Ogre::Degree(90), Ogre::Vector3::UNIT_Y);

	for (size_t i = 0; i < numVertices; ++i) {

		float* pFloat;
		posElem->baseVertexPointerToElement(pVertex + (i * vSize), &pFloat);

		Ogre::Vector3 v(pFloat[0], pFloat[1], pFloat[2]);

		//Rotate to fit with WF coord space.
		Ogre::Vector3 v2 = q * v;

		WFMath::Point<3> wfPoint = Convert::toWF<WFMath::Point<3>>(v2);

		if (!WFMath::Intersect<3>(bbox, wfPoint, false)) {
			std::stringstream ss;
			ss << "Point " << wfPoint << " is outside bbox " << bbox;
			S_LOG_WARNING(ss.str());
		}

		vertices.push_back(wfPoint.x()); //x in Ogre is X in WF
		vertices.push_back(wfPoint.y()); //z in Ogre is -y in WF
		vertices.push_back(wfPoint.z()); //y in Ogre is z in WF
	}
	vbuf->unlock();
}

void ModelDefinitionAtlasComposer::composeToStream(std::iostream& outstream, Model* model, const std::string& typeName, const std::string& parentTypeName, float scale, const std::string& collisionType)
{
	Atlas::Message::QueuedDecoder decoder;
	//std::fstream file;

	Atlas::Codecs::XML codec(outstream, outstream, decoder);
	Atlas::MultiLineListFormatter formatter(outstream, codec);
	Atlas::Message::Encoder encoder(formatter);
	formatter.streamBegin();
	encoder.streamMessageElement(compose(model, typeName, parentTypeName, scale, collisionType));

	formatter.streamEnd();
}

std::string ModelDefinitionAtlasComposer::composeToFile(Model* model, const std::string& typeName, const std::string& parentTypeName, float scale, const std::string& collisionType)
{
	if (model) {
		try {
			//make sure the directory exists
			std::string dir(EmberServices::getSingleton().getConfigService().getHomeDirectory(BaseDirType_DATA) + "typeexport/");

			if (!oslink::directory(dir).isExisting()) {
				S_LOG_INFO("Creating directory " << dir);
				oslink::directory::mkdir(dir.c_str());
			}

			const std::string fileName(dir + typeName + ".atlas");
			std::fstream exportFile(fileName.c_str(), std::fstream::out);

			S_LOG_INFO("Creating atlas type " << fileName);
			composeToStream(exportFile, model, typeName, parentTypeName, scale, collisionType);
			// 		ConsoleBackend::getSingletonPtr()->pushMessage(std::string("Creating atlas type ") + fileName, "info");
			return fileName;
		} catch (const std::exception& e) {
			S_LOG_WARNING("Error when exporting Model to Atlas data." << e);
		}
	}
	return "";

}

}

}
}
