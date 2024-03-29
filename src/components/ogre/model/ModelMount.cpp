//
// C++ Implementation: ModelMount
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2008
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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#include "ModelMount.h"
#include "Model.h"
#include "SubModel.h"
#include "components/ogre/Convert.h"
#include <Ogre.h>

#include <utility>

namespace Ember {
namespace OgreView {

namespace Model {

ModelMount::ModelMount(Model& model, std::unique_ptr<INodeProvider> nodeProvider, std::string pose) :
		mModel(model),
		mNodeProvider(std::move(nodeProvider)),
		mPose(std::move(pose)) {
	model.attachToNode(mNodeProvider.get());
}

ModelMount::~ModelMount() {
	mModel.attachToNode(nullptr);
}

void ModelMount::rescale(const WFMath::AxisBox<3>* wfBbox) {
	scaleNode(wfBbox);
}

void ModelMount::reset() {
	mNodeProvider->setScale(Ogre::Vector3::UNIT_SCALE);

	PoseDefinition const* pose = nullptr;
	if (!mPose.empty()) {
		const PoseDefinitionStore& poses = mModel.getDefinition()->getPoseDefinitions();
		auto I = poses.find(mPose);
		if (I != poses.end()) {
			pose = &I->second;
		}
	}

	if (pose) {
		mNodeProvider->setOffsets(pose->Translate, pose->Rotate);
	} else {
		mNodeProvider->setOffsets(getModel().getDefinition()->getTranslate(), getModel().getDefinition()->getRotation());
	}

}

Ogre::Node* ModelMount::getNode() const {
	return mNodeProvider->getNode();
}

void ModelMount::scaleNode(const WFMath::AxisBox<3>* wfBbox) {
	if (!mModel.isLoaded()) {
		return;
	}

	Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE;

	//make a copy of the original bbox

	Ogre::AxisAlignedBox defaultOgreBoundingBox = mModel.getCombinedBoundingBox();

	//We can only apply any meaningful scaling if there's a bounding box in the model. This might not be true if the model for example only contains a particle system or similar, and no entities
	Ogre::Vector3 defaultSize = defaultOgreBoundingBox.getSize();
	if (!defaultOgreBoundingBox.isNull() && (defaultSize.x != 0.0f && defaultSize.y != 0.0f && defaultSize.z != 0.0f)) {

		//apply any transformations required first so the bounding box we use as reference represents the way to mesh is adjusted through rotations set in the model definition
		Ogre::Matrix4 localTransform;
		localTransform.makeTransform(getModel().getDefinition()->getTranslate(), Ogre::Vector3::UNIT_SCALE, getModel().getDefinition()->getRotation());
		defaultOgreBoundingBox.transform(localTransform);

		defaultSize = defaultOgreBoundingBox.getSize();

		//Depending on whether the entity has a bounding box or not we'll use different scaling methods. Most entities should have bounding boxes, but not all.
		if (wfBbox && wfBbox->isValid()) {

			//The entity has a bounding box. We'll now check the "usescaleof" setting.
			//It it's either of "height", "width" or "depth" we'll scale the model uniformly so that the specified dimension matches up exactly between the bounding box of the Model and the bounding box of the entity.
			//If it's "none" we won't apply any scaling.
			//And if it's "all" we'll scale the Model so that it matches all dimensions of the entity's bounding box. This will in almost all cases however distort the Model so it's undesirable.

			//Note that after the Model has been scaled using the bounding box, it can still be scaled additionally through the "scale" setting in the ModelDefinition.

			Ogre::AxisAlignedBox ogreBbox(Convert::toOgre(*wfBbox));

			Ogre::Vector3 ogreSize = ogreBbox.getSize();


			switch (getModel().getUseScaleOf()) {
				case ModelDefinition::UseScaleOf::MODEL_HEIGHT:
					scale = Ogre::Vector3(std::abs(ogreSize.y / defaultSize.y));
					break;
				case ModelDefinition::UseScaleOf::MODEL_WIDTH:
					scale = Ogre::Vector3(std::abs(ogreSize.x / defaultSize.x));
					break;
				case ModelDefinition::UseScaleOf::MODEL_DEPTH:
					scale = Ogre::Vector3(std::abs(ogreSize.z / defaultSize.z));
					break;
				case ModelDefinition::UseScaleOf::MODEL_NONE:
					scale = Ogre::Vector3(1);
					break;
				case ModelDefinition::UseScaleOf::MODEL_ALL:
					scale.x = std::abs(ogreSize.x / defaultSize.x);
					scale.y = std::abs(ogreSize.y / defaultSize.y);
					scale.z = std::abs(ogreSize.z / defaultSize.z);
					break;
				case ModelDefinition::UseScaleOf::MODEL_FIT:
					scale.x = std::abs(ogreSize.x / defaultSize.x);
					scale.y = std::abs(ogreSize.y / defaultSize.y);
					scale.z = std::abs(ogreSize.z / defaultSize.z);

					Ogre::Quaternion orientation = Ogre::Quaternion::IDENTITY;
					auto scaledBbox = defaultOgreBoundingBox;
					scaledBbox.scale(scale);
					Ogre::Vector3 translation = ogreBbox.getCenter() - scaledBbox.getCenter();
					getNodeProvider()->setOffsets(translation / scale, orientation);
					break;
			}

		} else if (getModel().getDefinition()->getScale() == 0) {
			//If there's no bbox, and no scaling in the model (i.e. not even "1") we'll set the size of the model to a hardcoded small value (0.25 meters in each dimension).
			//This is of course a last resort; all good models that can belong to entities without bounding boxes should have a scale set

			S_LOG_WARNING("Could not find any scale set in the model '" << getModel().getName() << "' of type '" << getModel().getDefinition()->getOrigin() << "'. We'll thus default to scaling the mesh so it's 0.25 meters in each dimension.");

			scale.x = (0.25f / defaultSize.x);
			scale.y = (0.25f / defaultSize.y);
			scale.z = (0.25f / defaultSize.z);
		}

		//Lastly, check if we also should scale the model. This scaling is applied after the Model has been scaled to fit with the bounding box.
		Ogre::Real modelScale = mModel.getDefinition()->getScale();
		if (modelScale != 0 && modelScale != 1) {
			//only scale if it's not 1
			scale *= modelScale;
		}

		//If we've attached using a pose, we need to scale the translation defined in the pose according to our new scale
		if (!mPose.empty()) {
			const PoseDefinitionStore& poses = mModel.getDefinition()->getPoseDefinitions();
			auto I = poses.find(mPose);
			if (I != poses.end()) {
				Ogre::Quaternion orientation = Ogre::Quaternion::IDENTITY;
				Ogre::Vector3 translation = (orientation * (I->second.Translate * scale));
				orientation = orientation * I->second.Rotate;

				getNodeProvider()->setPositionAndOrientation(translation, orientation);
			}
		}

		mNodeProvider->setScale(scale);

	}
}

INodeProvider* ModelMount::getNodeProvider() const {
	return mNodeProvider.get();
}

}

}
}
