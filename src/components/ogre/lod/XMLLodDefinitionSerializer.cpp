/*
 * Copyright (c) 2013 Peter Szucs <peter.szucs.dev@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "XMLLodDefinitionSerializer.h"
#include "LodDefinition.h"

#include "components/ogre/XMLHelper.h"
#include "framework/tinyxml/tinyxml.h"
#include "framework/osdir.h"

#include <OgreStringConverter.h>
#include <OgreResourceGroupManager.h>

namespace Ember
{
namespace OgreView
{
namespace Lod
{

void XMLLodDefinitionSerializer::importLodDefinition(const Ogre::DataStreamPtr& stream, LodDefinition& lodDef) const
{
	TiXmlDocument xmlDoc;
	XMLHelper xmlHelper;
	if (!xmlHelper.Load(xmlDoc, stream)) {
		return;
	}

	// <lod>...</lod>
	TiXmlElement* rootElem = xmlDoc.RootElement();
	if (rootElem) {

		// <automatic enabled="true|false" />
		TiXmlElement* autElem = rootElem->FirstChildElement("automatic");
		if (autElem) {
			const char* tmp = autElem->Attribute("enabled");
			if (tmp) {
				lodDef.setUseAutomaticLod(Ogre::StringConverter::parseBool(tmp, true));
			}
		}

		// <manual>...</manual>
		TiXmlElement* manElem = rootElem->FirstChildElement("manual");
		if (manElem) {

			// <type>user|automatic</type>
			TiXmlElement* elem = manElem->FirstChildElement("type");
			if (elem) {
				const char* tmp = elem->GetText();
				if (tmp && strcmp(tmp, "automatic") == 0) {
					lodDef.setType(LodDefinition::LT_AUTOMATIC_VERTEX_REDUCTION);
				} else {
					lodDef.setType(LodDefinition::LT_USER_CREATED_MESH);
				}
			}

			// <strategy>distance|pixelcount</strategy>
			elem = manElem->FirstChildElement("strategy");
			if (elem) {
				const char* tmp = elem->GetText();
				if (tmp && strcmp(tmp, "distance") == 0) {
					lodDef.setStrategy(LodDefinition::LS_DISTANCE);
				} else {
					lodDef.setStrategy(LodDefinition::LS_PIXEL_COUNT);
				}
			}

			// <level>...</level> <level>...</level> <level>...</level>
			for (TiXmlElement* distElem = manElem->FirstChildElement("level");
			     distElem != 0;
			     distElem = distElem->NextSiblingElement("level")) {
				LodDistance dist;

				if (lodDef.getType() == LodDefinition::LT_USER_CREATED_MESH) {
					// <meshName>.../test.mesh</meshName>
					elem = distElem->FirstChildElement("meshName");
					if (elem) {
						const char* tmp = elem->GetText();
						bool isValidMeshName = Ogre::ResourceGroupManager::getSingleton().resourceExistsInAnyGroup(tmp);
						if (tmp && isValidMeshName) {
							dist.setMeshName(tmp);
						} else {
							S_LOG_FAILURE(
							    lodDef.getName()	<<
							    " contains invalid mesh name for user created lod level. Skipping lod level for distance "
							                        << distElem->Attribute("distance"));
							continue;
						}
					}
				} else {
					// <method>constant|proportional</method>
					elem = distElem->FirstChildElement("method");
					if (elem) {
						const char* tmp = elem->GetText();
						if (tmp) {
							if (strcmp(tmp, "constant") == 0) {
								dist.setReductionMethod(Ogre::LodLevel::VRM_CONSTANT);
							} else if (strcmp(tmp, "proportional") == 0) {
								dist.setReductionMethod(Ogre::LodLevel::VRM_PROPORTIONAL);
							} else {
								dist.setReductionMethod(Ogre::LodLevel::VRM_COLLAPSE_COST);
							}
						} else {
							dist.setReductionMethod(Ogre::LodLevel::VRM_PROPORTIONAL);
						}
					}

					// <value>0.5</value>
					elem = distElem->FirstChildElement("value");
					if (elem) {
						const char* tmp = elem->GetText();
						if (tmp) {
							dist.setReductionValue(Ogre::StringConverter::parseReal(tmp));
						}
					}
				}

				// <level distance="10">...</level>
				const char* distVal = distElem->Attribute("distance");
				if (distVal) {
					lodDef.addLodDistance(Ogre::StringConverter::parseReal(distVal), dist);
				}
			}
		}
	}
}

bool XMLLodDefinitionSerializer::exportScript(const LodDefinitionPtr& lodDef, const std::string& fileName) const
{
	if (fileName == "") {
		return false;
	}

	TiXmlDocument xmlDoc;

	if (!oslink::directory(mExportDirectory).isExisting()) {
		S_LOG_INFO("Creating directory " << mExportDirectory);
		oslink::directory::mkdir(mExportDirectory.c_str());
	}

	// <lod>...</lod>
	TiXmlElement rootElem("lod");

	{
		// <automatic enabled="true|false" />
		TiXmlElement autElem("automatic");
		autElem.SetAttribute("enabled", lodDef->getUseAutomaticLod() ? "true" : "false");

		// <manual>...</manual>
		TiXmlElement manElem("manual");

		{
			// <type>user|automatic</type>
			TiXmlElement typeElem("type");
			TiXmlText typeText(lodDef->getType() == LodDefinition::LT_AUTOMATIC_VERTEX_REDUCTION ? "automatic" : "user");
			typeElem.InsertEndChild(typeText);
			manElem.InsertEndChild(typeElem);

			// <strategy>distance|pixelcount</strategy>
			TiXmlElement strategyElem("strategy");
			TiXmlText strategyText(lodDef->getStrategy() == LodDefinition::LS_DISTANCE ? "distance" : "pixelcount");
			strategyElem.InsertEndChild(strategyText);
			manElem.InsertEndChild(strategyElem);

			// <level>...</level> <level>...</level> <level>...</level>
			const LodDefinition::LodDistanceMap& manualLod = lodDef->getManualLodData();
			LodDefinition::LodDistanceMap::const_iterator it;
			for (it = manualLod.begin(); it != manualLod.end(); it++) {

				// <level distance="10">...</level>
				TiXmlElement levelElem("level");
				levelElem.SetAttribute("distance", Ogre::StringConverter::toString(it->first));

				const LodDistance& dist = it->second;
				{
					if (lodDef->getType() == LodDefinition::LT_USER_CREATED_MESH) {
						// <meshName>.../test.mesh</meshName>
						TiXmlElement meshElem("meshName");
						TiXmlText meshText(dist.getMeshName());
						meshElem.InsertEndChild(meshText);
						levelElem.InsertEndChild(meshElem);
					} else {
						// <method>constant|proportional</method>
						TiXmlElement methodElem("method");
						const char* pMethodText;
						switch (dist.getReductionMethod()) {
						case Ogre::LodLevel::VRM_PROPORTIONAL:
							pMethodText = "proportional";
							break;

						case Ogre::LodLevel::VRM_CONSTANT:
							pMethodText = "constant";
							break;

						case Ogre::LodLevel::VRM_COLLAPSE_COST:
							pMethodText = "collapsecost";
							break;

						default:
							assert(0);
							break;
						}
						TiXmlText methodText(pMethodText);
						methodElem.InsertEndChild(methodText);

						// <value>0.5</value>
						TiXmlElement valueElem("value");
						TiXmlText valueText(Ogre::StringConverter::toString(dist.getReductionValue()));
						valueElem.InsertEndChild(valueText);

						levelElem.InsertEndChild(methodElem);
						levelElem.InsertEndChild(valueElem);
					}
				}
				manElem.InsertEndChild(levelElem);
			}
		}
		rootElem.InsertEndChild(autElem);
		rootElem.InsertEndChild(manElem);
	}

	xmlDoc.InsertEndChild(rootElem);
	S_LOG_INFO("Saved file " << (mExportDirectory + fileName));
	return xmlDoc.SaveFile((mExportDirectory + fileName).c_str());
}

XMLLodDefinitionSerializer::XMLLodDefinitionSerializer(const std::string& exportDirectory) :
	mExportDirectory(exportDirectory)
{

}

}
}
}
