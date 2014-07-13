#include "ParserOpenCOLLADA.h"
#include "sr/sr_euler.h"
#include <boost/version.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <algorithm>
#include <cctype>
#include <string>
#include <sbm/BMLDefs.h>
#ifdef EMBER_SB_GPU
#include <sbm/GPU/SbmTexture.h>
#endif
#include <sb/SBScene.h>
#include <sb/SBAssetManager.h>
#include <sb/SBSkeleton.h>
#include <sb/SBMotion.h>
#include <sbm/sbm_deformable_mesh.h>


class TextLineSplitter
{
public:

    TextLineSplitter( const size_t max_line_len );

    ~TextLineSplitter();

    void            SplitLine( const char *line,
                               const char sep_char = ','
                             );

    inline size_t   NumTokens( void ) const
    {
        return mNumTokens;
    }

    const char *    GetToken( const size_t token_idx ) const
    {
        assert( token_idx < mNumTokens );
        return mTokens[ token_idx ];
    }

private:
    const size_t    mStorageSize;

    char           *mBuff;
    char          **mTokens;
    size_t          mNumTokens;

    inline void     ResetContent( void )
    {
        memset( mBuff, 0, mStorageSize );
        // mark all items as empty:
        memset( mTokens, 0, mStorageSize * sizeof( char* ) );
        // reset counter for found items:
        mNumTokens = 0L;
    }
};

TextLineSplitter::TextLineSplitter( const size_t max_line_len ):
    mStorageSize ( max_line_len + 1L )
{
    // allocate memory
    mBuff   = new char  [ mStorageSize ];
    mTokens = new char* [ mStorageSize ];

    ResetContent();
}

TextLineSplitter::~TextLineSplitter()
{
    delete [] mBuff;
    delete [] mTokens;
}


void TextLineSplitter::SplitLine( const char *line,
                                  const char sep_char   /* = ',' */
                                )
{
    assert( sep_char != '\0' );

    ResetContent();
    strncpy( mBuff, line, mStorageSize );

    size_t idx       = 0L; // running index for characters

    do
    {
        assert( idx < mStorageSize );

        const char chr = line[ idx ]; // retrieve current character

        if( mTokens[ mNumTokens ] == NULL )
        {
            mTokens[ mNumTokens ] = &mBuff[ idx ];
        } // if

        if( chr == sep_char || chr == '\0' )
        { // item or line finished
            // overwrite separator with a 0-terminating character:
            mBuff[ idx ] = '\0';
            // count-up items:
            mNumTokens ++;
        } // if

    } while( line[ idx++ ] );
}

#ifdef EMBER_SB_OTHERFMT

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

void ParserOpenCOLLADA::getChildNodes(const std::string& nodeName, DOMNode* node, std::vector<DOMNode*>& childs )
{
	int type = node->getNodeType();
	std::string name;
	xml_utils::xml_translate(&name, node->getNodeName());
	std::string value;
	xml_utils::xml_translate(&value, node->getNodeValue());
	if (name == nodeName && node->getNodeType() ==  DOMNode::ELEMENT_NODE)
		childs.push_back(node);

	//if (!node->hasChildNodes()) // no child nodes
	//	return;
	DOMNode* child = NULL;
	//const DOMNodeList* list = node->getChildNodes();
	DOMNode* curNode = node->getFirstChild();
	//for (unsigned int c = 0; c < list->getLength(); c++)
	while (curNode)
	{
		getChildNodes(nodeName,curNode, childs);
		curNode = curNode->getNextSibling();
		//child = getNode(nodeName, list->item(c));
		//if (child)
		//	break;
	}
	//return child;

}

#endif

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

#ifdef EMBER_SB_OTHERFMT

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


XercesDOMParser* ParserOpenCOLLADA::getParserFromFile( std::string fileName )
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
		return NULL;
	}

	XercesDOMParser* parser = new XercesDOMParser();
	parser->setValidationScheme(XercesDOMParser::Val_Always);
	parser->setDoNamespaces(true);    // optional

	ErrorHandler* errHandler = (ErrorHandler*) new HandlerBase();
	parser->setErrorHandler(errHandler);

	try 
	{
		std::string filebasename = boost::filesystem::basename(fileName);
		parser->parse(fileName.c_str());		
		return parser;
	}
	catch (const XMLException& toCatch) 
	{
		std::string message = "";
		xml_utils::xml_translate(&message, toCatch.getMessage());
		LOG("Exception message is: %s", message.c_str());
		delete parser;
		return NULL;
	}
	catch (const DOMException& toCatch) {
		std::string message = "";
		xml_utils::xml_translate(&message, toCatch.msg);
		LOG("Exception message is: %s", message.c_str());
		delete parser;
		return NULL;
	}
	catch (...) {
		LOG("Unexpected Exception in ParserOpenCOLLADA::getNode()");
		delete parser;
		return NULL;
	}
	
}

DOMNode* ParserOpenCOLLADA::getNode(const std::string& nodeName, std::string fileName, int maximumDepth)
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
		return NULL;
	}

	XercesDOMParser* parser = new XercesDOMParser();
	parser->setValidationScheme(XercesDOMParser::Val_Always);
	parser->setDoNamespaces(true);    // optional

	ErrorHandler* errHandler = (ErrorHandler*) new HandlerBase();
	parser->setErrorHandler(errHandler);

	try 
	{
		std::string filebasename = boost::filesystem::basename(fileName);
		parser->parse(fileName.c_str());
		DOMDocument* doc = parser->getDocument();
		
		int depth = 0;
		//delete parser;
		return getNode(nodeName, doc, depth, maximumDepth);
	}
	catch (const XMLException& toCatch) 
	{
		std::string message = "";
		xml_utils::xml_translate(&message, toCatch.getMessage());
		LOG("Exception message is: %s", message.c_str());
		return NULL;
	}
	catch (const DOMException& toCatch) {
		std::string message = "";
		xml_utils::xml_translate(&message, toCatch.msg);
		LOG("Exception message is: %s", message.c_str());
		return NULL;
	}
		catch (...) {
		LOG("Unexpected Exception in ParserOpenCOLLADA::getNode()");
		return NULL;
	}

	delete parser;
	delete errHandler;
	return NULL;
}


DOMNode* ParserOpenCOLLADA::getNode(const std::string& nodeName, std::string fileName)
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
		return NULL;
	}

	XercesDOMParser* parser = new XercesDOMParser();
	parser->setValidationScheme(XercesDOMParser::Val_Always);
	parser->setDoNamespaces(true);    // optional

	ErrorHandler* errHandler = (ErrorHandler*) new HandlerBase();
	parser->setErrorHandler(errHandler);

	try 
	{
		std::string filebasename = boost::filesystem::basename(fileName);
		parser->parse(fileName.c_str());
		DOMDocument* doc = parser->getDocument();
		
		return getNode(nodeName, doc);
	}
	catch (const XMLException& toCatch) 
	{
		std::string message = "";
		xml_utils::xml_translate(&message, toCatch.getMessage());
		LOG("Exception message is: %s", message.c_str());
		return NULL;
	}
	catch (const DOMException& toCatch) {
		std::string message = "";
		xml_utils::xml_translate(&message, toCatch.msg);
		LOG("Exception message is: %s", message.c_str());
		return NULL;
	}
		catch (...) {
		LOG("Unexpected Exception in ParserOpenCOLLADA::getNode()");
		return NULL;
	}

	delete parser;
	delete errHandler;
	return NULL;
}

void ParserOpenCOLLADA::nodeStr(const XMLCh* s, std::string& out)
{
	xml_utils::xml_translate(&out, s);
}

