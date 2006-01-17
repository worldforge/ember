/*
** Lua binding: EmberOgre
** Generated automatically by tolua++-1.0.6 on Sun Jan 15 16:07:04 2006.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int tolua_EmberOgre_open (lua_State* tolua_S);

#include "required.h"
#include "components/ogre/carpenter/Carpenter.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_WFMath__Point_3_ (lua_State* tolua_S)
{
 WFMath::Point<3>* self = (WFMath::Point<3>*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_EmberOgre__Jesus (lua_State* tolua_S)
{
 EmberOgre::Jesus* self = (EmberOgre::Jesus*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_EmberOgre__AttachPointNode (lua_State* tolua_S)
{
 EmberOgre::AttachPointNode* self = (EmberOgre::AttachPointNode*) tolua_tousertype(tolua_S,1,0);
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

static int tolua_collect_WFMath__Point_2_ (lua_State* tolua_S)
{
 WFMath::Point<2>* self = (WFMath::Point<2>*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_EmberOgre__TerrainArea (lua_State* tolua_S)
{
 EmberOgre::TerrainArea* self = (EmberOgre::TerrainArea*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_Ogre__Degree (lua_State* tolua_S)
{
 Ogre::Degree* self = (Ogre::Degree*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_std__vector_std__string_ (lua_State* tolua_S)
{
 std::vector<std::string>* self = (std::vector<std::string>*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_EmberOgre__ColoredListItem (lua_State* tolua_S)
{
 EmberOgre::ColoredListItem* self = (EmberOgre::ColoredListItem*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_EmberOgre__LightWibbler (lua_State* tolua_S)
{
 EmberOgre::LightWibbler* self = (EmberOgre::LightWibbler*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_Ogre__MapIterator_Ogre__ResourceManager__ResourceHandleMap_ (lua_State* tolua_S)
{
 Ogre::MapIterator<Ogre::ResourceManager::ResourceHandleMap>* self = (Ogre::MapIterator<Ogre::ResourceManager::ResourceHandleMap>*) tolua_tousertype(tolua_S,1,0);
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

static int tolua_collect_EmberOgre__Construction (lua_State* tolua_S)
{
 EmberOgre::Construction* self = (EmberOgre::Construction*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_Ogre__Quaternion (lua_State* tolua_S)
{
 Ogre::Quaternion* self = (Ogre::Quaternion*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_EmberOgre__Widget (lua_State* tolua_S)
{
 EmberOgre::Widget* self = (EmberOgre::Widget*) tolua_tousertype(tolua_S,1,0);
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

static int tolua_collect_std__vector_EmberOgre__Model__SubModelDefinition__ (lua_State* tolua_S)
{
 std::vector<EmberOgre::Model::SubModelDefinition*>* self = (std::vector<EmberOgre::Model::SubModelDefinition*>*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_EmberOgre__Model__ModelDefnPtr (lua_State* tolua_S)
{
 EmberOgre::Model::ModelDefnPtr* self = (EmberOgre::Model::ModelDefnPtr*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_EmberOgre__ModelRenderer (lua_State* tolua_S)
{
 EmberOgre::ModelRenderer* self = (EmberOgre::ModelRenderer*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_WFMath__Vector_3_ (lua_State* tolua_S)
{
 WFMath::Vector<3>* self = (WFMath::Vector<3>*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_Ogre__Vector3 (lua_State* tolua_S)
{
 Ogre::Vector3* self = (Ogre::Vector3*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_EmberOgre__ModelBlock (lua_State* tolua_S)
{
 EmberOgre::ModelBlock* self = (EmberOgre::ModelBlock*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_Carpenter__AttachPoint (lua_State* tolua_S)
{
 Carpenter::AttachPoint* self = (Carpenter::AttachPoint*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_std__vector_EmberOgre__ModelBlock__ (lua_State* tolua_S)
{
 std::vector<EmberOgre::ModelBlock*>* self = (std::vector<EmberOgre::ModelBlock*>*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_Carpenter__Carpenter (lua_State* tolua_S)
{
 Carpenter::Carpenter* self = (Carpenter::Carpenter*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_std__vector_const_Carpenter__AttachPoint__ (lua_State* tolua_S)
{
 std::vector<const Carpenter::AttachPoint*>* self = (std::vector<const Carpenter::AttachPoint*>*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_std__vector_EmberOgre__Model__PartDefinition__ (lua_State* tolua_S)
{
 std::vector<EmberOgre::Model::PartDefinition*>* self = (std::vector<EmberOgre::Model::PartDefinition*>*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"std::vector<EmberOgre::Model::SubModelDefinition*>");
 tolua_usertype(tolua_S,"Carpenter::BlockSpec");
 tolua_usertype(tolua_S,"EmberOgre::AvatarEmberEntity");
 tolua_usertype(tolua_S,"Carpenter::BluePrint");
 tolua_usertype(tolua_S,"std::map<const std::string,Carpenter::BlockSpec>");
 tolua_usertype(tolua_S,"Carpenter::AttachPair");
 tolua_usertype(tolua_S,"EmberOgre::GUIManager");
 tolua_usertype(tolua_S,"sigc::signal<void,bool&>");
 tolua_usertype(tolua_S,"const");
 tolua_usertype(tolua_S,"EmberOgre::TerrainArea");
 tolua_usertype(tolua_S,"Ogre::Degree");
 tolua_usertype(tolua_S,"EmberOgre::Model::PartDefinition");
 tolua_usertype(tolua_S,"std::vector<EmberOgre::Model::ActionDefinition*>");
 tolua_usertype(tolua_S,"std::vector<std::string>");
 tolua_usertype(tolua_S,"EmberOgre::AvatarCamera");
 tolua_usertype(tolua_S,"sigc::signal<void,EmberOgre::GUIManager&>");
 tolua_usertype(tolua_S,"Ogre::MapIterator<Ogre::ResourceManager::ResourceHandleMap>");
 tolua_usertype(tolua_S,"EmberOgre::MouseMotion");
 tolua_usertype(tolua_S,"EmberOgre::AvatarController");
 tolua_usertype(tolua_S,"EmberOgre::Model::Model");
 tolua_usertype(tolua_S,"EmberOgre::MousePickerArgs");
 tolua_usertype(tolua_S,"std::map<const std::string,Carpenter::BluePrint>");
 tolua_usertype(tolua_S,"Ogre::AnimationState");
 tolua_usertype(tolua_S,"std::vector<EmberOgre::Model::SubEntityDefinition*>");
 tolua_usertype(tolua_S,"sigc::signal<void,EmberOgre::AvatarController&>");
 tolua_usertype(tolua_S,"sigc::signal<void,Ogre::Camera*>");
 tolua_usertype(tolua_S,"sigc::signal<void,EmberOgre::TerrainGenerator&>");
 tolua_usertype(tolua_S,"std::vector<EmberOgre::AttachPointNode*>");
 tolua_usertype(tolua_S,"Ogre::Resource");
 tolua_usertype(tolua_S,"sigc::signal<void,EmberOgre::MotionManager&>");
 tolua_usertype(tolua_S,"sigc::signal<void>");
 tolua_usertype(tolua_S,"EmberOgre::ModelMapping");
 tolua_usertype(tolua_S,"sigc::signal<void,const EmberOgre::EntityPickResult&,const EmberOgre::MousePickerArgs&>");
 tolua_usertype(tolua_S,"EmberOgre::Jesus");
 tolua_usertype(tolua_S,"sigc::signal<void,const SDL_keysym&,EmberOgre::Input::InputMode>");
 tolua_usertype(tolua_S,"sigc::signal<void,const EmberOgre::MouseMotion&,EmberOgre::Input::InputMode>");
 tolua_usertype(tolua_S,"EmberOgre::Model::Action");
 tolua_usertype(tolua_S,"sigc::signal<void,EmberOgre::EmberEntityFactory*>");
 tolua_usertype(tolua_S,"sigc::signal<void,EmberOgre::EmberEntity*>");
 tolua_usertype(tolua_S,"Carpenter::BuildingBlockBinding");
 tolua_usertype(tolua_S,"Ogre::RenderWindow");
 tolua_usertype(tolua_S,"Eris::View");
 tolua_usertype(tolua_S,"EmberOgre::Model::ActionDefinition");
 tolua_usertype(tolua_S,"std::vector<EmberOgre::Model::PartDefinition*>");
 tolua_usertype(tolua_S,"Ogre;;ResourceManager");
 tolua_usertype(tolua_S,"EmberOgre::Model::ModelDefinitionManager");
 tolua_usertype(tolua_S,"EmberOgre::ModelBlock");
 tolua_usertype(tolua_S,"sigc::signal<void,EmberOgre::AvatarMovementMode::Mode>");
 tolua_usertype(tolua_S,"WFMath::Point<3>");
 tolua_usertype(tolua_S,"EmberOgre::ModelRenderer");
 tolua_usertype(tolua_S,"Ogre::Camera");
 tolua_usertype(tolua_S,"Mercator::Area");
 tolua_usertype(tolua_S,"EmberOgre::EmberEntity");
 tolua_usertype(tolua_S,"WFMath::Quaternion");
 tolua_usertype(tolua_S,"WFMath::AxisBox<3>");
 tolua_usertype(tolua_S,"Ogre::Vector2");
 tolua_usertype(tolua_S,"EmberOgre::LightWibbler");
 tolua_usertype(tolua_S,"EmberOgre::Model::SubModel");
 tolua_usertype(tolua_S,"std::map<std::string,Carpenter::BluePrint*>");
 tolua_usertype(tolua_S,"Ogre::AxisAlignedBox");
 tolua_usertype(tolua_S,"sigc::signal<void,EmberOgre::Jesus*>");
 tolua_usertype(tolua_S,"Ogre::BillboardSet");
 tolua_usertype(tolua_S,"EmberOgre::AttachPointNode");
 tolua_usertype(tolua_S,"Carpenter::BuildingBlockSpec");
 tolua_usertype(tolua_S,"Ogre::Billboard");
 tolua_usertype(tolua_S,"EmberOgre::EntityPickResult");
 tolua_usertype(tolua_S,"Ogre::ControllerValue<Ogre::Real>");
 tolua_usertype(tolua_S,"WFMath::Point<2>");
 tolua_usertype(tolua_S,"std::vector<const Carpenter::AttachPoint*>");
 tolua_usertype(tolua_S,"std::vector<EmberOgre::ModelBlock*>");
 tolua_usertype(tolua_S,"sigc::signal<void,float>");
 tolua_usertype(tolua_S,"Ogre::ColourValue");
 tolua_usertype(tolua_S,"sigc::signal<void,EmberOgre::AvatarEmberEntity*>");
 tolua_usertype(tolua_S,"EmberOgre::Input");
 tolua_usertype(tolua_S,"SDLKey");
 tolua_usertype(tolua_S,"EmberOgre::ColoredListItem");
 tolua_usertype(tolua_S,"sigc::signal<void,EmberOgre::Input::InputMode>");
 tolua_usertype(tolua_S,"sigc::signal<void,EmberOgre::Input::MouseButton,EmberOgre::Input::InputMode>");
 tolua_usertype(tolua_S,"Carpenter::BuildingBlockDefinition");
 tolua_usertype(tolua_S,"CEGUI::ListboxTextItem");
 tolua_usertype(tolua_S,"EmberOgre::Model::ModelDefnPtr");
 tolua_usertype(tolua_S,"Carpenter::BuildingBlockBindingDefinition");
 tolua_usertype(tolua_S,"EmberOgre::EmberOgre");
 tolua_usertype(tolua_S,"EmberOgre::Model::ModelDefinition::AreaDefinition");
 tolua_usertype(tolua_S,"Carpenter::BuildingBlockSpecDefinition");
 tolua_usertype(tolua_S,"EmberOgre::Model::ModelDefinition");
 tolua_usertype(tolua_S,"std::map<const std::string,Carpenter::BuildingBlockSpec>");
 tolua_usertype(tolua_S,"EmberOgre::Model::SubModelDefinition");
 tolua_usertype(tolua_S,"CEGUI::StaticImage");
 tolua_usertype(tolua_S,"EmberOgre::Model::SubEntityDefinition");
 tolua_usertype(tolua_S,"Ogre::Real");
 tolua_usertype(tolua_S,"WFMath::Vector<3>");
 tolua_usertype(tolua_S,"EmberOgre::MousePicker");
 tolua_usertype(tolua_S,"EmberOgre::Construction");
 tolua_usertype(tolua_S,"EmberOgre::EmberPhysicalEntity");
 tolua_usertype(tolua_S,"Ogre::Quaternion");
 tolua_usertype(tolua_S,"sigc::signal<void,const std::string&,EmberOgre::EmberEntity*>");
 tolua_usertype(tolua_S,"EmberOgre::Avatar");
 tolua_usertype(tolua_S,"Ogre::SceneNode");
 tolua_usertype(tolua_S,"EmberOgre::Widget");
 tolua_usertype(tolua_S,"EmberOgre::AvatarMovementMode");
 tolua_usertype(tolua_S,"sigc::signal<void,Ogre::Real>");
 tolua_usertype(tolua_S,"Eris::Entity");
 tolua_usertype(tolua_S,"EmberOgre::MotionManager");
 tolua_usertype(tolua_S,"sigc::signal<void,const EmberOgre::MousePickerArgs&>");
 tolua_usertype(tolua_S,"Carpenter::BuildingBlock");
 tolua_usertype(tolua_S,"EmberOgre::EmberEntityFactory");
 tolua_usertype(tolua_S,"Ogre::Entity");
 tolua_usertype(tolua_S,"Ogre::Vector3");
 tolua_usertype(tolua_S,"CEGUI::Window");
 tolua_usertype(tolua_S,"Carpenter::AttachPoint");
 tolua_usertype(tolua_S,"EmberOgre::AvatarMovementState");
 tolua_usertype(tolua_S,"Carpenter::Carpenter");
 tolua_usertype(tolua_S,"EmberOgre::WorldEmberEntity");
 tolua_usertype(tolua_S,"EmberOgre::AvatarControllerMovement");
}

/* method: new of class  Carpenter::AttachPoint */
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

/* method: new_local of class  Carpenter::AttachPoint */
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

/* method: getName of class  Carpenter::AttachPoint */
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

/* method: getNormal of class  Carpenter::AttachPoint */
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

/* method: getPosition of class  Carpenter::AttachPoint */
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

/* method: getAttachPair of class  Carpenter::AttachPoint */
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

/* method: getSibling of class  Carpenter::AttachPoint */
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

/* method: getAttachPoint of class  Carpenter::AttachPair */
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

/* method: new of class  Carpenter::AttachPair */
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

/* method: new_local of class  Carpenter::AttachPair */
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

/* method: getPoint1 of class  Carpenter::AttachPair */
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

