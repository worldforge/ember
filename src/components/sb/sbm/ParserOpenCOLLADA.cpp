#include "ParserOpenCOLLADA.h"
#include "components/sb/sr/sr_euler.h"
#include <boost/version.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <algorithm>
#include <cctype>
#include <string>
#include "BMLDefs.h"
#include "components/sb/sb/SBScene.h"
#include "components/sb/sb/SBAssetManager.h"
#include "components/sb/sb/SBSkeleton.h"
#include "components/sb/sb/SBMotion.h"
#include "sbm_deformable_mesh.h"


DOMNode* ParserOpenCOLLADA::getNode(const std::string& nodeName, DOMNode* node)
{
	int type = node->getNodeType();
	if (type ==  DOMNode::ELEMENT_NODE)
	{
		char* name = XMLString::transcode(node->getNodeName());
		std::string strname = name;
		if (strname == nodeName)
		{	
			XMLString::release( &name );
			return node;
		}
		XMLString::release( &name );
	}

	DOMNode* child = NULL;
	//const DOMNodeList* list = node->getChildNodes();
	DOMNode* curNode = node->getFirstChild();
	while (curNode)
	//for (unsigned int c = 0; c < list->getLength(); c++)
	{
		//child = getNode(nodeName, list->item(c));
		child = getNode(nodeName, curNode);
		if (child)
			break;
		curNode = curNode->getNextSibling();
	}
	return child;
}

DOMNode* ParserOpenCOLLADA::getNode(const std::string& nodeName, DOMNode* node, int curDepth, int maximumDepth)
{
	int type = node->getNodeType();
	if (type ==  DOMNode::ELEMENT_NODE)
	{
		char* name = XMLString::transcode(node->getNodeName());
		std::string strname = name;
		if (strname == nodeName)
		{
			XMLString::release( &name );
			return node;
		}
		XMLString::release( &name );
	}

	if (maximumDepth > -1 &&
		curDepth >= maximumDepth)
		return NULL;

	curDepth++;
	

	DOMNode* child = NULL;
	//const DOMNodeList* list = node->getChildNodes();
	DOMNode* curNode = node->getFirstChild();
	while (curNode)
	//for (unsigned int c = 0; c < list->getLength(); c++)
	{
		child = getNode(nodeName, curNode, curDepth, maximumDepth);
		if (child)
			break;
		curNode = curNode->getNextSibling();
	}
	return child;
}

std::string ParserOpenCOLLADA::getNodeAttributeString( DOMNode* node, XMLCh* attrName )
{
	std::string sourceIdAttr = "";
	DOMNamedNodeMap* sourceAttr = node->getAttributes();
	DOMNode* sourceIdNode = sourceAttr->getNamedItem(attrName);					
	if (sourceIdNode)
		xml_utils::xml_translate(&sourceIdAttr, sourceIdNode->getNodeValue());
	return sourceIdAttr;
}

int ParserOpenCOLLADA::getNodeAttributeInt( DOMNode* node, XMLCh* attrName )
{
	std::string strAttr = getNodeAttributeString(node,attrName);
	return atoi(strAttr.c_str());
}


bool ParserOpenCOLLADA::parse(SkSkeleton& skeleton, SkMotion& motion, std::string pathName, float scale, bool doParseSkeleton, bool doParseMotion)
{
	try 
	{
		XMLPlatformUtils::Initialize();
	}
	catch (const XMLException& toCatch) 
	{
		std::string message = "";
		xml_utils::xml_translate(&message, toCatch.getMessage());
		std::cout << "Error during initialization! :\n" << message << "\n";
		return false;
	}

	XercesDOMParser* parser = new XercesDOMParser();
	parser->setValidationScheme(XercesDOMParser::Val_Always);
	parser->setDoNamespaces(true);    // optional

	ErrorHandler* errHandler = (ErrorHandler*) new HandlerBase();
	parser->setErrorHandler(errHandler);

	bool zaxis = false;

	try 
	{
		int order;
		std::string filebasename = boost::filesystem::basename(pathName);
		std::string fileextension = boost::filesystem::extension(pathName);
		motion.setName(filebasename.c_str());
		std::stringstream strstr;
		if (fileextension.size() > 0 && fileextension[0] == '.')
			strstr << filebasename << fileextension;
		else
			strstr << filebasename << "." << fileextension;
		skeleton.setName(strstr.str().c_str());
		parser->parse(pathName.c_str());
		DOMDocument* doc = parser->getDocument();


		DOMNode* asset = getNode("asset", doc, 0, 1);
		if (asset)
		{
			DOMNode* upNode = getNode("up_axis", asset);
			if (upNode)
			{
				std::string upAxisName;
				xml_utils::xml_translate(&upAxisName, upNode->getTextContent());
				if (upAxisName == "Z_UP" || upAxisName == "z_up")
				{
					// rotate the skeleton by -90 around the x-axis
					zaxis = true;
				}

			}
		}

		DOMNode* skNode = getNode("library_visual_scenes", doc, 0, 2);
		if (!skNode)
		{
			LOG("ParserOpenCOLLADA::parse ERR: no skeleton info contained in this file");
			delete parser;
			delete errHandler;
			return false;
		}
		std::map<std::string, std::string> materialId2Name;
		if (doParseSkeleton)
			parseLibraryVisualScenes(skNode, skeleton, motion, scale, order, materialId2Name);

		if (zaxis)
		{
			// get the root node
			SkJoint* root = skeleton.root();
			if (root)
			{
				if (root->quat())
				{
					SrQuat prerot = root->quat()->prerot();
					SrVec xaxis(1, 0, 0);
					SrQuat adjust(xaxis, 3.14159f / -2.0f);
					SrQuat final = adjust * prerot;
					root->quat()->prerot(final);
					root->offset(root->offset()*adjust);
				}
			}
		}		
		skeleton.updateGlobalMatricesZero();
		DOMNode* skmNode = getNode("library_animations", doc, 0, 2);
		if (!skmNode)
		{
		//	LOG("ParserOpenCOLLADA::parse WARNING: no motion info contained in this file");
			delete parser;
			delete errHandler;
			return true;
		}
		if (doParseMotion)
			parseLibraryAnimations(skmNode, skeleton, motion, scale, order, zaxis);
	//	animationPostProcess(skeleton, motion);
		
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
		LOG("Unexpected Exception in ParseOpenCollada::parse()");
		return false;
	}

	delete parser;
	delete errHandler;
	return true;
}

