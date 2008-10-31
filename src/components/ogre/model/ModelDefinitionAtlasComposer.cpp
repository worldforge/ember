//
// C++ Implementation: ModelDefinitionAtlasComposer
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@iteam.se>, (C) 2008
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

#include <Atlas/Message/Element.h>
#include <Atlas/Formatter.h>
#include <Atlas/Objects/Decoder.h>
#include <Atlas/Codecs/XML.h>
#include <Atlas/Message/MEncoder.h>
#include <Atlas/Message/QueuedDecoder.h>
#include <Ogre.h>
#include <wfmath/axisbox.h>
#include <wfmath/atlasconv.h>
#include "../MathConverter.h"
#include "services/config/ConfigService.h"

#include "framework/osdir.h"

#ifdef WIN32
	#include <tchar.h>
	#define snprintf _snprintf
    #include <io.h> // for _access, Win32 version of stat()
    #include <direct.h> // for _mkdir
//	#include <sys/stat.h>
	
	#include <iostream>
	#include <fstream>
	#include <ostream>
#else
	#include <dirent.h>
#endif


using namespace Atlas::Message;
namespace EmberOgre {

namespace Model {

ModelDefinitionAtlasComposer::ModelDefinitionAtlasComposer()
{
}


ModelDefinitionAtlasComposer::~ModelDefinitionAtlasComposer()
{
}

Atlas::Message::MapType ModelDefinitionAtlasComposer::compose(Model* model, const std::string& typeName, const std::string& parentTypeName, float scale)
{
	MapType mainMap;
	if (!model) {
		return mainMap;
	}
	MapType attributesMap;
	
	MapType bboxMap;

	model->getParentSceneNode()->rotate(Ogre::Vector3::UNIT_Y, Ogre::Degree(90));

	Ogre::AxisAlignedBox aabb(model->getWorldBoundingBox(true));
	if (scale != 0 && scale != 1.0f) {
		aabb.scale(Ogre::Vector3(scale, scale, scale));
	}
	WFMath::AxisBox<3> wfmathAabb(Ogre2Atlas(aabb));
	
	bboxMap["default"] = wfmathAabb.toAtlas();
	bboxMap["visibility"] = StringType("public");
	attributesMap["bbox"] = bboxMap;
	mainMap["attributes"] = attributesMap;
	
	mainMap["objtype"] = StringType("class");
	mainMap["id"] = StringType(typeName);
	
	ListType parents;
	parents.push_back(StringType(parentTypeName));
	mainMap["parents"] = parents;
	
	model->getParentSceneNode()->rotate(Ogre::Vector3::UNIT_Y, Ogre::Degree(-90));
	
	
	return mainMap;
}

void ModelDefinitionAtlasComposer::composeToStream(std::iostream& outstream, Model* model, const std::string& typeName, const std::string& parentTypeName, float scale)
{
	Atlas::Message::QueuedDecoder decoder;
	//std::fstream file;
	
	Atlas::Codecs::XML codec(outstream, decoder);
	Atlas::Formatter formatter(outstream, codec);
	Atlas::Message::Encoder encoder(formatter);
	formatter.streamBegin();
	encoder.streamMessageElement(compose(model, typeName, parentTypeName, scale));
		
	formatter.streamEnd();
}

void ModelDefinitionAtlasComposer::composeToFile(Model* model, const std::string& typeName, const std::string& parentTypeName, float scale)
{
	if (model) {
		///make sure the directory exists
		std::string dir(Ember::EmberServices::getSingletonPtr()->getConfigService()->getHomeDirectory() + "/typeexport/");
		
		if (!oslink::directory(dir).isExisting()) {
			S_LOG_INFO("Creating directory " << dir);
#ifdef __WIN32__
			mkdir(dir.c_str());
#else 
			mkdir(dir.c_str(), S_IRWXU);
#endif
		}
		
		const std::string fileName(dir + typeName + ".atlas");
		std::fstream exportFile(fileName.c_str(), std::fstream::out);
	
		S_LOG_INFO("Creating atlas type " << fileName);
		composeToStream(exportFile, model, typeName, parentTypeName, scale);
// 		Ember::ConsoleBackend::getSingletonPtr()->pushMessage(std::string("Creating atlas type ") + fileName);
	}

}


}

}
