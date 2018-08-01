/*
 Copyright (C) 2018 Erik Ogenvik

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "GeometryVisualization.h"
#include "domain/EmberEntity.h"
#include "components/ogre/Convert.h"

#include "framework/LoggingInstance.h"
#include "framework/AtlasQuery.h"
#include "components/ogre/model/Model.h"
#include "components/ogre/model/ModelRepresentationManager.h"

#include <wfmath/atlasconv.h>

#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreWireBoundingBox.h>
#include <OgreManualObject.h>
#include <OgreSubEntity.h>
#include <OgreMeshManager.h>
#include <random>

namespace Ember {
namespace OgreView {
namespace Authoring {

GeometryVisualization::GeometryVisualization(EmberEntity& entity, Ogre::SceneNode* sceneNode) :
		mEntity(entity),
		mSceneNode(sceneNode),
		mManualObject(nullptr),
		mOgreEntity(nullptr),
		mBboxConnection(entity.observe("bbox", sigc::mem_fun(*this, &GeometryVisualization::entity_BboxChanged))),
		mScaleConnection(entity.observe("scale", sigc::mem_fun(*this, &GeometryVisualization::entity_BboxChanged))) {
	mManualObject = sceneNode->getCreator()->createManualObject();
	mManualObject->setRenderQueueGroup(Ogre::RENDER_QUEUE_SKIES_LATE - 1); //We want to render the geometry on top of everything
	mSceneNode->attachObject(mManualObject);
	buildGeometry();
	updateBbox();
	updatePositionAndOrientation();
	mEntity.Moved.connect(sigc::mem_fun(*this, &GeometryVisualization::entity_Moved));
}

GeometryVisualization::~GeometryVisualization() {
	mBboxConnection.disconnect();
	mScaleConnection.disconnect();
	if (mManualObject) {
		mSceneNode->getCreator()->destroyManualObject(mManualObject);
	}
	if (mOgreEntity) {
		mSceneNode->getCreator()->destroyEntity(mOgreEntity);
	}

	mSceneNode->removeAndDestroyAllChildren();
	mSceneNode->getCreator()->destroySceneNode(mSceneNode);
}

void GeometryVisualization::entity_Moved() {
	updatePositionAndOrientation();
}

void GeometryVisualization::entity_BboxChanged(const Atlas::Message::Element& /*attributeValue*/) {
	updateBbox();
}

void GeometryVisualization::updateBbox() {
	if (mEntity.hasBBox() && mEntity.getBBox().isValid() && mBboxUpdateFn) {
		mBboxUpdateFn();
	}
}

void GeometryVisualization::updatePositionAndOrientation() {
	if (mEntity.getPredictedPos().isValid()) {
		mSceneNode->setPosition(Convert::toOgre(mEntity.getPredictedPos()));
	}
	if (mEntity.getOrientation().isValid()) {
		mSceneNode->setOrientation(Convert::toOgre(mEntity.getOrientation()));
	}
}

void GeometryVisualization::placeSphere(float radius, Ogre::Vector3 offset) {
	int numberOfRings = 16;
	int numberOfSegments = 16;
	float fDeltaRingAngle = (Ogre::Math::PI / numberOfRings);
	float fDeltaSegAngle = (2 * Ogre::Math::PI / numberOfSegments);
	size_t wVerticeIndex = mManualObject->getCurrentVertexCount();

	for (int ring = 0; ring <= numberOfRings; ring++) {
		float r0 = radius * sinf(ring * fDeltaRingAngle);
		float y0 = radius * cosf(ring * fDeltaRingAngle) + offset.y;

		for (int seg = 0; seg <= numberOfSegments; seg++) {
			float x0 = r0 * sinf(seg * fDeltaSegAngle) + offset.x;
			float z0 = r0 * cosf(seg * fDeltaSegAngle) + offset.z;

			mManualObject->position(x0, y0, z0);

			Ogre::Vector3 vNormal = Ogre::Vector3(x0, y0, z0).normalisedCopy();
			mManualObject->normal(vNormal);

			mManualObject->colour((vNormal.x + 1.0f) / 2.0f, (vNormal.y + 1.0f) / 2.0f, (vNormal.z + 1.0f) / 2, 1.0f);


			if (ring != numberOfRings) {
				mManualObject->index(wVerticeIndex + numberOfSegments + 1);
				mManualObject->index(wVerticeIndex);
				mManualObject->index(wVerticeIndex + numberOfSegments);
				mManualObject->index(wVerticeIndex + numberOfSegments + 1);
				mManualObject->index(wVerticeIndex + 1);
				mManualObject->index(wVerticeIndex);
				wVerticeIndex++;
			}
		};
	}
}

