/*
** Lua binding: Ogre
** Generated automatically by tolua++-1.0.6 on Sat Mar 18 16:03:09 2006.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int tolua_Ogre_open (lua_State* tolua_S);

#include "required.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_ushort (lua_State* tolua_S)
{
 ushort* self = (ushort*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_Ogre__MeshPtr (lua_State* tolua_S)
{
 Ogre::MeshPtr* self = (Ogre::MeshPtr*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_Ogre__ResourcePtr (lua_State* tolua_S)
{
 Ogre::ResourcePtr* self = (Ogre::ResourcePtr*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_Ogre__ResourceManager__ResourceHandleMap__mapped_type (lua_State* tolua_S)
{
 Ogre::ResourceManager::ResourceHandleMap::mapped_type* self = (Ogre::ResourceManager::ResourceHandleMap::mapped_type*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_Ogre__MapIterator_Ogre__Entity__ChildObjectList_ (lua_State* tolua_S)
{
 Ogre::MapIterator<Ogre::Entity::ChildObjectList>* self = (Ogre::MapIterator<Ogre::Entity::ChildObjectList>*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_Ogre__Vector3 (lua_State* tolua_S)
{
 Ogre::Vector3* self = (Ogre::Vector3*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_uint (lua_State* tolua_S)
{
 uint* self = (uint*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_Ogre__ResourceManager__ResourceHandleMap__key_type (lua_State* tolua_S)
{
 Ogre::ResourceManager::ResourceHandleMap::key_type* self = (Ogre::ResourceManager::ResourceHandleMap::key_type*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_Ogre__MapIterator_Ogre__ResourceManager__ResourceHandleMap_ (lua_State* tolua_S)
{
 Ogre::MapIterator<Ogre::ResourceManager::ResourceHandleMap>* self = (Ogre::MapIterator<Ogre::ResourceManager::ResourceHandleMap>*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_Ogre__Quaternion (lua_State* tolua_S)
{
 Ogre::Quaternion* self = (Ogre::Quaternion*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_Ogre__AxisAlignedBox (lua_State* tolua_S)
{
 Ogre::AxisAlignedBox* self = (Ogre::AxisAlignedBox*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_Ogre__MaterialPtr (lua_State* tolua_S)
{
 Ogre::MaterialPtr* self = (Ogre::MaterialPtr*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"Ogre::RenderTarget::FrameStats");
 tolua_usertype(tolua_S,"Ogre::Mesh");
 tolua_usertype(tolua_S,"Ogre::AnimationState");
 tolua_usertype(tolua_S,"Ogre::SkeletonInstance");
 tolua_usertype(tolua_S,"std::set<Ogre::Entity*>");
 tolua_usertype(tolua_S,"Ogre::MapIterator<Ogre::Entity::ChildObjectList>");
 tolua_usertype(tolua_S,"Ogre::Sphere");
 tolua_usertype(tolua_S,"Ogre::TagPoint");
 tolua_usertype(tolua_S,"Ogre::Resource");
 tolua_usertype(tolua_S,"Ogre::RenderWindow");
 tolua_usertype(tolua_S,"Ogre::ResourceManager::ResourceHandleMap::key_type");
 tolua_usertype(tolua_S,"Ogre::Quaternion");
 tolua_usertype(tolua_S,"Ogre::ResourceManager");
 tolua_usertype(tolua_S,"Ogre::AxisAlignedBox");
 tolua_usertype(tolua_S,"Ogre::MaterialPtr");
 tolua_usertype(tolua_S,"ushort");
 tolua_usertype(tolua_S,"Ogre::MeshManager");
 tolua_usertype(tolua_S,"Ogre::MeshPtr");
 tolua_usertype(tolua_S,"Ogre::ResourcePtr");
 tolua_usertype(tolua_S,"Ogre::ResourceManager::ResourceHandleMap::mapped_type");
 tolua_usertype(tolua_S,"Ogre::MovableObject");
 tolua_usertype(tolua_S,"Ogre::Material");
 tolua_usertype(tolua_S,"Ogre::MaterialManager");
 tolua_usertype(tolua_S,"Ogre::Entity");
 tolua_usertype(tolua_S,"Ogre::Vector3");
 tolua_usertype(tolua_S,"Ogre::Radian");
 tolua_usertype(tolua_S,"Ogre::AnimationStateSet");
 tolua_usertype(tolua_S,"uint");
 tolua_usertype(tolua_S,"Ogre::SubEntity");
 tolua_usertype(tolua_S,"Ogre::RenderTarget");
 tolua_usertype(tolua_S,"Ogre::MapIterator<Ogre::ResourceManager::ResourceHandleMap>");
}

/* get function: lastFPS of class  FrameStats */
static int tolua_get_Ogre__RenderTarget__FrameStats_lastFPS(lua_State* tolua_S)
{
  Ogre::RenderTarget::FrameStats* self = (Ogre::RenderTarget::FrameStats*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'lastFPS'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->lastFPS);
 return 1;
}

/* set function: lastFPS of class  FrameStats */
static int tolua_set_Ogre__RenderTarget__FrameStats_lastFPS(lua_State* tolua_S)
{
  Ogre::RenderTarget::FrameStats* self = (Ogre::RenderTarget::FrameStats*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'lastFPS'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->lastFPS = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}

/* get function: avgFPS of class  FrameStats */
static int tolua_get_Ogre__RenderTarget__FrameStats_avgFPS(lua_State* tolua_S)
{
  Ogre::RenderTarget::FrameStats* self = (Ogre::RenderTarget::FrameStats*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'avgFPS'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->avgFPS);
 return 1;
}

/* set function: avgFPS of class  FrameStats */
static int tolua_set_Ogre__RenderTarget__FrameStats_avgFPS(lua_State* tolua_S)
{
  Ogre::RenderTarget::FrameStats* self = (Ogre::RenderTarget::FrameStats*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'avgFPS'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->avgFPS = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}

/* get function: bestFPS of class  FrameStats */
static int tolua_get_Ogre__RenderTarget__FrameStats_bestFPS(lua_State* tolua_S)
{
  Ogre::RenderTarget::FrameStats* self = (Ogre::RenderTarget::FrameStats*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'bestFPS'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->bestFPS);
 return 1;
}

/* set function: bestFPS of class  FrameStats */
static int tolua_set_Ogre__RenderTarget__FrameStats_bestFPS(lua_State* tolua_S)
{
  Ogre::RenderTarget::FrameStats* self = (Ogre::RenderTarget::FrameStats*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'bestFPS'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->bestFPS = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}

/* get function: worstFPS of class  FrameStats */
static int tolua_get_Ogre__RenderTarget__FrameStats_worstFPS(lua_State* tolua_S)
{
  Ogre::RenderTarget::FrameStats* self = (Ogre::RenderTarget::FrameStats*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'worstFPS'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->worstFPS);
 return 1;
}

/* set function: worstFPS of class  FrameStats */
static int tolua_set_Ogre__RenderTarget__FrameStats_worstFPS(lua_State* tolua_S)
{
  Ogre::RenderTarget::FrameStats* self = (Ogre::RenderTarget::FrameStats*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'worstFPS'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->worstFPS = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}

/* get function: bestFrameTime of class  FrameStats */
static int tolua_get_Ogre__RenderTarget__FrameStats_unsigned_bestFrameTime(lua_State* tolua_S)
{
  Ogre::RenderTarget::FrameStats* self = (Ogre::RenderTarget::FrameStats*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'bestFrameTime'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->bestFrameTime);
 return 1;
}

/* set function: bestFrameTime of class  FrameStats */
static int tolua_set_Ogre__RenderTarget__FrameStats_unsigned_bestFrameTime(lua_State* tolua_S)
{
  Ogre::RenderTarget::FrameStats* self = (Ogre::RenderTarget::FrameStats*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'bestFrameTime'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->bestFrameTime = ((unsigned long)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}

/* get function: worstFrameTime of class  FrameStats */
static int tolua_get_Ogre__RenderTarget__FrameStats_unsigned_worstFrameTime(lua_State* tolua_S)
{
  Ogre::RenderTarget::FrameStats* self = (Ogre::RenderTarget::FrameStats*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'worstFrameTime'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->worstFrameTime);
 return 1;
}

/* set function: worstFrameTime of class  FrameStats */
static int tolua_set_Ogre__RenderTarget__FrameStats_unsigned_worstFrameTime(lua_State* tolua_S)
{
  Ogre::RenderTarget::FrameStats* self = (Ogre::RenderTarget::FrameStats*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'worstFrameTime'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->worstFrameTime = ((unsigned long)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}

/* get function: triangleCount of class  FrameStats */
static int tolua_get_Ogre__RenderTarget__FrameStats_unsigned_triangleCount(lua_State* tolua_S)
{
  Ogre::RenderTarget::FrameStats* self = (Ogre::RenderTarget::FrameStats*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'triangleCount'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->triangleCount);
 return 1;
}

/* set function: triangleCount of class  FrameStats */
static int tolua_set_Ogre__RenderTarget__FrameStats_unsigned_triangleCount(lua_State* tolua_S)
{
  Ogre::RenderTarget::FrameStats* self = (Ogre::RenderTarget::FrameStats*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'triangleCount'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->triangleCount = ((unsigned long)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}

/* method: getStatistics of class  Ogre::RenderTarget */
static int tolua_Ogre_Ogre_RenderTarget_getStatistics00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::RenderTarget",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::RenderTarget* self = (const Ogre::RenderTarget*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getStatistics'",NULL);
#endif
 {
  const Ogre::RenderTarget::FrameStats& tolua_ret = (const Ogre::RenderTarget::FrameStats&)  self->getStatistics();
 tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Ogre::RenderTarget::FrameStats");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getStatistics'.",&tolua_err);
 return 0;
#endif
}

/* method: hasMoreElements of class  Ogre::MapIterator<Ogre::ResourceManager::ResourceHandleMap> */
static int tolua_Ogre_Ogre_MapIterator_Ogre__ResourceManager__ResourceHandleMap__hasMoreElements00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::MapIterator<Ogre::ResourceManager::ResourceHandleMap>",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::MapIterator<Ogre::ResourceManager::ResourceHandleMap>* self = (const Ogre::MapIterator<Ogre::ResourceManager::ResourceHandleMap>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hasMoreElements'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->hasMoreElements();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'hasMoreElements'.",&tolua_err);
 return 0;
#endif
}

/* method: getNext of class  Ogre::MapIterator<Ogre::ResourceManager::ResourceHandleMap> */
static int tolua_Ogre_Ogre_MapIterator_Ogre__ResourceManager__ResourceHandleMap__getNext00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::MapIterator<Ogre::ResourceManager::ResourceHandleMap>",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::MapIterator<Ogre::ResourceManager::ResourceHandleMap>* self = (Ogre::MapIterator<Ogre::ResourceManager::ResourceHandleMap>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getNext'",NULL);
#endif
 {
  Ogre::ResourceManager::ResourceHandleMap::mapped_type tolua_ret = (Ogre::ResourceManager::ResourceHandleMap::mapped_type)  self->getNext();
 {
#ifdef __cplusplus
 void* tolua_obj = new Ogre::ResourceManager::ResourceHandleMap::mapped_type(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::ResourceManager::ResourceHandleMap::mapped_type");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::ResourceManager::ResourceHandleMap::mapped_type));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::ResourceManager::ResourceHandleMap::mapped_type");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getNext'.",&tolua_err);
 return 0;
#endif
}

/* method: peekNextValue of class  Ogre::MapIterator<Ogre::ResourceManager::ResourceHandleMap> */
static int tolua_Ogre_Ogre_MapIterator_Ogre__ResourceManager__ResourceHandleMap__peekNextValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::MapIterator<Ogre::ResourceManager::ResourceHandleMap>",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::MapIterator<Ogre::ResourceManager::ResourceHandleMap>* self = (Ogre::MapIterator<Ogre::ResourceManager::ResourceHandleMap>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'peekNextValue'",NULL);
#endif
 {
  Ogre::ResourceManager::ResourceHandleMap::mapped_type tolua_ret = (Ogre::ResourceManager::ResourceHandleMap::mapped_type)  self->peekNextValue();
 {
#ifdef __cplusplus
 void* tolua_obj = new Ogre::ResourceManager::ResourceHandleMap::mapped_type(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::ResourceManager::ResourceHandleMap::mapped_type");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::ResourceManager::ResourceHandleMap::mapped_type));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::ResourceManager::ResourceHandleMap::mapped_type");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'peekNextValue'.",&tolua_err);
 return 0;
#endif
}

/* method: peekNextKey of class  Ogre::MapIterator<Ogre::ResourceManager::ResourceHandleMap> */
static int tolua_Ogre_Ogre_MapIterator_Ogre__ResourceManager__ResourceHandleMap__peekNextKey00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::MapIterator<Ogre::ResourceManager::ResourceHandleMap>",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::MapIterator<Ogre::ResourceManager::ResourceHandleMap>* self = (Ogre::MapIterator<Ogre::ResourceManager::ResourceHandleMap>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'peekNextKey'",NULL);
#endif
 {
  Ogre::ResourceManager::ResourceHandleMap::key_type tolua_ret = (Ogre::ResourceManager::ResourceHandleMap::key_type)  self->peekNextKey();
 {
#ifdef __cplusplus
 void* tolua_obj = new Ogre::ResourceManager::ResourceHandleMap::key_type(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::ResourceManager::ResourceHandleMap::key_type");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::ResourceManager::ResourceHandleMap::key_type));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::ResourceManager::ResourceHandleMap::key_type");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'peekNextKey'.",&tolua_err);
 return 0;
#endif
}

/* method: moveNext of class  Ogre::MapIterator<Ogre::ResourceManager::ResourceHandleMap> */
static int tolua_Ogre_Ogre_MapIterator_Ogre__ResourceManager__ResourceHandleMap__moveNext00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::MapIterator<Ogre::ResourceManager::ResourceHandleMap>",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::MapIterator<Ogre::ResourceManager::ResourceHandleMap>* self = (Ogre::MapIterator<Ogre::ResourceManager::ResourceHandleMap>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'moveNext'",NULL);
#endif
 {
  self->moveNext();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'moveNext'.",&tolua_err);
 return 0;
#endif
}

/* method: load of class  Ogre::Resource */
static int tolua_Ogre_Ogre_Resource_load00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Resource",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Resource* self = (Ogre::Resource*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'load'",NULL);
#endif
 {
  self->load();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'load'.",&tolua_err);
 return 0;
#endif
}

/* method: reload of class  Ogre::Resource */
static int tolua_Ogre_Ogre_Resource_reload00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Resource",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Resource* self = (Ogre::Resource*)  tolua_tousertype(tolua_S,1,0);
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

/* method: isManuallyLoaded of class  Ogre::Resource */
static int tolua_Ogre_Ogre_Resource_isManuallyLoaded00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::Resource",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::Resource* self = (const Ogre::Resource*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isManuallyLoaded'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isManuallyLoaded();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isManuallyLoaded'.",&tolua_err);
 return 0;
#endif
}

/* method: unload of class  Ogre::Resource */
static int tolua_Ogre_Ogre_Resource_unload00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Resource",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Resource* self = (Ogre::Resource*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'unload'",NULL);
#endif
 {
  self->unload();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'unload'.",&tolua_err);
 return 0;
#endif
}

/* method: getSize of class  Ogre::Resource */
static int tolua_Ogre_Ogre_Resource_getSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::Resource",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::Resource* self = (const Ogre::Resource*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSize'",NULL);
#endif
 {
  uint tolua_ret = (uint)  self->getSize();
 {
#ifdef __cplusplus
 void* tolua_obj = new uint(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"uint");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(uint));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"uint");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSize'.",&tolua_err);
 return 0;