/* method: getPoint2 of class  Carpenter::AttachPair */
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

/* method: getName of class  Carpenter::AttachPair */
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

/* method: getType of class  Carpenter::AttachPair */
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

/* method: getName of class  Carpenter::BlockSpec */
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

/* method: getBoundingBox of class  Carpenter::BlockSpec */
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

/* method: getAttachPair of class  Carpenter::BlockSpec */
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

/* method: addAttachPair of class  Carpenter::BlockSpec */
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

/* method: setBoundingBox of class  Carpenter::BlockSpec */
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

/* method: getAllPoints of class  Carpenter::BlockSpec */
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

/* method: new of class  Carpenter::BuildingBlockSpecDefinition */
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

/* method: new_local of class  Carpenter::BuildingBlockSpecDefinition */
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

/* get function: mName of class  Carpenter::BuildingBlockSpecDefinition */
static int tolua_get_Carpenter__BuildingBlockSpecDefinition_mName(lua_State* tolua_S)
{
  Carpenter::BuildingBlockSpecDefinition* self = (Carpenter::BuildingBlockSpecDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mName'",NULL);
#endif
 tolua_pushcppstring(tolua_S,(const char*)self->mName);
 return 1;
}

/* set function: mName of class  Carpenter::BuildingBlockSpecDefinition */
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

/* get function: mBlockSpecName of class  Carpenter::BuildingBlockSpecDefinition */
static int tolua_get_Carpenter__BuildingBlockSpecDefinition_mBlockSpecName(lua_State* tolua_S)
{
  Carpenter::BuildingBlockSpecDefinition* self = (Carpenter::BuildingBlockSpecDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mBlockSpecName'",NULL);
#endif
 tolua_pushcppstring(tolua_S,(const char*)self->mBlockSpecName);
 return 1;
}

/* set function: mBlockSpecName of class  Carpenter::BuildingBlockSpecDefinition */
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

/* method: new of class  Carpenter::BuildingBlockSpec */
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

/* method: new_local of class  Carpenter::BuildingBlockSpec */
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

/* method: getDefinition of class  Carpenter::BuildingBlockSpec */
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

/* method: getBlockSpec of class  Carpenter::BuildingBlockSpec */
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

/* method: getName of class  Carpenter::BuildingBlockSpec */
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

/* method: new of class  Carpenter::Carpenter */
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

/* method: new_local of class  Carpenter::Carpenter */
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

/* method: delete of class  Carpenter::Carpenter */
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

/* method: createBlueprint of class  Carpenter::Carpenter */
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

/* method: getBuildingBlockSpec of class  Carpenter::Carpenter */
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

/* method: createBlockSpec of class  Carpenter::Carpenter */
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

/* method: createBuildingBlockSpec of class  Carpenter::Carpenter */
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

/* method: getBlockSpecs of class  Carpenter::Carpenter */
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

/* method: getBuildingBlockSpecs of class  Carpenter::Carpenter */
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

/* method: getBluePrints of class  Carpenter::Carpenter */
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

/* get function: SETCAMERADISTANCE of class  EmberOgre::AvatarCamera */
static int tolua_get_EmberOgre__AvatarCamera_SETCAMERADISTANCE(lua_State* tolua_S)
{
 tolua_pushcppstring(tolua_S,(const char*)EmberOgre::AvatarCamera::SETCAMERADISTANCE);
 return 1;
}

/* method: pitch of class  EmberOgre::AvatarCamera */
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

/* method: yaw of class  EmberOgre::AvatarCamera */
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

/* method: getPitch of class  EmberOgre::AvatarCamera */
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
  Ogre::Degree tolua_ret = (Ogre::Degree)  self->getPitch();
 {
#ifdef __cplusplus
 void* tolua_obj = new Ogre::Degree(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Degree");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::Degree));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Degree");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPitch'.",&tolua_err);
 return 0;
#endif
}

/* method: getYaw of class  EmberOgre::AvatarCamera */
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
  Ogre::Degree tolua_ret = (Ogre::Degree)  self->getYaw();
 {
#ifdef __cplusplus
 void* tolua_obj = new Ogre::Degree(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Degree");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::Degree));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Degree");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getYaw'.",&tolua_err);
 return 0;
#endif
}

/* method: getCamera of class  EmberOgre::AvatarCamera */
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

/* method: getCamera of class  EmberOgre::AvatarCamera */
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

/* method: getOrientation of class  EmberOgre::AvatarCamera */
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
  Ogre::Quaternion tolua_ret = (Ogre::Quaternion)  self->getOrientation(onlyHorizontal);
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
 tolua_error(tolua_S,"#ferror in function 'getOrientation'.",&tolua_err);
 return 0;
#endif
}

/* method: getPosition of class  EmberOgre::AvatarCamera */
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
  Ogre::Vector3 tolua_ret = (Ogre::Vector3)  self->getPosition();
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
 tolua_error(tolua_S,"#ferror in function 'getPosition'.",&tolua_err);
 return 0;
#endif
}

/* method: setMode of class  EmberOgre::AvatarCamera */
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

/* get function: MovedCamera of class  EmberOgre::AvatarCamera */
static int tolua_get_EmberOgre__AvatarCamera_MovedCamera(lua_State* tolua_S)
{
  EmberOgre::AvatarCamera* self = (EmberOgre::AvatarCamera*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'MovedCamera'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->MovedCamera,"sigc::signal<void,Ogre::Camera*>");
 return 1;
}

/* set function: MovedCamera of class  EmberOgre::AvatarCamera */
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

