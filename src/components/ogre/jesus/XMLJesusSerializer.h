//
// C++ Interface: XMLJesusSerializer
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2005
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
#ifndef EMBEROGREXMLJESUSSERIALIZER_H
#define EMBEROGREXMLJESUSSERIALIZER_H

#include "../OgreIncludes.h"
#include "framework/tinyxml/tinyxml.h"

namespace Carpenter
{
	class BluePrint;
	class AttachPoint;
	class BlockSpec;
};

namespace EmberOgre {

class Jesus;
/**
@author Erik Hjortsberg
*/
class XMLJesusSerializer{
public:
    XMLJesusSerializer(Jesus* jesus);

    ~XMLJesusSerializer();
	
	/**
	 *    Loads BlockSpecs from a xml file
	 * @param filename 
	 * @return 
	 */
	bool loadBlockSpec(const std::string & filename);
	
	
	/**
	 *    Loads BuildingBlockSpecDefinitions from a xml file.
	 * @param filename 
	 * @return 
	 */
	bool loadBuildingBlockSpecDefinition(const std::string& filename);
	
	
	/**
	 *    Loads ModelBlockMappings from a xml file.
	 * @param filename 
	 * @return 
	 */
	bool loadModelBlockMapping(const std::string& filename);
	
	
	/**
	 *    Loads and creates an instance of a BluePrint from a xml file.
	 *    Note that you have to call compile() on the resulting BluePrint to get it to arrange the BuildingBlocks.
	 *    NOTE: this should perhaps be moved to another class
	 * @param filename 
	 * @return 
	 */
	Carpenter::BluePrint* loadBlueprint(const std::string& filename);
	
	/**
	 *    Saves the supplied blueprint to a xml filename
	 * @param blueprint 
	 * @param filename 
	 * @return 
	 */
	void saveBlueprintToFile(Carpenter::BluePrint* blueprint, const std::string& filename);
	void saveBlueprintToFile(Carpenter::BluePrint* blueprint, const std::string& filename, const std::string& name);
		
private: 
	Jesus* mJesus;

	Carpenter::AttachPoint* readAttachPoint(TiXmlElement* elem);
	void readAttachPairs(Carpenter::BlockSpec *blockSpec, TiXmlElement* parentElem);
	
	/**
	 *    Utility method to fill the supplied WFMath object (for example a Vector or a Point) with xyz values from the Xerces Node
	 * @param  
	 * @param  
	 */
	template <typename T> void fillFromElement(TiXmlElement* , T& );


};

};

#endif