#endif
}

/* method: touch of class  Ogre::Resource */
static int tolua_Ogre_Ogre_Resource_touch00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Resource",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Resource* self = (Ogre::Resource*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'touch'",NULL);
#endif
 {
  self->touch();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'touch'.",&tolua_err);
 return 0;
#endif
}

/* method: getName of class  Ogre::Resource */
static int tolua_Ogre_Ogre_Resource_getName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::Resource",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::Resource* self = (const Ogre::Resource*)  tolua_tousertype(tolua_S,1,0);
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

/* method: isLoaded of class  Ogre::Resource */
static int tolua_Ogre_Ogre_Resource_isLoaded00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::Resource",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::Resource* self = (const Ogre::Resource*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isLoaded'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isLoaded();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isLoaded'.",&tolua_err);
 return 0;
#endif
}

/* method: getGroup of class  Ogre::Resource */
static int tolua_Ogre_Ogre_Resource_getGroup00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Resource",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Resource* self = (Ogre::Resource*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getGroup'",NULL);
#endif
 {
  const std::string tolua_ret = (const std::string)  self->getGroup();
 tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getGroup'.",&tolua_err);
 return 0;
#endif
}

/* method: getOrigin of class  Ogre::Resource */
static int tolua_Ogre_Ogre_Resource_getOrigin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::Resource",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::Resource* self = (const Ogre::Resource*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getOrigin'",NULL);
#endif
 {
  const std::string tolua_ret = (const std::string)  self->getOrigin();
 tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getOrigin'.",&tolua_err);
 return 0;
#endif
}

/* method: create of class  Ogre::ResourceManager */
static int tolua_Ogre_Ogre_ResourceManager_create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::ResourceManager",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::ResourceManager* self = (Ogre::ResourceManager*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string group = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'create'",NULL);
#endif
 {
  Ogre::ResourcePtr tolua_ret = (Ogre::ResourcePtr)  self->create(name,group);
 {
#ifdef __cplusplus
 void* tolua_obj = new Ogre::ResourcePtr(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::ResourcePtr");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::ResourcePtr));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::ResourcePtr");
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

/* method: reloadAll of class  Ogre::ResourceManager */
static int tolua_Ogre_Ogre_ResourceManager_reloadAll00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::ResourceManager",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::ResourceManager* self = (Ogre::ResourceManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'reloadAll'",NULL);
#endif
 {
  self->reloadAll();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'reloadAll'.",&tolua_err);
 return 0;
#endif
}

/* method: remove of class  Ogre::ResourceManager */
static int tolua_Ogre_Ogre_ResourceManager_remove00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::ResourceManager",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"Ogre::ResourcePtr",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::ResourceManager* self = (Ogre::ResourceManager*)  tolua_tousertype(tolua_S,1,0);
  Ogre::ResourcePtr* r = ((Ogre::ResourcePtr*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'remove'",NULL);
#endif
 {
  self->remove(*r);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'remove'.",&tolua_err);
 return 0;
#endif
}

/* method: remove of class  Ogre::ResourceManager */
static int tolua_Ogre_Ogre_ResourceManager_remove01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::ResourceManager",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  Ogre::ResourceManager* self = (Ogre::ResourceManager*)  tolua_tousertype(tolua_S,1,0);
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
tolua_lerror:
 return tolua_Ogre_Ogre_ResourceManager_remove00(tolua_S);
}

/* method: getByName of class  Ogre::ResourceManager */
static int tolua_Ogre_Ogre_ResourceManager_getByName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::ResourceManager",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::ResourceManager* self = (Ogre::ResourceManager*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getByName'",NULL);
#endif
 {
  Ogre::ResourcePtr tolua_ret = (Ogre::ResourcePtr)  self->getByName(name);
 {
#ifdef __cplusplus
 void* tolua_obj = new Ogre::ResourcePtr(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::ResourcePtr");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::ResourcePtr));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::ResourcePtr");
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

/* method: resourceExists of class  Ogre::ResourceManager */
static int tolua_Ogre_Ogre_ResourceManager_resourceExists00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::ResourceManager",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::ResourceManager* self = (Ogre::ResourceManager*)  tolua_tousertype(tolua_S,1,0);
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

/* method: getResourceIterator of class  Ogre::ResourceManager */
static int tolua_Ogre_Ogre_ResourceManager_getResourceIterator00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::ResourceManager",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::ResourceManager* self = (Ogre::ResourceManager*)  tolua_tousertype(tolua_S,1,0);
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

/* method: getSingleton of class  Ogre::MaterialManager */
static int tolua_Ogre_Ogre_MaterialManager_getSingleton00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"Ogre::MaterialManager",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
 {
  Ogre::MaterialManager& tolua_ret = (Ogre::MaterialManager&)  Ogre::MaterialManager::getSingleton();
 tolua_pushusertype(tolua_S,(void*)&tolua_ret,"Ogre::MaterialManager");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSingleton'.",&tolua_err);
 return 0;
#endif
}

/* method: load of class  Ogre::MaterialManager */
static int tolua_Ogre_Ogre_MaterialManager_load00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::MaterialManager",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::MaterialManager* self = (Ogre::MaterialManager*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string group = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'load'",NULL);
#endif
 {
  Ogre::MaterialPtr tolua_ret = (Ogre::MaterialPtr)  self->load(name,group);
 {
#ifdef __cplusplus
 void* tolua_obj = new Ogre::MaterialPtr(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::MaterialPtr");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::MaterialPtr));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::MaterialPtr");
#endif
 }
 tolua_pushcppstring(tolua_S,(const char*)name);
 tolua_pushcppstring(tolua_S,(const char*)group);
 }
 }
 return 3;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'load'.",&tolua_err);
 return 0;
#endif
}

/* method: getByName of class  Ogre::MaterialManager */
static int tolua_Ogre_Ogre_MaterialManager_getByName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::MaterialManager",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::MaterialManager* self = (Ogre::MaterialManager*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getByName'",NULL);
#endif
 {
  Ogre::MaterialPtr tolua_ret = (Ogre::MaterialPtr)  self->getByName(name);
 {
#ifdef __cplusplus
 void* tolua_obj = new Ogre::MaterialPtr(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::MaterialPtr");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::MaterialPtr));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::MaterialPtr");
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

/* method: create of class  Ogre::MaterialManager */
static int tolua_Ogre_Ogre_MaterialManager_create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::MaterialManager",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::MaterialManager* self = (Ogre::MaterialManager*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string group = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'create'",NULL);
#endif
 {
  Ogre::MaterialPtr tolua_ret = (Ogre::MaterialPtr)  self->create(name,group);
 {
#ifdef __cplusplus
 void* tolua_obj = new Ogre::MaterialPtr(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::MaterialPtr");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::MaterialPtr));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::MaterialPtr");
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

/* method: isTransparent of class  Ogre::Material */
static int tolua_Ogre_Ogre_Material_isTransparent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::Material",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::Material* self = (const Ogre::Material*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isTransparent'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isTransparent();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isTransparent'.",&tolua_err);
 return 0;
#endif
}

/* method: setReceiveShadows of class  Ogre::Material */
static int tolua_Ogre_Ogre_Material_setReceiveShadows00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Material",0,&tolua_err) ||
 !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Material* self = (Ogre::Material*)  tolua_tousertype(tolua_S,1,0);
  bool enabled = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setReceiveShadows'",NULL);
#endif
 {
  self->setReceiveShadows(enabled);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setReceiveShadows'.",&tolua_err);
 return 0;
#endif
}

/* method: getReceiveShadows of class  Ogre::Material */
static int tolua_Ogre_Ogre_Material_getReceiveShadows00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::Material",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::Material* self = (const Ogre::Material*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getReceiveShadows'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->getReceiveShadows();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getReceiveShadows'.",&tolua_err);
 return 0;
#endif
}

/* method: setTransparencyCastsShadows of class  Ogre::Material */
static int tolua_Ogre_Ogre_Material_setTransparencyCastsShadows00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Material",0,&tolua_err) ||
 !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Material* self = (Ogre::Material*)  tolua_tousertype(tolua_S,1,0);
  bool enabled = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setTransparencyCastsShadows'",NULL);
#endif
 {
  self->setTransparencyCastsShadows(enabled);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setTransparencyCastsShadows'.",&tolua_err);
 return 0;
#endif
}

/* method: getTransparencyCastsShadows of class  Ogre::Material */
static int tolua_Ogre_Ogre_Material_getTransparencyCastsShadows00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::Material",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::Material* self = (const Ogre::Material*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTransparencyCastsShadows'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->getTransparencyCastsShadows();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTransparencyCastsShadows'.",&tolua_err);
 return 0;
#endif
}

/* method: getNumTechniques of class  Ogre::Material */
static int tolua_Ogre_Ogre_Material_getNumTechniques00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::Material",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::Material* self = (const Ogre::Material*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getNumTechniques'",NULL);
#endif
 {
  unsigned short tolua_ret = (unsigned short)  self->getNumTechniques();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getNumTechniques'.",&tolua_err);
 return 0;
#endif
}

/* method: touch of class  Ogre::Material */
static int tolua_Ogre_Ogre_Material_touch00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Material",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Material* self = (Ogre::Material*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'touch'",NULL);
#endif
 {
  self->touch();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'touch'.",&tolua_err);
 return 0;
#endif
}

/* method: get of class  Ogre::MaterialPtr */
static int tolua_Ogre_Ogre_MaterialPtr_get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::MaterialPtr",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::MaterialPtr* self = (Ogre::MaterialPtr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get'",NULL);
#endif
 {
  Ogre::Material* tolua_ret = (Ogre::Material*)  self->get();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::Material");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get'.",&tolua_err);
 return 0;
