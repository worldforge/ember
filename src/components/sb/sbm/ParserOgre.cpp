/*
 *  ParserOgre.cpp - part of Motion Engine and SmartBody-lib
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
 */

#include "ParserOgre.h"
#include "sr/sr_euler.h"
#include <boost/version.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/algorithm/string.hpp>    
#include <boost/lexical_cast.hpp>    
#include <algorithm>
#include <map>
#include <cctype>
#include <string>
#include <fstream>
#include <stdio.h>
#include <sbm/BMLDefs.h>
#include <sb/SBMotion.h>
#include <sb/SBSkeleton.h>
#include <sb/SBTypes.h>
#include <sb/SBScene.h>

/*
#if !defined (__ANDROID__) && !defined(SB_IPHONE)
#include <sbm/GPU/SbmTexture.h>
#endif */


bool ParserOgre::parseSkinMesh( std::vector<SrModel*>& meshModelVec, std::vector<SkinWeight*>& skinWeights, std::string pathName, std::string& skeletonName, float scale, bool doParseMesh, bool doParseSkinWeight )
{
	try 
	{
		XMLPlatformUtils::Initialize();
	}
	catch (const XMLException& toCatch) 
	{
		std::string message = "";
		xml_utils::xml_translate(&message, toCatch.getMessage());
		LOG("Error during ParserOgre initialization! :\n %s\n", message.c_str());
		return false;
	}

	XercesDOMParser* parser = new XercesDOMParser();
	parser->setValidationScheme(XercesDOMParser::Val_Always);
	parser->setDoNamespaces(true);    // optional

	ErrorHandler* errHandler = (ErrorHandler*) new HandlerBase();
	parser->setErrorHandler(errHandler);

	bool parseOk = false;

	try 
	{
		std::string filebasename = boost::filesystem::basename(pathName);
		std::string fileextension = boost::filesystem::extension(pathName);
		boost::filesystem::path filepath(pathName);
		//LOG("directory string = %s",filepath.parent_path().directory_string().c_str());
		//std::string filepath = boost::filesystem::
		std::stringstream strstr;
		if (fileextension.size() > 0 && fileextension[0] == '.')
			strstr << filebasename << fileextension;
		else
			strstr << filebasename << "." << fileextension;
		parser->parse(pathName.c_str());
		DOMDocument* doc = parser->getDocument();

		if (doParseMesh)
		{
			DOMNode* meshNode = getNode("mesh", doc);
			if (!meshNode)
			{
				// is this a COLLADA file? 
				DOMNode* colladaNode = getNode("library_geometry", doc);
				if (colladaNode)
				{
					LOG("File is a COLLADA file, not an Ogre file. Please use a .dae extension.");
					return false;
				}
				LOG("<mesh> was not found in file %s.", pathName.c_str());
				return false;
			}
			parseOk = parseMesh(meshNode, meshModelVec, scale);
			if (parseOk)
			{
				parseMeshMaterial(meshModelVec,filepath.parent_path().string());
			}
			for (unsigned int i=0;i<meshModelVec.size();i++)
				meshModelVec[i]->validate();
		}


		if (doParseSkinWeight)
		{
			// find the skeleton
			DOMNode* skeletonLinkNode = getNode("skeletonlink", doc);
			if (skeletonLinkNode)
			{
				DOMNamedNodeMap* childAttr = skeletonLinkNode->getAttributes();
				const DOMNode* nameNode = childAttr->getNamedItem(BML::BMLDefs::ATTR_NAME);
				std::string nameAttr = "";
				if (nameNode)
				{
					xml_utils::xml_translate(&nameAttr, nameNode->getNodeValue());
					// if the reference is to a .skeleton file, transform it into a .skeleton.xml file
					if (nameAttr.find(".skeleton") == nameAttr.size() - 9)
					{
						nameAttr.append(".xml");
					}
					skeletonName = nameAttr;
				}
			}

			DOMNode* meshNode = getNode("mesh", doc);
			if (!meshNode)
			{
				// is this a COLLADA file? 
				DOMNode* colladaNode = getNode("library_controller", doc);
				if (colladaNode)
				{
					LOG("File is a COLLADA file, not an Ogre file. Please use a .dae extension.");
					return false;
				}
				LOG("<mesh> was not found in file %s. No skinweights will be loaded.", pathName.c_str());
				return false;
			}			
			parseOk =  parseSkinWeight(meshNode, skinWeights, scale);
		}

		return parseOk;

	}
	catch (const XMLException& toCatch) 
	{
		std::string message = "";
		xml_utils::xml_translate(&message, toCatch.getMessage());
		std::cout << "Exception message is: \n" << message << "\n";
		return false;
	}
	catch (const DOMException& toCatch) {
		std::string message = "";
		xml_utils::xml_translate(&message, toCatch.msg);
		std::cout << "Exception message is: \n" << message << "\n";
		return false;
	}
	catch (...) {
		LOG("Unexpected Exception in ParserOgreSkeleton::parse()");
		return false;
	}

	delete parser;
	delete errHandler;
	return true;

}


bool ParserOgre::parse(SmartBody::SBSkeleton& skeleton, std::vector<SmartBody::SBMotion*>& motions, std::string pathName, float scale, bool doParseSkeleton, bool doParseMotion)
{
	try 
	{
		XMLPlatformUtils::Initialize();
	}
	catch (const XMLException& toCatch) 
	{
		std::string message = "";
		xml_utils::xml_translate(&message, toCatch.getMessage());
		LOG("Error during ParserOgre initialization! :\n %s\n", message.c_str());
		return false;
	}

	XercesDOMParser* parser = new XercesDOMParser();
	parser->setValidationScheme(XercesDOMParser::Val_Always);
	parser->setDoNamespaces(true);    // optional

	ErrorHandler* errHandler = (ErrorHandler*) new HandlerBase();
	parser->setErrorHandler(errHandler);

	bool parseOk = false;

	try 
	{
		std::string filebasename = boost::filesystem::basename(pathName);
		std::string fileextension = boost::filesystem::extension(pathName);
		std::stringstream strstr;
		if (fileextension.size() > 0 && fileextension[0] == '.')
			strstr << filebasename << fileextension;
		else
			strstr << filebasename << "." << fileextension;
		skeleton.setName(strstr.str().c_str());
		parser->parse(pathName.c_str());
		DOMDocument* doc = parser->getDocument();

		if (doParseSkeleton)
		{
			DOMNode* skeletonNode = getNode("skeleton", doc);
			bool foundSkeleton = false;
			if (!skeletonNode)
			{
				DOMNode* skeletonLinkNode = getNode("skeletonlink", doc);
				if (skeletonLinkNode)
				{
					DOMNamedNodeMap* childAttr = skeletonLinkNode->getAttributes();
					const DOMNode* nameNode = childAttr->getNamedItem(BML::BMLDefs::ATTR_NAME);
					std::string nameAttr = "";
					if (nameNode)
					{
						xml_utils::xml_translate(&nameAttr, nameNode->getNodeValue());
						// if the reference is to a .skeleton file, transform it into a .skeleton.xml file
						if (nameAttr.find(".skeleton") == nameAttr.size() - 9)
						{
							nameAttr.append(".xml");
						}
						// make sure that file exists
						if (boost::filesystem::exists(nameAttr))
						{
							delete parser;
							parser = new XercesDOMParser();
							XercesDOMParser* parser = new XercesDOMParser();
							parser->setValidationScheme(XercesDOMParser::Val_Always);
							parser->setDoNamespaces(true);    // optional

							ErrorHandler* errHandler = (ErrorHandler*) new HandlerBase();
							parser->setErrorHandler(errHandler);
							parser->parse(nameAttr.c_str());
							DOMDocument* doc = parser->getDocument();
							skeletonNode = getNode("skeleton", doc);	
							if (skeletonNode)
								foundSkeleton = true;
						}
						else
						{
							LOG("Skeleton linked in file %s, which does not exist", nameAttr.c_str());
						}	
					}

				}
				if (!foundSkeleton)
				{
					// is this a COLLADA file? 
					DOMNode* colladaNode = getNode("library_visual_scenes", doc);
					if (colladaNode)
					{
						LOG("File is a COLLADA file, not an Ogre file. Please use a .dae extension.");
						return false;
					}
					LOG("<skeleton> was not found in file %s.", pathName.c_str());
					return false;
				}
			}
			parseOk = parseSkeleton(skeletonNode, skeleton, pathName, scale);
		}

			
		if (doParseMotion)
		{
			DOMNode* animations = getNode("animations", doc);
			if (!animations)
			{
				// is this a COLLADA file? 
				DOMNode* colladaNode = getNode("library_animations", doc);
				if (colladaNode)
				{
					LOG("File is a COLLADA file, not an Ogre file. Please use a .dae extension.");
					return false;
				}
				LOG("<animations> was not found in file %s. No motions will be loaded.", pathName.c_str());
				return false;
			}
			SmartBody::SBMotion* motion = new SmartBody::SBMotion(""); 
			motions.push_back(motion);
			parseOk =  parseMotion(animations, motions, motion, pathName, scale);
		}

		return parseOk;

	}
	catch (const XMLException& toCatch) 
	{
		std::string message = "";
		xml_utils::xml_translate(&message, toCatch.getMessage());
		std::cout << "Exception message is: \n" << message << "\n";
		return false;
	}
	catch (const DOMException& toCatch) {
		std::string message = "";
		xml_utils::xml_translate(&message, toCatch.msg);
		std::cout << "Exception message is: \n" << message << "\n";
		return false;
	}
	catch (...) {
		LOG("Unexpected Exception in ParserOgreSkeleton::parse()");
		return false;
	}

	delete parser;
	delete errHandler;
	return true;
}

