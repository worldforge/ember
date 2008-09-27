//
// C++ Interface: ModelMount
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
#ifndef EMBEROGREMODELMOUNT_H
#define EMBEROGREMODELMOUNT_H

#include <wfmath/axisbox.h>

namespace Ogre {
	class SceneNode;
}

namespace EmberOgre {

namespace Model {
	class Model;
}

/**
	@brief Acts as a mount for a model, connecting it to two scene nodes.
	A model can contain internal orientation and offset data. This requires us to use two different scene nodes for each model instance. The inner most scene node, called the "scale node", is the one which the Model is attached to. This is then offset and oriented in relation to the outer scene node. Whenever we move the Model, we move only the outer scene node, leaving the inner scene node completely in the hands of the Model.
	The scale node will be owned by this class.
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class ModelMount{
public:

	/**
	* @brief Ctor.
	* @param model The model which we need a mount for.
	* @param mainNode The main node. A scale node will be created as a child, to which the model then will be attached. Ownership of the main node isn't transferred to this class, but the scale node that's created will be owned, and destroyed, by this class.
	*/
	ModelMount(EmberOgre::Model::Model& model, Ogre::SceneNode* mainNode);

	/**
	 * @brief Dtor.
	 * The scale node will be destroyed here.
	 */
	virtual ~ModelMount();
	
	/**
	 * @brief Gets the main node. This is the node to use when positioning or rotating the Model.
	 * @return The main node.
	 */
	inline Ogre::SceneNode* getMainNode() const;
 
	/**
	 * @brief Gets the scale node. This is the node to which the Model is directly attached to. It should normally never be altered outside of this class.
	 * Under normal circumstances the scale node is handed solely by this class and should never be modified from outside functionality. Accessing through this method is mainly for lookup purposes, when you need to read the orientation or scale of the bounding box.
	 * @return The scale node, to which the Model is attached.
	 */
	inline Ogre::SceneNode* getScaleNode() const;
	
	/**
	 * @brief Gets the Model instance to which this mount is attached.
	 * @return The model instance.
	 */
	inline EmberOgre::Model::Model& getModel() const;
	
	/**
	 * @brief Rescales the model according to the bounding box.
	 * @param wfBbox The bounding box to the model should be scaled. If you don't have an axis box available, just send an unitialized one (which you can get by just creating a new AxisBox with the empty constructor).
	 */
	void rescale(const WFMath::AxisBox<3>& wfBbox);
	
	
protected:
	/**
	 *@brief The Model which this mount is attached to.
	 */
	EmberOgre::Model::Model& mModel;
	
	/**
	 * @brief The inner scale node, to which the model is attached to.
	 * It's this node that we'll change the orientation and position of. This node should never be touched by outside functionality.
	 */
	Ogre::SceneNode* mScaleNode;
	
	/**
	 * @brief The main node, to which the scale node is attached.
	 * This node is the external node, used for positioning the Model.
	 */
	Ogre::SceneNode* mMainNode;
	
	
	/**
	 * @brief Resets all scaling and rotation
	 */
	void reset();

	/**
	 * @brief Scales the scale node accoring to the submitted bounding box.
	 * @param wfBbox The bounding box, in WF space, for which we should scale the node.
	 */
	void scaleNode(const WFMath::AxisBox<3>& wfBbox);

};

Ogre::SceneNode* ModelMount::getMainNode() const
{
	return mMainNode;
}

Ogre::SceneNode* ModelMount::getScaleNode() const
{
	return mScaleNode;
}

EmberOgre::Model::Model& ModelMount::getModel() const
{
	return mModel;
}


}

#endif
