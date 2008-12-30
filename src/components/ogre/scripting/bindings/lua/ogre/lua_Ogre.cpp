/*
** Lua binding: Ogre
** Generated automatically by tolua++-1.0.92.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_Ogre_open (lua_State* tolua_S);

#include "required.h"
#include <OgreRoot.h>

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_Ogre__ResourceManager__ResourceMapIterator (lua_State* tolua_S)
{
 Ogre::ResourceManager::ResourceMapIterator* self = (Ogre::ResourceManager::ResourceMapIterator*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_Ogre__Math (lua_State* tolua_S)
{
 Ogre::Math* self = (Ogre::Math*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_Ogre__MapIterator_Ogre__Entity__ChildObjectList_ (lua_State* tolua_S)
{
 Ogre::MapIterator<Ogre::Entity::ChildObjectList>* self = (Ogre::MapIterator<Ogre::Entity::ChildObjectList>*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_Ogre__ResourceHandle (lua_State* tolua_S)
{
 Ogre::ResourceHandle* self = (Ogre::ResourceHandle*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_Ogre__Radian (lua_State* tolua_S)
{
 Ogre::Radian* self = (Ogre::Radian*) tolua_tousertype(tolua_S,1,0);
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

static int tolua_collect_ushort (lua_State* tolua_S)
{
 ushort* self = (ushort*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_Ogre__Degree (lua_State* tolua_S)
{
 Ogre::Degree* self = (Ogre::Degree*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_Ogre__ResourcePtr (lua_State* tolua_S)
{
 Ogre::ResourcePtr* self = (Ogre::ResourcePtr*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_std__pair_bool_float_ (lua_State* tolua_S)
{
 std::pair<bool,float>* self = (std::pair<bool,float>*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_Ogre__MaterialSerializer (lua_State* tolua_S)
{
 Ogre::MaterialSerializer* self = (Ogre::MaterialSerializer*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_size_t (lua_State* tolua_S)
{
 size_t* self = (size_t*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_Ogre__Vector3 (lua_State* tolua_S)
{
 Ogre::Vector3* self = (Ogre::Vector3*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_Ogre__ColourValue (lua_State* tolua_S)
{
 Ogre::ColourValue* self = (Ogre::ColourValue*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_Ogre__Matrix4 (lua_State* tolua_S)
{
 Ogre::Matrix4* self = (Ogre::Matrix4*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_uint (lua_State* tolua_S)
{
 uint* self = (uint*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_Ogre__MeshPtr (lua_State* tolua_S)
{
 Ogre::MeshPtr* self = (Ogre::MeshPtr*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_Ogre__Vector4 (lua_State* tolua_S)
{
 Ogre::Vector4* self = (Ogre::Vector4*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_Ogre__Matrix3 (lua_State* tolua_S)
{
 Ogre::Matrix3* self = (Ogre::Matrix3*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"Ogre::ResourceManager::ResourceMapIterator");
 tolua_usertype(tolua_S,"Ogre::RenderTarget");
 tolua_usertype(tolua_S,"Ogre::Radian");
 tolua_usertype(tolua_S,"Ogre::RenderWindow");
 tolua_usertype(tolua_S,"Ogre::Vector4");
 tolua_usertype(tolua_S,"Ogre::SceneManager");
 tolua_usertype(tolua_S,"Ogre::DataStreamPtr");
 tolua_usertype(tolua_S,"Ogre::Degree");
 tolua_usertype(tolua_S,"Ogre::MaterialSerializer");
 tolua_usertype(tolua_S,"size_t");
 tolua_usertype(tolua_S,"Ogre::GpuProgram");
 tolua_usertype(tolua_S,"Ogre::Node");
 tolua_usertype(tolua_S,"Ogre::Matrix3");
 tolua_usertype(tolua_S,"Ogre::RenderTarget::FrameStats");
 tolua_usertype(tolua_S,"Ogre::Mesh");
 tolua_usertype(tolua_S,"Ogre::AnimationState");
 tolua_usertype(tolua_S,"Ogre::SkeletonInstance");
 tolua_usertype(tolua_S,"Ogre::MapIterator<Ogre::Entity::ChildObjectList>");
 tolua_usertype(tolua_S,"Ogre::Resource");
 tolua_usertype(tolua_S,"std::set<Ogre::Entity*>");
 tolua_usertype(tolua_S,"Ogre::HighLevelGpuProgramPtr");
 tolua_usertype(tolua_S,"Ogre::HighLevelGpuProgramManager");
 tolua_usertype(tolua_S,"Ogre::Timer");
 tolua_usertype(tolua_S,"std::pair<bool,float>");
 tolua_usertype(tolua_S,"Ogre::TagPoint");
 tolua_usertype(tolua_S,"Renderable");
 tolua_usertype(tolua_S,"uint");
 tolua_usertype(tolua_S,"Ogre::MovableObject");
 tolua_usertype(tolua_S,"Ogre::Ray");
 tolua_usertype(tolua_S,"Ogre::Camera");
 tolua_usertype(tolua_S,"Ogre::Vector2");
 tolua_usertype(tolua_S,"Ogre::GpuProgramManager");
 tolua_usertype(tolua_S,"Ogre::ResourceHandle");
 tolua_usertype(tolua_S,"Ogre::AxisAlignedBox");
 tolua_usertype(tolua_S,"Ogre::Image");
 tolua_usertype(tolua_S,"Ogre::MeshPtr");
 tolua_usertype(tolua_S,"Ogre::Material");
 tolua_usertype(tolua_S,"Ogre::MaterialManager");
 tolua_usertype(tolua_S,"Ogre::MeshManager");
 tolua_usertype(tolua_S,"Ogre::VisibleObjectsBoundsInfo");
 tolua_usertype(tolua_S,"Ogre::Plane");
 tolua_usertype(tolua_S,"Ogre::FrameListener");
 tolua_usertype(tolua_S,"std::list<Ogre::Plane>");
 tolua_usertype(tolua_S,"Ogre::Root");
 tolua_usertype(tolua_S,"Ogre::SceneNode");
 tolua_usertype(tolua_S,"Ogre::Quaternion");
 tolua_usertype(tolua_S,"Ogre::ResourceManager");
 tolua_usertype(tolua_S,"Ogre::TexturePtr");
 tolua_usertype(tolua_S,"Ogre::MaterialPtr");
 tolua_usertype(tolua_S,"ushort");
 tolua_usertype(tolua_S,"Ogre::Matrix4");
 tolua_usertype(tolua_S,"Ogre::GpuProgramPtr");
 tolua_usertype(tolua_S,"Ogre::ResourcePtr");
 tolua_usertype(tolua_S,"std::vector<Ogre::Plane>");
 tolua_usertype(tolua_S,"Ogre::Texture");
 tolua_usertype(tolua_S,"Ogre::SubMesh");
 tolua_usertype(tolua_S,"Ogre::ColourValue");
 tolua_usertype(tolua_S,"Ogre::Entity");
 tolua_usertype(tolua_S,"Ogre::Vector3");
 tolua_usertype(tolua_S,"Ogre::TextureManager");
 tolua_usertype(tolua_S,"Ogre::AnimationStateSet");
 tolua_usertype(tolua_S,"Ogre::HighLevelGpuProgram");
 tolua_usertype(tolua_S,"Ogre::SubEntity");
 tolua_usertype(tolua_S,"Ogre::Sphere");
 tolua_usertype(tolua_S,"Ogre::Math");
}

/* get function: lastFPS of class  FrameStats */
#ifndef TOLUA_DISABLE_tolua_get_Ogre__RenderTarget__FrameStats_lastFPS
static int tolua_get_Ogre__RenderTarget__FrameStats_lastFPS(lua_State* tolua_S)
{
  Ogre::RenderTarget::FrameStats* self = (Ogre::RenderTarget::FrameStats*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'lastFPS'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->lastFPS);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: lastFPS of class  FrameStats */
#ifndef TOLUA_DISABLE_tolua_set_Ogre__RenderTarget__FrameStats_lastFPS
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
#endif //#ifndef TOLUA_DISABLE

/* get function: avgFPS of class  FrameStats */
#ifndef TOLUA_DISABLE_tolua_get_Ogre__RenderTarget__FrameStats_avgFPS
static int tolua_get_Ogre__RenderTarget__FrameStats_avgFPS(lua_State* tolua_S)
{
  Ogre::RenderTarget::FrameStats* self = (Ogre::RenderTarget::FrameStats*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'avgFPS'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->avgFPS);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: avgFPS of class  FrameStats */
#ifndef TOLUA_DISABLE_tolua_set_Ogre__RenderTarget__FrameStats_avgFPS
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
#endif //#ifndef TOLUA_DISABLE

/* get function: bestFPS of class  FrameStats */
#ifndef TOLUA_DISABLE_tolua_get_Ogre__RenderTarget__FrameStats_bestFPS
static int tolua_get_Ogre__RenderTarget__FrameStats_bestFPS(lua_State* tolua_S)
{
  Ogre::RenderTarget::FrameStats* self = (Ogre::RenderTarget::FrameStats*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'bestFPS'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->bestFPS);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: bestFPS of class  FrameStats */
#ifndef TOLUA_DISABLE_tolua_set_Ogre__RenderTarget__FrameStats_bestFPS
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
#endif //#ifndef TOLUA_DISABLE

/* get function: worstFPS of class  FrameStats */
#ifndef TOLUA_DISABLE_tolua_get_Ogre__RenderTarget__FrameStats_worstFPS
static int tolua_get_Ogre__RenderTarget__FrameStats_worstFPS(lua_State* tolua_S)
{
  Ogre::RenderTarget::FrameStats* self = (Ogre::RenderTarget::FrameStats*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'worstFPS'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->worstFPS);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: worstFPS of class  FrameStats */
#ifndef TOLUA_DISABLE_tolua_set_Ogre__RenderTarget__FrameStats_worstFPS
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
#endif //#ifndef TOLUA_DISABLE

/* get function: bestFrameTime of class  FrameStats */
#ifndef TOLUA_DISABLE_tolua_get_Ogre__RenderTarget__FrameStats_unsigned_bestFrameTime
static int tolua_get_Ogre__RenderTarget__FrameStats_unsigned_bestFrameTime(lua_State* tolua_S)
{
  Ogre::RenderTarget::FrameStats* self = (Ogre::RenderTarget::FrameStats*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'bestFrameTime'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->bestFrameTime);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: bestFrameTime of class  FrameStats */
#ifndef TOLUA_DISABLE_tolua_set_Ogre__RenderTarget__FrameStats_unsigned_bestFrameTime
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
#endif //#ifndef TOLUA_DISABLE

/* get function: worstFrameTime of class  FrameStats */
#ifndef TOLUA_DISABLE_tolua_get_Ogre__RenderTarget__FrameStats_unsigned_worstFrameTime
static int tolua_get_Ogre__RenderTarget__FrameStats_unsigned_worstFrameTime(lua_State* tolua_S)
{
  Ogre::RenderTarget::FrameStats* self = (Ogre::RenderTarget::FrameStats*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'worstFrameTime'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->worstFrameTime);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: worstFrameTime of class  FrameStats */
#ifndef TOLUA_DISABLE_tolua_set_Ogre__RenderTarget__FrameStats_unsigned_worstFrameTime
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
#endif //#ifndef TOLUA_DISABLE

/* get function: triangleCount of class  FrameStats */
#ifndef TOLUA_DISABLE_tolua_get_Ogre__RenderTarget__FrameStats_unsigned_triangleCount
static int tolua_get_Ogre__RenderTarget__FrameStats_unsigned_triangleCount(lua_State* tolua_S)
{
  Ogre::RenderTarget::FrameStats* self = (Ogre::RenderTarget::FrameStats*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'triangleCount'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->triangleCount);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: triangleCount of class  FrameStats */
#ifndef TOLUA_DISABLE_tolua_set_Ogre__RenderTarget__FrameStats_unsigned_triangleCount
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
#endif //#ifndef TOLUA_DISABLE

/* method: getStatistics of class  Ogre::RenderTarget */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_RenderTarget_getStatistics00
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
#endif //#ifndef TOLUA_DISABLE

/* method: load of class  Ogre::Resource */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Resource_load00
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
#endif //#ifndef TOLUA_DISABLE

/* method: reload of class  Ogre::Resource */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Resource_reload00
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
#endif //#ifndef TOLUA_DISABLE

/* method: isManuallyLoaded of class  Ogre::Resource */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Resource_isManuallyLoaded00
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
#endif //#ifndef TOLUA_DISABLE

/* method: unload of class  Ogre::Resource */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Resource_unload00
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
#endif //#ifndef TOLUA_DISABLE

/* method: getSize of class  Ogre::Resource */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Resource_getSize00
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
#endif //#ifndef TOLUA_DISABLE

/* method: touch of class  Ogre::Resource */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Resource_touch00
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
#endif //#ifndef TOLUA_DISABLE

/* method: getName of class  Ogre::Resource */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Resource_getName00
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
#endif //#ifndef TOLUA_DISABLE

/* method: isLoaded of class  Ogre::Resource */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Resource_isLoaded00
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
#endif //#ifndef TOLUA_DISABLE

/* method: getGroup of class  Ogre::Resource */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Resource_getGroup00
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
#endif //#ifndef TOLUA_DISABLE

/* method: getOrigin of class  Ogre::Resource */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Resource_getOrigin00
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
#endif //#ifndef TOLUA_DISABLE

/* method: get of class  Ogre::ResourcePtr */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_ResourcePtr_get00
static int tolua_Ogre_Ogre_ResourcePtr_get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::ResourcePtr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::ResourcePtr* self = (Ogre::ResourcePtr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get'",NULL);
#endif
  {
   Ogre::Resource* tolua_ret = (Ogre::Resource*)  self->get();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::Resource");
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

/* method: isNull of class  Ogre::ResourcePtr */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_ResourcePtr_isNull00
static int tolua_Ogre_Ogre_ResourcePtr_isNull00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::ResourcePtr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::ResourcePtr* self = (Ogre::ResourcePtr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isNull'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isNull();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isNull'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: create of class  Ogre::ResourceManager */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_ResourceManager_create00
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
#endif //#ifndef TOLUA_DISABLE

/* method: reloadAll of class  Ogre::ResourceManager */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_ResourceManager_reloadAll00
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
#endif //#ifndef TOLUA_DISABLE

/* method: remove of class  Ogre::ResourceManager */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_ResourceManager_remove00
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
#endif //#ifndef TOLUA_DISABLE

/* method: remove of class  Ogre::ResourceManager */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_ResourceManager_remove01
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
#endif //#ifndef TOLUA_DISABLE

/* method: remove of class  Ogre::ResourceManager */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_ResourceManager_remove02
static int tolua_Ogre_Ogre_ResourceManager_remove02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::ResourceManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::ResourceHandle",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ogre::ResourceManager* self = (Ogre::ResourceManager*)  tolua_tousertype(tolua_S,1,0);
  Ogre::ResourceHandle handle = *((Ogre::ResourceHandle*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'remove'",NULL);
#endif
  {
   self->remove(handle);
  }
 }
 return 0;
tolua_lerror:
 return tolua_Ogre_Ogre_ResourceManager_remove01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeAll of class  Ogre::ResourceManager */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_ResourceManager_removeAll00
static int tolua_Ogre_Ogre_ResourceManager_removeAll00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeAll'",NULL);
#endif
  {
   self->removeAll();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeAll'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getByName of class  Ogre::ResourceManager */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_ResourceManager_getByName00
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
#endif //#ifndef TOLUA_DISABLE

/* method: getByHandle of class  Ogre::ResourceManager */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_ResourceManager_getByHandle00
static int tolua_Ogre_Ogre_ResourceManager_getByHandle00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::ResourceManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::ResourceHandle",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::ResourceManager* self = (Ogre::ResourceManager*)  tolua_tousertype(tolua_S,1,0);
  Ogre::ResourceHandle handle = *((Ogre::ResourceHandle*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getByHandle'",NULL);
#endif
  {
   Ogre::ResourcePtr tolua_ret = (Ogre::ResourcePtr)  self->getByHandle(handle);
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
 tolua_error(tolua_S,"#ferror in function 'getByHandle'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: resourceExists of class  Ogre::ResourceManager */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_ResourceManager_resourceExists00
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
#endif //#ifndef TOLUA_DISABLE

/* method: resourceExists of class  Ogre::ResourceManager */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_ResourceManager_resourceExists01
static int tolua_Ogre_Ogre_ResourceManager_resourceExists01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::ResourceManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::ResourceHandle",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ogre::ResourceManager* self = (Ogre::ResourceManager*)  tolua_tousertype(tolua_S,1,0);
  Ogre::ResourceHandle handle = *((Ogre::ResourceHandle*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'resourceExists'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->resourceExists(handle);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_ResourceManager_resourceExists00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getResourceType of class  Ogre::ResourceManager */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_ResourceManager_getResourceType00
static int tolua_Ogre_Ogre_ResourceManager_getResourceType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::ResourceManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::ResourceManager* self = (const Ogre::ResourceManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getResourceType'",NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getResourceType();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getResourceType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getResourceIterator of class  Ogre::ResourceManager */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_ResourceManager_getResourceIterator00
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
   Ogre::ResourceManager::ResourceMapIterator tolua_ret = (Ogre::ResourceManager::ResourceMapIterator)  self->getResourceIterator();
   {
#ifdef __cplusplus
    void* tolua_obj = new Ogre::ResourceManager::ResourceMapIterator(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::ResourceManager::ResourceMapIterator");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::ResourceManager::ResourceMapIterator));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::ResourceManager::ResourceMapIterator");
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

/* method: delete of class  ResourceMapIterator */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_ResourceManager_ResourceMapIterator_delete00
static int tolua_Ogre_Ogre_ResourceManager_ResourceMapIterator_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::ResourceManager::ResourceMapIterator",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::ResourceManager::ResourceMapIterator* self = (Ogre::ResourceManager::ResourceMapIterator*)  tolua_tousertype(tolua_S,1,0);
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

/* method: hasMoreElements of class  ResourceMapIterator */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_ResourceManager_ResourceMapIterator_hasMoreElements00
static int tolua_Ogre_Ogre_ResourceManager_ResourceMapIterator_hasMoreElements00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::ResourceManager::ResourceMapIterator",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::ResourceManager::ResourceMapIterator* self = (const Ogre::ResourceManager::ResourceMapIterator*)  tolua_tousertype(tolua_S,1,0);
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
#endif //#ifndef TOLUA_DISABLE

/* method: getNext of class  ResourceMapIterator */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_ResourceManager_ResourceMapIterator_getNext00
static int tolua_Ogre_Ogre_ResourceManager_ResourceMapIterator_getNext00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::ResourceManager::ResourceMapIterator",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::ResourceManager::ResourceMapIterator* self = (Ogre::ResourceManager::ResourceMapIterator*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getNext'",NULL);
#endif
  {
   Ogre::ResourcePtr tolua_ret = (Ogre::ResourcePtr)  self->getNext();
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
 tolua_error(tolua_S,"#ferror in function 'getNext'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: peekNextValue of class  ResourceMapIterator */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_ResourceManager_ResourceMapIterator_peekNextValue00
static int tolua_Ogre_Ogre_ResourceManager_ResourceMapIterator_peekNextValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::ResourceManager::ResourceMapIterator",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::ResourceManager::ResourceMapIterator* self = (Ogre::ResourceManager::ResourceMapIterator*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'peekNextValue'",NULL);
#endif
  {
   Ogre::ResourcePtr tolua_ret = (Ogre::ResourcePtr)  self->peekNextValue();
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
 tolua_error(tolua_S,"#ferror in function 'peekNextValue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: peekNextKey of class  ResourceMapIterator */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_ResourceManager_ResourceMapIterator_peekNextKey00
static int tolua_Ogre_Ogre_ResourceManager_ResourceMapIterator_peekNextKey00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::ResourceManager::ResourceMapIterator",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::ResourceManager::ResourceMapIterator* self = (Ogre::ResourceManager::ResourceMapIterator*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'peekNextKey'",NULL);
#endif
  {
   Ogre::ResourceHandle tolua_ret = (Ogre::ResourceHandle)  self->peekNextKey();
   {
#ifdef __cplusplus
    void* tolua_obj = new Ogre::ResourceHandle(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::ResourceHandle");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::ResourceHandle));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::ResourceHandle");
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
#endif //#ifndef TOLUA_DISABLE

/* method: peekNextValuePtr of class  ResourceMapIterator */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_ResourceManager_ResourceMapIterator_peekNextValuePtr00
static int tolua_Ogre_Ogre_ResourceManager_ResourceMapIterator_peekNextValuePtr00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::ResourceManager::ResourceMapIterator",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::ResourceManager::ResourceMapIterator* self = (Ogre::ResourceManager::ResourceMapIterator*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'peekNextValuePtr'",NULL);
#endif
  {
   Ogre::ResourcePtr* tolua_ret = (Ogre::ResourcePtr*)  self->peekNextValuePtr();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::ResourcePtr");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'peekNextValuePtr'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: moveNext of class  ResourceMapIterator */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_ResourceManager_ResourceMapIterator_moveNext00
static int tolua_Ogre_Ogre_ResourceManager_ResourceMapIterator_moveNext00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::ResourceManager::ResourceMapIterator",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::ResourceManager::ResourceMapIterator* self = (Ogre::ResourceManager::ResourceMapIterator*)  tolua_tousertype(tolua_S,1,0);
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
#endif //#ifndef TOLUA_DISABLE

/* method: getSingleton of class  Ogre::MaterialManager */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_MaterialManager_getSingleton00
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
#endif //#ifndef TOLUA_DISABLE

/* method: load of class  Ogre::MaterialManager */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_MaterialManager_load00
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
#endif //#ifndef TOLUA_DISABLE

/* method: getByName of class  Ogre::MaterialManager */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_MaterialManager_getByName00
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
#endif //#ifndef TOLUA_DISABLE

/* method: create of class  Ogre::MaterialManager */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_MaterialManager_create00
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
#endif //#ifndef TOLUA_DISABLE

/* method: isTransparent of class  Ogre::Material */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Material_isTransparent00
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
#endif //#ifndef TOLUA_DISABLE

/* method: setReceiveShadows of class  Ogre::Material */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Material_setReceiveShadows00
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
#endif //#ifndef TOLUA_DISABLE

/* method: getReceiveShadows of class  Ogre::Material */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Material_getReceiveShadows00
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
#endif //#ifndef TOLUA_DISABLE

/* method: setTransparencyCastsShadows of class  Ogre::Material */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Material_setTransparencyCastsShadows00
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
#endif //#ifndef TOLUA_DISABLE

/* method: getTransparencyCastsShadows of class  Ogre::Material */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Material_getTransparencyCastsShadows00
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
#endif //#ifndef TOLUA_DISABLE

/* method: getNumTechniques of class  Ogre::Material */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Material_getNumTechniques00
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
#endif //#ifndef TOLUA_DISABLE

/* method: touch of class  Ogre::Material */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Material_touch00
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
#endif //#ifndef TOLUA_DISABLE

/* method: get of class  Ogre::MaterialPtr */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_MaterialPtr_get00
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
#endif //#ifndef TOLUA_DISABLE

/* method: isNull of class  Ogre::MaterialPtr */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_MaterialPtr_isNull00
static int tolua_Ogre_Ogre_MaterialPtr_isNull00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isNull'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isNull();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isNull'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Ogre::MaterialSerializer */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_MaterialSerializer_new00
static int tolua_Ogre_Ogre_MaterialSerializer_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::MaterialSerializer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Ogre::MaterialSerializer* tolua_ret = (Ogre::MaterialSerializer*)  new Ogre::MaterialSerializer();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::MaterialSerializer");
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

/* method: new_local of class  Ogre::MaterialSerializer */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_MaterialSerializer_new00_local
static int tolua_Ogre_Ogre_MaterialSerializer_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::MaterialSerializer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Ogre::MaterialSerializer* tolua_ret = (Ogre::MaterialSerializer*)  new Ogre::MaterialSerializer();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"Ogre::MaterialSerializer");
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

/* method: delete of class  Ogre::MaterialSerializer */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_MaterialSerializer_delete00
static int tolua_Ogre_Ogre_MaterialSerializer_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::MaterialSerializer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::MaterialSerializer* self = (Ogre::MaterialSerializer*)  tolua_tousertype(tolua_S,1,0);
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

/* method: queueForExport of class  Ogre::MaterialSerializer */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_MaterialSerializer_queueForExport00
static int tolua_Ogre_Ogre_MaterialSerializer_queueForExport00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::MaterialSerializer",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::MaterialPtr",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::MaterialSerializer* self = (Ogre::MaterialSerializer*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::MaterialPtr* pMat = ((const Ogre::MaterialPtr*)  tolua_tousertype(tolua_S,2,0));
  bool clearQueued = ((bool)  tolua_toboolean(tolua_S,3,false));
  bool exportDefaults = ((bool)  tolua_toboolean(tolua_S,4,false));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'queueForExport'",NULL);
#endif
  {
   self->queueForExport(*pMat,clearQueued,exportDefaults);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'queueForExport'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: exportQueued of class  Ogre::MaterialSerializer */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_MaterialSerializer_exportQueued00
static int tolua_Ogre_Ogre_MaterialSerializer_exportQueued00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::MaterialSerializer",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::MaterialSerializer* self = (Ogre::MaterialSerializer*)  tolua_tousertype(tolua_S,1,0);
  const std::string filename = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const bool includeProgDef = ((const bool)  tolua_toboolean(tolua_S,3,false));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'exportQueued'",NULL);
#endif
  {
   self->exportQueued(filename,includeProgDef);
   tolua_pushcppstring(tolua_S,(const char*)filename);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'exportQueued'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: exportQueued of class  Ogre::MaterialSerializer */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_MaterialSerializer_exportQueued01
static int tolua_Ogre_Ogre_MaterialSerializer_exportQueued01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::MaterialSerializer",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ogre::MaterialSerializer* self = (Ogre::MaterialSerializer*)  tolua_tousertype(tolua_S,1,0);
  const std::string filename = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const bool includeProgDef = ((const bool)  tolua_toboolean(tolua_S,3,0));
  const std::string programFilename = ((const std::string)  tolua_tocppstring(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'exportQueued'",NULL);
#endif
  {
   self->exportQueued(filename,includeProgDef,programFilename);
   tolua_pushcppstring(tolua_S,(const char*)filename);
   tolua_pushcppstring(tolua_S,(const char*)programFilename);
  }
 }
 return 2;
tolua_lerror:
 return tolua_Ogre_Ogre_MaterialSerializer_exportQueued00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: exportMaterial of class  Ogre::MaterialSerializer */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_MaterialSerializer_exportMaterial00
static int tolua_Ogre_Ogre_MaterialSerializer_exportMaterial00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::MaterialSerializer",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::MaterialPtr",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,1,&tolua_err) ||
     !tolua_isboolean(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::MaterialSerializer* self = (Ogre::MaterialSerializer*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::MaterialPtr* pMat = ((const Ogre::MaterialPtr*)  tolua_tousertype(tolua_S,2,0));
  const std::string filename = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  bool exportDefaults = ((bool)  tolua_toboolean(tolua_S,4,false));
  const bool includeProgDef = ((const bool)  tolua_toboolean(tolua_S,5,false));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'exportMaterial'",NULL);
#endif
  {
   self->exportMaterial(*pMat,filename,exportDefaults,includeProgDef);
   tolua_pushcppstring(tolua_S,(const char*)filename);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'exportMaterial'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: exportMaterial of class  Ogre::MaterialSerializer */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_MaterialSerializer_exportMaterial01
static int tolua_Ogre_Ogre_MaterialSerializer_exportMaterial01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::MaterialSerializer",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::MaterialPtr",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,5,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ogre::MaterialSerializer* self = (Ogre::MaterialSerializer*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::MaterialPtr* pMat = ((const Ogre::MaterialPtr*)  tolua_tousertype(tolua_S,2,0));
  const std::string filename = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  bool exportDefaults = ((bool)  tolua_toboolean(tolua_S,4,0));
  const bool includeProgDef = ((const bool)  tolua_toboolean(tolua_S,5,0));
  const std::string programFilename = ((const std::string)  tolua_tocppstring(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'exportMaterial'",NULL);
#endif
  {
   self->exportMaterial(*pMat,filename,exportDefaults,includeProgDef,programFilename);
   tolua_pushcppstring(tolua_S,(const char*)filename);
   tolua_pushcppstring(tolua_S,(const char*)programFilename);
  }
 }
 return 2;
tolua_lerror:
 return tolua_Ogre_Ogre_MaterialSerializer_exportMaterial00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getQueuedAsString of class  Ogre::MaterialSerializer */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_MaterialSerializer_getQueuedAsString00
static int tolua_Ogre_Ogre_MaterialSerializer_getQueuedAsString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::MaterialSerializer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::MaterialSerializer* self = (const Ogre::MaterialSerializer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getQueuedAsString'",NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getQueuedAsString();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getQueuedAsString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clearQueue of class  Ogre::MaterialSerializer */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_MaterialSerializer_clearQueue00
static int tolua_Ogre_Ogre_MaterialSerializer_clearQueue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::MaterialSerializer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::MaterialSerializer* self = (Ogre::MaterialSerializer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clearQueue'",NULL);
#endif
  {
   self->clearQueue();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clearQueue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: parseScript of class  Ogre::MaterialSerializer */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_MaterialSerializer_parseScript00
static int tolua_Ogre_Ogre_MaterialSerializer_parseScript00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::MaterialSerializer",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::DataStreamPtr",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::MaterialSerializer* self = (Ogre::MaterialSerializer*)  tolua_tousertype(tolua_S,1,0);
  Ogre::DataStreamPtr* stream = ((Ogre::DataStreamPtr*)  tolua_tousertype(tolua_S,2,0));
  const std::string groupName = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'parseScript'",NULL);
#endif
  {
   self->parseScript(*stream,groupName);
   tolua_pushcppstring(tolua_S,(const char*)groupName);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'parseScript'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMaterialName of class  Ogre::SubEntity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SubEntity_getMaterialName00
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
#endif //#ifndef TOLUA_DISABLE

/* method: setMaterialName of class  Ogre::SubEntity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SubEntity_setMaterialName00
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
#endif //#ifndef TOLUA_DISABLE

/* method: setVisible of class  Ogre::SubEntity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SubEntity_setVisible00
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
#endif //#ifndef TOLUA_DISABLE

/* method: isVisible of class  Ogre::SubEntity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SubEntity_isVisible00
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
#endif //#ifndef TOLUA_DISABLE

/* method: getMaterial of class  Ogre::SubEntity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SubEntity_getMaterial00
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
#endif //#ifndef TOLUA_DISABLE

/* method: getSingleton of class  Ogre::MeshManager */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_MeshManager_getSingleton00
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
#endif //#ifndef TOLUA_DISABLE

/* method: load of class  Ogre::MeshManager */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_MeshManager_load00
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
#endif //#ifndef TOLUA_DISABLE

/* method: getByName of class  Ogre::MeshManager */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_MeshManager_getByName00
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
#endif //#ifndef TOLUA_DISABLE

/* method: create of class  Ogre::MeshManager */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_MeshManager_create00
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
#endif //#ifndef TOLUA_DISABLE

/* method: getNumSubMeshes of class  Ogre::Mesh */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Mesh_getNumSubMeshes00
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
#endif //#ifndef TOLUA_DISABLE

/* method: getSubMesh of class  Ogre::Mesh */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Mesh_getSubMesh00
static int tolua_Ogre_Ogre_Mesh_getSubMesh00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Mesh",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Mesh* self = (const Ogre::Mesh*)  tolua_tousertype(tolua_S,1,0);
  unsigned short index = ((unsigned short)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSubMesh'",NULL);
#endif
  {
   Ogre::SubMesh* tolua_ret = (Ogre::SubMesh*)  self->getSubMesh(index);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::SubMesh");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSubMesh'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getBounds of class  Ogre::Mesh */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Mesh_getBounds00
static int tolua_Ogre_Ogre_Mesh_getBounds00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getBounds'",NULL);
#endif
  {
   const Ogre::AxisAlignedBox& tolua_ret = (const Ogre::AxisAlignedBox&)  self->getBounds();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Ogre::AxisAlignedBox");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getBounds'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: hasSkeleton of class  Ogre::Mesh */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Mesh_hasSkeleton00
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
#endif //#ifndef TOLUA_DISABLE

/* method: getSkeletonName of class  Ogre::Mesh */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Mesh_getSkeletonName00
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
#endif //#ifndef TOLUA_DISABLE

/* method: getNumLodLevels of class  Ogre::Mesh */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Mesh_getNumLodLevels00
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
#endif //#ifndef TOLUA_DISABLE

/* method: get of class  Ogre::MeshPtr */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_MeshPtr_get00
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
#endif //#ifndef TOLUA_DISABLE

/* method: get of class  Ogre::MeshPtr */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_MeshPtr_get01
static int tolua_Ogre_Ogre_MeshPtr_get01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::MeshPtr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::MeshPtr* self = (const Ogre::MeshPtr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get'",NULL);
#endif
  {
   Ogre::Mesh* tolua_ret = (Ogre::Mesh*)  self->get();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::Mesh");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_MeshPtr_get00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: isNull of class  Ogre::MeshPtr */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_MeshPtr_isNull00
static int tolua_Ogre_Ogre_MeshPtr_isNull00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isNull'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isNull();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isNull'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: useSharedVertices of class  Ogre::SubMesh */
#ifndef TOLUA_DISABLE_tolua_get_Ogre__SubMesh_useSharedVertices
static int tolua_get_Ogre__SubMesh_useSharedVertices(lua_State* tolua_S)
{
  Ogre::SubMesh* self = (Ogre::SubMesh*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'useSharedVertices'",NULL);
#endif
  tolua_pushboolean(tolua_S,(bool)self->useSharedVertices);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: useSharedVertices of class  Ogre::SubMesh */
#ifndef TOLUA_DISABLE_tolua_set_Ogre__SubMesh_useSharedVertices
static int tolua_set_Ogre__SubMesh_useSharedVertices(lua_State* tolua_S)
{
  Ogre::SubMesh* self = (Ogre::SubMesh*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'useSharedVertices'",NULL);
  if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->useSharedVertices = ((bool)  tolua_toboolean(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: parent of class  Ogre::SubMesh */
#ifndef TOLUA_DISABLE_tolua_get_Ogre__SubMesh_parent_ptr
static int tolua_get_Ogre__SubMesh_parent_ptr(lua_State* tolua_S)
{
  Ogre::SubMesh* self = (Ogre::SubMesh*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'parent'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)self->parent,"Ogre::Mesh");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: parent of class  Ogre::SubMesh */
#ifndef TOLUA_DISABLE_tolua_set_Ogre__SubMesh_parent_ptr
static int tolua_set_Ogre__SubMesh_parent_ptr(lua_State* tolua_S)
{
  Ogre::SubMesh* self = (Ogre::SubMesh*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'parent'",NULL);
  if (!tolua_isusertype(tolua_S,2,"Ogre::Mesh",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->parent = ((Ogre::Mesh*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: setMaterialName of class  Ogre::SubMesh */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SubMesh_setMaterialName00
static int tolua_Ogre_Ogre_SubMesh_setMaterialName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::SubMesh",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::SubMesh* self = (Ogre::SubMesh*)  tolua_tousertype(tolua_S,1,0);
  const std::string matName = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMaterialName'",NULL);
#endif
  {
   self->setMaterialName(matName);
   tolua_pushcppstring(tolua_S,(const char*)matName);
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

/* method: getMaterialName of class  Ogre::SubMesh */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SubMesh_getMaterialName00
static int tolua_Ogre_Ogre_SubMesh_getMaterialName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::SubMesh",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::SubMesh* self = (const Ogre::SubMesh*)  tolua_tousertype(tolua_S,1,0);
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
#endif //#ifndef TOLUA_DISABLE

/* get function: x of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_get_Ogre__Vector3_x
static int tolua_get_Ogre__Vector3_x(lua_State* tolua_S)
{
  Ogre::Vector3* self = (Ogre::Vector3*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->x);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: x of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_set_Ogre__Vector3_x
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
#endif //#ifndef TOLUA_DISABLE

/* get function: y of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_get_Ogre__Vector3_y
static int tolua_get_Ogre__Vector3_y(lua_State* tolua_S)
{
  Ogre::Vector3* self = (Ogre::Vector3*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->y);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: y of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_set_Ogre__Vector3_y
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
#endif //#ifndef TOLUA_DISABLE

/* get function: z of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_get_Ogre__Vector3_z
static int tolua_get_Ogre__Vector3_z(lua_State* tolua_S)
{
  Ogre::Vector3* self = (Ogre::Vector3*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'z'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->z);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: z of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_set_Ogre__Vector3_z
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
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Vector3_new00
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
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Vector3_new00_local
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
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Vector3_new01
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
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Vector3_new01_local
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
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Vector3_new02
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
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Vector3_new02_local
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
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Vector3_new03
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
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Vector3_new03_local
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
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Vector3_new04
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
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Vector3_new04_local
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
#endif //#ifndef TOLUA_DISABLE

/* method: operator[] of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Vector3__geti00
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
#endif //#ifndef TOLUA_DISABLE

/* method: operator&[] of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Vector3__seti00
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
#endif //#ifndef TOLUA_DISABLE

/* method: operator[] of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Vector3__geti01
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
#endif //#ifndef TOLUA_DISABLE

/* method: operator== of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Vector3__eq00
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
#endif //#ifndef TOLUA_DISABLE

/* method: operator+ of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Vector3__add00
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
#endif //#ifndef TOLUA_DISABLE

/* method: operator- of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Vector3__sub00
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
#endif //#ifndef TOLUA_DISABLE

/* method: operator* of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Vector3__mul00
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
#endif //#ifndef TOLUA_DISABLE

/* method: operator* of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Vector3__mul01
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
#endif //#ifndef TOLUA_DISABLE

/* method: operator/ of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Vector3__div00
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
#endif //#ifndef TOLUA_DISABLE

/* method: operator/ of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Vector3__div01
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
#endif //#ifndef TOLUA_DISABLE

/* method: operator- of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Vector3__sub01
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
#endif //#ifndef TOLUA_DISABLE

/* method: length of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Vector3_length00
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
#endif //#ifndef TOLUA_DISABLE

/* method: squaredLength of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Vector3_squaredLength00
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
#endif //#ifndef TOLUA_DISABLE

/* method: dotProduct of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Vector3_dotProduct00
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
#endif //#ifndef TOLUA_DISABLE

/* method: normalise of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Vector3_normalise00
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
#endif //#ifndef TOLUA_DISABLE

/* method: crossProduct of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Vector3_crossProduct00
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
#endif //#ifndef TOLUA_DISABLE

/* method: midPoint of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Vector3_midPoint00
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
#endif //#ifndef TOLUA_DISABLE

/* method: operator< of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Vector3__lt00
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
#endif //#ifndef TOLUA_DISABLE

/* method: makeFloor of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Vector3_makeFloor00
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
#endif //#ifndef TOLUA_DISABLE

/* method: makeCeil of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Vector3_makeCeil00
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
#endif //#ifndef TOLUA_DISABLE

/* method: perpendicular of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Vector3_perpendicular00
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
#endif //#ifndef TOLUA_DISABLE

/* method: randomDeviant of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Vector3_randomDeviant00
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
#endif //#ifndef TOLUA_DISABLE

/* method: getRotationTo of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Vector3_getRotationTo00
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
#endif //#ifndef TOLUA_DISABLE

/* method: isZeroLength of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Vector3_isZeroLength00
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
#endif //#ifndef TOLUA_DISABLE

/* method: normalisedCopy of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Vector3_normalisedCopy00
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
#endif //#ifndef TOLUA_DISABLE

/* method: reflect of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Vector3_reflect00
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
#endif //#ifndef TOLUA_DISABLE

/* method: positionEquals of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Vector3_positionEquals00
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
#endif //#ifndef TOLUA_DISABLE

/* method: directionEquals of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Vector3_directionEquals00
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
#endif //#ifndef TOLUA_DISABLE

/* get function: ZERO of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_get_Ogre__Vector3_ZERO
static int tolua_get_Ogre__Vector3_ZERO(lua_State* tolua_S)
{
  tolua_pushusertype(tolua_S,(void*)&Ogre::Vector3::ZERO,"const Ogre::Vector3");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: UNIT_X of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_get_Ogre__Vector3_UNIT_X
static int tolua_get_Ogre__Vector3_UNIT_X(lua_State* tolua_S)
{
  tolua_pushusertype(tolua_S,(void*)&Ogre::Vector3::UNIT_X,"const Ogre::Vector3");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: UNIT_Y of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_get_Ogre__Vector3_UNIT_Y
static int tolua_get_Ogre__Vector3_UNIT_Y(lua_State* tolua_S)
{
  tolua_pushusertype(tolua_S,(void*)&Ogre::Vector3::UNIT_Y,"const Ogre::Vector3");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: UNIT_Z of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_get_Ogre__Vector3_UNIT_Z
static int tolua_get_Ogre__Vector3_UNIT_Z(lua_State* tolua_S)
{
  tolua_pushusertype(tolua_S,(void*)&Ogre::Vector3::UNIT_Z,"const Ogre::Vector3");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: NEGATIVE_UNIT_X of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_get_Ogre__Vector3_NEGATIVE_UNIT_X
static int tolua_get_Ogre__Vector3_NEGATIVE_UNIT_X(lua_State* tolua_S)
{
  tolua_pushusertype(tolua_S,(void*)&Ogre::Vector3::NEGATIVE_UNIT_X,"const Ogre::Vector3");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: NEGATIVE_UNIT_Y of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_get_Ogre__Vector3_NEGATIVE_UNIT_Y
static int tolua_get_Ogre__Vector3_NEGATIVE_UNIT_Y(lua_State* tolua_S)
{
  tolua_pushusertype(tolua_S,(void*)&Ogre::Vector3::NEGATIVE_UNIT_Y,"const Ogre::Vector3");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: NEGATIVE_UNIT_Z of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_get_Ogre__Vector3_NEGATIVE_UNIT_Z
static int tolua_get_Ogre__Vector3_NEGATIVE_UNIT_Z(lua_State* tolua_S)
{
  tolua_pushusertype(tolua_S,(void*)&Ogre::Vector3::NEGATIVE_UNIT_Z,"const Ogre::Vector3");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: UNIT_SCALE of class  Ogre::Vector3 */
#ifndef TOLUA_DISABLE_tolua_get_Ogre__Vector3_UNIT_SCALE
static int tolua_get_Ogre__Vector3_UNIT_SCALE(lua_State* tolua_S)
{
  tolua_pushusertype(tolua_S,(void*)&Ogre::Vector3::UNIT_SCALE,"const Ogre::Vector3");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Quaternion_new00
static int tolua_Ogre_Ogre_Quaternion_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float fW = ((float)  tolua_tonumber(tolua_S,2,0));
  float fX = ((float)  tolua_tonumber(tolua_S,3,0));
  float fY = ((float)  tolua_tonumber(tolua_S,4,0));
  float fZ = ((float)  tolua_tonumber(tolua_S,5,0));
  {
   Ogre::Quaternion* tolua_ret = (Ogre::Quaternion*)  new Ogre::Quaternion(fW,fX,fY,fZ);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::Quaternion");
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

/* method: new_local of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Quaternion_new00_local
static int tolua_Ogre_Ogre_Quaternion_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float fW = ((float)  tolua_tonumber(tolua_S,2,0));
  float fX = ((float)  tolua_tonumber(tolua_S,3,0));
  float fY = ((float)  tolua_tonumber(tolua_S,4,0));
  float fZ = ((float)  tolua_tonumber(tolua_S,5,0));
  {
   Ogre::Quaternion* tolua_ret = (Ogre::Quaternion*)  new Ogre::Quaternion(fW,fX,fY,fZ);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"Ogre::Quaternion");
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

/* method: new of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Quaternion_new01
static int tolua_Ogre_Ogre_Quaternion_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Quaternion* rkQ = ((const Ogre::Quaternion*)  tolua_tousertype(tolua_S,2,0));
  {
   Ogre::Quaternion* tolua_ret = (Ogre::Quaternion*)  new Ogre::Quaternion(*rkQ);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::Quaternion");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_Quaternion_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Quaternion_new01_local
static int tolua_Ogre_Ogre_Quaternion_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Quaternion* rkQ = ((const Ogre::Quaternion*)  tolua_tousertype(tolua_S,2,0));
  {
   Ogre::Quaternion* tolua_ret = (Ogre::Quaternion*)  new Ogre::Quaternion(*rkQ);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"Ogre::Quaternion");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_Quaternion_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Quaternion_new02
static int tolua_Ogre_Ogre_Quaternion_new02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Matrix3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Matrix3* rot = ((const Ogre::Matrix3*)  tolua_tousertype(tolua_S,2,0));
  {
   Ogre::Quaternion* tolua_ret = (Ogre::Quaternion*)  new Ogre::Quaternion(*rot);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::Quaternion");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_Quaternion_new01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Quaternion_new02_local
static int tolua_Ogre_Ogre_Quaternion_new02_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Matrix3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Matrix3* rot = ((const Ogre::Matrix3*)  tolua_tousertype(tolua_S,2,0));
  {
   Ogre::Quaternion* tolua_ret = (Ogre::Quaternion*)  new Ogre::Quaternion(*rot);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"Ogre::Quaternion");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_Quaternion_new01_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Quaternion_new03
static int tolua_Ogre_Ogre_Quaternion_new03(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Radian",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Radian* rfAngle = ((const Ogre::Radian*)  tolua_tousertype(tolua_S,2,0));
  const Ogre::Vector3* rkAxis = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,3,0));
  {
   Ogre::Quaternion* tolua_ret = (Ogre::Quaternion*)  new Ogre::Quaternion(*rfAngle,*rkAxis);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::Quaternion");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_Quaternion_new02(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Quaternion_new03_local
static int tolua_Ogre_Ogre_Quaternion_new03_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Radian",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Radian* rfAngle = ((const Ogre::Radian*)  tolua_tousertype(tolua_S,2,0));
  const Ogre::Vector3* rkAxis = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,3,0));
  {
   Ogre::Quaternion* tolua_ret = (Ogre::Quaternion*)  new Ogre::Quaternion(*rfAngle,*rkAxis);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"Ogre::Quaternion");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_Quaternion_new02_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Quaternion_new04
static int tolua_Ogre_Ogre_Quaternion_new04(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Vector3* xaxis = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
  const Ogre::Vector3* yaxis = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,3,0));
  const Ogre::Vector3* zaxis = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,4,0));
  {
   Ogre::Quaternion* tolua_ret = (Ogre::Quaternion*)  new Ogre::Quaternion(*xaxis,*yaxis,*zaxis);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::Quaternion");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_Quaternion_new03(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Quaternion_new04_local
static int tolua_Ogre_Ogre_Quaternion_new04_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Vector3* xaxis = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
  const Ogre::Vector3* yaxis = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,3,0));
  const Ogre::Vector3* zaxis = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,4,0));
  {
   Ogre::Quaternion* tolua_ret = (Ogre::Quaternion*)  new Ogre::Quaternion(*xaxis,*yaxis,*zaxis);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"Ogre::Quaternion");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_Quaternion_new03_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: FromRotationMatrix of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Quaternion_FromRotationMatrix00
static int tolua_Ogre_Ogre_Quaternion_FromRotationMatrix00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Matrix3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Quaternion* self = (Ogre::Quaternion*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Matrix3* kRot = ((const Ogre::Matrix3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'FromRotationMatrix'",NULL);
#endif
  {
   self->FromRotationMatrix(*kRot);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'FromRotationMatrix'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ToRotationMatrix of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Quaternion_ToRotationMatrix00
static int tolua_Ogre_Ogre_Quaternion_ToRotationMatrix00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::Matrix3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Quaternion* self = (const Ogre::Quaternion*)  tolua_tousertype(tolua_S,1,0);
  Ogre::Matrix3* kRot = ((Ogre::Matrix3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ToRotationMatrix'",NULL);
#endif
  {
   self->ToRotationMatrix(*kRot);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ToRotationMatrix'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: FromAngleAxis of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Quaternion_FromAngleAxis00
static int tolua_Ogre_Ogre_Quaternion_FromAngleAxis00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Radian",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Quaternion* self = (Ogre::Quaternion*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Radian* rfAngle = ((const Ogre::Radian*)  tolua_tousertype(tolua_S,2,0));
  const Ogre::Vector3* rkAxis = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'FromAngleAxis'",NULL);
#endif
  {
   self->FromAngleAxis(*rfAngle,*rkAxis);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'FromAngleAxis'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ToAngleAxis of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Quaternion_ToAngleAxis00
static int tolua_Ogre_Ogre_Quaternion_ToAngleAxis00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::Radian",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Quaternion* self = (const Ogre::Quaternion*)  tolua_tousertype(tolua_S,1,0);
  Ogre::Radian* rfAngle = ((Ogre::Radian*)  tolua_tousertype(tolua_S,2,0));
  Ogre::Vector3* rkAxis = ((Ogre::Vector3*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ToAngleAxis'",NULL);
#endif
  {
   self->ToAngleAxis(*rfAngle,*rkAxis);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ToAngleAxis'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ToAngleAxis of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Quaternion_ToAngleAxis01
static int tolua_Ogre_Ogre_Quaternion_ToAngleAxis01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::Degree",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Quaternion* self = (const Ogre::Quaternion*)  tolua_tousertype(tolua_S,1,0);
  Ogre::Degree* dAngle = ((Ogre::Degree*)  tolua_tousertype(tolua_S,2,0));
  Ogre::Vector3* rkAxis = ((Ogre::Vector3*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ToAngleAxis'",NULL);
#endif
  {
   self->ToAngleAxis(*dAngle,*rkAxis);
  }
 }
 return 0;
tolua_lerror:
 return tolua_Ogre_Ogre_Quaternion_ToAngleAxis00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: FromAxes of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Quaternion_FromAxes00
static int tolua_Ogre_Ogre_Quaternion_FromAxes00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Quaternion* self = (Ogre::Quaternion*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector3* akAxis = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'FromAxes'",NULL);
#endif
  {
   self->FromAxes(akAxis);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'FromAxes'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: FromAxes of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Quaternion_FromAxes01
static int tolua_Ogre_Ogre_Quaternion_FromAxes01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ogre::Quaternion* self = (Ogre::Quaternion*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector3* xAxis = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
  const Ogre::Vector3* yAxis = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,3,0));
  const Ogre::Vector3* zAxis = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'FromAxes'",NULL);
#endif
  {
   self->FromAxes(*xAxis,*yAxis,*zAxis);
  }
 }
 return 0;
tolua_lerror:
 return tolua_Ogre_Ogre_Quaternion_FromAxes00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: ToAxes of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Quaternion_ToAxes00
static int tolua_Ogre_Ogre_Quaternion_ToAxes00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Quaternion* self = (const Ogre::Quaternion*)  tolua_tousertype(tolua_S,1,0);
  Ogre::Vector3* akAxis = ((Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ToAxes'",NULL);
#endif
  {
   self->ToAxes(akAxis);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ToAxes'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ToAxes of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Quaternion_ToAxes01
static int tolua_Ogre_Ogre_Quaternion_ToAxes01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::Vector3",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"Ogre::Vector3",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Quaternion* self = (const Ogre::Quaternion*)  tolua_tousertype(tolua_S,1,0);
  Ogre::Vector3* xAxis = ((Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
  Ogre::Vector3* yAxis = ((Ogre::Vector3*)  tolua_tousertype(tolua_S,3,0));
  Ogre::Vector3* zAxis = ((Ogre::Vector3*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ToAxes'",NULL);
#endif
  {
   self->ToAxes(*xAxis,*yAxis,*zAxis);
  }
 }
 return 0;
tolua_lerror:
 return tolua_Ogre_Ogre_Quaternion_ToAxes00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: xAxis of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Quaternion_xAxis00
static int tolua_Ogre_Ogre_Quaternion_xAxis00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Quaternion* self = (const Ogre::Quaternion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'xAxis'",NULL);
#endif
  {
   Ogre::Vector3 tolua_ret = (Ogre::Vector3)  self->xAxis();
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
 tolua_error(tolua_S,"#ferror in function 'xAxis'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: yAxis of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Quaternion_yAxis00
static int tolua_Ogre_Ogre_Quaternion_yAxis00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Quaternion* self = (const Ogre::Quaternion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'yAxis'",NULL);
#endif
  {
   Ogre::Vector3 tolua_ret = (Ogre::Vector3)  self->yAxis();
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
 tolua_error(tolua_S,"#ferror in function 'yAxis'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: zAxis of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Quaternion_zAxis00
static int tolua_Ogre_Ogre_Quaternion_zAxis00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Quaternion* self = (const Ogre::Quaternion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'zAxis'",NULL);
#endif
  {
   Ogre::Vector3 tolua_ret = (Ogre::Vector3)  self->zAxis();
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
 tolua_error(tolua_S,"#ferror in function 'zAxis'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator+ of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Quaternion__add00
static int tolua_Ogre_Ogre_Quaternion__add00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Quaternion* self = (const Ogre::Quaternion*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Quaternion* rkQ = ((const Ogre::Quaternion*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator+'",NULL);
#endif
  {
   Ogre::Quaternion tolua_ret = (Ogre::Quaternion)  self->operator+(*rkQ);
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
 tolua_error(tolua_S,"#ferror in function '.add'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator- of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Quaternion__sub00
static int tolua_Ogre_Ogre_Quaternion__sub00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Quaternion* self = (const Ogre::Quaternion*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Quaternion* rkQ = ((const Ogre::Quaternion*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator-'",NULL);
#endif
  {
   Ogre::Quaternion tolua_ret = (Ogre::Quaternion)  self->operator-(*rkQ);
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
 tolua_error(tolua_S,"#ferror in function '.sub'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator* of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Quaternion__mul00
static int tolua_Ogre_Ogre_Quaternion__mul00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Quaternion* self = (const Ogre::Quaternion*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Quaternion* rkQ = ((const Ogre::Quaternion*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator*'",NULL);
#endif
  {
   Ogre::Quaternion tolua_ret = (Ogre::Quaternion)  self->operator*(*rkQ);
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
 tolua_error(tolua_S,"#ferror in function '.mul'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator* of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Quaternion__mul01
static int tolua_Ogre_Ogre_Quaternion__mul01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Quaternion* self = (const Ogre::Quaternion*)  tolua_tousertype(tolua_S,1,0);
  float fScalar = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator*'",NULL);
#endif
  {
   Ogre::Quaternion tolua_ret = (Ogre::Quaternion)  self->operator*(fScalar);
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
 return tolua_Ogre_Ogre_Quaternion__mul00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator- of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Quaternion__sub01
static int tolua_Ogre_Ogre_Quaternion__sub01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Quaternion* self = (const Ogre::Quaternion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator-'",NULL);
#endif
  {
   Ogre::Quaternion tolua_ret = (Ogre::Quaternion)  self->operator-();
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
 return tolua_Ogre_Ogre_Quaternion__sub00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator== of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Quaternion__eq00
static int tolua_Ogre_Ogre_Quaternion__eq00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Quaternion* self = (const Ogre::Quaternion*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Quaternion* rhs = ((const Ogre::Quaternion*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator=='",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->operator==(*rhs);
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

/* method: Dot of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Quaternion_Dot00
static int tolua_Ogre_Ogre_Quaternion_Dot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Quaternion* self = (const Ogre::Quaternion*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Quaternion* rkQ = ((const Ogre::Quaternion*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Dot'",NULL);
#endif
  {
   float tolua_ret = (float)  self->Dot(*rkQ);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Dot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Norm of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Quaternion_Norm00
static int tolua_Ogre_Ogre_Quaternion_Norm00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Quaternion* self = (const Ogre::Quaternion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Norm'",NULL);
#endif
  {
   float tolua_ret = (float)  self->Norm();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Norm'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: normalise of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Quaternion_normalise00
static int tolua_Ogre_Ogre_Quaternion_normalise00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Quaternion* self = (Ogre::Quaternion*)  tolua_tousertype(tolua_S,1,0);
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
#endif //#ifndef TOLUA_DISABLE

/* method: Inverse of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Quaternion_Inverse00
static int tolua_Ogre_Ogre_Quaternion_Inverse00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Quaternion* self = (const Ogre::Quaternion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Inverse'",NULL);
#endif
  {
   Ogre::Quaternion tolua_ret = (Ogre::Quaternion)  self->Inverse();
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
 tolua_error(tolua_S,"#ferror in function 'Inverse'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: UnitInverse of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Quaternion_UnitInverse00
static int tolua_Ogre_Ogre_Quaternion_UnitInverse00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Quaternion* self = (const Ogre::Quaternion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'UnitInverse'",NULL);
#endif
  {
   Ogre::Quaternion tolua_ret = (Ogre::Quaternion)  self->UnitInverse();
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
 tolua_error(tolua_S,"#ferror in function 'UnitInverse'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Exp of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Quaternion_Exp00
static int tolua_Ogre_Ogre_Quaternion_Exp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Quaternion* self = (const Ogre::Quaternion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Exp'",NULL);
#endif
  {
   Ogre::Quaternion tolua_ret = (Ogre::Quaternion)  self->Exp();
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
 tolua_error(tolua_S,"#ferror in function 'Exp'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Log of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Quaternion_Log00
static int tolua_Ogre_Ogre_Quaternion_Log00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Quaternion* self = (const Ogre::Quaternion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Log'",NULL);
#endif
  {
   Ogre::Quaternion tolua_ret = (Ogre::Quaternion)  self->Log();
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
 tolua_error(tolua_S,"#ferror in function 'Log'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator* of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Quaternion__mul02
static int tolua_Ogre_Ogre_Quaternion__mul02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Quaternion* self = (const Ogre::Quaternion*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector3* rkVector = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator*'",NULL);
#endif
  {
   Ogre::Vector3 tolua_ret = (Ogre::Vector3)  self->operator*(*rkVector);
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
 return tolua_Ogre_Ogre_Quaternion__mul01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRoll of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Quaternion_getRoll00
static int tolua_Ogre_Ogre_Quaternion_getRoll00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Quaternion* self = (const Ogre::Quaternion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRoll'",NULL);
#endif
  {
   Ogre::Radian tolua_ret = (Ogre::Radian)  self->getRoll();
   {
#ifdef __cplusplus
    void* tolua_obj = new Ogre::Radian(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Radian");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::Radian));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Radian");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRoll'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPitch of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Quaternion_getPitch00
static int tolua_Ogre_Ogre_Quaternion_getPitch00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Quaternion* self = (const Ogre::Quaternion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPitch'",NULL);
#endif
  {
   Ogre::Radian tolua_ret = (Ogre::Radian)  self->getPitch();
   {
#ifdef __cplusplus
    void* tolua_obj = new Ogre::Radian(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Radian");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::Radian));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Radian");
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
#endif //#ifndef TOLUA_DISABLE

/* method: getYaw of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Quaternion_getYaw00
static int tolua_Ogre_Ogre_Quaternion_getYaw00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Quaternion* self = (const Ogre::Quaternion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getYaw'",NULL);
#endif
  {
   Ogre::Radian tolua_ret = (Ogre::Radian)  self->getYaw();
   {
#ifdef __cplusplus
    void* tolua_obj = new Ogre::Radian(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Radian");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::Radian));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Radian");
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
#endif //#ifndef TOLUA_DISABLE

/* method: equals of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Quaternion_equals00
static int tolua_Ogre_Ogre_Quaternion_equals00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Ogre::Radian",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Quaternion* self = (const Ogre::Quaternion*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Quaternion* rhs = ((const Ogre::Quaternion*)  tolua_tousertype(tolua_S,2,0));
  const Ogre::Radian* tolerance = ((const Ogre::Radian*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'equals'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->equals(*rhs,*tolerance);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'equals'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Slerp of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Quaternion_Slerp00
static int tolua_Ogre_Ogre_Quaternion_Slerp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float fT = ((float)  tolua_tonumber(tolua_S,2,0));
  const Ogre::Quaternion* rkP = ((const Ogre::Quaternion*)  tolua_tousertype(tolua_S,3,0));
  const Ogre::Quaternion* rkQ = ((const Ogre::Quaternion*)  tolua_tousertype(tolua_S,4,0));
  bool shortestPath = ((bool)  tolua_toboolean(tolua_S,5,false));
  {
   Ogre::Quaternion tolua_ret = (Ogre::Quaternion)  Ogre::Quaternion::Slerp(fT,*rkP,*rkQ,shortestPath);
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
 tolua_error(tolua_S,"#ferror in function 'Slerp'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SlerpExtraSpins of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Quaternion_SlerpExtraSpins00
static int tolua_Ogre_Ogre_Quaternion_SlerpExtraSpins00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float fT = ((float)  tolua_tonumber(tolua_S,2,0));
  const Ogre::Quaternion* rkP = ((const Ogre::Quaternion*)  tolua_tousertype(tolua_S,3,0));
  const Ogre::Quaternion* rkQ = ((const Ogre::Quaternion*)  tolua_tousertype(tolua_S,4,0));
  int iExtraSpins = ((int)  tolua_tonumber(tolua_S,5,0));
  {
   Ogre::Quaternion tolua_ret = (Ogre::Quaternion)  Ogre::Quaternion::SlerpExtraSpins(fT,*rkP,*rkQ,iExtraSpins);
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
 tolua_error(tolua_S,"#ferror in function 'SlerpExtraSpins'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Intermediate of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Quaternion_Intermediate00
static int tolua_Ogre_Ogre_Quaternion_Intermediate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,6,"Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Quaternion* rkQ0 = ((const Ogre::Quaternion*)  tolua_tousertype(tolua_S,2,0));
  const Ogre::Quaternion* rkQ1 = ((const Ogre::Quaternion*)  tolua_tousertype(tolua_S,3,0));
  const Ogre::Quaternion* rkQ2 = ((const Ogre::Quaternion*)  tolua_tousertype(tolua_S,4,0));
  Ogre::Quaternion* rka = ((Ogre::Quaternion*)  tolua_tousertype(tolua_S,5,0));
  Ogre::Quaternion* rkB = ((Ogre::Quaternion*)  tolua_tousertype(tolua_S,6,0));
  {
   Ogre::Quaternion::Intermediate(*rkQ0,*rkQ1,*rkQ2,*rka,*rkB);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Intermediate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Squad of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Quaternion_Squad00
static int tolua_Ogre_Ogre_Quaternion_Squad00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,6,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,7,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float fT = ((float)  tolua_tonumber(tolua_S,2,0));
  const Ogre::Quaternion* rkP = ((const Ogre::Quaternion*)  tolua_tousertype(tolua_S,3,0));
  const Ogre::Quaternion* rkA = ((const Ogre::Quaternion*)  tolua_tousertype(tolua_S,4,0));
  const Ogre::Quaternion* rkB = ((const Ogre::Quaternion*)  tolua_tousertype(tolua_S,5,0));
  const Ogre::Quaternion* rkQ = ((const Ogre::Quaternion*)  tolua_tousertype(tolua_S,6,0));
  bool shortestPath = ((bool)  tolua_toboolean(tolua_S,7,false));
  {
   Ogre::Quaternion tolua_ret = (Ogre::Quaternion)  Ogre::Quaternion::Squad(fT,*rkP,*rkA,*rkB,*rkQ,shortestPath);
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
 tolua_error(tolua_S,"#ferror in function 'Squad'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: nlerp of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Quaternion_nlerp00
static int tolua_Ogre_Ogre_Quaternion_nlerp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float fT = ((float)  tolua_tonumber(tolua_S,2,0));
  const Ogre::Quaternion* rkP = ((const Ogre::Quaternion*)  tolua_tousertype(tolua_S,3,0));
  const Ogre::Quaternion* rkQ = ((const Ogre::Quaternion*)  tolua_tousertype(tolua_S,4,0));
  bool shortestPath = ((bool)  tolua_toboolean(tolua_S,5,false));
  {
   Ogre::Quaternion tolua_ret = (Ogre::Quaternion)  Ogre::Quaternion::nlerp(fT,*rkP,*rkQ,shortestPath);
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
 tolua_error(tolua_S,"#ferror in function 'nlerp'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: ms_fEpsilon of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_get_Ogre__Quaternion_ms_fEpsilon
static int tolua_get_Ogre__Quaternion_ms_fEpsilon(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Ogre::Quaternion::ms_fEpsilon);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: ZERO of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_get_Ogre__Quaternion_ZERO
static int tolua_get_Ogre__Quaternion_ZERO(lua_State* tolua_S)
{
  tolua_pushusertype(tolua_S,(void*)&Ogre::Quaternion::ZERO,"const Ogre::Quaternion");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: IDENTITY of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_get_Ogre__Quaternion_IDENTITY
static int tolua_get_Ogre__Quaternion_IDENTITY(lua_State* tolua_S)
{
  tolua_pushusertype(tolua_S,(void*)&Ogre::Quaternion::IDENTITY,"const Ogre::Quaternion");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: w of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_get_Ogre__Quaternion_w
static int tolua_get_Ogre__Quaternion_w(lua_State* tolua_S)
{
  Ogre::Quaternion* self = (Ogre::Quaternion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'w'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->w);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: w of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_set_Ogre__Quaternion_w
static int tolua_set_Ogre__Quaternion_w(lua_State* tolua_S)
{
  Ogre::Quaternion* self = (Ogre::Quaternion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'w'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->w = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: x of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_get_Ogre__Quaternion_x
static int tolua_get_Ogre__Quaternion_x(lua_State* tolua_S)
{
  Ogre::Quaternion* self = (Ogre::Quaternion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->x);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: x of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_set_Ogre__Quaternion_x
static int tolua_set_Ogre__Quaternion_x(lua_State* tolua_S)
{
  Ogre::Quaternion* self = (Ogre::Quaternion*)  tolua_tousertype(tolua_S,1,0);
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
#endif //#ifndef TOLUA_DISABLE

/* get function: y of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_get_Ogre__Quaternion_y
static int tolua_get_Ogre__Quaternion_y(lua_State* tolua_S)
{
  Ogre::Quaternion* self = (Ogre::Quaternion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->y);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: y of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_set_Ogre__Quaternion_y
static int tolua_set_Ogre__Quaternion_y(lua_State* tolua_S)
{
  Ogre::Quaternion* self = (Ogre::Quaternion*)  tolua_tousertype(tolua_S,1,0);
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
#endif //#ifndef TOLUA_DISABLE

/* get function: z of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_get_Ogre__Quaternion_z
static int tolua_get_Ogre__Quaternion_z(lua_State* tolua_S)
{
  Ogre::Quaternion* self = (Ogre::Quaternion*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'z'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->z);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: z of class  Ogre::Quaternion */
#ifndef TOLUA_DISABLE_tolua_set_Ogre__Quaternion_z
static int tolua_set_Ogre__Quaternion_z(lua_State* tolua_S)
{
  Ogre::Quaternion* self = (Ogre::Quaternion*)  tolua_tousertype(tolua_S,1,0);
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
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Ogre::Radian */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Radian_new00
static int tolua_Ogre_Ogre_Radian_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Radian",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float r = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   Ogre::Radian* tolua_ret = (Ogre::Radian*)  new Ogre::Radian(r);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::Radian");
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

/* method: new_local of class  Ogre::Radian */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Radian_new00_local
static int tolua_Ogre_Ogre_Radian_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Radian",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float r = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   Ogre::Radian* tolua_ret = (Ogre::Radian*)  new Ogre::Radian(r);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"Ogre::Radian");
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

/* method: new of class  Ogre::Radian */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Radian_new01
static int tolua_Ogre_Ogre_Radian_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Radian",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Degree",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Degree* d = ((const Ogre::Degree*)  tolua_tousertype(tolua_S,2,0));
  {
   Ogre::Radian* tolua_ret = (Ogre::Radian*)  new Ogre::Radian(*d);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::Radian");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_Radian_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Ogre::Radian */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Radian_new01_local
static int tolua_Ogre_Ogre_Radian_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Radian",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Degree",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Degree* d = ((const Ogre::Degree*)  tolua_tousertype(tolua_S,2,0));
  {
   Ogre::Radian* tolua_ret = (Ogre::Radian*)  new Ogre::Radian(*d);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"Ogre::Radian");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_Radian_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: valueDegrees of class  Ogre::Radian */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Radian_valueDegrees00
static int tolua_Ogre_Ogre_Radian_valueDegrees00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Radian",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Radian* self = (const Ogre::Radian*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'valueDegrees'",NULL);
#endif
  {
   float tolua_ret = (float)  self->valueDegrees();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'valueDegrees'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: valueRadians of class  Ogre::Radian */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Radian_valueRadians00
static int tolua_Ogre_Ogre_Radian_valueRadians00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Radian",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Radian* self = (const Ogre::Radian*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'valueRadians'",NULL);
#endif
  {
   float tolua_ret = (float)  self->valueRadians();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'valueRadians'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: valueAngleUnits of class  Ogre::Radian */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Radian_valueAngleUnits00
static int tolua_Ogre_Ogre_Radian_valueAngleUnits00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Radian",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Radian* self = (const Ogre::Radian*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'valueAngleUnits'",NULL);
#endif
  {
   float tolua_ret = (float)  self->valueAngleUnits();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'valueAngleUnits'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator+ of class  Ogre::Radian */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Radian__add00
static int tolua_Ogre_Ogre_Radian__add00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Radian",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Radian",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Radian* self = (const Ogre::Radian*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Radian* r = ((const Ogre::Radian*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator+'",NULL);
#endif
  {
   Ogre::Radian tolua_ret = (Ogre::Radian)  self->operator+(*r);
   {
#ifdef __cplusplus
    void* tolua_obj = new Ogre::Radian(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Radian");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::Radian));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Radian");
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
#endif //#ifndef TOLUA_DISABLE

/* method: operator+ of class  Ogre::Radian */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Radian__add01
static int tolua_Ogre_Ogre_Radian__add01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Radian",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Degree",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Radian* self = (const Ogre::Radian*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Degree* d = ((const Ogre::Degree*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator+'",NULL);
#endif
  {
   Ogre::Radian tolua_ret = (Ogre::Radian)  self->operator+(*d);
   {
#ifdef __cplusplus
    void* tolua_obj = new Ogre::Radian(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Radian");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::Radian));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Radian");
#endif
   }
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_Radian__add00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator- of class  Ogre::Radian */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Radian__sub00
static int tolua_Ogre_Ogre_Radian__sub00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Radian",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Radian* self = (Ogre::Radian*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator-'",NULL);
#endif
  {
   Ogre::Radian tolua_ret = (Ogre::Radian)  self->operator-();
   {
#ifdef __cplusplus
    void* tolua_obj = new Ogre::Radian(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Radian");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::Radian));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Radian");
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
#endif //#ifndef TOLUA_DISABLE

/* method: operator- of class  Ogre::Radian */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Radian__sub01
static int tolua_Ogre_Ogre_Radian__sub01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Radian",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Radian",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Radian* self = (const Ogre::Radian*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Radian* r = ((const Ogre::Radian*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator-'",NULL);
#endif
  {
   Ogre::Radian tolua_ret = (Ogre::Radian)  self->operator-(*r);
   {
#ifdef __cplusplus
    void* tolua_obj = new Ogre::Radian(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Radian");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::Radian));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Radian");
#endif
   }
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_Radian__sub00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator- of class  Ogre::Radian */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Radian__sub02
static int tolua_Ogre_Ogre_Radian__sub02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Radian",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Degree",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Radian* self = (const Ogre::Radian*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Degree* d = ((const Ogre::Degree*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator-'",NULL);
#endif
  {
   Ogre::Radian tolua_ret = (Ogre::Radian)  self->operator-(*d);
   {
#ifdef __cplusplus
    void* tolua_obj = new Ogre::Radian(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Radian");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::Radian));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Radian");
#endif
   }
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_Radian__sub01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator* of class  Ogre::Radian */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Radian__mul00
static int tolua_Ogre_Ogre_Radian__mul00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Radian",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Radian* self = (const Ogre::Radian*)  tolua_tousertype(tolua_S,1,0);
  float f = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator*'",NULL);
#endif
  {
   Ogre::Radian tolua_ret = (Ogre::Radian)  self->operator*(f);
   {
#ifdef __cplusplus
    void* tolua_obj = new Ogre::Radian(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Radian");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::Radian));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Radian");
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
#endif //#ifndef TOLUA_DISABLE

/* method: operator* of class  Ogre::Radian */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Radian__mul01
static int tolua_Ogre_Ogre_Radian__mul01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Radian",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Radian",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Radian* self = (const Ogre::Radian*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Radian* f = ((const Ogre::Radian*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator*'",NULL);
#endif
  {
   Ogre::Radian tolua_ret = (Ogre::Radian)  self->operator*(*f);
   {
#ifdef __cplusplus
    void* tolua_obj = new Ogre::Radian(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Radian");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::Radian));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Radian");
#endif
   }
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_Radian__mul00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator/ of class  Ogre::Radian */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Radian__div00
static int tolua_Ogre_Ogre_Radian__div00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Radian",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Radian* self = (const Ogre::Radian*)  tolua_tousertype(tolua_S,1,0);
  float f = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator/'",NULL);
#endif
  {
   Ogre::Radian tolua_ret = (Ogre::Radian)  self->operator/(f);
   {
#ifdef __cplusplus
    void* tolua_obj = new Ogre::Radian(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Radian");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::Radian));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Radian");
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
#endif //#ifndef TOLUA_DISABLE

/* method: operator< of class  Ogre::Radian */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Radian__lt00
static int tolua_Ogre_Ogre_Radian__lt00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Radian",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Radian",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Radian* self = (const Ogre::Radian*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Radian* r = ((const Ogre::Radian*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator<'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->operator<(*r);
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
#endif //#ifndef TOLUA_DISABLE

/* method: operator<= of class  Ogre::Radian */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Radian__le00
static int tolua_Ogre_Ogre_Radian__le00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Radian",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Radian",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Radian* self = (const Ogre::Radian*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Radian* r = ((const Ogre::Radian*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator<='",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->operator<=(*r);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.le'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator== of class  Ogre::Radian */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Radian__eq00
static int tolua_Ogre_Ogre_Radian__eq00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Radian",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Radian",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Radian* self = (const Ogre::Radian*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Radian* r = ((const Ogre::Radian*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator=='",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->operator==(*r);
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

/* method: new of class  Ogre::Degree */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Degree_new00
static int tolua_Ogre_Ogre_Degree_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Degree",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float d = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   Ogre::Degree* tolua_ret = (Ogre::Degree*)  new Ogre::Degree(d);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::Degree");
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

/* method: new_local of class  Ogre::Degree */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Degree_new00_local
static int tolua_Ogre_Ogre_Degree_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Degree",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float d = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   Ogre::Degree* tolua_ret = (Ogre::Degree*)  new Ogre::Degree(d);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"Ogre::Degree");
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

/* method: new of class  Ogre::Degree */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Degree_new01
static int tolua_Ogre_Ogre_Degree_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Degree",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Radian",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Radian* r = ((const Ogre::Radian*)  tolua_tousertype(tolua_S,2,0));
  {
   Ogre::Degree* tolua_ret = (Ogre::Degree*)  new Ogre::Degree(*r);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::Degree");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_Degree_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Ogre::Degree */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Degree_new01_local
static int tolua_Ogre_Ogre_Degree_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Degree",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Radian",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Radian* r = ((const Ogre::Radian*)  tolua_tousertype(tolua_S,2,0));
  {
   Ogre::Degree* tolua_ret = (Ogre::Degree*)  new Ogre::Degree(*r);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"Ogre::Degree");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_Degree_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: valueDegrees of class  Ogre::Degree */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Degree_valueDegrees00
static int tolua_Ogre_Ogre_Degree_valueDegrees00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Degree",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Degree* self = (const Ogre::Degree*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'valueDegrees'",NULL);
#endif
  {
   float tolua_ret = (float)  self->valueDegrees();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'valueDegrees'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: valueRadians of class  Ogre::Degree */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Degree_valueRadians00
static int tolua_Ogre_Ogre_Degree_valueRadians00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Degree",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Degree* self = (const Ogre::Degree*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'valueRadians'",NULL);
#endif
  {
   float tolua_ret = (float)  self->valueRadians();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'valueRadians'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: valueAngleUnits of class  Ogre::Degree */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Degree_valueAngleUnits00
static int tolua_Ogre_Ogre_Degree_valueAngleUnits00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Degree",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Degree* self = (const Ogre::Degree*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'valueAngleUnits'",NULL);
#endif
  {
   float tolua_ret = (float)  self->valueAngleUnits();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'valueAngleUnits'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator+ of class  Ogre::Degree */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Degree__add00
static int tolua_Ogre_Ogre_Degree__add00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Degree",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Degree",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Degree* self = (const Ogre::Degree*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Degree* d = ((const Ogre::Degree*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator+'",NULL);
#endif
  {
   Ogre::Degree tolua_ret = (Ogre::Degree)  self->operator+(*d);
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
 tolua_error(tolua_S,"#ferror in function '.add'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator+ of class  Ogre::Degree */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Degree__add01
static int tolua_Ogre_Ogre_Degree__add01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Degree",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Radian",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Degree* self = (const Ogre::Degree*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Radian* r = ((const Ogre::Radian*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator+'",NULL);
#endif
  {
   Ogre::Degree tolua_ret = (Ogre::Degree)  self->operator+(*r);
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
tolua_lerror:
 return tolua_Ogre_Ogre_Degree__add00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator- of class  Ogre::Degree */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Degree__sub00
static int tolua_Ogre_Ogre_Degree__sub00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Degree",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Degree* self = (Ogre::Degree*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator-'",NULL);
#endif
  {
   Ogre::Degree tolua_ret = (Ogre::Degree)  self->operator-();
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
 tolua_error(tolua_S,"#ferror in function '.sub'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator- of class  Ogre::Degree */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Degree__sub01
static int tolua_Ogre_Ogre_Degree__sub01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Degree",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Degree",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Degree* self = (const Ogre::Degree*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Degree* d = ((const Ogre::Degree*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator-'",NULL);
#endif
  {
   Ogre::Degree tolua_ret = (Ogre::Degree)  self->operator-(*d);
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
tolua_lerror:
 return tolua_Ogre_Ogre_Degree__sub00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator- of class  Ogre::Degree */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Degree__sub02
static int tolua_Ogre_Ogre_Degree__sub02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Degree",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Radian",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Degree* self = (const Ogre::Degree*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Radian* r = ((const Ogre::Radian*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator-'",NULL);
#endif
  {
   Ogre::Degree tolua_ret = (Ogre::Degree)  self->operator-(*r);
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
tolua_lerror:
 return tolua_Ogre_Ogre_Degree__sub01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator* of class  Ogre::Degree */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Degree__mul00
static int tolua_Ogre_Ogre_Degree__mul00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Degree",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Degree* self = (const Ogre::Degree*)  tolua_tousertype(tolua_S,1,0);
  float f = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator*'",NULL);
#endif
  {
   Ogre::Degree tolua_ret = (Ogre::Degree)  self->operator*(f);
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
 tolua_error(tolua_S,"#ferror in function '.mul'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator* of class  Ogre::Degree */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Degree__mul01
static int tolua_Ogre_Ogre_Degree__mul01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Degree",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Degree",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Degree* self = (const Ogre::Degree*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Degree* f = ((const Ogre::Degree*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator*'",NULL);
#endif
  {
   Ogre::Degree tolua_ret = (Ogre::Degree)  self->operator*(*f);
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
tolua_lerror:
 return tolua_Ogre_Ogre_Degree__mul00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator/ of class  Ogre::Degree */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Degree__div00
static int tolua_Ogre_Ogre_Degree__div00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Degree",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Degree* self = (const Ogre::Degree*)  tolua_tousertype(tolua_S,1,0);
  float f = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator/'",NULL);
#endif
  {
   Ogre::Degree tolua_ret = (Ogre::Degree)  self->operator/(f);
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
 tolua_error(tolua_S,"#ferror in function '.div'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator< of class  Ogre::Degree */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Degree__lt00
static int tolua_Ogre_Ogre_Degree__lt00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Degree",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Degree",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Degree* self = (const Ogre::Degree*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Degree* d = ((const Ogre::Degree*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator<'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->operator<(*d);
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
#endif //#ifndef TOLUA_DISABLE

/* method: operator<= of class  Ogre::Degree */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Degree__le00
static int tolua_Ogre_Ogre_Degree__le00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Degree",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Degree",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Degree* self = (const Ogre::Degree*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Degree* d = ((const Ogre::Degree*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator<='",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->operator<=(*d);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.le'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator== of class  Ogre::Degree */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Degree__eq00
static int tolua_Ogre_Ogre_Degree__eq00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Degree",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Degree",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Degree* self = (const Ogre::Degree*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Degree* d = ((const Ogre::Degree*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator=='",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->operator==(*d);
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

/* method: new of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_new00
static int tolua_Ogre_Ogre_Math_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned int trigTableSize = ((unsigned int)  tolua_tonumber(tolua_S,2,4096));
  {
   Ogre::Math* tolua_ret = (Ogre::Math*)  new Ogre::Math(trigTableSize);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::Math");
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

/* method: new_local of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_new00_local
static int tolua_Ogre_Ogre_Math_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned int trigTableSize = ((unsigned int)  tolua_tonumber(tolua_S,2,4096));
  {
   Ogre::Math* tolua_ret = (Ogre::Math*)  new Ogre::Math(trigTableSize);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"Ogre::Math");
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

/* method: delete of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_delete00
static int tolua_Ogre_Ogre_Math_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Math* self = (Ogre::Math*)  tolua_tousertype(tolua_S,1,0);
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

/* method: IAbs of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_IAbs00
static int tolua_Ogre_Ogre_Math_IAbs00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int iValue = ((int)  tolua_tonumber(tolua_S,2,0));
  {
   int tolua_ret = (int)  Ogre::Math::IAbs(iValue);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IAbs'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ICeil of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_ICeil00
static int tolua_Ogre_Ogre_Math_ICeil00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float fValue = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   int tolua_ret = (int)  Ogre::Math::ICeil(fValue);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ICeil'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IFloor of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_IFloor00
static int tolua_Ogre_Ogre_Math_IFloor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float fValue = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   int tolua_ret = (int)  Ogre::Math::IFloor(fValue);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IFloor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ISign of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_ISign00
static int tolua_Ogre_Ogre_Math_ISign00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int iValue = ((int)  tolua_tonumber(tolua_S,2,0));
  {
   int tolua_ret = (int)  Ogre::Math::ISign(iValue);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ISign'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Abs of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_Abs00
static int tolua_Ogre_Ogre_Math_Abs00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float fValue = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   float tolua_ret = (float)  Ogre::Math::Abs(fValue);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Abs'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Abs of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_Abs01
static int tolua_Ogre_Ogre_Math_Abs01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Degree",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Degree* dValue = ((const Ogre::Degree*)  tolua_tousertype(tolua_S,2,0));
  {
   Ogre::Degree tolua_ret = (Ogre::Degree)  Ogre::Math::Abs(*dValue);
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
tolua_lerror:
 return tolua_Ogre_Ogre_Math_Abs00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: Abs of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_Abs02
static int tolua_Ogre_Ogre_Math_Abs02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Radian",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Radian* rValue = ((const Ogre::Radian*)  tolua_tousertype(tolua_S,2,0));
  {
   Ogre::Radian tolua_ret = (Ogre::Radian)  Ogre::Math::Abs(*rValue);
   {
#ifdef __cplusplus
    void* tolua_obj = new Ogre::Radian(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Radian");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::Radian));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Radian");
#endif
   }
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_Math_Abs01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: ACos of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_ACos00
static int tolua_Ogre_Ogre_Math_ACos00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float fValue = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   Ogre::Radian tolua_ret = (Ogre::Radian)  Ogre::Math::ACos(fValue);
   {
#ifdef __cplusplus
    void* tolua_obj = new Ogre::Radian(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Radian");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::Radian));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Radian");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ACos'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ASin of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_ASin00
static int tolua_Ogre_Ogre_Math_ASin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float fValue = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   Ogre::Radian tolua_ret = (Ogre::Radian)  Ogre::Math::ASin(fValue);
   {
#ifdef __cplusplus
    void* tolua_obj = new Ogre::Radian(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Radian");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::Radian));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Radian");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ASin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ATan of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_ATan00
static int tolua_Ogre_Ogre_Math_ATan00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float fValue = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   Ogre::Radian tolua_ret = (Ogre::Radian)  Ogre::Math::ATan(fValue);
   {
#ifdef __cplusplus
    void* tolua_obj = new Ogre::Radian(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Radian");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::Radian));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Radian");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ATan'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ATan2 of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_ATan200
static int tolua_Ogre_Ogre_Math_ATan200(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float fY = ((float)  tolua_tonumber(tolua_S,2,0));
  float fX = ((float)  tolua_tonumber(tolua_S,3,0));
  {
   Ogre::Radian tolua_ret = (Ogre::Radian)  Ogre::Math::ATan2(fY,fX);
   {
#ifdef __cplusplus
    void* tolua_obj = new Ogre::Radian(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Radian");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::Radian));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Radian");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ATan2'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Ceil of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_Ceil00
static int tolua_Ogre_Ogre_Math_Ceil00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float fValue = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   float tolua_ret = (float)  Ogre::Math::Ceil(fValue);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Ceil'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Cos of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_Cos00
static int tolua_Ogre_Ogre_Math_Cos00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Radian",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Radian* fValue = ((const Ogre::Radian*)  tolua_tousertype(tolua_S,2,0));
  bool useTables = ((bool)  tolua_toboolean(tolua_S,3,false));
  {
   float tolua_ret = (float)  Ogre::Math::Cos(*fValue,useTables);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Cos'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Cos of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_Cos01
static int tolua_Ogre_Ogre_Math_Cos01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  float fValue = ((float)  tolua_tonumber(tolua_S,2,0));
  bool useTables = ((bool)  tolua_toboolean(tolua_S,3,false));
  {
   float tolua_ret = (float)  Ogre::Math::Cos(fValue,useTables);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_Math_Cos00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: Exp of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_Exp00
static int tolua_Ogre_Ogre_Math_Exp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float fValue = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   float tolua_ret = (float)  Ogre::Math::Exp(fValue);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Exp'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Floor of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_Floor00
static int tolua_Ogre_Ogre_Math_Floor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float fValue = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   float tolua_ret = (float)  Ogre::Math::Floor(fValue);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Floor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Log of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_Log00
static int tolua_Ogre_Ogre_Math_Log00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float fValue = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   float tolua_ret = (float)  Ogre::Math::Log(fValue);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Log'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Pow of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_Pow00
static int tolua_Ogre_Ogre_Math_Pow00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float fBase = ((float)  tolua_tonumber(tolua_S,2,0));
  float fExponent = ((float)  tolua_tonumber(tolua_S,3,0));
  {
   float tolua_ret = (float)  Ogre::Math::Pow(fBase,fExponent);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Pow'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Sign of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_Sign00
static int tolua_Ogre_Ogre_Math_Sign00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float fValue = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   float tolua_ret = (float)  Ogre::Math::Sign(fValue);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Sign'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Sign of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_Sign01
static int tolua_Ogre_Ogre_Math_Sign01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Radian",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Radian* rValue = ((const Ogre::Radian*)  tolua_tousertype(tolua_S,2,0));
  {
   Ogre::Radian tolua_ret = (Ogre::Radian)  Ogre::Math::Sign(*rValue);
   {
#ifdef __cplusplus
    void* tolua_obj = new Ogre::Radian(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Radian");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::Radian));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Radian");
#endif
   }
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_Math_Sign00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: Sign of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_Sign02
static int tolua_Ogre_Ogre_Math_Sign02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Degree",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Degree* dValue = ((const Ogre::Degree*)  tolua_tousertype(tolua_S,2,0));
  {
   Ogre::Degree tolua_ret = (Ogre::Degree)  Ogre::Math::Sign(*dValue);
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
tolua_lerror:
 return tolua_Ogre_Ogre_Math_Sign01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: Sin of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_Sin00
static int tolua_Ogre_Ogre_Math_Sin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Radian",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Radian* fValue = ((const Ogre::Radian*)  tolua_tousertype(tolua_S,2,0));
  bool useTables = ((bool)  tolua_toboolean(tolua_S,3,false));
  {
   float tolua_ret = (float)  Ogre::Math::Sin(*fValue,useTables);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Sin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Sin of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_Sin01
static int tolua_Ogre_Ogre_Math_Sin01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  float fValue = ((float)  tolua_tonumber(tolua_S,2,0));
  bool useTables = ((bool)  tolua_toboolean(tolua_S,3,false));
  {
   float tolua_ret = (float)  Ogre::Math::Sin(fValue,useTables);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_Math_Sin00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: Sqr of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_Sqr00
static int tolua_Ogre_Ogre_Math_Sqr00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float fValue = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   float tolua_ret = (float)  Ogre::Math::Sqr(fValue);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Sqr'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Sqrt of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_Sqrt00
static int tolua_Ogre_Ogre_Math_Sqrt00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float fValue = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   float tolua_ret = (float)  Ogre::Math::Sqrt(fValue);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Sqrt'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Sqrt of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_Sqrt01
static int tolua_Ogre_Ogre_Math_Sqrt01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Radian",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Radian* fValue = ((const Ogre::Radian*)  tolua_tousertype(tolua_S,2,0));
  {
   Ogre::Radian tolua_ret = (Ogre::Radian)  Ogre::Math::Sqrt(*fValue);
   {
#ifdef __cplusplus
    void* tolua_obj = new Ogre::Radian(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Radian");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::Radian));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Radian");
#endif
   }
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_Math_Sqrt00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: Sqrt of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_Sqrt02
static int tolua_Ogre_Ogre_Math_Sqrt02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Degree",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Degree* fValue = ((const Ogre::Degree*)  tolua_tousertype(tolua_S,2,0));
  {
   Ogre::Degree tolua_ret = (Ogre::Degree)  Ogre::Math::Sqrt(*fValue);
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
tolua_lerror:
 return tolua_Ogre_Ogre_Math_Sqrt01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: InvSqrt of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_InvSqrt00
static int tolua_Ogre_Ogre_Math_InvSqrt00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float fValue = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   float tolua_ret = (float)  Ogre::Math::InvSqrt(fValue);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'InvSqrt'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: UnitRandom of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_UnitRandom00
static int tolua_Ogre_Ogre_Math_UnitRandom00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   float tolua_ret = (float)  Ogre::Math::UnitRandom();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'UnitRandom'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: RangeRandom of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_RangeRandom00
static int tolua_Ogre_Ogre_Math_RangeRandom00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float fLow = ((float)  tolua_tonumber(tolua_S,2,0));
  float fHigh = ((float)  tolua_tonumber(tolua_S,3,0));
  {
   float tolua_ret = (float)  Ogre::Math::RangeRandom(fLow,fHigh);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'RangeRandom'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SymmetricRandom of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_SymmetricRandom00
static int tolua_Ogre_Ogre_Math_SymmetricRandom00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   float tolua_ret = (float)  Ogre::Math::SymmetricRandom();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SymmetricRandom'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Tan of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_Tan00
static int tolua_Ogre_Ogre_Math_Tan00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Radian",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Radian* fValue = ((const Ogre::Radian*)  tolua_tousertype(tolua_S,2,0));
  bool useTables = ((bool)  tolua_toboolean(tolua_S,3,false));
  {
   float tolua_ret = (float)  Ogre::Math::Tan(*fValue,useTables);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Tan'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Tan of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_Tan01
static int tolua_Ogre_Ogre_Math_Tan01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  float fValue = ((float)  tolua_tonumber(tolua_S,2,0));
  bool useTables = ((bool)  tolua_toboolean(tolua_S,3,false));
  {
   float tolua_ret = (float)  Ogre::Math::Tan(fValue,useTables);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_Math_Tan00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: DegreesToRadians of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_DegreesToRadians00
static int tolua_Ogre_Ogre_Math_DegreesToRadians00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float degrees = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   float tolua_ret = (float)  Ogre::Math::DegreesToRadians(degrees);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DegreesToRadians'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: RadiansToDegrees of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_RadiansToDegrees00
static int tolua_Ogre_Ogre_Math_RadiansToDegrees00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float radians = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   float tolua_ret = (float)  Ogre::Math::RadiansToDegrees(radians);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'RadiansToDegrees'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setAngleUnit of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_setAngleUnit00
static int tolua_Ogre_Ogre_Math_setAngleUnit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Math::AngleUnit unit = ((Ogre::Math::AngleUnit) (int)  tolua_tonumber(tolua_S,2,0));
  {
   Ogre::Math::setAngleUnit(unit);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAngleUnit'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAngleUnit of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_getAngleUnit00
static int tolua_Ogre_Ogre_Math_getAngleUnit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Ogre::Math::AngleUnit tolua_ret = (Ogre::Math::AngleUnit)  Ogre::Math::getAngleUnit();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAngleUnit'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AngleUnitsToRadians of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_AngleUnitsToRadians00
static int tolua_Ogre_Ogre_Math_AngleUnitsToRadians00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float units = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   float tolua_ret = (float)  Ogre::Math::AngleUnitsToRadians(units);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AngleUnitsToRadians'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: RadiansToAngleUnits of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_RadiansToAngleUnits00
static int tolua_Ogre_Ogre_Math_RadiansToAngleUnits00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float radians = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   float tolua_ret = (float)  Ogre::Math::RadiansToAngleUnits(radians);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'RadiansToAngleUnits'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AngleUnitsToDegrees of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_AngleUnitsToDegrees00
static int tolua_Ogre_Ogre_Math_AngleUnitsToDegrees00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float units = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   float tolua_ret = (float)  Ogre::Math::AngleUnitsToDegrees(units);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AngleUnitsToDegrees'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: DegreesToAngleUnits of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_DegreesToAngleUnits00
static int tolua_Ogre_Ogre_Math_DegreesToAngleUnits00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float degrees = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   float tolua_ret = (float)  Ogre::Math::DegreesToAngleUnits(degrees);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DegreesToAngleUnits'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: pointInTri2D of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_pointInTri2D00
static int tolua_Ogre_Ogre_Math_pointInTri2D00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector2",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Ogre::Vector2",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"const Ogre::Vector2",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"const Ogre::Vector2",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Vector2* p = ((const Ogre::Vector2*)  tolua_tousertype(tolua_S,2,0));
  const Ogre::Vector2* a = ((const Ogre::Vector2*)  tolua_tousertype(tolua_S,3,0));
  const Ogre::Vector2* b = ((const Ogre::Vector2*)  tolua_tousertype(tolua_S,4,0));
  const Ogre::Vector2* c = ((const Ogre::Vector2*)  tolua_tousertype(tolua_S,5,0));
  {
   bool tolua_ret = (bool)  Ogre::Math::pointInTri2D(*p,*a,*b,*c);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'pointInTri2D'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: pointInTri3D of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_pointInTri3D00
static int tolua_Ogre_Ogre_Math_pointInTri3D00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,6,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Vector3* p = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
  const Ogre::Vector3* a = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,3,0));
  const Ogre::Vector3* b = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,4,0));
  const Ogre::Vector3* c = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,5,0));
  const Ogre::Vector3* normal = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,6,0));
  {
   bool tolua_ret = (bool)  Ogre::Math::pointInTri3D(*p,*a,*b,*c,*normal);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'pointInTri3D'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: intersects of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_intersects00
static int tolua_Ogre_Ogre_Math_intersects00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Ray",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Ogre::Plane",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Ray* ray = ((const Ogre::Ray*)  tolua_tousertype(tolua_S,2,0));
  const Ogre::Plane* plane = ((const Ogre::Plane*)  tolua_tousertype(tolua_S,3,0));
  {
   std::pair<bool,float> tolua_ret = (std::pair<bool,float>)  Ogre::Math::intersects(*ray,*plane);
   {
#ifdef __cplusplus
    void* tolua_obj = new std::pair<bool,float>(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"std::pair<bool,float>");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(std::pair<bool,float>));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"std::pair<bool,float>");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'intersects'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: intersects of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_intersects01
static int tolua_Ogre_Ogre_Math_intersects01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Ray",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Ogre::Sphere",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Ray* ray = ((const Ogre::Ray*)  tolua_tousertype(tolua_S,2,0));
  const Ogre::Sphere* sphere = ((const Ogre::Sphere*)  tolua_tousertype(tolua_S,3,0));
  bool discardInside = ((bool)  tolua_toboolean(tolua_S,4,true));
  {
   std::pair<bool,float> tolua_ret = (std::pair<bool,float>)  Ogre::Math::intersects(*ray,*sphere,discardInside);
   {
#ifdef __cplusplus
    void* tolua_obj = new std::pair<bool,float>(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"std::pair<bool,float>");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(std::pair<bool,float>));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"std::pair<bool,float>");
#endif
   }
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_Math_intersects00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: intersects of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_intersects02
static int tolua_Ogre_Ogre_Math_intersects02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Ray",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Ray* ray = ((const Ogre::Ray*)  tolua_tousertype(tolua_S,2,0));
  const Ogre::AxisAlignedBox* box = ((const Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,3,0));
  {
   std::pair<bool,float> tolua_ret = (std::pair<bool,float>)  Ogre::Math::intersects(*ray,*box);
   {
#ifdef __cplusplus
    void* tolua_obj = new std::pair<bool,float>(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"std::pair<bool,float>");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(std::pair<bool,float>));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"std::pair<bool,float>");
#endif
   }
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_Math_intersects01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: intersects of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_intersects03
static int tolua_Ogre_Ogre_Math_intersects03(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Ray",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Ray* ray = ((const Ogre::Ray*)  tolua_tousertype(tolua_S,2,0));
  const Ogre::AxisAlignedBox* box = ((const Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,3,0));
  float d1 = ((float)  tolua_tonumber(tolua_S,4,0));
  float d2 = ((float)  tolua_tonumber(tolua_S,5,0));
  {
   bool tolua_ret = (bool)  Ogre::Math::intersects(*ray,*box,&d1,&d2);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushnumber(tolua_S,(lua_Number)d1);
   tolua_pushnumber(tolua_S,(lua_Number)d2);
  }
 }
 return 3;
tolua_lerror:
 return tolua_Ogre_Ogre_Math_intersects02(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: intersects of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_intersects04
static int tolua_Ogre_Ogre_Math_intersects04(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Ray",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,6,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,7,1,&tolua_err) ||
     !tolua_isboolean(tolua_S,8,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,9,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Ray* ray = ((const Ogre::Ray*)  tolua_tousertype(tolua_S,2,0));
  const Ogre::Vector3* a = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,3,0));
  const Ogre::Vector3* b = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,4,0));
  const Ogre::Vector3* c = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,5,0));
  const Ogre::Vector3* normal = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,6,0));
  bool positiveSide = ((bool)  tolua_toboolean(tolua_S,7,true));
  bool negativeSide = ((bool)  tolua_toboolean(tolua_S,8,true));
  {
   std::pair<bool,float> tolua_ret = (std::pair<bool,float>)  Ogre::Math::intersects(*ray,*a,*b,*c,*normal,positiveSide,negativeSide);
   {
#ifdef __cplusplus
    void* tolua_obj = new std::pair<bool,float>(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"std::pair<bool,float>");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(std::pair<bool,float>));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"std::pair<bool,float>");
#endif
   }
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_Math_intersects03(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: intersects of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_intersects05
static int tolua_Ogre_Ogre_Math_intersects05(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Ray",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,6,1,&tolua_err) ||
     !tolua_isboolean(tolua_S,7,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Ray* ray = ((const Ogre::Ray*)  tolua_tousertype(tolua_S,2,0));
  const Ogre::Vector3* a = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,3,0));
  const Ogre::Vector3* b = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,4,0));
  const Ogre::Vector3* c = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,5,0));
  bool positiveSide = ((bool)  tolua_toboolean(tolua_S,6,true));
  bool negativeSide = ((bool)  tolua_toboolean(tolua_S,7,true));
  {
   std::pair<bool,float> tolua_ret = (std::pair<bool,float>)  Ogre::Math::intersects(*ray,*a,*b,*c,positiveSide,negativeSide);
   {
#ifdef __cplusplus
    void* tolua_obj = new std::pair<bool,float>(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"std::pair<bool,float>");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(std::pair<bool,float>));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"std::pair<bool,float>");
#endif
   }
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_Math_intersects04(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: intersects of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_intersects06
static int tolua_Ogre_Ogre_Math_intersects06(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Sphere",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Sphere* sphere = ((const Ogre::Sphere*)  tolua_tousertype(tolua_S,2,0));
  const Ogre::AxisAlignedBox* box = ((const Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  Ogre::Math::intersects(*sphere,*box);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_Math_intersects05(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: intersects of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_intersects07
static int tolua_Ogre_Ogre_Math_intersects07(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Plane",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Plane* plane = ((const Ogre::Plane*)  tolua_tousertype(tolua_S,2,0));
  const Ogre::AxisAlignedBox* box = ((const Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  Ogre::Math::intersects(*plane,*box);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_Math_intersects06(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: intersects of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_intersects08
static int tolua_Ogre_Ogre_Math_intersects08(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Ray",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const std::vector<Ogre::Plane>",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Ray* ray = ((const Ogre::Ray*)  tolua_tousertype(tolua_S,2,0));
  const std::vector<Ogre::Plane>* planeList = ((const std::vector<Ogre::Plane>*)  tolua_tousertype(tolua_S,3,0));
  bool normalIsOutside = ((bool)  tolua_toboolean(tolua_S,4,0));
  {
   std::pair<bool,float> tolua_ret = (std::pair<bool,float>)  Ogre::Math::intersects(*ray,*planeList,normalIsOutside);
   {
#ifdef __cplusplus
    void* tolua_obj = new std::pair<bool,float>(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"std::pair<bool,float>");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(std::pair<bool,float>));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"std::pair<bool,float>");
#endif
   }
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_Math_intersects07(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: intersects of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_intersects09
static int tolua_Ogre_Ogre_Math_intersects09(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Ray",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const std::list<Ogre::Plane>",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Ray* ray = ((const Ogre::Ray*)  tolua_tousertype(tolua_S,2,0));
  const std::list<Ogre::Plane>* planeList = ((const std::list<Ogre::Plane>*)  tolua_tousertype(tolua_S,3,0));
  bool normalIsOutside = ((bool)  tolua_toboolean(tolua_S,4,0));
  {
   std::pair<bool,float> tolua_ret = (std::pair<bool,float>)  Ogre::Math::intersects(*ray,*planeList,normalIsOutside);
   {
#ifdef __cplusplus
    void* tolua_obj = new std::pair<bool,float>(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"std::pair<bool,float>");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(std::pair<bool,float>));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"std::pair<bool,float>");
#endif
   }
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_Math_intersects08(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: intersects of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_intersects10
static int tolua_Ogre_Ogre_Math_intersects10(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Sphere",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Ogre::Plane",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Sphere* sphere = ((const Ogre::Sphere*)  tolua_tousertype(tolua_S,2,0));
  const Ogre::Plane* plane = ((const Ogre::Plane*)  tolua_tousertype(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  Ogre::Math::intersects(*sphere,*plane);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_Math_intersects09(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: RealEqual of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_RealEqual00
static int tolua_Ogre_Ogre_Math_RealEqual00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float a = ((float)  tolua_tonumber(tolua_S,2,0));
  float b = ((float)  tolua_tonumber(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  Ogre::Math::RealEqual(a,b);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'RealEqual'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: RealEqual of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_RealEqual01
static int tolua_Ogre_Ogre_Math_RealEqual01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  float a = ((float)  tolua_tonumber(tolua_S,2,0));
  float b = ((float)  tolua_tonumber(tolua_S,3,0));
  float tolerance = ((float)  tolua_tonumber(tolua_S,4,0));
  {
   bool tolua_ret = (bool)  Ogre::Math::RealEqual(a,b,tolerance);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_Math_RealEqual00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: calculateTangentSpaceVector of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_calculateTangentSpaceVector00
static int tolua_Ogre_Ogre_Math_calculateTangentSpaceVector00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,7,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,8,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,9,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,10,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,11,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Vector3* position1 = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
  const Ogre::Vector3* position2 = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,3,0));
  const Ogre::Vector3* position3 = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,4,0));
  float u1 = ((float)  tolua_tonumber(tolua_S,5,0));
  float v1 = ((float)  tolua_tonumber(tolua_S,6,0));
  float u2 = ((float)  tolua_tonumber(tolua_S,7,0));
  float v2 = ((float)  tolua_tonumber(tolua_S,8,0));
  float u3 = ((float)  tolua_tonumber(tolua_S,9,0));
  float v3 = ((float)  tolua_tonumber(tolua_S,10,0));
  {
   Ogre::Vector3 tolua_ret = (Ogre::Vector3)  Ogre::Math::calculateTangentSpaceVector(*position1,*position2,*position3,u1,v1,u2,v2,u3,v3);
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
 tolua_error(tolua_S,"#ferror in function 'calculateTangentSpaceVector'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: buildReflectionMatrix of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_buildReflectionMatrix00
static int tolua_Ogre_Ogre_Math_buildReflectionMatrix00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Plane",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Plane* p = ((const Ogre::Plane*)  tolua_tousertype(tolua_S,2,0));
  {
   Ogre::Matrix4 tolua_ret = (Ogre::Matrix4)  Ogre::Math::buildReflectionMatrix(*p);
   {
#ifdef __cplusplus
    void* tolua_obj = new Ogre::Matrix4(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Matrix4");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::Matrix4));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Matrix4");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'buildReflectionMatrix'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: calculateFaceNormal of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_calculateFaceNormal00
static int tolua_Ogre_Ogre_Math_calculateFaceNormal00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Vector3* v1 = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
  const Ogre::Vector3* v2 = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,3,0));
  const Ogre::Vector3* v3 = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,4,0));
  {
   Ogre::Vector4 tolua_ret = (Ogre::Vector4)  Ogre::Math::calculateFaceNormal(*v1,*v2,*v3);
   {
#ifdef __cplusplus
    void* tolua_obj = new Ogre::Vector4(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Vector4");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::Vector4));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Vector4");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'calculateFaceNormal'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: calculateBasicFaceNormal of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_calculateBasicFaceNormal00
static int tolua_Ogre_Ogre_Math_calculateBasicFaceNormal00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Vector3* v1 = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
  const Ogre::Vector3* v2 = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,3,0));
  const Ogre::Vector3* v3 = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,4,0));
  {
   Ogre::Vector3 tolua_ret = (Ogre::Vector3)  Ogre::Math::calculateBasicFaceNormal(*v1,*v2,*v3);
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
 tolua_error(tolua_S,"#ferror in function 'calculateBasicFaceNormal'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: calculateFaceNormalWithoutNormalize of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_calculateFaceNormalWithoutNormalize00
static int tolua_Ogre_Ogre_Math_calculateFaceNormalWithoutNormalize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Vector3* v1 = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
  const Ogre::Vector3* v2 = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,3,0));
  const Ogre::Vector3* v3 = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,4,0));
  {
   Ogre::Vector4 tolua_ret = (Ogre::Vector4)  Ogre::Math::calculateFaceNormalWithoutNormalize(*v1,*v2,*v3);
   {
#ifdef __cplusplus
    void* tolua_obj = new Ogre::Vector4(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Vector4");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::Vector4));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Vector4");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'calculateFaceNormalWithoutNormalize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: calculateBasicFaceNormalWithoutNormalize of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_calculateBasicFaceNormalWithoutNormalize00
static int tolua_Ogre_Ogre_Math_calculateBasicFaceNormalWithoutNormalize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Vector3* v1 = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
  const Ogre::Vector3* v2 = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,3,0));
  const Ogre::Vector3* v3 = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,4,0));
  {
   Ogre::Vector3 tolua_ret = (Ogre::Vector3)  Ogre::Math::calculateBasicFaceNormalWithoutNormalize(*v1,*v2,*v3);
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
 tolua_error(tolua_S,"#ferror in function 'calculateBasicFaceNormalWithoutNormalize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: gaussianDistribution of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Math_gaussianDistribution00
static int tolua_Ogre_Ogre_Math_gaussianDistribution00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Math",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float offset = ((float)  tolua_tonumber(tolua_S,3,0.0f));
  float scale = ((float)  tolua_tonumber(tolua_S,4,1.0f));
  {
   float tolua_ret = (float)  Ogre::Math::gaussianDistribution(x,offset,scale);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'gaussianDistribution'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: POS_INFINITY of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_get_Ogre__Math_POS_INFINITY
static int tolua_get_Ogre__Math_POS_INFINITY(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Ogre::Math::POS_INFINITY);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: NEG_INFINITY of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_get_Ogre__Math_NEG_INFINITY
static int tolua_get_Ogre__Math_NEG_INFINITY(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Ogre::Math::NEG_INFINITY);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: PI of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_get_Ogre__Math_PI
static int tolua_get_Ogre__Math_PI(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Ogre::Math::PI);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: TWO_PI of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_get_Ogre__Math_TWO_PI
static int tolua_get_Ogre__Math_TWO_PI(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Ogre::Math::TWO_PI);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: HALF_PI of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_get_Ogre__Math_HALF_PI
static int tolua_get_Ogre__Math_HALF_PI(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Ogre::Math::HALF_PI);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: fDeg2Rad of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_get_Ogre__Math_fDeg2Rad
static int tolua_get_Ogre__Math_fDeg2Rad(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Ogre::Math::fDeg2Rad);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: fRad2Deg of class  Ogre::Math */
#ifndef TOLUA_DISABLE_tolua_get_Ogre__Math_fRad2Deg
static int tolua_get_Ogre__Math_fRad2Deg(lua_State* tolua_S)
{
  tolua_pushnumber(tolua_S,(lua_Number)Ogre::Math::fRad2Deg);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMesh of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity_getMesh00
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
#endif //#ifndef TOLUA_DISABLE

/* method: getSubEntity of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity_getSubEntity00
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
#endif //#ifndef TOLUA_DISABLE

/* method: getSubEntity of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity_getSubEntity01
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
#endif //#ifndef TOLUA_DISABLE

/* method: getNumSubEntities of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity_getNumSubEntities00
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
#endif //#ifndef TOLUA_DISABLE

/* method: clone of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity_clone00
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
#endif //#ifndef TOLUA_DISABLE

/* method: setMaterialName of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity_setMaterialName00
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
#endif //#ifndef TOLUA_DISABLE

/* method: setRenderQueueGroup of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity_setRenderQueueGroup00
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
#endif //#ifndef TOLUA_DISABLE

/* method: getBoundingBox of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity_getBoundingBox00
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
#endif //#ifndef TOLUA_DISABLE

/* method: getChildObjectsBoundingBox of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity_getChildObjectsBoundingBox00
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
#endif //#ifndef TOLUA_DISABLE

/* method: getMovableType of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity_getMovableType00
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
#endif //#ifndef TOLUA_DISABLE

/* method: getAnimationState of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity_getAnimationState00
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
#endif //#ifndef TOLUA_DISABLE

/* method: getAllAnimationStates of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity_getAllAnimationStates00
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
#endif //#ifndef TOLUA_DISABLE

/* method: setDisplaySkeleton of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity_setDisplaySkeleton00
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
#endif //#ifndef TOLUA_DISABLE

/* method: getDisplaySkeleton of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity_getDisplaySkeleton00
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
#endif //#ifndef TOLUA_DISABLE

/* method: getManualLodLevel of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity_getManualLodLevel00
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
#endif //#ifndef TOLUA_DISABLE

/* method: getNumManualLodLevels of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity_getNumManualLodLevels00
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
#endif //#ifndef TOLUA_DISABLE

/* method: setMeshLodBias of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity_setMeshLodBias00
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
#endif //#ifndef TOLUA_DISABLE

/* method: setMaterialLodBias of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity_setMaterialLodBias00
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
#endif //#ifndef TOLUA_DISABLE

/* method: setPolygonModeOverrideable of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity_setPolygonModeOverrideable00
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
#endif //#ifndef TOLUA_DISABLE

/* method: attachObjectToBone of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity_attachObjectToBone00
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
#endif //#ifndef TOLUA_DISABLE

/* method: attachObjectToBone of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity_attachObjectToBone01
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
#endif //#ifndef TOLUA_DISABLE

/* method: attachObjectToBone of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity_attachObjectToBone02
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
#endif //#ifndef TOLUA_DISABLE

/* method: detachObjectFromBone of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity_detachObjectFromBone00
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
#endif //#ifndef TOLUA_DISABLE

/* method: detachObjectFromBone of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity_detachObjectFromBone01
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
#endif //#ifndef TOLUA_DISABLE

/* method: detachAllObjectsFromBone of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity_detachAllObjectsFromBone00
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
#endif //#ifndef TOLUA_DISABLE

/* method: getAttachedObjectIterator of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity_getAttachedObjectIterator00
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
#endif //#ifndef TOLUA_DISABLE

/* method: getBoundingRadius of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity_getBoundingRadius00
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
#endif //#ifndef TOLUA_DISABLE

/* method: getWorldBoundingBox of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity_getWorldBoundingBox00
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
#endif //#ifndef TOLUA_DISABLE

/* method: getWorldBoundingSphere of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity_getWorldBoundingSphere00
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
#endif //#ifndef TOLUA_DISABLE

/* method: hasSkeleton of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity_hasSkeleton00
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
#endif //#ifndef TOLUA_DISABLE

/* method: getSkeleton of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity_getSkeleton00
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
#endif //#ifndef TOLUA_DISABLE

/* method: isHardwareAnimationEnabled of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity_isHardwareAnimationEnabled00
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
#endif //#ifndef TOLUA_DISABLE

/* method: getSoftwareAnimationRequests of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity_getSoftwareAnimationRequests00
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
#endif //#ifndef TOLUA_DISABLE

/* method: getSoftwareAnimationNormalsRequests of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity_getSoftwareAnimationNormalsRequests00
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
#endif //#ifndef TOLUA_DISABLE

/* method: addSoftwareAnimationRequest of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity_addSoftwareAnimationRequest00
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
#endif //#ifndef TOLUA_DISABLE

/* method: removeSoftwareAnimationRequest of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity_removeSoftwareAnimationRequest00
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
#endif //#ifndef TOLUA_DISABLE

/* method: shareSkeletonInstanceWith of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity_shareSkeletonInstanceWith00
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
#endif //#ifndef TOLUA_DISABLE

/* method: hasVertexAnimation of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity_hasVertexAnimation00
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
#endif //#ifndef TOLUA_DISABLE

/* method: stopSharingSkeletonInstance of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity_stopSharingSkeletonInstance00
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
#endif //#ifndef TOLUA_DISABLE

/* method: sharesSkeletonInstance of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity_sharesSkeletonInstance00
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
#endif //#ifndef TOLUA_DISABLE

/* method: getSkeletonInstanceSharingSet of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity_getSkeletonInstanceSharingSet00
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
#endif //#ifndef TOLUA_DISABLE

/* method: refreshAvailableAnimationState of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity_refreshAvailableAnimationState00
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
#endif //#ifndef TOLUA_DISABLE

/* method: _updateAnimation of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity__updateAnimation00
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
#endif //#ifndef TOLUA_DISABLE

/* method: _isAnimated of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity__isAnimated00
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
#endif //#ifndef TOLUA_DISABLE

/* method: _isSkeletonAnimated of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity__isSkeletonAnimated00
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
#endif //#ifndef TOLUA_DISABLE

/* method: getTypeFlags of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity_getTypeFlags00
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
#endif //#ifndef TOLUA_DISABLE

/* method: chooseVertexDataForBinding of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity_chooseVertexDataForBinding00
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
#endif //#ifndef TOLUA_DISABLE

/* method: _getBuffersMarkedForAnimation of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity__getBuffersMarkedForAnimation00
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
#endif //#ifndef TOLUA_DISABLE

/* method: _markBuffersUsedForAnimation of class  Ogre::Entity */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Entity__markBuffersUsedForAnimation00
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
#endif //#ifndef TOLUA_DISABLE

/* method: getName of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_getName00
static int tolua_Ogre_Ogre_Node_getName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Node",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Node* self = (const Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
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

/* method: getParent of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_getParent00
static int tolua_Ogre_Ogre_Node_getParent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Node",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Node* self = (const Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getParent'",NULL);
#endif
  {
   Ogre::Node* tolua_ret = (Ogre::Node*)  self->getParent();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::Node");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getParent'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getOrientation of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_getOrientation00
static int tolua_Ogre_Ogre_Node_getOrientation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Node",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Node* self = (const Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getOrientation'",NULL);
#endif
  {
   const Ogre::Quaternion& tolua_ret = (const Ogre::Quaternion&)  self->getOrientation();
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

/* method: setOrientation of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_setOrientation00
static int tolua_Ogre_Ogre_Node_setOrientation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Node",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Node* self = (Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Quaternion* q = ((const Ogre::Quaternion*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setOrientation'",NULL);
#endif
  {
   self->setOrientation(*q);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setOrientation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setOrientation of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_setOrientation01
static int tolua_Ogre_Ogre_Node_setOrientation01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Node",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ogre::Node* self = (Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
  float w = ((float)  tolua_tonumber(tolua_S,2,0));
  float x = ((float)  tolua_tonumber(tolua_S,3,0));
  float y = ((float)  tolua_tonumber(tolua_S,4,0));
  float z = ((float)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setOrientation'",NULL);
#endif
  {
   self->setOrientation(w,x,y,z);
  }
 }
 return 0;
tolua_lerror:
 return tolua_Ogre_Ogre_Node_setOrientation00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: resetOrientation of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_resetOrientation00
static int tolua_Ogre_Ogre_Node_resetOrientation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Node",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Node* self = (Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'resetOrientation'",NULL);
#endif
  {
   self->resetOrientation();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'resetOrientation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setPosition of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_setPosition00
static int tolua_Ogre_Ogre_Node_setPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Node",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Node* self = (Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector3* pos = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPosition'",NULL);
#endif
  {
   self->setPosition(*pos);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setPosition of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_setPosition01
static int tolua_Ogre_Ogre_Node_setPosition01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Node",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ogre::Node* self = (Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float y = ((float)  tolua_tonumber(tolua_S,3,0));
  float z = ((float)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPosition'",NULL);
#endif
  {
   self->setPosition(x,y,z);
  }
 }
 return 0;
tolua_lerror:
 return tolua_Ogre_Ogre_Node_setPosition00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPosition of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_getPosition00
static int tolua_Ogre_Ogre_Node_getPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Node",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Node* self = (const Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
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

/* method: setScale of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_setScale00
static int tolua_Ogre_Ogre_Node_setScale00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Node",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Node* self = (Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector3* scale = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setScale'",NULL);
#endif
  {
   self->setScale(*scale);
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

/* method: setScale of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_setScale01
static int tolua_Ogre_Ogre_Node_setScale01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Node",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ogre::Node* self = (Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float y = ((float)  tolua_tonumber(tolua_S,3,0));
  float z = ((float)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setScale'",NULL);
#endif
  {
   self->setScale(x,y,z);
  }
 }
 return 0;
tolua_lerror:
 return tolua_Ogre_Ogre_Node_setScale00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getScale of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_getScale00
static int tolua_Ogre_Ogre_Node_getScale00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Node",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Node* self = (const Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getScale'",NULL);
#endif
  {
   const Ogre::Vector3& tolua_ret = (const Ogre::Vector3&)  self->getScale();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Ogre::Vector3");
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

/* method: setInheritOrientation of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_setInheritOrientation00
static int tolua_Ogre_Ogre_Node_setInheritOrientation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Node",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Node* self = (Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
  bool inherit = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setInheritOrientation'",NULL);
#endif
  {
   self->setInheritOrientation(inherit);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setInheritOrientation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getInheritOrientation of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_getInheritOrientation00
static int tolua_Ogre_Ogre_Node_getInheritOrientation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Node",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Node* self = (const Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getInheritOrientation'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->getInheritOrientation();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInheritOrientation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setInheritScale of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_setInheritScale00
static int tolua_Ogre_Ogre_Node_setInheritScale00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Node",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Node* self = (Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
  bool inherit = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setInheritScale'",NULL);
#endif
  {
   self->setInheritScale(inherit);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setInheritScale'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getInheritScale of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_getInheritScale00
static int tolua_Ogre_Ogre_Node_getInheritScale00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Node",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Node* self = (const Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getInheritScale'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->getInheritScale();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInheritScale'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: scale of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_scale00
static int tolua_Ogre_Ogre_Node_scale00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Node",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Node* self = (Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector3* scale = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'scale'",NULL);
#endif
  {
   self->scale(*scale);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'scale'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: scale of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_scale01
static int tolua_Ogre_Ogre_Node_scale01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Node",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ogre::Node* self = (Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float y = ((float)  tolua_tonumber(tolua_S,3,0));
  float z = ((float)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'scale'",NULL);
#endif
  {
   self->scale(x,y,z);
  }
 }
 return 0;
tolua_lerror:
 return tolua_Ogre_Ogre_Node_scale00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: translate of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_translate00
static int tolua_Ogre_Ogre_Node_translate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Node",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Node* self = (Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector3* d = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
  Ogre::Node::TransformSpace relativeTo = ((Ogre::Node::TransformSpace) (int)  tolua_tonumber(tolua_S,3,Ogre::Node::TS_PARENT));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'translate'",NULL);
#endif
  {
   self->translate(*d,relativeTo);
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

/* method: translate of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_translate01
static int tolua_Ogre_Ogre_Node_translate01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Node",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ogre::Node* self = (Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float y = ((float)  tolua_tonumber(tolua_S,3,0));
  float z = ((float)  tolua_tonumber(tolua_S,4,0));
  Ogre::Node::TransformSpace relativeTo = ((Ogre::Node::TransformSpace) (int)  tolua_tonumber(tolua_S,5,Ogre::Node::TS_PARENT));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'translate'",NULL);
#endif
  {
   self->translate(x,y,z,relativeTo);
  }
 }
 return 0;
tolua_lerror:
 return tolua_Ogre_Ogre_Node_translate00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: translate of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_translate02
static int tolua_Ogre_Ogre_Node_translate02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Node",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Matrix3",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ogre::Node* self = (Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Matrix3* axes = ((const Ogre::Matrix3*)  tolua_tousertype(tolua_S,2,0));
  float x = ((float)  tolua_tonumber(tolua_S,3,0));
  float y = ((float)  tolua_tonumber(tolua_S,4,0));
  float z = ((float)  tolua_tonumber(tolua_S,5,0));
  Ogre::Node::TransformSpace relativeTo = ((Ogre::Node::TransformSpace) (int)  tolua_tonumber(tolua_S,6,Ogre::Node::TS_PARENT));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'translate'",NULL);
#endif
  {
   self->translate(*axes,x,y,z,relativeTo);
  }
 }
 return 0;
tolua_lerror:
 return tolua_Ogre_Ogre_Node_translate01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: roll of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_roll00
static int tolua_Ogre_Ogre_Node_roll00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Node",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Radian",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Node* self = (Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Radian* angle = ((const Ogre::Radian*)  tolua_tousertype(tolua_S,2,0));
  Ogre::Node::TransformSpace relativeTo = ((Ogre::Node::TransformSpace) (int)  tolua_tonumber(tolua_S,3,Ogre::Node::TS_LOCAL));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'roll'",NULL);
#endif
  {
   self->roll(*angle,relativeTo);
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

/* method: pitch of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_pitch00
static int tolua_Ogre_Ogre_Node_pitch00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Node",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Radian",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Node* self = (Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Radian* angle = ((const Ogre::Radian*)  tolua_tousertype(tolua_S,2,0));
  Ogre::Node::TransformSpace relativeTo = ((Ogre::Node::TransformSpace) (int)  tolua_tonumber(tolua_S,3,Ogre::Node::TS_LOCAL));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'pitch'",NULL);
#endif
  {
   self->pitch(*angle,relativeTo);
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

/* method: yaw of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_yaw00
static int tolua_Ogre_Ogre_Node_yaw00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Node",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Radian",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Node* self = (Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Radian* angle = ((const Ogre::Radian*)  tolua_tousertype(tolua_S,2,0));
  Ogre::Node::TransformSpace relativeTo = ((Ogre::Node::TransformSpace) (int)  tolua_tonumber(tolua_S,3,Ogre::Node::TS_LOCAL));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'yaw'",NULL);
#endif
  {
   self->yaw(*angle,relativeTo);
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

/* method: rotate of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_rotate00
static int tolua_Ogre_Ogre_Node_rotate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Node",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Ogre::Radian",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Node* self = (Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector3* axis = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
  const Ogre::Radian* angle = ((const Ogre::Radian*)  tolua_tousertype(tolua_S,3,0));
  Ogre::Node::TransformSpace relativeTo = ((Ogre::Node::TransformSpace) (int)  tolua_tonumber(tolua_S,4,Ogre::Node::TS_LOCAL));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'rotate'",NULL);
#endif
  {
   self->rotate(*axis,*angle,relativeTo);
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

/* method: rotate of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_rotate01
static int tolua_Ogre_Ogre_Node_rotate01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Node",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ogre::Node* self = (Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Quaternion* q = ((const Ogre::Quaternion*)  tolua_tousertype(tolua_S,2,0));
  Ogre::Node::TransformSpace relativeTo = ((Ogre::Node::TransformSpace) (int)  tolua_tonumber(tolua_S,3,Ogre::Node::TS_LOCAL));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'rotate'",NULL);
#endif
  {
   self->rotate(*q,relativeTo);
  }
 }
 return 0;
tolua_lerror:
 return tolua_Ogre_Ogre_Node_rotate00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getLocalAxes of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_getLocalAxes00
static int tolua_Ogre_Ogre_Node_getLocalAxes00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Node",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Node* self = (const Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLocalAxes'",NULL);
#endif
  {
   Ogre::Matrix3 tolua_ret = (Ogre::Matrix3)  self->getLocalAxes();
   {
#ifdef __cplusplus
    void* tolua_obj = new Ogre::Matrix3(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Matrix3");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Ogre::Matrix3));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Ogre::Matrix3");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getLocalAxes'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createChild of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_createChild00
static int tolua_Ogre_Ogre_Node_createChild00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Node",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Node* self = (Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createChild'",NULL);
#endif
  {
   Ogre::Node* tolua_ret = (Ogre::Node*)  self->createChild();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::Node");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createChild'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createChild of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_createChild01
static int tolua_Ogre_Ogre_Node_createChild01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Node",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ogre::Node* self = (Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector3* translate = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createChild'",NULL);
#endif
  {
   Ogre::Node* tolua_ret = (Ogre::Node*)  self->createChild(*translate);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::Node");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_Node_createChild00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: createChild of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_createChild02
static int tolua_Ogre_Ogre_Node_createChild02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Node",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ogre::Node* self = (Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector3* translate = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
  const Ogre::Quaternion* rotate = ((const Ogre::Quaternion*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createChild'",NULL);
#endif
  {
   Ogre::Node* tolua_ret = (Ogre::Node*)  self->createChild(*translate,*rotate);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::Node");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_Node_createChild01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: createChild of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_createChild03
static int tolua_Ogre_Ogre_Node_createChild03(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Node",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ogre::Node* self = (Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createChild'",NULL);
#endif
  {
   Ogre::Node* tolua_ret = (Ogre::Node*)  self->createChild(name);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::Node");
   tolua_pushcppstring(tolua_S,(const char*)name);
  }
 }
 return 2;
tolua_lerror:
 return tolua_Ogre_Ogre_Node_createChild02(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: createChild of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_createChild04
static int tolua_Ogre_Ogre_Node_createChild04(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Node",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ogre::Node* self = (Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const Ogre::Vector3* translate = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createChild'",NULL);
#endif
  {
   Ogre::Node* tolua_ret = (Ogre::Node*)  self->createChild(name,*translate);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::Node");
   tolua_pushcppstring(tolua_S,(const char*)name);
  }
 }
 return 2;
tolua_lerror:
 return tolua_Ogre_Ogre_Node_createChild03(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: createChild of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_createChild05
static int tolua_Ogre_Ogre_Node_createChild05(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Node",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ogre::Node* self = (Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const Ogre::Vector3* translate = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,3,0));
  const Ogre::Quaternion* rotate = ((const Ogre::Quaternion*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createChild'",NULL);
#endif
  {
   Ogre::Node* tolua_ret = (Ogre::Node*)  self->createChild(name,*translate,*rotate);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::Node");
   tolua_pushcppstring(tolua_S,(const char*)name);
  }
 }
 return 2;
tolua_lerror:
 return tolua_Ogre_Ogre_Node_createChild04(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: addChild of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_addChild00
static int tolua_Ogre_Ogre_Node_addChild00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Node",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::Node",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Node* self = (Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
  Ogre::Node* child = ((Ogre::Node*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addChild'",NULL);
#endif
  {
   self->addChild(child);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addChild'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: numChildren of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_numChildren00
static int tolua_Ogre_Ogre_Node_numChildren00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Node",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Node* self = (const Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'numChildren'",NULL);
#endif
  {
   unsigned short tolua_ret = (unsigned short)  self->numChildren();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'numChildren'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getChild of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_getChildByIndex00
static int tolua_Ogre_Ogre_Node_getChildByIndex00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Node",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Node* self = (const Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
  unsigned short index = ((unsigned short)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getChild'",NULL);
#endif
  {
   Ogre::Node* tolua_ret = (Ogre::Node*)  self->getChild(index);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::Node");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getChildByIndex'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getChild of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_getChild00
static int tolua_Ogre_Ogre_Node_getChild00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Node",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Node* self = (const Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getChild'",NULL);
#endif
  {
   Ogre::Node* tolua_ret = (Ogre::Node*)  self->getChild(name);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::Node");
   tolua_pushcppstring(tolua_S,(const char*)name);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getChild'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeChild of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_removeChild00
static int tolua_Ogre_Ogre_Node_removeChild00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Node",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Node* self = (Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
  unsigned short index = ((unsigned short)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeChild'",NULL);
#endif
  {
   Ogre::Node* tolua_ret = (Ogre::Node*)  self->removeChild(index);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::Node");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeChild'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeChild of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_removeChild01
static int tolua_Ogre_Ogre_Node_removeChild01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Node",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::Node",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ogre::Node* self = (Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
  Ogre::Node* child = ((Ogre::Node*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeChild'",NULL);
#endif
  {
   Ogre::Node* tolua_ret = (Ogre::Node*)  self->removeChild(child);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::Node");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_Node_removeChild00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeChild of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_removeChild02
static int tolua_Ogre_Ogre_Node_removeChild02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Node",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ogre::Node* self = (Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeChild'",NULL);
#endif
  {
   Ogre::Node* tolua_ret = (Ogre::Node*)  self->removeChild(name);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::Node");
   tolua_pushcppstring(tolua_S,(const char*)name);
  }
 }
 return 2;
tolua_lerror:
 return tolua_Ogre_Ogre_Node_removeChild01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeAllChildren of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_removeAllChildren00
static int tolua_Ogre_Ogre_Node_removeAllChildren00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Node",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Node* self = (Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeAllChildren'",NULL);
#endif
  {
   self->removeAllChildren();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeAllChildren'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: _getDerivedOrientation of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node__getDerivedOrientation00
static int tolua_Ogre_Ogre_Node__getDerivedOrientation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Node",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Node* self = (const Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function '_getDerivedOrientation'",NULL);
#endif
  {
   const Ogre::Quaternion& tolua_ret = (const Ogre::Quaternion&)  self->_getDerivedOrientation();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Ogre::Quaternion");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '_getDerivedOrientation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: _getDerivedPosition of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node__getDerivedPosition00
static int tolua_Ogre_Ogre_Node__getDerivedPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Node",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Node* self = (const Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function '_getDerivedPosition'",NULL);
#endif
  {
   const Ogre::Vector3& tolua_ret = (const Ogre::Vector3&)  self->_getDerivedPosition();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Ogre::Vector3");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '_getDerivedPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: _getDerivedScale of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node__getDerivedScale00
static int tolua_Ogre_Ogre_Node__getDerivedScale00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Node",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Node* self = (const Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function '_getDerivedScale'",NULL);
#endif
  {
   const Ogre::Vector3& tolua_ret = (const Ogre::Vector3&)  self->_getDerivedScale();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Ogre::Vector3");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '_getDerivedScale'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: _getFullTransform of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node__getFullTransform00
static int tolua_Ogre_Ogre_Node__getFullTransform00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Node",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Node* self = (const Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function '_getFullTransform'",NULL);
#endif
  {
   const Ogre::Matrix4& tolua_ret = (const Ogre::Matrix4&)  self->_getFullTransform();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Ogre::Matrix4");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '_getFullTransform'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: _update of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node__update00
static int tolua_Ogre_Ogre_Node__update00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Node",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Node* self = (Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
  bool updateChildren = ((bool)  tolua_toboolean(tolua_S,2,0));
  bool parentHasChanged = ((bool)  tolua_toboolean(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function '_update'",NULL);
#endif
  {
   self->_update(updateChildren,parentHasChanged);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '_update'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMaterial of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_getMaterial00
static int tolua_Ogre_Ogre_Node_getMaterial00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Node",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Node* self = (const Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
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
#endif //#ifndef TOLUA_DISABLE

/* method: getWorldTransforms of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_getWorldTransforms00
static int tolua_Ogre_Ogre_Node_getWorldTransforms00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Node",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::Matrix4",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Node* self = (const Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
  Ogre::Matrix4* xform = ((Ogre::Matrix4*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getWorldTransforms'",NULL);
#endif
  {
   self->getWorldTransforms(xform);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getWorldTransforms'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: _getDerivedOrientation of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node__getDerivedOrientation01
static int tolua_Ogre_Ogre_Node__getDerivedOrientation01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Node",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Node* self = (const Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function '_getDerivedOrientation'",NULL);
#endif
  {
   const Ogre::Quaternion& tolua_ret = (const Ogre::Quaternion&)  self->_getDerivedOrientation();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Ogre::Quaternion");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_Node__getDerivedOrientation00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: _getDerivedPosition of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node__getDerivedPosition01
static int tolua_Ogre_Ogre_Node__getDerivedPosition01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Node",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Node* self = (const Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function '_getDerivedPosition'",NULL);
#endif
  {
   const Ogre::Vector3& tolua_ret = (const Ogre::Vector3&)  self->_getDerivedPosition();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Ogre::Vector3");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_Node__getDerivedPosition00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: setInitialState of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_setInitialState00
static int tolua_Ogre_Ogre_Node_setInitialState00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Node",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Node* self = (Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setInitialState'",NULL);
#endif
  {
   self->setInitialState();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setInitialState'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: resetToInitialState of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_resetToInitialState00
static int tolua_Ogre_Ogre_Node_resetToInitialState00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Node",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Node* self = (Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'resetToInitialState'",NULL);
#endif
  {
   self->resetToInitialState();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'resetToInitialState'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getInitialPosition of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_getInitialPosition00
static int tolua_Ogre_Ogre_Node_getInitialPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Node",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Node* self = (const Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getInitialPosition'",NULL);
#endif
  {
   const Ogre::Vector3& tolua_ret = (const Ogre::Vector3&)  self->getInitialPosition();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Ogre::Vector3");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInitialPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getInitialOrientation of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_getInitialOrientation00
static int tolua_Ogre_Ogre_Node_getInitialOrientation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Node",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Node* self = (const Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getInitialOrientation'",NULL);
#endif
  {
   const Ogre::Quaternion& tolua_ret = (const Ogre::Quaternion&)  self->getInitialOrientation();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Ogre::Quaternion");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInitialOrientation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getInitialScale of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_getInitialScale00
static int tolua_Ogre_Ogre_Node_getInitialScale00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Node",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Node* self = (const Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getInitialScale'",NULL);
#endif
  {
   const Ogre::Vector3& tolua_ret = (const Ogre::Vector3&)  self->getInitialScale();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Ogre::Vector3");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInitialScale'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSquaredViewDepth of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_getSquaredViewDepth00
static int tolua_Ogre_Ogre_Node_getSquaredViewDepth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Node",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Camera",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Node* self = (const Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Camera* cam = ((const Ogre::Camera*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSquaredViewDepth'",NULL);
#endif
  {
   float tolua_ret = (float)  self->getSquaredViewDepth(cam);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSquaredViewDepth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: needUpdate of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_needUpdate00
static int tolua_Ogre_Ogre_Node_needUpdate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Node",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Node* self = (Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
  bool forceParentUpdate = ((bool)  tolua_toboolean(tolua_S,2,false));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'needUpdate'",NULL);
#endif
  {
   self->needUpdate(forceParentUpdate);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'needUpdate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: requestUpdate of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_requestUpdate00
static int tolua_Ogre_Ogre_Node_requestUpdate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Node",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::Node",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Node* self = (Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
  Ogre::Node* child = ((Ogre::Node*)  tolua_tousertype(tolua_S,2,0));
  bool forceParentUpdate = ((bool)  tolua_toboolean(tolua_S,3,false));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'requestUpdate'",NULL);
#endif
  {
   self->requestUpdate(child,forceParentUpdate);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'requestUpdate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: cancelUpdate of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_cancelUpdate00
static int tolua_Ogre_Ogre_Node_cancelUpdate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Node",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::Node",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Node* self = (Ogre::Node*)  tolua_tousertype(tolua_S,1,0);
  Ogre::Node* child = ((Ogre::Node*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cancelUpdate'",NULL);
#endif
  {
   self->cancelUpdate(child);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cancelUpdate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: queueNeedUpdate of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_queueNeedUpdate00
static int tolua_Ogre_Ogre_Node_queueNeedUpdate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Node",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::Node",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Node* n = ((Ogre::Node*)  tolua_tousertype(tolua_S,2,0));
  {
   Ogre::Node::queueNeedUpdate(n);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'queueNeedUpdate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: processQueuedUpdates of class  Ogre::Node */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Node_processQueuedUpdates00
static int tolua_Ogre_Ogre_Node_processQueuedUpdates00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Node",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Ogre::Node::processQueuedUpdates();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'processQueuedUpdates'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: attachObject of class  Ogre::SceneNode */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SceneNode_attachObject00
static int tolua_Ogre_Ogre_SceneNode_attachObject00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::MovableObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::SceneNode* self = (Ogre::SceneNode*)  tolua_tousertype(tolua_S,1,0);
  Ogre::MovableObject* obj = ((Ogre::MovableObject*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'attachObject'",NULL);
#endif
  {
   self->attachObject(obj);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'attachObject'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: numAttachedObjects of class  Ogre::SceneNode */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SceneNode_numAttachedObjects00
static int tolua_Ogre_Ogre_SceneNode_numAttachedObjects00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::SceneNode* self = (const Ogre::SceneNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'numAttachedObjects'",NULL);
#endif
  {
   unsigned short tolua_ret = (unsigned short)  self->numAttachedObjects();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'numAttachedObjects'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAttachedObject of class  Ogre::SceneNode */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SceneNode_getAttachedObject00
static int tolua_Ogre_Ogre_SceneNode_getAttachedObject00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::SceneNode* self = (Ogre::SceneNode*)  tolua_tousertype(tolua_S,1,0);
  unsigned short index = ((unsigned short)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAttachedObject'",NULL);
#endif
  {
   Ogre::MovableObject* tolua_ret = (Ogre::MovableObject*)  self->getAttachedObject(index);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::MovableObject");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAttachedObject'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAttachedObject of class  Ogre::SceneNode */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SceneNode_getAttachedObject01
static int tolua_Ogre_Ogre_SceneNode_getAttachedObject01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ogre::SceneNode* self = (Ogre::SceneNode*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAttachedObject'",NULL);
#endif
  {
   Ogre::MovableObject* tolua_ret = (Ogre::MovableObject*)  self->getAttachedObject(name);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::MovableObject");
   tolua_pushcppstring(tolua_S,(const char*)name);
  }
 }
 return 2;
tolua_lerror:
 return tolua_Ogre_Ogre_SceneNode_getAttachedObject00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: detachObject of class  Ogre::SceneNode */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SceneNode_detachObject00
static int tolua_Ogre_Ogre_SceneNode_detachObject00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::SceneNode* self = (Ogre::SceneNode*)  tolua_tousertype(tolua_S,1,0);
  unsigned short index = ((unsigned short)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'detachObject'",NULL);
#endif
  {
   Ogre::MovableObject* tolua_ret = (Ogre::MovableObject*)  self->detachObject(index);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::MovableObject");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'detachObject'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: detachObject of class  Ogre::SceneNode */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SceneNode_detachObject01
static int tolua_Ogre_Ogre_SceneNode_detachObject01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::MovableObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ogre::SceneNode* self = (Ogre::SceneNode*)  tolua_tousertype(tolua_S,1,0);
  Ogre::MovableObject* obj = ((Ogre::MovableObject*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'detachObject'",NULL);
#endif
  {
   self->detachObject(obj);
  }
 }
 return 0;
tolua_lerror:
 return tolua_Ogre_Ogre_SceneNode_detachObject00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: detachObject of class  Ogre::SceneNode */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SceneNode_detachObject02
static int tolua_Ogre_Ogre_SceneNode_detachObject02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ogre::SceneNode* self = (Ogre::SceneNode*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'detachObject'",NULL);
#endif
  {
   Ogre::MovableObject* tolua_ret = (Ogre::MovableObject*)  self->detachObject(name);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::MovableObject");
   tolua_pushcppstring(tolua_S,(const char*)name);
  }
 }
 return 2;
tolua_lerror:
 return tolua_Ogre_Ogre_SceneNode_detachObject01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: detachAllObjects of class  Ogre::SceneNode */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SceneNode_detachAllObjects00
static int tolua_Ogre_Ogre_SceneNode_detachAllObjects00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::SceneNode* self = (Ogre::SceneNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'detachAllObjects'",NULL);
#endif
  {
   self->detachAllObjects();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'detachAllObjects'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isInSceneGraph of class  Ogre::SceneNode */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SceneNode_isInSceneGraph00
static int tolua_Ogre_Ogre_SceneNode_isInSceneGraph00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::SceneNode* self = (const Ogre::SceneNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isInSceneGraph'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isInSceneGraph();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isInSceneGraph'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: _notifyRootNode of class  Ogre::SceneNode */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SceneNode__notifyRootNode00
static int tolua_Ogre_Ogre_SceneNode__notifyRootNode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::SceneNode* self = (Ogre::SceneNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function '_notifyRootNode'",NULL);
#endif
  {
   self->_notifyRootNode();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '_notifyRootNode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: _update of class  Ogre::SceneNode */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SceneNode__update00
static int tolua_Ogre_Ogre_SceneNode__update00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::SceneNode* self = (Ogre::SceneNode*)  tolua_tousertype(tolua_S,1,0);
  bool updateChildren = ((bool)  tolua_toboolean(tolua_S,2,0));
  bool parentHasChanged = ((bool)  tolua_toboolean(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function '_update'",NULL);
#endif
  {
   self->_update(updateChildren,parentHasChanged);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '_update'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: _updateBounds of class  Ogre::SceneNode */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SceneNode__updateBounds00
static int tolua_Ogre_Ogre_SceneNode__updateBounds00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::SceneNode* self = (Ogre::SceneNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function '_updateBounds'",NULL);
#endif
  {
   self->_updateBounds();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '_updateBounds'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: _getWorldAABB of class  Ogre::SceneNode */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SceneNode__getWorldAABB00
static int tolua_Ogre_Ogre_SceneNode__getWorldAABB00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::SceneNode* self = (const Ogre::SceneNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function '_getWorldAABB'",NULL);
#endif
  {
   const Ogre::AxisAlignedBox& tolua_ret = (const Ogre::AxisAlignedBox&)  self->_getWorldAABB();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Ogre::AxisAlignedBox");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '_getWorldAABB'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getCreator of class  Ogre::SceneNode */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SceneNode_getCreator00
static int tolua_Ogre_Ogre_SceneNode_getCreator00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::SceneNode* self = (const Ogre::SceneNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCreator'",NULL);
#endif
  {
   Ogre::SceneManager* tolua_ret = (Ogre::SceneManager*)  self->getCreator();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::SceneManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getCreator'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeAndDestroyChild of class  Ogre::SceneNode */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SceneNode_removeAndDestroyChild00
static int tolua_Ogre_Ogre_SceneNode_removeAndDestroyChild00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::SceneNode* self = (Ogre::SceneNode*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeAndDestroyChild'",NULL);
#endif
  {
   self->removeAndDestroyChild(name);
   tolua_pushcppstring(tolua_S,(const char*)name);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeAndDestroyChild'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeAndDestroyChild of class  Ogre::SceneNode */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SceneNode_removeAndDestroyChild01
static int tolua_Ogre_Ogre_SceneNode_removeAndDestroyChild01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ogre::SceneNode* self = (Ogre::SceneNode*)  tolua_tousertype(tolua_S,1,0);
  unsigned short index = ((unsigned short)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeAndDestroyChild'",NULL);
#endif
  {
   self->removeAndDestroyChild(index);
  }
 }
 return 0;
tolua_lerror:
 return tolua_Ogre_Ogre_SceneNode_removeAndDestroyChild00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeAndDestroyAllChildren of class  Ogre::SceneNode */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SceneNode_removeAndDestroyAllChildren00
static int tolua_Ogre_Ogre_SceneNode_removeAndDestroyAllChildren00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::SceneNode* self = (Ogre::SceneNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeAndDestroyAllChildren'",NULL);
#endif
  {
   self->removeAndDestroyAllChildren();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeAndDestroyAllChildren'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: showBoundingBox of class  Ogre::SceneNode */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SceneNode_showBoundingBox00
static int tolua_Ogre_Ogre_SceneNode_showBoundingBox00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::SceneNode* self = (Ogre::SceneNode*)  tolua_tousertype(tolua_S,1,0);
  bool bShow = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'showBoundingBox'",NULL);
#endif
  {
   self->showBoundingBox(bShow);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'showBoundingBox'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getShowBoundingBox of class  Ogre::SceneNode */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SceneNode_getShowBoundingBox00
static int tolua_Ogre_Ogre_SceneNode_getShowBoundingBox00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::SceneNode* self = (const Ogre::SceneNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getShowBoundingBox'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->getShowBoundingBox();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getShowBoundingBox'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createChildSceneNode of class  Ogre::SceneNode */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SceneNode_createChildSceneNode00
static int tolua_Ogre_Ogre_SceneNode_createChildSceneNode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::SceneNode* self = (Ogre::SceneNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createChildSceneNode'",NULL);
#endif
  {
   Ogre::SceneNode* tolua_ret = (Ogre::SceneNode*)  self->createChildSceneNode();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::SceneNode");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createChildSceneNode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createChildSceneNode of class  Ogre::SceneNode */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SceneNode_createChildSceneNode01
static int tolua_Ogre_Ogre_SceneNode_createChildSceneNode01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ogre::SceneNode* self = (Ogre::SceneNode*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector3* translate = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createChildSceneNode'",NULL);
#endif
  {
   Ogre::SceneNode* tolua_ret = (Ogre::SceneNode*)  self->createChildSceneNode(*translate);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::SceneNode");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_SceneNode_createChildSceneNode00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: createChildSceneNode of class  Ogre::SceneNode */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SceneNode_createChildSceneNode02
static int tolua_Ogre_Ogre_SceneNode_createChildSceneNode02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ogre::SceneNode* self = (Ogre::SceneNode*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector3* translate = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
  const Ogre::Quaternion* rotate = ((const Ogre::Quaternion*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createChildSceneNode'",NULL);
#endif
  {
   Ogre::SceneNode* tolua_ret = (Ogre::SceneNode*)  self->createChildSceneNode(*translate,*rotate);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::SceneNode");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_SceneNode_createChildSceneNode01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: createChildSceneNode of class  Ogre::SceneNode */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SceneNode_createChildSceneNode03
static int tolua_Ogre_Ogre_SceneNode_createChildSceneNode03(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ogre::SceneNode* self = (Ogre::SceneNode*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createChildSceneNode'",NULL);
#endif
  {
   Ogre::SceneNode* tolua_ret = (Ogre::SceneNode*)  self->createChildSceneNode(name);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::SceneNode");
   tolua_pushcppstring(tolua_S,(const char*)name);
  }
 }
 return 2;
tolua_lerror:
 return tolua_Ogre_Ogre_SceneNode_createChildSceneNode02(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: createChildSceneNode of class  Ogre::SceneNode */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SceneNode_createChildSceneNode04
static int tolua_Ogre_Ogre_SceneNode_createChildSceneNode04(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ogre::SceneNode* self = (Ogre::SceneNode*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const Ogre::Vector3* translate = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createChildSceneNode'",NULL);
#endif
  {
   Ogre::SceneNode* tolua_ret = (Ogre::SceneNode*)  self->createChildSceneNode(name,*translate);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::SceneNode");
   tolua_pushcppstring(tolua_S,(const char*)name);
  }
 }
 return 2;
tolua_lerror:
 return tolua_Ogre_Ogre_SceneNode_createChildSceneNode03(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: createChildSceneNode of class  Ogre::SceneNode */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SceneNode_createChildSceneNode05
static int tolua_Ogre_Ogre_SceneNode_createChildSceneNode05(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"const Ogre::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ogre::SceneNode* self = (Ogre::SceneNode*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const Ogre::Vector3* translate = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,3,0));
  const Ogre::Quaternion* rotate = ((const Ogre::Quaternion*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createChildSceneNode'",NULL);
#endif
  {
   Ogre::SceneNode* tolua_ret = (Ogre::SceneNode*)  self->createChildSceneNode(name,*translate,*rotate);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::SceneNode");
   tolua_pushcppstring(tolua_S,(const char*)name);
  }
 }
 return 2;
tolua_lerror:
 return tolua_Ogre_Ogre_SceneNode_createChildSceneNode04(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: setFixedYawAxis of class  Ogre::SceneNode */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SceneNode_setFixedYawAxis00
static int tolua_Ogre_Ogre_SceneNode_setFixedYawAxis00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::SceneNode* self = (Ogre::SceneNode*)  tolua_tousertype(tolua_S,1,0);
  bool useFixed = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setFixedYawAxis'",NULL);
#endif
  {
   self->setFixedYawAxis(useFixed);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setFixedYawAxis'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setFixedYawAxis of class  Ogre::SceneNode */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SceneNode_setFixedYawAxis01
static int tolua_Ogre_Ogre_SceneNode_setFixedYawAxis01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ogre::SceneNode* self = (Ogre::SceneNode*)  tolua_tousertype(tolua_S,1,0);
  bool useFixed = ((bool)  tolua_toboolean(tolua_S,2,0));
  const Ogre::Vector3* fixedAxis = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setFixedYawAxis'",NULL);
#endif
  {
   self->setFixedYawAxis(useFixed,*fixedAxis);
  }
 }
 return 0;
tolua_lerror:
 return tolua_Ogre_Ogre_SceneNode_setFixedYawAxis00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: yaw of class  Ogre::SceneNode */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SceneNode_yaw00
static int tolua_Ogre_Ogre_SceneNode_yaw00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Radian",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::SceneNode* self = (Ogre::SceneNode*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Radian* angle = ((const Ogre::Radian*)  tolua_tousertype(tolua_S,2,0));
  Ogre::Node::TransformSpace relativeTo = ((Ogre::Node::TransformSpace) (int)  tolua_tonumber(tolua_S,3,Ogre::Node::TS_LOCAL));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'yaw'",NULL);
#endif
  {
   self->yaw(*angle,relativeTo);
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

/* method: setDirection of class  Ogre::SceneNode */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SceneNode_setDirection00
static int tolua_Ogre_Ogre_SceneNode_setDirection00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::SceneNode* self = (Ogre::SceneNode*)  tolua_tousertype(tolua_S,1,0);
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float y = ((float)  tolua_tonumber(tolua_S,3,0));
  float z = ((float)  tolua_tonumber(tolua_S,4,0));
  Ogre::Node::TransformSpace relativeTo = ((Ogre::Node::TransformSpace) (int)  tolua_tonumber(tolua_S,5,Ogre::Node::TS_LOCAL));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setDirection'",NULL);
#endif
  {
   self->setDirection(x,y,z,relativeTo);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setDirection'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setDirection of class  Ogre::SceneNode */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SceneNode_setDirection01
static int tolua_Ogre_Ogre_SceneNode_setDirection01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,6,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ogre::SceneNode* self = (Ogre::SceneNode*)  tolua_tousertype(tolua_S,1,0);
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float y = ((float)  tolua_tonumber(tolua_S,3,0));
  float z = ((float)  tolua_tonumber(tolua_S,4,0));
  Ogre::Node::TransformSpace relativeTo = ((Ogre::Node::TransformSpace) (int)  tolua_tonumber(tolua_S,5,0));
  const Ogre::Vector3* localDirectionVector = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setDirection'",NULL);
#endif
  {
   self->setDirection(x,y,z,relativeTo,*localDirectionVector);
  }
 }
 return 0;
tolua_lerror:
 return tolua_Ogre_Ogre_SceneNode_setDirection00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: setDirection of class  Ogre::SceneNode */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SceneNode_setDirection02
static int tolua_Ogre_Ogre_SceneNode_setDirection02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ogre::SceneNode* self = (Ogre::SceneNode*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector3* vec = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
  Ogre::Node::TransformSpace relativeTo = ((Ogre::Node::TransformSpace) (int)  tolua_tonumber(tolua_S,3,Ogre::Node::TS_LOCAL));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setDirection'",NULL);
#endif
  {
   self->setDirection(*vec,relativeTo);
  }
 }
 return 0;
tolua_lerror:
 return tolua_Ogre_Ogre_SceneNode_setDirection01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: setDirection of class  Ogre::SceneNode */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SceneNode_setDirection03
static int tolua_Ogre_Ogre_SceneNode_setDirection03(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ogre::SceneNode* self = (Ogre::SceneNode*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector3* vec = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
  Ogre::Node::TransformSpace relativeTo = ((Ogre::Node::TransformSpace) (int)  tolua_tonumber(tolua_S,3,0));
  const Ogre::Vector3* localDirectionVector = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setDirection'",NULL);
#endif
  {
   self->setDirection(*vec,relativeTo,*localDirectionVector);
  }
 }
 return 0;
tolua_lerror:
 return tolua_Ogre_Ogre_SceneNode_setDirection02(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: lookAt of class  Ogre::SceneNode */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SceneNode_lookAt00
static int tolua_Ogre_Ogre_SceneNode_lookAt00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::SceneNode* self = (Ogre::SceneNode*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector3* targetPoint = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
  Ogre::Node::TransformSpace relativeTo = ((Ogre::Node::TransformSpace) (int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'lookAt'",NULL);
#endif
  {
   self->lookAt(*targetPoint,relativeTo);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'lookAt'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: lookAt of class  Ogre::SceneNode */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SceneNode_lookAt01
static int tolua_Ogre_Ogre_SceneNode_lookAt01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ogre::SceneNode* self = (Ogre::SceneNode*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector3* targetPoint = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
  Ogre::Node::TransformSpace relativeTo = ((Ogre::Node::TransformSpace) (int)  tolua_tonumber(tolua_S,3,0));
  const Ogre::Vector3* localDirectionVector = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'lookAt'",NULL);
#endif
  {
   self->lookAt(*targetPoint,relativeTo,*localDirectionVector);
  }
 }
 return 0;
tolua_lerror:
 return tolua_Ogre_Ogre_SceneNode_lookAt00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: setAutoTracking of class  Ogre::SceneNode */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SceneNode_setAutoTracking00
static int tolua_Ogre_Ogre_SceneNode_setAutoTracking00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"Ogre::SceneNode",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::SceneNode* self = (Ogre::SceneNode*)  tolua_tousertype(tolua_S,1,0);
  bool enabled = ((bool)  tolua_toboolean(tolua_S,2,0));
  Ogre::SceneNode* target = ((Ogre::SceneNode*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAutoTracking'",NULL);
#endif
  {
   self->setAutoTracking(enabled,target);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAutoTracking'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setAutoTracking of class  Ogre::SceneNode */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SceneNode_setAutoTracking01
static int tolua_Ogre_Ogre_SceneNode_setAutoTracking01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ogre::SceneNode* self = (Ogre::SceneNode*)  tolua_tousertype(tolua_S,1,0);
  bool enabled = ((bool)  tolua_toboolean(tolua_S,2,0));
  Ogre::SceneNode* target = ((Ogre::SceneNode*)  tolua_tousertype(tolua_S,3,0));
  const Ogre::Vector3* localDirectionVector = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAutoTracking'",NULL);
#endif
  {
   self->setAutoTracking(enabled,target,*localDirectionVector);
  }
 }
 return 0;
tolua_lerror:
 return tolua_Ogre_Ogre_SceneNode_setAutoTracking00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: setAutoTracking of class  Ogre::SceneNode */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SceneNode_setAutoTracking02
static int tolua_Ogre_Ogre_SceneNode_setAutoTracking02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ogre::SceneNode* self = (Ogre::SceneNode*)  tolua_tousertype(tolua_S,1,0);
  bool enabled = ((bool)  tolua_toboolean(tolua_S,2,0));
  Ogre::SceneNode* target = ((Ogre::SceneNode*)  tolua_tousertype(tolua_S,3,0));
  const Ogre::Vector3* localDirectionVector = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,4,0));
  const Ogre::Vector3* offset = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAutoTracking'",NULL);
#endif
  {
   self->setAutoTracking(enabled,target,*localDirectionVector,*offset);
  }
 }
 return 0;
tolua_lerror:
 return tolua_Ogre_Ogre_SceneNode_setAutoTracking01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAutoTrackTarget of class  Ogre::SceneNode */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SceneNode_getAutoTrackTarget00
static int tolua_Ogre_Ogre_SceneNode_getAutoTrackTarget00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::SceneNode* self = (Ogre::SceneNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAutoTrackTarget'",NULL);
#endif
  {
   Ogre::SceneNode* tolua_ret = (Ogre::SceneNode*)  self->getAutoTrackTarget();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::SceneNode");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAutoTrackTarget'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAutoTrackOffset of class  Ogre::SceneNode */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SceneNode_getAutoTrackOffset00
static int tolua_Ogre_Ogre_SceneNode_getAutoTrackOffset00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::SceneNode* self = (Ogre::SceneNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAutoTrackOffset'",NULL);
#endif
  {
   const Ogre::Vector3& tolua_ret = (const Ogre::Vector3&)  self->getAutoTrackOffset();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Ogre::Vector3");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAutoTrackOffset'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAutoTrackLocalDirection of class  Ogre::SceneNode */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SceneNode_getAutoTrackLocalDirection00
static int tolua_Ogre_Ogre_SceneNode_getAutoTrackLocalDirection00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::SceneNode* self = (Ogre::SceneNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAutoTrackLocalDirection'",NULL);
#endif
  {
   const Ogre::Vector3& tolua_ret = (const Ogre::Vector3&)  self->getAutoTrackLocalDirection();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Ogre::Vector3");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAutoTrackLocalDirection'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getParentSceneNode of class  Ogre::SceneNode */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SceneNode_getParentSceneNode00
static int tolua_Ogre_Ogre_SceneNode_getParentSceneNode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::SceneNode* self = (const Ogre::SceneNode*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getParentSceneNode'",NULL);
#endif
  {
   Ogre::SceneNode* tolua_ret = (Ogre::SceneNode*)  self->getParentSceneNode();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::SceneNode");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getParentSceneNode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setVisible of class  Ogre::SceneNode */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SceneNode_setVisible00
static int tolua_Ogre_Ogre_SceneNode_setVisible00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::SceneNode* self = (Ogre::SceneNode*)  tolua_tousertype(tolua_S,1,0);
  bool visible = ((bool)  tolua_toboolean(tolua_S,2,0));
  bool cascade = ((bool)  tolua_toboolean(tolua_S,3,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setVisible'",NULL);
#endif
  {
   self->setVisible(visible,cascade);
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

/* method: flipVisibility of class  Ogre::SceneNode */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_SceneNode_flipVisibility00
static int tolua_Ogre_Ogre_SceneNode_flipVisibility00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::SceneNode",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::SceneNode* self = (Ogre::SceneNode*)  tolua_tousertype(tolua_S,1,0);
  bool cascade = ((bool)  tolua_toboolean(tolua_S,2,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'flipVisibility'",NULL);
#endif
  {
   self->flipVisibility(cascade);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flipVisibility'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get of class  Ogre::TexturePtr */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_TexturePtr_get00
static int tolua_Ogre_Ogre_TexturePtr_get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::TexturePtr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::TexturePtr* self = (Ogre::TexturePtr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get'",NULL);
#endif
  {
   Ogre::Texture* tolua_ret = (Ogre::Texture*)  self->get();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::Texture");
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

/* method: isNull of class  Ogre::TexturePtr */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_TexturePtr_isNull00
static int tolua_Ogre_Ogre_TexturePtr_isNull00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::TexturePtr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::TexturePtr* self = (Ogre::TexturePtr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isNull'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isNull();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isNull'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setTextureType of class  Ogre::Texture */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Texture_setTextureType00
static int tolua_Ogre_Ogre_Texture_setTextureType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Texture",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Texture* self = (Ogre::Texture*)  tolua_tousertype(tolua_S,1,0);
  Ogre::TextureType ttype = ((Ogre::TextureType) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setTextureType'",NULL);
#endif
  {
   self->setTextureType(ttype);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setTextureType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTextureType of class  Ogre::Texture */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Texture_getTextureType00
static int tolua_Ogre_Ogre_Texture_getTextureType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Texture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Texture* self = (const Ogre::Texture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTextureType'",NULL);
#endif
  {
   Ogre::TextureType tolua_ret = (Ogre::TextureType)  self->getTextureType();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTextureType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getNumMipmaps of class  Ogre::Texture */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Texture_getNumMipmaps00
static int tolua_Ogre_Ogre_Texture_getNumMipmaps00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Texture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Texture* self = (const Ogre::Texture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getNumMipmaps'",NULL);
#endif
  {
   size_t tolua_ret = (size_t)  self->getNumMipmaps();
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
 tolua_error(tolua_S,"#ferror in function 'getNumMipmaps'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setNumMipmaps of class  Ogre::Texture */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Texture_setNumMipmaps00
static int tolua_Ogre_Ogre_Texture_setNumMipmaps00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Texture",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"size_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Texture* self = (Ogre::Texture*)  tolua_tousertype(tolua_S,1,0);
  size_t num = *((size_t*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setNumMipmaps'",NULL);
#endif
  {
   self->setNumMipmaps(num);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setNumMipmaps'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMipmapsHardwareGenerated of class  Ogre::Texture */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Texture_getMipmapsHardwareGenerated00
static int tolua_Ogre_Ogre_Texture_getMipmapsHardwareGenerated00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Texture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Texture* self = (const Ogre::Texture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMipmapsHardwareGenerated'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->getMipmapsHardwareGenerated();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMipmapsHardwareGenerated'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getGamma of class  Ogre::Texture */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Texture_getGamma00
static int tolua_Ogre_Ogre_Texture_getGamma00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Texture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Texture* self = (const Ogre::Texture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getGamma'",NULL);
#endif
  {
   float tolua_ret = (float)  self->getGamma();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getGamma'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setGamma of class  Ogre::Texture */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Texture_setGamma00
static int tolua_Ogre_Ogre_Texture_setGamma00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Texture",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Texture* self = (Ogre::Texture*)  tolua_tousertype(tolua_S,1,0);
  float g = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setGamma'",NULL);
#endif
  {
   self->setGamma(g);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setGamma'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getHeight of class  Ogre::Texture */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Texture_getHeight00
static int tolua_Ogre_Ogre_Texture_getHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Texture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Texture* self = (const Ogre::Texture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getHeight'",NULL);
#endif
  {
   size_t tolua_ret = (size_t)  self->getHeight();
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
 tolua_error(tolua_S,"#ferror in function 'getHeight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getWidth of class  Ogre::Texture */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Texture_getWidth00
static int tolua_Ogre_Ogre_Texture_getWidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Texture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Texture* self = (const Ogre::Texture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getWidth'",NULL);
#endif
  {
   size_t tolua_ret = (size_t)  self->getWidth();
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
 tolua_error(tolua_S,"#ferror in function 'getWidth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getDepth of class  Ogre::Texture */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Texture_getDepth00
static int tolua_Ogre_Ogre_Texture_getDepth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Texture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Texture* self = (const Ogre::Texture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getDepth'",NULL);
#endif
  {
   size_t tolua_ret = (size_t)  self->getDepth();
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
 tolua_error(tolua_S,"#ferror in function 'getDepth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSrcHeight of class  Ogre::Texture */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Texture_getSrcHeight00
static int tolua_Ogre_Ogre_Texture_getSrcHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Texture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Texture* self = (const Ogre::Texture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSrcHeight'",NULL);
#endif
  {
   size_t tolua_ret = (size_t)  self->getSrcHeight();
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
 tolua_error(tolua_S,"#ferror in function 'getSrcHeight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSrcWidth of class  Ogre::Texture */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Texture_getSrcWidth00
static int tolua_Ogre_Ogre_Texture_getSrcWidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Texture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Texture* self = (const Ogre::Texture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSrcWidth'",NULL);
#endif
  {
   size_t tolua_ret = (size_t)  self->getSrcWidth();
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
 tolua_error(tolua_S,"#ferror in function 'getSrcWidth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSrcDepth of class  Ogre::Texture */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Texture_getSrcDepth00
static int tolua_Ogre_Ogre_Texture_getSrcDepth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Texture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Texture* self = (const Ogre::Texture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSrcDepth'",NULL);
#endif
  {
   size_t tolua_ret = (size_t)  self->getSrcDepth();
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
 tolua_error(tolua_S,"#ferror in function 'getSrcDepth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setHeight of class  Ogre::Texture */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Texture_setHeight00
static int tolua_Ogre_Ogre_Texture_setHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Texture",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"size_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Texture* self = (Ogre::Texture*)  tolua_tousertype(tolua_S,1,0);
  size_t h = *((size_t*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setHeight'",NULL);
#endif
  {
   self->setHeight(h);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setHeight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setWidth of class  Ogre::Texture */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Texture_setWidth00
static int tolua_Ogre_Ogre_Texture_setWidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Texture",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"size_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Texture* self = (Ogre::Texture*)  tolua_tousertype(tolua_S,1,0);
  size_t w = *((size_t*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setWidth'",NULL);
#endif
  {
   self->setWidth(w);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setWidth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setDepth of class  Ogre::Texture */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Texture_setDepth00
static int tolua_Ogre_Ogre_Texture_setDepth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Texture",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"size_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Texture* self = (Ogre::Texture*)  tolua_tousertype(tolua_S,1,0);
  size_t d = *((size_t*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setDepth'",NULL);
#endif
  {
   self->setDepth(d);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setDepth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getUsage of class  Ogre::Texture */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Texture_getUsage00
static int tolua_Ogre_Ogre_Texture_getUsage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Texture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Texture* self = (const Ogre::Texture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getUsage'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getUsage();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getUsage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setUsage of class  Ogre::Texture */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Texture_setUsage00
static int tolua_Ogre_Ogre_Texture_setUsage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Texture",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Texture* self = (Ogre::Texture*)  tolua_tousertype(tolua_S,1,0);
  int u = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setUsage'",NULL);
#endif
  {
   self->setUsage(u);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setUsage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: copyToTexture of class  Ogre::Texture */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Texture_copyToTexture00
static int tolua_Ogre_Ogre_Texture_copyToTexture00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Texture",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::TexturePtr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Texture* self = (Ogre::Texture*)  tolua_tousertype(tolua_S,1,0);
  Ogre::TexturePtr* target = ((Ogre::TexturePtr*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'copyToTexture'",NULL);
#endif
  {
   self->copyToTexture(*target);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'copyToTexture'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: loadImage of class  Ogre::Texture */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Texture_loadImage00
static int tolua_Ogre_Ogre_Texture_loadImage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Texture",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Image",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Texture* self = (Ogre::Texture*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Image* img = ((const Ogre::Image*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'loadImage'",NULL);
#endif
  {
   self->loadImage(*img);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'loadImage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: hasAlpha of class  Ogre::Texture */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Texture_hasAlpha00
static int tolua_Ogre_Ogre_Texture_hasAlpha00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Texture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Texture* self = (const Ogre::Texture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hasAlpha'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->hasAlpha();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'hasAlpha'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setDesiredIntegerBitDepth of class  Ogre::Texture */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Texture_setDesiredIntegerBitDepth00
static int tolua_Ogre_Ogre_Texture_setDesiredIntegerBitDepth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Texture",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Texture* self = (Ogre::Texture*)  tolua_tousertype(tolua_S,1,0);
  unsigned short bits = ((unsigned short)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setDesiredIntegerBitDepth'",NULL);
#endif
  {
   self->setDesiredIntegerBitDepth(bits);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setDesiredIntegerBitDepth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getDesiredIntegerBitDepth of class  Ogre::Texture */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Texture_getDesiredIntegerBitDepth00
static int tolua_Ogre_Ogre_Texture_getDesiredIntegerBitDepth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Texture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Texture* self = (const Ogre::Texture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getDesiredIntegerBitDepth'",NULL);
#endif
  {
   unsigned short tolua_ret = (unsigned short)  self->getDesiredIntegerBitDepth();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getDesiredIntegerBitDepth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setDesiredFloatBitDepth of class  Ogre::Texture */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Texture_setDesiredFloatBitDepth00
static int tolua_Ogre_Ogre_Texture_setDesiredFloatBitDepth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Texture",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Texture* self = (Ogre::Texture*)  tolua_tousertype(tolua_S,1,0);
  unsigned short bits = ((unsigned short)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setDesiredFloatBitDepth'",NULL);
#endif
  {
   self->setDesiredFloatBitDepth(bits);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setDesiredFloatBitDepth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getDesiredFloatBitDepth of class  Ogre::Texture */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Texture_getDesiredFloatBitDepth00
static int tolua_Ogre_Ogre_Texture_getDesiredFloatBitDepth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Texture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Texture* self = (const Ogre::Texture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getDesiredFloatBitDepth'",NULL);
#endif
  {
   unsigned short tolua_ret = (unsigned short)  self->getDesiredFloatBitDepth();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getDesiredFloatBitDepth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setDesiredBitDepths of class  Ogre::Texture */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Texture_setDesiredBitDepths00
static int tolua_Ogre_Ogre_Texture_setDesiredBitDepths00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Texture",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Texture* self = (Ogre::Texture*)  tolua_tousertype(tolua_S,1,0);
  unsigned short integerBits = ((unsigned short)  tolua_tonumber(tolua_S,2,0));
  unsigned short floatBits = ((unsigned short)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setDesiredBitDepths'",NULL);
#endif
  {
   self->setDesiredBitDepths(integerBits,floatBits);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setDesiredBitDepths'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setTreatLuminanceAsAlpha of class  Ogre::Texture */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Texture_setTreatLuminanceAsAlpha00
static int tolua_Ogre_Ogre_Texture_setTreatLuminanceAsAlpha00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Texture",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Texture* self = (Ogre::Texture*)  tolua_tousertype(tolua_S,1,0);
  bool asAlpha = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setTreatLuminanceAsAlpha'",NULL);
#endif
  {
   self->setTreatLuminanceAsAlpha(asAlpha);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setTreatLuminanceAsAlpha'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTreatLuminanceAsAlpha of class  Ogre::Texture */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Texture_getTreatLuminanceAsAlpha00
static int tolua_Ogre_Ogre_Texture_getTreatLuminanceAsAlpha00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Texture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Texture* self = (const Ogre::Texture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTreatLuminanceAsAlpha'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->getTreatLuminanceAsAlpha();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTreatLuminanceAsAlpha'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getNumFaces of class  Ogre::Texture */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Texture_getNumFaces00
static int tolua_Ogre_Ogre_Texture_getNumFaces00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Texture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Texture* self = (const Ogre::Texture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getNumFaces'",NULL);
#endif
  {
   size_t tolua_ret = (size_t)  self->getNumFaces();
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
 tolua_error(tolua_S,"#ferror in function 'getNumFaces'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setDefaultNumMipmaps of class  Ogre::TextureManager */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_TextureManager_setDefaultNumMipmaps00
static int tolua_Ogre_Ogre_TextureManager_setDefaultNumMipmaps00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::TextureManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"size_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::TextureManager* self = (Ogre::TextureManager*)  tolua_tousertype(tolua_S,1,0);
  size_t num = *((size_t*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setDefaultNumMipmaps'",NULL);
#endif
  {
   self->setDefaultNumMipmaps(num);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setDefaultNumMipmaps'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getDefaultNumMipmaps of class  Ogre::TextureManager */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_TextureManager_getDefaultNumMipmaps00
static int tolua_Ogre_Ogre_TextureManager_getDefaultNumMipmaps00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::TextureManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::TextureManager* self = (Ogre::TextureManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getDefaultNumMipmaps'",NULL);
#endif
  {
   size_t tolua_ret = (size_t)  self->getDefaultNumMipmaps();
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
 tolua_error(tolua_S,"#ferror in function 'getDefaultNumMipmaps'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSingleton of class  Ogre::TextureManager */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_TextureManager_getSingleton00
static int tolua_Ogre_Ogre_TextureManager_getSingleton00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::TextureManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Ogre::TextureManager& tolua_ret = (Ogre::TextureManager&)  Ogre::TextureManager::getSingleton();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"Ogre::TextureManager");
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

/* method: new of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_new00
static int tolua_Ogre_Ogre_AxisAlignedBox_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Ogre::AxisAlignedBox* tolua_ret = (Ogre::AxisAlignedBox*)  new Ogre::AxisAlignedBox();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::AxisAlignedBox");
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

/* method: new_local of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_new00_local
static int tolua_Ogre_Ogre_AxisAlignedBox_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Ogre::AxisAlignedBox* tolua_ret = (Ogre::AxisAlignedBox*)  new Ogre::AxisAlignedBox();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"Ogre::AxisAlignedBox");
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

/* method: new of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_new01
static int tolua_Ogre_Ogre_AxisAlignedBox_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::AxisAlignedBox* rkBox = ((const Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,2,0));
  {
   Ogre::AxisAlignedBox* tolua_ret = (Ogre::AxisAlignedBox*)  new Ogre::AxisAlignedBox(*rkBox);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::AxisAlignedBox");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_AxisAlignedBox_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_new01_local
static int tolua_Ogre_Ogre_AxisAlignedBox_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::AxisAlignedBox* rkBox = ((const Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,2,0));
  {
   Ogre::AxisAlignedBox* tolua_ret = (Ogre::AxisAlignedBox*)  new Ogre::AxisAlignedBox(*rkBox);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"Ogre::AxisAlignedBox");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_AxisAlignedBox_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_new02
static int tolua_Ogre_Ogre_AxisAlignedBox_new02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Vector3* min = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
  const Ogre::Vector3* max = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,3,0));
  {
   Ogre::AxisAlignedBox* tolua_ret = (Ogre::AxisAlignedBox*)  new Ogre::AxisAlignedBox(*min,*max);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::AxisAlignedBox");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_AxisAlignedBox_new01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_new02_local
static int tolua_Ogre_Ogre_AxisAlignedBox_new02_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::Vector3* min = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
  const Ogre::Vector3* max = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,3,0));
  {
   Ogre::AxisAlignedBox* tolua_ret = (Ogre::AxisAlignedBox*)  new Ogre::AxisAlignedBox(*min,*max);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"Ogre::AxisAlignedBox");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_AxisAlignedBox_new01_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_new03
static int tolua_Ogre_Ogre_AxisAlignedBox_new03(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,7,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  float mx = ((float)  tolua_tonumber(tolua_S,2,0));
  float my = ((float)  tolua_tonumber(tolua_S,3,0));
  float mz = ((float)  tolua_tonumber(tolua_S,4,0));
  float Mx = ((float)  tolua_tonumber(tolua_S,5,0));
  float My = ((float)  tolua_tonumber(tolua_S,6,0));
  float Mz = ((float)  tolua_tonumber(tolua_S,7,0));
  {
   Ogre::AxisAlignedBox* tolua_ret = (Ogre::AxisAlignedBox*)  new Ogre::AxisAlignedBox(mx,my,mz,Mx,My,Mz);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::AxisAlignedBox");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_AxisAlignedBox_new02(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_new03_local
static int tolua_Ogre_Ogre_AxisAlignedBox_new03_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,7,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  float mx = ((float)  tolua_tonumber(tolua_S,2,0));
  float my = ((float)  tolua_tonumber(tolua_S,3,0));
  float mz = ((float)  tolua_tonumber(tolua_S,4,0));
  float Mx = ((float)  tolua_tonumber(tolua_S,5,0));
  float My = ((float)  tolua_tonumber(tolua_S,6,0));
  float Mz = ((float)  tolua_tonumber(tolua_S,7,0));
  {
   Ogre::AxisAlignedBox* tolua_ret = (Ogre::AxisAlignedBox*)  new Ogre::AxisAlignedBox(mx,my,mz,Mx,My,Mz);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"Ogre::AxisAlignedBox");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_AxisAlignedBox_new02_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_delete00
static int tolua_Ogre_Ogre_AxisAlignedBox_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::AxisAlignedBox* self = (Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,1,0);
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

/* method: getMinimum of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_getMinimum00
static int tolua_Ogre_Ogre_AxisAlignedBox_getMinimum00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::AxisAlignedBox* self = (const Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMinimum'",NULL);
#endif
  {
   const Ogre::Vector3& tolua_ret = (const Ogre::Vector3&)  self->getMinimum();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Ogre::Vector3");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMinimum'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMinimum of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_getMinimum01
static int tolua_Ogre_Ogre_AxisAlignedBox_getMinimum01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ogre::AxisAlignedBox* self = (Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMinimum'",NULL);
#endif
  {
   Ogre::Vector3& tolua_ret = (Ogre::Vector3&)  self->getMinimum();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"Ogre::Vector3");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_AxisAlignedBox_getMinimum00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMaximum of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_getMaximum00
static int tolua_Ogre_Ogre_AxisAlignedBox_getMaximum00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::AxisAlignedBox* self = (const Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMaximum'",NULL);
#endif
  {
   const Ogre::Vector3& tolua_ret = (const Ogre::Vector3&)  self->getMaximum();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Ogre::Vector3");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMaximum'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMaximum of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_getMaximum01
static int tolua_Ogre_Ogre_AxisAlignedBox_getMaximum01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ogre::AxisAlignedBox* self = (Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMaximum'",NULL);
#endif
  {
   Ogre::Vector3& tolua_ret = (Ogre::Vector3&)  self->getMaximum();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"Ogre::Vector3");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_AxisAlignedBox_getMaximum00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: setMinimum of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_setMinimum00
static int tolua_Ogre_Ogre_AxisAlignedBox_setMinimum00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::AxisAlignedBox* self = (Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector3* vec = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMinimum'",NULL);
#endif
  {
   self->setMinimum(*vec);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setMinimum'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setMinimum of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_setMinimum01
static int tolua_Ogre_Ogre_AxisAlignedBox_setMinimum01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ogre::AxisAlignedBox* self = (Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,1,0);
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float y = ((float)  tolua_tonumber(tolua_S,3,0));
  float z = ((float)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMinimum'",NULL);
#endif
  {
   self->setMinimum(x,y,z);
  }
 }
 return 0;
tolua_lerror:
 return tolua_Ogre_Ogre_AxisAlignedBox_setMinimum00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: setMinimumX of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_setMinimumX00
static int tolua_Ogre_Ogre_AxisAlignedBox_setMinimumX00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::AxisAlignedBox* self = (Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,1,0);
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMinimumX'",NULL);
#endif
  {
   self->setMinimumX(x);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setMinimumX'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setMinimumY of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_setMinimumY00
static int tolua_Ogre_Ogre_AxisAlignedBox_setMinimumY00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::AxisAlignedBox* self = (Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,1,0);
  float y = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMinimumY'",NULL);
#endif
  {
   self->setMinimumY(y);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setMinimumY'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setMinimumZ of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_setMinimumZ00
static int tolua_Ogre_Ogre_AxisAlignedBox_setMinimumZ00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::AxisAlignedBox* self = (Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,1,0);
  float z = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMinimumZ'",NULL);
#endif
  {
   self->setMinimumZ(z);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setMinimumZ'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setMaximum of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_setMaximum00
static int tolua_Ogre_Ogre_AxisAlignedBox_setMaximum00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::AxisAlignedBox* self = (Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector3* vec = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMaximum'",NULL);
#endif
  {
   self->setMaximum(*vec);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setMaximum'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setMaximum of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_setMaximum01
static int tolua_Ogre_Ogre_AxisAlignedBox_setMaximum01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ogre::AxisAlignedBox* self = (Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,1,0);
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float y = ((float)  tolua_tonumber(tolua_S,3,0));
  float z = ((float)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMaximum'",NULL);
#endif
  {
   self->setMaximum(x,y,z);
  }
 }
 return 0;
tolua_lerror:
 return tolua_Ogre_Ogre_AxisAlignedBox_setMaximum00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: setMaximumX of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_setMaximumX00
static int tolua_Ogre_Ogre_AxisAlignedBox_setMaximumX00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::AxisAlignedBox* self = (Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,1,0);
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMaximumX'",NULL);
#endif
  {
   self->setMaximumX(x);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setMaximumX'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setMaximumY of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_setMaximumY00
static int tolua_Ogre_Ogre_AxisAlignedBox_setMaximumY00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::AxisAlignedBox* self = (Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,1,0);
  float y = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMaximumY'",NULL);
#endif
  {
   self->setMaximumY(y);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setMaximumY'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setMaximumZ of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_setMaximumZ00
static int tolua_Ogre_Ogre_AxisAlignedBox_setMaximumZ00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::AxisAlignedBox* self = (Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,1,0);
  float z = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMaximumZ'",NULL);
#endif
  {
   self->setMaximumZ(z);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setMaximumZ'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setExtents of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_setExtents00
static int tolua_Ogre_Ogre_AxisAlignedBox_setExtents00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::AxisAlignedBox* self = (Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector3* min = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
  const Ogre::Vector3* max = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setExtents'",NULL);
#endif
  {
   self->setExtents(*min,*max);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setExtents'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setExtents of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_setExtents01
static int tolua_Ogre_Ogre_AxisAlignedBox_setExtents01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,7,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ogre::AxisAlignedBox* self = (Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,1,0);
  float mx = ((float)  tolua_tonumber(tolua_S,2,0));
  float my = ((float)  tolua_tonumber(tolua_S,3,0));
  float mz = ((float)  tolua_tonumber(tolua_S,4,0));
  float Mx = ((float)  tolua_tonumber(tolua_S,5,0));
  float My = ((float)  tolua_tonumber(tolua_S,6,0));
  float Mz = ((float)  tolua_tonumber(tolua_S,7,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setExtents'",NULL);
#endif
  {
   self->setExtents(mx,my,mz,Mx,My,Mz);
  }
 }
 return 0;
tolua_lerror:
 return tolua_Ogre_Ogre_AxisAlignedBox_setExtents00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getCorner of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_getCorner00
static int tolua_Ogre_Ogre_AxisAlignedBox_getCorner00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::AxisAlignedBox* self = (const Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,1,0);
  Ogre::AxisAlignedBox::CornerEnum cornerToGet = ((Ogre::AxisAlignedBox::CornerEnum) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCorner'",NULL);
#endif
  {
   Ogre::Vector3 tolua_ret = (Ogre::Vector3)  self->getCorner(cornerToGet);
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
 tolua_error(tolua_S,"#ferror in function 'getCorner'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: merge of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_merge00
static int tolua_Ogre_Ogre_AxisAlignedBox_merge00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::AxisAlignedBox* self = (Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::AxisAlignedBox* rhs = ((const Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'merge'",NULL);
#endif
  {
   self->merge(*rhs);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'merge'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: merge of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_merge01
static int tolua_Ogre_Ogre_AxisAlignedBox_merge01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ogre::AxisAlignedBox* self = (Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector3* point = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'merge'",NULL);
#endif
  {
   self->merge(*point);
  }
 }
 return 0;
tolua_lerror:
 return tolua_Ogre_Ogre_AxisAlignedBox_merge00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: transform of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_transform00
static int tolua_Ogre_Ogre_AxisAlignedBox_transform00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Matrix4",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::AxisAlignedBox* self = (Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Matrix4* matrix = ((const Ogre::Matrix4*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'transform'",NULL);
#endif
  {
   self->transform(*matrix);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'transform'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: transformAffine of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_transformAffine00
static int tolua_Ogre_Ogre_AxisAlignedBox_transformAffine00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Matrix4",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::AxisAlignedBox* self = (Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Matrix4* m = ((const Ogre::Matrix4*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'transformAffine'",NULL);
#endif
  {
   self->transformAffine(*m);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'transformAffine'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setNull of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_setNull00
static int tolua_Ogre_Ogre_AxisAlignedBox_setNull00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::AxisAlignedBox* self = (Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setNull'",NULL);
#endif
  {
   self->setNull();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setNull'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isNull of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_isNull00
static int tolua_Ogre_Ogre_AxisAlignedBox_isNull00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::AxisAlignedBox* self = (const Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isNull'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isNull();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isNull'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isFinite of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_isFinite00
static int tolua_Ogre_Ogre_AxisAlignedBox_isFinite00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::AxisAlignedBox* self = (const Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isFinite'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isFinite();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isFinite'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setInfinite of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_setInfinite00
static int tolua_Ogre_Ogre_AxisAlignedBox_setInfinite00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::AxisAlignedBox* self = (Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setInfinite'",NULL);
#endif
  {
   self->setInfinite();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setInfinite'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isInfinite of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_isInfinite00
static int tolua_Ogre_Ogre_AxisAlignedBox_isInfinite00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::AxisAlignedBox* self = (const Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isInfinite'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isInfinite();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isInfinite'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: intersects of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_intersects00
static int tolua_Ogre_Ogre_AxisAlignedBox_intersects00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::AxisAlignedBox* self = (const Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::AxisAlignedBox* b2 = ((const Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'intersects'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->intersects(*b2);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'intersects'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: intersection of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_intersection00
static int tolua_Ogre_Ogre_AxisAlignedBox_intersection00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::AxisAlignedBox* self = (const Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::AxisAlignedBox* b2 = ((const Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'intersection'",NULL);
#endif
  {
   Ogre::AxisAlignedBox tolua_ret = (Ogre::AxisAlignedBox)  self->intersection(*b2);
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
 tolua_error(tolua_S,"#ferror in function 'intersection'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: volume of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_volume00
static int tolua_Ogre_Ogre_AxisAlignedBox_volume00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::AxisAlignedBox* self = (const Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'volume'",NULL);
#endif
  {
   float tolua_ret = (float)  self->volume();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'volume'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: scale of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_scale00
static int tolua_Ogre_Ogre_AxisAlignedBox_scale00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::AxisAlignedBox* self = (Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector3* s = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'scale'",NULL);
#endif
  {
   self->scale(*s);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'scale'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: intersects of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_intersects01
static int tolua_Ogre_Ogre_AxisAlignedBox_intersects01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Sphere",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::AxisAlignedBox* self = (const Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Sphere* s = ((const Ogre::Sphere*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'intersects'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->intersects(*s);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_AxisAlignedBox_intersects00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: intersects of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_intersects02
static int tolua_Ogre_Ogre_AxisAlignedBox_intersects02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Plane",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::AxisAlignedBox* self = (const Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Plane* p = ((const Ogre::Plane*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'intersects'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->intersects(*p);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_AxisAlignedBox_intersects01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: intersects of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_intersects03
static int tolua_Ogre_Ogre_AxisAlignedBox_intersects03(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::AxisAlignedBox* self = (const Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector3* v = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'intersects'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->intersects(*v);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_AxisAlignedBox_intersects02(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getCenter of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_getCenter00
static int tolua_Ogre_Ogre_AxisAlignedBox_getCenter00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::AxisAlignedBox* self = (const Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCenter'",NULL);
#endif
  {
   Ogre::Vector3 tolua_ret = (Ogre::Vector3)  self->getCenter();
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
 tolua_error(tolua_S,"#ferror in function 'getCenter'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSize of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_getSize00
static int tolua_Ogre_Ogre_AxisAlignedBox_getSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::AxisAlignedBox* self = (const Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSize'",NULL);
#endif
  {
   Ogre::Vector3 tolua_ret = (Ogre::Vector3)  self->getSize();
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
 tolua_error(tolua_S,"#ferror in function 'getSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getHalfSize of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_getHalfSize00
static int tolua_Ogre_Ogre_AxisAlignedBox_getHalfSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::AxisAlignedBox* self = (const Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getHalfSize'",NULL);
#endif
  {
   Ogre::Vector3 tolua_ret = (Ogre::Vector3)  self->getHalfSize();
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
 tolua_error(tolua_S,"#ferror in function 'getHalfSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: contains of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_contains00
static int tolua_Ogre_Ogre_AxisAlignedBox_contains00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::Vector3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::AxisAlignedBox* self = (const Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::Vector3* v = ((const Ogre::Vector3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'contains'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->contains(*v);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'contains'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: contains of class  Ogre::AxisAlignedBox */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_AxisAlignedBox_contains01
static int tolua_Ogre_Ogre_AxisAlignedBox_contains01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::AxisAlignedBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::AxisAlignedBox* self = (const Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::AxisAlignedBox* other = ((const Ogre::AxisAlignedBox*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'contains'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->contains(*other);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_Ogre_Ogre_AxisAlignedBox_contains00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* get function: ZERO of class  Ogre::ColourValue */
#ifndef TOLUA_DISABLE_tolua_get_Ogre__ColourValue_ZERO
static int tolua_get_Ogre__ColourValue_ZERO(lua_State* tolua_S)
{
  tolua_pushusertype(tolua_S,(void*)&Ogre::ColourValue::ZERO,"const Ogre::ColourValue");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Black of class  Ogre::ColourValue */
#ifndef TOLUA_DISABLE_tolua_get_Ogre__ColourValue_Black
static int tolua_get_Ogre__ColourValue_Black(lua_State* tolua_S)
{
  tolua_pushusertype(tolua_S,(void*)&Ogre::ColourValue::Black,"const Ogre::ColourValue");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: White of class  Ogre::ColourValue */
#ifndef TOLUA_DISABLE_tolua_get_Ogre__ColourValue_White
static int tolua_get_Ogre__ColourValue_White(lua_State* tolua_S)
{
  tolua_pushusertype(tolua_S,(void*)&Ogre::ColourValue::White,"const Ogre::ColourValue");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Red of class  Ogre::ColourValue */
#ifndef TOLUA_DISABLE_tolua_get_Ogre__ColourValue_Red
static int tolua_get_Ogre__ColourValue_Red(lua_State* tolua_S)
{
  tolua_pushusertype(tolua_S,(void*)&Ogre::ColourValue::Red,"const Ogre::ColourValue");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Green of class  Ogre::ColourValue */
#ifndef TOLUA_DISABLE_tolua_get_Ogre__ColourValue_Green
static int tolua_get_Ogre__ColourValue_Green(lua_State* tolua_S)
{
  tolua_pushusertype(tolua_S,(void*)&Ogre::ColourValue::Green,"const Ogre::ColourValue");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Blue of class  Ogre::ColourValue */
#ifndef TOLUA_DISABLE_tolua_get_Ogre__ColourValue_Blue
static int tolua_get_Ogre__ColourValue_Blue(lua_State* tolua_S)
{
  tolua_pushusertype(tolua_S,(void*)&Ogre::ColourValue::Blue,"const Ogre::ColourValue");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Ogre::ColourValue */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_ColourValue_new00
static int tolua_Ogre_Ogre_ColourValue_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::ColourValue",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float red = ((float)  tolua_tonumber(tolua_S,2,1.0f));
  float green = ((float)  tolua_tonumber(tolua_S,3,1.0f));
  float blue = ((float)  tolua_tonumber(tolua_S,4,1.0f));
  float alpha = ((float)  tolua_tonumber(tolua_S,5,1.0f));
  {
   Ogre::ColourValue* tolua_ret = (Ogre::ColourValue*)  new Ogre::ColourValue(red,green,blue,alpha);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::ColourValue");
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

/* method: new_local of class  Ogre::ColourValue */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_ColourValue_new00_local
static int tolua_Ogre_Ogre_ColourValue_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::ColourValue",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float red = ((float)  tolua_tonumber(tolua_S,2,1.0f));
  float green = ((float)  tolua_tonumber(tolua_S,3,1.0f));
  float blue = ((float)  tolua_tonumber(tolua_S,4,1.0f));
  float alpha = ((float)  tolua_tonumber(tolua_S,5,1.0f));
  {
   Ogre::ColourValue* tolua_ret = (Ogre::ColourValue*)  new Ogre::ColourValue(red,green,blue,alpha);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"Ogre::ColourValue");
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

/* get function: r of class  Ogre::ColourValue */
#ifndef TOLUA_DISABLE_tolua_get_Ogre__ColourValue_r
static int tolua_get_Ogre__ColourValue_r(lua_State* tolua_S)
{
  Ogre::ColourValue* self = (Ogre::ColourValue*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'r'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->r);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: r of class  Ogre::ColourValue */
#ifndef TOLUA_DISABLE_tolua_set_Ogre__ColourValue_r
static int tolua_set_Ogre__ColourValue_r(lua_State* tolua_S)
{
  Ogre::ColourValue* self = (Ogre::ColourValue*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'r'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->r = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: g of class  Ogre::ColourValue */
#ifndef TOLUA_DISABLE_tolua_get_Ogre__ColourValue_g
static int tolua_get_Ogre__ColourValue_g(lua_State* tolua_S)
{
  Ogre::ColourValue* self = (Ogre::ColourValue*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'g'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->g);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: g of class  Ogre::ColourValue */
#ifndef TOLUA_DISABLE_tolua_set_Ogre__ColourValue_g
static int tolua_set_Ogre__ColourValue_g(lua_State* tolua_S)
{
  Ogre::ColourValue* self = (Ogre::ColourValue*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'g'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->g = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: b of class  Ogre::ColourValue */
#ifndef TOLUA_DISABLE_tolua_get_Ogre__ColourValue_b
static int tolua_get_Ogre__ColourValue_b(lua_State* tolua_S)
{
  Ogre::ColourValue* self = (Ogre::ColourValue*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'b'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->b);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: b of class  Ogre::ColourValue */
#ifndef TOLUA_DISABLE_tolua_set_Ogre__ColourValue_b
static int tolua_set_Ogre__ColourValue_b(lua_State* tolua_S)
{
  Ogre::ColourValue* self = (Ogre::ColourValue*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'b'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->b = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: a of class  Ogre::ColourValue */
#ifndef TOLUA_DISABLE_tolua_get_Ogre__ColourValue_a
static int tolua_get_Ogre__ColourValue_a(lua_State* tolua_S)
{
  Ogre::ColourValue* self = (Ogre::ColourValue*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'a'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->a);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: a of class  Ogre::ColourValue */
#ifndef TOLUA_DISABLE_tolua_set_Ogre__ColourValue_a
static int tolua_set_Ogre__ColourValue_a(lua_State* tolua_S)
{
  Ogre::ColourValue* self = (Ogre::ColourValue*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'a'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->a = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAsRGBA of class  Ogre::ColourValue */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_ColourValue_getAsRGBA00
static int tolua_Ogre_Ogre_ColourValue_getAsRGBA00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::ColourValue",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::ColourValue* self = (const Ogre::ColourValue*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAsRGBA'",NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->getAsRGBA();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAsRGBA'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAsARGB of class  Ogre::ColourValue */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_ColourValue_getAsARGB00
static int tolua_Ogre_Ogre_ColourValue_getAsARGB00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::ColourValue",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::ColourValue* self = (const Ogre::ColourValue*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAsARGB'",NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->getAsARGB();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAsARGB'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAsBGRA of class  Ogre::ColourValue */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_ColourValue_getAsBGRA00
static int tolua_Ogre_Ogre_ColourValue_getAsBGRA00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::ColourValue",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::ColourValue* self = (const Ogre::ColourValue*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAsBGRA'",NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->getAsBGRA();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAsBGRA'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAsABGR of class  Ogre::ColourValue */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_ColourValue_getAsABGR00
static int tolua_Ogre_Ogre_ColourValue_getAsABGR00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::ColourValue",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::ColourValue* self = (const Ogre::ColourValue*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAsABGR'",NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->getAsABGR();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAsABGR'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setAsRGBA of class  Ogre::ColourValue */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_ColourValue_setAsRGBA00
static int tolua_Ogre_Ogre_ColourValue_setAsRGBA00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::ColourValue",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::ColourValue* self = (Ogre::ColourValue*)  tolua_tousertype(tolua_S,1,0);
  unsigned const int val = ((unsigned const int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAsRGBA'",NULL);
#endif
  {
   self->setAsRGBA(val);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAsRGBA'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setAsARGB of class  Ogre::ColourValue */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_ColourValue_setAsARGB00
static int tolua_Ogre_Ogre_ColourValue_setAsARGB00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::ColourValue",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::ColourValue* self = (Ogre::ColourValue*)  tolua_tousertype(tolua_S,1,0);
  unsigned const int val = ((unsigned const int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAsARGB'",NULL);
#endif
  {
   self->setAsARGB(val);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAsARGB'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setAsBGRA of class  Ogre::ColourValue */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_ColourValue_setAsBGRA00
static int tolua_Ogre_Ogre_ColourValue_setAsBGRA00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::ColourValue",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::ColourValue* self = (Ogre::ColourValue*)  tolua_tousertype(tolua_S,1,0);
  unsigned const int val = ((unsigned const int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAsBGRA'",NULL);
#endif
  {
   self->setAsBGRA(val);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAsBGRA'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setAsABGR of class  Ogre::ColourValue */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_ColourValue_setAsABGR00
static int tolua_Ogre_Ogre_ColourValue_setAsABGR00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::ColourValue",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::ColourValue* self = (Ogre::ColourValue*)  tolua_tousertype(tolua_S,1,0);
  unsigned const int val = ((unsigned const int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAsABGR'",NULL);
#endif
  {
   self->setAsABGR(val);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAsABGR'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: saturate of class  Ogre::ColourValue */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_ColourValue_saturate00
static int tolua_Ogre_Ogre_ColourValue_saturate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::ColourValue",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::ColourValue* self = (Ogre::ColourValue*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'saturate'",NULL);
#endif
  {
   self->saturate();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'saturate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: saturateCopy of class  Ogre::ColourValue */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_ColourValue_saturateCopy00
static int tolua_Ogre_Ogre_ColourValue_saturateCopy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::ColourValue",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::ColourValue* self = (const Ogre::ColourValue*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'saturateCopy'",NULL);
#endif
  {
   Ogre::ColourValue tolua_ret = (Ogre::ColourValue)  self->saturateCopy();
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
 tolua_error(tolua_S,"#ferror in function 'saturateCopy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator+ of class  Ogre::ColourValue */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_ColourValue__add00
static int tolua_Ogre_Ogre_ColourValue__add00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::ColourValue",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::ColourValue",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::ColourValue* self = (const Ogre::ColourValue*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::ColourValue* rkVector = ((const Ogre::ColourValue*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator+'",NULL);
#endif
  {
   Ogre::ColourValue tolua_ret = (Ogre::ColourValue)  self->operator+(*rkVector);
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
 tolua_error(tolua_S,"#ferror in function '.add'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator- of class  Ogre::ColourValue */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_ColourValue__sub00
static int tolua_Ogre_Ogre_ColourValue__sub00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::ColourValue",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::ColourValue",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::ColourValue* self = (const Ogre::ColourValue*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::ColourValue* rkVector = ((const Ogre::ColourValue*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator-'",NULL);
#endif
  {
   Ogre::ColourValue tolua_ret = (Ogre::ColourValue)  self->operator-(*rkVector);
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
 tolua_error(tolua_S,"#ferror in function '.sub'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator/ of class  Ogre::ColourValue */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_ColourValue__div00
static int tolua_Ogre_Ogre_ColourValue__div00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::ColourValue",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::ColourValue",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::ColourValue* self = (const Ogre::ColourValue*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::ColourValue* rhs = ((const Ogre::ColourValue*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator/'",NULL);
#endif
  {
   Ogre::ColourValue tolua_ret = (Ogre::ColourValue)  self->operator/(*rhs);
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
 tolua_error(tolua_S,"#ferror in function '.div'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator/ of class  Ogre::ColourValue */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_ColourValue__div01
static int tolua_Ogre_Ogre_ColourValue__div01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::ColourValue",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ogre::ColourValue* self = (const Ogre::ColourValue*)  tolua_tousertype(tolua_S,1,0);
  const float fScalar = ((const float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator/'",NULL);
#endif
  {
   Ogre::ColourValue tolua_ret = (Ogre::ColourValue)  self->operator/(fScalar);
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
tolua_lerror:
 return tolua_Ogre_Ogre_ColourValue__div00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: setHSB of class  Ogre::ColourValue */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_ColourValue_setHSB00
static int tolua_Ogre_Ogre_ColourValue_setHSB00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::ColourValue",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::ColourValue* self = (Ogre::ColourValue*)  tolua_tousertype(tolua_S,1,0);
  float hue = ((float)  tolua_tonumber(tolua_S,2,0));
  float saturation = ((float)  tolua_tonumber(tolua_S,3,0));
  float brightness = ((float)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setHSB'",NULL);
#endif
  {
   self->setHSB(hue,saturation,brightness);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setHSB'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isSyntaxSupported of class  Ogre::GpuProgramManager */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_GpuProgramManager_isSyntaxSupported00
static int tolua_Ogre_Ogre_GpuProgramManager_isSyntaxSupported00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::GpuProgramManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::GpuProgramManager* self = (const Ogre::GpuProgramManager*)  tolua_tousertype(tolua_S,1,0);
  const std::string syntaxCode = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isSyntaxSupported'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isSyntaxSupported(syntaxCode);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)syntaxCode);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isSyntaxSupported'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getByName of class  Ogre::GpuProgramManager */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_GpuProgramManager_getByName00
static int tolua_Ogre_Ogre_GpuProgramManager_getByName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::GpuProgramManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::GpuProgramManager* self = (Ogre::GpuProgramManager*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  bool preferHighLevelPrograms = ((bool)  tolua_toboolean(tolua_S,3,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getByName'",NULL);
#endif
  {
   Ogre::ResourcePtr tolua_ret = (Ogre::ResourcePtr)  self->getByName(name,preferHighLevelPrograms);
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
#endif //#ifndef TOLUA_DISABLE

/* method: getSingleton of class  Ogre::GpuProgramManager */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_GpuProgramManager_getSingleton00
static int tolua_Ogre_Ogre_GpuProgramManager_getSingleton00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::GpuProgramManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Ogre::GpuProgramManager& tolua_ret = (Ogre::GpuProgramManager&)  Ogre::GpuProgramManager::getSingleton();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"Ogre::GpuProgramManager");
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

/* method: getSingletonPtr of class  Ogre::GpuProgramManager */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_GpuProgramManager_getSingletonPtr00
static int tolua_Ogre_Ogre_GpuProgramManager_getSingletonPtr00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::GpuProgramManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Ogre::GpuProgramManager* tolua_ret = (Ogre::GpuProgramManager*)  Ogre::GpuProgramManager::getSingletonPtr();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::GpuProgramManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSingletonPtr'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setSourceFile of class  Ogre::GpuProgram */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_GpuProgram_setSourceFile00
static int tolua_Ogre_Ogre_GpuProgram_setSourceFile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::GpuProgram",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::GpuProgram* self = (Ogre::GpuProgram*)  tolua_tousertype(tolua_S,1,0);
  const std::string filename = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setSourceFile'",NULL);
#endif
  {
   self->setSourceFile(filename);
   tolua_pushcppstring(tolua_S,(const char*)filename);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setSourceFile'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setSource of class  Ogre::GpuProgram */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_GpuProgram_setSource00
static int tolua_Ogre_Ogre_GpuProgram_setSource00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::GpuProgram",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::GpuProgram* self = (Ogre::GpuProgram*)  tolua_tousertype(tolua_S,1,0);
  const std::string source = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setSource'",NULL);
#endif
  {
   self->setSource(source);
   tolua_pushcppstring(tolua_S,(const char*)source);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setSource'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSyntaxCode of class  Ogre::GpuProgram */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_GpuProgram_getSyntaxCode00
static int tolua_Ogre_Ogre_GpuProgram_getSyntaxCode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::GpuProgram",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::GpuProgram* self = (const Ogre::GpuProgram*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSyntaxCode'",NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getSyntaxCode();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSyntaxCode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setSyntaxCode of class  Ogre::GpuProgram */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_GpuProgram_setSyntaxCode00
static int tolua_Ogre_Ogre_GpuProgram_setSyntaxCode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::GpuProgram",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::GpuProgram* self = (Ogre::GpuProgram*)  tolua_tousertype(tolua_S,1,0);
  const std::string syntax = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setSyntaxCode'",NULL);
#endif
  {
   self->setSyntaxCode(syntax);
   tolua_pushcppstring(tolua_S,(const char*)syntax);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setSyntaxCode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSourceFile of class  Ogre::GpuProgram */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_GpuProgram_getSourceFile00
static int tolua_Ogre_Ogre_GpuProgram_getSourceFile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::GpuProgram",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::GpuProgram* self = (const Ogre::GpuProgram*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSourceFile'",NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getSourceFile();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSourceFile'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSource of class  Ogre::GpuProgram */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_GpuProgram_getSource00
static int tolua_Ogre_Ogre_GpuProgram_getSource00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::GpuProgram",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::GpuProgram* self = (const Ogre::GpuProgram*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSource'",NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getSource();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSource'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isSupported of class  Ogre::GpuProgram */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_GpuProgram_isSupported00
static int tolua_Ogre_Ogre_GpuProgram_isSupported00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::GpuProgram",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::GpuProgram* self = (const Ogre::GpuProgram*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isSupported'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isSupported();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isSupported'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setSkeletalAnimationIncluded of class  Ogre::GpuProgram */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_GpuProgram_setSkeletalAnimationIncluded00
static int tolua_Ogre_Ogre_GpuProgram_setSkeletalAnimationIncluded00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::GpuProgram",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::GpuProgram* self = (Ogre::GpuProgram*)  tolua_tousertype(tolua_S,1,0);
  bool included = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setSkeletalAnimationIncluded'",NULL);
#endif
  {
   self->setSkeletalAnimationIncluded(included);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setSkeletalAnimationIncluded'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isSkeletalAnimationIncluded of class  Ogre::GpuProgram */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_GpuProgram_isSkeletalAnimationIncluded00
static int tolua_Ogre_Ogre_GpuProgram_isSkeletalAnimationIncluded00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::GpuProgram",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::GpuProgram* self = (const Ogre::GpuProgram*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isSkeletalAnimationIncluded'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isSkeletalAnimationIncluded();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isSkeletalAnimationIncluded'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setMorphAnimationIncluded of class  Ogre::GpuProgram */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_GpuProgram_setMorphAnimationIncluded00
static int tolua_Ogre_Ogre_GpuProgram_setMorphAnimationIncluded00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::GpuProgram",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::GpuProgram* self = (Ogre::GpuProgram*)  tolua_tousertype(tolua_S,1,0);
  bool included = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMorphAnimationIncluded'",NULL);
#endif
  {
   self->setMorphAnimationIncluded(included);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setMorphAnimationIncluded'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setPoseAnimationIncluded of class  Ogre::GpuProgram */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_GpuProgram_setPoseAnimationIncluded00
static int tolua_Ogre_Ogre_GpuProgram_setPoseAnimationIncluded00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::GpuProgram",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::GpuProgram* self = (Ogre::GpuProgram*)  tolua_tousertype(tolua_S,1,0);
  unsigned short poseCount = ((unsigned short)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPoseAnimationIncluded'",NULL);
#endif
  {
   self->setPoseAnimationIncluded(poseCount);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setPoseAnimationIncluded'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isMorphAnimationIncluded of class  Ogre::GpuProgram */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_GpuProgram_isMorphAnimationIncluded00
static int tolua_Ogre_Ogre_GpuProgram_isMorphAnimationIncluded00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::GpuProgram",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::GpuProgram* self = (const Ogre::GpuProgram*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isMorphAnimationIncluded'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isMorphAnimationIncluded();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isMorphAnimationIncluded'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isPoseAnimationIncluded of class  Ogre::GpuProgram */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_GpuProgram_isPoseAnimationIncluded00
static int tolua_Ogre_Ogre_GpuProgram_isPoseAnimationIncluded00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::GpuProgram",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::GpuProgram* self = (const Ogre::GpuProgram*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isPoseAnimationIncluded'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isPoseAnimationIncluded();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isPoseAnimationIncluded'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getNumberOfPosesIncluded of class  Ogre::GpuProgram */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_GpuProgram_getNumberOfPosesIncluded00
static int tolua_Ogre_Ogre_GpuProgram_getNumberOfPosesIncluded00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::GpuProgram",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::GpuProgram* self = (const Ogre::GpuProgram*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getNumberOfPosesIncluded'",NULL);
#endif
  {
   unsigned short tolua_ret = (unsigned short)  self->getNumberOfPosesIncluded();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getNumberOfPosesIncluded'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setVertexTextureFetchRequired of class  Ogre::GpuProgram */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_GpuProgram_setVertexTextureFetchRequired00
static int tolua_Ogre_Ogre_GpuProgram_setVertexTextureFetchRequired00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::GpuProgram",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::GpuProgram* self = (Ogre::GpuProgram*)  tolua_tousertype(tolua_S,1,0);
  bool r = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setVertexTextureFetchRequired'",NULL);
#endif
  {
   self->setVertexTextureFetchRequired(r);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setVertexTextureFetchRequired'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isVertexTextureFetchRequired of class  Ogre::GpuProgram */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_GpuProgram_isVertexTextureFetchRequired00
static int tolua_Ogre_Ogre_GpuProgram_isVertexTextureFetchRequired00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::GpuProgram",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::GpuProgram* self = (const Ogre::GpuProgram*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isVertexTextureFetchRequired'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isVertexTextureFetchRequired();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isVertexTextureFetchRequired'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setAdjacencyInfoRequired of class  Ogre::GpuProgram */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_GpuProgram_setAdjacencyInfoRequired00
static int tolua_Ogre_Ogre_GpuProgram_setAdjacencyInfoRequired00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::GpuProgram",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::GpuProgram* self = (Ogre::GpuProgram*)  tolua_tousertype(tolua_S,1,0);
  bool r = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAdjacencyInfoRequired'",NULL);
#endif
  {
   self->setAdjacencyInfoRequired(r);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAdjacencyInfoRequired'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isAdjacencyInfoRequired of class  Ogre::GpuProgram */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_GpuProgram_isAdjacencyInfoRequired00
static int tolua_Ogre_Ogre_GpuProgram_isAdjacencyInfoRequired00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::GpuProgram",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::GpuProgram* self = (const Ogre::GpuProgram*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isAdjacencyInfoRequired'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isAdjacencyInfoRequired();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isAdjacencyInfoRequired'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: hasDefaultParameters of class  Ogre::GpuProgram */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_GpuProgram_hasDefaultParameters00
static int tolua_Ogre_Ogre_GpuProgram_hasDefaultParameters00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::GpuProgram",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::GpuProgram* self = (const Ogre::GpuProgram*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hasDefaultParameters'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->hasDefaultParameters();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'hasDefaultParameters'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setSurfaceAndPassLightStates of class  Ogre::GpuProgram */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_GpuProgram_setSurfaceAndPassLightStates00
static int tolua_Ogre_Ogre_GpuProgram_setSurfaceAndPassLightStates00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::GpuProgram",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::GpuProgram* self = (Ogre::GpuProgram*)  tolua_tousertype(tolua_S,1,0);
  bool state = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setSurfaceAndPassLightStates'",NULL);
#endif
  {
   self->setSurfaceAndPassLightStates(state);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setSurfaceAndPassLightStates'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPassSurfaceAndLightStates of class  Ogre::GpuProgram */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_GpuProgram_getPassSurfaceAndLightStates00
static int tolua_Ogre_Ogre_GpuProgram_getPassSurfaceAndLightStates00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::GpuProgram",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::GpuProgram* self = (const Ogre::GpuProgram*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPassSurfaceAndLightStates'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->getPassSurfaceAndLightStates();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPassSurfaceAndLightStates'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getLanguage of class  Ogre::GpuProgram */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_GpuProgram_getLanguage00
static int tolua_Ogre_Ogre_GpuProgram_getLanguage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::GpuProgram",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::GpuProgram* self = (const Ogre::GpuProgram*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLanguage'",NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getLanguage();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getLanguage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: hasCompileError of class  Ogre::GpuProgram */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_GpuProgram_hasCompileError00
static int tolua_Ogre_Ogre_GpuProgram_hasCompileError00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::GpuProgram",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::GpuProgram* self = (const Ogre::GpuProgram*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hasCompileError'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->hasCompileError();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'hasCompileError'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: resetCompileError of class  Ogre::GpuProgram */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_GpuProgram_resetCompileError00
static int tolua_Ogre_Ogre_GpuProgram_resetCompileError00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::GpuProgram",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::GpuProgram* self = (Ogre::GpuProgram*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'resetCompileError'",NULL);
#endif
  {
   self->resetCompileError();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'resetCompileError'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setManualNamedConstantsFile of class  Ogre::GpuProgram */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_GpuProgram_setManualNamedConstantsFile00
static int tolua_Ogre_Ogre_GpuProgram_setManualNamedConstantsFile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::GpuProgram",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::GpuProgram* self = (Ogre::GpuProgram*)  tolua_tousertype(tolua_S,1,0);
  const std::string paramDefFile = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setManualNamedConstantsFile'",NULL);
#endif
  {
   self->setManualNamedConstantsFile(paramDefFile);
   tolua_pushcppstring(tolua_S,(const char*)paramDefFile);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setManualNamedConstantsFile'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getManualNamedConstantsFile of class  Ogre::GpuProgram */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_GpuProgram_getManualNamedConstantsFile00
static int tolua_Ogre_Ogre_GpuProgram_getManualNamedConstantsFile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::GpuProgram",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::GpuProgram* self = (const Ogre::GpuProgram*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getManualNamedConstantsFile'",NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getManualNamedConstantsFile();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getManualNamedConstantsFile'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get of class  Ogre::GpuProgramPtr */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_GpuProgramPtr_get00
static int tolua_Ogre_Ogre_GpuProgramPtr_get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::GpuProgramPtr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::GpuProgramPtr* self = (Ogre::GpuProgramPtr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get'",NULL);
#endif
  {
   Ogre::GpuProgram* tolua_ret = (Ogre::GpuProgram*)  self->get();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::GpuProgram");
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

/* method: isNull of class  Ogre::GpuProgramPtr */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_GpuProgramPtr_isNull00
static int tolua_Ogre_Ogre_GpuProgramPtr_isNull00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::GpuProgramPtr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::GpuProgramPtr* self = (Ogre::GpuProgramPtr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isNull'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isNull();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isNull'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSingleton of class  Ogre::HighLevelGpuProgramManager */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_HighLevelGpuProgramManager_getSingleton00
static int tolua_Ogre_Ogre_HighLevelGpuProgramManager_getSingleton00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::HighLevelGpuProgramManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Ogre::HighLevelGpuProgramManager& tolua_ret = (Ogre::HighLevelGpuProgramManager&)  Ogre::HighLevelGpuProgramManager::getSingleton();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"Ogre::HighLevelGpuProgramManager");
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

/* method: getSingletonPtr of class  Ogre::HighLevelGpuProgramManager */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_HighLevelGpuProgramManager_getSingletonPtr00
static int tolua_Ogre_Ogre_HighLevelGpuProgramManager_getSingletonPtr00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::HighLevelGpuProgramManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Ogre::HighLevelGpuProgramManager* tolua_ret = (Ogre::HighLevelGpuProgramManager*)  Ogre::HighLevelGpuProgramManager::getSingletonPtr();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::HighLevelGpuProgramManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSingletonPtr'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get of class  Ogre::HighLevelGpuProgramPtr */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_HighLevelGpuProgramPtr_get00
static int tolua_Ogre_Ogre_HighLevelGpuProgramPtr_get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::HighLevelGpuProgramPtr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::HighLevelGpuProgramPtr* self = (Ogre::HighLevelGpuProgramPtr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get'",NULL);
#endif
  {
   Ogre::HighLevelGpuProgram* tolua_ret = (Ogre::HighLevelGpuProgram*)  self->get();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::HighLevelGpuProgram");
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

/* method: isNull of class  Ogre::HighLevelGpuProgramPtr */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_HighLevelGpuProgramPtr_isNull00
static int tolua_Ogre_Ogre_HighLevelGpuProgramPtr_isNull00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::HighLevelGpuProgramPtr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::HighLevelGpuProgramPtr* self = (Ogre::HighLevelGpuProgramPtr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isNull'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isNull();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isNull'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: saveConfig of class  Ogre::Root */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Root_saveConfig00
static int tolua_Ogre_Ogre_Root_saveConfig00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Root",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Root* self = (Ogre::Root*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'saveConfig'",NULL);
#endif
  {
   self->saveConfig();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'saveConfig'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: restoreConfig of class  Ogre::Root */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Root_restoreConfig00
static int tolua_Ogre_Ogre_Root_restoreConfig00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Root",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Root* self = (Ogre::Root*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'restoreConfig'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->restoreConfig();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'restoreConfig'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: showConfigDialog of class  Ogre::Root */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Root_showConfigDialog00
static int tolua_Ogre_Ogre_Root_showConfigDialog00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Root",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Root* self = (Ogre::Root*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'showConfigDialog'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->showConfigDialog();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'showConfigDialog'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isInitialised of class  Ogre::Root */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Root_isInitialised00
static int tolua_Ogre_Ogre_Root_isInitialised00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Root",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Root* self = (const Ogre::Root*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isInitialised'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isInitialised();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isInitialised'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTextureManager of class  Ogre::Root */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Root_getTextureManager00
static int tolua_Ogre_Ogre_Root_getTextureManager00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Root",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Root* self = (Ogre::Root*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTextureManager'",NULL);
#endif
  {
   Ogre::TextureManager* tolua_ret = (Ogre::TextureManager*)  self->getTextureManager();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::TextureManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTextureManager'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMeshManager of class  Ogre::Root */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Root_getMeshManager00
static int tolua_Ogre_Ogre_Root_getMeshManager00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Root",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Root* self = (Ogre::Root*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMeshManager'",NULL);
#endif
  {
   Ogre::MeshManager* tolua_ret = (Ogre::MeshManager*)  self->getMeshManager();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::MeshManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMeshManager'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getErrorDescription of class  Ogre::Root */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Root_getErrorDescription00
static int tolua_Ogre_Ogre_Root_getErrorDescription00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Root",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Root* self = (Ogre::Root*)  tolua_tousertype(tolua_S,1,0);
  long errorNumber = ((long)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getErrorDescription'",NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getErrorDescription(errorNumber);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getErrorDescription'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addFrameListener of class  Ogre::Root */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Root_addFrameListener00
static int tolua_Ogre_Ogre_Root_addFrameListener00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Root",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::FrameListener",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Root* self = (Ogre::Root*)  tolua_tousertype(tolua_S,1,0);
  Ogre::FrameListener* newListener = ((Ogre::FrameListener*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addFrameListener'",NULL);
#endif
  {
   self->addFrameListener(newListener);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addFrameListener'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeFrameListener of class  Ogre::Root */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Root_removeFrameListener00
static int tolua_Ogre_Ogre_Root_removeFrameListener00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Root",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::FrameListener",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Root* self = (Ogre::Root*)  tolua_tousertype(tolua_S,1,0);
  Ogre::FrameListener* oldListener = ((Ogre::FrameListener*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeFrameListener'",NULL);
#endif
  {
   self->removeFrameListener(oldListener);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeFrameListener'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: convertColourValue of class  Ogre::Root */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Root_convertColourValue00
static int tolua_Ogre_Ogre_Root_convertColourValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Root",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Ogre::ColourValue",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Root* self = (Ogre::Root*)  tolua_tousertype(tolua_S,1,0);
  const Ogre::ColourValue* colour = ((const Ogre::ColourValue*)  tolua_tousertype(tolua_S,2,0));
  unsigned int pDest = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'convertColourValue'",NULL);
#endif
  {
   self->convertColourValue(*colour,&pDest);
   tolua_pushnumber(tolua_S,(lua_Number)pDest);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'convertColourValue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTimer of class  Ogre::Root */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Root_getTimer00
static int tolua_Ogre_Ogre_Root_getTimer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ogre::Root",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Root* self = (Ogre::Root*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTimer'",NULL);
#endif
  {
   Ogre::Timer* tolua_ret = (Ogre::Timer*)  self->getTimer();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ogre::Timer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTimer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getNextFrameNumber of class  Ogre::Root */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Root_getNextFrameNumber00
static int tolua_Ogre_Ogre_Root_getNextFrameNumber00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ogre::Root",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ogre::Root* self = (const Ogre::Root*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getNextFrameNumber'",NULL);
#endif
  {
   unsigned long tolua_ret = (unsigned long)  self->getNextFrameNumber();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getNextFrameNumber'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSingleton of class  Ogre::Root */
#ifndef TOLUA_DISABLE_tolua_Ogre_Ogre_Root_getSingleton00
static int tolua_Ogre_Ogre_Root_getSingleton00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ogre::Root",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Ogre::Root& tolua_ret = (Ogre::Root&)  Ogre::Root::getSingleton();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"Ogre::Root");
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
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"ResourcePtr","Ogre::ResourcePtr","",tolua_collect_Ogre__ResourcePtr);
   #else
   tolua_cclass(tolua_S,"ResourcePtr","Ogre::ResourcePtr","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"ResourcePtr");
    tolua_function(tolua_S,"get",tolua_Ogre_Ogre_ResourcePtr_get00);
    tolua_function(tolua_S,"isNull",tolua_Ogre_Ogre_ResourcePtr_isNull00);
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
    tolua_function(tolua_S,"remove",tolua_Ogre_Ogre_ResourceManager_remove02);
    tolua_function(tolua_S,"removeAll",tolua_Ogre_Ogre_ResourceManager_removeAll00);
    tolua_function(tolua_S,"getByName",tolua_Ogre_Ogre_ResourceManager_getByName00);
    tolua_function(tolua_S,"getByHandle",tolua_Ogre_Ogre_ResourceManager_getByHandle00);
    tolua_function(tolua_S,"resourceExists",tolua_Ogre_Ogre_ResourceManager_resourceExists00);
    tolua_function(tolua_S,"resourceExists",tolua_Ogre_Ogre_ResourceManager_resourceExists01);
    tolua_function(tolua_S,"getResourceType",tolua_Ogre_Ogre_ResourceManager_getResourceType00);
    tolua_function(tolua_S,"getResourceIterator",tolua_Ogre_Ogre_ResourceManager_getResourceIterator00);
    #ifdef __cplusplus
    tolua_cclass(tolua_S,"ResourceMapIterator","Ogre::ResourceManager::ResourceMapIterator","",tolua_collect_Ogre__ResourceManager__ResourceMapIterator);
    #else
    tolua_cclass(tolua_S,"ResourceMapIterator","Ogre::ResourceManager::ResourceMapIterator","",NULL);
    #endif
    tolua_beginmodule(tolua_S,"ResourceMapIterator");
     tolua_function(tolua_S,"delete",tolua_Ogre_Ogre_ResourceManager_ResourceMapIterator_delete00);
     tolua_function(tolua_S,"hasMoreElements",tolua_Ogre_Ogre_ResourceManager_ResourceMapIterator_hasMoreElements00);
     tolua_function(tolua_S,"getNext",tolua_Ogre_Ogre_ResourceManager_ResourceMapIterator_getNext00);
     tolua_function(tolua_S,"peekNextValue",tolua_Ogre_Ogre_ResourceManager_ResourceMapIterator_peekNextValue00);
     tolua_function(tolua_S,"peekNextKey",tolua_Ogre_Ogre_ResourceManager_ResourceMapIterator_peekNextKey00);
     tolua_function(tolua_S,"peekNextValuePtr",tolua_Ogre_Ogre_ResourceManager_ResourceMapIterator_peekNextValuePtr00);
     tolua_function(tolua_S,"moveNext",tolua_Ogre_Ogre_ResourceManager_ResourceMapIterator_moveNext00);
    tolua_endmodule(tolua_S);
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
    tolua_function(tolua_S,"isNull",tolua_Ogre_Ogre_MaterialPtr_isNull00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"Ogre",0);
  tolua_beginmodule(tolua_S,"Ogre");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"MaterialSerializer","Ogre::MaterialSerializer","",tolua_collect_Ogre__MaterialSerializer);
   #else
   tolua_cclass(tolua_S,"MaterialSerializer","Ogre::MaterialSerializer","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"MaterialSerializer");
    tolua_function(tolua_S,"new",tolua_Ogre_Ogre_MaterialSerializer_new00);
    tolua_function(tolua_S,"new_local",tolua_Ogre_Ogre_MaterialSerializer_new00_local);
    tolua_function(tolua_S,".call",tolua_Ogre_Ogre_MaterialSerializer_new00_local);
    tolua_function(tolua_S,"delete",tolua_Ogre_Ogre_MaterialSerializer_delete00);
    tolua_function(tolua_S,"queueForExport",tolua_Ogre_Ogre_MaterialSerializer_queueForExport00);
    tolua_function(tolua_S,"exportQueued",tolua_Ogre_Ogre_MaterialSerializer_exportQueued00);
    tolua_function(tolua_S,"exportQueued",tolua_Ogre_Ogre_MaterialSerializer_exportQueued01);
    tolua_function(tolua_S,"exportMaterial",tolua_Ogre_Ogre_MaterialSerializer_exportMaterial00);
    tolua_function(tolua_S,"exportMaterial",tolua_Ogre_Ogre_MaterialSerializer_exportMaterial01);
    tolua_function(tolua_S,"getQueuedAsString",tolua_Ogre_Ogre_MaterialSerializer_getQueuedAsString00);
    tolua_function(tolua_S,"clearQueue",tolua_Ogre_Ogre_MaterialSerializer_clearQueue00);
    tolua_function(tolua_S,"parseScript",tolua_Ogre_Ogre_MaterialSerializer_parseScript00);
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
    tolua_function(tolua_S,"getSubMesh",tolua_Ogre_Ogre_Mesh_getSubMesh00);
    tolua_function(tolua_S,"getBounds",tolua_Ogre_Ogre_Mesh_getBounds00);
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
    tolua_function(tolua_S,"get",tolua_Ogre_Ogre_MeshPtr_get01);
    tolua_function(tolua_S,"isNull",tolua_Ogre_Ogre_MeshPtr_isNull00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"Ogre",0);
  tolua_beginmodule(tolua_S,"Ogre");
   tolua_cclass(tolua_S,"SubMesh","Ogre::SubMesh","",NULL);
   tolua_beginmodule(tolua_S,"SubMesh");
    tolua_variable(tolua_S,"useSharedVertices",tolua_get_Ogre__SubMesh_useSharedVertices,tolua_set_Ogre__SubMesh_useSharedVertices);
    tolua_variable(tolua_S,"parent",tolua_get_Ogre__SubMesh_parent_ptr,tolua_set_Ogre__SubMesh_parent_ptr);
    tolua_function(tolua_S,"setMaterialName",tolua_Ogre_Ogre_SubMesh_setMaterialName00);
    tolua_function(tolua_S,"getMaterialName",tolua_Ogre_Ogre_SubMesh_getMaterialName00);
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
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Quaternion","Ogre::Quaternion","",tolua_collect_Ogre__Quaternion);
   #else
   tolua_cclass(tolua_S,"Quaternion","Ogre::Quaternion","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Quaternion");
    tolua_function(tolua_S,"new",tolua_Ogre_Ogre_Quaternion_new00);
    tolua_function(tolua_S,"new_local",tolua_Ogre_Ogre_Quaternion_new00_local);
    tolua_function(tolua_S,".call",tolua_Ogre_Ogre_Quaternion_new00_local);
    tolua_function(tolua_S,"new",tolua_Ogre_Ogre_Quaternion_new01);
    tolua_function(tolua_S,"new_local",tolua_Ogre_Ogre_Quaternion_new01_local);
    tolua_function(tolua_S,".call",tolua_Ogre_Ogre_Quaternion_new01_local);
    tolua_function(tolua_S,"new",tolua_Ogre_Ogre_Quaternion_new02);
    tolua_function(tolua_S,"new_local",tolua_Ogre_Ogre_Quaternion_new02_local);
    tolua_function(tolua_S,".call",tolua_Ogre_Ogre_Quaternion_new02_local);
    tolua_function(tolua_S,"new",tolua_Ogre_Ogre_Quaternion_new03);
    tolua_function(tolua_S,"new_local",tolua_Ogre_Ogre_Quaternion_new03_local);
    tolua_function(tolua_S,".call",tolua_Ogre_Ogre_Quaternion_new03_local);
    tolua_function(tolua_S,"new",tolua_Ogre_Ogre_Quaternion_new04);
    tolua_function(tolua_S,"new_local",tolua_Ogre_Ogre_Quaternion_new04_local);
    tolua_function(tolua_S,".call",tolua_Ogre_Ogre_Quaternion_new04_local);
    tolua_function(tolua_S,"FromRotationMatrix",tolua_Ogre_Ogre_Quaternion_FromRotationMatrix00);
    tolua_function(tolua_S,"ToRotationMatrix",tolua_Ogre_Ogre_Quaternion_ToRotationMatrix00);
    tolua_function(tolua_S,"FromAngleAxis",tolua_Ogre_Ogre_Quaternion_FromAngleAxis00);
    tolua_function(tolua_S,"ToAngleAxis",tolua_Ogre_Ogre_Quaternion_ToAngleAxis00);
    tolua_function(tolua_S,"ToAngleAxis",tolua_Ogre_Ogre_Quaternion_ToAngleAxis01);
    tolua_function(tolua_S,"FromAxes",tolua_Ogre_Ogre_Quaternion_FromAxes00);
    tolua_function(tolua_S,"FromAxes",tolua_Ogre_Ogre_Quaternion_FromAxes01);
    tolua_function(tolua_S,"ToAxes",tolua_Ogre_Ogre_Quaternion_ToAxes00);
    tolua_function(tolua_S,"ToAxes",tolua_Ogre_Ogre_Quaternion_ToAxes01);
    tolua_function(tolua_S,"xAxis",tolua_Ogre_Ogre_Quaternion_xAxis00);
    tolua_function(tolua_S,"yAxis",tolua_Ogre_Ogre_Quaternion_yAxis00);
    tolua_function(tolua_S,"zAxis",tolua_Ogre_Ogre_Quaternion_zAxis00);
    tolua_function(tolua_S,".add",tolua_Ogre_Ogre_Quaternion__add00);
    tolua_function(tolua_S,".sub",tolua_Ogre_Ogre_Quaternion__sub00);
    tolua_function(tolua_S,".mul",tolua_Ogre_Ogre_Quaternion__mul00);
    tolua_function(tolua_S,".mul",tolua_Ogre_Ogre_Quaternion__mul01);
    tolua_function(tolua_S,".sub",tolua_Ogre_Ogre_Quaternion__sub01);
    tolua_function(tolua_S,".eq",tolua_Ogre_Ogre_Quaternion__eq00);
    tolua_function(tolua_S,"Dot",tolua_Ogre_Ogre_Quaternion_Dot00);
    tolua_function(tolua_S,"Norm",tolua_Ogre_Ogre_Quaternion_Norm00);
    tolua_function(tolua_S,"normalise",tolua_Ogre_Ogre_Quaternion_normalise00);
    tolua_function(tolua_S,"Inverse",tolua_Ogre_Ogre_Quaternion_Inverse00);
    tolua_function(tolua_S,"UnitInverse",tolua_Ogre_Ogre_Quaternion_UnitInverse00);
    tolua_function(tolua_S,"Exp",tolua_Ogre_Ogre_Quaternion_Exp00);
    tolua_function(tolua_S,"Log",tolua_Ogre_Ogre_Quaternion_Log00);
    tolua_function(tolua_S,".mul",tolua_Ogre_Ogre_Quaternion__mul02);
    tolua_function(tolua_S,"getRoll",tolua_Ogre_Ogre_Quaternion_getRoll00);
    tolua_function(tolua_S,"getPitch",tolua_Ogre_Ogre_Quaternion_getPitch00);
    tolua_function(tolua_S,"getYaw",tolua_Ogre_Ogre_Quaternion_getYaw00);
    tolua_function(tolua_S,"equals",tolua_Ogre_Ogre_Quaternion_equals00);
    tolua_function(tolua_S,"Slerp",tolua_Ogre_Ogre_Quaternion_Slerp00);
    tolua_function(tolua_S,"SlerpExtraSpins",tolua_Ogre_Ogre_Quaternion_SlerpExtraSpins00);
    tolua_function(tolua_S,"Intermediate",tolua_Ogre_Ogre_Quaternion_Intermediate00);
    tolua_function(tolua_S,"Squad",tolua_Ogre_Ogre_Quaternion_Squad00);
    tolua_function(tolua_S,"nlerp",tolua_Ogre_Ogre_Quaternion_nlerp00);
    tolua_variable(tolua_S,"ms_fEpsilon",tolua_get_Ogre__Quaternion_ms_fEpsilon,NULL);
    tolua_variable(tolua_S,"ZERO",tolua_get_Ogre__Quaternion_ZERO,NULL);
    tolua_variable(tolua_S,"IDENTITY",tolua_get_Ogre__Quaternion_IDENTITY,NULL);
    tolua_variable(tolua_S,"w",tolua_get_Ogre__Quaternion_w,tolua_set_Ogre__Quaternion_w);
    tolua_variable(tolua_S,"x",tolua_get_Ogre__Quaternion_x,tolua_set_Ogre__Quaternion_x);
    tolua_variable(tolua_S,"y",tolua_get_Ogre__Quaternion_y,tolua_set_Ogre__Quaternion_y);
    tolua_variable(tolua_S,"z",tolua_get_Ogre__Quaternion_z,tolua_set_Ogre__Quaternion_z);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"Ogre",0);
  tolua_beginmodule(tolua_S,"Ogre");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Radian","Ogre::Radian","",tolua_collect_Ogre__Radian);
   #else
   tolua_cclass(tolua_S,"Radian","Ogre::Radian","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Radian");
    tolua_function(tolua_S,"new",tolua_Ogre_Ogre_Radian_new00);
    tolua_function(tolua_S,"new_local",tolua_Ogre_Ogre_Radian_new00_local);
    tolua_function(tolua_S,".call",tolua_Ogre_Ogre_Radian_new00_local);
    tolua_function(tolua_S,"new",tolua_Ogre_Ogre_Radian_new01);
    tolua_function(tolua_S,"new_local",tolua_Ogre_Ogre_Radian_new01_local);
    tolua_function(tolua_S,".call",tolua_Ogre_Ogre_Radian_new01_local);
    tolua_function(tolua_S,"valueDegrees",tolua_Ogre_Ogre_Radian_valueDegrees00);
    tolua_function(tolua_S,"valueRadians",tolua_Ogre_Ogre_Radian_valueRadians00);
    tolua_function(tolua_S,"valueAngleUnits",tolua_Ogre_Ogre_Radian_valueAngleUnits00);
    tolua_function(tolua_S,".add",tolua_Ogre_Ogre_Radian__add00);
    tolua_function(tolua_S,".add",tolua_Ogre_Ogre_Radian__add01);
    tolua_function(tolua_S,".sub",tolua_Ogre_Ogre_Radian__sub00);
    tolua_function(tolua_S,".sub",tolua_Ogre_Ogre_Radian__sub01);
    tolua_function(tolua_S,".sub",tolua_Ogre_Ogre_Radian__sub02);
    tolua_function(tolua_S,".mul",tolua_Ogre_Ogre_Radian__mul00);
    tolua_function(tolua_S,".mul",tolua_Ogre_Ogre_Radian__mul01);
    tolua_function(tolua_S,".div",tolua_Ogre_Ogre_Radian__div00);
    tolua_function(tolua_S,".lt",tolua_Ogre_Ogre_Radian__lt00);
    tolua_function(tolua_S,".le",tolua_Ogre_Ogre_Radian__le00);
    tolua_function(tolua_S,".eq",tolua_Ogre_Ogre_Radian__eq00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Degree","Ogre::Degree","",tolua_collect_Ogre__Degree);
   #else
   tolua_cclass(tolua_S,"Degree","Ogre::Degree","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Degree");
    tolua_function(tolua_S,"new",tolua_Ogre_Ogre_Degree_new00);
    tolua_function(tolua_S,"new_local",tolua_Ogre_Ogre_Degree_new00_local);
    tolua_function(tolua_S,".call",tolua_Ogre_Ogre_Degree_new00_local);
    tolua_function(tolua_S,"new",tolua_Ogre_Ogre_Degree_new01);
    tolua_function(tolua_S,"new_local",tolua_Ogre_Ogre_Degree_new01_local);
    tolua_function(tolua_S,".call",tolua_Ogre_Ogre_Degree_new01_local);
    tolua_function(tolua_S,"valueDegrees",tolua_Ogre_Ogre_Degree_valueDegrees00);
    tolua_function(tolua_S,"valueRadians",tolua_Ogre_Ogre_Degree_valueRadians00);
    tolua_function(tolua_S,"valueAngleUnits",tolua_Ogre_Ogre_Degree_valueAngleUnits00);
    tolua_function(tolua_S,".add",tolua_Ogre_Ogre_Degree__add00);
    tolua_function(tolua_S,".add",tolua_Ogre_Ogre_Degree__add01);
    tolua_function(tolua_S,".sub",tolua_Ogre_Ogre_Degree__sub00);
    tolua_function(tolua_S,".sub",tolua_Ogre_Ogre_Degree__sub01);
    tolua_function(tolua_S,".sub",tolua_Ogre_Ogre_Degree__sub02);
    tolua_function(tolua_S,".mul",tolua_Ogre_Ogre_Degree__mul00);
    tolua_function(tolua_S,".mul",tolua_Ogre_Ogre_Degree__mul01);
    tolua_function(tolua_S,".div",tolua_Ogre_Ogre_Degree__div00);
    tolua_function(tolua_S,".lt",tolua_Ogre_Ogre_Degree__lt00);
    tolua_function(tolua_S,".le",tolua_Ogre_Ogre_Degree__le00);
    tolua_function(tolua_S,".eq",tolua_Ogre_Ogre_Degree__eq00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Math","Ogre::Math","",tolua_collect_Ogre__Math);
   #else
   tolua_cclass(tolua_S,"Math","Ogre::Math","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Math");
    tolua_constant(tolua_S,"AU_DEGREE",Ogre::Math::AU_DEGREE);
    tolua_constant(tolua_S,"AU_RADIAN",Ogre::Math::AU_RADIAN);
    tolua_function(tolua_S,"new",tolua_Ogre_Ogre_Math_new00);
    tolua_function(tolua_S,"new_local",tolua_Ogre_Ogre_Math_new00_local);
    tolua_function(tolua_S,".call",tolua_Ogre_Ogre_Math_new00_local);
    tolua_function(tolua_S,"delete",tolua_Ogre_Ogre_Math_delete00);
    tolua_function(tolua_S,"IAbs",tolua_Ogre_Ogre_Math_IAbs00);
    tolua_function(tolua_S,"ICeil",tolua_Ogre_Ogre_Math_ICeil00);
    tolua_function(tolua_S,"IFloor",tolua_Ogre_Ogre_Math_IFloor00);
    tolua_function(tolua_S,"ISign",tolua_Ogre_Ogre_Math_ISign00);
    tolua_function(tolua_S,"Abs",tolua_Ogre_Ogre_Math_Abs00);
    tolua_function(tolua_S,"Abs",tolua_Ogre_Ogre_Math_Abs01);
    tolua_function(tolua_S,"Abs",tolua_Ogre_Ogre_Math_Abs02);
    tolua_function(tolua_S,"ACos",tolua_Ogre_Ogre_Math_ACos00);
    tolua_function(tolua_S,"ASin",tolua_Ogre_Ogre_Math_ASin00);
    tolua_function(tolua_S,"ATan",tolua_Ogre_Ogre_Math_ATan00);
    tolua_function(tolua_S,"ATan2",tolua_Ogre_Ogre_Math_ATan200);
    tolua_function(tolua_S,"Ceil",tolua_Ogre_Ogre_Math_Ceil00);
    tolua_function(tolua_S,"Cos",tolua_Ogre_Ogre_Math_Cos00);
    tolua_function(tolua_S,"Cos",tolua_Ogre_Ogre_Math_Cos01);
    tolua_function(tolua_S,"Exp",tolua_Ogre_Ogre_Math_Exp00);
    tolua_function(tolua_S,"Floor",tolua_Ogre_Ogre_Math_Floor00);
    tolua_function(tolua_S,"Log",tolua_Ogre_Ogre_Math_Log00);
    tolua_function(tolua_S,"Pow",tolua_Ogre_Ogre_Math_Pow00);
    tolua_function(tolua_S,"Sign",tolua_Ogre_Ogre_Math_Sign00);
    tolua_function(tolua_S,"Sign",tolua_Ogre_Ogre_Math_Sign01);
    tolua_function(tolua_S,"Sign",tolua_Ogre_Ogre_Math_Sign02);
    tolua_function(tolua_S,"Sin",tolua_Ogre_Ogre_Math_Sin00);
    tolua_function(tolua_S,"Sin",tolua_Ogre_Ogre_Math_Sin01);
    tolua_function(tolua_S,"Sqr",tolua_Ogre_Ogre_Math_Sqr00);
    tolua_function(tolua_S,"Sqrt",tolua_Ogre_Ogre_Math_Sqrt00);
    tolua_function(tolua_S,"Sqrt",tolua_Ogre_Ogre_Math_Sqrt01);
    tolua_function(tolua_S,"Sqrt",tolua_Ogre_Ogre_Math_Sqrt02);
    tolua_function(tolua_S,"InvSqrt",tolua_Ogre_Ogre_Math_InvSqrt00);
    tolua_function(tolua_S,"UnitRandom",tolua_Ogre_Ogre_Math_UnitRandom00);
    tolua_function(tolua_S,"RangeRandom",tolua_Ogre_Ogre_Math_RangeRandom00);
    tolua_function(tolua_S,"SymmetricRandom",tolua_Ogre_Ogre_Math_SymmetricRandom00);
    tolua_function(tolua_S,"Tan",tolua_Ogre_Ogre_Math_Tan00);
    tolua_function(tolua_S,"Tan",tolua_Ogre_Ogre_Math_Tan01);
    tolua_function(tolua_S,"DegreesToRadians",tolua_Ogre_Ogre_Math_DegreesToRadians00);
    tolua_function(tolua_S,"RadiansToDegrees",tolua_Ogre_Ogre_Math_RadiansToDegrees00);
    tolua_function(tolua_S,"setAngleUnit",tolua_Ogre_Ogre_Math_setAngleUnit00);
    tolua_function(tolua_S,"getAngleUnit",tolua_Ogre_Ogre_Math_getAngleUnit00);
    tolua_function(tolua_S,"AngleUnitsToRadians",tolua_Ogre_Ogre_Math_AngleUnitsToRadians00);
    tolua_function(tolua_S,"RadiansToAngleUnits",tolua_Ogre_Ogre_Math_RadiansToAngleUnits00);
    tolua_function(tolua_S,"AngleUnitsToDegrees",tolua_Ogre_Ogre_Math_AngleUnitsToDegrees00);
    tolua_function(tolua_S,"DegreesToAngleUnits",tolua_Ogre_Ogre_Math_DegreesToAngleUnits00);
    tolua_function(tolua_S,"pointInTri2D",tolua_Ogre_Ogre_Math_pointInTri2D00);
    tolua_function(tolua_S,"pointInTri3D",tolua_Ogre_Ogre_Math_pointInTri3D00);
    tolua_function(tolua_S,"intersects",tolua_Ogre_Ogre_Math_intersects00);
    tolua_function(tolua_S,"intersects",tolua_Ogre_Ogre_Math_intersects01);
    tolua_function(tolua_S,"intersects",tolua_Ogre_Ogre_Math_intersects02);
    tolua_function(tolua_S,"intersects",tolua_Ogre_Ogre_Math_intersects03);
    tolua_function(tolua_S,"intersects",tolua_Ogre_Ogre_Math_intersects04);
    tolua_function(tolua_S,"intersects",tolua_Ogre_Ogre_Math_intersects05);
    tolua_function(tolua_S,"intersects",tolua_Ogre_Ogre_Math_intersects06);
    tolua_function(tolua_S,"intersects",tolua_Ogre_Ogre_Math_intersects07);
    tolua_function(tolua_S,"intersects",tolua_Ogre_Ogre_Math_intersects08);
    tolua_function(tolua_S,"intersects",tolua_Ogre_Ogre_Math_intersects09);
    tolua_function(tolua_S,"intersects",tolua_Ogre_Ogre_Math_intersects10);
    tolua_function(tolua_S,"RealEqual",tolua_Ogre_Ogre_Math_RealEqual00);
    tolua_function(tolua_S,"RealEqual",tolua_Ogre_Ogre_Math_RealEqual01);
    tolua_function(tolua_S,"calculateTangentSpaceVector",tolua_Ogre_Ogre_Math_calculateTangentSpaceVector00);
    tolua_function(tolua_S,"buildReflectionMatrix",tolua_Ogre_Ogre_Math_buildReflectionMatrix00);
    tolua_function(tolua_S,"calculateFaceNormal",tolua_Ogre_Ogre_Math_calculateFaceNormal00);
    tolua_function(tolua_S,"calculateBasicFaceNormal",tolua_Ogre_Ogre_Math_calculateBasicFaceNormal00);
    tolua_function(tolua_S,"calculateFaceNormalWithoutNormalize",tolua_Ogre_Ogre_Math_calculateFaceNormalWithoutNormalize00);
    tolua_function(tolua_S,"calculateBasicFaceNormalWithoutNormalize",tolua_Ogre_Ogre_Math_calculateBasicFaceNormalWithoutNormalize00);
    tolua_function(tolua_S,"gaussianDistribution",tolua_Ogre_Ogre_Math_gaussianDistribution00);
    tolua_variable(tolua_S,"POS_INFINITY",tolua_get_Ogre__Math_POS_INFINITY,NULL);
    tolua_variable(tolua_S,"NEG_INFINITY",tolua_get_Ogre__Math_NEG_INFINITY,NULL);
    tolua_variable(tolua_S,"PI",tolua_get_Ogre__Math_PI,NULL);
    tolua_variable(tolua_S,"TWO_PI",tolua_get_Ogre__Math_TWO_PI,NULL);
    tolua_variable(tolua_S,"HALF_PI",tolua_get_Ogre__Math_HALF_PI,NULL);
    tolua_variable(tolua_S,"fDeg2Rad",tolua_get_Ogre__Math_fDeg2Rad,NULL);
    tolua_variable(tolua_S,"fRad2Deg",tolua_get_Ogre__Math_fRad2Deg,NULL);
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
  tolua_module(tolua_S,"Ogre",0);
  tolua_beginmodule(tolua_S,"Ogre");
   tolua_cclass(tolua_S,"Node","Ogre::Node","Renderable",NULL);
   tolua_beginmodule(tolua_S,"Node");
    tolua_constant(tolua_S,"TS_LOCAL",Ogre::Node::TS_LOCAL);
    tolua_constant(tolua_S,"TS_PARENT",Ogre::Node::TS_PARENT);
    tolua_constant(tolua_S,"TS_WORLD",Ogre::Node::TS_WORLD);
    tolua_function(tolua_S,"getName",tolua_Ogre_Ogre_Node_getName00);
    tolua_function(tolua_S,"getParent",tolua_Ogre_Ogre_Node_getParent00);
    tolua_function(tolua_S,"getOrientation",tolua_Ogre_Ogre_Node_getOrientation00);
    tolua_function(tolua_S,"setOrientation",tolua_Ogre_Ogre_Node_setOrientation00);
    tolua_function(tolua_S,"setOrientation",tolua_Ogre_Ogre_Node_setOrientation01);
    tolua_function(tolua_S,"resetOrientation",tolua_Ogre_Ogre_Node_resetOrientation00);
    tolua_function(tolua_S,"setPosition",tolua_Ogre_Ogre_Node_setPosition00);
    tolua_function(tolua_S,"setPosition",tolua_Ogre_Ogre_Node_setPosition01);
    tolua_function(tolua_S,"getPosition",tolua_Ogre_Ogre_Node_getPosition00);
    tolua_function(tolua_S,"setScale",tolua_Ogre_Ogre_Node_setScale00);
    tolua_function(tolua_S,"setScale",tolua_Ogre_Ogre_Node_setScale01);
    tolua_function(tolua_S,"getScale",tolua_Ogre_Ogre_Node_getScale00);
    tolua_function(tolua_S,"setInheritOrientation",tolua_Ogre_Ogre_Node_setInheritOrientation00);
    tolua_function(tolua_S,"getInheritOrientation",tolua_Ogre_Ogre_Node_getInheritOrientation00);
    tolua_function(tolua_S,"setInheritScale",tolua_Ogre_Ogre_Node_setInheritScale00);
    tolua_function(tolua_S,"getInheritScale",tolua_Ogre_Ogre_Node_getInheritScale00);
    tolua_function(tolua_S,"scale",tolua_Ogre_Ogre_Node_scale00);
    tolua_function(tolua_S,"scale",tolua_Ogre_Ogre_Node_scale01);
    tolua_function(tolua_S,"translate",tolua_Ogre_Ogre_Node_translate00);
    tolua_function(tolua_S,"translate",tolua_Ogre_Ogre_Node_translate01);
    tolua_function(tolua_S,"translate",tolua_Ogre_Ogre_Node_translate02);
    tolua_function(tolua_S,"roll",tolua_Ogre_Ogre_Node_roll00);
    tolua_function(tolua_S,"pitch",tolua_Ogre_Ogre_Node_pitch00);
    tolua_function(tolua_S,"yaw",tolua_Ogre_Ogre_Node_yaw00);
    tolua_function(tolua_S,"rotate",tolua_Ogre_Ogre_Node_rotate00);
    tolua_function(tolua_S,"rotate",tolua_Ogre_Ogre_Node_rotate01);
    tolua_function(tolua_S,"getLocalAxes",tolua_Ogre_Ogre_Node_getLocalAxes00);
    tolua_function(tolua_S,"createChild",tolua_Ogre_Ogre_Node_createChild00);
    tolua_function(tolua_S,"createChild",tolua_Ogre_Ogre_Node_createChild01);
    tolua_function(tolua_S,"createChild",tolua_Ogre_Ogre_Node_createChild02);
    tolua_function(tolua_S,"createChild",tolua_Ogre_Ogre_Node_createChild03);
    tolua_function(tolua_S,"createChild",tolua_Ogre_Ogre_Node_createChild04);
    tolua_function(tolua_S,"createChild",tolua_Ogre_Ogre_Node_createChild05);
    tolua_function(tolua_S,"addChild",tolua_Ogre_Ogre_Node_addChild00);
    tolua_function(tolua_S,"numChildren",tolua_Ogre_Ogre_Node_numChildren00);
    tolua_function(tolua_S,"getChildByIndex",tolua_Ogre_Ogre_Node_getChildByIndex00);
    tolua_function(tolua_S,"getChild",tolua_Ogre_Ogre_Node_getChild00);
    tolua_function(tolua_S,"removeChild",tolua_Ogre_Ogre_Node_removeChild00);
    tolua_function(tolua_S,"removeChild",tolua_Ogre_Ogre_Node_removeChild01);
    tolua_function(tolua_S,"removeChild",tolua_Ogre_Ogre_Node_removeChild02);
    tolua_function(tolua_S,"removeAllChildren",tolua_Ogre_Ogre_Node_removeAllChildren00);
    tolua_function(tolua_S,"_getDerivedOrientation",tolua_Ogre_Ogre_Node__getDerivedOrientation00);
    tolua_function(tolua_S,"_getDerivedPosition",tolua_Ogre_Ogre_Node__getDerivedPosition00);
    tolua_function(tolua_S,"_getDerivedScale",tolua_Ogre_Ogre_Node__getDerivedScale00);
    tolua_function(tolua_S,"_getFullTransform",tolua_Ogre_Ogre_Node__getFullTransform00);
    tolua_function(tolua_S,"_update",tolua_Ogre_Ogre_Node__update00);
    tolua_function(tolua_S,"getMaterial",tolua_Ogre_Ogre_Node_getMaterial00);
    tolua_function(tolua_S,"getWorldTransforms",tolua_Ogre_Ogre_Node_getWorldTransforms00);
    tolua_function(tolua_S,"_getDerivedOrientation",tolua_Ogre_Ogre_Node__getDerivedOrientation01);
    tolua_function(tolua_S,"_getDerivedPosition",tolua_Ogre_Ogre_Node__getDerivedPosition01);
    tolua_function(tolua_S,"setInitialState",tolua_Ogre_Ogre_Node_setInitialState00);
    tolua_function(tolua_S,"resetToInitialState",tolua_Ogre_Ogre_Node_resetToInitialState00);
    tolua_function(tolua_S,"getInitialPosition",tolua_Ogre_Ogre_Node_getInitialPosition00);
    tolua_function(tolua_S,"getInitialOrientation",tolua_Ogre_Ogre_Node_getInitialOrientation00);
    tolua_function(tolua_S,"getInitialScale",tolua_Ogre_Ogre_Node_getInitialScale00);
    tolua_function(tolua_S,"getSquaredViewDepth",tolua_Ogre_Ogre_Node_getSquaredViewDepth00);
    tolua_function(tolua_S,"needUpdate",tolua_Ogre_Ogre_Node_needUpdate00);
    tolua_function(tolua_S,"requestUpdate",tolua_Ogre_Ogre_Node_requestUpdate00);
    tolua_function(tolua_S,"cancelUpdate",tolua_Ogre_Ogre_Node_cancelUpdate00);
    tolua_function(tolua_S,"queueNeedUpdate",tolua_Ogre_Ogre_Node_queueNeedUpdate00);
    tolua_function(tolua_S,"processQueuedUpdates",tolua_Ogre_Ogre_Node_processQueuedUpdates00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"Ogre",0);
  tolua_beginmodule(tolua_S,"Ogre");
   tolua_cclass(tolua_S,"VisibleObjectsBoundsInfo","Ogre::VisibleObjectsBoundsInfo","",NULL);
   tolua_beginmodule(tolua_S,"VisibleObjectsBoundsInfo");
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"SceneNode","Ogre::SceneNode","Ogre::Node",NULL);
   tolua_beginmodule(tolua_S,"SceneNode");
    tolua_function(tolua_S,"attachObject",tolua_Ogre_Ogre_SceneNode_attachObject00);
    tolua_function(tolua_S,"numAttachedObjects",tolua_Ogre_Ogre_SceneNode_numAttachedObjects00);
    tolua_function(tolua_S,"getAttachedObject",tolua_Ogre_Ogre_SceneNode_getAttachedObject00);
    tolua_function(tolua_S,"getAttachedObject",tolua_Ogre_Ogre_SceneNode_getAttachedObject01);
    tolua_function(tolua_S,"detachObject",tolua_Ogre_Ogre_SceneNode_detachObject00);
    tolua_function(tolua_S,"detachObject",tolua_Ogre_Ogre_SceneNode_detachObject01);
    tolua_function(tolua_S,"detachObject",tolua_Ogre_Ogre_SceneNode_detachObject02);
    tolua_function(tolua_S,"detachAllObjects",tolua_Ogre_Ogre_SceneNode_detachAllObjects00);
    tolua_function(tolua_S,"isInSceneGraph",tolua_Ogre_Ogre_SceneNode_isInSceneGraph00);
    tolua_function(tolua_S,"_notifyRootNode",tolua_Ogre_Ogre_SceneNode__notifyRootNode00);
    tolua_function(tolua_S,"_update",tolua_Ogre_Ogre_SceneNode__update00);
    tolua_function(tolua_S,"_updateBounds",tolua_Ogre_Ogre_SceneNode__updateBounds00);
    tolua_function(tolua_S,"_getWorldAABB",tolua_Ogre_Ogre_SceneNode__getWorldAABB00);
    tolua_function(tolua_S,"getCreator",tolua_Ogre_Ogre_SceneNode_getCreator00);
    tolua_function(tolua_S,"removeAndDestroyChild",tolua_Ogre_Ogre_SceneNode_removeAndDestroyChild00);
    tolua_function(tolua_S,"removeAndDestroyChild",tolua_Ogre_Ogre_SceneNode_removeAndDestroyChild01);
    tolua_function(tolua_S,"removeAndDestroyAllChildren",tolua_Ogre_Ogre_SceneNode_removeAndDestroyAllChildren00);
    tolua_function(tolua_S,"showBoundingBox",tolua_Ogre_Ogre_SceneNode_showBoundingBox00);
    tolua_function(tolua_S,"getShowBoundingBox",tolua_Ogre_Ogre_SceneNode_getShowBoundingBox00);
    tolua_function(tolua_S,"createChildSceneNode",tolua_Ogre_Ogre_SceneNode_createChildSceneNode00);
    tolua_function(tolua_S,"createChildSceneNode",tolua_Ogre_Ogre_SceneNode_createChildSceneNode01);
    tolua_function(tolua_S,"createChildSceneNode",tolua_Ogre_Ogre_SceneNode_createChildSceneNode02);
    tolua_function(tolua_S,"createChildSceneNode",tolua_Ogre_Ogre_SceneNode_createChildSceneNode03);
    tolua_function(tolua_S,"createChildSceneNode",tolua_Ogre_Ogre_SceneNode_createChildSceneNode04);
    tolua_function(tolua_S,"createChildSceneNode",tolua_Ogre_Ogre_SceneNode_createChildSceneNode05);
    tolua_function(tolua_S,"setFixedYawAxis",tolua_Ogre_Ogre_SceneNode_setFixedYawAxis00);
    tolua_function(tolua_S,"setFixedYawAxis",tolua_Ogre_Ogre_SceneNode_setFixedYawAxis01);
    tolua_function(tolua_S,"yaw",tolua_Ogre_Ogre_SceneNode_yaw00);
    tolua_function(tolua_S,"setDirection",tolua_Ogre_Ogre_SceneNode_setDirection00);
    tolua_function(tolua_S,"setDirection",tolua_Ogre_Ogre_SceneNode_setDirection01);
    tolua_function(tolua_S,"setDirection",tolua_Ogre_Ogre_SceneNode_setDirection02);
    tolua_function(tolua_S,"setDirection",tolua_Ogre_Ogre_SceneNode_setDirection03);
    tolua_function(tolua_S,"lookAt",tolua_Ogre_Ogre_SceneNode_lookAt00);
    tolua_function(tolua_S,"lookAt",tolua_Ogre_Ogre_SceneNode_lookAt01);
    tolua_function(tolua_S,"setAutoTracking",tolua_Ogre_Ogre_SceneNode_setAutoTracking00);
    tolua_function(tolua_S,"setAutoTracking",tolua_Ogre_Ogre_SceneNode_setAutoTracking01);
    tolua_function(tolua_S,"setAutoTracking",tolua_Ogre_Ogre_SceneNode_setAutoTracking02);
    tolua_function(tolua_S,"getAutoTrackTarget",tolua_Ogre_Ogre_SceneNode_getAutoTrackTarget00);
    tolua_function(tolua_S,"getAutoTrackOffset",tolua_Ogre_Ogre_SceneNode_getAutoTrackOffset00);
    tolua_function(tolua_S,"getAutoTrackLocalDirection",tolua_Ogre_Ogre_SceneNode_getAutoTrackLocalDirection00);
    tolua_function(tolua_S,"getParentSceneNode",tolua_Ogre_Ogre_SceneNode_getParentSceneNode00);
    tolua_function(tolua_S,"setVisible",tolua_Ogre_Ogre_SceneNode_setVisible00);
    tolua_function(tolua_S,"flipVisibility",tolua_Ogre_Ogre_SceneNode_flipVisibility00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"Ogre",0);
  tolua_beginmodule(tolua_S,"Ogre");
   tolua_constant(tolua_S,"TU_STATIC",Ogre::TU_STATIC);
   tolua_constant(tolua_S,"TU_DYNAMIC",Ogre::TU_DYNAMIC);
   tolua_constant(tolua_S,"TU_WRITE_ONLY",Ogre::TU_WRITE_ONLY);
   tolua_constant(tolua_S,"TU_STATIC_WRITE_ONLY",Ogre::TU_STATIC_WRITE_ONLY);
   tolua_constant(tolua_S,"TU_DYNAMIC_WRITE_ONLY",Ogre::TU_DYNAMIC_WRITE_ONLY);
   tolua_constant(tolua_S,"TU_DYNAMIC_WRITE_ONLY_DISCARDABLE",Ogre::TU_DYNAMIC_WRITE_ONLY_DISCARDABLE);
   tolua_constant(tolua_S,"TU_AUTOMIPMAP",Ogre::TU_AUTOMIPMAP);
   tolua_constant(tolua_S,"TU_RENDERTARGET",Ogre::TU_RENDERTARGET);
   tolua_constant(tolua_S,"TU_DEFAULT",Ogre::TU_DEFAULT);
   tolua_constant(tolua_S,"TEX_TYPE_1D",Ogre::TEX_TYPE_1D);
   tolua_constant(tolua_S,"TEX_TYPE_2D",Ogre::TEX_TYPE_2D);
   tolua_constant(tolua_S,"TEX_TYPE_3D",Ogre::TEX_TYPE_3D);
   tolua_constant(tolua_S,"TEX_TYPE_CUBE_MAP",Ogre::TEX_TYPE_CUBE_MAP);
   tolua_constant(tolua_S,"MIP_UNLIMITED",Ogre::MIP_UNLIMITED);
   tolua_constant(tolua_S,"MIP_DEFAULT",Ogre::MIP_DEFAULT);
   tolua_cclass(tolua_S,"TexturePtr","Ogre::TexturePtr","",NULL);
   tolua_beginmodule(tolua_S,"TexturePtr");
    tolua_function(tolua_S,"get",tolua_Ogre_Ogre_TexturePtr_get00);
    tolua_function(tolua_S,"isNull",tolua_Ogre_Ogre_TexturePtr_isNull00);
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"Texture","Ogre::Texture","Ogre::Resource",NULL);
   tolua_beginmodule(tolua_S,"Texture");
    tolua_function(tolua_S,"setTextureType",tolua_Ogre_Ogre_Texture_setTextureType00);
    tolua_function(tolua_S,"getTextureType",tolua_Ogre_Ogre_Texture_getTextureType00);
    tolua_function(tolua_S,"getNumMipmaps",tolua_Ogre_Ogre_Texture_getNumMipmaps00);
    tolua_function(tolua_S,"setNumMipmaps",tolua_Ogre_Ogre_Texture_setNumMipmaps00);
    tolua_function(tolua_S,"getMipmapsHardwareGenerated",tolua_Ogre_Ogre_Texture_getMipmapsHardwareGenerated00);
    tolua_function(tolua_S,"getGamma",tolua_Ogre_Ogre_Texture_getGamma00);
    tolua_function(tolua_S,"setGamma",tolua_Ogre_Ogre_Texture_setGamma00);
    tolua_function(tolua_S,"getHeight",tolua_Ogre_Ogre_Texture_getHeight00);
    tolua_function(tolua_S,"getWidth",tolua_Ogre_Ogre_Texture_getWidth00);
    tolua_function(tolua_S,"getDepth",tolua_Ogre_Ogre_Texture_getDepth00);
    tolua_function(tolua_S,"getSrcHeight",tolua_Ogre_Ogre_Texture_getSrcHeight00);
    tolua_function(tolua_S,"getSrcWidth",tolua_Ogre_Ogre_Texture_getSrcWidth00);
    tolua_function(tolua_S,"getSrcDepth",tolua_Ogre_Ogre_Texture_getSrcDepth00);
    tolua_function(tolua_S,"setHeight",tolua_Ogre_Ogre_Texture_setHeight00);
    tolua_function(tolua_S,"setWidth",tolua_Ogre_Ogre_Texture_setWidth00);
    tolua_function(tolua_S,"setDepth",tolua_Ogre_Ogre_Texture_setDepth00);
    tolua_function(tolua_S,"getUsage",tolua_Ogre_Ogre_Texture_getUsage00);
    tolua_function(tolua_S,"setUsage",tolua_Ogre_Ogre_Texture_setUsage00);
    tolua_function(tolua_S,"copyToTexture",tolua_Ogre_Ogre_Texture_copyToTexture00);
    tolua_function(tolua_S,"loadImage",tolua_Ogre_Ogre_Texture_loadImage00);
    tolua_function(tolua_S,"hasAlpha",tolua_Ogre_Ogre_Texture_hasAlpha00);
    tolua_function(tolua_S,"setDesiredIntegerBitDepth",tolua_Ogre_Ogre_Texture_setDesiredIntegerBitDepth00);
    tolua_function(tolua_S,"getDesiredIntegerBitDepth",tolua_Ogre_Ogre_Texture_getDesiredIntegerBitDepth00);
    tolua_function(tolua_S,"setDesiredFloatBitDepth",tolua_Ogre_Ogre_Texture_setDesiredFloatBitDepth00);
    tolua_function(tolua_S,"getDesiredFloatBitDepth",tolua_Ogre_Ogre_Texture_getDesiredFloatBitDepth00);
    tolua_function(tolua_S,"setDesiredBitDepths",tolua_Ogre_Ogre_Texture_setDesiredBitDepths00);
    tolua_function(tolua_S,"setTreatLuminanceAsAlpha",tolua_Ogre_Ogre_Texture_setTreatLuminanceAsAlpha00);
    tolua_function(tolua_S,"getTreatLuminanceAsAlpha",tolua_Ogre_Ogre_Texture_getTreatLuminanceAsAlpha00);
    tolua_function(tolua_S,"getNumFaces",tolua_Ogre_Ogre_Texture_getNumFaces00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"Ogre",0);
  tolua_beginmodule(tolua_S,"Ogre");
   tolua_cclass(tolua_S,"TextureManager","Ogre::TextureManager","Ogre::ResourceManager",NULL);
   tolua_beginmodule(tolua_S,"TextureManager");
    tolua_function(tolua_S,"setDefaultNumMipmaps",tolua_Ogre_Ogre_TextureManager_setDefaultNumMipmaps00);
    tolua_function(tolua_S,"getDefaultNumMipmaps",tolua_Ogre_Ogre_TextureManager_getDefaultNumMipmaps00);
    tolua_function(tolua_S,"getSingleton",tolua_Ogre_Ogre_TextureManager_getSingleton00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"Ogre",0);
  tolua_beginmodule(tolua_S,"Ogre");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"AxisAlignedBox","Ogre::AxisAlignedBox","",tolua_collect_Ogre__AxisAlignedBox);
   #else
   tolua_cclass(tolua_S,"AxisAlignedBox","Ogre::AxisAlignedBox","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"AxisAlignedBox");
    tolua_constant(tolua_S,"FAR_LEFT_BOTTOM",Ogre::AxisAlignedBox::FAR_LEFT_BOTTOM);
    tolua_constant(tolua_S,"FAR_LEFT_TOP",Ogre::AxisAlignedBox::FAR_LEFT_TOP);
    tolua_constant(tolua_S,"FAR_RIGHT_TOP",Ogre::AxisAlignedBox::FAR_RIGHT_TOP);
    tolua_constant(tolua_S,"FAR_RIGHT_BOTTOM",Ogre::AxisAlignedBox::FAR_RIGHT_BOTTOM);
    tolua_constant(tolua_S,"NEAR_RIGHT_BOTTOM",Ogre::AxisAlignedBox::NEAR_RIGHT_BOTTOM);
    tolua_constant(tolua_S,"NEAR_LEFT_BOTTOM",Ogre::AxisAlignedBox::NEAR_LEFT_BOTTOM);
    tolua_constant(tolua_S,"NEAR_LEFT_TOP",Ogre::AxisAlignedBox::NEAR_LEFT_TOP);
    tolua_constant(tolua_S,"NEAR_RIGHT_TOP",Ogre::AxisAlignedBox::NEAR_RIGHT_TOP);
    tolua_function(tolua_S,"new",tolua_Ogre_Ogre_AxisAlignedBox_new00);
    tolua_function(tolua_S,"new_local",tolua_Ogre_Ogre_AxisAlignedBox_new00_local);
    tolua_function(tolua_S,".call",tolua_Ogre_Ogre_AxisAlignedBox_new00_local);
    tolua_function(tolua_S,"new",tolua_Ogre_Ogre_AxisAlignedBox_new01);
    tolua_function(tolua_S,"new_local",tolua_Ogre_Ogre_AxisAlignedBox_new01_local);
    tolua_function(tolua_S,".call",tolua_Ogre_Ogre_AxisAlignedBox_new01_local);
    tolua_function(tolua_S,"new",tolua_Ogre_Ogre_AxisAlignedBox_new02);
    tolua_function(tolua_S,"new_local",tolua_Ogre_Ogre_AxisAlignedBox_new02_local);
    tolua_function(tolua_S,".call",tolua_Ogre_Ogre_AxisAlignedBox_new02_local);
    tolua_function(tolua_S,"new",tolua_Ogre_Ogre_AxisAlignedBox_new03);
    tolua_function(tolua_S,"new_local",tolua_Ogre_Ogre_AxisAlignedBox_new03_local);
    tolua_function(tolua_S,".call",tolua_Ogre_Ogre_AxisAlignedBox_new03_local);
    tolua_function(tolua_S,"delete",tolua_Ogre_Ogre_AxisAlignedBox_delete00);
    tolua_function(tolua_S,"getMinimum",tolua_Ogre_Ogre_AxisAlignedBox_getMinimum00);
    tolua_function(tolua_S,"getMinimum",tolua_Ogre_Ogre_AxisAlignedBox_getMinimum01);
    tolua_function(tolua_S,"getMaximum",tolua_Ogre_Ogre_AxisAlignedBox_getMaximum00);
    tolua_function(tolua_S,"getMaximum",tolua_Ogre_Ogre_AxisAlignedBox_getMaximum01);
    tolua_function(tolua_S,"setMinimum",tolua_Ogre_Ogre_AxisAlignedBox_setMinimum00);
    tolua_function(tolua_S,"setMinimum",tolua_Ogre_Ogre_AxisAlignedBox_setMinimum01);
    tolua_function(tolua_S,"setMinimumX",tolua_Ogre_Ogre_AxisAlignedBox_setMinimumX00);
    tolua_function(tolua_S,"setMinimumY",tolua_Ogre_Ogre_AxisAlignedBox_setMinimumY00);
    tolua_function(tolua_S,"setMinimumZ",tolua_Ogre_Ogre_AxisAlignedBox_setMinimumZ00);
    tolua_function(tolua_S,"setMaximum",tolua_Ogre_Ogre_AxisAlignedBox_setMaximum00);
    tolua_function(tolua_S,"setMaximum",tolua_Ogre_Ogre_AxisAlignedBox_setMaximum01);
    tolua_function(tolua_S,"setMaximumX",tolua_Ogre_Ogre_AxisAlignedBox_setMaximumX00);
    tolua_function(tolua_S,"setMaximumY",tolua_Ogre_Ogre_AxisAlignedBox_setMaximumY00);
    tolua_function(tolua_S,"setMaximumZ",tolua_Ogre_Ogre_AxisAlignedBox_setMaximumZ00);
    tolua_function(tolua_S,"setExtents",tolua_Ogre_Ogre_AxisAlignedBox_setExtents00);
    tolua_function(tolua_S,"setExtents",tolua_Ogre_Ogre_AxisAlignedBox_setExtents01);
    tolua_function(tolua_S,"getCorner",tolua_Ogre_Ogre_AxisAlignedBox_getCorner00);
    tolua_function(tolua_S,"merge",tolua_Ogre_Ogre_AxisAlignedBox_merge00);
    tolua_function(tolua_S,"merge",tolua_Ogre_Ogre_AxisAlignedBox_merge01);
    tolua_function(tolua_S,"transform",tolua_Ogre_Ogre_AxisAlignedBox_transform00);
    tolua_function(tolua_S,"transformAffine",tolua_Ogre_Ogre_AxisAlignedBox_transformAffine00);
    tolua_function(tolua_S,"setNull",tolua_Ogre_Ogre_AxisAlignedBox_setNull00);
    tolua_function(tolua_S,"isNull",tolua_Ogre_Ogre_AxisAlignedBox_isNull00);
    tolua_function(tolua_S,"isFinite",tolua_Ogre_Ogre_AxisAlignedBox_isFinite00);
    tolua_function(tolua_S,"setInfinite",tolua_Ogre_Ogre_AxisAlignedBox_setInfinite00);
    tolua_function(tolua_S,"isInfinite",tolua_Ogre_Ogre_AxisAlignedBox_isInfinite00);
    tolua_function(tolua_S,"intersects",tolua_Ogre_Ogre_AxisAlignedBox_intersects00);
    tolua_function(tolua_S,"intersection",tolua_Ogre_Ogre_AxisAlignedBox_intersection00);
    tolua_function(tolua_S,"volume",tolua_Ogre_Ogre_AxisAlignedBox_volume00);
    tolua_function(tolua_S,"scale",tolua_Ogre_Ogre_AxisAlignedBox_scale00);
    tolua_function(tolua_S,"intersects",tolua_Ogre_Ogre_AxisAlignedBox_intersects01);
    tolua_function(tolua_S,"intersects",tolua_Ogre_Ogre_AxisAlignedBox_intersects02);
    tolua_function(tolua_S,"intersects",tolua_Ogre_Ogre_AxisAlignedBox_intersects03);
    tolua_function(tolua_S,"getCenter",tolua_Ogre_Ogre_AxisAlignedBox_getCenter00);
    tolua_function(tolua_S,"getSize",tolua_Ogre_Ogre_AxisAlignedBox_getSize00);
    tolua_function(tolua_S,"getHalfSize",tolua_Ogre_Ogre_AxisAlignedBox_getHalfSize00);
    tolua_function(tolua_S,"contains",tolua_Ogre_Ogre_AxisAlignedBox_contains00);
    tolua_function(tolua_S,"contains",tolua_Ogre_Ogre_AxisAlignedBox_contains01);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"Ogre",0);
  tolua_beginmodule(tolua_S,"Ogre");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"ColourValue","Ogre::ColourValue","",tolua_collect_Ogre__ColourValue);
   #else
   tolua_cclass(tolua_S,"ColourValue","Ogre::ColourValue","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"ColourValue");
    tolua_variable(tolua_S,"ZERO",tolua_get_Ogre__ColourValue_ZERO,NULL);
    tolua_variable(tolua_S,"Black",tolua_get_Ogre__ColourValue_Black,NULL);
    tolua_variable(tolua_S,"White",tolua_get_Ogre__ColourValue_White,NULL);
    tolua_variable(tolua_S,"Red",tolua_get_Ogre__ColourValue_Red,NULL);
    tolua_variable(tolua_S,"Green",tolua_get_Ogre__ColourValue_Green,NULL);
    tolua_variable(tolua_S,"Blue",tolua_get_Ogre__ColourValue_Blue,NULL);
    tolua_function(tolua_S,"new",tolua_Ogre_Ogre_ColourValue_new00);
    tolua_function(tolua_S,"new_local",tolua_Ogre_Ogre_ColourValue_new00_local);
    tolua_function(tolua_S,".call",tolua_Ogre_Ogre_ColourValue_new00_local);
    tolua_variable(tolua_S,"r",tolua_get_Ogre__ColourValue_r,tolua_set_Ogre__ColourValue_r);
    tolua_variable(tolua_S,"g",tolua_get_Ogre__ColourValue_g,tolua_set_Ogre__ColourValue_g);
    tolua_variable(tolua_S,"b",tolua_get_Ogre__ColourValue_b,tolua_set_Ogre__ColourValue_b);
    tolua_variable(tolua_S,"a",tolua_get_Ogre__ColourValue_a,tolua_set_Ogre__ColourValue_a);
    tolua_function(tolua_S,"getAsRGBA",tolua_Ogre_Ogre_ColourValue_getAsRGBA00);
    tolua_function(tolua_S,"getAsARGB",tolua_Ogre_Ogre_ColourValue_getAsARGB00);
    tolua_function(tolua_S,"getAsBGRA",tolua_Ogre_Ogre_ColourValue_getAsBGRA00);
    tolua_function(tolua_S,"getAsABGR",tolua_Ogre_Ogre_ColourValue_getAsABGR00);
    tolua_function(tolua_S,"setAsRGBA",tolua_Ogre_Ogre_ColourValue_setAsRGBA00);
    tolua_function(tolua_S,"setAsARGB",tolua_Ogre_Ogre_ColourValue_setAsARGB00);
    tolua_function(tolua_S,"setAsBGRA",tolua_Ogre_Ogre_ColourValue_setAsBGRA00);
    tolua_function(tolua_S,"setAsABGR",tolua_Ogre_Ogre_ColourValue_setAsABGR00);
    tolua_function(tolua_S,"saturate",tolua_Ogre_Ogre_ColourValue_saturate00);
    tolua_function(tolua_S,"saturateCopy",tolua_Ogre_Ogre_ColourValue_saturateCopy00);
    tolua_function(tolua_S,".add",tolua_Ogre_Ogre_ColourValue__add00);
    tolua_function(tolua_S,".sub",tolua_Ogre_Ogre_ColourValue__sub00);
    tolua_function(tolua_S,".div",tolua_Ogre_Ogre_ColourValue__div00);
    tolua_function(tolua_S,".div",tolua_Ogre_Ogre_ColourValue__div01);
    tolua_function(tolua_S,"setHSB",tolua_Ogre_Ogre_ColourValue_setHSB00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"Ogre",0);
  tolua_beginmodule(tolua_S,"Ogre");
   tolua_cclass(tolua_S,"GpuProgramManager","Ogre::GpuProgramManager","Ogre::ResourceManager",NULL);
   tolua_beginmodule(tolua_S,"GpuProgramManager");
    tolua_function(tolua_S,"isSyntaxSupported",tolua_Ogre_Ogre_GpuProgramManager_isSyntaxSupported00);
    tolua_function(tolua_S,"getByName",tolua_Ogre_Ogre_GpuProgramManager_getByName00);
    tolua_function(tolua_S,"getSingleton",tolua_Ogre_Ogre_GpuProgramManager_getSingleton00);
    tolua_function(tolua_S,"getSingletonPtr",tolua_Ogre_Ogre_GpuProgramManager_getSingletonPtr00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"Ogre",0);
  tolua_beginmodule(tolua_S,"Ogre");
   tolua_cclass(tolua_S,"GpuProgram","Ogre::GpuProgram","Ogre::Resource",NULL);
   tolua_beginmodule(tolua_S,"GpuProgram");
    tolua_function(tolua_S,"setSourceFile",tolua_Ogre_Ogre_GpuProgram_setSourceFile00);
    tolua_function(tolua_S,"setSource",tolua_Ogre_Ogre_GpuProgram_setSource00);
    tolua_function(tolua_S,"getSyntaxCode",tolua_Ogre_Ogre_GpuProgram_getSyntaxCode00);
    tolua_function(tolua_S,"setSyntaxCode",tolua_Ogre_Ogre_GpuProgram_setSyntaxCode00);
    tolua_function(tolua_S,"getSourceFile",tolua_Ogre_Ogre_GpuProgram_getSourceFile00);
    tolua_function(tolua_S,"getSource",tolua_Ogre_Ogre_GpuProgram_getSource00);
    tolua_function(tolua_S,"isSupported",tolua_Ogre_Ogre_GpuProgram_isSupported00);
    tolua_function(tolua_S,"setSkeletalAnimationIncluded",tolua_Ogre_Ogre_GpuProgram_setSkeletalAnimationIncluded00);
    tolua_function(tolua_S,"isSkeletalAnimationIncluded",tolua_Ogre_Ogre_GpuProgram_isSkeletalAnimationIncluded00);
    tolua_function(tolua_S,"setMorphAnimationIncluded",tolua_Ogre_Ogre_GpuProgram_setMorphAnimationIncluded00);
    tolua_function(tolua_S,"setPoseAnimationIncluded",tolua_Ogre_Ogre_GpuProgram_setPoseAnimationIncluded00);
    tolua_function(tolua_S,"isMorphAnimationIncluded",tolua_Ogre_Ogre_GpuProgram_isMorphAnimationIncluded00);
    tolua_function(tolua_S,"isPoseAnimationIncluded",tolua_Ogre_Ogre_GpuProgram_isPoseAnimationIncluded00);
    tolua_function(tolua_S,"getNumberOfPosesIncluded",tolua_Ogre_Ogre_GpuProgram_getNumberOfPosesIncluded00);
    tolua_function(tolua_S,"setVertexTextureFetchRequired",tolua_Ogre_Ogre_GpuProgram_setVertexTextureFetchRequired00);
    tolua_function(tolua_S,"isVertexTextureFetchRequired",tolua_Ogre_Ogre_GpuProgram_isVertexTextureFetchRequired00);
    tolua_function(tolua_S,"setAdjacencyInfoRequired",tolua_Ogre_Ogre_GpuProgram_setAdjacencyInfoRequired00);
    tolua_function(tolua_S,"isAdjacencyInfoRequired",tolua_Ogre_Ogre_GpuProgram_isAdjacencyInfoRequired00);
    tolua_function(tolua_S,"hasDefaultParameters",tolua_Ogre_Ogre_GpuProgram_hasDefaultParameters00);
    tolua_function(tolua_S,"setSurfaceAndPassLightStates",tolua_Ogre_Ogre_GpuProgram_setSurfaceAndPassLightStates00);
    tolua_function(tolua_S,"getPassSurfaceAndLightStates",tolua_Ogre_Ogre_GpuProgram_getPassSurfaceAndLightStates00);
    tolua_function(tolua_S,"getLanguage",tolua_Ogre_Ogre_GpuProgram_getLanguage00);
    tolua_function(tolua_S,"hasCompileError",tolua_Ogre_Ogre_GpuProgram_hasCompileError00);
    tolua_function(tolua_S,"resetCompileError",tolua_Ogre_Ogre_GpuProgram_resetCompileError00);
    tolua_function(tolua_S,"setManualNamedConstantsFile",tolua_Ogre_Ogre_GpuProgram_setManualNamedConstantsFile00);
    tolua_function(tolua_S,"getManualNamedConstantsFile",tolua_Ogre_Ogre_GpuProgram_getManualNamedConstantsFile00);
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"GpuProgramPtr","Ogre::GpuProgramPtr","",NULL);
   tolua_beginmodule(tolua_S,"GpuProgramPtr");
    tolua_function(tolua_S,"get",tolua_Ogre_Ogre_GpuProgramPtr_get00);
    tolua_function(tolua_S,"isNull",tolua_Ogre_Ogre_GpuProgramPtr_isNull00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"Ogre",0);
  tolua_beginmodule(tolua_S,"Ogre");
   tolua_cclass(tolua_S,"HighLevelGpuProgramManager","Ogre::HighLevelGpuProgramManager","Ogre::ResourceManager",NULL);
   tolua_beginmodule(tolua_S,"HighLevelGpuProgramManager");
    tolua_function(tolua_S,"getSingleton",tolua_Ogre_Ogre_HighLevelGpuProgramManager_getSingleton00);
    tolua_function(tolua_S,"getSingletonPtr",tolua_Ogre_Ogre_HighLevelGpuProgramManager_getSingletonPtr00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"Ogre",0);
  tolua_beginmodule(tolua_S,"Ogre");
   tolua_cclass(tolua_S,"HighLevelGpuProgram","Ogre::HighLevelGpuProgram","Ogre::GpuProgram",NULL);
   tolua_beginmodule(tolua_S,"HighLevelGpuProgram");
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"HighLevelGpuProgramPtr","Ogre::HighLevelGpuProgramPtr","",NULL);
   tolua_beginmodule(tolua_S,"HighLevelGpuProgramPtr");
    tolua_function(tolua_S,"get",tolua_Ogre_Ogre_HighLevelGpuProgramPtr_get00);
    tolua_function(tolua_S,"isNull",tolua_Ogre_Ogre_HighLevelGpuProgramPtr_isNull00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"Ogre",0);
  tolua_beginmodule(tolua_S,"Ogre");
   tolua_cclass(tolua_S,"Root","Ogre::Root","",NULL);
   tolua_beginmodule(tolua_S,"Root");
    tolua_function(tolua_S,"saveConfig",tolua_Ogre_Ogre_Root_saveConfig00);
    tolua_function(tolua_S,"restoreConfig",tolua_Ogre_Ogre_Root_restoreConfig00);
    tolua_function(tolua_S,"showConfigDialog",tolua_Ogre_Ogre_Root_showConfigDialog00);
    tolua_function(tolua_S,"isInitialised",tolua_Ogre_Ogre_Root_isInitialised00);
    tolua_function(tolua_S,"getTextureManager",tolua_Ogre_Ogre_Root_getTextureManager00);
    tolua_function(tolua_S,"getMeshManager",tolua_Ogre_Ogre_Root_getMeshManager00);
    tolua_function(tolua_S,"getErrorDescription",tolua_Ogre_Ogre_Root_getErrorDescription00);
    tolua_function(tolua_S,"addFrameListener",tolua_Ogre_Ogre_Root_addFrameListener00);
    tolua_function(tolua_S,"removeFrameListener",tolua_Ogre_Ogre_Root_removeFrameListener00);
    tolua_function(tolua_S,"convertColourValue",tolua_Ogre_Ogre_Root_convertColourValue00);
    tolua_function(tolua_S,"getTimer",tolua_Ogre_Ogre_Root_getTimer00);
    tolua_function(tolua_S,"getNextFrameNumber",tolua_Ogre_Ogre_Root_getNextFrameNumber00);
    tolua_function(tolua_S,"getSingleton",tolua_Ogre_Ogre_Root_getSingleton00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_Ogre (lua_State* tolua_S) {
 return tolua_Ogre_open(tolua_S);
};
#endif