void ParserOpenCOLLADA::parseLibraryControllers(DOMNode* node, DeformableMesh* mesh, float scaleFactor, std::string jointPrefix)
{
	boost::char_separator<char> sep(" \n");
	//const DOMNodeList* list = node->getChildNodes();
	DOMNode* curNode = node->getFirstChild();
	while (curNode)
		//for (unsigned int c = 0; c < list->getLength(); c++)
	{
		//DOMNode* node = list->item(c);
		DOMNode* node = curNode;
		int type = node->getNodeType();
		std::string name;
		nodeStr(node->getNodeName(), name);
		std::string value;
		nodeStr(node->getNodeValue(), value);
		boost::algorithm::to_lower(name);
		if (name == "controller")
		{
			DOMNamedNodeMap* attributes = node->getAttributes();
			DOMNode* idNode = attributes->getNamedItem(BML::BMLDefs::ATTR_ID);
			if (!idNode)	continue;
			std::string skinId;
			nodeStr(idNode->getNodeValue(), skinId);
			if (node->hasChildNodes())
			{
				//const DOMNodeList* childrenList = node->getChildNodes();
				DOMNode* childrenCurNode = node->getFirstChild();
				while (childrenCurNode)
					//for (unsigned int cc = 0; cc < childrenList->getLength(); cc++)
				{
					//DOMNode* childNode = childrenList->item(cc);
					DOMNode* childNode = childrenCurNode;
					std::string childName;
					nodeStr(childNode->getNodeName(), childName);
					if (childName == "skin")	// parsing skinning weights
					{
						DOMNamedNodeMap* skinAttributes = childNode->getAttributes();			
						DOMNode* skinNode = skinAttributes->getNamedItem(BML::BMLDefs::ATTR_SOURCE);	
						std::string skinSource;
						nodeStr(skinNode->getNodeValue(), skinSource);
						skinSource = skinSource.substr(1, skinSource.size() - 1);
						SkinWeight* skinWeight = new SkinWeight();

						
						skinWeight->sourceMesh = skinSource;

						// further for children
						//const DOMNodeList* childListOfSkin = childNode->getChildNodes();
						DOMNode* childOfSkinCurNode = childNode->getFirstChild();
						while (childOfSkinCurNode)
							//for (unsigned int cSkin = 0; cSkin < childListOfSkin->getLength(); cSkin++)
						{
							//DOMNode* childNodeOfSkin = childListOfSkin->item(cSkin);
							DOMNode* childNodeOfSkin = childOfSkinCurNode;
							std::string childNameOfSkin;
							nodeStr(childNodeOfSkin->getNodeName(), childNameOfSkin);
							std::string bindJointName = skinSource + "-skin-joints";
							std::string bindWeightName = skinSource + "-skin-weights";
							std::string bindPoseMatName = skinSource + "-skin-bind_poses";

							if (childNameOfSkin == "bind_shape_matrix")
							{
								std::string tokenBlock;
								nodeStr(childNodeOfSkin->getTextContent(), tokenBlock);
								float bindShapeMat[16];

								boost::tokenizer<boost::char_separator<char> > tokens(tokenBlock, sep);
								int i = 0;
								for (boost::tokenizer<boost::char_separator<char> >::iterator it = tokens.begin();
									it != tokens.end();
									++it)
								{
									bindShapeMat[i] = (float)atof((*it).c_str());
									i++;
									if (i >= 16)
										break;
								}
								//for (int i = 0; i < 16; i++)
								//	bindShapeMat[i] = (float)atof(tokenize(tokenBlock).c_str());

								skinWeight->bindShapeMat.set(bindShapeMat);
								skinWeight->bindShapeMat.transpose();
								skinWeight->bindShapeMat.setl4(skinWeight->bindShapeMat.get_translation()*scaleFactor);
							}
							if (childNameOfSkin == "source")
							{
								DOMNamedNodeMap* sourceAttributes = childNodeOfSkin->getAttributes();
								std::string sourceId;
								nodeStr(sourceAttributes->getNamedItem(BML::BMLDefs::ATTR_ID)->getNodeValue(), sourceId);								
								boost::algorithm::to_lower(sourceId);
								bool isBindJointName = (sourceId.find("joints") != std::string::npos);
								bool isBindWeights = (sourceId.find("weights") != std::string::npos);
								bool isBindPoseMatrices = (sourceId.find("bind_poses") != std::string::npos || sourceId.find("matrices") != std::string::npos);

								//DOMNodeList* realContentNodeList = childNodeOfSkin->getChildNodes();
								DOMNode* realContentCurNode = childNodeOfSkin->getFirstChild();
								while (realContentCurNode)
									//for (unsigned int cSource = 0; cSource < realContentNodeList->getLength(); cSource++)
								{
									//DOMNode* realContentNode = realContentNodeList->item(cSource);
									DOMNode* realContentNode = realContentCurNode;
									std::string realNodeName;
									nodeStr(realContentNode->getNodeName(), realNodeName);		

									std::string tokenBlock;
									nodeStr(realContentNode->getTextContent(), tokenBlock);

									boost::tokenizer<boost::char_separator<char> > tokens(tokenBlock, sep);
									int matCounter = 0;
									float bindPosMat[16];
									SrMat newMat;									
									for (boost::tokenizer<boost::char_separator<char> >::iterator it = tokens.begin();
										it != tokens.end();
										++it)
									{
										//if ( sourceId == bindJointName && realNodeName == "Name_array") // joint name
										if ( isBindJointName && realNodeName == "Name_array") // joint name
										{
											std::string jointName = (*it);
											// check if the joint name start with the pre-fix and remove the prefix
											if ((*it).compare(0, jointPrefix.size(), jointPrefix) == 0)
											{
												jointName.erase(0, jointPrefix.size());
											}
											//cout << "joint name = " << jointName << endl;	
											skinWeight->infJointName.push_back(jointName);
										}
										//if ( sourceId == bindWeightName && realNodeName == "float_array") // joint weights
										if ( isBindWeights && realNodeName == "float_array") // joint weights
											skinWeight->bindWeight.push_back((float)atof((*it).c_str()));
										//if ( sourceId == bindPoseMatName && realNodeName == "float_array") // bind pose matrices
										if ( isBindPoseMatrices && realNodeName == "float_array") // bind pose matrices
										{
											bindPosMat[matCounter] = (float)atof((*it).c_str());
											matCounter ++;
											if (matCounter == 16)
											{
												matCounter = 0;
												newMat.set(bindPosMat);													
												newMat.transpose();
												//newMat = skinWeight->bindShapeMat.inverse()*newMat;
												SrVec newTran = newMat.get_translation()*scaleFactor;
												newMat.setl4(newTran);
												skinWeight->bindPoseMat.push_back(newMat);
											}
										}
									}
									realContentCurNode = realContentCurNode->getNextSibling();
								}								
							} // end of if (childNameOfSkin == "source")
							if (childNameOfSkin == "vertex_weights")
							{
								//DOMNodeList* indexNodeList = childNodeOfSkin->getChildNodes();
								DOMNode* indexCurNode = childNodeOfSkin->getFirstChild();
								while (indexCurNode)
									//for (unsigned int cVertexWeights = 0; cVertexWeights < indexNodeList->getLength(); cVertexWeights++)
								{
									//DOMNode* indexNode = indexNodeList->item(cVertexWeights);
									DOMNode* indexNode = indexCurNode;
									std::string indexNodeName;
									nodeStr(indexNode->getNodeName(), indexNodeName);
									std::string tokenBlock;
									nodeStr(indexNode->getTextContent(), tokenBlock);

									boost::tokenizer<boost::char_separator<char> > tokens(tokenBlock, sep);

									if (indexNodeName == "vcount")
									{
										for (boost::tokenizer<boost::char_separator<char> >::iterator it = tokens.begin();
											it != tokens.end();
											++it)
										{
											skinWeight->numInfJoints.push_back(atoi((*it).c_str()));
										}
									}
									else if (indexNodeName == "v")
									{
										for (boost::tokenizer<boost::char_separator<char> >::iterator it = tokens.begin();
											it != tokens.end();
											++it)
										{
											skinWeight->jointNameIndex.push_back(atoi((*it).c_str()));
											it++;
											skinWeight->weightIndex.push_back(atoi((*it).c_str()));
										}
									}
									else
									{
										indexCurNode = indexCurNode->getNextSibling();
										continue;
									}
									indexCurNode = indexCurNode->getNextSibling();
								}
							}
							childOfSkinCurNode = childOfSkinCurNode->getNextSibling();
						}
						if (mesh)
							mesh->skinWeights.push_back(skinWeight);
					} // end of if (childName == "skin")
					if (childName == "morph")	// parsing morph targets
					{
						DOMNamedNodeMap* morphAttributes = childNode->getAttributes();			
						DOMNode* morphNode = morphAttributes->getNamedItem(BML::BMLDefs::ATTR_SOURCE);	
						std::string morphName;
						nodeStr(morphNode->getNodeValue(), morphName);
						morphName = morphName.substr(1, morphName.size() - 1);
						std::string morphFullName = morphName + "-morph";						
						// futhur for children
						//const DOMNodeList* childListOfMorph = childNode->getChildNodes();
						DOMNode* childOfMorphCurNode = childNode->getFirstChild();
						while (childOfMorphCurNode)
							//for (unsigned int cMorph = 0; cMorph < childListOfMorph->getLength(); cMorph++)
						{
							//DOMNode* childNodeOfMorph = childListOfMorph->item(cMorph);
							DOMNode* childNodeOfMorph = childOfMorphCurNode;
							std::string childNameOfMorph;
							nodeStr(childNodeOfMorph->getNodeName(), childNameOfMorph);
							if (childNameOfMorph == "source")
							{
								//const DOMNodeList* childListOfSource = childNodeOfMorph->getChildNodes();
								DOMNode* childOfSourceCurNode = childNodeOfMorph->getFirstChild();
								while (childOfSourceCurNode)
									//for (size_t cMorphSource = 0; cMorphSource < childListOfSource->getLength(); cMorphSource++)
								{
									//DOMNode* childNodeOfSource = childListOfSource->item(cMorphSource);
									DOMNode* childNodeOfSource = childOfSourceCurNode;
									std::string childNameOfSource;
									nodeStr(childNodeOfSource->getNodeName(), childNameOfSource);
									if (childNameOfSource == "IDREF_array" || childNameOfSource == "Name_array")
									{
										std::vector<std::string> refMesh;
										refMesh.push_back(morphName);
										std::string tokenBlock;
										nodeStr(childNodeOfSource->getTextContent(), tokenBlock);
										boost::char_separator<char> sep2(" \n");
										boost::tokenizer<boost::char_separator<char> > tokens(tokenBlock, sep2);
										for (boost::tokenizer<boost::char_separator<char> >::iterator it = tokens.begin();
											it != tokens.end();
											++it)
										{
											refMesh.push_back((*it));
										}
										mesh->morphTargets.insert(make_pair(morphFullName, refMesh));
									}
									childOfSourceCurNode = childOfSourceCurNode->getNextSibling();
								}
							}
							childOfMorphCurNode = childOfMorphCurNode->getNextSibling();
						}
					} // end of if (childName == "morph")
					childrenCurNode = childrenCurNode->getNextSibling();
				}
			}
		}
		curNode = curNode->getNextSibling();
	}
	/*
	// cache the joint names for each skin weight
	if (char_p && char_p->dMesh_p)
	{	
		char_p->dMesh_p->skeletonName = char_p->getSkeleton()->getName();
		for (size_t x = 0; x < char_p->dMesh_p->skinWeights.size(); x++)
		{
			SkinWeight* skinWeight = char_p->dMesh_p->skinWeights[x];
			for (size_t j = 0; j < skinWeight->infJointName.size(); j++)
			{
				std::string& jointName = skinWeight->infJointName[j];
				SkJoint* curJoint = char_p->getSkeleton()->search_joint(jointName.c_str());
				skinWeight->infJoint.push_back(curJoint); // NOTE: If joints are added/removed during runtime, this list will contain stale data
			}
		}
	}
	*/
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
// 								if (materialId2Name.find(targetName) == materialId2Name.end() && targetName != "")
// 									materialId2Name.insert(std::make_pair(targetName, materialName));
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
// 								if (materialId2Name.find(targetName) == materialId2Name.end() && targetName != "")
// 									materialId2Name.insert(std::make_pair(targetName, materialName));
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
// 			SrQuat rotateOffset;
// 			if (zaxis && jName == skeleton.root()->name())
// 			{
// 				// rotate by 90 degree				
// 				SrVec xaxis(1, 0, 0);
// 				SrQuat adjust(xaxis, 3.14159f / -2.0f);
// 				rotateOffset = adjust;	
// 			}
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
// 				quat = rotateOffset*quat;
// 				offset = offset*rotateOffset;
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

void ParserOpenCOLLADA::parseLibraryAnimations2(DOMNode* node, SkSkeleton& skeleton, SkMotion& motion, float scale, int& order)
{
	SkChannelArray& skChannels = skeleton.channels();
	motion.init(skChannels);
	SkChannelArray& motionChannels = motion.channels();
	SkChannelArray channelsForAdjusting;
	
	std::map<std::string,std::vector<SrMat> > jointTransformMap;
	std::map<std::string,std::vector<std::string> > jointRotationOrderMap;

	//const DOMNodeList* list = node->getChildNodes();
	DOMNode* curNode = node->getFirstChild();
	while (curNode)
	//for (unsigned int i = 0; i < list->getLength(); i++)
	{
		//DOMNode* node1 = list->item(i);
		DOMNode* node1 = curNode;
		std::string node1Name;
		xml_utils::xml_translate(&node1Name, node1->getNodeName());
		if (node1Name == "animation")
		{
			DOMNamedNodeMap* animationAttr = node1->getAttributes();
			DOMNode* idNode = animationAttr->getNamedItem(BML::BMLDefs::ATTR_ID);
			std::string idAttr;
			if (idNode)
				xml_utils::xml_translate(&idAttr, idNode->getNodeValue());
			std::string jointName = tokenize(idAttr, ".-");	
			std::string channelType = tokenize(idAttr, "_");
			//LOG("joint name = %s, channel type = %s",jointName.c_str(),channelType.c_str());
			int numTimeInput = -1;
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
					DOMNamedNodeMap* sourceAttr = node2->getAttributes();
					DOMNode* sourceIdNode = sourceAttr->getNamedItem(BML::BMLDefs::ATTR_ID);					
					std::string sourceIdAttr;
					xml_utils::xml_translate(&sourceIdAttr, sourceIdNode->getNodeValue());
					size_t pos = sourceIdAttr.find_last_of("-");
					std::string op = sourceIdAttr.substr(pos + 1);
					if (sourceIdAttr.find("input") != std::string::npos) op = "input";
					else if (sourceIdAttr.find("output") != std::string::npos) op = "output";

					//const DOMNodeList* list2 = node2->getChildNodes();
					DOMNode* curNode2 = node2->getFirstChild();
					while (curNode2)
					//for (unsigned int k = 0; k < list2->getLength(); k++)
					{
						//DOMNode* node3 = list2->item(k);
						DOMNode* node3 = curNode2;
						std::string node3Name;
						xml_utils::xml_translate(&node3Name, node3->getNodeName());
						if (node3Name == "float_array")
						{
							DOMNamedNodeMap* arrayAttr = node3->getAttributes();
							DOMNode* arrayCountNode = arrayAttr->getNamedItem(BML::BMLDefs::ATTR_COUNT);
							std::string temp;
							xml_utils::xml_translate(&temp, arrayCountNode->getNodeValue());
							int counter = atoi(temp.c_str());
							std::string arrayString;
							xml_utils::xml_translate(&arrayString, node3->getTextContent());
							//std::vector<std::string> tokens;
							//vhcl::Tokenize(arrayString, tokens, " \n");
							TextLineSplitter spl(counter);
							spl.SplitLine(arrayString.c_str(), ' ');
						
							if (op == "input")
							{
								numTimeInput = counter;
								std::string jName = tokenize(sourceIdAttr, ".-");
								if (motion.frames() == 0)
								{
									for (int frameCt = 0; frameCt < counter; frameCt++)
									{
										//motion.insert_frame(frameCt, (float)atof(tokens[frameCt].c_str()));
										motion.insert_frame(frameCt, (float)atof(spl.GetToken(frameCt)));
										for (int postureCt = 0; postureCt < motion.posture_size(); postureCt++)
											motion.posture(frameCt)[postureCt] = 0.0f;										
									}								
								}
/*								if (motion.frames() < counter)
								{
									for (int frameCt = 0; frameCt < counter; frameCt++)
									{
										float k = (float)atof(tokenize(arrayString).c_str());
										float keyTime = motion.keytime(frameCt);
										if (keyTime > k)
										{
											bool flag = motion.insert_frame(frameCt, k);
											for (int postureCt = 0; postureCt < motion.posture_size(); postureCt++)
												motion.posture(frameCt)[postureCt] = 0.0f;
										}
									}
								}
*/
							}

							if (op == "output")
							{
								if (sourceIdAttr.find("transform") != std::string::npos)
								{
									// load a transform matrix
									int stride = 16;
									SrMat tran;
									std::string jName = tokenize(sourceIdAttr, ".-");
									SkJoint* joint = skeleton.search_joint(jName.c_str());
									jointTransformMap[jName] = std::vector<SrMat>();
									jointTransformMap[jName].resize(motion.frames());
									for (int frameCt = 0; frameCt < motion.frames(); frameCt++)
									{
										for (int m = 0; m < stride; m++)
										{
											//tran[m] = (float)atof(tokens[frameCt*stride+m].c_str());
											tran[m] = (float)atof(spl.GetToken(frameCt*stride+m));
										}
 										tran.transpose();
										jointTransformMap[jName][frameCt] = tran;
										
										SrVec offset;												
										offset.x = tran[12];
										offset.y = tran[13];
										offset.z = tran[14];
										if (joint)
											offset -= joint->offset();
										SrQuat quat(tran);
										SrVec euler = quat.getEuler();		
										int channelId = -1;
										// put in translation
										channelId = getMotionChannelId(motionChannels,jName+"-translate");
										if (channelId >= 0)
										{
											for (int k=0;k<3;k++)
												motion.posture(frameCt)[channelId+k] = offset[k];
										}
										// put in rotation
										channelId = getMotionChannelId(motionChannels,jName+"-rotateX");
										if (channelId >= 0)
										{
											for (int k=0;k<4;k++)
												motion.posture(frameCt)[channelId+k] = quat.getData(k);
										}										
									}
								}								
								else
								{
									int channelId = getMotionChannelId(motionChannels, sourceIdAttr);									
									if (channelId >= 0)
									{
										int stride = counter / numTimeInput;
										for (int frameCt = 0; frameCt < motion.frames(); frameCt++)
										{
											for (int strideCt = 0; strideCt < stride; strideCt++)
											{
												//if (tokens.size() <= (unsigned int)frameCt) continue;
												if (spl.NumTokens() <= (unsigned int)frameCt) continue;

												//float v = (float)atof(tokens[frameCt*stride+strideCt].c_str());
												float v = (float)atof(spl.GetToken(frameCt*stride+strideCt));
												motion.posture(frameCt)[channelId + strideCt] = v * scale;
											}
										}
									}
								}								
							}
						}
						curNode2 = curNode2->getNextSibling();
					}
				}
				curNode1 = curNode1->getNextSibling();
			}
		}
		curNode = curNode->getNextSibling();
	}