void ParserOpenCOLLADA::parseLibraryVisualScenes(DOMNode* node, SkSkeleton& skeleton, SkMotion& motion, float scale, int& order, std::map<std::string, std::string>& materialId2Name)
{
	//const DOMNodeList* list1 = node->getChildNodes();
	DOMNode* curNode1 = node->getFirstChild();
	while (curNode1)
	//for (unsigned int c = 0; c < list1->getLength(); c++)
	{
		//DOMNode* node1 = list1->item(c);
		DOMNode* node1 = curNode1;
		std::string nodeName;
		xml_utils::xml_translate(&nodeName, node1->getNodeName());
		if (nodeName == "visual_scene")
			parseJoints(node1, skeleton, motion, scale, order, materialId2Name, NULL);
		curNode1 = curNode1->getNextSibling();
	}	
}

void ParserOpenCOLLADA::parseJoints(DOMNode* node, SkSkeleton& skeleton, SkMotion& motion, float scale, int& order, std::map<std::string, std::string>& materialId2Name, SkJoint* parent, bool hasRootJoint)
{
	//const DOMNodeList* list = node->getChildNodes();
	DOMNode* curNode = node->getFirstChild();
	while (curNode)
	//for (unsigned int i = 0; i < list->getLength(); i++)
	{
		//DOMNode* childNode = list->item(i);
		DOMNode* childNode = curNode;
		std::string nodeName;
		xml_utils::xml_translate(&nodeName, childNode->getNodeName());
		if (nodeName == "node")
		{
			DOMNamedNodeMap* childAttr = childNode->getAttributes();
			DOMNode* idNode = childAttr->getNamedItem(BML::BMLDefs::ATTR_ID);
			std::string idAttr = "";
			if (idNode)
				xml_utils::xml_translate(&idAttr, idNode->getNodeValue());

			DOMNode* nameNode = childAttr->getNamedItem(BML::BMLDefs::ATTR_NAME);
			std::string nameAttr = "";
			if (nameNode)
				xml_utils::xml_translate(&nameAttr, nameNode->getNodeValue());

			DOMNode* sidNode = childAttr->getNamedItem(BML::BMLDefs::ATTR_SID);
			std::string sidAttr = "";
			if (sidNode)
				xml_utils::xml_translate(&sidAttr, sidNode->getNodeValue());

			DOMNode* typeNode = childAttr->getNamedItem(BML::BMLDefs::ATTR_TYPE);
			std::string typeAttr = "";
			
			if (typeNode)
				xml_utils::xml_translate(&typeAttr, typeNode->getNodeValue());

			DOMNode* tempMaterialNode = ParserOpenCOLLADA::getNode("bind_material", childNode, 0, 2);

			// process this node as a joint if it's a non-geometry node
			bool treatAsJoint = false;
			if (typeAttr == "NODE" && hasRootJoint)
			{
				treatAsJoint = true;
				DOMNode* geometryNode = ParserOpenCOLLADA::getNode("instance_geometry", childNode, 0, 1);
				if (geometryNode)
					treatAsJoint = false;
			}


			if (treatAsJoint || typeAttr == "JOINT" || (nameAttr.find("Bip")!= std::string::npos && skeleton.root() == NULL) )
			{
				hasRootJoint = true;
				int index = -1;
				if (parent != NULL)	
					index = parent->index();

				SkJoint* joint = skeleton.add_joint(SkJoint::TypeQuat, index);
				joint->quat()->activate();
				joint->name(nameAttr);
				joint->extName(nameAttr);
				joint->extID(idAttr);
				joint->extSID(sidAttr);


				bool hasTranslate = false;

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

				std::vector<std::string> orderVec;

				if (parent == NULL && !skeleton.root())
					skeleton.root(joint);

				//const DOMNodeList* infoList = childNode->getChildNodes();
				DOMNode* infoCurNode = childNode->getFirstChild();
				while (infoCurNode)
				//for (unsigned int j = 0; j < infoList->getLength(); j++)
				{
					//DOMNode* infoNode = infoList->item(j);
					DOMNode* infoNode = infoCurNode;
					std::string infoNodeName;
					xml_utils::xml_translate(&infoNodeName, infoNode->getNodeName());
					if (infoNodeName == "matrix")
					{
						std::string matrixString;
						xml_utils::xml_translate(&matrixString, infoNode->getTextContent());
						std::vector<std::string> tokens;
						vhcl::Tokenize(matrixString, tokens, " \n");
						SrMat matrix;
						for (int m = 0; m < 16; m++)
							matrix[m] = (float)atof(tokens[m].c_str());
						matrix.transpose();
						offset.x = matrix[12];
						offset.y = matrix[13];
						offset.z = matrix[14];
						SrQuat quat(matrix);
						SrVec euler = quat.getEuler();
						rotx = euler[0];
						roty = euler[1];
						rotz = euler[2];					
						orderVec.push_back("Y");
						orderVec.push_back("X");
						orderVec.push_back("Z");
					}
					if (infoNodeName == "translate")
					{
						if (!hasTranslate) // need to distinguish between "origin" and "translate" nodes. For now, assume that the first one is the only valid one 
						{
							std::string offsetString;
							xml_utils::xml_translate(&offsetString, infoNode->getTextContent());
							std::vector<std::string> tokens;
							vhcl::Tokenize(offsetString, tokens, " \n");
							offset.x = (float)atof(tokens[0].c_str()) * scale;
							offset.y = (float)atof(tokens[1].c_str()) * scale;
							offset.z = (float)atof(tokens[2].c_str()) * scale;
							hasTranslate = true;
						}

						
					}
					if (infoNodeName == "rotate")
					{
						DOMNamedNodeMap* rotateAttr = infoNode->getAttributes();
						
						DOMNode* sidNode = rotateAttr->getNamedItem(BML::BMLDefs::ATTR_SID);
						std::string sidAttr;
						xml_utils::xml_translate(&sidAttr, sidNode->getNodeValue());

						if (sidAttr.substr(0, 11) == "jointOrient")
						{
							std::string jointOrientationString;
							xml_utils::xml_translate(&jointOrientationString, infoNode->getTextContent());
							std::vector<std::string> tokens;
							vhcl::Tokenize(jointOrientationString, tokens, " \n");
							float finalValue;
							for (int tokenizeC = 0; tokenizeC < 4; tokenizeC++)
								finalValue = (float)atof(tokens[tokenizeC].c_str());
							if (sidAttr == "jointOrientX") jorientx = finalValue;
							if (sidAttr == "jointOrientY") jorienty = finalValue;
							if (sidAttr == "jointOrientZ") jorientz = finalValue;
							if (orderVec.size() != 3)
								orderVec.push_back(sidAttr.substr(11, 1));
						}
						if (sidAttr.substr(0, 6) == "rotate")
						{
							std::string rotationString;
							xml_utils::xml_translate(&rotationString, infoNode->getTextContent());
							std::vector<std::string> tokens;
							vhcl::Tokenize(rotationString, tokens, " \n");
							float finalValue;
							for (int tokenizeC = 0; tokenizeC < 4; tokenizeC++)
								finalValue = (float)atof(tokens[tokenizeC].c_str());
							if (sidAttr == "rotateX") rotx = finalValue;
							if (sidAttr == "rotateY") roty = finalValue;
							if (sidAttr == "rotateZ") rotz = finalValue;
							if (orderVec.size() != 3)
								orderVec.push_back(sidAttr.substr(6, 1));
						}
						if (sidAttr.substr(0, 8) == "rotation")
						{
							std::string rotationString;
							xml_utils::xml_translate(&rotationString, infoNode->getTextContent());
							std::vector<std::string> tokens;
							vhcl::Tokenize(rotationString, tokens, " \n");
							float finalValue;
							for (int tokenizeC = 0; tokenizeC < 4; tokenizeC++)
								finalValue = (float)atof(tokens[tokenizeC].c_str());
							if (sidAttr == "rotationX") rotx = finalValue;
							if (sidAttr == "rotationY") roty = finalValue;
							if (sidAttr == "rotationZ") rotz = finalValue;
							if (orderVec.size() != 3)
								orderVec.push_back(sidAttr.substr(8, 1));
						}
					}
					infoCurNode = infoCurNode->getNextSibling();
				}
				order = getRotationOrder(orderVec);
				if (order == -1)
				{
					if (orderVec.size() == 0)
					{
						order = 321;
					}
					else
					{
						LOG("COLLADA Parser: skeleton joint has invalid rotations.");
					}
				}

				SrMat rotMat;
				rotx *= float(M_PI) / 180.0f;
				roty *= float(M_PI) / 180.0f;
				rotz *= float(M_PI) / 180.0f;
				sr_euler_mat(order, rotMat, rotx, roty, rotz);
				SrMat jorientMat;
				jorientx *= float(M_PI) / 180.0f;
				jorienty *= float(M_PI) / 180.0f;
				jorientz *= float(M_PI) / 180.0f;
				sr_euler_mat(order, jorientMat, jorientx, jorienty, jorientz);
				joint->offset(offset);
				SrMat finalRotMat = rotMat;
				SrQuat quat = SrQuat(rotMat);
				SkJointQuat* jointQuat = joint->quat();
				jointQuat->prerot(quat);
				SrQuat jorientQ = SrQuat(jorientMat);
				jointQuat->orientation(jorientQ);

				DOMNode* geometryNode = ParserOpenCOLLADA::getNode("instance_geometry", childNode, 0, 1);
				if (geometryNode)	// might need to add support for rotation as well later when it happens
				{
					DOMNamedNodeMap* geometryNodeAttr = geometryNode->getAttributes();
					DOMNode* urlAttr = geometryNodeAttr->getNamedItem(BML::BMLDefs::ATTR_URL);
					std::string sidAttr;
					xml_utils::xml_translate(&sidAttr, urlAttr->getNodeValue());
					sidAttr = sidAttr.substr(1);
					//LOG("translate: %f, %f, %f", offset.x, offset.y, offset.z);
					//LOG("instance_geometry: %s", sidAttr.c_str());
					SrModel* newModel = new SrModel();
					newModel->name = SrString(sidAttr.c_str());
					newModel->translate(offset);
					if (!parent)
					{
						LOG("No parent for geometry '%s', geometry will be ignored...", (const char*) newModel->name);
						delete newModel;
					}
					else
					{
						parent->visgeo(newModel);
					}
					
				}
				DOMNode* materialNode = ParserOpenCOLLADA::getNode("bind_material", childNode, 0, 2);
				if (materialNode)
				{
					DOMNode* techniqueCommonNode = ParserOpenCOLLADA::getNode("technique_common", materialNode);
					if (techniqueCommonNode)
					{
						//const DOMNodeList* materialList = techniqueCommonNode->getChildNodes();
						DOMNode* materialCurNode = techniqueCommonNode->getFirstChild();
						while (materialCurNode)
						//for (unsigned int ml = 0; ml < materialList->getLength(); ml++)
						{
							//DOMNode* childNode = materialList->item(ml);
							DOMNode* childNode = materialCurNode;
							std::string nodeName;
							xml_utils::xml_translate(&nodeName, childNode->getNodeName());
							if (nodeName == "instance_material")
							{
								DOMNamedNodeMap* materialAttr = childNode->getAttributes();
								DOMNode* symbolNode = materialAttr->getNamedItem(BML::BMLDefs::ATTR_SYMBOL);
								std::string materialName;
								xml_utils::xml_translate(&materialName, symbolNode->getNodeValue());

								DOMNode* targetNode = materialAttr->getNamedItem(BML::BMLDefs::ATTR_TARGET);
								std::string targetNameString;
								xml_utils::xml_translate(&targetNameString, targetNode->getNodeValue());
								std::string targetName = "";
								if (targetNameString.length() > 0)
									targetName = targetNameString.substr(1);
								//if (materialId2Name.find(targetName) == materialId2Name.end() && targetName != "")
									//materialId2Name.insert(std::make_pair(targetName, materialName));
								if (materialId2Name.find(materialName) == materialId2Name.end() && materialName != "")
									materialId2Name.insert(std::make_pair(materialName, targetName));

							}
							materialCurNode = materialCurNode->getNextSibling();
						}
					}
				}

				parseJoints(curNode, skeleton, motion, scale, order, materialId2Name, joint, hasRootJoint);
			}
			else if (typeAttr == "NODE" || tempMaterialNode)
			{
				DOMNode* translateNode = ParserOpenCOLLADA::getNode("translate", childNode, 0, 1);
				SrVec offset;
				if (translateNode)
				{
					std::string offsetString;
					xml_utils::xml_translate(&offsetString, translateNode->getTextContent());
					std::vector<std::string> tokens;
					vhcl::Tokenize(offsetString, tokens, " \n");
					offset.x = (float)atof(tokens[0].c_str()) * scale;
					offset.y = (float)atof(tokens[1].c_str()) * scale;
					offset.z = (float)atof(tokens[2].c_str()) * scale;
				}
				DOMNode* geometryNode = ParserOpenCOLLADA::getNode("instance_geometry", childNode, 0, 1);
				if (geometryNode)	// might need to add support for rotation as well later when it happens
				{
					DOMNamedNodeMap* geometryNodeAttr = geometryNode->getAttributes();
					DOMNode* urlAttr = geometryNodeAttr->getNamedItem(BML::BMLDefs::ATTR_URL);
					std::string sidAttr;
					xml_utils::xml_translate(&sidAttr, urlAttr->getNodeValue());
					sidAttr = sidAttr.substr(1);
					//LOG("translate: %f, %f, %f", offset.x, offset.y, offset.z);
					//LOG("instance_geometry: %s", sidAttr.c_str());
					SrModel* newModel = new SrModel();
					newModel->name = SrString(sidAttr.c_str());
					newModel->translate(offset);
					if (!parent)
					{
						LOG("No parent for geometry '%s', geometry will be ignored...", (const char*) newModel->name);
						delete newModel;
					}
					else
					{
						parent->visgeo(newModel);
					}
					
				}
				DOMNode* materialNode = ParserOpenCOLLADA::getNode("bind_material", childNode, 0, 2);
				if (materialNode)
				{
					DOMNode* techniqueCommonNode = ParserOpenCOLLADA::getNode("technique_common", materialNode);
					if (techniqueCommonNode)
					{
						//const DOMNodeList* materialList = techniqueCommonNode->getChildNodes();
						DOMNode* materialCurNode = techniqueCommonNode->getFirstChild();
						while (materialCurNode)
						//for (unsigned int ml = 0; ml < materialList->getLength(); ml++)
						{
							//DOMNode* childNode = materialList->item(ml);
							DOMNode* childNode = materialCurNode;
							std::string nodeName;
							xml_utils::xml_translate(&nodeName, childNode->getNodeName());
							if (nodeName == "instance_material")
							{
								DOMNamedNodeMap* materialAttr = childNode->getAttributes();
								DOMNode* symbolNode = materialAttr->getNamedItem(BML::BMLDefs::ATTR_SYMBOL);
								std::string materialName;
								xml_utils::xml_translate(&materialName, symbolNode->getNodeValue());

								DOMNode* targetNode = materialAttr->getNamedItem(BML::BMLDefs::ATTR_TARGET);
								std::string targetNameString;
								xml_utils::xml_translate(&targetNameString, targetNode->getNodeValue());
								std::string targetName = "";
								if (targetNameString.length() > 0)
									targetName = targetNameString.substr(1);
 								//if (materialId2Name.find(targetName) == materialId2Name.end() && targetName != "")
 									//materialId2Name.insert(std::make_pair(targetName, materialName));
								if (materialId2Name.find(materialName) == materialId2Name.end() && materialName != "")
									materialId2Name.insert(std::make_pair(materialName, targetName));

							}
							materialCurNode = materialCurNode->getNextSibling();
						}
					}
				}
				parseJoints(curNode, skeleton, motion, scale, order, materialId2Name, parent, hasRootJoint);
			}
			else
				parseJoints(curNode, skeleton, motion, scale, order, materialId2Name, parent, hasRootJoint);
		}

		curNode = curNode->getNextSibling();
	}
}

