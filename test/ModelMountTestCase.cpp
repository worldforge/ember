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



class TestModel: public Model::Model
{
public:

	Ogre::AxisAlignedBox bbox;

	TestModel() :
		Model::Model("testmodel")
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
	WFMath::AxisBox<3> axisBox(WFMath::Point<3>(0, 0, 0), WFMath::Point<3>(10, 10, 10));

	model.bbox = Ogre::AxisAlignedBox(Ogre::Vector3(0, 0, 0), Ogre::Vector3(5, 5, 5));
	mount.rescale(&axisBox);
	CPPUNIT_ASSERT(node.getScale() == Ogre::Vector3(2, 2, 2));

	model.bbox = Ogre::AxisAlignedBox(Ogre::Vector3(0, 0, 0), Ogre::Vector3(20, 20, 20));
	mount.rescale(&axisBox);
	CPPUNIT_ASSERT(node.getScale() == Ogre::Vector3(0.5, 0.5, 0.5));

	model.bbox = Ogre::AxisAlignedBox(Ogre::Vector3(10, 10, 10), Ogre::Vector3(20, 20, 20));
	mount.rescale(&axisBox);
	CPPUNIT_ASSERT(node.getScale() == Ogre::Vector3(1, 1, 1));

	model.bbox = Ogre::AxisAlignedBox(Ogre::Vector3(0, 10, 15), Ogre::Vector3(20, 20, 20));
	mount.rescale(&axisBox);
	CPPUNIT_ASSERT(node.getScale() == Ogre::Vector3(2, 1, 0.5));

}
void ModelMountTestCase::testModelMountScaling()
{
	Ogre::Root root;
	Ogre::SceneManager* sceneManager = root.createSceneManager(Ogre::ST_GENERIC);

	TestModel model;

	//First test with a straight forward case.
	Ogre::SceneNode* node = sceneManager->getRootSceneNode()->createChildSceneNode();
	//We get an error when it's destroyed. So we don't destroy it.
	SceneNodeProvider* nodeProvider = new SceneNodeProvider(*node, &model);
	Model::ModelMount mount(model, nodeProvider);
	scaleAndTestMount(model, mount, nodeProvider->getNode());

	//Test with the parent node being scaled
	node->setScale(Ogre::Vector3(3, 0.2, 200));
	Ogre::SceneNode* subNode = node->createChildSceneNode();
	nodeProvider = new SceneNodeProvider(*subNode, &model);
	Model::ModelMount mount2(model, nodeProvider);
	scaleAndTestMount(model, mount2, nodeProvider->getNode());

	//Test with the parent node being scaled and rotated
	node->setScale(Ogre::Vector3(3, 0.2, 200));
	node->yaw(Ogre::Degree(42));
	node->pitch(Ogre::Degree(76));
	node->roll(Ogre::Degree(98));
	subNode = node->createChildSceneNode();
	nodeProvider = new SceneNodeProvider(*subNode, &model);
	Model::ModelMount mount3(model, nodeProvider);
	scaleAndTestMount(model, mount3, nodeProvider->getNode());
}


}
