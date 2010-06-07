#include "EntityIconDragDropPreview.h"

#include "main/Application.h"
#include "../EmberOgre.h"
#include "../Avatar.h"
#include "../Convert.h"

#include "EntityIcon.h"
#include <Eris/TypeInfo.h>
#include "components/ogre/EmberEntity.h"
#include "../authoring/DetachedEntity.h"
#include <Eris/Connection.h>
#include <OgreSceneManager.h>
#include "components/ogre/SceneNodeProvider.h"
#include "components/ogre/model/Model.h"
#include "components/ogre/model/ModelMount.h"
#include "components/ogre/mapping/EmberEntityMappingManager.h"

#include "components/entitymapping/Definitions/CaseDefinition.h"
#include "components/entitymapping/Cases/CaseBase.h"

using namespace Ember;
namespace EmberOgre
{

class EmberEntity;

namespace Gui
{



EntityIconDragDropPreview::EntityIconDragDropPreview(Eris::Connection& conn) :
		mConn(conn), mModelMount(0), mModel(0), mMovement(0), mAxisMarker(0)
{

	mOrientation.identity();
}

EntityIconDragDropPreview::~EntityIconDragDropPreview()
{
}

void EntityIconDragDropPreview::createPreview(EntityIcon* icon)
{

	EmberEntity* entity = icon->getEntity();
	Eris::TypeInfo* erisType = entity->getType();

	EmberEntity& avatar = EmberOgre::getSingleton().getAvatar()->getEmberEntity();

	WFMath::Vector<3> offset(2, 0, 0);
	mPos = (avatar.getPosition().isValid() ? avatar.getPosition() : WFMath::Point<3>::ZERO()) + (avatar.getOrientation().isValid() ? offset.rotate(avatar.getOrientation()) : WFMath::Vector<3>::ZERO());

	mEntityMessage = entity->getAttributes();
	mEntityMessage["loc"] = avatar.getLocation()->getId();
	mEntityMessage["name"] = erisType->getName();
	mEntityMessage["parents"] = Atlas::Message::ListType(1, erisType->getName());

	Eris::View* view = Ember::Application::getSingleton().getMainView();

	if (view)
	{
		// Temporary entity
		mEntity = new Authoring::DetachedEntity("-1", erisType, mConn.getTypeService());
		mEntity->setFromMessage(mEntityMessage);

		// Creating scene node
		mEntityNode = EmberOgre::getSingleton().getWorld()->getSceneManager().getRootSceneNode()->createChildSceneNode();

		if (!mAxisMarker) {
			try {
				mAxisMarker = mEntityNode->getCreator()->createEntity("EntityCreator_axisMarker", "axes.mesh");
				mEntityNode->attachObject(mAxisMarker);
			} catch (const std::exception& ex) {
				S_LOG_WARNING("Error when loading axes mesh."<< ex);
			}
		}

		// Making model from temporary entity
		EntityIconDragDropPreviewActionCreator actionCreator(*this);
		std::auto_ptr<Ember::EntityMapping::EntityMapping> modelMapping(Mapping::EmberEntityMappingManager::getSingleton().getManager().createMapping(*mEntity, &actionCreator, 0));
		if (modelMapping.get()) {
			modelMapping->initialize();
		}

		// Registering move adapter to track mouse movements
		mMovement = new EntityIconDragDropPreviewMovement(*this, *mEntity, mEntityNode);
		//mMoveAdapter->addAdapter();
	}
}

void EntityIconDragDropPreview::setModel(const std::string& modelName)
{
	if (mModel) {
		if (mModel->getDefinition()->getName() == modelName) {
			return;
		} else {
			///Reset the model mount to start with.
			delete mModelMount;
			mModelMount = 0;
			EmberOgre::getSingleton().getWorld()->getSceneManager().destroyMovableObject(mModel);
		}
	}
	mModel = Model::Model::createModel(EmberOgre::getSingleton().getWorld()->getSceneManager(), modelName);
	mModel->Reloaded.connect(sigc::mem_fun(*this, &EntityIconDragDropPreview::model_Reloaded));

	///if the model definition isn't valid, use a placeholder
	if (!mModel->getDefinition()->isValid()) {
		S_LOG_FAILURE( "Could not find " << modelName << ", using placeholder.");
		///add a placeholder model
		Model::ModelDefnPtr modelDef = mModel->getDefinition();
		modelDef->createSubModelDefinition("3d_objects/primitives/models/box.mesh")->createPartDefinition("main")->setShow(true);
		modelDef->setValid(true);
		modelDef->reloadAllInstances();
	}

	mModelMount = new Model::ModelMount(*mModel, new SceneNodeProvider(*mEntityNode, mModel));

	initFromModel();

	// Setting inital position and orientation
	if (mPos.isValid()) {
		mEntityNode->setPosition(Convert::toOgre(mPos));
	}
	mEntityNode->setOrientation(Convert::toOgre(mOrientation));
}

void EntityIconDragDropPreview::initFromModel()
{
	scaleNode();
}

void EntityIconDragDropPreview::model_Reloaded()
{
	initFromModel();
}

void EntityIconDragDropPreview::scaleNode()
{
	if (mModelMount) {
		mModelMount->rescale(hasBBox() ? &getBBox() : 0);
	} else {
		S_LOG_WARNING("Tried to scale node without there being a valid model mount.");
	}
}

Model::Model* EntityIconDragDropPreview::getModel()
{
	return mModel;
}

bool EntityIconDragDropPreview::hasBBox()
{
	return mEntity->hasBBox();
}

const WFMath::AxisBox<3> & EntityIconDragDropPreview::getBBox()
{
	return mEntity->getBBox();
}


EntityIconDragDropPreviewActionCreator::EntityIconDragDropPreviewActionCreator(EntityIconDragDropPreview& entityIconDragDropPreview)
		: mEntityIconDragDropPreview(entityIconDragDropPreview)
{
}

EntityIconDragDropPreviewActionCreator::~EntityIconDragDropPreviewActionCreator()
{
}

void EntityIconDragDropPreviewActionCreator::createActions(EntityMapping::EntityMapping& modelMapping, EntityMapping::Cases::CaseBase* aCase, EntityMapping::Definitions::CaseDefinition& caseDefinition)
{
	EntityMapping::Definitions::CaseDefinition::ActionStore::iterator endJ = caseDefinition.getActions().end();
	for (EntityMapping::Definitions::CaseDefinition::ActionStore::iterator J = caseDefinition.getActions().begin(); J != endJ; ++J) {
		if (J->getType() == "display-model") {
			mEntityIconDragDropPreview.setModel(J->getValue());
		} else if (J->getType() == "hide-model") {
			mEntityIconDragDropPreview.setModel("");
		}
	}
}

EntityIconDragDropPreviewMovementBridge::EntityIconDragDropPreviewMovementBridge(EntityIconDragDropPreview& creator, Authoring::DetachedEntity& entity, Ogre::SceneNode* node) :
	::EmberOgre::Authoring::EntityMoverBase(entity, node), mCreator(creator)
{
}

EntityIconDragDropPreviewMovementBridge::~EntityIconDragDropPreviewMovementBridge()
{
}

void EntityIconDragDropPreviewMovementBridge::finalizeMovement()
{
}
void EntityIconDragDropPreviewMovementBridge::cancelMovement()
{
}

EntityIconDragDropPreviewMovement::EntityIconDragDropPreviewMovement(EntityIconDragDropPreview& mEntityIconDragDropPreview, Authoring::DetachedEntity& entity, Ogre::SceneNode* node)
{
	// When the point is moved, an instance of this will be created and the movement handled by it.
	// Note that ownership will be transferred to the adapter, so we shouldn't keep a reference
	EntityIconDragDropPreviewMovementBridge* bridge = new EntityIconDragDropPreviewMovementBridge(mEntityIconDragDropPreview, entity, node);
	mMoveAdapter.attachToBridge(bridge);
	mMoveAdapter.update();
}

EntityIconDragDropPreviewMovement::~EntityIconDragDropPreviewMovement()
{
}

}
}