void ParserOpenCOLLADA::parseLibraryAnimations( DOMNode* node, SkSkeleton& skeleton, SkMotion& motion, float scale, int& order, bool zaxis )
{
	SkChannelArray& skChannels = skeleton.channels();
	motion.init(skChannels);
	SkChannelArray& motionChannels = motion.channels();
	SkChannelArray channelsForAdjusting;

	std::map<std::string, ColladaFloatArray > floatArrayMap;
	std::map<std::string, ColladaSampler > samplerMap;
	std::vector<ColladChannel> channelSamplerNameMap;

	std::map<std::string,std::vector<std::string> > jointRotationOrderMap;

	//const DOMNodeList* list = node->getChildNodes();
	DOMNode* curNode = node->getFirstChild();
	// load all array of floats with corresponding channel names and sample rates
	while (curNode)
	//for (unsigned int i = 0; i < list->getLength(); i++)
	{
		//DOMNode* node1 = list->item(i);	
		DOMNode* node1 = curNode;
		std::string node1Name;
		xml_utils::xml_translate(&node1Name, node1->getNodeName());
		if (node1Name == "animation")
		{			
			parseNodeAnimation(node1, floatArrayMap, scale, samplerMap, channelSamplerNameMap, skeleton);
		}
		curNode = curNode->getNextSibling();
	}

	std::vector<ColladChannel>::iterator mi;
	for ( mi  = channelSamplerNameMap.begin();
		  mi != channelSamplerNameMap.end();
		  mi++)
	{
		ColladChannel& colChannel = *mi;
		SkJoint* joint = skeleton.search_joint(colChannel.targetJointName.c_str());
		if (!joint) continue; // joint does not exist in the skeleton
		if (samplerMap.find(colChannel.sourceName) == samplerMap.end()) continue; // sampler does not exist
		ColladaSampler& sampler = samplerMap[colChannel.sourceName];
		if (floatArrayMap.find(sampler.inputName) == floatArrayMap.end() || floatArrayMap.find(sampler.outputName) == floatArrayMap.end()) 
			continue; // no float array

		ColladaFloatArray& inFloatArray = floatArrayMap[sampler.inputName];
		ColladaFloatArray& outFloatArray = floatArrayMap[sampler.outputName];

		// insert frames for the motion
		if (motion.frames() == 0)
		{
			for (unsigned int frameCt = 0; frameCt < inFloatArray.floatArray.size(); frameCt++)
			{
				motion.insert_frame(frameCt, inFloatArray.floatArray[frameCt]);
				for (int postureCt = 0; postureCt < motion.posture_size(); postureCt++)
					motion.posture(frameCt)[postureCt] = 0.0f;										
			}								
		}		
		if (colChannel.targetType == "matrix")
		{
			int stride = 16;
			SrMat tran;
			std::string jName = colChannel.targetJointName;	
	//		SrQuat rotateOffset;
	// 		if (zaxis && jName == skeleton.root()->name())
	//		{
	//			// rotate by 90 degree				
	//			SrVec xaxis(1, 0, 0);
	//			SrQuat adjust(xaxis, 3.14159f / -2.0f);
	//			rotateOffset = adjust;	
	//		}
			for (int frameCt = 0; frameCt < motion.frames(); frameCt++)
			{
				SrMat tran;				
				for (int m = 0; m < stride; m++)
				{
					int idx = frameCt*stride+m;
					if (idx >= (int) outFloatArray.floatArray.size())
						break;
					tran[m] = outFloatArray.floatArray[idx];
				}
				tran.transpose();				
				SrVec offset;												
				offset.x = tran[12];
				offset.y = tran[13];
				offset.z = tran[14];
 				//if (joint)
 				//	offset -= joint->offset();
				//offset = offset*joint->quat()->prerot();
				SrQuat quat(tran);	
	//			quat = rotateOffset*quat;
	//			offset = offset*rotateOffset;
				//quat = joint->quat()->prerot().inverse()*quat;
				int channelId = -1;
				// put in translation
				std::string strTranslate = "translate";
				channelId = getMotionChannelId(motionChannels,jName,strTranslate);
				if (channelId >= 0)
				{
					for (int k=0;k<3;k++)
						motion.posture(frameCt)[channelId+k] = offset[k];
				}
				// put in rotation
				std::string strRotation = "rotateX";
				channelId = getMotionChannelId(motionChannels,jName,strRotation);
				if (channelId >= 0)
				{
					for (int k=0;k<4;k++)
						motion.posture(frameCt)[channelId+k] = quat.getData(k);
				}										
			}
		}
		else
		{
			int channelId = getMotionChannelId(motionChannels, colChannel.targetJointName,colChannel.targetType);									
			if (channelId >= 0)
			{
				int stride = outFloatArray.stride;
				for (int frameCt = 0; frameCt < motion.frames(); frameCt++)
				{
					for (int strideCt = 0; strideCt < stride; strideCt++)
					{
						int idx = frameCt*stride + strideCt;
						if (idx >= (int)outFloatArray.floatArray.size()) continue;
						motion.posture(frameCt)[channelId + strideCt] = outFloatArray.floatArray[idx];
					}
				}
			}	

			std::string jointName = colChannel.targetJointName;
			std::string channelType = colChannel.targetType;
			if (channelType == "rotateX" || channelType == "rotateY" || channelType == "rotateZ")
			{
				if (channelsForAdjusting.search(jointName.c_str(), SkChannel::Quat) < 0)
					channelsForAdjusting.add(jointName.c_str(), SkChannel::Quat);
				if (jointRotationOrderMap.find(jointName) == jointRotationOrderMap.end())
				{
					std::vector<std::string> emptyString;
					jointRotationOrderMap[jointName] = emptyString;
				}
				std::string rotationOrder = channelType.substr(channelType.size()-1,1);
				jointRotationOrderMap[jointName].push_back(rotationOrder);
			}
			if (channelType == "translate")
			{
				channelsForAdjusting.add(jointName.c_str(), SkChannel::XPos);
				channelsForAdjusting.add(jointName.c_str(), SkChannel::YPos);
				channelsForAdjusting.add(jointName.c_str(), SkChannel::ZPos);
			}
			if (channelType == "translateX")
				channelsForAdjusting.add(jointName.c_str(), SkChannel::XPos);
			if (channelType == "translateY")
				channelsForAdjusting.add(jointName.c_str(), SkChannel::YPos);
			if (channelType == "translateZ")
				channelsForAdjusting.add(jointName.c_str(), SkChannel::ZPos);
		}

	}	

	// remap the euler angles to quaternion if necessary
	std::vector<int> quatIndices;	
	int rootIdx = -1;
	std::map<std::string,std::vector<std::string> >::iterator vi;
	for (vi  = jointRotationOrderMap.begin();
		vi != jointRotationOrderMap.end();
		vi++)
	{
		std::string jointName = (*vi).first;
		//LOG("joint name = %s",jointName.c_str());
		int channelID = motionChannels.search(jointName,SkChannel::Quat);
		if (channelID != -1)
		{
			int quatIdx = motionChannels.float_position(channelID);
			if (zaxis && jointName == skeleton.root()->jointName())
				rootIdx = quatIdx;
			quatIndices.push_back(quatIdx);
		}		
	}

	for (int frameCt = 0; frameCt < motion.frames(); frameCt++)
	{
		for (size_t i = 0; i < quatIndices.size(); i++)
		{
			int quatId = quatIndices[i];
			float rotx = motion.posture(frameCt)[quatId + 0] / scale;
			float roty = motion.posture(frameCt)[quatId + 1] / scale;
			float rotz = motion.posture(frameCt)[quatId + 2] / scale;
			//LOG("rotx = %f, roty = %f, rotz = %f",rotx,roty,rotz);
			rotx *= float(M_PI) / 180.0f;
			roty *= float(M_PI) / 180.0f;
			rotz *= float(M_PI) / 180.0f;
			SrMat mat;
			sr_euler_mat(order, mat, rotx, roty, rotz);			
			SrQuat quat = SrQuat(mat);			
			motion.posture(frameCt)[quatId + 0] = quat.w;
			motion.posture(frameCt)[quatId + 1] = quat.x;
			motion.posture(frameCt)[quatId + 2] = quat.y;
			motion.posture(frameCt)[quatId + 3] = quat.z;
			//LOG("w = %f, x = %f, y = %f, z = %f",quat.w, quat.x, quat.y, quat.z);
		}

		if (zaxis) // rotate the root joints depending on up axis
		{
			std::string rootName = skeleton.root()->jointName();
			SrVec newPos;
			SrQuat newQuatRot;
			SrVec xaxis(1, 0, 0);
			SrQuat adjust(xaxis, 3.14159f / -2.0f);
			int channelID = motionChannels.search(rootName,SkChannel::Quat);
			if (channelID != -1)
			{
				int quatId = motionChannels.float_position(channelID);
				newQuatRot.w = motion.posture(frameCt)[quatId + 0];
				newQuatRot.x = motion.posture(frameCt)[quatId + 1];
				newQuatRot.y = motion.posture(frameCt)[quatId + 2];
				newQuatRot.z = motion.posture(frameCt)[quatId + 3];
				newQuatRot = adjust*newQuatRot;
				motion.posture(frameCt)[quatId + 0] = newQuatRot.w;
				motion.posture(frameCt)[quatId + 1] = newQuatRot.x;
				motion.posture(frameCt)[quatId + 2] = newQuatRot.y;
				motion.posture(frameCt)[quatId + 3] = newQuatRot.z;
			}

			for (int k=0;k<3;k++)
			{
				int chanType = SkChannel::XPos + k; 				
				int xID = motionChannels.search(rootName,(SkChannel::Type)chanType);
				if (xID)
					newPos[k] = motion.posture(frameCt)[motionChannels.float_position(xID)];
			}			
			newPos = newPos*adjust;
			for (int k=0;k<3;k++)
			{
				int chanType = SkChannel::XPos + k; 	
				int xID = motionChannels.search(rootName,(SkChannel::Type)chanType);
				if (xID)
					motion.posture(frameCt)[motionChannels.float_position(xID)] = newPos[k];
			}
		}
	}

	

	double duration = double(motion.duration());
	motion.synch_points.set_time(0.0, duration / 3.0, duration / 2.0, duration / 2.0, duration / 2.0, duration * 2.0/3.0, duration);
	motion.compress();
	// now there's adjust for the channels by default
	//animationPostProcessByChannels(skeleton, motion, channelsForAdjusting);
	animationPostProcess(skeleton,motion);
}