DOMNode* ParserOgre::getNode(const std::string& nodeName, DOMNode* node)
{
	int type = node->getNodeType();
	std::string name;
	xml_utils::xml_translate(&name, node->getNodeName());
	std::string value;
	xml_utils::xml_translate(&value, node->getNodeValue());
	if (name == nodeName && node->getNodeType() ==  DOMNode::ELEMENT_NODE)
		return node;

	DOMNode* child = NULL;
	const DOMNodeList* list = node->getChildNodes();
	for (unsigned int c = 0; c < list->getLength(); c++)
	{
		child = getNode(nodeName, list->item(c));
		if (child)
			break;
	}
	return child;
}

bool ParserOgre::parseSkeleton(DOMNode* skeletonNode, SmartBody::SBSkeleton& skeleton, std::string pathName, float scale)
{
	// get the bone hierarchy
	//LOG("Start Parse Ogre Skeleton");
	DOMNode* hierarchy = getNode("bonehierarchy", skeletonNode);
	if (!hierarchy)
	{
		LOG("<hierarchy> was not found in file %s.", pathName.c_str());
		return false;
	}

	std::map<std::string, std::string> parentHierarchy;

	DOMNode* child = NULL;
	const DOMNodeList* list = hierarchy->getChildNodes();
	for (unsigned int i = 0; i < list->getLength(); i++)
	{
		child = list->item(i);
		DOMNamedNodeMap* childAttr = child->getAttributes();
		if (childAttr)
		{
			const DOMNode* boneNode = childAttr->getNamedItem(BML::BMLDefs::OGRE_BONE);
			std::string boneAttr = "";
			if (boneNode)
				xml_utils::xml_translate(&boneAttr, boneNode->getNodeValue());
			const DOMNode* parentNode = childAttr->getNamedItem(BML::BMLDefs::OGRE_PARENT);
			std::string parentAttr = "";
			if (parentNode)
				xml_utils::xml_translate(&parentAttr, parentNode->getNodeValue());
			//LOG("bone node = %s, parent node = %s",boneAttr.c_str(),parentAttr.c_str());
			if (boneNode && parentNode)
				parentHierarchy.insert(std::pair<std::string, std::string>(boneAttr, parentAttr));
		}
			
	}

	// get the bones
	DOMNode* bones = getNode("bones", skeletonNode);
	if (!hierarchy)
	{
		LOG("<bones> was not found in file %s.", pathName.c_str());
		return false;
	}
	const DOMNodeList* boneList = bones->getChildNodes();
	for (unsigned int i = 0; i < boneList->getLength(); i++)
	{
		child = boneList->item(i);
		DOMNamedNodeMap* childAttr = child->getAttributes();
		if (childAttr)
		{
			const DOMNode* idNode = childAttr->getNamedItem(BML::BMLDefs::OGRE_ID);
			std::string idAttr = "";
			if (idNode)
				xml_utils::xml_translate(&idAttr, idNode->getNodeValue());
			const DOMNode* nameNode = childAttr->getNamedItem(BML::BMLDefs::OGRE_NAME);
			std::string nameAttr = "";
			if (nameNode)
				xml_utils::xml_translate(&nameAttr, nameNode->getNodeValue());

			if (idNode && nameNode)
			{
				int parentId = skeleton.joints().size() + 10;
				
				/*
				std::map<std::string, std::string>::iterator iter = parentHierarchy.find(nameAttr);
				if (iter != parentHierarchy.end())
				{
					const std::string& parent = (*iter).second;
					SkJoint* parentJoint = skeleton.linear_search_joint(parent.c_str());
					if (!parentJoint)
					{
						LOG("Parent joint %s to joint %s was not found in file %s.", parent.c_str(), nameAttr.c_str(), pathName.c_str());
					}
					else
					{
						parentId = parentJoint->index();
					}
				}
				*/			

				if (skeleton.joints().size() == 0)
					parentId = -1;
				SkJoint* joint = skeleton.add_joint(SkJoint::TypeQuat, parentId);
				joint->quat()->activate();
				joint->name(nameAttr);
				joint->extName(nameAttr);
				joint->extID(idAttr);
				joint->extSID(idAttr);

				skeleton.channels().add(joint->jointName(), SkChannel::XPos);
				skeleton.channels().add(joint->jointName(), SkChannel::YPos);
				skeleton.channels().add(joint->jointName(), SkChannel::ZPos);
				joint->pos()->limits(SkVecLimits::X, false);
				joint->pos()->limits(SkVecLimits::Y, false);
				joint->pos()->limits(SkVecLimits::Z, false);
				skeleton.channels().add(joint->jointName(), SkChannel::Quat);
				joint->quat()->activate();
				float rotx = 0.0f;
				float roty = 0.0f;
				float rotz = 0.0f;
				float jorientx = 0.0f;
				float jorienty = 0.0f;
				float jorientz = 0.0f;
				SrVec offset;

				const DOMNodeList* transformList = child->getChildNodes();
				for (unsigned int j = 0; j < transformList->getLength(); j++)
				{
					DOMNode* transformNode = transformList->item(j);
					std::string transformNodeName;
					xml_utils::xml_translate(&transformNodeName, transformNode->getNodeName());
					if (transformNodeName == "position")
					{
						DOMNamedNodeMap* positionAttr = transformNode->getAttributes();
						if (positionAttr)
						{
							const DOMNode* xNode = positionAttr->getNamedItem(BML::BMLDefs::OGRE_X);
							std::string xAttr = "";
							if (xNode)
								xml_utils::xml_translate(&xAttr, xNode->getNodeValue());
							offset.x = (float) atof(xAttr.c_str());
							const DOMNode* yNode = positionAttr->getNamedItem(BML::BMLDefs::OGRE_Y);
							std::string yAttr = "";
							if (yNode)
								xml_utils::xml_translate(&yAttr, yNode->getNodeValue());
							offset.y = (float) atof(yAttr.c_str());
							const DOMNode* zNode = positionAttr->getNamedItem(BML::BMLDefs::OGRE_Z);
							std::string zAttr = "";
							if (zNode)
								xml_utils::xml_translate(&zAttr, zNode->getNodeValue());
							offset.z = (float) atof(zAttr.c_str());
							offset *= scale;
							joint->offset(offset);
						}
					}
					else if (transformNodeName == "rotation")
					{
						float angle;
						DOMNamedNodeMap* rotationAttr = transformNode->getAttributes();
						if (rotationAttr)
						{					
							const DOMNode* angleNode = rotationAttr->getNamedItem(BML::BMLDefs::OGRE_ANGLE);
							std::string angleAttr = "";
							if (angleNode)
								xml_utils::xml_translate(&angleAttr, angleNode->getNodeValue());
							angle = (float) atof(angleAttr.c_str());
						}
						const DOMNodeList* angleChildList = transformNode->getChildNodes();
						for (unsigned int k = 0; k < angleChildList->getLength(); k++)
						{
							DOMNode* angleChildNode = angleChildList->item(k);
							std::string angleChildNodeName;
							xml_utils::xml_translate(&angleChildNodeName, angleChildNode->getNodeName());
							if (angleChildNodeName == "axis")
							{
								DOMNamedNodeMap* axisAttr = angleChildNode->getAttributes();
								if (axisAttr)
								{
									SrVec axis;
									const DOMNode* xNode = axisAttr->getNamedItem(BML::BMLDefs::OGRE_X);
									std::string xAttr = "";
									if (xNode)
										xml_utils::xml_translate(&xAttr, xNode->getNodeValue());
									axis.x = (float) atof(xAttr.c_str());
									const DOMNode* yNode = axisAttr->getNamedItem(BML::BMLDefs::OGRE_Y);
									std::string yAttr = "";
									if (yNode)
										xml_utils::xml_translate(&yAttr, yNode->getNodeValue());
									axis.y = (float) atof(yAttr.c_str());
									const DOMNode* zNode = axisAttr->getNamedItem(BML::BMLDefs::OGRE_Z);
									std::string zAttr = "";
									if (zNode)
										xml_utils::xml_translate(&zAttr, zNode->getNodeValue());
									axis.z = (float) atof(zAttr.c_str());
									//LOG("axis = %f %f %f, angle = %f",axis.x,axis.y,axis.z,angle);
									SrQuat orientation(axis, angle);	
									SkJointQuat* jointQuat = joint->quat();
									jointQuat->prerot(orientation);
								}
							}
						}
					}
				}
			}	
		}
	}

	SkJoint* rootJoint = NULL;
	for (unsigned int i=0;i<skeleton.joints().size();i++)
	{
		SkJoint* joint = skeleton.joints()[i];
		std::string jointName = joint->jointName();
		std::map<std::string, std::string>::iterator iter = parentHierarchy.find(jointName);
		if (iter != parentHierarchy.end())
		{
			const std::string& parent = (*iter).second;
			SkJoint* parentJoint = skeleton.linear_search_joint(parent.c_str());
			if (!parentJoint)
			{
				LOG("Parent joint %s to joint %s was not found in file %s.", parent.c_str(), jointName.c_str(), pathName.c_str());
			}
			else
			{
				parentJoint->add_child(joint);
			}
		}
		else // root joint
		{
			//LOG("root joint = %s",joint->name().c_str());
			rootJoint = joint;
			skeleton.root(rootJoint);
		}
	}	

	skeleton.updateGlobalMatricesZero();

	return true;
}