void GeometryVisualization::placeCylinderX(float radius, float halfHeight, Ogre::Vector3 offset) {

	int numberOfSegments = 32;
	float fDeltaSegAngle = (2 * Ogre::Math::PI / numberOfSegments);
	size_t vertexIndex = mManualObject->getCurrentVertexCount() - 1;

	//First do the top circle
	float x0 = halfHeight + offset.x;

	Ogre::Vector3 vNormal(1, 0, 0);
	mManualObject->position(Ogre::Vector3(x0, offset.y, offset.z));
	mManualObject->normal(vNormal);
	mManualObject->colour((vNormal.x + 1.0f) / 2.0f, (vNormal.y + 1.0f) / 2.0f, (vNormal.z + 1.0f) / 2.0f, 1.0f);
	vertexIndex++;
	size_t topCenterVertexIndex = vertexIndex;

	for (int seg = 0; seg <= numberOfSegments; ++seg) {
		float y0 = radius * sinf(seg * fDeltaSegAngle) + offset.y;
		float z0 = radius * cosf(seg * fDeltaSegAngle) + offset.z;

		mManualObject->position(Ogre::Vector3(x0, y0, z0));

		mManualObject->normal(vNormal);

		mManualObject->colour((vNormal.x + 1.0f) / 2.0f, (vNormal.y + 1.0f) / 2.0f, (vNormal.z + 1.0f) / 2.0f, 1.0f);

		vertexIndex++;
	};
	for (int seg = 0; seg <= numberOfSegments; ++seg) {
		mManualObject->index(topCenterVertexIndex);
		mManualObject->index(topCenterVertexIndex + seg + 1);
		mManualObject->index(topCenterVertexIndex + seg);
	}


	//Then do the bottom circle
	x0 = -halfHeight + offset.x;

	vNormal = Ogre::Vector3(-1, 0, 0);
	mManualObject->position(Ogre::Vector3(x0, offset.y, offset.z));
	mManualObject->normal(vNormal);
	mManualObject->colour((vNormal.x + 1.0f) / 2.0f, (vNormal.y + 1.0f) / 2.0f, (vNormal.z + 1.0f) / 2.0f, 1.0f);
	vertexIndex++;
	size_t bottomCenterVertexIndex = vertexIndex;

	for (int seg = 0; seg <= numberOfSegments; ++seg) {
		float y0 = radius * sinf(seg * fDeltaSegAngle) + offset.y;
		float z0 = radius * cosf(seg * fDeltaSegAngle) + offset.z;

		mManualObject->position(Ogre::Vector3(x0, y0, z0));

		mManualObject->normal(vNormal);

		mManualObject->colour((vNormal.x + 1.0f) / 2.0f, (vNormal.y + 1.0f) / 2.0f, (vNormal.z + 1.0f) / 2.0f, 1.0f);

		vertexIndex++;
	};
	for (int seg = 0; seg <= numberOfSegments; ++seg) {
		mManualObject->index(bottomCenterVertexIndex);
		mManualObject->index(bottomCenterVertexIndex + seg);
		mManualObject->index(bottomCenterVertexIndex + seg + 1);
	}

	//Lastly do the cylinder walls
	for (int seg = 0; seg <= numberOfSegments; ++seg) {
		mManualObject->quad(topCenterVertexIndex + seg + 1, topCenterVertexIndex + seg + 2, bottomCenterVertexIndex + seg + 2, bottomCenterVertexIndex + seg + 1);
	}

}