void ParserOpenCOLLADA::parseNodeAnimation( DOMNode* node1, std::map<std::string, ColladaFloatArray > &floatArrayMap, float scale, std::map<std::string, ColladaSampler > &samplerMap, std::vector<ColladChannel> &channelSamplerNameMap, SkSkeleton &skeleton )
{
	std::string idAttr = getNodeAttributeString(node1,BML::BMLDefs::ATTR_ID);
	//const DOMNodeList* list1 = node1->getChildNodes();	
	DOMNode* curNode1 = node1->getFirstChild();	
	while (curNode1)
	//for (unsigned int j = 0; j < list1->getLength(); j++)
	{
		//DOMNode* node2 = list1->item(j);
		DOMNode* node2 = curNode1;
		std::string node2Name;
		xml_utils::xml_translate(&node2Name, node2->getNodeName());
		if (node2Name == "source")
		{							
			std::string sourceIdAttr = getNodeAttributeString(node2,BML::BMLDefs::ATTR_ID);
			//const DOMNodeList* list2 = node2->getChildNodes();
			DOMNode* curNode2 = node2->getFirstChild();
			while (curNode2)
			//for (unsigned int k = 0; k < list2->getLength(); k++)
			{
				//DOMNode* node3 = list2->item(k);
				DOMNode* node3 = curNode2;
				std::string node3Name;
				xml_utils::xml_translate(&node3Name, node3->getNodeName());
				// parse float array
				if (node3Name == "float_array")
				{							
					int counter = getNodeAttributeInt(node3,BML::BMLDefs::ATTR_COUNT); 
					std::string nodeID = getNodeAttributeString(node3, BML::BMLDefs::ATTR_ID);
					std::string arrayString;
					xml_utils::xml_translate(&arrayString, node3->getTextContent());
					std::vector<std::string> tokens;
					vhcl::Tokenize(arrayString, tokens, " \n");		
					if (floatArrayMap.find(sourceIdAttr) == floatArrayMap.end())
					{
						floatArrayMap[sourceIdAttr] = ColladaFloatArray();
					}
					ColladaFloatArray& colFloatArray = floatArrayMap[sourceIdAttr];
					std::vector<float>& floatArray = colFloatArray.floatArray;
					floatArray.resize(counter);						
					for (int m=0;m<counter;m++)
					{
						float v = (float)atof(tokens[m].c_str());
						floatArray[m] = v * scale;
					}												
					DOMNode* accessorNode = getNode("accessor",node2);
					if (accessorNode)
					{
						colFloatArray.stride = getNodeAttributeInt(accessorNode,BML::BMLDefs::ATTR_STRIDE);
						DOMNode* paramNode = getNode("param",accessorNode);
						if (paramNode)
						{
							colFloatArray.accessorParam = getNodeAttributeString(paramNode,BML::BMLDefs::ATTR_NAME);
						}
					}
				}
				curNode2 = curNode2->getNextSibling();
			}
		}
		else if (node2Name == "sampler")
		{
			std::string samplerID = getNodeAttributeString(node2,BML::BMLDefs::ATTR_ID);
			if (samplerMap.find(samplerID) == samplerMap.end())
			{
				samplerMap[samplerID] = ColladaSampler();
			}
			ColladaSampler& sampler = samplerMap[samplerID];
			//const DOMNodeList* list2 = node2->getChildNodes();
			DOMNode* curNode2 = node2->getFirstChild();
			while (curNode2)
			//for (unsigned int k = 0; k < list2->getLength(); k++)
			{
				//DOMNode* node3 = list2->item(k);
				DOMNode* node3 = curNode2;
				std::string node3Name;
				xml_utils::xml_translate(&node3Name, node3->getNodeName());						
				if (node3Name == "input")
				{
					std::string attrSemantic = getNodeAttributeString(node3,BML::BMLDefs::ATTR_SEMANTIC);
					if (attrSemantic == "INPUT")
					{
						sampler.inputName = getNodeAttributeString(node3,BML::BMLDefs::ATTR_SOURCE).substr(1);	
						//LOG("sampelr input name = %s",sampler.inputName.c_str());
					}
					else if (attrSemantic == "OUTPUT")
					{
						sampler.outputName = getNodeAttributeString(node3,BML::BMLDefs::ATTR_SOURCE).substr(1);
						//LOG("sampelr input name = %s",sampler.outputName.c_str());
					}
				}
				curNode2 = curNode2->getNextSibling();
			}
		} 
		else if (node2Name == "channel")
		{
			std::string target = getNodeAttributeString(node2,BML::BMLDefs::ATTR_TARGET);
			std::string source = getNodeAttributeString(node2,BML::BMLDefs::ATTR_SOURCE);
			channelSamplerNameMap.push_back(ColladChannel());
			ColladChannel& colChannel = channelSamplerNameMap.back();
			colChannel.sourceName = source.substr(1);
			//LOG("colChannel input name = %s",colChannel.sourceName.c_str());
			std::vector<std::string> tokens;
			vhcl::Tokenize(target, tokens, "/.");
			//LOG("token1 = %s, token2 = %s",tokens[0].c_str(),tokens[1].c_str());
			std::string jname = tokens[0];
			SkJoint* joint = skeleton.search_joint(jname.c_str());
			if (joint) jname = joint->jointName();
			colChannel.targetJointName = jname;
			colChannel.targetType = tokens[1];					
		}
		else if (node2Name == "animation") // for some reasons this kind of recursion does happen in some OpenCollada files
		{
			parseNodeAnimation(node2,floatArrayMap,scale,samplerMap,channelSamplerNameMap,skeleton);
		}
		curNode1 = curNode1->getNextSibling();
	}
}