#endif
}

/* method: getMaterialName of class  Ogre::SubEntity */
static int tolua_Ogre_Ogre_SubEntity_getMaterialName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::SubEntity",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::SubEntity* self = (const Ogre::SubEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMaterialName'",NULL);
#endif
 {
  const std::string tolua_ret = (const std::string)  self->getMaterialName();
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

/* method: setMaterialName of class  Ogre::SubEntity */
static int tolua_Ogre_Ogre_SubEntity_setMaterialName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::SubEntity",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::SubEntity* self = (Ogre::SubEntity*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMaterialName'",NULL);
#endif
 {
  self->setMaterialName(name);
 tolua_pushcppstring(tolua_S,(const char*)name);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setMaterialName'.",&tolua_err);
 return 0;
#endif
}

/* method: setVisible of class  Ogre::SubEntity */
static int tolua_Ogre_Ogre_SubEntity_setVisible00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::SubEntity",0,&tolua_err) ||
 !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::SubEntity* self = (Ogre::SubEntity*)  tolua_tousertype(tolua_S,1,0);
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

/* method: isVisible of class  Ogre::SubEntity */
static int tolua_Ogre_Ogre_SubEntity_isVisible00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::SubEntity",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::SubEntity* self = (const Ogre::SubEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isVisible'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isVisible();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isVisible'.",&tolua_err);
 return 0;
#endif
}

/* method: getMaterial of class  Ogre::SubEntity */
static int tolua_Ogre_Ogre_SubEntity_getMaterial00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::SubEntity",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::SubEntity* self = (const Ogre::SubEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMaterial'",NULL);
#endif
 {
  const Ogre::MaterialPtr& tolua_ret = (const Ogre::MaterialPtr&)  self->getMaterial();
 tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Ogre::MaterialPtr");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMaterial'.",&tolua_err);
 return 0;
#endif
}

/* method: getSingleton of class  Ogre::MeshManager */
static int tolua_Ogre_Ogre_MeshManager_getSingleton00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"Ogre::MeshManager",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
 {
  Ogre::MeshManager& tolua_ret = (Ogre::MeshManager&)  Ogre::MeshManager::getSingleton();
 tolua_pushusertype(tolua_S,(void*)&tolua_ret,"Ogre::MeshManager");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSingleton'.",&tolua_err);
 return 0;
#endif
}

/* method: load of class  Ogre::MeshManager */
static int tolua_Ogre_Ogre_MeshManager_load00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::MeshManager",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::MeshManager* self = (Ogre::MeshManager*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string group = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'load'",NULL);
#endif
 {
  Ogre::MeshPtr tolua_ret = (Ogre::MeshPtr)  self->load(name,group);
 {
#ifdef __cplusplus
 void* tolua_obj = new Ogre::MeshPtr(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::MeshPtr");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::MeshPtr));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::MeshPtr");
#endif
 }
 tolua_pushcppstring(tolua_S,(const char*)name);
 tolua_pushcppstring(tolua_S,(const char*)group);
 }
 }
 return 3;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'load'.",&tolua_err);
 return 0;
#endif
}

/* method: getByName of class  Ogre::MeshManager */
static int tolua_Ogre_Ogre_MeshManager_getByName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::MeshManager",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::MeshManager* self = (Ogre::MeshManager*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getByName'",NULL);
#endif
 {
  Ogre::MeshPtr tolua_ret = (Ogre::MeshPtr)  self->getByName(name);
 {
#ifdef __cplusplus
 void* tolua_obj = new Ogre::MeshPtr(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::MeshPtr");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::MeshPtr));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::MeshPtr");
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

/* method: create of class  Ogre::MeshManager */
static int tolua_Ogre_Ogre_MeshManager_create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::MeshManager",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::MeshManager* self = (Ogre::MeshManager*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string group = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'create'",NULL);
#endif
 {
  Ogre::MeshPtr tolua_ret = (Ogre::MeshPtr)  self->create(name,group);
 {
#ifdef __cplusplus
 void* tolua_obj = new Ogre::MeshPtr(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::MeshPtr");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::MeshPtr));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::MeshPtr");
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

/* method: getNumSubMeshes of class  Ogre::Mesh */
static int tolua_Ogre_Ogre_Mesh_getNumSubMeshes00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::Mesh",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::Mesh* self = (const Ogre::Mesh*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getNumSubMeshes'",NULL);
#endif
 {
  unsigned short tolua_ret = (unsigned short)  self->getNumSubMeshes();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getNumSubMeshes'.",&tolua_err);
 return 0;
#endif
}

/* method: hasSkeleton of class  Ogre::Mesh */
static int tolua_Ogre_Ogre_Mesh_hasSkeleton00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::Mesh",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::Mesh* self = (const Ogre::Mesh*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hasSkeleton'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->hasSkeleton();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'hasSkeleton'.",&tolua_err);
 return 0;
#endif
}

/* method: getSkeletonName of class  Ogre::Mesh */
static int tolua_Ogre_Ogre_Mesh_getSkeletonName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::Mesh",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::Mesh* self = (const Ogre::Mesh*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSkeletonName'",NULL);
#endif
 {
  const std::string tolua_ret = (const std::string)  self->getSkeletonName();
 tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSkeletonName'.",&tolua_err);
 return 0;
#endif
}

/* method: getNumLodLevels of class  Ogre::Mesh */
static int tolua_Ogre_Ogre_Mesh_getNumLodLevels00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::Mesh",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::Mesh* self = (const Ogre::Mesh*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getNumLodLevels'",NULL);
#endif
 {
  ushort tolua_ret = (ushort)  self->getNumLodLevels();
 {
#ifdef __cplusplus
 void* tolua_obj = new ushort(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"ushort");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(ushort));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"ushort");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getNumLodLevels'.",&tolua_err);
 return 0;
#endif
}

/* method: get of class  Ogre::MeshPtr */
static int tolua_Ogre_Ogre_MeshPtr_get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::MeshPtr",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::MeshPtr* self = (Ogre::MeshPtr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get'",NULL);
#endif
 {
  Ogre::Mesh* tolua_ret = (Ogre::Mesh*)  self->get();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::Mesh");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get'.",&tolua_err);
 return 0;
#endif
}

/* get function: x of class  Ogre::Vector3 */
static int tolua_get_Ogre__Vector3_x(lua_State* tolua_S)
{
  Ogre::Vector3* self = (Ogre::Vector3*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->x);
 return 1;
}

/* set function: x of class  Ogre::Vector3 */
static int tolua_set_Ogre__Vector3_x(lua_State* tolua_S)
{
  Ogre::Vector3* self = (Ogre::Vector3*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->x = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}

/* get function: y of class  Ogre::Vector3 */
static int tolua_get_Ogre__Vector3_y(lua_State* tolua_S)
{
  Ogre::Vector3* self = (Ogre::Vector3*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->y);
 return 1;
}

/* set function: y of class  Ogre::Vector3 */
static int tolua_set_Ogre__Vector3_y(lua_State* tolua_S)
{
  Ogre::Vector3* self = (Ogre::Vector3*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->y = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}

/* get function: z of class  Ogre::Vector3 */
static int tolua_get_Ogre__Vector3_z(lua_State* tolua_S)
{
  Ogre::Vector3* self = (Ogre::Vector3*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'z'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->z);
 return 1;
}

/* set function: z of class  Ogre::Vector3 */
static int tolua_set_Ogre__Vector3_z(lua_State* tolua_S)
{
  Ogre::Vector3* self = (Ogre::Vector3*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'z'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->z = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}

/* method: new of class  Ogre::Vector3 */
static int tolua_Ogre_Ogre_Vector3_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"Ogre::Vector3",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
 {
  Ogre::Vector3* tolua_ret = (Ogre::Vector3*)  new Ogre::Vector3();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::Vector3");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}

/* method: new_local of class  Ogre::Vector3 */
static int tolua_Ogre_Ogre_Vector3_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"Ogre::Vector3",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
 {
  Ogre::Vector3* tolua_ret = (Ogre::Vector3*)  new Ogre::Vector3();
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"Ogre::Vector3");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}

/* method: new of class  Ogre::Vector3 */
static int tolua_Ogre_Ogre_Vector3_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"Ogre::Vector3",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  float fX = ((float)  tolua_tonumber(tolua_S,2,0));
  float fY = ((float)  tolua_tonumber(tolua_S,3,0));
  float fZ = ((float)  tolua_tonumber(tolua_S,4,0));
 {
  Ogre::Vector3* tolua_ret = (Ogre::Vector3*)  new Ogre::Vector3(fX,fY,fZ);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::Vector3");
 }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_Vector3_new00(tolua_S);
}

/* method: new_local of class  Ogre::Vector3 */
static int tolua_Ogre_Ogre_Vector3_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"Ogre::Vector3",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  float fX = ((float)  tolua_tonumber(tolua_S,2,0));
  float fY = ((float)  tolua_tonumber(tolua_S,3,0));
  float fZ = ((float)  tolua_tonumber(tolua_S,4,0));
 {
  Ogre::Vector3* tolua_ret = (Ogre::Vector3*)  new Ogre::Vector3(fX,fY,fZ);
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"Ogre::Vector3");
 }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_Vector3_new00_local(tolua_S);
}

/* method: new of class  Ogre::Vector3 */
static int tolua_Ogre_Ogre_Vector3_new02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"Ogre::Vector3",0,&tolua_err) ||
 !tolua_istable(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  float afCoordinate[3];
 {
#ifndef TOLUA_RELEASE
 if (!tolua_isnumberarray(tolua_S,2,3,0,&tolua_err))
 goto tolua_lerror;
 else
#endif
 {
 int i;
 for(i=0; i<3;i++)
  afCoordinate[i] = ((float)  tolua_tofieldnumber(tolua_S,2,i+1,0));
 }
 }
 {
  Ogre::Vector3* tolua_ret = (Ogre::Vector3*)  new Ogre::Vector3(afCoordinate);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::Vector3");
 }
 {
 int i;
 for(i=0; i<3;i++)
 tolua_pushfieldnumber(tolua_S,2,i+1,(lua_Number) afCoordinate[i]);
 }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_Vector3_new01(tolua_S);
}

/* method: new_local of class  Ogre::Vector3 */
static int tolua_Ogre_Ogre_Vector3_new02_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"Ogre::Vector3",0,&tolua_err) ||
 !tolua_istable(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  float afCoordinate[3];
 {
#ifndef TOLUA_RELEASE
 if (!tolua_isnumberarray(tolua_S,2,3,0,&tolua_err))
 goto tolua_lerror;
 else
#endif
 {
 int i;
 for(i=0; i<3;i++)
  afCoordinate[i] = ((float)  tolua_tofieldnumber(tolua_S,2,i+1,0));
 }
 }
 {
  Ogre::Vector3* tolua_ret = (Ogre::Vector3*)  new Ogre::Vector3(afCoordinate);
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"Ogre::Vector3");
 }
 {
 int i;
 for(i=0; i<3;i++)
 tolua_pushfieldnumber(tolua_S,2,i+1,(lua_Number) afCoordinate[i]);
 }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_Vector3_new01_local(tolua_S);
}

/* method: new of class  Ogre::Vector3 */
static int tolua_Ogre_Ogre_Vector3_new03(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"Ogre::Vector3",0,&tolua_err) ||
 !tolua_istable(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  int afCoordinate[3];
 {
#ifndef TOLUA_RELEASE
 if (!tolua_isnumberarray(tolua_S,2,3,0,&tolua_err))
 goto tolua_lerror;
 else
#endif
 {
 int i;
 for(i=0; i<3;i++)
  afCoordinate[i] = ((int)  tolua_tofieldnumber(tolua_S,2,i+1,0));
 }
 }
 {
  Ogre::Vector3* tolua_ret = (Ogre::Vector3*)  new Ogre::Vector3(afCoordinate);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::Vector3");
 }
 {
 int i;
 for(i=0; i<3;i++)
 tolua_pushfieldnumber(tolua_S,2,i+1,(lua_Number) afCoordinate[i]);
 }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_Vector3_new02(tolua_S);
}

/* method: new_local of class  Ogre::Vector3 */
static int tolua_Ogre_Ogre_Vector3_new03_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"Ogre::Vector3",0,&tolua_err) ||
 !tolua_istable(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  int afCoordinate[3];
 {
#ifndef TOLUA_RELEASE
 if (!tolua_isnumberarray(tolua_S,2,3,0,&tolua_err))
 goto tolua_lerror;
 else
#endif
 {
 int i;
 for(i=0; i<3;i++)
  afCoordinate[i] = ((int)  tolua_tofieldnumber(tolua_S,2,i+1,0));
 }
 }
 {
  Ogre::Vector3* tolua_ret = (Ogre::Vector3*)  new Ogre::Vector3(afCoordinate);
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"Ogre::Vector3");
 }
 {
 int i;
 for(i=0; i<3;i++)
 tolua_pushfieldnumber(tolua_S,2,i+1,(lua_Number) afCoordinate[i]);
 }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_Vector3_new02_local(tolua_S);
}