void GeometryVisualization::placeCylinderY(float radius, float halfHeight, Ogre::Vector3 offset) {


	int numberOfSegments = 32;
	float fDeltaSegAngle = (2 * Ogre::Math::PI / numberOfSegments);
	size_t vertexIndex = mManualObject->getCurrentVertexCount() - 1;

	//First do the top circle
	float y0 = halfHeight + offset.y;

	Ogre::Vector3 vNormal(0, 1, 0);
	mManualObject->position(Ogre::Vector3(offset.x, y0, offset.z));
	mManualObject->normal(vNormal);
	mManualObject->colour((vNormal.x + 1.0f) / 2.0f, (vNormal.y + 1.0f) / 2.0f, (vNormal.z + 1.0f) / 2.0f, 1.0f);
	vertexIndex++;
	size_t topCenterVertexIndex = vertexIndex;

	for (int seg = 0; seg <= numberOfSegments; ++seg) {
		float x0 = radius * sinf(seg * fDeltaSegAngle) + offset.x;
		float z0 = radius * cosf(seg * fDeltaSegAngle) + offset.z;

		mManualObject->position(Ogre::Vector3(x0, y0, z0));

		mManualObject->normal(vNormal);

		mManualObject->colour((vNormal.x + 1.0f) / 2.0f, (vNormal.y + 1.0f) / 2.0f, (vNormal.z + 1.0f) / 2.0f, 1.0f);

		vertexIndex++;
	};
	for (int seg = 0; seg <= numberOfSegments; ++seg) {
		mManualObject->index(topCenterVertexIndex);
		mManualObject->index(topCenterVertexIndex + seg);
		mManualObject->index(topCenterVertexIndex + seg + 1);
	}


	//Then do the bottom circle
	y0 = -halfHeight + offset.y;

	vNormal = Ogre::Vector3(0, -1, 0);
	mManualObject->position(Ogre::Vector3(offset.x, y0, offset.z));
	mManualObject->normal(vNormal);
	mManualObject->colour((vNormal.x + 1.0f) / 2.0f, (vNormal.y + 1.0f) / 2.0f, (vNormal.z + 1.0f) / 2.0f, 1.0f);
	vertexIndex++;
	size_t bottomCenterVertexIndex = vertexIndex;

	for (int seg = 0; seg <= numberOfSegments; ++seg) {
		float x0 = radius * sinf(seg * fDeltaSegAngle) + offset.x;
		float z0 = radius * cosf(seg * fDeltaSegAngle) + offset.z;

		mManualObject->position(Ogre::Vector3(x0, y0, z0));

		mManualObject->normal(vNormal);

		mManualObject->colour((vNormal.x + 1.0f) / 2.0f, (vNormal.y + 1.0f) / 2.0f, (vNormal.z + 1.0f) / 2.0f, 1.0f);

		vertexIndex++;
	};
	for (int seg = 0; seg <= numberOfSegments; ++seg) {
		mManualObject->index(bottomCenterVertexIndex);
		mManualObject->index(bottomCenterVertexIndex + seg + 1);
		mManualObject->index(bottomCenterVertexIndex + seg);
	}

	//Lastly do the cylinder walls
	for (int seg = 0; seg <= numberOfSegments; ++seg) {
		mManualObject->quad(topCenterVertexIndex + seg + 2, topCenterVertexIndex + seg + 1, bottomCenterVertexIndex + seg + 1, bottomCenterVertexIndex + seg + 2);
	}

}