void ParserOpenCOLLADA::animationPostProcess(SkSkeleton& skeleton, SkMotion& motion)
{
	ParserOpenCOLLADA::animationPostProcessByChannels(skeleton, motion, motion.channels());
}

void ParserOpenCOLLADA::animationPostProcessByChannels(SkSkeleton& skeleton, SkMotion& motion, SkChannelArray& motionChannels)
{
	int numChannel = motionChannels.size(); 
	std::vector<std::pair<SkJoint*, int> > jointIndexMap;
	for (int j = 0; j < numChannel; j++)
	{
		SkChannel& chan = motionChannels[j];
		const std::string chanName = motionChannels.name(j);
		SkChannel::Type chanType = chan.type;
		SkJoint* joint = skeleton.search_joint(chanName.c_str());
		if (!joint)
		{
			jointIndexMap.push_back(std::pair<SkJoint*, int>());
			continue;
		}

		int id = motion.channels().search(chanName.c_str(), chanType);
		int dataId = motion.channels().float_position(id);
		if (dataId < 0)
		{
			jointIndexMap.push_back(std::pair<SkJoint*, int>());
			continue;
		}
		jointIndexMap.push_back(std::pair<SkJoint*, int>(joint, dataId));
	}

	
	for (int i = 0; i < motion.frames(); i++)
	{
		for (int j = 0; j < numChannel; j++)
		{
			SkChannel& chan = motionChannels[j];
		/*	const std::string chanName = motionChannels.name(j);
			SkChannel::Type chanType = chan.type;
			SkJoint* joint = skeleton.search_joint(chanName.c_str());
			if (!joint)
				continue;

			int id = motion.channels().search(chanName.c_str(), chanType);
			int dataId = motion.channels().float_position(id);
			if (dataId < 0)
				continue;
				*/
			SkJoint* joint = jointIndexMap[j].first;
			int dataId = jointIndexMap[j].second;
			if (!joint || dataId < 0)
				continue;
			SkChannel::Type chanType = chan.type;
			SrVec offset = joint->offset();					
			if (chanType == SkChannel::XPos)
			{
				float v = motion.posture(i)[dataId];
				motion.posture(i)[dataId] = v - offset[0];
			}
			if (chanType == SkChannel::YPos)
			{
				float v = motion.posture(i)[dataId];
				motion.posture(i)[dataId] = v - offset[1];
			}
			if (chanType == SkChannel::ZPos)
			{
				float v = motion.posture(i)[dataId];
				motion.posture(i)[dataId] = v - offset[2];
			}


			if (chanType == SkChannel::Quat)
			{
				SrQuat globalQuat = SrQuat(motion.posture(i)[dataId], motion.posture(i)[dataId + 1], motion.posture(i)[dataId + 2], motion.posture(i)[dataId + 3]);
				SrQuat preQuat = joint->quat()->prerot();
				//LOG("prerot = %f",preQuat.angle());
				SrQuat localQuat = preQuat.inverse() * globalQuat;
				motion.posture(i)[dataId] = localQuat.w;
				motion.posture(i)[dataId + 1] = localQuat.x;
				motion.posture(i)[dataId + 2] = localQuat.y;
				motion.posture(i)[dataId + 3] = localQuat.z;
			}

		}
	}
}