/* get function: EventChangedCameraDistance of class  EmberOgre::AvatarCamera */
static int tolua_get_EmberOgre__AvatarCamera_EventChangedCameraDistance(lua_State* tolua_S)
{
  EmberOgre::AvatarCamera* self = (EmberOgre::AvatarCamera*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventChangedCameraDistance'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->EventChangedCameraDistance,"sigc::signal<void,Ogre::Real>");
 return 1;
}

/* set function: EventChangedCameraDistance of class  EmberOgre::AvatarCamera */
static int tolua_set_EmberOgre__AvatarCamera_EventChangedCameraDistance(lua_State* tolua_S)
{
  EmberOgre::AvatarCamera* self = (EmberOgre::AvatarCamera*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventChangedCameraDistance'",NULL);
 if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,Ogre::Real>",0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventChangedCameraDistance = *((sigc::signal<void,Ogre::Real>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}

/* method: setClosestPickingDistance of class  EmberOgre::AvatarCamera */
static int tolua_EmberOgre_EmberOgre_AvatarCamera_setClosestPickingDistance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"EmberOgre::AvatarCamera",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"Ogre::Real",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  EmberOgre::AvatarCamera* self = (EmberOgre::AvatarCamera*)  tolua_tousertype(tolua_S,1,0);
  Ogre::Real distance = *((Ogre::Real*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setClosestPickingDistance'",NULL);
#endif
 {
  self->setClosestPickingDistance(distance);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setClosestPickingDistance'.",&tolua_err);
 return 0;
#endif
}

/* method: getClosestPickingDistance of class  EmberOgre::AvatarCamera */
static int tolua_EmberOgre_EmberOgre_AvatarCamera_getClosestPickingDistance00(lua_State* tolua_S)
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
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getClosestPickingDistance'",NULL);
#endif
 {
  Ogre::Real tolua_ret = (Ogre::Real)  self->getClosestPickingDistance();
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
 tolua_error(tolua_S,"#ferror in function 'getClosestPickingDistance'.",&tolua_err);
 return 0;
#endif
}

/* method: attach of class  EmberOgre::AvatarCamera */
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

/* method: setCameraDistance of class  EmberOgre::AvatarCamera */
static int tolua_EmberOgre_EmberOgre_AvatarCamera_setCameraDistance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"EmberOgre::AvatarCamera",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"Ogre::Real",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  EmberOgre::AvatarCamera* self = (EmberOgre::AvatarCamera*)  tolua_tousertype(tolua_S,1,0);
  Ogre::Real distance = *((Ogre::Real*)  tolua_tousertype(tolua_S,2,0));
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

/* method: new of class  EmberOgre::AvatarControllerMovement */
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

/* method: new_local of class  EmberOgre::AvatarControllerMovement */
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

/* get function: rotationDegHoriz of class  EmberOgre::AvatarControllerMovement */
static int tolua_get_EmberOgre__AvatarControllerMovement_rotationDegHoriz(lua_State* tolua_S)
{
  EmberOgre::AvatarControllerMovement* self = (EmberOgre::AvatarControllerMovement*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'rotationDegHoriz'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->rotationDegHoriz);
 return 1;
}

/* set function: rotationDegHoriz of class  EmberOgre::AvatarControllerMovement */
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

/* get function: rotationDegVert of class  EmberOgre::AvatarControllerMovement */
static int tolua_get_EmberOgre__AvatarControllerMovement_rotationDegVert(lua_State* tolua_S)
{
  EmberOgre::AvatarControllerMovement* self = (EmberOgre::AvatarControllerMovement*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'rotationDegVert'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->rotationDegVert);
 return 1;
}

/* set function: rotationDegVert of class  EmberOgre::AvatarControllerMovement */
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

/* get function: timeSlice of class  EmberOgre::AvatarControllerMovement */
static int tolua_get_EmberOgre__AvatarControllerMovement_timeSlice(lua_State* tolua_S)
{
  EmberOgre::AvatarControllerMovement* self = (EmberOgre::AvatarControllerMovement*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'timeSlice'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->timeSlice,"Ogre::Real");
 return 1;
}

/* set function: timeSlice of class  EmberOgre::AvatarControllerMovement */
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

/* get function: movementDirection of class  EmberOgre::AvatarControllerMovement */
static int tolua_get_EmberOgre__AvatarControllerMovement_movementDirection(lua_State* tolua_S)
{
  EmberOgre::AvatarControllerMovement* self = (EmberOgre::AvatarControllerMovement*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'movementDirection'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->movementDirection,"Ogre::Vector3");
 return 1;
}

/* set function: movementDirection of class  EmberOgre::AvatarControllerMovement */
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

/* get function: mode of class  EmberOgre::AvatarControllerMovement */
static int tolua_get_EmberOgre__AvatarControllerMovement_mode(lua_State* tolua_S)
{
  EmberOgre::AvatarControllerMovement* self = (EmberOgre::AvatarControllerMovement*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mode'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->mode);
 return 1;
}

/* set function: mode of class  EmberOgre::AvatarControllerMovement */
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

/* get function: isMoving of class  EmberOgre::AvatarControllerMovement */
static int tolua_get_EmberOgre__AvatarControllerMovement_isMoving(lua_State* tolua_S)
{
  EmberOgre::AvatarControllerMovement* self = (EmberOgre::AvatarControllerMovement*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'isMoving'",NULL);
#endif
 tolua_pushboolean(tolua_S,(bool)self->isMoving);
 return 1;
}

/* set function: isMoving of class  EmberOgre::AvatarControllerMovement */
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

/* get function: cameraOrientation of class  EmberOgre::AvatarControllerMovement */
static int tolua_get_EmberOgre__AvatarControllerMovement_cameraOrientation(lua_State* tolua_S)
{
  EmberOgre::AvatarControllerMovement* self = (EmberOgre::AvatarControllerMovement*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'cameraOrientation'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->cameraOrientation,"Ogre::Quaternion");
 return 1;
}

/* set function: cameraOrientation of class  EmberOgre::AvatarControllerMovement */
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

/* get function: EventMovementModeChanged of class  EmberOgre::AvatarController */
static int tolua_get_EmberOgre__AvatarController_EventMovementModeChanged(lua_State* tolua_S)
{
  EmberOgre::AvatarController* self = (EmberOgre::AvatarController*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventMovementModeChanged'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->EventMovementModeChanged,"sigc::signal<void,EmberOgre::AvatarMovementMode::Mode>");
 return 1;
}

/* set function: EventMovementModeChanged of class  EmberOgre::AvatarController */
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

/* method: getAvatarCamera of class  EmberOgre::AvatarController */
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

/* method: detachCamera of class  EmberOgre::AvatarController */
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

/* method: attachCamera of class  EmberOgre::AvatarController */
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

/* method: getCurrentMovement of class  EmberOgre::AvatarController */
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

/* get function: isMoving of class  EmberOgre::AvatarMovementState */
static int tolua_get_EmberOgre__AvatarMovementState_isMoving(lua_State* tolua_S)
{
  EmberOgre::AvatarMovementState* self = (EmberOgre::AvatarMovementState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'isMoving'",NULL);
#endif
 tolua_pushboolean(tolua_S,(bool)self->isMoving);
 return 1;
}

/* set function: isMoving of class  EmberOgre::AvatarMovementState */
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

/* get function: isRunning of class  EmberOgre::AvatarMovementState */
static int tolua_get_EmberOgre__AvatarMovementState_isRunning(lua_State* tolua_S)
{
  EmberOgre::AvatarMovementState* self = (EmberOgre::AvatarMovementState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'isRunning'",NULL);
#endif
 tolua_pushboolean(tolua_S,(bool)self->isRunning);
 return 1;
}

/* set function: isRunning of class  EmberOgre::AvatarMovementState */
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

/* get function: velocity of class  EmberOgre::AvatarMovementState */
static int tolua_get_EmberOgre__AvatarMovementState_velocity(lua_State* tolua_S)
{
  EmberOgre::AvatarMovementState* self = (EmberOgre::AvatarMovementState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'velocity'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->velocity,"Ogre::Vector3");
 return 1;
}

/* set function: velocity of class  EmberOgre::AvatarMovementState */
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

/* get function: orientation of class  EmberOgre::AvatarMovementState */
static int tolua_get_EmberOgre__AvatarMovementState_orientation(lua_State* tolua_S)
{
  EmberOgre::AvatarMovementState* self = (EmberOgre::AvatarMovementState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'orientation'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->orientation,"Ogre::Quaternion");
 return 1;
}

/* set function: orientation of class  EmberOgre::AvatarMovementState */
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

/* method: getAvatarCamera of class  EmberOgre::Avatar */
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

/* method: getAvatarSceneNode of class  EmberOgre::Avatar */
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

/* method: setAvatarController of class  EmberOgre::Avatar */
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

/* method: getAvatarEmberEntity of class  EmberOgre::Avatar */
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

/* method: setMinIntervalOfRotationChanges of class  EmberOgre::Avatar */
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

/* get function: EventAddedEntityToInventory of class  EmberOgre::Avatar */
static int tolua_get_EmberOgre__Avatar_EventAddedEntityToInventory(lua_State* tolua_S)
{
  EmberOgre::Avatar* self = (EmberOgre::Avatar*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventAddedEntityToInventory'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->EventAddedEntityToInventory,"sigc::signal<void,EmberOgre::EmberEntity*>");
 return 1;
}

/* set function: EventAddedEntityToInventory of class  EmberOgre::Avatar */
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

/* get function: EventRemovedEntityFromInventory of class  EmberOgre::Avatar */
static int tolua_get_EmberOgre__Avatar_EventRemovedEntityFromInventory(lua_State* tolua_S)
{
  EmberOgre::Avatar* self = (EmberOgre::Avatar*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventRemovedEntityFromInventory'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->EventRemovedEntityFromInventory,"sigc::signal<void,EmberOgre::EmberEntity*>");
 return 1;
}

/* set function: EventRemovedEntityFromInventory of class  EmberOgre::Avatar */
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

/* method: getWorld of class  EmberOgre::EmberEntityFactory */
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

/* method: getSceneNode of class  EmberOgre::EmberEntity */
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

/* method: hasSuggestedResponses of class  EmberOgre::EmberEntity */
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

/* method: getSuggestedResponses of class  EmberOgre::EmberEntity */
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
  std::vector<std::string> tolua_ret = (std::vector<std::string>)  self->getSuggestedResponses();
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
 tolua_error(tolua_S,"#ferror in function 'getSuggestedResponses'.",&tolua_err);
 return 0;
#endif
}

/* method: setVisible of class  EmberOgre::EmberEntity */
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

/* method: getEmberLocation of class  EmberOgre::EmberEntity */
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

/* method: isInitialized of class  EmberOgre::EmberEntity */
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

/* method: getMovementMode of class  EmberOgre::EmberEntity */
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

/* method: showOgreBoundingBox of class  EmberOgre::EmberEntity */
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

/* method: showErisBoundingBox of class  EmberOgre::EmberEntity */
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

/* method: getShowOgreBoundingBox of class  EmberOgre::EmberEntity */
static int tolua_EmberOgre_EmberOgre_EmberEntity_getShowOgreBoundingBox00(lua_State* tolua_S)
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

/* method: getShowErisBoundingBox of class  EmberOgre::EmberEntity */
static int tolua_EmberOgre_EmberOgre_EmberEntity_getShowErisBoundingBox00(lua_State* tolua_S)
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

/* method: getWorldBoundingBox of class  EmberOgre::EmberEntity */
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

/* method: getDefaultUseOperators of class  EmberOgre::EmberEntity */
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

/* get function: ACTION_STAND of class  EmberOgre::EmberPhysicalEntity */
static int tolua_get_EmberOgre__EmberPhysicalEntity_ACTION_STAND(lua_State* tolua_S)
{
 tolua_pushusertype(tolua_S,(void*)&EmberOgre::EmberPhysicalEntity::ACTION_STAND,"const const");
 return 1;
}

/* get function: ACTION_RUN of class  EmberOgre::EmberPhysicalEntity */
static int tolua_get_EmberOgre__EmberPhysicalEntity_ACTION_RUN(lua_State* tolua_S)
{
 tolua_pushusertype(tolua_S,(void*)&EmberOgre::EmberPhysicalEntity::ACTION_RUN,"const const");
 return 1;
}

/* get function: ACTION_WALK of class  EmberOgre::EmberPhysicalEntity */
static int tolua_get_EmberOgre__EmberPhysicalEntity_ACTION_WALK(lua_State* tolua_S)
{
 tolua_pushusertype(tolua_S,(void*)&EmberOgre::EmberPhysicalEntity::ACTION_WALK,"const const");
 return 1;
}

/* get function: ACTION_SWIM of class  EmberOgre::EmberPhysicalEntity */
static int tolua_get_EmberOgre__EmberPhysicalEntity_ACTION_SWIM(lua_State* tolua_S)
{
 tolua_pushusertype(tolua_S,(void*)&EmberOgre::EmberPhysicalEntity::ACTION_SWIM,"const const");
 return 1;
}

/* get function: ACTION_FLOAT of class  EmberOgre::EmberPhysicalEntity */
static int tolua_get_EmberOgre__EmberPhysicalEntity_ACTION_FLOAT(lua_State* tolua_S)
{
 tolua_pushusertype(tolua_S,(void*)&EmberOgre::EmberPhysicalEntity::ACTION_FLOAT,"const const");
 return 1;
}

/* method: EmberPhysicalEntity::getModel of class  EmberOgre::EmberPhysicalEntity */
static int tolua_EmberOgre_EmberOgre_EmberPhysicalEntity_EmberPhysicalEntity__getModel00(lua_State* tolua_S)
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
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'EmberPhysicalEntity::getModel'",NULL);
#endif
 {
  EmberOgre::Model::Model* tolua_ret = (EmberOgre::Model::Model*)  self->EmberPhysicalEntity::getModel();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Model::Model");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'EmberPhysicalEntity__getModel'.",&tolua_err);
 return 0;
#endif
}

/* method: EmberPhysicalEntity::getScaleNode of class  EmberOgre::EmberPhysicalEntity */
static int tolua_EmberOgre_EmberOgre_EmberPhysicalEntity_EmberPhysicalEntity__getScaleNode00(lua_State* tolua_S)
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
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'EmberPhysicalEntity::getScaleNode'",NULL);
#endif
 {
  Ogre::SceneNode* tolua_ret = (Ogre::SceneNode*)  self->EmberPhysicalEntity::getScaleNode();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::SceneNode");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'EmberPhysicalEntity__getScaleNode'.",&tolua_err);
 return 0;
#endif
}

/* method: setVisible of class  EmberOgre::EmberPhysicalEntity */
static int tolua_EmberOgre_EmberOgre_EmberPhysicalEntity_setVisible00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"EmberOgre::EmberPhysicalEntity",0,&tolua_err) ||
 !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  EmberOgre::EmberPhysicalEntity* self = (EmberOgre::EmberPhysicalEntity*)  tolua_tousertype(tolua_S,1,0);
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

/* method: showOgreBoundingBox of class  EmberOgre::EmberPhysicalEntity */
static int tolua_EmberOgre_EmberOgre_EmberPhysicalEntity_showOgreBoundingBox00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"EmberOgre::EmberPhysicalEntity",0,&tolua_err) ||
 !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  EmberOgre::EmberPhysicalEntity* self = (EmberOgre::EmberPhysicalEntity*)  tolua_tousertype(tolua_S,1,0);
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

/* method: getShowOgreBoundingBox of class  EmberOgre::EmberPhysicalEntity */
static int tolua_EmberOgre_EmberOgre_EmberPhysicalEntity_getShowOgreBoundingBox00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"EmberOgre::EmberPhysicalEntity",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  EmberOgre::EmberPhysicalEntity* self = (EmberOgre::EmberPhysicalEntity*)  tolua_tousertype(tolua_S,1,0);
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

/* method: getEntityAttachedToPoint of class  EmberOgre::EmberPhysicalEntity */
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
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getEntityAttachedToPoint'.",&tolua_err);
 return 0;
#endif
}

/* method: getWorldBoundingBox of class  EmberOgre::EmberPhysicalEntity */
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

/* method: getAvatar of class  EmberOgre::AvatarEmberEntity */
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

/* method: getAvatarSceneNode of class  EmberOgre::AvatarEmberEntity */
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

/* method: getSingleton of class  EmberOgre::GUIManager */
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

/* get function: SCREENSHOT of class  EmberOgre::GUIManager */
static int tolua_get_EmberOgre__GUIManager_SCREENSHOT(lua_State* tolua_S)
{
 tolua_pushcppstring(tolua_S,(const char*)EmberOgre::GUIManager::SCREENSHOT);
 return 1;
}

/* get function: TOGGLEINPUTMODE of class  EmberOgre::GUIManager */
static int tolua_get_EmberOgre__GUIManager_TOGGLEINPUTMODE(lua_State* tolua_S)
{
 tolua_pushcppstring(tolua_S,(const char*)EmberOgre::GUIManager::TOGGLEINPUTMODE);
 return 1;
}

/* get function: AppendIGChatLine of class  EmberOgre::GUIManager */
static int tolua_get_EmberOgre__GUIManager_AppendIGChatLine(lua_State* tolua_S)
{
  EmberOgre::GUIManager* self = (EmberOgre::GUIManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'AppendIGChatLine'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->AppendIGChatLine,"sigc::signal<void,const std::string&,EmberOgre::EmberEntity*>");
 return 1;
}

/* set function: AppendIGChatLine of class  EmberOgre::GUIManager */
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

/* get function: AppendOOGChatLine of class  EmberOgre::GUIManager */
static int tolua_get_EmberOgre__GUIManager_AppendOOGChatLine(lua_State* tolua_S)
{
  EmberOgre::GUIManager* self = (EmberOgre::GUIManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'AppendOOGChatLine'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->AppendOOGChatLine,"sigc::signal<void,const std::string&,EmberOgre::EmberEntity*>");
 return 1;
}

/* set function: AppendOOGChatLine of class  EmberOgre::GUIManager */
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

/* get function: EventEntityAction of class  EmberOgre::GUIManager */
static int tolua_get_EmberOgre__GUIManager_EventEntityAction(lua_State* tolua_S)
{
  EmberOgre::GUIManager* self = (EmberOgre::GUIManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventEntityAction'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->EventEntityAction,"sigc::signal<void,const std::string&,EmberOgre::EmberEntity*>");
 return 1;
}

/* set function: EventEntityAction of class  EmberOgre::GUIManager */
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

/* get function: EventFrameStarted of class  EmberOgre::GUIManager */
static int tolua_get_EmberOgre__GUIManager_EventFrameStarted(lua_State* tolua_S)
{
  EmberOgre::GUIManager* self = (EmberOgre::GUIManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventFrameStarted'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->EventFrameStarted,"sigc::signal<void,float>");
 return 1;
}

/* set function: EventFrameStarted of class  EmberOgre::GUIManager */
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

/* method: removeWidget of class  EmberOgre::GUIManager */
static int tolua_EmberOgre_EmberOgre_GUIManager_removeWidget00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"EmberOgre::GUIManager",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"EmberOgre::Widget",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  EmberOgre::GUIManager* self = (EmberOgre::GUIManager*)  tolua_tousertype(tolua_S,1,0);
  EmberOgre::Widget* widget = ((EmberOgre::Widget*)  tolua_tousertype(tolua_S,2,0));
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

/* method: addWidget of class  EmberOgre::GUIManager */
static int tolua_EmberOgre_EmberOgre_GUIManager_addWidget00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"EmberOgre::GUIManager",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"EmberOgre::Widget",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  EmberOgre::GUIManager* self = (EmberOgre::GUIManager*)  tolua_tousertype(tolua_S,1,0);
  EmberOgre::Widget* widget = ((EmberOgre::Widget*)  tolua_tousertype(tolua_S,2,0));
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

/* method: EmitEntityAction of class  EmberOgre::GUIManager */
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

/* method: getMainSheet of class  EmberOgre::GUIManager */
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

/* method: setDebugText of class  EmberOgre::GUIManager */
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

/* method: isInGUIMode of class  EmberOgre::GUIManager */
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

/* method: isInMovementKeysMode of class  EmberOgre::GUIManager */
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

/* method: getMousePicker of class  EmberOgre::GUIManager */
static int tolua_EmberOgre_EmberOgre_GUIManager_getMousePicker00(lua_State* tolua_S)
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
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMousePicker'",NULL);
#endif
 {
  EmberOgre::MousePicker* tolua_ret = (EmberOgre::MousePicker*)  self->getMousePicker();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::MousePicker");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMousePicker'.",&tolua_err);
 return 0;
#endif
}

/* method: getInput of class  EmberOgre::GUIManager */
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
  EmberOgre::Input* tolua_ret = (EmberOgre::Input*)  self->getInput();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Input");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInput'.",&tolua_err);
 return 0;
#endif
}

/* method: pushMousePicker of class  EmberOgre::GUIManager */
static int tolua_EmberOgre_EmberOgre_GUIManager_pushMousePicker00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"EmberOgre::GUIManager",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"EmberOgre::MousePicker",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  EmberOgre::GUIManager* self = (EmberOgre::GUIManager*)  tolua_tousertype(tolua_S,1,0);
  EmberOgre::MousePicker* mousePicker = ((EmberOgre::MousePicker*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'pushMousePicker'",NULL);
#endif
 {
  self->pushMousePicker(mousePicker);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'pushMousePicker'.",&tolua_err);
 return 0;
#endif
}

/* method: popMousePicker of class  EmberOgre::GUIManager */
static int tolua_EmberOgre_EmberOgre_GUIManager_popMousePicker00(lua_State* tolua_S)
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
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'popMousePicker'",NULL);
#endif
 {
  EmberOgre::MousePicker* tolua_ret = (EmberOgre::MousePicker*)  self->popMousePicker();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::MousePicker");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'popMousePicker'.",&tolua_err);
 return 0;
#endif
}

/* method: createWidget of class  EmberOgre::GUIManager */
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
  EmberOgre::Widget* tolua_ret = (EmberOgre::Widget*)  self->createWidget();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Widget");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createWidget'.",&tolua_err);
 return 0;
#endif
}

/* method: createWidget of class  EmberOgre::GUIManager */
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
  EmberOgre::Widget* tolua_ret = (EmberOgre::Widget*)  self->createWidget(name);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Widget");
 tolua_pushcppstring(tolua_S,(const char*)name);
 }
 }
 return 2;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_GUIManager_createWidget00(tolua_S);
}

/* get function: entity of class  EmberOgre::EntityPickResult */
static int tolua_get_EmberOgre__EntityPickResult_entity_ptr(lua_State* tolua_S)
{
  EmberOgre::EntityPickResult* self = (EmberOgre::EntityPickResult*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'entity'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)self->entity,"EmberOgre::EmberEntity");
 return 1;
}

/* set function: entity of class  EmberOgre::EntityPickResult */
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

/* get function: position of class  EmberOgre::EntityPickResult */
static int tolua_get_EmberOgre__EntityPickResult_position(lua_State* tolua_S)
{
  EmberOgre::EntityPickResult* self = (EmberOgre::EntityPickResult*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'position'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->position,"Ogre::Vector3");
 return 1;
}

/* set function: position of class  EmberOgre::EntityPickResult */
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

/* get function: distance of class  EmberOgre::EntityPickResult */
static int tolua_get_EmberOgre__EntityPickResult_distance(lua_State* tolua_S)
{
  EmberOgre::EntityPickResult* self = (EmberOgre::EntityPickResult*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'distance'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->distance,"Ogre::Real");
 return 1;
}

/* set function: distance of class  EmberOgre::EntityPickResult */
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

/* get function: windowX of class  EmberOgre::MousePickerArgs */
static int tolua_get_EmberOgre__MousePickerArgs_windowX(lua_State* tolua_S)
{
  EmberOgre::MousePickerArgs* self = (EmberOgre::MousePickerArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'windowX'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->windowX);
 return 1;
}

/* set function: windowX of class  EmberOgre::MousePickerArgs */
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