// 	for (std::map<std::string,std::vector<SrMat> >::iterator mi  = jointTransformMap.begin();
// 		 mi != jointTransformMap.end();
// 		 mi++)
// 	{
// 		std::string jName = mi->first;
// 		std::vector<SrMat>& childMatList = mi->second;
// 		SkJoint* joint = skeleton.search_joint(jName.c_str());
// 		if (!joint) continue;
// 		SkJoint* parent = joint->parent();
// 		if (parent)
// 		{
// 			std::vector<SrMat>& parentMatList = jointTransformMap[parent->name()];
// 			for (unsigned int i=0;i<childMatList.size();i++)
// 			{
// 				SrMat tran = childMatList[i];//*parentMatList[i].inverse();
// 				SrVec offset;												
// 				offset.x = 0;//tran[12];
// 				offset.y = 0;//tran[13];
// 				offset.z = 0;//tran[14];
// 				SrQuat quat(tran);
// 				SrVec euler = quat.getEuler();
// 				int channelId = -1;
// 				// put in translation
// 				channelId = getMotionChannelId(motionChannels,jName+"-translate");
// 				if (channelId >= 0)
// 				{
// 					for (int k=0;k<3;k++)
// 						motion.posture(i)[channelId+k] = offset[k];
// 				}
// 				// put in rotation
// 				channelId = getMotionChannelId(motionChannels,jName+"-rotateX");
// 				if (channelId >= 0)
// 				{
// 					for (int k=0;k<3;k++)
// 						motion.posture(i)[channelId+k] = euler[k];
// 				}	
// 			}
// 		}
// 	}



	// now transfer the motion euler data to quaternion data
	
	std::vector<int> quatIndices;
	/*
	for (int i = 0; i < motionChannels.size(); i++)
	{
		SkChannel& chan = motionChannels[i];
		
		if (chan.type == SkChannel::Quat)
		{
			int id = motionChannels.float_position(i);
			quatIndices.push_back(id);
		}
	}
	*/
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
			quatIndices.push_back(motionChannels.float_position(channelID));
		}		
	}


	
	for (int frameCt = 0; frameCt < motion.frames(); frameCt++)
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
		}

	double duration = double(motion.duration());
	motion.synch_points.set_time(0.0, duration / 3.0, duration / 2.0, duration / 2.0, duration / 2.0, duration * 2.0/3.0, duration);
	motion.compress();	
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

std::string ParserOpenCOLLADA::getGeometryType(std::string idString)
{
	boost::algorithm::to_lower(idString);
	size_t found = idString.find("position");
	if (found != std::string::npos)
		return "positions";

	found = idString.find("binormal");
	if (found != std::string::npos)
		return "";

	found = idString.find("normal");
	if (found != std::string::npos)
		return "normals";

	found = idString.find("uv");
	if (found != std::string::npos)
		return "texcoords";

	found = idString.find("map");
	if (found != std::string::npos)
		return "texcoords";

//	LOG("ParserOpenCOLLADA::getGeometryType WARNING: type %s not supported!", idString.c_str());	
	return "";
}