int ParserOpenCOLLADA::getMotionChannelId( SkChannelArray& channels, std::string& jointName, std::string& targetType )
{
	int id = -1;
	int dataId = -1;	
	SkChannel::Type chanType;
	if (targetType == "translate")
	{
		chanType = SkChannel::XPos;
		id = channels.search(jointName, chanType);
		dataId = channels.float_position(id);
	}
	else if (targetType == "translateX")
	{
		chanType = SkChannel::XPos;
		id = channels.search(jointName, chanType);
		dataId = channels.float_position(id);
	}
	else if (targetType == "translateY")
	{
		chanType = SkChannel::YPos;
		id = channels.search(jointName, chanType);
		dataId = channels.float_position(id);
	}
	else if (targetType == "translateZ")
	{
		chanType = SkChannel::ZPos;
		id = channels.search(jointName, chanType);
		dataId = channels.float_position(id);
	}
	else if (targetType == "rotateX")
	{
		chanType = SkChannel::Quat;
		id = channels.search(jointName, chanType);
		dataId = channels.float_position(id);
	}
	else if (targetType == "rotateY")
	{
		chanType = SkChannel::Quat;
		id = channels.search(jointName, chanType);
		dataId = channels.float_position(id);
		if (id >= 0)
			dataId += 1;
	}
	else if (targetType == "rotateZ")
	{
		chanType = SkChannel::Quat;
		id = channels.search(jointName, chanType);
		dataId = channels.float_position(id);
		if (id >= 0)
			dataId += 2;
	}
	return dataId;	
}