/* method: new of class  Ogre::Vector3 */
static int tolua_Ogre_Ogre_Vector3_new04(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"Ogre::Vector3",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  const Ogre::Vector3* rkVector = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
 {
  Ogre::Vector3* tolua_ret = (Ogre::Vector3*)  new Ogre::Vector3(*rkVector);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::Vector3");
 }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_Vector3_new03(tolua_S);
}

/* method: new_local of class  Ogre::Vector3 */
static int tolua_Ogre_Ogre_Vector3_new04_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"Ogre::Vector3",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  const Ogre::Vector3* rkVector = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
 {
  Ogre::Vector3* tolua_ret = (Ogre::Vector3*)  new Ogre::Vector3(*rkVector);
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"Ogre::Vector3");
 }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_Vector3_new03_local(tolua_S);
}

/* method: operator[] of class  Ogre::Vector3 */
static int tolua_Ogre_Ogre_Vector3__geti00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::Vector3",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::Vector3* self = (const Ogre::Vector3*)  tolua_tousertype(tolua_S,1,0);
  long i = ((long)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator[]'",NULL);
#endif
 {
  float tolua_ret = (float)  self->operator[](i);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.geti'.",&tolua_err);
 return 0;
#endif
}

/* method: operator&[] of class  Ogre::Vector3 */
static int tolua_Ogre_Ogre_Vector3__seti00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Vector3",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Vector3* self = (Ogre::Vector3*)  tolua_tousertype(tolua_S,1,0);
  long i = ((long)  tolua_tonumber(tolua_S,2,0));
  float tolua_value = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator&[]'",NULL);
#endif
 self->operator[](i) =  tolua_value;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.seti'.",&tolua_err);
 return 0;
#endif
}

/* method: operator[] of class  Ogre::Vector3 */
static int tolua_Ogre_Ogre_Vector3__geti01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Vector3",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  Ogre::Vector3* self = (Ogre::Vector3*)  tolua_tousertype(tolua_S,1,0);
  long i = ((long)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator[]'",NULL);
#endif
 {
  float tolua_ret = (float)  self->operator[](i);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_Vector3__geti00(tolua_S);
}

/* method: operator== of class  Ogre::Vector3 */
static int tolua_Ogre_Ogre_Vector3__eq00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Vector3",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Vector3* self = (Ogre::Vector3*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector3* rkVector = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator=='",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->operator==(*rkVector);
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

/* method: operator+ of class  Ogre::Vector3 */
static int tolua_Ogre_Ogre_Vector3__add00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Vector3",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Vector3* self = (Ogre::Vector3*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector3* rkVector = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator+'",NULL);
#endif
 {
  Ogre::Vector3 tolua_ret = (Ogre::Vector3)  self->operator+(*rkVector);
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
 tolua_error(tolua_S,"#ferror in function '.add'.",&tolua_err);
 return 0;
#endif
}

/* method: operator- of class  Ogre::Vector3 */
static int tolua_Ogre_Ogre_Vector3__sub00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Vector3",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Vector3* self = (Ogre::Vector3*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector3* rkVector = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator-'",NULL);
#endif
 {
  Ogre::Vector3 tolua_ret = (Ogre::Vector3)  self->operator-(*rkVector);
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
 tolua_error(tolua_S,"#ferror in function '.sub'.",&tolua_err);
 return 0;
#endif
}

/* method: operator* of class  Ogre::Vector3 */
static int tolua_Ogre_Ogre_Vector3__mul00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Vector3",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Vector3* self = (Ogre::Vector3*)  tolua_tousertype(tolua_S,1,0);
  float fScalar = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator*'",NULL);
#endif
 {
  Ogre::Vector3 tolua_ret = (Ogre::Vector3)  self->operator*(fScalar);
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
 tolua_error(tolua_S,"#ferror in function '.mul'.",&tolua_err);
 return 0;
#endif
}

/* method: operator* of class  Ogre::Vector3 */
static int tolua_Ogre_Ogre_Vector3__mul01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Vector3",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  Ogre::Vector3* self = (Ogre::Vector3*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector3* rhs = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator*'",NULL);
#endif
 {
  Ogre::Vector3 tolua_ret = (Ogre::Vector3)  self->operator*(*rhs);
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
 return tolua_Ogre_Ogre_Vector3__mul00(tolua_S);
}

/* method: operator/ of class  Ogre::Vector3 */
static int tolua_Ogre_Ogre_Vector3__div00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Vector3",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Vector3* self = (Ogre::Vector3*)  tolua_tousertype(tolua_S,1,0);
  float fScalar = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator/'",NULL);
#endif
 {
  Ogre::Vector3 tolua_ret = (Ogre::Vector3)  self->operator/(fScalar);
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
 tolua_error(tolua_S,"#ferror in function '.div'.",&tolua_err);
 return 0;
#endif
}

/* method: operator/ of class  Ogre::Vector3 */
static int tolua_Ogre_Ogre_Vector3__div01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Vector3",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  Ogre::Vector3* self = (Ogre::Vector3*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector3* rhs = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator/'",NULL);
#endif
 {
  Ogre::Vector3 tolua_ret = (Ogre::Vector3)  self->operator/(*rhs);
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
 return tolua_Ogre_Ogre_Vector3__div00(tolua_S);
}

/* method: operator- of class  Ogre::Vector3 */
static int tolua_Ogre_Ogre_Vector3__sub01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Vector3",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  Ogre::Vector3* self = (Ogre::Vector3*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator-'",NULL);
#endif
 {
  Ogre::Vector3 tolua_ret = (Ogre::Vector3)  self->operator-();
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
 return tolua_Ogre_Ogre_Vector3__sub00(tolua_S);
}

/* method: length of class  Ogre::Vector3 */
static int tolua_Ogre_Ogre_Vector3_length00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Vector3",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Vector3* self = (Ogre::Vector3*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'length'",NULL);
#endif
 {
  float tolua_ret = (float)  self->length();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'length'.",&tolua_err);
 return 0;
#endif
}

/* method: squaredLength of class  Ogre::Vector3 */
static int tolua_Ogre_Ogre_Vector3_squaredLength00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Vector3",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Vector3* self = (Ogre::Vector3*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'squaredLength'",NULL);
#endif
 {
  float tolua_ret = (float)  self->squaredLength();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'squaredLength'.",&tolua_err);
 return 0;
#endif
}

/* method: dotProduct of class  Ogre::Vector3 */
static int tolua_Ogre_Ogre_Vector3_dotProduct00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Vector3",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Vector3* self = (Ogre::Vector3*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector3* vec = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'dotProduct'",NULL);
#endif
 {
  float tolua_ret = (float)  self->dotProduct(*vec);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'dotProduct'.",&tolua_err);
 return 0;
#endif
}

/* method: normalise of class  Ogre::Vector3 */
static int tolua_Ogre_Ogre_Vector3_normalise00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Vector3",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Vector3* self = (Ogre::Vector3*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'normalise'",NULL);
#endif
 {
  float tolua_ret = (float)  self->normalise();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'normalise'.",&tolua_err);
 return 0;
#endif
}

/* method: crossProduct of class  Ogre::Vector3 */
static int tolua_Ogre_Ogre_Vector3_crossProduct00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Vector3",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Vector3* self = (Ogre::Vector3*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector3* rkVector = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'crossProduct'",NULL);
#endif
 {
  Ogre::Vector3 tolua_ret = (Ogre::Vector3)  self->crossProduct(*rkVector);
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
 tolua_error(tolua_S,"#ferror in function 'crossProduct'.",&tolua_err);
 return 0;
#endif
}

/* method: midPoint of class  Ogre::Vector3 */
static int tolua_Ogre_Ogre_Vector3_midPoint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Vector3",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Vector3* self = (Ogre::Vector3*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector3* vec = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'midPoint'",NULL);
#endif
 {
  Ogre::Vector3 tolua_ret = (Ogre::Vector3)  self->midPoint(*vec);
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
 tolua_error(tolua_S,"#ferror in function 'midPoint'.",&tolua_err);
 return 0;
#endif
}

/* method: operator< of class  Ogre::Vector3 */
static int tolua_Ogre_Ogre_Vector3__lt00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Vector3",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Vector3* self = (Ogre::Vector3*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector3* rhs = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator<'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->operator<(*rhs);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.lt'.",&tolua_err);
 return 0;
#endif
}

/* method: makeFloor of class  Ogre::Vector3 */
static int tolua_Ogre_Ogre_Vector3_makeFloor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Vector3",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Vector3* self = (Ogre::Vector3*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector3* cmp = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'makeFloor'",NULL);
#endif
 {
  self->makeFloor(*cmp);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'makeFloor'.",&tolua_err);
 return 0;
#endif
}

/* method: makeCeil of class  Ogre::Vector3 */
static int tolua_Ogre_Ogre_Vector3_makeCeil00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Vector3",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Vector3* self = (Ogre::Vector3*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector3* cmp = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'makeCeil'",NULL);
#endif
 {
  self->makeCeil(*cmp);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'makeCeil'.",&tolua_err);
 return 0;
#endif
}

/* method: perpendicular of class  Ogre::Vector3 */
static int tolua_Ogre_Ogre_Vector3_perpendicular00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Vector3",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Vector3* self = (Ogre::Vector3*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'perpendicular'",NULL);
#endif
 {
  Ogre::Vector3 tolua_ret = (Ogre::Vector3)  self->perpendicular();
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
 tolua_error(tolua_S,"#ferror in function 'perpendicular'.",&tolua_err);
 return 0;
#endif
}

/* method: randomDeviant of class  Ogre::Vector3 */
static int tolua_Ogre_Ogre_Vector3_randomDeviant00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Vector3",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const Ogre::Radian",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,3,"const Ogre::Vector3",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Vector3* self = (Ogre::Vector3*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Radian* angle = ((const Ogre::Radian*)  tolua_tousertype(tolua_S,2,0));
  const Ogre::Vector3* up = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'randomDeviant'",NULL);
#endif
 {
  Ogre::Vector3 tolua_ret = (Ogre::Vector3)  self->randomDeviant(*angle,*up);
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
 tolua_error(tolua_S,"#ferror in function 'randomDeviant'.",&tolua_err);
 return 0;
#endif
}

/* method: getRotationTo of class  Ogre::Vector3 */
static int tolua_Ogre_Ogre_Vector3_getRotationTo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Vector3",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Vector3* self = (Ogre::Vector3*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector3* dest = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRotationTo'",NULL);
#endif
 {
  Ogre::Quaternion tolua_ret = (Ogre::Quaternion)  self->getRotationTo(*dest);
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
 tolua_error(tolua_S,"#ferror in function 'getRotationTo'.",&tolua_err);
 return 0;
#endif
}

/* method: isZeroLength of class  Ogre::Vector3 */
static int tolua_Ogre_Ogre_Vector3_isZeroLength00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Vector3",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Vector3* self = (Ogre::Vector3*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isZeroLength'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isZeroLength();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isZeroLength'.",&tolua_err);
 return 0;
#endif
}

/* method: normalisedCopy of class  Ogre::Vector3 */
static int tolua_Ogre_Ogre_Vector3_normalisedCopy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Vector3",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Vector3* self = (Ogre::Vector3*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'normalisedCopy'",NULL);
#endif
 {
  Ogre::Vector3 tolua_ret = (Ogre::Vector3)  self->normalisedCopy();
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
 tolua_error(tolua_S,"#ferror in function 'normalisedCopy'.",&tolua_err);
 return 0;
#endif
}

/* method: reflect of class  Ogre::Vector3 */
static int tolua_Ogre_Ogre_Vector3_reflect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Vector3",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Vector3* self = (Ogre::Vector3*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector3* normal = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'reflect'",NULL);
#endif
 {
  Ogre::Vector3 tolua_ret = (Ogre::Vector3)  self->reflect(*normal);
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
 tolua_error(tolua_S,"#ferror in function 'reflect'.",&tolua_err);
 return 0;
#endif
}

/* method: positionEquals of class  Ogre::Vector3 */
static int tolua_Ogre_Ogre_Vector3_positionEquals00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Vector3",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Vector3* self = (Ogre::Vector3*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector3* rhs = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
  float tolerance = ((float)  tolua_tonumber(tolua_S,3,1e-03));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'positionEquals'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->positionEquals(*rhs,tolerance);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'positionEquals'.",&tolua_err);
 return 0;
