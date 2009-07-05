/*
** Lua binding: EmberOgre
** Generated automatically by tolua++-1.0.92.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_EmberOgre_open (lua_State* tolua_S);

#include "required.h"
#include "components/ogre/widgets/AssetsManager.h"
#include "components/ogre/widgets/Compass.h"
#include "components/ogre/manipulation/GUIAdapter.h"
#include "components/ogre/manipulation/EntityRecipe.h"
#define __operator_ptr operator*
#define __operator_pp operator++
#include "components/ogre/manipulation/EntityRecipeManager.h"
#include "components/ogre/widgets/EntityCreator.h"
#include "components/ogre/terrain/Map.h"
#include "components/ogre/terrain/TerrainLayerDefinition.h"
#include "components/ogre/terrain/TerrainLayerDefinitionManager.h"
#define __operator_ptr operator*
#define __operator_pp operator++

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_EmberOgre__Model__ModelDefnPtr (lua_State* tolua_S)
{
 EmberOgre::Model::ModelDefnPtr* self = (EmberOgre::Model::ModelDefnPtr*) tolua_tousertype(tolua_S,1,0);
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

static int tolua_collect_EmberOgre__Gui__RenderedCompassImpl (lua_State* tolua_S)
{
 EmberOgre::Gui::RenderedCompassImpl* self = (EmberOgre::Gui::RenderedCompassImpl*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_std__vector_EmberOgre__Model__PartDefinition__ (lua_State* tolua_S)
{
 std::vector<EmberOgre::Model::PartDefinition*>* self = (std::vector<EmberOgre::Model::PartDefinition*>*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__GUIAdapter (lua_State* tolua_S)
{
 EmberOgre::GUIAdapter* self = (EmberOgre::GUIAdapter*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__SimpleRenderContext (lua_State* tolua_S)
{
 EmberOgre::SimpleRenderContext* self = (EmberOgre::SimpleRenderContext*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Terrain__TerrainLayerDefinitionManager__DefinitionStore__const_iterator (lua_State* tolua_S)
{
 EmberOgre::Terrain::TerrainLayerDefinitionManager::DefinitionStore::const_iterator* self = (EmberOgre::Terrain::TerrainLayerDefinitionManager::DefinitionStore::const_iterator*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_WFMath__Point_2_ (lua_State* tolua_S)
{
 WFMath::Point<2>* self = (WFMath::Point<2>*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Gui__Vector3Adapter (lua_State* tolua_S)
{
 EmberOgre::Gui::Vector3Adapter* self = (EmberOgre::Gui::Vector3Adapter*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__EntityRecipe (lua_State* tolua_S)
{
 EmberOgre::EntityRecipe* self = (EmberOgre::EntityRecipe*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Gui__AssetsManager (lua_State* tolua_S)
{
 EmberOgre::Gui::AssetsManager* self = (EmberOgre::Gui::AssetsManager*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__GUIAdaptersStore__const_iterator (lua_State* tolua_S)
{
 EmberOgre::GUIAdaptersStore::const_iterator* self = (EmberOgre::GUIAdaptersStore::const_iterator*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_std__vector_Eris__Task__ (lua_State* tolua_S)
{
 std::vector<Eris::Task*>* self = (std::vector<Eris::Task*>*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_std__vector_std__string_ (lua_State* tolua_S)
{
 std::vector<std::string>* self = (std::vector<std::string>*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_size_t (lua_State* tolua_S)
{
 size_t* self = (size_t*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Gui__EntityEditor (lua_State* tolua_S)
{
 EmberOgre::Gui::EntityEditor* self = (EmberOgre::Gui::EntityEditor*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Terrain__Map (lua_State* tolua_S)
{
 EmberOgre::Terrain::Map* self = (EmberOgre::Terrain::Map*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Gui__QuaternionAdapter (lua_State* tolua_S)
{
 EmberOgre::Gui::QuaternionAdapter* self = (EmberOgre::Gui::QuaternionAdapter*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__OgreInfo (lua_State* tolua_S)
{
 EmberOgre::OgreInfo* self = (EmberOgre::OgreInfo*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_std__vector_EmberOgre__Model__SubModelDefinition__ (lua_State* tolua_S)
{
 std::vector<EmberOgre::Model::SubModelDefinition*>* self = (std::vector<EmberOgre::Model::SubModelDefinition*>*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Gui__CompassCameraAnchor (lua_State* tolua_S)
{
 EmberOgre::Gui::CompassCameraAnchor* self = (EmberOgre::Gui::CompassCameraAnchor*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_WFMath__Point_3_ (lua_State* tolua_S)
{
 WFMath::Point<3>* self = (WFMath::Point<3>*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__EntityRecipeManager (lua_State* tolua_S)
{
 EmberOgre::EntityRecipeManager* self = (EmberOgre::EntityRecipeManager*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Model__Model (lua_State* tolua_S)
{
 EmberOgre::Model::Model* self = (EmberOgre::Model::Model*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_Ogre__ResourcePtr (lua_State* tolua_S)
{
 Ogre::ResourcePtr* self = (Ogre::ResourcePtr*) tolua_tousertype(tolua_S,1,0);
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

static int tolua_collect_WFMath__Vector_3_ (lua_State* tolua_S)
{
 WFMath::Vector<3>* self = (WFMath::Vector<3>*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Gui__ModelRenderer (lua_State* tolua_S)
{
 EmberOgre::Gui::ModelRenderer* self = (EmberOgre::Gui::ModelRenderer*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_Ogre__Real (lua_State* tolua_S)
{
 Ogre::Real* self = (Ogre::Real*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Gui__OgreEntityRenderer (lua_State* tolua_S)
{
 EmberOgre::Gui::OgreEntityRenderer* self = (EmberOgre::Gui::OgreEntityRenderer*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Gui__IconBase (lua_State* tolua_S)
{
 EmberOgre::Gui::IconBase* self = (EmberOgre::Gui::IconBase*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Gui__ListHolder (lua_State* tolua_S)
{
 EmberOgre::Gui::ListHolder* self = (EmberOgre::Gui::ListHolder*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_Atlas__Message__MapType (lua_State* tolua_S)
{
 Atlas::Message::MapType* self = (Atlas::Message::MapType*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_Ogre__Quaternion (lua_State* tolua_S)
{
 Ogre::Quaternion* self = (Ogre::Quaternion*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Gui__OverlayCompassImpl (lua_State* tolua_S)
{
 EmberOgre::Gui::OverlayCompassImpl* self = (EmberOgre::Gui::OverlayCompassImpl*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_Ogre__TexturePtr (lua_State* tolua_S)
{
 Ogre::TexturePtr* self = (Ogre::TexturePtr*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Model__ModelDefinitionAtlasComposer (lua_State* tolua_S)
{
 EmberOgre::Model::ModelDefinitionAtlasComposer* self = (EmberOgre::Model::ModelDefinitionAtlasComposer*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Gui__EntityCreator (lua_State* tolua_S)
{
 EmberOgre::Gui::EntityCreator* self = (EmberOgre::Gui::EntityCreator*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Terrain__TerrainArea (lua_State* tolua_S)
{
 EmberOgre::Terrain::TerrainArea* self = (EmberOgre::Terrain::TerrainArea*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_Ogre__Vector2 (lua_State* tolua_S)
{
 Ogre::Vector2* self = (Ogre::Vector2*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__AvatarControllerMovement (lua_State* tolua_S)
{
 EmberOgre::AvatarControllerMovement* self = (EmberOgre::AvatarControllerMovement*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Gui__CEGUICompassImpl (lua_State* tolua_S)
{
 EmberOgre::Gui::CEGUICompassImpl* self = (EmberOgre::Gui::CEGUICompassImpl*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Gui__Compass (lua_State* tolua_S)
{
 EmberOgre::Gui::Compass* self = (EmberOgre::Gui::Compass*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Terrain__TerrainDefPoint (lua_State* tolua_S)
{
 EmberOgre::Terrain::TerrainDefPoint* self = (EmberOgre::Terrain::TerrainDefPoint*) tolua_tousertype(tolua_S,1,0);
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

static int tolua_collect_EmberOgre__Gui__Icons__IconManager (lua_State* tolua_S)
{
 EmberOgre::Gui::Icons::IconManager* self = (EmberOgre::Gui::Icons::IconManager*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__AttributeObserver (lua_State* tolua_S)
{
 EmberOgre::AttributeObserver* self = (EmberOgre::AttributeObserver*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Gui__CompassThirdPersonCameraAnchor (lua_State* tolua_S)
{
 EmberOgre::Gui::CompassThirdPersonCameraAnchor* self = (EmberOgre::Gui::CompassThirdPersonCameraAnchor*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Gui__StackableContainer (lua_State* tolua_S)
{
 EmberOgre::Gui::StackableContainer* self = (EmberOgre::Gui::StackableContainer*) tolua_tousertype(tolua_S,1,0);
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
 tolua_usertype(tolua_S,"sigc::signal<void,EmberOgre::Terrain::TerrainShader*>");
 tolua_usertype(tolua_S,"EmberOgre::Gui::RenderedCompassImpl");
 tolua_usertype(tolua_S,"EmberOgre::Terrain::TerrainMod");
 tolua_usertype(tolua_S,"CEGUI::TreeItem");
 tolua_usertype(tolua_S,"Ogre::SceneManager");
 tolua_usertype(tolua_S,"EmberOgre::SimpleRenderContext");
 tolua_usertype(tolua_S,"Mercator::BasePoint");
 tolua_usertype(tolua_S,"Ogre::Degree");
 tolua_usertype(tolua_S,"sigc::signal<void,EmberOgre::Gui::EntityIcon*>");
 tolua_usertype(tolua_S,"std::vector<EmberOgre::Model::ActionDefinition*>");
 tolua_usertype(tolua_S,"EmberOgre::Gui::EntityEditor");
 tolua_usertype(tolua_S,"sigc::signal<void,EmberOgre::GUIManager&>");
 tolua_usertype(tolua_S,"EmberOgre::Gui::CompassCameraAnchor");
 tolua_usertype(tolua_S,"EmberOgre::Terrain::TerrainInfo");
 tolua_usertype(tolua_S,"EmberOgre::MovableObjectRenderer");
 tolua_usertype(tolua_S,"EmberOgre::Terrain::BasePointUserObject");
 tolua_usertype(tolua_S,"EmberOgre::Terrain::TerrainDefPoint");
 tolua_usertype(tolua_S,"EmberOgre::Gui::Adapters::Atlas::MapAdapter");
 tolua_usertype(tolua_S,"EmberOgre::Gui::ICompassImpl");
 tolua_usertype(tolua_S,"sigc::signal<void,EmberOgre::Terrain::TerrainShader*,EmberOgre::Terrain::TerrainGenerator::AreaStore*>");
 tolua_usertype(tolua_S,"sigc::signal<void,EmberOgre::EmberEntity*>");
 tolua_usertype(tolua_S,"sigc::signal<void,const EmberOgre::Terrain::TerrainEditAction*>");
 tolua_usertype(tolua_S,"Ogre::TRect<float>");
 tolua_usertype(tolua_S,"EmberOgre::Gui::EntityIconSlot");
 tolua_usertype(tolua_S,"EmberOgre::Gui::CompassSceneNodeAnchor");
 tolua_usertype(tolua_S,"EmberOgre::Gui::TexturePair");
 tolua_usertype(tolua_S,"sigc::signal<void,EmberOgre::AvatarMovementMode::Mode>");
 tolua_usertype(tolua_S,"Atlas::Message::Element");
 tolua_usertype(tolua_S,"std::vector<EmberOgre::Terrain::TerrainEditBasePointMovement>");
 tolua_usertype(tolua_S,"Ogre::Vector2");
 tolua_usertype(tolua_S,"std::iostream");
 tolua_usertype(tolua_S,"EmberOgre::Gui::IconBar");
 tolua_usertype(tolua_S,"EmberOgre::Terrain::TerrainLayerDefinition");
 tolua_usertype(tolua_S,"EmberOgre::Convert");
 tolua_usertype(tolua_S,"sigc::signal<void,EmberOgre::Terrain::BasePointUserObject*>");
 tolua_usertype(tolua_S,"Ogre::AxisAlignedBox");
 tolua_usertype(tolua_S,"EmberOgre::Gui::EntityIconManager");
 tolua_usertype(tolua_S,"WFMath::Point<2>");
 tolua_usertype(tolua_S,"Mercator::Terrain");
 tolua_usertype(tolua_S,"sigc::signal<void,float>");
 tolua_usertype(tolua_S,"sigc::signal<void,EmberOgre::AvatarEmberEntity*>");
 tolua_usertype(tolua_S,"EmberOgre::Environment::ISun");
 tolua_usertype(tolua_S,"Ember::ConsoleCommandWrapper");
 tolua_usertype(tolua_S,"EmberOgre::Model::ModelDefinition");
 tolua_usertype(tolua_S,"WFMath::Quaternion");
 tolua_usertype(tolua_S,"Ember::Singleton<EntityRecipeManager>");
 tolua_usertype(tolua_S,"EmberOgre::Environment::IFog");
 tolua_usertype(tolua_S,"EmberOgre::Model::AttachPointDefinition");
 tolua_usertype(tolua_S,"EmberOgre::EmberPhysicalEntity");
 tolua_usertype(tolua_S,"EmberOgre::Terrain::ISceneManagerAdapter");
 tolua_usertype(tolua_S,"sigc::signal<void,const std::string&,EmberOgre::EmberEntity*>");
 tolua_usertype(tolua_S,"EmberOgre::Avatar");
 tolua_usertype(tolua_S,"Ogre::SceneNode");
 tolua_usertype(tolua_S,"sigc::signal<void,EmberOgre::Terrain::TerrainGenerator&>");
 tolua_usertype(tolua_S,"EmberOgre::AvatarMovementMode");
 tolua_usertype(tolua_S,"sigc::signal<void,EmberOgre::MotionManager&>");
 tolua_usertype(tolua_S,"Eris::Entity");
 tolua_usertype(tolua_S,"Ogre::Light");
 tolua_usertype(tolua_S,"EmberOgre::Gui::Compass");
 tolua_usertype(tolua_S,"sigc::signal<void,const std::string&>");
 tolua_usertype(tolua_S,"EmberOgre::EmberEntityFactory");
 tolua_usertype(tolua_S,"Ogre::Entity");
 tolua_usertype(tolua_S,"Ogre::Vector3");
 tolua_usertype(tolua_S,"sigc::signal<void,EmberOgre::EmberEntityFactory*>");
 tolua_usertype(tolua_S,"Ogre::RenderWindow");
 tolua_usertype(tolua_S,"EmberOgre::Gui::CompassThirdPersonCameraAnchor");
 tolua_usertype(tolua_S,"EmberOgre::Model::Action");
 tolua_usertype(tolua_S,"EmberOgre::Terrain::TerrainLayerDefinitionManager::DefinitionStore::const_iterator");
 tolua_usertype(tolua_S,"sigc::signal<void,const Ogre::ColourValue&>");
 tolua_usertype(tolua_S,"EmberOgre::Terrain::TerrainLayerDefinitionManager::DefinitionStore");
 tolua_usertype(tolua_S,"std::vector<std::string>");
 tolua_usertype(tolua_S,"CEGUI::Image");
 tolua_usertype(tolua_S,"sigc::signal<void,const Atlas::Message::Element&>");
 tolua_usertype(tolua_S,"sigc::signal<void,const EmberOgre::EntityPickResult&,const EmberOgre::MousePickerArgs&>");
 tolua_usertype(tolua_S,"EmberOgre::OgreInfo");
 tolua_usertype(tolua_S,"EmberOgre::AvatarEmberEntity");
 tolua_usertype(tolua_S,"EmberOgre::Gui::Icons::Icon");
 tolua_usertype(tolua_S,"Ogre::ResourcePtr");
 tolua_usertype(tolua_S,"Ogre::Radian");
 tolua_usertype(tolua_S,"CEGUI::Editbox");
 tolua_usertype(tolua_S,"EmberOgre::Terrain::Map");
 tolua_usertype(tolua_S,"EmberOgre::Gui::QuaternionAdapter");
 tolua_usertype(tolua_S,"EmberOgre::Gui::Widget");
 tolua_usertype(tolua_S,"EmberOgre::Terrain::TerrainPage");
 tolua_usertype(tolua_S,"Eris::Task");
 tolua_usertype(tolua_S,"const");
 tolua_usertype(tolua_S,"std::vector<Eris::Task*>");
 tolua_usertype(tolua_S,"EmberOgre::EntityRecipe");
 tolua_usertype(tolua_S,"std::vector<EmberOgre::Terrain::TerrainDefPoint>");
 tolua_usertype(tolua_S,"EmberOgre::GUIAdaptersStore::const_iterator");
 tolua_usertype(tolua_S,"CEGUI::Window");
 tolua_usertype(tolua_S,"Ogre::Viewport");
 tolua_usertype(tolua_S,"size_t");
 tolua_usertype(tolua_S,"WFMath::Point<3>");
 tolua_usertype(tolua_S,"EmberOgre::Terrain::TerrainLayerDefinitionManager");
 tolua_usertype(tolua_S,"EmberOgre::AvatarCamera");
 tolua_usertype(tolua_S,"EmberOgre::Terrain::TerrainShader");
 tolua_usertype(tolua_S,"Ember::Singleton<EmberOgre::EntityRecipeManager>");
 tolua_usertype(tolua_S,"CEGUI::ListboxItem");
 tolua_usertype(tolua_S,"CEGUI::Imageset");
 tolua_usertype(tolua_S,"EmberOgre::AvatarController");
 tolua_usertype(tolua_S,"EmberOgre::Model::Model");
 tolua_usertype(tolua_S,"EmberOgre::MousePickerArgs");
 tolua_usertype(tolua_S,"EmberOgre::Gui::ConsoleAdapter");
 tolua_usertype(tolua_S,"Ogre::DataStreamPtr");
 tolua_usertype(tolua_S,"std::vector<EmberOgre::Model::SubEntityDefinition*>");
 tolua_usertype(tolua_S,"EmberOgre::Terrain::MapView");
 tolua_usertype(tolua_S,"sigc::signal<void,Ogre::Camera&>");
 tolua_usertype(tolua_S,"std::vector<EmberOgre::Model::AttachPointDefinition>");
 tolua_usertype(tolua_S,"EmberOgre::Environment::ISky");
 tolua_usertype(tolua_S,"EmberOgre::Gui::IconBase");
 tolua_usertype(tolua_S,"std::map<int,EmberOgre::Terrain::TerrainGenerator::TerrainPagecolumn>");
 tolua_usertype(tolua_S,"EmberOgre::EntityRecipeManager");
 tolua_usertype(tolua_S,"EmberOgre::GUIAdaptersStore::value_type");
 tolua_usertype(tolua_S,"EmberOgre::GUIAdaptersStore");
 tolua_usertype(tolua_S,"EmberOgre::EntityRecipePtr");
 tolua_usertype(tolua_S,"std::map<std::string,EmberOgre::Model::ViewDefinition*>");
 tolua_usertype(tolua_S,"Eris::TypeService");
 tolua_usertype(tolua_S,"Ogre::ManualResourceLoader");
 tolua_usertype(tolua_S,"Ogre::ResourceHandle");
 tolua_usertype(tolua_S,"Ogre::String");
 tolua_usertype(tolua_S,"EmberOgre::Model::SubEntityDefinition");
 tolua_usertype(tolua_S,"EmberOgre::GUIAdapter");
 tolua_usertype(tolua_S,"EmberOgre::Gui::OverlayCompassImpl");
 tolua_usertype(tolua_S,"Atlas::Message::MapType");
 tolua_usertype(tolua_S,"EmberOgre::Model::ModelDefinitionAtlasComposer");
 tolua_usertype(tolua_S,"Ogre::ResourceManager");
 tolua_usertype(tolua_S,"EmberOgre::Model::ModelDefinitionManager");
 tolua_usertype(tolua_S,"EmberOgre::AttributeObserver");
 tolua_usertype(tolua_S,"EmberOgre::EntityWorldPickListener");
 tolua_usertype(tolua_S,"Ogre::MovableObject");
 tolua_usertype(tolua_S,"EmberOgre::TerrainPosition");
 tolua_usertype(tolua_S,"EmberOgre::Terrain::TerrainEditor");
 tolua_usertype(tolua_S,"Mercator::Area");
 tolua_usertype(tolua_S,"EmberOgre::EmberEntity");
 tolua_usertype(tolua_S,"EmberOgre::Environment::Foliage");
 tolua_usertype(tolua_S,"EmberOgre::Gui::ListHolder");
 tolua_usertype(tolua_S,"EmberOgre::Gui::ModelRenderer");
 tolua_usertype(tolua_S,"EmberOgre::Gui::OgreEntityRenderer");
 tolua_usertype(tolua_S,"EmberOgre::Gui::MovableObjectRenderer");
 tolua_usertype(tolua_S,"EmberOgre::Terrain::TerrainPageSurfaceLayer");
 tolua_usertype(tolua_S,"sigc::signal<void,EmberOgre::Terrain::TerrainPage&>");
 tolua_usertype(tolua_S,"sigc::signal<void,EmberOgre::Jesus*>");
 tolua_usertype(tolua_S,"EmberOgre::Terrain::TerrainGenerator");
 tolua_usertype(tolua_S,"WFMath::Vector<3>");
 tolua_usertype(tolua_S,"std::vector<EmberOgre::Model::PartDefinition*>");
 tolua_usertype(tolua_S,"WFMath::Vector<2>");
 tolua_usertype(tolua_S,"EmberOgre::EntityPickResult");
 tolua_usertype(tolua_S,"CEGUI::PushButton");
 tolua_usertype(tolua_S,"EmberOgre::Model::ModelDefnPtr");
 tolua_usertype(tolua_S,"EmberOgre::Gui::Vector3Adapter");
 tolua_usertype(tolua_S,"CEGUI::DragContainer");
 tolua_usertype(tolua_S,"EmberOgre::Model::PartDefinition");
 tolua_usertype(tolua_S,"EmberOgre::Model::ActionDefinition");
 tolua_usertype(tolua_S,"Ogre::Resource");
 tolua_usertype(tolua_S,"sigc::signal<void,EmberOgre::AvatarController&>");
 tolua_usertype(tolua_S,"EmberOgre::GUIManager");
 tolua_usertype(tolua_S,"CEGUI::Listbox");
 tolua_usertype(tolua_S,"EmberOgre::Model::SubModelDefinition");
 tolua_usertype(tolua_S,"Mercator::Shader");
 tolua_usertype(tolua_S,"Ogre::RenderTexture");
 tolua_usertype(tolua_S,"CEGUI::ListboxTextItem");
 tolua_usertype(tolua_S,"EmberOgre::MotionManager");
 tolua_usertype(tolua_S,"EmberOgre::Model::SubModel");
 tolua_usertype(tolua_S,"EmberOgre::EmberOgre");
 tolua_usertype(tolua_S,"EmberOgre::Gui::EntityIconDragDropTarget");
 tolua_usertype(tolua_S,"Ogre::Camera");
 tolua_usertype(tolua_S,"CEGUI::UVector2");
 tolua_usertype(tolua_S,"EmberOgre::Model::ViewDefinition");
 tolua_usertype(tolua_S,"EmberOgre::Terrain::TerrainArea");
 tolua_usertype(tolua_S,"EmberOgre::Gui::Icons::IconManager");
 tolua_usertype(tolua_S,"EmberOgre::AvatarControllerMovement");
 tolua_usertype(tolua_S,"Ogre::Real");
 tolua_usertype(tolua_S,"Eris::TypeInfo");
 tolua_usertype(tolua_S,"EmberOgre::Gui::EntityIcon");
 tolua_usertype(tolua_S,"EmberOgre::Environment::IWater");
 tolua_usertype(tolua_S,"EmberOgre::MotionManager::MotionManagerInfo");
 tolua_usertype(tolua_S,"Ogre::Quaternion");
 tolua_usertype(tolua_S,"EmberOgre::EntityMoveManager");
 tolua_usertype(tolua_S,"sigc::signal<void>");
 tolua_usertype(tolua_S,"Ogre::MaterialPtr");
 tolua_usertype(tolua_S,"EmberOgre::Gui::EntityCreator");
 tolua_usertype(tolua_S,"EmberOgre::Gui::AssetsManager");
 tolua_usertype(tolua_S,"Ogre::TexturePtr");
 tolua_usertype(tolua_S,"std::ostream");
 tolua_usertype(tolua_S,"Ogre::ColourValue");
 tolua_usertype(tolua_S,"Ogre::NameValuePairList");
 tolua_usertype(tolua_S,"EmberOgre::Environment::Environment");
 tolua_usertype(tolua_S,"EmberOgre::Gui::StackableContainer");
 tolua_usertype(tolua_S,"EmberOgre::Gui::ColouredTreeItem");
 tolua_usertype(tolua_S,"EmberOgre::WorldEmberEntity");
 tolua_usertype(tolua_S,"EmberOgre::Terrain::TerrainEditBasePointMovement");
 tolua_usertype(tolua_S,"EmberOgre::Terrain::TerrainEditAction");
 tolua_usertype(tolua_S,"EmberOgre::AvatarMovementState");
 tolua_usertype(tolua_S,"EmberOgre::Gui::ColouredListItem");
 tolua_usertype(tolua_S,"Ogre::uint32");
 tolua_usertype(tolua_S,"EmberOgre::Gui::CEGUICompassImpl");
}

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
   Ogre::Camera& tolua_ret = (Ogre::Camera&)  self->getCamera();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"Ogre::Camera");
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
   Ogre::Camera& tolua_ret = (Ogre::Camera&)  self->getCamera();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"Ogre::Camera");
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
  tolua_pushusertype(tolua_S,(void*)&self->MovedCamera,"sigc::signal<void,Ogre::Camera&>");
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
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,Ogre::Camera&>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->MovedCamera = *((sigc::signal<void,Ogre::Camera&>*)  tolua_tousertype(tolua_S,2,0))
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
     !tolua_isusertype(tolua_S,3,"Ogre::Vector2",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::AvatarCamera* self = (EmberOgre::AvatarCamera*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector3* worldPos = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
  Ogre::Vector2* screenPos = ((Ogre::Vector2*)  tolua_tousertype(tolua_S,3,0));
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

/* method: getRootNode of class  EmberOgre::AvatarCamera */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_AvatarCamera_getRootNode00
static int tolua_EmberOgre_EmberOgre_AvatarCamera_getRootNode00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRootNode'",NULL);
#endif
  {
   Ogre::SceneNode* tolua_ret = (Ogre::SceneNode*)  self->getRootNode();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::SceneNode");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRootNode'.",&tolua_err);
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