int ParserOpenCOLLADA::getMotionChannelId(SkChannelArray& mChannels, const std::string& sourceName)
{
	int id = -1;
	int dataId = -1;
	std::string sourceNameCopy = sourceName;
	std::string jName = tokenize(sourceNameCopy, ".-");
	SkChannel::Type chanType;
	
	if (sourceName.find("translate") != std::string::npos)
	{
		chanType = SkChannel::XPos;
		id = mChannels.search(jName, chanType);
		dataId = mChannels.float_position(id);
	}
	else if (sourceName.find("translateX") != std::string::npos)
	{
		chanType = SkChannel::XPos;
		id = mChannels.search(jName, chanType);
		dataId = mChannels.float_position(id);
	}
	else if (sourceName.find("translateY") != std::string::npos)
	{
		chanType = SkChannel::YPos;
		id = mChannels.search(jName, chanType);
		dataId = mChannels.float_position(id);
	}
	else if (sourceName.find("translateZ") != std::string::npos)
	{
		chanType = SkChannel::ZPos;
		id = mChannels.search(jName, chanType);
		dataId = mChannels.float_position(id);
	}
	else if (sourceName.find("rotateX") != std::string::npos)
	{
		chanType = SkChannel::Quat;
		id = mChannels.search(jName, chanType);
		dataId = mChannels.float_position(id);
	}
	else if (sourceName.find("rotateY") != std::string::npos)
	{
		chanType = SkChannel::Quat;
		id = mChannels.search(jName, chanType);
		dataId = mChannels.float_position(id);
		if (id >= 0)
			dataId += 1;
	}
	else if (sourceName.find("rotateZ") != std::string::npos)
	{
		chanType = SkChannel::Quat;
		id = mChannels.search(jName, chanType);
		dataId = mChannels.float_position(id);
		if (id >= 0)
			dataId += 2;
	}
	return dataId;
}