#endif
}

/* method: directionEquals of class  Ogre::Vector3 */
static int tolua_Ogre_Ogre_Vector3_directionEquals00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Vector3",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,3,"const Ogre::Radian",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Vector3* self = (Ogre::Vector3*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector3* rhs = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
  const Ogre::Radian* tolerance = ((const Ogre::Radian*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'directionEquals'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->directionEquals(*rhs,*tolerance);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'directionEquals'.",&tolua_err);
 return 0;
#endif
}

/* get function: ZERO of class  Ogre::Vector3 */
static int tolua_get_Ogre__Vector3_ZERO(lua_State* tolua_S)
{
 tolua_pushusertype(tolua_S,(void*)&Ogre::Vector3::ZERO,"const Ogre::Vector3");
 return 1;
}

/* get function: UNIT_X of class  Ogre::Vector3 */
static int tolua_get_Ogre__Vector3_UNIT_X(lua_State* tolua_S)
{
 tolua_pushusertype(tolua_S,(void*)&Ogre::Vector3::UNIT_X,"const Ogre::Vector3");
 return 1;
}

/* get function: UNIT_Y of class  Ogre::Vector3 */
static int tolua_get_Ogre__Vector3_UNIT_Y(lua_State* tolua_S)
{
 tolua_pushusertype(tolua_S,(void*)&Ogre::Vector3::UNIT_Y,"const Ogre::Vector3");
 return 1;
}

/* get function: UNIT_Z of class  Ogre::Vector3 */
static int tolua_get_Ogre__Vector3_UNIT_Z(lua_State* tolua_S)
{
 tolua_pushusertype(tolua_S,(void*)&Ogre::Vector3::UNIT_Z,"const Ogre::Vector3");
 return 1;
}

/* get function: NEGATIVE_UNIT_X of class  Ogre::Vector3 */
static int tolua_get_Ogre__Vector3_NEGATIVE_UNIT_X(lua_State* tolua_S)
{
 tolua_pushusertype(tolua_S,(void*)&Ogre::Vector3::NEGATIVE_UNIT_X,"const Ogre::Vector3");
 return 1;
}

/* get function: NEGATIVE_UNIT_Y of class  Ogre::Vector3 */
static int tolua_get_Ogre__Vector3_NEGATIVE_UNIT_Y(lua_State* tolua_S)
{
 tolua_pushusertype(tolua_S,(void*)&Ogre::Vector3::NEGATIVE_UNIT_Y,"const Ogre::Vector3");
 return 1;
}

/* get function: NEGATIVE_UNIT_Z of class  Ogre::Vector3 */
static int tolua_get_Ogre__Vector3_NEGATIVE_UNIT_Z(lua_State* tolua_S)
{
 tolua_pushusertype(tolua_S,(void*)&Ogre::Vector3::NEGATIVE_UNIT_Z,"const Ogre::Vector3");
 return 1;
}

/* get function: UNIT_SCALE of class  Ogre::Vector3 */
static int tolua_get_Ogre__Vector3_UNIT_SCALE(lua_State* tolua_S)
{
 tolua_pushusertype(tolua_S,(void*)&Ogre::Vector3::UNIT_SCALE,"const Ogre::Vector3");
 return 1;
}

/* method: getMesh of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity_getMesh00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::Entity",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::Entity* self = (const Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMesh'",NULL);
#endif
 {
  const Ogre::MeshPtr& tolua_ret = (const Ogre::MeshPtr&)  self->getMesh();
 tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Ogre::MeshPtr");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMesh'.",&tolua_err);
 return 0;
#endif
}

/* method: getSubEntity of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity_getSubEntity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::Entity",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::Entity* self = (const Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
  unsigned int index = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSubEntity'",NULL);
#endif
 {
  Ogre::SubEntity* tolua_ret = (Ogre::SubEntity*)  self->getSubEntity(index);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::SubEntity");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSubEntity'.",&tolua_err);
 return 0;
#endif
}

/* method: getSubEntity of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity_getSubEntity01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::Entity",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  const Ogre::Entity* self = (const Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSubEntity'",NULL);
#endif
 {
  Ogre::SubEntity* tolua_ret = (Ogre::SubEntity*)  self->getSubEntity(name);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::SubEntity");
 tolua_pushcppstring(tolua_S,(const char*)name);
 }
 }
 return 2;
tolua_lerror:
 return tolua_Ogre_Ogre_Entity_getSubEntity00(tolua_S);
}

/* method: getNumSubEntities of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity_getNumSubEntities00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::Entity",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::Entity* self = (const Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getNumSubEntities'",NULL);
#endif
 {
  unsigned int tolua_ret = (unsigned int)  self->getNumSubEntities();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getNumSubEntities'.",&tolua_err);
 return 0;
#endif
}

/* method: clone of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity_clone00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::Entity",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::Entity* self = (const Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
  const std::string newName = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clone'",NULL);
#endif
 {
  Ogre::Entity* tolua_ret = (Ogre::Entity*)  self->clone(newName);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::Entity");
 tolua_pushcppstring(tolua_S,(const char*)newName);
 }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clone'.",&tolua_err);
 return 0;
#endif
}

/* method: setMaterialName of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity_setMaterialName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Entity",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Entity* self = (Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMaterialName'",NULL);
#endif
 {
  self->setMaterialName(name);
 tolua_pushcppstring(tolua_S,(const char*)name);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setMaterialName'.",&tolua_err);
 return 0;
#endif
}

/* method: setRenderQueueGroup of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity_setRenderQueueGroup00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Entity",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Entity* self = (Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
  char queueID = ((char)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRenderQueueGroup'",NULL);
#endif
 {
  self->setRenderQueueGroup(queueID);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setRenderQueueGroup'.",&tolua_err);
 return 0;
#endif
}

/* method: getBoundingBox of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity_getBoundingBox00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::Entity",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::Entity* self = (const Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getBoundingBox'",NULL);
#endif
 {
  const Ogre::AxisAlignedBox& tolua_ret = (const Ogre::AxisAlignedBox&)  self->getBoundingBox();
 tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Ogre::AxisAlignedBox");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getBoundingBox'.",&tolua_err);
 return 0;
#endif
}

/* method: getChildObjectsBoundingBox of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity_getChildObjectsBoundingBox00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::Entity",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::Entity* self = (const Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getChildObjectsBoundingBox'",NULL);
#endif
 {
  Ogre::AxisAlignedBox tolua_ret = (Ogre::AxisAlignedBox)  self->getChildObjectsBoundingBox();
 {
#ifdef __cplusplus
 void* tolua_obj = new Ogre::AxisAlignedBox(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::AxisAlignedBox");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::AxisAlignedBox));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::AxisAlignedBox");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getChildObjectsBoundingBox'.",&tolua_err);
 return 0;
#endif
}

/* method: getMovableType of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity_getMovableType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::Entity",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::Entity* self = (const Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMovableType'",NULL);
#endif
 {
  const std::string tolua_ret = (const std::string)  self->getMovableType();
 tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMovableType'.",&tolua_err);
 return 0;
#endif
}

/* method: getAnimationState of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity_getAnimationState00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::Entity",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::Entity* self = (const Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAnimationState'",NULL);
#endif
 {
  Ogre::AnimationState* tolua_ret = (Ogre::AnimationState*)  self->getAnimationState(name);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::AnimationState");
 tolua_pushcppstring(tolua_S,(const char*)name);
 }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAnimationState'.",&tolua_err);
 return 0;
#endif
}

/* method: getAllAnimationStates of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity_getAllAnimationStates00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::Entity",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::Entity* self = (const Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAllAnimationStates'",NULL);
#endif
 {
  Ogre::AnimationStateSet* tolua_ret = (Ogre::AnimationStateSet*)  self->getAllAnimationStates();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::AnimationStateSet");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAllAnimationStates'.",&tolua_err);
 return 0;
#endif
}

/* method: setDisplaySkeleton of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity_setDisplaySkeleton00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Entity",0,&tolua_err) ||
 !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Entity* self = (Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
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

/* method: getDisplaySkeleton of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity_getDisplaySkeleton00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::Entity",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::Entity* self = (const Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
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

/* method: getManualLodLevel of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity_getManualLodLevel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::Entity",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::Entity* self = (const Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getManualLodLevel'",NULL);
#endif
 {
  Ogre::Entity* tolua_ret = (Ogre::Entity*)  self->getManualLodLevel(index);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::Entity");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getManualLodLevel'.",&tolua_err);
 return 0;
#endif
}

/* method: getNumManualLodLevels of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity_getNumManualLodLevels00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::Entity",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::Entity* self = (const Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getNumManualLodLevels'",NULL);
#endif
 {
  int tolua_ret = (int)  self->getNumManualLodLevels();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getNumManualLodLevels'.",&tolua_err);
 return 0;
#endif
}

/* method: setMeshLodBias of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity_setMeshLodBias00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Entity",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Entity* self = (Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
  float factor = ((float)  tolua_tonumber(tolua_S,2,0));
  unsigned short maxDetailIndex = ((unsigned short)  tolua_tonumber(tolua_S,3,0));
  unsigned short minDetailIndex = ((unsigned short)  tolua_tonumber(tolua_S,4,99));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMeshLodBias'",NULL);
#endif
 {
  self->setMeshLodBias(factor,maxDetailIndex,minDetailIndex);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setMeshLodBias'.",&tolua_err);
 return 0;
#endif
}

/* method: setMaterialLodBias of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity_setMaterialLodBias00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Entity",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
 !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Entity* self = (Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
  float factor = ((float)  tolua_tonumber(tolua_S,2,0));
  unsigned short maxDetailIndex = ((unsigned short)  tolua_tonumber(tolua_S,3,0));
  unsigned short minDetailIndex = ((unsigned short)  tolua_tonumber(tolua_S,4,99));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMaterialLodBias'",NULL);
#endif
 {
  self->setMaterialLodBias(factor,maxDetailIndex,minDetailIndex);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setMaterialLodBias'.",&tolua_err);
 return 0;
#endif
}

/* method: setPolygonModeOverrideable of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity_setPolygonModeOverrideable00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Entity",0,&tolua_err) ||
 !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Entity* self = (Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
  bool PolygonModeOverrideable = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPolygonModeOverrideable'",NULL);
#endif
 {
  self->setPolygonModeOverrideable(PolygonModeOverrideable);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setPolygonModeOverrideable'.",&tolua_err);
 return 0;
#endif
}

/* method: attachObjectToBone of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity_attachObjectToBone00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Entity",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isusertype(tolua_S,3,"Ogre::MovableObject",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Entity* self = (Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
  const std::string boneName = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  Ogre::MovableObject* pMovable = ((Ogre::MovableObject*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'attachObjectToBone'",NULL);
#endif
 {
  Ogre::TagPoint* tolua_ret = (Ogre::TagPoint*)  self->attachObjectToBone(boneName,pMovable);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::TagPoint");
 tolua_pushcppstring(tolua_S,(const char*)boneName);
 }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'attachObjectToBone'.",&tolua_err);
 return 0;
#endif
}

/* method: attachObjectToBone of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity_attachObjectToBone01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Entity",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isusertype(tolua_S,3,"Ogre::MovableObject",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,4,"const Ogre::Quaternion",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  Ogre::Entity* self = (Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
  const std::string boneName = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  Ogre::MovableObject* pMovable = ((Ogre::MovableObject*)  tolua_tousertype(tolua_S,3,0));
  const Ogre::Quaternion* offsetOrientation = ((const Ogre::Quaternion*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'attachObjectToBone'",NULL);
#endif
 {
  Ogre::TagPoint* tolua_ret = (Ogre::TagPoint*)  self->attachObjectToBone(boneName,pMovable,*offsetOrientation);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::TagPoint");
 tolua_pushcppstring(tolua_S,(const char*)boneName);
 }
 }
 return 2;
tolua_lerror:
 return tolua_Ogre_Ogre_Entity_attachObjectToBone00(tolua_S);
}

/* method: attachObjectToBone of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity_attachObjectToBone02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Entity",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isusertype(tolua_S,3,"Ogre::MovableObject",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,4,"const Ogre::Quaternion",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,5,"const Ogre::Vector3",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  Ogre::Entity* self = (Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
  const std::string boneName = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  Ogre::MovableObject* pMovable = ((Ogre::MovableObject*)  tolua_tousertype(tolua_S,3,0));
  const Ogre::Quaternion* offsetOrientation = ((const Ogre::Quaternion*)  tolua_tousertype(tolua_S,4,0));
  const Ogre::Vector3* offsetPosition = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,5,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'attachObjectToBone'",NULL);
#endif
 {
  Ogre::TagPoint* tolua_ret = (Ogre::TagPoint*)  self->attachObjectToBone(boneName,pMovable,*offsetOrientation,*offsetPosition);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::TagPoint");
 tolua_pushcppstring(tolua_S,(const char*)boneName);
 }
 }
 return 2;
tolua_lerror:
 return tolua_Ogre_Ogre_Entity_attachObjectToBone01(tolua_S);
}

/* method: detachObjectFromBone of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity_detachObjectFromBone00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Entity",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Entity* self = (Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
  const std::string movableName = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'detachObjectFromBone'",NULL);
#endif
 {
  Ogre::MovableObject* tolua_ret = (Ogre::MovableObject*)  self->detachObjectFromBone(movableName);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::MovableObject");
 tolua_pushcppstring(tolua_S,(const char*)movableName);
 }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'detachObjectFromBone'.",&tolua_err);
 return 0;
#endif
}

/* method: detachObjectFromBone of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity_detachObjectFromBone01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Entity",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"Ogre::MovableObject",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  Ogre::Entity* self = (Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
  Ogre::MovableObject* obj = ((Ogre::MovableObject*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'detachObjectFromBone'",NULL);
#endif
 {
  self->detachObjectFromBone(obj);
 }
 }
 return 0;
tolua_lerror:
 return tolua_Ogre_Ogre_Entity_detachObjectFromBone00(tolua_S);
}

/* method: detachAllObjectsFromBone of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity_detachAllObjectsFromBone00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Entity",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Entity* self = (Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'detachAllObjectsFromBone'",NULL);
#endif
 {
  self->detachAllObjectsFromBone();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'detachAllObjectsFromBone'.",&tolua_err);
 return 0;
#endif
}

/* method: getAttachedObjectIterator of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity_getAttachedObjectIterator00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Entity",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Entity* self = (Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAttachedObjectIterator'",NULL);
#endif
 {
   Ogre::MapIterator<Ogre::Entity::ChildObjectList> tolua_ret = (  Ogre::MapIterator<Ogre::Entity::ChildObjectList>)  self->getAttachedObjectIterator();
 {
#ifdef __cplusplus
 void* tolua_obj = new Ogre::MapIterator<Ogre::Entity::ChildObjectList>(tolua_ret);
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::MapIterator<Ogre::Entity::ChildObjectList>");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::MapIterator<Ogre::Entity::ChildObjectList>));
 tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::MapIterator<Ogre::Entity::ChildObjectList>");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAttachedObjectIterator'.",&tolua_err);
 return 0;
#endif
}

/* method: getBoundingRadius of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity_getBoundingRadius00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::Entity",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::Entity* self = (const Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getBoundingRadius'",NULL);
#endif
 {
  float tolua_ret = (float)  self->getBoundingRadius();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getBoundingRadius'.",&tolua_err);
 return 0;
#endif
}

/* method: getWorldBoundingBox of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity_getWorldBoundingBox00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::Entity",0,&tolua_err) ||
 !tolua_isboolean(tolua_S,2,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::Entity* self = (const Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
  bool derive = ((bool)  tolua_toboolean(tolua_S,2,false));
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

/* method: getWorldBoundingSphere of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity_getWorldBoundingSphere00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::Entity",0,&tolua_err) ||
 !tolua_isboolean(tolua_S,2,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::Entity* self = (const Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
  bool derive = ((bool)  tolua_toboolean(tolua_S,2,false));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getWorldBoundingSphere'",NULL);
#endif
 {
  const Ogre::Sphere& tolua_ret = (const Ogre::Sphere&)  self->getWorldBoundingSphere(derive);
 tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Ogre::Sphere");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getWorldBoundingSphere'.",&tolua_err);
 return 0;
#endif
}

/* method: setNormaliseNormals of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity_setNormaliseNormals00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Entity",0,&tolua_err) ||
 !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Entity* self = (Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
  bool normalise = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setNormaliseNormals'",NULL);
#endif
 {
  self->setNormaliseNormals(normalise);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setNormaliseNormals'.",&tolua_err);
 return 0;
#endif
}

/* method: getNormaliseNormals of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity_getNormaliseNormals00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::Entity",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::Entity* self = (const Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getNormaliseNormals'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->getNormaliseNormals();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getNormaliseNormals'.",&tolua_err);
 return 0;
#endif
}

/* method: hasSkeleton of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity_hasSkeleton00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::Entity",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::Entity* self = (const Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hasSkeleton'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->hasSkeleton();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'hasSkeleton'.",&tolua_err);
 return 0;
#endif
}

/* method: getSkeleton of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity_getSkeleton00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::Entity",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::Entity* self = (const Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSkeleton'",NULL);
#endif
 {
  Ogre::SkeletonInstance* tolua_ret = (Ogre::SkeletonInstance*)  self->getSkeleton();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::SkeletonInstance");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSkeleton'.",&tolua_err);
 return 0;
#endif
}

/* method: isHardwareAnimationEnabled of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity_isHardwareAnimationEnabled00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::Entity",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::Entity* self = (const Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isHardwareAnimationEnabled'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->isHardwareAnimationEnabled();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isHardwareAnimationEnabled'.",&tolua_err);
 return 0;
#endif
}

/* method: getSoftwareAnimationRequests of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity_getSoftwareAnimationRequests00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::Entity",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::Entity* self = (const Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSoftwareAnimationRequests'",NULL);
#endif
 {
  int tolua_ret = (int)  self->getSoftwareAnimationRequests();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSoftwareAnimationRequests'.",&tolua_err);
 return 0;
#endif
}

/* method: getSoftwareAnimationNormalsRequests of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity_getSoftwareAnimationNormalsRequests00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::Entity",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::Entity* self = (const Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSoftwareAnimationNormalsRequests'",NULL);
#endif
 {
  int tolua_ret = (int)  self->getSoftwareAnimationNormalsRequests();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSoftwareAnimationNormalsRequests'.",&tolua_err);
 return 0;
#endif
}

/* method: addSoftwareAnimationRequest of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity_addSoftwareAnimationRequest00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Entity",0,&tolua_err) ||
 !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Entity* self = (Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
  bool normalsAlso = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addSoftwareAnimationRequest'",NULL);
#endif
 {
  self->addSoftwareAnimationRequest(normalsAlso);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addSoftwareAnimationRequest'.",&tolua_err);
 return 0;
#endif
}

/* method: removeSoftwareAnimationRequest of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity_removeSoftwareAnimationRequest00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Entity",0,&tolua_err) ||
 !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Entity* self = (Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
  bool normalsAlso = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeSoftwareAnimationRequest'",NULL);
#endif
 {
  self->removeSoftwareAnimationRequest(normalsAlso);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeSoftwareAnimationRequest'.",&tolua_err);
 return 0;
#endif
}

/* method: shareSkeletonInstanceWith of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity_shareSkeletonInstanceWith00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Entity",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"Ogre::Entity",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Entity* self = (Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
  Ogre::Entity* entity = ((Ogre::Entity*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'shareSkeletonInstanceWith'",NULL);
#endif
 {
  self->shareSkeletonInstanceWith(entity);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'shareSkeletonInstanceWith'.",&tolua_err);
 return 0;
#endif
}

/* method: hasVertexAnimation of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity_hasVertexAnimation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::Entity",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::Entity* self = (const Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hasVertexAnimation'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->hasVertexAnimation();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'hasVertexAnimation'.",&tolua_err);
 return 0;
#endif
}

/* method: stopSharingSkeletonInstance of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity_stopSharingSkeletonInstance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Entity",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Entity* self = (Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'stopSharingSkeletonInstance'",NULL);
#endif
 {
  self->stopSharingSkeletonInstance();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'stopSharingSkeletonInstance'.",&tolua_err);
 return 0;
#endif
}

/* method: sharesSkeletonInstance of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity_sharesSkeletonInstance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::Entity",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::Entity* self = (const Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'sharesSkeletonInstance'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->sharesSkeletonInstance();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sharesSkeletonInstance'.",&tolua_err);
 return 0;
#endif
}

/* method: getSkeletonInstanceSharingSet of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity_getSkeletonInstanceSharingSet00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::Entity",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::Entity* self = (const Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSkeletonInstanceSharingSet'",NULL);
#endif
 {
   const std::set<Ogre::Entity*>* tolua_ret = (  const std::set<Ogre::Entity*>*)  self->getSkeletonInstanceSharingSet();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"const std::set<Ogre::Entity*>");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSkeletonInstanceSharingSet'.",&tolua_err);
 return 0;
#endif
}

/* method: refreshAvailableAnimationState of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity_refreshAvailableAnimationState00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Entity",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Entity* self = (Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'refreshAvailableAnimationState'",NULL);
#endif
 {
  self->refreshAvailableAnimationState();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'refreshAvailableAnimationState'.",&tolua_err);
 return 0;
#endif
}

/* method: _updateAnimation of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity__updateAnimation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Entity",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Entity* self = (Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function '_updateAnimation'",NULL);
#endif
 {
  self->_updateAnimation();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '_updateAnimation'.",&tolua_err);
 return 0;
#endif
}

/* method: _isAnimated of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity__isAnimated00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::Entity",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::Entity* self = (const Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function '_isAnimated'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->_isAnimated();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '_isAnimated'.",&tolua_err);
 return 0;
#endif
}

/* method: _isSkeletonAnimated of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity__isSkeletonAnimated00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::Entity",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::Entity* self = (const Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function '_isSkeletonAnimated'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->_isSkeletonAnimated();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '_isSkeletonAnimated'.",&tolua_err);
 return 0;
#endif
}

/* method: getTypeFlags of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity_getTypeFlags00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::Entity",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::Entity* self = (const Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTypeFlags'",NULL);
#endif
 {
  long tolua_ret = (long)  self->getTypeFlags();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTypeFlags'.",&tolua_err);
 return 0;
#endif
}

/* method: chooseVertexDataForBinding of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity_chooseVertexDataForBinding00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::Entity",0,&tolua_err) ||
 !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::Entity* self = (const Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
  bool hasVertexAnim = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'chooseVertexDataForBinding'",NULL);
#endif
 {
  Ogre::Entity::VertexDataBindChoice tolua_ret = (Ogre::Entity::VertexDataBindChoice)  self->chooseVertexDataForBinding(hasVertexAnim);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'chooseVertexDataForBinding'.",&tolua_err);
 return 0;
#endif
}

/* method: _getBuffersMarkedForAnimation of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity__getBuffersMarkedForAnimation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Ogre::Entity",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Ogre::Entity* self = (const Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function '_getBuffersMarkedForAnimation'",NULL);
#endif
 {
  bool tolua_ret = (bool)  self->_getBuffersMarkedForAnimation();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '_getBuffersMarkedForAnimation'.",&tolua_err);
 return 0;
#endif
}

/* method: _markBuffersUsedForAnimation of class  Ogre::Entity */
static int tolua_Ogre_Ogre_Entity__markBuffersUsedForAnimation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Ogre::Entity",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Ogre::Entity* self = (Ogre::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function '_markBuffersUsedForAnimation'",NULL);
#endif
 {
  self->_markBuffersUsedForAnimation();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '_markBuffersUsedForAnimation'.",&tolua_err);
 return 0;
#endif
}