void GeometryVisualization::placeCylinderZ(float radius, float halfHeight, Ogre::Vector3 offset) {

	int numberOfSegments = 32;
	float fDeltaSegAngle = (2 * Ogre::Math::PI / numberOfSegments);
	size_t vertexIndex = mManualObject->getCurrentVertexCount() - 1;

	//First do the top circle
	float z0 = halfHeight + offset.z;

	Ogre::Vector3 vNormal(0, 0, 1);
	mManualObject->position(Ogre::Vector3(offset.x, offset.y, z0));
	mManualObject->normal(vNormal);
	mManualObject->colour((vNormal.x + 1.0f) / 2.0f, (vNormal.y + 1.0f) / 2.0f, (vNormal.z + 1.0f) / 2.0f, 1.0f);
	vertexIndex++;
	size_t topCenterVertexIndex = vertexIndex;

	for (int seg = 0; seg <= numberOfSegments; ++seg) {
		float y0 = radius * sinf(seg * fDeltaSegAngle) + offset.y;
		float x0 = radius * cosf(seg * fDeltaSegAngle) + offset.x;

		mManualObject->position(Ogre::Vector3(x0, y0, z0));

		mManualObject->normal(vNormal);

		mManualObject->colour((vNormal.x + 1.0f) / 2.0f, (vNormal.y + 1.0f) / 2.0f, (vNormal.z + 1.0f) / 2.0f, 1.0f);

		vertexIndex++;
	};
	for (int seg = 0; seg <= numberOfSegments; ++seg) {
		mManualObject->index(topCenterVertexIndex);
		mManualObject->index(topCenterVertexIndex + seg);
		mManualObject->index(topCenterVertexIndex + seg + 1);
	}


	//Then do the bottom circle
	z0 = -halfHeight + offset.z;

	vNormal = Ogre::Vector3(0, 0, -1);
	mManualObject->position(Ogre::Vector3(offset.x, offset.y, z0));
	mManualObject->normal(vNormal);
	mManualObject->colour((vNormal.x + 1.0f) / 2.0f, (vNormal.y + 1.0f) / 2.0f, (vNormal.z + 1.0f) / 2.0f, 1.0f);
	vertexIndex++;
	size_t bottomCenterVertexIndex = vertexIndex;

	for (int seg = 0; seg <= numberOfSegments; ++seg) {
		float y0 = radius * sinf(seg * fDeltaSegAngle) + offset.y;
		float x0 = radius * cosf(seg * fDeltaSegAngle) + offset.x;

		mManualObject->position(Ogre::Vector3(x0, y0, z0));

		mManualObject->normal(vNormal);

		mManualObject->colour((vNormal.x + 1.0f) / 2.0f, (vNormal.y + 1.0f) / 2.0f, (vNormal.z + 1.0f) / 2.0f, 1.0f);

		vertexIndex++;
	};
	for (int seg = 0; seg <= numberOfSegments; ++seg) {
		mManualObject->index(bottomCenterVertexIndex);
		mManualObject->index(bottomCenterVertexIndex + seg + 1);
		mManualObject->index(bottomCenterVertexIndex + seg);
	}

	//Lastly do the cylinder walls
	for (int seg = 0; seg <= numberOfSegments; ++seg) {
		mManualObject->quad(topCenterVertexIndex + seg + 2, topCenterVertexIndex + seg + 1, bottomCenterVertexIndex + seg + 1, bottomCenterVertexIndex + seg + 2);
	}

}

void GeometryVisualization::placeBox(Ogre::AxisAlignedBox bbox) {
	std::vector<Ogre::Vector3> vertices = {
			{bbox.getMaximum().x, bbox.getMaximum().y, bbox.getMaximum().z},
			{bbox.getMinimum().x, bbox.getMaximum().y, bbox.getMaximum().z},
			{bbox.getMinimum().x, bbox.getMaximum().y, bbox.getMinimum().z},
			{bbox.getMaximum().x, bbox.getMaximum().y, bbox.getMinimum().z},
			{bbox.getMaximum().x, bbox.getMinimum().y, bbox.getMaximum().z},
			{bbox.getMinimum().x, bbox.getMinimum().y, bbox.getMaximum().z},
			{bbox.getMinimum().x, bbox.getMinimum().y, bbox.getMinimum().z},
			{bbox.getMaximum().x, bbox.getMinimum().y, bbox.getMinimum().z}
	};

	//Counter-clockwise
	std::vector<std::tuple<Ogre::uint32, Ogre::uint32, Ogre::uint32, Ogre::uint32>> quads;
	quads.emplace_back(std::make_tuple(0, 3, 2, 1));
	quads.emplace_back(std::make_tuple(4, 5, 6, 7));
	quads.emplace_back(std::make_tuple(1, 5, 4, 0));
	quads.emplace_back(std::make_tuple(2, 6, 5, 1));
	quads.emplace_back(std::make_tuple(3, 7, 6, 2));
	quads.emplace_back(std::make_tuple(0, 4, 7, 3));

	auto firstVertex = static_cast<Ogre::uint32>(mManualObject->getCurrentVertexCount());
	for (const auto& vertex : vertices) {
		mManualObject->position(vertex);
		auto normalized = vertex.normalisedCopy();
		mManualObject->colour(normalized.x, normalized.y, normalized.z, 1.0f);
	}

	for (const auto& quad : quads) {
		mManualObject->quad(std::get<0>(quad) + firstVertex, std::get<1>(quad) + firstVertex, std::get<2>(quad) + firstVertex, std::get<3>(quad) + firstVertex);
	}
}


