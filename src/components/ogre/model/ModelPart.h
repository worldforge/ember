//
// C++ Interface: ModelPart
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2008
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
#ifndef EMBEROGRE_MODELMODELPART_H
#define EMBEROGRE_MODELMODELPART_H

#include <vector>
#include <string>


namespace EmberOgre {

namespace Model {

class SubModelPart;

/**
@brief A part of a model, which can be turned on and off.

Each model is separated into one or many "parts" which consists of one or many sub entities using specific materials. By showing or hiding certain parts you can simulate concepts such as different clothes or attached objects. A part can belong to a "group", which is just an arbitrary name. Parts belonging to the same group are mutually exlusive and only one part can be shown at once. When a part which belongs to a certain group is shown, all other parts in the same group are automatically hidden.
A typical example of usage of this would be a human model where there's ten different parts for different shirt combinations. Each shirt part points towards the same mesh, but with different materials. They all belong to the same group, "shirt". Whenever one shirt is shown, all of the others are automatically hidden.
@author Erik Hjortsberg <erik@worldforge.org>
*/
class ModelPart
{
public:
	typedef std::vector<SubModelPart*> SubModelPartStore;
	/**
	 * @brief Default ctor.
	 */
	ModelPart();
	
	/**
	 * @brief Adds a sub model part to this model part.
	 * @param part The sub model part to add.
	 */
	void addSubModelPart(SubModelPart* part);
	
	/**
	 * @brief Hides this part.
	 */
	void hide();
	
	/**
	 * @brief Shows this part.
	 */
	void show();
	
	/**
	 * @brief Gets whether the part is visible or not.
	 * @return True if the part is visible.
	 */
	bool getVisible() const;
	
	/**
	 * @brief Sets whether the part is visible or not.
	 * @param visible True if the part should be visible.
	 */
	void setVisible(bool visible);
	
	/**
	 * @brief Gets the name of the group which this part belongs to.
	 * @return The name of the group which this part belongs to.
	 */
	const std::string& getGroupName() const;
	
	/**
	 * @brief Sets the name of the group to which this part belongs to.
	 * @param groupName The name of the group which this part should belong to.
	 */
	void setGroupName(const std::string& groupName);
	
protected:
	/**
	 * @brief Whether the part is shown or not.
	 */
	bool mVisible;
	
	/**
	 * @brief A store of the sub model parts belonging to this part.
	 */
	SubModelPartStore mSubModelParts;
	
	/**
	 * @brief The name of the group to which this part belongs.
	 */
	std::string mGroupName;
};

}

}

#endif