void ParserOpenCOLLADA::parseLibraryGeometries( DOMNode* node, const char* file, SrArray<SrMaterial>& M, SrStringArray& mnames,std::map<std::string, std::string>& materialId2Name, std::map<std::string,std::string>& mtlTexMap, std::map<std::string,std::string>& mtlTexBumpMap, std::map<std::string,std::string>& mtlTexSpecularMap,std::vector<SrModel*>& meshModelVec, float scale )
{
	std::map<std::string,bool> vertexSemantics;
	//const DOMNodeList* list = node->getChildNodes();
	DOMNode* curNode = node->getFirstChild();
	while (curNode)
	//for (unsigned int c = 0; c < list->getLength(); c++)
	{
		//LOG("parseLibraryGeometries, iter %d", c);
		//DOMNode* node = list->item(c);
		DOMNode* node = curNode;
		std::string nodeName;
		xml_utils::xml_translate(&nodeName ,node->getNodeName());	
		if (nodeName == "geometry")
		{
			//LOG("nodeName = geometry");
			std::map<std::string, std::string> verticesArrayMap;
			std::map<std::string, std::vector<SrVec> > floatArrayMap;	

			SrModel* newModel = new SrModel();
			DOMNamedNodeMap* nodeAttr = node->getAttributes();
			DOMNode* nameNode = nodeAttr->getNamedItem(BML::BMLDefs::ATTR_NAME);
			std::string nameAttr = "";
			DOMNode* idNode = nodeAttr->getNamedItem(BML::BMLDefs::ATTR_ID);
			std::string idString;
			xml_utils::xml_translate(&idString, idNode->getNodeValue());

			if (nameNode)
				xml_utils::xml_translate(&nameAttr, nameNode->getNodeValue());

			newModel->name = SrString(idString.c_str());
			DOMNode* meshNode = ParserOpenCOLLADA::getNode("mesh", node);
			if (!meshNode)	continue;
			DOMNode* meshCurNode = meshNode->getFirstChild();
			while (meshCurNode)
			//for (unsigned int c1 = 0; c1 < meshNode->getChildNodes()->getLength(); c1++)
			{
				//DOMNode* node1 = meshNode->getChildNodes()->item(c1);
				DOMNode* node1 = meshCurNode;
				std::string nodeName1;
				xml_utils::xml_translate(&nodeName1, node1->getNodeName());
				
				if (nodeName1 == "source")
				{
					//LOG("nodeName1 = source");
					DOMNamedNodeMap* sourceAttr = node1->getAttributes();
					DOMNode* idNode = sourceAttr->getNamedItem(BML::BMLDefs::ATTR_ID);
					std::string idString;
					xml_utils::xml_translate(&idString, idNode->getNodeValue());
					std::string sourceID = idString;
					size_t pos = idString.find_last_of("-");
					std::string tempString = idString.substr(pos + 1);
					idString = tempString;
					std::transform(idString.begin(), idString.end(), idString.begin(), ::tolower);
					std::string idType = getGeometryType(idString);
					// below is a faster way to parse all the data, have potential bug
					DOMNode* floatNode = ParserOpenCOLLADA::getNode("float_array", node1);			
					DOMNode* countNode = floatNode->getAttributes()->getNamedItem(BML::BMLDefs::ATTR_COUNT);				
					DOMNode* accessorNode = ParserOpenCOLLADA::getNode("accessor", node1);
					DOMNode* strideNode = accessorNode->getAttributes()->getNamedItem(BML::BMLDefs::ATTR_STRIDE);
					int count = xml_utils::xml_translate_int(countNode->getNodeValue());
					int stride = xml_utils::xml_translate_int(strideNode->getNodeValue());				
					//int strde = atoi()		
					std::string floatString;
					xml_utils::xml_translate(&floatString, floatNode->getTextContent());

					boost::char_separator<char> sep(" \n");
					boost::tokenizer<boost::char_separator<char> > tokens(floatString, sep);
					int i = 0;
					floatArrayMap[sourceID] = std::vector<SrVec>();			
					for (boost::tokenizer<boost::char_separator<char> >::iterator it = tokens.begin();
							it != tokens.end();
							)
					{
						int nstep = stride > 3 ? 3 : stride;
						SrVec tempV;
						for (int k=0;k<nstep;k++)
						{
							tempV[k] = (float)atof((*it).c_str());
							it++;
						}
						floatArrayMap[sourceID].push_back(tempV);
					}
					/*
					std::vector<std::string> tokens;
					vhcl::Tokenize(floatString, tokens, " \n");
					
					floatArrayMap[sourceID] = std::vector<SrVec>();					
					for (int i=0; i< count ; i+= stride)
					{
						int nstep = stride > 3 ? 3 : stride;		
						SrVec tempV;
						for (int k=0;k<nstep;k++)
						{
							tempV[k] = (float)atof(tokens[i+k].c_str());
						}
						floatArrayMap[sourceID].push_back(tempV);
					}
					*/


#if 0
					if (idType == "positions")
						count /= 3;
					if (idType == "normals")
						count /= 3;
					if (idType == "texcoords")
						count /= 2;
					std::string floatString;
					xml_utils::xml_translate(&floatString, floatNode->getTextContent());
					std::vector<std::string> tokens;
					vhcl::Tokenize(floatString, tokens, " \n");
					int index = 0;
					for (int i = 0; i < count; i++)
					{
						if (idType == "positions")
						{
							newModel->V.push();
							newModel->V.top().x = (float)atof(tokens[index++].c_str());
							newModel->V.top().y = (float)atof(tokens[index++].c_str());
							newModel->V.top().z = (float)atof(tokens[index++].c_str());
						}
						if (idType == "normals")
						{
							newModel->N.push();
							newModel->N.top().x = (float)atof(tokens[index++].c_str());
							newModel->N.top().y = (float)atof(tokens[index++].c_str());
							newModel->N.top().z = (float)atof(tokens[index++].c_str());
						}
						if (idType == "texcoords")
						{
							newModel->T.push();
							newModel->T.top().x = (float)atof(tokens[index++].c_str());
							newModel->T.top().y = (float)atof(tokens[index++].c_str());
						}
					}
#endif
					// duplication???
					//meshCurNode = meshCurNode->getNextSibling();
				}			
				if (nodeName1 == "vertices")
				{
					//LOG("nodeName1 = vertices");
					vertexSemantics.clear();
					DOMNode* verticeCurNode = node1->getFirstChild();
					while (verticeCurNode)
					//for (unsigned int c2 = 0; c2 < node1->getChildNodes()->getLength(); c2++)
					{
						//DOMNode* inputNode = node1->getChildNodes()->item(c2);
						DOMNode* inputNode = verticeCurNode;
						if (XMLString::compareString(inputNode->getNodeName(), BML::BMLDefs::ATTR_INPUT) == 0)
						{
							DOMNamedNodeMap* inputNodeAttr = inputNode->getAttributes();
							DOMNode* semanticNode = inputNodeAttr->getNamedItem(BML::BMLDefs::ATTR_SEMANTIC);
							std::string inputSemantic;
							xml_utils::xml_translate(&inputSemantic, semanticNode->getNodeValue());
							vertexSemantics[inputSemantic] = true;

							DOMNode* sourceNameNode = inputNodeAttr->getNamedItem(BML::BMLDefs::ATTR_SOURCE);
							std::string sourceName = xml_utils::xml_translate_string(sourceNameNode->getNodeValue());
							setModelVertexSource(sourceName,inputSemantic,newModel,floatArrayMap);
						}
						verticeCurNode = verticeCurNode->getNextSibling();
					}										
				}

				if (nodeName1 == "triangles" || nodeName1 == "polylist" || nodeName1 == "polygons")
				{
					//LOG("nodeName1 = %s", nodeName1.c_str());
					int curmtl = -1;
					DOMNamedNodeMap* nodeAttr1 = node1->getAttributes();
					DOMNode* countNode = nodeAttr1->getNamedItem(BML::BMLDefs::ATTR_COUNT);
					std::string temp;
					xml_utils::xml_translate(&temp, countNode->getNodeValue());
					int count = atoi(temp.c_str());
					DOMNode* materialNode = nodeAttr1->getNamedItem(BML::BMLDefs::ATTR_MATERIAL);
					std::string materialName;
					if (materialNode)
						xml_utils::xml_translate(&materialName, materialNode->getNodeValue());
					std::string materialID = materialName;
					if (materialId2Name.find(materialName) != materialId2Name.end())
						materialID = materialId2Name[materialName];
					curmtl = mnames.lsearch(materialID.c_str());
					//curmtl = mnames.lsearch(materialName.c_str());
					std::map<int, std::string> inputMap;
					int pStride = 0;
					std::vector<int> vcountList;
					DOMNode* triangleCurNode = node1->getFirstChild();
					while (triangleCurNode)
					//for (unsigned int c2 = 0; c2 < node1->getChildNodes()->getLength(); c2++)
					{
						//DOMNode* inputNode = node1->getChildNodes()->item(c2);
						DOMNode* inputNode = triangleCurNode;
						if (XMLString::compareString(inputNode->getNodeName(), BML::BMLDefs::ATTR_INPUT) == 0)
						{
							DOMNamedNodeMap* inputNodeAttr = inputNode->getAttributes();
							DOMNode* semanticNode = inputNodeAttr->getNamedItem(BML::BMLDefs::ATTR_SEMANTIC);
							std::string inputSemantic;
							xml_utils::xml_translate(&inputSemantic, semanticNode->getNodeValue());
							DOMNode* offsetNode = inputNodeAttr->getNamedItem(BML::BMLDefs::ATTR_OFFSET);
							std::string temp;
							xml_utils::xml_translate(&temp, offsetNode->getNodeValue());
							DOMNode* sourceNameNode = inputNodeAttr->getNamedItem(BML::BMLDefs::ATTR_SOURCE);
							std::string sourceName = xml_utils::xml_translate_string(sourceNameNode->getNodeValue());
							int offset = atoi(temp.c_str());
							if (pStride <= offset)	pStride = offset;
							if (inputMap.find(offset) != inputMap.end())	// same offset is wrong
							{
								if (inputSemantic == "VERTEX" || inputSemantic == "NORMAL" || inputSemantic == "TEXCOORD")
									LOG("ParserOpenCOLLADA::parseLibraryGeometries ERR: file not correct.");
							}
							else
							{
								setModelVertexSource(sourceName,inputSemantic,newModel,floatArrayMap);
								// should not allow same input semantic
								bool hasDuplicate = false;
								std::map<int, std::string>::iterator iter = inputMap.begin();
								for (; iter != inputMap.end(); iter++)
								{
									if (iter->second == inputSemantic)
									{
										hasDuplicate = true;
										break;
									}
								}
								if (!hasDuplicate)
									inputMap.insert(std::make_pair(offset, inputSemantic));
								else
									inputMap.insert(std::make_pair(offset, "null"));
							}
						}
						if (XMLString::compareString(inputNode->getNodeName(), BML::BMLDefs::ATTR_VCOUNT) == 0)
						{
							std::string vcountString;
							xml_utils::xml_translate(&vcountString, inputNode->getTextContent());
							std::vector<std::string> tokens;
							vhcl::Tokenize(vcountString, tokens, " \n");
							for (int i = 0; i < count; i++)
								vcountList.push_back(atoi(tokens[i].c_str()));
						}

						triangleCurNode = triangleCurNode->getNextSibling();
					}
					int totalVertex = 0;
					for (size_t i = 0; i < vcountList.size(); i++)
						totalVertex += vcountList[i];

					if (vcountList.size() == 0)
					{
						for (int i = 0; i < count; i++)
							vcountList.push_back(3);
					}

					//DOMNode* pNode = ParserOpenCOLLADA::getNode("p", node1);

					std::string pString = "";
					DOMNode* pNode = node1->getFirstChild();
					while (pNode)
					{
						if (strcmp(xml_utils::xml_translate_string(pNode->getNodeName()).c_str(), "p") == 0)
						{
							pString += xml_utils::xml_translate_string(pNode->getNodeValue());
							pString += " ";
						}
						pNode = pNode->getNextSibling();
					}

					//std::string pString;
					//xml_utils::xml_translate(&pString, pNode->getTextContent());

					/*
					std::vector<std::string> tokens;
					vhcl::Tokenize(pString, tokens, " \n");
					int index = 0;
					for (int i = 0; i < count; i++)
					{
					*/

					pStride += 1;
					boost::char_separator<char> sep(" \n");
					boost::tokenizer<boost::char_separator<char> > tokens(pString, sep);
					boost::tokenizer<boost::char_separator<char> >::iterator it = tokens.begin();
					int index = 0;
					for (int i = 0; i < count; i++)
					{
						std::vector<int> fVec;
						std::vector<int> ftVec;
						std::vector<int> fnVec;
						for (int j = 0; j < vcountList[i]; j++)
						{
							for (int k = 0; k < pStride; k++)
							{
								std::string semantic = inputMap[k];
								if (semantic == "VERTEX")
								{
									if (vertexSemantics.find("POSITION") != vertexSemantics.end())																								
										fVec.push_back(atoi((*it).c_str()));
									if (vertexSemantics.find("NORMAL") != vertexSemantics.end())
										fnVec.push_back(atoi((*it).c_str()));									
								}
								if (semantic == "TEXCOORD")
									ftVec.push_back(atoi((*it).c_str()));

								if (semantic == "NORMAL" && vertexSemantics.find("NORMAL") == vertexSemantics.end())
									fnVec.push_back(atoi((*it).c_str()));
								it++;
								index++;
							}
						}

						// process each polylist
						for (size_t x = 2; x < fVec.size(); x++)
						{
							newModel->F.push().set(fVec[0], fVec[x - 1], fVec[x]);
							newModel->Fm.push() = curmtl;
							if (ftVec.size() > x)
								newModel->Ft.push().set(ftVec[0], ftVec[x - 1], ftVec[x]);
							else
								newModel->Ft.push().set(0, 0, 0);
							if (fnVec.size() > x)
								newModel->Fn.push().set(fnVec[0], fnVec[x - 1], fnVec[x]);
							else
								newModel->Fn.push().set(0, 1, 0);
						}
					}
					/*
					if (tokens.size() != index)
						LOG("ParserOpenCOLLADA::parseLibraryGeometries ERR: parsing <p> list uncorrectly (%s)!", nameAttr.c_str());
						*/
				}
				meshCurNode = meshCurNode->getNextSibling();
			}

			newModel->mtlTextureNameMap = mtlTexMap;
			newModel->mtlNormalTexNameMap = mtlTexBumpMap;
			newModel->mtlSpecularTexNameMap = mtlTexSpecularMap;
			newModel->M = M;
			newModel->mtlnames = mnames;

			newModel->validate();
			newModel->remove_redundant_materials();
//			newModel->remove_redundant_normals();
			newModel->compress();
			meshModelVec.push_back(newModel);

			SrString path = file;
			SrString filename;
			path.extract_file_name(filename);
			SrStringArray paths;
			paths.push ( path );
			for (int i = 0; i < newModel->M.size(); i++)
			{
			   std::string matName = newModel->mtlnames[i];
			   if (newModel->mtlTextureNameMap.find(matName) != newModel->mtlTextureNameMap.end())
			   {
				   ParserOpenCOLLADA::load_texture(SbmTextureManager::TEXTURE_DIFFUSE, newModel->mtlTextureNameMap[matName].c_str(), paths);	   
			   }	
			   if (newModel->mtlNormalTexNameMap.find(matName) != newModel->mtlNormalTexNameMap.end())
			   {
				   ParserOpenCOLLADA::load_texture(SbmTextureManager::TEXTURE_NORMALMAP, newModel->mtlNormalTexNameMap[matName].c_str(), paths);	   
			   }
			   if (newModel->mtlSpecularTexNameMap.find(matName) != newModel->mtlSpecularTexNameMap.end())
			   {
				   //LOG("Load specular map = %s",newModel->mtlSpecularTexNameMap[matName].c_str());
				   ParserOpenCOLLADA::load_texture(SbmTextureManager::TEXTURE_SPECULARMAP, newModel->mtlSpecularTexNameMap[matName].c_str(), paths);	   
			   }
			}
		}
		curNode = curNode->getNextSibling();
	}	
}