/* Open function */
TOLUA_API int tolua_Ogre_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
 tolua_module(tolua_S,"Ogre",0);
 tolua_beginmodule(tolua_S,"Ogre");
 tolua_cclass(tolua_S,"RenderTarget","Ogre::RenderTarget","",NULL);
 tolua_beginmodule(tolua_S,"RenderTarget");
 tolua_constant(tolua_S,"SF_NONE",Ogre::RenderTarget::SF_NONE);
 tolua_constant(tolua_S,"SF_FPS",Ogre::RenderTarget::SF_FPS);
 tolua_constant(tolua_S,"SF_AVG_FPS",Ogre::RenderTarget::SF_AVG_FPS);
 tolua_constant(tolua_S,"SF_BEST_FPS",Ogre::RenderTarget::SF_BEST_FPS);
 tolua_constant(tolua_S,"SF_WORST_FPS",Ogre::RenderTarget::SF_WORST_FPS);
 tolua_constant(tolua_S,"SF_TRIANGLE_COUNT",Ogre::RenderTarget::SF_TRIANGLE_COUNT);
 tolua_constant(tolua_S,"SF_ALL",Ogre::RenderTarget::SF_ALL);
 tolua_cclass(tolua_S,"FrameStats","Ogre::RenderTarget::FrameStats","",NULL);
 tolua_beginmodule(tolua_S,"FrameStats");
 tolua_variable(tolua_S,"lastFPS",tolua_get_Ogre__RenderTarget__FrameStats_lastFPS,tolua_set_Ogre__RenderTarget__FrameStats_lastFPS);
 tolua_variable(tolua_S,"avgFPS",tolua_get_Ogre__RenderTarget__FrameStats_avgFPS,tolua_set_Ogre__RenderTarget__FrameStats_avgFPS);
 tolua_variable(tolua_S,"bestFPS",tolua_get_Ogre__RenderTarget__FrameStats_bestFPS,tolua_set_Ogre__RenderTarget__FrameStats_bestFPS);
 tolua_variable(tolua_S,"worstFPS",tolua_get_Ogre__RenderTarget__FrameStats_worstFPS,tolua_set_Ogre__RenderTarget__FrameStats_worstFPS);
 tolua_variable(tolua_S,"bestFrameTime",tolua_get_Ogre__RenderTarget__FrameStats_unsigned_bestFrameTime,tolua_set_Ogre__RenderTarget__FrameStats_unsigned_bestFrameTime);
 tolua_variable(tolua_S,"worstFrameTime",tolua_get_Ogre__RenderTarget__FrameStats_unsigned_worstFrameTime,tolua_set_Ogre__RenderTarget__FrameStats_unsigned_worstFrameTime);
 tolua_variable(tolua_S,"triangleCount",tolua_get_Ogre__RenderTarget__FrameStats_unsigned_triangleCount,tolua_set_Ogre__RenderTarget__FrameStats_unsigned_triangleCount);
 tolua_endmodule(tolua_S);
 tolua_function(tolua_S,"getStatistics",tolua_Ogre_Ogre_RenderTarget_getStatistics00);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 tolua_module(tolua_S,"Ogre",0);
 tolua_beginmodule(tolua_S,"Ogre");
 tolua_cclass(tolua_S,"RenderWindow","Ogre::RenderWindow","Ogre::RenderTarget",NULL);
 tolua_beginmodule(tolua_S,"RenderWindow");
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 tolua_module(tolua_S,"Ogre",0);
 tolua_beginmodule(tolua_S,"Ogre");
