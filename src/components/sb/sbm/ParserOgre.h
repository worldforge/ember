/*
 *  ParserOpenCOLLADA.h - part of Motion Engine and SmartBody-lib
 *  Copyright (C) 2008  University of Southern California
 *
 *  SmartBody-lib is free software: you can redistribute it and/or
 *  modify it under the terms of the Lesser GNU General Public License
 *  as published by the Free Software Foundation, version 3 of the
 *  license.
 *
 *  SmartBody-lib is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Lesser GNU General Public License for more details.
 *
 *  You should have received a copy of the Lesser GNU General Public
 *  License along with SmartBody-lib.  If not, see:
 *      http://www.gnu.org/licenses/lgpl-3.0.txt
 *
 *  CONTRIBUTORS:
 *      Yuyu Xu, USC
 */

#ifndef _PARSER_OGRRESKELETON_H_
#define _PARSER_OGRRESKELETON_H_

#include <fstream>
#include "sbm/sbm_deformable_mesh.h"
#include "sbm/xercesc_utils.hpp"
#include "sk/sk_skeleton.h"
#include "sk/sk_motion.h"

#include "sbm/gwiz_math.h"

namespace SmartBody {

	class SBMotion;
	class SBSkeleton;
}

class ParserOgre
{
	public:
		static bool parse(SmartBody::SBSkeleton& skeleton, std::vector<SmartBody::SBMotion*>& motions, std::string fileName, float scale, bool doParseSkeleton, bool doParseMotion);		
		static bool parseSkinMesh(std::vector<SrModel*>& meshModelVec, std::vector<SkinWeight*>& skinWeights, std::string filename, std::string& skeletonName, float scale, bool doParseMesh, bool doParseSkinWeight);
		static DOMNode* getNode(const std::string& nodeName, DOMNode* node);

		static bool parseSkeleton(DOMNode* skeletonNode, SmartBody::SBSkeleton& skeleton, std::string fileName, float scale);
		static bool parseMotion(DOMNode* motionNode, std::vector<SmartBody::SBMotion*>& motions, SmartBody::SBMotion* motion,std::string fileName, float scale);
		static bool parseMesh(DOMNode* meshNode, std::vector<SrModel*>& meshModelVec, float scaleFactor);
		static bool parseSkinWeight(DOMNode* node, std::vector<SkinWeight*>& skinWeights, float scaleFactor);
		static bool parseMeshMaterial(std::vector<SrModel*>& meshModelVec, std::string materialFilePath);
			
		static bool exportOgreXMLMesh(DeformableMesh* mesh, std::string meshName, std::string outPathName);
protected:
		/*static void loadTexture(int type, std::string texFileName, const SrStringArray& paths);*/
		static void loadMeshMaterial(std::vector<SrModel*>& meshModelVec, std::string materialFileName, std::string materialFilePath);

};

#endif