void ParserOpenCOLLADA::setModelVertexSource( std::string& sourceName, std::string& semanticName, SrModel* model, VecListMap& vecMap )
{
	std::string srcName = sourceName;
	std::vector<SrVec>* sourceArray = NULL;
	if (srcName[0] == '#') 
	{
		srcName.erase(0,1);
		if (vecMap.find(srcName) != vecMap.end())
			sourceArray = &vecMap[srcName];
	}

	if ( semanticName == "POSITION" && sourceArray && model->V.size() == 0)
	{
		bool printOut = false;
// 		if (sourceName.find("LEyeLashShape-positions") != std::string::npos || sourceName.find("REyelashShape-positions") != std::string::npos)
// 		{
// 			LOG("sourname = %s",sourceName.c_str());
// 			printOut = true;
// 		}
		for (unsigned int i=0;i<sourceArray->size();i++)
		{
// 			if (printOut)
// 			{
// 				SrVec pos = (*sourceArray)[i];
// 				LOG("pos = %f %f %f",pos[0],pos[1],pos[2]);
// 			}
			model->V.push((*sourceArray)[i]);										
		}
	}
	else if (semanticName == "NORMAL" && sourceArray && model->N.size() == 0)
	{
		for (unsigned int i=0;i<sourceArray->size();i++)
		{
			model->N.push((*sourceArray)[i]);										
		}
	}
	else if (semanticName == "TEXCOORD" && sourceArray && model->T.size() == 0)
	{
		for (unsigned int i=0;i<sourceArray->size();i++)
		{
			SrVec ts = (*sourceArray)[i];
			model->T.push(SrVec2(ts[0],ts[1]));										
		}
	}
}

void ParserOpenCOLLADA::load_texture(int type, const char* file, const SrStringArray& paths)
{
	SrString s;
	SrInput in;
	std::string imageFile = file;
	in.init( fopen(file,"r"));
	int i = 0;
	while ( !in.valid() && i < paths.size())
	{
		s = paths[i++];
		s << file;
		imageFile = s;
		in.init ( fopen(s,"r") );
	}
	if (!in.valid()) return;		
	SbmTextureManager& texManager = SbmTextureManager::singleton();
	texManager.loadTexture(type,file,s);	
}


void ParserOpenCOLLADA::parseLibraryMaterials(DOMNode* node, std::map<std::string, std::string>& effectId2MaterialId)
{
	//const DOMNodeList* list = node->getChildNodes();
	DOMNode* curNode = node->getFirstChild();
	while (curNode)
	//for (unsigned int c = 0; c < list->getLength(); c++)
	{
		//DOMNode* node = list->item(c);
		DOMNode* node = curNode;
		std::string nodeName;
		xml_utils::xml_translate(&nodeName, node->getNodeName());
		if (nodeName == "material")
		{
			DOMNamedNodeMap* sourceAttr = node->getAttributes();
			DOMNode* idNode = sourceAttr->getNamedItem(BML::BMLDefs::ATTR_ID);
			std::string materialId;
			xml_utils::xml_translate(&materialId, idNode->getNodeValue());
			DOMNode* meshNode = ParserOpenCOLLADA::getNode("instance_effect", node);
			if (!meshNode)	continue;
			DOMNamedNodeMap* materialSourceAttr = meshNode->getAttributes();
			DOMNode* urlNode = materialSourceAttr->getNamedItem(BML::BMLDefs::ATTR_URL);
			if (!urlNode)	continue;
			std::string urlString;
			xml_utils::xml_translate(&urlString, urlNode->getNodeValue());
			// get ride of the "#" in front, potential bug here if other file has different format
			if (urlString != "")
			{
				std::string effectId = urlString.substr(1);
				if (effectId2MaterialId.find(effectId) == effectId2MaterialId.end())
					effectId2MaterialId.insert(std::make_pair(effectId, materialId));
				else
					LOG("ParserOpenCOLLADA::parseLibraryMaterials ERR: two effects mapped to material %s", materialId.c_str());
			}
		}
		curNode = curNode->getNextSibling();
	}
}

void ParserOpenCOLLADA::parseLibraryImages(DOMNode* node, std::map<std::string, std::string>& pictureId2File, std::map<std::string, std::string>& pictureId2Name)
{
	//const DOMNodeList* list = node->getChildNodes();
	DOMNode* curNode = node->getFirstChild();
	while (curNode)
	//for (unsigned int c = 0; c < list->getLength(); c++)
	{
		//DOMNode* node = list->item(c);
		DOMNode* node = curNode;
		std::string nodeName;
		xml_utils::xml_translate(&nodeName, node->getNodeName());
		if (nodeName == "image")
		{
			DOMNamedNodeMap* imageAttr = node->getAttributes();
			DOMNode* idNode = imageAttr->getNamedItem(BML::BMLDefs::ATTR_ID);
			std::string imageId;
			xml_utils::xml_translate(&imageId, idNode->getNodeValue());
			DOMNode* initFromNode = ParserOpenCOLLADA::getNode("init_from", node);
			std::string imageFile;
			xml_utils::xml_translate(&imageFile, initFromNode->getTextContent());

			DOMNode* nameNode = imageAttr->getNamedItem(BML::BMLDefs::ATTR_NAME);
			std::string imageName = imageId;
			if (nameNode)
				xml_utils::xml_translate(&imageName, nameNode->getTextContent());
			if (pictureId2File.find(imageId) == pictureId2File.end())
			{
				pictureId2File.insert(std::make_pair(imageId, imageFile));
				pictureId2Name.insert(std::make_pair(imageId, imageName));
			}
			else
				LOG("ParserOpenCOLLADA::parseLibraryImages ERR: two image files mapped to same image id %s", imageId.c_str());
		}
		curNode = curNode->getNextSibling();
	}
}

