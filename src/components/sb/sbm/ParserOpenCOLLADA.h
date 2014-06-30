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

#ifndef _PARSER_OPENCOLLADA_H_
#define _PARSER_OPENCOLLADA_H_

#include <fstream>
#include <stdio.h>
#include "sbm/xercesc_utils.hpp"
#include "sk/sk_skeleton.h"
#include "sk/sk_motion.h"
#include <sb/SBJoint.h>
#include <sr/sr_material.h>

#include "sbm/gwiz_math.h"

typedef std::map<std::string, std::vector<SrVec> > VecListMap;
class DeformableMesh;

struct ColladaFloatArray
{
	std::vector<float> floatArray;
	std::string accessorParam;
	int stride;	 
};

struct ColladaSampler
{
	std::string inputName;
	std::string outputName;
};

struct ColladChannel
{
	std::string sourceName;
	std::string targetJointName;
	std::string targetType;
};

class ParserOpenCOLLADA
{
	public:
		static DOMNode* getNode(const std::string& nodeName, DOMNode* node);
		static DOMNode* getNode(const std::string& nodeName, DOMNode* node, int curDepth, int maximumDepth);
		static std::string getNodeAttributeString(DOMNode* node, XMLCh* attrName);
		static int         getNodeAttributeInt(DOMNode* node, XMLCh* attrName);

		// parse from files
		static bool parse(SkSkeleton& skeleton, SkMotion& motion, std::string fileName, float scale, bool doParseSkeleton, bool doParseMotion);

		// parse nodes
		static void parseLibraryVisualScenes(DOMNode* node, SkSkeleton& skeleton, SkMotion& motion, float scale, int& order, std::map<std::string, std::string>& materialId2Name);
		static void parseJoints(DOMNode* node, SkSkeleton& skeleton, SkMotion& motion, float scale, int& order, std::map<std::string, std::string>& materialId2Name, SkJoint* parent = NULL, bool hasRootJoint = false);
		static void parseLibraryAnimations(DOMNode* node, SkSkeleton& skeleton, SkMotion& motion, float scale, int& order, bool zaxis = false);

		static void parseNodeAnimation(DOMNode* node1, std::map<std::string, ColladaFloatArray > &floatArrayMap, float scale, std::map<std::string, ColladaSampler > &samplerMap, std::vector<ColladChannel> &channelSamplerNameMap, SkSkeleton &skeleton );
		
		static void animationPostProcess(SkSkeleton& skeleton, SkMotion& motion);
		static void animationPostProcessByChannels(SkSkeleton& skeleton, SkMotion& motion, SkChannelArray& channels);



		

	private:
		static int getMotionChannelId(SkChannelArray& channels, const std::string&  sourceName);
		static int getMotionChannelId(SkChannelArray& channels, std::string& jointName, std::string& targetType);
		static std::string tokenize(std::string& str,const std::string& delimiters = " ", int mode = 1);
		static int getRotationOrder(std::vector<std::string> orderVec);
};

#endif
