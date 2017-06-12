#include "ModelMountTestCase.h"

#include "components/ogre/model/ModelMount.h"
#include "components/ogre/model/Model.h"
#include "components/ogre/model/ModelDefinition.h"
#include "components/ogre/SceneNodeProvider.h"
#include <Ogre.h>
#include <wfmath/axisbox.h>
#include <wfmath/point.h>

using namespace Ember::OgreView;
using namespace WFMath;

namespace Ember
{


/**
 * Checks equality with an epsilon of 0.00001f.
 * This is suited for sizes between 0 and 100.
 * @param v1
 * @param v2
 * @return True if the vectors was considered to be equal.
 */
bool equals(const Ogre::Vector3& v1, const Ogre::Vector3& v2)
{
	return (std::abs(v1.x - v2.x) < 0.00001f) &&
			(std::abs(v1.y - v2.y) < 0.00001f) &&
			(std::abs(v1.z - v2.z) < 0.00001f);
}


class TestModel: public Model::Model
{
public:

	Ogre::AxisAlignedBox bbox;

	TestModel(Ogre::SceneManager& manager, Ogre::SharedPtr<ModelDefinition> definition) :
		Model::Model(manager, definition, "testmodel")
	{
		mDefinition = ::Ember::OgreView::Model::ModelDefinitionPtr(new ::Ember::OgreView::Model::ModelDefinition(0, "modeldef", 1, ""));
		mDefinition->setTranslate(Ogre::Vector3::ZERO);
	}

	const Ogre::AxisAlignedBox& getBoundingBox(void) const
	{
		return bbox;
	}

};

void scaleAndTestMount(TestModel& model, Model::ModelMount& mount, const Ogre::Node& node)
{
	WFMath::AxisBox<3> axisBox(WFMath::Point<3>(0, 0, 0), WFMath::Point<3>(10.0f, 10.0f, 10.0f));

	model.bbox = Ogre::AxisAlignedBox(Ogre::Vector3(0, 0, 0), Ogre::Vector3(5.0f, 5.0f, 5.0f));
	mount.rescale(&axisBox);
	CPPUNIT_ASSERT(equals(node.getScale(), Ogre::Vector3(2.0f, 2.0f, 2.0f)));

	model.bbox = Ogre::AxisAlignedBox(Ogre::Vector3(0, 0, 0), Ogre::Vector3(20.0f, 20.0f, 20.0f));
	mount.rescale(&axisBox);
	CPPUNIT_ASSERT(equals(node.getScale(), Ogre::Vector3(0.5f, 0.5f, 0.5f)));

	model.bbox = Ogre::AxisAlignedBox(Ogre::Vector3(10.0f, 10.0f, 10.0f), Ogre::Vector3(20.0f, 20.0f, 20.0f));
	mount.rescale(&axisBox);
	CPPUNIT_ASSERT(equals(node.getScale(), Ogre::Vector3(1.0f, 1.0f, 1.0f)));

	model.bbox = Ogre::AxisAlignedBox(Ogre::Vector3(0, 10.0f, 15.0f), Ogre::Vector3(20.0f, 20.0f, 20.0f));
	mount.rescale(&axisBox);
	CPPUNIT_ASSERT(equals(node.getScale(), Ogre::Vector3(2.0f, 1.0f, 0.5f)));

}
void ModelMountTestCase::testModelMountScaling()
{
	Ogre::Root root;
	Ogre::SceneManager* sceneManager = root.createSceneManager(Ogre::ST_GENERIC);

	TestModel model;

	//First test with a straight forward case.
	Ogre::SceneNode* node = sceneManager->getRootSceneNode()->createChildSceneNode();
	//We get an error when it's destroyed. So we don't destroy it.
	SceneNodeProvider* nodeProvider = new SceneNodeProvider(*node, "", &model);
	Model::ModelMount mount(model, nodeProvider);
	scaleAndTestMount(model, mount, nodeProvider->getNode());

	//Test with the parent node being scaled
	node->setScale(Ogre::Vector3(3.0f, 0.2f, 200.0f));
	Ogre::SceneNode* subNode = node->createChildSceneNode();
	nodeProvider = new SceneNodeProvider(*subNode, "", &model);
	Model::ModelMount mount2(model, nodeProvider);
	scaleAndTestMount(model, mount2, nodeProvider->getNode());

	//Test with the parent node being scaled and rotated
	node->setScale(Ogre::Vector3(3.0f, 0.2f, 200.0f));
	node->yaw(Ogre::Degree(42));
	node->pitch(Ogre::Degree(76));
	node->roll(Ogre::Degree(98));
	subNode = node->createChildSceneNode();
	nodeProvider = new SceneNodeProvider(*subNode, "", &model);
	Model::ModelMount mount3(model, nodeProvider);
	scaleAndTestMount(model, mount3, nodeProvider->getNode());
}


}
