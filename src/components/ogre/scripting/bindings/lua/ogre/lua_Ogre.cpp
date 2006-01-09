/*
** Lua binding: Ogre
** Generated automatically by tolua++-1.0.6 on Sun Jan  8 20:23:32 2006.
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

static int tolua_collect_Ogre__ResourceManager__ResourceHandleMap__key_type (lua_State* tolua_S)
{
 Ogre::ResourceManager::ResourceHandleMap::key_type* self = (Ogre::ResourceManager::ResourceHandleMap::key_type*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_uint (lua_State* tolua_S)
{
 uint* self = (uint*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_Ogre__MapIterator_Ogre__ResourceManager__ResourceHandleMap_ (lua_State* tolua_S)
{
 Ogre::MapIterator<Ogre::ResourceManager::ResourceHandleMap>* self = (Ogre::MapIterator<Ogre::ResourceManager::ResourceHandleMap>*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_Ogre__ResourceManager__ResourceHandleMap__mapped_type (lua_State* tolua_S)
{
 Ogre::ResourceManager::ResourceHandleMap::mapped_type* self = (Ogre::ResourceManager::ResourceHandleMap::mapped_type*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_Ogre__ResourcePtr (lua_State* tolua_S)
{
 Ogre::ResourcePtr* self = (Ogre::ResourcePtr*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"Ogre::RenderTarget::FrameStats");
 tolua_usertype(tolua_S,"Ogre::Mesh");
 tolua_usertype(tolua_S,"Ogre::Resource");
 tolua_usertype(tolua_S,"Ogre::RenderWindow");
 tolua_usertype(tolua_S,"Ogre::ResourceManager::ResourceHandleMap::key_type");
 tolua_usertype(tolua_S,"Ogre::ResourceManager");
 tolua_usertype(tolua_S,"Ogre::MaterialPtr");
 tolua_usertype(tolua_S,"ushort");
 tolua_usertype(tolua_S,"Ogre::MeshPtr");
 tolua_usertype(tolua_S,"Ogre::ResourcePtr");
 tolua_usertype(tolua_S,"Ogre::ResourceManager::ResourceHandleMap::mapped_type");
 tolua_usertype(tolua_S,"Ogre::Material");
 tolua_usertype(tolua_S,"Ogre::MaterialManager");
 tolua_usertype(tolua_S,"Ogre::MeshManager");
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
  Ogre::MaterialManager& tolua_ret = (Ogre::MaterialManager&)  Ogre::MeshManager::getSingleton();
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
 tolua_cclass(tolua_S,"MaterialPtr","Ogre::MaterialPtr","",NULL);
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
 tolua_cclass(tolua_S,"MeshPtr","Ogre::MeshPtr","",NULL);
 tolua_beginmodule(tolua_S,"MeshPtr");
 tolua_function(tolua_S,"get",tolua_Ogre_Ogre_MeshPtr_get00);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}
