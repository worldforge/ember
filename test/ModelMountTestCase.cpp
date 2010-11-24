#include "ModelMountTestCase.h"

#include "components/ogre/model/ModelMount.h"
#include "components/ogre/model/Model.h"
#include "components/ogre/model/ModelDefinition.h"
#include "components/ogre/SceneNodeProvider.h"
#include <Ogre.h>
#include <wfmath/axisbox.h>
#include <wfmath/point.h>

using namespace EmberOgre;
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
		mDefinition = ::EmberOgre::Model::ModelDefnPtr(new ::EmberOgre::Model::ModelDefinition(0, "modeldef", 1, ""));
		mDefinition->setTranslate(Ogre::Vector3::ZERO);
	}

	const Ogre::AxisAlignedBox& getBoundingBox(void) const
	{
		return bbox;
	}

};

void ModelMountTestCase::testModelMountScaling()
{
	Ogre::Root root;
	Ogre::SceneManager* sceneManager = root.createSceneManager(Ogre::ST_GENERIC);

	TestModel model;

	Ogre::SceneNode* node = sceneManager->getRootSceneNode()->createChildSceneNode();

	//We get an error when it's destroyed. So we don't destroy it.
	SceneNodeProvider* nodeProvider = new SceneNodeProvider(*node, &model);

	Model::ModelMount mount(model, nodeProvider);

	WFMath::AxisBox<3> axisBox(WFMath::Point<3>(0, 0, 0), WFMath::Point<3>(10, 10, 10));
	model.bbox = Ogre::AxisAlignedBox(Ogre::Vector3(0, 0, 0), Ogre::Vector3(5, 5, 5));

	mount.rescale(&axisBox);

	CPPUNIT_ASSERT(nodeProvider->getNode().getScale() == Ogre::Vector3(2, 2, 2));

}
}