/* method: setClientVisible of class  EmberOgre::EmberEntity */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EmberEntity_setClientVisible00
static int tolua_EmberOgre_EmberOgre_EmberEntity_setClientVisible00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setClientVisible'",NULL);
#endif
  {
   self->setClientVisible(visible);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setClientVisible'.",&tolua_err);
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

/* method: getActions of class  EmberOgre::EmberEntity */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EmberEntity_getActions00
static int tolua_EmberOgre_EmberOgre_EmberEntity_getActions00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getActions'",NULL);
#endif
  {
   std::vector<std::string> tolua_ret = (std::vector<std::string>)  self->getActions();
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
 tolua_error(tolua_S,"#ferror in function 'getActions'.",&tolua_err);
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

/* method: dumpAttributes of class  EmberOgre::EmberEntity */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EmberEntity_dumpAttributes00
static int tolua_EmberOgre_EmberOgre_EmberEntity_dumpAttributes00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::EmberEntity",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"std::iostream",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"std::ostream",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::EmberEntity* self = (const EmberOgre::EmberEntity*)  tolua_tousertype(tolua_S,1,0);
  std::iostream* outstream = ((std::iostream*)  tolua_tousertype(tolua_S,2,0));
  std::ostream* logOutstream = ((std::ostream*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'dumpAttributes'",NULL);
#endif
  {
   self->dumpAttributes(*outstream,*logOutstream);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'dumpAttributes'.",&tolua_err);
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
   EmberOgre::Terrain::TerrainArea* tolua_ret = (EmberOgre::Terrain::TerrainArea*)  new EmberOgre::Terrain::TerrainArea(*entity);
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
   EmberOgre::Terrain::TerrainArea* tolua_ret = (EmberOgre::Terrain::TerrainArea*)  new EmberOgre::Terrain::TerrainArea(*entity);
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

/* method: getName of class  EmberOgre::Terrain::TerrainLayerDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinition_getName00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinition_getName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Terrain::TerrainLayerDefinition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Terrain::TerrainLayerDefinition* self = (const EmberOgre::Terrain::TerrainLayerDefinition*)  tolua_tousertype(tolua_S,1,0);
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

/* method: getAreaId of class  EmberOgre::Terrain::TerrainLayerDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinition_getAreaId00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinition_getAreaId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Terrain::TerrainLayerDefinition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Terrain::TerrainLayerDefinition* self = (const EmberOgre::Terrain::TerrainLayerDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAreaId'",NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->getAreaId();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAreaId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getDiffuseTextureName of class  EmberOgre::Terrain::TerrainLayerDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinition_getDiffuseTextureName00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinition_getDiffuseTextureName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Terrain::TerrainLayerDefinition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Terrain::TerrainLayerDefinition* self = (const EmberOgre::Terrain::TerrainLayerDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getDiffuseTextureName'",NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getDiffuseTextureName();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getDiffuseTextureName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getNormalMapTextureName of class  EmberOgre::Terrain::TerrainLayerDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinition_getNormalMapTextureName00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinition_getNormalMapTextureName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Terrain::TerrainLayerDefinition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Terrain::TerrainLayerDefinition* self = (const EmberOgre::Terrain::TerrainLayerDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getNormalMapTextureName'",NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getNormalMapTextureName();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getNormalMapTextureName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getShaderName of class  EmberOgre::Terrain::TerrainLayerDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinition_getShaderName00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinition_getShaderName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Terrain::TerrainLayerDefinition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Terrain::TerrainLayerDefinition* self = (const EmberOgre::Terrain::TerrainLayerDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getShaderName'",NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getShaderName();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getShaderName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTileSize of class  EmberOgre::Terrain::TerrainLayerDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinition_getTileSize00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinition_getTileSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Terrain::TerrainLayerDefinition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Terrain::TerrainLayerDefinition* self = (const EmberOgre::Terrain::TerrainLayerDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTileSize'",NULL);
#endif
  {
   float tolua_ret = (float)  self->getTileSize();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTileSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::Terrain::TerrainDefPoint */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainDefPoint_new00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainDefPoint_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Terrain::TerrainDefPoint",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float y = ((float)  tolua_tonumber(tolua_S,3,0));
  float terrainHeight = ((float)  tolua_tonumber(tolua_S,4,0));
  {
   EmberOgre::Terrain::TerrainDefPoint* tolua_ret = (EmberOgre::Terrain::TerrainDefPoint*)  new EmberOgre::Terrain::TerrainDefPoint(x,y,terrainHeight);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Terrain::TerrainDefPoint");
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

/* method: new_local of class  EmberOgre::Terrain::TerrainDefPoint */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainDefPoint_new00_local
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainDefPoint_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Terrain::TerrainDefPoint",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float y = ((float)  tolua_tonumber(tolua_S,3,0));
  float terrainHeight = ((float)  tolua_tonumber(tolua_S,4,0));
  {
   EmberOgre::Terrain::TerrainDefPoint* tolua_ret = (EmberOgre::Terrain::TerrainDefPoint*)  new EmberOgre::Terrain::TerrainDefPoint(x,y,terrainHeight);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Terrain::TerrainDefPoint");
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

/* method: getPosition of class  EmberOgre::Terrain::TerrainDefPoint */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainDefPoint_getPosition00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainDefPoint_getPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Terrain::TerrainDefPoint",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Terrain::TerrainDefPoint* self = (const EmberOgre::Terrain::TerrainDefPoint*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPosition'",NULL);
#endif
  {
   const WFMath::Point<2>& tolua_ret = (const WFMath::Point<2>&)  self->getPosition();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const WFMath::Point<2>");
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

/* method: getHeight of class  EmberOgre::Terrain::TerrainDefPoint */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainDefPoint_getHeight00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainDefPoint_getHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Terrain::TerrainDefPoint",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Terrain::TerrainDefPoint* self = (const EmberOgre::Terrain::TerrainDefPoint*)  tolua_tousertype(tolua_S,1,0);
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

/* method: updateTerrain of class  EmberOgre::Terrain::TerrainGenerator */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_updateTerrain00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_updateTerrain00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Terrain::TerrainGenerator",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const std::vector<EmberOgre::Terrain::TerrainDefPoint>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Terrain::TerrainGenerator* self = (EmberOgre::Terrain::TerrainGenerator*)  tolua_tousertype(tolua_S,1,0);
  const std::vector<EmberOgre::Terrain::TerrainDefPoint>* terrainIndexPoints = ((const std::vector<EmberOgre::Terrain::TerrainDefPoint>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'updateTerrain'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->updateTerrain(*terrainIndexPoints);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'updateTerrain'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isValidTerrainAt of class  EmberOgre::Terrain::TerrainGenerator */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_isValidTerrainAt00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_isValidTerrainAt00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Terrain::TerrainGenerator",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const WFMath::Point<2>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Terrain::TerrainGenerator* self = (EmberOgre::Terrain::TerrainGenerator*)  tolua_tousertype(tolua_S,1,0);
  const WFMath::Point<2>* pos = ((const WFMath::Point<2>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isValidTerrainAt'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isValidTerrainAt(*pos);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isValidTerrainAt'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTerrain of class  EmberOgre::Terrain::TerrainGenerator */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_getTerrain00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_getTerrain00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Terrain::TerrainGenerator",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Terrain::TerrainGenerator* self = (EmberOgre::Terrain::TerrainGenerator*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTerrain'",NULL);
#endif
  {
   Mercator::Terrain& tolua_ret = (Mercator::Terrain&)  self->getTerrain();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"Mercator::Terrain");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTerrain'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMax of class  EmberOgre::Terrain::TerrainGenerator */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_getMax00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_getMax00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Terrain::TerrainGenerator",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Terrain::TerrainGenerator* self = (const EmberOgre::Terrain::TerrainGenerator*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMax'",NULL);
#endif
  {
   const WFMath::Point<2> tolua_ret = (const WFMath::Point<2>)  self->getMax();
   {
#ifdef __cplusplus
    void* tolua_obj = new WFMath::Point<2>(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"const WFMath::Point<2>");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(const WFMath::Point<2>));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"const WFMath::Point<2>");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMax'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMin of class  EmberOgre::Terrain::TerrainGenerator */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_getMin00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_getMin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Terrain::TerrainGenerator",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Terrain::TerrainGenerator* self = (const EmberOgre::Terrain::TerrainGenerator*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMin'",NULL);
#endif
  {
   const WFMath::Point<2> tolua_ret = (const WFMath::Point<2>)  self->getMin();
   {
#ifdef __cplusplus
    void* tolua_obj = new WFMath::Point<2>(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"const WFMath::Point<2>");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(const WFMath::Point<2>));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"const WFMath::Point<2>");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPageMetersSize of class  EmberOgre::Terrain::TerrainGenerator */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_getPageMetersSize00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_getPageMetersSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Terrain::TerrainGenerator",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Terrain::TerrainGenerator* self = (EmberOgre::Terrain::TerrainGenerator*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPageMetersSize'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getPageMetersSize();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPageMetersSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPageIndexSize of class  EmberOgre::Terrain::TerrainGenerator */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_getPageIndexSize00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_getPageIndexSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Terrain::TerrainGenerator",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Terrain::TerrainGenerator* self = (EmberOgre::Terrain::TerrainGenerator*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPageIndexSize'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getPageIndexSize();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPageIndexSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addArea of class  EmberOgre::Terrain::TerrainGenerator */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_addArea00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_addArea00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Terrain::TerrainGenerator",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"EmberOgre::Terrain::TerrainArea",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Terrain::TerrainGenerator* self = (EmberOgre::Terrain::TerrainGenerator*)  tolua_tousertype(tolua_S,1,0);
  EmberOgre::Terrain::TerrainArea* terrainArea = ((EmberOgre::Terrain::TerrainArea*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addArea'",NULL);
#endif
  {
   self->addArea(terrainArea);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addArea'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addTerrainMod of class  EmberOgre::Terrain::TerrainGenerator */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_addTerrainMod00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_addTerrainMod00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Terrain::TerrainGenerator",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"EmberOgre::Terrain::TerrainMod",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Terrain::TerrainGenerator* self = (EmberOgre::Terrain::TerrainGenerator*)  tolua_tousertype(tolua_S,1,0);
  EmberOgre::Terrain::TerrainMod* terrainMod = ((EmberOgre::Terrain::TerrainMod*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addTerrainMod'",NULL);
#endif
  {
   self->addTerrainMod(terrainMod);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addTerrainMod'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTerrainPageAtIndex of class  EmberOgre::Terrain::TerrainGenerator */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_getTerrainPageAtIndex00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_getTerrainPageAtIndex00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Terrain::TerrainGenerator",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector2",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Terrain::TerrainGenerator* self = (EmberOgre::Terrain::TerrainGenerator*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector2* ogreIndexPosition = ((const Ogre::Vector2*)  tolua_tousertype(tolua_S,2,0));
  bool createIfMissing = ((bool)  tolua_toboolean(tolua_S,3,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTerrainPageAtIndex'",NULL);
#endif
  {
   EmberOgre::Terrain::TerrainPage* tolua_ret = (EmberOgre::Terrain::TerrainPage*)  self->getTerrainPageAtIndex(*ogreIndexPosition,createIfMissing);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Terrain::TerrainPage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTerrainPageAtIndex'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTerrainPageAtPosition of class  EmberOgre::Terrain::TerrainGenerator */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_getTerrainPageAtPosition00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_getTerrainPageAtPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Terrain::TerrainGenerator",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const WFMath::Point<2>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Terrain::TerrainGenerator* self = (EmberOgre::Terrain::TerrainGenerator*)  tolua_tousertype(tolua_S,1,0);
  const WFMath::Point<2>* worldPosition = ((const WFMath::Point<2>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTerrainPageAtPosition'",NULL);
#endif
  {
   EmberOgre::Terrain::TerrainPage* tolua_ret = (EmberOgre::Terrain::TerrainPage*)  self->getTerrainPageAtPosition(*worldPosition);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Terrain::TerrainPage");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTerrainPageAtPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTerrainInfo of class  EmberOgre::Terrain::TerrainGenerator */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_getTerrainInfo00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_getTerrainInfo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Terrain::TerrainGenerator",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Terrain::TerrainGenerator* self = (const EmberOgre::Terrain::TerrainGenerator*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTerrainInfo'",NULL);
#endif
  {
   const EmberOgre::Terrain::TerrainInfo& tolua_ret = (const EmberOgre::Terrain::TerrainInfo&)  self->getTerrainInfo();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const EmberOgre::Terrain::TerrainInfo");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTerrainInfo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventWorldSizeChanged of class  EmberOgre::Terrain::TerrainGenerator */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__Terrain__TerrainGenerator_EventWorldSizeChanged
static int tolua_get_EmberOgre__Terrain__TerrainGenerator_EventWorldSizeChanged(lua_State* tolua_S)
{
  EmberOgre::Terrain::TerrainGenerator* self = (EmberOgre::Terrain::TerrainGenerator*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventWorldSizeChanged'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventWorldSizeChanged,"sigc::signal<void>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventWorldSizeChanged of class  EmberOgre::Terrain::TerrainGenerator */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__Terrain__TerrainGenerator_EventWorldSizeChanged
static int tolua_set_EmberOgre__Terrain__TerrainGenerator_EventWorldSizeChanged(lua_State* tolua_S)
{
  EmberOgre::Terrain::TerrainGenerator* self = (EmberOgre::Terrain::TerrainGenerator*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventWorldSizeChanged'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventWorldSizeChanged = *((sigc::signal<void>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAdapter of class  EmberOgre::Terrain::TerrainGenerator */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_getAdapter00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_getAdapter00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Terrain::TerrainGenerator",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Terrain::TerrainGenerator* self = (const EmberOgre::Terrain::TerrainGenerator*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAdapter'",NULL);
#endif
  {
   EmberOgre::Terrain::ISceneManagerAdapter* tolua_ret = (EmberOgre::Terrain::ISceneManagerAdapter*)  self->getAdapter();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Terrain::ISceneManagerAdapter");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAdapter'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTerrainPages of class  EmberOgre::Terrain::TerrainGenerator */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_getTerrainPages00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_getTerrainPages00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Terrain::TerrainGenerator",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Terrain::TerrainGenerator* self = (const EmberOgre::Terrain::TerrainGenerator*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTerrainPages'",NULL);
#endif
  {
    const std::map<int,EmberOgre::Terrain::TerrainGenerator::TerrainPagecolumn>& tolua_ret = (  const std::map<int,EmberOgre::Terrain::TerrainGenerator::TerrainPagecolumn>&)  self->getTerrainPages();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const std::map<int,EmberOgre::Terrain::TerrainGenerator::TerrainPagecolumn>");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTerrainPages'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createShader of class  EmberOgre::Terrain::TerrainGenerator */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_createShader00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_createShader00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Terrain::TerrainGenerator",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const EmberOgre::Terrain::TerrainLayerDefinition",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"Mercator::Shader",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Terrain::TerrainGenerator* self = (EmberOgre::Terrain::TerrainGenerator*)  tolua_tousertype(tolua_S,1,0);
  const EmberOgre::Terrain::TerrainLayerDefinition* layerDef = ((const EmberOgre::Terrain::TerrainLayerDefinition*)  tolua_tousertype(tolua_S,2,0));
  Mercator::Shader* mercatorShader = ((Mercator::Shader*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createShader'",NULL);
#endif
  {
   EmberOgre::Terrain::TerrainShader* tolua_ret = (EmberOgre::Terrain::TerrainShader*)  self->createShader(layerDef,mercatorShader);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Terrain::TerrainShader");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createShader'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: updateShadows of class  EmberOgre::Terrain::TerrainGenerator */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_updateShadows00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_updateShadows00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Terrain::TerrainGenerator",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Terrain::TerrainGenerator* self = (EmberOgre::Terrain::TerrainGenerator*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'updateShadows'",NULL);
#endif
  {
   self->updateShadows();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'updateShadows'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: UpdateShadows of class  EmberOgre::Terrain::TerrainGenerator */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__Terrain__TerrainGenerator_UpdateShadows
static int tolua_get_EmberOgre__Terrain__TerrainGenerator_UpdateShadows(lua_State* tolua_S)
{
  EmberOgre::Terrain::TerrainGenerator* self = (EmberOgre::Terrain::TerrainGenerator*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'UpdateShadows'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->UpdateShadows,"const Ember::ConsoleCommandWrapper");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getShadowColourAt of class  EmberOgre::Terrain::TerrainGenerator */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_getShadowColourAt00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_getShadowColourAt00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Terrain::TerrainGenerator",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector2",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"Ogre::uint32",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Terrain::TerrainGenerator* self = (EmberOgre::Terrain::TerrainGenerator*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector2* position = ((const Ogre::Vector2*)  tolua_tousertype(tolua_S,2,0));
  Ogre::uint32* colour = ((Ogre::uint32*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getShadowColourAt'",NULL);
#endif
  {
   self->getShadowColourAt(*position,*colour);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getShadowColourAt'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getShadowColourAt of class  EmberOgre::Terrain::TerrainGenerator */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_getShadowColourAt01
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_getShadowColourAt01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Terrain::TerrainGenerator",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector2",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"Ogre::ColourValue",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  EmberOgre::Terrain::TerrainGenerator* self = (EmberOgre::Terrain::TerrainGenerator*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector2* position = ((const Ogre::Vector2*)  tolua_tousertype(tolua_S,2,0));
  Ogre::ColourValue* colour = ((Ogre::ColourValue*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getShadowColourAt'",NULL);
#endif
  {
   self->getShadowColourAt(*position,*colour);
  }
 }
 return 0;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_getShadowColourAt00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventLayerUpdated of class  EmberOgre::Terrain::TerrainGenerator */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__Terrain__TerrainGenerator_EventLayerUpdated
static int tolua_get_EmberOgre__Terrain__TerrainGenerator_EventLayerUpdated(lua_State* tolua_S)
{
  EmberOgre::Terrain::TerrainGenerator* self = (EmberOgre::Terrain::TerrainGenerator*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventLayerUpdated'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventLayerUpdated,"sigc::signal<void,EmberOgre::Terrain::TerrainShader*,EmberOgre::Terrain::TerrainGenerator::AreaStore*>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventLayerUpdated of class  EmberOgre::Terrain::TerrainGenerator */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__Terrain__TerrainGenerator_EventLayerUpdated
static int tolua_set_EmberOgre__Terrain__TerrainGenerator_EventLayerUpdated(lua_State* tolua_S)
{
  EmberOgre::Terrain::TerrainGenerator* self = (EmberOgre::Terrain::TerrainGenerator*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventLayerUpdated'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,EmberOgre::Terrain::TerrainShader*,EmberOgre::Terrain::TerrainGenerator::AreaStore*>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventLayerUpdated = *((sigc::signal<void,EmberOgre::Terrain::TerrainShader*,EmberOgre::Terrain::TerrainGenerator::AreaStore*>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventShaderCreated of class  EmberOgre::Terrain::TerrainGenerator */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__Terrain__TerrainGenerator_EventShaderCreated
static int tolua_get_EmberOgre__Terrain__TerrainGenerator_EventShaderCreated(lua_State* tolua_S)
{
  EmberOgre::Terrain::TerrainGenerator* self = (EmberOgre::Terrain::TerrainGenerator*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventShaderCreated'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventShaderCreated,"sigc::signal<void,EmberOgre::Terrain::TerrainShader*>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventShaderCreated of class  EmberOgre::Terrain::TerrainGenerator */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__Terrain__TerrainGenerator_EventShaderCreated
static int tolua_set_EmberOgre__Terrain__TerrainGenerator_EventShaderCreated(lua_State* tolua_S)
{
  EmberOgre::Terrain::TerrainGenerator* self = (EmberOgre::Terrain::TerrainGenerator*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventShaderCreated'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,EmberOgre::Terrain::TerrainShader*>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventShaderCreated = *((sigc::signal<void,EmberOgre::Terrain::TerrainShader*>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventTerrainPageGeometryUpdated of class  EmberOgre::Terrain::TerrainGenerator */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__Terrain__TerrainGenerator_EventTerrainPageGeometryUpdated
static int tolua_get_EmberOgre__Terrain__TerrainGenerator_EventTerrainPageGeometryUpdated(lua_State* tolua_S)
{
  EmberOgre::Terrain::TerrainGenerator* self = (EmberOgre::Terrain::TerrainGenerator*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventTerrainPageGeometryUpdated'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventTerrainPageGeometryUpdated,"sigc::signal<void,EmberOgre::Terrain::TerrainPage&>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventTerrainPageGeometryUpdated of class  EmberOgre::Terrain::TerrainGenerator */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__Terrain__TerrainGenerator_EventTerrainPageGeometryUpdated
static int tolua_set_EmberOgre__Terrain__TerrainGenerator_EventTerrainPageGeometryUpdated(lua_State* tolua_S)
{
  EmberOgre::Terrain::TerrainGenerator* self = (EmberOgre::Terrain::TerrainGenerator*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventTerrainPageGeometryUpdated'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,EmberOgre::Terrain::TerrainPage&>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventTerrainPageGeometryUpdated = *((sigc::signal<void,EmberOgre::Terrain::TerrainPage&>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getFoliageBatchSize of class  EmberOgre::Terrain::TerrainGenerator */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_getFoliageBatchSize00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_getFoliageBatchSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Terrain::TerrainGenerator",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Terrain::TerrainGenerator* self = (const EmberOgre::Terrain::TerrainGenerator*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFoliageBatchSize'",NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->getFoliageBatchSize();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getFoliageBatchSize'.",&tolua_err);
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

/* method: getRadius of class  EmberOgre::Terrain::TerrainEditor */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_getRadius00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_getRadius00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRadius'",NULL);
#endif
  {
   float tolua_ret = (float)  self->getRadius();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRadius'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setRadius of class  EmberOgre::Terrain::TerrainEditor */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_setRadius00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_setRadius00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Terrain::TerrainEditor",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Terrain::TerrainEditor* self = (EmberOgre::Terrain::TerrainEditor*)  tolua_tousertype(tolua_S,1,0);
  float radiusInMeters = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRadius'",NULL);
#endif
  {
   self->setRadius(radiusInMeters);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setRadius'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getFalloff of class  EmberOgre::Terrain::TerrainEditor */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_getFalloff00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_getFalloff00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFalloff'",NULL);
#endif
  {
   float tolua_ret = (float)  self->getFalloff();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getFalloff'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: seFalloff of class  EmberOgre::Terrain::TerrainEditor */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_seFalloff00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_seFalloff00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Terrain::TerrainEditor",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Terrain::TerrainEditor* self = (EmberOgre::Terrain::TerrainEditor*)  tolua_tousertype(tolua_S,1,0);
  float falloff = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'seFalloff'",NULL);
#endif
  {
   self->seFalloff(falloff);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'seFalloff'.",&tolua_err);
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

/* get function: EventFirstTimeShown of class  EmberOgre::Gui::Widget */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__Gui__Widget_EventFirstTimeShown
static int tolua_get_EmberOgre__Gui__Widget_EventFirstTimeShown(lua_State* tolua_S)
{
  EmberOgre::Gui::Widget* self = (EmberOgre::Gui::Widget*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventFirstTimeShown'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventFirstTimeShown,"sigc::signal<void>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventFirstTimeShown of class  EmberOgre::Gui::Widget */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__Gui__Widget_EventFirstTimeShown
static int tolua_set_EmberOgre__Gui__Widget_EventFirstTimeShown(lua_State* tolua_S)
{
  EmberOgre::Gui::Widget* self = (EmberOgre::Gui::Widget*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventFirstTimeShown'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventFirstTimeShown = *((sigc::signal<void>*)  tolua_tousertype(tolua_S,2,0))
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

/* method: new of class  EmberOgre::Gui::IconBase */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_IconBase_new01
static int tolua_EmberOgre_EmberOgre_Gui_IconBase_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::IconBase",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const CEGUI::Image",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"const CEGUI::Image",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"const CEGUI::Image",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,6,"const CEGUI::Image",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,7,"CEGUI::UVector2",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const CEGUI::Image* background = ((const CEGUI::Image*)  tolua_tousertype(tolua_S,3,0));
  const CEGUI::Image* foreground = ((const CEGUI::Image*)  tolua_tousertype(tolua_S,4,0));
  const CEGUI::Image* borderInactive = ((const CEGUI::Image*)  tolua_tousertype(tolua_S,5,0));
  const CEGUI::Image* borderActive = ((const CEGUI::Image*)  tolua_tousertype(tolua_S,6,0));
  CEGUI::UVector2 size = *((CEGUI::UVector2*)  tolua_tousertype(tolua_S,7,0));
  {
   EmberOgre::Gui::IconBase* tolua_ret = (EmberOgre::Gui::IconBase*)  new EmberOgre::Gui::IconBase(name,background,foreground,borderInactive,borderActive,size);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::IconBase");
   tolua_pushcppstring(tolua_S,(const char*)name);
  }
 }
 return 2;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_Gui_IconBase_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::Gui::IconBase */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_IconBase_new01_local
static int tolua_EmberOgre_EmberOgre_Gui_IconBase_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::IconBase",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const CEGUI::Image",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"const CEGUI::Image",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"const CEGUI::Image",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,6,"const CEGUI::Image",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,7,"CEGUI::UVector2",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const CEGUI::Image* background = ((const CEGUI::Image*)  tolua_tousertype(tolua_S,3,0));
  const CEGUI::Image* foreground = ((const CEGUI::Image*)  tolua_tousertype(tolua_S,4,0));
  const CEGUI::Image* borderInactive = ((const CEGUI::Image*)  tolua_tousertype(tolua_S,5,0));
  const CEGUI::Image* borderActive = ((const CEGUI::Image*)  tolua_tousertype(tolua_S,6,0));
  CEGUI::UVector2 size = *((CEGUI::UVector2*)  tolua_tousertype(tolua_S,7,0));
  {
   EmberOgre::Gui::IconBase* tolua_ret = (EmberOgre::Gui::IconBase*)  new EmberOgre::Gui::IconBase(name,background,foreground,borderInactive,borderActive,size);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Gui::IconBase");
   tolua_pushcppstring(tolua_S,(const char*)name);
  }
 }
 return 2;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_Gui_IconBase_new00_local(tolua_S);
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

/* method: create of class  EmberOgre::Gui::ColouredTreeItem */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_ColouredTreeItem_new00
static int tolua_EmberOgre_EmberOgre_Gui_ColouredTreeItem_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::ColouredTreeItem",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::string text = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  {
   CEGUI::TreeItem* tolua_ret = (CEGUI::TreeItem*)  EmberOgre::Gui::ColouredTreeItem::create(text);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::TreeItem");
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

/* method: create of class  EmberOgre::Gui::ColouredTreeItem */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_ColouredTreeItem_new01
static int tolua_EmberOgre_EmberOgre_Gui_ColouredTreeItem_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::ColouredTreeItem",0,&tolua_err) ||
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
   CEGUI::TreeItem* tolua_ret = (CEGUI::TreeItem*)  EmberOgre::Gui::ColouredTreeItem::create(text,item_id);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::TreeItem");
   tolua_pushcppstring(tolua_S,(const char*)text);
  }
 }
 return 2;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_Gui_ColouredTreeItem_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: create of class  EmberOgre::Gui::ColouredTreeItem */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_ColouredTreeItem_new02
static int tolua_EmberOgre_EmberOgre_Gui_ColouredTreeItem_new02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::ColouredTreeItem",0,&tolua_err) ||
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
   CEGUI::TreeItem* tolua_ret = (CEGUI::TreeItem*)  EmberOgre::Gui::ColouredTreeItem::create(text,item_id,item_data);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::TreeItem");
   tolua_pushcppstring(tolua_S,(const char*)text);
  }
 }
 return 2;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_Gui_ColouredTreeItem_new01(tolua_S);
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

/* method: materialAsText of class  EmberOgre::Gui::AssetsManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_AssetsManager_materialAsText00
static int tolua_EmberOgre_EmberOgre_Gui_AssetsManager_materialAsText00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::AssetsManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::MaterialPtr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::AssetsManager* self = (EmberOgre::Gui::AssetsManager*)  tolua_tousertype(tolua_S,1,0);
  Ogre::MaterialPtr material = *((Ogre::MaterialPtr*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'materialAsText'",NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->materialAsText(material);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'materialAsText'.",&tolua_err);
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

/* method: refresh of class  EmberOgre::Gui::Compass */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_Compass_refresh00
static int tolua_EmberOgre_EmberOgre_Gui_Compass_refresh00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'refresh'",NULL);
#endif
  {
   self->refresh();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'refresh'.",&tolua_err);
 return 0;
#endif
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

/* method: new of class  EmberOgre::Gui::RenderedCompassImpl */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_RenderedCompassImpl_new00
static int tolua_EmberOgre_EmberOgre_Gui_RenderedCompassImpl_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::RenderedCompassImpl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   EmberOgre::Gui::RenderedCompassImpl* tolua_ret = (EmberOgre::Gui::RenderedCompassImpl*)  new EmberOgre::Gui::RenderedCompassImpl();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::RenderedCompassImpl");
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

/* method: new_local of class  EmberOgre::Gui::RenderedCompassImpl */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_RenderedCompassImpl_new00_local
static int tolua_EmberOgre_EmberOgre_Gui_RenderedCompassImpl_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::RenderedCompassImpl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   EmberOgre::Gui::RenderedCompassImpl* tolua_ret = (EmberOgre::Gui::RenderedCompassImpl*)  new EmberOgre::Gui::RenderedCompassImpl();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Gui::RenderedCompassImpl");
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

/* method: new of class  EmberOgre::Gui::RenderedCompassImpl */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_RenderedCompassImpl_new01
static int tolua_EmberOgre_EmberOgre_Gui_RenderedCompassImpl_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::RenderedCompassImpl",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  std::string compassMaterialName = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  {
   EmberOgre::Gui::RenderedCompassImpl* tolua_ret = (EmberOgre::Gui::RenderedCompassImpl*)  new EmberOgre::Gui::RenderedCompassImpl(compassMaterialName);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::RenderedCompassImpl");
  }
 }
 return 1;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_Gui_RenderedCompassImpl_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::Gui::RenderedCompassImpl */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_RenderedCompassImpl_new01_local
static int tolua_EmberOgre_EmberOgre_Gui_RenderedCompassImpl_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::RenderedCompassImpl",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  std::string compassMaterialName = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  {
   EmberOgre::Gui::RenderedCompassImpl* tolua_ret = (EmberOgre::Gui::RenderedCompassImpl*)  new EmberOgre::Gui::RenderedCompassImpl(compassMaterialName);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Gui::RenderedCompassImpl");
  }
 }
 return 1;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_Gui_RenderedCompassImpl_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::Gui::RenderedCompassImpl */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_RenderedCompassImpl_new02
static int tolua_EmberOgre_EmberOgre_Gui_RenderedCompassImpl_new02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::RenderedCompassImpl",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  std::string compassMaterialName = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  std::string pointerMaterialName = ((std::string)  tolua_tocppstring(tolua_S,3,0));
  {
   EmberOgre::Gui::RenderedCompassImpl* tolua_ret = (EmberOgre::Gui::RenderedCompassImpl*)  new EmberOgre::Gui::RenderedCompassImpl(compassMaterialName,pointerMaterialName);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::RenderedCompassImpl");
  }
 }
 return 1;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_Gui_RenderedCompassImpl_new01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::Gui::RenderedCompassImpl */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_RenderedCompassImpl_new02_local
static int tolua_EmberOgre_EmberOgre_Gui_RenderedCompassImpl_new02_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::RenderedCompassImpl",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  std::string compassMaterialName = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  std::string pointerMaterialName = ((std::string)  tolua_tocppstring(tolua_S,3,0));
  {
   EmberOgre::Gui::RenderedCompassImpl* tolua_ret = (EmberOgre::Gui::RenderedCompassImpl*)  new EmberOgre::Gui::RenderedCompassImpl(compassMaterialName,pointerMaterialName);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Gui::RenderedCompassImpl");
  }
 }
 return 1;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_Gui_RenderedCompassImpl_new01_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  EmberOgre::Gui::RenderedCompassImpl */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_RenderedCompassImpl_delete00
static int tolua_EmberOgre_EmberOgre_Gui_RenderedCompassImpl_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::RenderedCompassImpl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::RenderedCompassImpl* self = (EmberOgre::Gui::RenderedCompassImpl*)  tolua_tousertype(tolua_S,1,0);
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

/* method: getTexture of class  EmberOgre::Gui::RenderedCompassImpl */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_RenderedCompassImpl_getTexture00
static int tolua_EmberOgre_EmberOgre_Gui_RenderedCompassImpl_getTexture00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Gui::RenderedCompassImpl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Gui::RenderedCompassImpl* self = (const EmberOgre::Gui::RenderedCompassImpl*)  tolua_tousertype(tolua_S,1,0);
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

/* method: getPointerTexture of class  EmberOgre::Gui::RenderedCompassImpl */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_RenderedCompassImpl_getPointerTexture00
static int tolua_EmberOgre_EmberOgre_Gui_RenderedCompassImpl_getPointerTexture00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Gui::RenderedCompassImpl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Gui::RenderedCompassImpl* self = (const EmberOgre::Gui::RenderedCompassImpl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPointerTexture'",NULL);
#endif
  {
   Ogre::TexturePtr tolua_ret = (Ogre::TexturePtr)  self->getPointerTexture();
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
 tolua_error(tolua_S,"#ferror in function 'getPointerTexture'.",&tolua_err);
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

/* method: new of class  EmberOgre::Gui::CompassThirdPersonCameraAnchor */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_CompassThirdPersonCameraAnchor_new00
static int tolua_EmberOgre_EmberOgre_Gui_CompassThirdPersonCameraAnchor_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::CompassThirdPersonCameraAnchor",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"EmberOgre::Gui::Compass",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"Ogre::Camera",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Compass* compass = ((EmberOgre::Gui::Compass*)  tolua_tousertype(tolua_S,2,0));
  Ogre::Camera* camera = ((Ogre::Camera*)  tolua_tousertype(tolua_S,3,0));
  Ogre::SceneNode* node = ((Ogre::SceneNode*)  tolua_tousertype(tolua_S,4,0));
  {
   EmberOgre::Gui::CompassThirdPersonCameraAnchor* tolua_ret = (EmberOgre::Gui::CompassThirdPersonCameraAnchor*)  new EmberOgre::Gui::CompassThirdPersonCameraAnchor(*compass,camera,node);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::CompassThirdPersonCameraAnchor");
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

/* method: new_local of class  EmberOgre::Gui::CompassThirdPersonCameraAnchor */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_CompassThirdPersonCameraAnchor_new00_local
static int tolua_EmberOgre_EmberOgre_Gui_CompassThirdPersonCameraAnchor_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::CompassThirdPersonCameraAnchor",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"EmberOgre::Gui::Compass",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"Ogre::Camera",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Compass* compass = ((EmberOgre::Gui::Compass*)  tolua_tousertype(tolua_S,2,0));
  Ogre::Camera* camera = ((Ogre::Camera*)  tolua_tousertype(tolua_S,3,0));
  Ogre::SceneNode* node = ((Ogre::SceneNode*)  tolua_tousertype(tolua_S,4,0));
  {
   EmberOgre::Gui::CompassThirdPersonCameraAnchor* tolua_ret = (EmberOgre::Gui::CompassThirdPersonCameraAnchor*)  new EmberOgre::Gui::CompassThirdPersonCameraAnchor(*compass,camera,node);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Gui::CompassThirdPersonCameraAnchor");
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

/* method: delete of class  EmberOgre::Gui::CompassThirdPersonCameraAnchor */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_CompassThirdPersonCameraAnchor_delete00
static int tolua_EmberOgre_EmberOgre_Gui_CompassThirdPersonCameraAnchor_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::CompassThirdPersonCameraAnchor",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::CompassThirdPersonCameraAnchor* self = (EmberOgre::Gui::CompassThirdPersonCameraAnchor*)  tolua_tousertype(tolua_S,1,0);
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

/* method: operator== of class  EmberOgre::Gui::EntityIcon */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_EntityIcon__eq00
static int tolua_EmberOgre_EmberOgre_Gui_EntityIcon__eq00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::EntityIcon",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const EmberOgre::Gui::EntityIcon",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::EntityIcon* self = (EmberOgre::Gui::EntityIcon*)  tolua_tousertype(tolua_S,1,0);
  const EmberOgre::Gui::EntityIcon* value = ((const EmberOgre::Gui::EntityIcon*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator=='",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->operator==(*value);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.eq'.",&tolua_err);
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

/* method: setDisplaySkeleton of class  EmberOgre::Model::Model */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_Model_setDisplaySkeleton00
static int tolua_EmberOgre_EmberOgre_Model_Model_setDisplaySkeleton00(lua_State* tolua_S)
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
  bool display = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setDisplaySkeleton'",NULL);
#endif
  {
   self->setDisplaySkeleton(display);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setDisplaySkeleton'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getDisplaySkeleton of class  EmberOgre::Model::Model */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_Model_getDisplaySkeleton00
static int tolua_EmberOgre_EmberOgre_Model_Model_getDisplaySkeleton00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getDisplaySkeleton'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->getDisplaySkeleton();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getDisplaySkeleton'.",&tolua_err);
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
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Model::ModelDefinition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Model::ModelDefinition* self = (const EmberOgre::Model::ModelDefinition*)  tolua_tousertype(tolua_S,1,0);
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

/* method: setShowContained of class  EmberOgre::Model::ModelDefinition */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_ModelDefinition_setShowContained00
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinition_setShowContained00(lua_State* tolua_S)
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
  bool show = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setShowContained'",NULL);
#endif
  {
   self->setShowContained(show);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setShowContained'.",&tolua_err);
 return 0;
#endif
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

/* method: toOgre<Ogre::Vector3 > of class  EmberOgre::Convert */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Convert_toOgre_Vector300
static int tolua_EmberOgre_EmberOgre_Convert_toOgre_Vector300(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Convert",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const WFMath::Point<2>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const WFMath::Point<2>* p = ((const WFMath::Point<2>*)  tolua_tousertype(tolua_S,2,0));
  {
   Ogre::Vector3 tolua_ret = (Ogre::Vector3)  EmberOgre::Convert::toOgre<Ogre::Vector3 >(*p);
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
 tolua_error(tolua_S,"#ferror in function 'toOgre_Vector3'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: toOgre<Ogre::Vector2 > of class  EmberOgre::Convert */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Convert_toOgre_Vector200
static int tolua_EmberOgre_EmberOgre_Convert_toOgre_Vector200(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Convert",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const WFMath::Point<2>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const WFMath::Point<2>* p = ((const WFMath::Point<2>*)  tolua_tousertype(tolua_S,2,0));
  {
   Ogre::Vector2 tolua_ret = (Ogre::Vector2)  EmberOgre::Convert::toOgre<Ogre::Vector2 >(*p);
   {
#ifdef __cplusplus
    void* tolua_obj = new Ogre::Vector2(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Vector2");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::Vector2));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Vector2");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'toOgre_Vector2'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: toOgre of class  EmberOgre::Convert */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Convert_toOgre00
static int tolua_EmberOgre_EmberOgre_Convert_toOgre00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Convert",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const WFMath::Vector<2>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const WFMath::Vector<2>* p = ((const WFMath::Vector<2>*)  tolua_tousertype(tolua_S,2,0));
  {
   Ogre::Vector2 tolua_ret = (Ogre::Vector2)  EmberOgre::Convert::toOgre(*p);
   {
#ifdef __cplusplus
    void* tolua_obj = new Ogre::Vector2(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Vector2");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::Vector2));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Vector2");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'toOgre'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: toWF<WFMath::Point<3> > of class  EmberOgre::Convert */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Convert_toWF_Point300
static int tolua_EmberOgre_EmberOgre_Convert_toWF_Point300(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Convert",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Vector3* p = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
  {
   WFMath::Point<3> tolua_ret = (WFMath::Point<3>)  EmberOgre::Convert::toWF<WFMath::Point<3> >(*p);
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
 tolua_error(tolua_S,"#ferror in function 'toWF_Point3'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: toWF<WFMath::Point<2> > of class  EmberOgre::Convert */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Convert_toWF_Point200
static int tolua_EmberOgre_EmberOgre_Convert_toWF_Point200(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Convert",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Vector3* p = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
  {
   WFMath::Point<2> tolua_ret = (WFMath::Point<2>)  EmberOgre::Convert::toWF<WFMath::Point<2> >(*p);
   {
#ifdef __cplusplus
    void* tolua_obj = new WFMath::Point<2>(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"WFMath::Point<2>");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(WFMath::Point<2>));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"WFMath::Point<2>");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'toWF_Point2'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: toWF<WFMath::Vector<3> > of class  EmberOgre::Convert */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Convert_toWF_Vector300
static int tolua_EmberOgre_EmberOgre_Convert_toWF_Vector300(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Convert",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Vector3* p = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
  {
   WFMath::Vector<3> tolua_ret = (WFMath::Vector<3>)  EmberOgre::Convert::toWF<WFMath::Vector<3> >(*p);
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
 tolua_error(tolua_S,"#ferror in function 'toWF_Vector3'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: toWF of class  EmberOgre::Convert */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Convert_toWF00
static int tolua_EmberOgre_EmberOgre_Convert_toWF00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Convert",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector2",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Vector2* p = ((const Ogre::Vector2*)  tolua_tousertype(tolua_S,2,0));
  {
   WFMath::Point<2> tolua_ret = (WFMath::Point<2>)  EmberOgre::Convert::toWF(*p);
   {
#ifdef __cplusplus
    void* tolua_obj = new WFMath::Point<2>(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"WFMath::Point<2>");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(WFMath::Point<2>));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"WFMath::Point<2>");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'toWF'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: toOgre of class  EmberOgre::Convert */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Convert_toOgre01
static int tolua_EmberOgre_EmberOgre_Convert_toOgre01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Convert",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const WFMath::Point<3>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const WFMath::Point<3>* p = ((const WFMath::Point<3>*)  tolua_tousertype(tolua_S,2,0));
  {
   Ogre::Vector3 tolua_ret = (Ogre::Vector3)  EmberOgre::Convert::toOgre(*p);
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
 return tolua_EmberOgre_EmberOgre_Convert_toOgre00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: toOgre of class  EmberOgre::Convert */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Convert_toOgre02
static int tolua_EmberOgre_EmberOgre_Convert_toOgre02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Convert",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const WFMath::Vector<3>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const WFMath::Vector<3>* v = ((const WFMath::Vector<3>*)  tolua_tousertype(tolua_S,2,0));
  {
   Ogre::Vector3 tolua_ret = (Ogre::Vector3)  EmberOgre::Convert::toOgre(*v);
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
 return tolua_EmberOgre_EmberOgre_Convert_toOgre01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: toOgre of class  EmberOgre::Convert */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Convert_toOgre03
static int tolua_EmberOgre_EmberOgre_Convert_toOgre03(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Convert",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const WFMath::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const WFMath::Quaternion* aq = ((const WFMath::Quaternion*)  tolua_tousertype(tolua_S,2,0));
  {
   Ogre::Quaternion tolua_ret = (Ogre::Quaternion)  EmberOgre::Convert::toOgre(*aq);
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
 return tolua_EmberOgre_EmberOgre_Convert_toOgre02(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: toWF of class  EmberOgre::Convert */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Convert_toWF01
static int tolua_EmberOgre_EmberOgre_Convert_toWF01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Convert",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Quaternion* aq = ((const Ogre::Quaternion*)  tolua_tousertype(tolua_S,2,0));
  {
   WFMath::Quaternion tolua_ret = (WFMath::Quaternion)  EmberOgre::Convert::toWF(*aq);
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
 return tolua_EmberOgre_EmberOgre_Convert_toWF00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::SimpleRenderContext */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_SimpleRenderContext_new00
static int tolua_EmberOgre_EmberOgre_SimpleRenderContext_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::SimpleRenderContext",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::string prefix = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  int width = ((int)  tolua_tonumber(tolua_S,3,0));
  int height = ((int)  tolua_tonumber(tolua_S,4,0));
  {
   EmberOgre::SimpleRenderContext* tolua_ret = (EmberOgre::SimpleRenderContext*)  new EmberOgre::SimpleRenderContext(prefix,width,height);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::SimpleRenderContext");
   tolua_pushcppstring(tolua_S,(const char*)prefix);
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

/* method: new_local of class  EmberOgre::SimpleRenderContext */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_SimpleRenderContext_new00_local
static int tolua_EmberOgre_EmberOgre_SimpleRenderContext_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::SimpleRenderContext",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::string prefix = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  int width = ((int)  tolua_tonumber(tolua_S,3,0));
  int height = ((int)  tolua_tonumber(tolua_S,4,0));
  {
   EmberOgre::SimpleRenderContext* tolua_ret = (EmberOgre::SimpleRenderContext*)  new EmberOgre::SimpleRenderContext(prefix,width,height);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::SimpleRenderContext");
   tolua_pushcppstring(tolua_S,(const char*)prefix);
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

/* method: new of class  EmberOgre::SimpleRenderContext */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_SimpleRenderContext_new01
static int tolua_EmberOgre_EmberOgre_SimpleRenderContext_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::SimpleRenderContext",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"Ogre::TexturePtr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const std::string prefix = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  Ogre::TexturePtr texturePtr = *((Ogre::TexturePtr*)  tolua_tousertype(tolua_S,3,0));
  {
   EmberOgre::SimpleRenderContext* tolua_ret = (EmberOgre::SimpleRenderContext*)  new EmberOgre::SimpleRenderContext(prefix,texturePtr);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::SimpleRenderContext");
   tolua_pushcppstring(tolua_S,(const char*)prefix);
  }
 }
 return 2;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_SimpleRenderContext_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::SimpleRenderContext */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_SimpleRenderContext_new01_local
static int tolua_EmberOgre_EmberOgre_SimpleRenderContext_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::SimpleRenderContext",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"Ogre::TexturePtr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const std::string prefix = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  Ogre::TexturePtr texturePtr = *((Ogre::TexturePtr*)  tolua_tousertype(tolua_S,3,0));
  {
   EmberOgre::SimpleRenderContext* tolua_ret = (EmberOgre::SimpleRenderContext*)  new EmberOgre::SimpleRenderContext(prefix,texturePtr);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::SimpleRenderContext");
   tolua_pushcppstring(tolua_S,(const char*)prefix);
  }
 }
 return 2;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_SimpleRenderContext_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSceneNode of class  EmberOgre::SimpleRenderContext */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_SimpleRenderContext_getSceneNode00
static int tolua_EmberOgre_EmberOgre_SimpleRenderContext_getSceneNode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::SimpleRenderContext",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::SimpleRenderContext* self = (const EmberOgre::SimpleRenderContext*)  tolua_tousertype(tolua_S,1,0);
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

/* method: getCamera of class  EmberOgre::SimpleRenderContext */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_SimpleRenderContext_getCamera00
static int tolua_EmberOgre_EmberOgre_SimpleRenderContext_getCamera00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::SimpleRenderContext",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::SimpleRenderContext* self = (const EmberOgre::SimpleRenderContext*)  tolua_tousertype(tolua_S,1,0);
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

/* method: getDefaultCameraDistance of class  EmberOgre::SimpleRenderContext */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_SimpleRenderContext_getDefaultCameraDistance00
static int tolua_EmberOgre_EmberOgre_SimpleRenderContext_getDefaultCameraDistance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::SimpleRenderContext",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::SimpleRenderContext* self = (const EmberOgre::SimpleRenderContext*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getDefaultCameraDistance'",NULL);
#endif
  {
   float tolua_ret = (float)  self->getDefaultCameraDistance();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getDefaultCameraDistance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setActive of class  EmberOgre::SimpleRenderContext */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_SimpleRenderContext_setActive00
static int tolua_EmberOgre_EmberOgre_SimpleRenderContext_setActive00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::SimpleRenderContext",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::SimpleRenderContext* self = (EmberOgre::SimpleRenderContext*)  tolua_tousertype(tolua_S,1,0);
  bool active = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setActive'",NULL);
#endif
  {
   self->setActive(active);
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

/* method: showFull of class  EmberOgre::SimpleRenderContext */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_SimpleRenderContext_showFull00
static int tolua_EmberOgre_EmberOgre_SimpleRenderContext_showFull00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::SimpleRenderContext",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::MovableObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::SimpleRenderContext* self = (EmberOgre::SimpleRenderContext*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::MovableObject* object = ((const Ogre::MovableObject*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'showFull'",NULL);
#endif
  {
   self->showFull(object);
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

/* method: repositionCamera of class  EmberOgre::SimpleRenderContext */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_SimpleRenderContext_repositionCamera00
static int tolua_EmberOgre_EmberOgre_SimpleRenderContext_repositionCamera00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::SimpleRenderContext",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::SimpleRenderContext* self = (EmberOgre::SimpleRenderContext*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'repositionCamera'",NULL);
#endif
  {
   self->repositionCamera();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'repositionCamera'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: pitch of class  EmberOgre::SimpleRenderContext */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_SimpleRenderContext_pitch00
static int tolua_EmberOgre_EmberOgre_SimpleRenderContext_pitch00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::SimpleRenderContext",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::Degree",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::SimpleRenderContext* self = (EmberOgre::SimpleRenderContext*)  tolua_tousertype(tolua_S,1,0);
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

/* method: yaw of class  EmberOgre::SimpleRenderContext */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_SimpleRenderContext_yaw00
static int tolua_EmberOgre_EmberOgre_SimpleRenderContext_yaw00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::SimpleRenderContext",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::Degree",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::SimpleRenderContext* self = (EmberOgre::SimpleRenderContext*)  tolua_tousertype(tolua_S,1,0);
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

/* method: roll of class  EmberOgre::SimpleRenderContext */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_SimpleRenderContext_roll00
static int tolua_EmberOgre_EmberOgre_SimpleRenderContext_roll00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::SimpleRenderContext",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::Degree",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::SimpleRenderContext* self = (EmberOgre::SimpleRenderContext*)  tolua_tousertype(tolua_S,1,0);
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

/* method: setCameraDistance of class  EmberOgre::SimpleRenderContext */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_SimpleRenderContext_setCameraDistance00
static int tolua_EmberOgre_EmberOgre_SimpleRenderContext_setCameraDistance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::SimpleRenderContext",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::SimpleRenderContext* self = (EmberOgre::SimpleRenderContext*)  tolua_tousertype(tolua_S,1,0);
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

/* method: getCameraDistance of class  EmberOgre::SimpleRenderContext */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_SimpleRenderContext_getCameraDistance00
static int tolua_EmberOgre_EmberOgre_SimpleRenderContext_getCameraDistance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::SimpleRenderContext",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::SimpleRenderContext* self = (const EmberOgre::SimpleRenderContext*)  tolua_tousertype(tolua_S,1,0);
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

/* method: getAbsoluteCameraDistance of class  EmberOgre::SimpleRenderContext */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_SimpleRenderContext_getAbsoluteCameraDistance00
static int tolua_EmberOgre_EmberOgre_SimpleRenderContext_getAbsoluteCameraDistance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::SimpleRenderContext",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::SimpleRenderContext* self = (const EmberOgre::SimpleRenderContext*)  tolua_tousertype(tolua_S,1,0);
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

/* method: getEntityRotation of class  EmberOgre::SimpleRenderContext */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_SimpleRenderContext_getEntityRotation00
static int tolua_EmberOgre_EmberOgre_SimpleRenderContext_getEntityRotation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::SimpleRenderContext",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::SimpleRenderContext* self = (EmberOgre::SimpleRenderContext*)  tolua_tousertype(tolua_S,1,0);
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

/* method: resetCameraOrientation of class  EmberOgre::SimpleRenderContext */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_SimpleRenderContext_resetCameraOrientation00
static int tolua_EmberOgre_EmberOgre_SimpleRenderContext_resetCameraOrientation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::SimpleRenderContext",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::SimpleRenderContext* self = (EmberOgre::SimpleRenderContext*)  tolua_tousertype(tolua_S,1,0);
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

/* method: getSceneManager of class  EmberOgre::SimpleRenderContext */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_SimpleRenderContext_getSceneManager00
static int tolua_EmberOgre_EmberOgre_SimpleRenderContext_getSceneManager00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::SimpleRenderContext",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::SimpleRenderContext* self = (const EmberOgre::SimpleRenderContext*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSceneManager'",NULL);
#endif
  {
   Ogre::SceneManager* tolua_ret = (Ogre::SceneManager*)  self->getSceneManager();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::SceneManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSceneManager'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRenderTexture of class  EmberOgre::SimpleRenderContext */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_SimpleRenderContext_getRenderTexture00
static int tolua_EmberOgre_EmberOgre_SimpleRenderContext_getRenderTexture00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::SimpleRenderContext",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::SimpleRenderContext* self = (EmberOgre::SimpleRenderContext*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRenderTexture'",NULL);
#endif
  {
   Ogre::RenderTexture* tolua_ret = (Ogre::RenderTexture*)  self->getRenderTexture();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::RenderTexture");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRenderTexture'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTexture of class  EmberOgre::SimpleRenderContext */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_SimpleRenderContext_getTexture00
static int tolua_EmberOgre_EmberOgre_SimpleRenderContext_getTexture00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::SimpleRenderContext",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::SimpleRenderContext* self = (EmberOgre::SimpleRenderContext*)  tolua_tousertype(tolua_S,1,0);
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

/* method: getCameraRootNode of class  EmberOgre::SimpleRenderContext */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_SimpleRenderContext_getCameraRootNode00
static int tolua_EmberOgre_EmberOgre_SimpleRenderContext_getCameraRootNode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::SimpleRenderContext",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::SimpleRenderContext* self = (const EmberOgre::SimpleRenderContext*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCameraRootNode'",NULL);
#endif
  {
   Ogre::SceneNode* tolua_ret = (Ogre::SceneNode*)  self->getCameraRootNode();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::SceneNode");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getCameraRootNode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getViewport of class  EmberOgre::SimpleRenderContext */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_SimpleRenderContext_getViewport00
static int tolua_EmberOgre_EmberOgre_SimpleRenderContext_getViewport00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::SimpleRenderContext",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::SimpleRenderContext* self = (const EmberOgre::SimpleRenderContext*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getViewport'",NULL);
#endif
  {
   Ogre::Viewport* tolua_ret = (Ogre::Viewport*)  self->getViewport();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::Viewport");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getViewport'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getLight of class  EmberOgre::SimpleRenderContext */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_SimpleRenderContext_getLight00
static int tolua_EmberOgre_EmberOgre_SimpleRenderContext_getLight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::SimpleRenderContext",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::SimpleRenderContext* self = (EmberOgre::SimpleRenderContext*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLight'",NULL);
#endif
  {
   Ogre::Light* tolua_ret = (Ogre::Light*)  self->getLight();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::Light");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getLight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setBackgroundColour of class  EmberOgre::SimpleRenderContext */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_SimpleRenderContext_setBackgroundColour00
static int tolua_EmberOgre_EmberOgre_SimpleRenderContext_setBackgroundColour00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::SimpleRenderContext",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::ColourValue",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::SimpleRenderContext* self = (EmberOgre::SimpleRenderContext*)  tolua_tousertype(tolua_S,1,0);
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

/* method: setBackgroundColour of class  EmberOgre::SimpleRenderContext */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_SimpleRenderContext_setBackgroundColour01
static int tolua_EmberOgre_EmberOgre_SimpleRenderContext_setBackgroundColour01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::SimpleRenderContext",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  EmberOgre::SimpleRenderContext* self = (EmberOgre::SimpleRenderContext*)  tolua_tousertype(tolua_S,1,0);
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
 return tolua_EmberOgre_EmberOgre_SimpleRenderContext_setBackgroundColour00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: setTexture of class  EmberOgre::SimpleRenderContext */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_SimpleRenderContext_setTexture00
static int tolua_EmberOgre_EmberOgre_SimpleRenderContext_setTexture00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::SimpleRenderContext",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::TexturePtr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::SimpleRenderContext* self = (EmberOgre::SimpleRenderContext*)  tolua_tousertype(tolua_S,1,0);
  Ogre::TexturePtr texture = *((Ogre::TexturePtr*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setTexture'",NULL);
#endif
  {
   self->setTexture(texture);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setTexture'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getCameraPositionMode of class  EmberOgre::SimpleRenderContext */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_SimpleRenderContext_getCameraPositionMode00
static int tolua_EmberOgre_EmberOgre_SimpleRenderContext_getCameraPositionMode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::SimpleRenderContext",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::SimpleRenderContext* self = (const EmberOgre::SimpleRenderContext*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCameraPositionMode'",NULL);
#endif
  {
   EmberOgre::SimpleRenderContext::CameraPositioningMode tolua_ret = (EmberOgre::SimpleRenderContext::CameraPositioningMode)  self->getCameraPositionMode();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getCameraPositionMode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setCameraPositionMode of class  EmberOgre::SimpleRenderContext */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_SimpleRenderContext_setCameraPositionMode00
static int tolua_EmberOgre_EmberOgre_SimpleRenderContext_setCameraPositionMode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::SimpleRenderContext",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::SimpleRenderContext* self = (EmberOgre::SimpleRenderContext*)  tolua_tousertype(tolua_S,1,0);
  EmberOgre::SimpleRenderContext::CameraPositioningMode mode = ((EmberOgre::SimpleRenderContext::CameraPositioningMode) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setCameraPositionMode'",NULL);
#endif
  {
   self->setCameraPositionMode(mode);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setCameraPositionMode'.",&tolua_err);
 return 0;
#endif
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

/* method: showAxis of class  EmberOgre::Gui::MovableObjectRenderer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_showAxis00
static int tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_showAxis00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'showAxis'",NULL);
#endif
  {
   self->showAxis();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'showAxis'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: hideAxis of class  EmberOgre::Gui::MovableObjectRenderer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_hideAxis00
static int tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_hideAxis00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hideAxis'",NULL);
#endif
  {
   self->hideAxis();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'hideAxis'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getCameraPositionMode of class  EmberOgre::Gui::MovableObjectRenderer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_getCameraPositionMode00
static int tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_getCameraPositionMode00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCameraPositionMode'",NULL);
#endif
  {
   EmberOgre::SimpleRenderContext::CameraPositioningMode tolua_ret = (EmberOgre::SimpleRenderContext::CameraPositioningMode)  self->getCameraPositionMode();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getCameraPositionMode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setCameraPositionMode of class  EmberOgre::Gui::MovableObjectRenderer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_setCameraPositionMode00
static int tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_setCameraPositionMode00(lua_State* tolua_S)
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
  EmberOgre::SimpleRenderContext::CameraPositioningMode mode = ((EmberOgre::SimpleRenderContext::CameraPositioningMode) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setCameraPositionMode'",NULL);
#endif
  {
   self->setCameraPositionMode(mode);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setCameraPositionMode'.",&tolua_err);
 return 0;
#endif
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

/* method: repositionSceneNode of class  EmberOgre::Gui::ModelRenderer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_ModelRenderer_repositionSceneNode00
static int tolua_EmberOgre_EmberOgre_Gui_ModelRenderer_repositionSceneNode00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'repositionSceneNode'",NULL);
#endif
  {
   self->repositionSceneNode();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'repositionSceneNode'.",&tolua_err);
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
  tolua_pushnumber(tolua_S,(lua_Number)self->distance);
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
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->distance = ((float)  tolua_tonumber(tolua_S,2,0))
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

/* method: new of class  EmberOgre::Model::ModelDefinitionAtlasComposer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_ModelDefinitionAtlasComposer_new00
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinitionAtlasComposer_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Model::ModelDefinitionAtlasComposer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   EmberOgre::Model::ModelDefinitionAtlasComposer* tolua_ret = (EmberOgre::Model::ModelDefinitionAtlasComposer*)  new EmberOgre::Model::ModelDefinitionAtlasComposer();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Model::ModelDefinitionAtlasComposer");
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

/* method: new_local of class  EmberOgre::Model::ModelDefinitionAtlasComposer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_ModelDefinitionAtlasComposer_new00_local
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinitionAtlasComposer_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Model::ModelDefinitionAtlasComposer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   EmberOgre::Model::ModelDefinitionAtlasComposer* tolua_ret = (EmberOgre::Model::ModelDefinitionAtlasComposer*)  new EmberOgre::Model::ModelDefinitionAtlasComposer();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Model::ModelDefinitionAtlasComposer");
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

/* method: delete of class  EmberOgre::Model::ModelDefinitionAtlasComposer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_ModelDefinitionAtlasComposer_delete00
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinitionAtlasComposer_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::ModelDefinitionAtlasComposer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::ModelDefinitionAtlasComposer* self = (EmberOgre::Model::ModelDefinitionAtlasComposer*)  tolua_tousertype(tolua_S,1,0);
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

/* method: compose of class  EmberOgre::Model::ModelDefinitionAtlasComposer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_ModelDefinitionAtlasComposer_compose00
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinitionAtlasComposer_compose00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::ModelDefinitionAtlasComposer",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"EmberOgre::Model::Model",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::ModelDefinitionAtlasComposer* self = (EmberOgre::Model::ModelDefinitionAtlasComposer*)  tolua_tousertype(tolua_S,1,0);
  EmberOgre::Model::Model* model = ((EmberOgre::Model::Model*)  tolua_tousertype(tolua_S,2,0));
  const std::string typeName = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  const std::string parentTypeName = ((const std::string)  tolua_tocppstring(tolua_S,4,0));
  float scale = ((float)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'compose'",NULL);
#endif
  {
   Atlas::Message::MapType tolua_ret = (Atlas::Message::MapType)  self->compose(model,typeName,parentTypeName,scale);
   {
#ifdef __cplusplus
    void* tolua_obj = new Atlas::Message::MapType(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Atlas::Message::MapType");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Atlas::Message::MapType));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Atlas::Message::MapType");
#endif
   }
   tolua_pushcppstring(tolua_S,(const char*)typeName);
   tolua_pushcppstring(tolua_S,(const char*)parentTypeName);
  }
 }
 return 3;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'compose'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: composeToStream of class  EmberOgre::Model::ModelDefinitionAtlasComposer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_ModelDefinitionAtlasComposer_composeToStream00
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinitionAtlasComposer_composeToStream00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::ModelDefinitionAtlasComposer",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"std::iostream",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"EmberOgre::Model::Model",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::ModelDefinitionAtlasComposer* self = (EmberOgre::Model::ModelDefinitionAtlasComposer*)  tolua_tousertype(tolua_S,1,0);
  std::iostream* outstream = ((std::iostream*)  tolua_tousertype(tolua_S,2,0));
  EmberOgre::Model::Model* model = ((EmberOgre::Model::Model*)  tolua_tousertype(tolua_S,3,0));
  const std::string typeName = ((const std::string)  tolua_tocppstring(tolua_S,4,0));
  const std::string parentTypeName = ((const std::string)  tolua_tocppstring(tolua_S,5,0));
  float scale = ((float)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'composeToStream'",NULL);
#endif
  {
   self->composeToStream(*outstream,model,typeName,parentTypeName,scale);
   tolua_pushcppstring(tolua_S,(const char*)typeName);
   tolua_pushcppstring(tolua_S,(const char*)parentTypeName);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'composeToStream'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: composeToFile of class  EmberOgre::Model::ModelDefinitionAtlasComposer */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Model_ModelDefinitionAtlasComposer_composeToFile00
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinitionAtlasComposer_composeToFile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Model::ModelDefinitionAtlasComposer",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"EmberOgre::Model::Model",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::ModelDefinitionAtlasComposer* self = (EmberOgre::Model::ModelDefinitionAtlasComposer*)  tolua_tousertype(tolua_S,1,0);
  EmberOgre::Model::Model* model = ((EmberOgre::Model::Model*)  tolua_tousertype(tolua_S,2,0));
  const std::string typeName = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  const std::string parentTypeName = ((const std::string)  tolua_tocppstring(tolua_S,4,0));
  float scale = ((float)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'composeToFile'",NULL);
#endif
  {
   self->composeToFile(model,typeName,parentTypeName,scale);
   tolua_pushcppstring(tolua_S,(const char*)typeName);
   tolua_pushcppstring(tolua_S,(const char*)parentTypeName);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'composeToFile'.",&tolua_err);
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

/* method: new of class  EmberOgre::GUIAdapter */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_GUIAdapter_new00
static int tolua_EmberOgre_EmberOgre_GUIAdapter_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::GUIAdapter",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::string type = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  {
   EmberOgre::GUIAdapter* tolua_ret = (EmberOgre::GUIAdapter*)  new EmberOgre::GUIAdapter(type);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::GUIAdapter");
   tolua_pushcppstring(tolua_S,(const char*)type);
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

/* method: new_local of class  EmberOgre::GUIAdapter */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_GUIAdapter_new00_local
static int tolua_EmberOgre_EmberOgre_GUIAdapter_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::GUIAdapter",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::string type = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  {
   EmberOgre::GUIAdapter* tolua_ret = (EmberOgre::GUIAdapter*)  new EmberOgre::GUIAdapter(type);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::GUIAdapter");
   tolua_pushcppstring(tolua_S,(const char*)type);
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

/* method: delete of class  EmberOgre::GUIAdapter */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_GUIAdapter_delete00
static int tolua_EmberOgre_EmberOgre_GUIAdapter_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::GUIAdapter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::GUIAdapter* self = (EmberOgre::GUIAdapter*)  tolua_tousertype(tolua_S,1,0);
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

/* method: getType of class  EmberOgre::GUIAdapter */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_GUIAdapter_getType00
static int tolua_EmberOgre_EmberOgre_GUIAdapter_getType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::GUIAdapter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::GUIAdapter* self = (const EmberOgre::GUIAdapter*)  tolua_tousertype(tolua_S,1,0);
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

/* method: attach of class  EmberOgre::GUIAdapter */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_GUIAdapter_attach00
static int tolua_EmberOgre_EmberOgre_GUIAdapter_attach00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::GUIAdapter",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::GUIAdapter* self = (EmberOgre::GUIAdapter*)  tolua_tousertype(tolua_S,1,0);
  CEGUI::Window* window = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'attach'",NULL);
#endif
  {
   self->attach(window);
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

/* method: detach of class  EmberOgre::GUIAdapter */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_GUIAdapter_detach00
static int tolua_EmberOgre_EmberOgre_GUIAdapter_detach00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::GUIAdapter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::GUIAdapter* self = (EmberOgre::GUIAdapter*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'detach'",NULL);
#endif
  {
   self->detach();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'detach'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getValue of class  EmberOgre::GUIAdapter */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_GUIAdapter_getValue00
static int tolua_EmberOgre_EmberOgre_GUIAdapter_getValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::GUIAdapter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::GUIAdapter* self = (EmberOgre::GUIAdapter*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getValue'",NULL);
#endif
  {
   Atlas::Message::Element tolua_ret = (Atlas::Message::Element)  self->getValue();
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
 tolua_error(tolua_S,"#ferror in function 'getValue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setTitle of class  EmberOgre::GUIAdapter */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_GUIAdapter_setTitle00
static int tolua_EmberOgre_EmberOgre_GUIAdapter_setTitle00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::GUIAdapter",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::GUIAdapter* self = (EmberOgre::GUIAdapter*)  tolua_tousertype(tolua_S,1,0);
  const std::string title = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setTitle'",NULL);
#endif
  {
   self->setTitle(title);
   tolua_pushcppstring(tolua_S,(const char*)title);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setTitle'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTitle of class  EmberOgre::GUIAdapter */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_GUIAdapter_getTitle00
static int tolua_EmberOgre_EmberOgre_GUIAdapter_getTitle00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::GUIAdapter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::GUIAdapter* self = (const EmberOgre::GUIAdapter*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTitle'",NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getTitle();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTitle'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setTooltip of class  EmberOgre::GUIAdapter */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_GUIAdapter_setTooltip00
static int tolua_EmberOgre_EmberOgre_GUIAdapter_setTooltip00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::GUIAdapter",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::GUIAdapter* self = (EmberOgre::GUIAdapter*)  tolua_tousertype(tolua_S,1,0);
  const std::string tooltip = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setTooltip'",NULL);
#endif
  {
   self->setTooltip(tooltip);
   tolua_pushcppstring(tolua_S,(const char*)tooltip);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setTooltip'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTooltip of class  EmberOgre::GUIAdapter */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_GUIAdapter_getTooltip00
static int tolua_EmberOgre_EmberOgre_GUIAdapter_getTooltip00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::GUIAdapter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::GUIAdapter* self = (const EmberOgre::GUIAdapter*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTooltip'",NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getTooltip();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTooltip'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addSuggestion of class  EmberOgre::GUIAdapter */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_GUIAdapter_addSuggestion00
static int tolua_EmberOgre_EmberOgre_GUIAdapter_addSuggestion00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::GUIAdapter",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::GUIAdapter* self = (EmberOgre::GUIAdapter*)  tolua_tousertype(tolua_S,1,0);
  const std::string value = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string text = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addSuggestion'",NULL);
#endif
  {
   self->addSuggestion(value,text);
   tolua_pushcppstring(tolua_S,(const char*)value);
   tolua_pushcppstring(tolua_S,(const char*)text);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addSuggestion'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::EntityRecipe */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EntityRecipe_new00
static int tolua_EmberOgre_EmberOgre_EntityRecipe_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::EntityRecipe",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::ResourceManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Ogre::String",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"Ogre::ResourceHandle",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"const Ogre::String",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,6,1,&tolua_err) ||
     !tolua_isusertype(tolua_S,7,"Ogre::ManualResourceLoader",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::ResourceManager* creator = ((Ogre::ResourceManager*)  tolua_tousertype(tolua_S,2,0));
  const Ogre::String* name = ((const Ogre::String*)  tolua_tousertype(tolua_S,3,0));
  Ogre::ResourceHandle handle = *((Ogre::ResourceHandle*)  tolua_tousertype(tolua_S,4,0));
  const Ogre::String* group = ((const Ogre::String*)  tolua_tousertype(tolua_S,5,0));
  bool isManual = ((bool)  tolua_toboolean(tolua_S,6,false));
  Ogre::ManualResourceLoader* loader = ((Ogre::ManualResourceLoader*)  tolua_tousertype(tolua_S,7,0));
  {
   EmberOgre::EntityRecipe* tolua_ret = (EmberOgre::EntityRecipe*)  new EmberOgre::EntityRecipe(creator,*name,handle,*group,isManual,loader);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::EntityRecipe");
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

/* method: new_local of class  EmberOgre::EntityRecipe */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EntityRecipe_new00_local
static int tolua_EmberOgre_EmberOgre_EntityRecipe_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::EntityRecipe",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::ResourceManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Ogre::String",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"Ogre::ResourceHandle",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"const Ogre::String",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,6,1,&tolua_err) ||
     !tolua_isusertype(tolua_S,7,"Ogre::ManualResourceLoader",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::ResourceManager* creator = ((Ogre::ResourceManager*)  tolua_tousertype(tolua_S,2,0));
  const Ogre::String* name = ((const Ogre::String*)  tolua_tousertype(tolua_S,3,0));
  Ogre::ResourceHandle handle = *((Ogre::ResourceHandle*)  tolua_tousertype(tolua_S,4,0));
  const Ogre::String* group = ((const Ogre::String*)  tolua_tousertype(tolua_S,5,0));
  bool isManual = ((bool)  tolua_toboolean(tolua_S,6,false));
  Ogre::ManualResourceLoader* loader = ((Ogre::ManualResourceLoader*)  tolua_tousertype(tolua_S,7,0));
  {
   EmberOgre::EntityRecipe* tolua_ret = (EmberOgre::EntityRecipe*)  new EmberOgre::EntityRecipe(creator,*name,handle,*group,isManual,loader);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::EntityRecipe");
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

/* method: delete of class  EmberOgre::EntityRecipe */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EntityRecipe_delete00
static int tolua_EmberOgre_EmberOgre_EntityRecipe_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::EntityRecipe",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::EntityRecipe* self = (EmberOgre::EntityRecipe*)  tolua_tousertype(tolua_S,1,0);
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

/* method: loadImpl of class  EmberOgre::EntityRecipe */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EntityRecipe_loadImpl00
static int tolua_EmberOgre_EmberOgre_EntityRecipe_loadImpl00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::EntityRecipe",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::EntityRecipe* self = (EmberOgre::EntityRecipe*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'loadImpl'",NULL);
#endif
  {
   self->loadImpl();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'loadImpl'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: unloadImpl of class  EmberOgre::EntityRecipe */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EntityRecipe_unloadImpl00
static int tolua_EmberOgre_EmberOgre_EntityRecipe_unloadImpl00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::EntityRecipe",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::EntityRecipe* self = (EmberOgre::EntityRecipe*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'unloadImpl'",NULL);
#endif
  {
   self->unloadImpl();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'unloadImpl'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: calculateSize of class  EmberOgre::EntityRecipe */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EntityRecipe_calculateSize00
static int tolua_EmberOgre_EmberOgre_EntityRecipe_calculateSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::EntityRecipe",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::EntityRecipe* self = (const EmberOgre::EntityRecipe*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'calculateSize'",NULL);
#endif
  {
   size_t tolua_ret = (size_t)  self->calculateSize();
   {
#ifdef __cplusplus
    void* tolua_obj = new size_t(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"size_t");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(size_t));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"size_t");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'calculateSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getGUIAdapters of class  EmberOgre::EntityRecipe */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EntityRecipe_getGUIAdapters00
static int tolua_EmberOgre_EmberOgre_EntityRecipe_getGUIAdapters00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::EntityRecipe",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::EntityRecipe* self = (EmberOgre::EntityRecipe*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getGUIAdapters'",NULL);
#endif
  {
   const EmberOgre::GUIAdaptersStore& tolua_ret = (const EmberOgre::GUIAdaptersStore&)  self->getGUIAdapters();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const EmberOgre::GUIAdaptersStore");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getGUIAdapters'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setAuthor of class  EmberOgre::EntityRecipe */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EntityRecipe_setAuthor00
static int tolua_EmberOgre_EmberOgre_EntityRecipe_setAuthor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::EntityRecipe",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::EntityRecipe* self = (EmberOgre::EntityRecipe*)  tolua_tousertype(tolua_S,1,0);
  const std::string author = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAuthor'",NULL);
#endif
  {
   self->setAuthor(author);
   tolua_pushcppstring(tolua_S,(const char*)author);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAuthor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAuthor of class  EmberOgre::EntityRecipe */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EntityRecipe_getAuthor00
static int tolua_EmberOgre_EmberOgre_EntityRecipe_getAuthor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::EntityRecipe",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::EntityRecipe* self = (const EmberOgre::EntityRecipe*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAuthor'",NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getAuthor();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAuthor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setDescription of class  EmberOgre::EntityRecipe */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EntityRecipe_setDescription00
static int tolua_EmberOgre_EmberOgre_EntityRecipe_setDescription00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::EntityRecipe",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::EntityRecipe* self = (EmberOgre::EntityRecipe*)  tolua_tousertype(tolua_S,1,0);
  const std::string description = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setDescription'",NULL);
#endif
  {
   self->setDescription(description);
   tolua_pushcppstring(tolua_S,(const char*)description);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setDescription'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getDescription of class  EmberOgre::EntityRecipe */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EntityRecipe_getDescription00
static int tolua_EmberOgre_EmberOgre_EntityRecipe_getDescription00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::EntityRecipe",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::EntityRecipe* self = (const EmberOgre::EntityRecipe*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getDescription'",NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getDescription();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getDescription'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createEntity of class  EmberOgre::EntityRecipe */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EntityRecipe_createEntity00
static int tolua_EmberOgre_EmberOgre_EntityRecipe_createEntity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::EntityRecipe",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Eris::TypeService",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::EntityRecipe* self = (EmberOgre::EntityRecipe*)  tolua_tousertype(tolua_S,1,0);
  Eris::TypeService* typeService = ((Eris::TypeService*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createEntity'",NULL);
#endif
  {
   self->createEntity(*typeService);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createEntity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get of class  EmberOgre::EntityRecipePtr */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EntityRecipePtr_get00
static int tolua_EmberOgre_EmberOgre_EntityRecipePtr_get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::EntityRecipePtr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::EntityRecipePtr* self = (EmberOgre::EntityRecipePtr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get'",NULL);
#endif
  {
   EmberOgre::EntityRecipe* tolua_ret = (EmberOgre::EntityRecipe*)  self->get();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::EntityRecipe");
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

/* get function: first of class  value_type */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__GUIAdaptersStore__value_type_first
static int tolua_get_EmberOgre__GUIAdaptersStore__value_type_first(lua_State* tolua_S)
{
  EmberOgre::GUIAdaptersStore::value_type* self = (EmberOgre::GUIAdaptersStore::value_type*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'first'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->first);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: second of class  value_type */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__GUIAdaptersStore__value_type_second_ptr
static int tolua_get_EmberOgre__GUIAdaptersStore__value_type_second_ptr(lua_State* tolua_S)
{
  EmberOgre::GUIAdaptersStore::value_type* self = (EmberOgre::GUIAdaptersStore::value_type*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'second'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)self->second,"EmberOgre::GUIAdapter");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: second of class  value_type */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__GUIAdaptersStore__value_type_second_ptr
static int tolua_set_EmberOgre__GUIAdaptersStore__value_type_second_ptr(lua_State* tolua_S)
{
  EmberOgre::GUIAdaptersStore::value_type* self = (EmberOgre::GUIAdaptersStore::value_type*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'second'",NULL);
  if (!tolua_isusertype(tolua_S,2,"EmberOgre::GUIAdapter",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->second = ((EmberOgre::GUIAdapter*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: __operator_ptr of class  const_iterator */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_GUIAdaptersStore_const_iterator_get00
static int tolua_EmberOgre_EmberOgre_GUIAdaptersStore_const_iterator_get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::GUIAdaptersStore::const_iterator",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::GUIAdaptersStore::const_iterator* self = (EmberOgre::GUIAdaptersStore::const_iterator*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function '__operator_ptr'",NULL);
#endif
  {
   EmberOgre::GUIAdaptersStore::value_type& tolua_ret = (EmberOgre::GUIAdaptersStore::value_type&)  self->__operator_ptr();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"EmberOgre::GUIAdaptersStore::value_type");
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

/* method: __operator_pp of class  const_iterator */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_GUIAdaptersStore_const_iterator_next00
static int tolua_EmberOgre_EmberOgre_GUIAdaptersStore_const_iterator_next00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::GUIAdaptersStore::const_iterator",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::GUIAdaptersStore::const_iterator* self = (EmberOgre::GUIAdaptersStore::const_iterator*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function '__operator_pp'",NULL);
#endif
  {
   EmberOgre::GUIAdaptersStore::const_iterator& tolua_ret = (EmberOgre::GUIAdaptersStore::const_iterator&)  self->__operator_pp();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"EmberOgre::GUIAdaptersStore::const_iterator");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'next'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator== of class  const_iterator */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_GUIAdaptersStore_const_iterator__eq00
static int tolua_EmberOgre_EmberOgre_GUIAdaptersStore_const_iterator__eq00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::GUIAdaptersStore::const_iterator",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const EmberOgre::GUIAdaptersStore::const_iterator",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::GUIAdaptersStore::const_iterator* self = (EmberOgre::GUIAdaptersStore::const_iterator*)  tolua_tousertype(tolua_S,1,0);
  const EmberOgre::GUIAdaptersStore::const_iterator* value = ((const EmberOgre::GUIAdaptersStore::const_iterator*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator=='",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->operator==(*value);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.eq'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: begin of class  EmberOgre::GUIAdaptersStore */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_GUIAdaptersStore_begin00
static int tolua_EmberOgre_EmberOgre_GUIAdaptersStore_begin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::GUIAdaptersStore",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::GUIAdaptersStore* self = (const EmberOgre::GUIAdaptersStore*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'begin'",NULL);
#endif
  {
   EmberOgre::GUIAdaptersStore::const_iterator tolua_ret = (EmberOgre::GUIAdaptersStore::const_iterator)  self->begin();
   {
#ifdef __cplusplus
    void* tolua_obj = new EmberOgre::GUIAdaptersStore::const_iterator(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"EmberOgre::GUIAdaptersStore::const_iterator");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(EmberOgre::GUIAdaptersStore::const_iterator));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"EmberOgre::GUIAdaptersStore::const_iterator");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'begin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: end of class  EmberOgre::GUIAdaptersStore */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_GUIAdaptersStore_theEnd00
static int tolua_EmberOgre_EmberOgre_GUIAdaptersStore_theEnd00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::GUIAdaptersStore",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::GUIAdaptersStore* self = (const EmberOgre::GUIAdaptersStore*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'end'",NULL);
#endif
  {
   EmberOgre::GUIAdaptersStore::const_iterator tolua_ret = (EmberOgre::GUIAdaptersStore::const_iterator)  self->end();
   {
#ifdef __cplusplus
    void* tolua_obj = new EmberOgre::GUIAdaptersStore::const_iterator(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"EmberOgre::GUIAdaptersStore::const_iterator");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(EmberOgre::GUIAdaptersStore::const_iterator));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"EmberOgre::GUIAdaptersStore::const_iterator");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'theEnd'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSingleton of class  EmberOgre::EntityRecipeManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EntityRecipeManager_getSingleton00
static int tolua_EmberOgre_EmberOgre_EntityRecipeManager_getSingleton00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::EntityRecipeManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   EmberOgre::EntityRecipeManager& tolua_ret = (EmberOgre::EntityRecipeManager&)  EmberOgre::EntityRecipeManager::getSingleton();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"EmberOgre::EntityRecipeManager");
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

/* method: new of class  EmberOgre::EntityRecipeManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EntityRecipeManager_new00
static int tolua_EmberOgre_EmberOgre_EntityRecipeManager_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::EntityRecipeManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   EmberOgre::EntityRecipeManager* tolua_ret = (EmberOgre::EntityRecipeManager*)  new EmberOgre::EntityRecipeManager();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::EntityRecipeManager");
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

/* method: new_local of class  EmberOgre::EntityRecipeManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EntityRecipeManager_new00_local
static int tolua_EmberOgre_EmberOgre_EntityRecipeManager_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::EntityRecipeManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   EmberOgre::EntityRecipeManager* tolua_ret = (EmberOgre::EntityRecipeManager*)  new EmberOgre::EntityRecipeManager();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::EntityRecipeManager");
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

/* method: delete of class  EmberOgre::EntityRecipeManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EntityRecipeManager_delete00
static int tolua_EmberOgre_EmberOgre_EntityRecipeManager_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::EntityRecipeManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::EntityRecipeManager* self = (EmberOgre::EntityRecipeManager*)  tolua_tousertype(tolua_S,1,0);
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

/* method: create of class  EmberOgre::EntityRecipeManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EntityRecipeManager_create00
static int tolua_EmberOgre_EmberOgre_EntityRecipeManager_create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::EntityRecipeManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::String",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Ogre::String",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,1,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"Ogre::ManualResourceLoader",1,&tolua_err) ||
     !tolua_isusertype(tolua_S,6,"const Ogre::NameValuePairList",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::EntityRecipeManager* self = (EmberOgre::EntityRecipeManager*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::String* name = ((const Ogre::String*)  tolua_tousertype(tolua_S,2,0));
  const Ogre::String* group = ((const Ogre::String*)  tolua_tousertype(tolua_S,3,0));
  bool isManual = ((bool)  tolua_toboolean(tolua_S,4,false));
  Ogre::ManualResourceLoader* loader = ((Ogre::ManualResourceLoader*)  tolua_tousertype(tolua_S,5,0));
  const Ogre::NameValuePairList* createParams = ((const Ogre::NameValuePairList*)  tolua_tousertype(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'create'",NULL);
#endif
  {
   Ogre::ResourcePtr tolua_ret = (Ogre::ResourcePtr)  self->create(*name,*group,isManual,loader,createParams);
   {
#ifdef __cplusplus
    void* tolua_obj = new Ogre::ResourcePtr(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::ResourcePtr");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::ResourcePtr));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::ResourcePtr");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: parseScript of class  EmberOgre::EntityRecipeManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EntityRecipeManager_parseScript00
static int tolua_EmberOgre_EmberOgre_EntityRecipeManager_parseScript00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::EntityRecipeManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::DataStreamPtr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Ogre::String",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::EntityRecipeManager* self = (EmberOgre::EntityRecipeManager*)  tolua_tousertype(tolua_S,1,0);
  Ogre::DataStreamPtr* stream = ((Ogre::DataStreamPtr*)  tolua_tousertype(tolua_S,2,0));
  const Ogre::String* groupName = ((const Ogre::String*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'parseScript'",NULL);
#endif
  {
   self->parseScript(*stream,*groupName);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'parseScript'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: __Ember of class  EmberOgre::EntityRecipeManager */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__EntityRecipeManager___Ember__Singleton_EntityRecipeManager___
static int tolua_get_EmberOgre__EntityRecipeManager___Ember__Singleton_EntityRecipeManager___(lua_State* tolua_S)
{
  EmberOgre::EntityRecipeManager* self = (EmberOgre::EntityRecipeManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable '__Ember'",NULL);
#endif
#ifdef __cplusplus
  tolua_pushusertype(tolua_S,(void*)static_cast<Ember::Singleton<EmberOgre::EntityRecipeManager>*>(self), "Ember::Singleton<EmberOgre::EntityRecipeManager>");
#else
  tolua_pushusertype(tolua_S,(void*)((Ember::Singleton<EmberOgre::EntityRecipeManager>*)self), "Ember::Singleton<EmberOgre::EntityRecipeManager>");
#endif
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::Gui::EntityCreator */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_EntityCreator_new00
static int tolua_EmberOgre_EmberOgre_Gui_EntityCreator_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::EntityCreator",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   EmberOgre::Gui::EntityCreator* tolua_ret = (EmberOgre::Gui::EntityCreator*)  new EmberOgre::Gui::EntityCreator();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::EntityCreator");
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

/* method: new_local of class  EmberOgre::Gui::EntityCreator */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_EntityCreator_new00_local
static int tolua_EmberOgre_EmberOgre_Gui_EntityCreator_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::EntityCreator",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   EmberOgre::Gui::EntityCreator* tolua_ret = (EmberOgre::Gui::EntityCreator*)  new EmberOgre::Gui::EntityCreator();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Gui::EntityCreator");
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

/* method: delete of class  EmberOgre::Gui::EntityCreator */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_EntityCreator_delete00
static int tolua_EmberOgre_EmberOgre_Gui_EntityCreator_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::EntityCreator",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::EntityCreator* self = (EmberOgre::Gui::EntityCreator*)  tolua_tousertype(tolua_S,1,0);
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

/* method: setRecipe of class  EmberOgre::Gui::EntityCreator */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_EntityCreator_setRecipe00
static int tolua_EmberOgre_EmberOgre_Gui_EntityCreator_setRecipe00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::EntityCreator",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"EmberOgre::EntityRecipe",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::EntityCreator* self = (EmberOgre::Gui::EntityCreator*)  tolua_tousertype(tolua_S,1,0);
  EmberOgre::EntityRecipe* recipe = ((EmberOgre::EntityRecipe*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRecipe'",NULL);
#endif
  {
   self->setRecipe(*recipe);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setRecipe'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: toggleCreateMode of class  EmberOgre::Gui::EntityCreator */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_EntityCreator_toggleCreateMode00
static int tolua_EmberOgre_EmberOgre_Gui_EntityCreator_toggleCreateMode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::EntityCreator",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::EntityCreator* self = (EmberOgre::Gui::EntityCreator*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'toggleCreateMode'",NULL);
#endif
  {
   self->toggleCreateMode();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'toggleCreateMode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setRandomizeOrientation of class  EmberOgre::Gui::EntityCreator */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_EntityCreator_setRandomizeOrientation00
static int tolua_EmberOgre_EmberOgre_Gui_EntityCreator_setRandomizeOrientation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::EntityCreator",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::EntityCreator* self = (EmberOgre::Gui::EntityCreator*)  tolua_tousertype(tolua_S,1,0);
  bool randomize = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRandomizeOrientation'",NULL);
#endif
  {
   self->setRandomizeOrientation(randomize);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setRandomizeOrientation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createEntity of class  EmberOgre::Gui::EntityCreator */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_EntityCreator_createEntity00
static int tolua_EmberOgre_EmberOgre_Gui_EntityCreator_createEntity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::EntityCreator",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::EntityCreator* self = (EmberOgre::Gui::EntityCreator*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createEntity'",NULL);
#endif
  {
   self->createEntity();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createEntity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: cleanupCreation of class  EmberOgre::Gui::EntityCreator */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Gui_EntityCreator_cleanupCreation00
static int tolua_EmberOgre_EmberOgre_Gui_EntityCreator_cleanupCreation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::EntityCreator",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::EntityCreator* self = (EmberOgre::Gui::EntityCreator*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cleanupCreation'",NULL);
#endif
  {
   self->cleanupCreation();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cleanupCreation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: mWidget of class  EmberOgre::Gui::EntityCreator */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__Gui__EntityCreator_mWidget_ptr
static int tolua_get_EmberOgre__Gui__EntityCreator_mWidget_ptr(lua_State* tolua_S)
{
  EmberOgre::Gui::EntityCreator* self = (EmberOgre::Gui::EntityCreator*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mWidget'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)self->mWidget,"EmberOgre::Gui::Widget");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: mWidget of class  EmberOgre::Gui::EntityCreator */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__Gui__EntityCreator_mWidget_ptr
static int tolua_set_EmberOgre__Gui__EntityCreator_mWidget_ptr(lua_State* tolua_S)
{
  EmberOgre::Gui::EntityCreator* self = (EmberOgre::Gui::EntityCreator*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mWidget'",NULL);
  if (!tolua_isusertype(tolua_S,2,"EmberOgre::Gui::Widget",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->mWidget = ((EmberOgre::Gui::Widget*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventTypeInfoLoaded of class  EmberOgre::Gui::EntityCreator */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__Gui__EntityCreator_EventTypeInfoLoaded
static int tolua_get_EmberOgre__Gui__EntityCreator_EventTypeInfoLoaded(lua_State* tolua_S)
{
  EmberOgre::Gui::EntityCreator* self = (EmberOgre::Gui::EntityCreator*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventTypeInfoLoaded'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventTypeInfoLoaded,"sigc::signal<void>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventTypeInfoLoaded of class  EmberOgre::Gui::EntityCreator */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__Gui__EntityCreator_EventTypeInfoLoaded
static int tolua_set_EmberOgre__Gui__EntityCreator_EventTypeInfoLoaded(lua_State* tolua_S)
{
  EmberOgre::Gui::EntityCreator* self = (EmberOgre::Gui::EntityCreator*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventTypeInfoLoaded'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventTypeInfoLoaded = *((sigc::signal<void>*)  tolua_tousertype(tolua_S,2,0))
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

/* method: setResolution of class  EmberOgre::Terrain::Map */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_Map_setResolution00
static int tolua_EmberOgre_EmberOgre_Terrain_Map_setResolution00(lua_State* tolua_S)
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
  float metersPerPixel = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setResolution'",NULL);
#endif
  {
   self->setResolution(metersPerPixel);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setResolution'.",&tolua_err);
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

/* method: __operator_ptr of class  const_iterator */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinitionManager_DefinitionStore_const_iterator_get00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinitionManager_DefinitionStore_const_iterator_get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Terrain::TerrainLayerDefinitionManager::DefinitionStore::const_iterator",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Terrain::TerrainLayerDefinitionManager::DefinitionStore::const_iterator* self = (EmberOgre::Terrain::TerrainLayerDefinitionManager::DefinitionStore::const_iterator*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function '__operator_ptr'",NULL);
#endif
  {
   EmberOgre::Terrain::TerrainLayerDefinition* tolua_ret = (EmberOgre::Terrain::TerrainLayerDefinition*)  self->__operator_ptr();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Terrain::TerrainLayerDefinition");
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

/* method: __operator_pp of class  const_iterator */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinitionManager_DefinitionStore_const_iterator_next00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinitionManager_DefinitionStore_const_iterator_next00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Terrain::TerrainLayerDefinitionManager::DefinitionStore::const_iterator",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Terrain::TerrainLayerDefinitionManager::DefinitionStore::const_iterator* self = (EmberOgre::Terrain::TerrainLayerDefinitionManager::DefinitionStore::const_iterator*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function '__operator_pp'",NULL);
#endif
  {
   EmberOgre::Terrain::TerrainLayerDefinitionManager::DefinitionStore::const_iterator& tolua_ret = (EmberOgre::Terrain::TerrainLayerDefinitionManager::DefinitionStore::const_iterator&)  self->__operator_pp();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"EmberOgre::Terrain::TerrainLayerDefinitionManager::DefinitionStore::const_iterator");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'next'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: begin of class  DefinitionStore */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinitionManager_DefinitionStore_begin00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinitionManager_DefinitionStore_begin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Terrain::TerrainLayerDefinitionManager::DefinitionStore",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Terrain::TerrainLayerDefinitionManager::DefinitionStore* self = (const EmberOgre::Terrain::TerrainLayerDefinitionManager::DefinitionStore*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'begin'",NULL);
#endif
  {
   EmberOgre::Terrain::TerrainLayerDefinitionManager::DefinitionStore::const_iterator tolua_ret = (EmberOgre::Terrain::TerrainLayerDefinitionManager::DefinitionStore::const_iterator)  self->begin();
   {
#ifdef __cplusplus
    void* tolua_obj = new EmberOgre::Terrain::TerrainLayerDefinitionManager::DefinitionStore::const_iterator(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"EmberOgre::Terrain::TerrainLayerDefinitionManager::DefinitionStore::const_iterator");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(EmberOgre::Terrain::TerrainLayerDefinitionManager::DefinitionStore::const_iterator));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"EmberOgre::Terrain::TerrainLayerDefinitionManager::DefinitionStore::const_iterator");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'begin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: end of class  DefinitionStore */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinitionManager_DefinitionStore_theEnd00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinitionManager_DefinitionStore_theEnd00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Terrain::TerrainLayerDefinitionManager::DefinitionStore",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Terrain::TerrainLayerDefinitionManager::DefinitionStore* self = (const EmberOgre::Terrain::TerrainLayerDefinitionManager::DefinitionStore*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'end'",NULL);
#endif
  {
   EmberOgre::Terrain::TerrainLayerDefinitionManager::DefinitionStore::const_iterator tolua_ret = (EmberOgre::Terrain::TerrainLayerDefinitionManager::DefinitionStore::const_iterator)  self->end();
   {
#ifdef __cplusplus
    void* tolua_obj = new EmberOgre::Terrain::TerrainLayerDefinitionManager::DefinitionStore::const_iterator(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"EmberOgre::Terrain::TerrainLayerDefinitionManager::DefinitionStore::const_iterator");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(EmberOgre::Terrain::TerrainLayerDefinitionManager::DefinitionStore::const_iterator));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"EmberOgre::Terrain::TerrainLayerDefinitionManager::DefinitionStore::const_iterator");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'theEnd'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: size of class  DefinitionStore */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinitionManager_DefinitionStore_size00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinitionManager_DefinitionStore_size00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Terrain::TerrainLayerDefinitionManager::DefinitionStore",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Terrain::TerrainLayerDefinitionManager::DefinitionStore* self = (const EmberOgre::Terrain::TerrainLayerDefinitionManager::DefinitionStore*)  tolua_tousertype(tolua_S,1,0);
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

/* method: getSingleton of class  EmberOgre::Terrain::TerrainLayerDefinitionManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinitionManager_getSingleton00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinitionManager_getSingleton00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Terrain::TerrainLayerDefinitionManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   EmberOgre::Terrain::TerrainLayerDefinitionManager& tolua_ret = (EmberOgre::Terrain::TerrainLayerDefinitionManager&)  EmberOgre::Terrain::TerrainLayerDefinitionManager::getSingleton();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"EmberOgre::Terrain::TerrainLayerDefinitionManager");
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

/* method: parseScript of class  EmberOgre::Terrain::TerrainLayerDefinitionManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinitionManager_parseScript00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinitionManager_parseScript00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Terrain::TerrainLayerDefinitionManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::DataStreamPtr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Ogre::String",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Terrain::TerrainLayerDefinitionManager* self = (EmberOgre::Terrain::TerrainLayerDefinitionManager*)  tolua_tousertype(tolua_S,1,0);
  Ogre::DataStreamPtr* stream = ((Ogre::DataStreamPtr*)  tolua_tousertype(tolua_S,2,0));
  const Ogre::String* groupName = ((const Ogre::String*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'parseScript'",NULL);
#endif
  {
   self->parseScript(*stream,*groupName);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'parseScript'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getDefinitions of class  EmberOgre::Terrain::TerrainLayerDefinitionManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinitionManager_getDefinitions00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinitionManager_getDefinitions00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Terrain::TerrainLayerDefinitionManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Terrain::TerrainLayerDefinitionManager* self = (const EmberOgre::Terrain::TerrainLayerDefinitionManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getDefinitions'",NULL);
#endif
  {
   const EmberOgre::Terrain::TerrainLayerDefinitionManager::DefinitionStore& tolua_ret = (const EmberOgre::Terrain::TerrainLayerDefinitionManager::DefinitionStore&)  self->getDefinitions();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const EmberOgre::Terrain::TerrainLayerDefinitionManager::DefinitionStore");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getDefinitions'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getDefinitionForArea of class  EmberOgre::Terrain::TerrainLayerDefinitionManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinitionManager_getDefinitionForArea00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinitionManager_getDefinitionForArea00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Terrain::TerrainLayerDefinitionManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Terrain::TerrainLayerDefinitionManager* self = (EmberOgre::Terrain::TerrainLayerDefinitionManager*)  tolua_tousertype(tolua_S,1,0);
  unsigned int areaIndex = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getDefinitionForArea'",NULL);
#endif
  {
   EmberOgre::Terrain::TerrainLayerDefinition* tolua_ret = (EmberOgre::Terrain::TerrainLayerDefinition*)  self->getDefinitionForArea(areaIndex);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Terrain::TerrainLayerDefinition");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getDefinitionForArea'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getDefinitionForShader of class  EmberOgre::Terrain::TerrainLayerDefinitionManager */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinitionManager_getDefinitionForShader00
static int tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinitionManager_getDefinitionForShader00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Terrain::TerrainLayerDefinitionManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Terrain::TerrainLayerDefinitionManager* self = (EmberOgre::Terrain::TerrainLayerDefinitionManager*)  tolua_tousertype(tolua_S,1,0);
  const std::string shaderType = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getDefinitionForShader'",NULL);
#endif
  {
   EmberOgre::Terrain::TerrainLayerDefinition* tolua_ret = (EmberOgre::Terrain::TerrainLayerDefinition*)  self->getDefinitionForShader(shaderType);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Terrain::TerrainLayerDefinition");
   tolua_pushcppstring(tolua_S,(const char*)shaderType);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getDefinitionForShader'.",&tolua_err);
 return 0;
#endif
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

/* method: getTerrainGenerator of class  EmberOgre::EmberOgre */
#ifndef TOLUA_DISABLE_tolua_EmberOgre_EmberOgre_EmberOgre_getTerrainGenerator00
static int tolua_EmberOgre_EmberOgre_EmberOgre_getTerrainGenerator00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTerrainGenerator'",NULL);
#endif
  {
   EmberOgre::Terrain::TerrainGenerator* tolua_ret = (EmberOgre::Terrain::TerrainGenerator*)  self->getTerrainGenerator();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Terrain::TerrainGenerator");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTerrainGenerator'.",&tolua_err);
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

/* Open function */
TOLUA_API int tolua_EmberOgre_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
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
    tolua_function(tolua_S,"getRootNode",tolua_EmberOgre_EmberOgre_AvatarCamera_getRootNode00);
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
    tolua_function(tolua_S,"setClientVisible",tolua_EmberOgre_EmberOgre_EmberEntity_setClientVisible00);
    tolua_function(tolua_S,"getEmberLocation",tolua_EmberOgre_EmberOgre_EmberEntity_getEmberLocation00);
    tolua_function(tolua_S,"isInitialized",tolua_EmberOgre_EmberOgre_EmberEntity_isInitialized00);
    tolua_function(tolua_S,"getMovementMode",tolua_EmberOgre_EmberOgre_EmberEntity_getMovementMode00);
    tolua_function(tolua_S,"showOgreBoundingBox",tolua_EmberOgre_EmberOgre_EmberEntity_showOgreBoundingBox00);
    tolua_function(tolua_S,"showErisBoundingBox",tolua_EmberOgre_EmberOgre_EmberEntity_showErisBoundingBox00);
    tolua_function(tolua_S,"getShowOgreBoundingBox",tolua_EmberOgre_EmberOgre_EmberEntity_getShowOgreBoundingBox00);
    tolua_function(tolua_S,"getShowErisBoundingBox",tolua_EmberOgre_EmberOgre_EmberEntity_getShowErisBoundingBox00);
    tolua_function(tolua_S,"getWorldBoundingBox",tolua_EmberOgre_EmberOgre_EmberEntity_getWorldBoundingBox00);
    tolua_function(tolua_S,"getDefaultUseOperators",tolua_EmberOgre_EmberOgre_EmberEntity_getDefaultUseOperators00);
    tolua_function(tolua_S,"getActions",tolua_EmberOgre_EmberOgre_EmberEntity_getActions00);
    tolua_function(tolua_S,"setVisualize",tolua_EmberOgre_EmberOgre_EmberEntity_setVisualize00);
    tolua_function(tolua_S,"getVisualize",tolua_EmberOgre_EmberOgre_EmberEntity_getVisualize00);
    tolua_function(tolua_S,"dumpAttributes",tolua_EmberOgre_EmberOgre_EmberEntity_dumpAttributes00);
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
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_module(tolua_S,"Terrain",0);
   tolua_beginmodule(tolua_S,"Terrain");
    tolua_cclass(tolua_S,"TerrainShader","EmberOgre::Terrain::TerrainShader","",NULL);
    tolua_beginmodule(tolua_S,"TerrainShader");
    tolua_endmodule(tolua_S);
    tolua_cclass(tolua_S,"TerrainPage","EmberOgre::Terrain::TerrainPage","",NULL);
    tolua_beginmodule(tolua_S,"TerrainPage");
    tolua_endmodule(tolua_S);
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
    tolua_cclass(tolua_S,"TerrainMod","EmberOgre::Terrain::TerrainMod","",NULL);
    tolua_beginmodule(tolua_S,"TerrainMod");
    tolua_endmodule(tolua_S);
    tolua_cclass(tolua_S,"TerrainLayerDefinition","EmberOgre::Terrain::TerrainLayerDefinition","",NULL);
    tolua_beginmodule(tolua_S,"TerrainLayerDefinition");
     tolua_function(tolua_S,"getName",tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinition_getName00);
     tolua_function(tolua_S,"getAreaId",tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinition_getAreaId00);
     tolua_function(tolua_S,"getDiffuseTextureName",tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinition_getDiffuseTextureName00);
     tolua_function(tolua_S,"getNormalMapTextureName",tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinition_getNormalMapTextureName00);
     tolua_function(tolua_S,"getShaderName",tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinition_getShaderName00);
     tolua_function(tolua_S,"getTileSize",tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinition_getTileSize00);
    tolua_endmodule(tolua_S);
    tolua_cclass(tolua_S,"TerrainPageSurfaceLayer","EmberOgre::Terrain::TerrainPageSurfaceLayer","",NULL);
    tolua_beginmodule(tolua_S,"TerrainPageSurfaceLayer");
    tolua_endmodule(tolua_S);
    tolua_cclass(tolua_S,"ISceneManagerAdapter","EmberOgre::Terrain::ISceneManagerAdapter","",NULL);
    tolua_beginmodule(tolua_S,"ISceneManagerAdapter");
    tolua_endmodule(tolua_S);
    #ifdef __cplusplus
    tolua_cclass(tolua_S,"TerrainDefPoint","EmberOgre::Terrain::TerrainDefPoint","",tolua_collect_EmberOgre__Terrain__TerrainDefPoint);
    #else
    tolua_cclass(tolua_S,"TerrainDefPoint","EmberOgre::Terrain::TerrainDefPoint","",NULL);
    #endif
    tolua_beginmodule(tolua_S,"TerrainDefPoint");
     tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_Terrain_TerrainDefPoint_new00);
     tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_Terrain_TerrainDefPoint_new00_local);
     tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_Terrain_TerrainDefPoint_new00_local);
     tolua_function(tolua_S,"getPosition",tolua_EmberOgre_EmberOgre_Terrain_TerrainDefPoint_getPosition00);
     tolua_function(tolua_S,"getHeight",tolua_EmberOgre_EmberOgre_Terrain_TerrainDefPoint_getHeight00);
    tolua_endmodule(tolua_S);
    tolua_cclass(tolua_S,"TerrainGenerator","EmberOgre::Terrain::TerrainGenerator","",NULL);
    tolua_beginmodule(tolua_S,"TerrainGenerator");
     tolua_function(tolua_S,"updateTerrain",tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_updateTerrain00);
     tolua_function(tolua_S,"isValidTerrainAt",tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_isValidTerrainAt00);
     tolua_function(tolua_S,"getTerrain",tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_getTerrain00);
     tolua_function(tolua_S,"getMax",tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_getMax00);
     tolua_function(tolua_S,"getMin",tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_getMin00);
     tolua_function(tolua_S,"getPageMetersSize",tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_getPageMetersSize00);
     tolua_function(tolua_S,"getPageIndexSize",tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_getPageIndexSize00);
     tolua_function(tolua_S,"addArea",tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_addArea00);
     tolua_function(tolua_S,"addTerrainMod",tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_addTerrainMod00);
     tolua_function(tolua_S,"getTerrainPageAtIndex",tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_getTerrainPageAtIndex00);
     tolua_function(tolua_S,"getTerrainPageAtPosition",tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_getTerrainPageAtPosition00);
     tolua_function(tolua_S,"getTerrainInfo",tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_getTerrainInfo00);
     tolua_variable(tolua_S,"EventWorldSizeChanged",tolua_get_EmberOgre__Terrain__TerrainGenerator_EventWorldSizeChanged,tolua_set_EmberOgre__Terrain__TerrainGenerator_EventWorldSizeChanged);
     tolua_function(tolua_S,"getAdapter",tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_getAdapter00);
     tolua_function(tolua_S,"getTerrainPages",tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_getTerrainPages00);
     tolua_function(tolua_S,"createShader",tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_createShader00);
     tolua_function(tolua_S,"updateShadows",tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_updateShadows00);
     tolua_variable(tolua_S,"UpdateShadows",tolua_get_EmberOgre__Terrain__TerrainGenerator_UpdateShadows,NULL);
     tolua_function(tolua_S,"getShadowColourAt",tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_getShadowColourAt00);
     tolua_function(tolua_S,"getShadowColourAt",tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_getShadowColourAt01);
     tolua_variable(tolua_S,"EventLayerUpdated",tolua_get_EmberOgre__Terrain__TerrainGenerator_EventLayerUpdated,tolua_set_EmberOgre__Terrain__TerrainGenerator_EventLayerUpdated);
     tolua_variable(tolua_S,"EventShaderCreated",tolua_get_EmberOgre__Terrain__TerrainGenerator_EventShaderCreated,tolua_set_EmberOgre__Terrain__TerrainGenerator_EventShaderCreated);
     tolua_variable(tolua_S,"EventTerrainPageGeometryUpdated",tolua_get_EmberOgre__Terrain__TerrainGenerator_EventTerrainPageGeometryUpdated,tolua_set_EmberOgre__Terrain__TerrainGenerator_EventTerrainPageGeometryUpdated);
     tolua_function(tolua_S,"getFoliageBatchSize",tolua_EmberOgre_EmberOgre_Terrain_TerrainGenerator_getFoliageBatchSize00);
    tolua_endmodule(tolua_S);
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
     tolua_function(tolua_S,"getRadius",tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_getRadius00);
     tolua_function(tolua_S,"setRadius",tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_setRadius00);
     tolua_function(tolua_S,"getFalloff",tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_getFalloff00);
     tolua_function(tolua_S,"seFalloff",tolua_EmberOgre_EmberOgre_Terrain_TerrainEditor_seFalloff00);
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
    tolua_endmodule(tolua_S);
    tolua_function(tolua_S,"getSingleton",tolua_EmberOgre_EmberOgre_MotionManager_getSingleton00);
    tolua_function(tolua_S,"addEntity",tolua_EmberOgre_EmberOgre_MotionManager_addEntity00);
    tolua_function(tolua_S,"removeEntity",tolua_EmberOgre_EmberOgre_MotionManager_removeEntity00);
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
     tolua_variable(tolua_S,"EventFirstTimeShown",tolua_get_EmberOgre__Gui__Widget_EventFirstTimeShown,tolua_set_EmberOgre__Gui__Widget_EventFirstTimeShown);
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
     tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_Gui_IconBase_new01);
     tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_Gui_IconBase_new01_local);
     tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_Gui_IconBase_new01_local);
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
    tolua_cclass(tolua_S,"ColouredTreeItem","EmberOgre::Gui::ColouredTreeItem","CEGUI::TreeItem",NULL);
    tolua_beginmodule(tolua_S,"ColouredTreeItem");
     tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_Gui_ColouredTreeItem_new00);
     tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_Gui_ColouredTreeItem_new01);
     tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_Gui_ColouredTreeItem_new02);
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
     tolua_function(tolua_S,"materialAsText",tolua_EmberOgre_EmberOgre_Gui_AssetsManager_materialAsText00);
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
     tolua_function(tolua_S,"refresh",tolua_EmberOgre_EmberOgre_Gui_Compass_refresh00);
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
    tolua_endmodule(tolua_S);
    #ifdef __cplusplus
    tolua_cclass(tolua_S,"RenderedCompassImpl","EmberOgre::Gui::RenderedCompassImpl","EmberOgre::Gui::ICompassImpl",tolua_collect_EmberOgre__Gui__RenderedCompassImpl);
    #else
    tolua_cclass(tolua_S,"RenderedCompassImpl","EmberOgre::Gui::RenderedCompassImpl","EmberOgre::Gui::ICompassImpl",NULL);
    #endif
    tolua_beginmodule(tolua_S,"RenderedCompassImpl");
     tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_Gui_RenderedCompassImpl_new00);
     tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_Gui_RenderedCompassImpl_new00_local);
     tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_Gui_RenderedCompassImpl_new00_local);
     tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_Gui_RenderedCompassImpl_new01);
     tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_Gui_RenderedCompassImpl_new01_local);
     tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_Gui_RenderedCompassImpl_new01_local);
     tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_Gui_RenderedCompassImpl_new02);
     tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_Gui_RenderedCompassImpl_new02_local);
     tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_Gui_RenderedCompassImpl_new02_local);
     tolua_function(tolua_S,"delete",tolua_EmberOgre_EmberOgre_Gui_RenderedCompassImpl_delete00);
     tolua_function(tolua_S,"getTexture",tolua_EmberOgre_EmberOgre_Gui_RenderedCompassImpl_getTexture00);
     tolua_function(tolua_S,"getPointerTexture",tolua_EmberOgre_EmberOgre_Gui_RenderedCompassImpl_getPointerTexture00);
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
    #ifdef __cplusplus
    tolua_cclass(tolua_S,"CompassThirdPersonCameraAnchor","EmberOgre::Gui::CompassThirdPersonCameraAnchor","",tolua_collect_EmberOgre__Gui__CompassThirdPersonCameraAnchor);
    #else
    tolua_cclass(tolua_S,"CompassThirdPersonCameraAnchor","EmberOgre::Gui::CompassThirdPersonCameraAnchor","",NULL);
    #endif
    tolua_beginmodule(tolua_S,"CompassThirdPersonCameraAnchor");
     tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_Gui_CompassThirdPersonCameraAnchor_new00);
     tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_Gui_CompassThirdPersonCameraAnchor_new00_local);
     tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_Gui_CompassThirdPersonCameraAnchor_new00_local);
     tolua_function(tolua_S,"delete",tolua_EmberOgre_EmberOgre_Gui_CompassThirdPersonCameraAnchor_delete00);
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
     tolua_function(tolua_S,".eq",tolua_EmberOgre_EmberOgre_Gui_EntityIcon__eq00);
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
     tolua_function(tolua_S,"setDisplaySkeleton",tolua_EmberOgre_EmberOgre_Model_Model_setDisplaySkeleton00);
     tolua_function(tolua_S,"getDisplaySkeleton",tolua_EmberOgre_EmberOgre_Model_Model_getDisplaySkeleton00);
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
     tolua_function(tolua_S,"setShowContained",tolua_EmberOgre_EmberOgre_Model_ModelDefinition_setShowContained00);
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
   tolua_cclass(tolua_S,"TerrainPosition","EmberOgre::TerrainPosition","",NULL);
   tolua_beginmodule(tolua_S,"TerrainPosition");
    tolua_function(tolua_S,"isValid",tolua_EmberOgre_EmberOgre_TerrainPosition_isValid00);
    tolua_function(tolua_S,"setToOrigin",tolua_EmberOgre_EmberOgre_TerrainPosition_setToOrigin00);
    tolua_function(tolua_S,"x",tolua_EmberOgre_EmberOgre_TerrainPosition_x00);
    tolua_function(tolua_S,"x",tolua_EmberOgre_EmberOgre_TerrainPosition_x01);
    tolua_function(tolua_S,"y",tolua_EmberOgre_EmberOgre_TerrainPosition_y00);
    tolua_function(tolua_S,"y",tolua_EmberOgre_EmberOgre_TerrainPosition_y01);
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"Convert","EmberOgre::Convert","",NULL);
   tolua_beginmodule(tolua_S,"Convert");
    tolua_function(tolua_S,"toOgre_Vector3",tolua_EmberOgre_EmberOgre_Convert_toOgre_Vector300);
    tolua_function(tolua_S,"toOgre_Vector2",tolua_EmberOgre_EmberOgre_Convert_toOgre_Vector200);
    tolua_function(tolua_S,"toOgre",tolua_EmberOgre_EmberOgre_Convert_toOgre00);
    tolua_function(tolua_S,"toWF_Point3",tolua_EmberOgre_EmberOgre_Convert_toWF_Point300);
    tolua_function(tolua_S,"toWF_Point2",tolua_EmberOgre_EmberOgre_Convert_toWF_Point200);
    tolua_function(tolua_S,"toWF_Vector3",tolua_EmberOgre_EmberOgre_Convert_toWF_Vector300);
    tolua_function(tolua_S,"toWF",tolua_EmberOgre_EmberOgre_Convert_toWF00);
    tolua_function(tolua_S,"toOgre",tolua_EmberOgre_EmberOgre_Convert_toOgre01);
    tolua_function(tolua_S,"toOgre",tolua_EmberOgre_EmberOgre_Convert_toOgre02);
    tolua_function(tolua_S,"toOgre",tolua_EmberOgre_EmberOgre_Convert_toOgre03);
    tolua_function(tolua_S,"toWF",tolua_EmberOgre_EmberOgre_Convert_toWF01);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"SimpleRenderContext","EmberOgre::SimpleRenderContext","",tolua_collect_EmberOgre__SimpleRenderContext);
   #else
   tolua_cclass(tolua_S,"SimpleRenderContext","EmberOgre::SimpleRenderContext","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"SimpleRenderContext");
    tolua_constant(tolua_S,"CPM_OBJECTCENTER",EmberOgre::SimpleRenderContext::CPM_OBJECTCENTER);
    tolua_constant(tolua_S,"CPM_WORLDCENTER",EmberOgre::SimpleRenderContext::CPM_WORLDCENTER);
    tolua_constant(tolua_S,"CPM_FREE",EmberOgre::SimpleRenderContext::CPM_FREE);
    tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_SimpleRenderContext_new00);
    tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_SimpleRenderContext_new00_local);
    tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_SimpleRenderContext_new00_local);
    tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_SimpleRenderContext_new01);
    tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_SimpleRenderContext_new01_local);
    tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_SimpleRenderContext_new01_local);
    tolua_function(tolua_S,"getSceneNode",tolua_EmberOgre_EmberOgre_SimpleRenderContext_getSceneNode00);
    tolua_function(tolua_S,"getCamera",tolua_EmberOgre_EmberOgre_SimpleRenderContext_getCamera00);
    tolua_function(tolua_S,"getDefaultCameraDistance",tolua_EmberOgre_EmberOgre_SimpleRenderContext_getDefaultCameraDistance00);
    tolua_function(tolua_S,"setActive",tolua_EmberOgre_EmberOgre_SimpleRenderContext_setActive00);
    tolua_function(tolua_S,"showFull",tolua_EmberOgre_EmberOgre_SimpleRenderContext_showFull00);
    tolua_function(tolua_S,"repositionCamera",tolua_EmberOgre_EmberOgre_SimpleRenderContext_repositionCamera00);
    tolua_function(tolua_S,"pitch",tolua_EmberOgre_EmberOgre_SimpleRenderContext_pitch00);
    tolua_function(tolua_S,"yaw",tolua_EmberOgre_EmberOgre_SimpleRenderContext_yaw00);
    tolua_function(tolua_S,"roll",tolua_EmberOgre_EmberOgre_SimpleRenderContext_roll00);
    tolua_function(tolua_S,"setCameraDistance",tolua_EmberOgre_EmberOgre_SimpleRenderContext_setCameraDistance00);
    tolua_function(tolua_S,"getCameraDistance",tolua_EmberOgre_EmberOgre_SimpleRenderContext_getCameraDistance00);
    tolua_function(tolua_S,"getAbsoluteCameraDistance",tolua_EmberOgre_EmberOgre_SimpleRenderContext_getAbsoluteCameraDistance00);
    tolua_function(tolua_S,"getEntityRotation",tolua_EmberOgre_EmberOgre_SimpleRenderContext_getEntityRotation00);
    tolua_function(tolua_S,"resetCameraOrientation",tolua_EmberOgre_EmberOgre_SimpleRenderContext_resetCameraOrientation00);
    tolua_function(tolua_S,"getSceneManager",tolua_EmberOgre_EmberOgre_SimpleRenderContext_getSceneManager00);
    tolua_function(tolua_S,"getRenderTexture",tolua_EmberOgre_EmberOgre_SimpleRenderContext_getRenderTexture00);
    tolua_function(tolua_S,"getTexture",tolua_EmberOgre_EmberOgre_SimpleRenderContext_getTexture00);
    tolua_function(tolua_S,"getCameraRootNode",tolua_EmberOgre_EmberOgre_SimpleRenderContext_getCameraRootNode00);
    tolua_function(tolua_S,"getViewport",tolua_EmberOgre_EmberOgre_SimpleRenderContext_getViewport00);
    tolua_function(tolua_S,"getLight",tolua_EmberOgre_EmberOgre_SimpleRenderContext_getLight00);
    tolua_function(tolua_S,"setBackgroundColour",tolua_EmberOgre_EmberOgre_SimpleRenderContext_setBackgroundColour00);
    tolua_function(tolua_S,"setBackgroundColour",tolua_EmberOgre_EmberOgre_SimpleRenderContext_setBackgroundColour01);
    tolua_function(tolua_S,"setTexture",tolua_EmberOgre_EmberOgre_SimpleRenderContext_setTexture00);
    tolua_function(tolua_S,"getCameraPositionMode",tolua_EmberOgre_EmberOgre_SimpleRenderContext_getCameraPositionMode00);
    tolua_function(tolua_S,"setCameraPositionMode",tolua_EmberOgre_EmberOgre_SimpleRenderContext_setCameraPositionMode00);
   tolua_endmodule(tolua_S);
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
     tolua_function(tolua_S,"showAxis",tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_showAxis00);
     tolua_function(tolua_S,"hideAxis",tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_hideAxis00);
     tolua_function(tolua_S,"getCameraPositionMode",tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_getCameraPositionMode00);
     tolua_function(tolua_S,"setCameraPositionMode",tolua_EmberOgre_EmberOgre_Gui_MovableObjectRenderer_setCameraPositionMode00);
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
     tolua_function(tolua_S,"repositionSceneNode",tolua_EmberOgre_EmberOgre_Gui_ModelRenderer_repositionSceneNode00);
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
    tolua_cclass(tolua_S,"ModelDefinitionManager","EmberOgre::Model::ModelDefinitionManager","Ogre::ResourceManager",NULL);
    tolua_beginmodule(tolua_S,"ModelDefinitionManager");
     tolua_function(tolua_S,"getSingleton",tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_getSingleton00);
     tolua_function(tolua_S,"exportScript",tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_exportScript00);
     tolua_function(tolua_S,"create",tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_create00);
     tolua_function(tolua_S,"getByName",tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_getByName00);
     tolua_function(tolua_S,"getAllMeshes",tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_getAllMeshes00);
     tolua_function(tolua_S,"getShowModels",tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_getShowModels00);
     tolua_function(tolua_S,"setShowModels",tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_setShowModels00);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_module(tolua_S,"Model",0);
   tolua_beginmodule(tolua_S,"Model");
    #ifdef __cplusplus
    tolua_cclass(tolua_S,"ModelDefinitionAtlasComposer","EmberOgre::Model::ModelDefinitionAtlasComposer","",tolua_collect_EmberOgre__Model__ModelDefinitionAtlasComposer);
    #else
    tolua_cclass(tolua_S,"ModelDefinitionAtlasComposer","EmberOgre::Model::ModelDefinitionAtlasComposer","",NULL);
    #endif
    tolua_beginmodule(tolua_S,"ModelDefinitionAtlasComposer");
     tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_Model_ModelDefinitionAtlasComposer_new00);
     tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_Model_ModelDefinitionAtlasComposer_new00_local);
     tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_Model_ModelDefinitionAtlasComposer_new00_local);
     tolua_function(tolua_S,"delete",tolua_EmberOgre_EmberOgre_Model_ModelDefinitionAtlasComposer_delete00);
     tolua_function(tolua_S,"compose",tolua_EmberOgre_EmberOgre_Model_ModelDefinitionAtlasComposer_compose00);
     tolua_function(tolua_S,"composeToStream",tolua_EmberOgre_EmberOgre_Model_ModelDefinitionAtlasComposer_composeToStream00);
     tolua_function(tolua_S,"composeToFile",tolua_EmberOgre_EmberOgre_Model_ModelDefinitionAtlasComposer_composeToFile00);
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
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"GUIAdapter","EmberOgre::GUIAdapter","",tolua_collect_EmberOgre__GUIAdapter);
   #else
   tolua_cclass(tolua_S,"GUIAdapter","EmberOgre::GUIAdapter","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"GUIAdapter");
    tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_GUIAdapter_new00);
    tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_GUIAdapter_new00_local);
    tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_GUIAdapter_new00_local);
    tolua_function(tolua_S,"delete",tolua_EmberOgre_EmberOgre_GUIAdapter_delete00);
    tolua_function(tolua_S,"getType",tolua_EmberOgre_EmberOgre_GUIAdapter_getType00);
    tolua_function(tolua_S,"attach",tolua_EmberOgre_EmberOgre_GUIAdapter_attach00);
    tolua_function(tolua_S,"detach",tolua_EmberOgre_EmberOgre_GUIAdapter_detach00);
    tolua_function(tolua_S,"getValue",tolua_EmberOgre_EmberOgre_GUIAdapter_getValue00);
    tolua_function(tolua_S,"setTitle",tolua_EmberOgre_EmberOgre_GUIAdapter_setTitle00);
    tolua_function(tolua_S,"getTitle",tolua_EmberOgre_EmberOgre_GUIAdapter_getTitle00);
    tolua_function(tolua_S,"setTooltip",tolua_EmberOgre_EmberOgre_GUIAdapter_setTooltip00);
    tolua_function(tolua_S,"getTooltip",tolua_EmberOgre_EmberOgre_GUIAdapter_getTooltip00);
    tolua_function(tolua_S,"addSuggestion",tolua_EmberOgre_EmberOgre_GUIAdapter_addSuggestion00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"EntityRecipe","EmberOgre::EntityRecipe","Ogre::Resource",tolua_collect_EmberOgre__EntityRecipe);
   #else
   tolua_cclass(tolua_S,"EntityRecipe","EmberOgre::EntityRecipe","Ogre::Resource",NULL);
   #endif
   tolua_beginmodule(tolua_S,"EntityRecipe");
    tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_EntityRecipe_new00);
    tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_EntityRecipe_new00_local);
    tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_EntityRecipe_new00_local);
    tolua_function(tolua_S,"delete",tolua_EmberOgre_EmberOgre_EntityRecipe_delete00);
    tolua_function(tolua_S,"loadImpl",tolua_EmberOgre_EmberOgre_EntityRecipe_loadImpl00);
    tolua_function(tolua_S,"unloadImpl",tolua_EmberOgre_EmberOgre_EntityRecipe_unloadImpl00);
    tolua_function(tolua_S,"calculateSize",tolua_EmberOgre_EmberOgre_EntityRecipe_calculateSize00);
    tolua_function(tolua_S,"getGUIAdapters",tolua_EmberOgre_EmberOgre_EntityRecipe_getGUIAdapters00);
    tolua_function(tolua_S,"setAuthor",tolua_EmberOgre_EmberOgre_EntityRecipe_setAuthor00);
    tolua_function(tolua_S,"getAuthor",tolua_EmberOgre_EmberOgre_EntityRecipe_getAuthor00);
    tolua_function(tolua_S,"setDescription",tolua_EmberOgre_EmberOgre_EntityRecipe_setDescription00);
    tolua_function(tolua_S,"getDescription",tolua_EmberOgre_EmberOgre_EntityRecipe_getDescription00);
    tolua_function(tolua_S,"createEntity",tolua_EmberOgre_EmberOgre_EntityRecipe_createEntity00);
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"EntityRecipePtr","EmberOgre::EntityRecipePtr","",NULL);
   tolua_beginmodule(tolua_S,"EntityRecipePtr");
    tolua_function(tolua_S,"get",tolua_EmberOgre_EmberOgre_EntityRecipePtr_get00);
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"GUIAdaptersStore","EmberOgre::GUIAdaptersStore","",NULL);
   tolua_beginmodule(tolua_S,"GUIAdaptersStore");
    tolua_cclass(tolua_S,"value_type","EmberOgre::GUIAdaptersStore::value_type","",NULL);
    tolua_beginmodule(tolua_S,"value_type");
     tolua_variable(tolua_S,"first",tolua_get_EmberOgre__GUIAdaptersStore__value_type_first,NULL);
     tolua_variable(tolua_S,"second",tolua_get_EmberOgre__GUIAdaptersStore__value_type_second_ptr,tolua_set_EmberOgre__GUIAdaptersStore__value_type_second_ptr);
    tolua_endmodule(tolua_S);
    #ifdef __cplusplus
    tolua_cclass(tolua_S,"const_iterator","EmberOgre::GUIAdaptersStore::const_iterator","",tolua_collect_EmberOgre__GUIAdaptersStore__const_iterator);
    #else
    tolua_cclass(tolua_S,"const_iterator","EmberOgre::GUIAdaptersStore::const_iterator","",NULL);
    #endif
    tolua_beginmodule(tolua_S,"const_iterator");
     tolua_function(tolua_S,"get",tolua_EmberOgre_EmberOgre_GUIAdaptersStore_const_iterator_get00);
     tolua_function(tolua_S,"next",tolua_EmberOgre_EmberOgre_GUIAdaptersStore_const_iterator_next00);
     tolua_function(tolua_S,".eq",tolua_EmberOgre_EmberOgre_GUIAdaptersStore_const_iterator__eq00);
    tolua_endmodule(tolua_S);
    tolua_function(tolua_S,"begin",tolua_EmberOgre_EmberOgre_GUIAdaptersStore_begin00);
    tolua_function(tolua_S,"theEnd",tolua_EmberOgre_EmberOgre_GUIAdaptersStore_theEnd00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);

  { /* begin embedded lua code */
   int top = lua_gettop(tolua_S);
   static unsigned char B[] = {
    10,102,117,110, 99,116,105,111,110, 32, 69,109, 98,101,114,
     79,103,114,101, 46, 71, 85, 73, 65,100, 97,112,116,101,114,
    115, 83,116,111,114,101, 58,112, 97,105,114,115, 40, 41, 10,
    108,111, 99, 97,108, 32, 73, 32, 61, 32,115,101,108,102, 58,
     98,101,103,105,110, 40, 41, 10,108,111, 99, 97,108, 32, 73,
    101,110,100, 32, 61, 32,115,101,108,102, 58,116,104,101, 69,
    110,100, 40, 41, 10,114,101,116,117,114,110, 32,102,117,110,
     99,116,105,111,110, 40, 41, 10,105,102, 32, 73, 32,126, 61,
     32, 73,101,110,100, 32,116,104,101,110, 10,108,111, 99, 97,
    108, 32,118, 97,108, 32, 61, 32, 73, 58,103,101,116, 40, 41,
     10, 73, 32, 61, 32, 73, 58,110,101,120,116, 40, 41, 10,114,
    101,116,117,114,110, 32,118, 97,108, 46,102,105,114,115,116,
     44, 32,118, 97,108, 46,115,101, 99,111,110,100, 10,101,110,
    100, 10,101,110,100, 10,101,110,100,32
   };
   tolua_dobuffer(tolua_S,(char*)B,sizeof(B),"tolua: embedded Lua code 1");
   lua_settop(tolua_S, top);
  } /* end of embedded lua code */

  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"EntityRecipeManager","EmberOgre::EntityRecipeManager","Ogre::ResourceManager",tolua_collect_EmberOgre__EntityRecipeManager);
   #else
   tolua_cclass(tolua_S,"EntityRecipeManager","EmberOgre::EntityRecipeManager","Ogre::ResourceManager",NULL);
   #endif
   tolua_beginmodule(tolua_S,"EntityRecipeManager");
    tolua_function(tolua_S,"getSingleton",tolua_EmberOgre_EmberOgre_EntityRecipeManager_getSingleton00);
    tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_EntityRecipeManager_new00);
    tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_EntityRecipeManager_new00_local);
    tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_EntityRecipeManager_new00_local);
    tolua_function(tolua_S,"delete",tolua_EmberOgre_EmberOgre_EntityRecipeManager_delete00);
    tolua_function(tolua_S,"create",tolua_EmberOgre_EmberOgre_EntityRecipeManager_create00);
    tolua_function(tolua_S,"parseScript",tolua_EmberOgre_EmberOgre_EntityRecipeManager_parseScript00);
    tolua_variable(tolua_S,"__Ember__Singleton_EntityRecipeManager___",tolua_get_EmberOgre__EntityRecipeManager___Ember__Singleton_EntityRecipeManager___,NULL);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_module(tolua_S,"Gui",0);
   tolua_beginmodule(tolua_S,"Gui");
    #ifdef __cplusplus
    tolua_cclass(tolua_S,"EntityCreator","EmberOgre::Gui::EntityCreator","",tolua_collect_EmberOgre__Gui__EntityCreator);
    #else
    tolua_cclass(tolua_S,"EntityCreator","EmberOgre::Gui::EntityCreator","",NULL);
    #endif
    tolua_beginmodule(tolua_S,"EntityCreator");
     tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_Gui_EntityCreator_new00);
     tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_Gui_EntityCreator_new00_local);
     tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_Gui_EntityCreator_new00_local);
     tolua_function(tolua_S,"delete",tolua_EmberOgre_EmberOgre_Gui_EntityCreator_delete00);
     tolua_function(tolua_S,"setRecipe",tolua_EmberOgre_EmberOgre_Gui_EntityCreator_setRecipe00);
     tolua_function(tolua_S,"toggleCreateMode",tolua_EmberOgre_EmberOgre_Gui_EntityCreator_toggleCreateMode00);
     tolua_function(tolua_S,"setRandomizeOrientation",tolua_EmberOgre_EmberOgre_Gui_EntityCreator_setRandomizeOrientation00);
     tolua_function(tolua_S,"createEntity",tolua_EmberOgre_EmberOgre_Gui_EntityCreator_createEntity00);
     tolua_function(tolua_S,"cleanupCreation",tolua_EmberOgre_EmberOgre_Gui_EntityCreator_cleanupCreation00);
     tolua_variable(tolua_S,"mWidget",tolua_get_EmberOgre__Gui__EntityCreator_mWidget_ptr,tolua_set_EmberOgre__Gui__EntityCreator_mWidget_ptr);
     tolua_variable(tolua_S,"EventTypeInfoLoaded",tolua_get_EmberOgre__Gui__EntityCreator_EventTypeInfoLoaded,tolua_set_EmberOgre__Gui__EntityCreator_EventTypeInfoLoaded);
    tolua_endmodule(tolua_S);
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
     tolua_function(tolua_S,"setResolution",tolua_EmberOgre_EmberOgre_Terrain_Map_setResolution00);
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
   tolua_module(tolua_S,"Terrain",0);
   tolua_beginmodule(tolua_S,"Terrain");
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_module(tolua_S,"Terrain",0);
   tolua_beginmodule(tolua_S,"Terrain");
    tolua_cclass(tolua_S,"TerrainLayerDefinitionManager","EmberOgre::Terrain::TerrainLayerDefinitionManager","Ogre::ResourceManager",NULL);
    tolua_beginmodule(tolua_S,"TerrainLayerDefinitionManager");
     tolua_cclass(tolua_S,"DefinitionStore","EmberOgre::Terrain::TerrainLayerDefinitionManager::DefinitionStore","",NULL);
     tolua_beginmodule(tolua_S,"DefinitionStore");
      #ifdef __cplusplus
      tolua_cclass(tolua_S,"const_iterator","EmberOgre::Terrain::TerrainLayerDefinitionManager::DefinitionStore::const_iterator","",tolua_collect_EmberOgre__Terrain__TerrainLayerDefinitionManager__DefinitionStore__const_iterator);
      #else
      tolua_cclass(tolua_S,"const_iterator","EmberOgre::Terrain::TerrainLayerDefinitionManager::DefinitionStore::const_iterator","",NULL);
      #endif
      tolua_beginmodule(tolua_S,"const_iterator");
       tolua_function(tolua_S,"get",tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinitionManager_DefinitionStore_const_iterator_get00);
       tolua_function(tolua_S,"next",tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinitionManager_DefinitionStore_const_iterator_next00);
      tolua_endmodule(tolua_S);
      tolua_function(tolua_S,"begin",tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinitionManager_DefinitionStore_begin00);
      tolua_function(tolua_S,"theEnd",tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinitionManager_DefinitionStore_theEnd00);
      tolua_function(tolua_S,"size",tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinitionManager_DefinitionStore_size00);
     tolua_endmodule(tolua_S);
     tolua_function(tolua_S,"getSingleton",tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinitionManager_getSingleton00);
     tolua_function(tolua_S,"parseScript",tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinitionManager_parseScript00);
     tolua_function(tolua_S,"getDefinitions",tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinitionManager_getDefinitions00);
     tolua_function(tolua_S,"getDefinitionForArea",tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinitionManager_getDefinitionForArea00);
     tolua_function(tolua_S,"getDefinitionForShader",tolua_EmberOgre_EmberOgre_Terrain_TerrainLayerDefinitionManager_getDefinitionForShader00);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);

  { /* begin embedded lua code */
   int top = lua_gettop(tolua_S);
   static unsigned char B[] = {
    10,102,117,110, 99,116,105,111,110, 32, 69,109, 98,101,114,
     79,103,114,101, 46, 84,101,114,114, 97,105,110, 46, 84,101,
    114,114, 97,105,110, 76, 97,121,101,114, 68,101,102,105,110,
    105,116,105,111,110, 77, 97,110, 97,103,101,114, 46, 68,101,
    102,105,110,105,116,105,111,110, 83,116,111,114,101, 58,105,
    112, 97,105,114,115, 40, 41, 10,108,111, 99, 97,108, 32, 73,
     32, 61, 32,115,101,108,102, 58, 98,101,103,105,110, 40, 41,
     10,108,111, 99, 97,108, 32, 73,101,110,100, 32, 61, 32,115,
    101,108,102, 58,116,104,101, 69,110,100, 40, 41, 10,108,111,
     99, 97,108, 32,105,110,100,101,120, 32, 61, 32, 45, 49, 59,
     10,114,101,116,117,114,110, 32,102,117,110, 99,116,105,111,
    110, 40, 41, 10,105,102, 32,105,110,100,101,120, 32, 43, 32,
     49, 32, 60, 32,115,101,108,102, 58,115,105,122,101, 40, 41,
     32,116,104,101,110, 10,108,111, 99, 97,108, 32,118, 97,108,
     32, 61, 32, 73, 58,103,101,116, 40, 41, 10,105,110,100,101,
    120, 32, 61, 32,105,110,100,101,120, 32, 43, 32, 49, 10, 73,
     32, 61, 32, 73, 58,110,101,120,116, 40, 41, 10,114,101,116,
    117,114,110, 32,105,110,100,101,120, 44, 32,118, 97,108, 10,
    101,110,100, 10,101,110,100, 10,101,110,100,32
   };
   tolua_dobuffer(tolua_S,(char*)B,sizeof(B),"tolua: embedded Lua code 2");
   lua_settop(tolua_S, top);
  } /* end of embedded lua code */

  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_cclass(tolua_S,"EmberOgre","EmberOgre::EmberOgre","",NULL);
   tolua_beginmodule(tolua_S,"EmberOgre");
    tolua_function(tolua_S,"getSingleton",tolua_EmberOgre_EmberOgre_EmberOgre_getSingleton00);
    tolua_function(tolua_S,"getAvatar",tolua_EmberOgre_EmberOgre_EmberOgre_getAvatar00);
    tolua_function(tolua_S,"getTerrainGenerator",tolua_EmberOgre_EmberOgre_EmberOgre_getTerrainGenerator00);
    tolua_function(tolua_S,"getMotionManager",tolua_EmberOgre_EmberOgre_EmberOgre_getMotionManager00);
    tolua_function(tolua_S,"getEntityFactory",tolua_EmberOgre_EmberOgre_EmberOgre_getEntityFactory00);
    tolua_function(tolua_S,"getMainCamera",tolua_EmberOgre_EmberOgre_EmberOgre_getMainCamera00);
    tolua_function(tolua_S,"getAvatarController",tolua_EmberOgre_EmberOgre_EmberOgre_getAvatarController00);
    tolua_function(tolua_S,"getMoveManager",tolua_EmberOgre_EmberOgre_EmberOgre_getMoveManager00);
    tolua_function(tolua_S,"getEmberEntity",tolua_EmberOgre_EmberOgre_EmberOgre_getEmberEntity00);
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

