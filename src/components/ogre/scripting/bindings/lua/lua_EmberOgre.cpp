/*
** Lua binding: EmberOgre
** Generated automatically by tolua++-1.0.92 on Thu Mar 27 20:07:04 2008.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_EmberOgre_open (lua_State* tolua_S);

#include "required.h"
#include "components/ogre/carpenter/Carpenter.h"
#include "components/ogre/widgets/AssetsManager.h"
#include "components/ogre/widgets/Compass.h"
#include "components/ogre/terrain/Map.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_EmberOgre__Model__ModelDefnPtr (lua_State* tolua_S)
{
 EmberOgre::Model::ModelDefnPtr* self = (EmberOgre::Model::ModelDefnPtr*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Jesus (lua_State* tolua_S)
{
 EmberOgre::Jesus* self = (EmberOgre::Jesus*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_std__set_std__string_ (lua_State* tolua_S)
{
 std::set<std::string>* self = (std::set<std::string>*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Gui__IconBar (lua_State* tolua_S)
{
 EmberOgre::Gui::IconBar* self = (EmberOgre::Gui::IconBar*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_Carpenter__AttachPair (lua_State* tolua_S)
{
 Carpenter::AttachPair* self = (Carpenter::AttachPair*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_Carpenter__BuildingBlockSpec (lua_State* tolua_S)
{
 Carpenter::BuildingBlockSpec* self = (Carpenter::BuildingBlockSpec*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__AttributeObserver (lua_State* tolua_S)
{
 EmberOgre::AttributeObserver* self = (EmberOgre::AttributeObserver*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Gui__StackableContainer (lua_State* tolua_S)
{
 EmberOgre::Gui::StackableContainer* self = (EmberOgre::Gui::StackableContainer*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Gui__Icons__IconManager (lua_State* tolua_S)
{
 EmberOgre::Gui::Icons::IconManager* self = (EmberOgre::Gui::Icons::IconManager*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_std__vector_const_Carpenter__AttachPoint__ (lua_State* tolua_S)
{
 std::vector<const Carpenter::AttachPoint*>* self = (std::vector<const Carpenter::AttachPoint*>*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_std__vector_EmberOgre__ModelBlock__ (lua_State* tolua_S)
{
 std::vector<EmberOgre::ModelBlock*>* self = (std::vector<EmberOgre::ModelBlock*>*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__AttachPointNode (lua_State* tolua_S)
{
 EmberOgre::AttachPointNode* self = (EmberOgre::AttachPointNode*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Gui__QuaternionAdapter (lua_State* tolua_S)
{
 EmberOgre::Gui::QuaternionAdapter* self = (EmberOgre::Gui::QuaternionAdapter*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Gui__Vector3Adapter (lua_State* tolua_S)
{
 EmberOgre::Gui::Vector3Adapter* self = (EmberOgre::Gui::Vector3Adapter*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_std__vector_std__string_ (lua_State* tolua_S)
{
 std::vector<std::string>* self = (std::vector<std::string>*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_std__vector_Eris__Task__ (lua_State* tolua_S)
{
 std::vector<Eris::Task*>* self = (std::vector<Eris::Task*>*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Gui__EntityEditor (lua_State* tolua_S)
{
 EmberOgre::Gui::EntityEditor* self = (EmberOgre::Gui::EntityEditor*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_std__vector_EmberOgre__Model__SubModelDefinition__ (lua_State* tolua_S)
{
 std::vector<EmberOgre::Model::SubModelDefinition*>* self = (std::vector<EmberOgre::Model::SubModelDefinition*>*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__LightWibbler (lua_State* tolua_S)
{
 EmberOgre::LightWibbler* self = (EmberOgre::LightWibbler*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__OgreInfo (lua_State* tolua_S)
{
 EmberOgre::OgreInfo* self = (EmberOgre::OgreInfo*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_Ogre__MapIterator_Ogre__ResourceManager__ResourceHandleMap_ (lua_State* tolua_S)
{
 Ogre::MapIterator<Ogre::ResourceManager::ResourceHandleMap>* self = (Ogre::MapIterator<Ogre::ResourceManager::ResourceHandleMap>*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Gui__CompassCameraAnchor (lua_State* tolua_S)
{
 EmberOgre::Gui::CompassCameraAnchor* self = (EmberOgre::Gui::CompassCameraAnchor*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Gui__ModelRenderer (lua_State* tolua_S)
{
 EmberOgre::Gui::ModelRenderer* self = (EmberOgre::Gui::ModelRenderer*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Gui__OgreEntityRenderer (lua_State* tolua_S)
{
 EmberOgre::Gui::OgreEntityRenderer* self = (EmberOgre::Gui::OgreEntityRenderer*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Model__Model (lua_State* tolua_S)
{
 EmberOgre::Model::Model* self = (EmberOgre::Model::Model*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_Carpenter__BuildingBlockSpecDefinition (lua_State* tolua_S)
{
 Carpenter::BuildingBlockSpecDefinition* self = (Carpenter::BuildingBlockSpecDefinition*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Gui__ConsoleAdapter (lua_State* tolua_S)
{
 EmberOgre::Gui::ConsoleAdapter* self = (EmberOgre::Gui::ConsoleAdapter*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_WFMath__Quaternion (lua_State* tolua_S)
{
 WFMath::Quaternion* self = (WFMath::Quaternion*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_std__vector_EmberOgre__Model__SubEntityDefinition__ (lua_State* tolua_S)
{
 std::vector<EmberOgre::Model::SubEntityDefinition*>* self = (std::vector<EmberOgre::Model::SubEntityDefinition*>*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_WFMath__Point_3_ (lua_State* tolua_S)
{
 WFMath::Point<3>* self = (WFMath::Point<3>*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_WFMath__Vector_3_ (lua_State* tolua_S)
{
 WFMath::Vector<3>* self = (WFMath::Vector<3>*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_Ogre__Real (lua_State* tolua_S)
{
 Ogre::Real* self = (Ogre::Real*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_std__vector_EmberOgre__AttachPointNode__ (lua_State* tolua_S)
{
 std::vector<EmberOgre::AttachPointNode*>* self = (std::vector<EmberOgre::AttachPointNode*>*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Gui__IconBase (lua_State* tolua_S)
{
 EmberOgre::Gui::IconBase* self = (EmberOgre::Gui::IconBase*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Construction (lua_State* tolua_S)
{
 EmberOgre::Construction* self = (EmberOgre::Construction*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__TerrainPosition (lua_State* tolua_S)
{
 EmberOgre::TerrainPosition* self = (EmberOgre::TerrainPosition*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_Ogre__Quaternion (lua_State* tolua_S)
{
 Ogre::Quaternion* self = (Ogre::Quaternion*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_std__vector_EmberOgre__Model__PartDefinition__ (lua_State* tolua_S)
{
 std::vector<EmberOgre::Model::PartDefinition*>* self = (std::vector<EmberOgre::Model::PartDefinition*>*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_Ogre__TexturePtr (lua_State* tolua_S)
{
 Ogre::TexturePtr* self = (Ogre::TexturePtr*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__ModelBlock (lua_State* tolua_S)
{
 EmberOgre::ModelBlock* self = (EmberOgre::ModelBlock*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Terrain__Map (lua_State* tolua_S)
{
 EmberOgre::Terrain::Map* self = (EmberOgre::Terrain::Map*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Terrain__TerrainArea (lua_State* tolua_S)
{
 EmberOgre::Terrain::TerrainArea* self = (EmberOgre::Terrain::TerrainArea*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Gui__OverlayCompassImpl (lua_State* tolua_S)
{
 EmberOgre::Gui::OverlayCompassImpl* self = (EmberOgre::Gui::OverlayCompassImpl*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__AvatarControllerMovement (lua_State* tolua_S)
{
 EmberOgre::AvatarControllerMovement* self = (EmberOgre::AvatarControllerMovement*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_Ogre__ColourValue (lua_State* tolua_S)
{
 Ogre::ColourValue* self = (Ogre::ColourValue*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Gui__Compass (lua_State* tolua_S)
{
 EmberOgre::Gui::Compass* self = (EmberOgre::Gui::Compass*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Gui__CEGUICompassImpl (lua_State* tolua_S)
{
 EmberOgre::Gui::CEGUICompassImpl* self = (EmberOgre::Gui::CEGUICompassImpl*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Gui__CompassSceneNodeAnchor (lua_State* tolua_S)
{
 EmberOgre::Gui::CompassSceneNodeAnchor* self = (EmberOgre::Gui::CompassSceneNodeAnchor*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Gui__TexturePair (lua_State* tolua_S)
{
 EmberOgre::Gui::TexturePair* self = (EmberOgre::Gui::TexturePair*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_Ogre__Vector3 (lua_State* tolua_S)
{
 Ogre::Vector3* self = (Ogre::Vector3*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Gui__AssetsManager (lua_State* tolua_S)
{
 EmberOgre::Gui::AssetsManager* self = (EmberOgre::Gui::AssetsManager*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_Carpenter__AttachPoint (lua_State* tolua_S)
{
 Carpenter::AttachPoint* self = (Carpenter::AttachPoint*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Gui__ListHolder (lua_State* tolua_S)
{
 EmberOgre::Gui::ListHolder* self = (EmberOgre::Gui::ListHolder*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_Carpenter__Carpenter (lua_State* tolua_S)
{
 Carpenter::Carpenter* self = (Carpenter::Carpenter*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_Atlas__Message__Element (lua_State* tolua_S)
{
 Atlas::Message::Element* self = (Atlas::Message::Element*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Terrain__TerrainEditor (lua_State* tolua_S)
{
 EmberOgre::Terrain::TerrainEditor* self = (EmberOgre::Terrain::TerrainEditor*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"std::vector<EmberOgre::Model::SubModelDefinition*>");
 tolua_usertype(tolua_S,"std::set<std::string>");
 tolua_usertype(tolua_S,"EmberOgre::AttachPointNode");
 tolua_usertype(tolua_S,"std::map<const std::string,Carpenter::BlockSpec>");
 tolua_usertype(tolua_S,"Ogre::Billboard");
 tolua_usertype(tolua_S,"Eris::TypeInfo");
 tolua_usertype(tolua_S,"Ogre::SceneManager");
 tolua_usertype(tolua_S,"Mercator::BasePoint");
 tolua_usertype(tolua_S,"Ogre::Degree");
 tolua_usertype(tolua_S,"sigc::signal<void,EmberOgre::Gui::EntityIcon*>");
 tolua_usertype(tolua_S,"std::vector<EmberOgre::Model::ActionDefinition*>");
 tolua_usertype(tolua_S,"EmberOgre::Gui::EntityEditor");
 tolua_usertype(tolua_S,"EmberOgre::LightWibbler");
 tolua_usertype(tolua_S,"sigc::signal<void,EmberOgre::GUIManager&>");
 tolua_usertype(tolua_S,"Ogre::MapIterator<Ogre::ResourceManager::ResourceHandleMap>");
 tolua_usertype(tolua_S,"EmberOgre::Gui::CompassCameraAnchor");
 tolua_usertype(tolua_S,"EmberOgre::MovableObjectRenderer");
 tolua_usertype(tolua_S,"EmberOgre::Terrain::BasePointUserObject");
 tolua_usertype(tolua_S,"EmberOgre::Gui::Adapters::Atlas::MapAdapter");
 tolua_usertype(tolua_S,"EmberOgre::Gui::ICompassImpl");
 tolua_usertype(tolua_S,"EmberOgre::Model::Action");
 tolua_usertype(tolua_S,"sigc::signal<void,EmberOgre::EmberEntity*>");
 tolua_usertype(tolua_S,"sigc::signal<void,const EmberOgre::Terrain::TerrainEditAction*>");
 tolua_usertype(tolua_S,"Ogre::TRect<float>");
 tolua_usertype(tolua_S,"EmberOgre::Gui::EntityIconSlot");
 tolua_usertype(tolua_S,"EmberOgre::Gui::CompassSceneNodeAnchor");
 tolua_usertype(tolua_S,"EmberOgre::Gui::TexturePair");
 tolua_usertype(tolua_S,"EmberOgre::ModelBlock");
 tolua_usertype(tolua_S,"sigc::signal<void,EmberOgre::AvatarMovementMode::Mode>");
 tolua_usertype(tolua_S,"Atlas::Message::Element");
 tolua_usertype(tolua_S,"std::vector<EmberOgre::Terrain::TerrainEditBasePointMovement>");
 tolua_usertype(tolua_S,"WFMath::Point<3>");
 tolua_usertype(tolua_S,"WFMath::AxisBox<3>");
 tolua_usertype(tolua_S,"EmberOgre::Gui::IconBar");
 tolua_usertype(tolua_S,"sigc::signal<void,EmberOgre::Terrain::BasePointUserObject*>");
 tolua_usertype(tolua_S,"Ogre::AxisAlignedBox");
 tolua_usertype(tolua_S,"EmberOgre::Gui::EntityIconManager");
 tolua_usertype(tolua_S,"EmberOgre::Gui::Icons::IconManager");
 tolua_usertype(tolua_S,"std::vector<const Carpenter::AttachPoint*>");
 tolua_usertype(tolua_S,"std::vector<EmberOgre::ModelBlock*>");
 tolua_usertype(tolua_S,"sigc::signal<void,float>");
 tolua_usertype(tolua_S,"sigc::signal<void,EmberOgre::AvatarEmberEntity*>");
 tolua_usertype(tolua_S,"EmberOgre::Environment::ISun");
 tolua_usertype(tolua_S,"SDLKey");
 tolua_usertype(tolua_S,"Carpenter::BuildingBlock");
 tolua_usertype(tolua_S,"Carpenter::BuildingBlockBindingDefinition");
 tolua_usertype(tolua_S,"EmberOgre::Model::ModelDefinition");
 tolua_usertype(tolua_S,"std::map<const std::string,Carpenter::BuildingBlockSpec>");
 tolua_usertype(tolua_S,"EmberOgre::Environment::IFog");
 tolua_usertype(tolua_S,"EmberOgre::Model::AttachPointDefinition");
 tolua_usertype(tolua_S,"EmberOgre::EmberPhysicalEntity");
 tolua_usertype(tolua_S,"sigc::signal<void,const std::string&,EmberOgre::EmberEntity*>");
 tolua_usertype(tolua_S,"EmberOgre::Avatar");
 tolua_usertype(tolua_S,"CEGUI::Window");
 tolua_usertype(tolua_S,"EmberOgre::AvatarMovementMode");
 tolua_usertype(tolua_S,"Eris::Entity");
 tolua_usertype(tolua_S,"EmberOgre::Gui::Compass");
 tolua_usertype(tolua_S,"sigc::signal<void,const std::string&>");
 tolua_usertype(tolua_S,"EmberOgre::EmberEntityFactory");
 tolua_usertype(tolua_S,"Ogre::Entity");
 tolua_usertype(tolua_S,"Ogre::Vector3");
 tolua_usertype(tolua_S,"EmberOgre::Gui::EntityIconDragDropTarget");
 tolua_usertype(tolua_S,"Carpenter::Carpenter");
 tolua_usertype(tolua_S,"sigc::signal<void,const Ogre::ColourValue&>");
 tolua_usertype(tolua_S,"CEGUI::Image");
 tolua_usertype(tolua_S,"Carpenter::BlockSpec");
 tolua_usertype(tolua_S,"WFMath::Vector<3>");
 tolua_usertype(tolua_S,"EmberOgre::Gui::Icons::Icon");
 tolua_usertype(tolua_S,"Ogre::Radian");
 tolua_usertype(tolua_S,"CEGUI::Editbox");
 tolua_usertype(tolua_S,"const");
 tolua_usertype(tolua_S,"EmberOgre::Model::PartDefinition");
 tolua_usertype(tolua_S,"sigc::signal<void,EmberOgre::Input::MouseButton,EmberOgre::Input::InputMode>");
 tolua_usertype(tolua_S,"std::vector<std::string>");
 tolua_usertype(tolua_S,"sigc::signal<void,EmberOgre::Terrain::TerrainGenerator&>");
 tolua_usertype(tolua_S,"sigc::signal<void,EmberOgre::MotionManager&>");
 tolua_usertype(tolua_S,"EmberOgre::Construction");
 tolua_usertype(tolua_S,"EmberOgre::AvatarCamera");
 tolua_usertype(tolua_S,"sigc::signal<void,EmberOgre::EmberEntityFactory*>");
 tolua_usertype(tolua_S,"Ogre::RenderWindow");
 tolua_usertype(tolua_S,"CEGUI::ListboxItem");
 tolua_usertype(tolua_S,"CEGUI::Imageset");
 tolua_usertype(tolua_S,"EmberOgre::AvatarController");
 tolua_usertype(tolua_S,"EmberOgre::Model::Model");
 tolua_usertype(tolua_S,"EmberOgre::MousePickerArgs");
 tolua_usertype(tolua_S,"std::map<const std::string,Carpenter::BluePrint>");
 tolua_usertype(tolua_S,"Ogre::AnimationState");
 tolua_usertype(tolua_S,"std::vector<EmberOgre::Model::SubEntityDefinition*>");
 tolua_usertype(tolua_S,"EmberOgre::Terrain::MapView");
 tolua_usertype(tolua_S,"sigc::signal<void,Ogre::Camera*>");
 tolua_usertype(tolua_S,"std::vector<EmberOgre::Model::AttachPointDefinition>");
 tolua_usertype(tolua_S,"EmberOgre::Environment::ISky");
 tolua_usertype(tolua_S,"EmberOgre::Gui::IconBase");
 tolua_usertype(tolua_S,"EmberOgre::Model::ModelDefnPtr");
 tolua_usertype(tolua_S,"sigc::signal<void,const Atlas::Message::Element&>");
 tolua_usertype(tolua_S,"EmberOgre::EntityWorldPickListener");
 tolua_usertype(tolua_S,"EmberOgre::OgreInfo");
 tolua_usertype(tolua_S,"EmberOgre::Terrain::Map");
 tolua_usertype(tolua_S,"std::map<std::string,EmberOgre::Model::ViewDefinition*>");
 tolua_usertype(tolua_S,"sigc::signal<void,const EmberOgre::MouseMotion&,EmberOgre::Input::InputMode>");
 tolua_usertype(tolua_S,"EmberOgre::Gui::QuaternionAdapter");
 tolua_usertype(tolua_S,"EmberOgre::Gui::Widget");
 tolua_usertype(tolua_S,"CEGUI::DragContainer");
 tolua_usertype(tolua_S,"EmberOgre::Model::SubEntityDefinition");
 tolua_usertype(tolua_S,"std::vector<Eris::Task*>");
 tolua_usertype(tolua_S,"std::vector<EmberOgre::Model::PartDefinition*>");
 tolua_usertype(tolua_S,"EmberOgre::Terrain::TerrainArea");
 tolua_usertype(tolua_S,"Ogre;;ResourceManager");
 tolua_usertype(tolua_S,"EmberOgre::Model::ModelDefinitionManager");
 tolua_usertype(tolua_S,"sigc::signal<void,const EmberOgre::EntityPickResult&,const EmberOgre::MousePickerArgs&>");
 tolua_usertype(tolua_S,"EmberOgre::AttributeObserver");
 tolua_usertype(tolua_S,"std::map<std::string,Carpenter::BluePrint*>");
 tolua_usertype(tolua_S,"EmberOgre::GUIManager");
 tolua_usertype(tolua_S,"EmberOgre::Gui::ListHolder");
 tolua_usertype(tolua_S,"Ogre::Camera");
 tolua_usertype(tolua_S,"EmberOgre::Gui::OverlayCompassImpl");
 tolua_usertype(tolua_S,"EmberOgre::EmberEntity");
 tolua_usertype(tolua_S,"EmberOgre::Gui::ModelRenderer");
 tolua_usertype(tolua_S,"EmberOgre::Gui::OgreEntityRenderer");
 tolua_usertype(tolua_S,"EmberOgre::Gui::MovableObjectRenderer");
 tolua_usertype(tolua_S,"WFMath::Quaternion");
 tolua_usertype(tolua_S,"EmberOgre::ModelMapping");
 tolua_usertype(tolua_S,"Ogre::Vector2");
 tolua_usertype(tolua_S,"EmberOgre::TerrainPosition");
 tolua_usertype(tolua_S,"sigc::signal<void,EmberOgre::Jesus*>");
 tolua_usertype(tolua_S,"std::vector<EmberOgre::AttachPointNode*>");
 tolua_usertype(tolua_S,"Ogre::BillboardSet");
 tolua_usertype(tolua_S,"Carpenter::BuildingBlockSpec");
 tolua_usertype(tolua_S,"Ogre::ControllerValue<Ogre::Real>");
 tolua_usertype(tolua_S,"EmberOgre::EntityPickResult");
 tolua_usertype(tolua_S,"CEGUI::PushButton");
 tolua_usertype(tolua_S,"EmberOgre::Jesus");
 tolua_usertype(tolua_S,"EmberOgre::Gui::Vector3Adapter");
 tolua_usertype(tolua_S,"EmberOgre::Gui::ConsoleAdapter");
 tolua_usertype(tolua_S,"sigc::signal<void,EmberOgre::Input::InputMode>");
 tolua_usertype(tolua_S,"EmberOgre::AvatarEmberEntity");
 tolua_usertype(tolua_S,"Carpenter::BluePrint");
 tolua_usertype(tolua_S,"sigc::signal<void,EmberOgre::AvatarController&>");
 tolua_usertype(tolua_S,"Carpenter::AttachPair");
 tolua_usertype(tolua_S,"CEGUI::Listbox");
 tolua_usertype(tolua_S,"sigc::signal<void,const SDL_keysym&,EmberOgre::Input::InputMode>");
 tolua_usertype(tolua_S,"EmberOgre::Input");
 tolua_usertype(tolua_S,"Carpenter::BuildingBlockDefinition");
 tolua_usertype(tolua_S,"CEGUI::ListboxTextItem");
 tolua_usertype(tolua_S,"EmberOgre::Environment::Foliage");
 tolua_usertype(tolua_S,"EmberOgre::MouseMotion");
 tolua_usertype(tolua_S,"EmberOgre::EmberOgre");
 tolua_usertype(tolua_S,"EmberOgre::Model::ActionDefinition");
 tolua_usertype(tolua_S,"Carpenter::BuildingBlockSpecDefinition");
 tolua_usertype(tolua_S,"Ogre::Resource");
 tolua_usertype(tolua_S,"EmberOgre::Model::ViewDefinition");
 tolua_usertype(tolua_S,"EmberOgre::Terrain::TerrainEditor");
 tolua_usertype(tolua_S,"EmberOgre::Model::SubModelDefinition");
 tolua_usertype(tolua_S,"Mercator::Area");
 tolua_usertype(tolua_S,"Ogre::Real");
 tolua_usertype(tolua_S,"EmberOgre::MotionManager");
 tolua_usertype(tolua_S,"EmberOgre::Gui::EntityIcon");
 tolua_usertype(tolua_S,"EmberOgre::Model::SubModel");
 tolua_usertype(tolua_S,"EmberOgre::MotionManager::MotionManagerInfo");
 tolua_usertype(tolua_S,"Ogre::Quaternion");
 tolua_usertype(tolua_S,"EmberOgre::EntityMoveManager");
 tolua_usertype(tolua_S,"EmberOgre::AvatarControllerMovement");
 tolua_usertype(tolua_S,"Carpenter::BuildingBlockBinding");
 tolua_usertype(tolua_S,"Eris::Task");
 tolua_usertype(tolua_S,"EmberOgre::Environment::IWater");
 tolua_usertype(tolua_S,"EmberOgre::Gui::AssetsManager");
 tolua_usertype(tolua_S,"EmberOgre::AvatarMovementState");
 tolua_usertype(tolua_S,"Ogre::ColourValue");
 tolua_usertype(tolua_S,"Ogre::TexturePtr");
 tolua_usertype(tolua_S,"EmberOgre::Environment::Environment");
 tolua_usertype(tolua_S,"EmberOgre::Terrain::TerrainEditAction");
 tolua_usertype(tolua_S,"EmberOgre::Gui::StackableContainer");
 tolua_usertype(tolua_S,"EmberOgre::WorldEmberEntity");
 tolua_usertype(tolua_S,"EmberOgre::Terrain::TerrainEditBasePointMovement");
 tolua_usertype(tolua_S,"Carpenter::AttachPoint");
 tolua_usertype(tolua_S,"sigc::signal<void>");
 tolua_usertype(tolua_S,"EmberOgre::Gui::ColouredListItem");
 tolua_usertype(tolua_S,"Ogre::SceneNode");
 tolua_usertype(tolua_S,"EmberOgre::Gui::CEGUICompassImpl");
}

/* method: getAttachPoint of class  Carpenter::AttachPair */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_Carpenter_AttachPair_getAttachPoint00
static int tolua_EmberOgre_Carpenter_AttachPair_getAttachPoint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Carpenter::AttachPair",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Carpenter::AttachPair* self = (const Carpenter::AttachPair*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAttachPoint'",NULL);
#endif
  {
   const Carpenter::AttachPoint* tolua_ret = (const Carpenter::AttachPoint*)  self->getAttachPoint(name);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"const Carpenter::AttachPoint");
   tolua_pushcppstring(tolua_S,(const char*)name);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAttachPoint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Carpenter::AttachPair */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_Carpenter_AttachPair_new00
static int tolua_EmberOgre_Carpenter_AttachPair_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Carpenter::AttachPair",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"Carpenter::AttachPoint",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"Carpenter::AttachPoint",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string type = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  Carpenter::AttachPoint point1 = *((Carpenter::AttachPoint*)  tolua_tousertype(tolua_S,4,0));
  Carpenter::AttachPoint point2 = *((Carpenter::AttachPoint*)  tolua_tousertype(tolua_S,5,0));
  {
   Carpenter::AttachPair* tolua_ret = (Carpenter::AttachPair*)  new Carpenter::AttachPair(name,type,point1,point2);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Carpenter::AttachPair");
   tolua_pushcppstring(tolua_S,(const char*)name);
   tolua_pushcppstring(tolua_S,(const char*)type);
  }
 }
 return 3;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Carpenter::AttachPair */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_Carpenter_AttachPair_new00_local
static int tolua_EmberOgre_Carpenter_AttachPair_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Carpenter::AttachPair",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"Carpenter::AttachPoint",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"Carpenter::AttachPoint",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string type = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  Carpenter::AttachPoint point1 = *((Carpenter::AttachPoint*)  tolua_tousertype(tolua_S,4,0));
  Carpenter::AttachPoint point2 = *((Carpenter::AttachPoint*)  tolua_tousertype(tolua_S,5,0));
  {
   Carpenter::AttachPair* tolua_ret = (Carpenter::AttachPair*)  new Carpenter::AttachPair(name,type,point1,point2);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"Carpenter::AttachPair");
   tolua_pushcppstring(tolua_S,(const char*)name);
   tolua_pushcppstring(tolua_S,(const char*)type);
  }
 }
 return 3;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPoint1 of class  Carpenter::AttachPair */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_Carpenter_AttachPair_getPoint100
static int tolua_EmberOgre_Carpenter_AttachPair_getPoint100(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Carpenter::AttachPair",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Carpenter::AttachPair* self = (const Carpenter::AttachPair*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPoint1'",NULL);
#endif
  {
   const Carpenter::AttachPoint& tolua_ret = (const Carpenter::AttachPoint&)  self->getPoint1();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Carpenter::AttachPoint");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPoint1'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPoint2 of class  Carpenter::AttachPair */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_Carpenter_AttachPair_getPoint200
static int tolua_EmberOgre_Carpenter_AttachPair_getPoint200(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Carpenter::AttachPair",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Carpenter::AttachPair* self = (const Carpenter::AttachPair*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPoint2'",NULL);
#endif
  {
   const Carpenter::AttachPoint& tolua_ret = (const Carpenter::AttachPoint&)  self->getPoint2();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Carpenter::AttachPoint");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPoint2'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getName of class  Carpenter::AttachPair */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_Carpenter_AttachPair_getName00
static int tolua_EmberOgre_Carpenter_AttachPair_getName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Carpenter::AttachPair",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Carpenter::AttachPair* self = (const Carpenter::AttachPair*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getName'",NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getName();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getType of class  Carpenter::AttachPair */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_Carpenter_AttachPair_getType00
static int tolua_EmberOgre_Carpenter_AttachPair_getType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Carpenter::AttachPair",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Carpenter::AttachPair* self = (const Carpenter::AttachPair*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getType'",NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getType();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Carpenter::AttachPoint */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_Carpenter_AttachPoint_new00
static int tolua_EmberOgre_Carpenter_AttachPoint_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Carpenter::AttachPoint",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"WFMath::Point<3>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"WFMath::Vector<3>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::string mName = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  WFMath::Point<3> position = *((WFMath::Point<3>*)  tolua_tousertype(tolua_S,3,0));
  WFMath::Vector<3> normal = *((WFMath::Vector<3>*)  tolua_tousertype(tolua_S,4,0));
  {
   Carpenter::AttachPoint* tolua_ret = (Carpenter::AttachPoint*)  new Carpenter::AttachPoint(mName,position,normal);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Carpenter::AttachPoint");
   tolua_pushcppstring(tolua_S,(const char*)mName);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Carpenter::AttachPoint */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_Carpenter_AttachPoint_new00_local
static int tolua_EmberOgre_Carpenter_AttachPoint_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Carpenter::AttachPoint",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"WFMath::Point<3>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"WFMath::Vector<3>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::string mName = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  WFMath::Point<3> position = *((WFMath::Point<3>*)  tolua_tousertype(tolua_S,3,0));
  WFMath::Vector<3> normal = *((WFMath::Vector<3>*)  tolua_tousertype(tolua_S,4,0));
  {
   Carpenter::AttachPoint* tolua_ret = (Carpenter::AttachPoint*)  new Carpenter::AttachPoint(mName,position,normal);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"Carpenter::AttachPoint");
   tolua_pushcppstring(tolua_S,(const char*)mName);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getName of class  Carpenter::AttachPoint */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_Carpenter_AttachPoint_getName00
static int tolua_EmberOgre_Carpenter_AttachPoint_getName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Carpenter::AttachPoint",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Carpenter::AttachPoint* self = (const Carpenter::AttachPoint*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getName'",NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getName();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getNormal of class  Carpenter::AttachPoint */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_Carpenter_AttachPoint_getNormal00
static int tolua_EmberOgre_Carpenter_AttachPoint_getNormal00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Carpenter::AttachPoint",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Carpenter::AttachPoint* self = (const Carpenter::AttachPoint*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getNormal'",NULL);
#endif
  {
   const WFMath::Vector<3>& tolua_ret = (const WFMath::Vector<3>&)  self->getNormal();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const WFMath::Vector<3>");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getNormal'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPosition of class  Carpenter::AttachPoint */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_Carpenter_AttachPoint_getPosition00
static int tolua_EmberOgre_Carpenter_AttachPoint_getPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Carpenter::AttachPoint",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Carpenter::AttachPoint* self = (const Carpenter::AttachPoint*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPosition'",NULL);
#endif
  {
   const WFMath::Point<3>& tolua_ret = (const WFMath::Point<3>&)  self->getPosition();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const WFMath::Point<3>");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAttachPair of class  Carpenter::AttachPoint */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_Carpenter_AttachPoint_getAttachPair00
static int tolua_EmberOgre_Carpenter_AttachPoint_getAttachPair00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Carpenter::AttachPoint",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Carpenter::AttachPoint* self = (const Carpenter::AttachPoint*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAttachPair'",NULL);
#endif
  {
   const Carpenter::AttachPair* tolua_ret = (const Carpenter::AttachPair*)  self->getAttachPair();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"const Carpenter::AttachPair");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAttachPair'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSibling of class  Carpenter::AttachPoint */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_Carpenter_AttachPoint_getSibling00
static int tolua_EmberOgre_Carpenter_AttachPoint_getSibling00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Carpenter::AttachPoint",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Carpenter::AttachPoint* self = (const Carpenter::AttachPoint*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSibling'",NULL);
#endif
  {
   const Carpenter::AttachPoint* tolua_ret = (const Carpenter::AttachPoint*)  self->getSibling();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"const Carpenter::AttachPoint");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSibling'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getName of class  Carpenter::BlockSpec */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_Carpenter_BlockSpec_getName00
static int tolua_EmberOgre_Carpenter_BlockSpec_getName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Carpenter::BlockSpec",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Carpenter::BlockSpec* self = (const Carpenter::BlockSpec*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getName'",NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getName();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getBoundingBox of class  Carpenter::BlockSpec */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_Carpenter_BlockSpec_getBoundingBox00
static int tolua_EmberOgre_Carpenter_BlockSpec_getBoundingBox00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Carpenter::BlockSpec",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Carpenter::BlockSpec* self = (const Carpenter::BlockSpec*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getBoundingBox'",NULL);
#endif
  {
   const WFMath::AxisBox<3>& tolua_ret = (const WFMath::AxisBox<3>&)  self->getBoundingBox();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const WFMath::AxisBox<3>");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getBoundingBox'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAttachPair of class  Carpenter::BlockSpec */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_Carpenter_BlockSpec_getAttachPair00
static int tolua_EmberOgre_Carpenter_BlockSpec_getAttachPair00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Carpenter::BlockSpec",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Carpenter::BlockSpec* self = (const Carpenter::BlockSpec*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAttachPair'",NULL);
#endif
  {
   const Carpenter::AttachPair* tolua_ret = (const Carpenter::AttachPair*)  self->getAttachPair(name);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"const Carpenter::AttachPair");
   tolua_pushcppstring(tolua_S,(const char*)name);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAttachPair'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addAttachPair of class  Carpenter::BlockSpec */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_Carpenter_BlockSpec_addAttachPair00
static int tolua_EmberOgre_Carpenter_BlockSpec_addAttachPair00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Carpenter::BlockSpec",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Carpenter::AttachPair",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Carpenter::BlockSpec* self = (Carpenter::BlockSpec*)  tolua_tousertype(tolua_S,1,0);
  Carpenter::AttachPair* pair = ((Carpenter::AttachPair*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addAttachPair'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->addAttachPair(pair);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addAttachPair'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setBoundingBox of class  Carpenter::BlockSpec */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_Carpenter_BlockSpec_setBoundingBox00
static int tolua_EmberOgre_Carpenter_BlockSpec_setBoundingBox00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Carpenter::BlockSpec",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"WFMath::AxisBox<3>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Carpenter::BlockSpec* self = (Carpenter::BlockSpec*)  tolua_tousertype(tolua_S,1,0);
  WFMath::AxisBox<3> bbox = *((WFMath::AxisBox<3>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setBoundingBox'",NULL);
#endif
  {
   self->setBoundingBox(bbox);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setBoundingBox'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAllPoints of class  Carpenter::BlockSpec */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_Carpenter_BlockSpec_getAllPoints00
static int tolua_EmberOgre_Carpenter_BlockSpec_getAllPoints00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Carpenter::BlockSpec",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Carpenter::BlockSpec* self = (const Carpenter::BlockSpec*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAllPoints'",NULL);
#endif
  {
   const std::vector<const Carpenter::AttachPoint*> tolua_ret = (const std::vector<const Carpenter::AttachPoint*>)  self->getAllPoints();
   {
#ifdef __cplusplus
    void* tolua_obj = new std::vector<const Carpenter::AttachPoint*>(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"const std::vector<const Carpenter::AttachPoint*>");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(const std::vector<const Carpenter::AttachPoint*>));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"const std::vector<const Carpenter::AttachPoint*>");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAllPoints'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Carpenter::BuildingBlockSpec */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_Carpenter_BuildingBlockSpec_new00
static int tolua_EmberOgre_Carpenter_BuildingBlockSpec_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Carpenter::BuildingBlockSpec",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Carpenter::BuildingBlockSpec* tolua_ret = (Carpenter::BuildingBlockSpec*)  new Carpenter::BuildingBlockSpec();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Carpenter::BuildingBlockSpec");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Carpenter::BuildingBlockSpec */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_Carpenter_BuildingBlockSpec_new00_local
static int tolua_EmberOgre_Carpenter_BuildingBlockSpec_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Carpenter::BuildingBlockSpec",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Carpenter::BuildingBlockSpec* tolua_ret = (Carpenter::BuildingBlockSpec*)  new Carpenter::BuildingBlockSpec();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"Carpenter::BuildingBlockSpec");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getDefinition of class  Carpenter::BuildingBlockSpec */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_Carpenter_BuildingBlockSpec_getDefinition00
static int tolua_EmberOgre_Carpenter_BuildingBlockSpec_getDefinition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Carpenter::BuildingBlockSpec",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Carpenter::BuildingBlockSpec* self = (const Carpenter::BuildingBlockSpec*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getDefinition'",NULL);
#endif
  {
   const Carpenter::BuildingBlockSpecDefinition& tolua_ret = (const Carpenter::BuildingBlockSpecDefinition&)  self->getDefinition();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Carpenter::BuildingBlockSpecDefinition");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getDefinition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getBlockSpec of class  Carpenter::BuildingBlockSpec */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_Carpenter_BuildingBlockSpec_getBlockSpec00
static int tolua_EmberOgre_Carpenter_BuildingBlockSpec_getBlockSpec00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Carpenter::BuildingBlockSpec",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Carpenter::BuildingBlockSpec* self = (const Carpenter::BuildingBlockSpec*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getBlockSpec'",NULL);
#endif
  {
   const Carpenter::BlockSpec* tolua_ret = (const Carpenter::BlockSpec*)  self->getBlockSpec();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"const Carpenter::BlockSpec");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getBlockSpec'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getName of class  Carpenter::BuildingBlockSpec */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_Carpenter_BuildingBlockSpec_getName00
static int tolua_EmberOgre_Carpenter_BuildingBlockSpec_getName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Carpenter::BuildingBlockSpec",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Carpenter::BuildingBlockSpec* self = (const Carpenter::BuildingBlockSpec*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getName'",NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getName();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Carpenter::BuildingBlockSpecDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_Carpenter_BuildingBlockSpecDefinition_new00
static int tolua_EmberOgre_Carpenter_BuildingBlockSpecDefinition_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Carpenter::BuildingBlockSpecDefinition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Carpenter::BuildingBlockSpecDefinition* tolua_ret = (Carpenter::BuildingBlockSpecDefinition*)  new Carpenter::BuildingBlockSpecDefinition();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Carpenter::BuildingBlockSpecDefinition");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Carpenter::BuildingBlockSpecDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_Carpenter_BuildingBlockSpecDefinition_new00_local
static int tolua_EmberOgre_Carpenter_BuildingBlockSpecDefinition_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Carpenter::BuildingBlockSpecDefinition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Carpenter::BuildingBlockSpecDefinition* tolua_ret = (Carpenter::BuildingBlockSpecDefinition*)  new Carpenter::BuildingBlockSpecDefinition();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"Carpenter::BuildingBlockSpecDefinition");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: mName of class  Carpenter::BuildingBlockSpecDefinition */
#ifndef TOLUA_DISABLE_tolua_get_Carpenter__BuildingBlockSpecDefinition_mName
static int tolua_get_Carpenter__BuildingBlockSpecDefinition_mName(lua_State* tolua_S)
{
  Carpenter::BuildingBlockSpecDefinition* self = (Carpenter::BuildingBlockSpecDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mName'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->mName);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: mName of class  Carpenter::BuildingBlockSpecDefinition */
#ifndef TOLUA_DISABLE_tolua_set_Carpenter__BuildingBlockSpecDefinition_mName
static int tolua_set_Carpenter__BuildingBlockSpecDefinition_mName(lua_State* tolua_S)
{
  Carpenter::BuildingBlockSpecDefinition* self = (Carpenter::BuildingBlockSpecDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mName'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->mName = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: mBlockSpecName of class  Carpenter::BuildingBlockSpecDefinition */
#ifndef TOLUA_DISABLE_tolua_get_Carpenter__BuildingBlockSpecDefinition_mBlockSpecName
static int tolua_get_Carpenter__BuildingBlockSpecDefinition_mBlockSpecName(lua_State* tolua_S)
{
  Carpenter::BuildingBlockSpecDefinition* self = (Carpenter::BuildingBlockSpecDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mBlockSpecName'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->mBlockSpecName);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: mBlockSpecName of class  Carpenter::BuildingBlockSpecDefinition */
#ifndef TOLUA_DISABLE_tolua_set_Carpenter__BuildingBlockSpecDefinition_mBlockSpecName
static int tolua_set_Carpenter__BuildingBlockSpecDefinition_mBlockSpecName(lua_State* tolua_S)
{
  Carpenter::BuildingBlockSpecDefinition* self = (Carpenter::BuildingBlockSpecDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mBlockSpecName'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->mBlockSpecName = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Carpenter::Carpenter */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_Carpenter_Carpenter_new00
static int tolua_EmberOgre_Carpenter_Carpenter_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Carpenter::Carpenter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Carpenter::Carpenter* tolua_ret = (Carpenter::Carpenter*)  new Carpenter::Carpenter();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Carpenter::Carpenter");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Carpenter::Carpenter */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_Carpenter_Carpenter_new00_local
static int tolua_EmberOgre_Carpenter_Carpenter_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Carpenter::Carpenter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Carpenter::Carpenter* tolua_ret = (Carpenter::Carpenter*)  new Carpenter::Carpenter();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"Carpenter::Carpenter");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  Carpenter::Carpenter */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_Carpenter_Carpenter_delete00
static int tolua_EmberOgre_Carpenter_Carpenter_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Carpenter::Carpenter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Carpenter::Carpenter* self = (Carpenter::Carpenter*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createBlueprint of class  Carpenter::Carpenter */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_Carpenter_Carpenter_createBlueprint00
static int tolua_EmberOgre_Carpenter_Carpenter_createBlueprint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Carpenter::Carpenter",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Carpenter::Carpenter* self = (Carpenter::Carpenter*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createBlueprint'",NULL);
#endif
  {
   Carpenter::BluePrint* tolua_ret = (Carpenter::BluePrint*)  self->createBlueprint(name);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Carpenter::BluePrint");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createBlueprint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getBuildingBlockSpec of class  Carpenter::Carpenter */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_Carpenter_Carpenter_getBuildingBlockSpec00
static int tolua_EmberOgre_Carpenter_Carpenter_getBuildingBlockSpec00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Carpenter::Carpenter",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Carpenter::Carpenter* self = (Carpenter::Carpenter*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getBuildingBlockSpec'",NULL);
#endif
  {
   Carpenter::BuildingBlockSpec* tolua_ret = (Carpenter::BuildingBlockSpec*)  self->getBuildingBlockSpec(name);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Carpenter::BuildingBlockSpec");
   tolua_pushcppstring(tolua_S,(const char*)name);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getBuildingBlockSpec'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createBlockSpec of class  Carpenter::Carpenter */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_Carpenter_Carpenter_createBlockSpec00
static int tolua_EmberOgre_Carpenter_Carpenter_createBlockSpec00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Carpenter::Carpenter",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Carpenter::Carpenter* self = (Carpenter::Carpenter*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createBlockSpec'",NULL);
#endif
  {
   Carpenter::BlockSpec* tolua_ret = (Carpenter::BlockSpec*)  self->createBlockSpec(name);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Carpenter::BlockSpec");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createBlockSpec'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createBuildingBlockSpec of class  Carpenter::Carpenter */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_Carpenter_Carpenter_createBuildingBlockSpec00
static int tolua_EmberOgre_Carpenter_Carpenter_createBuildingBlockSpec00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Carpenter::Carpenter",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Carpenter::BuildingBlockSpecDefinition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Carpenter::Carpenter* self = (Carpenter::Carpenter*)  tolua_tousertype(tolua_S,1,0);
  Carpenter::BuildingBlockSpecDefinition definition = *((Carpenter::BuildingBlockSpecDefinition*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createBuildingBlockSpec'",NULL);
#endif
  {
   Carpenter::BuildingBlockSpec* tolua_ret = (Carpenter::BuildingBlockSpec*)  self->createBuildingBlockSpec(definition);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Carpenter::BuildingBlockSpec");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createBuildingBlockSpec'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getBlockSpecs of class  Carpenter::Carpenter */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_Carpenter_Carpenter_getBlockSpecs00
static int tolua_EmberOgre_Carpenter_Carpenter_getBlockSpecs00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Carpenter::Carpenter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Carpenter::Carpenter* self = (const Carpenter::Carpenter*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getBlockSpecs'",NULL);
#endif
  {
   const std::map<const std::string,Carpenter::BlockSpec>* tolua_ret = (const std::map<const std::string,Carpenter::BlockSpec>*)  self->getBlockSpecs();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"const std::map<const std::string,Carpenter::BlockSpec>");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getBlockSpecs'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getBuildingBlockSpecs of class  Carpenter::Carpenter */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_Carpenter_Carpenter_getBuildingBlockSpecs00
static int tolua_EmberOgre_Carpenter_Carpenter_getBuildingBlockSpecs00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Carpenter::Carpenter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Carpenter::Carpenter* self = (const Carpenter::Carpenter*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getBuildingBlockSpecs'",NULL);
#endif
  {
   const std::map<const std::string,Carpenter::BuildingBlockSpec>* tolua_ret = (const std::map<const std::string,Carpenter::BuildingBlockSpec>*)  self->getBuildingBlockSpecs();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"const std::map<const std::string,Carpenter::BuildingBlockSpec>");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getBuildingBlockSpecs'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getBluePrints of class  Carpenter::Carpenter */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_Carpenter_Carpenter_getBluePrints00
static int tolua_EmberOgre_Carpenter_Carpenter_getBluePrints00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Carpenter::Carpenter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Carpenter::Carpenter* self = (const Carpenter::Carpenter*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getBluePrints'",NULL);
#endif
  {
   const std::map<const std::string,Carpenter::BluePrint>* tolua_ret = (const std::map<const std::string,Carpenter::BluePrint>*)  self->getBluePrints();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"const std::map<const std::string,Carpenter::BluePrint>");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getBluePrints'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: pitch of class  EmberOgre::AvatarCamera */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_AvatarCamera_pitch00
static int tolua_EmberOgre_EmberOgre_AvatarCamera_pitch00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::AvatarCamera",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::Degree",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::AvatarCamera* self = (EmberOgre::AvatarCamera*)  tolua_tousertype(tolua_S,1,0);
  Ogre::Degree degrees = *((Ogre::Degree*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'pitch'",NULL);
#endif
  {
   self->pitch(degrees);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'pitch'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: yaw of class  EmberOgre::AvatarCamera */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_AvatarCamera_yaw00
static int tolua_EmberOgre_EmberOgre_AvatarCamera_yaw00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::AvatarCamera",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::Degree",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::AvatarCamera* self = (EmberOgre::AvatarCamera*)  tolua_tousertype(tolua_S,1,0);
  Ogre::Degree degrees = *((Ogre::Degree*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'yaw'",NULL);
#endif
  {
   self->yaw(degrees);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'yaw'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPitch of class  EmberOgre::AvatarCamera */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_AvatarCamera_getPitch00
static int tolua_EmberOgre_EmberOgre_AvatarCamera_getPitch00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::AvatarCamera",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::AvatarCamera* self = (const EmberOgre::AvatarCamera*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPitch'",NULL);
#endif
  {
   const Ogre::Degree& tolua_ret = (const Ogre::Degree&)  self->getPitch();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Ogre::Degree");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPitch'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getYaw of class  EmberOgre::AvatarCamera */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_AvatarCamera_getYaw00
static int tolua_EmberOgre_EmberOgre_AvatarCamera_getYaw00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::AvatarCamera",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::AvatarCamera* self = (const EmberOgre::AvatarCamera*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getYaw'",NULL);
#endif
  {
   const Ogre::Degree& tolua_ret = (const Ogre::Degree&)  self->getYaw();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Ogre::Degree");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getYaw'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getCamera of class  EmberOgre::AvatarCamera */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_AvatarCamera_getCamera00
static int tolua_EmberOgre_EmberOgre_AvatarCamera_getCamera00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::AvatarCamera",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::AvatarCamera* self = (EmberOgre::AvatarCamera*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCamera'",NULL);
#endif
  {
   Ogre::Camera* tolua_ret = (Ogre::Camera*)  self->getCamera();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::Camera");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getCamera'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getCamera of class  EmberOgre::AvatarCamera */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_AvatarCamera_getCamera01
static int tolua_EmberOgre_EmberOgre_AvatarCamera_getCamera01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::AvatarCamera",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const EmberOgre::AvatarCamera* self = (const EmberOgre::AvatarCamera*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCamera'",NULL);
#endif
  {
   Ogre::Camera* tolua_ret = (Ogre::Camera*)  self->getCamera();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::Camera");
  }
 }
 return 1;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_AvatarCamera_getCamera00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getOrientation of class  EmberOgre::AvatarCamera */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_AvatarCamera_getOrientation00
static int tolua_EmberOgre_EmberOgre_AvatarCamera_getOrientation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::AvatarCamera",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::AvatarCamera* self = (const EmberOgre::AvatarCamera*)  tolua_tousertype(tolua_S,1,0);
  bool onlyHorizontal = ((bool)  tolua_toboolean(tolua_S,2,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getOrientation'",NULL);
#endif
  {
   const Ogre::Quaternion& tolua_ret = (const Ogre::Quaternion&)  self->getOrientation(onlyHorizontal);
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Ogre::Quaternion");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getOrientation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPosition of class  EmberOgre::AvatarCamera */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_AvatarCamera_getPosition00
static int tolua_EmberOgre_EmberOgre_AvatarCamera_getPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::AvatarCamera",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::AvatarCamera* self = (const EmberOgre::AvatarCamera*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPosition'",NULL);
#endif
  {
   const Ogre::Vector3& tolua_ret = (const Ogre::Vector3&)  self->getPosition();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Ogre::Vector3");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setMode of class  EmberOgre::AvatarCamera */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_AvatarCamera_setMode00
static int tolua_EmberOgre_EmberOgre_AvatarCamera_setMode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::AvatarCamera",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::AvatarCamera* self = (EmberOgre::AvatarCamera*)  tolua_tousertype(tolua_S,1,0);
  EmberOgre::AvatarCamera::Mode mode = ((EmberOgre::AvatarCamera::Mode) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMode'",NULL);
#endif
  {
   self->setMode(mode);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setMode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setAvatarNode of class  EmberOgre::AvatarCamera */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_AvatarCamera_setAvatarNode00
static int tolua_EmberOgre_EmberOgre_AvatarCamera_setAvatarNode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::AvatarCamera",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::AvatarCamera* self = (EmberOgre::AvatarCamera*)  tolua_tousertype(tolua_S,1,0);
  Ogre::SceneNode* sceneNode = ((Ogre::SceneNode*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAvatarNode'",NULL);
#endif
  {
   self->setAvatarNode(sceneNode);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAvatarNode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: MovedCamera of class  EmberOgre::AvatarCamera */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__AvatarCamera_MovedCamera
static int tolua_get_EmberOgre__AvatarCamera_MovedCamera(lua_State* tolua_S)
{
  EmberOgre::AvatarCamera* self = (EmberOgre::AvatarCamera*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'MovedCamera'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->MovedCamera,"sigc::signal<void,Ogre::Camera*>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: MovedCamera of class  EmberOgre::AvatarCamera */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__AvatarCamera_MovedCamera
static int tolua_set_EmberOgre__AvatarCamera_MovedCamera(lua_State* tolua_S)
{
  EmberOgre::AvatarCamera* self = (EmberOgre::AvatarCamera*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'MovedCamera'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,Ogre::Camera*>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->MovedCamera = *((sigc::signal<void,Ogre::Camera*>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventChangedCameraDistance of class  EmberOgre::AvatarCamera */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__AvatarCamera_EventChangedCameraDistance
static int tolua_get_EmberOgre__AvatarCamera_EventChangedCameraDistance(lua_State* tolua_S)
{
  EmberOgre::AvatarCamera* self = (EmberOgre::AvatarCamera*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventChangedCameraDistance'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventChangedCameraDistance,"sigc::signal<void,float>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventChangedCameraDistance of class  EmberOgre::AvatarCamera */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__AvatarCamera_EventChangedCameraDistance
static int tolua_set_EmberOgre__AvatarCamera_EventChangedCameraDistance(lua_State* tolua_S)
{
  EmberOgre::AvatarCamera* self = (EmberOgre::AvatarCamera*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventChangedCameraDistance'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,float>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventChangedCameraDistance = *((sigc::signal<void,float>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: pickInWorld of class  EmberOgre::AvatarCamera */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_AvatarCamera_pickInWorld00
static int tolua_EmberOgre_EmberOgre_AvatarCamera_pickInWorld00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::AvatarCamera",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"const EmberOgre::MousePickerArgs",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::AvatarCamera* self = (EmberOgre::AvatarCamera*)  tolua_tousertype(tolua_S,1,0);
  float mouseX = ((float)  tolua_tonumber(tolua_S,2,0));
  float mouseY = ((float)  tolua_tonumber(tolua_S,3,0));
  const EmberOgre::MousePickerArgs* args = ((const EmberOgre::MousePickerArgs*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'pickInWorld'",NULL);
#endif
  {
   self->pickInWorld(mouseX,mouseY,*args);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'pickInWorld'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: worldToScreen of class  EmberOgre::AvatarCamera */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_AvatarCamera_worldToScreen00
static int tolua_EmberOgre_EmberOgre_AvatarCamera_worldToScreen00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::AvatarCamera",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::AvatarCamera* self = (EmberOgre::AvatarCamera*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector3* worldPos = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
  Ogre::Vector3* screenPos = ((Ogre::Vector3*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'worldToScreen'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->worldToScreen(*worldPos,*screenPos);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'worldToScreen'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: attach of class  EmberOgre::AvatarCamera */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_AvatarCamera_attach00
static int tolua_EmberOgre_EmberOgre_AvatarCamera_attach00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::AvatarCamera",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::AvatarCamera* self = (EmberOgre::AvatarCamera*)  tolua_tousertype(tolua_S,1,0);
  Ogre::SceneNode* toNode = ((Ogre::SceneNode*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'attach'",NULL);
#endif
  {
   self->attach(toNode);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'attach'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setCameraDistance of class  EmberOgre::AvatarCamera */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_AvatarCamera_setCameraDistance00
static int tolua_EmberOgre_EmberOgre_AvatarCamera_setCameraDistance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::AvatarCamera",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::AvatarCamera* self = (EmberOgre::AvatarCamera*)  tolua_tousertype(tolua_S,1,0);
  float distance = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setCameraDistance'",NULL);
#endif
  {
   self->setCameraDistance(distance);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setCameraDistance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: enableCompositor of class  EmberOgre::AvatarCamera */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_AvatarCamera_enableCompositor00
static int tolua_EmberOgre_EmberOgre_AvatarCamera_enableCompositor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::AvatarCamera",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::AvatarCamera* self = (EmberOgre::AvatarCamera*)  tolua_tousertype(tolua_S,1,0);
  const std::string compositorName = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  bool enable = ((bool)  tolua_toboolean(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'enableCompositor'",NULL);
#endif
  {
   self->enableCompositor(compositorName,enable);
   tolua_pushcppstring(tolua_S,(const char*)compositorName);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'enableCompositor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: toggleRenderMode of class  EmberOgre::AvatarCamera */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_AvatarCamera_toggleRenderMode00
static int tolua_EmberOgre_EmberOgre_AvatarCamera_toggleRenderMode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::AvatarCamera",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::AvatarCamera* self = (EmberOgre::AvatarCamera*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'toggleRenderMode'",NULL);
#endif
  {
   self->toggleRenderMode();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'toggleRenderMode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: takeScreenshot of class  EmberOgre::AvatarCamera */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_AvatarCamera_takeScreenshot00
static int tolua_EmberOgre_EmberOgre_AvatarCamera_takeScreenshot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::AvatarCamera",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::AvatarCamera* self = (EmberOgre::AvatarCamera*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'takeScreenshot'",NULL);
#endif
  {
   self->takeScreenshot();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'takeScreenshot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::AvatarControllerMovement */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_AvatarControllerMovement_new00
static int tolua_EmberOgre_EmberOgre_AvatarControllerMovement_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::AvatarControllerMovement",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   EmberOgre::AvatarControllerMovement* tolua_ret = (EmberOgre::AvatarControllerMovement*)  new EmberOgre::AvatarControllerMovement();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::AvatarControllerMovement");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::AvatarControllerMovement */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_AvatarControllerMovement_new00_local
static int tolua_EmberOgre_EmberOgre_AvatarControllerMovement_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::AvatarControllerMovement",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   EmberOgre::AvatarControllerMovement* tolua_ret = (EmberOgre::AvatarControllerMovement*)  new EmberOgre::AvatarControllerMovement();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::AvatarControllerMovement");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: rotationDegHoriz of class  EmberOgre::AvatarControllerMovement */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__AvatarControllerMovement_rotationDegHoriz
static int tolua_get_EmberOgre__AvatarControllerMovement_rotationDegHoriz(lua_State* tolua_S)
{
  EmberOgre::AvatarControllerMovement* self = (EmberOgre::AvatarControllerMovement*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'rotationDegHoriz'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->rotationDegHoriz);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: rotationDegHoriz of class  EmberOgre::AvatarControllerMovement */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__AvatarControllerMovement_rotationDegHoriz
static int tolua_set_EmberOgre__AvatarControllerMovement_rotationDegHoriz(lua_State* tolua_S)
{
  EmberOgre::AvatarControllerMovement* self = (EmberOgre::AvatarControllerMovement*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'rotationDegHoriz'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->rotationDegHoriz = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: rotationDegVert of class  EmberOgre::AvatarControllerMovement */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__AvatarControllerMovement_rotationDegVert
static int tolua_get_EmberOgre__AvatarControllerMovement_rotationDegVert(lua_State* tolua_S)
{
  EmberOgre::AvatarControllerMovement* self = (EmberOgre::AvatarControllerMovement*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'rotationDegVert'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->rotationDegVert);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: rotationDegVert of class  EmberOgre::AvatarControllerMovement */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__AvatarControllerMovement_rotationDegVert
static int tolua_set_EmberOgre__AvatarControllerMovement_rotationDegVert(lua_State* tolua_S)
{
  EmberOgre::AvatarControllerMovement* self = (EmberOgre::AvatarControllerMovement*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'rotationDegVert'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->rotationDegVert = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: timeSlice of class  EmberOgre::AvatarControllerMovement */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__AvatarControllerMovement_timeSlice
static int tolua_get_EmberOgre__AvatarControllerMovement_timeSlice(lua_State* tolua_S)
{
  EmberOgre::AvatarControllerMovement* self = (EmberOgre::AvatarControllerMovement*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'timeSlice'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->timeSlice,"Ogre::Real");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: timeSlice of class  EmberOgre::AvatarControllerMovement */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__AvatarControllerMovement_timeSlice
static int tolua_set_EmberOgre__AvatarControllerMovement_timeSlice(lua_State* tolua_S)
{
  EmberOgre::AvatarControllerMovement* self = (EmberOgre::AvatarControllerMovement*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'timeSlice'",NULL);
  if (!tolua_isusertype(tolua_S,2,"Ogre::Real",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->timeSlice = *((Ogre::Real*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: movementDirection of class  EmberOgre::AvatarControllerMovement */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__AvatarControllerMovement_movementDirection
static int tolua_get_EmberOgre__AvatarControllerMovement_movementDirection(lua_State* tolua_S)
{
  EmberOgre::AvatarControllerMovement* self = (EmberOgre::AvatarControllerMovement*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'movementDirection'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->movementDirection,"Ogre::Vector3");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: movementDirection of class  EmberOgre::AvatarControllerMovement */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__AvatarControllerMovement_movementDirection
static int tolua_set_EmberOgre__AvatarControllerMovement_movementDirection(lua_State* tolua_S)
{
  EmberOgre::AvatarControllerMovement* self = (EmberOgre::AvatarControllerMovement*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'movementDirection'",NULL);
  if (!tolua_isusertype(tolua_S,2,"Ogre::Vector3",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->movementDirection = *((Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: mode of class  EmberOgre::AvatarControllerMovement */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__AvatarControllerMovement_mode
static int tolua_get_EmberOgre__AvatarControllerMovement_mode(lua_State* tolua_S)
{
  EmberOgre::AvatarControllerMovement* self = (EmberOgre::AvatarControllerMovement*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mode'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->mode);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: mode of class  EmberOgre::AvatarControllerMovement */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__AvatarControllerMovement_mode
static int tolua_set_EmberOgre__AvatarControllerMovement_mode(lua_State* tolua_S)
{
  EmberOgre::AvatarControllerMovement* self = (EmberOgre::AvatarControllerMovement*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mode'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->mode = ((EmberOgre::AvatarMovementMode::Mode) (int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: isMoving of class  EmberOgre::AvatarControllerMovement */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__AvatarControllerMovement_isMoving
static int tolua_get_EmberOgre__AvatarControllerMovement_isMoving(lua_State* tolua_S)
{
  EmberOgre::AvatarControllerMovement* self = (EmberOgre::AvatarControllerMovement*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'isMoving'",NULL);
#endif
  tolua_pushboolean(tolua_S,(bool)self->isMoving);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: isMoving of class  EmberOgre::AvatarControllerMovement */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__AvatarControllerMovement_isMoving
static int tolua_set_EmberOgre__AvatarControllerMovement_isMoving(lua_State* tolua_S)
{
  EmberOgre::AvatarControllerMovement* self = (EmberOgre::AvatarControllerMovement*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'isMoving'",NULL);
  if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->isMoving = ((bool)  tolua_toboolean(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: cameraOrientation of class  EmberOgre::AvatarControllerMovement */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__AvatarControllerMovement_cameraOrientation
static int tolua_get_EmberOgre__AvatarControllerMovement_cameraOrientation(lua_State* tolua_S)
{
  EmberOgre::AvatarControllerMovement* self = (EmberOgre::AvatarControllerMovement*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'cameraOrientation'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->cameraOrientation,"Ogre::Quaternion");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: cameraOrientation of class  EmberOgre::AvatarControllerMovement */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__AvatarControllerMovement_cameraOrientation
static int tolua_set_EmberOgre__AvatarControllerMovement_cameraOrientation(lua_State* tolua_S)
{
  EmberOgre::AvatarControllerMovement* self = (EmberOgre::AvatarControllerMovement*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'cameraOrientation'",NULL);
  if (!tolua_isusertype(tolua_S,2,"Ogre::Quaternion",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->cameraOrientation = *((Ogre::Quaternion*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventMovementModeChanged of class  EmberOgre::AvatarController */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__AvatarController_EventMovementModeChanged
static int tolua_get_EmberOgre__AvatarController_EventMovementModeChanged(lua_State* tolua_S)
{
  EmberOgre::AvatarController* self = (EmberOgre::AvatarController*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventMovementModeChanged'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventMovementModeChanged,"sigc::signal<void,EmberOgre::AvatarMovementMode::Mode>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventMovementModeChanged of class  EmberOgre::AvatarController */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__AvatarController_EventMovementModeChanged
static int tolua_set_EmberOgre__AvatarController_EventMovementModeChanged(lua_State* tolua_S)
{
  EmberOgre::AvatarController* self = (EmberOgre::AvatarController*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventMovementModeChanged'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,EmberOgre::AvatarMovementMode::Mode>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventMovementModeChanged = *((sigc::signal<void,EmberOgre::AvatarMovementMode::Mode>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAvatarCamera of class  EmberOgre::AvatarController */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_AvatarController_getAvatarCamera00
static int tolua_EmberOgre_EmberOgre_AvatarController_getAvatarCamera00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::AvatarController",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::AvatarController* self = (const EmberOgre::AvatarController*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAvatarCamera'",NULL);
#endif
  {
   EmberOgre::AvatarCamera* tolua_ret = (EmberOgre::AvatarCamera*)  self->getAvatarCamera();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::AvatarCamera");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAvatarCamera'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: detachCamera of class  EmberOgre::AvatarController */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_AvatarController_detachCamera00
static int tolua_EmberOgre_EmberOgre_AvatarController_detachCamera00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::AvatarController",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::AvatarController* self = (EmberOgre::AvatarController*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'detachCamera'",NULL);
#endif
  {
   self->detachCamera();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'detachCamera'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: attachCamera of class  EmberOgre::AvatarController */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_AvatarController_attachCamera00
static int tolua_EmberOgre_EmberOgre_AvatarController_attachCamera00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::AvatarController",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::AvatarController* self = (EmberOgre::AvatarController*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'attachCamera'",NULL);
#endif
  {
   self->attachCamera();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'attachCamera'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getCurrentMovement of class  EmberOgre::AvatarController */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_AvatarController_getCurrentMovement00
static int tolua_EmberOgre_EmberOgre_AvatarController_getCurrentMovement00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::AvatarController",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::AvatarController* self = (const EmberOgre::AvatarController*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCurrentMovement'",NULL);
#endif
  {
   const EmberOgre::AvatarControllerMovement& tolua_ret = (const EmberOgre::AvatarControllerMovement&)  self->getCurrentMovement();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const EmberOgre::AvatarControllerMovement");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getCurrentMovement'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: moveToPoint of class  EmberOgre::AvatarController */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_AvatarController_moveToPoint00
static int tolua_EmberOgre_EmberOgre_AvatarController_moveToPoint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::AvatarController",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::AvatarController* self = (EmberOgre::AvatarController*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector3* point = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'moveToPoint'",NULL);
#endif
  {
   self->moveToPoint(*point);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'moveToPoint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: teleportTo of class  EmberOgre::AvatarController */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_AvatarController_teleportTo00
static int tolua_EmberOgre_EmberOgre_AvatarController_teleportTo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::AvatarController",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"EmberOgre::EmberEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::AvatarController* self = (EmberOgre::AvatarController*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector3* point = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
  EmberOgre::EmberEntity* locationEntity = ((EmberOgre::EmberEntity*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'teleportTo'",NULL);
#endif
  {
   self->teleportTo(*point,locationEntity);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'teleportTo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: isMoving of class  EmberOgre::AvatarMovementState */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__AvatarMovementState_isMoving
static int tolua_get_EmberOgre__AvatarMovementState_isMoving(lua_State* tolua_S)
{
  EmberOgre::AvatarMovementState* self = (EmberOgre::AvatarMovementState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'isMoving'",NULL);
#endif
  tolua_pushboolean(tolua_S,(bool)self->isMoving);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: isMoving of class  EmberOgre::AvatarMovementState */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__AvatarMovementState_isMoving
static int tolua_set_EmberOgre__AvatarMovementState_isMoving(lua_State* tolua_S)
{
  EmberOgre::AvatarMovementState* self = (EmberOgre::AvatarMovementState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'isMoving'",NULL);
  if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->isMoving = ((bool)  tolua_toboolean(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: isRunning of class  EmberOgre::AvatarMovementState */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__AvatarMovementState_isRunning
static int tolua_get_EmberOgre__AvatarMovementState_isRunning(lua_State* tolua_S)
{
  EmberOgre::AvatarMovementState* self = (EmberOgre::AvatarMovementState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'isRunning'",NULL);
#endif
  tolua_pushboolean(tolua_S,(bool)self->isRunning);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: isRunning of class  EmberOgre::AvatarMovementState */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__AvatarMovementState_isRunning
static int tolua_set_EmberOgre__AvatarMovementState_isRunning(lua_State* tolua_S)
{
  EmberOgre::AvatarMovementState* self = (EmberOgre::AvatarMovementState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'isRunning'",NULL);
  if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->isRunning = ((bool)  tolua_toboolean(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: velocity of class  EmberOgre::AvatarMovementState */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__AvatarMovementState_velocity
static int tolua_get_EmberOgre__AvatarMovementState_velocity(lua_State* tolua_S)
{
  EmberOgre::AvatarMovementState* self = (EmberOgre::AvatarMovementState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'velocity'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->velocity,"Ogre::Vector3");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: velocity of class  EmberOgre::AvatarMovementState */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__AvatarMovementState_velocity
static int tolua_set_EmberOgre__AvatarMovementState_velocity(lua_State* tolua_S)
{
  EmberOgre::AvatarMovementState* self = (EmberOgre::AvatarMovementState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'velocity'",NULL);
  if (!tolua_isusertype(tolua_S,2,"Ogre::Vector3",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->velocity = *((Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: orientation of class  EmberOgre::AvatarMovementState */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__AvatarMovementState_orientation
static int tolua_get_EmberOgre__AvatarMovementState_orientation(lua_State* tolua_S)
{
  EmberOgre::AvatarMovementState* self = (EmberOgre::AvatarMovementState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'orientation'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->orientation,"Ogre::Quaternion");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: orientation of class  EmberOgre::AvatarMovementState */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__AvatarMovementState_orientation
static int tolua_set_EmberOgre__AvatarMovementState_orientation(lua_State* tolua_S)
{
  EmberOgre::AvatarMovementState* self = (EmberOgre::AvatarMovementState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'orientation'",NULL);
  if (!tolua_isusertype(tolua_S,2,"Ogre::Quaternion",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->orientation = *((Ogre::Quaternion*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAvatarCamera of class  EmberOgre::Avatar */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Avatar_getAvatarCamera00
static int tolua_EmberOgre_EmberOgre_Avatar_getAvatarCamera00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Avatar",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Avatar* self = (const EmberOgre::Avatar*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAvatarCamera'",NULL);
#endif
  {
   EmberOgre::AvatarCamera* tolua_ret = (EmberOgre::AvatarCamera*)  self->getAvatarCamera();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::AvatarCamera");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAvatarCamera'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAvatarSceneNode of class  EmberOgre::Avatar */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Avatar_getAvatarSceneNode00
static int tolua_EmberOgre_EmberOgre_Avatar_getAvatarSceneNode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Avatar",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Avatar* self = (const EmberOgre::Avatar*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAvatarSceneNode'",NULL);
#endif
  {
   Ogre::SceneNode* tolua_ret = (Ogre::SceneNode*)  self->getAvatarSceneNode();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::SceneNode");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAvatarSceneNode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setAvatarController of class  EmberOgre::Avatar */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Avatar_setAvatarController00
static int tolua_EmberOgre_EmberOgre_Avatar_setAvatarController00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Avatar",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"EmberOgre::AvatarController",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Avatar* self = (EmberOgre::Avatar*)  tolua_tousertype(tolua_S,1,0);
  EmberOgre::AvatarController* avatarController = ((EmberOgre::AvatarController*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAvatarController'",NULL);
#endif
  {
   self->setAvatarController(avatarController);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAvatarController'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAvatarEmberEntity of class  EmberOgre::Avatar */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Avatar_getAvatarEmberEntity00
static int tolua_EmberOgre_EmberOgre_Avatar_getAvatarEmberEntity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Avatar",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Avatar* self = (EmberOgre::Avatar*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAvatarEmberEntity'",NULL);
#endif
  {
   EmberOgre::AvatarEmberEntity* tolua_ret = (EmberOgre::AvatarEmberEntity*)  self->getAvatarEmberEntity();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::AvatarEmberEntity");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAvatarEmberEntity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setMinIntervalOfRotationChanges of class  EmberOgre::Avatar */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Avatar_setMinIntervalOfRotationChanges00
static int tolua_EmberOgre_EmberOgre_Avatar_setMinIntervalOfRotationChanges00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Avatar",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::Real",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Avatar* self = (EmberOgre::Avatar*)  tolua_tousertype(tolua_S,1,0);
  Ogre::Real milliseconds = *((Ogre::Real*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMinIntervalOfRotationChanges'",NULL);
#endif
  {
   self->setMinIntervalOfRotationChanges(milliseconds);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setMinIntervalOfRotationChanges'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventAddedEntityToInventory of class  EmberOgre::Avatar */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__Avatar_EventAddedEntityToInventory
static int tolua_get_EmberOgre__Avatar_EventAddedEntityToInventory(lua_State* tolua_S)
{
  EmberOgre::Avatar* self = (EmberOgre::Avatar*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventAddedEntityToInventory'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventAddedEntityToInventory,"sigc::signal<void,EmberOgre::EmberEntity*>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventAddedEntityToInventory of class  EmberOgre::Avatar */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__Avatar_EventAddedEntityToInventory
static int tolua_set_EmberOgre__Avatar_EventAddedEntityToInventory(lua_State* tolua_S)
{
  EmberOgre::Avatar* self = (EmberOgre::Avatar*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventAddedEntityToInventory'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,EmberOgre::EmberEntity*>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventAddedEntityToInventory = *((sigc::signal<void,EmberOgre::EmberEntity*>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventRemovedEntityFromInventory of class  EmberOgre::Avatar */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__Avatar_EventRemovedEntityFromInventory
static int tolua_get_EmberOgre__Avatar_EventRemovedEntityFromInventory(lua_State* tolua_S)
{
  EmberOgre::Avatar* self = (EmberOgre::Avatar*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventRemovedEntityFromInventory'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventRemovedEntityFromInventory,"sigc::signal<void,EmberOgre::EmberEntity*>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventRemovedEntityFromInventory of class  EmberOgre::Avatar */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__Avatar_EventRemovedEntityFromInventory
static int tolua_set_EmberOgre__Avatar_EventRemovedEntityFromInventory(lua_State* tolua_S)
{
  EmberOgre::Avatar* self = (EmberOgre::Avatar*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventRemovedEntityFromInventory'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,EmberOgre::EmberEntity*>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventRemovedEntityFromInventory = *((sigc::signal<void,EmberOgre::EmberEntity*>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: isAdmin of class  EmberOgre::Avatar */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Avatar_isAdmin00
static int tolua_EmberOgre_EmberOgre_Avatar_isAdmin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Avatar",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Avatar* self = (const EmberOgre::Avatar*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isAdmin'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isAdmin();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isAdmin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getWorld of class  EmberOgre::EmberEntityFactory */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EmberEntityFactory_getWorld00
static int tolua_EmberOgre_EmberOgre_EmberEntityFactory_getWorld00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::EmberEntityFactory",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::EmberEntityFactory* self = (const EmberOgre::EmberEntityFactory*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getWorld'",NULL);
#endif
  {
   EmberOgre::WorldEmberEntity* tolua_ret = (EmberOgre::WorldEmberEntity*)  self->getWorld();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::WorldEmberEntity");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getWorld'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: dumpAttributesOfEntity of class  EmberOgre::EmberEntityFactory */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EmberEntityFactory_dumpAttributesOfEntity00
static int tolua_EmberOgre_EmberOgre_EmberEntityFactory_dumpAttributesOfEntity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::EmberEntityFactory",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::EmberEntityFactory* self = (const EmberOgre::EmberEntityFactory*)  tolua_tousertype(tolua_S,1,0);
  const std::string entityId = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'dumpAttributesOfEntity'",NULL);
#endif
  {
   self->dumpAttributesOfEntity(entityId);
   tolua_pushcppstring(tolua_S,(const char*)entityId);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'dumpAttributesOfEntity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSceneNode of class  EmberOgre::EmberEntity */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EmberEntity_getSceneNode00
static int tolua_EmberOgre_EmberOgre_EmberEntity_getSceneNode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::EmberEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::EmberEntity* self = (const EmberOgre::EmberEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSceneNode'",NULL);
#endif
  {
   Ogre::SceneNode* tolua_ret = (Ogre::SceneNode*)  self->getSceneNode();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::SceneNode");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSceneNode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: hasSuggestedResponses of class  EmberOgre::EmberEntity */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EmberEntity_hasSuggestedResponses00
static int tolua_EmberOgre_EmberOgre_EmberEntity_hasSuggestedResponses00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::EmberEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::EmberEntity* self = (const EmberOgre::EmberEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hasSuggestedResponses'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->hasSuggestedResponses();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'hasSuggestedResponses'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSuggestedResponses of class  EmberOgre::EmberEntity */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EmberEntity_getSuggestedResponses00
static int tolua_EmberOgre_EmberOgre_EmberEntity_getSuggestedResponses00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::EmberEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::EmberEntity* self = (const EmberOgre::EmberEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSuggestedResponses'",NULL);
#endif
  {
   const std::vector<std::string>& tolua_ret = (const std::vector<std::string>&)  self->getSuggestedResponses();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const std::vector<std::string>");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSuggestedResponses'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setVisible of class  EmberOgre::EmberEntity */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EmberEntity_setVisible00
static int tolua_EmberOgre_EmberOgre_EmberEntity_setVisible00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::EmberEntity",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::EmberEntity* self = (EmberOgre::EmberEntity*)  tolua_tousertype(tolua_S,1,0);
  bool visible = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setVisible'",NULL);
#endif
  {
   self->setVisible(visible);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setVisible'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getEmberLocation of class  EmberOgre::EmberEntity */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EmberEntity_getEmberLocation00
static int tolua_EmberOgre_EmberOgre_EmberEntity_getEmberLocation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::EmberEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::EmberEntity* self = (const EmberOgre::EmberEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getEmberLocation'",NULL);
#endif
  {
   EmberOgre::EmberEntity* tolua_ret = (EmberOgre::EmberEntity*)  self->getEmberLocation();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::EmberEntity");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getEmberLocation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isInitialized of class  EmberOgre::EmberEntity */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EmberEntity_isInitialized00
static int tolua_EmberOgre_EmberOgre_EmberEntity_isInitialized00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::EmberEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::EmberEntity* self = (const EmberOgre::EmberEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isInitialized'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isInitialized();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isInitialized'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMovementMode of class  EmberOgre::EmberEntity */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EmberEntity_getMovementMode00
static int tolua_EmberOgre_EmberOgre_EmberEntity_getMovementMode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::EmberEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::EmberEntity* self = (const EmberOgre::EmberEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMovementMode'",NULL);
#endif
  {
   EmberOgre::EmberEntity::MovementMode tolua_ret = (EmberOgre::EmberEntity::MovementMode)  self->getMovementMode();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMovementMode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: showOgreBoundingBox of class  EmberOgre::EmberEntity */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EmberEntity_showOgreBoundingBox00
static int tolua_EmberOgre_EmberOgre_EmberEntity_showOgreBoundingBox00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::EmberEntity",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::EmberEntity* self = (EmberOgre::EmberEntity*)  tolua_tousertype(tolua_S,1,0);
  bool show = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'showOgreBoundingBox'",NULL);
#endif
  {
   self->showOgreBoundingBox(show);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'showOgreBoundingBox'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: showErisBoundingBox of class  EmberOgre::EmberEntity */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EmberEntity_showErisBoundingBox00
static int tolua_EmberOgre_EmberOgre_EmberEntity_showErisBoundingBox00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::EmberEntity",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::EmberEntity* self = (EmberOgre::EmberEntity*)  tolua_tousertype(tolua_S,1,0);
  bool show = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'showErisBoundingBox'",NULL);
#endif
  {
   self->showErisBoundingBox(show);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'showErisBoundingBox'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getShowOgreBoundingBox of class  EmberOgre::EmberEntity */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EmberEntity_getShowOgreBoundingBox00
static int tolua_EmberOgre_EmberOgre_EmberEntity_getShowOgreBoundingBox00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::EmberEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::EmberEntity* self = (const EmberOgre::EmberEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getShowOgreBoundingBox'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->getShowOgreBoundingBox();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getShowOgreBoundingBox'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getShowErisBoundingBox of class  EmberOgre::EmberEntity */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EmberEntity_getShowErisBoundingBox00
static int tolua_EmberOgre_EmberOgre_EmberEntity_getShowErisBoundingBox00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::EmberEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::EmberEntity* self = (const EmberOgre::EmberEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getShowErisBoundingBox'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->getShowErisBoundingBox();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getShowErisBoundingBox'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getWorldBoundingBox of class  EmberOgre::EmberEntity */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EmberEntity_getWorldBoundingBox00
static int tolua_EmberOgre_EmberOgre_EmberEntity_getWorldBoundingBox00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::EmberEntity",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::EmberEntity* self = (const EmberOgre::EmberEntity*)  tolua_tousertype(tolua_S,1,0);
  bool derive = ((bool)  tolua_toboolean(tolua_S,2,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getWorldBoundingBox'",NULL);
#endif
  {
   const Ogre::AxisAlignedBox& tolua_ret = (const Ogre::AxisAlignedBox&)  self->getWorldBoundingBox(derive);
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Ogre::AxisAlignedBox");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getWorldBoundingBox'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getDefaultUseOperators of class  EmberOgre::EmberEntity */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EmberEntity_getDefaultUseOperators00
static int tolua_EmberOgre_EmberOgre_EmberEntity_getDefaultUseOperators00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::EmberEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::EmberEntity* self = (EmberOgre::EmberEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getDefaultUseOperators'",NULL);
#endif
  {
   std::vector<std::string> tolua_ret = (std::vector<std::string>)  self->getDefaultUseOperators();
   {
#ifdef __cplusplus
    void* tolua_obj = new std::vector<std::string>(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"std::vector<std::string>");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(std::vector<std::string>));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"std::vector<std::string>");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getDefaultUseOperators'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setVisualize of class  EmberOgre::EmberEntity */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EmberEntity_setVisualize00
static int tolua_EmberOgre_EmberOgre_EmberEntity_setVisualize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::EmberEntity",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::EmberEntity* self = (EmberOgre::EmberEntity*)  tolua_tousertype(tolua_S,1,0);
  const std::string visualization = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  bool visualize = ((bool)  tolua_toboolean(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setVisualize'",NULL);
#endif
  {
   self->setVisualize(visualization,visualize);
   tolua_pushcppstring(tolua_S,(const char*)visualization);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setVisualize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVisualize of class  EmberOgre::EmberEntity */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EmberEntity_getVisualize00
static int tolua_EmberOgre_EmberOgre_EmberEntity_getVisualize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::EmberEntity",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::EmberEntity* self = (const EmberOgre::EmberEntity*)  tolua_tousertype(tolua_S,1,0);
  const std::string visualization = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVisualize'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->getVisualize(visualization);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)visualization);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVisualize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: ACTION_STAND of class  EmberOgre::EmberPhysicalEntity */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__EmberPhysicalEntity_ACTION_STAND
static int tolua_get_EmberOgre__EmberPhysicalEntity_ACTION_STAND(lua_State* tolua_S)
{
  tolua_pushusertype(tolua_S,(void*)&EmberOgre::EmberPhysicalEntity::ACTION_STAND,"const const");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: ACTION_RUN of class  EmberOgre::EmberPhysicalEntity */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__EmberPhysicalEntity_ACTION_RUN
static int tolua_get_EmberOgre__EmberPhysicalEntity_ACTION_RUN(lua_State* tolua_S)
{
  tolua_pushusertype(tolua_S,(void*)&EmberOgre::EmberPhysicalEntity::ACTION_RUN,"const const");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: ACTION_WALK of class  EmberOgre::EmberPhysicalEntity */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__EmberPhysicalEntity_ACTION_WALK
static int tolua_get_EmberOgre__EmberPhysicalEntity_ACTION_WALK(lua_State* tolua_S)
{
  tolua_pushusertype(tolua_S,(void*)&EmberOgre::EmberPhysicalEntity::ACTION_WALK,"const const");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: ACTION_SWIM of class  EmberOgre::EmberPhysicalEntity */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__EmberPhysicalEntity_ACTION_SWIM
static int tolua_get_EmberOgre__EmberPhysicalEntity_ACTION_SWIM(lua_State* tolua_S)
{
  tolua_pushusertype(tolua_S,(void*)&EmberOgre::EmberPhysicalEntity::ACTION_SWIM,"const const");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: ACTION_FLOAT of class  EmberOgre::EmberPhysicalEntity */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__EmberPhysicalEntity_ACTION_FLOAT
static int tolua_get_EmberOgre__EmberPhysicalEntity_ACTION_FLOAT(lua_State* tolua_S)
{
  tolua_pushusertype(tolua_S,(void*)&EmberOgre::EmberPhysicalEntity::ACTION_FLOAT,"const const");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getModel of class  EmberOgre::EmberPhysicalEntity */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EmberPhysicalEntity_getModel00
static int tolua_EmberOgre_EmberOgre_EmberPhysicalEntity_getModel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::EmberPhysicalEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::EmberPhysicalEntity* self = (const EmberOgre::EmberPhysicalEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getModel'",NULL);
#endif
  {
   EmberOgre::Model::Model* tolua_ret = (EmberOgre::Model::Model*)  self->getModel();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Model::Model");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getModel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getScaleNode of class  EmberOgre::EmberPhysicalEntity */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EmberPhysicalEntity_getScaleNode00
static int tolua_EmberOgre_EmberOgre_EmberPhysicalEntity_getScaleNode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::EmberPhysicalEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::EmberPhysicalEntity* self = (const EmberOgre::EmberPhysicalEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getScaleNode'",NULL);
#endif
  {
   Ogre::SceneNode* tolua_ret = (Ogre::SceneNode*)  self->getScaleNode();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::SceneNode");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getScaleNode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getEntityAttachedToPoint of class  EmberOgre::EmberPhysicalEntity */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EmberPhysicalEntity_getEntityAttachedToPoint00
static int tolua_EmberOgre_EmberOgre_EmberPhysicalEntity_getEntityAttachedToPoint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::EmberPhysicalEntity",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::EmberPhysicalEntity* self = (EmberOgre::EmberPhysicalEntity*)  tolua_tousertype(tolua_S,1,0);
  const std::string attachPoint = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getEntityAttachedToPoint'",NULL);
#endif
  {
   EmberOgre::EmberEntity* tolua_ret = (EmberOgre::EmberEntity*)  self->getEntityAttachedToPoint(attachPoint);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::EmberEntity");
   tolua_pushcppstring(tolua_S,(const char*)attachPoint);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getEntityAttachedToPoint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getWorldBoundingBox of class  EmberOgre::EmberPhysicalEntity */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EmberPhysicalEntity_getWorldBoundingBox00
static int tolua_EmberOgre_EmberOgre_EmberPhysicalEntity_getWorldBoundingBox00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::EmberPhysicalEntity",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::EmberPhysicalEntity* self = (const EmberOgre::EmberPhysicalEntity*)  tolua_tousertype(tolua_S,1,0);
  bool derive = ((bool)  tolua_toboolean(tolua_S,2,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getWorldBoundingBox'",NULL);
#endif
  {
   const Ogre::AxisAlignedBox& tolua_ret = (const Ogre::AxisAlignedBox&)  self->getWorldBoundingBox(derive);
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Ogre::AxisAlignedBox");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getWorldBoundingBox'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAvatar of class  EmberOgre::AvatarEmberEntity */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_AvatarEmberEntity_getAvatar00
static int tolua_EmberOgre_EmberOgre_AvatarEmberEntity_getAvatar00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::AvatarEmberEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::AvatarEmberEntity* self = (EmberOgre::AvatarEmberEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAvatar'",NULL);
#endif
  {
   EmberOgre::Avatar* tolua_ret = (EmberOgre::Avatar*)  self->getAvatar();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Avatar");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAvatar'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAvatarSceneNode of class  EmberOgre::AvatarEmberEntity */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_AvatarEmberEntity_getAvatarSceneNode00
static int tolua_EmberOgre_EmberOgre_AvatarEmberEntity_getAvatarSceneNode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::AvatarEmberEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::AvatarEmberEntity* self = (EmberOgre::AvatarEmberEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAvatarSceneNode'",NULL);
#endif
  {
   Ogre::SceneNode* tolua_ret = (Ogre::SceneNode*)  self->getAvatarSceneNode();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::SceneNode");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAvatarSceneNode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getEnvironment of class  EmberOgre::WorldEmberEntity */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_WorldEmberEntity_getEnvironment00
static int tolua_EmberOgre_EmberOgre_WorldEmberEntity_getEnvironment00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::WorldEmberEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::WorldEmberEntity* self = (const EmberOgre::WorldEmberEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getEnvironment'",NULL);
#endif
  {
   EmberOgre::Environment::Environment* tolua_ret = (EmberOgre::Environment::Environment*)  self->getEnvironment();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Environment::Environment");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getEnvironment'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getFoliage of class  EmberOgre::WorldEmberEntity */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_WorldEmberEntity_getFoliage00
static int tolua_EmberOgre_EmberOgre_WorldEmberEntity_getFoliage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::WorldEmberEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::WorldEmberEntity* self = (const EmberOgre::WorldEmberEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFoliage'",NULL);
#endif
  {
   EmberOgre::Environment::Foliage* tolua_ret = (EmberOgre::Environment::Foliage*)  self->getFoliage();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Environment::Foliage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getFoliage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventFoliageCreated of class  EmberOgre::WorldEmberEntity */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__WorldEmberEntity_EventFoliageCreated
static int tolua_get_EmberOgre__WorldEmberEntity_EventFoliageCreated(lua_State* tolua_S)
{
  EmberOgre::WorldEmberEntity* self = (EmberOgre::WorldEmberEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventFoliageCreated'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventFoliageCreated,"sigc::signal<void>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventFoliageCreated of class  EmberOgre::WorldEmberEntity */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__WorldEmberEntity_EventFoliageCreated
static int tolua_set_EmberOgre__WorldEmberEntity_EventFoliageCreated(lua_State* tolua_S)
{
  EmberOgre::WorldEmberEntity* self = (EmberOgre::WorldEmberEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventFoliageCreated'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventFoliageCreated = *((sigc::signal<void>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventEnvironmentCreated of class  EmberOgre::WorldEmberEntity */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__WorldEmberEntity_EventEnvironmentCreated
static int tolua_get_EmberOgre__WorldEmberEntity_EventEnvironmentCreated(lua_State* tolua_S)
{
  EmberOgre::WorldEmberEntity* self = (EmberOgre::WorldEmberEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventEnvironmentCreated'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventEnvironmentCreated,"sigc::signal<void>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventEnvironmentCreated of class  EmberOgre::WorldEmberEntity */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__WorldEmberEntity_EventEnvironmentCreated
static int tolua_set_EmberOgre__WorldEmberEntity_EventEnvironmentCreated(lua_State* tolua_S)
{
  EmberOgre::WorldEmberEntity* self = (EmberOgre::WorldEmberEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventEnvironmentCreated'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventEnvironmentCreated = *((sigc::signal<void>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: setAmbientLight of class  EmberOgre::Environment::ISun */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Environment_ISun_setAmbientLight00
static int tolua_EmberOgre_EmberOgre_Environment_ISun_setAmbientLight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Environment::ISun",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::ColourValue",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Environment::ISun* self = (EmberOgre::Environment::ISun*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::ColourValue* colour = ((const Ogre::ColourValue*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAmbientLight'",NULL);
#endif
  {
   self->setAmbientLight(*colour);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAmbientLight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setDensity of class  EmberOgre::Environment::IFog */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Environment_IFog_setDensity00
static int tolua_EmberOgre_EmberOgre_Environment_IFog_setDensity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Environment::IFog",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Environment::IFog* self = (EmberOgre::Environment::IFog*)  tolua_tousertype(tolua_S,1,0);
  float density = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setDensity'",NULL);
#endif
  {
   self->setDensity(density);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setDensity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getDensity of class  EmberOgre::Environment::IFog */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Environment_IFog_getDensity00
static int tolua_EmberOgre_EmberOgre_Environment_IFog_getDensity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Environment::IFog",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Environment::IFog* self = (const EmberOgre::Environment::IFog*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getDensity'",NULL);
#endif
  {
   float tolua_ret = (float)  self->getDensity();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getDensity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSun of class  EmberOgre::Environment::Environment */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Environment_Environment_getSun00
static int tolua_EmberOgre_EmberOgre_Environment_Environment_getSun00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Environment::Environment",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Environment::Environment* self = (EmberOgre::Environment::Environment*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSun'",NULL);
#endif
  {
   EmberOgre::Environment::ISun* tolua_ret = (EmberOgre::Environment::ISun*)  self->getSun();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Environment::ISun");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSun'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSky of class  EmberOgre::Environment::Environment */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Environment_Environment_getSky00
static int tolua_EmberOgre_EmberOgre_Environment_Environment_getSky00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Environment::Environment",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Environment::Environment* self = (EmberOgre::Environment::Environment*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSky'",NULL);
#endif
  {
   EmberOgre::Environment::ISky* tolua_ret = (EmberOgre::Environment::ISky*)  self->getSky();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Environment::ISky");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSky'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getFog of class  EmberOgre::Environment::Environment */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Environment_Environment_getFog00
static int tolua_EmberOgre_EmberOgre_Environment_Environment_getFog00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Environment::Environment",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Environment::Environment* self = (EmberOgre::Environment::Environment*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFog'",NULL);
#endif
  {
   EmberOgre::Environment::IFog* tolua_ret = (EmberOgre::Environment::IFog*)  self->getFog();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Environment::IFog");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getFog'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getWater of class  EmberOgre::Environment::Environment */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Environment_Environment_getWater00
static int tolua_EmberOgre_EmberOgre_Environment_Environment_getWater00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Environment::Environment",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Environment::Environment* self = (EmberOgre::Environment::Environment*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getWater'",NULL);
#endif
  {
   EmberOgre::Environment::IWater* tolua_ret = (EmberOgre::Environment::IWater*)  self->getWater();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Environment::IWater");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getWater'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setTime of class  EmberOgre::Environment::Environment */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Environment_Environment_setTime00
static int tolua_EmberOgre_EmberOgre_Environment_Environment_setTime00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Environment::Environment",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Environment::Environment* self = (EmberOgre::Environment::Environment*)  tolua_tousertype(tolua_S,1,0);
  int hour = ((int)  tolua_tonumber(tolua_S,2,0));
  int minute = ((int)  tolua_tonumber(tolua_S,3,0));
  int second = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setTime'",NULL);
#endif
  {
   self->setTime(hour,minute,second);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setTime'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setTime of class  EmberOgre::Environment::Environment */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Environment_Environment_setTime01
static int tolua_EmberOgre_EmberOgre_Environment_Environment_setTime01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Environment::Environment",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  EmberOgre::Environment::Environment* self = (EmberOgre::Environment::Environment*)  tolua_tousertype(tolua_S,1,0);
  int seconds = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setTime'",NULL);
#endif
  {
   self->setTime(seconds);
  }
 }
 return 0;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_Environment_Environment_setTime00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: setAmbientLight of class  EmberOgre::Environment::Environment */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Environment_Environment_setAmbientLight00
static int tolua_EmberOgre_EmberOgre_Environment_Environment_setAmbientLight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Environment::Environment",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::ColourValue",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Environment::Environment* self = (EmberOgre::Environment::Environment*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::ColourValue* colour = ((const Ogre::ColourValue*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAmbientLight'",NULL);
#endif
  {
   self->setAmbientLight(*colour);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAmbientLight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventUpdatedAmbientLight of class  EmberOgre::Environment::Environment */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__Environment__Environment_EventUpdatedAmbientLight
static int tolua_get_EmberOgre__Environment__Environment_EventUpdatedAmbientLight(lua_State* tolua_S)
{
  EmberOgre::Environment::Environment* self = (EmberOgre::Environment::Environment*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventUpdatedAmbientLight'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventUpdatedAmbientLight,"sigc::signal<void,const Ogre::ColourValue&>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventUpdatedAmbientLight of class  EmberOgre::Environment::Environment */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__Environment__Environment_EventUpdatedAmbientLight
static int tolua_set_EmberOgre__Environment__Environment_EventUpdatedAmbientLight(lua_State* tolua_S)
{
  EmberOgre::Environment::Environment* self = (EmberOgre::Environment::Environment*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventUpdatedAmbientLight'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,const Ogre::ColourValue&>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventUpdatedAmbientLight = *((sigc::signal<void,const Ogre::ColourValue&>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSingleton of class  EmberOgre::GUIManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_GUIManager_getSingleton00
static int tolua_EmberOgre_EmberOgre_GUIManager_getSingleton00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::GUIManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   EmberOgre::GUIManager& tolua_ret = (EmberOgre::GUIManager&)  EmberOgre::GUIManager::getSingleton();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"EmberOgre::GUIManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSingleton'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: AppendIGChatLine of class  EmberOgre::GUIManager */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__GUIManager_AppendIGChatLine
static int tolua_get_EmberOgre__GUIManager_AppendIGChatLine(lua_State* tolua_S)
{
  EmberOgre::GUIManager* self = (EmberOgre::GUIManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'AppendIGChatLine'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->AppendIGChatLine,"sigc::signal<void,const std::string&,EmberOgre::EmberEntity*>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: AppendIGChatLine of class  EmberOgre::GUIManager */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__GUIManager_AppendIGChatLine
static int tolua_set_EmberOgre__GUIManager_AppendIGChatLine(lua_State* tolua_S)
{
  EmberOgre::GUIManager* self = (EmberOgre::GUIManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'AppendIGChatLine'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,const std::string&,EmberOgre::EmberEntity*>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->AppendIGChatLine = *((sigc::signal<void,const std::string&,EmberOgre::EmberEntity*>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: AppendOOGChatLine of class  EmberOgre::GUIManager */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__GUIManager_AppendOOGChatLine
static int tolua_get_EmberOgre__GUIManager_AppendOOGChatLine(lua_State* tolua_S)
{
  EmberOgre::GUIManager* self = (EmberOgre::GUIManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'AppendOOGChatLine'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->AppendOOGChatLine,"sigc::signal<void,const std::string&,EmberOgre::EmberEntity*>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: AppendOOGChatLine of class  EmberOgre::GUIManager */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__GUIManager_AppendOOGChatLine
static int tolua_set_EmberOgre__GUIManager_AppendOOGChatLine(lua_State* tolua_S)
{
  EmberOgre::GUIManager* self = (EmberOgre::GUIManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'AppendOOGChatLine'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,const std::string&,EmberOgre::EmberEntity*>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->AppendOOGChatLine = *((sigc::signal<void,const std::string&,EmberOgre::EmberEntity*>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: AppendAvatarImaginary of class  EmberOgre::GUIManager */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__GUIManager_AppendAvatarImaginary
static int tolua_get_EmberOgre__GUIManager_AppendAvatarImaginary(lua_State* tolua_S)
{
  EmberOgre::GUIManager* self = (EmberOgre::GUIManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'AppendAvatarImaginary'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->AppendAvatarImaginary,"sigc::signal<void,const std::string&>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: AppendAvatarImaginary of class  EmberOgre::GUIManager */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__GUIManager_AppendAvatarImaginary
static int tolua_set_EmberOgre__GUIManager_AppendAvatarImaginary(lua_State* tolua_S)
{
  EmberOgre::GUIManager* self = (EmberOgre::GUIManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'AppendAvatarImaginary'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,const std::string&>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->AppendAvatarImaginary = *((sigc::signal<void,const std::string&>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventEntityAction of class  EmberOgre::GUIManager */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__GUIManager_EventEntityAction
static int tolua_get_EmberOgre__GUIManager_EventEntityAction(lua_State* tolua_S)
{
  EmberOgre::GUIManager* self = (EmberOgre::GUIManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventEntityAction'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventEntityAction,"sigc::signal<void,const std::string&,EmberOgre::EmberEntity*>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventEntityAction of class  EmberOgre::GUIManager */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__GUIManager_EventEntityAction
static int tolua_set_EmberOgre__GUIManager_EventEntityAction(lua_State* tolua_S)
{
  EmberOgre::GUIManager* self = (EmberOgre::GUIManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventEntityAction'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,const std::string&,EmberOgre::EmberEntity*>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventEntityAction = *((sigc::signal<void,const std::string&,EmberOgre::EmberEntity*>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventFrameStarted of class  EmberOgre::GUIManager */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__GUIManager_EventFrameStarted
static int tolua_get_EmberOgre__GUIManager_EventFrameStarted(lua_State* tolua_S)
{
  EmberOgre::GUIManager* self = (EmberOgre::GUIManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventFrameStarted'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventFrameStarted,"sigc::signal<void,float>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventFrameStarted of class  EmberOgre::GUIManager */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__GUIManager_EventFrameStarted
static int tolua_set_EmberOgre__GUIManager_EventFrameStarted(lua_State* tolua_S)
{
  EmberOgre::GUIManager* self = (EmberOgre::GUIManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventFrameStarted'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,float>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventFrameStarted = *((sigc::signal<void,float>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeWidget of class  EmberOgre::GUIManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_GUIManager_removeWidget00
static int tolua_EmberOgre_EmberOgre_GUIManager_removeWidget00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::GUIManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"EmberOgre::Gui::Widget",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::GUIManager* self = (EmberOgre::GUIManager*)  tolua_tousertype(tolua_S,1,0);
  EmberOgre::Gui::Widget* widget = ((EmberOgre::Gui::Widget*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeWidget'",NULL);
#endif
  {
   self->removeWidget(widget);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeWidget'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addWidget of class  EmberOgre::GUIManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_GUIManager_addWidget00
static int tolua_EmberOgre_EmberOgre_GUIManager_addWidget00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::GUIManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"EmberOgre::Gui::Widget",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::GUIManager* self = (EmberOgre::GUIManager*)  tolua_tousertype(tolua_S,1,0);
  EmberOgre::Gui::Widget* widget = ((EmberOgre::Gui::Widget*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addWidget'",NULL);
#endif
  {
   self->addWidget(widget);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addWidget'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: EmitEntityAction of class  EmberOgre::GUIManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_GUIManager_EmitEntityAction00
static int tolua_EmberOgre_EmberOgre_GUIManager_EmitEntityAction00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::GUIManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"EmberOgre::EmberEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::GUIManager* self = (EmberOgre::GUIManager*)  tolua_tousertype(tolua_S,1,0);
  const std::string action = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  EmberOgre::EmberEntity* entity = ((EmberOgre::EmberEntity*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'EmitEntityAction'",NULL);
#endif
  {
   self->EmitEntityAction(action,entity);
   tolua_pushcppstring(tolua_S,(const char*)action);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'EmitEntityAction'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMainSheet of class  EmberOgre::GUIManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_GUIManager_getMainSheet00
static int tolua_EmberOgre_EmberOgre_GUIManager_getMainSheet00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::GUIManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::GUIManager* self = (EmberOgre::GUIManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMainSheet'",NULL);
#endif
  {
   CEGUI::Window* tolua_ret = (CEGUI::Window*)  self->getMainSheet();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Window");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMainSheet'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setDebugText of class  EmberOgre::GUIManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_GUIManager_setDebugText00
static int tolua_EmberOgre_EmberOgre_GUIManager_setDebugText00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::GUIManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::GUIManager* self = (EmberOgre::GUIManager*)  tolua_tousertype(tolua_S,1,0);
  const std::string text = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setDebugText'",NULL);
#endif
  {
   self->setDebugText(text);
   tolua_pushcppstring(tolua_S,(const char*)text);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setDebugText'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isInGUIMode of class  EmberOgre::GUIManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_GUIManager_isInGUIMode00
static int tolua_EmberOgre_EmberOgre_GUIManager_isInGUIMode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::GUIManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::GUIManager* self = (const EmberOgre::GUIManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isInGUIMode'",NULL);
#endif
  {
   const bool tolua_ret = (const bool)  self->isInGUIMode();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isInGUIMode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isInMovementKeysMode of class  EmberOgre::GUIManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_GUIManager_isInMovementKeysMode00
static int tolua_EmberOgre_EmberOgre_GUIManager_isInMovementKeysMode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::GUIManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::GUIManager* self = (const EmberOgre::GUIManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isInMovementKeysMode'",NULL);
#endif
  {
   const bool tolua_ret = (const bool)  self->isInMovementKeysMode();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isInMovementKeysMode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getInput of class  EmberOgre::GUIManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_GUIManager_getInput00
static int tolua_EmberOgre_EmberOgre_GUIManager_getInput00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::GUIManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::GUIManager* self = (const EmberOgre::GUIManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getInput'",NULL);
#endif
  {
   EmberOgre::Input& tolua_ret = (EmberOgre::Input&)  self->getInput();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"EmberOgre::Input");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInput'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createWindow of class  EmberOgre::GUIManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_GUIManager_createWindow00
static int tolua_EmberOgre_EmberOgre_GUIManager_createWindow00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::GUIManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::GUIManager* self = (EmberOgre::GUIManager*)  tolua_tousertype(tolua_S,1,0);
  const std::string windowType = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createWindow'",NULL);
#endif
  {
   CEGUI::Window* tolua_ret = (CEGUI::Window*)  self->createWindow(windowType);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Window");
   tolua_pushcppstring(tolua_S,(const char*)windowType);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createWindow'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createWindow of class  EmberOgre::GUIManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_GUIManager_createWindow01
static int tolua_EmberOgre_EmberOgre_GUIManager_createWindow01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::GUIManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  EmberOgre::GUIManager* self = (EmberOgre::GUIManager*)  tolua_tousertype(tolua_S,1,0);
  const std::string windowType = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string windowName = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createWindow'",NULL);
#endif
  {
   CEGUI::Window* tolua_ret = (CEGUI::Window*)  self->createWindow(windowType,windowName);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Window");
   tolua_pushcppstring(tolua_S,(const char*)windowType);
   tolua_pushcppstring(tolua_S,(const char*)windowName);
  }
 }
 return 3;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_GUIManager_createWindow00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: createWidget of class  EmberOgre::GUIManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_GUIManager_createWidget00
static int tolua_EmberOgre_EmberOgre_GUIManager_createWidget00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::GUIManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::GUIManager* self = (EmberOgre::GUIManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createWidget'",NULL);
#endif
  {
   EmberOgre::Gui::Widget* tolua_ret = (EmberOgre::Gui::Widget*)  self->createWidget();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::Widget");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createWidget'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createWidget of class  EmberOgre::GUIManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_GUIManager_createWidget01
static int tolua_EmberOgre_EmberOgre_GUIManager_createWidget01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::GUIManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  EmberOgre::GUIManager* self = (EmberOgre::GUIManager*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createWidget'",NULL);
#endif
  {
   EmberOgre::Gui::Widget* tolua_ret = (EmberOgre::Gui::Widget*)  self->createWidget(name);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::Widget");
   tolua_pushcppstring(tolua_S,(const char*)name);
  }
 }
 return 2;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_GUIManager_createWidget00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getDefaultScheme of class  EmberOgre::GUIManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_GUIManager_getDefaultScheme00
static int tolua_EmberOgre_EmberOgre_GUIManager_getDefaultScheme00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::GUIManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::GUIManager* self = (const EmberOgre::GUIManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getDefaultScheme'",NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getDefaultScheme();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getDefaultScheme'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getEntityPickListener of class  EmberOgre::GUIManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_GUIManager_getEntityPickListener00
static int tolua_EmberOgre_EmberOgre_GUIManager_getEntityPickListener00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::GUIManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::GUIManager* self = (const EmberOgre::GUIManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getEntityPickListener'",NULL);
#endif
  {
   EmberOgre::EntityWorldPickListener* tolua_ret = (EmberOgre::EntityWorldPickListener*)  self->getEntityPickListener();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::EntityWorldPickListener");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getEntityPickListener'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getIconManager of class  EmberOgre::GUIManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_GUIManager_getIconManager00
static int tolua_EmberOgre_EmberOgre_GUIManager_getIconManager00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::GUIManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::GUIManager* self = (EmberOgre::GUIManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getIconManager'",NULL);
#endif
  {
   EmberOgre::Gui::Icons::IconManager* tolua_ret = (EmberOgre::Gui::Icons::IconManager*)  self->getIconManager();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::Icons::IconManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getIconManager'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getEntityIconManager of class  EmberOgre::GUIManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_GUIManager_getEntityIconManager00
static int tolua_EmberOgre_EmberOgre_GUIManager_getEntityIconManager00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::GUIManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::GUIManager* self = (EmberOgre::GUIManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getEntityIconManager'",NULL);
#endif
  {
   EmberOgre::Gui::EntityIconManager* tolua_ret = (EmberOgre::Gui::EntityIconManager*)  self->getEntityIconManager();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::EntityIconManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getEntityIconManager'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::Terrain::TerrainArea */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainArea_new00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainArea_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Terrain::TerrainArea",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"EmberOgre::EmberEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::EmberEntity* entity = ((EmberOgre::EmberEntity*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::Terrain::TerrainArea* tolua_ret = (EmberOgre::Terrain::TerrainArea*)  new EmberOgre::Terrain::TerrainArea(entity);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Terrain::TerrainArea");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::Terrain::TerrainArea */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainArea_new00_local
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainArea_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Terrain::TerrainArea",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"EmberOgre::EmberEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::EmberEntity* entity = ((EmberOgre::EmberEntity*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::Terrain::TerrainArea* tolua_ret = (EmberOgre::Terrain::TerrainArea*)  new EmberOgre::Terrain::TerrainArea(entity);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Terrain::TerrainArea");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: init of class  EmberOgre::Terrain::TerrainArea */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainArea_init00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainArea_init00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Terrain::TerrainArea",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Terrain::TerrainArea* self = (EmberOgre::Terrain::TerrainArea*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->init();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'init'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getArea of class  EmberOgre::Terrain::TerrainArea */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainArea_getArea00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainArea_getArea00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Terrain::TerrainArea",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Terrain::TerrainArea* self = (const EmberOgre::Terrain::TerrainArea*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getArea'",NULL);
#endif
  {
   Mercator::Area* tolua_ret = (Mercator::Area*)  self->getArea();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Mercator::Area");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getArea'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setArea of class  EmberOgre::Terrain::TerrainArea */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainArea_setArea00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainArea_setArea00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Terrain::TerrainArea",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Mercator::Area",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Terrain::TerrainArea* self = (EmberOgre::Terrain::TerrainArea*)  tolua_tousertype(tolua_S,1,0);
  Mercator::Area* area = ((Mercator::Area*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setArea'",NULL);
#endif
  {
   self->setArea(area);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setArea'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getBasePoint of class  EmberOgre::Terrain::BasePointUserObject */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_BasePointUserObject_getBasePoint00
static int tolua_EmberOgre_EmberOgre_Terrain_BasePointUserObject_getBasePoint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Terrain::BasePointUserObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Terrain::BasePointUserObject* self = (const EmberOgre::Terrain::BasePointUserObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getBasePoint'",NULL);
#endif
  {
   const Mercator::BasePoint& tolua_ret = (const Mercator::BasePoint&)  self->getBasePoint();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Mercator::BasePoint");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getBasePoint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getBasePointMarkerNode of class  EmberOgre::Terrain::BasePointUserObject */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_BasePointUserObject_getBasePointMarkerNode00
static int tolua_EmberOgre_EmberOgre_Terrain_BasePointUserObject_getBasePointMarkerNode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Terrain::BasePointUserObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Terrain::BasePointUserObject* self = (const EmberOgre::Terrain::BasePointUserObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getBasePointMarkerNode'",NULL);
#endif
  {
   Ogre::SceneNode* tolua_ret = (Ogre::SceneNode*)  self->getBasePointMarkerNode();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::SceneNode");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getBasePointMarkerNode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPosition of class  EmberOgre::Terrain::BasePointUserObject */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_BasePointUserObject_getPosition00
static int tolua_EmberOgre_EmberOgre_Terrain_BasePointUserObject_getPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Terrain::BasePointUserObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Terrain::BasePointUserObject* self = (const EmberOgre::Terrain::BasePointUserObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPosition'",NULL);
#endif
  {
   const EmberOgre::TerrainPosition& tolua_ret = (const EmberOgre::TerrainPosition&)  self->getPosition();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const EmberOgre::TerrainPosition");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getHeight of class  EmberOgre::Terrain::BasePointUserObject */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_BasePointUserObject_getHeight00
static int tolua_EmberOgre_EmberOgre_Terrain_BasePointUserObject_getHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Terrain::BasePointUserObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Terrain::BasePointUserObject* self = (const EmberOgre::Terrain::BasePointUserObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getHeight'",NULL);
#endif
  {
   float tolua_ret = (float)  self->getHeight();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getHeight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: translate of class  EmberOgre::Terrain::BasePointUserObject */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_BasePointUserObject_translate00
static int tolua_EmberOgre_EmberOgre_Terrain_BasePointUserObject_translate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Terrain::BasePointUserObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Terrain::BasePointUserObject* self = (EmberOgre::Terrain::BasePointUserObject*)  tolua_tousertype(tolua_S,1,0);
  float verticalMovement = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'translate'",NULL);
#endif
  {
   self->translate(verticalMovement);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'translate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventUpdatedPosition of class  EmberOgre::Terrain::BasePointUserObject */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__Terrain__BasePointUserObject_EventUpdatedPosition
static int tolua_get_EmberOgre__Terrain__BasePointUserObject_EventUpdatedPosition(lua_State* tolua_S)
{
  EmberOgre::Terrain::BasePointUserObject* self = (EmberOgre::Terrain::BasePointUserObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventUpdatedPosition'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventUpdatedPosition,"sigc::signal<void>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventUpdatedPosition of class  EmberOgre::Terrain::BasePointUserObject */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__Terrain__BasePointUserObject_EventUpdatedPosition
static int tolua_set_EmberOgre__Terrain__BasePointUserObject_EventUpdatedPosition(lua_State* tolua_S)
{
  EmberOgre::Terrain::BasePointUserObject* self = (EmberOgre::Terrain::BasePointUserObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventUpdatedPosition'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventUpdatedPosition = *((sigc::signal<void>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMovements of class  EmberOgre::Terrain::TerrainEditAction */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainEditAction_getMovements00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainEditAction_getMovements00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Terrain::TerrainEditAction",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Terrain::TerrainEditAction* self = (const EmberOgre::Terrain::TerrainEditAction*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMovements'",NULL);
#endif
  {
    const std::vector<EmberOgre::Terrain::TerrainEditBasePointMovement>& tolua_ret = (  const std::vector<EmberOgre::Terrain::TerrainEditBasePointMovement>&)  self->getMovements();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const std::vector<EmberOgre::Terrain::TerrainEditBasePointMovement>");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMovements'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMovements of class  EmberOgre::Terrain::TerrainEditAction */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainEditAction_getMovements01
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainEditAction_getMovements01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Terrain::TerrainEditAction",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  EmberOgre::Terrain::TerrainEditAction* self = (EmberOgre::Terrain::TerrainEditAction*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMovements'",NULL);
#endif
  {
    std::vector<EmberOgre::Terrain::TerrainEditBasePointMovement>& tolua_ret = (  std::vector<EmberOgre::Terrain::TerrainEditBasePointMovement>&)  self->getMovements();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"std::vector<EmberOgre::Terrain::TerrainEditBasePointMovement>");
  }
 }
 return 1;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_Terrain_TerrainEditAction_getMovements00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVerticalMovement of class  EmberOgre::Terrain::TerrainEditBasePointMovement */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainEditBasePointMovement_getVerticalMovement00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainEditBasePointMovement_getVerticalMovement00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Terrain::TerrainEditBasePointMovement",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Terrain::TerrainEditBasePointMovement* self = (const EmberOgre::Terrain::TerrainEditBasePointMovement*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVerticalMovement'",NULL);
#endif
  {
   float tolua_ret = (float)  self->getVerticalMovement();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVerticalMovement'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPosition of class  EmberOgre::Terrain::TerrainEditBasePointMovement */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainEditBasePointMovement_getPosition00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainEditBasePointMovement_getPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Terrain::TerrainEditBasePointMovement",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Terrain::TerrainEditBasePointMovement* self = (const EmberOgre::Terrain::TerrainEditBasePointMovement*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPosition'",NULL);
#endif
  {
   const EmberOgre::TerrainPosition& tolua_ret = (const EmberOgre::TerrainPosition&)  self->getPosition();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const EmberOgre::TerrainPosition");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::Terrain::TerrainEditor */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_new00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Terrain::TerrainEditor",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   EmberOgre::Terrain::TerrainEditor* tolua_ret = (EmberOgre::Terrain::TerrainEditor*)  new EmberOgre::Terrain::TerrainEditor();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Terrain::TerrainEditor");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::Terrain::TerrainEditor */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_new00_local
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Terrain::TerrainEditor",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   EmberOgre::Terrain::TerrainEditor* tolua_ret = (EmberOgre::Terrain::TerrainEditor*)  new EmberOgre::Terrain::TerrainEditor();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Terrain::TerrainEditor");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  EmberOgre::Terrain::TerrainEditor */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_delete00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Terrain::TerrainEditor",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Terrain::TerrainEditor* self = (EmberOgre::Terrain::TerrainEditor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: showOverlay of class  EmberOgre::Terrain::TerrainEditor */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_showOverlay00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_showOverlay00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Terrain::TerrainEditor",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Terrain::TerrainEditor* self = (EmberOgre::Terrain::TerrainEditor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'showOverlay'",NULL);
#endif
  {
   self->showOverlay();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'showOverlay'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: hideOverlay of class  EmberOgre::Terrain::TerrainEditor */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_hideOverlay00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_hideOverlay00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Terrain::TerrainEditor",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Terrain::TerrainEditor* self = (EmberOgre::Terrain::TerrainEditor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hideOverlay'",NULL);
#endif
  {
   self->hideOverlay();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'hideOverlay'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createOverlay of class  EmberOgre::Terrain::TerrainEditor */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_createOverlay00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_createOverlay00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Terrain::TerrainEditor",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Terrain::TerrainEditor* self = (EmberOgre::Terrain::TerrainEditor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createOverlay'",NULL);
#endif
  {
   self->createOverlay();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createOverlay'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isOverlayShown of class  EmberOgre::Terrain::TerrainEditor */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_isOverlayShown00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_isOverlayShown00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Terrain::TerrainEditor",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Terrain::TerrainEditor* self = (const EmberOgre::Terrain::TerrainEditor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isOverlayShown'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isOverlayShown();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isOverlayShown'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: commitAction of class  EmberOgre::Terrain::TerrainEditor */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_commitAction00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_commitAction00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Terrain::TerrainEditor",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const EmberOgre::Terrain::TerrainEditAction",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Terrain::TerrainEditor* self = (EmberOgre::Terrain::TerrainEditor*)  tolua_tousertype(tolua_S,1,0);
  const EmberOgre::Terrain::TerrainEditAction* action = ((const EmberOgre::Terrain::TerrainEditAction*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'commitAction'",NULL);
#endif
  {
   self->commitAction(*action);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'commitAction'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventPickedBasePoint of class  EmberOgre::Terrain::TerrainEditor */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__Terrain__TerrainEditor_EventPickedBasePoint
static int tolua_get_EmberOgre__Terrain__TerrainEditor_EventPickedBasePoint(lua_State* tolua_S)
{
  EmberOgre::Terrain::TerrainEditor* self = (EmberOgre::Terrain::TerrainEditor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventPickedBasePoint'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventPickedBasePoint,"sigc::signal<void,EmberOgre::Terrain::BasePointUserObject*>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventPickedBasePoint of class  EmberOgre::Terrain::TerrainEditor */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__Terrain__TerrainEditor_EventPickedBasePoint
static int tolua_set_EmberOgre__Terrain__TerrainEditor_EventPickedBasePoint(lua_State* tolua_S)
{
  EmberOgre::Terrain::TerrainEditor* self = (EmberOgre::Terrain::TerrainEditor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventPickedBasePoint'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,EmberOgre::Terrain::BasePointUserObject*>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventPickedBasePoint = *((sigc::signal<void,EmberOgre::Terrain::BasePointUserObject*>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventActionCreated of class  EmberOgre::Terrain::TerrainEditor */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__Terrain__TerrainEditor_EventActionCreated
static int tolua_get_EmberOgre__Terrain__TerrainEditor_EventActionCreated(lua_State* tolua_S)
{
  EmberOgre::Terrain::TerrainEditor* self = (EmberOgre::Terrain::TerrainEditor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventActionCreated'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventActionCreated,"sigc::signal<void,const EmberOgre::Terrain::TerrainEditAction*>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventActionCreated of class  EmberOgre::Terrain::TerrainEditor */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__Terrain__TerrainEditor_EventActionCreated
static int tolua_set_EmberOgre__Terrain__TerrainEditor_EventActionCreated(lua_State* tolua_S)
{
  EmberOgre::Terrain::TerrainEditor* self = (EmberOgre::Terrain::TerrainEditor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventActionCreated'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,const EmberOgre::Terrain::TerrainEditAction*>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventActionCreated = *((sigc::signal<void,const EmberOgre::Terrain::TerrainEditAction*>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventSelectedBasePointUpdatedPosition of class  EmberOgre::Terrain::TerrainEditor */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__Terrain__TerrainEditor_EventSelectedBasePointUpdatedPosition
static int tolua_get_EmberOgre__Terrain__TerrainEditor_EventSelectedBasePointUpdatedPosition(lua_State* tolua_S)
{
  EmberOgre::Terrain::TerrainEditor* self = (EmberOgre::Terrain::TerrainEditor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventSelectedBasePointUpdatedPosition'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventSelectedBasePointUpdatedPosition,"sigc::signal<void,EmberOgre::Terrain::BasePointUserObject*>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventSelectedBasePointUpdatedPosition of class  EmberOgre::Terrain::TerrainEditor */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__Terrain__TerrainEditor_EventSelectedBasePointUpdatedPosition
static int tolua_set_EmberOgre__Terrain__TerrainEditor_EventSelectedBasePointUpdatedPosition(lua_State* tolua_S)
{
  EmberOgre::Terrain::TerrainEditor* self = (EmberOgre::Terrain::TerrainEditor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventSelectedBasePointUpdatedPosition'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,EmberOgre::Terrain::BasePointUserObject*>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventSelectedBasePointUpdatedPosition = *((sigc::signal<void,EmberOgre::Terrain::BasePointUserObject*>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getCurrentBasePointUserObject of class  EmberOgre::Terrain::TerrainEditor */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_getCurrentBasePointUserObject00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_getCurrentBasePointUserObject00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Terrain::TerrainEditor",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Terrain::TerrainEditor* self = (const EmberOgre::Terrain::TerrainEditor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCurrentBasePointUserObject'",NULL);
#endif
  {
   EmberOgre::Terrain::BasePointUserObject* tolua_ret = (EmberOgre::Terrain::BasePointUserObject*)  self->getCurrentBasePointUserObject();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Terrain::BasePointUserObject");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getCurrentBasePointUserObject'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: sendChangesToServer of class  EmberOgre::Terrain::TerrainEditor */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_sendChangesToServer00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_sendChangesToServer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Terrain::TerrainEditor",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Terrain::TerrainEditor* self = (EmberOgre::Terrain::TerrainEditor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'sendChangesToServer'",NULL);
#endif
  {
   self->sendChangesToServer();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sendChangesToServer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createAction of class  EmberOgre::Terrain::TerrainEditor */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_createAction00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_createAction00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Terrain::TerrainEditor",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Terrain::TerrainEditor* self = (EmberOgre::Terrain::TerrainEditor*)  tolua_tousertype(tolua_S,1,0);
  bool alsoCommit = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createAction'",NULL);
#endif
  {
   self->createAction(alsoCommit);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createAction'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: undoLastAction of class  EmberOgre::Terrain::TerrainEditor */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_undoLastAction00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_undoLastAction00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Terrain::TerrainEditor",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Terrain::TerrainEditor* self = (EmberOgre::Terrain::TerrainEditor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'undoLastAction'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->undoLastAction();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'undoLastAction'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: redoAction of class  EmberOgre::Terrain::TerrainEditor */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_redoAction00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_redoAction00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Terrain::TerrainEditor",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Terrain::TerrainEditor* self = (EmberOgre::Terrain::TerrainEditor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'redoAction'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->redoAction();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'redoAction'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: AnimatedEntities of class  MotionManagerInfo */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__MotionManager__MotionManagerInfo_AnimatedEntities
static int tolua_get_EmberOgre__MotionManager__MotionManagerInfo_AnimatedEntities(lua_State* tolua_S)
{
  EmberOgre::MotionManager::MotionManagerInfo* self = (EmberOgre::MotionManager::MotionManagerInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'AnimatedEntities'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->AnimatedEntities);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: AnimatedEntities of class  MotionManagerInfo */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__MotionManager__MotionManagerInfo_AnimatedEntities
static int tolua_set_EmberOgre__MotionManager__MotionManagerInfo_AnimatedEntities(lua_State* tolua_S)
{
  EmberOgre::MotionManager::MotionManagerInfo* self = (EmberOgre::MotionManager::MotionManagerInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'AnimatedEntities'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->AnimatedEntities = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: MovingEntities of class  MotionManagerInfo */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__MotionManager__MotionManagerInfo_MovingEntities
static int tolua_get_EmberOgre__MotionManager__MotionManagerInfo_MovingEntities(lua_State* tolua_S)
{
  EmberOgre::MotionManager::MotionManagerInfo* self = (EmberOgre::MotionManager::MotionManagerInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'MovingEntities'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->MovingEntities);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: MovingEntities of class  MotionManagerInfo */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__MotionManager__MotionManagerInfo_MovingEntities
static int tolua_set_EmberOgre__MotionManager__MotionManagerInfo_MovingEntities(lua_State* tolua_S)
{
  EmberOgre::MotionManager::MotionManagerInfo* self = (EmberOgre::MotionManager::MotionManagerInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'MovingEntities'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->MovingEntities = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Animations of class  MotionManagerInfo */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__MotionManager__MotionManagerInfo_Animations
static int tolua_get_EmberOgre__MotionManager__MotionManagerInfo_Animations(lua_State* tolua_S)
{
  EmberOgre::MotionManager::MotionManagerInfo* self = (EmberOgre::MotionManager::MotionManagerInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Animations'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->Animations);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Animations of class  MotionManagerInfo */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__MotionManager__MotionManagerInfo_Animations
static int tolua_set_EmberOgre__MotionManager__MotionManagerInfo_Animations(lua_State* tolua_S)
{
  EmberOgre::MotionManager::MotionManagerInfo* self = (EmberOgre::MotionManager::MotionManagerInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Animations'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Animations = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSingleton of class  EmberOgre::MotionManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_MotionManager_getSingleton00
static int tolua_EmberOgre_EmberOgre_MotionManager_getSingleton00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::MotionManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   EmberOgre::MotionManager& tolua_ret = (EmberOgre::MotionManager&)  EmberOgre::MotionManager::getSingleton();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"EmberOgre::MotionManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSingleton'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addEntity of class  EmberOgre::MotionManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_MotionManager_addEntity00
static int tolua_EmberOgre_EmberOgre_MotionManager_addEntity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::MotionManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"EmberOgre::EmberEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::MotionManager* self = (EmberOgre::MotionManager*)  tolua_tousertype(tolua_S,1,0);
  EmberOgre::EmberEntity* entity = ((EmberOgre::EmberEntity*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addEntity'",NULL);
#endif
  {
   self->addEntity(entity);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addEntity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeEntity of class  EmberOgre::MotionManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_MotionManager_removeEntity00
static int tolua_EmberOgre_EmberOgre_MotionManager_removeEntity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::MotionManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"EmberOgre::EmberEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::MotionManager* self = (EmberOgre::MotionManager*)  tolua_tousertype(tolua_S,1,0);
  EmberOgre::EmberEntity* entity = ((EmberOgre::EmberEntity*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeEntity'",NULL);
#endif
  {
   self->removeEntity(entity);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeEntity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addAnimation of class  EmberOgre::MotionManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_MotionManager_addAnimation00
static int tolua_EmberOgre_EmberOgre_MotionManager_addAnimation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::MotionManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::AnimationState",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::MotionManager* self = (EmberOgre::MotionManager*)  tolua_tousertype(tolua_S,1,0);
  Ogre::AnimationState* animationState = ((Ogre::AnimationState*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addAnimation'",NULL);
#endif
  {
   self->addAnimation(animationState);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addAnimation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeAnimation of class  EmberOgre::MotionManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_MotionManager_removeAnimation00
static int tolua_EmberOgre_EmberOgre_MotionManager_removeAnimation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::MotionManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::AnimationState",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::MotionManager* self = (EmberOgre::MotionManager*)  tolua_tousertype(tolua_S,1,0);
  Ogre::AnimationState* animationState = ((Ogre::AnimationState*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeAnimation'",NULL);
#endif
  {
   self->removeAnimation(animationState);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeAnimation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: pauseAnimation of class  EmberOgre::MotionManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_MotionManager_pauseAnimation00
static int tolua_EmberOgre_EmberOgre_MotionManager_pauseAnimation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::MotionManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::AnimationState",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::MotionManager* self = (EmberOgre::MotionManager*)  tolua_tousertype(tolua_S,1,0);
  Ogre::AnimationState* animationState = ((Ogre::AnimationState*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'pauseAnimation'",NULL);
#endif
  {
   self->pauseAnimation(animationState);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'pauseAnimation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: unpauseAnimation of class  EmberOgre::MotionManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_MotionManager_unpauseAnimation00
static int tolua_EmberOgre_EmberOgre_MotionManager_unpauseAnimation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::MotionManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::AnimationState",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::MotionManager* self = (EmberOgre::MotionManager*)  tolua_tousertype(tolua_S,1,0);
  Ogre::AnimationState* animationState = ((Ogre::AnimationState*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'unpauseAnimation'",NULL);
#endif
  {
   self->unpauseAnimation(animationState);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'unpauseAnimation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addAnimatedEntity of class  EmberOgre::MotionManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_MotionManager_addAnimatedEntity00
static int tolua_EmberOgre_EmberOgre_MotionManager_addAnimatedEntity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::MotionManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"EmberOgre::EmberPhysicalEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::MotionManager* self = (EmberOgre::MotionManager*)  tolua_tousertype(tolua_S,1,0);
  EmberOgre::EmberPhysicalEntity* entity = ((EmberOgre::EmberPhysicalEntity*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addAnimatedEntity'",NULL);
#endif
  {
   self->addAnimatedEntity(entity);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addAnimatedEntity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeAnimatedEntity of class  EmberOgre::MotionManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_MotionManager_removeAnimatedEntity00
static int tolua_EmberOgre_EmberOgre_MotionManager_removeAnimatedEntity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::MotionManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"EmberOgre::EmberPhysicalEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::MotionManager* self = (EmberOgre::MotionManager*)  tolua_tousertype(tolua_S,1,0);
  EmberOgre::EmberPhysicalEntity* entity = ((EmberOgre::EmberPhysicalEntity*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeAnimatedEntity'",NULL);
#endif
  {
   self->removeAnimatedEntity(entity);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeAnimatedEntity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getInfo of class  EmberOgre::MotionManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_MotionManager_getInfo00
static int tolua_EmberOgre_EmberOgre_MotionManager_getInfo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::MotionManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::MotionManager* self = (const EmberOgre::MotionManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getInfo'",NULL);
#endif
  {
   const EmberOgre::MotionManager::MotionManagerInfo& tolua_ret = (const EmberOgre::MotionManager::MotionManagerInfo&)  self->getInfo();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const EmberOgre::MotionManager::MotionManagerInfo");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInfo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: show of class  EmberOgre::Gui::Widget */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_Widget_show00
static int tolua_EmberOgre_EmberOgre_Gui_Widget_show00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Widget",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Widget* self = (EmberOgre::Gui::Widget*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'show'",NULL);
#endif
  {
   self->show();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'show'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: hide of class  EmberOgre::Gui::Widget */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_Widget_hide00
static int tolua_EmberOgre_EmberOgre_Gui_Widget_hide00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Widget",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Widget* self = (EmberOgre::Gui::Widget*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hide'",NULL);
#endif
  {
   self->hide();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'hide'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getWindow of class  EmberOgre::Gui::Widget */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_Widget_getWindow00
static int tolua_EmberOgre_EmberOgre_Gui_Widget_getWindow00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Widget",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Widget* self = (EmberOgre::Gui::Widget*)  tolua_tousertype(tolua_S,1,0);
  const std::string windowName = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getWindow'",NULL);
#endif
  {
   CEGUI::Window* tolua_ret = (CEGUI::Window*)  self->getWindow(windowName);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Window");
   tolua_pushcppstring(tolua_S,(const char*)windowName);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getWindow'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: enableCloseButton of class  EmberOgre::Gui::Widget */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_Widget_enableCloseButton00
static int tolua_EmberOgre_EmberOgre_Gui_Widget_enableCloseButton00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Widget",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Widget* self = (EmberOgre::Gui::Widget*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'enableCloseButton'",NULL);
#endif
  {
   self->enableCloseButton();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'enableCloseButton'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: registerConsoleVisibilityToggleCommand of class  EmberOgre::Gui::Widget */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_Widget_registerConsoleVisibilityToggleCommand00
static int tolua_EmberOgre_EmberOgre_Gui_Widget_registerConsoleVisibilityToggleCommand00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Widget",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Widget* self = (EmberOgre::Gui::Widget*)  tolua_tousertype(tolua_S,1,0);
  const std::string commandSuffix = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'registerConsoleVisibilityToggleCommand'",NULL);
#endif
  {
   self->registerConsoleVisibilityToggleCommand(commandSuffix);
   tolua_pushcppstring(tolua_S,(const char*)commandSuffix);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'registerConsoleVisibilityToggleCommand'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMainSheet of class  EmberOgre::Gui::Widget */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_Widget_getMainSheet00
static int tolua_EmberOgre_EmberOgre_Gui_Widget_getMainSheet00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Widget",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Widget* self = (EmberOgre::Gui::Widget*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMainSheet'",NULL);
#endif
  {
   CEGUI::Window* tolua_ret = (CEGUI::Window*)  self->getMainSheet();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Window");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMainSheet'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMainWindow of class  EmberOgre::Gui::Widget */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_Widget_getMainWindow00
static int tolua_EmberOgre_EmberOgre_Gui_Widget_getMainWindow00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Widget",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Widget* self = (EmberOgre::Gui::Widget*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMainWindow'",NULL);
#endif
  {
   CEGUI::Window* tolua_ret = (CEGUI::Window*)  self->getMainWindow();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Window");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMainWindow'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createWindow of class  EmberOgre::Gui::Widget */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_Widget_createWindow00
static int tolua_EmberOgre_EmberOgre_Gui_Widget_createWindow00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Widget",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Widget* self = (EmberOgre::Gui::Widget*)  tolua_tousertype(tolua_S,1,0);
  const std::string windowType = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createWindow'",NULL);
#endif
  {
   CEGUI::Window* tolua_ret = (CEGUI::Window*)  self->createWindow(windowType);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Window");
   tolua_pushcppstring(tolua_S,(const char*)windowType);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createWindow'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createWindow of class  EmberOgre::Gui::Widget */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_Widget_createWindow01
static int tolua_EmberOgre_EmberOgre_Gui_Widget_createWindow01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Widget",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  EmberOgre::Gui::Widget* self = (EmberOgre::Gui::Widget*)  tolua_tousertype(tolua_S,1,0);
  const std::string windowType = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string windowName = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createWindow'",NULL);
#endif
  {
   CEGUI::Window* tolua_ret = (CEGUI::Window*)  self->createWindow(windowType,windowName);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Window");
   tolua_pushcppstring(tolua_S,(const char*)windowType);
   tolua_pushcppstring(tolua_S,(const char*)windowName);
  }
 }
 return 3;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_Gui_Widget_createWindow00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: loadMainSheet of class  EmberOgre::Gui::Widget */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_Widget_loadMainSheet00
static int tolua_EmberOgre_EmberOgre_Gui_Widget_loadMainSheet00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Widget",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Widget* self = (EmberOgre::Gui::Widget*)  tolua_tousertype(tolua_S,1,0);
  const std::string filename = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string prefix = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'loadMainSheet'",NULL);
#endif
  {
   CEGUI::Window* tolua_ret = (CEGUI::Window*)  self->loadMainSheet(filename,prefix);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Window");
   tolua_pushcppstring(tolua_S,(const char*)filename);
   tolua_pushcppstring(tolua_S,(const char*)prefix);
  }
 }
 return 3;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'loadMainSheet'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPrefix of class  EmberOgre::Gui::Widget */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_Widget_getPrefix00
static int tolua_EmberOgre_EmberOgre_Gui_Widget_getPrefix00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Gui::Widget",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Gui::Widget* self = (const EmberOgre::Gui::Widget*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPrefix'",NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getPrefix();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPrefix'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getIsActiveWindowOpaque of class  EmberOgre::Gui::Widget */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_Widget_getIsActiveWindowOpaque00
static int tolua_EmberOgre_EmberOgre_Gui_Widget_getIsActiveWindowOpaque00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Gui::Widget",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Gui::Widget* self = (const EmberOgre::Gui::Widget*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getIsActiveWindowOpaque'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->getIsActiveWindowOpaque();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getIsActiveWindowOpaque'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setIsActiveWindowOpaque of class  EmberOgre::Gui::Widget */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_Widget_setIsActiveWindowOpaque00
static int tolua_EmberOgre_EmberOgre_Gui_Widget_setIsActiveWindowOpaque00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Widget",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Widget* self = (EmberOgre::Gui::Widget*)  tolua_tousertype(tolua_S,1,0);
  bool isOpaque = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setIsActiveWindowOpaque'",NULL);
#endif
  {
   self->setIsActiveWindowOpaque(isOpaque);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setIsActiveWindowOpaque'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getDefaultScheme of class  EmberOgre::Gui::Widget */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_Widget_getDefaultScheme00
static int tolua_EmberOgre_EmberOgre_Gui_Widget_getDefaultScheme00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Gui::Widget",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Gui::Widget* self = (const EmberOgre::Gui::Widget*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getDefaultScheme'",NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getDefaultScheme();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getDefaultScheme'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addTabbableWindow of class  EmberOgre::Gui::Widget */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_Widget_addTabbableWindow00
static int tolua_EmberOgre_EmberOgre_Gui_Widget_addTabbableWindow00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Widget",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Widget* self = (EmberOgre::Gui::Widget*)  tolua_tousertype(tolua_S,1,0);
  CEGUI::Window* window = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addTabbableWindow'",NULL);
#endif
  {
   self->addTabbableWindow(window);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addTabbableWindow'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: closeTabGroup of class  EmberOgre::Gui::Widget */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_Widget_closeTabGroup00
static int tolua_EmberOgre_EmberOgre_Gui_Widget_closeTabGroup00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Widget",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Widget* self = (EmberOgre::Gui::Widget*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'closeTabGroup'",NULL);
#endif
  {
   self->closeTabGroup();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'closeTabGroup'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventFrameStarted of class  EmberOgre::Gui::Widget */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__Gui__Widget_EventFrameStarted
static int tolua_get_EmberOgre__Gui__Widget_EventFrameStarted(lua_State* tolua_S)
{
  EmberOgre::Gui::Widget* self = (EmberOgre::Gui::Widget*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventFrameStarted'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventFrameStarted,"sigc::signal<void,float>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventFrameStarted of class  EmberOgre::Gui::Widget */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__Gui__Widget_EventFrameStarted
static int tolua_set_EmberOgre__Gui__Widget_EventFrameStarted(lua_State* tolua_S)
{
  EmberOgre::Gui::Widget* self = (EmberOgre::Gui::Widget*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventFrameStarted'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,float>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventFrameStarted = *((sigc::signal<void,float>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::Gui::IconBase */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_IconBase_new00
static int tolua_EmberOgre_EmberOgre_Gui_IconBase_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::IconBase",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const CEGUI::Image",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"const CEGUI::Image",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"const CEGUI::Image",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,6,"const CEGUI::Image",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const CEGUI::Image* background = ((const CEGUI::Image*)  tolua_tousertype(tolua_S,3,0));
  const CEGUI::Image* foreground = ((const CEGUI::Image*)  tolua_tousertype(tolua_S,4,0));
  const CEGUI::Image* borderInactive = ((const CEGUI::Image*)  tolua_tousertype(tolua_S,5,0));
  const CEGUI::Image* borderActive = ((const CEGUI::Image*)  tolua_tousertype(tolua_S,6,0));
  {
   EmberOgre::Gui::IconBase* tolua_ret = (EmberOgre::Gui::IconBase*)  new EmberOgre::Gui::IconBase(name,background,foreground,borderInactive,borderActive);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::IconBase");
   tolua_pushcppstring(tolua_S,(const char*)name);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::Gui::IconBase */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_IconBase_new00_local
static int tolua_EmberOgre_EmberOgre_Gui_IconBase_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::IconBase",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const CEGUI::Image",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"const CEGUI::Image",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"const CEGUI::Image",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,6,"const CEGUI::Image",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const CEGUI::Image* background = ((const CEGUI::Image*)  tolua_tousertype(tolua_S,3,0));
  const CEGUI::Image* foreground = ((const CEGUI::Image*)  tolua_tousertype(tolua_S,4,0));
  const CEGUI::Image* borderInactive = ((const CEGUI::Image*)  tolua_tousertype(tolua_S,5,0));
  const CEGUI::Image* borderActive = ((const CEGUI::Image*)  tolua_tousertype(tolua_S,6,0));
  {
   EmberOgre::Gui::IconBase* tolua_ret = (EmberOgre::Gui::IconBase*)  new EmberOgre::Gui::IconBase(name,background,foreground,borderInactive,borderActive);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Gui::IconBase");
   tolua_pushcppstring(tolua_S,(const char*)name);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  EmberOgre::Gui::IconBase */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_IconBase_delete00
static int tolua_EmberOgre_EmberOgre_Gui_IconBase_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::IconBase",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::IconBase* self = (EmberOgre::Gui::IconBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getContainer of class  EmberOgre::Gui::IconBase */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_IconBase_getContainer00
static int tolua_EmberOgre_EmberOgre_Gui_IconBase_getContainer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::IconBase",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::IconBase* self = (EmberOgre::Gui::IconBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getContainer'",NULL);
#endif
  {
   CEGUI::Window* tolua_ret = (CEGUI::Window*)  self->getContainer();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Window");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getContainer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getButton of class  EmberOgre::Gui::IconBase */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_IconBase_getButton00
static int tolua_EmberOgre_EmberOgre_Gui_IconBase_getButton00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::IconBase",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::IconBase* self = (EmberOgre::Gui::IconBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getButton'",NULL);
#endif
  {
   CEGUI::PushButton* tolua_ret = (CEGUI::PushButton*)  self->getButton();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::PushButton");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getButton'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setForeground of class  EmberOgre::Gui::IconBase */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_IconBase_setForeground00
static int tolua_EmberOgre_EmberOgre_Gui_IconBase_setForeground00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::IconBase",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const CEGUI::Image",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::IconBase* self = (EmberOgre::Gui::IconBase*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::Image* image = ((const CEGUI::Image*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setForeground'",NULL);
#endif
  {
   self->setForeground(image);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setForeground'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: loadImageFromImageset of class  EmberOgre::Gui::IconBase */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_IconBase_loadImageFromImageset00
static int tolua_EmberOgre_EmberOgre_Gui_IconBase_loadImageFromImageset00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::IconBase",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::string imagesetName = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string image = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  {
   const CEGUI::Image* tolua_ret = (const CEGUI::Image*)  EmberOgre::Gui::IconBase::loadImageFromImageset(imagesetName,image);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"const CEGUI::Image");
   tolua_pushcppstring(tolua_S,(const char*)imagesetName);
   tolua_pushcppstring(tolua_S,(const char*)image);
  }
 }
 return 3;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'loadImageFromImageset'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::Gui::IconBar */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_IconBar_new00
static int tolua_EmberOgre_EmberOgre_Gui_IconBar_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::IconBar",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  {
   EmberOgre::Gui::IconBar* tolua_ret = (EmberOgre::Gui::IconBar*)  new EmberOgre::Gui::IconBar(name);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::IconBar");
   tolua_pushcppstring(tolua_S,(const char*)name);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::Gui::IconBar */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_IconBar_new00_local
static int tolua_EmberOgre_EmberOgre_Gui_IconBar_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::IconBar",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  {
   EmberOgre::Gui::IconBar* tolua_ret = (EmberOgre::Gui::IconBar*)  new EmberOgre::Gui::IconBar(name);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Gui::IconBar");
   tolua_pushcppstring(tolua_S,(const char*)name);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  EmberOgre::Gui::IconBar */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_IconBar_delete00
static int tolua_EmberOgre_EmberOgre_Gui_IconBar_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::IconBar",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::IconBar* self = (EmberOgre::Gui::IconBar*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addIcon of class  EmberOgre::Gui::IconBar */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_IconBar_addIcon00
static int tolua_EmberOgre_EmberOgre_Gui_IconBar_addIcon00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::IconBar",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"EmberOgre::Gui::IconBase",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::IconBar* self = (EmberOgre::Gui::IconBar*)  tolua_tousertype(tolua_S,1,0);
  EmberOgre::Gui::IconBase* iconBase = ((EmberOgre::Gui::IconBase*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addIcon'",NULL);
#endif
  {
   self->addIcon(iconBase);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addIcon'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeIcon of class  EmberOgre::Gui::IconBar */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_IconBar_removeIcon00
static int tolua_EmberOgre_EmberOgre_Gui_IconBar_removeIcon00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::IconBar",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"EmberOgre::Gui::IconBase",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::IconBar* self = (EmberOgre::Gui::IconBar*)  tolua_tousertype(tolua_S,1,0);
  EmberOgre::Gui::IconBase* iconBase = ((EmberOgre::Gui::IconBase*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeIcon'",NULL);
#endif
  {
   self->removeIcon(iconBase);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeIcon'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getWindow of class  EmberOgre::Gui::IconBar */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_IconBar_getWindow00
static int tolua_EmberOgre_EmberOgre_Gui_IconBar_getWindow00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::IconBar",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::IconBar* self = (EmberOgre::Gui::IconBar*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getWindow'",NULL);
#endif
  {
   CEGUI::Window* tolua_ret = (CEGUI::Window*)  self->getWindow();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Window");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getWindow'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setIconPadding of class  EmberOgre::Gui::IconBar */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_IconBar_setIconPadding00
static int tolua_EmberOgre_EmberOgre_Gui_IconBar_setIconPadding00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::IconBar",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::IconBar* self = (EmberOgre::Gui::IconBar*)  tolua_tousertype(tolua_S,1,0);
  int iconPadding = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setIconPadding'",NULL);
#endif
  {
   self->setIconPadding(iconPadding);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setIconPadding'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAbsoluteHeight of class  EmberOgre::Gui::IconBar */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_IconBar_getAbsoluteHeight00
static int tolua_EmberOgre_EmberOgre_Gui_IconBar_getAbsoluteHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::IconBar",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::IconBar* self = (EmberOgre::Gui::IconBar*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAbsoluteHeight'",NULL);
#endif
  {
   float tolua_ret = (float)  self->getAbsoluteHeight();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAbsoluteHeight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAbsoluteWidth of class  EmberOgre::Gui::IconBar */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_IconBar_getAbsoluteWidth00
static int tolua_EmberOgre_EmberOgre_Gui_IconBar_getAbsoluteWidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::IconBar",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::IconBar* self = (EmberOgre::Gui::IconBar*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAbsoluteWidth'",NULL);
#endif
  {
   float tolua_ret = (float)  self->getAbsoluteWidth();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAbsoluteWidth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::Gui::EntityEditor */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_EntityEditor_new00
static int tolua_EmberOgre_EmberOgre_Gui_EntityEditor_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::EntityEditor",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Eris::Entity",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"EmberOgre::Gui::Adapters::Atlas::MapAdapter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Eris::Entity* entity = ((Eris::Entity*)  tolua_tousertype(tolua_S,2,0));
  EmberOgre::Gui::Adapters::Atlas::MapAdapter* rootAdapter = ((EmberOgre::Gui::Adapters::Atlas::MapAdapter*)  tolua_tousertype(tolua_S,3,0));
  {
   EmberOgre::Gui::EntityEditor* tolua_ret = (EmberOgre::Gui::EntityEditor*)  new EmberOgre::Gui::EntityEditor(entity,rootAdapter);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::EntityEditor");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::Gui::EntityEditor */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_EntityEditor_new00_local
static int tolua_EmberOgre_EmberOgre_Gui_EntityEditor_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::EntityEditor",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Eris::Entity",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"EmberOgre::Gui::Adapters::Atlas::MapAdapter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Eris::Entity* entity = ((Eris::Entity*)  tolua_tousertype(tolua_S,2,0));
  EmberOgre::Gui::Adapters::Atlas::MapAdapter* rootAdapter = ((EmberOgre::Gui::Adapters::Atlas::MapAdapter*)  tolua_tousertype(tolua_S,3,0));
  {
   EmberOgre::Gui::EntityEditor* tolua_ret = (EmberOgre::Gui::EntityEditor*)  new EmberOgre::Gui::EntityEditor(entity,rootAdapter);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Gui::EntityEditor");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  EmberOgre::Gui::EntityEditor */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_EntityEditor_delete00
static int tolua_EmberOgre_EmberOgre_Gui_EntityEditor_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::EntityEditor",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::EntityEditor* self = (EmberOgre::Gui::EntityEditor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: submitChanges of class  EmberOgre::Gui::EntityEditor */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_EntityEditor_submitChanges00
static int tolua_EmberOgre_EmberOgre_Gui_EntityEditor_submitChanges00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::EntityEditor",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::EntityEditor* self = (EmberOgre::Gui::EntityEditor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'submitChanges'",NULL);
#endif
  {
   self->submitChanges();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'submitChanges'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createMapElement of class  EmberOgre::Gui::EntityEditor */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_EntityEditor_createMapElement00
static int tolua_EmberOgre_EmberOgre_Gui_EntityEditor_createMapElement00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::EntityEditor",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::EntityEditor* self = (EmberOgre::Gui::EntityEditor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createMapElement'",NULL);
#endif
  {
   Atlas::Message::Element tolua_ret = (Atlas::Message::Element)  self->createMapElement();
   {
#ifdef __cplusplus
    void* tolua_obj = new Atlas::Message::Element(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Atlas::Message::Element");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Atlas::Message::Element));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Atlas::Message::Element");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createMapElement'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createListElement of class  EmberOgre::Gui::EntityEditor */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_EntityEditor_createListElement00
static int tolua_EmberOgre_EmberOgre_Gui_EntityEditor_createListElement00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::EntityEditor",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::EntityEditor* self = (EmberOgre::Gui::EntityEditor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createListElement'",NULL);
#endif
  {
   Atlas::Message::Element tolua_ret = (Atlas::Message::Element)  self->createListElement();
   {
#ifdef __cplusplus
    void* tolua_obj = new Atlas::Message::Element(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Atlas::Message::Element");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Atlas::Message::Element));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Atlas::Message::Element");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createListElement'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createStringElement of class  EmberOgre::Gui::EntityEditor */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_EntityEditor_createStringElement00
static int tolua_EmberOgre_EmberOgre_Gui_EntityEditor_createStringElement00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::EntityEditor",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::EntityEditor* self = (EmberOgre::Gui::EntityEditor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createStringElement'",NULL);
#endif
  {
   Atlas::Message::Element tolua_ret = (Atlas::Message::Element)  self->createStringElement();
   {
#ifdef __cplusplus
    void* tolua_obj = new Atlas::Message::Element(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Atlas::Message::Element");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Atlas::Message::Element));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Atlas::Message::Element");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createStringElement'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createIntElement of class  EmberOgre::Gui::EntityEditor */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_EntityEditor_createIntElement00
static int tolua_EmberOgre_EmberOgre_Gui_EntityEditor_createIntElement00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::EntityEditor",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::EntityEditor* self = (EmberOgre::Gui::EntityEditor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createIntElement'",NULL);
#endif
  {
   Atlas::Message::Element tolua_ret = (Atlas::Message::Element)  self->createIntElement();
   {
#ifdef __cplusplus
    void* tolua_obj = new Atlas::Message::Element(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Atlas::Message::Element");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Atlas::Message::Element));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Atlas::Message::Element");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createIntElement'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createFloatElement of class  EmberOgre::Gui::EntityEditor */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_EntityEditor_createFloatElement00
static int tolua_EmberOgre_EmberOgre_Gui_EntityEditor_createFloatElement00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::EntityEditor",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::EntityEditor* self = (EmberOgre::Gui::EntityEditor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createFloatElement'",NULL);
#endif
  {
   Atlas::Message::Element tolua_ret = (Atlas::Message::Element)  self->createFloatElement();
   {
#ifdef __cplusplus
    void* tolua_obj = new Atlas::Message::Element(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Atlas::Message::Element");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Atlas::Message::Element));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Atlas::Message::Element");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createFloatElement'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createPosition2dElement of class  EmberOgre::Gui::EntityEditor */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_EntityEditor_createPosition2dElement00
static int tolua_EmberOgre_EmberOgre_Gui_EntityEditor_createPosition2dElement00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::EntityEditor",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::EntityEditor* self = (EmberOgre::Gui::EntityEditor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createPosition2dElement'",NULL);
#endif
  {
   Atlas::Message::Element tolua_ret = (Atlas::Message::Element)  self->createPosition2dElement();
   {
#ifdef __cplusplus
    void* tolua_obj = new Atlas::Message::Element(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Atlas::Message::Element");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Atlas::Message::Element));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Atlas::Message::Element");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createPosition2dElement'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::Gui::StackableContainer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_StackableContainer_new00
static int tolua_EmberOgre_EmberOgre_Gui_StackableContainer_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::StackableContainer",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* window = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::Gui::StackableContainer* tolua_ret = (EmberOgre::Gui::StackableContainer*)  new EmberOgre::Gui::StackableContainer(window);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::StackableContainer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::Gui::StackableContainer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_StackableContainer_new00_local
static int tolua_EmberOgre_EmberOgre_Gui_StackableContainer_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::StackableContainer",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* window = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::Gui::StackableContainer* tolua_ret = (EmberOgre::Gui::StackableContainer*)  new EmberOgre::Gui::StackableContainer(window);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Gui::StackableContainer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  EmberOgre::Gui::StackableContainer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_StackableContainer_delete00
static int tolua_EmberOgre_EmberOgre_Gui_StackableContainer_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::StackableContainer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::StackableContainer* self = (EmberOgre::Gui::StackableContainer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setInnerContainerWindow of class  EmberOgre::Gui::StackableContainer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_StackableContainer_setInnerContainerWindow00
static int tolua_EmberOgre_EmberOgre_Gui_StackableContainer_setInnerContainerWindow00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::StackableContainer",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::StackableContainer* self = (EmberOgre::Gui::StackableContainer*)  tolua_tousertype(tolua_S,1,0);
  CEGUI::Window* window = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setInnerContainerWindow'",NULL);
#endif
  {
   self->setInnerContainerWindow(window);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setInnerContainerWindow'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getWindow of class  EmberOgre::Gui::StackableContainer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_StackableContainer_getWindow00
static int tolua_EmberOgre_EmberOgre_Gui_StackableContainer_getWindow00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::StackableContainer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::StackableContainer* self = (EmberOgre::Gui::StackableContainer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getWindow'",NULL);
#endif
  {
   CEGUI::Window* tolua_ret = (CEGUI::Window*)  self->getWindow();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Window");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getWindow'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setPadding of class  EmberOgre::Gui::StackableContainer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_StackableContainer_setPadding00
static int tolua_EmberOgre_EmberOgre_Gui_StackableContainer_setPadding00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::StackableContainer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::StackableContainer* self = (EmberOgre::Gui::StackableContainer*)  tolua_tousertype(tolua_S,1,0);
  int padding = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPadding'",NULL);
#endif
  {
   self->setPadding(padding);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setPadding'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPadding of class  EmberOgre::Gui::StackableContainer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_StackableContainer_getPadding00
static int tolua_EmberOgre_EmberOgre_Gui_StackableContainer_getPadding00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Gui::StackableContainer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Gui::StackableContainer* self = (const EmberOgre::Gui::StackableContainer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPadding'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getPadding();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPadding'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAbsoluteHeight of class  EmberOgre::Gui::StackableContainer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_StackableContainer_getAbsoluteHeight00
static int tolua_EmberOgre_EmberOgre_Gui_StackableContainer_getAbsoluteHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::StackableContainer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::StackableContainer* self = (EmberOgre::Gui::StackableContainer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAbsoluteHeight'",NULL);
#endif
  {
   float tolua_ret = (float)  self->getAbsoluteHeight();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAbsoluteHeight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAbsoluteWidth of class  EmberOgre::Gui::StackableContainer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_StackableContainer_getAbsoluteWidth00
static int tolua_EmberOgre_EmberOgre_Gui_StackableContainer_getAbsoluteWidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::StackableContainer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::StackableContainer* self = (EmberOgre::Gui::StackableContainer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAbsoluteWidth'",NULL);
#endif
  {
   float tolua_ret = (float)  self->getAbsoluteWidth();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAbsoluteWidth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setFlowDirection of class  EmberOgre::Gui::StackableContainer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_StackableContainer_setFlowDirection00
static int tolua_EmberOgre_EmberOgre_Gui_StackableContainer_setFlowDirection00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::StackableContainer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::StackableContainer* self = (EmberOgre::Gui::StackableContainer*)  tolua_tousertype(tolua_S,1,0);
  EmberOgre::Gui::StackableContainer::FlowDirection flowDirection = ((EmberOgre::Gui::StackableContainer::FlowDirection) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setFlowDirection'",NULL);
#endif
  {
   self->setFlowDirection(flowDirection);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setFlowDirection'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getFlowDirection of class  EmberOgre::Gui::StackableContainer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_StackableContainer_getFlowDirection00
static int tolua_EmberOgre_EmberOgre_Gui_StackableContainer_getFlowDirection00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Gui::StackableContainer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Gui::StackableContainer* self = (const EmberOgre::Gui::StackableContainer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFlowDirection'",NULL);
#endif
  {
   EmberOgre::Gui::StackableContainer::FlowDirection tolua_ret = (EmberOgre::Gui::StackableContainer::FlowDirection)  self->getFlowDirection();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getFlowDirection'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: repositionWindows of class  EmberOgre::Gui::StackableContainer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_StackableContainer_repositionWindows00
static int tolua_EmberOgre_EmberOgre_Gui_StackableContainer_repositionWindows00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::StackableContainer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::StackableContainer* self = (EmberOgre::Gui::StackableContainer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'repositionWindows'",NULL);
#endif
  {
   self->repositionWindows();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'repositionWindows'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: disconnect of class  EmberOgre::Gui::StackableContainer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_StackableContainer_disconnect00
static int tolua_EmberOgre_EmberOgre_Gui_StackableContainer_disconnect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::StackableContainer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::StackableContainer* self = (EmberOgre::Gui::StackableContainer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'disconnect'",NULL);
#endif
  {
   self->disconnect();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'disconnect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::Gui::ConsoleAdapter */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_ConsoleAdapter_new00
static int tolua_EmberOgre_EmberOgre_Gui_ConsoleAdapter_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::ConsoleAdapter",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CEGUI::Editbox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CEGUI::Editbox* inputBox = ((CEGUI::Editbox*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::Gui::ConsoleAdapter* tolua_ret = (EmberOgre::Gui::ConsoleAdapter*)  new EmberOgre::Gui::ConsoleAdapter(inputBox);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::ConsoleAdapter");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::Gui::ConsoleAdapter */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_ConsoleAdapter_new00_local
static int tolua_EmberOgre_EmberOgre_Gui_ConsoleAdapter_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::ConsoleAdapter",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CEGUI::Editbox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CEGUI::Editbox* inputBox = ((CEGUI::Editbox*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::Gui::ConsoleAdapter* tolua_ret = (EmberOgre::Gui::ConsoleAdapter*)  new EmberOgre::Gui::ConsoleAdapter(inputBox);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Gui::ConsoleAdapter");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  EmberOgre::Gui::ConsoleAdapter */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_ConsoleAdapter_delete00
static int tolua_EmberOgre_EmberOgre_Gui_ConsoleAdapter_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::ConsoleAdapter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::ConsoleAdapter* self = (EmberOgre::Gui::ConsoleAdapter*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventCommandExecuted of class  EmberOgre::Gui::ConsoleAdapter */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__Gui__ConsoleAdapter_EventCommandExecuted
static int tolua_get_EmberOgre__Gui__ConsoleAdapter_EventCommandExecuted(lua_State* tolua_S)
{
  EmberOgre::Gui::ConsoleAdapter* self = (EmberOgre::Gui::ConsoleAdapter*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventCommandExecuted'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventCommandExecuted,"sigc::signal<void,const std::string&>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventCommandExecuted of class  EmberOgre::Gui::ConsoleAdapter */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__Gui__ConsoleAdapter_EventCommandExecuted
static int tolua_set_EmberOgre__Gui__ConsoleAdapter_EventCommandExecuted(lua_State* tolua_S)
{
  EmberOgre::Gui::ConsoleAdapter* self = (EmberOgre::Gui::ConsoleAdapter*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventCommandExecuted'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,const std::string&>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventCommandExecuted = *((sigc::signal<void,const std::string&>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: createColouredListItem of class  EmberOgre::Gui::ColouredListItem */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_ColouredListItem_new00
static int tolua_EmberOgre_EmberOgre_Gui_ColouredListItem_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::ColouredListItem",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::string text = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  {
   CEGUI::ListboxItem* tolua_ret = (CEGUI::ListboxItem*)  EmberOgre::Gui::ColouredListItem::createColouredListItem(text);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::ListboxItem");
   tolua_pushcppstring(tolua_S,(const char*)text);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createColouredListItem of class  EmberOgre::Gui::ColouredListItem */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_ColouredListItem_new01
static int tolua_EmberOgre_EmberOgre_Gui_ColouredListItem_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::ColouredListItem",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const std::string text = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  unsigned int item_id = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
  {
   CEGUI::ListboxItem* tolua_ret = (CEGUI::ListboxItem*)  EmberOgre::Gui::ColouredListItem::createColouredListItem(text,item_id);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::ListboxItem");
   tolua_pushcppstring(tolua_S,(const char*)text);
  }
 }
 return 2;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_Gui_ColouredListItem_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: createColouredListItem of class  EmberOgre::Gui::ColouredListItem */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_ColouredListItem_new02
static int tolua_EmberOgre_EmberOgre_Gui_ColouredListItem_new02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::ColouredListItem",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isuserdata(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const std::string text = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  unsigned int item_id = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
  void* item_data = ((void*)  tolua_touserdata(tolua_S,4,0));
  {
   CEGUI::ListboxItem* tolua_ret = (CEGUI::ListboxItem*)  EmberOgre::Gui::ColouredListItem::createColouredListItem(text,item_id,item_data);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::ListboxItem");
   tolua_pushcppstring(tolua_S,(const char*)text);
  }
 }
 return 2;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_Gui_ColouredListItem_new01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::Gui::TexturePair */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_TexturePair_new00
static int tolua_EmberOgre_EmberOgre_Gui_TexturePair_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::TexturePair",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::TexturePtr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const CEGUI::Image",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"CEGUI::Imageset",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::TexturePtr mOgreTexture = *((Ogre::TexturePtr*)  tolua_tousertype(tolua_S,2,0));
  const CEGUI::Image* mTextureImage = ((const CEGUI::Image*)  tolua_tousertype(tolua_S,3,0));
  CEGUI::Imageset* mTextureImageset = ((CEGUI::Imageset*)  tolua_tousertype(tolua_S,4,0));
  {
   EmberOgre::Gui::TexturePair* tolua_ret = (EmberOgre::Gui::TexturePair*)  new EmberOgre::Gui::TexturePair(mOgreTexture,mTextureImage,mTextureImageset);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::TexturePair");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::Gui::TexturePair */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_TexturePair_new00_local
static int tolua_EmberOgre_EmberOgre_Gui_TexturePair_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::TexturePair",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::TexturePtr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const CEGUI::Image",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"CEGUI::Imageset",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::TexturePtr mOgreTexture = *((Ogre::TexturePtr*)  tolua_tousertype(tolua_S,2,0));
  const CEGUI::Image* mTextureImage = ((const CEGUI::Image*)  tolua_tousertype(tolua_S,3,0));
  CEGUI::Imageset* mTextureImageset = ((CEGUI::Imageset*)  tolua_tousertype(tolua_S,4,0));
  {
   EmberOgre::Gui::TexturePair* tolua_ret = (EmberOgre::Gui::TexturePair*)  new EmberOgre::Gui::TexturePair(mOgreTexture,mTextureImage,mTextureImageset);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Gui::TexturePair");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::Gui::TexturePair */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_TexturePair_new01
static int tolua_EmberOgre_EmberOgre_Gui_TexturePair_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::TexturePair",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  {
   EmberOgre::Gui::TexturePair* tolua_ret = (EmberOgre::Gui::TexturePair*)  new EmberOgre::Gui::TexturePair();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::TexturePair");
  }
 }
 return 1;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_Gui_TexturePair_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::Gui::TexturePair */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_TexturePair_new01_local
static int tolua_EmberOgre_EmberOgre_Gui_TexturePair_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::TexturePair",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  {
   EmberOgre::Gui::TexturePair* tolua_ret = (EmberOgre::Gui::TexturePair*)  new EmberOgre::Gui::TexturePair();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Gui::TexturePair");
  }
 }
 return 1;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_Gui_TexturePair_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getOgreTexture of class  EmberOgre::Gui::TexturePair */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_TexturePair_getOgreTexture00
static int tolua_EmberOgre_EmberOgre_Gui_TexturePair_getOgreTexture00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Gui::TexturePair",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Gui::TexturePair* self = (const EmberOgre::Gui::TexturePair*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getOgreTexture'",NULL);
#endif
  {
   Ogre::TexturePtr tolua_ret = (Ogre::TexturePtr)  self->getOgreTexture();
   {
#ifdef __cplusplus
    void* tolua_obj = new Ogre::TexturePtr(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::TexturePtr");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::TexturePtr));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::TexturePtr");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getOgreTexture'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTextureImage of class  EmberOgre::Gui::TexturePair */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_TexturePair_getTextureImage00
static int tolua_EmberOgre_EmberOgre_Gui_TexturePair_getTextureImage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Gui::TexturePair",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Gui::TexturePair* self = (const EmberOgre::Gui::TexturePair*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTextureImage'",NULL);
#endif
  {
   const CEGUI::Image* tolua_ret = (const CEGUI::Image*)  self->getTextureImage();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"const CEGUI::Image");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTextureImage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTextureImageset of class  EmberOgre::Gui::TexturePair */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_TexturePair_getTextureImageset00
static int tolua_EmberOgre_EmberOgre_Gui_TexturePair_getTextureImageset00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Gui::TexturePair",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Gui::TexturePair* self = (const EmberOgre::Gui::TexturePair*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTextureImageset'",NULL);
#endif
  {
   CEGUI::Imageset* tolua_ret = (CEGUI::Imageset*)  self->getTextureImageset();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Imageset");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTextureImageset'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: hasData of class  EmberOgre::Gui::TexturePair */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_TexturePair_hasData00
static int tolua_EmberOgre_EmberOgre_Gui_TexturePair_hasData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::TexturePair",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::TexturePair* self = (EmberOgre::Gui::TexturePair*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hasData'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->hasData();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'hasData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::Gui::AssetsManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_AssetsManager_new00
static int tolua_EmberOgre_EmberOgre_Gui_AssetsManager_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::AssetsManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   EmberOgre::Gui::AssetsManager* tolua_ret = (EmberOgre::Gui::AssetsManager*)  new EmberOgre::Gui::AssetsManager();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::AssetsManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::Gui::AssetsManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_AssetsManager_new00_local
static int tolua_EmberOgre_EmberOgre_Gui_AssetsManager_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::AssetsManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   EmberOgre::Gui::AssetsManager* tolua_ret = (EmberOgre::Gui::AssetsManager*)  new EmberOgre::Gui::AssetsManager();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Gui::AssetsManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  EmberOgre::Gui::AssetsManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_AssetsManager_delete00
static int tolua_EmberOgre_EmberOgre_Gui_AssetsManager_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::AssetsManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::AssetsManager* self = (EmberOgre::Gui::AssetsManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: showTexture of class  EmberOgre::Gui::AssetsManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_AssetsManager_showTexture00
static int tolua_EmberOgre_EmberOgre_Gui_AssetsManager_showTexture00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::AssetsManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::AssetsManager* self = (EmberOgre::Gui::AssetsManager*)  tolua_tousertype(tolua_S,1,0);
  const std::string textureName = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'showTexture'",NULL);
#endif
  {
   EmberOgre::Gui::TexturePair tolua_ret = (EmberOgre::Gui::TexturePair)  self->showTexture(textureName);
   {
#ifdef __cplusplus
    void* tolua_obj = new EmberOgre::Gui::TexturePair(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"EmberOgre::Gui::TexturePair");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(EmberOgre::Gui::TexturePair));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"EmberOgre::Gui::TexturePair");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'showTexture'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createTextureImage of class  EmberOgre::Gui::AssetsManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_AssetsManager_createTextureImage00
static int tolua_EmberOgre_EmberOgre_Gui_AssetsManager_createTextureImage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::AssetsManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::TexturePtr",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::AssetsManager* self = (EmberOgre::Gui::AssetsManager*)  tolua_tousertype(tolua_S,1,0);
  Ogre::TexturePtr texturePtr = *((Ogre::TexturePtr*)  tolua_tousertype(tolua_S,2,0));
  const std::string imageSetName = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createTextureImage'",NULL);
#endif
  {
   EmberOgre::Gui::TexturePair tolua_ret = (EmberOgre::Gui::TexturePair)  self->createTextureImage(texturePtr,imageSetName);
   {
#ifdef __cplusplus
    void* tolua_obj = new EmberOgre::Gui::TexturePair(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"EmberOgre::Gui::TexturePair");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(EmberOgre::Gui::TexturePair));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"EmberOgre::Gui::TexturePair");
#endif
   }
   tolua_pushcppstring(tolua_S,(const char*)imageSetName);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createTextureImage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::Gui::Compass */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_Compass_new00
static int tolua_EmberOgre_EmberOgre_Gui_Compass_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::Compass",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"EmberOgre::Gui::ICompassImpl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::ICompassImpl* compassImpl = ((EmberOgre::Gui::ICompassImpl*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::Gui::Compass* tolua_ret = (EmberOgre::Gui::Compass*)  new EmberOgre::Gui::Compass(compassImpl);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::Compass");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::Gui::Compass */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_Compass_new00_local
static int tolua_EmberOgre_EmberOgre_Gui_Compass_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::Compass",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"EmberOgre::Gui::ICompassImpl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::ICompassImpl* compassImpl = ((EmberOgre::Gui::ICompassImpl*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::Gui::Compass* tolua_ret = (EmberOgre::Gui::Compass*)  new EmberOgre::Gui::Compass(compassImpl);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Gui::Compass");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  EmberOgre::Gui::Compass */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_Compass_delete00
static int tolua_EmberOgre_EmberOgre_Gui_Compass_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Compass",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Compass* self = (EmberOgre::Gui::Compass*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMap of class  EmberOgre::Gui::Compass */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_Compass_getMap00
static int tolua_EmberOgre_EmberOgre_Gui_Compass_getMap00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Compass",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Compass* self = (EmberOgre::Gui::Compass*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMap'",NULL);
#endif
  {
   EmberOgre::Terrain::Map& tolua_ret = (EmberOgre::Terrain::Map&)  self->getMap();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"EmberOgre::Terrain::Map");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMap'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: reposition of class  EmberOgre::Gui::Compass */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_Compass_reposition00
static int tolua_EmberOgre_EmberOgre_Gui_Compass_reposition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Compass",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Compass* self = (EmberOgre::Gui::Compass*)  tolua_tousertype(tolua_S,1,0);
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float y = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'reposition'",NULL);
#endif
  {
   self->reposition(x,y);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'reposition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: rotate of class  EmberOgre::Gui::Compass */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_Compass_rotate00
static int tolua_EmberOgre_EmberOgre_Gui_Compass_rotate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Compass",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Degree",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Compass* self = (EmberOgre::Gui::Compass*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Degree* degree = ((const Ogre::Degree*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'rotate'",NULL);
#endif
  {
   self->rotate(*degree);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'rotate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: rotate of class  EmberOgre::Gui::Compass */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_Compass_rotate01
static int tolua_EmberOgre_EmberOgre_Gui_Compass_rotate01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Compass",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Radian",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  EmberOgre::Gui::Compass* self = (EmberOgre::Gui::Compass*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Radian* radian = ((const Ogre::Radian*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'rotate'",NULL);
#endif
  {
   self->rotate(*radian);
  }
 }
 return 0;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_Gui_Compass_rotate00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::Gui::CEGUICompassImpl */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_CEGUICompassImpl_new00
static int tolua_EmberOgre_EmberOgre_Gui_CEGUICompassImpl_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::CEGUICompassImpl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   EmberOgre::Gui::CEGUICompassImpl* tolua_ret = (EmberOgre::Gui::CEGUICompassImpl*)  new EmberOgre::Gui::CEGUICompassImpl();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::CEGUICompassImpl");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::Gui::CEGUICompassImpl */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_CEGUICompassImpl_new00_local
static int tolua_EmberOgre_EmberOgre_Gui_CEGUICompassImpl_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::CEGUICompassImpl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   EmberOgre::Gui::CEGUICompassImpl* tolua_ret = (EmberOgre::Gui::CEGUICompassImpl*)  new EmberOgre::Gui::CEGUICompassImpl();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Gui::CEGUICompassImpl");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  EmberOgre::Gui::CEGUICompassImpl */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_CEGUICompassImpl_delete00
static int tolua_EmberOgre_EmberOgre_Gui_CEGUICompassImpl_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::CEGUICompassImpl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::CEGUICompassImpl* self = (EmberOgre::Gui::CEGUICompassImpl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getViewImage of class  EmberOgre::Gui::CEGUICompassImpl */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_CEGUICompassImpl_getViewImage00
static int tolua_EmberOgre_EmberOgre_Gui_CEGUICompassImpl_getViewImage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::CEGUICompassImpl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::CEGUICompassImpl* self = (EmberOgre::Gui::CEGUICompassImpl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getViewImage'",NULL);
#endif
  {
   const CEGUI::Image* tolua_ret = (const CEGUI::Image*)  self->getViewImage();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"const CEGUI::Image");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getViewImage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: reposition of class  EmberOgre::Gui::CEGUICompassImpl */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_CEGUICompassImpl_reposition00
static int tolua_EmberOgre_EmberOgre_Gui_CEGUICompassImpl_reposition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::CEGUICompassImpl",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::CEGUICompassImpl* self = (EmberOgre::Gui::CEGUICompassImpl*)  tolua_tousertype(tolua_S,1,0);
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float y = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'reposition'",NULL);
#endif
  {
   self->reposition(x,y);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'reposition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: rotate of class  EmberOgre::Gui::CEGUICompassImpl */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_CEGUICompassImpl_rotate00
static int tolua_EmberOgre_EmberOgre_Gui_CEGUICompassImpl_rotate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::CEGUICompassImpl",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Degree",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::CEGUICompassImpl* self = (EmberOgre::Gui::CEGUICompassImpl*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Degree* degree = ((const Ogre::Degree*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'rotate'",NULL);
#endif
  {
   self->rotate(*degree);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'rotate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::Gui::OverlayCompassImpl */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_OverlayCompassImpl_new00
static int tolua_EmberOgre_EmberOgre_Gui_OverlayCompassImpl_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::OverlayCompassImpl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   EmberOgre::Gui::OverlayCompassImpl* tolua_ret = (EmberOgre::Gui::OverlayCompassImpl*)  new EmberOgre::Gui::OverlayCompassImpl();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::OverlayCompassImpl");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::Gui::OverlayCompassImpl */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_OverlayCompassImpl_new00_local
static int tolua_EmberOgre_EmberOgre_Gui_OverlayCompassImpl_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::OverlayCompassImpl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   EmberOgre::Gui::OverlayCompassImpl* tolua_ret = (EmberOgre::Gui::OverlayCompassImpl*)  new EmberOgre::Gui::OverlayCompassImpl();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Gui::OverlayCompassImpl");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  EmberOgre::Gui::OverlayCompassImpl */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_OverlayCompassImpl_delete00
static int tolua_EmberOgre_EmberOgre_Gui_OverlayCompassImpl_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::OverlayCompassImpl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::OverlayCompassImpl* self = (EmberOgre::Gui::OverlayCompassImpl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: reposition of class  EmberOgre::Gui::OverlayCompassImpl */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_OverlayCompassImpl_reposition00
static int tolua_EmberOgre_EmberOgre_Gui_OverlayCompassImpl_reposition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::OverlayCompassImpl",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::OverlayCompassImpl* self = (EmberOgre::Gui::OverlayCompassImpl*)  tolua_tousertype(tolua_S,1,0);
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float y = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'reposition'",NULL);
#endif
  {
   self->reposition(x,y);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'reposition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: rotate of class  EmberOgre::Gui::OverlayCompassImpl */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_OverlayCompassImpl_rotate00
static int tolua_EmberOgre_EmberOgre_Gui_OverlayCompassImpl_rotate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::OverlayCompassImpl",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Degree",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::OverlayCompassImpl* self = (EmberOgre::Gui::OverlayCompassImpl*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Degree* degree = ((const Ogre::Degree*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'rotate'",NULL);
#endif
  {
   self->rotate(*degree);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'rotate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::Gui::CompassCameraAnchor */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_CompassCameraAnchor_new00
static int tolua_EmberOgre_EmberOgre_Gui_CompassCameraAnchor_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::CompassCameraAnchor",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"EmberOgre::Gui::Compass",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"Ogre::Camera",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Compass* compass = ((EmberOgre::Gui::Compass*)  tolua_tousertype(tolua_S,2,0));
  Ogre::Camera* camera = ((Ogre::Camera*)  tolua_tousertype(tolua_S,3,0));
  {
   EmberOgre::Gui::CompassCameraAnchor* tolua_ret = (EmberOgre::Gui::CompassCameraAnchor*)  new EmberOgre::Gui::CompassCameraAnchor(*compass,camera);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::CompassCameraAnchor");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::Gui::CompassCameraAnchor */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_CompassCameraAnchor_new00_local
static int tolua_EmberOgre_EmberOgre_Gui_CompassCameraAnchor_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::CompassCameraAnchor",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"EmberOgre::Gui::Compass",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"Ogre::Camera",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Compass* compass = ((EmberOgre::Gui::Compass*)  tolua_tousertype(tolua_S,2,0));
  Ogre::Camera* camera = ((Ogre::Camera*)  tolua_tousertype(tolua_S,3,0));
  {
   EmberOgre::Gui::CompassCameraAnchor* tolua_ret = (EmberOgre::Gui::CompassCameraAnchor*)  new EmberOgre::Gui::CompassCameraAnchor(*compass,camera);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Gui::CompassCameraAnchor");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  EmberOgre::Gui::CompassCameraAnchor */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_CompassCameraAnchor_delete00
static int tolua_EmberOgre_EmberOgre_Gui_CompassCameraAnchor_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::CompassCameraAnchor",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::CompassCameraAnchor* self = (EmberOgre::Gui::CompassCameraAnchor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::Gui::CompassSceneNodeAnchor */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_CompassSceneNodeAnchor_new00
static int tolua_EmberOgre_EmberOgre_Gui_CompassSceneNodeAnchor_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::CompassSceneNodeAnchor",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"EmberOgre::Gui::Compass",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Compass* compass = ((EmberOgre::Gui::Compass*)  tolua_tousertype(tolua_S,2,0));
  Ogre::SceneNode* sceneNode = ((Ogre::SceneNode*)  tolua_tousertype(tolua_S,3,0));
  {
   EmberOgre::Gui::CompassSceneNodeAnchor* tolua_ret = (EmberOgre::Gui::CompassSceneNodeAnchor*)  new EmberOgre::Gui::CompassSceneNodeAnchor(*compass,sceneNode);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::CompassSceneNodeAnchor");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::Gui::CompassSceneNodeAnchor */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_CompassSceneNodeAnchor_new00_local
static int tolua_EmberOgre_EmberOgre_Gui_CompassSceneNodeAnchor_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::CompassSceneNodeAnchor",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"EmberOgre::Gui::Compass",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Compass* compass = ((EmberOgre::Gui::Compass*)  tolua_tousertype(tolua_S,2,0));
  Ogre::SceneNode* sceneNode = ((Ogre::SceneNode*)  tolua_tousertype(tolua_S,3,0));
  {
   EmberOgre::Gui::CompassSceneNodeAnchor* tolua_ret = (EmberOgre::Gui::CompassSceneNodeAnchor*)  new EmberOgre::Gui::CompassSceneNodeAnchor(*compass,sceneNode);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Gui::CompassSceneNodeAnchor");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  EmberOgre::Gui::CompassSceneNodeAnchor */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_CompassSceneNodeAnchor_delete00
static int tolua_EmberOgre_EmberOgre_Gui_CompassSceneNodeAnchor_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::CompassSceneNodeAnchor",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::CompassSceneNodeAnchor* self = (EmberOgre::Gui::CompassSceneNodeAnchor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getImage of class  EmberOgre::Gui::Icons::Icon */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_Icons_Icon_getImage00
static int tolua_EmberOgre_EmberOgre_Gui_Icons_Icon_getImage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Icons::Icon",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Icons::Icon* self = (EmberOgre::Gui::Icons::Icon*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getImage'",NULL);
#endif
  {
   const CEGUI::Image* tolua_ret = (const CEGUI::Image*)  self->getImage();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"const CEGUI::Image");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getImage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getImage of class  EmberOgre::Gui::Icons::Icon */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_Icons_Icon_getImage01
static int tolua_EmberOgre_EmberOgre_Gui_Icons_Icon_getImage01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Gui::Icons::Icon",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const EmberOgre::Gui::Icons::Icon* self = (const EmberOgre::Gui::Icons::Icon*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getImage'",NULL);
#endif
  {
   const CEGUI::Image* tolua_ret = (const CEGUI::Image*)  self->getImage();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"const CEGUI::Image");
  }
 }
 return 1;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_Gui_Icons_Icon_getImage00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::Gui::Icons::IconManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_Icons_IconManager_new00
static int tolua_EmberOgre_EmberOgre_Gui_Icons_IconManager_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::Icons::IconManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   EmberOgre::Gui::Icons::IconManager* tolua_ret = (EmberOgre::Gui::Icons::IconManager*)  new EmberOgre::Gui::Icons::IconManager();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::Icons::IconManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::Gui::Icons::IconManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_Icons_IconManager_new00_local
static int tolua_EmberOgre_EmberOgre_Gui_Icons_IconManager_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::Icons::IconManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   EmberOgre::Gui::Icons::IconManager* tolua_ret = (EmberOgre::Gui::Icons::IconManager*)  new EmberOgre::Gui::Icons::IconManager();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Gui::Icons::IconManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  EmberOgre::Gui::Icons::IconManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_Icons_IconManager_delete00
static int tolua_EmberOgre_EmberOgre_Gui_Icons_IconManager_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Icons::IconManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Icons::IconManager* self = (EmberOgre::Gui::Icons::IconManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getIcon of class  EmberOgre::Gui::Icons::IconManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_Icons_IconManager_getIcon00
static int tolua_EmberOgre_EmberOgre_Gui_Icons_IconManager_getIcon00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Icons::IconManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"EmberOgre::EmberEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Icons::IconManager* self = (EmberOgre::Gui::Icons::IconManager*)  tolua_tousertype(tolua_S,1,0);
  int pixelWidth = ((int)  tolua_tonumber(tolua_S,2,0));
  EmberOgre::EmberEntity* entity = ((EmberOgre::EmberEntity*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getIcon'",NULL);
#endif
  {
   EmberOgre::Gui::Icons::Icon* tolua_ret = (EmberOgre::Gui::Icons::Icon*)  self->getIcon(pixelWidth,entity);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::Icons::Icon");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getIcon'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getIcon of class  EmberOgre::Gui::Icons::IconManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_Icons_IconManager_getIcon01
static int tolua_EmberOgre_EmberOgre_Gui_Icons_IconManager_getIcon01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Icons::IconManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"Eris::TypeInfo",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  EmberOgre::Gui::Icons::IconManager* self = (EmberOgre::Gui::Icons::IconManager*)  tolua_tousertype(tolua_S,1,0);
  int pixelWidth = ((int)  tolua_tonumber(tolua_S,2,0));
  Eris::TypeInfo* erisType = ((Eris::TypeInfo*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getIcon'",NULL);
#endif
  {
   EmberOgre::Gui::Icons::Icon* tolua_ret = (EmberOgre::Gui::Icons::Icon*)  self->getIcon(pixelWidth,erisType);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::Icons::Icon");
  }
 }
 return 1;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_Gui_Icons_IconManager_getIcon00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: addEntityIcon of class  EmberOgre::Gui::EntityIconSlot */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_EntityIconSlot_addEntityIcon00
static int tolua_EmberOgre_EmberOgre_Gui_EntityIconSlot_addEntityIcon00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::EntityIconSlot",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"EmberOgre::Gui::EntityIcon",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::EntityIconSlot* self = (EmberOgre::Gui::EntityIconSlot*)  tolua_tousertype(tolua_S,1,0);
  EmberOgre::Gui::EntityIcon* icon = ((EmberOgre::Gui::EntityIcon*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addEntityIcon'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->addEntityIcon(icon);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addEntityIcon'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeEntityIcon of class  EmberOgre::Gui::EntityIconSlot */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_EntityIconSlot_removeEntityIcon00
static int tolua_EmberOgre_EmberOgre_Gui_EntityIconSlot_removeEntityIcon00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::EntityIconSlot",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::EntityIconSlot* self = (EmberOgre::Gui::EntityIconSlot*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeEntityIcon'",NULL);
#endif
  {
   EmberOgre::Gui::EntityIcon* tolua_ret = (EmberOgre::Gui::EntityIcon*)  self->removeEntityIcon();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::EntityIcon");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeEntityIcon'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getEntityIcon of class  EmberOgre::Gui::EntityIconSlot */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_EntityIconSlot_getEntityIcon00
static int tolua_EmberOgre_EmberOgre_Gui_EntityIconSlot_getEntityIcon00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::EntityIconSlot",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::EntityIconSlot* self = (EmberOgre::Gui::EntityIconSlot*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getEntityIcon'",NULL);
#endif
  {
   EmberOgre::Gui::EntityIcon* tolua_ret = (EmberOgre::Gui::EntityIcon*)  self->getEntityIcon();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::EntityIcon");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getEntityIcon'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getWindow of class  EmberOgre::Gui::EntityIconSlot */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_EntityIconSlot_getWindow00
static int tolua_EmberOgre_EmberOgre_Gui_EntityIconSlot_getWindow00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::EntityIconSlot",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::EntityIconSlot* self = (EmberOgre::Gui::EntityIconSlot*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getWindow'",NULL);
#endif
  {
   CEGUI::Window* tolua_ret = (CEGUI::Window*)  self->getWindow();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Window");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getWindow'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: notifyIconDraggedOff of class  EmberOgre::Gui::EntityIconSlot */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_EntityIconSlot_notifyIconDraggedOff00
static int tolua_EmberOgre_EmberOgre_Gui_EntityIconSlot_notifyIconDraggedOff00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::EntityIconSlot",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"EmberOgre::Gui::EntityIcon",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::EntityIconSlot* self = (EmberOgre::Gui::EntityIconSlot*)  tolua_tousertype(tolua_S,1,0);
  EmberOgre::Gui::EntityIcon* entityIcon = ((EmberOgre::Gui::EntityIcon*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'notifyIconDraggedOff'",NULL);
#endif
  {
   self->notifyIconDraggedOff(entityIcon);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'notifyIconDraggedOff'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventIconDraggedOff of class  EmberOgre::Gui::EntityIconSlot */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__Gui__EntityIconSlot_EventIconDraggedOff
static int tolua_get_EmberOgre__Gui__EntityIconSlot_EventIconDraggedOff(lua_State* tolua_S)
{
  EmberOgre::Gui::EntityIconSlot* self = (EmberOgre::Gui::EntityIconSlot*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventIconDraggedOff'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventIconDraggedOff,"sigc::signal<void,EmberOgre::Gui::EntityIcon*>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventIconDraggedOff of class  EmberOgre::Gui::EntityIconSlot */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__Gui__EntityIconSlot_EventIconDraggedOff
static int tolua_set_EmberOgre__Gui__EntityIconSlot_EventIconDraggedOff(lua_State* tolua_S)
{
  EmberOgre::Gui::EntityIconSlot* self = (EmberOgre::Gui::EntityIconSlot*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventIconDraggedOff'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,EmberOgre::Gui::EntityIcon*>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventIconDraggedOff = *((sigc::signal<void,EmberOgre::Gui::EntityIcon*>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getImage of class  EmberOgre::Gui::EntityIcon */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_EntityIcon_getImage00
static int tolua_EmberOgre_EmberOgre_Gui_EntityIcon_getImage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::EntityIcon",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::EntityIcon* self = (EmberOgre::Gui::EntityIcon*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getImage'",NULL);
#endif
  {
   CEGUI::Window* tolua_ret = (CEGUI::Window*)  self->getImage();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Window");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getImage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getDragContainer of class  EmberOgre::Gui::EntityIcon */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_EntityIcon_getDragContainer00
static int tolua_EmberOgre_EmberOgre_Gui_EntityIcon_getDragContainer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::EntityIcon",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::EntityIcon* self = (EmberOgre::Gui::EntityIcon*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getDragContainer'",NULL);
#endif
  {
   CEGUI::DragContainer* tolua_ret = (CEGUI::DragContainer*)  self->getDragContainer();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::DragContainer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getDragContainer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getIcon of class  EmberOgre::Gui::EntityIcon */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_EntityIcon_getIcon00
static int tolua_EmberOgre_EmberOgre_Gui_EntityIcon_getIcon00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::EntityIcon",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::EntityIcon* self = (EmberOgre::Gui::EntityIcon*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getIcon'",NULL);
#endif
  {
   EmberOgre::Gui::Icons::Icon* tolua_ret = (EmberOgre::Gui::Icons::Icon*)  self->getIcon();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::Icons::Icon");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getIcon'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setSlot of class  EmberOgre::Gui::EntityIcon */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_EntityIcon_setSlot00
static int tolua_EmberOgre_EmberOgre_Gui_EntityIcon_setSlot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::EntityIcon",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"EmberOgre::Gui::EntityIconSlot",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::EntityIcon* self = (EmberOgre::Gui::EntityIcon*)  tolua_tousertype(tolua_S,1,0);
  EmberOgre::Gui::EntityIconSlot* slot = ((EmberOgre::Gui::EntityIconSlot*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setSlot'",NULL);
#endif
  {
   self->setSlot(slot);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setSlot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSlot of class  EmberOgre::Gui::EntityIcon */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_EntityIcon_getSlot00
static int tolua_EmberOgre_EmberOgre_Gui_EntityIcon_getSlot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::EntityIcon",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::EntityIcon* self = (EmberOgre::Gui::EntityIcon*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSlot'",NULL);
#endif
  {
   EmberOgre::Gui::EntityIconSlot* tolua_ret = (EmberOgre::Gui::EntityIconSlot*)  self->getSlot();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::EntityIconSlot");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSlot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setTooltipText of class  EmberOgre::Gui::EntityIcon */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_EntityIcon_setTooltipText00
static int tolua_EmberOgre_EmberOgre_Gui_EntityIcon_setTooltipText00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::EntityIcon",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::EntityIcon* self = (EmberOgre::Gui::EntityIcon*)  tolua_tousertype(tolua_S,1,0);
  const std::string text = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setTooltipText'",NULL);
#endif
  {
   self->setTooltipText(text);
   tolua_pushcppstring(tolua_S,(const char*)text);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setTooltipText'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getEntity of class  EmberOgre::Gui::EntityIcon */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_EntityIcon_getEntity00
static int tolua_EmberOgre_EmberOgre_Gui_EntityIcon_getEntity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::EntityIcon",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::EntityIcon* self = (EmberOgre::Gui::EntityIcon*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getEntity'",NULL);
#endif
  {
   EmberOgre::EmberEntity* tolua_ret = (EmberOgre::EmberEntity*)  self->getEntity();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::EmberEntity");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getEntity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createSlot of class  EmberOgre::Gui::EntityIconManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_EntityIconManager_createSlot00
static int tolua_EmberOgre_EmberOgre_Gui_EntityIconManager_createSlot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::EntityIconManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::EntityIconManager* self = (EmberOgre::Gui::EntityIconManager*)  tolua_tousertype(tolua_S,1,0);
  unsigned int pixelSize = ((unsigned int)  tolua_tonumber(tolua_S,2,64));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createSlot'",NULL);
#endif
  {
   EmberOgre::Gui::EntityIconSlot* tolua_ret = (EmberOgre::Gui::EntityIconSlot*)  self->createSlot(pixelSize);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::EntityIconSlot");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createSlot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: destroySlot of class  EmberOgre::Gui::EntityIconManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_EntityIconManager_destroySlot00
static int tolua_EmberOgre_EmberOgre_Gui_EntityIconManager_destroySlot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::EntityIconManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"EmberOgre::Gui::EntityIconSlot",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::EntityIconManager* self = (EmberOgre::Gui::EntityIconManager*)  tolua_tousertype(tolua_S,1,0);
  EmberOgre::Gui::EntityIconSlot* slot = ((EmberOgre::Gui::EntityIconSlot*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroySlot'",NULL);
#endif
  {
   self->destroySlot(slot);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'destroySlot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createIcon of class  EmberOgre::Gui::EntityIconManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_EntityIconManager_createIcon00
static int tolua_EmberOgre_EmberOgre_Gui_EntityIconManager_createIcon00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::EntityIconManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"EmberOgre::Gui::Icons::Icon",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"EmberOgre::EmberEntity",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::EntityIconManager* self = (EmberOgre::Gui::EntityIconManager*)  tolua_tousertype(tolua_S,1,0);
  EmberOgre::Gui::Icons::Icon* icon = ((EmberOgre::Gui::Icons::Icon*)  tolua_tousertype(tolua_S,2,0));
  EmberOgre::EmberEntity* entity = ((EmberOgre::EmberEntity*)  tolua_tousertype(tolua_S,3,0));
  unsigned int pixelSize = ((unsigned int)  tolua_tonumber(tolua_S,4,64));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createIcon'",NULL);
#endif
  {
   EmberOgre::Gui::EntityIcon* tolua_ret = (EmberOgre::Gui::EntityIcon*)  self->createIcon(icon,entity,pixelSize);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::EntityIcon");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createIcon'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: destroyIcon of class  EmberOgre::Gui::EntityIconManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_EntityIconManager_destroyIcon00
static int tolua_EmberOgre_EmberOgre_Gui_EntityIconManager_destroyIcon00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::EntityIconManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"EmberOgre::Gui::EntityIcon",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::EntityIconManager* self = (EmberOgre::Gui::EntityIconManager*)  tolua_tousertype(tolua_S,1,0);
  EmberOgre::Gui::EntityIcon* icon = ((EmberOgre::Gui::EntityIcon*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroyIcon'",NULL);
#endif
  {
   self->destroyIcon(icon);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'destroyIcon'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventIconDragStart of class  EmberOgre::Gui::EntityIconManager */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__Gui__EntityIconManager_EventIconDragStart
static int tolua_get_EmberOgre__Gui__EntityIconManager_EventIconDragStart(lua_State* tolua_S)
{
  EmberOgre::Gui::EntityIconManager* self = (EmberOgre::Gui::EntityIconManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventIconDragStart'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventIconDragStart,"sigc::signal<void,EmberOgre::Gui::EntityIcon*>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventIconDragStart of class  EmberOgre::Gui::EntityIconManager */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__Gui__EntityIconManager_EventIconDragStart
static int tolua_set_EmberOgre__Gui__EntityIconManager_EventIconDragStart(lua_State* tolua_S)
{
  EmberOgre::Gui::EntityIconManager* self = (EmberOgre::Gui::EntityIconManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventIconDragStart'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,EmberOgre::Gui::EntityIcon*>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventIconDragStart = *((sigc::signal<void,EmberOgre::Gui::EntityIcon*>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventIconDragStop of class  EmberOgre::Gui::EntityIconManager */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__Gui__EntityIconManager_EventIconDragStop
static int tolua_get_EmberOgre__Gui__EntityIconManager_EventIconDragStop(lua_State* tolua_S)
{
  EmberOgre::Gui::EntityIconManager* self = (EmberOgre::Gui::EntityIconManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventIconDragStop'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventIconDragStop,"sigc::signal<void,EmberOgre::Gui::EntityIcon*>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventIconDragStop of class  EmberOgre::Gui::EntityIconManager */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__Gui__EntityIconManager_EventIconDragStop
static int tolua_set_EmberOgre__Gui__EntityIconManager_EventIconDragStop(lua_State* tolua_S)
{
  EmberOgre::Gui::EntityIconManager* self = (EmberOgre::Gui::EntityIconManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventIconDragStop'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,EmberOgre::Gui::EntityIcon*>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventIconDragStop = *((sigc::signal<void,EmberOgre::Gui::EntityIcon*>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventIconEntered of class  EmberOgre::Gui::EntityIconDragDropTarget */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__Gui__EntityIconDragDropTarget_EventIconEntered
static int tolua_get_EmberOgre__Gui__EntityIconDragDropTarget_EventIconEntered(lua_State* tolua_S)
{
  EmberOgre::Gui::EntityIconDragDropTarget* self = (EmberOgre::Gui::EntityIconDragDropTarget*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventIconEntered'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventIconEntered,"sigc::signal<void,EmberOgre::Gui::EntityIcon*>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventIconEntered of class  EmberOgre::Gui::EntityIconDragDropTarget */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__Gui__EntityIconDragDropTarget_EventIconEntered
static int tolua_set_EmberOgre__Gui__EntityIconDragDropTarget_EventIconEntered(lua_State* tolua_S)
{
  EmberOgre::Gui::EntityIconDragDropTarget* self = (EmberOgre::Gui::EntityIconDragDropTarget*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventIconEntered'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,EmberOgre::Gui::EntityIcon*>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventIconEntered = *((sigc::signal<void,EmberOgre::Gui::EntityIcon*>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventIconLeaves of class  EmberOgre::Gui::EntityIconDragDropTarget */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__Gui__EntityIconDragDropTarget_EventIconLeaves
static int tolua_get_EmberOgre__Gui__EntityIconDragDropTarget_EventIconLeaves(lua_State* tolua_S)
{
  EmberOgre::Gui::EntityIconDragDropTarget* self = (EmberOgre::Gui::EntityIconDragDropTarget*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventIconLeaves'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventIconLeaves,"sigc::signal<void,EmberOgre::Gui::EntityIcon*>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventIconLeaves of class  EmberOgre::Gui::EntityIconDragDropTarget */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__Gui__EntityIconDragDropTarget_EventIconLeaves
static int tolua_set_EmberOgre__Gui__EntityIconDragDropTarget_EventIconLeaves(lua_State* tolua_S)
{
  EmberOgre::Gui::EntityIconDragDropTarget* self = (EmberOgre::Gui::EntityIconDragDropTarget*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventIconLeaves'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,EmberOgre::Gui::EntityIcon*>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventIconLeaves = *((sigc::signal<void,EmberOgre::Gui::EntityIcon*>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventIconDropped of class  EmberOgre::Gui::EntityIconDragDropTarget */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__Gui__EntityIconDragDropTarget_EventIconDropped
static int tolua_get_EmberOgre__Gui__EntityIconDragDropTarget_EventIconDropped(lua_State* tolua_S)
{
  EmberOgre::Gui::EntityIconDragDropTarget* self = (EmberOgre::Gui::EntityIconDragDropTarget*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventIconDropped'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventIconDropped,"sigc::signal<void,EmberOgre::Gui::EntityIcon*>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventIconDropped of class  EmberOgre::Gui::EntityIconDragDropTarget */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__Gui__EntityIconDragDropTarget_EventIconDropped
static int tolua_set_EmberOgre__Gui__EntityIconDragDropTarget_EventIconDropped(lua_State* tolua_S)
{
  EmberOgre::Gui::EntityIconDragDropTarget* self = (EmberOgre::Gui::EntityIconDragDropTarget*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventIconDropped'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,EmberOgre::Gui::EntityIcon*>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventIconDropped = *((sigc::signal<void,EmberOgre::Gui::EntityIcon*>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: createModel of class  EmberOgre::Model::Model */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_Model_createModel00
static int tolua_EmberOgre_EmberOgre_Model_Model_createModel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Model::Model",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::SceneManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::SceneManager* sceneManager = ((Ogre::SceneManager*)  tolua_tousertype(tolua_S,2,0));
  const std::string modelType = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  {
   EmberOgre::Model::Model* tolua_ret = (EmberOgre::Model::Model*)  EmberOgre::Model::Model::createModel(sceneManager,modelType);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Model::Model");
   tolua_pushcppstring(tolua_S,(const char*)modelType);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createModel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createModel of class  EmberOgre::Model::Model */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_Model_createModel01
static int tolua_EmberOgre_EmberOgre_Model_Model_createModel01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Model::Model",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::SceneManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ogre::SceneManager* sceneManager = ((Ogre::SceneManager*)  tolua_tousertype(tolua_S,2,0));
  const std::string modelType = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,4,0));
  {
   EmberOgre::Model::Model* tolua_ret = (EmberOgre::Model::Model*)  EmberOgre::Model::Model::createModel(sceneManager,modelType,name);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Model::Model");
   tolua_pushcppstring(tolua_S,(const char*)modelType);
   tolua_pushcppstring(tolua_S,(const char*)name);
  }
 }
 return 3;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_Model_Model_createModel00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: reload of class  EmberOgre::Model::Model */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_Model_reload00
static int tolua_EmberOgre_EmberOgre_Model_Model_reload00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::Model",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::Model* self = (EmberOgre::Model::Model*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'reload'",NULL);
#endif
  {
   self->reload();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'reload'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  EmberOgre::Model::Model */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_Model_delete00
static int tolua_EmberOgre_EmberOgre_Model_Model_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::Model",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::Model* self = (EmberOgre::Model::Model*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addSubmodel of class  EmberOgre::Model::Model */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_Model_addSubmodel00
static int tolua_EmberOgre_EmberOgre_Model_Model_addSubmodel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::Model",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"EmberOgre::Model::SubModel",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::Model* self = (EmberOgre::Model::Model*)  tolua_tousertype(tolua_S,1,0);
  EmberOgre::Model::SubModel* submodel = ((EmberOgre::Model::SubModel*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addSubmodel'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->addSubmodel(submodel);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addSubmodel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeSubmodel of class  EmberOgre::Model::Model */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_Model_removeSubmodel00
static int tolua_EmberOgre_EmberOgre_Model_Model_removeSubmodel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::Model",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"EmberOgre::Model::SubModel",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::Model* self = (EmberOgre::Model::Model*)  tolua_tousertype(tolua_S,1,0);
  EmberOgre::Model::SubModel* submodel = ((EmberOgre::Model::SubModel*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeSubmodel'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->removeSubmodel(submodel);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeSubmodel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAction of class  EmberOgre::Model::Model */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_Model_getAction00
static int tolua_EmberOgre_EmberOgre_Model_Model_getAction00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::Model",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::Model* self = (EmberOgre::Model::Model*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAction'",NULL);
#endif
  {
   EmberOgre::Model::Action* tolua_ret = (EmberOgre::Model::Action*)  self->getAction(name);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Model::Action");
   tolua_pushcppstring(tolua_S,(const char*)name);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAction'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: showPart of class  EmberOgre::Model::Model */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_Model_showPart00
static int tolua_EmberOgre_EmberOgre_Model_Model_showPart00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::Model",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::Model* self = (EmberOgre::Model::Model*)  tolua_tousertype(tolua_S,1,0);
  const std::string partName = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'showPart'",NULL);
#endif
  {
   self->showPart(partName);
   tolua_pushcppstring(tolua_S,(const char*)partName);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'showPart'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: hidePart of class  EmberOgre::Model::Model */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_Model_hidePart00
static int tolua_EmberOgre_EmberOgre_Model_Model_hidePart00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::Model",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::Model* self = (EmberOgre::Model::Model*)  tolua_tousertype(tolua_S,1,0);
  const std::string partName = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hidePart'",NULL);
#endif
  {
   self->hidePart(partName);
   tolua_pushcppstring(tolua_S,(const char*)partName);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'hidePart'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setVisible of class  EmberOgre::Model::Model */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_Model_setVisible00
static int tolua_EmberOgre_EmberOgre_Model_Model_setVisible00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::Model",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::Model* self = (EmberOgre::Model::Model*)  tolua_tousertype(tolua_S,1,0);
  bool visible = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setVisible'",NULL);
#endif
  {
   self->setVisible(visible);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setVisible'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getScale of class  EmberOgre::Model::Model */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_Model_getScale00
static int tolua_EmberOgre_EmberOgre_Model_Model_getScale00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Model::Model",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Model::Model* self = (const EmberOgre::Model::Model*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getScale'",NULL);
#endif
  {
   const Ogre::Real tolua_ret = (const Ogre::Real)  self->getScale();
   {
#ifdef __cplusplus
    void* tolua_obj = new Ogre::Real(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"const Ogre::Real");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(const Ogre::Real));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"const Ogre::Real");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getScale'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRotation of class  EmberOgre::Model::Model */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_Model_getRotation00
static int tolua_EmberOgre_EmberOgre_Model_Model_getRotation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Model::Model",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Model::Model* self = (const EmberOgre::Model::Model*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRotation'",NULL);
#endif
  {
   const Ogre::Quaternion& tolua_ret = (const Ogre::Quaternion&)  self->getRotation();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Ogre::Quaternion");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRotation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getUseScaleOf of class  EmberOgre::Model::Model */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_Model_getUseScaleOf00
static int tolua_EmberOgre_EmberOgre_Model_Model_getUseScaleOf00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Model::Model",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Model::Model* self = (const EmberOgre::Model::Model*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getUseScaleOf'",NULL);
#endif
  {
   const EmberOgre::Model::ModelDefinition::UseScaleOf tolua_ret = (const EmberOgre::Model::ModelDefinition::UseScaleOf)  self->getUseScaleOf();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getUseScaleOf'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getName of class  EmberOgre::Model::Model */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_Model_getName00
static int tolua_EmberOgre_EmberOgre_Model_Model_getName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Model::Model",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Model::Model* self = (const EmberOgre::Model::Model*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getName'",NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getName();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getDefinition of class  EmberOgre::Model::Model */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_Model_getDefinition00
static int tolua_EmberOgre_EmberOgre_Model_Model_getDefinition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Model::Model",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Model::Model* self = (const EmberOgre::Model::Model*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getDefinition'",NULL);
#endif
  {
   EmberOgre::Model::ModelDefnPtr tolua_ret = (EmberOgre::Model::ModelDefnPtr)  self->getDefinition();
   {
#ifdef __cplusplus
    void* tolua_obj = new EmberOgre::Model::ModelDefnPtr(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"EmberOgre::Model::ModelDefnPtr");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(EmberOgre::Model::ModelDefnPtr));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"EmberOgre::Model::ModelDefnPtr");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getDefinition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: hasAttachPoint of class  EmberOgre::Model::Model */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_Model_hasAttachPoint00
static int tolua_EmberOgre_EmberOgre_Model_Model_hasAttachPoint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Model::Model",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Model::Model* self = (const EmberOgre::Model::Model*)  tolua_tousertype(tolua_S,1,0);
  const std::string attachPoint = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hasAttachPoint'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->hasAttachPoint(attachPoint);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)attachPoint);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'hasAttachPoint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: hasParticles of class  EmberOgre::Model::Model */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_Model_hasParticles00
static int tolua_EmberOgre_EmberOgre_Model_Model_hasParticles00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Model::Model",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Model::Model* self = (const EmberOgre::Model::Model*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hasParticles'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->hasParticles();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'hasParticles'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSubModel of class  EmberOgre::Model::Model */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_Model_getSubModel00
static int tolua_EmberOgre_EmberOgre_Model_Model_getSubModel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::Model",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::Model* self = (EmberOgre::Model::Model*)  tolua_tousertype(tolua_S,1,0);
  unsigned int index = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSubModel'",NULL);
#endif
  {
   EmberOgre::Model::SubModel* tolua_ret = (EmberOgre::Model::SubModel*)  self->getSubModel(index);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Model::SubModel");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSubModel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getEntity of class  EmberOgre::Model::SubModel */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_SubModel_getEntity00
static int tolua_EmberOgre_EmberOgre_Model_SubModel_getEntity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Model::SubModel",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Model::SubModel* self = (const EmberOgre::Model::SubModel*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getEntity'",NULL);
#endif
  {
   Ogre::Entity* tolua_ret = (Ogre::Entity*)  self->getEntity();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::Entity");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getEntity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Name of class  EmberOgre::Model::ViewDefinition */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__Model__ViewDefinition_Name
static int tolua_get_EmberOgre__Model__ViewDefinition_Name(lua_State* tolua_S)
{
  EmberOgre::Model::ViewDefinition* self = (EmberOgre::Model::ViewDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Name'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->Name);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Name of class  EmberOgre::Model::ViewDefinition */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__Model__ViewDefinition_Name
static int tolua_set_EmberOgre__Model__ViewDefinition_Name(lua_State* tolua_S)
{
  EmberOgre::Model::ViewDefinition* self = (EmberOgre::Model::ViewDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Name'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Name = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Rotation of class  EmberOgre::Model::ViewDefinition */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__Model__ViewDefinition_Rotation
static int tolua_get_EmberOgre__Model__ViewDefinition_Rotation(lua_State* tolua_S)
{
  EmberOgre::Model::ViewDefinition* self = (EmberOgre::Model::ViewDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Rotation'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->Rotation,"Ogre::Quaternion");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Rotation of class  EmberOgre::Model::ViewDefinition */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__Model__ViewDefinition_Rotation
static int tolua_set_EmberOgre__Model__ViewDefinition_Rotation(lua_State* tolua_S)
{
  EmberOgre::Model::ViewDefinition* self = (EmberOgre::Model::ViewDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Rotation'",NULL);
  if (!tolua_isusertype(tolua_S,2,"Ogre::Quaternion",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Rotation = *((Ogre::Quaternion*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Distance of class  EmberOgre::Model::ViewDefinition */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__Model__ViewDefinition_Distance
static int tolua_get_EmberOgre__Model__ViewDefinition_Distance(lua_State* tolua_S)
{
  EmberOgre::Model::ViewDefinition* self = (EmberOgre::Model::ViewDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Distance'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->Distance);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Distance of class  EmberOgre::Model::ViewDefinition */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__Model__ViewDefinition_Distance
static int tolua_set_EmberOgre__Model__ViewDefinition_Distance(lua_State* tolua_S)
{
  EmberOgre::Model::ViewDefinition* self = (EmberOgre::Model::ViewDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Distance'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Distance = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Name of class  EmberOgre::Model::AttachPointDefinition */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__Model__AttachPointDefinition_Name
static int tolua_get_EmberOgre__Model__AttachPointDefinition_Name(lua_State* tolua_S)
{
  EmberOgre::Model::AttachPointDefinition* self = (EmberOgre::Model::AttachPointDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Name'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->Name);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Name of class  EmberOgre::Model::AttachPointDefinition */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__Model__AttachPointDefinition_Name
static int tolua_set_EmberOgre__Model__AttachPointDefinition_Name(lua_State* tolua_S)
{
  EmberOgre::Model::AttachPointDefinition* self = (EmberOgre::Model::AttachPointDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Name'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Name = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: BoneName of class  EmberOgre::Model::AttachPointDefinition */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__Model__AttachPointDefinition_BoneName
static int tolua_get_EmberOgre__Model__AttachPointDefinition_BoneName(lua_State* tolua_S)
{
  EmberOgre::Model::AttachPointDefinition* self = (EmberOgre::Model::AttachPointDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'BoneName'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->BoneName);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: BoneName of class  EmberOgre::Model::AttachPointDefinition */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__Model__AttachPointDefinition_BoneName
static int tolua_set_EmberOgre__Model__AttachPointDefinition_BoneName(lua_State* tolua_S)
{
  EmberOgre::Model::AttachPointDefinition* self = (EmberOgre::Model::AttachPointDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'BoneName'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->BoneName = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Rotation of class  EmberOgre::Model::AttachPointDefinition */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__Model__AttachPointDefinition_Rotation
static int tolua_get_EmberOgre__Model__AttachPointDefinition_Rotation(lua_State* tolua_S)
{
  EmberOgre::Model::AttachPointDefinition* self = (EmberOgre::Model::AttachPointDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Rotation'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->Rotation,"Ogre::Quaternion");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Rotation of class  EmberOgre::Model::AttachPointDefinition */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__Model__AttachPointDefinition_Rotation
static int tolua_set_EmberOgre__Model__AttachPointDefinition_Rotation(lua_State* tolua_S)
{
  EmberOgre::Model::AttachPointDefinition* self = (EmberOgre::Model::AttachPointDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Rotation'",NULL);
  if (!tolua_isusertype(tolua_S,2,"Ogre::Quaternion",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Rotation = *((Ogre::Quaternion*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSubEntityName of class  EmberOgre::Model::SubEntityDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_SubEntityDefinition_getSubEntityName00
static int tolua_EmberOgre_EmberOgre_Model_SubEntityDefinition_getSubEntityName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::SubEntityDefinition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::SubEntityDefinition* self = (EmberOgre::Model::SubEntityDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSubEntityName'",NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getSubEntityName();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSubEntityName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSubEntityIndex of class  EmberOgre::Model::SubEntityDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_SubEntityDefinition_getSubEntityIndex00
static int tolua_EmberOgre_EmberOgre_Model_SubEntityDefinition_getSubEntityIndex00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::SubEntityDefinition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::SubEntityDefinition* self = (EmberOgre::Model::SubEntityDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSubEntityIndex'",NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->getSubEntityIndex();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSubEntityIndex'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMaterialName of class  EmberOgre::Model::SubEntityDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_SubEntityDefinition_getMaterialName00
static int tolua_EmberOgre_EmberOgre_Model_SubEntityDefinition_getMaterialName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::SubEntityDefinition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::SubEntityDefinition* self = (EmberOgre::Model::SubEntityDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMaterialName'",NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getMaterialName();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMaterialName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setMaterialName of class  EmberOgre::Model::SubEntityDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_SubEntityDefinition_setMaterialName00
static int tolua_EmberOgre_EmberOgre_Model_SubEntityDefinition_setMaterialName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::SubEntityDefinition",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::SubEntityDefinition* self = (EmberOgre::Model::SubEntityDefinition*)  tolua_tousertype(tolua_S,1,0);
  std::string materialName = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMaterialName'",NULL);
#endif
  {
   self->setMaterialName(materialName);
   tolua_pushcppstring(tolua_S,(const char*)materialName);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setMaterialName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPartDefinition of class  EmberOgre::Model::SubEntityDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_SubEntityDefinition_getPartDefinition00
static int tolua_EmberOgre_EmberOgre_Model_SubEntityDefinition_getPartDefinition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::SubEntityDefinition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::SubEntityDefinition* self = (EmberOgre::Model::SubEntityDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPartDefinition'",NULL);
#endif
  {
   EmberOgre::Model::PartDefinition& tolua_ret = (EmberOgre::Model::PartDefinition&)  self->getPartDefinition();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"EmberOgre::Model::PartDefinition");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPartDefinition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setName of class  EmberOgre::Model::PartDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_PartDefinition_setName00
static int tolua_EmberOgre_EmberOgre_Model_PartDefinition_setName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::PartDefinition",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::PartDefinition* self = (EmberOgre::Model::PartDefinition*)  tolua_tousertype(tolua_S,1,0);
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setName'",NULL);
#endif
  {
   self->setName(name);
   tolua_pushcppstring(tolua_S,(const char*)name);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getName of class  EmberOgre::Model::PartDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_PartDefinition_getName00
static int tolua_EmberOgre_EmberOgre_Model_PartDefinition_getName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::PartDefinition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::PartDefinition* self = (EmberOgre::Model::PartDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getName'",NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getName();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setShow of class  EmberOgre::Model::PartDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_PartDefinition_setShow00
static int tolua_EmberOgre_EmberOgre_Model_PartDefinition_setShow00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::PartDefinition",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::PartDefinition* self = (EmberOgre::Model::PartDefinition*)  tolua_tousertype(tolua_S,1,0);
  bool show = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setShow'",NULL);
#endif
  {
   self->setShow(show);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setShow'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getShow of class  EmberOgre::Model::PartDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_PartDefinition_getShow00
static int tolua_EmberOgre_EmberOgre_Model_PartDefinition_getShow00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::PartDefinition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::PartDefinition* self = (EmberOgre::Model::PartDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getShow'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->getShow();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getShow'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createSubEntityDefinition of class  EmberOgre::Model::PartDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_PartDefinition_createSubEntityDefinition00
static int tolua_EmberOgre_EmberOgre_Model_PartDefinition_createSubEntityDefinition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::PartDefinition",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::PartDefinition* self = (EmberOgre::Model::PartDefinition*)  tolua_tousertype(tolua_S,1,0);
  std::string subEntityName = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createSubEntityDefinition'",NULL);
#endif
  {
   EmberOgre::Model::SubEntityDefinition* tolua_ret = (EmberOgre::Model::SubEntityDefinition*)  self->createSubEntityDefinition(subEntityName);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Model::SubEntityDefinition");
   tolua_pushcppstring(tolua_S,(const char*)subEntityName);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createSubEntityDefinition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createSubEntityDefinition of class  EmberOgre::Model::PartDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_PartDefinition_createSubEntityDefinition01
static int tolua_EmberOgre_EmberOgre_Model_PartDefinition_createSubEntityDefinition01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::PartDefinition",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  EmberOgre::Model::PartDefinition* self = (EmberOgre::Model::PartDefinition*)  tolua_tousertype(tolua_S,1,0);
  unsigned int subEntityIndex = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createSubEntityDefinition'",NULL);
#endif
  {
   EmberOgre::Model::SubEntityDefinition* tolua_ret = (EmberOgre::Model::SubEntityDefinition*)  self->createSubEntityDefinition(subEntityIndex);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Model::SubEntityDefinition");
  }
 }
 return 1;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_Model_PartDefinition_createSubEntityDefinition00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSubEntityDefinitions of class  EmberOgre::Model::PartDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_PartDefinition_getSubEntityDefinitions00
static int tolua_EmberOgre_EmberOgre_Model_PartDefinition_getSubEntityDefinitions00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::PartDefinition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::PartDefinition* self = (EmberOgre::Model::PartDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSubEntityDefinitions'",NULL);
#endif
  {
   std::vector<EmberOgre::Model::SubEntityDefinition*> tolua_ret = (std::vector<EmberOgre::Model::SubEntityDefinition*>)  self->getSubEntityDefinitions();
   {
#ifdef __cplusplus
    void* tolua_obj = new std::vector<EmberOgre::Model::SubEntityDefinition*>(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"std::vector<EmberOgre::Model::SubEntityDefinition*>");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(std::vector<EmberOgre::Model::SubEntityDefinition*>));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"std::vector<EmberOgre::Model::SubEntityDefinition*>");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSubEntityDefinitions'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeSubEntityDefinition of class  EmberOgre::Model::PartDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_PartDefinition_removeSubEntityDefinition00
static int tolua_EmberOgre_EmberOgre_Model_PartDefinition_removeSubEntityDefinition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::PartDefinition",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"EmberOgre::Model::SubEntityDefinition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::PartDefinition* self = (EmberOgre::Model::PartDefinition*)  tolua_tousertype(tolua_S,1,0);
  EmberOgre::Model::SubEntityDefinition* def = ((EmberOgre::Model::SubEntityDefinition*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeSubEntityDefinition'",NULL);
#endif
  {
   self->removeSubEntityDefinition(def);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeSubEntityDefinition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSubModelDefinition of class  EmberOgre::Model::PartDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_PartDefinition_getSubModelDefinition00
static int tolua_EmberOgre_EmberOgre_Model_PartDefinition_getSubModelDefinition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::PartDefinition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::PartDefinition* self = (EmberOgre::Model::PartDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSubModelDefinition'",NULL);
#endif
  {
   EmberOgre::Model::SubModelDefinition& tolua_ret = (EmberOgre::Model::SubModelDefinition&)  self->getSubModelDefinition();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"EmberOgre::Model::SubModelDefinition");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSubModelDefinition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMeshName of class  EmberOgre::Model::SubModelDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_SubModelDefinition_getMeshName00
static int tolua_EmberOgre_EmberOgre_Model_SubModelDefinition_getMeshName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::SubModelDefinition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::SubModelDefinition* self = (EmberOgre::Model::SubModelDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMeshName'",NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getMeshName();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMeshName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createPartDefinition of class  EmberOgre::Model::SubModelDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_SubModelDefinition_createPartDefinition00
static int tolua_EmberOgre_EmberOgre_Model_SubModelDefinition_createPartDefinition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::SubModelDefinition",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::SubModelDefinition* self = (EmberOgre::Model::SubModelDefinition*)  tolua_tousertype(tolua_S,1,0);
  std::string partname = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createPartDefinition'",NULL);
#endif
  {
   EmberOgre::Model::PartDefinition* tolua_ret = (EmberOgre::Model::PartDefinition*)  self->createPartDefinition(partname);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Model::PartDefinition");
   tolua_pushcppstring(tolua_S,(const char*)partname);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createPartDefinition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPartDefinitions of class  EmberOgre::Model::SubModelDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_SubModelDefinition_getPartDefinitions00
static int tolua_EmberOgre_EmberOgre_Model_SubModelDefinition_getPartDefinitions00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::SubModelDefinition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::SubModelDefinition* self = (EmberOgre::Model::SubModelDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPartDefinitions'",NULL);
#endif
  {
   std::vector<EmberOgre::Model::PartDefinition*> tolua_ret = (std::vector<EmberOgre::Model::PartDefinition*>)  self->getPartDefinitions();
   {
#ifdef __cplusplus
    void* tolua_obj = new std::vector<EmberOgre::Model::PartDefinition*>(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"std::vector<EmberOgre::Model::PartDefinition*>");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(std::vector<EmberOgre::Model::PartDefinition*>));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"std::vector<EmberOgre::Model::PartDefinition*>");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPartDefinitions'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: removePartDefinition of class  EmberOgre::Model::SubModelDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_SubModelDefinition_removePartDefinition00
static int tolua_EmberOgre_EmberOgre_Model_SubModelDefinition_removePartDefinition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::SubModelDefinition",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"EmberOgre::Model::PartDefinition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::SubModelDefinition* self = (EmberOgre::Model::SubModelDefinition*)  tolua_tousertype(tolua_S,1,0);
  EmberOgre::Model::PartDefinition* def = ((EmberOgre::Model::PartDefinition*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removePartDefinition'",NULL);
#endif
  {
   self->removePartDefinition(def);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removePartDefinition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getModelDefinition of class  EmberOgre::Model::SubModelDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_SubModelDefinition_getModelDefinition00
static int tolua_EmberOgre_EmberOgre_Model_SubModelDefinition_getModelDefinition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::SubModelDefinition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::SubModelDefinition* self = (EmberOgre::Model::SubModelDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getModelDefinition'",NULL);
#endif
  {
   EmberOgre::Model::ModelDefinition& tolua_ret = (EmberOgre::Model::ModelDefinition&)  self->getModelDefinition();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"EmberOgre::Model::ModelDefinition");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getModelDefinition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isValid of class  EmberOgre::Model::ModelDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_ModelDefinition_isValid00
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinition_isValid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::ModelDefinition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::ModelDefinition* self = (EmberOgre::Model::ModelDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isValid'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isValid();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isValid'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setValid of class  EmberOgre::Model::ModelDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_ModelDefinition_setValid00
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinition_setValid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::ModelDefinition",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::ModelDefinition* self = (EmberOgre::Model::ModelDefinition*)  tolua_tousertype(tolua_S,1,0);
  bool valid = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setValid'",NULL);
#endif
  {
   self->setValid(valid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setValid'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getScale of class  EmberOgre::Model::ModelDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_ModelDefinition_getScale00
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinition_getScale00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::ModelDefinition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::ModelDefinition* self = (EmberOgre::Model::ModelDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getScale'",NULL);
#endif
  {
   float tolua_ret = (float)  self->getScale();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getScale'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setScale of class  EmberOgre::Model::ModelDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_ModelDefinition_setScale00
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinition_setScale00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::ModelDefinition",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::ModelDefinition* self = (EmberOgre::Model::ModelDefinition*)  tolua_tousertype(tolua_S,1,0);
  float scale = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setScale'",NULL);
#endif
  {
   self->setScale(scale);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setScale'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getUseScaleOf of class  EmberOgre::Model::ModelDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_ModelDefinition_getUseScaleOf00
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinition_getUseScaleOf00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::ModelDefinition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::ModelDefinition* self = (EmberOgre::Model::ModelDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getUseScaleOf'",NULL);
#endif
  {
   EmberOgre::Model::ModelDefinition::UseScaleOf tolua_ret = (EmberOgre::Model::ModelDefinition::UseScaleOf)  self->getUseScaleOf();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getUseScaleOf'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setUseScaleOf of class  EmberOgre::Model::ModelDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_ModelDefinition_setUseScaleOf00
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinition_setUseScaleOf00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::ModelDefinition",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::ModelDefinition* self = (EmberOgre::Model::ModelDefinition*)  tolua_tousertype(tolua_S,1,0);
  EmberOgre::Model::ModelDefinition::UseScaleOf useScale = ((EmberOgre::Model::ModelDefinition::UseScaleOf) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setUseScaleOf'",NULL);
#endif
  {
   self->setUseScaleOf(useScale);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setUseScaleOf'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTranslate of class  EmberOgre::Model::ModelDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_ModelDefinition_getTranslate00
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinition_getTranslate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::ModelDefinition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::ModelDefinition* self = (EmberOgre::Model::ModelDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTranslate'",NULL);
#endif
  {
   Ogre::Vector3& tolua_ret = (Ogre::Vector3&)  self->getTranslate();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"Ogre::Vector3");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTranslate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setTranslate of class  EmberOgre::Model::ModelDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_ModelDefinition_setTranslate00
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinition_setTranslate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::ModelDefinition",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::ModelDefinition* self = (EmberOgre::Model::ModelDefinition*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector3 translate = *((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setTranslate'",NULL);
#endif
  {
   self->setTranslate(translate);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setTranslate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getShowContained of class  EmberOgre::Model::ModelDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_ModelDefinition_getShowContained00
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinition_getShowContained00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::ModelDefinition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::ModelDefinition* self = (EmberOgre::Model::ModelDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getShowContained'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->getShowContained();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getShowContained'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getShowContained of class  EmberOgre::Model::ModelDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_ModelDefinition_getShowContained01
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinition_getShowContained01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::ModelDefinition",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  EmberOgre::Model::ModelDefinition* self = (EmberOgre::Model::ModelDefinition*)  tolua_tousertype(tolua_S,1,0);
  bool show = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getShowContained'",NULL);
#endif
  {
   self->getShowContained(show);
  }
 }
 return 0;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_Model_ModelDefinition_getShowContained00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getContentOffset of class  EmberOgre::Model::ModelDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_ModelDefinition_getContentOffset00
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinition_getContentOffset00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Model::ModelDefinition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Model::ModelDefinition* self = (const EmberOgre::Model::ModelDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getContentOffset'",NULL);
#endif
  {
   const Ogre::Vector3& tolua_ret = (const Ogre::Vector3&)  self->getContentOffset();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Ogre::Vector3");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getContentOffset'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setContentOffset of class  EmberOgre::Model::ModelDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_ModelDefinition_setContentOffset00
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinition_setContentOffset00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::ModelDefinition",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::ModelDefinition* self = (EmberOgre::Model::ModelDefinition*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector3* tolua_var_1 = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setContentOffset'",NULL);
#endif
  {
   self->setContentOffset(*tolua_var_1);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setContentOffset'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRotation of class  EmberOgre::Model::ModelDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_ModelDefinition_getRotation00
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinition_getRotation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Model::ModelDefinition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Model::ModelDefinition* self = (const EmberOgre::Model::ModelDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRotation'",NULL);
#endif
  {
   const Ogre::Quaternion& tolua_ret = (const Ogre::Quaternion&)  self->getRotation();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Ogre::Quaternion");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRotation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setRotation of class  EmberOgre::Model::ModelDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_ModelDefinition_setRotation00
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinition_setRotation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::ModelDefinition",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::ModelDefinition* self = (EmberOgre::Model::ModelDefinition*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Quaternion rotation = *((const Ogre::Quaternion*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRotation'",NULL);
#endif
  {
   self->setRotation(rotation);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setRotation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createSubModelDefinition of class  EmberOgre::Model::ModelDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_ModelDefinition_createSubModelDefinition00
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinition_createSubModelDefinition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::ModelDefinition",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::ModelDefinition* self = (EmberOgre::Model::ModelDefinition*)  tolua_tousertype(tolua_S,1,0);
  std::string meshname = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createSubModelDefinition'",NULL);
#endif
  {
   EmberOgre::Model::SubModelDefinition* tolua_ret = (EmberOgre::Model::SubModelDefinition*)  self->createSubModelDefinition(meshname);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Model::SubModelDefinition");
   tolua_pushcppstring(tolua_S,(const char*)meshname);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createSubModelDefinition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSubModelDefinitions of class  EmberOgre::Model::ModelDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_ModelDefinition_getSubModelDefinitions00
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinition_getSubModelDefinitions00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::ModelDefinition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::ModelDefinition* self = (EmberOgre::Model::ModelDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSubModelDefinitions'",NULL);
#endif
  {
   std::vector<EmberOgre::Model::SubModelDefinition*>& tolua_ret = (std::vector<EmberOgre::Model::SubModelDefinition*>&)  self->getSubModelDefinitions();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"std::vector<EmberOgre::Model::SubModelDefinition*>");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSubModelDefinitions'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeSubModelDefinition of class  EmberOgre::Model::ModelDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_ModelDefinition_removeSubModelDefinition00
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinition_removeSubModelDefinition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::ModelDefinition",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"EmberOgre::Model::SubModelDefinition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::ModelDefinition* self = (EmberOgre::Model::ModelDefinition*)  tolua_tousertype(tolua_S,1,0);
  EmberOgre::Model::SubModelDefinition* def = ((EmberOgre::Model::SubModelDefinition*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeSubModelDefinition'",NULL);
#endif
  {
   self->removeSubModelDefinition(def);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeSubModelDefinition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createActionDefinition of class  EmberOgre::Model::ModelDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_ModelDefinition_createActionDefinition00
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinition_createActionDefinition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::ModelDefinition",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::ModelDefinition* self = (EmberOgre::Model::ModelDefinition*)  tolua_tousertype(tolua_S,1,0);
  std::string actionname = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createActionDefinition'",NULL);
#endif
  {
   EmberOgre::Model::ActionDefinition* tolua_ret = (EmberOgre::Model::ActionDefinition*)  self->createActionDefinition(actionname);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Model::ActionDefinition");
   tolua_pushcppstring(tolua_S,(const char*)actionname);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createActionDefinition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getActionDefinitions of class  EmberOgre::Model::ModelDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_ModelDefinition_getActionDefinitions00
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinition_getActionDefinitions00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::ModelDefinition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::ModelDefinition* self = (EmberOgre::Model::ModelDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getActionDefinitions'",NULL);
#endif
  {
   std::vector<EmberOgre::Model::ActionDefinition*>& tolua_ret = (std::vector<EmberOgre::Model::ActionDefinition*>&)  self->getActionDefinitions();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"std::vector<EmberOgre::Model::ActionDefinition*>");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getActionDefinitions'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeActionDefinition of class  EmberOgre::Model::ModelDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_ModelDefinition_removeActionDefinition00
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinition_removeActionDefinition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::ModelDefinition",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"EmberOgre::Model::ActionDefinition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::ModelDefinition* self = (EmberOgre::Model::ModelDefinition*)  tolua_tousertype(tolua_S,1,0);
  EmberOgre::Model::ActionDefinition* def = ((EmberOgre::Model::ActionDefinition*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeActionDefinition'",NULL);
#endif
  {
   self->removeActionDefinition(def);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeActionDefinition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAttachPointsDefinitions of class  EmberOgre::Model::ModelDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_ModelDefinition_getAttachPointsDefinitions00
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinition_getAttachPointsDefinitions00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::ModelDefinition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::ModelDefinition* self = (EmberOgre::Model::ModelDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAttachPointsDefinitions'",NULL);
#endif
  {
   const std::vector<EmberOgre::Model::AttachPointDefinition>& tolua_ret = (const std::vector<EmberOgre::Model::AttachPointDefinition>&)  self->getAttachPointsDefinitions();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const std::vector<EmberOgre::Model::AttachPointDefinition>");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAttachPointsDefinitions'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createViewDefinition of class  EmberOgre::Model::ModelDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_ModelDefinition_createViewDefinition00
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinition_createViewDefinition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::ModelDefinition",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::ModelDefinition* self = (EmberOgre::Model::ModelDefinition*)  tolua_tousertype(tolua_S,1,0);
  const std::string viewname = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createViewDefinition'",NULL);
#endif
  {
   EmberOgre::Model::ViewDefinition* tolua_ret = (EmberOgre::Model::ViewDefinition*)  self->createViewDefinition(viewname);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Model::ViewDefinition");
   tolua_pushcppstring(tolua_S,(const char*)viewname);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createViewDefinition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getViewDefinitions of class  EmberOgre::Model::ModelDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_ModelDefinition_getViewDefinitions00
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinition_getViewDefinitions00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::ModelDefinition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::ModelDefinition* self = (EmberOgre::Model::ModelDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getViewDefinitions'",NULL);
#endif
  {
    const std::map<std::string,EmberOgre::Model::ViewDefinition*>& tolua_ret = (  const std::map<std::string,EmberOgre::Model::ViewDefinition*>&)  self->getViewDefinitions();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const std::map<std::string,EmberOgre::Model::ViewDefinition*>");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getViewDefinitions'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeViewDefinition of class  EmberOgre::Model::ModelDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_ModelDefinition_removeViewDefinition00
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinition_removeViewDefinition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::ModelDefinition",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::ModelDefinition* self = (EmberOgre::Model::ModelDefinition*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeViewDefinition'",NULL);
#endif
  {
   self->removeViewDefinition(name);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeViewDefinition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: reloadAllInstances of class  EmberOgre::Model::ModelDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_ModelDefinition_reloadAllInstances00
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinition_reloadAllInstances00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::ModelDefinition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::ModelDefinition* self = (EmberOgre::Model::ModelDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'reloadAllInstances'",NULL);
#endif
  {
   self->reloadAllInstances();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'reloadAllInstances'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get of class  EmberOgre::Model::ModelDefnPtr */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_ModelDefnPtr_get00
static int tolua_EmberOgre_EmberOgre_Model_ModelDefnPtr_get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::ModelDefnPtr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::ModelDefnPtr* self = (EmberOgre::Model::ModelDefnPtr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get'",NULL);
#endif
  {
   EmberOgre::Model::ModelDefinition* tolua_ret = (EmberOgre::Model::ModelDefinition*)  self->get();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Model::ModelDefinition");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: xPosition of class  EmberOgre::MouseMotion */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__MouseMotion_xPosition
static int tolua_get_EmberOgre__MouseMotion_xPosition(lua_State* tolua_S)
{
  EmberOgre::MouseMotion* self = (EmberOgre::MouseMotion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'xPosition'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->xPosition);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: xPosition of class  EmberOgre::MouseMotion */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__MouseMotion_xPosition
static int tolua_set_EmberOgre__MouseMotion_xPosition(lua_State* tolua_S)
{
  EmberOgre::MouseMotion* self = (EmberOgre::MouseMotion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'xPosition'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->xPosition = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: yPosition of class  EmberOgre::MouseMotion */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__MouseMotion_yPosition
static int tolua_get_EmberOgre__MouseMotion_yPosition(lua_State* tolua_S)
{
  EmberOgre::MouseMotion* self = (EmberOgre::MouseMotion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'yPosition'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->yPosition);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: yPosition of class  EmberOgre::MouseMotion */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__MouseMotion_yPosition
static int tolua_set_EmberOgre__MouseMotion_yPosition(lua_State* tolua_S)
{
  EmberOgre::MouseMotion* self = (EmberOgre::MouseMotion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'yPosition'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->yPosition = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: xRelativeMovement of class  EmberOgre::MouseMotion */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__MouseMotion_xRelativeMovement
static int tolua_get_EmberOgre__MouseMotion_xRelativeMovement(lua_State* tolua_S)
{
  EmberOgre::MouseMotion* self = (EmberOgre::MouseMotion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'xRelativeMovement'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->xRelativeMovement,"Ogre::Real");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: xRelativeMovement of class  EmberOgre::MouseMotion */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__MouseMotion_xRelativeMovement
static int tolua_set_EmberOgre__MouseMotion_xRelativeMovement(lua_State* tolua_S)
{
  EmberOgre::MouseMotion* self = (EmberOgre::MouseMotion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'xRelativeMovement'",NULL);
  if (!tolua_isusertype(tolua_S,2,"Ogre::Real",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->xRelativeMovement = *((Ogre::Real*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: yRelativeMovement of class  EmberOgre::MouseMotion */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__MouseMotion_yRelativeMovement
static int tolua_get_EmberOgre__MouseMotion_yRelativeMovement(lua_State* tolua_S)
{
  EmberOgre::MouseMotion* self = (EmberOgre::MouseMotion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'yRelativeMovement'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->yRelativeMovement,"Ogre::Real");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: yRelativeMovement of class  EmberOgre::MouseMotion */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__MouseMotion_yRelativeMovement
static int tolua_set_EmberOgre__MouseMotion_yRelativeMovement(lua_State* tolua_S)
{
  EmberOgre::MouseMotion* self = (EmberOgre::MouseMotion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'yRelativeMovement'",NULL);
  if (!tolua_isusertype(tolua_S,2,"Ogre::Real",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->yRelativeMovement = *((Ogre::Real*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: xRelativeMovementInPixels of class  EmberOgre::MouseMotion */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__MouseMotion_xRelativeMovementInPixels
static int tolua_get_EmberOgre__MouseMotion_xRelativeMovementInPixels(lua_State* tolua_S)
{
  EmberOgre::MouseMotion* self = (EmberOgre::MouseMotion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'xRelativeMovementInPixels'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->xRelativeMovementInPixels);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: xRelativeMovementInPixels of class  EmberOgre::MouseMotion */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__MouseMotion_xRelativeMovementInPixels
static int tolua_set_EmberOgre__MouseMotion_xRelativeMovementInPixels(lua_State* tolua_S)
{
  EmberOgre::MouseMotion* self = (EmberOgre::MouseMotion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'xRelativeMovementInPixels'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->xRelativeMovementInPixels = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: yRelativeMovementInPixels of class  EmberOgre::MouseMotion */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__MouseMotion_yRelativeMovementInPixels
static int tolua_get_EmberOgre__MouseMotion_yRelativeMovementInPixels(lua_State* tolua_S)
{
  EmberOgre::MouseMotion* self = (EmberOgre::MouseMotion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'yRelativeMovementInPixels'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->yRelativeMovementInPixels);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: yRelativeMovementInPixels of class  EmberOgre::MouseMotion */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__MouseMotion_yRelativeMovementInPixels
static int tolua_set_EmberOgre__MouseMotion_yRelativeMovementInPixels(lua_State* tolua_S)
{
  EmberOgre::MouseMotion* self = (EmberOgre::MouseMotion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'yRelativeMovementInPixels'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->yRelativeMovementInPixels = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: timeSinceLastMovement of class  EmberOgre::MouseMotion */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__MouseMotion_timeSinceLastMovement
static int tolua_get_EmberOgre__MouseMotion_timeSinceLastMovement(lua_State* tolua_S)
{
  EmberOgre::MouseMotion* self = (EmberOgre::MouseMotion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'timeSinceLastMovement'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->timeSinceLastMovement,"Ogre::Real");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: timeSinceLastMovement of class  EmberOgre::MouseMotion */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__MouseMotion_timeSinceLastMovement
static int tolua_set_EmberOgre__MouseMotion_timeSinceLastMovement(lua_State* tolua_S)
{
  EmberOgre::MouseMotion* self = (EmberOgre::MouseMotion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'timeSinceLastMovement'",NULL);
  if (!tolua_isusertype(tolua_S,2,"Ogre::Real",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->timeSinceLastMovement = *((Ogre::Real*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSingleton of class  EmberOgre::Input */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Input_getSingleton00
static int tolua_EmberOgre_EmberOgre_Input_getSingleton00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Input",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   EmberOgre::Input& tolua_ret = (EmberOgre::Input&)  EmberOgre::Input::getSingleton();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"EmberOgre::Input");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSingleton'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventKeyPressed of class  EmberOgre::Input */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__Input_EventKeyPressed
static int tolua_get_EmberOgre__Input_EventKeyPressed(lua_State* tolua_S)
{
  EmberOgre::Input* self = (EmberOgre::Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventKeyPressed'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventKeyPressed,"sigc::signal<void,const SDL_keysym&,EmberOgre::Input::InputMode>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventKeyPressed of class  EmberOgre::Input */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__Input_EventKeyPressed
static int tolua_set_EmberOgre__Input_EventKeyPressed(lua_State* tolua_S)
{
  EmberOgre::Input* self = (EmberOgre::Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventKeyPressed'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,const SDL_keysym&,EmberOgre::Input::InputMode>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventKeyPressed = *((sigc::signal<void,const SDL_keysym&,EmberOgre::Input::InputMode>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventKeyReleased of class  EmberOgre::Input */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__Input_EventKeyReleased
static int tolua_get_EmberOgre__Input_EventKeyReleased(lua_State* tolua_S)
{
  EmberOgre::Input* self = (EmberOgre::Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventKeyReleased'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventKeyReleased,"sigc::signal<void,const SDL_keysym&,EmberOgre::Input::InputMode>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventKeyReleased of class  EmberOgre::Input */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__Input_EventKeyReleased
static int tolua_set_EmberOgre__Input_EventKeyReleased(lua_State* tolua_S)
{
  EmberOgre::Input* self = (EmberOgre::Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventKeyReleased'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,const SDL_keysym&,EmberOgre::Input::InputMode>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventKeyReleased = *((sigc::signal<void,const SDL_keysym&,EmberOgre::Input::InputMode>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventMouseMoved of class  EmberOgre::Input */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__Input_EventMouseMoved
static int tolua_get_EmberOgre__Input_EventMouseMoved(lua_State* tolua_S)
{
  EmberOgre::Input* self = (EmberOgre::Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventMouseMoved'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventMouseMoved,"sigc::signal<void,const EmberOgre::MouseMotion&,EmberOgre::Input::InputMode>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventMouseMoved of class  EmberOgre::Input */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__Input_EventMouseMoved
static int tolua_set_EmberOgre__Input_EventMouseMoved(lua_State* tolua_S)
{
  EmberOgre::Input* self = (EmberOgre::Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventMouseMoved'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,const EmberOgre::MouseMotion&,EmberOgre::Input::InputMode>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventMouseMoved = *((sigc::signal<void,const EmberOgre::MouseMotion&,EmberOgre::Input::InputMode>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventMouseButtonPressed of class  EmberOgre::Input */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__Input_EventMouseButtonPressed
static int tolua_get_EmberOgre__Input_EventMouseButtonPressed(lua_State* tolua_S)
{
  EmberOgre::Input* self = (EmberOgre::Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventMouseButtonPressed'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventMouseButtonPressed,"sigc::signal<void,EmberOgre::Input::MouseButton,EmberOgre::Input::InputMode>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventMouseButtonPressed of class  EmberOgre::Input */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__Input_EventMouseButtonPressed
static int tolua_set_EmberOgre__Input_EventMouseButtonPressed(lua_State* tolua_S)
{
  EmberOgre::Input* self = (EmberOgre::Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventMouseButtonPressed'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,EmberOgre::Input::MouseButton,EmberOgre::Input::InputMode>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventMouseButtonPressed = *((sigc::signal<void,EmberOgre::Input::MouseButton,EmberOgre::Input::InputMode>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventMouseButtonReleased of class  EmberOgre::Input */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__Input_EventMouseButtonReleased
static int tolua_get_EmberOgre__Input_EventMouseButtonReleased(lua_State* tolua_S)
{
  EmberOgre::Input* self = (EmberOgre::Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventMouseButtonReleased'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventMouseButtonReleased,"sigc::signal<void,EmberOgre::Input::MouseButton,EmberOgre::Input::InputMode>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventMouseButtonReleased of class  EmberOgre::Input */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__Input_EventMouseButtonReleased
static int tolua_set_EmberOgre__Input_EventMouseButtonReleased(lua_State* tolua_S)
{
  EmberOgre::Input* self = (EmberOgre::Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventMouseButtonReleased'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,EmberOgre::Input::MouseButton,EmberOgre::Input::InputMode>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventMouseButtonReleased = *((sigc::signal<void,EmberOgre::Input::MouseButton,EmberOgre::Input::InputMode>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventChangedInputMode of class  EmberOgre::Input */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__Input_EventChangedInputMode
static int tolua_get_EmberOgre__Input_EventChangedInputMode(lua_State* tolua_S)
{
  EmberOgre::Input* self = (EmberOgre::Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventChangedInputMode'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventChangedInputMode,"sigc::signal<void,EmberOgre::Input::InputMode>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventChangedInputMode of class  EmberOgre::Input */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__Input_EventChangedInputMode
static int tolua_set_EmberOgre__Input_EventChangedInputMode(lua_State* tolua_S)
{
  EmberOgre::Input* self = (EmberOgre::Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventChangedInputMode'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,EmberOgre::Input::InputMode>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventChangedInputMode = *((sigc::signal<void,EmberOgre::Input::InputMode>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: isKeyDown of class  EmberOgre::Input */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Input_isKeyDown00
static int tolua_EmberOgre_EmberOgre_Input_isKeyDown00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Input",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const SDLKey",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Input* self = (const EmberOgre::Input*)  tolua_tousertype(tolua_S,1,0);
  const SDLKey* tolua_var_2 = ((const SDLKey*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isKeyDown'",NULL);
#endif
  {
   const bool tolua_ret = (const bool)  self->isKeyDown(*tolua_var_2);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isKeyDown'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setInputMode of class  EmberOgre::Input */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Input_setInputMode00
static int tolua_EmberOgre_EmberOgre_Input_setInputMode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Input",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Input* self = (EmberOgre::Input*)  tolua_tousertype(tolua_S,1,0);
  EmberOgre::Input::InputMode mode = ((EmberOgre::Input::InputMode) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setInputMode'",NULL);
#endif
  {
   self->setInputMode(mode);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setInputMode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getInputMode of class  EmberOgre::Input */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Input_getInputMode00
static int tolua_EmberOgre_EmberOgre_Input_getInputMode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Input",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Input* self = (const EmberOgre::Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getInputMode'",NULL);
#endif
  {
   EmberOgre::Input::InputMode tolua_ret = (EmberOgre::Input::InputMode)  self->getInputMode();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInputMode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: toggleInputMode of class  EmberOgre::Input */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Input_toggleInputMode00
static int tolua_EmberOgre_EmberOgre_Input_toggleInputMode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Input",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Input* self = (EmberOgre::Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'toggleInputMode'",NULL);
#endif
  {
   EmberOgre::Input::InputMode tolua_ret = (EmberOgre::Input::InputMode)  self->toggleInputMode();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'toggleInputMode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::Jesus */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Jesus_new00
static int tolua_EmberOgre_EmberOgre_Jesus_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Jesus",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Carpenter::Carpenter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Carpenter::Carpenter* carpenter = ((Carpenter::Carpenter*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::Jesus* tolua_ret = (EmberOgre::Jesus*)  new EmberOgre::Jesus(carpenter);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Jesus");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::Jesus */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Jesus_new00_local
static int tolua_EmberOgre_EmberOgre_Jesus_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Jesus",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Carpenter::Carpenter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Carpenter::Carpenter* carpenter = ((Carpenter::Carpenter*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::Jesus* tolua_ret = (EmberOgre::Jesus*)  new EmberOgre::Jesus(carpenter);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Jesus");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  EmberOgre::Jesus */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Jesus_delete00
static int tolua_EmberOgre_EmberOgre_Jesus_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Jesus",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Jesus* self = (EmberOgre::Jesus*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createModelForBlockType of class  EmberOgre::Jesus */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Jesus_createModelForBlockType00
static int tolua_EmberOgre_EmberOgre_Jesus_createModelForBlockType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Jesus",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Jesus* self = (EmberOgre::Jesus*)  tolua_tousertype(tolua_S,1,0);
  const std::string blockType = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string modelName = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createModelForBlockType'",NULL);
#endif
  {
   EmberOgre::Model::Model* tolua_ret = (EmberOgre::Model::Model*)  self->createModelForBlockType(blockType,modelName);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Model::Model");
   tolua_pushcppstring(tolua_S,(const char*)blockType);
   tolua_pushcppstring(tolua_S,(const char*)modelName);
  }
 }
 return 3;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createModelForBlockType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getColourForAttachPoint of class  EmberOgre::Jesus */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Jesus_getColourForAttachPoint00
static int tolua_EmberOgre_EmberOgre_Jesus_getColourForAttachPoint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Jesus",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Carpenter::AttachPoint",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Jesus* self = (const EmberOgre::Jesus*)  tolua_tousertype(tolua_S,1,0);
  const Carpenter::AttachPoint* point = ((const Carpenter::AttachPoint*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getColourForAttachPoint'",NULL);
#endif
  {
   Ogre::ColourValue tolua_ret = (Ogre::ColourValue)  self->getColourForAttachPoint(point);
   {
#ifdef __cplusplus
    void* tolua_obj = new Ogre::ColourValue(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::ColourValue");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::ColourValue));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::ColourValue");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getColourForAttachPoint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getCarpenter of class  EmberOgre::Jesus */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Jesus_getCarpenter00
static int tolua_EmberOgre_EmberOgre_Jesus_getCarpenter00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Jesus",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Jesus* self = (const EmberOgre::Jesus*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCarpenter'",NULL);
#endif
  {
   Carpenter::Carpenter* tolua_ret = (Carpenter::Carpenter*)  self->getCarpenter();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Carpenter::Carpenter");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getCarpenter'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addBluePrint of class  EmberOgre::Jesus */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Jesus_addBluePrint00
static int tolua_EmberOgre_EmberOgre_Jesus_addBluePrint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Jesus",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Carpenter::BluePrint",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Jesus* self = (EmberOgre::Jesus*)  tolua_tousertype(tolua_S,1,0);
  Carpenter::BluePrint* blueprint = ((Carpenter::BluePrint*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addBluePrint'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->addBluePrint(blueprint);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addBluePrint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getBluePrint of class  EmberOgre::Jesus */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Jesus_getBluePrint00
static int tolua_EmberOgre_EmberOgre_Jesus_getBluePrint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Jesus",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Jesus* self = (const EmberOgre::Jesus*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getBluePrint'",NULL);
#endif
  {
   Carpenter::BluePrint* tolua_ret = (Carpenter::BluePrint*)  self->getBluePrint(name);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Carpenter::BluePrint");
   tolua_pushcppstring(tolua_S,(const char*)name);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getBluePrint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAllBluePrints of class  EmberOgre::Jesus */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Jesus_getAllBluePrints00
static int tolua_EmberOgre_EmberOgre_Jesus_getAllBluePrints00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Jesus",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Jesus* self = (const EmberOgre::Jesus*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAllBluePrints'",NULL);
#endif
  {
   const std::map<std::string,Carpenter::BluePrint*>* tolua_ret = (const std::map<std::string,Carpenter::BluePrint*>*)  self->getAllBluePrints();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"const std::map<std::string,Carpenter::BluePrint*>");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAllBluePrints'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::LightWibbler */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_LightWibbler_new00
static int tolua_EmberOgre_EmberOgre_LightWibbler_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::LightWibbler",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::ColourValue",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"Ogre::Billboard",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::ColourValue* originalColour = ((const Ogre::ColourValue*)  tolua_tousertype(tolua_S,2,0));
  Ogre::Billboard* billboard = ((Ogre::Billboard*)  tolua_tousertype(tolua_S,3,0));
  {
   EmberOgre::LightWibbler* tolua_ret = (EmberOgre::LightWibbler*)  new EmberOgre::LightWibbler(*originalColour,billboard);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::LightWibbler");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::LightWibbler */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_LightWibbler_new00_local
static int tolua_EmberOgre_EmberOgre_LightWibbler_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::LightWibbler",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::ColourValue",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"Ogre::Billboard",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::ColourValue* originalColour = ((const Ogre::ColourValue*)  tolua_tousertype(tolua_S,2,0));
  Ogre::Billboard* billboard = ((Ogre::Billboard*)  tolua_tousertype(tolua_S,3,0));
  {
   EmberOgre::LightWibbler* tolua_ret = (EmberOgre::LightWibbler*)  new EmberOgre::LightWibbler(*originalColour,billboard);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::LightWibbler");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getValue of class  EmberOgre::LightWibbler */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_LightWibbler_getValue00
static int tolua_EmberOgre_EmberOgre_LightWibbler_getValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::LightWibbler",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::LightWibbler* self = (const EmberOgre::LightWibbler*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getValue'",NULL);
#endif
  {
   Ogre::Real tolua_ret = (Ogre::Real)  self->getValue();
   {
#ifdef __cplusplus
    void* tolua_obj = new Ogre::Real(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Real");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::Real));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Real");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getValue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setValue of class  EmberOgre::LightWibbler */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_LightWibbler_setValue00
static int tolua_EmberOgre_EmberOgre_LightWibbler_setValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::LightWibbler",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::Real",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::LightWibbler* self = (EmberOgre::LightWibbler*)  tolua_tousertype(tolua_S,1,0);
  Ogre::Real value = *((Ogre::Real*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setValue'",NULL);
#endif
  {
   self->setValue(value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setValue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::AttachPointNode */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_AttachPointNode_new00
static int tolua_EmberOgre_EmberOgre_AttachPointNode_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::AttachPointNode",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"EmberOgre::ModelBlock",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"const Carpenter::AttachPoint",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"Ogre::ColourValue",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,6,"Ogre::BillboardSet",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::ModelBlock* modelBlock = ((EmberOgre::ModelBlock*)  tolua_tousertype(tolua_S,2,0));
  Ogre::SceneNode* modelNode = ((Ogre::SceneNode*)  tolua_tousertype(tolua_S,3,0));
  const Carpenter::AttachPoint* attachPoint = ((const Carpenter::AttachPoint*)  tolua_tousertype(tolua_S,4,0));
  Ogre::ColourValue colour = *((Ogre::ColourValue*)  tolua_tousertype(tolua_S,5,0));
  Ogre::BillboardSet* billboardSet = ((Ogre::BillboardSet*)  tolua_tousertype(tolua_S,6,0));
  {
   EmberOgre::AttachPointNode* tolua_ret = (EmberOgre::AttachPointNode*)  new EmberOgre::AttachPointNode(modelBlock,modelNode,attachPoint,colour,billboardSet);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::AttachPointNode");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::AttachPointNode */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_AttachPointNode_new00_local
static int tolua_EmberOgre_EmberOgre_AttachPointNode_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::AttachPointNode",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"EmberOgre::ModelBlock",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"const Carpenter::AttachPoint",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"Ogre::ColourValue",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,6,"Ogre::BillboardSet",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::ModelBlock* modelBlock = ((EmberOgre::ModelBlock*)  tolua_tousertype(tolua_S,2,0));
  Ogre::SceneNode* modelNode = ((Ogre::SceneNode*)  tolua_tousertype(tolua_S,3,0));
  const Carpenter::AttachPoint* attachPoint = ((const Carpenter::AttachPoint*)  tolua_tousertype(tolua_S,4,0));
  Ogre::ColourValue colour = *((Ogre::ColourValue*)  tolua_tousertype(tolua_S,5,0));
  Ogre::BillboardSet* billboardSet = ((Ogre::BillboardSet*)  tolua_tousertype(tolua_S,6,0));
  {
   EmberOgre::AttachPointNode* tolua_ret = (EmberOgre::AttachPointNode*)  new EmberOgre::AttachPointNode(modelBlock,modelNode,attachPoint,colour,billboardSet);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::AttachPointNode");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  EmberOgre::AttachPointNode */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_AttachPointNode_delete00
static int tolua_EmberOgre_EmberOgre_AttachPointNode_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::AttachPointNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::AttachPointNode* self = (EmberOgre::AttachPointNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: select of class  EmberOgre::AttachPointNode */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_AttachPointNode_select00
static int tolua_EmberOgre_EmberOgre_AttachPointNode_select00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::AttachPointNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::AttachPointNode* self = (EmberOgre::AttachPointNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'select'",NULL);
#endif
  {
   self->select();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'select'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: deselect of class  EmberOgre::AttachPointNode */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_AttachPointNode_deselect00
static int tolua_EmberOgre_EmberOgre_AttachPointNode_deselect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::AttachPointNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::AttachPointNode* self = (EmberOgre::AttachPointNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'deselect'",NULL);
#endif
  {
   self->deselect();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'deselect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAttachPoint of class  EmberOgre::AttachPointNode */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_AttachPointNode_getAttachPoint00
static int tolua_EmberOgre_EmberOgre_AttachPointNode_getAttachPoint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::AttachPointNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::AttachPointNode* self = (EmberOgre::AttachPointNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAttachPoint'",NULL);
#endif
  {
   const Carpenter::AttachPoint* tolua_ret = (const Carpenter::AttachPoint*)  self->getAttachPoint();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"const Carpenter::AttachPoint");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAttachPoint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::ModelBlock */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_ModelBlock_new00
static int tolua_EmberOgre_EmberOgre_ModelBlock_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::ModelBlock",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Carpenter::BuildingBlock",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"EmberOgre::Model::Model",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"EmberOgre::Construction",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::SceneNode* baseNode = ((Ogre::SceneNode*)  tolua_tousertype(tolua_S,2,0));
  const Carpenter::BuildingBlock* buildingBlock = ((const Carpenter::BuildingBlock*)  tolua_tousertype(tolua_S,3,0));
  EmberOgre::Model::Model* model = ((EmberOgre::Model::Model*)  tolua_tousertype(tolua_S,4,0));
  EmberOgre::Construction* construction = ((EmberOgre::Construction*)  tolua_tousertype(tolua_S,5,0));
  {
   EmberOgre::ModelBlock* tolua_ret = (EmberOgre::ModelBlock*)  new EmberOgre::ModelBlock(baseNode,buildingBlock,model,construction);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::ModelBlock");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::ModelBlock */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_ModelBlock_new00_local
static int tolua_EmberOgre_EmberOgre_ModelBlock_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::ModelBlock",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Carpenter::BuildingBlock",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"EmberOgre::Model::Model",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"EmberOgre::Construction",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::SceneNode* baseNode = ((Ogre::SceneNode*)  tolua_tousertype(tolua_S,2,0));
  const Carpenter::BuildingBlock* buildingBlock = ((const Carpenter::BuildingBlock*)  tolua_tousertype(tolua_S,3,0));
  EmberOgre::Model::Model* model = ((EmberOgre::Model::Model*)  tolua_tousertype(tolua_S,4,0));
  EmberOgre::Construction* construction = ((EmberOgre::Construction*)  tolua_tousertype(tolua_S,5,0));
  {
   EmberOgre::ModelBlock* tolua_ret = (EmberOgre::ModelBlock*)  new EmberOgre::ModelBlock(baseNode,buildingBlock,model,construction);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::ModelBlock");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  EmberOgre::ModelBlock */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_ModelBlock_delete00
static int tolua_EmberOgre_EmberOgre_ModelBlock_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::ModelBlock",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::ModelBlock* self = (EmberOgre::ModelBlock*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getBuildingBlock of class  EmberOgre::ModelBlock */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_ModelBlock_getBuildingBlock00
static int tolua_EmberOgre_EmberOgre_ModelBlock_getBuildingBlock00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::ModelBlock",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::ModelBlock* self = (EmberOgre::ModelBlock*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getBuildingBlock'",NULL);
#endif
  {
   const Carpenter::BuildingBlock* tolua_ret = (const Carpenter::BuildingBlock*)  self->getBuildingBlock();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"const Carpenter::BuildingBlock");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getBuildingBlock'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getConstruction of class  EmberOgre::ModelBlock */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_ModelBlock_getConstruction00
static int tolua_EmberOgre_EmberOgre_ModelBlock_getConstruction00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::ModelBlock",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::ModelBlock* self = (const EmberOgre::ModelBlock*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getConstruction'",NULL);
#endif
  {
   EmberOgre::Construction* tolua_ret = (EmberOgre::Construction*)  self->getConstruction();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Construction");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getConstruction'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createAttachPointNodes of class  EmberOgre::ModelBlock */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_ModelBlock_createAttachPointNodes00
static int tolua_EmberOgre_EmberOgre_ModelBlock_createAttachPointNodes00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::ModelBlock",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::ModelBlock* self = (EmberOgre::ModelBlock*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createAttachPointNodes'",NULL);
#endif
  {
   self->createAttachPointNodes();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createAttachPointNodes'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: select of class  EmberOgre::ModelBlock */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_ModelBlock_select00
static int tolua_EmberOgre_EmberOgre_ModelBlock_select00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::ModelBlock",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::ModelBlock* self = (EmberOgre::ModelBlock*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'select'",NULL);
#endif
  {
   self->select();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'select'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: deselect of class  EmberOgre::ModelBlock */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_ModelBlock_deselect00
static int tolua_EmberOgre_EmberOgre_ModelBlock_deselect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::ModelBlock",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::ModelBlock* self = (EmberOgre::ModelBlock*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'deselect'",NULL);
#endif
  {
   self->deselect();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'deselect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAttachPointNodes of class  EmberOgre::ModelBlock */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_ModelBlock_getAttachPointNodes00
static int tolua_EmberOgre_EmberOgre_ModelBlock_getAttachPointNodes00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::ModelBlock",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::ModelBlock* self = (const EmberOgre::ModelBlock*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAttachPointNodes'",NULL);
#endif
  {
   std::vector<EmberOgre::AttachPointNode*> tolua_ret = (std::vector<EmberOgre::AttachPointNode*>)  self->getAttachPointNodes();
   {
#ifdef __cplusplus
    void* tolua_obj = new std::vector<EmberOgre::AttachPointNode*>(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"std::vector<EmberOgre::AttachPointNode*>");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(std::vector<EmberOgre::AttachPointNode*>));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"std::vector<EmberOgre::AttachPointNode*>");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAttachPointNodes'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getModel of class  EmberOgre::ModelBlock */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_ModelBlock_getModel00
static int tolua_EmberOgre_EmberOgre_ModelBlock_getModel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::ModelBlock",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::ModelBlock* self = (const EmberOgre::ModelBlock*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getModel'",NULL);
#endif
  {
   const EmberOgre::Model::Model* tolua_ret = (const EmberOgre::Model::Model*)  self->getModel();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"const EmberOgre::Model::Model");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getModel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getNode of class  EmberOgre::ModelBlock */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_ModelBlock_getNode00
static int tolua_EmberOgre_EmberOgre_ModelBlock_getNode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::ModelBlock",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::ModelBlock* self = (const EmberOgre::ModelBlock*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getNode'",NULL);
#endif
  {
   const Ogre::SceneNode* tolua_ret = (const Ogre::SceneNode*)  self->getNode();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"const Ogre::SceneNode");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getNode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: model of class  EmberOgre::ModelMapping */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__ModelMapping_model
static int tolua_get_EmberOgre__ModelMapping_model(lua_State* tolua_S)
{
  EmberOgre::ModelMapping* self = (EmberOgre::ModelMapping*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'model'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->model);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: blocktype of class  EmberOgre::ModelMapping */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__ModelMapping_blocktype
static int tolua_get_EmberOgre__ModelMapping_blocktype(lua_State* tolua_S)
{
  EmberOgre::ModelMapping* self = (EmberOgre::ModelMapping*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'blocktype'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->blocktype);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::Construction */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Construction_new00
static int tolua_EmberOgre_EmberOgre_Construction_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Construction",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Carpenter::BluePrint",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"EmberOgre::Jesus",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Carpenter::BluePrint* blueprint = ((Carpenter::BluePrint*)  tolua_tousertype(tolua_S,2,0));
  EmberOgre::Jesus* jesus = ((EmberOgre::Jesus*)  tolua_tousertype(tolua_S,3,0));
  Ogre::SceneNode* node = ((Ogre::SceneNode*)  tolua_tousertype(tolua_S,4,0));
  {
   EmberOgre::Construction* tolua_ret = (EmberOgre::Construction*)  new EmberOgre::Construction(blueprint,jesus,node);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Construction");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::Construction */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Construction_new00_local
static int tolua_EmberOgre_EmberOgre_Construction_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Construction",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Carpenter::BluePrint",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"EmberOgre::Jesus",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Carpenter::BluePrint* blueprint = ((Carpenter::BluePrint*)  tolua_tousertype(tolua_S,2,0));
  EmberOgre::Jesus* jesus = ((EmberOgre::Jesus*)  tolua_tousertype(tolua_S,3,0));
  Ogre::SceneNode* node = ((Ogre::SceneNode*)  tolua_tousertype(tolua_S,4,0));
  {
   EmberOgre::Construction* tolua_ret = (EmberOgre::Construction*)  new EmberOgre::Construction(blueprint,jesus,node);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Construction");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  EmberOgre::Construction */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Construction_delete00
static int tolua_EmberOgre_EmberOgre_Construction_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Construction",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Construction* self = (EmberOgre::Construction*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getJesus of class  EmberOgre::Construction */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Construction_getJesus00
static int tolua_EmberOgre_EmberOgre_Construction_getJesus00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Construction",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Construction* self = (const EmberOgre::Construction*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getJesus'",NULL);
#endif
  {
   EmberOgre::Jesus* tolua_ret = (EmberOgre::Jesus*)  self->getJesus();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Jesus");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getJesus'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getBluePrint of class  EmberOgre::Construction */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Construction_getBluePrint00
static int tolua_EmberOgre_EmberOgre_Construction_getBluePrint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Construction",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Construction* self = (const EmberOgre::Construction*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getBluePrint'",NULL);
#endif
  {
   Carpenter::BluePrint* tolua_ret = (Carpenter::BluePrint*)  self->getBluePrint();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Carpenter::BluePrint");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getBluePrint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: buildFromBluePrint of class  EmberOgre::Construction */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Construction_buildFromBluePrint00
static int tolua_EmberOgre_EmberOgre_Construction_buildFromBluePrint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Construction",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Construction* self = (EmberOgre::Construction*)  tolua_tousertype(tolua_S,1,0);
  bool createAttachPointNodes = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'buildFromBluePrint'",NULL);
#endif
  {
   self->buildFromBluePrint(createAttachPointNodes);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'buildFromBluePrint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createModelBlock of class  EmberOgre::Construction */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Construction_createModelBlock00
static int tolua_EmberOgre_EmberOgre_Construction_createModelBlock00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Construction",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Carpenter::BuildingBlock",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Construction* self = (EmberOgre::Construction*)  tolua_tousertype(tolua_S,1,0);
  const Carpenter::BuildingBlock* buildingBlock = ((const Carpenter::BuildingBlock*)  tolua_tousertype(tolua_S,2,0));
  bool createAttachPointNodes = ((bool)  tolua_toboolean(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createModelBlock'",NULL);
#endif
  {
   EmberOgre::ModelBlock* tolua_ret = (EmberOgre::ModelBlock*)  self->createModelBlock(buildingBlock,createAttachPointNodes);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::ModelBlock");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createModelBlock'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getModelBlocks of class  EmberOgre::Construction */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Construction_getModelBlocks00
static int tolua_EmberOgre_EmberOgre_Construction_getModelBlocks00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Construction",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Construction* self = (const EmberOgre::Construction*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getModelBlocks'",NULL);
#endif
  {
   std::vector<EmberOgre::ModelBlock*> tolua_ret = (std::vector<EmberOgre::ModelBlock*>)  self->getModelBlocks();
   {
#ifdef __cplusplus
    void* tolua_obj = new std::vector<EmberOgre::ModelBlock*>(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"std::vector<EmberOgre::ModelBlock*>");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(std::vector<EmberOgre::ModelBlock*>));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"std::vector<EmberOgre::ModelBlock*>");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getModelBlocks'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: remove of class  EmberOgre::Construction */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Construction_remove00
static int tolua_EmberOgre_EmberOgre_Construction_remove00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Construction",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"EmberOgre::ModelBlock",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Construction* self = (EmberOgre::Construction*)  tolua_tousertype(tolua_S,1,0);
  EmberOgre::ModelBlock* modelBlock = ((EmberOgre::ModelBlock*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'remove'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->remove(modelBlock);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'remove'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isValid of class  EmberOgre::TerrainPosition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_TerrainPosition_isValid00
static int tolua_EmberOgre_EmberOgre_TerrainPosition_isValid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::TerrainPosition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::TerrainPosition* self = (const EmberOgre::TerrainPosition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isValid'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isValid();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isValid'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setToOrigin of class  EmberOgre::TerrainPosition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_TerrainPosition_setToOrigin00
static int tolua_EmberOgre_EmberOgre_TerrainPosition_setToOrigin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::TerrainPosition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::TerrainPosition* self = (EmberOgre::TerrainPosition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setToOrigin'",NULL);
#endif
  {
   EmberOgre::TerrainPosition& tolua_ret = (EmberOgre::TerrainPosition&)  self->setToOrigin();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"EmberOgre::TerrainPosition");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setToOrigin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: x of class  EmberOgre::TerrainPosition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_TerrainPosition_x00
static int tolua_EmberOgre_EmberOgre_TerrainPosition_x00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::TerrainPosition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::TerrainPosition* self = (const EmberOgre::TerrainPosition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'x'",NULL);
#endif
  {
   float tolua_ret = (float)  self->x();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'x'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: x of class  EmberOgre::TerrainPosition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_TerrainPosition_x01
static int tolua_EmberOgre_EmberOgre_TerrainPosition_x01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::TerrainPosition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  EmberOgre::TerrainPosition* self = (EmberOgre::TerrainPosition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'x'",NULL);
#endif
  {
   float tolua_ret = (float)  self->x();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_TerrainPosition_x00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: y of class  EmberOgre::TerrainPosition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_TerrainPosition_y00
static int tolua_EmberOgre_EmberOgre_TerrainPosition_y00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::TerrainPosition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::TerrainPosition* self = (const EmberOgre::TerrainPosition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'y'",NULL);
#endif
  {
   float tolua_ret = (float)  self->y();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'y'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: y of class  EmberOgre::TerrainPosition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_TerrainPosition_y01
static int tolua_EmberOgre_EmberOgre_TerrainPosition_y01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::TerrainPosition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  EmberOgre::TerrainPosition* self = (EmberOgre::TerrainPosition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'y'",NULL);
#endif
  {
   float tolua_ret = (float)  self->y();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_TerrainPosition_y00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* function: EmberOgre::Atlas2Ogre */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Atlas2Ogre00
static int tolua_EmberOgre_EmberOgre_Atlas2Ogre00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::TerrainPosition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::TerrainPosition* p = ((const EmberOgre::TerrainPosition*)  tolua_tousertype(tolua_S,1,0));
  {
   Ogre::Vector3 tolua_ret = (Ogre::Vector3)  EmberOgre::Atlas2Ogre(*p);
   {
#ifdef __cplusplus
    void* tolua_obj = new Ogre::Vector3(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Vector3");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::Vector3));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Vector3");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Atlas2Ogre'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: EmberOgre::Ogre2Atlas */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Ogre2Atlas00
static int tolua_EmberOgre_EmberOgre_Ogre2Atlas00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Vector3* p = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,1,0));
  {
   WFMath::Point<3> tolua_ret = (WFMath::Point<3>)  EmberOgre::Ogre2Atlas(*p);
   {
#ifdef __cplusplus
    void* tolua_obj = new WFMath::Point<3>(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"WFMath::Point<3>");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(WFMath::Point<3>));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"WFMath::Point<3>");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Ogre2Atlas'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: EmberOgre::Ogre2Atlas */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Ogre2Atlas01
static int tolua_EmberOgre_EmberOgre_Ogre2Atlas01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Vector2",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Vector2* p = ((const Ogre::Vector2*)  tolua_tousertype(tolua_S,1,0));
  {
   EmberOgre::TerrainPosition tolua_ret = (EmberOgre::TerrainPosition)  EmberOgre::Ogre2Atlas(*p);
   {
#ifdef __cplusplus
    void* tolua_obj = new EmberOgre::TerrainPosition(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"EmberOgre::TerrainPosition");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(EmberOgre::TerrainPosition));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"EmberOgre::TerrainPosition");
#endif
   }
  }
 }
 return 1;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_Ogre2Atlas00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* function: EmberOgre::Ogre2Atlas_TerrainPosition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Ogre2Atlas_TerrainPosition00
static int tolua_EmberOgre_EmberOgre_Ogre2Atlas_TerrainPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Vector3* p = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,1,0));
  {
   EmberOgre::TerrainPosition tolua_ret = (EmberOgre::TerrainPosition)  EmberOgre::Ogre2Atlas_TerrainPosition(*p);
   {
#ifdef __cplusplus
    void* tolua_obj = new EmberOgre::TerrainPosition(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"EmberOgre::TerrainPosition");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(EmberOgre::TerrainPosition));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"EmberOgre::TerrainPosition");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Ogre2Atlas_TerrainPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: EmberOgre::Ogre2Atlas_Vector3 */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Ogre2Atlas_Vector300
static int tolua_EmberOgre_EmberOgre_Ogre2Atlas_Vector300(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Vector3* p = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,1,0));
  {
   WFMath::Vector<3> tolua_ret = (WFMath::Vector<3>)  EmberOgre::Ogre2Atlas_Vector3(*p);
   {
#ifdef __cplusplus
    void* tolua_obj = new WFMath::Vector<3>(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"WFMath::Vector<3>");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(WFMath::Vector<3>));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"WFMath::Vector<3>");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Ogre2Atlas_Vector3'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: EmberOgre::Atlas2Ogre */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Atlas2Ogre01
static int tolua_EmberOgre_EmberOgre_Atlas2Ogre01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const WFMath::Point<3>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const WFMath::Point<3>* p = ((const WFMath::Point<3>*)  tolua_tousertype(tolua_S,1,0));
  {
   Ogre::Vector3 tolua_ret = (Ogre::Vector3)  EmberOgre::Atlas2Ogre(*p);
   {
#ifdef __cplusplus
    void* tolua_obj = new Ogre::Vector3(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Vector3");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::Vector3));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Vector3");
#endif
   }
  }
 }
 return 1;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_Atlas2Ogre00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* function: EmberOgre::Atlas2Ogre */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Atlas2Ogre02
static int tolua_EmberOgre_EmberOgre_Atlas2Ogre02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const WFMath::Vector<3>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const WFMath::Vector<3>* v = ((const WFMath::Vector<3>*)  tolua_tousertype(tolua_S,1,0));
  {
   Ogre::Vector3 tolua_ret = (Ogre::Vector3)  EmberOgre::Atlas2Ogre(*v);
   {
#ifdef __cplusplus
    void* tolua_obj = new Ogre::Vector3(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Vector3");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::Vector3));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Vector3");
#endif
   }
  }
 }
 return 1;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_Atlas2Ogre01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* function: EmberOgre::Atlas2Ogre */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Atlas2Ogre03
static int tolua_EmberOgre_EmberOgre_Atlas2Ogre03(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const WFMath::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const WFMath::Quaternion* aq = ((const WFMath::Quaternion*)  tolua_tousertype(tolua_S,1,0));
  {
   Ogre::Quaternion tolua_ret = (Ogre::Quaternion)  EmberOgre::Atlas2Ogre(*aq);
   {
#ifdef __cplusplus
    void* tolua_obj = new Ogre::Quaternion(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Quaternion");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::Quaternion));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Quaternion");
#endif
   }
  }
 }
 return 1;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_Atlas2Ogre02(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* function: EmberOgre::Ogre2Atlas */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Ogre2Atlas02
static int tolua_EmberOgre_EmberOgre_Ogre2Atlas02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Quaternion* aq = ((const Ogre::Quaternion*)  tolua_tousertype(tolua_S,1,0));
  {
   WFMath::Quaternion tolua_ret = (WFMath::Quaternion)  EmberOgre::Ogre2Atlas(*aq);
   {
#ifdef __cplusplus
    void* tolua_obj = new WFMath::Quaternion(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"WFMath::Quaternion");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(WFMath::Quaternion));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"WFMath::Quaternion");
#endif
   }
  }
 }
 return 1;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_Ogre2Atlas01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: showFull of class  EmberOgre::Gui::MovableObjectRenderer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_showFull00
static int tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_showFull00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::MovableObjectRenderer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::MovableObjectRenderer* self = (EmberOgre::Gui::MovableObjectRenderer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'showFull'",NULL);
#endif
  {
   self->showFull();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'showFull'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setCameraDistance of class  EmberOgre::Gui::MovableObjectRenderer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_setCameraDistance00
static int tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_setCameraDistance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::MovableObjectRenderer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::MovableObjectRenderer* self = (EmberOgre::Gui::MovableObjectRenderer*)  tolua_tousertype(tolua_S,1,0);
  float distance = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setCameraDistance'",NULL);
#endif
  {
   self->setCameraDistance(distance);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setCameraDistance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getCameraDistance of class  EmberOgre::Gui::MovableObjectRenderer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_getCameraDistance00
static int tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_getCameraDistance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::MovableObjectRenderer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::MovableObjectRenderer* self = (EmberOgre::Gui::MovableObjectRenderer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCameraDistance'",NULL);
#endif
  {
   float tolua_ret = (float)  self->getCameraDistance();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getCameraDistance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAbsoluteCameraDistance of class  EmberOgre::Gui::MovableObjectRenderer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_getAbsoluteCameraDistance00
static int tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_getAbsoluteCameraDistance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::MovableObjectRenderer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::MovableObjectRenderer* self = (EmberOgre::Gui::MovableObjectRenderer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAbsoluteCameraDistance'",NULL);
#endif
  {
   float tolua_ret = (float)  self->getAbsoluteCameraDistance();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAbsoluteCameraDistance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getIsInputCatchingAllowed of class  EmberOgre::Gui::MovableObjectRenderer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_getIsInputCatchingAllowed00
static int tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_getIsInputCatchingAllowed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Gui::MovableObjectRenderer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Gui::MovableObjectRenderer* self = (const EmberOgre::Gui::MovableObjectRenderer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getIsInputCatchingAllowed'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->getIsInputCatchingAllowed();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getIsInputCatchingAllowed'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setIsInputCatchingAllowed of class  EmberOgre::Gui::MovableObjectRenderer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_setIsInputCatchingAllowed00
static int tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_setIsInputCatchingAllowed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::MovableObjectRenderer",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::MovableObjectRenderer* self = (EmberOgre::Gui::MovableObjectRenderer*)  tolua_tousertype(tolua_S,1,0);
  bool allowed = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setIsInputCatchingAllowed'",NULL);
#endif
  {
   self->setIsInputCatchingAllowed(allowed);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setIsInputCatchingAllowed'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setAutoShowFull of class  EmberOgre::Gui::MovableObjectRenderer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_setAutoShowFull00
static int tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_setAutoShowFull00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::MovableObjectRenderer",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::MovableObjectRenderer* self = (EmberOgre::Gui::MovableObjectRenderer*)  tolua_tousertype(tolua_S,1,0);
  bool showFull = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAutoShowFull'",NULL);
#endif
  {
   self->setAutoShowFull(showFull);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAutoShowFull'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAutoShowFull of class  EmberOgre::Gui::MovableObjectRenderer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_getAutoShowFull00
static int tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_getAutoShowFull00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Gui::MovableObjectRenderer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Gui::MovableObjectRenderer* self = (const EmberOgre::Gui::MovableObjectRenderer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAutoShowFull'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->getAutoShowFull();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAutoShowFull'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getActive of class  EmberOgre::Gui::MovableObjectRenderer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_getActive00
static int tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_getActive00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Gui::MovableObjectRenderer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Gui::MovableObjectRenderer* self = (const EmberOgre::Gui::MovableObjectRenderer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getActive'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->getActive();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getActive'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setActive of class  EmberOgre::Gui::MovableObjectRenderer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_setActive00
static int tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_setActive00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::MovableObjectRenderer",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::MovableObjectRenderer* self = (EmberOgre::Gui::MovableObjectRenderer*)  tolua_tousertype(tolua_S,1,0);
  bool isActive = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setActive'",NULL);
#endif
  {
   self->setActive(isActive);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setActive'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getEntityRotation of class  EmberOgre::Gui::MovableObjectRenderer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_getEntityRotation00
static int tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_getEntityRotation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::MovableObjectRenderer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::MovableObjectRenderer* self = (EmberOgre::Gui::MovableObjectRenderer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getEntityRotation'",NULL);
#endif
  {
   Ogre::Quaternion tolua_ret = (Ogre::Quaternion)  self->getEntityRotation();
   {
#ifdef __cplusplus
    void* tolua_obj = new Ogre::Quaternion(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Quaternion");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::Quaternion));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Quaternion");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getEntityRotation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: resetCameraOrientation of class  EmberOgre::Gui::MovableObjectRenderer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_resetCameraOrientation00
static int tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_resetCameraOrientation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::MovableObjectRenderer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::MovableObjectRenderer* self = (EmberOgre::Gui::MovableObjectRenderer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'resetCameraOrientation'",NULL);
#endif
  {
   self->resetCameraOrientation();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'resetCameraOrientation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: pitch of class  EmberOgre::Gui::MovableObjectRenderer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_pitch00
static int tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_pitch00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::MovableObjectRenderer",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::Degree",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::MovableObjectRenderer* self = (EmberOgre::Gui::MovableObjectRenderer*)  tolua_tousertype(tolua_S,1,0);
  Ogre::Degree degrees = *((Ogre::Degree*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'pitch'",NULL);
#endif
  {
   self->pitch(degrees);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'pitch'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: yaw of class  EmberOgre::Gui::MovableObjectRenderer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_yaw00
static int tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_yaw00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::MovableObjectRenderer",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::Degree",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::MovableObjectRenderer* self = (EmberOgre::Gui::MovableObjectRenderer*)  tolua_tousertype(tolua_S,1,0);
  Ogre::Degree degrees = *((Ogre::Degree*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'yaw'",NULL);
#endif
  {
   self->yaw(degrees);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'yaw'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: roll of class  EmberOgre::Gui::MovableObjectRenderer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_roll00
static int tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_roll00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::MovableObjectRenderer",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::Degree",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::MovableObjectRenderer* self = (EmberOgre::Gui::MovableObjectRenderer*)  tolua_tousertype(tolua_S,1,0);
  Ogre::Degree degrees = *((Ogre::Degree*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'roll'",NULL);
#endif
  {
   self->roll(degrees);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'roll'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: updateRender of class  EmberOgre::Gui::MovableObjectRenderer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_updateRender00
static int tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_updateRender00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::MovableObjectRenderer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::MovableObjectRenderer* self = (EmberOgre::Gui::MovableObjectRenderer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'updateRender'",NULL);
#endif
  {
   self->updateRender();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'updateRender'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setBackgroundColour of class  EmberOgre::Gui::MovableObjectRenderer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_setBackgroundColour00
static int tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_setBackgroundColour00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::MovableObjectRenderer",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::ColourValue",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::MovableObjectRenderer* self = (EmberOgre::Gui::MovableObjectRenderer*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::ColourValue* colour = ((const Ogre::ColourValue*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setBackgroundColour'",NULL);
#endif
  {
   self->setBackgroundColour(*colour);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setBackgroundColour'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setBackgroundColour of class  EmberOgre::Gui::MovableObjectRenderer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_setBackgroundColour01
static int tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_setBackgroundColour01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::MovableObjectRenderer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  EmberOgre::Gui::MovableObjectRenderer* self = (EmberOgre::Gui::MovableObjectRenderer*)  tolua_tousertype(tolua_S,1,0);
  float red = ((float)  tolua_tonumber(tolua_S,2,0));
  float green = ((float)  tolua_tonumber(tolua_S,3,0));
  float blue = ((float)  tolua_tonumber(tolua_S,4,0));
  float alpha = ((float)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setBackgroundColour'",NULL);
#endif
  {
   self->setBackgroundColour(red,green,blue,alpha);
  }
 }
 return 0;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_setBackgroundColour00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::Gui::OgreEntityRenderer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_OgreEntityRenderer_new00
static int tolua_EmberOgre_EmberOgre_Gui_OgreEntityRenderer_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::OgreEntityRenderer",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* image = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::Gui::OgreEntityRenderer* tolua_ret = (EmberOgre::Gui::OgreEntityRenderer*)  new EmberOgre::Gui::OgreEntityRenderer(image);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::OgreEntityRenderer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::Gui::OgreEntityRenderer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_OgreEntityRenderer_new00_local
static int tolua_EmberOgre_EmberOgre_Gui_OgreEntityRenderer_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::OgreEntityRenderer",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* image = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::Gui::OgreEntityRenderer* tolua_ret = (EmberOgre::Gui::OgreEntityRenderer*)  new EmberOgre::Gui::OgreEntityRenderer(image);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Gui::OgreEntityRenderer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  EmberOgre::Gui::OgreEntityRenderer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_OgreEntityRenderer_delete00
static int tolua_EmberOgre_EmberOgre_Gui_OgreEntityRenderer_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::OgreEntityRenderer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::OgreEntityRenderer* self = (EmberOgre::Gui::OgreEntityRenderer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: showEntity of class  EmberOgre::Gui::OgreEntityRenderer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_OgreEntityRenderer_showEntity00
static int tolua_EmberOgre_EmberOgre_Gui_OgreEntityRenderer_showEntity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::OgreEntityRenderer",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::OgreEntityRenderer* self = (EmberOgre::Gui::OgreEntityRenderer*)  tolua_tousertype(tolua_S,1,0);
  const std::string mesh = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'showEntity'",NULL);
#endif
  {
   self->showEntity(mesh);
   tolua_pushcppstring(tolua_S,(const char*)mesh);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'showEntity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getEntity of class  EmberOgre::Gui::OgreEntityRenderer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_OgreEntityRenderer_getEntity00
static int tolua_EmberOgre_EmberOgre_Gui_OgreEntityRenderer_getEntity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::OgreEntityRenderer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::OgreEntityRenderer* self = (EmberOgre::Gui::OgreEntityRenderer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getEntity'",NULL);
#endif
  {
   Ogre::Entity* tolua_ret = (Ogre::Entity*)  self->getEntity();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::Entity");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getEntity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::Gui::ModelRenderer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_ModelRenderer_new00
static int tolua_EmberOgre_EmberOgre_Gui_ModelRenderer_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::ModelRenderer",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* image = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::Gui::ModelRenderer* tolua_ret = (EmberOgre::Gui::ModelRenderer*)  new EmberOgre::Gui::ModelRenderer(image);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::ModelRenderer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::Gui::ModelRenderer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_ModelRenderer_new00_local
static int tolua_EmberOgre_EmberOgre_Gui_ModelRenderer_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::ModelRenderer",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* image = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::Gui::ModelRenderer* tolua_ret = (EmberOgre::Gui::ModelRenderer*)  new EmberOgre::Gui::ModelRenderer(image);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Gui::ModelRenderer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  EmberOgre::Gui::ModelRenderer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_ModelRenderer_delete00
static int tolua_EmberOgre_EmberOgre_Gui_ModelRenderer_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::ModelRenderer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::ModelRenderer* self = (EmberOgre::Gui::ModelRenderer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: showModel of class  EmberOgre::Gui::ModelRenderer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_ModelRenderer_showModel00
static int tolua_EmberOgre_EmberOgre_Gui_ModelRenderer_showModel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::ModelRenderer",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::ModelRenderer* self = (EmberOgre::Gui::ModelRenderer*)  tolua_tousertype(tolua_S,1,0);
  const std::string modelName = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'showModel'",NULL);
#endif
  {
   self->showModel(modelName);
   tolua_pushcppstring(tolua_S,(const char*)modelName);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'showModel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getModel of class  EmberOgre::Gui::ModelRenderer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_ModelRenderer_getModel00
static int tolua_EmberOgre_EmberOgre_Gui_ModelRenderer_getModel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::ModelRenderer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::ModelRenderer* self = (EmberOgre::Gui::ModelRenderer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getModel'",NULL);
#endif
  {
   EmberOgre::Model::Model* tolua_ret = (EmberOgre::Model::Model*)  self->getModel();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Model::Model");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getModel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::Gui::ListHolder */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_ListHolder_new00
static int tolua_EmberOgre_EmberOgre_Gui_ListHolder_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::ListHolder",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CEGUI::Listbox",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"CEGUI::Editbox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CEGUI::Listbox* listbox = ((CEGUI::Listbox*)  tolua_tousertype(tolua_S,2,0));
  CEGUI::Editbox* filterEditbox = ((CEGUI::Editbox*)  tolua_tousertype(tolua_S,3,0));
  {
   EmberOgre::Gui::ListHolder* tolua_ret = (EmberOgre::Gui::ListHolder*)  new EmberOgre::Gui::ListHolder(listbox,filterEditbox);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::ListHolder");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::Gui::ListHolder */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_ListHolder_new00_local
static int tolua_EmberOgre_EmberOgre_Gui_ListHolder_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::ListHolder",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CEGUI::Listbox",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"CEGUI::Editbox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CEGUI::Listbox* listbox = ((CEGUI::Listbox*)  tolua_tousertype(tolua_S,2,0));
  CEGUI::Editbox* filterEditbox = ((CEGUI::Editbox*)  tolua_tousertype(tolua_S,3,0));
  {
   EmberOgre::Gui::ListHolder* tolua_ret = (EmberOgre::Gui::ListHolder*)  new EmberOgre::Gui::ListHolder(listbox,filterEditbox);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Gui::ListHolder");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  EmberOgre::Gui::ListHolder */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_ListHolder_delete00
static int tolua_EmberOgre_EmberOgre_Gui_ListHolder_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::ListHolder",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::ListHolder* self = (EmberOgre::Gui::ListHolder*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addItem of class  EmberOgre::Gui::ListHolder */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_ListHolder_addItem00
static int tolua_EmberOgre_EmberOgre_Gui_ListHolder_addItem00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::ListHolder",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CEGUI::ListboxItem",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::ListHolder* self = (EmberOgre::Gui::ListHolder*)  tolua_tousertype(tolua_S,1,0);
  CEGUI::ListboxItem* item = ((CEGUI::ListboxItem*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addItem'",NULL);
#endif
  {
   self->addItem(item);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addItem'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: insertItem of class  EmberOgre::Gui::ListHolder */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_ListHolder_insertItem00
static int tolua_EmberOgre_EmberOgre_Gui_ListHolder_insertItem00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::ListHolder",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CEGUI::ListboxItem",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const CEGUI::ListboxItem",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::ListHolder* self = (EmberOgre::Gui::ListHolder*)  tolua_tousertype(tolua_S,1,0);
  CEGUI::ListboxItem* item = ((CEGUI::ListboxItem*)  tolua_tousertype(tolua_S,2,0));
  const CEGUI::ListboxItem* position = ((const CEGUI::ListboxItem*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'insertItem'",NULL);
#endif
  {
   self->insertItem(item,position);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'insertItem'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeItem of class  EmberOgre::Gui::ListHolder */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_ListHolder_removeItem00
static int tolua_EmberOgre_EmberOgre_Gui_ListHolder_removeItem00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::ListHolder",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const CEGUI::ListboxItem",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::ListHolder* self = (EmberOgre::Gui::ListHolder*)  tolua_tousertype(tolua_S,1,0);
  const CEGUI::ListboxItem* item = ((const CEGUI::ListboxItem*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeItem'",NULL);
#endif
  {
   self->removeItem(item);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeItem'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: resetList of class  EmberOgre::Gui::ListHolder */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_ListHolder_resetList00
static int tolua_EmberOgre_EmberOgre_Gui_ListHolder_resetList00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::ListHolder",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::ListHolder* self = (EmberOgre::Gui::ListHolder*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'resetList'",NULL);
#endif
  {
   self->resetList();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'resetList'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: windowX of class  EmberOgre::MousePickerArgs */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__MousePickerArgs_windowX
static int tolua_get_EmberOgre__MousePickerArgs_windowX(lua_State* tolua_S)
{
  EmberOgre::MousePickerArgs* self = (EmberOgre::MousePickerArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'windowX'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->windowX);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: windowX of class  EmberOgre::MousePickerArgs */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__MousePickerArgs_windowX
static int tolua_set_EmberOgre__MousePickerArgs_windowX(lua_State* tolua_S)
{
  EmberOgre::MousePickerArgs* self = (EmberOgre::MousePickerArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'windowX'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->windowX = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: windowY of class  EmberOgre::MousePickerArgs */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__MousePickerArgs_windowY
static int tolua_get_EmberOgre__MousePickerArgs_windowY(lua_State* tolua_S)
{
  EmberOgre::MousePickerArgs* self = (EmberOgre::MousePickerArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'windowY'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->windowY);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: windowY of class  EmberOgre::MousePickerArgs */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__MousePickerArgs_windowY
static int tolua_set_EmberOgre__MousePickerArgs_windowY(lua_State* tolua_S)
{
  EmberOgre::MousePickerArgs* self = (EmberOgre::MousePickerArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'windowY'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->windowY = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: pickType of class  EmberOgre::MousePickerArgs */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__MousePickerArgs_pickType
static int tolua_get_EmberOgre__MousePickerArgs_pickType(lua_State* tolua_S)
{
  EmberOgre::MousePickerArgs* self = (EmberOgre::MousePickerArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pickType'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->pickType);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: pickType of class  EmberOgre::MousePickerArgs */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__MousePickerArgs_pickType
static int tolua_set_EmberOgre__MousePickerArgs_pickType(lua_State* tolua_S)
{
  EmberOgre::MousePickerArgs* self = (EmberOgre::MousePickerArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pickType'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->pickType = ((EmberOgre::MousePickType) (int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: entity of class  EmberOgre::EntityPickResult */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__EntityPickResult_entity_ptr
static int tolua_get_EmberOgre__EntityPickResult_entity_ptr(lua_State* tolua_S)
{
  EmberOgre::EntityPickResult* self = (EmberOgre::EntityPickResult*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'entity'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)self->entity,"EmberOgre::EmberEntity");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: entity of class  EmberOgre::EntityPickResult */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__EntityPickResult_entity_ptr
static int tolua_set_EmberOgre__EntityPickResult_entity_ptr(lua_State* tolua_S)
{
  EmberOgre::EntityPickResult* self = (EmberOgre::EntityPickResult*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'entity'",NULL);
  if (!tolua_isusertype(tolua_S,2,"EmberOgre::EmberEntity",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->entity = ((EmberOgre::EmberEntity*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: position of class  EmberOgre::EntityPickResult */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__EntityPickResult_position
static int tolua_get_EmberOgre__EntityPickResult_position(lua_State* tolua_S)
{
  EmberOgre::EntityPickResult* self = (EmberOgre::EntityPickResult*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'position'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->position,"Ogre::Vector3");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: position of class  EmberOgre::EntityPickResult */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__EntityPickResult_position
static int tolua_set_EmberOgre__EntityPickResult_position(lua_State* tolua_S)
{
  EmberOgre::EntityPickResult* self = (EmberOgre::EntityPickResult*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'position'",NULL);
  if (!tolua_isusertype(tolua_S,2,"Ogre::Vector3",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->position = *((Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: distance of class  EmberOgre::EntityPickResult */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__EntityPickResult_distance
static int tolua_get_EmberOgre__EntityPickResult_distance(lua_State* tolua_S)
{
  EmberOgre::EntityPickResult* self = (EmberOgre::EntityPickResult*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'distance'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->distance,"Ogre::Real");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: distance of class  EmberOgre::EntityPickResult */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__EntityPickResult_distance
static int tolua_set_EmberOgre__EntityPickResult_distance(lua_State* tolua_S)
{
  EmberOgre::EntityPickResult* self = (EmberOgre::EntityPickResult*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'distance'",NULL);
  if (!tolua_isusertype(tolua_S,2,"Ogre::Real",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->distance = *((Ogre::Real*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventPickedEntity of class  EmberOgre::EntityWorldPickListener */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__EntityWorldPickListener_EventPickedEntity
static int tolua_get_EmberOgre__EntityWorldPickListener_EventPickedEntity(lua_State* tolua_S)
{
  EmberOgre::EntityWorldPickListener* self = (EmberOgre::EntityWorldPickListener*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventPickedEntity'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventPickedEntity,"sigc::signal<void,const EmberOgre::EntityPickResult&,const EmberOgre::MousePickerArgs&>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventPickedEntity of class  EmberOgre::EntityWorldPickListener */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__EntityWorldPickListener_EventPickedEntity
static int tolua_set_EmberOgre__EntityWorldPickListener_EventPickedEntity(lua_State* tolua_S)
{
  EmberOgre::EntityWorldPickListener* self = (EmberOgre::EntityWorldPickListener*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventPickedEntity'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,const EmberOgre::EntityPickResult&,const EmberOgre::MousePickerArgs&>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventPickedEntity = *((sigc::signal<void,const EmberOgre::EntityPickResult&,const EmberOgre::MousePickerArgs&>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSingleton of class  EmberOgre::Model::ModelDefinitionManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_getSingleton00
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_getSingleton00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Model::ModelDefinitionManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   EmberOgre::Model::ModelDefinitionManager& tolua_ret = (EmberOgre::Model::ModelDefinitionManager&)  EmberOgre::Model::ModelDefinitionManager::getSingleton();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"EmberOgre::Model::ModelDefinitionManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSingleton'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: exportScript of class  EmberOgre::Model::ModelDefinitionManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_exportScript00
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_exportScript00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::ModelDefinitionManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"EmberOgre::Model::ModelDefnPtr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::ModelDefinitionManager* self = (EmberOgre::Model::ModelDefinitionManager*)  tolua_tousertype(tolua_S,1,0);
  EmberOgre::Model::ModelDefnPtr definition = *((EmberOgre::Model::ModelDefnPtr*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'exportScript'",NULL);
#endif
  {
   self->exportScript(definition);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'exportScript'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: create of class  EmberOgre::Model::ModelDefinitionManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_create00
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::ModelDefinitionManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::ModelDefinitionManager* self = (EmberOgre::Model::ModelDefinitionManager*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string group = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'create'",NULL);
#endif
  {
   EmberOgre::Model::ModelDefnPtr tolua_ret = (EmberOgre::Model::ModelDefnPtr)  self->create(name,group);
   {
#ifdef __cplusplus
    void* tolua_obj = new EmberOgre::Model::ModelDefnPtr(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"EmberOgre::Model::ModelDefnPtr");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(EmberOgre::Model::ModelDefnPtr));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"EmberOgre::Model::ModelDefnPtr");
#endif
   }
   tolua_pushcppstring(tolua_S,(const char*)name);
   tolua_pushcppstring(tolua_S,(const char*)group);
  }
 }
 return 3;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: remove of class  EmberOgre::Model::ModelDefinitionManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_remove00
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_remove00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::ModelDefinitionManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::ModelDefinitionManager* self = (EmberOgre::Model::ModelDefinitionManager*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'remove'",NULL);
#endif
  {
   self->remove(name);
   tolua_pushcppstring(tolua_S,(const char*)name);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'remove'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getByName of class  EmberOgre::Model::ModelDefinitionManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_getByName00
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_getByName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::ModelDefinitionManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::ModelDefinitionManager* self = (EmberOgre::Model::ModelDefinitionManager*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getByName'",NULL);
#endif
  {
   EmberOgre::Model::ModelDefnPtr tolua_ret = (EmberOgre::Model::ModelDefnPtr)  self->getByName(name);
   {
#ifdef __cplusplus
    void* tolua_obj = new EmberOgre::Model::ModelDefnPtr(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"EmberOgre::Model::ModelDefnPtr");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(EmberOgre::Model::ModelDefnPtr));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"EmberOgre::Model::ModelDefnPtr");
#endif
   }
   tolua_pushcppstring(tolua_S,(const char*)name);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getByName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: resourceExists of class  EmberOgre::Model::ModelDefinitionManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_resourceExists00
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_resourceExists00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::ModelDefinitionManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::ModelDefinitionManager* self = (EmberOgre::Model::ModelDefinitionManager*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'resourceExists'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->resourceExists(name);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)name);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'resourceExists'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getResourceIterator of class  EmberOgre::Model::ModelDefinitionManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_getResourceIterator00
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_getResourceIterator00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::ModelDefinitionManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::ModelDefinitionManager* self = (EmberOgre::Model::ModelDefinitionManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getResourceIterator'",NULL);
#endif
  {
   Ogre::MapIterator<Ogre::ResourceManager::ResourceHandleMap> tolua_ret = (Ogre::MapIterator<Ogre::ResourceManager::ResourceHandleMap>)  self->getResourceIterator();
   {
#ifdef __cplusplus
    void* tolua_obj = new Ogre::MapIterator<Ogre::ResourceManager::ResourceHandleMap>(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::MapIterator<Ogre::ResourceManager::ResourceHandleMap>");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::MapIterator<Ogre::ResourceManager::ResourceHandleMap>));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::MapIterator<Ogre::ResourceManager::ResourceHandleMap>");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getResourceIterator'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAllMeshes of class  EmberOgre::Model::ModelDefinitionManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_getAllMeshes00
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_getAllMeshes00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Model::ModelDefinitionManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Model::ModelDefinitionManager* self = (const EmberOgre::Model::ModelDefinitionManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAllMeshes'",NULL);
#endif
  {
   const std::vector<std::string> tolua_ret = (const std::vector<std::string>)  self->getAllMeshes();
   {
#ifdef __cplusplus
    void* tolua_obj = new std::vector<std::string>(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"const std::vector<std::string>");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(const std::vector<std::string>));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"const std::vector<std::string>");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAllMeshes'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getShowModels of class  EmberOgre::Model::ModelDefinitionManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_getShowModels00
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_getShowModels00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Model::ModelDefinitionManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Model::ModelDefinitionManager* self = (const EmberOgre::Model::ModelDefinitionManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getShowModels'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->getShowModels();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getShowModels'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setShowModels of class  EmberOgre::Model::ModelDefinitionManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_setShowModels00
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_setShowModels00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::ModelDefinitionManager",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::ModelDefinitionManager* self = (EmberOgre::Model::ModelDefinitionManager*)  tolua_tousertype(tolua_S,1,0);
  bool show = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setShowModels'",NULL);
#endif
  {
   self->setShowModels(show);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setShowModels'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: startMove of class  EmberOgre::EntityMoveManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EntityMoveManager_startMove00
static int tolua_EmberOgre_EmberOgre_EntityMoveManager_startMove00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::EntityMoveManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"EmberOgre::EmberEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::EntityMoveManager* self = (EmberOgre::EntityMoveManager*)  tolua_tousertype(tolua_S,1,0);
  EmberOgre::EmberEntity* entity = ((EmberOgre::EmberEntity*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'startMove'",NULL);
#endif
  {
   self->startMove(entity);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'startMove'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: runCommand of class  EmberOgre::EntityMoveManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EntityMoveManager_runCommand00
static int tolua_EmberOgre_EmberOgre_EntityMoveManager_runCommand00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::EntityMoveManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::EntityMoveManager* self = (EmberOgre::EntityMoveManager*)  tolua_tousertype(tolua_S,1,0);
  const std::string command = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string args = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'runCommand'",NULL);
#endif
  {
   self->runCommand(command,args);
   tolua_pushcppstring(tolua_S,(const char*)command);
   tolua_pushcppstring(tolua_S,(const char*)args);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'runCommand'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventStartMoving of class  EmberOgre::EntityMoveManager */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__EntityMoveManager_EventStartMoving
static int tolua_get_EmberOgre__EntityMoveManager_EventStartMoving(lua_State* tolua_S)
{
  EmberOgre::EntityMoveManager* self = (EmberOgre::EntityMoveManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventStartMoving'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventStartMoving,"sigc::signal<void,EmberOgre::EmberEntity*>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventStartMoving of class  EmberOgre::EntityMoveManager */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__EntityMoveManager_EventStartMoving
static int tolua_set_EmberOgre__EntityMoveManager_EventStartMoving(lua_State* tolua_S)
{
  EmberOgre::EntityMoveManager* self = (EmberOgre::EntityMoveManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventStartMoving'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,EmberOgre::EmberEntity*>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventStartMoving = *((sigc::signal<void,EmberOgre::EmberEntity*>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventFinishedMoving of class  EmberOgre::EntityMoveManager */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__EntityMoveManager_EventFinishedMoving
static int tolua_get_EmberOgre__EntityMoveManager_EventFinishedMoving(lua_State* tolua_S)
{
  EmberOgre::EntityMoveManager* self = (EmberOgre::EntityMoveManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventFinishedMoving'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventFinishedMoving,"sigc::signal<void>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventFinishedMoving of class  EmberOgre::EntityMoveManager */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__EntityMoveManager_EventFinishedMoving
static int tolua_set_EmberOgre__EntityMoveManager_EventFinishedMoving(lua_State* tolua_S)
{
  EmberOgre::EntityMoveManager* self = (EmberOgre::EntityMoveManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventFinishedMoving'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventFinishedMoving = *((sigc::signal<void>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventCancelledMoving of class  EmberOgre::EntityMoveManager */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__EntityMoveManager_EventCancelledMoving
static int tolua_get_EmberOgre__EntityMoveManager_EventCancelledMoving(lua_State* tolua_S)
{
  EmberOgre::EntityMoveManager* self = (EmberOgre::EntityMoveManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventCancelledMoving'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventCancelledMoving,"sigc::signal<void>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventCancelledMoving of class  EmberOgre::EntityMoveManager */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__EntityMoveManager_EventCancelledMoving
static int tolua_set_EmberOgre__EntityMoveManager_EventCancelledMoving(lua_State* tolua_S)
{
  EmberOgre::EntityMoveManager* self = (EmberOgre::EntityMoveManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventCancelledMoving'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventCancelledMoving = *((sigc::signal<void>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: clear of class  std::vector<std::string> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_vector_std__string__clear00
static int tolua_EmberOgre_std_vector_std__string__clear00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"std::vector<std::string>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::vector<std::string>* self = (std::vector<std::string>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clear'",NULL);
#endif
  {
   self->clear();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clear'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: size of class  std::vector<std::string> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_vector_std__string__size00
static int tolua_EmberOgre_std_vector_std__string__size00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const std::vector<std::string>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::vector<std::string>* self = (const std::vector<std::string>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'size'",NULL);
#endif
  {
   int tolua_ret = (int)  self->size();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'size'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator[] of class  std::vector<std::string> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_vector_std__string___geti00
static int tolua_EmberOgre_std_vector_std__string___geti00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const std::vector<std::string>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::vector<std::string>* self = (const std::vector<std::string>*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator[]'",NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->operator[](index);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.geti'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator&[] of class  std::vector<std::string> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_vector_std__string___seti00
static int tolua_EmberOgre_std_vector_std__string___seti00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"std::vector<std::string>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::vector<std::string>* self = (std::vector<std::string>*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  std::string tolua_value = ((std::string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator&[]'",NULL);
#endif
  self->operator[](index) =  tolua_value;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.seti'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator[] of class  std::vector<std::string> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_vector_std__string___geti01
static int tolua_EmberOgre_std_vector_std__string___geti01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"std::vector<std::string>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  std::vector<std::string>* self = (std::vector<std::string>*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator[]'",NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->operator[](index);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_EmberOgre_std_vector_std__string___geti00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: push_back of class  std::vector<std::string> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_vector_std__string__push_back00
static int tolua_EmberOgre_std_vector_std__string__push_back00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"std::vector<std::string>",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::vector<std::string>* self = (std::vector<std::string>*)  tolua_tousertype(tolua_S,1,0);
  std::string val = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'push_back'",NULL);
#endif
  {
   self->push_back(val);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'push_back'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  std::vector<std::string> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_vector_std__string__new00
static int tolua_EmberOgre_std_vector_std__string__new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"std::vector<std::string>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   std::vector<std::string>* tolua_ret = (std::vector<std::string>*)  new std::vector<std::string>();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"std::vector<std::string>");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  std::vector<std::string> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_vector_std__string__new00_local
static int tolua_EmberOgre_std_vector_std__string__new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"std::vector<std::string>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   std::vector<std::string>* tolua_ret = (std::vector<std::string>*)  new std::vector<std::string>();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"std::vector<std::string>");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  std::vector<std::string> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_vector_std__string__delete00
static int tolua_EmberOgre_std_vector_std__string__delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"std::vector<std::string>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::vector<std::string>* self = (std::vector<std::string>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clear of class  std::vector<EmberOgre::Model::SubModelDefinition*> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_vector_EmberOgre__Model__SubModelDefinition___clear00
static int tolua_EmberOgre_std_vector_EmberOgre__Model__SubModelDefinition___clear00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"std::vector<EmberOgre::Model::SubModelDefinition*>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::vector<EmberOgre::Model::SubModelDefinition*>* self = (std::vector<EmberOgre::Model::SubModelDefinition*>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clear'",NULL);
#endif
  {
   self->clear();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clear'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: size of class  std::vector<EmberOgre::Model::SubModelDefinition*> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_vector_EmberOgre__Model__SubModelDefinition___size00
static int tolua_EmberOgre_std_vector_EmberOgre__Model__SubModelDefinition___size00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const std::vector<EmberOgre::Model::SubModelDefinition*>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::vector<EmberOgre::Model::SubModelDefinition*>* self = (const std::vector<EmberOgre::Model::SubModelDefinition*>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'size'",NULL);
#endif
  {
   int tolua_ret = (int)  self->size();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'size'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator[] of class  std::vector<EmberOgre::Model::SubModelDefinition*> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_vector_EmberOgre__Model__SubModelDefinition____geti00
static int tolua_EmberOgre_std_vector_EmberOgre__Model__SubModelDefinition____geti00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const std::vector<EmberOgre::Model::SubModelDefinition*>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::vector<EmberOgre::Model::SubModelDefinition*>* self = (const std::vector<EmberOgre::Model::SubModelDefinition*>*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator[]'",NULL);
#endif
  {
   const EmberOgre::Model::SubModelDefinition* tolua_ret = (const EmberOgre::Model::SubModelDefinition*)  self->operator[](index);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"const EmberOgre::Model::SubModelDefinition");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.geti'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator&[] of class  std::vector<EmberOgre::Model::SubModelDefinition*> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_vector_EmberOgre__Model__SubModelDefinition____seti00
static int tolua_EmberOgre_std_vector_EmberOgre__Model__SubModelDefinition____seti00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"std::vector<EmberOgre::Model::SubModelDefinition*>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"EmberOgre::Model::SubModelDefinition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::vector<EmberOgre::Model::SubModelDefinition*>* self = (std::vector<EmberOgre::Model::SubModelDefinition*>*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  EmberOgre::Model::SubModelDefinition* tolua_value = ((EmberOgre::Model::SubModelDefinition*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator&[]'",NULL);
#endif
  self->operator[](index) =  tolua_value;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.seti'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator[] of class  std::vector<EmberOgre::Model::SubModelDefinition*> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_vector_EmberOgre__Model__SubModelDefinition____geti01
static int tolua_EmberOgre_std_vector_EmberOgre__Model__SubModelDefinition____geti01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"std::vector<EmberOgre::Model::SubModelDefinition*>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  std::vector<EmberOgre::Model::SubModelDefinition*>* self = (std::vector<EmberOgre::Model::SubModelDefinition*>*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator[]'",NULL);
#endif
  {
   EmberOgre::Model::SubModelDefinition* tolua_ret = (EmberOgre::Model::SubModelDefinition*)  self->operator[](index);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Model::SubModelDefinition");
  }
 }
 return 1;
tolua_lerror:
 return tolua_EmberOgre_std_vector_EmberOgre__Model__SubModelDefinition____geti00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: push_back of class  std::vector<EmberOgre::Model::SubModelDefinition*> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_vector_EmberOgre__Model__SubModelDefinition___push_back00
static int tolua_EmberOgre_std_vector_EmberOgre__Model__SubModelDefinition___push_back00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"std::vector<EmberOgre::Model::SubModelDefinition*>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"EmberOgre::Model::SubModelDefinition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::vector<EmberOgre::Model::SubModelDefinition*>* self = (std::vector<EmberOgre::Model::SubModelDefinition*>*)  tolua_tousertype(tolua_S,1,0);
  EmberOgre::Model::SubModelDefinition* val = ((EmberOgre::Model::SubModelDefinition*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'push_back'",NULL);
#endif
  {
   self->push_back(val);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'push_back'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  std::vector<EmberOgre::Model::SubModelDefinition*> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_vector_EmberOgre__Model__SubModelDefinition___new00
static int tolua_EmberOgre_std_vector_EmberOgre__Model__SubModelDefinition___new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"std::vector<EmberOgre::Model::SubModelDefinition*>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   std::vector<EmberOgre::Model::SubModelDefinition*>* tolua_ret = (std::vector<EmberOgre::Model::SubModelDefinition*>*)  new std::vector<EmberOgre::Model::SubModelDefinition*>();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"std::vector<EmberOgre::Model::SubModelDefinition*>");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  std::vector<EmberOgre::Model::SubModelDefinition*> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_vector_EmberOgre__Model__SubModelDefinition___new00_local
static int tolua_EmberOgre_std_vector_EmberOgre__Model__SubModelDefinition___new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"std::vector<EmberOgre::Model::SubModelDefinition*>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   std::vector<EmberOgre::Model::SubModelDefinition*>* tolua_ret = (std::vector<EmberOgre::Model::SubModelDefinition*>*)  new std::vector<EmberOgre::Model::SubModelDefinition*>();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"std::vector<EmberOgre::Model::SubModelDefinition*>");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  std::vector<EmberOgre::Model::SubModelDefinition*> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_vector_EmberOgre__Model__SubModelDefinition___delete00
static int tolua_EmberOgre_std_vector_EmberOgre__Model__SubModelDefinition___delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"std::vector<EmberOgre::Model::SubModelDefinition*>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::vector<EmberOgre::Model::SubModelDefinition*>* self = (std::vector<EmberOgre::Model::SubModelDefinition*>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clear of class  std::vector<EmberOgre::Model::PartDefinition*> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_vector_EmberOgre__Model__PartDefinition___clear00
static int tolua_EmberOgre_std_vector_EmberOgre__Model__PartDefinition___clear00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"std::vector<EmberOgre::Model::PartDefinition*>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::vector<EmberOgre::Model::PartDefinition*>* self = (std::vector<EmberOgre::Model::PartDefinition*>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clear'",NULL);
#endif
  {
   self->clear();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clear'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: size of class  std::vector<EmberOgre::Model::PartDefinition*> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_vector_EmberOgre__Model__PartDefinition___size00
static int tolua_EmberOgre_std_vector_EmberOgre__Model__PartDefinition___size00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const std::vector<EmberOgre::Model::PartDefinition*>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::vector<EmberOgre::Model::PartDefinition*>* self = (const std::vector<EmberOgre::Model::PartDefinition*>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'size'",NULL);
#endif
  {
   int tolua_ret = (int)  self->size();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'size'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator[] of class  std::vector<EmberOgre::Model::PartDefinition*> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_vector_EmberOgre__Model__PartDefinition____geti00
static int tolua_EmberOgre_std_vector_EmberOgre__Model__PartDefinition____geti00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const std::vector<EmberOgre::Model::PartDefinition*>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::vector<EmberOgre::Model::PartDefinition*>* self = (const std::vector<EmberOgre::Model::PartDefinition*>*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator[]'",NULL);
#endif
  {
   const EmberOgre::Model::PartDefinition* tolua_ret = (const EmberOgre::Model::PartDefinition*)  self->operator[](index);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"const EmberOgre::Model::PartDefinition");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.geti'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator&[] of class  std::vector<EmberOgre::Model::PartDefinition*> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_vector_EmberOgre__Model__PartDefinition____seti00
static int tolua_EmberOgre_std_vector_EmberOgre__Model__PartDefinition____seti00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"std::vector<EmberOgre::Model::PartDefinition*>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"EmberOgre::Model::PartDefinition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::vector<EmberOgre::Model::PartDefinition*>* self = (std::vector<EmberOgre::Model::PartDefinition*>*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  EmberOgre::Model::PartDefinition* tolua_value = ((EmberOgre::Model::PartDefinition*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator&[]'",NULL);
#endif
  self->operator[](index) =  tolua_value;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.seti'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator[] of class  std::vector<EmberOgre::Model::PartDefinition*> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_vector_EmberOgre__Model__PartDefinition____geti01
static int tolua_EmberOgre_std_vector_EmberOgre__Model__PartDefinition____geti01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"std::vector<EmberOgre::Model::PartDefinition*>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  std::vector<EmberOgre::Model::PartDefinition*>* self = (std::vector<EmberOgre::Model::PartDefinition*>*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator[]'",NULL);
#endif
  {
   EmberOgre::Model::PartDefinition* tolua_ret = (EmberOgre::Model::PartDefinition*)  self->operator[](index);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Model::PartDefinition");
  }
 }
 return 1;
tolua_lerror:
 return tolua_EmberOgre_std_vector_EmberOgre__Model__PartDefinition____geti00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: push_back of class  std::vector<EmberOgre::Model::PartDefinition*> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_vector_EmberOgre__Model__PartDefinition___push_back00
static int tolua_EmberOgre_std_vector_EmberOgre__Model__PartDefinition___push_back00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"std::vector<EmberOgre::Model::PartDefinition*>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"EmberOgre::Model::PartDefinition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::vector<EmberOgre::Model::PartDefinition*>* self = (std::vector<EmberOgre::Model::PartDefinition*>*)  tolua_tousertype(tolua_S,1,0);
  EmberOgre::Model::PartDefinition* val = ((EmberOgre::Model::PartDefinition*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'push_back'",NULL);
#endif
  {
   self->push_back(val);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'push_back'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  std::vector<EmberOgre::Model::PartDefinition*> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_vector_EmberOgre__Model__PartDefinition___new00
static int tolua_EmberOgre_std_vector_EmberOgre__Model__PartDefinition___new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"std::vector<EmberOgre::Model::PartDefinition*>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   std::vector<EmberOgre::Model::PartDefinition*>* tolua_ret = (std::vector<EmberOgre::Model::PartDefinition*>*)  new std::vector<EmberOgre::Model::PartDefinition*>();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"std::vector<EmberOgre::Model::PartDefinition*>");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  std::vector<EmberOgre::Model::PartDefinition*> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_vector_EmberOgre__Model__PartDefinition___new00_local
static int tolua_EmberOgre_std_vector_EmberOgre__Model__PartDefinition___new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"std::vector<EmberOgre::Model::PartDefinition*>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   std::vector<EmberOgre::Model::PartDefinition*>* tolua_ret = (std::vector<EmberOgre::Model::PartDefinition*>*)  new std::vector<EmberOgre::Model::PartDefinition*>();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"std::vector<EmberOgre::Model::PartDefinition*>");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  std::vector<EmberOgre::Model::PartDefinition*> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_vector_EmberOgre__Model__PartDefinition___delete00
static int tolua_EmberOgre_std_vector_EmberOgre__Model__PartDefinition___delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"std::vector<EmberOgre::Model::PartDefinition*>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::vector<EmberOgre::Model::PartDefinition*>* self = (std::vector<EmberOgre::Model::PartDefinition*>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clear of class  std::vector<EmberOgre::Model::SubEntityDefinition*> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_vector_EmberOgre__Model__SubEntityDefinition___clear00
static int tolua_EmberOgre_std_vector_EmberOgre__Model__SubEntityDefinition___clear00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"std::vector<EmberOgre::Model::SubEntityDefinition*>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::vector<EmberOgre::Model::SubEntityDefinition*>* self = (std::vector<EmberOgre::Model::SubEntityDefinition*>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clear'",NULL);
#endif
  {
   self->clear();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clear'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: size of class  std::vector<EmberOgre::Model::SubEntityDefinition*> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_vector_EmberOgre__Model__SubEntityDefinition___size00
static int tolua_EmberOgre_std_vector_EmberOgre__Model__SubEntityDefinition___size00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const std::vector<EmberOgre::Model::SubEntityDefinition*>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::vector<EmberOgre::Model::SubEntityDefinition*>* self = (const std::vector<EmberOgre::Model::SubEntityDefinition*>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'size'",NULL);
#endif
  {
   int tolua_ret = (int)  self->size();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'size'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator[] of class  std::vector<EmberOgre::Model::SubEntityDefinition*> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_vector_EmberOgre__Model__SubEntityDefinition____geti00
static int tolua_EmberOgre_std_vector_EmberOgre__Model__SubEntityDefinition____geti00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const std::vector<EmberOgre::Model::SubEntityDefinition*>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::vector<EmberOgre::Model::SubEntityDefinition*>* self = (const std::vector<EmberOgre::Model::SubEntityDefinition*>*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator[]'",NULL);
#endif
  {
   const EmberOgre::Model::SubEntityDefinition* tolua_ret = (const EmberOgre::Model::SubEntityDefinition*)  self->operator[](index);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"const EmberOgre::Model::SubEntityDefinition");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.geti'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator&[] of class  std::vector<EmberOgre::Model::SubEntityDefinition*> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_vector_EmberOgre__Model__SubEntityDefinition____seti00
static int tolua_EmberOgre_std_vector_EmberOgre__Model__SubEntityDefinition____seti00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"std::vector<EmberOgre::Model::SubEntityDefinition*>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"EmberOgre::Model::SubEntityDefinition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::vector<EmberOgre::Model::SubEntityDefinition*>* self = (std::vector<EmberOgre::Model::SubEntityDefinition*>*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  EmberOgre::Model::SubEntityDefinition* tolua_value = ((EmberOgre::Model::SubEntityDefinition*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator&[]'",NULL);
#endif
  self->operator[](index) =  tolua_value;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.seti'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator[] of class  std::vector<EmberOgre::Model::SubEntityDefinition*> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_vector_EmberOgre__Model__SubEntityDefinition____geti01
static int tolua_EmberOgre_std_vector_EmberOgre__Model__SubEntityDefinition____geti01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"std::vector<EmberOgre::Model::SubEntityDefinition*>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  std::vector<EmberOgre::Model::SubEntityDefinition*>* self = (std::vector<EmberOgre::Model::SubEntityDefinition*>*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator[]'",NULL);
#endif
  {
   EmberOgre::Model::SubEntityDefinition* tolua_ret = (EmberOgre::Model::SubEntityDefinition*)  self->operator[](index);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Model::SubEntityDefinition");
  }
 }
 return 1;
tolua_lerror:
 return tolua_EmberOgre_std_vector_EmberOgre__Model__SubEntityDefinition____geti00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: push_back of class  std::vector<EmberOgre::Model::SubEntityDefinition*> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_vector_EmberOgre__Model__SubEntityDefinition___push_back00
static int tolua_EmberOgre_std_vector_EmberOgre__Model__SubEntityDefinition___push_back00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"std::vector<EmberOgre::Model::SubEntityDefinition*>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"EmberOgre::Model::SubEntityDefinition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::vector<EmberOgre::Model::SubEntityDefinition*>* self = (std::vector<EmberOgre::Model::SubEntityDefinition*>*)  tolua_tousertype(tolua_S,1,0);
  EmberOgre::Model::SubEntityDefinition* val = ((EmberOgre::Model::SubEntityDefinition*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'push_back'",NULL);
#endif
  {
   self->push_back(val);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'push_back'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  std::vector<EmberOgre::Model::SubEntityDefinition*> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_vector_EmberOgre__Model__SubEntityDefinition___new00
static int tolua_EmberOgre_std_vector_EmberOgre__Model__SubEntityDefinition___new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"std::vector<EmberOgre::Model::SubEntityDefinition*>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   std::vector<EmberOgre::Model::SubEntityDefinition*>* tolua_ret = (std::vector<EmberOgre::Model::SubEntityDefinition*>*)  new std::vector<EmberOgre::Model::SubEntityDefinition*>();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"std::vector<EmberOgre::Model::SubEntityDefinition*>");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  std::vector<EmberOgre::Model::SubEntityDefinition*> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_vector_EmberOgre__Model__SubEntityDefinition___new00_local
static int tolua_EmberOgre_std_vector_EmberOgre__Model__SubEntityDefinition___new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"std::vector<EmberOgre::Model::SubEntityDefinition*>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   std::vector<EmberOgre::Model::SubEntityDefinition*>* tolua_ret = (std::vector<EmberOgre::Model::SubEntityDefinition*>*)  new std::vector<EmberOgre::Model::SubEntityDefinition*>();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"std::vector<EmberOgre::Model::SubEntityDefinition*>");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  std::vector<EmberOgre::Model::SubEntityDefinition*> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_vector_EmberOgre__Model__SubEntityDefinition___delete00
static int tolua_EmberOgre_std_vector_EmberOgre__Model__SubEntityDefinition___delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"std::vector<EmberOgre::Model::SubEntityDefinition*>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::vector<EmberOgre::Model::SubEntityDefinition*>* self = (std::vector<EmberOgre::Model::SubEntityDefinition*>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clear of class  std::vector<Eris::Task*> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_vector_Eris__Task___clear00
static int tolua_EmberOgre_std_vector_Eris__Task___clear00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"std::vector<Eris::Task*>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::vector<Eris::Task*>* self = (std::vector<Eris::Task*>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clear'",NULL);
#endif
  {
   self->clear();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clear'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: size of class  std::vector<Eris::Task*> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_vector_Eris__Task___size00
static int tolua_EmberOgre_std_vector_Eris__Task___size00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const std::vector<Eris::Task*>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::vector<Eris::Task*>* self = (const std::vector<Eris::Task*>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'size'",NULL);
#endif
  {
   int tolua_ret = (int)  self->size();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'size'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator[] of class  std::vector<Eris::Task*> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_vector_Eris__Task____geti00
static int tolua_EmberOgre_std_vector_Eris__Task____geti00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const std::vector<Eris::Task*>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::vector<Eris::Task*>* self = (const std::vector<Eris::Task*>*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator[]'",NULL);
#endif
  {
   const Eris::Task* tolua_ret = (const Eris::Task*)  self->operator[](index);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"const Eris::Task");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.geti'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator&[] of class  std::vector<Eris::Task*> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_vector_Eris__Task____seti00
static int tolua_EmberOgre_std_vector_Eris__Task____seti00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"std::vector<Eris::Task*>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"Eris::Task",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::vector<Eris::Task*>* self = (std::vector<Eris::Task*>*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  Eris::Task* tolua_value = ((Eris::Task*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator&[]'",NULL);
#endif
  self->operator[](index) =  tolua_value;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.seti'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator[] of class  std::vector<Eris::Task*> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_vector_Eris__Task____geti01
static int tolua_EmberOgre_std_vector_Eris__Task____geti01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"std::vector<Eris::Task*>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  std::vector<Eris::Task*>* self = (std::vector<Eris::Task*>*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator[]'",NULL);
#endif
  {
   Eris::Task* tolua_ret = (Eris::Task*)  self->operator[](index);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Eris::Task");
  }
 }
 return 1;
tolua_lerror:
 return tolua_EmberOgre_std_vector_Eris__Task____geti00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: push_back of class  std::vector<Eris::Task*> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_vector_Eris__Task___push_back00
static int tolua_EmberOgre_std_vector_Eris__Task___push_back00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"std::vector<Eris::Task*>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Eris::Task",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::vector<Eris::Task*>* self = (std::vector<Eris::Task*>*)  tolua_tousertype(tolua_S,1,0);
  Eris::Task* val = ((Eris::Task*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'push_back'",NULL);
#endif
  {
   self->push_back(val);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'push_back'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  std::vector<Eris::Task*> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_vector_Eris__Task___new00
static int tolua_EmberOgre_std_vector_Eris__Task___new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"std::vector<Eris::Task*>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   std::vector<Eris::Task*>* tolua_ret = (std::vector<Eris::Task*>*)  new std::vector<Eris::Task*>();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"std::vector<Eris::Task*>");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  std::vector<Eris::Task*> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_vector_Eris__Task___new00_local
static int tolua_EmberOgre_std_vector_Eris__Task___new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"std::vector<Eris::Task*>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   std::vector<Eris::Task*>* tolua_ret = (std::vector<Eris::Task*>*)  new std::vector<Eris::Task*>();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"std::vector<Eris::Task*>");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  std::vector<Eris::Task*> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_vector_Eris__Task___delete00
static int tolua_EmberOgre_std_vector_Eris__Task___delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"std::vector<Eris::Task*>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::vector<Eris::Task*>* self = (std::vector<Eris::Task*>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clear of class  std::set<std::string> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_set_std__string__clear00
static int tolua_EmberOgre_std_set_std__string__clear00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"std::set<std::string>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::set<std::string>* self = (std::set<std::string>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clear'",NULL);
#endif
  {
   self->clear();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clear'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: size of class  std::set<std::string> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_set_std__string__size00
static int tolua_EmberOgre_std_set_std__string__size00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const std::set<std::string>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::set<std::string>* self = (const std::set<std::string>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'size'",NULL);
#endif
  {
   int tolua_ret = (int)  self->size();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'size'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  std::set<std::string> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_set_std__string__new00
static int tolua_EmberOgre_std_set_std__string__new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"std::set<std::string>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   std::set<std::string>* tolua_ret = (std::set<std::string>*)  new std::set<std::string>();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"std::set<std::string>");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  std::set<std::string> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_set_std__string__new00_local
static int tolua_EmberOgre_std_set_std__string__new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"std::set<std::string>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   std::set<std::string>* tolua_ret = (std::set<std::string>*)  new std::set<std::string>();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"std::set<std::string>");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  std::set<std::string> */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_std_set_std__string__delete00
static int tolua_EmberOgre_std_set_std__string__delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"std::set<std::string>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::set<std::string>* self = (std::set<std::string>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::Gui::Vector3Adapter */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_Vector3Adapter_new00
static int tolua_EmberOgre_EmberOgre_Gui_Vector3Adapter_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::Vector3Adapter",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* xWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
  CEGUI::Window* yWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,3,0));
  CEGUI::Window* zWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,4,0));
  const Ogre::Vector3* vector = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,5,0));
  {
   EmberOgre::Gui::Vector3Adapter* tolua_ret = (EmberOgre::Gui::Vector3Adapter*)  new EmberOgre::Gui::Vector3Adapter(xWindow,yWindow,zWindow,*vector);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::Vector3Adapter");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::Gui::Vector3Adapter */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_Vector3Adapter_new00_local
static int tolua_EmberOgre_EmberOgre_Gui_Vector3Adapter_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::Vector3Adapter",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* xWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
  CEGUI::Window* yWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,3,0));
  CEGUI::Window* zWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,4,0));
  const Ogre::Vector3* vector = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,5,0));
  {
   EmberOgre::Gui::Vector3Adapter* tolua_ret = (EmberOgre::Gui::Vector3Adapter*)  new EmberOgre::Gui::Vector3Adapter(xWindow,yWindow,zWindow,*vector);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Gui::Vector3Adapter");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::Gui::Vector3Adapter */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_Vector3Adapter_new01
static int tolua_EmberOgre_EmberOgre_Gui_Vector3Adapter_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::Vector3Adapter",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  CEGUI::Window* xWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
  CEGUI::Window* yWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,3,0));
  CEGUI::Window* zWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,4,0));
  {
   EmberOgre::Gui::Vector3Adapter* tolua_ret = (EmberOgre::Gui::Vector3Adapter*)  new EmberOgre::Gui::Vector3Adapter(xWindow,yWindow,zWindow);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::Vector3Adapter");
  }
 }
 return 1;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_Gui_Vector3Adapter_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::Gui::Vector3Adapter */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_Vector3Adapter_new01_local
static int tolua_EmberOgre_EmberOgre_Gui_Vector3Adapter_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::Vector3Adapter",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  CEGUI::Window* xWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
  CEGUI::Window* yWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,3,0));
  CEGUI::Window* zWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,4,0));
  {
   EmberOgre::Gui::Vector3Adapter* tolua_ret = (EmberOgre::Gui::Vector3Adapter*)  new EmberOgre::Gui::Vector3Adapter(xWindow,yWindow,zWindow);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Gui::Vector3Adapter");
  }
 }
 return 1;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_Gui_Vector3Adapter_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  EmberOgre::Gui::Vector3Adapter */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_Vector3Adapter_delete00
static int tolua_EmberOgre_EmberOgre_Gui_Vector3Adapter_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Vector3Adapter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Vector3Adapter* self = (EmberOgre::Gui::Vector3Adapter*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getValue of class  EmberOgre::Gui::Vector3Adapter */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_Vector3Adapter_getValue00
static int tolua_EmberOgre_EmberOgre_Gui_Vector3Adapter_getValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Gui::Vector3Adapter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Gui::Vector3Adapter* self = (const EmberOgre::Gui::Vector3Adapter*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getValue'",NULL);
#endif
  {
   const Ogre::Vector3& tolua_ret = (const Ogre::Vector3&)  self->getValue();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Ogre::Vector3");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getValue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getOriginalValue of class  EmberOgre::Gui::Vector3Adapter */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_Vector3Adapter_getOriginalValue00
static int tolua_EmberOgre_EmberOgre_Gui_Vector3Adapter_getOriginalValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Gui::Vector3Adapter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Gui::Vector3Adapter* self = (const EmberOgre::Gui::Vector3Adapter*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getOriginalValue'",NULL);
#endif
  {
   const Ogre::Vector3& tolua_ret = (const Ogre::Vector3&)  self->getOriginalValue();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Ogre::Vector3");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getOriginalValue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setValue of class  EmberOgre::Gui::Vector3Adapter */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_Vector3Adapter_setValue00
static int tolua_EmberOgre_EmberOgre_Gui_Vector3Adapter_setValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Vector3Adapter",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Vector3Adapter* self = (EmberOgre::Gui::Vector3Adapter*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector3* vector = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setValue'",NULL);
#endif
  {
   self->setValue(*vector);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setValue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: updateGui of class  EmberOgre::Gui::Vector3Adapter */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_Vector3Adapter_updateGui00
static int tolua_EmberOgre_EmberOgre_Gui_Vector3Adapter_updateGui00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Vector3Adapter",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Vector3Adapter* self = (EmberOgre::Gui::Vector3Adapter*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector3* vector = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'updateGui'",NULL);
#endif
  {
   self->updateGui(*vector);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'updateGui'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventValueChanged of class  EmberOgre::Gui::Vector3Adapter */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__Gui__Vector3Adapter_EventValueChanged
static int tolua_get_EmberOgre__Gui__Vector3Adapter_EventValueChanged(lua_State* tolua_S)
{
  EmberOgre::Gui::Vector3Adapter* self = (EmberOgre::Gui::Vector3Adapter*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventValueChanged'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventValueChanged,"sigc::signal<void>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventValueChanged of class  EmberOgre::Gui::Vector3Adapter */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__Gui__Vector3Adapter_EventValueChanged
static int tolua_set_EmberOgre__Gui__Vector3Adapter_EventValueChanged(lua_State* tolua_S)
{
  EmberOgre::Gui::Vector3Adapter* self = (EmberOgre::Gui::Vector3Adapter*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventValueChanged'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventValueChanged = *((sigc::signal<void>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::Gui::QuaternionAdapter */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_QuaternionAdapter_new00
static int tolua_EmberOgre_EmberOgre_Gui_QuaternionAdapter_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::QuaternionAdapter",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* degreeWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
  CEGUI::Window* xWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,3,0));
  CEGUI::Window* yWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,4,0));
  CEGUI::Window* zWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,5,0));
  {
   EmberOgre::Gui::QuaternionAdapter* tolua_ret = (EmberOgre::Gui::QuaternionAdapter*)  new EmberOgre::Gui::QuaternionAdapter(degreeWindow,xWindow,yWindow,zWindow);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::QuaternionAdapter");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::Gui::QuaternionAdapter */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_QuaternionAdapter_new00_local
static int tolua_EmberOgre_EmberOgre_Gui_QuaternionAdapter_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::QuaternionAdapter",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CEGUI::Window* degreeWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
  CEGUI::Window* xWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,3,0));
  CEGUI::Window* yWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,4,0));
  CEGUI::Window* zWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,5,0));
  {
   EmberOgre::Gui::QuaternionAdapter* tolua_ret = (EmberOgre::Gui::QuaternionAdapter*)  new EmberOgre::Gui::QuaternionAdapter(degreeWindow,xWindow,yWindow,zWindow);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Gui::QuaternionAdapter");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::Gui::QuaternionAdapter */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_QuaternionAdapter_new01
static int tolua_EmberOgre_EmberOgre_Gui_QuaternionAdapter_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::QuaternionAdapter",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,6,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  CEGUI::Window* degreeWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
  CEGUI::Window* xWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,3,0));
  CEGUI::Window* yWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,4,0));
  CEGUI::Window* zWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,5,0));
  const Ogre::Quaternion* quaternion = ((const Ogre::Quaternion*)  tolua_tousertype(tolua_S,6,0));
  {
   EmberOgre::Gui::QuaternionAdapter* tolua_ret = (EmberOgre::Gui::QuaternionAdapter*)  new EmberOgre::Gui::QuaternionAdapter(degreeWindow,xWindow,yWindow,zWindow,*quaternion);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::QuaternionAdapter");
  }
 }
 return 1;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_Gui_QuaternionAdapter_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::Gui::QuaternionAdapter */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_QuaternionAdapter_new01_local
static int tolua_EmberOgre_EmberOgre_Gui_QuaternionAdapter_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::QuaternionAdapter",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,6,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  CEGUI::Window* degreeWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
  CEGUI::Window* xWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,3,0));
  CEGUI::Window* yWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,4,0));
  CEGUI::Window* zWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,5,0));
  const Ogre::Quaternion* quaternion = ((const Ogre::Quaternion*)  tolua_tousertype(tolua_S,6,0));
  {
   EmberOgre::Gui::QuaternionAdapter* tolua_ret = (EmberOgre::Gui::QuaternionAdapter*)  new EmberOgre::Gui::QuaternionAdapter(degreeWindow,xWindow,yWindow,zWindow,*quaternion);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Gui::QuaternionAdapter");
  }
 }
 return 1;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_Gui_QuaternionAdapter_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::Gui::QuaternionAdapter */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_QuaternionAdapter_new02
static int tolua_EmberOgre_EmberOgre_Gui_QuaternionAdapter_new02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::QuaternionAdapter",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  CEGUI::Window* degreeWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
  CEGUI::Window* xWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,3,0));
  CEGUI::Window* yWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,4,0));
  CEGUI::Window* zWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,5,0));
  {
   EmberOgre::Gui::QuaternionAdapter* tolua_ret = (EmberOgre::Gui::QuaternionAdapter*)  new EmberOgre::Gui::QuaternionAdapter(degreeWindow,xWindow,yWindow,zWindow);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::QuaternionAdapter");
  }
 }
 return 1;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_Gui_QuaternionAdapter_new01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::Gui::QuaternionAdapter */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_QuaternionAdapter_new02_local
static int tolua_EmberOgre_EmberOgre_Gui_QuaternionAdapter_new02_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::QuaternionAdapter",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  CEGUI::Window* degreeWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
  CEGUI::Window* xWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,3,0));
  CEGUI::Window* yWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,4,0));
  CEGUI::Window* zWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,5,0));
  {
   EmberOgre::Gui::QuaternionAdapter* tolua_ret = (EmberOgre::Gui::QuaternionAdapter*)  new EmberOgre::Gui::QuaternionAdapter(degreeWindow,xWindow,yWindow,zWindow);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Gui::QuaternionAdapter");
  }
 }
 return 1;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_Gui_QuaternionAdapter_new01_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  EmberOgre::Gui::QuaternionAdapter */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_QuaternionAdapter_delete00
static int tolua_EmberOgre_EmberOgre_Gui_QuaternionAdapter_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::QuaternionAdapter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::QuaternionAdapter* self = (EmberOgre::Gui::QuaternionAdapter*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getValue of class  EmberOgre::Gui::QuaternionAdapter */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_QuaternionAdapter_getValue00
static int tolua_EmberOgre_EmberOgre_Gui_QuaternionAdapter_getValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Gui::QuaternionAdapter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Gui::QuaternionAdapter* self = (const EmberOgre::Gui::QuaternionAdapter*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getValue'",NULL);
#endif
  {
   const Ogre::Quaternion& tolua_ret = (const Ogre::Quaternion&)  self->getValue();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Ogre::Quaternion");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getValue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getOriginalValue of class  EmberOgre::Gui::QuaternionAdapter */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_QuaternionAdapter_getOriginalValue00
static int tolua_EmberOgre_EmberOgre_Gui_QuaternionAdapter_getOriginalValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Gui::QuaternionAdapter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Gui::QuaternionAdapter* self = (const EmberOgre::Gui::QuaternionAdapter*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getOriginalValue'",NULL);
#endif
  {
   const Ogre::Quaternion& tolua_ret = (const Ogre::Quaternion&)  self->getOriginalValue();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Ogre::Quaternion");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getOriginalValue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setValue of class  EmberOgre::Gui::QuaternionAdapter */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_QuaternionAdapter_setValue00
static int tolua_EmberOgre_EmberOgre_Gui_QuaternionAdapter_setValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::QuaternionAdapter",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::QuaternionAdapter* self = (EmberOgre::Gui::QuaternionAdapter*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Quaternion* quaternion = ((const Ogre::Quaternion*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setValue'",NULL);
#endif
  {
   self->setValue(*quaternion);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setValue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: updateGui of class  EmberOgre::Gui::QuaternionAdapter */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_QuaternionAdapter_updateGui00
static int tolua_EmberOgre_EmberOgre_Gui_QuaternionAdapter_updateGui00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::QuaternionAdapter",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::QuaternionAdapter* self = (EmberOgre::Gui::QuaternionAdapter*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Quaternion* vector = ((const Ogre::Quaternion*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'updateGui'",NULL);
#endif
  {
   self->updateGui(*vector);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'updateGui'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventValueChanged of class  EmberOgre::Gui::QuaternionAdapter */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__Gui__QuaternionAdapter_EventValueChanged
static int tolua_get_EmberOgre__Gui__QuaternionAdapter_EventValueChanged(lua_State* tolua_S)
{
  EmberOgre::Gui::QuaternionAdapter* self = (EmberOgre::Gui::QuaternionAdapter*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventValueChanged'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventValueChanged,"sigc::signal<void>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventValueChanged of class  EmberOgre::Gui::QuaternionAdapter */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__Gui__QuaternionAdapter_EventValueChanged
static int tolua_set_EmberOgre__Gui__QuaternionAdapter_EventValueChanged(lua_State* tolua_S)
{
  EmberOgre::Gui::QuaternionAdapter* self = (EmberOgre::Gui::QuaternionAdapter*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventValueChanged'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventValueChanged = *((sigc::signal<void>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::Terrain::Map */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_Map_new00
static int tolua_EmberOgre_EmberOgre_Terrain_Map_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Terrain::Map",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   EmberOgre::Terrain::Map* tolua_ret = (EmberOgre::Terrain::Map*)  new EmberOgre::Terrain::Map();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Terrain::Map");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::Terrain::Map */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_Map_new00_local
static int tolua_EmberOgre_EmberOgre_Terrain_Map_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Terrain::Map",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   EmberOgre::Terrain::Map* tolua_ret = (EmberOgre::Terrain::Map*)  new EmberOgre::Terrain::Map();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Terrain::Map");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  EmberOgre::Terrain::Map */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_Map_delete00
static int tolua_EmberOgre_EmberOgre_Terrain_Map_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Terrain::Map",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Terrain::Map* self = (EmberOgre::Terrain::Map*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: initialize of class  EmberOgre::Terrain::Map */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_Map_initialize00
static int tolua_EmberOgre_EmberOgre_Terrain_Map_initialize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Terrain::Map",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Terrain::Map* self = (EmberOgre::Terrain::Map*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'initialize'",NULL);
#endif
  {
   self->initialize();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'initialize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTexture of class  EmberOgre::Terrain::Map */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_Map_getTexture00
static int tolua_EmberOgre_EmberOgre_Terrain_Map_getTexture00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Terrain::Map",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Terrain::Map* self = (EmberOgre::Terrain::Map*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTexture'",NULL);
#endif
  {
   Ogre::TexturePtr tolua_ret = (Ogre::TexturePtr)  self->getTexture();
   {
#ifdef __cplusplus
    void* tolua_obj = new Ogre::TexturePtr(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::TexturePtr");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::TexturePtr));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::TexturePtr");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTexture'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: render of class  EmberOgre::Terrain::Map */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_Map_render00
static int tolua_EmberOgre_EmberOgre_Terrain_Map_render00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Terrain::Map",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Terrain::Map* self = (EmberOgre::Terrain::Map*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'render'",NULL);
#endif
  {
   self->render();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'render'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: reposition of class  EmberOgre::Terrain::Map */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_Map_reposition00
static int tolua_EmberOgre_EmberOgre_Terrain_Map_reposition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Terrain::Map",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::Vector2",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Terrain::Map* self = (EmberOgre::Terrain::Map*)  tolua_tousertype(tolua_S,1,0);
  Ogre::Vector2 pos = *((Ogre::Vector2*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'reposition'",NULL);
#endif
  {
   self->reposition(pos);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'reposition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: reposition of class  EmberOgre::Terrain::Map */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_Map_reposition01
static int tolua_EmberOgre_EmberOgre_Terrain_Map_reposition01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Terrain::Map",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  EmberOgre::Terrain::Map* self = (EmberOgre::Terrain::Map*)  tolua_tousertype(tolua_S,1,0);
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float y = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'reposition'",NULL);
#endif
  {
   self->reposition(x,y);
  }
 }
 return 0;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_Terrain_Map_reposition00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: setDistance of class  EmberOgre::Terrain::Map */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_Map_setDistance00
static int tolua_EmberOgre_EmberOgre_Terrain_Map_setDistance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Terrain::Map",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Terrain::Map* self = (EmberOgre::Terrain::Map*)  tolua_tousertype(tolua_S,1,0);
  float distance = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setDistance'",NULL);
#endif
  {
   self->setDistance(distance);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setDistance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getDistance of class  EmberOgre::Terrain::Map */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_Map_getDistance00
static int tolua_EmberOgre_EmberOgre_Terrain_Map_getDistance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Terrain::Map",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Terrain::Map* self = (const EmberOgre::Terrain::Map*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getDistance'",NULL);
#endif
  {
   float tolua_ret = (float)  self->getDistance();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getDistance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getResolution of class  EmberOgre::Terrain::Map */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_Map_getResolution00
static int tolua_EmberOgre_EmberOgre_Terrain_Map_getResolution00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Terrain::Map",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Terrain::Map* self = (const EmberOgre::Terrain::Map*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getResolution'",NULL);
#endif
  {
   float tolua_ret = (float)  self->getResolution();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getResolution'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getResolutionMeters of class  EmberOgre::Terrain::Map */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_Map_getResolutionMeters00
static int tolua_EmberOgre_EmberOgre_Terrain_Map_getResolutionMeters00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Terrain::Map",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Terrain::Map* self = (const EmberOgre::Terrain::Map*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getResolutionMeters'",NULL);
#endif
  {
   float tolua_ret = (float)  self->getResolutionMeters();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getResolutionMeters'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getView of class  EmberOgre::Terrain::Map */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_Map_getView00
static int tolua_EmberOgre_EmberOgre_Terrain_Map_getView00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Terrain::Map",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Terrain::Map* self = (EmberOgre::Terrain::Map*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getView'",NULL);
#endif
  {
   EmberOgre::Terrain::MapView& tolua_ret = (EmberOgre::Terrain::MapView&)  self->getView();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"EmberOgre::Terrain::MapView");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getView'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: reposition of class  EmberOgre::Terrain::MapView */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_MapView_reposition00
static int tolua_EmberOgre_EmberOgre_Terrain_MapView_reposition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Terrain::MapView",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::Vector2",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Terrain::MapView* self = (EmberOgre::Terrain::MapView*)  tolua_tousertype(tolua_S,1,0);
  Ogre::Vector2 pos = *((Ogre::Vector2*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'reposition'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->reposition(pos);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'reposition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRelativeViewBounds of class  EmberOgre::Terrain::MapView */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_MapView_getRelativeViewBounds00
static int tolua_EmberOgre_EmberOgre_Terrain_MapView_getRelativeViewBounds00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Terrain::MapView",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Terrain::MapView* self = (const EmberOgre::Terrain::MapView*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRelativeViewBounds'",NULL);
#endif
  {
   const Ogre::TRect<float>& tolua_ret = (const Ogre::TRect<float>&)  self->getRelativeViewBounds();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Ogre::TRect<float>");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRelativeViewBounds'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::OgreInfo */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_OgreInfo_new00
static int tolua_EmberOgre_EmberOgre_OgreInfo_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::OgreInfo",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   EmberOgre::OgreInfo* tolua_ret = (EmberOgre::OgreInfo*)  new EmberOgre::OgreInfo();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::OgreInfo");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::OgreInfo */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_OgreInfo_new00_local
static int tolua_EmberOgre_EmberOgre_OgreInfo_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::OgreInfo",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   EmberOgre::OgreInfo* tolua_ret = (EmberOgre::OgreInfo*)  new EmberOgre::OgreInfo();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::OgreInfo");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  EmberOgre::OgreInfo */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_OgreInfo_delete00
static int tolua_EmberOgre_EmberOgre_OgreInfo_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::OgreInfo",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::OgreInfo* self = (EmberOgre::OgreInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isIndirect of class  EmberOgre::OgreInfo */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_OgreInfo_isIndirect00
static int tolua_EmberOgre_EmberOgre_OgreInfo_isIndirect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::OgreInfo",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::OgreInfo* self = (const EmberOgre::OgreInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isIndirect'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isIndirect();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isIndirect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::AttributeObserver */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_AttributeObserver_new00
static int tolua_EmberOgre_EmberOgre_AttributeObserver_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::AttributeObserver",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Eris::Entity",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Eris::Entity* entity = ((Eris::Entity*)  tolua_tousertype(tolua_S,2,0));
  const std::string attributeName = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  {
   EmberOgre::AttributeObserver* tolua_ret = (EmberOgre::AttributeObserver*)  new EmberOgre::AttributeObserver(entity,attributeName);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::AttributeObserver");
   tolua_pushcppstring(tolua_S,(const char*)attributeName);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::AttributeObserver */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_AttributeObserver_new00_local
static int tolua_EmberOgre_EmberOgre_AttributeObserver_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::AttributeObserver",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Eris::Entity",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Eris::Entity* entity = ((Eris::Entity*)  tolua_tousertype(tolua_S,2,0));
  const std::string attributeName = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  {
   EmberOgre::AttributeObserver* tolua_ret = (EmberOgre::AttributeObserver*)  new EmberOgre::AttributeObserver(entity,attributeName);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::AttributeObserver");
   tolua_pushcppstring(tolua_S,(const char*)attributeName);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  EmberOgre::AttributeObserver */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_AttributeObserver_delete00
static int tolua_EmberOgre_EmberOgre_AttributeObserver_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::AttributeObserver",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::AttributeObserver* self = (EmberOgre::AttributeObserver*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventChanged of class  EmberOgre::AttributeObserver */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__AttributeObserver_EventChanged
static int tolua_get_EmberOgre__AttributeObserver_EventChanged(lua_State* tolua_S)
{
  EmberOgre::AttributeObserver* self = (EmberOgre::AttributeObserver*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventChanged'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventChanged,"sigc::signal<void,const Atlas::Message::Element&>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventChanged of class  EmberOgre::AttributeObserver */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__AttributeObserver_EventChanged
static int tolua_set_EmberOgre__AttributeObserver_EventChanged(lua_State* tolua_S)
{
  EmberOgre::AttributeObserver* self = (EmberOgre::AttributeObserver*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventChanged'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,const Atlas::Message::Element&>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventChanged = *((sigc::signal<void,const Atlas::Message::Element&>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSingleton of class  EmberOgre::EmberOgre */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EmberOgre_getSingleton00
static int tolua_EmberOgre_EmberOgre_EmberOgre_getSingleton00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::EmberOgre",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   EmberOgre::EmberOgre& tolua_ret = (EmberOgre::EmberOgre&)  EmberOgre::EmberOgre::getSingleton();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"EmberOgre::EmberOgre");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSingleton'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAvatar of class  EmberOgre::EmberOgre */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EmberOgre_getAvatar00
static int tolua_EmberOgre_EmberOgre_EmberOgre_getAvatar00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::EmberOgre",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::EmberOgre* self = (const EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAvatar'",NULL);
#endif
  {
   EmberOgre::Avatar* tolua_ret = (EmberOgre::Avatar*)  self->getAvatar();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Avatar");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAvatar'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMotionManager of class  EmberOgre::EmberOgre */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EmberOgre_getMotionManager00
static int tolua_EmberOgre_EmberOgre_EmberOgre_getMotionManager00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::EmberOgre",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::EmberOgre* self = (const EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMotionManager'",NULL);
#endif
  {
   EmberOgre::MotionManager* tolua_ret = (EmberOgre::MotionManager*)  self->getMotionManager();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::MotionManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMotionManager'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getEntityFactory of class  EmberOgre::EmberOgre */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EmberOgre_getEntityFactory00
static int tolua_EmberOgre_EmberOgre_EmberOgre_getEntityFactory00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::EmberOgre",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::EmberOgre* self = (const EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getEntityFactory'",NULL);
#endif
  {
   EmberOgre::EmberEntityFactory* tolua_ret = (EmberOgre::EmberEntityFactory*)  self->getEntityFactory();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::EmberEntityFactory");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getEntityFactory'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMainCamera of class  EmberOgre::EmberOgre */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EmberOgre_getMainCamera00
static int tolua_EmberOgre_EmberOgre_EmberOgre_getMainCamera00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::EmberOgre",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::EmberOgre* self = (const EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMainCamera'",NULL);
#endif
  {
   EmberOgre::AvatarCamera* tolua_ret = (EmberOgre::AvatarCamera*)  self->getMainCamera();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::AvatarCamera");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMainCamera'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAvatarController of class  EmberOgre::EmberOgre */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EmberOgre_getAvatarController00
static int tolua_EmberOgre_EmberOgre_EmberOgre_getAvatarController00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::EmberOgre",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::EmberOgre* self = (const EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAvatarController'",NULL);
#endif
  {
   EmberOgre::AvatarController* tolua_ret = (EmberOgre::AvatarController*)  self->getAvatarController();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::AvatarController");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAvatarController'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMoveManager of class  EmberOgre::EmberOgre */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EmberOgre_getMoveManager00
static int tolua_EmberOgre_EmberOgre_EmberOgre_getMoveManager00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::EmberOgre",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::EmberOgre* self = (const EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMoveManager'",NULL);
#endif
  {
   EmberOgre::EntityMoveManager* tolua_ret = (EmberOgre::EntityMoveManager*)  self->getMoveManager();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::EntityMoveManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMoveManager'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getEmberEntity of class  EmberOgre::EmberOgre */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EmberOgre_getEmberEntity00
static int tolua_EmberOgre_EmberOgre_EmberOgre_getEmberEntity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::EmberOgre",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::EmberOgre* self = (EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
  const std::string eid = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getEmberEntity'",NULL);
#endif
  {
   EmberOgre::EmberEntity* tolua_ret = (EmberOgre::EmberEntity*)  self->getEmberEntity(eid);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::EmberEntity");
   tolua_pushcppstring(tolua_S,(const char*)eid);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getEmberEntity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getJesus of class  EmberOgre::EmberOgre */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EmberOgre_getJesus00
static int tolua_EmberOgre_EmberOgre_EmberOgre_getJesus00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::EmberOgre",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::EmberOgre* self = (const EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getJesus'",NULL);
#endif
  {
   EmberOgre::Jesus* tolua_ret = (EmberOgre::Jesus*)  self->getJesus();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Jesus");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getJesus'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRenderWindow of class  EmberOgre::EmberOgre */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EmberOgre_getRenderWindow00
static int tolua_EmberOgre_EmberOgre_EmberOgre_getRenderWindow00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::EmberOgre",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::EmberOgre* self = (const EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRenderWindow'",NULL);
#endif
  {
   Ogre::RenderWindow* tolua_ret = (Ogre::RenderWindow*)  self->getRenderWindow();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::RenderWindow");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRenderWindow'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventCreatedEmberEntityFactory of class  EmberOgre::EmberOgre */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__EmberOgre_EventCreatedEmberEntityFactory
static int tolua_get_EmberOgre__EmberOgre_EventCreatedEmberEntityFactory(lua_State* tolua_S)
{
  EmberOgre::EmberOgre* self = (EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventCreatedEmberEntityFactory'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventCreatedEmberEntityFactory,"sigc::signal<void,EmberOgre::EmberEntityFactory*>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventCreatedEmberEntityFactory of class  EmberOgre::EmberOgre */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__EmberOgre_EventCreatedEmberEntityFactory
static int tolua_set_EmberOgre__EmberOgre_EventCreatedEmberEntityFactory(lua_State* tolua_S)
{
  EmberOgre::EmberOgre* self = (EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventCreatedEmberEntityFactory'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,EmberOgre::EmberEntityFactory*>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventCreatedEmberEntityFactory = *((sigc::signal<void,EmberOgre::EmberEntityFactory*>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventCreatedAvatarEntity of class  EmberOgre::EmberOgre */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__EmberOgre_EventCreatedAvatarEntity
static int tolua_get_EmberOgre__EmberOgre_EventCreatedAvatarEntity(lua_State* tolua_S)
{
  EmberOgre::EmberOgre* self = (EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventCreatedAvatarEntity'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventCreatedAvatarEntity,"sigc::signal<void,EmberOgre::AvatarEmberEntity*>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventCreatedAvatarEntity of class  EmberOgre::EmberOgre */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__EmberOgre_EventCreatedAvatarEntity
static int tolua_set_EmberOgre__EmberOgre_EventCreatedAvatarEntity(lua_State* tolua_S)
{
  EmberOgre::EmberOgre* self = (EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventCreatedAvatarEntity'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,EmberOgre::AvatarEmberEntity*>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventCreatedAvatarEntity = *((sigc::signal<void,EmberOgre::AvatarEmberEntity*>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventCreatedJesus of class  EmberOgre::EmberOgre */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__EmberOgre_EventCreatedJesus
static int tolua_get_EmberOgre__EmberOgre_EventCreatedJesus(lua_State* tolua_S)
{
  EmberOgre::EmberOgre* self = (EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventCreatedJesus'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventCreatedJesus,"sigc::signal<void,EmberOgre::Jesus*>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventCreatedJesus of class  EmberOgre::EmberOgre */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__EmberOgre_EventCreatedJesus
static int tolua_set_EmberOgre__EmberOgre_EventCreatedJesus(lua_State* tolua_S)
{
  EmberOgre::EmberOgre* self = (EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventCreatedJesus'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,EmberOgre::Jesus*>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventCreatedJesus = *((sigc::signal<void,EmberOgre::Jesus*>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getWorldSceneNode of class  EmberOgre::EmberOgre */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EmberOgre_getWorldSceneNode00
static int tolua_EmberOgre_EmberOgre_EmberOgre_getWorldSceneNode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::EmberOgre",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::EmberOgre* self = (const EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getWorldSceneNode'",NULL);
#endif
  {
   Ogre::SceneNode* tolua_ret = (Ogre::SceneNode*)  self->getWorldSceneNode();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::SceneNode");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getWorldSceneNode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRootSceneNode of class  EmberOgre::EmberOgre */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EmberOgre_getRootSceneNode00
static int tolua_EmberOgre_EmberOgre_EmberOgre_getRootSceneNode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::EmberOgre",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::EmberOgre* self = (const EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRootSceneNode'",NULL);
#endif
  {
   Ogre::SceneNode* tolua_ret = (Ogre::SceneNode*)  self->getRootSceneNode();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::SceneNode");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRootSceneNode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventGUIManagerCreated of class  EmberOgre::EmberOgre */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__EmberOgre_EventGUIManagerCreated
static int tolua_get_EmberOgre__EmberOgre_EventGUIManagerCreated(lua_State* tolua_S)
{
  EmberOgre::EmberOgre* self = (EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventGUIManagerCreated'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventGUIManagerCreated,"sigc::signal<void,EmberOgre::GUIManager&>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventGUIManagerCreated of class  EmberOgre::EmberOgre */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__EmberOgre_EventGUIManagerCreated
static int tolua_set_EmberOgre__EmberOgre_EventGUIManagerCreated(lua_State* tolua_S)
{
  EmberOgre::EmberOgre* self = (EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventGUIManagerCreated'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,EmberOgre::GUIManager&>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventGUIManagerCreated = *((sigc::signal<void,EmberOgre::GUIManager&>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventGUIManagerInitialized of class  EmberOgre::EmberOgre */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__EmberOgre_EventGUIManagerInitialized
static int tolua_get_EmberOgre__EmberOgre_EventGUIManagerInitialized(lua_State* tolua_S)
{
  EmberOgre::EmberOgre* self = (EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventGUIManagerInitialized'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventGUIManagerInitialized,"sigc::signal<void,EmberOgre::GUIManager&>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventGUIManagerInitialized of class  EmberOgre::EmberOgre */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__EmberOgre_EventGUIManagerInitialized
static int tolua_set_EmberOgre__EmberOgre_EventGUIManagerInitialized(lua_State* tolua_S)
{
  EmberOgre::EmberOgre* self = (EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventGUIManagerInitialized'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,EmberOgre::GUIManager&>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventGUIManagerInitialized = *((sigc::signal<void,EmberOgre::GUIManager&>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventMotionManagerCreated of class  EmberOgre::EmberOgre */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__EmberOgre_EventMotionManagerCreated
static int tolua_get_EmberOgre__EmberOgre_EventMotionManagerCreated(lua_State* tolua_S)
{
  EmberOgre::EmberOgre* self = (EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventMotionManagerCreated'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventMotionManagerCreated,"sigc::signal<void,EmberOgre::MotionManager&>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventMotionManagerCreated of class  EmberOgre::EmberOgre */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__EmberOgre_EventMotionManagerCreated
static int tolua_set_EmberOgre__EmberOgre_EventMotionManagerCreated(lua_State* tolua_S)
{
  EmberOgre::EmberOgre* self = (EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventMotionManagerCreated'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,EmberOgre::MotionManager&>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventMotionManagerCreated = *((sigc::signal<void,EmberOgre::MotionManager&>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventTerrainGeneratorCreated of class  EmberOgre::EmberOgre */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__EmberOgre_EventTerrainGeneratorCreated
static int tolua_get_EmberOgre__EmberOgre_EventTerrainGeneratorCreated(lua_State* tolua_S)
{
  EmberOgre::EmberOgre* self = (EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventTerrainGeneratorCreated'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventTerrainGeneratorCreated,"sigc::signal<void,EmberOgre::Terrain::TerrainGenerator&>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventTerrainGeneratorCreated of class  EmberOgre::EmberOgre */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__EmberOgre_EventTerrainGeneratorCreated
static int tolua_set_EmberOgre__EmberOgre_EventTerrainGeneratorCreated(lua_State* tolua_S)
{
  EmberOgre::EmberOgre* self = (EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventTerrainGeneratorCreated'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,EmberOgre::Terrain::TerrainGenerator&>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventTerrainGeneratorCreated = *((sigc::signal<void,EmberOgre::Terrain::TerrainGenerator&>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventAvatarControllerCreated of class  EmberOgre::EmberOgre */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__EmberOgre_EventAvatarControllerCreated
static int tolua_get_EmberOgre__EmberOgre_EventAvatarControllerCreated(lua_State* tolua_S)
{
  EmberOgre::EmberOgre* self = (EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventAvatarControllerCreated'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventAvatarControllerCreated,"sigc::signal<void,EmberOgre::AvatarController&>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventAvatarControllerCreated of class  EmberOgre::EmberOgre */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__EmberOgre_EventAvatarControllerCreated
static int tolua_set_EmberOgre__EmberOgre_EventAvatarControllerCreated(lua_State* tolua_S)
{
  EmberOgre::EmberOgre* self = (EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventAvatarControllerCreated'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,EmberOgre::AvatarController&>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventAvatarControllerCreated = *((sigc::signal<void,EmberOgre::AvatarController&>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventSceneCreated of class  EmberOgre::EmberOgre */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__EmberOgre_EventSceneCreated
static int tolua_get_EmberOgre__EmberOgre_EventSceneCreated(lua_State* tolua_S)
{
  EmberOgre::EmberOgre* self = (EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventSceneCreated'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventSceneCreated,"sigc::signal<void>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventSceneCreated of class  EmberOgre::EmberOgre */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__EmberOgre_EventSceneCreated
static int tolua_set_EmberOgre__EmberOgre_EventSceneCreated(lua_State* tolua_S)
{
  EmberOgre::EmberOgre* self = (EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventSceneCreated'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventSceneCreated = *((sigc::signal<void>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getEntity of class  EmberOgre::EmberOgre */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EmberOgre_getEntity00
static int tolua_EmberOgre_EmberOgre_EmberOgre_getEntity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::EmberOgre",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::EmberOgre* self = (EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
  const std::string id = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getEntity'",NULL);
#endif
  {
   EmberOgre::EmberEntity* tolua_ret = (EmberOgre::EmberEntity*)  self->getEntity(id);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::EmberEntity");
   tolua_pushcppstring(tolua_S,(const char*)id);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getEntity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_EmberOgre_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_module(tolua_S,"Carpenter",0);
  tolua_beginmodule(tolua_S,"Carpenter");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"AttachPair","Carpenter::AttachPair","",tolua_collect_Carpenter__AttachPair);
   #else
   tolua_cclass(tolua_S,"AttachPair","Carpenter::AttachPair","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"AttachPair");
    tolua_function(tolua_S,"getAttachPoint",tolua_EmberOgre_Carpenter_AttachPair_getAttachPoint00);
    tolua_function(tolua_S,"new",tolua_EmberOgre_Carpenter_AttachPair_new00);
    tolua_function(tolua_S,"new_local",tolua_EmberOgre_Carpenter_AttachPair_new00_local);
    tolua_function(tolua_S,".call",tolua_EmberOgre_Carpenter_AttachPair_new00_local);
    tolua_function(tolua_S,"getPoint1",tolua_EmberOgre_Carpenter_AttachPair_getPoint100);
    tolua_function(tolua_S,"getPoint2",tolua_EmberOgre_Carpenter_AttachPair_getPoint200);
    tolua_function(tolua_S,"getName",tolua_EmberOgre_Carpenter_AttachPair_getName00);
    tolua_function(tolua_S,"getType",tolua_EmberOgre_Carpenter_AttachPair_getType00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"AttachPoint","Carpenter::AttachPoint","",tolua_collect_Carpenter__AttachPoint);
   #else
   tolua_cclass(tolua_S,"AttachPoint","Carpenter::AttachPoint","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"AttachPoint");
    tolua_function(tolua_S,"new",tolua_EmberOgre_Carpenter_AttachPoint_new00);
    tolua_function(tolua_S,"new_local",tolua_EmberOgre_Carpenter_AttachPoint_new00_local);
    tolua_function(tolua_S,".call",tolua_EmberOgre_Carpenter_AttachPoint_new00_local);
    tolua_function(tolua_S,"getName",tolua_EmberOgre_Carpenter_AttachPoint_getName00);
    tolua_function(tolua_S,"getNormal",tolua_EmberOgre_Carpenter_AttachPoint_getNormal00);
    tolua_function(tolua_S,"getPosition",tolua_EmberOgre_Carpenter_AttachPoint_getPosition00);
    tolua_function(tolua_S,"getAttachPair",tolua_EmberOgre_Carpenter_AttachPoint_getAttachPair00);
    tolua_function(tolua_S,"getSibling",tolua_EmberOgre_Carpenter_AttachPoint_getSibling00);
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"BlockSpec","Carpenter::BlockSpec","",NULL);
   tolua_beginmodule(tolua_S,"BlockSpec");
    tolua_function(tolua_S,"getName",tolua_EmberOgre_Carpenter_BlockSpec_getName00);
    tolua_function(tolua_S,"getBoundingBox",tolua_EmberOgre_Carpenter_BlockSpec_getBoundingBox00);
    tolua_function(tolua_S,"getAttachPair",tolua_EmberOgre_Carpenter_BlockSpec_getAttachPair00);
    tolua_function(tolua_S,"addAttachPair",tolua_EmberOgre_Carpenter_BlockSpec_addAttachPair00);
    tolua_function(tolua_S,"setBoundingBox",tolua_EmberOgre_Carpenter_BlockSpec_setBoundingBox00);
    tolua_function(tolua_S,"getAllPoints",tolua_EmberOgre_Carpenter_BlockSpec_getAllPoints00);
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"BuildingBlock","Carpenter::BuildingBlock","",NULL);
   tolua_beginmodule(tolua_S,"BuildingBlock");
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"BuildingBlockSpec","Carpenter::BuildingBlockSpec","",tolua_collect_Carpenter__BuildingBlockSpec);
   #else
   tolua_cclass(tolua_S,"BuildingBlockSpec","Carpenter::BuildingBlockSpec","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"BuildingBlockSpec");
    tolua_function(tolua_S,"new",tolua_EmberOgre_Carpenter_BuildingBlockSpec_new00);
    tolua_function(tolua_S,"new_local",tolua_EmberOgre_Carpenter_BuildingBlockSpec_new00_local);
    tolua_function(tolua_S,".call",tolua_EmberOgre_Carpenter_BuildingBlockSpec_new00_local);
    tolua_function(tolua_S,"getDefinition",tolua_EmberOgre_Carpenter_BuildingBlockSpec_getDefinition00);
    tolua_function(tolua_S,"getBlockSpec",tolua_EmberOgre_Carpenter_BuildingBlockSpec_getBlockSpec00);
    tolua_function(tolua_S,"getName",tolua_EmberOgre_Carpenter_BuildingBlockSpec_getName00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"BuildingBlockSpecDefinition","Carpenter::BuildingBlockSpecDefinition","",tolua_collect_Carpenter__BuildingBlockSpecDefinition);
   #else
   tolua_cclass(tolua_S,"BuildingBlockSpecDefinition","Carpenter::BuildingBlockSpecDefinition","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"BuildingBlockSpecDefinition");
    tolua_function(tolua_S,"new",tolua_EmberOgre_Carpenter_BuildingBlockSpecDefinition_new00);
    tolua_function(tolua_S,"new_local",tolua_EmberOgre_Carpenter_BuildingBlockSpecDefinition_new00_local);
    tolua_function(tolua_S,".call",tolua_EmberOgre_Carpenter_BuildingBlockSpecDefinition_new00_local);
    tolua_variable(tolua_S,"mName",tolua_get_Carpenter__BuildingBlockSpecDefinition_mName,tolua_set_Carpenter__BuildingBlockSpecDefinition_mName);
    tolua_variable(tolua_S,"mBlockSpecName",tolua_get_Carpenter__BuildingBlockSpecDefinition_mBlockSpecName,tolua_set_Carpenter__BuildingBlockSpecDefinition_mBlockSpecName);
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"BuildingBlockBinding","Carpenter::BuildingBlockBinding","",NULL);
   tolua_beginmodule(tolua_S,"BuildingBlockBinding");
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"BuildingBlockBindingDefinition","Carpenter::BuildingBlockBindingDefinition","",NULL);
   tolua_beginmodule(tolua_S,"BuildingBlockBindingDefinition");
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"BuildingBlockDefinition","Carpenter::BuildingBlockDefinition","",NULL);
   tolua_beginmodule(tolua_S,"BuildingBlockDefinition");
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"BluePrint","Carpenter::BluePrint","",NULL);
   tolua_beginmodule(tolua_S,"BluePrint");
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Carpenter","Carpenter::Carpenter","",tolua_collect_Carpenter__Carpenter);
   #else
   tolua_cclass(tolua_S,"Carpenter","Carpenter::Carpenter","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Carpenter");
    tolua_function(tolua_S,"new",tolua_EmberOgre_Carpenter_Carpenter_new00);
    tolua_function(tolua_S,"new_local",tolua_EmberOgre_Carpenter_Carpenter_new00_local);
    tolua_function(tolua_S,".call",tolua_EmberOgre_Carpenter_Carpenter_new00_local);
    tolua_function(tolua_S,"delete",tolua_EmberOgre_Carpenter_Carpenter_delete00);
    tolua_function(tolua_S,"createBlueprint",tolua_EmberOgre_Carpenter_Carpenter_createBlueprint00);
    tolua_function(tolua_S,"getBuildingBlockSpec",tolua_EmberOgre_Carpenter_Carpenter_getBuildingBlockSpec00);
    tolua_function(tolua_S,"createBlockSpec",tolua_EmberOgre_Carpenter_Carpenter_createBlockSpec00);
    tolua_function(tolua_S,"createBuildingBlockSpec",tolua_EmberOgre_Carpenter_Carpenter_createBuildingBlockSpec00);
    tolua_function(tolua_S,"getBlockSpecs",tolua_EmberOgre_Carpenter_Carpenter_getBlockSpecs00);
    tolua_function(tolua_S,"getBuildingBlockSpecs",tolua_EmberOgre_Carpenter_Carpenter_getBuildingBlockSpecs00);
    tolua_function(tolua_S,"getBluePrints",tolua_EmberOgre_Carpenter_Carpenter_getBluePrints00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_cclass(tolua_S,"AvatarCamera","EmberOgre::AvatarCamera","",NULL);
   tolua_beginmodule(tolua_S,"AvatarCamera");
    tolua_constant(tolua_S,"MODE_THIRD_PERSON",EmberOgre::AvatarCamera::MODE_THIRD_PERSON);
    tolua_constant(tolua_S,"MODE_FIRST_PERSON",EmberOgre::AvatarCamera::MODE_FIRST_PERSON);
    tolua_function(tolua_S,"pitch",tolua_EmberOgre_EmberOgre_AvatarCamera_pitch00);
    tolua_function(tolua_S,"yaw",tolua_EmberOgre_EmberOgre_AvatarCamera_yaw00);
    tolua_function(tolua_S,"getPitch",tolua_EmberOgre_EmberOgre_AvatarCamera_getPitch00);
    tolua_function(tolua_S,"getYaw",tolua_EmberOgre_EmberOgre_AvatarCamera_getYaw00);
    tolua_function(tolua_S,"getCamera",tolua_EmberOgre_EmberOgre_AvatarCamera_getCamera00);
    tolua_function(tolua_S,"getCamera",tolua_EmberOgre_EmberOgre_AvatarCamera_getCamera01);
    tolua_function(tolua_S,"getOrientation",tolua_EmberOgre_EmberOgre_AvatarCamera_getOrientation00);
    tolua_function(tolua_S,"getPosition",tolua_EmberOgre_EmberOgre_AvatarCamera_getPosition00);
    tolua_function(tolua_S,"setMode",tolua_EmberOgre_EmberOgre_AvatarCamera_setMode00);
    tolua_function(tolua_S,"setAvatarNode",tolua_EmberOgre_EmberOgre_AvatarCamera_setAvatarNode00);
    tolua_variable(tolua_S,"MovedCamera",tolua_get_EmberOgre__AvatarCamera_MovedCamera,tolua_set_EmberOgre__AvatarCamera_MovedCamera);
    tolua_variable(tolua_S,"EventChangedCameraDistance",tolua_get_EmberOgre__AvatarCamera_EventChangedCameraDistance,tolua_set_EmberOgre__AvatarCamera_EventChangedCameraDistance);
    tolua_function(tolua_S,"pickInWorld",tolua_EmberOgre_EmberOgre_AvatarCamera_pickInWorld00);
    tolua_function(tolua_S,"worldToScreen",tolua_EmberOgre_EmberOgre_AvatarCamera_worldToScreen00);
    tolua_function(tolua_S,"attach",tolua_EmberOgre_EmberOgre_AvatarCamera_attach00);
    tolua_function(tolua_S,"setCameraDistance",tolua_EmberOgre_EmberOgre_AvatarCamera_setCameraDistance00);
    tolua_function(tolua_S,"enableCompositor",tolua_EmberOgre_EmberOgre_AvatarCamera_enableCompositor00);
    tolua_function(tolua_S,"toggleRenderMode",tolua_EmberOgre_EmberOgre_AvatarCamera_toggleRenderMode00);
    tolua_function(tolua_S,"takeScreenshot",tolua_EmberOgre_EmberOgre_AvatarCamera_takeScreenshot00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_cclass(tolua_S,"AvatarMovementMode","EmberOgre::AvatarMovementMode","",NULL);
   tolua_beginmodule(tolua_S,"AvatarMovementMode");
    tolua_constant(tolua_S,"MM_WALK",EmberOgre::AvatarMovementMode::MM_WALK);
    tolua_constant(tolua_S,"MM_RUN",EmberOgre::AvatarMovementMode::MM_RUN);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"AvatarControllerMovement","EmberOgre::AvatarControllerMovement","",tolua_collect_EmberOgre__AvatarControllerMovement);
   #else
   tolua_cclass(tolua_S,"AvatarControllerMovement","EmberOgre::AvatarControllerMovement","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"AvatarControllerMovement");
    tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_AvatarControllerMovement_new00);
    tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_AvatarControllerMovement_new00_local);
    tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_AvatarControllerMovement_new00_local);
    tolua_variable(tolua_S,"rotationDegHoriz",tolua_get_EmberOgre__AvatarControllerMovement_rotationDegHoriz,tolua_set_EmberOgre__AvatarControllerMovement_rotationDegHoriz);
    tolua_variable(tolua_S,"rotationDegVert",tolua_get_EmberOgre__AvatarControllerMovement_rotationDegVert,tolua_set_EmberOgre__AvatarControllerMovement_rotationDegVert);
    tolua_variable(tolua_S,"timeSlice",tolua_get_EmberOgre__AvatarControllerMovement_timeSlice,tolua_set_EmberOgre__AvatarControllerMovement_timeSlice);
    tolua_variable(tolua_S,"movementDirection",tolua_get_EmberOgre__AvatarControllerMovement_movementDirection,tolua_set_EmberOgre__AvatarControllerMovement_movementDirection);
    tolua_variable(tolua_S,"mode",tolua_get_EmberOgre__AvatarControllerMovement_mode,tolua_set_EmberOgre__AvatarControllerMovement_mode);
    tolua_variable(tolua_S,"isMoving",tolua_get_EmberOgre__AvatarControllerMovement_isMoving,tolua_set_EmberOgre__AvatarControllerMovement_isMoving);
    tolua_variable(tolua_S,"cameraOrientation",tolua_get_EmberOgre__AvatarControllerMovement_cameraOrientation,tolua_set_EmberOgre__AvatarControllerMovement_cameraOrientation);
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"AvatarController","EmberOgre::AvatarController","",NULL);
   tolua_beginmodule(tolua_S,"AvatarController");
    tolua_variable(tolua_S,"EventMovementModeChanged",tolua_get_EmberOgre__AvatarController_EventMovementModeChanged,tolua_set_EmberOgre__AvatarController_EventMovementModeChanged);
    tolua_function(tolua_S,"getAvatarCamera",tolua_EmberOgre_EmberOgre_AvatarController_getAvatarCamera00);
    tolua_function(tolua_S,"detachCamera",tolua_EmberOgre_EmberOgre_AvatarController_detachCamera00);
    tolua_function(tolua_S,"attachCamera",tolua_EmberOgre_EmberOgre_AvatarController_attachCamera00);
    tolua_function(tolua_S,"getCurrentMovement",tolua_EmberOgre_EmberOgre_AvatarController_getCurrentMovement00);
    tolua_function(tolua_S,"moveToPoint",tolua_EmberOgre_EmberOgre_AvatarController_moveToPoint00);
    tolua_function(tolua_S,"teleportTo",tolua_EmberOgre_EmberOgre_AvatarController_teleportTo00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_cclass(tolua_S,"AvatarMovementState","EmberOgre::AvatarMovementState","",NULL);
   tolua_beginmodule(tolua_S,"AvatarMovementState");
    tolua_variable(tolua_S,"isMoving",tolua_get_EmberOgre__AvatarMovementState_isMoving,tolua_set_EmberOgre__AvatarMovementState_isMoving);
    tolua_variable(tolua_S,"isRunning",tolua_get_EmberOgre__AvatarMovementState_isRunning,tolua_set_EmberOgre__AvatarMovementState_isRunning);
    tolua_variable(tolua_S,"velocity",tolua_get_EmberOgre__AvatarMovementState_velocity,tolua_set_EmberOgre__AvatarMovementState_velocity);
    tolua_variable(tolua_S,"orientation",tolua_get_EmberOgre__AvatarMovementState_orientation,tolua_set_EmberOgre__AvatarMovementState_orientation);
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"Avatar","EmberOgre::Avatar","",NULL);
   tolua_beginmodule(tolua_S,"Avatar");
    tolua_function(tolua_S,"getAvatarCamera",tolua_EmberOgre_EmberOgre_Avatar_getAvatarCamera00);
    tolua_function(tolua_S,"getAvatarSceneNode",tolua_EmberOgre_EmberOgre_Avatar_getAvatarSceneNode00);
    tolua_function(tolua_S,"setAvatarController",tolua_EmberOgre_EmberOgre_Avatar_setAvatarController00);
    tolua_function(tolua_S,"getAvatarEmberEntity",tolua_EmberOgre_EmberOgre_Avatar_getAvatarEmberEntity00);
    tolua_function(tolua_S,"setMinIntervalOfRotationChanges",tolua_EmberOgre_EmberOgre_Avatar_setMinIntervalOfRotationChanges00);
    tolua_variable(tolua_S,"EventAddedEntityToInventory",tolua_get_EmberOgre__Avatar_EventAddedEntityToInventory,tolua_set_EmberOgre__Avatar_EventAddedEntityToInventory);
    tolua_variable(tolua_S,"EventRemovedEntityFromInventory",tolua_get_EmberOgre__Avatar_EventRemovedEntityFromInventory,tolua_set_EmberOgre__Avatar_EventRemovedEntityFromInventory);
    tolua_function(tolua_S,"isAdmin",tolua_EmberOgre_EmberOgre_Avatar_isAdmin00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_cclass(tolua_S,"EmberEntityFactory","EmberOgre::EmberEntityFactory","",NULL);
   tolua_beginmodule(tolua_S,"EmberEntityFactory");
    tolua_function(tolua_S,"getWorld",tolua_EmberOgre_EmberOgre_EmberEntityFactory_getWorld00);
    tolua_function(tolua_S,"dumpAttributesOfEntity",tolua_EmberOgre_EmberOgre_EmberEntityFactory_dumpAttributesOfEntity00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_cclass(tolua_S,"EmberEntity","EmberOgre::EmberEntity","Eris::Entity",NULL);
   tolua_beginmodule(tolua_S,"EmberEntity");
    tolua_constant(tolua_S,"MM_DEFAULT",EmberOgre::EmberEntity::MM_DEFAULT);
    tolua_constant(tolua_S,"MM_STANDING",EmberOgre::EmberEntity::MM_STANDING);
    tolua_constant(tolua_S,"MM_FLOATING",EmberOgre::EmberEntity::MM_FLOATING);
    tolua_constant(tolua_S,"MM_PROJECTILE",EmberOgre::EmberEntity::MM_PROJECTILE);
    tolua_constant(tolua_S,"MM_SWIMMING",EmberOgre::EmberEntity::MM_SWIMMING);
    tolua_constant(tolua_S,"MM_WALKING",EmberOgre::EmberEntity::MM_WALKING);
    tolua_constant(tolua_S,"MM_RUNNING",EmberOgre::EmberEntity::MM_RUNNING);
    tolua_constant(tolua_S,"MM_FIXED",EmberOgre::EmberEntity::MM_FIXED);
    tolua_function(tolua_S,"getSceneNode",tolua_EmberOgre_EmberOgre_EmberEntity_getSceneNode00);
    tolua_function(tolua_S,"hasSuggestedResponses",tolua_EmberOgre_EmberOgre_EmberEntity_hasSuggestedResponses00);
    tolua_function(tolua_S,"getSuggestedResponses",tolua_EmberOgre_EmberOgre_EmberEntity_getSuggestedResponses00);
    tolua_function(tolua_S,"setVisible",tolua_EmberOgre_EmberOgre_EmberEntity_setVisible00);
    tolua_function(tolua_S,"getEmberLocation",tolua_EmberOgre_EmberOgre_EmberEntity_getEmberLocation00);
    tolua_function(tolua_S,"isInitialized",tolua_EmberOgre_EmberOgre_EmberEntity_isInitialized00);
    tolua_function(tolua_S,"getMovementMode",tolua_EmberOgre_EmberOgre_EmberEntity_getMovementMode00);
    tolua_function(tolua_S,"showOgreBoundingBox",tolua_EmberOgre_EmberOgre_EmberEntity_showOgreBoundingBox00);
    tolua_function(tolua_S,"showErisBoundingBox",tolua_EmberOgre_EmberOgre_EmberEntity_showErisBoundingBox00);
    tolua_function(tolua_S,"getShowOgreBoundingBox",tolua_EmberOgre_EmberOgre_EmberEntity_getShowOgreBoundingBox00);
    tolua_function(tolua_S,"getShowErisBoundingBox",tolua_EmberOgre_EmberOgre_EmberEntity_getShowErisBoundingBox00);
    tolua_function(tolua_S,"getWorldBoundingBox",tolua_EmberOgre_EmberOgre_EmberEntity_getWorldBoundingBox00);
    tolua_function(tolua_S,"getDefaultUseOperators",tolua_EmberOgre_EmberOgre_EmberEntity_getDefaultUseOperators00);
    tolua_function(tolua_S,"setVisualize",tolua_EmberOgre_EmberOgre_EmberEntity_setVisualize00);
    tolua_function(tolua_S,"getVisualize",tolua_EmberOgre_EmberOgre_EmberEntity_getVisualize00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_cclass(tolua_S,"EmberPhysicalEntity","EmberOgre::EmberPhysicalEntity","EmberOgre::EmberEntity",NULL);
   tolua_beginmodule(tolua_S,"EmberPhysicalEntity");
    tolua_variable(tolua_S,"ACTION_STAND",tolua_get_EmberOgre__EmberPhysicalEntity_ACTION_STAND,NULL);
    tolua_variable(tolua_S,"ACTION_RUN",tolua_get_EmberOgre__EmberPhysicalEntity_ACTION_RUN,NULL);
    tolua_variable(tolua_S,"ACTION_WALK",tolua_get_EmberOgre__EmberPhysicalEntity_ACTION_WALK,NULL);
    tolua_variable(tolua_S,"ACTION_SWIM",tolua_get_EmberOgre__EmberPhysicalEntity_ACTION_SWIM,NULL);
    tolua_variable(tolua_S,"ACTION_FLOAT",tolua_get_EmberOgre__EmberPhysicalEntity_ACTION_FLOAT,NULL);
    tolua_function(tolua_S,"getModel",tolua_EmberOgre_EmberOgre_EmberPhysicalEntity_getModel00);
    tolua_function(tolua_S,"getScaleNode",tolua_EmberOgre_EmberOgre_EmberPhysicalEntity_getScaleNode00);
    tolua_function(tolua_S,"getEntityAttachedToPoint",tolua_EmberOgre_EmberOgre_EmberPhysicalEntity_getEntityAttachedToPoint00);
    tolua_function(tolua_S,"getWorldBoundingBox",tolua_EmberOgre_EmberOgre_EmberPhysicalEntity_getWorldBoundingBox00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_cclass(tolua_S,"AvatarEmberEntity","EmberOgre::AvatarEmberEntity","EmberOgre::EmberPhysicalEntity",NULL);
   tolua_beginmodule(tolua_S,"AvatarEmberEntity");
    tolua_function(tolua_S,"getAvatar",tolua_EmberOgre_EmberOgre_AvatarEmberEntity_getAvatar00);
    tolua_function(tolua_S,"getAvatarSceneNode",tolua_EmberOgre_EmberOgre_AvatarEmberEntity_getAvatarSceneNode00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_cclass(tolua_S,"WorldEmberEntity","EmberOgre::WorldEmberEntity","EmberOgre::EmberEntity",NULL);
   tolua_beginmodule(tolua_S,"WorldEmberEntity");
    tolua_function(tolua_S,"getEnvironment",tolua_EmberOgre_EmberOgre_WorldEmberEntity_getEnvironment00);
    tolua_function(tolua_S,"getFoliage",tolua_EmberOgre_EmberOgre_WorldEmberEntity_getFoliage00);
    tolua_variable(tolua_S,"EventFoliageCreated",tolua_get_EmberOgre__WorldEmberEntity_EventFoliageCreated,tolua_set_EmberOgre__WorldEmberEntity_EventFoliageCreated);
    tolua_variable(tolua_S,"EventEnvironmentCreated",tolua_get_EmberOgre__WorldEmberEntity_EventEnvironmentCreated,tolua_set_EmberOgre__WorldEmberEntity_EventEnvironmentCreated);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_module(tolua_S,"Environment",0);
   tolua_beginmodule(tolua_S,"Environment");
    tolua_cclass(tolua_S,"ISun","EmberOgre::Environment::ISun","",NULL);
    tolua_beginmodule(tolua_S,"ISun");
     tolua_function(tolua_S,"setAmbientLight",tolua_EmberOgre_EmberOgre_Environment_ISun_setAmbientLight00);
    tolua_endmodule(tolua_S);
    tolua_cclass(tolua_S,"ISky","EmberOgre::Environment::ISky","",NULL);
    tolua_beginmodule(tolua_S,"ISky");
    tolua_endmodule(tolua_S);
    tolua_cclass(tolua_S,"IFog","EmberOgre::Environment::IFog","",NULL);
    tolua_beginmodule(tolua_S,"IFog");
     tolua_function(tolua_S,"setDensity",tolua_EmberOgre_EmberOgre_Environment_IFog_setDensity00);
     tolua_function(tolua_S,"getDensity",tolua_EmberOgre_EmberOgre_Environment_IFog_getDensity00);
    tolua_endmodule(tolua_S);
    tolua_cclass(tolua_S,"IWater","EmberOgre::Environment::IWater","",NULL);
    tolua_beginmodule(tolua_S,"IWater");
    tolua_endmodule(tolua_S);
    tolua_cclass(tolua_S,"Environment","EmberOgre::Environment::Environment","",NULL);
    tolua_beginmodule(tolua_S,"Environment");
     tolua_function(tolua_S,"getSun",tolua_EmberOgre_EmberOgre_Environment_Environment_getSun00);
     tolua_function(tolua_S,"getSky",tolua_EmberOgre_EmberOgre_Environment_Environment_getSky00);
     tolua_function(tolua_S,"getFog",tolua_EmberOgre_EmberOgre_Environment_Environment_getFog00);
     tolua_function(tolua_S,"getWater",tolua_EmberOgre_EmberOgre_Environment_Environment_getWater00);
     tolua_function(tolua_S,"setTime",tolua_EmberOgre_EmberOgre_Environment_Environment_setTime00);
     tolua_function(tolua_S,"setTime",tolua_EmberOgre_EmberOgre_Environment_Environment_setTime01);
     tolua_function(tolua_S,"setAmbientLight",tolua_EmberOgre_EmberOgre_Environment_Environment_setAmbientLight00);
     tolua_variable(tolua_S,"EventUpdatedAmbientLight",tolua_get_EmberOgre__Environment__Environment_EventUpdatedAmbientLight,tolua_set_EmberOgre__Environment__Environment_EventUpdatedAmbientLight);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_cclass(tolua_S,"GUIManager","EmberOgre::GUIManager","",NULL);
   tolua_beginmodule(tolua_S,"GUIManager");
    tolua_function(tolua_S,"getSingleton",tolua_EmberOgre_EmberOgre_GUIManager_getSingleton00);
    tolua_variable(tolua_S,"AppendIGChatLine",tolua_get_EmberOgre__GUIManager_AppendIGChatLine,tolua_set_EmberOgre__GUIManager_AppendIGChatLine);
    tolua_variable(tolua_S,"AppendOOGChatLine",tolua_get_EmberOgre__GUIManager_AppendOOGChatLine,tolua_set_EmberOgre__GUIManager_AppendOOGChatLine);
    tolua_variable(tolua_S,"AppendAvatarImaginary",tolua_get_EmberOgre__GUIManager_AppendAvatarImaginary,tolua_set_EmberOgre__GUIManager_AppendAvatarImaginary);
    tolua_variable(tolua_S,"EventEntityAction",tolua_get_EmberOgre__GUIManager_EventEntityAction,tolua_set_EmberOgre__GUIManager_EventEntityAction);
    tolua_variable(tolua_S,"EventFrameStarted",tolua_get_EmberOgre__GUIManager_EventFrameStarted,tolua_set_EmberOgre__GUIManager_EventFrameStarted);
    tolua_function(tolua_S,"removeWidget",tolua_EmberOgre_EmberOgre_GUIManager_removeWidget00);
    tolua_function(tolua_S,"addWidget",tolua_EmberOgre_EmberOgre_GUIManager_addWidget00);
    tolua_function(tolua_S,"EmitEntityAction",tolua_EmberOgre_EmberOgre_GUIManager_EmitEntityAction00);
    tolua_function(tolua_S,"getMainSheet",tolua_EmberOgre_EmberOgre_GUIManager_getMainSheet00);
    tolua_function(tolua_S,"setDebugText",tolua_EmberOgre_EmberOgre_GUIManager_setDebugText00);
    tolua_function(tolua_S,"isInGUIMode",tolua_EmberOgre_EmberOgre_GUIManager_isInGUIMode00);
    tolua_function(tolua_S,"isInMovementKeysMode",tolua_EmberOgre_EmberOgre_GUIManager_isInMovementKeysMode00);
    tolua_function(tolua_S,"getInput",tolua_EmberOgre_EmberOgre_GUIManager_getInput00);
    tolua_function(tolua_S,"createWindow",tolua_EmberOgre_EmberOgre_GUIManager_createWindow00);
    tolua_function(tolua_S,"createWindow",tolua_EmberOgre_EmberOgre_GUIManager_createWindow01);
    tolua_function(tolua_S,"createWidget",tolua_EmberOgre_EmberOgre_GUIManager_createWidget00);
    tolua_function(tolua_S,"createWidget",tolua_EmberOgre_EmberOgre_GUIManager_createWidget01);
    tolua_function(tolua_S,"getDefaultScheme",tolua_EmberOgre_EmberOgre_GUIManager_getDefaultScheme00);
    tolua_function(tolua_S,"getEntityPickListener",tolua_EmberOgre_EmberOgre_GUIManager_getEntityPickListener00);
    tolua_function(tolua_S,"getIconManager",tolua_EmberOgre_EmberOgre_GUIManager_getIconManager00);
    tolua_function(tolua_S,"getEntityIconManager",tolua_EmberOgre_EmberOgre_GUIManager_getEntityIconManager00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"Mercator",0);
  tolua_beginmodule(tolua_S,"Mercator");
   tolua_cclass(tolua_S,"Area","Mercator::Area","",NULL);
   tolua_beginmodule(tolua_S,"Area");
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_module(tolua_S,"Terrain",0);
   tolua_beginmodule(tolua_S,"Terrain");
    #ifdef __cplusplus
    tolua_cclass(tolua_S,"TerrainArea","EmberOgre::Terrain::TerrainArea","",tolua_collect_EmberOgre__Terrain__TerrainArea);
    #else
    tolua_cclass(tolua_S,"TerrainArea","EmberOgre::Terrain::TerrainArea","",NULL);
    #endif
    tolua_beginmodule(tolua_S,"TerrainArea");
     tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_Terrain_TerrainArea_new00);
     tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_Terrain_TerrainArea_new00_local);
     tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_Terrain_TerrainArea_new00_local);
     tolua_function(tolua_S,"init",tolua_EmberOgre_EmberOgre_Terrain_TerrainArea_init00);
     tolua_function(tolua_S,"getArea",tolua_EmberOgre_EmberOgre_Terrain_TerrainArea_getArea00);
     tolua_function(tolua_S,"setArea",tolua_EmberOgre_EmberOgre_Terrain_TerrainArea_setArea00);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_module(tolua_S,"Terrain",0);
   tolua_beginmodule(tolua_S,"Terrain");
    tolua_cclass(tolua_S,"BasePointUserObject","EmberOgre::Terrain::BasePointUserObject","",NULL);
    tolua_beginmodule(tolua_S,"BasePointUserObject");
     tolua_function(tolua_S,"getBasePoint",tolua_EmberOgre_EmberOgre_Terrain_BasePointUserObject_getBasePoint00);
     tolua_function(tolua_S,"getBasePointMarkerNode",tolua_EmberOgre_EmberOgre_Terrain_BasePointUserObject_getBasePointMarkerNode00);
     tolua_function(tolua_S,"getPosition",tolua_EmberOgre_EmberOgre_Terrain_BasePointUserObject_getPosition00);
     tolua_function(tolua_S,"getHeight",tolua_EmberOgre_EmberOgre_Terrain_BasePointUserObject_getHeight00);
     tolua_function(tolua_S,"translate",tolua_EmberOgre_EmberOgre_Terrain_BasePointUserObject_translate00);
     tolua_variable(tolua_S,"EventUpdatedPosition",tolua_get_EmberOgre__Terrain__BasePointUserObject_EventUpdatedPosition,tolua_set_EmberOgre__Terrain__BasePointUserObject_EventUpdatedPosition);
    tolua_endmodule(tolua_S);
    tolua_cclass(tolua_S,"TerrainEditAction","EmberOgre::Terrain::TerrainEditAction","",NULL);
    tolua_beginmodule(tolua_S,"TerrainEditAction");
     tolua_function(tolua_S,"getMovements",tolua_EmberOgre_EmberOgre_Terrain_TerrainEditAction_getMovements00);
     tolua_function(tolua_S,"getMovements",tolua_EmberOgre_EmberOgre_Terrain_TerrainEditAction_getMovements01);
    tolua_endmodule(tolua_S);
    tolua_cclass(tolua_S,"TerrainEditBasePointMovement","EmberOgre::Terrain::TerrainEditBasePointMovement","",NULL);
    tolua_beginmodule(tolua_S,"TerrainEditBasePointMovement");
     tolua_function(tolua_S,"getVerticalMovement",tolua_EmberOgre_EmberOgre_Terrain_TerrainEditBasePointMovement_getVerticalMovement00);
     tolua_function(tolua_S,"getPosition",tolua_EmberOgre_EmberOgre_Terrain_TerrainEditBasePointMovement_getPosition00);
    tolua_endmodule(tolua_S);
    #ifdef __cplusplus
    tolua_cclass(tolua_S,"TerrainEditor","EmberOgre::Terrain::TerrainEditor","",tolua_collect_EmberOgre__Terrain__TerrainEditor);
    #else
    tolua_cclass(tolua_S,"TerrainEditor","EmberOgre::Terrain::TerrainEditor","",NULL);
    #endif
    tolua_beginmodule(tolua_S,"TerrainEditor");
     tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_new00);
     tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_new00_local);
     tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_new00_local);
     tolua_function(tolua_S,"delete",tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_delete00);
     tolua_function(tolua_S,"showOverlay",tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_showOverlay00);
     tolua_function(tolua_S,"hideOverlay",tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_hideOverlay00);
     tolua_function(tolua_S,"createOverlay",tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_createOverlay00);
     tolua_function(tolua_S,"isOverlayShown",tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_isOverlayShown00);
     tolua_function(tolua_S,"commitAction",tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_commitAction00);
     tolua_variable(tolua_S,"EventPickedBasePoint",tolua_get_EmberOgre__Terrain__TerrainEditor_EventPickedBasePoint,tolua_set_EmberOgre__Terrain__TerrainEditor_EventPickedBasePoint);
     tolua_variable(tolua_S,"EventActionCreated",tolua_get_EmberOgre__Terrain__TerrainEditor_EventActionCreated,tolua_set_EmberOgre__Terrain__TerrainEditor_EventActionCreated);
     tolua_variable(tolua_S,"EventSelectedBasePointUpdatedPosition",tolua_get_EmberOgre__Terrain__TerrainEditor_EventSelectedBasePointUpdatedPosition,tolua_set_EmberOgre__Terrain__TerrainEditor_EventSelectedBasePointUpdatedPosition);
     tolua_function(tolua_S,"getCurrentBasePointUserObject",tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_getCurrentBasePointUserObject00);
     tolua_function(tolua_S,"sendChangesToServer",tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_sendChangesToServer00);
     tolua_function(tolua_S,"createAction",tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_createAction00);
     tolua_function(tolua_S,"undoLastAction",tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_undoLastAction00);
     tolua_function(tolua_S,"redoAction",tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_redoAction00);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_cclass(tolua_S,"MotionManager","EmberOgre::MotionManager","",NULL);
   tolua_beginmodule(tolua_S,"MotionManager");
    tolua_cclass(tolua_S,"MotionManagerInfo","EmberOgre::MotionManager::MotionManagerInfo","",NULL);
    tolua_beginmodule(tolua_S,"MotionManagerInfo");
     tolua_variable(tolua_S,"AnimatedEntities",tolua_get_EmberOgre__MotionManager__MotionManagerInfo_AnimatedEntities,tolua_set_EmberOgre__MotionManager__MotionManagerInfo_AnimatedEntities);
     tolua_variable(tolua_S,"MovingEntities",tolua_get_EmberOgre__MotionManager__MotionManagerInfo_MovingEntities,tolua_set_EmberOgre__MotionManager__MotionManagerInfo_MovingEntities);
     tolua_variable(tolua_S,"Animations",tolua_get_EmberOgre__MotionManager__MotionManagerInfo_Animations,tolua_set_EmberOgre__MotionManager__MotionManagerInfo_Animations);
    tolua_endmodule(tolua_S);
    tolua_function(tolua_S,"getSingleton",tolua_EmberOgre_EmberOgre_MotionManager_getSingleton00);
    tolua_function(tolua_S,"addEntity",tolua_EmberOgre_EmberOgre_MotionManager_addEntity00);
    tolua_function(tolua_S,"removeEntity",tolua_EmberOgre_EmberOgre_MotionManager_removeEntity00);
    tolua_function(tolua_S,"addAnimation",tolua_EmberOgre_EmberOgre_MotionManager_addAnimation00);
    tolua_function(tolua_S,"removeAnimation",tolua_EmberOgre_EmberOgre_MotionManager_removeAnimation00);
    tolua_function(tolua_S,"pauseAnimation",tolua_EmberOgre_EmberOgre_MotionManager_pauseAnimation00);
    tolua_function(tolua_S,"unpauseAnimation",tolua_EmberOgre_EmberOgre_MotionManager_unpauseAnimation00);
    tolua_function(tolua_S,"addAnimatedEntity",tolua_EmberOgre_EmberOgre_MotionManager_addAnimatedEntity00);
    tolua_function(tolua_S,"removeAnimatedEntity",tolua_EmberOgre_EmberOgre_MotionManager_removeAnimatedEntity00);
    tolua_function(tolua_S,"getInfo",tolua_EmberOgre_EmberOgre_MotionManager_getInfo00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_module(tolua_S,"Gui",0);
   tolua_beginmodule(tolua_S,"Gui");
    tolua_cclass(tolua_S,"Widget","EmberOgre::Gui::Widget","",NULL);
    tolua_beginmodule(tolua_S,"Widget");
     tolua_function(tolua_S,"show",tolua_EmberOgre_EmberOgre_Gui_Widget_show00);
     tolua_function(tolua_S,"hide",tolua_EmberOgre_EmberOgre_Gui_Widget_hide00);
     tolua_function(tolua_S,"getWindow",tolua_EmberOgre_EmberOgre_Gui_Widget_getWindow00);
     tolua_function(tolua_S,"enableCloseButton",tolua_EmberOgre_EmberOgre_Gui_Widget_enableCloseButton00);
     tolua_function(tolua_S,"registerConsoleVisibilityToggleCommand",tolua_EmberOgre_EmberOgre_Gui_Widget_registerConsoleVisibilityToggleCommand00);
     tolua_function(tolua_S,"getMainSheet",tolua_EmberOgre_EmberOgre_Gui_Widget_getMainSheet00);
     tolua_function(tolua_S,"getMainWindow",tolua_EmberOgre_EmberOgre_Gui_Widget_getMainWindow00);
     tolua_function(tolua_S,"createWindow",tolua_EmberOgre_EmberOgre_Gui_Widget_createWindow00);
     tolua_function(tolua_S,"createWindow",tolua_EmberOgre_EmberOgre_Gui_Widget_createWindow01);
     tolua_function(tolua_S,"loadMainSheet",tolua_EmberOgre_EmberOgre_Gui_Widget_loadMainSheet00);
     tolua_function(tolua_S,"getPrefix",tolua_EmberOgre_EmberOgre_Gui_Widget_getPrefix00);
     tolua_function(tolua_S,"getIsActiveWindowOpaque",tolua_EmberOgre_EmberOgre_Gui_Widget_getIsActiveWindowOpaque00);
     tolua_function(tolua_S,"setIsActiveWindowOpaque",tolua_EmberOgre_EmberOgre_Gui_Widget_setIsActiveWindowOpaque00);
     tolua_function(tolua_S,"getDefaultScheme",tolua_EmberOgre_EmberOgre_Gui_Widget_getDefaultScheme00);
     tolua_function(tolua_S,"addTabbableWindow",tolua_EmberOgre_EmberOgre_Gui_Widget_addTabbableWindow00);
     tolua_function(tolua_S,"closeTabGroup",tolua_EmberOgre_EmberOgre_Gui_Widget_closeTabGroup00);
     tolua_variable(tolua_S,"EventFrameStarted",tolua_get_EmberOgre__Gui__Widget_EventFrameStarted,tolua_set_EmberOgre__Gui__Widget_EventFrameStarted);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_module(tolua_S,"Gui",0);
   tolua_beginmodule(tolua_S,"Gui");
    #ifdef __cplusplus
    tolua_cclass(tolua_S,"IconBase","EmberOgre::Gui::IconBase","",tolua_collect_EmberOgre__Gui__IconBase);
    #else
    tolua_cclass(tolua_S,"IconBase","EmberOgre::Gui::IconBase","",NULL);
    #endif
    tolua_beginmodule(tolua_S,"IconBase");
     tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_Gui_IconBase_new00);
     tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_Gui_IconBase_new00_local);
     tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_Gui_IconBase_new00_local);
     tolua_function(tolua_S,"delete",tolua_EmberOgre_EmberOgre_Gui_IconBase_delete00);
     tolua_function(tolua_S,"getContainer",tolua_EmberOgre_EmberOgre_Gui_IconBase_getContainer00);
     tolua_function(tolua_S,"getButton",tolua_EmberOgre_EmberOgre_Gui_IconBase_getButton00);
     tolua_function(tolua_S,"setForeground",tolua_EmberOgre_EmberOgre_Gui_IconBase_setForeground00);
     tolua_function(tolua_S,"loadImageFromImageset",tolua_EmberOgre_EmberOgre_Gui_IconBase_loadImageFromImageset00);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_module(tolua_S,"Gui",0);
   tolua_beginmodule(tolua_S,"Gui");
    #ifdef __cplusplus
    tolua_cclass(tolua_S,"IconBar","EmberOgre::Gui::IconBar","",tolua_collect_EmberOgre__Gui__IconBar);
    #else
    tolua_cclass(tolua_S,"IconBar","EmberOgre::Gui::IconBar","",NULL);
    #endif
    tolua_beginmodule(tolua_S,"IconBar");
     tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_Gui_IconBar_new00);
     tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_Gui_IconBar_new00_local);
     tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_Gui_IconBar_new00_local);
     tolua_function(tolua_S,"delete",tolua_EmberOgre_EmberOgre_Gui_IconBar_delete00);
     tolua_function(tolua_S,"addIcon",tolua_EmberOgre_EmberOgre_Gui_IconBar_addIcon00);
     tolua_function(tolua_S,"removeIcon",tolua_EmberOgre_EmberOgre_Gui_IconBar_removeIcon00);
     tolua_function(tolua_S,"getWindow",tolua_EmberOgre_EmberOgre_Gui_IconBar_getWindow00);
     tolua_function(tolua_S,"setIconPadding",tolua_EmberOgre_EmberOgre_Gui_IconBar_setIconPadding00);
     tolua_function(tolua_S,"getAbsoluteHeight",tolua_EmberOgre_EmberOgre_Gui_IconBar_getAbsoluteHeight00);
     tolua_function(tolua_S,"getAbsoluteWidth",tolua_EmberOgre_EmberOgre_Gui_IconBar_getAbsoluteWidth00);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_module(tolua_S,"Gui",0);
   tolua_beginmodule(tolua_S,"Gui");
    #ifdef __cplusplus
    tolua_cclass(tolua_S,"EntityEditor","EmberOgre::Gui::EntityEditor","",tolua_collect_EmberOgre__Gui__EntityEditor);
    #else
    tolua_cclass(tolua_S,"EntityEditor","EmberOgre::Gui::EntityEditor","",NULL);
    #endif
    tolua_beginmodule(tolua_S,"EntityEditor");
     tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_Gui_EntityEditor_new00);
     tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_Gui_EntityEditor_new00_local);
     tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_Gui_EntityEditor_new00_local);
     tolua_function(tolua_S,"delete",tolua_EmberOgre_EmberOgre_Gui_EntityEditor_delete00);
     tolua_function(tolua_S,"submitChanges",tolua_EmberOgre_EmberOgre_Gui_EntityEditor_submitChanges00);
     tolua_function(tolua_S,"createMapElement",tolua_EmberOgre_EmberOgre_Gui_EntityEditor_createMapElement00);
     tolua_function(tolua_S,"createListElement",tolua_EmberOgre_EmberOgre_Gui_EntityEditor_createListElement00);
     tolua_function(tolua_S,"createStringElement",tolua_EmberOgre_EmberOgre_Gui_EntityEditor_createStringElement00);
     tolua_function(tolua_S,"createIntElement",tolua_EmberOgre_EmberOgre_Gui_EntityEditor_createIntElement00);
     tolua_function(tolua_S,"createFloatElement",tolua_EmberOgre_EmberOgre_Gui_EntityEditor_createFloatElement00);
     tolua_function(tolua_S,"createPosition2dElement",tolua_EmberOgre_EmberOgre_Gui_EntityEditor_createPosition2dElement00);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_module(tolua_S,"Gui",0);
   tolua_beginmodule(tolua_S,"Gui");
    #ifdef __cplusplus
    tolua_cclass(tolua_S,"StackableContainer","EmberOgre::Gui::StackableContainer","",tolua_collect_EmberOgre__Gui__StackableContainer);
    #else
    tolua_cclass(tolua_S,"StackableContainer","EmberOgre::Gui::StackableContainer","",NULL);
    #endif
    tolua_beginmodule(tolua_S,"StackableContainer");
     tolua_constant(tolua_S,"Horizontal",EmberOgre::Gui::StackableContainer::Horizontal);
     tolua_constant(tolua_S,"Vertical",EmberOgre::Gui::StackableContainer::Vertical);
     tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_Gui_StackableContainer_new00);
     tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_Gui_StackableContainer_new00_local);
     tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_Gui_StackableContainer_new00_local);
     tolua_function(tolua_S,"delete",tolua_EmberOgre_EmberOgre_Gui_StackableContainer_delete00);
     tolua_function(tolua_S,"setInnerContainerWindow",tolua_EmberOgre_EmberOgre_Gui_StackableContainer_setInnerContainerWindow00);
     tolua_function(tolua_S,"getWindow",tolua_EmberOgre_EmberOgre_Gui_StackableContainer_getWindow00);
     tolua_function(tolua_S,"setPadding",tolua_EmberOgre_EmberOgre_Gui_StackableContainer_setPadding00);
     tolua_function(tolua_S,"getPadding",tolua_EmberOgre_EmberOgre_Gui_StackableContainer_getPadding00);
     tolua_function(tolua_S,"getAbsoluteHeight",tolua_EmberOgre_EmberOgre_Gui_StackableContainer_getAbsoluteHeight00);
     tolua_function(tolua_S,"getAbsoluteWidth",tolua_EmberOgre_EmberOgre_Gui_StackableContainer_getAbsoluteWidth00);
     tolua_function(tolua_S,"setFlowDirection",tolua_EmberOgre_EmberOgre_Gui_StackableContainer_setFlowDirection00);
     tolua_function(tolua_S,"getFlowDirection",tolua_EmberOgre_EmberOgre_Gui_StackableContainer_getFlowDirection00);
     tolua_function(tolua_S,"repositionWindows",tolua_EmberOgre_EmberOgre_Gui_StackableContainer_repositionWindows00);
     tolua_function(tolua_S,"disconnect",tolua_EmberOgre_EmberOgre_Gui_StackableContainer_disconnect00);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_module(tolua_S,"Gui",0);
   tolua_beginmodule(tolua_S,"Gui");
    #ifdef __cplusplus
    tolua_cclass(tolua_S,"ConsoleAdapter","EmberOgre::Gui::ConsoleAdapter","",tolua_collect_EmberOgre__Gui__ConsoleAdapter);
    #else
    tolua_cclass(tolua_S,"ConsoleAdapter","EmberOgre::Gui::ConsoleAdapter","",NULL);
    #endif
    tolua_beginmodule(tolua_S,"ConsoleAdapter");
     tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_Gui_ConsoleAdapter_new00);
     tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_Gui_ConsoleAdapter_new00_local);
     tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_Gui_ConsoleAdapter_new00_local);
     tolua_function(tolua_S,"delete",tolua_EmberOgre_EmberOgre_Gui_ConsoleAdapter_delete00);
     tolua_variable(tolua_S,"EventCommandExecuted",tolua_get_EmberOgre__Gui__ConsoleAdapter_EventCommandExecuted,tolua_set_EmberOgre__Gui__ConsoleAdapter_EventCommandExecuted);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_module(tolua_S,"Gui",0);
   tolua_beginmodule(tolua_S,"Gui");
    tolua_cclass(tolua_S,"ColouredListItem","EmberOgre::Gui::ColouredListItem","CEGUI::ListboxTextItem",NULL);
    tolua_beginmodule(tolua_S,"ColouredListItem");
     tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_Gui_ColouredListItem_new00);
     tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_Gui_ColouredListItem_new01);
     tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_Gui_ColouredListItem_new02);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_module(tolua_S,"Gui",0);
   tolua_beginmodule(tolua_S,"Gui");
    #ifdef __cplusplus
    tolua_cclass(tolua_S,"TexturePair","EmberOgre::Gui::TexturePair","",tolua_collect_EmberOgre__Gui__TexturePair);
    #else
    tolua_cclass(tolua_S,"TexturePair","EmberOgre::Gui::TexturePair","",NULL);
    #endif
    tolua_beginmodule(tolua_S,"TexturePair");
     tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_Gui_TexturePair_new00);
     tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_Gui_TexturePair_new00_local);
     tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_Gui_TexturePair_new00_local);
     tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_Gui_TexturePair_new01);
     tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_Gui_TexturePair_new01_local);
     tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_Gui_TexturePair_new01_local);
     tolua_function(tolua_S,"getOgreTexture",tolua_EmberOgre_EmberOgre_Gui_TexturePair_getOgreTexture00);
     tolua_function(tolua_S,"getTextureImage",tolua_EmberOgre_EmberOgre_Gui_TexturePair_getTextureImage00);
     tolua_function(tolua_S,"getTextureImageset",tolua_EmberOgre_EmberOgre_Gui_TexturePair_getTextureImageset00);
     tolua_function(tolua_S,"hasData",tolua_EmberOgre_EmberOgre_Gui_TexturePair_hasData00);
    tolua_endmodule(tolua_S);
    #ifdef __cplusplus
    tolua_cclass(tolua_S,"AssetsManager","EmberOgre::Gui::AssetsManager","",tolua_collect_EmberOgre__Gui__AssetsManager);
    #else
    tolua_cclass(tolua_S,"AssetsManager","EmberOgre::Gui::AssetsManager","",NULL);
    #endif
    tolua_beginmodule(tolua_S,"AssetsManager");
     tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_Gui_AssetsManager_new00);
     tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_Gui_AssetsManager_new00_local);
     tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_Gui_AssetsManager_new00_local);
     tolua_function(tolua_S,"delete",tolua_EmberOgre_EmberOgre_Gui_AssetsManager_delete00);
     tolua_function(tolua_S,"showTexture",tolua_EmberOgre_EmberOgre_Gui_AssetsManager_showTexture00);
     tolua_function(tolua_S,"createTextureImage",tolua_EmberOgre_EmberOgre_Gui_AssetsManager_createTextureImage00);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_module(tolua_S,"Gui",0);
   tolua_beginmodule(tolua_S,"Gui");
    tolua_cclass(tolua_S,"ICompassImpl","EmberOgre::Gui::ICompassImpl","",NULL);
    tolua_beginmodule(tolua_S,"ICompassImpl");
    tolua_endmodule(tolua_S);
    #ifdef __cplusplus
    tolua_cclass(tolua_S,"Compass","EmberOgre::Gui::Compass","",tolua_collect_EmberOgre__Gui__Compass);
    #else
    tolua_cclass(tolua_S,"Compass","EmberOgre::Gui::Compass","",NULL);
    #endif
    tolua_beginmodule(tolua_S,"Compass");
     tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_Gui_Compass_new00);
     tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_Gui_Compass_new00_local);
     tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_Gui_Compass_new00_local);
     tolua_function(tolua_S,"delete",tolua_EmberOgre_EmberOgre_Gui_Compass_delete00);
     tolua_function(tolua_S,"getMap",tolua_EmberOgre_EmberOgre_Gui_Compass_getMap00);
     tolua_function(tolua_S,"reposition",tolua_EmberOgre_EmberOgre_Gui_Compass_reposition00);
     tolua_function(tolua_S,"rotate",tolua_EmberOgre_EmberOgre_Gui_Compass_rotate00);
     tolua_function(tolua_S,"rotate",tolua_EmberOgre_EmberOgre_Gui_Compass_rotate01);
    tolua_endmodule(tolua_S);
    #ifdef __cplusplus
    tolua_cclass(tolua_S,"CEGUICompassImpl","EmberOgre::Gui::CEGUICompassImpl","EmberOgre::Gui::ICompassImpl",tolua_collect_EmberOgre__Gui__CEGUICompassImpl);
    #else
    tolua_cclass(tolua_S,"CEGUICompassImpl","EmberOgre::Gui::CEGUICompassImpl","EmberOgre::Gui::ICompassImpl",NULL);
    #endif
    tolua_beginmodule(tolua_S,"CEGUICompassImpl");
     tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_Gui_CEGUICompassImpl_new00);
     tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_Gui_CEGUICompassImpl_new00_local);
     tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_Gui_CEGUICompassImpl_new00_local);
     tolua_function(tolua_S,"delete",tolua_EmberOgre_EmberOgre_Gui_CEGUICompassImpl_delete00);
     tolua_function(tolua_S,"getViewImage",tolua_EmberOgre_EmberOgre_Gui_CEGUICompassImpl_getViewImage00);
     tolua_function(tolua_S,"reposition",tolua_EmberOgre_EmberOgre_Gui_CEGUICompassImpl_reposition00);
     tolua_function(tolua_S,"rotate",tolua_EmberOgre_EmberOgre_Gui_CEGUICompassImpl_rotate00);
    tolua_endmodule(tolua_S);
    #ifdef __cplusplus
    tolua_cclass(tolua_S,"OverlayCompassImpl","EmberOgre::Gui::OverlayCompassImpl","EmberOgre::Gui::ICompassImpl",tolua_collect_EmberOgre__Gui__OverlayCompassImpl);
    #else
    tolua_cclass(tolua_S,"OverlayCompassImpl","EmberOgre::Gui::OverlayCompassImpl","EmberOgre::Gui::ICompassImpl",NULL);
    #endif
    tolua_beginmodule(tolua_S,"OverlayCompassImpl");
     tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_Gui_OverlayCompassImpl_new00);
     tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_Gui_OverlayCompassImpl_new00_local);
     tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_Gui_OverlayCompassImpl_new00_local);
     tolua_function(tolua_S,"delete",tolua_EmberOgre_EmberOgre_Gui_OverlayCompassImpl_delete00);
     tolua_function(tolua_S,"reposition",tolua_EmberOgre_EmberOgre_Gui_OverlayCompassImpl_reposition00);
     tolua_function(tolua_S,"rotate",tolua_EmberOgre_EmberOgre_Gui_OverlayCompassImpl_rotate00);
    tolua_endmodule(tolua_S);
    #ifdef __cplusplus
    tolua_cclass(tolua_S,"CompassCameraAnchor","EmberOgre::Gui::CompassCameraAnchor","",tolua_collect_EmberOgre__Gui__CompassCameraAnchor);
    #else
    tolua_cclass(tolua_S,"CompassCameraAnchor","EmberOgre::Gui::CompassCameraAnchor","",NULL);
    #endif
    tolua_beginmodule(tolua_S,"CompassCameraAnchor");
     tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_Gui_CompassCameraAnchor_new00);
     tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_Gui_CompassCameraAnchor_new00_local);
     tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_Gui_CompassCameraAnchor_new00_local);
     tolua_function(tolua_S,"delete",tolua_EmberOgre_EmberOgre_Gui_CompassCameraAnchor_delete00);
    tolua_endmodule(tolua_S);
    #ifdef __cplusplus
    tolua_cclass(tolua_S,"CompassSceneNodeAnchor","EmberOgre::Gui::CompassSceneNodeAnchor","",tolua_collect_EmberOgre__Gui__CompassSceneNodeAnchor);
    #else
    tolua_cclass(tolua_S,"CompassSceneNodeAnchor","EmberOgre::Gui::CompassSceneNodeAnchor","",NULL);
    #endif
    tolua_beginmodule(tolua_S,"CompassSceneNodeAnchor");
     tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_Gui_CompassSceneNodeAnchor_new00);
     tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_Gui_CompassSceneNodeAnchor_new00_local);
     tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_Gui_CompassSceneNodeAnchor_new00_local);
     tolua_function(tolua_S,"delete",tolua_EmberOgre_EmberOgre_Gui_CompassSceneNodeAnchor_delete00);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_module(tolua_S,"Gui",0);
   tolua_beginmodule(tolua_S,"Gui");
    tolua_module(tolua_S,"Icons",0);
    tolua_beginmodule(tolua_S,"Icons");
     tolua_cclass(tolua_S,"Icon","EmberOgre::Gui::Icons::Icon","",NULL);
     tolua_beginmodule(tolua_S,"Icon");
      tolua_function(tolua_S,"getImage",tolua_EmberOgre_EmberOgre_Gui_Icons_Icon_getImage00);
      tolua_function(tolua_S,"getImage",tolua_EmberOgre_EmberOgre_Gui_Icons_Icon_getImage01);
     tolua_endmodule(tolua_S);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_module(tolua_S,"Gui",0);
   tolua_beginmodule(tolua_S,"Gui");
    tolua_module(tolua_S,"Icons",0);
    tolua_beginmodule(tolua_S,"Icons");
     #ifdef __cplusplus
     tolua_cclass(tolua_S,"IconManager","EmberOgre::Gui::Icons::IconManager","",tolua_collect_EmberOgre__Gui__Icons__IconManager);
     #else
     tolua_cclass(tolua_S,"IconManager","EmberOgre::Gui::Icons::IconManager","",NULL);
     #endif
     tolua_beginmodule(tolua_S,"IconManager");
      tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_Gui_Icons_IconManager_new00);
      tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_Gui_Icons_IconManager_new00_local);
      tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_Gui_Icons_IconManager_new00_local);
      tolua_function(tolua_S,"delete",tolua_EmberOgre_EmberOgre_Gui_Icons_IconManager_delete00);
      tolua_function(tolua_S,"getIcon",tolua_EmberOgre_EmberOgre_Gui_Icons_IconManager_getIcon00);
      tolua_function(tolua_S,"getIcon",tolua_EmberOgre_EmberOgre_Gui_Icons_IconManager_getIcon01);
     tolua_endmodule(tolua_S);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_module(tolua_S,"Gui",0);
   tolua_beginmodule(tolua_S,"Gui");
    tolua_cclass(tolua_S,"EntityIconSlot","EmberOgre::Gui::EntityIconSlot","EmberOgre::Gui::EntityIconDragDropTarget",NULL);
    tolua_beginmodule(tolua_S,"EntityIconSlot");
     tolua_function(tolua_S,"addEntityIcon",tolua_EmberOgre_EmberOgre_Gui_EntityIconSlot_addEntityIcon00);
     tolua_function(tolua_S,"removeEntityIcon",tolua_EmberOgre_EmberOgre_Gui_EntityIconSlot_removeEntityIcon00);
     tolua_function(tolua_S,"getEntityIcon",tolua_EmberOgre_EmberOgre_Gui_EntityIconSlot_getEntityIcon00);
     tolua_function(tolua_S,"getWindow",tolua_EmberOgre_EmberOgre_Gui_EntityIconSlot_getWindow00);
     tolua_function(tolua_S,"notifyIconDraggedOff",tolua_EmberOgre_EmberOgre_Gui_EntityIconSlot_notifyIconDraggedOff00);
     tolua_variable(tolua_S,"EventIconDraggedOff",tolua_get_EmberOgre__Gui__EntityIconSlot_EventIconDraggedOff,tolua_set_EmberOgre__Gui__EntityIconSlot_EventIconDraggedOff);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_module(tolua_S,"Gui",0);
   tolua_beginmodule(tolua_S,"Gui");
    tolua_cclass(tolua_S,"EntityIcon","EmberOgre::Gui::EntityIcon","EmberOgre::Gui::EntityIconDragDropTarget",NULL);
    tolua_beginmodule(tolua_S,"EntityIcon");
     tolua_function(tolua_S,"getImage",tolua_EmberOgre_EmberOgre_Gui_EntityIcon_getImage00);
     tolua_function(tolua_S,"getDragContainer",tolua_EmberOgre_EmberOgre_Gui_EntityIcon_getDragContainer00);
     tolua_function(tolua_S,"getIcon",tolua_EmberOgre_EmberOgre_Gui_EntityIcon_getIcon00);
     tolua_function(tolua_S,"setSlot",tolua_EmberOgre_EmberOgre_Gui_EntityIcon_setSlot00);
     tolua_function(tolua_S,"getSlot",tolua_EmberOgre_EmberOgre_Gui_EntityIcon_getSlot00);
     tolua_function(tolua_S,"setTooltipText",tolua_EmberOgre_EmberOgre_Gui_EntityIcon_setTooltipText00);
     tolua_function(tolua_S,"getEntity",tolua_EmberOgre_EmberOgre_Gui_EntityIcon_getEntity00);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_module(tolua_S,"Gui",0);
   tolua_beginmodule(tolua_S,"Gui");
    tolua_cclass(tolua_S,"EntityIconManager","EmberOgre::Gui::EntityIconManager","",NULL);
    tolua_beginmodule(tolua_S,"EntityIconManager");
     tolua_function(tolua_S,"createSlot",tolua_EmberOgre_EmberOgre_Gui_EntityIconManager_createSlot00);
     tolua_function(tolua_S,"destroySlot",tolua_EmberOgre_EmberOgre_Gui_EntityIconManager_destroySlot00);
     tolua_function(tolua_S,"createIcon",tolua_EmberOgre_EmberOgre_Gui_EntityIconManager_createIcon00);
     tolua_function(tolua_S,"destroyIcon",tolua_EmberOgre_EmberOgre_Gui_EntityIconManager_destroyIcon00);
     tolua_variable(tolua_S,"EventIconDragStart",tolua_get_EmberOgre__Gui__EntityIconManager_EventIconDragStart,tolua_set_EmberOgre__Gui__EntityIconManager_EventIconDragStart);
     tolua_variable(tolua_S,"EventIconDragStop",tolua_get_EmberOgre__Gui__EntityIconManager_EventIconDragStop,tolua_set_EmberOgre__Gui__EntityIconManager_EventIconDragStop);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_module(tolua_S,"Gui",0);
   tolua_beginmodule(tolua_S,"Gui");
    tolua_cclass(tolua_S,"EntityIconDragDropTarget","EmberOgre::Gui::EntityIconDragDropTarget","",NULL);
    tolua_beginmodule(tolua_S,"EntityIconDragDropTarget");
     tolua_variable(tolua_S,"EventIconEntered",tolua_get_EmberOgre__Gui__EntityIconDragDropTarget_EventIconEntered,tolua_set_EmberOgre__Gui__EntityIconDragDropTarget_EventIconEntered);
     tolua_variable(tolua_S,"EventIconLeaves",tolua_get_EmberOgre__Gui__EntityIconDragDropTarget_EventIconLeaves,tolua_set_EmberOgre__Gui__EntityIconDragDropTarget_EventIconLeaves);
     tolua_variable(tolua_S,"EventIconDropped",tolua_get_EmberOgre__Gui__EntityIconDragDropTarget_EventIconDropped,tolua_set_EmberOgre__Gui__EntityIconDragDropTarget_EventIconDropped);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_module(tolua_S,"Model",0);
   tolua_beginmodule(tolua_S,"Model");
    #ifdef __cplusplus
    tolua_cclass(tolua_S,"Model","EmberOgre::Model::Model","",tolua_collect_EmberOgre__Model__Model);
    #else
    tolua_cclass(tolua_S,"Model","EmberOgre::Model::Model","",NULL);
    #endif
    tolua_beginmodule(tolua_S,"Model");
     tolua_function(tolua_S,"createModel",tolua_EmberOgre_EmberOgre_Model_Model_createModel00);
     tolua_function(tolua_S,"createModel",tolua_EmberOgre_EmberOgre_Model_Model_createModel01);
     tolua_function(tolua_S,"reload",tolua_EmberOgre_EmberOgre_Model_Model_reload00);
     tolua_function(tolua_S,"delete",tolua_EmberOgre_EmberOgre_Model_Model_delete00);
     tolua_function(tolua_S,"addSubmodel",tolua_EmberOgre_EmberOgre_Model_Model_addSubmodel00);
     tolua_function(tolua_S,"removeSubmodel",tolua_EmberOgre_EmberOgre_Model_Model_removeSubmodel00);
     tolua_function(tolua_S,"getAction",tolua_EmberOgre_EmberOgre_Model_Model_getAction00);
     tolua_function(tolua_S,"showPart",tolua_EmberOgre_EmberOgre_Model_Model_showPart00);
     tolua_function(tolua_S,"hidePart",tolua_EmberOgre_EmberOgre_Model_Model_hidePart00);
     tolua_function(tolua_S,"setVisible",tolua_EmberOgre_EmberOgre_Model_Model_setVisible00);
     tolua_function(tolua_S,"getScale",tolua_EmberOgre_EmberOgre_Model_Model_getScale00);
     tolua_function(tolua_S,"getRotation",tolua_EmberOgre_EmberOgre_Model_Model_getRotation00);
     tolua_function(tolua_S,"getUseScaleOf",tolua_EmberOgre_EmberOgre_Model_Model_getUseScaleOf00);
     tolua_function(tolua_S,"getName",tolua_EmberOgre_EmberOgre_Model_Model_getName00);
     tolua_function(tolua_S,"getDefinition",tolua_EmberOgre_EmberOgre_Model_Model_getDefinition00);
     tolua_function(tolua_S,"hasAttachPoint",tolua_EmberOgre_EmberOgre_Model_Model_hasAttachPoint00);
     tolua_function(tolua_S,"hasParticles",tolua_EmberOgre_EmberOgre_Model_Model_hasParticles00);
     tolua_function(tolua_S,"getSubModel",tolua_EmberOgre_EmberOgre_Model_Model_getSubModel00);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_module(tolua_S,"Model",0);
   tolua_beginmodule(tolua_S,"Model");
    tolua_cclass(tolua_S,"SubModel","EmberOgre::Model::SubModel","",NULL);
    tolua_beginmodule(tolua_S,"SubModel");
     tolua_function(tolua_S,"getEntity",tolua_EmberOgre_EmberOgre_Model_SubModel_getEntity00);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_module(tolua_S,"Model",0);
   tolua_beginmodule(tolua_S,"Model");
    tolua_cclass(tolua_S,"ViewDefinition","EmberOgre::Model::ViewDefinition","",NULL);
    tolua_beginmodule(tolua_S,"ViewDefinition");
     tolua_variable(tolua_S,"Name",tolua_get_EmberOgre__Model__ViewDefinition_Name,tolua_set_EmberOgre__Model__ViewDefinition_Name);
     tolua_variable(tolua_S,"Rotation",tolua_get_EmberOgre__Model__ViewDefinition_Rotation,tolua_set_EmberOgre__Model__ViewDefinition_Rotation);
     tolua_variable(tolua_S,"Distance",tolua_get_EmberOgre__Model__ViewDefinition_Distance,tolua_set_EmberOgre__Model__ViewDefinition_Distance);
    tolua_endmodule(tolua_S);
    tolua_cclass(tolua_S,"AttachPointDefinition","EmberOgre::Model::AttachPointDefinition","",NULL);
    tolua_beginmodule(tolua_S,"AttachPointDefinition");
     tolua_variable(tolua_S,"Name",tolua_get_EmberOgre__Model__AttachPointDefinition_Name,tolua_set_EmberOgre__Model__AttachPointDefinition_Name);
     tolua_variable(tolua_S,"BoneName",tolua_get_EmberOgre__Model__AttachPointDefinition_BoneName,tolua_set_EmberOgre__Model__AttachPointDefinition_BoneName);
     tolua_variable(tolua_S,"Rotation",tolua_get_EmberOgre__Model__AttachPointDefinition_Rotation,tolua_set_EmberOgre__Model__AttachPointDefinition_Rotation);
    tolua_endmodule(tolua_S);
    tolua_cclass(tolua_S,"SubEntityDefinition","EmberOgre::Model::SubEntityDefinition","",NULL);
    tolua_beginmodule(tolua_S,"SubEntityDefinition");
     tolua_function(tolua_S,"getSubEntityName",tolua_EmberOgre_EmberOgre_Model_SubEntityDefinition_getSubEntityName00);
     tolua_function(tolua_S,"getSubEntityIndex",tolua_EmberOgre_EmberOgre_Model_SubEntityDefinition_getSubEntityIndex00);
     tolua_function(tolua_S,"getMaterialName",tolua_EmberOgre_EmberOgre_Model_SubEntityDefinition_getMaterialName00);
     tolua_function(tolua_S,"setMaterialName",tolua_EmberOgre_EmberOgre_Model_SubEntityDefinition_setMaterialName00);
     tolua_function(tolua_S,"getPartDefinition",tolua_EmberOgre_EmberOgre_Model_SubEntityDefinition_getPartDefinition00);
    tolua_endmodule(tolua_S);
    tolua_cclass(tolua_S,"PartDefinition","EmberOgre::Model::PartDefinition","",NULL);
    tolua_beginmodule(tolua_S,"PartDefinition");
     tolua_function(tolua_S,"setName",tolua_EmberOgre_EmberOgre_Model_PartDefinition_setName00);
     tolua_function(tolua_S,"getName",tolua_EmberOgre_EmberOgre_Model_PartDefinition_getName00);
     tolua_function(tolua_S,"setShow",tolua_EmberOgre_EmberOgre_Model_PartDefinition_setShow00);
     tolua_function(tolua_S,"getShow",tolua_EmberOgre_EmberOgre_Model_PartDefinition_getShow00);
     tolua_function(tolua_S,"createSubEntityDefinition",tolua_EmberOgre_EmberOgre_Model_PartDefinition_createSubEntityDefinition00);
     tolua_function(tolua_S,"createSubEntityDefinition",tolua_EmberOgre_EmberOgre_Model_PartDefinition_createSubEntityDefinition01);
     tolua_function(tolua_S,"getSubEntityDefinitions",tolua_EmberOgre_EmberOgre_Model_PartDefinition_getSubEntityDefinitions00);
     tolua_function(tolua_S,"removeSubEntityDefinition",tolua_EmberOgre_EmberOgre_Model_PartDefinition_removeSubEntityDefinition00);
     tolua_function(tolua_S,"getSubModelDefinition",tolua_EmberOgre_EmberOgre_Model_PartDefinition_getSubModelDefinition00);
    tolua_endmodule(tolua_S);
    tolua_cclass(tolua_S,"SubModelDefinition","EmberOgre::Model::SubModelDefinition","",NULL);
    tolua_beginmodule(tolua_S,"SubModelDefinition");
     tolua_function(tolua_S,"getMeshName",tolua_EmberOgre_EmberOgre_Model_SubModelDefinition_getMeshName00);
     tolua_function(tolua_S,"createPartDefinition",tolua_EmberOgre_EmberOgre_Model_SubModelDefinition_createPartDefinition00);
     tolua_function(tolua_S,"getPartDefinitions",tolua_EmberOgre_EmberOgre_Model_SubModelDefinition_getPartDefinitions00);
     tolua_function(tolua_S,"removePartDefinition",tolua_EmberOgre_EmberOgre_Model_SubModelDefinition_removePartDefinition00);
     tolua_function(tolua_S,"getModelDefinition",tolua_EmberOgre_EmberOgre_Model_SubModelDefinition_getModelDefinition00);
    tolua_endmodule(tolua_S);
    tolua_cclass(tolua_S,"ModelDefinition","EmberOgre::Model::ModelDefinition","Ogre::Resource",NULL);
    tolua_beginmodule(tolua_S,"ModelDefinition");
     tolua_constant(tolua_S,"MODEL_ALL",EmberOgre::Model::ModelDefinition::MODEL_ALL);
     tolua_constant(tolua_S,"MODEL_NONE",EmberOgre::Model::ModelDefinition::MODEL_NONE);
     tolua_constant(tolua_S,"MODEL_WIDTH",EmberOgre::Model::ModelDefinition::MODEL_WIDTH);
     tolua_constant(tolua_S,"MODEL_DEPTH",EmberOgre::Model::ModelDefinition::MODEL_DEPTH);
     tolua_constant(tolua_S,"MODEL_HEIGHT",EmberOgre::Model::ModelDefinition::MODEL_HEIGHT);
     tolua_function(tolua_S,"isValid",tolua_EmberOgre_EmberOgre_Model_ModelDefinition_isValid00);
     tolua_function(tolua_S,"setValid",tolua_EmberOgre_EmberOgre_Model_ModelDefinition_setValid00);
     tolua_function(tolua_S,"getScale",tolua_EmberOgre_EmberOgre_Model_ModelDefinition_getScale00);
     tolua_function(tolua_S,"setScale",tolua_EmberOgre_EmberOgre_Model_ModelDefinition_setScale00);
     tolua_function(tolua_S,"getUseScaleOf",tolua_EmberOgre_EmberOgre_Model_ModelDefinition_getUseScaleOf00);
     tolua_function(tolua_S,"setUseScaleOf",tolua_EmberOgre_EmberOgre_Model_ModelDefinition_setUseScaleOf00);
     tolua_function(tolua_S,"getTranslate",tolua_EmberOgre_EmberOgre_Model_ModelDefinition_getTranslate00);
     tolua_function(tolua_S,"setTranslate",tolua_EmberOgre_EmberOgre_Model_ModelDefinition_setTranslate00);
     tolua_function(tolua_S,"getShowContained",tolua_EmberOgre_EmberOgre_Model_ModelDefinition_getShowContained00);
     tolua_function(tolua_S,"getShowContained",tolua_EmberOgre_EmberOgre_Model_ModelDefinition_getShowContained01);
     tolua_function(tolua_S,"getContentOffset",tolua_EmberOgre_EmberOgre_Model_ModelDefinition_getContentOffset00);
     tolua_function(tolua_S,"setContentOffset",tolua_EmberOgre_EmberOgre_Model_ModelDefinition_setContentOffset00);
     tolua_function(tolua_S,"getRotation",tolua_EmberOgre_EmberOgre_Model_ModelDefinition_getRotation00);
     tolua_function(tolua_S,"setRotation",tolua_EmberOgre_EmberOgre_Model_ModelDefinition_setRotation00);
     tolua_function(tolua_S,"createSubModelDefinition",tolua_EmberOgre_EmberOgre_Model_ModelDefinition_createSubModelDefinition00);
     tolua_function(tolua_S,"getSubModelDefinitions",tolua_EmberOgre_EmberOgre_Model_ModelDefinition_getSubModelDefinitions00);
     tolua_function(tolua_S,"removeSubModelDefinition",tolua_EmberOgre_EmberOgre_Model_ModelDefinition_removeSubModelDefinition00);
     tolua_function(tolua_S,"createActionDefinition",tolua_EmberOgre_EmberOgre_Model_ModelDefinition_createActionDefinition00);
     tolua_function(tolua_S,"getActionDefinitions",tolua_EmberOgre_EmberOgre_Model_ModelDefinition_getActionDefinitions00);
     tolua_function(tolua_S,"removeActionDefinition",tolua_EmberOgre_EmberOgre_Model_ModelDefinition_removeActionDefinition00);
     tolua_function(tolua_S,"getAttachPointsDefinitions",tolua_EmberOgre_EmberOgre_Model_ModelDefinition_getAttachPointsDefinitions00);
     tolua_function(tolua_S,"createViewDefinition",tolua_EmberOgre_EmberOgre_Model_ModelDefinition_createViewDefinition00);
     tolua_function(tolua_S,"getViewDefinitions",tolua_EmberOgre_EmberOgre_Model_ModelDefinition_getViewDefinitions00);
     tolua_function(tolua_S,"removeViewDefinition",tolua_EmberOgre_EmberOgre_Model_ModelDefinition_removeViewDefinition00);
     tolua_function(tolua_S,"reloadAllInstances",tolua_EmberOgre_EmberOgre_Model_ModelDefinition_reloadAllInstances00);
    tolua_endmodule(tolua_S);
    #ifdef __cplusplus
    tolua_cclass(tolua_S,"ModelDefnPtr","EmberOgre::Model::ModelDefnPtr","",tolua_collect_EmberOgre__Model__ModelDefnPtr);
    #else
    tolua_cclass(tolua_S,"ModelDefnPtr","EmberOgre::Model::ModelDefnPtr","",NULL);
    #endif
    tolua_beginmodule(tolua_S,"ModelDefnPtr");
     tolua_function(tolua_S,"get",tolua_EmberOgre_EmberOgre_Model_ModelDefnPtr_get00);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_cclass(tolua_S,"MouseMotion","EmberOgre::MouseMotion","",NULL);
   tolua_beginmodule(tolua_S,"MouseMotion");
    tolua_variable(tolua_S,"xPosition",tolua_get_EmberOgre__MouseMotion_xPosition,tolua_set_EmberOgre__MouseMotion_xPosition);
    tolua_variable(tolua_S,"yPosition",tolua_get_EmberOgre__MouseMotion_yPosition,tolua_set_EmberOgre__MouseMotion_yPosition);
    tolua_variable(tolua_S,"xRelativeMovement",tolua_get_EmberOgre__MouseMotion_xRelativeMovement,tolua_set_EmberOgre__MouseMotion_xRelativeMovement);
    tolua_variable(tolua_S,"yRelativeMovement",tolua_get_EmberOgre__MouseMotion_yRelativeMovement,tolua_set_EmberOgre__MouseMotion_yRelativeMovement);
    tolua_variable(tolua_S,"xRelativeMovementInPixels",tolua_get_EmberOgre__MouseMotion_xRelativeMovementInPixels,tolua_set_EmberOgre__MouseMotion_xRelativeMovementInPixels);
    tolua_variable(tolua_S,"yRelativeMovementInPixels",tolua_get_EmberOgre__MouseMotion_yRelativeMovementInPixels,tolua_set_EmberOgre__MouseMotion_yRelativeMovementInPixels);
    tolua_variable(tolua_S,"timeSinceLastMovement",tolua_get_EmberOgre__MouseMotion_timeSinceLastMovement,tolua_set_EmberOgre__MouseMotion_timeSinceLastMovement);
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"Input","EmberOgre::Input","",NULL);
   tolua_beginmodule(tolua_S,"Input");
    tolua_function(tolua_S,"getSingleton",tolua_EmberOgre_EmberOgre_Input_getSingleton00);
    tolua_constant(tolua_S,"MouseButtonLeft",EmberOgre::Input::MouseButtonLeft);
    tolua_constant(tolua_S,"MouseButtonRight",EmberOgre::Input::MouseButtonRight);
    tolua_constant(tolua_S,"MouseButtonMiddle",EmberOgre::Input::MouseButtonMiddle);
    tolua_constant(tolua_S,"IM_GUI",EmberOgre::Input::IM_GUI);
    tolua_constant(tolua_S,"IM_MOVEMENT",EmberOgre::Input::IM_MOVEMENT);
    tolua_variable(tolua_S,"EventKeyPressed",tolua_get_EmberOgre__Input_EventKeyPressed,tolua_set_EmberOgre__Input_EventKeyPressed);
    tolua_variable(tolua_S,"EventKeyReleased",tolua_get_EmberOgre__Input_EventKeyReleased,tolua_set_EmberOgre__Input_EventKeyReleased);
    tolua_variable(tolua_S,"EventMouseMoved",tolua_get_EmberOgre__Input_EventMouseMoved,tolua_set_EmberOgre__Input_EventMouseMoved);
    tolua_variable(tolua_S,"EventMouseButtonPressed",tolua_get_EmberOgre__Input_EventMouseButtonPressed,tolua_set_EmberOgre__Input_EventMouseButtonPressed);
    tolua_variable(tolua_S,"EventMouseButtonReleased",tolua_get_EmberOgre__Input_EventMouseButtonReleased,tolua_set_EmberOgre__Input_EventMouseButtonReleased);
    tolua_variable(tolua_S,"EventChangedInputMode",tolua_get_EmberOgre__Input_EventChangedInputMode,tolua_set_EmberOgre__Input_EventChangedInputMode);
    tolua_function(tolua_S,"isKeyDown",tolua_EmberOgre_EmberOgre_Input_isKeyDown00);
    tolua_function(tolua_S,"setInputMode",tolua_EmberOgre_EmberOgre_Input_setInputMode00);
    tolua_function(tolua_S,"getInputMode",tolua_EmberOgre_EmberOgre_Input_getInputMode00);
    tolua_function(tolua_S,"toggleInputMode",tolua_EmberOgre_EmberOgre_Input_toggleInputMode00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Jesus","EmberOgre::Jesus","",tolua_collect_EmberOgre__Jesus);
   #else
   tolua_cclass(tolua_S,"Jesus","EmberOgre::Jesus","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Jesus");
    tolua_constant(tolua_S,"CM_MODELBLOCK",EmberOgre::Jesus::CM_MODELBLOCK);
    tolua_constant(tolua_S,"CM_ATTACHPOINT",EmberOgre::Jesus::CM_ATTACHPOINT);
    tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_Jesus_new00);
    tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_Jesus_new00_local);
    tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_Jesus_new00_local);
    tolua_function(tolua_S,"delete",tolua_EmberOgre_EmberOgre_Jesus_delete00);
    tolua_function(tolua_S,"createModelForBlockType",tolua_EmberOgre_EmberOgre_Jesus_createModelForBlockType00);
    tolua_function(tolua_S,"getColourForAttachPoint",tolua_EmberOgre_EmberOgre_Jesus_getColourForAttachPoint00);
    tolua_function(tolua_S,"getCarpenter",tolua_EmberOgre_EmberOgre_Jesus_getCarpenter00);
    tolua_function(tolua_S,"addBluePrint",tolua_EmberOgre_EmberOgre_Jesus_addBluePrint00);
    tolua_function(tolua_S,"getBluePrint",tolua_EmberOgre_EmberOgre_Jesus_getBluePrint00);
    tolua_function(tolua_S,"getAllBluePrints",tolua_EmberOgre_EmberOgre_Jesus_getAllBluePrints00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"LightWibbler","EmberOgre::LightWibbler","Ogre::ControllerValue<Ogre::Real>",tolua_collect_EmberOgre__LightWibbler);
   #else
   tolua_cclass(tolua_S,"LightWibbler","EmberOgre::LightWibbler","Ogre::ControllerValue<Ogre::Real>",NULL);
   #endif
   tolua_beginmodule(tolua_S,"LightWibbler");
    tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_LightWibbler_new00);
    tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_LightWibbler_new00_local);
    tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_LightWibbler_new00_local);
    tolua_function(tolua_S,"getValue",tolua_EmberOgre_EmberOgre_LightWibbler_getValue00);
    tolua_function(tolua_S,"setValue",tolua_EmberOgre_EmberOgre_LightWibbler_setValue00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"AttachPointNode","EmberOgre::AttachPointNode","",tolua_collect_EmberOgre__AttachPointNode);
   #else
   tolua_cclass(tolua_S,"AttachPointNode","EmberOgre::AttachPointNode","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"AttachPointNode");
    tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_AttachPointNode_new00);
    tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_AttachPointNode_new00_local);
    tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_AttachPointNode_new00_local);
    tolua_function(tolua_S,"delete",tolua_EmberOgre_EmberOgre_AttachPointNode_delete00);
    tolua_function(tolua_S,"select",tolua_EmberOgre_EmberOgre_AttachPointNode_select00);
    tolua_function(tolua_S,"deselect",tolua_EmberOgre_EmberOgre_AttachPointNode_deselect00);
    tolua_function(tolua_S,"getAttachPoint",tolua_EmberOgre_EmberOgre_AttachPointNode_getAttachPoint00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"ModelBlock","EmberOgre::ModelBlock","",tolua_collect_EmberOgre__ModelBlock);
   #else
   tolua_cclass(tolua_S,"ModelBlock","EmberOgre::ModelBlock","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"ModelBlock");
    tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_ModelBlock_new00);
    tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_ModelBlock_new00_local);
    tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_ModelBlock_new00_local);
    tolua_function(tolua_S,"delete",tolua_EmberOgre_EmberOgre_ModelBlock_delete00);
    tolua_function(tolua_S,"getBuildingBlock",tolua_EmberOgre_EmberOgre_ModelBlock_getBuildingBlock00);
    tolua_function(tolua_S,"getConstruction",tolua_EmberOgre_EmberOgre_ModelBlock_getConstruction00);
    tolua_function(tolua_S,"createAttachPointNodes",tolua_EmberOgre_EmberOgre_ModelBlock_createAttachPointNodes00);
    tolua_function(tolua_S,"select",tolua_EmberOgre_EmberOgre_ModelBlock_select00);
    tolua_function(tolua_S,"deselect",tolua_EmberOgre_EmberOgre_ModelBlock_deselect00);
    tolua_function(tolua_S,"getAttachPointNodes",tolua_EmberOgre_EmberOgre_ModelBlock_getAttachPointNodes00);
    tolua_function(tolua_S,"getModel",tolua_EmberOgre_EmberOgre_ModelBlock_getModel00);
    tolua_function(tolua_S,"getNode",tolua_EmberOgre_EmberOgre_ModelBlock_getNode00);
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"ModelMapping","EmberOgre::ModelMapping","",NULL);
   tolua_beginmodule(tolua_S,"ModelMapping");
    tolua_variable(tolua_S,"model",tolua_get_EmberOgre__ModelMapping_model,NULL);
    tolua_variable(tolua_S,"blocktype",tolua_get_EmberOgre__ModelMapping_blocktype,NULL);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Construction","EmberOgre::Construction","",tolua_collect_EmberOgre__Construction);
   #else
   tolua_cclass(tolua_S,"Construction","EmberOgre::Construction","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Construction");
    tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_Construction_new00);
    tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_Construction_new00_local);
    tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_Construction_new00_local);
    tolua_function(tolua_S,"delete",tolua_EmberOgre_EmberOgre_Construction_delete00);
    tolua_function(tolua_S,"getJesus",tolua_EmberOgre_EmberOgre_Construction_getJesus00);
    tolua_function(tolua_S,"getBluePrint",tolua_EmberOgre_EmberOgre_Construction_getBluePrint00);
    tolua_function(tolua_S,"buildFromBluePrint",tolua_EmberOgre_EmberOgre_Construction_buildFromBluePrint00);
    tolua_function(tolua_S,"createModelBlock",tolua_EmberOgre_EmberOgre_Construction_createModelBlock00);
    tolua_function(tolua_S,"getModelBlocks",tolua_EmberOgre_EmberOgre_Construction_getModelBlocks00);
    tolua_function(tolua_S,"remove",tolua_EmberOgre_EmberOgre_Construction_remove00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"TerrainPosition","EmberOgre::TerrainPosition","",tolua_collect_EmberOgre__TerrainPosition);
   #else
   tolua_cclass(tolua_S,"TerrainPosition","EmberOgre::TerrainPosition","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"TerrainPosition");
    tolua_function(tolua_S,"isValid",tolua_EmberOgre_EmberOgre_TerrainPosition_isValid00);
    tolua_function(tolua_S,"setToOrigin",tolua_EmberOgre_EmberOgre_TerrainPosition_setToOrigin00);
    tolua_function(tolua_S,"x",tolua_EmberOgre_EmberOgre_TerrainPosition_x00);
    tolua_function(tolua_S,"x",tolua_EmberOgre_EmberOgre_TerrainPosition_x01);
    tolua_function(tolua_S,"y",tolua_EmberOgre_EmberOgre_TerrainPosition_y00);
    tolua_function(tolua_S,"y",tolua_EmberOgre_EmberOgre_TerrainPosition_y01);
   tolua_endmodule(tolua_S);
   tolua_function(tolua_S,"Atlas2Ogre",tolua_EmberOgre_EmberOgre_Atlas2Ogre00);
   tolua_function(tolua_S,"Ogre2Atlas",tolua_EmberOgre_EmberOgre_Ogre2Atlas00);
   tolua_function(tolua_S,"Ogre2Atlas",tolua_EmberOgre_EmberOgre_Ogre2Atlas01);
   tolua_function(tolua_S,"Ogre2Atlas_TerrainPosition",tolua_EmberOgre_EmberOgre_Ogre2Atlas_TerrainPosition00);
   tolua_function(tolua_S,"Ogre2Atlas_Vector3",tolua_EmberOgre_EmberOgre_Ogre2Atlas_Vector300);
   tolua_function(tolua_S,"Atlas2Ogre",tolua_EmberOgre_EmberOgre_Atlas2Ogre01);
   tolua_function(tolua_S,"Atlas2Ogre",tolua_EmberOgre_EmberOgre_Atlas2Ogre02);
   tolua_function(tolua_S,"Atlas2Ogre",tolua_EmberOgre_EmberOgre_Atlas2Ogre03);
   tolua_function(tolua_S,"Ogre2Atlas",tolua_EmberOgre_EmberOgre_Ogre2Atlas02);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_module(tolua_S,"Gui",0);
   tolua_beginmodule(tolua_S,"Gui");
    tolua_cclass(tolua_S,"MovableObjectRenderer","EmberOgre::Gui::MovableObjectRenderer","",NULL);
    tolua_beginmodule(tolua_S,"MovableObjectRenderer");
     tolua_function(tolua_S,"showFull",tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_showFull00);
     tolua_function(tolua_S,"setCameraDistance",tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_setCameraDistance00);
     tolua_function(tolua_S,"getCameraDistance",tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_getCameraDistance00);
     tolua_function(tolua_S,"getAbsoluteCameraDistance",tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_getAbsoluteCameraDistance00);
     tolua_function(tolua_S,"getIsInputCatchingAllowed",tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_getIsInputCatchingAllowed00);
     tolua_function(tolua_S,"setIsInputCatchingAllowed",tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_setIsInputCatchingAllowed00);
     tolua_function(tolua_S,"setAutoShowFull",tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_setAutoShowFull00);
     tolua_function(tolua_S,"getAutoShowFull",tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_getAutoShowFull00);
     tolua_function(tolua_S,"getActive",tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_getActive00);
     tolua_function(tolua_S,"setActive",tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_setActive00);
     tolua_function(tolua_S,"getEntityRotation",tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_getEntityRotation00);
     tolua_function(tolua_S,"resetCameraOrientation",tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_resetCameraOrientation00);
     tolua_function(tolua_S,"pitch",tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_pitch00);
     tolua_function(tolua_S,"yaw",tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_yaw00);
     tolua_function(tolua_S,"roll",tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_roll00);
     tolua_function(tolua_S,"updateRender",tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_updateRender00);
     tolua_function(tolua_S,"setBackgroundColour",tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_setBackgroundColour00);
     tolua_function(tolua_S,"setBackgroundColour",tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_setBackgroundColour01);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_module(tolua_S,"Gui",0);
   tolua_beginmodule(tolua_S,"Gui");
    #ifdef __cplusplus
    tolua_cclass(tolua_S,"OgreEntityRenderer","EmberOgre::Gui::OgreEntityRenderer","EmberOgre::MovableObjectRenderer",tolua_collect_EmberOgre__Gui__OgreEntityRenderer);
    #else
    tolua_cclass(tolua_S,"OgreEntityRenderer","EmberOgre::Gui::OgreEntityRenderer","EmberOgre::MovableObjectRenderer",NULL);
    #endif
    tolua_beginmodule(tolua_S,"OgreEntityRenderer");
     tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_Gui_OgreEntityRenderer_new00);
     tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_Gui_OgreEntityRenderer_new00_local);
     tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_Gui_OgreEntityRenderer_new00_local);
     tolua_function(tolua_S,"delete",tolua_EmberOgre_EmberOgre_Gui_OgreEntityRenderer_delete00);
     tolua_function(tolua_S,"showEntity",tolua_EmberOgre_EmberOgre_Gui_OgreEntityRenderer_showEntity00);
     tolua_function(tolua_S,"getEntity",tolua_EmberOgre_EmberOgre_Gui_OgreEntityRenderer_getEntity00);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_module(tolua_S,"Gui",0);
   tolua_beginmodule(tolua_S,"Gui");
    #ifdef __cplusplus
    tolua_cclass(tolua_S,"ModelRenderer","EmberOgre::Gui::ModelRenderer","EmberOgre::Gui::MovableObjectRenderer",tolua_collect_EmberOgre__Gui__ModelRenderer);
    #else
    tolua_cclass(tolua_S,"ModelRenderer","EmberOgre::Gui::ModelRenderer","EmberOgre::Gui::MovableObjectRenderer",NULL);
    #endif
    tolua_beginmodule(tolua_S,"ModelRenderer");
     tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_Gui_ModelRenderer_new00);
     tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_Gui_ModelRenderer_new00_local);
     tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_Gui_ModelRenderer_new00_local);
     tolua_function(tolua_S,"delete",tolua_EmberOgre_EmberOgre_Gui_ModelRenderer_delete00);
     tolua_function(tolua_S,"showModel",tolua_EmberOgre_EmberOgre_Gui_ModelRenderer_showModel00);
     tolua_function(tolua_S,"getModel",tolua_EmberOgre_EmberOgre_Gui_ModelRenderer_getModel00);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_module(tolua_S,"Gui",0);
   tolua_beginmodule(tolua_S,"Gui");
    #ifdef __cplusplus
    tolua_cclass(tolua_S,"ListHolder","EmberOgre::Gui::ListHolder","",tolua_collect_EmberOgre__Gui__ListHolder);
    #else
    tolua_cclass(tolua_S,"ListHolder","EmberOgre::Gui::ListHolder","",NULL);
    #endif
    tolua_beginmodule(tolua_S,"ListHolder");
     tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_Gui_ListHolder_new00);
     tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_Gui_ListHolder_new00_local);
     tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_Gui_ListHolder_new00_local);
     tolua_function(tolua_S,"delete",tolua_EmberOgre_EmberOgre_Gui_ListHolder_delete00);
     tolua_function(tolua_S,"addItem",tolua_EmberOgre_EmberOgre_Gui_ListHolder_addItem00);
     tolua_function(tolua_S,"insertItem",tolua_EmberOgre_EmberOgre_Gui_ListHolder_insertItem00);
     tolua_function(tolua_S,"removeItem",tolua_EmberOgre_EmberOgre_Gui_ListHolder_removeItem00);
     tolua_function(tolua_S,"resetList",tolua_EmberOgre_EmberOgre_Gui_ListHolder_resetList00);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_constant(tolua_S,"MPT_CLICK",EmberOgre::MPT_CLICK);
   tolua_constant(tolua_S,"MPT_DOUBLECLICK",EmberOgre::MPT_DOUBLECLICK);
   tolua_cclass(tolua_S,"MousePickerArgs","EmberOgre::MousePickerArgs","",NULL);
   tolua_beginmodule(tolua_S,"MousePickerArgs");
    tolua_variable(tolua_S,"windowX",tolua_get_EmberOgre__MousePickerArgs_windowX,tolua_set_EmberOgre__MousePickerArgs_windowX);
    tolua_variable(tolua_S,"windowY",tolua_get_EmberOgre__MousePickerArgs_windowY,tolua_set_EmberOgre__MousePickerArgs_windowY);
    tolua_variable(tolua_S,"pickType",tolua_get_EmberOgre__MousePickerArgs_pickType,tolua_set_EmberOgre__MousePickerArgs_pickType);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_cclass(tolua_S,"EntityPickResult","EmberOgre::EntityPickResult","",NULL);
   tolua_beginmodule(tolua_S,"EntityPickResult");
    tolua_variable(tolua_S,"entity",tolua_get_EmberOgre__EntityPickResult_entity_ptr,tolua_set_EmberOgre__EntityPickResult_entity_ptr);
    tolua_variable(tolua_S,"position",tolua_get_EmberOgre__EntityPickResult_position,tolua_set_EmberOgre__EntityPickResult_position);
    tolua_variable(tolua_S,"distance",tolua_get_EmberOgre__EntityPickResult_distance,tolua_set_EmberOgre__EntityPickResult_distance);
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"EntityWorldPickListener","EmberOgre::EntityWorldPickListener","",NULL);
   tolua_beginmodule(tolua_S,"EntityWorldPickListener");
    tolua_variable(tolua_S,"EventPickedEntity",tolua_get_EmberOgre__EntityWorldPickListener_EventPickedEntity,tolua_set_EmberOgre__EntityWorldPickListener_EventPickedEntity);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_module(tolua_S,"Model",0);
   tolua_beginmodule(tolua_S,"Model");
    tolua_cclass(tolua_S,"ModelDefinitionManager","EmberOgre::Model::ModelDefinitionManager","Ogre;;ResourceManager",NULL);
    tolua_beginmodule(tolua_S,"ModelDefinitionManager");
     tolua_function(tolua_S,"getSingleton",tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_getSingleton00);
     tolua_function(tolua_S,"exportScript",tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_exportScript00);
     tolua_function(tolua_S,"create",tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_create00);
     tolua_function(tolua_S,"remove",tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_remove00);
     tolua_function(tolua_S,"getByName",tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_getByName00);
     tolua_function(tolua_S,"resourceExists",tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_resourceExists00);
     tolua_function(tolua_S,"getResourceIterator",tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_getResourceIterator00);
     tolua_function(tolua_S,"getAllMeshes",tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_getAllMeshes00);
     tolua_function(tolua_S,"getShowModels",tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_getShowModels00);
     tolua_function(tolua_S,"setShowModels",tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_setShowModels00);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_cclass(tolua_S,"EntityMoveManager","EmberOgre::EntityMoveManager","",NULL);
   tolua_beginmodule(tolua_S,"EntityMoveManager");
    tolua_function(tolua_S,"startMove",tolua_EmberOgre_EmberOgre_EntityMoveManager_startMove00);
    tolua_function(tolua_S,"runCommand",tolua_EmberOgre_EmberOgre_EntityMoveManager_runCommand00);
    tolua_variable(tolua_S,"EventStartMoving",tolua_get_EmberOgre__EntityMoveManager_EventStartMoving,tolua_set_EmberOgre__EntityMoveManager_EventStartMoving);
    tolua_variable(tolua_S,"EventFinishedMoving",tolua_get_EmberOgre__EntityMoveManager_EventFinishedMoving,tolua_set_EmberOgre__EntityMoveManager_EventFinishedMoving);
    tolua_variable(tolua_S,"EventCancelledMoving",tolua_get_EmberOgre__EntityMoveManager_EventCancelledMoving,tolua_set_EmberOgre__EntityMoveManager_EventCancelledMoving);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"std",0);
  tolua_beginmodule(tolua_S,"std");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"vector_std__string_","std::vector<std::string>","",tolua_collect_std__vector_std__string_);
   #else
   tolua_cclass(tolua_S,"vector_std__string_","std::vector<std::string>","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"vector_std__string_");
    tolua_function(tolua_S,"clear",tolua_EmberOgre_std_vector_std__string__clear00);
    tolua_function(tolua_S,"size",tolua_EmberOgre_std_vector_std__string__size00);
    tolua_function(tolua_S,".geti",tolua_EmberOgre_std_vector_std__string___geti00);
    tolua_function(tolua_S,".seti",tolua_EmberOgre_std_vector_std__string___seti00);
    tolua_function(tolua_S,".geti",tolua_EmberOgre_std_vector_std__string___geti01);
    tolua_function(tolua_S,"push_back",tolua_EmberOgre_std_vector_std__string__push_back00);
    tolua_function(tolua_S,"new",tolua_EmberOgre_std_vector_std__string__new00);
    tolua_function(tolua_S,"new_local",tolua_EmberOgre_std_vector_std__string__new00_local);
    tolua_function(tolua_S,".call",tolua_EmberOgre_std_vector_std__string__new00_local);
    tolua_function(tolua_S,"delete",tolua_EmberOgre_std_vector_std__string__delete00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"vector_EmberOgre__Model__SubModelDefinition__","std::vector<EmberOgre::Model::SubModelDefinition*>","",tolua_collect_std__vector_EmberOgre__Model__SubModelDefinition__);
   #else
   tolua_cclass(tolua_S,"vector_EmberOgre__Model__SubModelDefinition__","std::vector<EmberOgre::Model::SubModelDefinition*>","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"vector_EmberOgre__Model__SubModelDefinition__");
    tolua_function(tolua_S,"clear",tolua_EmberOgre_std_vector_EmberOgre__Model__SubModelDefinition___clear00);
    tolua_function(tolua_S,"size",tolua_EmberOgre_std_vector_EmberOgre__Model__SubModelDefinition___size00);
    tolua_function(tolua_S,".geti",tolua_EmberOgre_std_vector_EmberOgre__Model__SubModelDefinition____geti00);
    tolua_function(tolua_S,".seti",tolua_EmberOgre_std_vector_EmberOgre__Model__SubModelDefinition____seti00);
    tolua_function(tolua_S,".geti",tolua_EmberOgre_std_vector_EmberOgre__Model__SubModelDefinition____geti01);
    tolua_function(tolua_S,"push_back",tolua_EmberOgre_std_vector_EmberOgre__Model__SubModelDefinition___push_back00);
    tolua_function(tolua_S,"new",tolua_EmberOgre_std_vector_EmberOgre__Model__SubModelDefinition___new00);
    tolua_function(tolua_S,"new_local",tolua_EmberOgre_std_vector_EmberOgre__Model__SubModelDefinition___new00_local);
    tolua_function(tolua_S,".call",tolua_EmberOgre_std_vector_EmberOgre__Model__SubModelDefinition___new00_local);
    tolua_function(tolua_S,"delete",tolua_EmberOgre_std_vector_EmberOgre__Model__SubModelDefinition___delete00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"vector_EmberOgre__Model__PartDefinition__","std::vector<EmberOgre::Model::PartDefinition*>","",tolua_collect_std__vector_EmberOgre__Model__PartDefinition__);
   #else
   tolua_cclass(tolua_S,"vector_EmberOgre__Model__PartDefinition__","std::vector<EmberOgre::Model::PartDefinition*>","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"vector_EmberOgre__Model__PartDefinition__");
    tolua_function(tolua_S,"clear",tolua_EmberOgre_std_vector_EmberOgre__Model__PartDefinition___clear00);
    tolua_function(tolua_S,"size",tolua_EmberOgre_std_vector_EmberOgre__Model__PartDefinition___size00);
    tolua_function(tolua_S,".geti",tolua_EmberOgre_std_vector_EmberOgre__Model__PartDefinition____geti00);
    tolua_function(tolua_S,".seti",tolua_EmberOgre_std_vector_EmberOgre__Model__PartDefinition____seti00);
    tolua_function(tolua_S,".geti",tolua_EmberOgre_std_vector_EmberOgre__Model__PartDefinition____geti01);
    tolua_function(tolua_S,"push_back",tolua_EmberOgre_std_vector_EmberOgre__Model__PartDefinition___push_back00);
    tolua_function(tolua_S,"new",tolua_EmberOgre_std_vector_EmberOgre__Model__PartDefinition___new00);
    tolua_function(tolua_S,"new_local",tolua_EmberOgre_std_vector_EmberOgre__Model__PartDefinition___new00_local);
    tolua_function(tolua_S,".call",tolua_EmberOgre_std_vector_EmberOgre__Model__PartDefinition___new00_local);
    tolua_function(tolua_S,"delete",tolua_EmberOgre_std_vector_EmberOgre__Model__PartDefinition___delete00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"vector_EmberOgre__Model__SubEntityDefinition__","std::vector<EmberOgre::Model::SubEntityDefinition*>","",tolua_collect_std__vector_EmberOgre__Model__SubEntityDefinition__);
   #else
   tolua_cclass(tolua_S,"vector_EmberOgre__Model__SubEntityDefinition__","std::vector<EmberOgre::Model::SubEntityDefinition*>","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"vector_EmberOgre__Model__SubEntityDefinition__");
    tolua_function(tolua_S,"clear",tolua_EmberOgre_std_vector_EmberOgre__Model__SubEntityDefinition___clear00);
    tolua_function(tolua_S,"size",tolua_EmberOgre_std_vector_EmberOgre__Model__SubEntityDefinition___size00);
    tolua_function(tolua_S,".geti",tolua_EmberOgre_std_vector_EmberOgre__Model__SubEntityDefinition____geti00);
    tolua_function(tolua_S,".seti",tolua_EmberOgre_std_vector_EmberOgre__Model__SubEntityDefinition____seti00);
    tolua_function(tolua_S,".geti",tolua_EmberOgre_std_vector_EmberOgre__Model__SubEntityDefinition____geti01);
    tolua_function(tolua_S,"push_back",tolua_EmberOgre_std_vector_EmberOgre__Model__SubEntityDefinition___push_back00);
    tolua_function(tolua_S,"new",tolua_EmberOgre_std_vector_EmberOgre__Model__SubEntityDefinition___new00);
    tolua_function(tolua_S,"new_local",tolua_EmberOgre_std_vector_EmberOgre__Model__SubEntityDefinition___new00_local);
    tolua_function(tolua_S,".call",tolua_EmberOgre_std_vector_EmberOgre__Model__SubEntityDefinition___new00_local);
    tolua_function(tolua_S,"delete",tolua_EmberOgre_std_vector_EmberOgre__Model__SubEntityDefinition___delete00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"vector_Eris__Task__","std::vector<Eris::Task*>","",tolua_collect_std__vector_Eris__Task__);
   #else
   tolua_cclass(tolua_S,"vector_Eris__Task__","std::vector<Eris::Task*>","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"vector_Eris__Task__");
    tolua_function(tolua_S,"clear",tolua_EmberOgre_std_vector_Eris__Task___clear00);
    tolua_function(tolua_S,"size",tolua_EmberOgre_std_vector_Eris__Task___size00);
    tolua_function(tolua_S,".geti",tolua_EmberOgre_std_vector_Eris__Task____geti00);
    tolua_function(tolua_S,".seti",tolua_EmberOgre_std_vector_Eris__Task____seti00);
    tolua_function(tolua_S,".geti",tolua_EmberOgre_std_vector_Eris__Task____geti01);
    tolua_function(tolua_S,"push_back",tolua_EmberOgre_std_vector_Eris__Task___push_back00);
    tolua_function(tolua_S,"new",tolua_EmberOgre_std_vector_Eris__Task___new00);
    tolua_function(tolua_S,"new_local",tolua_EmberOgre_std_vector_Eris__Task___new00_local);
    tolua_function(tolua_S,".call",tolua_EmberOgre_std_vector_Eris__Task___new00_local);
    tolua_function(tolua_S,"delete",tolua_EmberOgre_std_vector_Eris__Task___delete00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"set_std__string_","std::set<std::string>","",tolua_collect_std__set_std__string_);
   #else
   tolua_cclass(tolua_S,"set_std__string_","std::set<std::string>","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"set_std__string_");
    tolua_function(tolua_S,"clear",tolua_EmberOgre_std_set_std__string__clear00);
    tolua_function(tolua_S,"size",tolua_EmberOgre_std_set_std__string__size00);
    tolua_function(tolua_S,"new",tolua_EmberOgre_std_set_std__string__new00);
    tolua_function(tolua_S,"new_local",tolua_EmberOgre_std_set_std__string__new00_local);
    tolua_function(tolua_S,".call",tolua_EmberOgre_std_set_std__string__new00_local);
    tolua_function(tolua_S,"delete",tolua_EmberOgre_std_set_std__string__delete00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_module(tolua_S,"Gui",0);
   tolua_beginmodule(tolua_S,"Gui");
    #ifdef __cplusplus
    tolua_cclass(tolua_S,"Vector3Adapter","EmberOgre::Gui::Vector3Adapter","",tolua_collect_EmberOgre__Gui__Vector3Adapter);
    #else
    tolua_cclass(tolua_S,"Vector3Adapter","EmberOgre::Gui::Vector3Adapter","",NULL);
    #endif
    tolua_beginmodule(tolua_S,"Vector3Adapter");
     tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_Gui_Vector3Adapter_new00);
     tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_Gui_Vector3Adapter_new00_local);
     tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_Gui_Vector3Adapter_new00_local);
     tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_Gui_Vector3Adapter_new01);
     tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_Gui_Vector3Adapter_new01_local);
     tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_Gui_Vector3Adapter_new01_local);
     tolua_function(tolua_S,"delete",tolua_EmberOgre_EmberOgre_Gui_Vector3Adapter_delete00);
     tolua_function(tolua_S,"getValue",tolua_EmberOgre_EmberOgre_Gui_Vector3Adapter_getValue00);
     tolua_function(tolua_S,"getOriginalValue",tolua_EmberOgre_EmberOgre_Gui_Vector3Adapter_getOriginalValue00);
     tolua_function(tolua_S,"setValue",tolua_EmberOgre_EmberOgre_Gui_Vector3Adapter_setValue00);
     tolua_function(tolua_S,"updateGui",tolua_EmberOgre_EmberOgre_Gui_Vector3Adapter_updateGui00);
     tolua_variable(tolua_S,"EventValueChanged",tolua_get_EmberOgre__Gui__Vector3Adapter_EventValueChanged,tolua_set_EmberOgre__Gui__Vector3Adapter_EventValueChanged);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_module(tolua_S,"Gui",0);
   tolua_beginmodule(tolua_S,"Gui");
    #ifdef __cplusplus
    tolua_cclass(tolua_S,"QuaternionAdapter","EmberOgre::Gui::QuaternionAdapter","",tolua_collect_EmberOgre__Gui__QuaternionAdapter);
    #else
    tolua_cclass(tolua_S,"QuaternionAdapter","EmberOgre::Gui::QuaternionAdapter","",NULL);
    #endif
    tolua_beginmodule(tolua_S,"QuaternionAdapter");
     tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_Gui_QuaternionAdapter_new00);
     tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_Gui_QuaternionAdapter_new00_local);
     tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_Gui_QuaternionAdapter_new00_local);
     tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_Gui_QuaternionAdapter_new01);
     tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_Gui_QuaternionAdapter_new01_local);
     tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_Gui_QuaternionAdapter_new01_local);
     tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_Gui_QuaternionAdapter_new02);
     tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_Gui_QuaternionAdapter_new02_local);
     tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_Gui_QuaternionAdapter_new02_local);
     tolua_function(tolua_S,"delete",tolua_EmberOgre_EmberOgre_Gui_QuaternionAdapter_delete00);
     tolua_function(tolua_S,"getValue",tolua_EmberOgre_EmberOgre_Gui_QuaternionAdapter_getValue00);
     tolua_function(tolua_S,"getOriginalValue",tolua_EmberOgre_EmberOgre_Gui_QuaternionAdapter_getOriginalValue00);
     tolua_function(tolua_S,"setValue",tolua_EmberOgre_EmberOgre_Gui_QuaternionAdapter_setValue00);
     tolua_function(tolua_S,"updateGui",tolua_EmberOgre_EmberOgre_Gui_QuaternionAdapter_updateGui00);
     tolua_variable(tolua_S,"EventValueChanged",tolua_get_EmberOgre__Gui__QuaternionAdapter_EventValueChanged,tolua_set_EmberOgre__Gui__QuaternionAdapter_EventValueChanged);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_module(tolua_S,"Terrain",0);
   tolua_beginmodule(tolua_S,"Terrain");
    #ifdef __cplusplus
    tolua_cclass(tolua_S,"Map","EmberOgre::Terrain::Map","",tolua_collect_EmberOgre__Terrain__Map);
    #else
    tolua_cclass(tolua_S,"Map","EmberOgre::Terrain::Map","",NULL);
    #endif
    tolua_beginmodule(tolua_S,"Map");
     tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_Terrain_Map_new00);
     tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_Terrain_Map_new00_local);
     tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_Terrain_Map_new00_local);
     tolua_function(tolua_S,"delete",tolua_EmberOgre_EmberOgre_Terrain_Map_delete00);
     tolua_function(tolua_S,"initialize",tolua_EmberOgre_EmberOgre_Terrain_Map_initialize00);
     tolua_function(tolua_S,"getTexture",tolua_EmberOgre_EmberOgre_Terrain_Map_getTexture00);
     tolua_function(tolua_S,"render",tolua_EmberOgre_EmberOgre_Terrain_Map_render00);
     tolua_function(tolua_S,"reposition",tolua_EmberOgre_EmberOgre_Terrain_Map_reposition00);
     tolua_function(tolua_S,"reposition",tolua_EmberOgre_EmberOgre_Terrain_Map_reposition01);
     tolua_function(tolua_S,"setDistance",tolua_EmberOgre_EmberOgre_Terrain_Map_setDistance00);
     tolua_function(tolua_S,"getDistance",tolua_EmberOgre_EmberOgre_Terrain_Map_getDistance00);
     tolua_function(tolua_S,"getResolution",tolua_EmberOgre_EmberOgre_Terrain_Map_getResolution00);
     tolua_function(tolua_S,"getResolutionMeters",tolua_EmberOgre_EmberOgre_Terrain_Map_getResolutionMeters00);
     tolua_function(tolua_S,"getView",tolua_EmberOgre_EmberOgre_Terrain_Map_getView00);
    tolua_endmodule(tolua_S);
    tolua_cclass(tolua_S,"MapView","EmberOgre::Terrain::MapView","",NULL);
    tolua_beginmodule(tolua_S,"MapView");
     tolua_function(tolua_S,"reposition",tolua_EmberOgre_EmberOgre_Terrain_MapView_reposition00);
     tolua_function(tolua_S,"getRelativeViewBounds",tolua_EmberOgre_EmberOgre_Terrain_MapView_getRelativeViewBounds00);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"OgreInfo","EmberOgre::OgreInfo","",tolua_collect_EmberOgre__OgreInfo);
   #else
   tolua_cclass(tolua_S,"OgreInfo","EmberOgre::OgreInfo","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"OgreInfo");
    tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_OgreInfo_new00);
    tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_OgreInfo_new00_local);
    tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_OgreInfo_new00_local);
    tolua_function(tolua_S,"delete",tolua_EmberOgre_EmberOgre_OgreInfo_delete00);
    tolua_function(tolua_S,"isIndirect",tolua_EmberOgre_EmberOgre_OgreInfo_isIndirect00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"AttributeObserver","EmberOgre::AttributeObserver","",tolua_collect_EmberOgre__AttributeObserver);
   #else
   tolua_cclass(tolua_S,"AttributeObserver","EmberOgre::AttributeObserver","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"AttributeObserver");
    tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_AttributeObserver_new00);
    tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_AttributeObserver_new00_local);
    tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_AttributeObserver_new00_local);
    tolua_function(tolua_S,"delete",tolua_EmberOgre_EmberOgre_AttributeObserver_delete00);
    tolua_variable(tolua_S,"EventChanged",tolua_get_EmberOgre__AttributeObserver_EventChanged,tolua_set_EmberOgre__AttributeObserver_EventChanged);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_cclass(tolua_S,"EmberOgre","EmberOgre::EmberOgre","",NULL);
   tolua_beginmodule(tolua_S,"EmberOgre");
    tolua_function(tolua_S,"getSingleton",tolua_EmberOgre_EmberOgre_EmberOgre_getSingleton00);
    tolua_function(tolua_S,"getAvatar",tolua_EmberOgre_EmberOgre_EmberOgre_getAvatar00);
    tolua_function(tolua_S,"getMotionManager",tolua_EmberOgre_EmberOgre_EmberOgre_getMotionManager00);
    tolua_function(tolua_S,"getEntityFactory",tolua_EmberOgre_EmberOgre_EmberOgre_getEntityFactory00);
    tolua_function(tolua_S,"getMainCamera",tolua_EmberOgre_EmberOgre_EmberOgre_getMainCamera00);
    tolua_function(tolua_S,"getAvatarController",tolua_EmberOgre_EmberOgre_EmberOgre_getAvatarController00);
    tolua_function(tolua_S,"getMoveManager",tolua_EmberOgre_EmberOgre_EmberOgre_getMoveManager00);
    tolua_function(tolua_S,"getEmberEntity",tolua_EmberOgre_EmberOgre_EmberOgre_getEmberEntity00);
    tolua_function(tolua_S,"getJesus",tolua_EmberOgre_EmberOgre_EmberOgre_getJesus00);
    tolua_function(tolua_S,"getRenderWindow",tolua_EmberOgre_EmberOgre_EmberOgre_getRenderWindow00);
    tolua_variable(tolua_S,"EventCreatedEmberEntityFactory",tolua_get_EmberOgre__EmberOgre_EventCreatedEmberEntityFactory,tolua_set_EmberOgre__EmberOgre_EventCreatedEmberEntityFactory);
    tolua_variable(tolua_S,"EventCreatedAvatarEntity",tolua_get_EmberOgre__EmberOgre_EventCreatedAvatarEntity,tolua_set_EmberOgre__EmberOgre_EventCreatedAvatarEntity);
    tolua_variable(tolua_S,"EventCreatedJesus",tolua_get_EmberOgre__EmberOgre_EventCreatedJesus,tolua_set_EmberOgre__EmberOgre_EventCreatedJesus);
    tolua_function(tolua_S,"getWorldSceneNode",tolua_EmberOgre_EmberOgre_EmberOgre_getWorldSceneNode00);
    tolua_function(tolua_S,"getRootSceneNode",tolua_EmberOgre_EmberOgre_EmberOgre_getRootSceneNode00);
    tolua_variable(tolua_S,"EventGUIManagerCreated",tolua_get_EmberOgre__EmberOgre_EventGUIManagerCreated,tolua_set_EmberOgre__EmberOgre_EventGUIManagerCreated);
    tolua_variable(tolua_S,"EventGUIManagerInitialized",tolua_get_EmberOgre__EmberOgre_EventGUIManagerInitialized,tolua_set_EmberOgre__EmberOgre_EventGUIManagerInitialized);
    tolua_variable(tolua_S,"EventMotionManagerCreated",tolua_get_EmberOgre__EmberOgre_EventMotionManagerCreated,tolua_set_EmberOgre__EmberOgre_EventMotionManagerCreated);
    tolua_variable(tolua_S,"EventTerrainGeneratorCreated",tolua_get_EmberOgre__EmberOgre_EventTerrainGeneratorCreated,tolua_set_EmberOgre__EmberOgre_EventTerrainGeneratorCreated);
    tolua_variable(tolua_S,"EventAvatarControllerCreated",tolua_get_EmberOgre__EmberOgre_EventAvatarControllerCreated,tolua_set_EmberOgre__EmberOgre_EventAvatarControllerCreated);
    tolua_variable(tolua_S,"EventSceneCreated",tolua_get_EmberOgre__EmberOgre_EventSceneCreated,tolua_set_EmberOgre__EmberOgre_EventSceneCreated);
    tolua_function(tolua_S,"getEntity",tolua_EmberOgre_EmberOgre_EmberOgre_getEntity00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_EmberOgre (lua_State* tolua_S) {
 return tolua_EmberOgre_open(tolua_S);
};
#endif