#ifdef __cplusplus
 tolua_cclass(tolua_S,"MapIterator_Ogre__ResourceManager__ResourceHandleMap_","Ogre::MapIterator<Ogre::ResourceManager::ResourceHandleMap>","",tolua_collect_Ogre__MapIterator_Ogre__ResourceManager__ResourceHandleMap_);
#else
 tolua_cclass(tolua_S,"MapIterator_Ogre__ResourceManager__ResourceHandleMap_","Ogre::MapIterator<Ogre::ResourceManager::ResourceHandleMap>","",NULL);
#endif
 tolua_beginmodule(tolua_S,"MapIterator_Ogre__ResourceManager__ResourceHandleMap_");
 tolua_function(tolua_S,"hasMoreElements",tolua_Ogre_Ogre_MapIterator_Ogre__ResourceManager__ResourceHandleMap__hasMoreElements00);
 tolua_function(tolua_S,"getNext",tolua_Ogre_Ogre_MapIterator_Ogre__ResourceManager__ResourceHandleMap__getNext00);
 tolua_function(tolua_S,"peekNextValue",tolua_Ogre_Ogre_MapIterator_Ogre__ResourceManager__ResourceHandleMap__peekNextValue00);
 tolua_function(tolua_S,"peekNextKey",tolua_Ogre_Ogre_MapIterator_Ogre__ResourceManager__ResourceHandleMap__peekNextKey00);
 tolua_function(tolua_S,"moveNext",tolua_Ogre_Ogre_MapIterator_Ogre__ResourceManager__ResourceHandleMap__moveNext00);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 tolua_module(tolua_S,"Ogre",0);
 tolua_beginmodule(tolua_S,"Ogre");
 tolua_cclass(tolua_S,"Resource","Ogre::Resource","",NULL);
 tolua_beginmodule(tolua_S,"Resource");
 tolua_function(tolua_S,"load",tolua_Ogre_Ogre_Resource_load00);
 tolua_function(tolua_S,"reload",tolua_Ogre_Ogre_Resource_reload00);
 tolua_function(tolua_S,"isManuallyLoaded",tolua_Ogre_Ogre_Resource_isManuallyLoaded00);
 tolua_function(tolua_S,"unload",tolua_Ogre_Ogre_Resource_unload00);
 tolua_function(tolua_S,"getSize",tolua_Ogre_Ogre_Resource_getSize00);
 tolua_function(tolua_S,"touch",tolua_Ogre_Ogre_Resource_touch00);
 tolua_function(tolua_S,"getName",tolua_Ogre_Ogre_Resource_getName00);
 tolua_function(tolua_S,"isLoaded",tolua_Ogre_Ogre_Resource_isLoaded00);
 tolua_function(tolua_S,"getGroup",tolua_Ogre_Ogre_Resource_getGroup00);
 tolua_function(tolua_S,"getOrigin",tolua_Ogre_Ogre_Resource_getOrigin00);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 tolua_module(tolua_S,"Ogre",0);
 tolua_beginmodule(tolua_S,"Ogre");
 tolua_cclass(tolua_S,"ResourceManager","Ogre::ResourceManager","",NULL);
 tolua_beginmodule(tolua_S,"ResourceManager");
 tolua_function(tolua_S,"create",tolua_Ogre_Ogre_ResourceManager_create00);
 tolua_function(tolua_S,"reloadAll",tolua_Ogre_Ogre_ResourceManager_reloadAll00);
 tolua_function(tolua_S,"remove",tolua_Ogre_Ogre_ResourceManager_remove00);
 tolua_function(tolua_S,"remove",tolua_Ogre_Ogre_ResourceManager_remove01);
 tolua_function(tolua_S,"getByName",tolua_Ogre_Ogre_ResourceManager_getByName00);
 tolua_function(tolua_S,"resourceExists",tolua_Ogre_Ogre_ResourceManager_resourceExists00);
 tolua_function(tolua_S,"getResourceIterator",tolua_Ogre_Ogre_ResourceManager_getResourceIterator00);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 tolua_module(tolua_S,"Ogre",0);
 tolua_beginmodule(tolua_S,"Ogre");
 tolua_cclass(tolua_S,"MaterialManager","Ogre::MaterialManager","Ogre::ResourceManager",NULL);
 tolua_beginmodule(tolua_S,"MaterialManager");
 tolua_function(tolua_S,"getSingleton",tolua_Ogre_Ogre_MaterialManager_getSingleton00);
 tolua_function(tolua_S,"load",tolua_Ogre_Ogre_MaterialManager_load00);
 tolua_function(tolua_S,"getByName",tolua_Ogre_Ogre_MaterialManager_getByName00);
 tolua_function(tolua_S,"create",tolua_Ogre_Ogre_MaterialManager_create00);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 tolua_module(tolua_S,"Ogre",0);
 tolua_beginmodule(tolua_S,"Ogre");
 tolua_cclass(tolua_S,"Material","Ogre::Material","Ogre::Resource",NULL);
 tolua_beginmodule(tolua_S,"Material");
 tolua_function(tolua_S,"isTransparent",tolua_Ogre_Ogre_Material_isTransparent00);
 tolua_function(tolua_S,"setReceiveShadows",tolua_Ogre_Ogre_Material_setReceiveShadows00);
 tolua_function(tolua_S,"getReceiveShadows",tolua_Ogre_Ogre_Material_getReceiveShadows00);
 tolua_function(tolua_S,"setTransparencyCastsShadows",tolua_Ogre_Ogre_Material_setTransparencyCastsShadows00);
 tolua_function(tolua_S,"getTransparencyCastsShadows",tolua_Ogre_Ogre_Material_getTransparencyCastsShadows00);
 tolua_function(tolua_S,"getNumTechniques",tolua_Ogre_Ogre_Material_getNumTechniques00);
 tolua_function(tolua_S,"touch",tolua_Ogre_Ogre_Material_touch00);
 tolua_endmodule(tolua_S);
#ifdef __cplusplus
 tolua_cclass(tolua_S,"MaterialPtr","Ogre::MaterialPtr","",tolua_collect_Ogre__MaterialPtr);
#else
 tolua_cclass(tolua_S,"MaterialPtr","Ogre::MaterialPtr","",NULL);
#endif
 tolua_beginmodule(tolua_S,"MaterialPtr");
 tolua_function(tolua_S,"get",tolua_Ogre_Ogre_MaterialPtr_get00);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 tolua_module(tolua_S,"Ogre",0);
 tolua_beginmodule(tolua_S,"Ogre");
 tolua_cclass(tolua_S,"SubEntity","Ogre::SubEntity","",NULL);
 tolua_beginmodule(tolua_S,"SubEntity");
 tolua_function(tolua_S,"getMaterialName",tolua_Ogre_Ogre_SubEntity_getMaterialName00);
 tolua_function(tolua_S,"setMaterialName",tolua_Ogre_Ogre_SubEntity_setMaterialName00);
 tolua_function(tolua_S,"setVisible",tolua_Ogre_Ogre_SubEntity_setVisible00);
 tolua_function(tolua_S,"isVisible",tolua_Ogre_Ogre_SubEntity_isVisible00);
 tolua_function(tolua_S,"getMaterial",tolua_Ogre_Ogre_SubEntity_getMaterial00);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 tolua_module(tolua_S,"Ogre",0);
 tolua_beginmodule(tolua_S,"Ogre");
 tolua_cclass(tolua_S,"MeshManager","Ogre::MeshManager","Ogre::ResourceManager",NULL);
 tolua_beginmodule(tolua_S,"MeshManager");
 tolua_function(tolua_S,"getSingleton",tolua_Ogre_Ogre_MeshManager_getSingleton00);
 tolua_function(tolua_S,"load",tolua_Ogre_Ogre_MeshManager_load00);
 tolua_function(tolua_S,"getByName",tolua_Ogre_Ogre_MeshManager_getByName00);
 tolua_function(tolua_S,"create",tolua_Ogre_Ogre_MeshManager_create00);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 tolua_module(tolua_S,"Ogre",0);
 tolua_beginmodule(tolua_S,"Ogre");
 tolua_cclass(tolua_S,"Mesh","Ogre::Mesh","Ogre::Resource",NULL);
 tolua_beginmodule(tolua_S,"Mesh");
 tolua_function(tolua_S,"getNumSubMeshes",tolua_Ogre_Ogre_Mesh_getNumSubMeshes00);
 tolua_function(tolua_S,"hasSkeleton",tolua_Ogre_Ogre_Mesh_hasSkeleton00);
 tolua_function(tolua_S,"getSkeletonName",tolua_Ogre_Ogre_Mesh_getSkeletonName00);
 tolua_function(tolua_S,"getNumLodLevels",tolua_Ogre_Ogre_Mesh_getNumLodLevels00);
 tolua_endmodule(tolua_S);
#ifdef __cplusplus
 tolua_cclass(tolua_S,"MeshPtr","Ogre::MeshPtr","",tolua_collect_Ogre__MeshPtr);
#else
 tolua_cclass(tolua_S,"MeshPtr","Ogre::MeshPtr","",NULL);
#endif
 tolua_beginmodule(tolua_S,"MeshPtr");
 tolua_function(tolua_S,"get",tolua_Ogre_Ogre_MeshPtr_get00);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 tolua_module(tolua_S,"Ogre",0);
 tolua_beginmodule(tolua_S,"Ogre");
#ifdef __cplusplus
 tolua_cclass(tolua_S,"Vector3","Ogre::Vector3","",tolua_collect_Ogre__Vector3);
#else
 tolua_cclass(tolua_S,"Vector3","Ogre::Vector3","",NULL);