bool ParserOgre::parseMotion(DOMNode* animationsNode, std::vector<SmartBody::SBMotion*>& motions, SmartBody::SBMotion* motion, std::string pathName, float scale)
{
	// many animations might be present. for now, only parse one of them
	DOMNode* animationNode = NULL;
	const DOMNodeList* animationList = animationsNode->getChildNodes();
	bool hasAnimation = false;
	for (unsigned int i = 0; i < animationList->getLength(); i++)
	{
		animationNode = animationList->item(i);
		std::string animationName;
		xml_utils::xml_translate(&animationName, animationNode->getNodeName());
		if (animationName == "animation")
		{
			if (hasAnimation)
			{
				SmartBody::SBMotion* motion2 = new SmartBody::SBMotion();
				motions.push_back(motion2);
				motion = motion2;
			}
			hasAnimation = true;

			// perform animation parsing in two passes. Ogre motion data does not require
			// a fixed number of frames for each channel of data, so this data must be
			// converted first.
			// first pass = establish all keytimes
			// second pass = interpolate and fill in data

			int numKeys = 0;
			std::set<float> times;
			const DOMNodeList* firstPassAnimationChildren = animationNode->getChildNodes();
			for (unsigned int a = 0; a < firstPassAnimationChildren->getLength(); a++)
			{
				DOMNode* firstPassTracksNode = firstPassAnimationChildren->item(a);
				std::string firstPassTracksNodeStr;
				xml_utils::xml_translate(&firstPassTracksNodeStr, firstPassTracksNode->getNodeName());
				if (firstPassTracksNodeStr == "tracks")
				{
					const DOMNodeList* firstPassTracksNodeChildren = firstPassTracksNode->getChildNodes();
					for (unsigned int b = 0; b < firstPassTracksNodeChildren->getLength(); b++)
					{
						DOMNode* firstPassTrackNode = firstPassTracksNodeChildren->item(b);
						std::string firstPassTrackNodeStr;
						xml_utils::xml_translate(&firstPassTrackNodeStr, firstPassTrackNode->getNodeName());
						if (firstPassTrackNodeStr == "track")
						{
							const DOMNodeList* firstPassTrackNodeChildren = firstPassTrackNode->getChildNodes();
							for (unsigned int c = 0; c < firstPassTrackNodeChildren->getLength(); c++)
							{
								DOMNode* firstPassKeyframesNode = firstPassTrackNodeChildren->item(c);
								std::string firstPassKeyframesNodeStr;
								xml_utils::xml_translate(&firstPassKeyframesNodeStr, firstPassKeyframesNode->getNodeName());
								if (firstPassKeyframesNodeStr == "keyframes")
								{
									const DOMNodeList* firstPassKeyframesChildren = firstPassKeyframesNode->getChildNodes();
									for (unsigned int d = 0; d < firstPassKeyframesChildren->getLength(); d++)
									{
										DOMNode* firstPassKeyframeNode = firstPassKeyframesChildren->item(d);
										std::string firstPassKeyframeNodeStr;
										xml_utils::xml_translate(&firstPassKeyframeNodeStr, firstPassKeyframeNode->getNodeName());
										if (firstPassKeyframeNodeStr == "keyframe")
										{
											numKeys++;
											DOMNamedNodeMap* keyframeAttributeList = firstPassKeyframeNode->getAttributes();
											if (keyframeAttributeList)
											{
												const DOMNode* timeNode = keyframeAttributeList->getNamedItem(BML::BMLDefs::OGRE_TIME);
												std::string timeStr;
												xml_utils::xml_translate(&timeStr, timeNode->getNodeValue());
												float time = (float) atof(timeStr.c_str());
												times.insert(time);
											}
										}
									}
								}
							}
						}
					}
				}
			}
			std::vector<float> allTimes;
			for (std::set<float>::iterator iter = times.begin(); iter != times.end(); iter++)
				allTimes.push_back(*iter);
			
			std::sort(allTimes.begin(), allTimes.end());

			int numKeyFrames = allTimes.size();
			std::vector<float*> allData;

			int counter = 0;
			std::map<float, int> timeMap;
			for (std::vector<float>::iterator iter = allTimes.begin();
				 iter != allTimes.end();
				 iter++)
			{
				timeMap.insert(std::pair<float, int>((*iter), counter));
				counter++;
			}

			SkChannelArray motionChannels;

			// second pass, parsing the data
			DOMNamedNodeMap* animationAttributeList = animationNode->getAttributes();
			if (animationAttributeList)
			{
				const DOMNode* nameNode = animationAttributeList->getNamedItem(BML::BMLDefs::OGRE_NAME);
				std::string animationName;
				xml_utils::xml_translate(&animationName, nameNode->getNodeValue());
				motion->setName(animationName);
				const DOMNode* lengthNode = animationAttributeList->getNamedItem(BML::BMLDefs::OGRE_LENGTH);
				std::string lengthStr;
				xml_utils::xml_translate(&lengthStr, lengthNode->getNodeValue());
				float length = (float) atof(lengthStr.c_str());

				const DOMNodeList* tracksList = animationNode->getChildNodes();
				for (unsigned int j = 0; j < tracksList->getLength(); j++)
				{
					DOMNode* tracksNode = tracksList->item(j);
					std::string tracksStr;
					xml_utils::xml_translate(&tracksStr, tracksNode->getNodeName());
					if (tracksStr == "tracks")
					{
						const DOMNodeList* tracksList = tracksNode->getChildNodes();
						for (unsigned int k = 0; k < tracksList->getLength(); k++)
						{
							DOMNode* trackNode = tracksList->item(k);
							std::string trackStr;
							xml_utils::xml_translate(&trackStr, trackNode->getNodeName());
							if (trackStr == "track")
							{
								std::vector<float> vecTime(numKeyFrames, 0);
								std::vector<float> vecTranslateX(numKeyFrames, 0);
								std::vector<float> vecTranslateY(numKeyFrames, 0);
								std::vector<float> vecTranslateZ(numKeyFrames, 0);
								std::vector<SrVec> vecAxis(numKeyFrames);
								SrVec vec(0, 1, 0);
								for (size_t v = 0; v < (size_t) numKeyFrames; v++)
									vecAxis[v] = vec;
								std::vector<float> vecAngle(numKeyFrames, 0);
								int firstKey = -1;
								int lastKey = -1;

								std::vector<bool> vecKey(numKeyFrames, false);

								std::string trackBoneStr;
								DOMNamedNodeMap* trackAttributeList = trackNode->getAttributes();
								if (trackAttributeList)
								{
									const DOMNode* trackBoneNode = trackAttributeList->getNamedItem(BML::BMLDefs::OGRE_BONE);
									xml_utils::xml_translate(&trackBoneStr, trackBoneNode->getNodeValue());
									if (trackBoneStr == "")
										continue;
									// .............................
									const DOMNodeList* keyframesList = trackNode->getChildNodes();
									for (unsigned int l = 0; l < keyframesList->getLength(); l++)
									{
										DOMNode* keyframesNode = keyframesList->item(l);
										std::string keyframesStr;
										xml_utils::xml_translate(&keyframesStr, keyframesNode->getNodeName());
										if (keyframesStr == "keyframes")
										{										
											const DOMNodeList* keyframeList = keyframesNode->getChildNodes();
											for (unsigned int m = 0; m < keyframeList->getLength(); m++)
											{
												DOMNode* keyframeNode = keyframeList->item(m);
												std::string keyframeStr;
												xml_utils::xml_translate(&keyframeStr, keyframeNode->getNodeName());
												if (keyframeStr == "keyframe")
												{
													DOMNamedNodeMap* keyframeAttributeList = keyframeNode->getAttributes();
													if (keyframeAttributeList)
													{
														const DOMNode* timeNode = keyframeAttributeList->getNamedItem(BML::BMLDefs::OGRE_TIME);
														std::string timeStr;
														xml_utils::xml_translate(&timeStr, timeNode->getNodeValue());
														float time = (float) atof(timeStr.c_str());
														// determine the index of this key
														std::map<float, int>::iterator keyIndexIter = timeMap.find(time);
														int keyIndex = (*keyIndexIter).second;
														
														vecKey[keyIndex] = true;
														if (firstKey == -1)
															firstKey = keyIndex;
														lastKey = keyIndex;

														vecTime.push_back(time); 

														SrVec translation;
														SrVec axis;
														float angle = 0.f;
														const DOMNodeList* transformList = keyframeNode->getChildNodes();
														for (unsigned int n = 0; n < transformList->getLength(); n++)
														{
															DOMNode* transformNode = transformList->item(n);
															std::string transformNodeName;
															xml_utils::xml_translate(&transformNodeName, transformNode->getNodeName());
															if (transformNodeName == "translate")
															{
																DOMNamedNodeMap* positionAttr = transformNode->getAttributes();
																if (positionAttr)
																{
																	const DOMNode* xNode = positionAttr->getNamedItem(BML::BMLDefs::OGRE_X);
																	std::string xAttr = "";
																	if (xNode)
																		xml_utils::xml_translate(&xAttr, xNode->getNodeValue());
																	translation.x = (float) atof(xAttr.c_str());
																	const DOMNode* yNode = positionAttr->getNamedItem(BML::BMLDefs::OGRE_Y);
																	std::string yAttr = "";
																	if (yNode)
																		xml_utils::xml_translate(&yAttr, yNode->getNodeValue());
																	translation.y = (float) atof(yAttr.c_str());
																	const DOMNode* zNode = positionAttr->getNamedItem(BML::BMLDefs::OGRE_Z);
																	std::string zAttr = "";
																	if (zNode)
																		xml_utils::xml_translate(&zAttr, zNode->getNodeValue());
																	translation.z = (float) atof(zAttr.c_str());

																	vecTranslateX[keyIndex] = translation.x;
																	vecTranslateY[keyIndex] = translation.y;
																	vecTranslateZ[keyIndex] = translation.z;
																}
															}
															else if (transformNodeName == "rotate")
															{
																float angle;
																DOMNamedNodeMap* rotationAttr = transformNode->getAttributes();
																if (rotationAttr)
																{			
																	DOMNode* angleNode = rotationAttr->getNamedItem(BML::BMLDefs::OGRE_ANGLE);
																	std::string angleAttr = "";
																	if (angleNode)
																		xml_utils::xml_translate(&angleAttr, angleNode->getNodeValue());
																	angle = (float) atof(angleAttr.c_str());
																}
																
																SrVec axis;
																const DOMNodeList* angleChildList = transformNode->getChildNodes();
																for (unsigned int o = 0; o < angleChildList->getLength(); o++)
																{
																	DOMNode* angleChildNode = angleChildList->item(o);
																	std::string angleChildNodeName;
																	xml_utils::xml_translate(&angleChildNodeName, angleChildNode->getNodeName());
																	if (angleChildNodeName == "axis")
																	{
																		DOMNamedNodeMap* axisAttr = angleChildNode->getAttributes();
																		if (axisAttr)
																		{																			
																			DOMNode* xNode = axisAttr->getNamedItem(BML::BMLDefs::OGRE_X);
																			std::string xAttr = "";
																			if (xNode)
																				xml_utils::xml_translate(&xAttr, xNode->getNodeValue());
																			axis.x = (float) atof(xAttr.c_str());
																			DOMNode* yNode = axisAttr->getNamedItem(BML::BMLDefs::OGRE_Y);
																			std::string yAttr = "";
																			if (yNode)
																				xml_utils::xml_translate(&yAttr, yNode->getNodeValue());
																			axis.y = (float) atof(yAttr.c_str());
																			DOMNode* zNode = axisAttr->getNamedItem(BML::BMLDefs::OGRE_Z);
																			std::string zAttr = "";
																			if (zNode)
																				xml_utils::xml_translate(&zAttr, zNode->getNodeValue());
																			axis.z = (float) atof(zAttr.c_str());																						
																		}
																	}
																}
																vecAxis[keyIndex] = axis;
																vecAngle[keyIndex] = angle;
															}
														}
													}
												}
											}
										}
									}
								}
								if (trackBoneStr == "")
									continue;

								// interpolate any missing data
								if (numKeys > 0)
								{
									int leftKey = -1;
									int rightKey = -1;
									int lastLeftKey = -1;
									for (size_t b = 0; b < vecKey.size(); b++)
									{
										if (vecKey[b] == false)
										{
											float curTime = allTimes[b];
											// get the first right key
											for (size_t c = b + 1; c < (size_t) vecKey.size(); c++)
											{
												if (vecKey[c] == true)
												{
													rightKey = c;
													break;
												}
											}
											if (rightKey == -1)
											{
												// no right key
												// replicate all data from the first left key
												for (size_t f = b; f < vecKey.size(); f++)
												{
													vecTranslateX[f] = vecTranslateX[leftKey];
													vecTranslateY[f] = vecTranslateY[leftKey];
													vecTranslateZ[f] = vecTranslateZ[leftKey];
													vecAxis[f] = vecAxis[leftKey];
													vecAngle[f] = vecAngle[leftKey];
												}
												break;
											}
											// get the left key
											if (leftKey == -1)
											{
												// no left key, use right key value
												for (size_t f = b - 1; f >= 0; f++)
												{
													vecTranslateX[f] = vecTranslateX[rightKey];
													vecTranslateY[f] = vecTranslateY[rightKey];
													vecTranslateZ[f] = vecTranslateZ[rightKey];
													vecAxis[f] = vecAxis[rightKey];
													vecAngle[f] = vecAngle[rightKey];
												}
												break;
											}
											// interpolate between left and right keys
											float leftTime = allTimes[leftKey];
											float rightTime = allTimes[rightKey];
											float denomTime = rightTime - leftTime;
											float interp = 1.0f;
											if (fabs(denomTime) < .001)
											{
												interp = 1.0f;
											}
											else
											{
												interp = (curTime - leftTime) / (rightTime - leftTime);
											}

											vecTranslateX[b] = (1.0f - interp) * vecTranslateX[leftKey] + interp * vecTranslateX[rightKey];
											vecTranslateY[b] = (1.0f - interp) * vecTranslateY[leftKey] + interp * vecTranslateY[rightKey];
											vecTranslateZ[b] = (1.0f - interp) * vecTranslateZ[leftKey] + interp * vecTranslateZ[rightKey];;

											SrQuat leftQuat(vecAxis[leftKey], vecAngle[leftKey]);
											SrQuat rightQuat(vecAxis[rightKey], vecAngle[rightKey]);
											SrQuat interpQuat = slerp(leftQuat, rightQuat, interp);
											vecAxis[b] = interpQuat.axis();
											vecAngle[b] = interpQuat.angle();											
										}
										else
										{
											leftKey = b;
										}
									}
								}
								
								// finished with the track
								// add a frame for that channel							
								motionChannels.add(trackBoneStr, SkChannel::XPos);
								motionChannels.add(trackBoneStr, SkChannel::YPos);
								motionChannels.add(trackBoneStr, SkChannel::ZPos);
								motionChannels.add(trackBoneStr, SkChannel::Quat);

								float* data = NULL;
								size_t xsize = vecTranslateX.size();
								data = new float[xsize];
								for (size_t t = 0; t < xsize; t++)
									data[t] = vecTranslateX[t] * scale;
								allData.push_back(data);

								size_t ysize = vecTranslateY.size();
								data = new float[ysize];
								for (size_t  t = 0; t < ysize; t++)
									data[t] = vecTranslateY[t] * scale;
								allData.push_back(data);

								size_t zsize = vecTranslateZ.size();
								data = new float[zsize];
								for (size_t t = 0; t < zsize; t++)
									data[t] = vecTranslateZ[t] * scale;
								allData.push_back(data);

								size_t anglesize = vecAngle.size();
								size_t axisSize = vecAxis.size();
								data = new float[4 * anglesize];
								for (size_t t = 0; t < anglesize; t++)
								{
									SrQuat quat(vecAxis[t], vecAngle[t]);
									data[t * 4] = quat.w;
									data[t * 4 + 1] = quat.x;
									data[t * 4 + 2] = quat.y;
									data[t * 4 + 3] = quat.z;
								}
								allData.push_back(data);
							}
						}
					}
				}

				motion->init(motionChannels);

				int counter = 0;
				for (std::vector<float>::iterator iter = allTimes.begin();
					 iter != allTimes.end();
					 iter++)
				{
					// set the key times
					motion->insert_frame(counter, (*iter));
					counter++;
				}

				// add all the data to the motion
				for (size_t f = 0; f < (size_t) numKeyFrames; f++)
				{
					float* posture = motion->posture(f);	
					int psize = motion->posture_size();
					int counter = 0;
					for (size_t x = 0; x < allData.size(); x++)
					{
						int stage = x % 4;
						if (stage < 3)
						{
							posture[counter] = allData[x][f];
							counter++;
						}
						else
						{
							posture[counter] = allData[x][f * 4];
							posture[counter + 1] = allData[x][f  * 4 + 1];
							posture[counter + 2] = allData[x][f  * 4 + 2];
							posture[counter + 3] = allData[x][f  * 4 + 3];
							counter += 4;
						}
					}
				}

				// clean up allData
				for (size_t allDataCounter = 0; allDataCounter < allData.size(); ++allDataCounter)
				{
					delete allData[allDataCounter];
					allData[allDataCounter]	 = NULL;
				}
				allData.clear();
			}
			else
			{
				LOG("Animation in file %s has no name or length, will not be parsed.", pathName.c_str());
				return false;
			}
		}
	}
	return true;
}