std::string ParserOpenCOLLADA::tokenize(std::string& str, const std::string& delimiters, int mode)
{
	// Skip delimiters at beginning.
	std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	// Find first "non-delimiter".
	std::string::size_type pos     = str.find_first_of(delimiters, lastPos);

	if (std::string::npos != pos || std::string::npos != lastPos)
	{
		// Found a token, add it to the vector.
		std::string return_string = str.substr(lastPos, pos - lastPos);
		std::string::size_type lastPos = str.find_first_not_of(delimiters, pos);
		if (mode == 1)
		{
			if (std::string::npos == lastPos)	str = "";
			else								str = str.substr(lastPos, str.size() - lastPos);
		}
		return return_string;
	}
	else
		return "";
}

int ParserOpenCOLLADA::getRotationOrder(std::vector<std::string> orderVec)
{
	if (orderVec.size() == 3)
	{
		if (orderVec[0] == "X" && orderVec[1] == "Y" && orderVec[2] == "Z")
			return 321;
		if (orderVec[0] == "X" && orderVec[1] == "Z" && orderVec[2] == "Y")
			return 231;
		if (orderVec[0] == "Y" && orderVec[1] == "X" && orderVec[2] == "Z")
			return 312;
		if (orderVec[0] == "Y" && orderVec[1] == "Z" && orderVec[2] == "X")
			return 132;
		if (orderVec[0] == "Z" && orderVec[1] == "X" && orderVec[2] == "Y")
			return 213;
		if (orderVec[0] == "Z" && orderVec[1] == "Y" && orderVec[2] == "X")
			return 123;
	}
	else if (orderVec.size() == 2)
	{
		if (orderVec[0] == "X" && orderVec[1] == "Y")
			return 321;
		if (orderVec[0] == "X" && orderVec[1] == "Z")
			return 231;
		if (orderVec[0] == "Y" && orderVec[1] == "X")
			return 312;
		if (orderVec[0] == "Y" && orderVec[1] == "Z")
			return 132;
		if (orderVec[0] == "Z" && orderVec[1] == "X")
			return 213;
		if (orderVec[0] == "Z" && orderVec[1] == "Y")
			return 123;
	}
	else if (orderVec.size() == 1)
	{
		if (orderVec[0] == "X")
			return 321;
		if (orderVec[0] == "Y")
			return 312;
		if (orderVec[0] == "Z")
			return 213;
	}
	return -1;
}