#endif
 tolua_beginmodule(tolua_S,"Vector3");
 tolua_variable(tolua_S,"x",tolua_get_Ogre__Vector3_x,tolua_set_Ogre__Vector3_x);
 tolua_variable(tolua_S,"y",tolua_get_Ogre__Vector3_y,tolua_set_Ogre__Vector3_y);
 tolua_variable(tolua_S,"z",tolua_get_Ogre__Vector3_z,tolua_set_Ogre__Vector3_z);
 tolua_function(tolua_S,"new",tolua_Ogre_Ogre_Vector3_new00);
 tolua_function(tolua_S,"new_local",tolua_Ogre_Ogre_Vector3_new00_local);
 tolua_function(tolua_S,".call",tolua_Ogre_Ogre_Vector3_new00_local);
 tolua_function(tolua_S,"new",tolua_Ogre_Ogre_Vector3_new01);
 tolua_function(tolua_S,"new_local",tolua_Ogre_Ogre_Vector3_new01_local);
 tolua_function(tolua_S,".call",tolua_Ogre_Ogre_Vector3_new01_local);
 tolua_function(tolua_S,"new",tolua_Ogre_Ogre_Vector3_new02);
 tolua_function(tolua_S,"new_local",tolua_Ogre_Ogre_Vector3_new02_local);
 tolua_function(tolua_S,".call",tolua_Ogre_Ogre_Vector3_new02_local);
 tolua_function(tolua_S,"new",tolua_Ogre_Ogre_Vector3_new03);
 tolua_function(tolua_S,"new_local",tolua_Ogre_Ogre_Vector3_new03_local);
 tolua_function(tolua_S,".call",tolua_Ogre_Ogre_Vector3_new03_local);
 tolua_function(tolua_S,"new",tolua_Ogre_Ogre_Vector3_new04);
 tolua_function(tolua_S,"new_local",tolua_Ogre_Ogre_Vector3_new04_local);
 tolua_function(tolua_S,".call",tolua_Ogre_Ogre_Vector3_new04_local);
 tolua_function(tolua_S,".geti",tolua_Ogre_Ogre_Vector3__geti00);
 tolua_function(tolua_S,".seti",tolua_Ogre_Ogre_Vector3__seti00);
 tolua_function(tolua_S,".geti",tolua_Ogre_Ogre_Vector3__geti01);
 tolua_function(tolua_S,".eq",tolua_Ogre_Ogre_Vector3__eq00);
 tolua_function(tolua_S,".add",tolua_Ogre_Ogre_Vector3__add00);
 tolua_function(tolua_S,".sub",tolua_Ogre_Ogre_Vector3__sub00);
 tolua_function(tolua_S,".mul",tolua_Ogre_Ogre_Vector3__mul00);
 tolua_function(tolua_S,".mul",tolua_Ogre_Ogre_Vector3__mul01);
 tolua_function(tolua_S,".div",tolua_Ogre_Ogre_Vector3__div00);
 tolua_function(tolua_S,".div",tolua_Ogre_Ogre_Vector3__div01);
 tolua_function(tolua_S,".sub",tolua_Ogre_Ogre_Vector3__sub01);
 tolua_function(tolua_S,"length",tolua_Ogre_Ogre_Vector3_length00);
 tolua_function(tolua_S,"squaredLength",tolua_Ogre_Ogre_Vector3_squaredLength00);
 tolua_function(tolua_S,"dotProduct",tolua_Ogre_Ogre_Vector3_dotProduct00);
 tolua_function(tolua_S,"normalise",tolua_Ogre_Ogre_Vector3_normalise00);
 tolua_function(tolua_S,"crossProduct",tolua_Ogre_Ogre_Vector3_crossProduct00);
 tolua_function(tolua_S,"midPoint",tolua_Ogre_Ogre_Vector3_midPoint00);
 tolua_function(tolua_S,".lt",tolua_Ogre_Ogre_Vector3__lt00);
 tolua_function(tolua_S,"makeFloor",tolua_Ogre_Ogre_Vector3_makeFloor00);
 tolua_function(tolua_S,"makeCeil",tolua_Ogre_Ogre_Vector3_makeCeil00);
 tolua_function(tolua_S,"perpendicular",tolua_Ogre_Ogre_Vector3_perpendicular00);
 tolua_function(tolua_S,"randomDeviant",tolua_Ogre_Ogre_Vector3_randomDeviant00);
 tolua_function(tolua_S,"getRotationTo",tolua_Ogre_Ogre_Vector3_getRotationTo00);
 tolua_function(tolua_S,"isZeroLength",tolua_Ogre_Ogre_Vector3_isZeroLength00);
 tolua_function(tolua_S,"normalisedCopy",tolua_Ogre_Ogre_Vector3_normalisedCopy00);
 tolua_function(tolua_S,"reflect",tolua_Ogre_Ogre_Vector3_reflect00);
 tolua_function(tolua_S,"positionEquals",tolua_Ogre_Ogre_Vector3_positionEquals00);
 tolua_function(tolua_S,"directionEquals",tolua_Ogre_Ogre_Vector3_directionEquals00);
 tolua_variable(tolua_S,"ZERO",tolua_get_Ogre__Vector3_ZERO,NULL);
 tolua_variable(tolua_S,"UNIT_X",tolua_get_Ogre__Vector3_UNIT_X,NULL);
 tolua_variable(tolua_S,"UNIT_Y",tolua_get_Ogre__Vector3_UNIT_Y,NULL);
 tolua_variable(tolua_S,"UNIT_Z",tolua_get_Ogre__Vector3_UNIT_Z,NULL);
 tolua_variable(tolua_S,"NEGATIVE_UNIT_X",tolua_get_Ogre__Vector3_NEGATIVE_UNIT_X,NULL);
 tolua_variable(tolua_S,"NEGATIVE_UNIT_Y",tolua_get_Ogre__Vector3_NEGATIVE_UNIT_Y,NULL);
 tolua_variable(tolua_S,"NEGATIVE_UNIT_Z",tolua_get_Ogre__Vector3_NEGATIVE_UNIT_Z,NULL);
 tolua_variable(tolua_S,"UNIT_SCALE",tolua_get_Ogre__Vector3_UNIT_SCALE,NULL);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 tolua_module(tolua_S,"Ogre",0);
 tolua_beginmodule(tolua_S,"Ogre");
 tolua_cclass(tolua_S,"Entity","Ogre::Entity","Ogre::MovableObject",NULL);
 tolua_beginmodule(tolua_S,"Entity");
 tolua_function(tolua_S,"getMesh",tolua_Ogre_Ogre_Entity_getMesh00);
 tolua_function(tolua_S,"getSubEntity",tolua_Ogre_Ogre_Entity_getSubEntity00);
 tolua_function(tolua_S,"getSubEntity",tolua_Ogre_Ogre_Entity_getSubEntity01);
 tolua_function(tolua_S,"getNumSubEntities",tolua_Ogre_Ogre_Entity_getNumSubEntities00);
 tolua_function(tolua_S,"clone",tolua_Ogre_Ogre_Entity_clone00);
 tolua_function(tolua_S,"setMaterialName",tolua_Ogre_Ogre_Entity_setMaterialName00);
 tolua_function(tolua_S,"setRenderQueueGroup",tolua_Ogre_Ogre_Entity_setRenderQueueGroup00);
 tolua_function(tolua_S,"getBoundingBox",tolua_Ogre_Ogre_Entity_getBoundingBox00);
 tolua_function(tolua_S,"getChildObjectsBoundingBox",tolua_Ogre_Ogre_Entity_getChildObjectsBoundingBox00);
 tolua_function(tolua_S,"getMovableType",tolua_Ogre_Ogre_Entity_getMovableType00);
 tolua_function(tolua_S,"getAnimationState",tolua_Ogre_Ogre_Entity_getAnimationState00);
 tolua_function(tolua_S,"getAllAnimationStates",tolua_Ogre_Ogre_Entity_getAllAnimationStates00);
 tolua_function(tolua_S,"setDisplaySkeleton",tolua_Ogre_Ogre_Entity_setDisplaySkeleton00);
 tolua_function(tolua_S,"getDisplaySkeleton",tolua_Ogre_Ogre_Entity_getDisplaySkeleton00);
 tolua_function(tolua_S,"getManualLodLevel",tolua_Ogre_Ogre_Entity_getManualLodLevel00);
 tolua_function(tolua_S,"getNumManualLodLevels",tolua_Ogre_Ogre_Entity_getNumManualLodLevels00);
 tolua_function(tolua_S,"setMeshLodBias",tolua_Ogre_Ogre_Entity_setMeshLodBias00);
 tolua_function(tolua_S,"setMaterialLodBias",tolua_Ogre_Ogre_Entity_setMaterialLodBias00);
 tolua_function(tolua_S,"setPolygonModeOverrideable",tolua_Ogre_Ogre_Entity_setPolygonModeOverrideable00);
 tolua_function(tolua_S,"attachObjectToBone",tolua_Ogre_Ogre_Entity_attachObjectToBone00);
 tolua_function(tolua_S,"attachObjectToBone",tolua_Ogre_Ogre_Entity_attachObjectToBone01);
 tolua_function(tolua_S,"attachObjectToBone",tolua_Ogre_Ogre_Entity_attachObjectToBone02);
 tolua_function(tolua_S,"detachObjectFromBone",tolua_Ogre_Ogre_Entity_detachObjectFromBone00);
 tolua_function(tolua_S,"detachObjectFromBone",tolua_Ogre_Ogre_Entity_detachObjectFromBone01);
 tolua_function(tolua_S,"detachAllObjectsFromBone",tolua_Ogre_Ogre_Entity_detachAllObjectsFromBone00);
 tolua_function(tolua_S,"getAttachedObjectIterator",tolua_Ogre_Ogre_Entity_getAttachedObjectIterator00);
 tolua_function(tolua_S,"getBoundingRadius",tolua_Ogre_Ogre_Entity_getBoundingRadius00);
 tolua_function(tolua_S,"getWorldBoundingBox",tolua_Ogre_Ogre_Entity_getWorldBoundingBox00);
 tolua_function(tolua_S,"getWorldBoundingSphere",tolua_Ogre_Ogre_Entity_getWorldBoundingSphere00);
 tolua_function(tolua_S,"setNormaliseNormals",tolua_Ogre_Ogre_Entity_setNormaliseNormals00);
 tolua_function(tolua_S,"getNormaliseNormals",tolua_Ogre_Ogre_Entity_getNormaliseNormals00);
 tolua_function(tolua_S,"hasSkeleton",tolua_Ogre_Ogre_Entity_hasSkeleton00);
 tolua_function(tolua_S,"getSkeleton",tolua_Ogre_Ogre_Entity_getSkeleton00);
 tolua_function(tolua_S,"isHardwareAnimationEnabled",tolua_Ogre_Ogre_Entity_isHardwareAnimationEnabled00);
 tolua_function(tolua_S,"getSoftwareAnimationRequests",tolua_Ogre_Ogre_Entity_getSoftwareAnimationRequests00);
 tolua_function(tolua_S,"getSoftwareAnimationNormalsRequests",tolua_Ogre_Ogre_Entity_getSoftwareAnimationNormalsRequests00);
 tolua_function(tolua_S,"addSoftwareAnimationRequest",tolua_Ogre_Ogre_Entity_addSoftwareAnimationRequest00);
 tolua_function(tolua_S,"removeSoftwareAnimationRequest",tolua_Ogre_Ogre_Entity_removeSoftwareAnimationRequest00);
 tolua_function(tolua_S,"shareSkeletonInstanceWith",tolua_Ogre_Ogre_Entity_shareSkeletonInstanceWith00);
 tolua_function(tolua_S,"hasVertexAnimation",tolua_Ogre_Ogre_Entity_hasVertexAnimation00);
 tolua_function(tolua_S,"stopSharingSkeletonInstance",tolua_Ogre_Ogre_Entity_stopSharingSkeletonInstance00);
 tolua_function(tolua_S,"sharesSkeletonInstance",tolua_Ogre_Ogre_Entity_sharesSkeletonInstance00);
 tolua_function(tolua_S,"getSkeletonInstanceSharingSet",tolua_Ogre_Ogre_Entity_getSkeletonInstanceSharingSet00);
 tolua_function(tolua_S,"refreshAvailableAnimationState",tolua_Ogre_Ogre_Entity_refreshAvailableAnimationState00);
 tolua_function(tolua_S,"_updateAnimation",tolua_Ogre_Ogre_Entity__updateAnimation00);
 tolua_function(tolua_S,"_isAnimated",tolua_Ogre_Ogre_Entity__isAnimated00);
 tolua_function(tolua_S,"_isSkeletonAnimated",tolua_Ogre_Ogre_Entity__isSkeletonAnimated00);
 tolua_function(tolua_S,"getTypeFlags",tolua_Ogre_Ogre_Entity_getTypeFlags00);
 tolua_constant(tolua_S,"BIND_ORIGINAL",Ogre::Entity::BIND_ORIGINAL);
 tolua_constant(tolua_S,"BIND_SOFTWARE_SKELETAL",Ogre::Entity::BIND_SOFTWARE_SKELETAL);
 tolua_constant(tolua_S,"BIND_SOFTWARE_MORPH",Ogre::Entity::BIND_SOFTWARE_MORPH);
 tolua_constant(tolua_S,"BIND_HARDWARE_MORPH",Ogre::Entity::BIND_HARDWARE_MORPH);
 tolua_function(tolua_S,"chooseVertexDataForBinding",tolua_Ogre_Ogre_Entity_chooseVertexDataForBinding00);
 tolua_function(tolua_S,"_getBuffersMarkedForAnimation",tolua_Ogre_Ogre_Entity__getBuffersMarkedForAnimation00);
 tolua_function(tolua_S,"_markBuffersUsedForAnimation",tolua_Ogre_Ogre_Entity__markBuffersUsedForAnimation00);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}