void GeometryVisualization::buildGeometry() {

	auto buildBoxFn = [this]() {
		mManualObject->clear();
		mManualObject->begin("/common/base/authoring/geometry", Ogre::RenderOperation::OT_TRIANGLE_LIST);

		placeBox(Convert::toOgre(mEntity.getBBox()));

		mManualObject->end();

	};

	auto buildSphereFn = [this](const WFMath::Vector<3>& size, float radius) {
		mManualObject->clear();
		mManualObject->begin("/common/base/authoring/geometry", Ogre::RenderOperation::OT_TRIANGLE_LIST);

		auto offset = mEntity.getBBox().lowCorner() + (size / 2.0f);

		placeSphere(radius, Convert::toOgre(offset));
		mManualObject->end();

	};

	if (mEntity.hasAttr("geometry")) {
		auto& geometry = mEntity.valueOfAttr("geometry");
		AtlasQuery::find<Atlas::Message::StringType>(geometry, "type", [&](const Atlas::Message::StringType& type) {
			if (type == "mesh") {
				if (geometry.asMap().find("indices") != geometry.asMap().end()) {
					AtlasQuery::find<Atlas::Message::ListType>(geometry, "indices", [&](const Atlas::Message::ListType& indices) {
						AtlasQuery::find<Atlas::Message::ListType>(
								geometry, "vertices", [&](const Atlas::Message::ListType& vertices) {
									mManualObject->clear();

									mManualObject->begin("/common/base/authoring/geometry", Ogre::RenderOperation::OT_TRIANGLE_LIST);

									mManualObject->estimateIndexCount(indices.size());
									mManualObject->estimateVertexCount(vertices.size());

									for (size_t i = 0; i < vertices.size(); i += 3) {
										Ogre::Vector3 vertex(static_cast<const Ogre::Real>(vertices[i].Float()),
															 static_cast<const Ogre::Real>(vertices[i + 1].Float()),
															 static_cast<const Ogre::Real>(vertices[i + 2].Float()));
										mManualObject->position(vertex);
										auto normalized = vertex.normalisedCopy();
										mManualObject->colour((normalized.x + 1.0f) / 2.0f, (normalized.y + 1.0f) / 2.0f, (normalized.z + 1.0f) / 2.0f, 1.0f);
									}
									for (size_t i = 0; i < indices.size(); i += 3) {
										mManualObject->triangle(static_cast<Ogre::uint32>(indices[i].Int()),
																static_cast<Ogre::uint32>(indices[i + 1].Int()),
																static_cast<Ogre::uint32>(indices[i + 2].Int()));
									}

									mManualObject->end();

									mBboxUpdateFn = [this]() {
										Ogre::Vector3 entitySize = Convert::toOgre(mEntity.getBBox()).getSize();

										Ogre::Vector3 manualObjectSize = mManualObject->getBoundingBox().getSize();

										Ogre::Vector3 scaling = entitySize / manualObjectSize;
										mSceneNode->setScale(scaling);
									};
								}
						);
					});
				} else {
					AtlasQuery::find<Atlas::Message::StringType>(geometry, "path", [&](const Atlas::Message::StringType& path) {
						auto meshPtr = Ogre::MeshManager::getSingleton().getByName(path, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
						if (meshPtr) {
							auto entity = mSceneNode->getCreator()->createEntity(meshPtr);
							for (auto subentity :                                        entity->getSubEntities()) {
								subentity->setMaterialName("/common/base/authoring/normals", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
							}
							mOgreEntity = entity;
							mSceneNode->attachObject(entity);

							mBboxUpdateFn = [this]() {
								Ogre::Vector3 scaling = Convert::toOgre(mEntity.getBBox()).getSize() / mOgreEntity->getBoundingBox().getSize();
								mSceneNode->setScale(scaling);
							};
						}
					});
				}
			} else if (type == "box") {
				mBboxUpdateFn = buildBoxFn;
			} else if (type == "sphere") {
				mBboxUpdateFn = [=]() {
					auto size = mEntity.getBBox().highCorner() - mEntity.getBBox().lowCorner();
					float radius = std::min(size.x(), std::min(size.y(), size.z())) * 0.5f;

					auto scalerI = geometry.Map().find("scaler");
					if (scalerI != geometry.Map().end() && scalerI->second.isString()) {
						auto scaler = scalerI->second.String();
						if (scaler == "min") {
							radius = std::min(size.x(), std::min(size.y(), size.z())) * 0.5f;
						} else if (scaler == "max") {
							radius = std::max(size.x(), std::max(size.y(), size.z())) * 0.5f;
						} else if (scaler == "x") {
							radius = size.x() * 0.5f;
						} else if (scaler == "y") {
							radius = size.y() * 0.5f;
						} else if (scaler == "z") {
							radius = size.z() * 0.5f;
						}
 					}

					buildSphereFn(size, radius);
				};
			} else if (type == "capsule-x") {
				mBboxUpdateFn = [&]() {
					mManualObject->clear();
					mManualObject->begin("/common/base/authoring/geometry", Ogre::RenderOperation::OT_TRIANGLE_LIST);

					auto size = mEntity.getBBox().highCorner() - mEntity.getBBox().lowCorner();
					float radius = std::min(size.y(), size.z()) * 0.5f;

					auto scalerI = geometry.Map().find("scaler");
					if (scalerI != geometry.Map().end() && scalerI->second.isString()) {
						auto scaler = scalerI->second.String();
						if (scaler == "min") {
							radius = std::min(size.y(), size.z()) * 0.5f;
						} else if (scaler == "max") {
							radius = std::max(size.y(), size.z()) * 0.5f;
						} else if (scaler == "x") {
							radius = size.x() * 0.5f;
						} else if (scaler == "y") {
							radius = size.y() * 0.5f;
						} else if (scaler == "z") {
							radius = size.z() * 0.5f;
						}
					}

					auto offset = mEntity.getBBox().lowCorner() + (size / 2.0f);
					offset.x() = mEntity.getBBox().highCorner().x() - radius;
					placeSphere(radius, Convert::toOgre(offset));

					offset = mEntity.getBBox().lowCorner() + (size / 2.0f);
					offset.x() = mEntity.getBBox().lowCorner().x() + radius;
					placeSphere(radius, Convert::toOgre(offset));

					offset = mEntity.getBBox().lowCorner() + (size / 2.0f);
					placeCylinderX(radius, (size.x() / 2.0f) - radius, Convert::toOgre(offset));

					mManualObject->end();
				};
			} else if (type == "capsule-y") {
				mBboxUpdateFn = [&]() {
					mManualObject->clear();
					mManualObject->begin("/common/base/authoring/geometry", Ogre::RenderOperation::OT_TRIANGLE_LIST);

					auto size = mEntity.getBBox().highCorner() - mEntity.getBBox().lowCorner();
					float radius = std::min(size.x(), size.z()) * 0.5f;

					auto scalerI = geometry.Map().find("scaler");
					if (scalerI != geometry.Map().end() && scalerI->second.isString()) {
						auto scaler = scalerI->second.String();
						if (scaler == "min") {
							radius = std::min(size.x(), size.z()) * 0.5f;
						} else if (scaler == "max") {
							radius = std::max(size.x(), size.z()) * 0.5f;
						} else if (scaler == "x") {
							radius = size.x() * 0.5f;
						} else if (scaler == "y") {
							radius = size.y() * 0.5f;
						} else if (scaler == "z") {
							radius = size.z() * 0.5f;
						}
					}

					auto offset = mEntity.getBBox().lowCorner() + (size / 2.0f);
					offset.y() = mEntity.getBBox().highCorner().y() - radius;
					placeSphere(radius, Convert::toOgre(offset));

					offset = mEntity.getBBox().lowCorner() + (size / 2.0f);
					offset.y() = mEntity.getBBox().lowCorner().y() + radius;
					placeSphere(radius, Convert::toOgre(offset));

					offset = mEntity.getBBox().lowCorner() + (size / 2.0f);
					placeCylinderY(radius, (size.y() / 2.0f) - radius, Convert::toOgre(offset));

					mManualObject->end();
				};
			} else if (type == "capsule-z") {
				mBboxUpdateFn = [&]() {
					mManualObject->clear();
					mManualObject->begin("/common/base/authoring/geometry", Ogre::RenderOperation::OT_TRIANGLE_LIST);

					auto size = mEntity.getBBox().highCorner() - mEntity.getBBox().lowCorner();
					float radius = std::min(size.x(), size.y()) * 0.5f;

					auto scalerI = geometry.Map().find("scaler");
					if (scalerI != geometry.Map().end() && scalerI->second.isString()) {
						auto scaler = scalerI->second.String();
						if (scaler == "min") {
							radius = std::min(size.y(), size.x()) * 0.5f;
						} else if (scaler == "max") {
							radius = std::max(size.y(), size.x()) * 0.5f;
						} else if (scaler == "x") {
							radius = size.x() * 0.5f;
						} else if (scaler == "y") {
							radius = size.y() * 0.5f;
						} else if (scaler == "z") {
							radius = size.z() * 0.5f;
						}
					}

					auto offset = mEntity.getBBox().lowCorner() + (size / 2.0f);
					offset.z() = mEntity.getBBox().highCorner().z() - radius;
					placeSphere(radius, Convert::toOgre(offset));

					offset = mEntity.getBBox().lowCorner() + (size / 2.0f);
					offset.z() = mEntity.getBBox().lowCorner().z() + radius;
					placeSphere(radius, Convert::toOgre(offset));

					offset = mEntity.getBBox().lowCorner() + (size / 2.0f);
					placeCylinderZ(radius, (size.z() / 2.0f) - radius, Convert::toOgre(offset));

					mManualObject->end();
				};
			} else if (type == "cylinder-x") {
				mBboxUpdateFn = [&]() {
					mManualObject->clear();
					mManualObject->begin("/common/base/authoring/geometry", Ogre::RenderOperation::OT_TRIANGLE_LIST);

					auto size = mEntity.getBBox().highCorner() - mEntity.getBBox().lowCorner();
					float radius = std::min(size.y(), size.z()) * 0.5f;

					auto offset = mEntity.getBBox().lowCorner() + (size / 2.0f);
					placeCylinderX(radius, (size.x() / 2.0f), Convert::toOgre(offset));

					mManualObject->end();
				};
			} else if (type == "cylinder-y") {
				mBboxUpdateFn = [&]() {
					mManualObject->clear();
					mManualObject->begin("/common/base/authoring/geometry", Ogre::RenderOperation::OT_TRIANGLE_LIST);

					auto size = mEntity.getBBox().highCorner() - mEntity.getBBox().lowCorner();
					float radius = std::min(size.x(), size.z()) * 0.5f;

					auto offset = mEntity.getBBox().lowCorner() + (size / 2.0f);

					offset = mEntity.getBBox().lowCorner() + (size / 2.0f);
					placeCylinderY(radius, size.y() / 2.0f, Convert::toOgre(offset));

					mManualObject->end();
				};
			} else if (type == "cylinder-z") {
				mBboxUpdateFn = [&]() {
					mManualObject->clear();
					mManualObject->begin("/common/base/authoring/geometry", Ogre::RenderOperation::OT_TRIANGLE_LIST);

					auto size = mEntity.getBBox().highCorner() - mEntity.getBBox().lowCorner();
					float radius = std::min(size.x(), size.y()) * 0.5f;

					auto offset = mEntity.getBBox().lowCorner() + (size / 2.0f);
					placeCylinderZ(radius, (size.z() / 2.0f), Convert::toOgre(offset));

					mManualObject->end();
				};
			} else if (type == "compound") {
				mBboxUpdateFn = [&]() {
					mManualObject->clear();
					mManualObject->begin("/common/base/authoring/geometry", Ogre::RenderOperation::OT_TRIANGLE_LIST);

					AtlasQuery::find<Atlas::Message::ListType>(geometry, "shapes", [&](const Atlas::Message::ListType& shapes) {
						for (auto& shapeElement : shapes) {
							AtlasQuery::find<std::string>(shapeElement, "type", [&](const std::string& type) {
								if (type == "box") {
									AtlasQuery::find<Atlas::Message::ListType>(shapeElement, "points", [&](const Atlas::Message::ListType& points) {
										WFMath::AxisBox<3> bbox(points);

										placeBox(Convert::toOgre(bbox));
									});
								}
							});
						}
					});

					mManualObject->end();


					auto entityAxisBox = Convert::toOgre(mEntity.getBBox());
					auto scaling = entityAxisBox.getSize() / mManualObject->getBoundingBox().getSize();
					mSceneNode->setScale(scaling);
				};
			}

		});
	} else {
		mBboxUpdateFn = buildBoxFn;
	}
}


}
}
}