void ParserOpenCOLLADA::parseLibraryEffects( DOMNode* node, std::map<std::string, std::string>&effectId2MaterialId, std::map<std::string, std::string>& materialId2Name, std::map<std::string, std::string>& pictureId2File, std::map<std::string, std::string>& pictureId2Name, SrArray<SrMaterial>& M, SrStringArray& mnames, std::map<std::string,std::string>& mtlTexMap, std::map<std::string,std::string>& mtlTexBumpMap, std::map<std::string,std::string>& mtlTexSpecularMap )
{
	//const DOMNodeList* list = node->getChildNodes();
	DOMNode* curNode = node->getFirstChild();
	while (curNode)
	//for (unsigned int c = 0; c < list->getLength(); c++)
	{
		//DOMNode* node = list->item(c);
		DOMNode* node = curNode;
		std::string nodeName;
		xml_utils::xml_translate(&nodeName, node->getNodeName());
		if (nodeName == "effect")
		{
			DOMNamedNodeMap* effectAttr = node->getAttributes();
			DOMNode* idNode = effectAttr->getNamedItem(BML::BMLDefs::ATTR_ID);
			std::string effectId;
			xml_utils::xml_translate(&effectId, idNode->getNodeValue());
			std::string materialId = effectId2MaterialId[effectId];
			//std::string materialName = materialId2Name[materialId];
			SrMaterial material;
			material.init();
			M.push(material);
			SrString matName(materialId.c_str());
			mnames.push(matName);

			std::vector<DOMNode*> initNodes;
			ParserOpenCOLLADA::getChildNodes("init_from", node, initNodes);
			std::string diffuseTexture, normalTexture, specularTexture;
			diffuseTexture = "";
			normalTexture = "";
			specularTexture = "";
			DOMNode* diffuseNode = ParserOpenCOLLADA::getNode("diffuse",node);
			DOMNode* bumpNode = ParserOpenCOLLADA::getNode("bump",node);
			DOMNode* specularNode = ParserOpenCOLLADA::getNode("specularLevel",node);
			if (diffuseNode)
			{
				DOMNode* texNode = ParserOpenCOLLADA::getNode("texture",diffuseNode);
				if (texNode)
				{
					DOMNamedNodeMap* texAttr = texNode->getAttributes();
					DOMNode* texAttrNode = texAttr->getNamedItem(BML::BMLDefs::ATTR_TEXTURE);			
					std::string texID;
					xml_utils::xml_translate(&texID, texAttrNode->getNodeValue());
					diffuseTexture = texID;


					std::string imageId = diffuseTexture;
					std::string imageFile = pictureId2File[imageId];
					std::string mtlName = mnames.top();
#if (BOOST_VERSION > 104400)
					std::string fileExt = boost::filesystem::extension(imageFile);
#else
					std::string fileExt = boost::filesystem2::extension(imageFile);
#endif
					std::string fileName = boost::filesystem::basename(imageFile);
					if (diffuseTexture.find(imageId) != std::string::npos)
						mtlTexMap[mtlName] = fileName + fileExt;		

				}
				DOMNode* colorNode = ParserOpenCOLLADA::getNode("color", diffuseNode);
				if (colorNode)
				{
					std::string color;
					xml_utils::xml_translate(&color, colorNode->getTextContent());
					std::vector<std::string> tokens;
					vhcl::Tokenize(color, tokens, " \n");
					float w = 1;
					if (tokens.size() == 4)
						w = (float)atof(tokens[3].c_str());
					M.top().diffuse = SrColor((float)atof(tokens[0].c_str()), (float)atof(tokens[1].c_str()), (float)atof(tokens[2].c_str()), w);
				}
			}

			if (bumpNode)
			{
				DOMNode* texNode = ParserOpenCOLLADA::getNode("texture",bumpNode);
				if (texNode)
				{
					DOMNamedNodeMap* texAttr = texNode->getAttributes();
					DOMNode* texAttrNode = texAttr->getNamedItem(BML::BMLDefs::ATTR_TEXTURE);			
					std::string texID;
					xml_utils::xml_translate(&texID, texAttrNode->getNodeValue());
					normalTexture = texID;

					std::string imageId = diffuseTexture;
					std::string imageFile = pictureId2File[imageId];
					std::string mtlName = mnames.top();
#if (BOOST_VERSION > 104400)
					std::string fileExt = boost::filesystem::extension(imageFile);
#else
					std::string fileExt = boost::filesystem2::extension(imageFile);
#endif
					std::string fileName = boost::filesystem::basename(imageFile);
					if (diffuseTexture.find(imageId) != std::string::npos)
						mtlTexBumpMap[mtlName] = fileName + fileExt;	

					M.top().specular = SrColor(0.1f,0.1f,0.1f,1.f);
					M.top().shininess = 20;
				}			
			}

			if (specularNode)
			{
				DOMNode* texNode = ParserOpenCOLLADA::getNode("texture",specularNode);
				if (texNode)
				{
					DOMNamedNodeMap* texAttr = texNode->getAttributes();
					DOMNode* texAttrNode = texAttr->getNamedItem(BML::BMLDefs::ATTR_TEXTURE);			
					std::string texID;
					xml_utils::xml_translate(&texID, texAttrNode->getNodeValue());
					specularTexture = texID;

					std::string imageId = diffuseTexture;
					std::string imageFile = pictureId2File[imageId];
					std::string mtlName = mnames.top();
#if (BOOST_VERSION > 104400)
					std::string fileExt = boost::filesystem::extension(imageFile);
#else
					std::string fileExt = boost::filesystem2::extension(imageFile);
#endif
					std::string fileName = boost::filesystem::basename(imageFile);
					if (diffuseTexture.find(imageId) != std::string::npos)
						mtlTexSpecularMap[mtlName] = fileName + fileExt;	

					M.top().specular = SrColor(0.1f,0.1f,0.1f,1.f);
					M.top().shininess = 20;
				}			
			}
			//DOMNode* initFromNode = ParserOpenCOLLADA::getNode("init_from", node);
			//if (initFromNode)
			// get all textures
			for (unsigned int i=0;i<initNodes.size();i++)
			{
				std::string imageId;
				DOMNode* initFromNode = initNodes[i];
				xml_utils::xml_translate(&imageId, initFromNode->getTextContent());
				std::string imageName = imageId;
				if (pictureId2Name.find(imageId) != pictureId2Name.end())
					imageName = pictureId2Name[imageId];

				std::string imageFile = pictureId2File[imageId];
				SrString mapKaName(imageFile.c_str());
				std::string texFile = (const char*) mapKaName;
				std::string mtlName = mnames.top();
#if (BOOST_VERSION > 104400)
				std::string fileExt = boost::filesystem::extension(texFile);
#else
				std::string fileExt = boost::filesystem2::extension(texFile);
#endif
				std::string fileName = boost::filesystem::basename(texFile);
				if (diffuseTexture.find(imageName) != std::string::npos)
					mtlTexMap[mtlName] = fileName + fileExt;	
				else if (normalTexture.find(imageName) != std::string::npos)
					mtlTexBumpMap[mtlName] = fileName + fileExt;
				else if (specularTexture.find(imageName) != std::string::npos)
					mtlTexSpecularMap[mtlName] = fileName + fileExt;				
			}

			DOMNode* emissionNode = ParserOpenCOLLADA::getNode("emission", node);
			if (emissionNode)
			{
				DOMNode* colorNode = ParserOpenCOLLADA::getNode("color", emissionNode);
				std::string color;
				xml_utils::xml_translate(&color, colorNode->getTextContent());
				std::vector<std::string> tokens;
				vhcl::Tokenize(color, tokens, " \n");
				float w = 1;
				if (tokens.size() == 4)
					w = (float)atof(tokens[3].c_str());
				M.top().emission = SrColor((float)atof(tokens[0].c_str()), (float)atof(tokens[1].c_str()), (float)atof(tokens[2].c_str()), w);
			}

			DOMNode* ambientNode = ParserOpenCOLLADA::getNode("ambient", node);
			if (ambientNode)
			{
				DOMNode* colorNode = ParserOpenCOLLADA::getNode("color", ambientNode);
				std::string color;
				xml_utils::xml_translate(&color, colorNode->getTextContent());
				std::vector<std::string> tokens;
				vhcl::Tokenize(color, tokens, " \n");
				float w = 1;
				if (tokens.size() == 4)
					w = (float)atof(tokens[3].c_str());
				M.top().ambient = SrColor((float)atof(tokens[0].c_str()), (float)atof(tokens[1].c_str()), (float)atof(tokens[2].c_str()), w);
			}
			DOMNode* transparentNode = ParserOpenCOLLADA::getNode("transparent", node);
			if (transparentNode)
			{
				DOMNamedNodeMap* transparentNodeAttr = transparentNode->getAttributes();
				float alpha = 1.f;	
				DOMNode* opaqueNode = transparentNodeAttr->getNamedItem(BML::BMLDefs::ATTR_OPAQUE);			
				std::string opaqueMode;
				if (opaqueNode)
				{
					xml_utils::xml_translate(&opaqueMode, opaqueNode->getTextContent());
						
					if (opaqueMode == "RGB_ZERO")
					{
						DOMNode* colorNode = ParserOpenCOLLADA::getNode("color", transparentNode);		
						std::string color;
						if (colorNode)
						{
							xml_utils::xml_translate(&color, colorNode->getTextContent());
							std::vector<std::string> tokens;
							vhcl::Tokenize(color, tokens, " \n");
							SrVec colorVec;
							if (tokens.size() >= 3)
							{
								for (int i=0;i<3;i++)
								{
									colorVec[i] = (float)atof(tokens[i].c_str());
								}
							}
							alpha = 1.f - (colorVec[0]+colorVec[1]+colorVec[2])/3;
							//alpha = 1.f - colorVec.norm();		
						}
														
					}
				}
				//float alpha = float(1.0 - xml_utils::xml_translate_float(colorNode->getTextContent()));				
				M.top().diffuse.a = (srbyte) ( alpha*255.0f );
			}
		}
		curNode = curNode->getNextSibling();
	}
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

bool ParserOpenCOLLADA::parseStaticMesh( std::vector<SrModel*>& meshModelVecs, std::string fileName )
{
	XercesDOMParser* parser = ParserOpenCOLLADA::getParserFromFile(fileName);
	if (!parser)
	{
		LOG("Could not load from file %s",fileName.c_str());
		return false;
	}

	//DOMNode* geometryNode = ParserOpenCOLLADA::getNode("library_geometries", fileName, 2);	
	DOMDocument* doc = parser->getDocument();
	int depth = 0;
	DOMNode* geometryNode = getNode("library_geometries", doc, depth, 2);		 
	if (geometryNode)
	{
		// first from library visual scene retrieve the material id to name mapping (TODO: needs reorganizing the assets management)
		std::map<std::string, std::string> materialId2Name;
		//DOMNode* visualSceneNode = ParserOpenCOLLADA::getNode("library_visual_scenes", fileName, 2);
		depth = 0;
		DOMNode* visualSceneNode = getNode("library_visual_scenes", doc, depth, 2);	
		if (!visualSceneNode)
			LOG("mcu_character_load_mesh ERR: .dae file doesn't contain correct geometry information.");
		SkSkeleton skeleton;
		SkMotion motion;
		int order;
		ParserOpenCOLLADA::parseLibraryVisualScenes(visualSceneNode, skeleton, motion, 1.0, order, materialId2Name);

		// get picture id to file mapping
		std::map<std::string, std::string> pictureId2File;
		std::map<std::string, std::string> pictureId2Name;
		//DOMNode* imageNode = ParserOpenCOLLADA::getNode("library_images", fileName, 2);
		depth = 0;
		DOMNode* imageNode = getNode("library_images", doc, depth, 2);	
		if (imageNode)
			ParserOpenCOLLADA::parseLibraryImages(imageNode, pictureId2File,pictureId2Name);

		// start parsing mateiral
		std::map<std::string, std::string> effectId2MaterialId;
		//DOMNode* materialNode = ParserOpenCOLLADA::getNode("library_materials", fileName, 2);
		depth = 0;
		DOMNode* materialNode = getNode("library_materials", doc, depth, 2);	
		if (materialNode)
			ParserOpenCOLLADA::parseLibraryMaterials(materialNode, effectId2MaterialId);

		// start parsing effect
		SrArray<SrMaterial> M;
		SrStringArray mnames;
		std::map<std::string,std::string> mtlTextMap;
		std::map<std::string,std::string> mtlTextBumpMap;
		std::map<std::string,std::string> mtlTextSpecularMap;
		//DOMNode* effectNode = ParserOpenCOLLADA::getNode("library_effects", fileName, 2);
		depth = 0;
		DOMNode* effectNode = getNode("library_effects", doc, depth, 2);	
		if (effectNode)
		{
			ParserOpenCOLLADA::parseLibraryEffects(effectNode, effectId2MaterialId, materialId2Name, pictureId2File, pictureId2Name, M, mnames, mtlTextMap, mtlTextBumpMap, mtlTextSpecularMap);
		}
		// parsing geometry
		ParserOpenCOLLADA::parseLibraryGeometries(geometryNode, fileName.c_str(), M, mnames, materialId2Name, mtlTextMap, mtlTextBumpMap, mtlTextSpecularMap, meshModelVecs, 1.0f);
	}
	else
	{
		LOG( "Could not load mesh from file '%s'", fileName.c_str());
		if (parser)
			delete parser;
		return false;
	}
	if (parser)
		delete parser;
	return true;
}



bool ParserOpenCOLLADA::exportCollada( std::string outPathname, std::string skeletonName, std::string deformMeshName, std::vector<std::string> motionNames, bool exportSk, bool exportMesh, bool exportMotion )
{	
	std::string colladaName = deformMeshName ;
	std::string fullColladaPathName = outPathname + "/" + colladaName;
	FILE* fp = fopen(fullColladaPathName.c_str(),"wt");
	SmartBody::SBAssetManager* assetManager = SmartBody::SBScene::getScene()->getAssetManager();
	fprintf(fp,"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
	fprintf(fp,"<COLLADA xmlns=\"http://www.collada.org/2005/11/COLLADASchema\" version=\"1.4.1\">\n");

	fprintf(fp,"<asset>\n");
	fprintf(fp,"<contributor>\n");
	fprintf(fp,"<author>SmartBody</author>\n");
	fprintf(fp,"<authoring_tool>COLLADA SmartBody exporter</authoring_tool>\n");
	fprintf(fp,"<comments></comments>\n");
	fprintf(fp,"</contributor>\n");
	//fprintf(fp,"<created>2012-06-01T21:28:15Z</created>\");
	//fprintf(fp,"<modified>2012-06-01T21:28:15Z</modified>\n");
	fprintf(fp,"<revision></revision>\n");
	fprintf(fp,"<title></title>\n");
	fprintf(fp,"<unit meter=\"0.01\"/>\n");
	fprintf(fp,"<up_axis>Y_UP</up_axis>\n");
	fprintf(fp,"</asset>\n");

#if (BOOST_VERSION > 104400)	
	namespace fs =  boost::filesystem;
	//using boost::filesystem::dot;
	//using boost::filesystem::slash;
#else	
	namespace fs = boost::filesystem2;
	//using boost::filesystem2::dot;
	//using boost::filesystem2::slash;
#endif

	DeformableMesh* defMesh = assetManager->getDeformableMesh(deformMeshName);
	if (exportMesh && defMesh)
	{		
		ParserOpenCOLLADA::exportMaterials(fp,deformMeshName);
		ParserOpenCOLLADA::exportSkinMesh(fp,deformMeshName);
		SbmTextureManager& texManager = SbmTextureManager::singleton();
		for (unsigned int i=0;i<defMesh->subMeshList.size();i++)
		{
			SbmSubMesh* subMesh = defMesh->subMeshList[i];
			SbmTexture* tex = texManager.findTexture(SbmTextureManager::TEXTURE_DIFFUSE,subMesh->texName.c_str());
			if (tex) // copy texture
			{
				std::string ext = boost::filesystem::extension( tex->getFileName() );
				std::string texBaseName = boost::filesystem::basename( tex->getFileName() );
				std::string newFileName = outPathname + "/" + texBaseName + ext;
				if (!fs::exists(fs::path(newFileName)))
					fs::copy_file(fs::path(tex->getFileName()),fs::path(newFileName));
			}
		}
	}
	if (exportSk)
	{
		// decide whether to export instance of geometry as well
		std::string defMeshName = "";
		if (exportMesh)
			defMeshName = deformMeshName;
		ParserOpenCOLLADA::exportVisualScene(fp,skeletonName, defMeshName);
	}
	fprintf(fp,"</COLLADA>\n");
	fclose(fp);
	return true;
}

bool ParserOpenCOLLADA::exportMaterials( FILE* fp, std::string deformMeshName )
{
	SmartBody::SBAssetManager* assetManager = SmartBody::SBScene::getScene()->getAssetManager();
	DeformableMesh* defMesh = assetManager->getDeformableMesh(deformMeshName);
	if (!defMesh) return false;
	
	// write-out material -> effect map
	fprintf(fp,"<library_materials>\n");
	for (unsigned int i=0;i<defMesh->subMeshList.size();i++)
	{
		SbmSubMesh* subMesh = defMesh->subMeshList[i];
		std::string& matName = subMesh->matName;
		std::string effectName = matName+"-fx";
		fprintf(fp,"<material id=\"%s\" name=\"%s\">\n",matName.c_str(),matName.c_str());
		fprintf(fp,"<instance_effect url=\"#%s\"/>\n",effectName.c_str());
		fprintf(fp,"</material>\n");
	}
	fprintf(fp,"</library_materials>\n");
	SbmTextureManager& texManger = SbmTextureManager::singleton();
	fprintf(fp,"<library_effects>\n");
	for (unsigned int i=0;i<defMesh->subMeshList.size();i++)
	{
		SbmSubMesh* subMesh = defMesh->subMeshList[i];
		std::string effectID = subMesh->matName + "-fx";
		fprintf(fp,"<effect id=\"%s\">\n",effectID.c_str());
		fprintf(fp,"<profile_COMMON>\n");
		SbmTexture* diffTex = texManger.findTexture(SbmTextureManager::TEXTURE_DIFFUSE, subMesh->texName.c_str());
		//SbmTexture* bumpTex = texManger.findTexture(SbmTextureManager::TEXTURE_NORMALMAP, subMesh->normalMapName.c_str());
		//SbmTexture* specTex = texManger.findTexture(SbmTextureManager::TEXTURE_SPECULARMAP, subMesh->specularMapName.c_str());

		std::string diffuseTexSamplerID = "";
		if (diffTex) // only export diffuse texture for now
		{
			std::string texName = diffTex->getName();
			diffuseTexSamplerID = exportMaerialTexParam(fp, texName);
		}
		// write out material technique
		fprintf(fp,"<technique sid=\"common\">\n");
		// color component
		fprintf(fp,"<phong>\n");
		SrMaterial& mat = subMesh->material;		
		float color[4];		
		mat.emission.get(color);
		fprintf(fp,"<emission><color>%f %f %f %f</color></emission>\n",color[0],color[1],color[2],color[3]);
		mat.ambient.get(color);
		fprintf(fp,"<ambient><color>%f %f %f %f</color></ambient>\n",color[0],color[1],color[2],color[3]);
		fprintf(fp,"<diffuse>\n");
		if (diffuseTexSamplerID != "") // write-out texture sampler
		{
			fprintf(fp,"<texture texture=\"%s\" texcoord=\"TEX0\">\n",diffuseTexSamplerID.c_str());
			fprintf(fp,"</texture>\n");
		}
		else // write-out diffuse color
		{
			mat.diffuse.get(color);
			fprintf(fp,"<color>%f %f %f %f</color>\n",color[0],color[1],color[2],color[3]);
		}
		fprintf(fp,"</diffuse>\n");

		mat.specular.get(color);
		fprintf(fp,"<specular><color>%f %f %f %f</color></specular>\n",color[0],color[1],color[2],color[3]);
		float shininess = mat.shininess;
		fprintf(fp,"<shininess><float>%f</float></shininess>\n",shininess);
		fprintf(fp,"</phong>\n");
		fprintf(fp,"</technique>\n");		

		fprintf(fp,"</profile_COMMON>\n");
		fprintf(fp,"</effect>\n");		
	}
	fprintf(fp,"</library_effects>\n");

	// write-out images
	fprintf(fp,"<library_images>\n");
	for (unsigned int i=0;i<defMesh->subMeshList.size();i++)
	{
		SbmSubMesh* subMesh = defMesh->subMeshList[i];
		SbmTexture* diffTex = texManger.findTexture(SbmTextureManager::TEXTURE_DIFFUSE, subMesh->texName.c_str());
		if (diffTex) 
		{
			fprintf(fp,"<image id=\"%s\" name=\"%s\">\n",diffTex->getName().c_str(), diffTex->getName().c_str());
			fprintf(fp,"<init_from>%s</init_from>\n",diffTex->getFileName().c_str());
			fprintf(fp,"</image>\n");
		}		
	}
	fprintf(fp,"</library_images>\n");
	return true;
}


bool ParserOpenCOLLADA::exportSkinMesh( FILE* fp, std::string deformMeshName )
{
	SmartBody::SBAssetManager* assetManager = SmartBody::SBScene::getScene()->getAssetManager();
	DeformableMesh* defMesh = assetManager->getDeformableMesh(deformMeshName);
	if (!defMesh) return false;

	fprintf(fp,"<library_geometries>\n");
	// export geometry
	for (unsigned int i=0;i<defMesh->dMeshStatic_p.size();i++)
	{		
		SrModel& model = defMesh->dMeshStatic_p[i]->shape();
		bool hasNormal = false, hasTexCoord = false, hasColor = false;
		if (model.N.size() > 0 && model.Fn.size() == model.F.size()) hasNormal = true; // export normal
		if (model.T.size() > 0 && model.Ft.size() == model.F.size()) hasTexCoord = true;
		if (model.Vc.size() == model.V.size()) hasColor = true;

		std::string modelName = (const char*) model.name;

		if (modelName == "") // no model name
		{
			modelName = "shape"+boost::lexical_cast<std::string>(i);
		}
		fprintf(fp,"<geometry id=\"%s\" name=\"%s\">\n",modelName.c_str(),modelName.c_str());
		fprintf(fp,"<mesh>\n");
		// write-out position array
		std::string positionID = modelName + "-positions";
		std::string positionArrayID = positionID+"-array";

		{
			fprintf(fp,"<source id=\"%s\" name=\"%s\">\n",positionID.c_str(),positionID.c_str());
			fprintf(fp,"<float_array id=\"%s\" count=\"%d\">", positionArrayID.c_str(),model.V.size()*3);
			for (int k=0;k<model.V.size();k++)
				fprintf(fp,"%f %f %f ",model.V[k][0],model.V[k][1],model.V[k][2]);
			fprintf(fp,"</float_array>\n");
			fprintf(fp,"<technique_common>\n");
			fprintf(fp,"<accessor source=\"#%s\" count=\"%d\" stride=\"%d\">\n",positionArrayID.c_str(),model.V.size(),3);
			fprintf(fp,"<param name=\"X\" type=\"float\"/>\n");
			fprintf(fp,"<param name=\"Y\" type=\"float\"/>\n");
			fprintf(fp,"<param name=\"Z\" type=\"float\"/>\n");
			fprintf(fp,"</accessor>\n");
			fprintf(fp,"</technique_common>\n");
			fprintf(fp,"</source>\n");
		}

		// write-out normal array
		std::string normalID = modelName + "-normal";
		std::string normalArrayID = normalID+"-array";
		hasNormal = model.N.size() > 0;
		if (hasNormal) // has vertex normals
		{
			fprintf(fp,"<source id=\"%s\" name=\"%s\">\n",normalID.c_str(),normalID.c_str());
			fprintf(fp,"<float_array id=\"%s\" count=\"%d\">", normalArrayID.c_str(),model.N.size()*3);
			for (int k=0;k<model.N.size();k++)
				fprintf(fp,"%f %f %f ",model.N[k][0],model.N[k][1],model.N[k][2]);
			fprintf(fp,"</float_array>\n");
			fprintf(fp,"<technique_common>\n");
			fprintf(fp,"<accessor source=\"#%s\" count=\"%d\" stride=\"%d\">\n",normalArrayID.c_str(),model.N.size(),3);
			fprintf(fp,"<param name=\"X\" type=\"float\"/>\n");
			fprintf(fp,"<param name=\"Y\" type=\"float\"/>\n");
			fprintf(fp,"<param name=\"Z\" type=\"float\"/>\n");
			fprintf(fp,"</accessor>\n");
			fprintf(fp,"</technique_common>\n");
			fprintf(fp,"</source>\n");
		}

		
		// Note : this is a hack to support vertex color in Collada parsing, I don't think this is standard practice ... 
		std::string colorID = modelName + "-colors";
		std::string colorArrayID = colorID+"-array";

		{
			fprintf(fp,"<source id=\"%s\" name=\"%s\">\n",colorID.c_str(),colorID.c_str());
			fprintf(fp,"<float_array id=\"%s\" count=\"%d\">", colorArrayID.c_str(),model.Vc.size()*3);
			for (int k=0;k<model.Vc.size();k++)
				fprintf(fp,"%f %f %f ",model.Vc[k][0],model.Vc[k][1],model.Vc[k][2]);
			fprintf(fp,"</float_array>\n");
			fprintf(fp,"<technique_common>\n");
			fprintf(fp,"<accessor source=\"#%s\" count=\"%d\" stride=\"%d\">\n",colorArrayID.c_str(),model.Vc.size(),3);
			fprintf(fp,"<param name=\"R\" type=\"float\"/>\n");
			fprintf(fp,"<param name=\"G\" type=\"float\"/>\n");
			fprintf(fp,"<param name=\"B\" type=\"float\"/>\n");
			//fprintf(fp,"<param name=\"A\" type=\"float\"/>\n");
			fprintf(fp,"</accessor>\n");
			fprintf(fp,"</technique_common>\n");
			fprintf(fp,"</source>\n");
		}

		// write-out texcoord array
		std::string texCoordID = modelName + "-map1";
		std::string texCoordArrayID = texCoordID+"-array";
		hasTexCoord = model.T.size() > 0;
		if (hasTexCoord) // has vertex normals
		{
			fprintf(fp,"<source id=\"%s\" name=\"%s\">\n",texCoordID.c_str(),texCoordID.c_str());
			fprintf(fp,"<float_array id=\"%s\" count=\"%d\">", texCoordArrayID.c_str(),model.T.size()*2);
			for (int k=0;k<model.T.size();k++)
				fprintf(fp,"%f %f ",model.T[k][0],model.T[k][1]);
			fprintf(fp,"</float_array>\n");
			fprintf(fp,"<technique_common>\n");
			fprintf(fp,"<accessor source=\"#%s\" count=\"%d\" stride=\"%d\">\n",texCoordArrayID.c_str(),model.T.size(),2);
			fprintf(fp,"<param name=\"U\" type=\"float\"/>\n");
			fprintf(fp,"<param name=\"V\" type=\"float\"/>\n");			
			fprintf(fp,"</accessor>\n");
			fprintf(fp,"</technique_common>\n");
			fprintf(fp,"</source>\n");
		}

		// write out vertices semantic
		std::string vertexID = modelName+"-vertices";
		fprintf(fp,"<vertices id=\"%s\" name=\"%s\">\n",vertexID.c_str(),vertexID.c_str());
		fprintf(fp,"<input semantic=\"POSITION\" source=\"#%s\"/>\n",positionID.c_str());
		fprintf(fp,"</vertices>\n");

		// write out triangles
		std::string matName = "defaultMat";
		std::map<std::string,std::vector<int> >::iterator mi;
		for ( mi  = model.mtlFaceIndices.begin();
			  mi != model.mtlFaceIndices.end();
			  mi++)
		{
			matName = mi->first;
			std::string matID = matName+"_SG";
			std::vector<int>& mtlFaces = mi->second;
			int offset = 0;
			fprintf(fp,"<triangles material=\"%s\" count=\"%d\">\n",matID.c_str(),mtlFaces.size());
			fprintf(fp,"<input semantic=\"VERTEX\" source=\"#%s\" offset=\"%d\"/>\n",vertexID.c_str(),offset++);
			if (hasNormal)
			{
				fprintf(fp,"<input semantic=\"NORMAL\" source=\"#%s\" offset=\"%d\"/>\n",normalID.c_str(),offset++);
			}
			if (hasTexCoord)
			{
				fprintf(fp,"<input semantic=\"TEXCOORD\" source=\"#%s\" offset=\"%d\"/>\n",texCoordID.c_str(),offset++);
			}
			if (hasColor)
			{
				fprintf(fp,"<input semantic=\"COLOR\" source=\"#%s\" offset=\"%d\"/>\n",colorID.c_str(),offset++);
			}
			fprintf(fp,"<p>");			
			for (unsigned int k=0;k<mtlFaces.size();k++)
			{
				int fidx = mtlFaces[k];
				SrModel::Face* f = &model.F[fidx];
				SrModel::Face *fn = NULL, *ft = NULL;
				if (hasNormal)
					fn =  &model.Fn[fidx];
				if (hasTexCoord)
					ft = &model.Ft[fidx];
				
				for (int j=0;j<3;j++)
				{
					fprintf(fp,"%d ",(*f)[j]);
					if (hasNormal)
						fprintf(fp,"%d ",(*fn)[j]);
					if (hasTexCoord)
						fprintf(fp,"%d ",(*ft)[j]);
					if (hasColor)
						fprintf(fp,"%d ",(*f)[j]);
				}
			}
			fprintf(fp,"</p>\n");
			fprintf(fp,"</triangles>\n");
		}
		fprintf(fp,"</mesh>\n");
		fprintf(fp,"</geometry>\n");
	}
	fprintf(fp,"</library_geometries>\n");

	// export skin binding
	fprintf(fp,"<library_controllers>\n");
	for (unsigned int i=0;i<defMesh->skinWeights.size();i++)
	{
		SkinWeight* skinWeight = defMesh->skinWeights[i];
		std::string skinID = skinWeight->sourceMesh+"-skin";
		fprintf(fp,"<controller id=\"%s\"  name=\"%s\">\n",skinID.c_str(),skinID.c_str());
		fprintf(fp,"<skin source=\"#%s\">\n",skinWeight->sourceMesh.c_str());
		SrMat& bm = skinWeight->bindShapeMat;
		SrMat bmT = bm; bmT.transpose();
		fprintf(fp,"<bind_shape_matrix>");
		for (int k=0;k<16;k++)
			fprintf(fp,"%f ",bmT.get(k));
		fprintf(fp,"</bind_shape_matrix>\n");
		// output joint array
		std::string skinJointID = skinID + "-joints";
		std::string skinJointArrayID = skinJointID + "-array";
		fprintf(fp,"<source id=\"%s\" name=\"%s\">\n",skinJointID.c_str(),skinJointID.c_str());
		fprintf(fp,"<Name_array id=\"%s\" count=\"%d\">", skinJointArrayID.c_str(), skinWeight->infJointName.size());
		for (unsigned int k=0;k<skinWeight->infJointName.size();k++)
			fprintf(fp,"%s ",skinWeight->infJointName[k].c_str());
		fprintf(fp,"</Name_array>\n");
		fprintf(fp,"<technique_common>\n");
		fprintf(fp,"<accessor source=\"#%s\" count=\"%d\" stride=\"1\">\n",skinJointArrayID.c_str(),skinWeight->infJointName.size());
		fprintf(fp,"<param name=\"JOINT\" type=\"name\"/>\n");
		fprintf(fp,"</accessor>\n");
		fprintf(fp,"</technique_common>\n");
		fprintf(fp,"</source>\n");
		
		// output bind pose array
		std::string skinBindPoseID = skinID + "-bind_poses";
		std::string skinBindPoseArrayID = skinBindPoseID + "-array";
		fprintf(fp,"<source id=\"%s\" name=\"%s\">\n",skinBindPoseID.c_str(),skinBindPoseID.c_str());
		fprintf(fp,"<float_array id=\"%s\" count=\"%d\">", skinBindPoseArrayID.c_str(), skinWeight->bindPoseMat.size()*16);
		
		for (unsigned int k=0;k<skinWeight->bindPoseMat.size();k++)
		{
			SrMat bpT = skinWeight->bindPoseMat[k]; bpT.transpose();
			for (int j=0;j<16;j++)
				fprintf(fp,"%f ",bpT.get(j));
		}
		fprintf(fp,"</float_array>\n");
		fprintf(fp,"<technique_common>\n");
		fprintf(fp,"<accessor source=\"#%s\" count=\"%d\" stride=\"16\">\n",skinBindPoseArrayID.c_str(),skinWeight->bindPoseMat.size());
		fprintf(fp,"<param name=\"TRANSFORM\" type=\"float4x4\"/>\n");
		fprintf(fp,"</accessor>\n");
		fprintf(fp,"</technique_common>\n");
		fprintf(fp,"</source>\n");

		// output skin weights
		std::string skinWeightID = skinID + "-weights";
		std::string skinWeightArrayID = skinWeightID + "-array";
		fprintf(fp,"<source id=\"%s\" name=\"%s\">\n",skinWeightID.c_str(),skinWeightID.c_str());
		fprintf(fp,"<float_array id=\"%s\" count=\"%d\">", skinWeightArrayID.c_str(), skinWeight->bindWeight.size());
		for (unsigned int k=0;k<skinWeight->bindWeight.size();k++)
			fprintf(fp,"%f ",skinWeight->bindWeight[k]);
		fprintf(fp,"</float_array>\n");

		fprintf(fp,"<technique_common>\n");
		fprintf(fp,"<accessor source=\"#%s\" count=\"%d\" stride=\"1\">\n",skinWeightArrayID.c_str(),skinWeight->bindWeight.size());
		fprintf(fp,"<param name=\"WEIGHT\" type=\"float\"/>\n");
		fprintf(fp,"</accessor>\n");
		fprintf(fp,"</technique_common>\n");
		fprintf(fp,"</source>\n");

		// output joints semantics
		fprintf(fp,"<joints>\n");
		fprintf(fp,"<input semantic=\"JOINT\" source=\"#%s\"/>\n",skinJointID.c_str());
		fprintf(fp,"<input semantic=\"INV_BIND_MATRIX\" source=\"#%s\"/>\n",skinBindPoseID.c_str());
		fprintf(fp,"</joints>\n");
		// output vertex weights semantics
		fprintf(fp,"<vertex_weights count=\"%d\">\n",skinWeight->numInfJoints.size());
		fprintf(fp,"<input semantic=\"JOINT\" source=\"#%s\" offset=\"0\"/>\n", skinJointID.c_str());
		fprintf(fp,"<input semantic=\"WEIGHT\" source=\"#%s\" offset=\"1\"/>\n", skinWeightID.c_str());
		
		fprintf(fp,"<vcount>"); // inf joint counts
		for (unsigned int k=0;k<skinWeight->numInfJoints.size();k++)
			fprintf(fp,"%d ",skinWeight->numInfJoints[k]);
		fprintf(fp,"</vcount>\n");
		fprintf(fp,"<v>"); // actual joint name index and weight index
		for (unsigned int k=0;k<skinWeight->jointNameIndex.size();k++)
		{
			fprintf(fp,"%d %d ",skinWeight->jointNameIndex[k], skinWeight->weightIndex[k]);
		}
		fprintf(fp,"</v>\n");
		fprintf(fp,"</vertex_weights>\n");
		fprintf(fp,"</skin>\n");
		fprintf(fp,"</controller>\n");
	}

	fprintf(fp,"</library_controllers>\n");

	return true;
}

bool ParserOpenCOLLADA::exportVisualScene( FILE* fp, std::string skeletonName, std::string defMeshName )
{
	SmartBody::SBAssetManager* assetManager = SmartBody::SBScene::getScene()->getAssetManager();
	SmartBody::SBSkeleton* sbSk = assetManager->getSkeleton(skeletonName);
	DeformableMesh* defMesh = assetManager->getDeformableMesh(defMeshName);
	if (!sbSk) return false;
	fprintf(fp,"<library_visual_scenes>\n");
	fprintf(fp,"<visual_scene id=\"RootNode\" name=\"RootNode\">\n");
	// write-out skeleton hierarchy
	SmartBody::SBJoint* rootJoint = dynamic_cast<SmartBody::SBJoint*>(sbSk->root());
	writeJointNode(fp,rootJoint);
	// write-out instance of skinned mesh, if it exists
	if (defMesh)
	{
		//fprintf(fp,"<node id=\"SkinnedMesh\" name=\"SkinnedMesh\" type=\"NODE\">\n");
		std::string rootJointName = rootJoint->getName();
		for (unsigned int i=0;i<defMesh->skinWeights.size();i++)
		{
			SkinWeight* skinWeight = defMesh->skinWeights[i];
			std::string meshID = skinWeight->sourceMesh;
			std::string controllerID = skinWeight->sourceMesh+"-skin";
			int validMeshIdx = defMesh->getValidSkinMesh(skinWeight->sourceMesh);
			SrModel& model = defMesh->dMeshStatic_p[validMeshIdx]->shape();
			std::string matName = "default";
			if (model.mtlnames.size() > 0)
				matName = model.mtlnames[0];
			std::string matID = matName+"_SG";
			
			fprintf(fp,"<node id=\"%s\" name=\"%s\" type=\"NODE\">\n",meshID.c_str(),meshID.c_str());
			fprintf(fp,"<translate side=\"translate\">0 0 0</translate>\n");
			fprintf(fp,"<instance_controller url=\"#%s\">\n",controllerID.c_str());
			fprintf(fp,"<skeleton>#%s</skeleton>\n",rootJointName.c_str());
			// write-out bind material for this mesh
			fprintf(fp,"<bind_material>\n");
			fprintf(fp,"<technique_common>\n");
			fprintf(fp,"<instance_material symbol=\"%s\" target=\"#%s\"/>\n",matID.c_str(),matName.c_str());
			fprintf(fp,"</technique_common>\n");
			fprintf(fp,"</bind_material>\n");
			fprintf(fp,"</instance_controller>\n");
			fprintf(fp,"</node>\n");
		}
		//fprintf(fp,"</node>\n");
	}

	fprintf(fp,"</visual_scene>\n");
	fprintf(fp,"</library_visual_scenes>\n");
	fprintf(fp,"<scene>\n");
	fprintf(fp,"<instance_visual_scene url=\"#RootNode\"/>\n");
	fprintf(fp,"</scene>\n");
	return true;
}

void ParserOpenCOLLADA::writeJointNode( FILE* fp, SmartBody::SBJoint* joint )
{
	if (!joint) return;
	// write out the joint name as node id
	fprintf(fp,"<node id=\"%s\" name=\"%s\" sid=\"%s\" type=\"JOINT\">\n",joint->getName().c_str(), joint->getName().c_str(), joint->getName().c_str());
	// output joint local translation
	SrVec pos = joint->getOffset();
	fprintf(fp,"<translate sid=\"translate\">%f %f %f</translate>\n", pos[0],pos[1],pos[2]);
	// output joint pre-rotation & orientation
	SrQuat quat = joint->quat()->prerot();
	SrVec eulerPrerot = quat.getEuler();
	fprintf(fp,"<rotate sid=\"rotateY\">0 1 0 %f</rotate>\n",eulerPrerot[1]);	
	fprintf(fp,"<rotate sid=\"rotateX\">1 0 0 %f</rotate>\n",eulerPrerot[0]);	
	fprintf(fp,"<rotate sid=\"rotateZ\">0 0 1 %f</rotate>\n",eulerPrerot[2]);	
	SrQuat jorient = joint->quat()->orientation();
	SrVec eulerOrient = jorient.getEuler();
	fprintf(fp,"<rotate sid=\"jointOrientY\">0 1 0 %f</rotate>\n",eulerOrient[1]);	
	fprintf(fp,"<rotate sid=\"jointOrientX\">1 0 0 %f</rotate>\n",eulerOrient[0]);	
	fprintf(fp,"<rotate sid=\"jointOrientZ\">0 0 1 %f</rotate>\n",eulerOrient[2]);	

	// recursively output child joints
	for (int i=0;i<joint->getNumChildren();i++)
	{
		writeJointNode(fp,joint->getChild(i));
	}

	fprintf(fp,"</node>\n");
}

std::string ParserOpenCOLLADA::exportMaerialTexParam( FILE* fp, std::string texName )
{
	std::string texSurfaceID = texName + "-surface";
	std::string texSamplerID = texName + "-sampler";
	fprintf(fp,"<newparam sid=\"%s\">\n",texSurfaceID.c_str());
	fprintf(fp,"<surface type=\"2D\">\n");
	fprintf(fp,"<init_from>%s</init_from>",texName.c_str());
	fprintf(fp,"</surface>\n");
	fprintf(fp,"</newparam>\n");

	fprintf(fp,"<newparam sid=\"%s\">\n",texSamplerID.c_str());
	fprintf(fp,"<sampler2D>\n");
	fprintf(fp,"<source>%s</source>",texSurfaceID.c_str());
	fprintf(fp,"</sampler2D>\n");
	fprintf(fp,"</newparam>\n");

	return texSamplerID;
}

#endif