bool ParserOgre::parseMesh( DOMNode* meshNode, std::vector<SrModel*>& meshModelVec, float scaleFactor )
{
	//LOG("ParseOgre::parseMesh");
	DOMNode* subMeshNode = getNode("submeshes",meshNode);
	if (!subMeshNode) return false;
	const DOMNodeList* subMeshList = subMeshNode->getChildNodes();
	//LOG("Num of submeshes = %d",subMeshList->getLength());
	for (unsigned int i=0;i<subMeshList->getLength(); i++)
	{
		DOMNode* subMesh = subMeshList->item(i);	
		std::string subMeshStr;
		xml_utils::xml_translate(&subMeshStr, subMesh->getNodeName());
		if (subMeshStr != "submesh")
			continue;
		SrModel* model = new SrModel();
		std::string meshName = subMeshStr + boost::lexical_cast<std::string>(i);
		model->name = meshName.c_str();
		DOMNamedNodeMap* subMeshAttr = subMesh->getAttributes();
		std::string materialName = "";
		if (subMeshAttr)
		{
			const DOMNode* matNode = subMeshAttr->getNamedItem(BML::BMLDefs::OGRE_MATERIAL);
			std::string matAttr = "";
			if (matNode)
				xml_utils::xml_translate(&matAttr, matNode->getNodeValue());
			materialName = matAttr;
		}
		if (materialName != "")
			model->mtlnames.push(materialName.c_str());

		meshModelVec.push_back(model);
		//LOG("SubMesh %d ... ",i);
		const DOMNodeList* subMeshChildren = subMesh->getChildNodes();
		for (unsigned int a = 0; a < subMeshChildren->getLength(); a++)
		{
			DOMNode* subMeshChild = subMeshChildren->item(a);
			std::string childNodeStr;
			xml_utils::xml_translate(&childNodeStr, subMeshChild->getNodeName());
			if (childNodeStr == "geometry")
			{
				//LOG("parse geometry");
				DOMNamedNodeMap* childAttr = subMeshChild->getAttributes();
				int vertexCount = 0;
				if (childAttr)
				{
					const DOMNode* countNode = childAttr->getNamedItem(BML::BMLDefs::OGRE_VERTEX_COUNT);
					std::string countAttr = "";
					if (countNode)
						xml_utils::xml_translate(&countAttr, countNode->getNodeValue());
					vertexCount = atoi(countAttr.c_str());
				}
				const DOMNodeList* bufferList = subMeshChild->getChildNodes();
				for (unsigned int b = 0; b < bufferList->getLength(); b++)
				{
					DOMNode* bufferNode = bufferList->item(b);
					std::string bufferNodeStr;
					xml_utils::xml_translate(&bufferNodeStr, bufferNode->getNodeName());
					if (bufferNodeStr != "vertexbuffer")
						continue;
					//LOG("vertex buffer %d ...",b);
					const DOMNodeList* vertexList = bufferNode->getChildNodes();
					DOMNode* vertexNode = bufferNode->getFirstChild();
					//for (unsigned int v = 0; v < vertexList->getLength(); v++)
					while (vertexNode)
					{
						//DOMNode* vertexNode = vertexList->item(v);
						std::string vertexStr;
						xml_utils::xml_translate(&vertexStr, vertexNode->getNodeName());
						if (vertexStr != "vertex")
						{
							vertexNode = vertexNode->getNextSibling();
							continue;
						}
						//LOG("vertex %d ... ",v);
						const DOMNodeList* vertexDataList = vertexNode->getChildNodes();
						for (unsigned int j = 0; j < vertexDataList->getLength(); j++)
						{
							DOMNode* transformNode = vertexDataList->item(j);
							std::string transformNodeName;
							xml_utils::xml_translate(&transformNodeName, transformNode->getNodeName());
							if (transformNodeName == "position" || transformNodeName == "normal")
							{
								DOMNamedNodeMap* positionAttr = transformNode->getAttributes();
								if (positionAttr)
								{
									SrVec offset;
									const DOMNode* xNode = positionAttr->getNamedItem(BML::BMLDefs::OGRE_X);
									std::string xAttr = "";
									if (xNode)
										xml_utils::xml_translate(&xAttr, xNode->getNodeValue());
									offset.x = (float) atof(xAttr.c_str());
									const DOMNode* yNode = positionAttr->getNamedItem(BML::BMLDefs::OGRE_Y);
									std::string yAttr = "";
									if (yNode)
										xml_utils::xml_translate(&yAttr, yNode->getNodeValue());
									offset.y = (float) atof(yAttr.c_str());
									const DOMNode* zNode = positionAttr->getNamedItem(BML::BMLDefs::OGRE_Z);
									std::string zAttr = "";
									if (zNode)
										xml_utils::xml_translate(&zAttr, zNode->getNodeValue());
									offset.z = (float) atof(zAttr.c_str());									
									if (transformNodeName == "position")
										model->V.push(offset);
									else if (transformNodeName == "normal")
										model->N.push(offset);
								}
								
							}							
							else if (transformNodeName == "texcoord")
							{
								DOMNamedNodeMap* texcoordAttr = transformNode->getAttributes();
								if (texcoordAttr)
								{
									SrPnt2 offset;
									const DOMNode* xNode = texcoordAttr->getNamedItem(BML::BMLDefs::OGRE_U);
									std::string xAttr = "";
									if (xNode)
										xml_utils::xml_translate(&xAttr, xNode->getNodeValue());
									offset.x = (float) atof(xAttr.c_str());
									const DOMNode* yNode = texcoordAttr->getNamedItem(BML::BMLDefs::OGRE_V);
									std::string yAttr = "";
									if (yNode)
										xml_utils::xml_translate(&yAttr, yNode->getNodeValue());
									offset.y = 1.f - (float) atof(yAttr.c_str());	
									//offset.y = (float) atof(yAttr.c_str());	
									model->T.push(offset);									
								}
							}						
						}
						vertexNode = vertexNode->getNextSibling();
					}
				}
				//LOG("parse geometry complete");

			}
			else if (childNodeStr == "faces")
			{
				//LOG("parse faces");
				const DOMNodeList* faceList = subMeshChild->getChildNodes();
				DOMNode* faceNode = subMeshChild->getFirstChild();
				//std::map<int,int> infJointCount;
				//LOG("weight list size = %d",weightList->getLength());
				//for (unsigned int w = 0; w < weightList->getLength(); w++)
				//{
				int w = 0;
				while (faceNode)
				//for (unsigned int f = 0; f < faceList->getLength(); f++)
				{
					//DOMNode* faceNode = faceList->item(f);
					std::string faceStr;
					xml_utils::xml_translate(&faceStr, faceNode->getNodeName());
					if (faceStr != "face")
					{
						faceNode = faceNode->getNextSibling();
						continue;
					}
					DOMNamedNodeMap* faceAttr = faceNode->getAttributes();
					if (faceAttr)
					{
						int v1,v2,v3;
						const DOMNode* xNode = faceAttr->getNamedItem(BML::BMLDefs::OGRE_V1);
						std::string xAttr = "";
						if (xNode)
							xml_utils::xml_translate(&xAttr, xNode->getNodeValue());
						v1 = atoi(xAttr.c_str());
						const DOMNode* yNode = faceAttr->getNamedItem(BML::BMLDefs::OGRE_V2);
						std::string yAttr = "";
						if (yNode)
							xml_utils::xml_translate(&yAttr, yNode->getNodeValue());
						v2 = atoi(yAttr.c_str());
						const DOMNode* zNode = faceAttr->getNamedItem(BML::BMLDefs::OGRE_V3);
						std::string zAttr = "";
						if (zNode)
							xml_utils::xml_translate(&zAttr, zNode->getNodeValue());
						v3 = atoi(zAttr.c_str());

						int t1,t2,t3;

						const DOMNode* txNode = faceAttr->getNamedItem(BML::BMLDefs::OGRE_T1);
						std::string txAttr = "-1";
						if (txNode)
							xml_utils::xml_translate(&txAttr, txNode->getNodeValue());
						t1 = atoi(txAttr.c_str());
						const DOMNode* tyNode = faceAttr->getNamedItem(BML::BMLDefs::OGRE_T2);
						std::string tyAttr = "-1";
						if (tyNode)
							xml_utils::xml_translate(&tyAttr, tyNode->getNodeValue());
						t2 = atoi(tyAttr.c_str());
						const DOMNode* tzNode = faceAttr->getNamedItem(BML::BMLDefs::OGRE_T3);
						std::string tzAttr = "-1";
						if (tzNode)
							xml_utils::xml_translate(&tzAttr, tzNode->getNodeValue());
						t3 = atoi(tzAttr.c_str());

						if (t1 == -1 || t2 == -1 || t3 == -1)
						{
							t1 = v1; t2 = v2; t3 = v3;
						}
						
						// no material for now.
						model->Fm.push(0); // use first material
						model->F.push().set(v1,v2,v3);
						model->Ft.push().set(t1,t2,t3);
						model->Fn.push().set(v1,v2,v3);
					}	
					faceNode = faceNode->getNextSibling();
				}
				//LOG("parse faces complete");
			}
		}
	}
	LOG("ParseOgre::parseMesh complete");
	return true;
}

