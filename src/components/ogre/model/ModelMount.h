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

namespace WFMath
{
template<int> class AxisBox;
}

namespace Ogre
{
class SceneNode;
class Node;
class MovableObject;
}

namespace EmberOgre
{

class INodeProvider;

namespace Model
{
class Model;

/**
 @brief Acts as a mount for a model, connecting it to two scene nodes.
 A model can contain internal orientation and offset data. This requires us to use two different scene nodes for each model instance. The inner most scene node, called the "scale node", is the one which the Model is attached to. This is then offset and oriented in relation to the outer scene node. Whenever we move the Model, we move only the outer scene node, leaving the inner scene node completely in the hands of the Model.
 The scale node will be owned by this class.
 @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 */
class ModelMount
{
public:

	/**
	 * @brief Ctor.
	 * @param model The model which we need a mount for.
	 * @param nodeProvider Provides an instance of a subclass of Ogre::Node to which the Model can be attached.
	 */
	ModelMount(::EmberOgre::Model::Model& model, INodeProvider* nodeProvider);

	/**
	 * @brief Dtor.
	 * The scale node will be destroyed here.
	 */
	virtual ~ModelMount();

	/**
	 * @brief Gets the Model instance to which this mount is attached.
	 * @return The model instance.
	 */
	::EmberOgre::Model::Model& getModel() const;

	/**
	 * @brief Rescales the model according to the bounding box.
	 * @param wfBbox The bounding box to the model should be scaled. If you don't have an axis box available, just send null.
	 */
	void rescale(const WFMath::AxisBox<3>* wfBbox);

	/**
	 * @brief Resets all scaling and rotation
	 */
	void reset();

	/**
	 * @brief Gets the node provider, which is responsible for providing nodes to which the model mount can attach the model.
	 * @return The node provider in use.
	 */
	INodeProvider* getNodeProvider() const;

protected:
	/**
	 *@brief The Model which this mount is attached to.
	 */
	::EmberOgre::Model::Model& mModel;

	/**
	 * @brief The node provider used by the mount.
	 * The node to which the Model is attached to is provided by this instance. It can be either a SceneNode or a TagPoint.
	 */
	INodeProvider* mNodeProvider;

	/**
	 * @brief Scales the scale node accoring to the submitted bounding box.
	 * @note You should in almost all cases call reset() before calling this method.
	 * @param wfBbox The bounding box, in WF space, for which we should scale the node, or null if there's no bbox available.
	 */
	void scaleNode(const WFMath::AxisBox<3>* wfBbox);

	/**
	 * @brief Gets the active scaling node. If the model is attached to a node already, this will be used, else the mScaleNode will be used.
	 * @return The active scaling node.
	 */
	Ogre::Node* getActiveScaleNode() const;

};

inline ::EmberOgre::Model::Model& ModelMount::getModel() const
{
	return mModel;
}

}

}

#endif
