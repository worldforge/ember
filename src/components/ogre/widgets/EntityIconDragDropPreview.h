#ifndef EMBEROGRE_GUIENTITYICONDRAGDROPPREVIEW_H
#define EMBEROGRE_GUIENTITYICONDRAGDROPPREVIEW_H

#include "components/ogre/authoring/EntityRecipe.h"
#include "components/entitymapping/IActionCreator.h"
#include "components/ogre/authoring/MovementAdapter.h"
#include "components/ogre/authoring/IMovementBridge.h"
#include "components/ogre/authoring/EntityMoverBase.h"
#include <wfmath/point.h>
#include <wfmath/quaternion.h>

namespace Eris {
class Connection;
class TypeInfo;
}

namespace EmberOgre {

namespace Model {
class Model;
class ModelMount;
}

namespace Authoring {
class DetachedEntity;
}

namespace Gui {

class EntityIcon;
class EntityIconDragDropPreviewMovement;

class EntityIconDragDropPreview {
public:
	EntityIconDragDropPreview(Eris::Connection& conn);
	virtual ~EntityIconDragDropPreview();

	void createPreview(EntityIcon* icon);
	void setModel(const std::string& modelName);


	void initFromModel();

	void scaleNode();

	void model_Reloaded();

	Model::Model* getModel();
	bool hasBBox();
	const WFMath::AxisBox<3> & getBBox();

	void typeService_BoundType(Eris::TypeInfo* typeInfo);

private:
	Eris::Connection& mConn;
	Authoring::DetachedEntity* mEntity;
	Atlas::Message::MapType mEntityMessage;
	Ogre::SceneNode* mEntityNode;
	Ogre::Entity* mAxisMarker;
	Model::Model* mModel;
	Model::ModelMount* mModelMount;
	WFMath::Point<3> mPos;
	WFMath::Quaternion mOrientation;
	EntityIconDragDropPreviewMovement* mMovement;
};

class EntityIconDragDropPreviewActionCreator  : public Ember::EntityMapping::IActionCreator {
public:
	EntityIconDragDropPreviewActionCreator(EntityIconDragDropPreview& entityIconDragDrop);
	~EntityIconDragDropPreviewActionCreator();
	virtual void createActions(Ember::EntityMapping::EntityMapping& modelMapping, Ember::EntityMapping::Cases::CaseBase* aCase, Ember::EntityMapping::Definitions::CaseDefinition& caseDefinition);
protected:
	EntityIconDragDropPreview& mEntityIconDragDropPreview;
};

class EntityIconDragDropPreviewMovementBridge : public ::EmberOgre::Authoring::EntityMoverBase
{
public:

	EntityIconDragDropPreviewMovementBridge(EntityIconDragDropPreview& creator, Authoring::DetachedEntity& entity, Ogre::SceneNode* node);
	virtual ~EntityIconDragDropPreviewMovementBridge();

	virtual void finalizeMovement();
	virtual void cancelMovement();

private:
	EntityIconDragDropPreview& mCreator;

};

class EntityIconDragDropPreviewMovement
{
public:

	EntityIconDragDropPreviewMovement(EntityIconDragDropPreview& mEntityIconDragDropPreview, Authoring::DetachedEntity& entity, Ogre::SceneNode* node);
	~EntityIconDragDropPreviewMovement();

protected:
	Authoring::MovementAdapter mMoveAdapter;
};

}
}

#endif