bool ParserOgre::parseSkinWeight( DOMNode* meshNode, std::vector<SkinWeight*>& skinWeights, float scaleFactor )
{
	//LOG("ParseOgre::parseSkinWeight");
	DOMNode* subMeshNode = getNode("submeshes",meshNode);
	if (!subMeshNode) return false;
	const DOMNodeList* subMeshList = subMeshNode->getChildNodes();

	for (unsigned int i=0;i<subMeshList->getLength(); i++)
	{
		DOMNode* subMesh = subMeshList->item(i);	
		std::string subMeshStr;
		xml_utils::xml_translate(&subMeshStr, subMesh->getNodeName());
		if (subMeshStr != "submesh")
			continue;
		//SrModel* model = new SrModel();
		std::string meshName = subMeshStr + boost::lexical_cast<std::string>(i);
		const DOMNodeList* subMeshChildren = subMesh->getChildNodes();
		for (unsigned int a = 0; a < subMeshChildren->getLength(); a++)
		{
			DOMNode* subMeshChild = subMeshChildren->item(a);
			std::string childNodeStr;
			xml_utils::xml_translate(&childNodeStr, subMeshChild->getNodeName());
			if (childNodeStr == "boneassignments")
			{		
				SkinWeight* sw = new SkinWeight();
				sw->sourceMesh = meshName;
				skinWeights.push_back(sw);
				const DOMNodeList* weightList = subMeshChild->getChildNodes();				
				int prevVtxIdx = -1;
				int infJointCount = 0;
				DOMNode* weightNode = subMeshChild->getFirstChild();
				//std::map<int,int> infJointCount;
				//LOG("weight list size = %d",weightList->getLength());
				//for (unsigned int w = 0; w < weightList->getLength(); w++)
				//{
				int w = 0;
				while (weightNode)
				{
					//DOMNode* weightNode = weightList->item(w);
					DOMNamedNodeMap* weightAttr = weightNode->getAttributes();
					if (weightAttr)
					{
						int vtxIdx, boneIdx;
						float weight;
						const DOMNode* vtxIdxNode = weightAttr->getNamedItem(BML::BMLDefs::OGRE_VERTEX_INDEX);
						std::string xAttr = "";
						if (vtxIdxNode)
							xml_utils::xml_translate(&xAttr, vtxIdxNode->getNodeValue());
						vtxIdx = atoi(xAttr.c_str());
						if (prevVtxIdx != vtxIdx)
						{
							if (prevVtxIdx != -1)
							{
								sw->numInfJoints.push_back(infJointCount);
							}
							prevVtxIdx = vtxIdx;
							infJointCount = 0;
						}
						infJointCount++;
						const DOMNode* boneIdxNode = weightAttr->getNamedItem(BML::BMLDefs::OGRE_BONE_INDEX);
						std::string yAttr = "";
						if (boneIdxNode)
							xml_utils::xml_translate(&yAttr, boneIdxNode->getNodeValue());
						boneIdx = atoi(yAttr.c_str());
						const DOMNode* zNode = weightAttr->getNamedItem(BML::BMLDefs::OGRE_WEIGHT);
						std::string zAttr = "";
						if (zNode)
							xml_utils::xml_translate(&zAttr, zNode->getNodeValue());
						weight = (float) atof(zAttr.c_str());
						
						sw->weightIndex.push_back(sw->bindWeight.size());
						sw->bindWeight.push_back(weight);
						sw->jointNameIndex.push_back(boneIdx);	
						//sw->bindPoseMat.push_back(SrMat::id);
					}					
					weightNode = weightNode->getNextSibling();
					w++;
				}	
				sw->numInfJoints.push_back(infJointCount); // add the last set of infJoints
				sw->normalizeWeights();
			}
		}			
	}

	LOG("ParseOgre::parseSkinWeight Complete");
	return true;
}