/* get function: windowY of class  EmberOgre::MousePickerArgs */
static int tolua_get_EmberOgre__MousePickerArgs_windowY(lua_State* tolua_S)
{
  EmberOgre::MousePickerArgs* self = (EmberOgre::MousePickerArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'windowY'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->windowY);
 return 1;
}

/* set function: windowY of class  EmberOgre::MousePickerArgs */
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

/* method: doMousePicking of class  EmberOgre::MousePicker */
static int tolua_EmberOgre_EmberOgre_MousePicker_doMousePicking00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"EmberOgre::MousePicker",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const Ogre::Real",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,3,"const Ogre::Real",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,4,"const EmberOgre::MousePickerArgs",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  EmberOgre::MousePicker* self = (EmberOgre::MousePicker*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Real x = *((const Ogre::Real*)  tolua_tousertype(tolua_S,2,0));
  const Ogre::Real y = *((const Ogre::Real*)  tolua_tousertype(tolua_S,3,0));
  const EmberOgre::MousePickerArgs* args = ((const EmberOgre::MousePickerArgs*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'doMousePicking'",NULL);
#endif
 {
  self->doMousePicking(x,y,*args);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'doMousePicking'.",&tolua_err);
 return 0;
#endif
}

/* get function: EventPickedEntity of class  EmberOgre::MousePicker */
static int tolua_get_EmberOgre__MousePicker_EventPickedEntity(lua_State* tolua_S)
{
  EmberOgre::MousePicker* self = (EmberOgre::MousePicker*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventPickedEntity'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->EventPickedEntity,"sigc::signal<void,const EmberOgre::EntityPickResult&,const EmberOgre::MousePickerArgs&>");
 return 1;
}

/* set function: EventPickedEntity of class  EmberOgre::MousePicker */
static int tolua_set_EmberOgre__MousePicker_EventPickedEntity(lua_State* tolua_S)
{
  EmberOgre::MousePicker* self = (EmberOgre::MousePicker*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: EventPickedNothing of class  EmberOgre::MousePicker */
static int tolua_get_EmberOgre__MousePicker_EventPickedNothing(lua_State* tolua_S)
{
  EmberOgre::MousePicker* self = (EmberOgre::MousePicker*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventPickedNothing'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->EventPickedNothing,"sigc::signal<void,const EmberOgre::MousePickerArgs&>");
 return 1;
}

/* set function: EventPickedNothing of class  EmberOgre::MousePicker */
static int tolua_set_EmberOgre__MousePicker_EventPickedNothing(lua_State* tolua_S)
{
  EmberOgre::MousePicker* self = (EmberOgre::MousePicker*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventPickedNothing'",NULL);
 if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,const EmberOgre::MousePickerArgs&>",0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventPickedNothing = *((sigc::signal<void,const EmberOgre::MousePickerArgs&>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}

/* method: getLastPickedEntity of class  EmberOgre::MousePicker */
static int tolua_EmberOgre_EmberOgre_MousePicker_getLastPickedEntity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"EmberOgre::MousePicker",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  EmberOgre::MousePicker* self = (EmberOgre::MousePicker*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLastPickedEntity'",NULL);
#endif
 {
  EmberOgre::EmberEntity* tolua_ret = (EmberOgre::EmberEntity*)  self->getLastPickedEntity();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::EmberEntity");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getLastPickedEntity'.",&tolua_err);
 return 0;
#endif
}

/* method: new of class  EmberOgre::TerrainArea */
static int tolua_EmberOgre_EmberOgre_TerrainArea_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"EmberOgre::TerrainArea",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"EmberOgre::EmberEntity",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  EmberOgre::EmberEntity* entity = ((EmberOgre::EmberEntity*)  tolua_tousertype(tolua_S,2,0));
 {
  EmberOgre::TerrainArea* tolua_ret = (EmberOgre::TerrainArea*)  new EmberOgre::TerrainArea(entity);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::TerrainArea");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}

/* method: new_local of class  EmberOgre::TerrainArea */
static int tolua_EmberOgre_EmberOgre_TerrainArea_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"EmberOgre::TerrainArea",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"EmberOgre::EmberEntity",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  EmberOgre::EmberEntity* entity = ((EmberOgre::EmberEntity*)  tolua_tousertype(tolua_S,2,0));
 {
  EmberOgre::TerrainArea* tolua_ret = (EmberOgre::TerrainArea*)  new EmberOgre::TerrainArea(entity);
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::TerrainArea");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}

/* method: init of class  EmberOgre::TerrainArea */
static int tolua_EmberOgre_EmberOgre_TerrainArea_init00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"EmberOgre::TerrainArea",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  EmberOgre::TerrainArea* self = (EmberOgre::TerrainArea*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'init'",NULL);
#endif
 {
  self->init();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'init'.",&tolua_err);
 return 0;
#endif
}

/* method: getArea of class  EmberOgre::TerrainArea */
static int tolua_EmberOgre_EmberOgre_TerrainArea_getArea00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const EmberOgre::TerrainArea",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::TerrainArea* self = (const EmberOgre::TerrainArea*)  tolua_tousertype(tolua_S,1,0);
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

/* method: setArea of class  EmberOgre::TerrainArea */
static int tolua_EmberOgre_EmberOgre_TerrainArea_setArea00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"EmberOgre::TerrainArea",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"Mercator::Area",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  EmberOgre::TerrainArea* self = (EmberOgre::TerrainArea*)  tolua_tousertype(tolua_S,1,0);
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

/* method: getSingleton of class  EmberOgre::MotionManager */
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

/* method: addEntity of class  EmberOgre::MotionManager */
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

/* method: removeEntity of class  EmberOgre::MotionManager */
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

/* method: addAnimation of class  EmberOgre::MotionManager */
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

/* method: removeAnimation of class  EmberOgre::MotionManager */
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

/* method: pauseAnimation of class  EmberOgre::MotionManager */
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

/* method: unpauseAnimation of class  EmberOgre::MotionManager */
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

/* method: addAnimatedEntity of class  EmberOgre::MotionManager */
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

/* method: removeAnimatedEntity of class  EmberOgre::MotionManager */
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

/* method: new of class  EmberOgre::ColoredListItem */
static int tolua_EmberOgre_EmberOgre_ColoredListItem_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"EmberOgre::ColoredListItem",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  std::string text = ((std::string)  tolua_tocppstring(tolua_S,2,0));
 {
  EmberOgre::ColoredListItem* tolua_ret = (EmberOgre::ColoredListItem*)  new EmberOgre::ColoredListItem(text);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::ColoredListItem");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}

/* method: new_local of class  EmberOgre::ColoredListItem */
static int tolua_EmberOgre_EmberOgre_ColoredListItem_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"EmberOgre::ColoredListItem",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  std::string text = ((std::string)  tolua_tocppstring(tolua_S,2,0));
 {
  EmberOgre::ColoredListItem* tolua_ret = (EmberOgre::ColoredListItem*)  new EmberOgre::ColoredListItem(text);
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::ColoredListItem");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}

/* method: new of class  EmberOgre::ColoredListItem */
static int tolua_EmberOgre_EmberOgre_ColoredListItem_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"EmberOgre::ColoredListItem",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  std::string text = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  unsigned int item_id = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
 {
  EmberOgre::ColoredListItem* tolua_ret = (EmberOgre::ColoredListItem*)  new EmberOgre::ColoredListItem(text,item_id);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::ColoredListItem");
 }
 }
 return 1;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_ColoredListItem_new00(tolua_S);
}

/* method: new_local of class  EmberOgre::ColoredListItem */
static int tolua_EmberOgre_EmberOgre_ColoredListItem_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"EmberOgre::ColoredListItem",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  std::string text = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  unsigned int item_id = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
 {
  EmberOgre::ColoredListItem* tolua_ret = (EmberOgre::ColoredListItem*)  new EmberOgre::ColoredListItem(text,item_id);
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::ColoredListItem");
 }
 }
 return 1;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_ColoredListItem_new00_local(tolua_S);
}

/* method: new of class  EmberOgre::ColoredListItem */
static int tolua_EmberOgre_EmberOgre_ColoredListItem_new02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"EmberOgre::ColoredListItem",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isuserdata(tolua_S,4,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  std::string text = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  unsigned int item_id = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
  void* item_data = ((void*)  tolua_touserdata(tolua_S,4,0));
 {
  EmberOgre::ColoredListItem* tolua_ret = (EmberOgre::ColoredListItem*)  new EmberOgre::ColoredListItem(text,item_id,item_data);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::ColoredListItem");
 }
 }
 return 1;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_ColoredListItem_new01(tolua_S);
}

/* method: new_local of class  EmberOgre::ColoredListItem */
static int tolua_EmberOgre_EmberOgre_ColoredListItem_new02_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"EmberOgre::ColoredListItem",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isuserdata(tolua_S,4,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  std::string text = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  unsigned int item_id = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
  void* item_data = ((void*)  tolua_touserdata(tolua_S,4,0));
 {
  EmberOgre::ColoredListItem* tolua_ret = (EmberOgre::ColoredListItem*)  new EmberOgre::ColoredListItem(text,item_id,item_data);
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::ColoredListItem");
 }
 }
 return 1;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_ColoredListItem_new01_local(tolua_S);
}

/* method: delete of class  EmberOgre::Widget */
static int tolua_EmberOgre_EmberOgre_Widget_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"EmberOgre::Widget",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Widget* self = (EmberOgre::Widget*)  tolua_tousertype(tolua_S,1,0);
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

/* method: show of class  EmberOgre::Widget */
static int tolua_EmberOgre_EmberOgre_Widget_show00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"EmberOgre::Widget",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Widget* self = (EmberOgre::Widget*)  tolua_tousertype(tolua_S,1,0);
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

/* method: hide of class  EmberOgre::Widget */
static int tolua_EmberOgre_EmberOgre_Widget_hide00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"EmberOgre::Widget",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Widget* self = (EmberOgre::Widget*)  tolua_tousertype(tolua_S,1,0);
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

/* method: getWindow of class  EmberOgre::Widget */
static int tolua_EmberOgre_EmberOgre_Widget_getWindow00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"EmberOgre::Widget",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Widget* self = (EmberOgre::Widget*)  tolua_tousertype(tolua_S,1,0);
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

/* method: enableCloseButton of class  EmberOgre::Widget */
static int tolua_EmberOgre_EmberOgre_Widget_enableCloseButton00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"EmberOgre::Widget",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Widget* self = (EmberOgre::Widget*)  tolua_tousertype(tolua_S,1,0);
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

/* method: registerConsoleVisibilityToggleCommand of class  EmberOgre::Widget */
static int tolua_EmberOgre_EmberOgre_Widget_registerConsoleVisibilityToggleCommand00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"EmberOgre::Widget",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Widget* self = (EmberOgre::Widget*)  tolua_tousertype(tolua_S,1,0);
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

/* method: getMainSheet of class  EmberOgre::Widget */
static int tolua_EmberOgre_EmberOgre_Widget_getMainSheet00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"EmberOgre::Widget",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Widget* self = (EmberOgre::Widget*)  tolua_tousertype(tolua_S,1,0);
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

/* method: getMainWindow of class  EmberOgre::Widget */
static int tolua_EmberOgre_EmberOgre_Widget_getMainWindow00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"EmberOgre::Widget",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Widget* self = (EmberOgre::Widget*)  tolua_tousertype(tolua_S,1,0);
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

/* method: loadMainSheet of class  EmberOgre::Widget */
static int tolua_EmberOgre_EmberOgre_Widget_loadMainSheet00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"EmberOgre::Widget",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Widget* self = (EmberOgre::Widget*)  tolua_tousertype(tolua_S,1,0);
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

/* method: getPrefix of class  EmberOgre::Widget */
static int tolua_EmberOgre_EmberOgre_Widget_getPrefix00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const EmberOgre::Widget",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Widget* self = (const EmberOgre::Widget*)  tolua_tousertype(tolua_S,1,0);
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

/* method: new of class  EmberOgre::Model::Model */
static int tolua_EmberOgre_EmberOgre_Model_Model_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"EmberOgre::Model::Model",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
 {
  EmberOgre::Model::Model* tolua_ret = (EmberOgre::Model::Model*)  new EmberOgre::Model::Model(name);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Model::Model");
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

/* method: new_local of class  EmberOgre::Model::Model */
static int tolua_EmberOgre_EmberOgre_Model_Model_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"EmberOgre::Model::Model",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
 {
  EmberOgre::Model::Model* tolua_ret = (EmberOgre::Model::Model*)  new EmberOgre::Model::Model(name);
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Model::Model");
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

/* method: new of class  EmberOgre::Model::Model */
static int tolua_EmberOgre_EmberOgre_Model_Model_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"EmberOgre::Model::Model",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  EmberOgre::Model::Model* tolua_ret = (EmberOgre::Model::Model*)  new EmberOgre::Model::Model();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Model::Model");
 }
 }
 return 1;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_Model_Model_new00(tolua_S);
}

/* method: new_local of class  EmberOgre::Model::Model */
static int tolua_EmberOgre_EmberOgre_Model_Model_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"EmberOgre::Model::Model",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
 {
  EmberOgre::Model::Model* tolua_ret = (EmberOgre::Model::Model*)  new EmberOgre::Model::Model();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Model::Model");
 }
 }
 return 1;
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_Model_Model_new00_local(tolua_S);
}

/* method: create of class  EmberOgre::Model::Model */
static int tolua_EmberOgre_EmberOgre_Model_Model_create00(lua_State* tolua_S)
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
  const std::string modelType = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'create'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->create(modelType);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 tolua_pushcppstring(tolua_S,(const char*)modelType);
 }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
 return 0;
#endif
}

/* method: reload of class  EmberOgre::Model::Model */
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

/* method: delete of class  EmberOgre::Model::Model */
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

/* method: addSubmodel of class  EmberOgre::Model::Model */
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

/* method: removeSubmodel of class  EmberOgre::Model::Model */
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

/* method: getAction of class  EmberOgre::Model::Model */
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

/* method: showPart of class  EmberOgre::Model::Model */
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

/* method: hidePart of class  EmberOgre::Model::Model */
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

/* method: setVisible of class  EmberOgre::Model::Model */
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

/* method: getScale of class  EmberOgre::Model::Model */
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

/* method: getRotation of class  EmberOgre::Model::Model */
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
  const Ogre::Real tolua_ret = (const Ogre::Real)  self->getRotation();
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
 tolua_error(tolua_S,"#ferror in function 'getRotation'.",&tolua_err);
 return 0;
#endif
}

/* method: getUseScaleOf of class  EmberOgre::Model::Model */
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

/* method: getName of class  EmberOgre::Model::Model */
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

/* method: getDefinition of class  EmberOgre::Model::Model */
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

/* method: hasAttachPoint of class  EmberOgre::Model::Model */
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

/* method: hasParticles of class  EmberOgre::Model::Model */
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

/* method: getSubModel of class  EmberOgre::Model::Model */
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

/* method: getEntity of class  EmberOgre::Model::SubModel */
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

/* method: getSubEntityName of class  EmberOgre::Model::SubEntityDefinition */
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

/* method: getSubEntityIndex of class  EmberOgre::Model::SubEntityDefinition */
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

/* method: getMaterialName of class  EmberOgre::Model::SubEntityDefinition */
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

/* method: setMaterialName of class  EmberOgre::Model::SubEntityDefinition */
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

/* method: getPartDefinition of class  EmberOgre::Model::SubEntityDefinition */
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

/* method: setName of class  EmberOgre::Model::PartDefinition */
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

/* method: getName of class  EmberOgre::Model::PartDefinition */
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

/* method: setShow of class  EmberOgre::Model::PartDefinition */
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

/* method: getShow of class  EmberOgre::Model::PartDefinition */
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

/* method: createSubEntityDefinition of class  EmberOgre::Model::PartDefinition */
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

/* method: createSubEntityDefinition of class  EmberOgre::Model::PartDefinition */
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

/* method: getSubEntityDefinitions of class  EmberOgre::Model::PartDefinition */
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

/* method: removeSubEntityDefinition of class  EmberOgre::Model::PartDefinition */
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

/* method: getSubModelDefinition of class  EmberOgre::Model::PartDefinition */
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

/* method: getMeshName of class  EmberOgre::Model::SubModelDefinition */
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

/* method: createPartDefinition of class  EmberOgre::Model::SubModelDefinition */
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

/* method: getPartDefinitions of class  EmberOgre::Model::SubModelDefinition */
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

/* method: removePartDefinition of class  EmberOgre::Model::SubModelDefinition */
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

/* method: getModelDefinition of class  EmberOgre::Model::SubModelDefinition */
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

/* get function: Id of class  AreaDefinition */
static int tolua_get_EmberOgre__Model__ModelDefinition__AreaDefinition_Id(lua_State* tolua_S)
{
  EmberOgre::Model::ModelDefinition::AreaDefinition* self = (EmberOgre::Model::ModelDefinition::AreaDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Id'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->Id);
 return 1;
}

/* set function: Id of class  AreaDefinition */
static int tolua_set_EmberOgre__Model__ModelDefinition__AreaDefinition_Id(lua_State* tolua_S)
{
  EmberOgre::Model::ModelDefinition::AreaDefinition* self = (EmberOgre::Model::ModelDefinition::AreaDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Id'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Id = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}

/* get function: TextureName of class  AreaDefinition */
static int tolua_get_EmberOgre__Model__ModelDefinition__AreaDefinition_TextureName(lua_State* tolua_S)
{
  EmberOgre::Model::ModelDefinition::AreaDefinition* self = (EmberOgre::Model::ModelDefinition::AreaDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'TextureName'",NULL);
#endif
 tolua_pushcppstring(tolua_S,(const char*)self->TextureName);
 return 1;
}

/* set function: TextureName of class  AreaDefinition */
static int tolua_set_EmberOgre__Model__ModelDefinition__AreaDefinition_TextureName(lua_State* tolua_S)
{
  EmberOgre::Model::ModelDefinition::AreaDefinition* self = (EmberOgre::Model::ModelDefinition::AreaDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'TextureName'",NULL);
 if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->TextureName = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}

/* get function: MaterialName of class  AreaDefinition */
static int tolua_get_EmberOgre__Model__ModelDefinition__AreaDefinition_MaterialName(lua_State* tolua_S)
{
  EmberOgre::Model::ModelDefinition::AreaDefinition* self = (EmberOgre::Model::ModelDefinition::AreaDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'MaterialName'",NULL);
#endif
 tolua_pushcppstring(tolua_S,(const char*)self->MaterialName);
 return 1;
}

/* set function: MaterialName of class  AreaDefinition */
static int tolua_set_EmberOgre__Model__ModelDefinition__AreaDefinition_MaterialName(lua_State* tolua_S)
{
  EmberOgre::Model::ModelDefinition::AreaDefinition* self = (EmberOgre::Model::ModelDefinition::AreaDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'MaterialName'",NULL);
 if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->MaterialName = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}

/* method: isValid of class  EmberOgre::Model::ModelDefinition */
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

/* method: setValid of class  EmberOgre::Model::ModelDefinition */
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

/* method: getScale of class  EmberOgre::Model::ModelDefinition */
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

/* method: setScale of class  EmberOgre::Model::ModelDefinition */
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

/* method: getUseScaleOf of class  EmberOgre::Model::ModelDefinition */
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

/* method: setUseScaleOf of class  EmberOgre::Model::ModelDefinition */
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

/* method: getTranslate of class  EmberOgre::Model::ModelDefinition */
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

/* method: setTranslate of class  EmberOgre::Model::ModelDefinition */
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinition_setTranslate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"EmberOgre::Model::ModelDefinition",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"Ogre::Vector3",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::ModelDefinition* self = (EmberOgre::Model::ModelDefinition*)  tolua_tousertype(tolua_S,1,0);
  Ogre::Vector3 translate = *((Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
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

/* method: getShowContained of class  EmberOgre::Model::ModelDefinition */
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

/* method: getShowContained of class  EmberOgre::Model::ModelDefinition */
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

/* method: getContentOffset of class  EmberOgre::Model::ModelDefinition */
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinition_getContentOffset00(lua_State* tolua_S)
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
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getContentOffset'",NULL);
#endif
 {
  Ogre::Vector3* tolua_ret = (Ogre::Vector3*)  self->getContentOffset();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::Vector3");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getContentOffset'.",&tolua_err);
 return 0;
#endif
}

/* method: setContentOffset of class  EmberOgre::Model::ModelDefinition */
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinition_setContentOffset00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"EmberOgre::Model::ModelDefinition",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"Ogre::Vector3",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Model::ModelDefinition* self = (EmberOgre::Model::ModelDefinition*)  tolua_tousertype(tolua_S,1,0);
  Ogre::Vector3* offset = ((Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setContentOffset'",NULL);
#endif
 {
  self->setContentOffset(offset);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setContentOffset'.",&tolua_err);
 return 0;
#endif
}

/* method: getRotation of class  EmberOgre::Model::ModelDefinition */
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinition_getRotation00(lua_State* tolua_S)
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
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRotation'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getRotation();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRotation'.",&tolua_err);
 return 0;
#endif
}

/* method: setRotation of class  EmberOgre::Model::ModelDefinition */
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinition_setRotation00(lua_State* tolua_S)
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
  float rotation = ((float)  tolua_tonumber(tolua_S,2,0));
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

/* method: createSubModelDefinition of class  EmberOgre::Model::ModelDefinition */
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

/* method: getSubModelDefinitions of class  EmberOgre::Model::ModelDefinition */
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

/* method: removeSubModelDefinition of class  EmberOgre::Model::ModelDefinition */
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

/* method: createActionDefinition of class  EmberOgre::Model::ModelDefinition */
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

/* method: getActionDefinitions of class  EmberOgre::Model::ModelDefinition */
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

/* method: removeActionDefinition of class  EmberOgre::Model::ModelDefinition */
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

/* method: get of class  EmberOgre::Model::ModelDefnPtr */
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

/* get function: xPosition of class  EmberOgre::MouseMotion */
static int tolua_get_EmberOgre__MouseMotion_xPosition(lua_State* tolua_S)
{
  EmberOgre::MouseMotion* self = (EmberOgre::MouseMotion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'xPosition'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->xPosition);
 return 1;
}

/* set function: xPosition of class  EmberOgre::MouseMotion */
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

/* get function: yPosition of class  EmberOgre::MouseMotion */
static int tolua_get_EmberOgre__MouseMotion_yPosition(lua_State* tolua_S)
{
  EmberOgre::MouseMotion* self = (EmberOgre::MouseMotion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'yPosition'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->yPosition);
 return 1;
}

/* set function: yPosition of class  EmberOgre::MouseMotion */
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

/* get function: xRelativeMovement of class  EmberOgre::MouseMotion */
static int tolua_get_EmberOgre__MouseMotion_xRelativeMovement(lua_State* tolua_S)
{
  EmberOgre::MouseMotion* self = (EmberOgre::MouseMotion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'xRelativeMovement'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->xRelativeMovement,"Ogre::Real");
 return 1;
}

/* set function: xRelativeMovement of class  EmberOgre::MouseMotion */
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

/* get function: yRelativeMovement of class  EmberOgre::MouseMotion */
static int tolua_get_EmberOgre__MouseMotion_yRelativeMovement(lua_State* tolua_S)
{
  EmberOgre::MouseMotion* self = (EmberOgre::MouseMotion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'yRelativeMovement'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->yRelativeMovement,"Ogre::Real");
 return 1;
}

/* set function: yRelativeMovement of class  EmberOgre::MouseMotion */
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

/* get function: xRelativeMovementInPixels of class  EmberOgre::MouseMotion */
static int tolua_get_EmberOgre__MouseMotion_xRelativeMovementInPixels(lua_State* tolua_S)
{
  EmberOgre::MouseMotion* self = (EmberOgre::MouseMotion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'xRelativeMovementInPixels'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->xRelativeMovementInPixels);
 return 1;
}

/* set function: xRelativeMovementInPixels of class  EmberOgre::MouseMotion */
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

/* get function: yRelativeMovementInPixels of class  EmberOgre::MouseMotion */
static int tolua_get_EmberOgre__MouseMotion_yRelativeMovementInPixels(lua_State* tolua_S)
{
  EmberOgre::MouseMotion* self = (EmberOgre::MouseMotion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'yRelativeMovementInPixels'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->yRelativeMovementInPixels);
 return 1;
}

/* set function: yRelativeMovementInPixels of class  EmberOgre::MouseMotion */
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

/* get function: timeSinceLastMovement of class  EmberOgre::MouseMotion */
static int tolua_get_EmberOgre__MouseMotion_timeSinceLastMovement(lua_State* tolua_S)
{
  EmberOgre::MouseMotion* self = (EmberOgre::MouseMotion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'timeSinceLastMovement'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->timeSinceLastMovement,"Ogre::Real");
 return 1;
}

/* set function: timeSinceLastMovement of class  EmberOgre::MouseMotion */
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

/* get function: EventKeyPressed of class  EmberOgre::Input */
static int tolua_get_EmberOgre__Input_EventKeyPressed(lua_State* tolua_S)
{
  EmberOgre::Input* self = (EmberOgre::Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventKeyPressed'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->EventKeyPressed,"sigc::signal<void,const SDL_keysym&,EmberOgre::Input::InputMode>");
 return 1;
}

/* set function: EventKeyPressed of class  EmberOgre::Input */
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

/* get function: EventKeyReleased of class  EmberOgre::Input */
static int tolua_get_EmberOgre__Input_EventKeyReleased(lua_State* tolua_S)
{
  EmberOgre::Input* self = (EmberOgre::Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventKeyReleased'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->EventKeyReleased,"sigc::signal<void,const SDL_keysym&,EmberOgre::Input::InputMode>");
 return 1;
}

/* set function: EventKeyReleased of class  EmberOgre::Input */
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

/* get function: EventMouseMoved of class  EmberOgre::Input */
static int tolua_get_EmberOgre__Input_EventMouseMoved(lua_State* tolua_S)
{
  EmberOgre::Input* self = (EmberOgre::Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventMouseMoved'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->EventMouseMoved,"sigc::signal<void,const EmberOgre::MouseMotion&,EmberOgre::Input::InputMode>");
 return 1;
}

/* set function: EventMouseMoved of class  EmberOgre::Input */
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

/* get function: EventMouseButtonPressed of class  EmberOgre::Input */
static int tolua_get_EmberOgre__Input_EventMouseButtonPressed(lua_State* tolua_S)
{
  EmberOgre::Input* self = (EmberOgre::Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventMouseButtonPressed'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->EventMouseButtonPressed,"sigc::signal<void,EmberOgre::Input::MouseButton,EmberOgre::Input::InputMode>");
 return 1;
}

/* set function: EventMouseButtonPressed of class  EmberOgre::Input */
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

/* get function: EventMouseButtonReleased of class  EmberOgre::Input */
static int tolua_get_EmberOgre__Input_EventMouseButtonReleased(lua_State* tolua_S)
{
  EmberOgre::Input* self = (EmberOgre::Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventMouseButtonReleased'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->EventMouseButtonReleased,"sigc::signal<void,EmberOgre::Input::MouseButton,EmberOgre::Input::InputMode>");
 return 1;
}

/* set function: EventMouseButtonReleased of class  EmberOgre::Input */
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

/* get function: EventChangedInputMode of class  EmberOgre::Input */
static int tolua_get_EmberOgre__Input_EventChangedInputMode(lua_State* tolua_S)
{
  EmberOgre::Input* self = (EmberOgre::Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventChangedInputMode'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->EventChangedInputMode,"sigc::signal<void,EmberOgre::Input::InputMode>");
 return 1;
}

/* set function: EventChangedInputMode of class  EmberOgre::Input */
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

/* method: isKeyDown of class  EmberOgre::Input */
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
  const SDLKey* tolua_var_1 = ((const SDLKey*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isKeyDown'",NULL);
#endif
 {
  const bool tolua_ret = (const bool)  self->isKeyDown(*tolua_var_1);
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

/* method: setInputMode of class  EmberOgre::Input */
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

/* method: getInputMode of class  EmberOgre::Input */
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

/* method: toggleInputMode of class  EmberOgre::Input */
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

/* method: new of class  EmberOgre::Jesus */
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

/* method: new_local of class  EmberOgre::Jesus */
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

/* method: delete of class  EmberOgre::Jesus */
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

/* method: createModelForBlockType of class  EmberOgre::Jesus */
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

/* method: getColourForAttachPoint of class  EmberOgre::Jesus */
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

/* method: getCarpenter of class  EmberOgre::Jesus */
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

/* method: addBluePrint of class  EmberOgre::Jesus */
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

/* method: getBluePrint of class  EmberOgre::Jesus */
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

/* method: getAllBluePrints of class  EmberOgre::Jesus */
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

/* method: new of class  EmberOgre::LightWibbler */
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

/* method: new_local of class  EmberOgre::LightWibbler */
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

/* method: getValue of class  EmberOgre::LightWibbler */
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

/* method: setValue of class  EmberOgre::LightWibbler */
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

/* method: new of class  EmberOgre::AttachPointNode */
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

/* method: new_local of class  EmberOgre::AttachPointNode */
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

/* method: delete of class  EmberOgre::AttachPointNode */
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

/* method: select of class  EmberOgre::AttachPointNode */
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

/* method: deselect of class  EmberOgre::AttachPointNode */
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

/* method: getAttachPoint of class  EmberOgre::AttachPointNode */
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

/* method: new of class  EmberOgre::ModelBlock */
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

/* method: new_local of class  EmberOgre::ModelBlock */
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

/* method: delete of class  EmberOgre::ModelBlock */
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

/* method: getBuildingBlock of class  EmberOgre::ModelBlock */
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

/* method: getConstruction of class  EmberOgre::ModelBlock */
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

/* method: createAttachPointNodes of class  EmberOgre::ModelBlock */
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

/* method: select of class  EmberOgre::ModelBlock */
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

/* method: deselect of class  EmberOgre::ModelBlock */
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

/* method: getAttachPointNodes of class  EmberOgre::ModelBlock */
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

/* method: getModel of class  EmberOgre::ModelBlock */
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

/* method: getNode of class  EmberOgre::ModelBlock */
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

/* get function: model of class  EmberOgre::ModelMapping */
static int tolua_get_EmberOgre__ModelMapping_model(lua_State* tolua_S)
{
  EmberOgre::ModelMapping* self = (EmberOgre::ModelMapping*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'model'",NULL);
#endif
 tolua_pushcppstring(tolua_S,(const char*)self->model);
 return 1;
}

/* get function: blocktype of class  EmberOgre::ModelMapping */
static int tolua_get_EmberOgre__ModelMapping_blocktype(lua_State* tolua_S)
{
  EmberOgre::ModelMapping* self = (EmberOgre::ModelMapping*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'blocktype'",NULL);
#endif
 tolua_pushcppstring(tolua_S,(const char*)self->blocktype);
 return 1;
}

/* method: new of class  EmberOgre::Construction */
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

/* method: new_local of class  EmberOgre::Construction */
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

/* method: delete of class  EmberOgre::Construction */
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

/* method: getJesus of class  EmberOgre::Construction */
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

/* method: getBluePrint of class  EmberOgre::Construction */
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

/* method: buildFromBluePrint of class  EmberOgre::Construction */
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

/* method: createModelBlock of class  EmberOgre::Construction */
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

/* method: getModelBlocks of class  EmberOgre::Construction */
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

/* method: remove of class  EmberOgre::Construction */
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

/* function: EmberOgre::Atlas2Ogre */
static int tolua_EmberOgre_EmberOgre_Atlas2Ogre00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const WFMath::Point<2>",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const WFMath::Point<2>* p = ((const WFMath::Point<2>*)  tolua_tousertype(tolua_S,1,0));
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

/* function: EmberOgre::Ogre2Atlas */
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

/* function: EmberOgre::Ogre2Atlas */
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
   WFMath::Point<2> tolua_ret = (  WFMath::Point<2>)  EmberOgre::Ogre2Atlas(*p);
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
tolua_lerror:
 return tolua_EmberOgre_EmberOgre_Ogre2Atlas00(tolua_S);
}

/* function: EmberOgre::Ogre2Atlas_TerrainPosition */
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
   WFMath::Point<2> tolua_ret = (  WFMath::Point<2>)  EmberOgre::Ogre2Atlas_TerrainPosition(*p);
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
 tolua_error(tolua_S,"#ferror in function 'Ogre2Atlas_TerrainPosition'.",&tolua_err);
 return 0;
#endif
}

/* function: EmberOgre::Ogre2Atlas_Vector3 */
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

/* function: EmberOgre::Atlas2Ogre */
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

/* function: EmberOgre::Atlas2Ogre */
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

/* function: EmberOgre::Atlas2Ogre */
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

/* function: EmberOgre::Ogre2Atlas */
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

/* method: new of class  EmberOgre::ModelRenderer */
static int tolua_EmberOgre_EmberOgre_ModelRenderer_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"EmberOgre::ModelRenderer",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"CEGUI::StaticImage",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::StaticImage* image = ((CEGUI::StaticImage*)  tolua_tousertype(tolua_S,2,0));
 {
  EmberOgre::ModelRenderer* tolua_ret = (EmberOgre::ModelRenderer*)  new EmberOgre::ModelRenderer(image);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::ModelRenderer");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}

/* method: new_local of class  EmberOgre::ModelRenderer */
static int tolua_EmberOgre_EmberOgre_ModelRenderer_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"EmberOgre::ModelRenderer",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"CEGUI::StaticImage",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CEGUI::StaticImage* image = ((CEGUI::StaticImage*)  tolua_tousertype(tolua_S,2,0));
 {
  EmberOgre::ModelRenderer* tolua_ret = (EmberOgre::ModelRenderer*)  new EmberOgre::ModelRenderer(image);
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::ModelRenderer");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}

/* method: delete of class  EmberOgre::ModelRenderer */
static int tolua_EmberOgre_EmberOgre_ModelRenderer_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"EmberOgre::ModelRenderer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  EmberOgre::ModelRenderer* self = (EmberOgre::ModelRenderer*)  tolua_tousertype(tolua_S,1,0);
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

/* method: showModel of class  EmberOgre::ModelRenderer */
static int tolua_EmberOgre_EmberOgre_ModelRenderer_showModel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"EmberOgre::ModelRenderer",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  EmberOgre::ModelRenderer* self = (EmberOgre::ModelRenderer*)  tolua_tousertype(tolua_S,1,0);
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

/* method: getModel of class  EmberOgre::ModelRenderer */
static int tolua_EmberOgre_EmberOgre_ModelRenderer_getModel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"EmberOgre::ModelRenderer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  EmberOgre::ModelRenderer* self = (EmberOgre::ModelRenderer*)  tolua_tousertype(tolua_S,1,0);
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

/* method: showFullModel of class  EmberOgre::ModelRenderer */
static int tolua_EmberOgre_EmberOgre_ModelRenderer_showFullModel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"EmberOgre::ModelRenderer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  EmberOgre::ModelRenderer* self = (EmberOgre::ModelRenderer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'showFullModel'",NULL);
#endif
 {
  self->showFullModel();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'showFullModel'.",&tolua_err);
 return 0;
#endif
}

/* method: setCameraDistance of class  EmberOgre::ModelRenderer */
static int tolua_EmberOgre_EmberOgre_ModelRenderer_setCameraDistance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"EmberOgre::ModelRenderer",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  EmberOgre::ModelRenderer* self = (EmberOgre::ModelRenderer*)  tolua_tousertype(tolua_S,1,0);
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

/* method: getCameraDistance of class  EmberOgre::ModelRenderer */
static int tolua_EmberOgre_EmberOgre_ModelRenderer_getCameraDistance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"EmberOgre::ModelRenderer",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  EmberOgre::ModelRenderer* self = (EmberOgre::ModelRenderer*)  tolua_tousertype(tolua_S,1,0);
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

/* method: getSingleton of class  EmberOgre::Model::ModelDefinitionManager */
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

/* method: getAreaDefinition of class  EmberOgre::Model::ModelDefinitionManager */
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_getAreaDefinition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const EmberOgre::Model::ModelDefinitionManager",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Model::ModelDefinitionManager* self = (const EmberOgre::Model::ModelDefinitionManager*)  tolua_tousertype(tolua_S,1,0);
  int layer = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAreaDefinition'",NULL);
#endif
 {
  const EmberOgre::Model::ModelDefinition::AreaDefinition* tolua_ret = (const EmberOgre::Model::ModelDefinition::AreaDefinition*)  self->getAreaDefinition(layer);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"const EmberOgre::Model::ModelDefinition::AreaDefinition");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAreaDefinition'.",&tolua_err);
 return 0;
#endif
}

/* method: exportScript of class  EmberOgre::Model::ModelDefinitionManager */
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

/* method: create of class  EmberOgre::Model::ModelDefinitionManager */
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

/* method: remove of class  EmberOgre::Model::ModelDefinitionManager */
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

/* method: getByName of class  EmberOgre::Model::ModelDefinitionManager */
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

/* method: resourceExists of class  EmberOgre::Model::ModelDefinitionManager */
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

/* method: getResourceIterator of class  EmberOgre::Model::ModelDefinitionManager */
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

/* method: getAllMeshes of class  EmberOgre::Model::ModelDefinitionManager */
static int tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_getAllMeshes00(lua_State* tolua_S)
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
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAllMeshes'",NULL);
#endif
 {
  std::vector<std::string> tolua_ret = (std::vector<std::string>)  self->getAllMeshes();
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
 tolua_error(tolua_S,"#ferror in function 'getAllMeshes'.",&tolua_err);
 return 0;
#endif
}

/* method: clear of class  std::vector<std::string> */
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

/* method: size of class  std::vector<std::string> */
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

/* method: operator[] of class  std::vector<std::string> */
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

/* method: operator&[] of class  std::vector<std::string> */
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

/* method: operator[] of class  std::vector<std::string> */
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

/* method: push_back of class  std::vector<std::string> */
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

/* method: new of class  std::vector<std::string> */
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

/* method: new_local of class  std::vector<std::string> */
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

/* method: delete of class  std::vector<std::string> */
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

/* method: clear of class  std::vector<EmberOgre::Model::SubModelDefinition*> */
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

/* method: size of class  std::vector<EmberOgre::Model::SubModelDefinition*> */
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

/* method: operator[] of class  std::vector<EmberOgre::Model::SubModelDefinition*> */
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

/* method: operator&[] of class  std::vector<EmberOgre::Model::SubModelDefinition*> */
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

/* method: operator[] of class  std::vector<EmberOgre::Model::SubModelDefinition*> */
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

/* method: push_back of class  std::vector<EmberOgre::Model::SubModelDefinition*> */
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

/* method: new of class  std::vector<EmberOgre::Model::SubModelDefinition*> */
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

/* method: new_local of class  std::vector<EmberOgre::Model::SubModelDefinition*> */
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

/* method: delete of class  std::vector<EmberOgre::Model::SubModelDefinition*> */
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

/* method: clear of class  std::vector<EmberOgre::Model::PartDefinition*> */
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

/* method: size of class  std::vector<EmberOgre::Model::PartDefinition*> */
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

/* method: operator[] of class  std::vector<EmberOgre::Model::PartDefinition*> */
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

/* method: operator&[] of class  std::vector<EmberOgre::Model::PartDefinition*> */
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

/* method: operator[] of class  std::vector<EmberOgre::Model::PartDefinition*> */
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

/* method: push_back of class  std::vector<EmberOgre::Model::PartDefinition*> */
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

/* method: new of class  std::vector<EmberOgre::Model::PartDefinition*> */
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

/* method: new_local of class  std::vector<EmberOgre::Model::PartDefinition*> */
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

/* method: delete of class  std::vector<EmberOgre::Model::PartDefinition*> */
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

/* method: clear of class  std::vector<EmberOgre::Model::SubEntityDefinition*> */
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

/* method: size of class  std::vector<EmberOgre::Model::SubEntityDefinition*> */
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

/* method: operator[] of class  std::vector<EmberOgre::Model::SubEntityDefinition*> */
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

/* method: operator&[] of class  std::vector<EmberOgre::Model::SubEntityDefinition*> */
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

/* method: operator[] of class  std::vector<EmberOgre::Model::SubEntityDefinition*> */
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

/* method: push_back of class  std::vector<EmberOgre::Model::SubEntityDefinition*> */
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

/* method: new of class  std::vector<EmberOgre::Model::SubEntityDefinition*> */
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

/* method: new_local of class  std::vector<EmberOgre::Model::SubEntityDefinition*> */
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

/* method: delete of class  std::vector<EmberOgre::Model::SubEntityDefinition*> */
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

/* method: getSingleton of class  EmberOgre::EmberOgre */
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

/* method: getAvatar of class  EmberOgre::EmberOgre */
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

/* method: getMotionManager of class  EmberOgre::EmberOgre */
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

/* method: getEntityFactory of class  EmberOgre::EmberOgre */
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

/* method: getMainCamera of class  EmberOgre::EmberOgre */
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

/* method: getAvatarController of class  EmberOgre::EmberOgre */
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

/* method: getEmberEntity of class  EmberOgre::EmberOgre */
static int tolua_EmberOgre_EmberOgre_EmberOgre_getEmberEntity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const EmberOgre::EmberOgre",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::EmberOgre* self = (const EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
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

/* method: getMainView of class  EmberOgre::EmberOgre */
static int tolua_EmberOgre_EmberOgre_EmberOgre_getMainView00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"EmberOgre::EmberOgre",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  EmberOgre::EmberOgre* self = (EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMainView'",NULL);
#endif
 {
  Eris::View* tolua_ret = (Eris::View*)  self->getMainView();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Eris::View");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMainView'.",&tolua_err);
 return 0;
#endif
}

/* method: getJesus of class  EmberOgre::EmberOgre */
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

/* method: getRenderWindow of class  EmberOgre::EmberOgre */
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

/* get function: EventCreatedEmberEntityFactory of class  EmberOgre::EmberOgre */
static int tolua_get_EmberOgre__EmberOgre_EventCreatedEmberEntityFactory(lua_State* tolua_S)
{
  EmberOgre::EmberOgre* self = (EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventCreatedEmberEntityFactory'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->EventCreatedEmberEntityFactory,"sigc::signal<void,EmberOgre::EmberEntityFactory*>");
 return 1;
}

/* set function: EventCreatedEmberEntityFactory of class  EmberOgre::EmberOgre */
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

/* get function: EventCreatedAvatarEntity of class  EmberOgre::EmberOgre */
static int tolua_get_EmberOgre__EmberOgre_EventCreatedAvatarEntity(lua_State* tolua_S)
{
  EmberOgre::EmberOgre* self = (EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventCreatedAvatarEntity'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->EventCreatedAvatarEntity,"sigc::signal<void,EmberOgre::AvatarEmberEntity*>");
 return 1;
}

/* set function: EventCreatedAvatarEntity of class  EmberOgre::EmberOgre */
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

/* get function: EventCreatedJesus of class  EmberOgre::EmberOgre */
static int tolua_get_EmberOgre__EmberOgre_EventCreatedJesus(lua_State* tolua_S)
{
  EmberOgre::EmberOgre* self = (EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventCreatedJesus'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->EventCreatedJesus,"sigc::signal<void,EmberOgre::Jesus*>");
 return 1;
}

/* set function: EventCreatedJesus of class  EmberOgre::EmberOgre */
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

/* get function: EventRequestQuit of class  EmberOgre::EmberOgre */
static int tolua_get_EmberOgre__EmberOgre_EventRequestQuit(lua_State* tolua_S)
{
  EmberOgre::EmberOgre* self = (EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventRequestQuit'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->EventRequestQuit,"sigc::signal<void,bool&>");
 return 1;
}

/* set function: EventRequestQuit of class  EmberOgre::EmberOgre */
static int tolua_set_EmberOgre__EmberOgre_EventRequestQuit(lua_State* tolua_S)
{
  EmberOgre::EmberOgre* self = (EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventRequestQuit'",NULL);
 if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,bool&>",0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventRequestQuit = *((sigc::signal<void,bool&>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}

/* get function: EventStartErisPoll of class  EmberOgre::EmberOgre */
static int tolua_get_EmberOgre__EmberOgre_EventStartErisPoll(lua_State* tolua_S)
{
  EmberOgre::EmberOgre* self = (EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventStartErisPoll'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->EventStartErisPoll,"sigc::signal<void>");
 return 1;
}

/* set function: EventStartErisPoll of class  EmberOgre::EmberOgre */
static int tolua_set_EmberOgre__EmberOgre_EventStartErisPoll(lua_State* tolua_S)
{
  EmberOgre::EmberOgre* self = (EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventStartErisPoll'",NULL);
 if (!tolua_isusertype(tolua_S,2,"sigc::signal<void>",0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventStartErisPoll = *((sigc::signal<void>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}

/* get function: EventEndErisPoll of class  EmberOgre::EmberOgre */
static int tolua_get_EmberOgre__EmberOgre_EventEndErisPoll(lua_State* tolua_S)
{
  EmberOgre::EmberOgre* self = (EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventEndErisPoll'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->EventEndErisPoll,"sigc::signal<void>");
 return 1;
}

/* set function: EventEndErisPoll of class  EmberOgre::EmberOgre */
static int tolua_set_EmberOgre__EmberOgre_EventEndErisPoll(lua_State* tolua_S)
{
  EmberOgre::EmberOgre* self = (EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventEndErisPoll'",NULL);
 if (!tolua_isusertype(tolua_S,2,"sigc::signal<void>",0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventEndErisPoll = *((sigc::signal<void>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}

/* method: getWorldSceneNode of class  EmberOgre::EmberOgre */
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

/* method: getRootSceneNode of class  EmberOgre::EmberOgre */
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

/* get function: EventGUIManagerCreated of class  EmberOgre::EmberOgre */
static int tolua_get_EmberOgre__EmberOgre_EventGUIManagerCreated(lua_State* tolua_S)
{
  EmberOgre::EmberOgre* self = (EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventGUIManagerCreated'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->EventGUIManagerCreated,"sigc::signal<void,EmberOgre::GUIManager&>");
 return 1;
}

/* set function: EventGUIManagerCreated of class  EmberOgre::EmberOgre */
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

/* get function: EventGUIManagerInitialized of class  EmberOgre::EmberOgre */
static int tolua_get_EmberOgre__EmberOgre_EventGUIManagerInitialized(lua_State* tolua_S)
{
  EmberOgre::EmberOgre* self = (EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventGUIManagerInitialized'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->EventGUIManagerInitialized,"sigc::signal<void,EmberOgre::GUIManager&>");
 return 1;
}

/* set function: EventGUIManagerInitialized of class  EmberOgre::EmberOgre */
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

/* get function: EventMotionManagerCreated of class  EmberOgre::EmberOgre */
static int tolua_get_EmberOgre__EmberOgre_EventMotionManagerCreated(lua_State* tolua_S)
{
  EmberOgre::EmberOgre* self = (EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventMotionManagerCreated'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->EventMotionManagerCreated,"sigc::signal<void,EmberOgre::MotionManager&>");
 return 1;
}

/* set function: EventMotionManagerCreated of class  EmberOgre::EmberOgre */
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

/* get function: EventTerrainGeneratorCreated of class  EmberOgre::EmberOgre */
static int tolua_get_EmberOgre__EmberOgre_EventTerrainGeneratorCreated(lua_State* tolua_S)
{
  EmberOgre::EmberOgre* self = (EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventTerrainGeneratorCreated'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->EventTerrainGeneratorCreated,"sigc::signal<void,EmberOgre::TerrainGenerator&>");
 return 1;
}

/* set function: EventTerrainGeneratorCreated of class  EmberOgre::EmberOgre */
static int tolua_set_EmberOgre__EmberOgre_EventTerrainGeneratorCreated(lua_State* tolua_S)
{
  EmberOgre::EmberOgre* self = (EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventTerrainGeneratorCreated'",NULL);
 if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,EmberOgre::TerrainGenerator&>",0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventTerrainGeneratorCreated = *((sigc::signal<void,EmberOgre::TerrainGenerator&>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}

/* get function: EventAvatarControllerCreated of class  EmberOgre::EmberOgre */
static int tolua_get_EmberOgre__EmberOgre_EventAvatarControllerCreated(lua_State* tolua_S)
{
  EmberOgre::EmberOgre* self = (EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventAvatarControllerCreated'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->EventAvatarControllerCreated,"sigc::signal<void,EmberOgre::AvatarController&>");
 return 1;
}

/* set function: EventAvatarControllerCreated of class  EmberOgre::EmberOgre */
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

/* get function: EventSceneCreated of class  EmberOgre::EmberOgre */
static int tolua_get_EmberOgre__EmberOgre_EventSceneCreated(lua_State* tolua_S)
{
  EmberOgre::EmberOgre* self = (EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventSceneCreated'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->EventSceneCreated,"sigc::signal<void>");
 return 1;
}

/* set function: EventSceneCreated of class  EmberOgre::EmberOgre */
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

/* method: getEntity of class  EmberOgre::EmberOgre */
static int tolua_EmberOgre_EmberOgre_EmberOgre_getEntity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const EmberOgre::EmberOgre",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::EmberOgre* self = (const EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
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

/* method: requestQuit of class  EmberOgre::EmberOgre */
static int tolua_EmberOgre_EmberOgre_EmberOgre_requestQuit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"EmberOgre::EmberOgre",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  EmberOgre::EmberOgre* self = (EmberOgre::EmberOgre*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'requestQuit'",NULL);
#endif
 {
  self->requestQuit();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'requestQuit'.",&tolua_err);
 return 0;
#endif
}

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
 tolua_endmodule(tolua_S);
#ifdef __cplusplus
 tolua_cclass(tolua_S,"AttachPoint","Carpenter::AttachPoint","",tolua_collect_Carpenter__AttachPoint);
#else
 tolua_cclass(tolua_S,"AttachPoint","Carpenter::AttachPoint","",NULL);
#endif
 tolua_beginmodule(tolua_S,"AttachPoint");
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"BlockSpec","Carpenter::BlockSpec","",NULL);
 tolua_beginmodule(tolua_S,"BlockSpec");
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
 tolua_endmodule(tolua_S);
#ifdef __cplusplus
 tolua_cclass(tolua_S,"BuildingBlockSpecDefinition","Carpenter::BuildingBlockSpecDefinition","",tolua_collect_Carpenter__BuildingBlockSpecDefinition);
#else
 tolua_cclass(tolua_S,"BuildingBlockSpecDefinition","Carpenter::BuildingBlockSpecDefinition","",NULL);
#endif
 tolua_beginmodule(tolua_S,"BuildingBlockSpecDefinition");
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
 tolua_cclass(tolua_S,"BlockSpec","Carpenter::BlockSpec","",NULL);
 tolua_beginmodule(tolua_S,"BlockSpec");
 tolua_function(tolua_S,"getName",tolua_EmberOgre_Carpenter_BlockSpec_getName00);
 tolua_function(tolua_S,"getBoundingBox",tolua_EmberOgre_Carpenter_BlockSpec_getBoundingBox00);
 tolua_function(tolua_S,"getAttachPair",tolua_EmberOgre_Carpenter_BlockSpec_getAttachPair00);
 tolua_function(tolua_S,"addAttachPair",tolua_EmberOgre_Carpenter_BlockSpec_addAttachPair00);
 tolua_function(tolua_S,"setBoundingBox",tolua_EmberOgre_Carpenter_BlockSpec_setBoundingBox00);
 tolua_function(tolua_S,"getAllPoints",tolua_EmberOgre_Carpenter_BlockSpec_getAllPoints00);
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
 tolua_variable(tolua_S,"SETCAMERADISTANCE",tolua_get_EmberOgre__AvatarCamera_SETCAMERADISTANCE,NULL);
 tolua_function(tolua_S,"pitch",tolua_EmberOgre_EmberOgre_AvatarCamera_pitch00);
 tolua_function(tolua_S,"yaw",tolua_EmberOgre_EmberOgre_AvatarCamera_yaw00);
 tolua_function(tolua_S,"getPitch",tolua_EmberOgre_EmberOgre_AvatarCamera_getPitch00);
 tolua_function(tolua_S,"getYaw",tolua_EmberOgre_EmberOgre_AvatarCamera_getYaw00);
 tolua_function(tolua_S,"getCamera",tolua_EmberOgre_EmberOgre_AvatarCamera_getCamera00);
 tolua_function(tolua_S,"getCamera",tolua_EmberOgre_EmberOgre_AvatarCamera_getCamera01);
 tolua_function(tolua_S,"getOrientation",tolua_EmberOgre_EmberOgre_AvatarCamera_getOrientation00);
 tolua_function(tolua_S,"getPosition",tolua_EmberOgre_EmberOgre_AvatarCamera_getPosition00);
 tolua_function(tolua_S,"setMode",tolua_EmberOgre_EmberOgre_AvatarCamera_setMode00);
 tolua_variable(tolua_S,"MovedCamera",tolua_get_EmberOgre__AvatarCamera_MovedCamera,tolua_set_EmberOgre__AvatarCamera_MovedCamera);
 tolua_variable(tolua_S,"EventChangedCameraDistance",tolua_get_EmberOgre__AvatarCamera_EventChangedCameraDistance,tolua_set_EmberOgre__AvatarCamera_EventChangedCameraDistance);
 tolua_function(tolua_S,"setClosestPickingDistance",tolua_EmberOgre_EmberOgre_AvatarCamera_setClosestPickingDistance00);
 tolua_function(tolua_S,"getClosestPickingDistance",tolua_EmberOgre_EmberOgre_AvatarCamera_getClosestPickingDistance00);
 tolua_function(tolua_S,"attach",tolua_EmberOgre_EmberOgre_AvatarCamera_attach00);
 tolua_function(tolua_S,"setCameraDistance",tolua_EmberOgre_EmberOgre_AvatarCamera_setCameraDistance00);
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
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 tolua_module(tolua_S,"EmberOgre",0);
 tolua_beginmodule(tolua_S,"EmberOgre");
 tolua_cclass(tolua_S,"EmberEntityFactory","EmberOgre::EmberEntityFactory","",NULL);
 tolua_beginmodule(tolua_S,"EmberEntityFactory");
 tolua_function(tolua_S,"getWorld",tolua_EmberOgre_EmberOgre_EmberEntityFactory_getWorld00);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 tolua_module(tolua_S,"EmberOgre",0);
 tolua_beginmodule(tolua_S,"EmberOgre");
 tolua_cclass(tolua_S,"EmberEntity","EmberOgre::EmberEntity","Eris::Entity",NULL);
 tolua_beginmodule(tolua_S,"EmberEntity");
 tolua_constant(tolua_S,"CM_AVATAR",EmberOgre::EmberEntity::CM_AVATAR);
 tolua_constant(tolua_S,"CM_ENTITY",EmberOgre::EmberEntity::CM_ENTITY);
 tolua_constant(tolua_S,"CM_NATURE",EmberOgre::EmberEntity::CM_NATURE);
 tolua_constant(tolua_S,"CM_UNDEFINED",EmberOgre::EmberEntity::CM_UNDEFINED);
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
 tolua_function(tolua_S,"EmberPhysicalEntity__getModel",tolua_EmberOgre_EmberOgre_EmberPhysicalEntity_EmberPhysicalEntity__getModel00);
 tolua_function(tolua_S,"EmberPhysicalEntity__getScaleNode",tolua_EmberOgre_EmberOgre_EmberPhysicalEntity_EmberPhysicalEntity__getScaleNode00);
 tolua_function(tolua_S,"setVisible",tolua_EmberOgre_EmberOgre_EmberPhysicalEntity_setVisible00);
 tolua_function(tolua_S,"showOgreBoundingBox",tolua_EmberOgre_EmberOgre_EmberPhysicalEntity_showOgreBoundingBox00);
 tolua_function(tolua_S,"getShowOgreBoundingBox",tolua_EmberOgre_EmberOgre_EmberPhysicalEntity_getShowOgreBoundingBox00);
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
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 tolua_module(tolua_S,"EmberOgre",0);
 tolua_beginmodule(tolua_S,"EmberOgre");
 tolua_cclass(tolua_S,"GUIManager","EmberOgre::GUIManager","",NULL);
 tolua_beginmodule(tolua_S,"GUIManager");
 tolua_function(tolua_S,"getSingleton",tolua_EmberOgre_EmberOgre_GUIManager_getSingleton00);
 tolua_variable(tolua_S,"SCREENSHOT",tolua_get_EmberOgre__GUIManager_SCREENSHOT,NULL);
 tolua_variable(tolua_S,"TOGGLEINPUTMODE",tolua_get_EmberOgre__GUIManager_TOGGLEINPUTMODE,NULL);
 tolua_variable(tolua_S,"AppendIGChatLine",tolua_get_EmberOgre__GUIManager_AppendIGChatLine,tolua_set_EmberOgre__GUIManager_AppendIGChatLine);
 tolua_variable(tolua_S,"AppendOOGChatLine",tolua_get_EmberOgre__GUIManager_AppendOOGChatLine,tolua_set_EmberOgre__GUIManager_AppendOOGChatLine);
 tolua_variable(tolua_S,"EventEntityAction",tolua_get_EmberOgre__GUIManager_EventEntityAction,tolua_set_EmberOgre__GUIManager_EventEntityAction);
 tolua_variable(tolua_S,"EventFrameStarted",tolua_get_EmberOgre__GUIManager_EventFrameStarted,tolua_set_EmberOgre__GUIManager_EventFrameStarted);
 tolua_function(tolua_S,"removeWidget",tolua_EmberOgre_EmberOgre_GUIManager_removeWidget00);
 tolua_function(tolua_S,"addWidget",tolua_EmberOgre_EmberOgre_GUIManager_addWidget00);
 tolua_function(tolua_S,"EmitEntityAction",tolua_EmberOgre_EmberOgre_GUIManager_EmitEntityAction00);
 tolua_function(tolua_S,"getMainSheet",tolua_EmberOgre_EmberOgre_GUIManager_getMainSheet00);
 tolua_function(tolua_S,"setDebugText",tolua_EmberOgre_EmberOgre_GUIManager_setDebugText00);
 tolua_function(tolua_S,"isInGUIMode",tolua_EmberOgre_EmberOgre_GUIManager_isInGUIMode00);
 tolua_function(tolua_S,"isInMovementKeysMode",tolua_EmberOgre_EmberOgre_GUIManager_isInMovementKeysMode00);
 tolua_function(tolua_S,"getMousePicker",tolua_EmberOgre_EmberOgre_GUIManager_getMousePicker00);
 tolua_function(tolua_S,"getInput",tolua_EmberOgre_EmberOgre_GUIManager_getInput00);
 tolua_function(tolua_S,"pushMousePicker",tolua_EmberOgre_EmberOgre_GUIManager_pushMousePicker00);
 tolua_function(tolua_S,"popMousePicker",tolua_EmberOgre_EmberOgre_GUIManager_popMousePicker00);
 tolua_function(tolua_S,"createWidget",tolua_EmberOgre_EmberOgre_GUIManager_createWidget00);
 tolua_function(tolua_S,"createWidget",tolua_EmberOgre_EmberOgre_GUIManager_createWidget01);
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
 tolua_cclass(tolua_S,"MousePickerArgs","EmberOgre::MousePickerArgs","",NULL);
 tolua_beginmodule(tolua_S,"MousePickerArgs");
 tolua_variable(tolua_S,"windowX",tolua_get_EmberOgre__MousePickerArgs_windowX,tolua_set_EmberOgre__MousePickerArgs_windowX);
 tolua_variable(tolua_S,"windowY",tolua_get_EmberOgre__MousePickerArgs_windowY,tolua_set_EmberOgre__MousePickerArgs_windowY);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"MousePicker","EmberOgre::MousePicker","",NULL);
 tolua_beginmodule(tolua_S,"MousePicker");
 tolua_function(tolua_S,"doMousePicking",tolua_EmberOgre_EmberOgre_MousePicker_doMousePicking00);
 tolua_variable(tolua_S,"EventPickedEntity",tolua_get_EmberOgre__MousePicker_EventPickedEntity,tolua_set_EmberOgre__MousePicker_EventPickedEntity);
 tolua_variable(tolua_S,"EventPickedNothing",tolua_get_EmberOgre__MousePicker_EventPickedNothing,tolua_set_EmberOgre__MousePicker_EventPickedNothing);
 tolua_function(tolua_S,"getLastPickedEntity",tolua_EmberOgre_EmberOgre_MousePicker_getLastPickedEntity00);
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
#ifdef __cplusplus
 tolua_cclass(tolua_S,"TerrainArea","EmberOgre::TerrainArea","",tolua_collect_EmberOgre__TerrainArea);
#else
 tolua_cclass(tolua_S,"TerrainArea","EmberOgre::TerrainArea","",NULL);
#endif
 tolua_beginmodule(tolua_S,"TerrainArea");
 tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_TerrainArea_new00);
 tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_TerrainArea_new00_local);
 tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_TerrainArea_new00_local);
 tolua_function(tolua_S,"init",tolua_EmberOgre_EmberOgre_TerrainArea_init00);
 tolua_function(tolua_S,"getArea",tolua_EmberOgre_EmberOgre_TerrainArea_getArea00);
 tolua_function(tolua_S,"setArea",tolua_EmberOgre_EmberOgre_TerrainArea_setArea00);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 tolua_module(tolua_S,"EmberOgre",0);
 tolua_beginmodule(tolua_S,"EmberOgre");
 tolua_cclass(tolua_S,"MotionManager","EmberOgre::MotionManager","",NULL);
 tolua_beginmodule(tolua_S,"MotionManager");
 tolua_function(tolua_S,"getSingleton",tolua_EmberOgre_EmberOgre_MotionManager_getSingleton00);
 tolua_function(tolua_S,"addEntity",tolua_EmberOgre_EmberOgre_MotionManager_addEntity00);
 tolua_function(tolua_S,"removeEntity",tolua_EmberOgre_EmberOgre_MotionManager_removeEntity00);
 tolua_function(tolua_S,"addAnimation",tolua_EmberOgre_EmberOgre_MotionManager_addAnimation00);
 tolua_function(tolua_S,"removeAnimation",tolua_EmberOgre_EmberOgre_MotionManager_removeAnimation00);
 tolua_function(tolua_S,"pauseAnimation",tolua_EmberOgre_EmberOgre_MotionManager_pauseAnimation00);
 tolua_function(tolua_S,"unpauseAnimation",tolua_EmberOgre_EmberOgre_MotionManager_unpauseAnimation00);
 tolua_function(tolua_S,"addAnimatedEntity",tolua_EmberOgre_EmberOgre_MotionManager_addAnimatedEntity00);
 tolua_function(tolua_S,"removeAnimatedEntity",tolua_EmberOgre_EmberOgre_MotionManager_removeAnimatedEntity00);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 tolua_module(tolua_S,"EmberOgre",0);
 tolua_beginmodule(tolua_S,"EmberOgre");
#ifdef __cplusplus
 tolua_cclass(tolua_S,"ColoredListItem","EmberOgre::ColoredListItem","CEGUI::ListboxTextItem",tolua_collect_EmberOgre__ColoredListItem);
#else
 tolua_cclass(tolua_S,"ColoredListItem","EmberOgre::ColoredListItem","CEGUI::ListboxTextItem",NULL);
#endif
 tolua_beginmodule(tolua_S,"ColoredListItem");
 tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_ColoredListItem_new00);
 tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_ColoredListItem_new00_local);
 tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_ColoredListItem_new00_local);
 tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_ColoredListItem_new01);
 tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_ColoredListItem_new01_local);
 tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_ColoredListItem_new01_local);
 tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_ColoredListItem_new02);
 tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_ColoredListItem_new02_local);
 tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_ColoredListItem_new02_local);
 tolua_endmodule(tolua_S);
#ifdef __cplusplus
 tolua_cclass(tolua_S,"Widget","EmberOgre::Widget","",tolua_collect_EmberOgre__Widget);
#else
 tolua_cclass(tolua_S,"Widget","EmberOgre::Widget","",NULL);
#endif
 tolua_beginmodule(tolua_S,"Widget");
 tolua_function(tolua_S,"delete",tolua_EmberOgre_EmberOgre_Widget_delete00);
 tolua_function(tolua_S,"show",tolua_EmberOgre_EmberOgre_Widget_show00);
 tolua_function(tolua_S,"hide",tolua_EmberOgre_EmberOgre_Widget_hide00);
 tolua_function(tolua_S,"getWindow",tolua_EmberOgre_EmberOgre_Widget_getWindow00);
 tolua_function(tolua_S,"enableCloseButton",tolua_EmberOgre_EmberOgre_Widget_enableCloseButton00);
 tolua_function(tolua_S,"registerConsoleVisibilityToggleCommand",tolua_EmberOgre_EmberOgre_Widget_registerConsoleVisibilityToggleCommand00);
 tolua_function(tolua_S,"getMainSheet",tolua_EmberOgre_EmberOgre_Widget_getMainSheet00);
 tolua_function(tolua_S,"getMainWindow",tolua_EmberOgre_EmberOgre_Widget_getMainWindow00);
 tolua_function(tolua_S,"loadMainSheet",tolua_EmberOgre_EmberOgre_Widget_loadMainSheet00);
 tolua_function(tolua_S,"getPrefix",tolua_EmberOgre_EmberOgre_Widget_getPrefix00);
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
 tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_Model_Model_new00);
 tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_Model_Model_new00_local);
 tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_Model_Model_new00_local);
 tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_Model_Model_new01);
 tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_Model_Model_new01_local);
 tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_Model_Model_new01_local);
 tolua_function(tolua_S,"create",tolua_EmberOgre_EmberOgre_Model_Model_create00);
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
 tolua_cclass(tolua_S,"AreaDefinition","EmberOgre::Model::ModelDefinition::AreaDefinition","",NULL);
 tolua_beginmodule(tolua_S,"AreaDefinition");
 tolua_variable(tolua_S,"Id",tolua_get_EmberOgre__Model__ModelDefinition__AreaDefinition_Id,tolua_set_EmberOgre__Model__ModelDefinition__AreaDefinition_Id);
 tolua_variable(tolua_S,"TextureName",tolua_get_EmberOgre__Model__ModelDefinition__AreaDefinition_TextureName,tolua_set_EmberOgre__Model__ModelDefinition__AreaDefinition_TextureName);
 tolua_variable(tolua_S,"MaterialName",tolua_get_EmberOgre__Model__ModelDefinition__AreaDefinition_MaterialName,tolua_set_EmberOgre__Model__ModelDefinition__AreaDefinition_MaterialName);
 tolua_endmodule(tolua_S);
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
#ifdef __cplusplus
 tolua_cclass(tolua_S,"ModelRenderer","EmberOgre::ModelRenderer","",tolua_collect_EmberOgre__ModelRenderer);
#else
 tolua_cclass(tolua_S,"ModelRenderer","EmberOgre::ModelRenderer","",NULL);
#endif
 tolua_beginmodule(tolua_S,"ModelRenderer");
 tolua_function(tolua_S,"new",tolua_EmberOgre_EmberOgre_ModelRenderer_new00);
 tolua_function(tolua_S,"new_local",tolua_EmberOgre_EmberOgre_ModelRenderer_new00_local);
 tolua_function(tolua_S,".call",tolua_EmberOgre_EmberOgre_ModelRenderer_new00_local);
 tolua_function(tolua_S,"delete",tolua_EmberOgre_EmberOgre_ModelRenderer_delete00);
 tolua_function(tolua_S,"showModel",tolua_EmberOgre_EmberOgre_ModelRenderer_showModel00);
 tolua_function(tolua_S,"getModel",tolua_EmberOgre_EmberOgre_ModelRenderer_getModel00);
 tolua_function(tolua_S,"showFullModel",tolua_EmberOgre_EmberOgre_ModelRenderer_showFullModel00);
 tolua_function(tolua_S,"setCameraDistance",tolua_EmberOgre_EmberOgre_ModelRenderer_setCameraDistance00);
 tolua_function(tolua_S,"getCameraDistance",tolua_EmberOgre_EmberOgre_ModelRenderer_getCameraDistance00);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 tolua_module(tolua_S,"EmberOgre",0);
 tolua_beginmodule(tolua_S,"EmberOgre");
 tolua_module(tolua_S,"Model",0);
 tolua_beginmodule(tolua_S,"Model");
 tolua_cclass(tolua_S,"ModelDefinitionManager","EmberOgre::Model::ModelDefinitionManager","Ogre;;ResourceManager",NULL);
 tolua_beginmodule(tolua_S,"ModelDefinitionManager");
 tolua_function(tolua_S,"getSingleton",tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_getSingleton00);
 tolua_function(tolua_S,"getAreaDefinition",tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_getAreaDefinition00);
 tolua_function(tolua_S,"exportScript",tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_exportScript00);
 tolua_function(tolua_S,"create",tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_create00);
 tolua_function(tolua_S,"remove",tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_remove00);
 tolua_function(tolua_S,"getByName",tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_getByName00);
 tolua_function(tolua_S,"resourceExists",tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_resourceExists00);
 tolua_function(tolua_S,"getResourceIterator",tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_getResourceIterator00);
 tolua_function(tolua_S,"getAllMeshes",tolua_EmberOgre_EmberOgre_Model_ModelDefinitionManager_getAllMeshes00);
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
 tolua_function(tolua_S,"getEmberEntity",tolua_EmberOgre_EmberOgre_EmberOgre_getEmberEntity00);
 tolua_function(tolua_S,"getMainView",tolua_EmberOgre_EmberOgre_EmberOgre_getMainView00);
 tolua_function(tolua_S,"getJesus",tolua_EmberOgre_EmberOgre_EmberOgre_getJesus00);
 tolua_function(tolua_S,"getRenderWindow",tolua_EmberOgre_EmberOgre_EmberOgre_getRenderWindow00);
 tolua_variable(tolua_S,"EventCreatedEmberEntityFactory",tolua_get_EmberOgre__EmberOgre_EventCreatedEmberEntityFactory,tolua_set_EmberOgre__EmberOgre_EventCreatedEmberEntityFactory);
 tolua_variable(tolua_S,"EventCreatedAvatarEntity",tolua_get_EmberOgre__EmberOgre_EventCreatedAvatarEntity,tolua_set_EmberOgre__EmberOgre_EventCreatedAvatarEntity);
 tolua_variable(tolua_S,"EventCreatedJesus",tolua_get_EmberOgre__EmberOgre_EventCreatedJesus,tolua_set_EmberOgre__EmberOgre_EventCreatedJesus);
 tolua_variable(tolua_S,"EventRequestQuit",tolua_get_EmberOgre__EmberOgre_EventRequestQuit,tolua_set_EmberOgre__EmberOgre_EventRequestQuit);
 tolua_variable(tolua_S,"EventStartErisPoll",tolua_get_EmberOgre__EmberOgre_EventStartErisPoll,tolua_set_EmberOgre__EmberOgre_EventStartErisPoll);
 tolua_variable(tolua_S,"EventEndErisPoll",tolua_get_EmberOgre__EmberOgre_EventEndErisPoll,tolua_set_EmberOgre__EmberOgre_EventEndErisPoll);
 tolua_function(tolua_S,"getWorldSceneNode",tolua_EmberOgre_EmberOgre_EmberOgre_getWorldSceneNode00);
 tolua_function(tolua_S,"getRootSceneNode",tolua_EmberOgre_EmberOgre_EmberOgre_getRootSceneNode00);
 tolua_variable(tolua_S,"EventGUIManagerCreated",tolua_get_EmberOgre__EmberOgre_EventGUIManagerCreated,tolua_set_EmberOgre__EmberOgre_EventGUIManagerCreated);
 tolua_variable(tolua_S,"EventGUIManagerInitialized",tolua_get_EmberOgre__EmberOgre_EventGUIManagerInitialized,tolua_set_EmberOgre__EmberOgre_EventGUIManagerInitialized);
 tolua_variable(tolua_S,"EventMotionManagerCreated",tolua_get_EmberOgre__EmberOgre_EventMotionManagerCreated,tolua_set_EmberOgre__EmberOgre_EventMotionManagerCreated);
 tolua_variable(tolua_S,"EventTerrainGeneratorCreated",tolua_get_EmberOgre__EmberOgre_EventTerrainGeneratorCreated,tolua_set_EmberOgre__EmberOgre_EventTerrainGeneratorCreated);
 tolua_variable(tolua_S,"EventAvatarControllerCreated",tolua_get_EmberOgre__EmberOgre_EventAvatarControllerCreated,tolua_set_EmberOgre__EmberOgre_EventAvatarControllerCreated);
 tolua_variable(tolua_S,"EventSceneCreated",tolua_get_EmberOgre__EmberOgre_EventSceneCreated,tolua_set_EmberOgre__EmberOgre_EventSceneCreated);
 tolua_function(tolua_S,"getEntity",tolua_EmberOgre_EmberOgre_EmberOgre_getEntity00);
 tolua_function(tolua_S,"requestQuit",tolua_EmberOgre_EmberOgre_EmberOgre_requestQuit00);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}