bool isFloat(std::string someString)
{
	using boost::lexical_cast;
	using boost::bad_lexical_cast; 
	try
	{
		boost::lexical_cast<float>(someString);
	}
	catch (bad_lexical_cast &)
	{
		return false;
	}

	return true;
}

void ParserOgre::loadMeshMaterial( std::vector<SrModel*>& meshModelVec, std::string materialFileName, std::string materialFilePath )
{	
	std::map<std::string, std::vector<int> > materialModelIndexMap;
	std::map<std::string, std::string> materialTextureMap;
	std::map<std::string, std::string> materialNormalMap;
	std::map<std::string, SrMaterial> materialMap;
	for (unsigned int i=0;i<meshModelVec.size();i++)
	{
		SrModel* model = meshModelVec[i];
		if (model->mtlnames.size() > 0)
		{			
			std::string matName = model->mtlnames.get(0);
			if (materialModelIndexMap.find(matName) == materialModelIndexMap.end())
			{
				materialModelIndexMap[matName] = std::vector<int>();
				SrMaterial tempMat; tempMat.init();
				materialMap[matName] = tempMat;
			}
			materialModelIndexMap[matName].push_back(i);

		}
	}
	std::ifstream t(materialFileName.c_str());
	std::stringstream buffer;
	buffer << t.rdbuf();
	std::string matFileContent = buffer.str();
	std::vector<std::string> tokens;
	vhcl::Tokenize(matFileContent,tokens," \t\n\f\b\r"); // remove all spaces or special escape sequences
	unsigned int idx = 0;
	while (idx < tokens.size())
	{
		if (tokens[idx] == "material")
		{
			idx++;
			if (materialModelIndexMap.find(tokens[idx]) != materialModelIndexMap.end())
			// the material name is in the mesh models, parse this material
			{
				std::string materialName = tokens[idx];
				//LOG("material name = %s",materialName.c_str());
				SrMaterial& curMaterial = materialMap[materialName];
				int bracketCounter = 0;
				do {
					idx++;
					if (tokens[idx] == "{")
					{
						bracketCounter++;
					}
					else if (tokens[idx] == "}")
					{
						bracketCounter--;
					}
					else if (tokens[idx] == "texture") // texture file
					{
						idx++;
						materialTextureMap[materialName] = tokens[idx]; // set texture map						
					}
					else if (tokens[idx] == "ambient")
					{
						std::vector<float> floatVec;
						int offset = 1;
						while (isFloat(tokens[idx+1]))
						{
							floatVec.push_back((float)atof(tokens[idx+1].c_str()));
							idx++;
						}			
						//LOG("ambient, vec size = %d",floatVec.size());
						if (floatVec.size() >=3 && floatVec.size() <= 4)
						{
							float c[4];
							for (unsigned int i=0;i<floatVec.size();i++)
								c[i] = floatVec[i];
							if (floatVec.size() == 3)
								c[3] = 1.f;
							//LOG("color = %f %f %f %f",c[0],c[1],c[2],c[3]);
							curMaterial.ambient = SrColor(c);
						}						
					}
					else if (tokens[idx] == "diffuse")
					{
						std::vector<float> floatVec;
						int offset = 1;
						while (isFloat(tokens[idx+1]))
						{
							floatVec.push_back((float)atof(tokens[idx+1].c_str()));
							idx++;
						}			
						//LOG("diffuse, vec size = %d",floatVec.size());
						if (floatVec.size() >=3 && floatVec.size() <= 4)
						{
							float c[4];
							for (unsigned int i=0;i<floatVec.size();i++)
								c[i] = floatVec[i];
							if (floatVec.size() == 3)
								c[3] = 1.f;
							//LOG("color = %f %f %f %f",c[0],c[1],c[2],c[3]);
							curMaterial.diffuse = SrColor(c);
						}	

					}
					else if (tokens[idx] == "specular")
					{
						std::vector<float> floatVec;
						int offset = 1;
						while (isFloat(tokens[idx+1]))
						{
							floatVec.push_back((float)atof(tokens[idx+1].c_str()));
							idx++;
						}
						//LOG("specular, vec size = %d",floatVec.size());
						if (floatVec.size() >=3 && floatVec.size() <= 5)
						{
							float c[4];
							for (unsigned int i=0;i<3;i++)
								c[i] = floatVec[i];
							if (floatVec.size() < 5)
								c[3] = 1.f;			

							if (floatVec.size() == 5)
							{
								c[3] = floatVec[3];
								curMaterial.shininess = (int)floatVec[4];
							}
							else if (floatVec.size() == 4)
								curMaterial.shininess = (int)floatVec[3];

							curMaterial.specular = SrColor(c);
							//LOG("color = %f %f %f %f",c[0],c[1],c[2],c[3]);
						}						
					}				

				} while (bracketCounter != 0);

			}
		}
		idx++;
	}

	SrStringArray pathArray;
	pathArray.push(materialFilePath.c_str());

	std::map<std::string, std::vector<int> >::iterator mi;
	for ( mi  = materialModelIndexMap.begin();
		  mi != materialModelIndexMap.end();
		  mi++)
	{
		std::string matName = mi->first;
		std::string textureName = "";
/*
#if !defined (__ANDROID__) && !defined(SB_IPHONE)
		if (materialTextureMap.find(matName) != materialTextureMap.end())
		{
			 textureName = materialTextureMap[matName];
			 ParserOgre::loadTexture(SbmTextureManager::TEXTURE_DIFFUSE,textureName,pathArray);
		}
#endif
*/
		std::vector<int> modelIdxList = mi->second;
		for (unsigned int j=0;j<modelIdxList.size();j++)
		{
			int modelIdx = modelIdxList[j];
			SrModel* model = meshModelVec[modelIdx];
			model->M.size(1);
			model->M.set(0,materialMap[matName]);
			if (textureName != "")
				model->mtlTextureNameMap[matName] = textureName;
		}
	}



// 	in.lowercase_tokens(false);
// 	in.init ( fopen(materialFileName.c_str(),"rt") );
// 
// 	if ( !in.valid() ) return; // could not get materials
// 
// 	while ( !in.finished() )
// 	{ in.get_token();	  
// 	  if ( in.last_token() == "material" )
// 	  { 	
// 		  in.get_token();
// 		  std::string materialName = in.last_token();
// 		  if (materialModelIndexMap.find(materialName) != materialModelIndexMap.end())
// 		  {
// 			 			  			  
// 		  }
// 	  }	  	
// 	}	
}

bool ParserOgre::parseMeshMaterial( std::vector<SrModel*>& meshModelVec, std::string materialFilePath )
{
#if (BOOST_VERSION > 104400)
	boost::filesystem::path curpath( materialFilePath );
	boost::filesystem::directory_iterator end;
	std::vector<std::string> materialFileList;
	for (boost::filesystem::directory_iterator iter(curpath); iter != end ; iter++)
	{
		const boost::filesystem::path& cur = *iter;
		if (boost::filesystem::is_regular(cur))
		{
			std::string fileName = (cur).string();			
#else
	boost::filesystem2::path curpath( materialFilePath );
	boost::filesystem2::directory_iterator end;
	std::vector<std::string> materialFileList;
	for (boost::filesystem2::directory_iterator iter(curpath); iter != end ; iter++)
	{
		if (boost::filesystem::is_regular(*iter))
		{
			std::string fileName = (*iter).string();			
#endif
			std::string ext = boost::filesystem::extension(fileName);
			if (ext == ".material" || ext == ".MATERIAL")
			{
				materialFileList.push_back(fileName);
			}			
		}
	}
	for (unsigned int i=0;i<materialFileList.size();i++)
	{
		loadMeshMaterial(meshModelVec,materialFileList[i],materialFilePath+"/");
	}
	return true;
}

/*
void ParserOgre::loadTexture( int type, std::string texFileName, const SrStringArray& paths )
{
#if !defined (__ANDROID__) && !defined(SB_IPHONE)
	SrString s;
	SrInput in;
	std::string imageFile = texFileName;
	in.init( fopen(texFileName.c_str(),"r"));
	int i = 0;
	while ( !in.valid() && i < paths.size())
	{
		s = paths[i++];
		s << texFileName.c_str();
		imageFile = s;
		in.init ( fopen(s,"r") );
	}
	if (!in.valid()) return;		
	SbmTextureManager& texManager = SbmTextureManager::singleton();
	texManager.loadTexture(type,texFileName.c_str(),s);	
#endif
}
*/

bool ParserOgre::exportOgreXMLMesh( DeformableMesh* defMesh, std::string meshName, std::string outPathName )
{
	std::string meshFileName = outPathName + "/" + meshName + ".mesh.xml";
	std::string materialFileName = outPathName + "/" + meshName + ".material";
	std::vector<std::string> textureFileNames;
	SmartBody::SBSkeleton* skel = SmartBody::SBScene::getScene()->getSkeleton(defMesh->skeletonName);
	if (!skel)
		return false;

	// write out mesh file
	FILE* fp = fopen(meshFileName.c_str(),"wt");
	fprintf(fp,"<mesh>\n");
	fprintf(fp,"<submeshes>\n"); 
	std::vector<SkinWeight*>& skinWeights = defMesh->skinWeights;
	std::vector<SrSnModel*>& models = defMesh->dMeshStatic_p;
	
	for (unsigned int i=0;i<skinWeights.size();i++)
	{
		SkinWeight* weight = skinWeights[i];
		SrModel& model = models[i]->shape();
		SrMaterial mat;
		std::string matName = "defaultMat";	
		std::string textureName = "";
		if (model.M.size() > 0)
		{
			matName = model.mtlnames[0];						
			mat = model.M[0];
		}
		fprintf(fp,"<submesh material=\"%s\">\n", matName.c_str());

		// export vertices
		fprintf(fp,"<geometry vertexcount=\"%d\">\n",model.V.size());
		if (model.V.size() != model.N.size())
			model.computeNormals();
		fprintf(fp,"<vertexbuffer positions=\"true\" normals=\"true\">\n");
		for (unsigned int k=0;k< (size_t) model.V.size();k++)
		{
			SrVec& p = model.V[k];
			SrVec& n = model.N[k];
			fprintf(fp,"<vertex>\n");
			fprintf(fp,"<position x=\"%f\" y=\"%f\" z=\"%f\" />\n",p[0],p[1],p[2]);			
			fprintf(fp,"<normal x=\"%f\" y=\"%f\" z=\"%f\" />\n",n[0],n[1],n[2]);
			fprintf(fp,"</vertex>\n");
		}
		fprintf(fp,"</vertexbuffer>\n");

		if (model.T.size() == model.V.size())
		{
			fprintf(fp,"<vertexbuffer texture_coord_dimensions_0=\"2\" texture_coords=\"1\">\n");
			for (unsigned int k=0;k< (size_t) model.T.size();k++)
			{
				//const Vector3 &p = m.vertices[i].pos;
				SrPnt2& tex = model.T[k];
				fprintf(fp,"<vertex>\n");
				fprintf(fp,"<texcoord u=\"%f\" v=\"%f\" />\n",tex[0],1.f - tex[1]);		
				fprintf(fp,"</vertex>\n");
			}
			fprintf(fp,"</vertexbuffer>\n");
		}		
		fprintf(fp,"</geometry>\n");
		fprintf(fp,"<boneassignments>\n");
		int counter = 0;
		for (unsigned int k=0;k<weight->numInfJoints.size();k++)
		{
			int nw = weight->numInfJoints[k];
			for (int w = 0; w < nw; w++)
			{
				int widx = weight->weightIndex[counter];
				int bidx = weight->jointNameIndex[counter];
				//std::string& boneName = weight->infJointName[bidx];				
				fprintf(fp,"<vertexboneassignment vertexindex=\"%d\" boneindex=\"%d\" weight=\"%f\" />\n",k,weight->bindWeight[widx],bidx);
				counter++;
			}			
		}
		fprintf(fp,"</boneassignments>\n");
		fprintf(fp,"</submesh>\n");
	}

	fprintf(fp,"</submeshes>\n"); 
	fprintf(fp,"<skeletonlink name=\"%s\" />\n",defMesh->skeletonName.c_str());
	fprintf(fp,"</mesh>\n");
	fclose(fp);
	return true;
}

