/*
** Lua binding: Eris
** Generated automatically by tolua++-1.0.92.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "components/lua/tolua++.h"

/* Exported function */
TOLUA_API int  tolua_Eris_open (lua_State* tolua_S);

#include "required.h"
#include <Eris/Types.h>
#include <Eris/Entity.h>
#include <Eris/ServerInfo.h>
#include <Eris/Metaserver.h>
#include <Eris/Connection.h>
#include <Eris/Account.h>
#include <Eris/TypeInfo.h>
#include <Eris/TypeService.h>
#include <Eris/Avatar.h>

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_WFMath__Point_3_ (lua_State* tolua_S)
{
 WFMath::Point<3>* self = (WFMath::Point<3>*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_sigc__connection (lua_State* tolua_S)
{
 sigc::connection* self = (sigc::connection*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_WFMath__Vector_3_ (lua_State* tolua_S)
{
 WFMath::Vector<3>* self = (WFMath::Vector<3>*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_WFMath__Quaternion (lua_State* tolua_S)
{
 WFMath::Quaternion* self = (WFMath::Quaternion*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"WFMath::Point<3>");
 tolua_usertype(tolua_S,"WFMath::AxisBox<3>");
 tolua_usertype(tolua_S,"WFMath::Vector<3>");
 tolua_usertype(tolua_S,"sigc::signal<void,const Eris::ServerInfo&>");
 tolua_usertype(tolua_S,"Eris::TypeService");
 tolua_usertype(tolua_S,"sigc::signal<void,const Atlas::Objects::Operation::RootOperation&>");
 tolua_usertype(tolua_S,"Atlas::Objects::Root");
 tolua_usertype(tolua_S,"Eris::Account");
 tolua_usertype(tolua_S,"sigc::signal<void,Eris::Task*>");
 tolua_usertype(tolua_S,"sigc::signal<void,const Atlas::Objects::Root&>");
 tolua_usertype(tolua_S,"sigc::signal<void,int>");
 tolua_usertype(tolua_S,"sigc::slot<void,const Atlas::Message::Element&>");
 tolua_usertype(tolua_S,"WFMath::Quaternion");
 tolua_usertype(tolua_S,"std::set<Eris::TypeInfo*>");
 tolua_usertype(tolua_S,"Eris::Meta");
 tolua_usertype(tolua_S,"Eris::Connection");
 tolua_usertype(tolua_S,"sigc::signal<void,Eris::Entity*>");
 tolua_usertype(tolua_S,"Eris::Task");
 tolua_usertype(tolua_S,"Eris::Avatar");
 tolua_usertype(tolua_S,"Eris::ServerInfo");
 tolua_usertype(tolua_S,"Eris::Entity");
 tolua_usertype(tolua_S,"sigc::signal<void,Eris::TypeInfo*>");
 tolua_usertype(tolua_S,"std::vector<Eris::Task*>");
 tolua_usertype(tolua_S,"sigc::signal<void,const std::string&>");
 tolua_usertype(tolua_S,"Eris::TypeInfo");
 tolua_usertype(tolua_S,"Eris::View");
 tolua_usertype(tolua_S,"virtualsigc::trackable");
 tolua_usertype(tolua_S,"Atlas::Message::MapType");
 tolua_usertype(tolua_S,"sigc::connection");
 tolua_usertype(tolua_S,"sigc::signal<void>");
 tolua_usertype(tolua_S,"sigc::signal<void,bool>");
 tolua_usertype(tolua_S,"Atlas::Message::Element");
 tolua_usertype(tolua_S,"sigc::signal<void,const std::set<std::string>&>");
}

/* method: numContained of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_Entity_numContained00
static int tolua_Eris_Eris_Entity_numContained00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::Entity* self = (const Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'numContained'",NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->numContained();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'numContained'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getContained of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_Entity_getContained00
static int tolua_Eris_Eris_Entity_getContained00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::Entity",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::Entity* self = (const Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
  unsigned int index = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getContained'",NULL);
#endif
  {
   Eris::Entity* tolua_ret = (Eris::Entity*)  self->getContained(index);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Eris::Entity");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getContained'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: valueOfAttr of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_Entity_valueOfAttr00
static int tolua_Eris_Eris_Entity_valueOfAttr00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::Entity",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::Entity* self = (const Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
  const std::string attr = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'valueOfAttr'",NULL);
#endif
  {
   const Atlas::Message::Element& tolua_ret = (const Atlas::Message::Element&)  self->valueOfAttr(attr);
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Atlas::Message::Element");
   tolua_pushcppstring(tolua_S,(const char*)attr);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'valueOfAttr'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: hasAttr of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_Entity_hasAttr00
static int tolua_Eris_Eris_Entity_hasAttr00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::Entity",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::Entity* self = (const Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
  const std::string p = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hasAttr'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->hasAttr(p);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)p);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'hasAttr'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: observe of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_Entity_observe00
static int tolua_Eris_Eris_Entity_observe00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Eris::Entity",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const sigc::slot<void,const Atlas::Message::Element&>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Eris::Entity* self = (Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
  const std::string attr = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const sigc::slot<void,const Atlas::Message::Element&>* aslot = ((const sigc::slot<void,const Atlas::Message::Element&>*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'observe'",NULL);
#endif
  {
   sigc::connection tolua_ret = (sigc::connection)  self->observe(attr,*aslot);
   {
#ifdef __cplusplus
    void* tolua_obj = new sigc::connection(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"sigc::connection");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(sigc::connection));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"sigc::connection");
#endif
   }
   tolua_pushcppstring(tolua_S,(const char*)attr);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'observe'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getId of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_Entity_getId00
static int tolua_Eris_Eris_Entity_getId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::Entity* self = (const Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getId'",NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getId();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getName of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_Entity_getName00
static int tolua_Eris_Eris_Entity_getName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::Entity* self = (const Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
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

/* method: getStamp of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_Entity_getStamp00
static int tolua_Eris_Eris_Entity_getStamp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::Entity* self = (const Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getStamp'",NULL);
#endif
  {
   float tolua_ret = (float)  self->getStamp();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getStamp'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getType of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_Entity_getType00
static int tolua_Eris_Eris_Entity_getType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::Entity* self = (const Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getType'",NULL);
#endif
  {
   Eris::TypeInfo* tolua_ret = (Eris::TypeInfo*)  self->getType();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Eris::TypeInfo");
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

/* method: getLocation of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_Entity_getLocation00
static int tolua_Eris_Eris_Entity_getLocation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::Entity* self = (const Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLocation'",NULL);
#endif
  {
   Eris::Entity* tolua_ret = (Eris::Entity*)  self->getLocation();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Eris::Entity");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getLocation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPosition of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_Entity_getPosition00
static int tolua_Eris_Eris_Entity_getPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::Entity* self = (const Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPosition'",NULL);
#endif
  {
   WFMath::Point<3> tolua_ret = (WFMath::Point<3>)  self->getPosition();
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
 tolua_error(tolua_S,"#ferror in function 'getPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAttributes of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_Entity_getAttributes00
static int tolua_Eris_Eris_Entity_getAttributes00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::Entity* self = (const Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAttributes'",NULL);
#endif
  {
   const Atlas::Message::MapType& tolua_ret = (const Atlas::Message::MapType&)  self->getAttributes();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Atlas::Message::MapType");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAttributes'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isMoving of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_Entity_isMoving00
static int tolua_Eris_Eris_Entity_isMoving00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::Entity* self = (const Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isMoving'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isMoving();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isMoving'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPredictedPos of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_Entity_getPredictedPos00
static int tolua_Eris_Eris_Entity_getPredictedPos00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::Entity* self = (const Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPredictedPos'",NULL);
#endif
  {
   WFMath::Point<3> tolua_ret = (WFMath::Point<3>)  self->getPredictedPos();
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
 tolua_error(tolua_S,"#ferror in function 'getPredictedPos'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPredictedVelocity of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_Entity_getPredictedVelocity00
static int tolua_Eris_Eris_Entity_getPredictedVelocity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::Entity* self = (const Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPredictedVelocity'",NULL);
#endif
  {
   WFMath::Vector<3> tolua_ret = (WFMath::Vector<3>)  self->getPredictedVelocity();
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
 tolua_error(tolua_S,"#ferror in function 'getPredictedVelocity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getViewPosition of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_Entity_getViewPosition00
static int tolua_Eris_Eris_Entity_getViewPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::Entity* self = (const Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getViewPosition'",NULL);
#endif
  {
   WFMath::Point<3> tolua_ret = (WFMath::Point<3>)  self->getViewPosition();
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
 tolua_error(tolua_S,"#ferror in function 'getViewPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getViewOrientation of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_Entity_getViewOrientation00
static int tolua_Eris_Eris_Entity_getViewOrientation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::Entity* self = (const Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getViewOrientation'",NULL);
#endif
  {
   WFMath::Quaternion tolua_ret = (WFMath::Quaternion)  self->getViewOrientation();
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
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getViewOrientation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVelocity of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_Entity_getVelocity00
static int tolua_Eris_Eris_Entity_getVelocity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::Entity* self = (const Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVelocity'",NULL);
#endif
  {
   const WFMath::Vector<3>& tolua_ret = (const WFMath::Vector<3>&)  self->getVelocity();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const WFMath::Vector<3>");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVelocity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getOrientation of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_Entity_getOrientation00
static int tolua_Eris_Eris_Entity_getOrientation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::Entity* self = (const Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getOrientation'",NULL);
#endif
  {
   const WFMath::Quaternion& tolua_ret = (const WFMath::Quaternion&)  self->getOrientation();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const WFMath::Quaternion");
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

/* method: getBBox of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_Entity_getBBox00
static int tolua_Eris_Eris_Entity_getBBox00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::Entity* self = (const Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getBBox'",NULL);
#endif
  {
   const WFMath::AxisBox<3>& tolua_ret = (const WFMath::AxisBox<3>&)  self->getBBox();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const WFMath::AxisBox<3>");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getBBox'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: hasBBox of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_Entity_hasBBox00
static int tolua_Eris_Eris_Entity_hasBBox00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::Entity* self = (const Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hasBBox'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->hasBBox();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'hasBBox'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: hasChild of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_Entity_hasChild00
static int tolua_Eris_Eris_Entity_hasChild00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::Entity",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::Entity* self = (const Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
  const std::string eid = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hasChild'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->hasChild(eid);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)eid);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'hasChild'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isVisible of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_Entity_isVisible00
static int tolua_Eris_Eris_Entity_isVisible00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::Entity* self = (const Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: ChildAdded of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_get_Eris__Entity_ChildAdded
static int tolua_get_Eris__Entity_ChildAdded(lua_State* tolua_S)
{
  Eris::Entity* self = (Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'ChildAdded'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->ChildAdded,"sigc::signal<void,Eris::Entity*>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: ChildAdded of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_set_Eris__Entity_ChildAdded
static int tolua_set_Eris__Entity_ChildAdded(lua_State* tolua_S)
{
  Eris::Entity* self = (Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'ChildAdded'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,Eris::Entity*>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->ChildAdded = *((sigc::signal<void,Eris::Entity*>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: ChildRemoved of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_get_Eris__Entity_ChildRemoved
static int tolua_get_Eris__Entity_ChildRemoved(lua_State* tolua_S)
{
  Eris::Entity* self = (Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'ChildRemoved'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->ChildRemoved,"sigc::signal<void,Eris::Entity*>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: ChildRemoved of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_set_Eris__Entity_ChildRemoved
static int tolua_set_Eris__Entity_ChildRemoved(lua_State* tolua_S)
{
  Eris::Entity* self = (Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'ChildRemoved'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,Eris::Entity*>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->ChildRemoved = *((sigc::signal<void,Eris::Entity*>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: LocationChanged of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_get_Eris__Entity_LocationChanged
static int tolua_get_Eris__Entity_LocationChanged(lua_State* tolua_S)
{
  Eris::Entity* self = (Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'LocationChanged'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->LocationChanged,"sigc::signal<void,Eris::Entity*>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: LocationChanged of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_set_Eris__Entity_LocationChanged
static int tolua_set_Eris__Entity_LocationChanged(lua_State* tolua_S)
{
  Eris::Entity* self = (Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'LocationChanged'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,Eris::Entity*>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->LocationChanged = *((sigc::signal<void,Eris::Entity*>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Changed of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_get_Eris__Entity_Changed
static int tolua_get_Eris__Entity_Changed(lua_State* tolua_S)
{
  Eris::Entity* self = (Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Changed'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->Changed,"sigc::signal<void,const std::set<std::string>&>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Changed of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_set_Eris__Entity_Changed
static int tolua_set_Eris__Entity_Changed(lua_State* tolua_S)
{
  Eris::Entity* self = (Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Changed'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,const std::set<std::string>&>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Changed = *((sigc::signal<void,const std::set<std::string>&>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Moved of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_get_Eris__Entity_Moved
static int tolua_get_Eris__Entity_Moved(lua_State* tolua_S)
{
  Eris::Entity* self = (Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Moved'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->Moved,"sigc::signal<void>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Moved of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_set_Eris__Entity_Moved
static int tolua_set_Eris__Entity_Moved(lua_State* tolua_S)
{
  Eris::Entity* self = (Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Moved'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Moved = *((sigc::signal<void>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Moving of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_get_Eris__Entity_Moving
static int tolua_get_Eris__Entity_Moving(lua_State* tolua_S)
{
  Eris::Entity* self = (Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Moving'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->Moving,"sigc::signal<void,bool>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Moving of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_set_Eris__Entity_Moving
static int tolua_set_Eris__Entity_Moving(lua_State* tolua_S)
{
  Eris::Entity* self = (Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Moving'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,bool>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Moving = *((sigc::signal<void,bool>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Say of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_get_Eris__Entity_Say
static int tolua_get_Eris__Entity_Say(lua_State* tolua_S)
{
  Eris::Entity* self = (Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Say'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->Say,"sigc::signal<void,const Atlas::Objects::Root&>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Say of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_set_Eris__Entity_Say
static int tolua_set_Eris__Entity_Say(lua_State* tolua_S)
{
  Eris::Entity* self = (Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Say'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,const Atlas::Objects::Root&>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Say = *((sigc::signal<void,const Atlas::Objects::Root&>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Emote of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_get_Eris__Entity_Emote
static int tolua_get_Eris__Entity_Emote(lua_State* tolua_S)
{
  Eris::Entity* self = (Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Emote'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->Emote,"sigc::signal<void,const std::string&>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Emote of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_set_Eris__Entity_Emote
static int tolua_set_Eris__Entity_Emote(lua_State* tolua_S)
{
  Eris::Entity* self = (Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Emote'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,const std::string&>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Emote = *((sigc::signal<void,const std::string&>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Acted of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_get_Eris__Entity_Acted
static int tolua_get_Eris__Entity_Acted(lua_State* tolua_S)
{
  Eris::Entity* self = (Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Acted'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->Acted,"sigc::signal<void,const Atlas::Objects::Operation::RootOperation&>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Acted of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_set_Eris__Entity_Acted
static int tolua_set_Eris__Entity_Acted(lua_State* tolua_S)
{
  Eris::Entity* self = (Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Acted'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,const Atlas::Objects::Operation::RootOperation&>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Acted = *((sigc::signal<void,const Atlas::Objects::Operation::RootOperation&>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Noise of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_get_Eris__Entity_Noise
static int tolua_get_Eris__Entity_Noise(lua_State* tolua_S)
{
  Eris::Entity* self = (Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Noise'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->Noise,"sigc::signal<void,const Atlas::Objects::Root&>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Noise of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_set_Eris__Entity_Noise
static int tolua_set_Eris__Entity_Noise(lua_State* tolua_S)
{
  Eris::Entity* self = (Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Noise'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,const Atlas::Objects::Root&>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Noise = *((sigc::signal<void,const Atlas::Objects::Root&>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: VisibilityChanged of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_get_Eris__Entity_VisibilityChanged
static int tolua_get_Eris__Entity_VisibilityChanged(lua_State* tolua_S)
{
  Eris::Entity* self = (Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'VisibilityChanged'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->VisibilityChanged,"sigc::signal<void,bool>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: VisibilityChanged of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_set_Eris__Entity_VisibilityChanged
static int tolua_set_Eris__Entity_VisibilityChanged(lua_State* tolua_S)
{
  Eris::Entity* self = (Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'VisibilityChanged'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,bool>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->VisibilityChanged = *((sigc::signal<void,bool>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: BeingDeleted of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_get_Eris__Entity_BeingDeleted
static int tolua_get_Eris__Entity_BeingDeleted(lua_State* tolua_S)
{
  Eris::Entity* self = (Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'BeingDeleted'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->BeingDeleted,"sigc::signal<void>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: BeingDeleted of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_set_Eris__Entity_BeingDeleted
static int tolua_set_Eris__Entity_BeingDeleted(lua_State* tolua_S)
{
  Eris::Entity* self = (Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'BeingDeleted'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->BeingDeleted = *((sigc::signal<void>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: TaskAdded of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_get_Eris__Entity_TaskAdded
static int tolua_get_Eris__Entity_TaskAdded(lua_State* tolua_S)
{
  Eris::Entity* self = (Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'TaskAdded'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->TaskAdded,"sigc::signal<void,Eris::Task*>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: TaskAdded of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_set_Eris__Entity_TaskAdded
static int tolua_set_Eris__Entity_TaskAdded(lua_State* tolua_S)
{
  Eris::Entity* self = (Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'TaskAdded'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,Eris::Task*>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->TaskAdded = *((sigc::signal<void,Eris::Task*>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: TaskRemoved of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_get_Eris__Entity_TaskRemoved
static int tolua_get_Eris__Entity_TaskRemoved(lua_State* tolua_S)
{
  Eris::Entity* self = (Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'TaskRemoved'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->TaskRemoved,"sigc::signal<void,Eris::Task*>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: TaskRemoved of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_set_Eris__Entity_TaskRemoved
static int tolua_set_Eris__Entity_TaskRemoved(lua_State* tolua_S)
{
  Eris::Entity* self = (Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'TaskRemoved'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,Eris::Task*>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->TaskRemoved = *((sigc::signal<void,Eris::Task*>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTasks of class  Eris::Entity */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_Entity_getTasks00
static int tolua_Eris_Eris_Entity_getTasks00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::Entity* self = (const Eris::Entity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTasks'",NULL);
#endif
  {
   const std::vector<Eris::Task*>& tolua_ret = (const std::vector<Eris::Task*>&)  self->getTasks();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const std::vector<Eris::Task*>");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTasks'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getStatus of class  Eris::ServerInfo */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_ServerInfo_getStatus00
static int tolua_Eris_Eris_ServerInfo_getStatus00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::ServerInfo",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::ServerInfo* self = (const Eris::ServerInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getStatus'",NULL);
#endif
  {
   Eris::ServerInfo::Status tolua_ret = (Eris::ServerInfo::Status)  self->getStatus();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getStatus'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getHostname of class  Eris::ServerInfo */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_ServerInfo_getHostname00
static int tolua_Eris_Eris_ServerInfo_getHostname00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::ServerInfo",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::ServerInfo* self = (const Eris::ServerInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getHostname'",NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getHostname();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getHostname'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getServername of class  Eris::ServerInfo */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_ServerInfo_getServername00
static int tolua_Eris_Eris_ServerInfo_getServername00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::ServerInfo",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::ServerInfo* self = (const Eris::ServerInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getServername'",NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getServername();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getServername'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRuleset of class  Eris::ServerInfo */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_ServerInfo_getRuleset00
static int tolua_Eris_Eris_ServerInfo_getRuleset00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::ServerInfo",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::ServerInfo* self = (const Eris::ServerInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRuleset'",NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getRuleset();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRuleset'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getServer of class  Eris::ServerInfo */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_ServerInfo_getServer00
static int tolua_Eris_Eris_ServerInfo_getServer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::ServerInfo",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::ServerInfo* self = (const Eris::ServerInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getServer'",NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getServer();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getServer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVersion of class  Eris::ServerInfo */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_ServerInfo_getVersion00
static int tolua_Eris_Eris_ServerInfo_getVersion00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::ServerInfo",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::ServerInfo* self = (const Eris::ServerInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVersion'",NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getVersion();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVersion'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getBuildDate of class  Eris::ServerInfo */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_ServerInfo_getBuildDate00
static int tolua_Eris_Eris_ServerInfo_getBuildDate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::ServerInfo",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::ServerInfo* self = (const Eris::ServerInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getBuildDate'",NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getBuildDate();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getBuildDate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getNumClients of class  Eris::ServerInfo */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_ServerInfo_getNumClients00
static int tolua_Eris_Eris_ServerInfo_getNumClients00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::ServerInfo",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::ServerInfo* self = (const Eris::ServerInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getNumClients'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getNumClients();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getNumClients'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPing of class  Eris::ServerInfo */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_ServerInfo_getPing00
static int tolua_Eris_Eris_ServerInfo_getPing00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::ServerInfo",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::ServerInfo* self = (const Eris::ServerInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPing'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getPing();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPing'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getUptime of class  Eris::ServerInfo */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_ServerInfo_getUptime00
static int tolua_Eris_Eris_ServerInfo_getUptime00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::ServerInfo",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::ServerInfo* self = (const Eris::ServerInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getUptime'",NULL);
#endif
  {
   double tolua_ret = (double)  self->getUptime();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getUptime'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getGameServerCount of class  Eris::Meta */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_Meta_getGameServerCount00
static int tolua_Eris_Eris_Meta_getGameServerCount00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::Meta",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::Meta* self = (const Eris::Meta*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getGameServerCount'",NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->getGameServerCount();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getGameServerCount'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getInfoForServer of class  Eris::Meta */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_Meta_getInfoForServer00
static int tolua_Eris_Eris_Meta_getInfoForServer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::Meta",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::Meta* self = (const Eris::Meta*)  tolua_tousertype(tolua_S,1,0);
  unsigned int index = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getInfoForServer'",NULL);
#endif
  {
   const Eris::ServerInfo& tolua_ret = (const Eris::ServerInfo&)  self->getInfoForServer(index);
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Eris::ServerInfo");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInfoForServer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: queryServerByIndex of class  Eris::Meta */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_Meta_queryServerByIndex00
static int tolua_Eris_Eris_Meta_queryServerByIndex00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Eris::Meta",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Eris::Meta* self = (Eris::Meta*)  tolua_tousertype(tolua_S,1,0);
  unsigned int index = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'queryServerByIndex'",NULL);
#endif
  {
   self->queryServerByIndex(index);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'queryServerByIndex'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: refresh of class  Eris::Meta */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_Meta_refresh00
static int tolua_Eris_Eris_Meta_refresh00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Eris::Meta",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Eris::Meta* self = (Eris::Meta*)  tolua_tousertype(tolua_S,1,0);
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

/* method: cancel of class  Eris::Meta */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_Meta_cancel00
static int tolua_Eris_Eris_Meta_cancel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Eris::Meta",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Eris::Meta* self = (Eris::Meta*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cancel'",NULL);
#endif
  {
   self->cancel();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cancel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: ReceivedServerInfo of class  Eris::Meta */
#ifndef TOLUA_DISABLE_tolua_get_Eris__Meta_ReceivedServerInfo
static int tolua_get_Eris__Meta_ReceivedServerInfo(lua_State* tolua_S)
{
  Eris::Meta* self = (Eris::Meta*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'ReceivedServerInfo'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->ReceivedServerInfo,"sigc::signal<void,const Eris::ServerInfo&>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: ReceivedServerInfo of class  Eris::Meta */
#ifndef TOLUA_DISABLE_tolua_set_Eris__Meta_ReceivedServerInfo
static int tolua_set_Eris__Meta_ReceivedServerInfo(lua_State* tolua_S)
{
  Eris::Meta* self = (Eris::Meta*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'ReceivedServerInfo'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,const Eris::ServerInfo&>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->ReceivedServerInfo = *((sigc::signal<void,const Eris::ServerInfo&>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: CompletedServerList of class  Eris::Meta */
#ifndef TOLUA_DISABLE_tolua_get_Eris__Meta_CompletedServerList
static int tolua_get_Eris__Meta_CompletedServerList(lua_State* tolua_S)
{
  Eris::Meta* self = (Eris::Meta*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'CompletedServerList'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->CompletedServerList,"sigc::signal<void,int>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: CompletedServerList of class  Eris::Meta */
#ifndef TOLUA_DISABLE_tolua_set_Eris__Meta_CompletedServerList
static int tolua_set_Eris__Meta_CompletedServerList(lua_State* tolua_S)
{
  Eris::Meta* self = (Eris::Meta*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'CompletedServerList'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,int>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->CompletedServerList = *((sigc::signal<void,int>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: AllQueriesDone of class  Eris::Meta */
#ifndef TOLUA_DISABLE_tolua_get_Eris__Meta_AllQueriesDone
static int tolua_get_Eris__Meta_AllQueriesDone(lua_State* tolua_S)
{
  Eris::Meta* self = (Eris::Meta*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'AllQueriesDone'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->AllQueriesDone,"sigc::signal<void>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: AllQueriesDone of class  Eris::Meta */
#ifndef TOLUA_DISABLE_tolua_set_Eris__Meta_AllQueriesDone
static int tolua_set_Eris__Meta_AllQueriesDone(lua_State* tolua_S)
{
  Eris::Meta* self = (Eris::Meta*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'AllQueriesDone'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->AllQueriesDone = *((sigc::signal<void>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Failure of class  Eris::Meta */
#ifndef TOLUA_DISABLE_tolua_get_Eris__Meta_Failure
static int tolua_get_Eris__Meta_Failure(lua_State* tolua_S)
{
  Eris::Meta* self = (Eris::Meta*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Failure'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->Failure,"sigc::signal<void,const std::string&>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Failure of class  Eris::Meta */
#ifndef TOLUA_DISABLE_tolua_set_Eris__Meta_Failure
static int tolua_set_Eris__Meta_Failure(lua_State* tolua_S)
{
  Eris::Meta* self = (Eris::Meta*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Failure'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,const std::string&>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Failure = *((sigc::signal<void,const std::string&>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTypeService of class  Eris::Connection */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_Connection_getTypeService00
static int tolua_Eris_Eris_Connection_getTypeService00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::Connection",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::Connection* self = (const Eris::Connection*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTypeService'",NULL);
#endif
  {
   Eris::TypeService* tolua_ret = (Eris::TypeService*)  self->getTypeService();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Eris::TypeService");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTypeService'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: GotServerInfo of class  Eris::Connection */
#ifndef TOLUA_DISABLE_tolua_get_Eris__Connection_GotServerInfo
static int tolua_get_Eris__Connection_GotServerInfo(lua_State* tolua_S)
{
  Eris::Connection* self = (Eris::Connection*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'GotServerInfo'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->GotServerInfo,"sigc::signal<void>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: GotServerInfo of class  Eris::Connection */
#ifndef TOLUA_DISABLE_tolua_set_Eris__Connection_GotServerInfo
static int tolua_set_Eris__Connection_GotServerInfo(lua_State* tolua_S)
{
  Eris::Connection* self = (Eris::Connection*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'GotServerInfo'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->GotServerInfo = *((sigc::signal<void>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Connected of class  Eris::Connection */
#ifndef TOLUA_DISABLE_tolua_get_Eris__Connection_Connected
static int tolua_get_Eris__Connection_Connected(lua_State* tolua_S)
{
  Eris::Connection* self = (Eris::Connection*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Connected'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->Connected,"sigc::signal<void>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Connected of class  Eris::Connection */
#ifndef TOLUA_DISABLE_tolua_set_Eris__Connection_Connected
static int tolua_set_Eris__Connection_Connected(lua_State* tolua_S)
{
  Eris::Connection* self = (Eris::Connection*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Connected'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Connected = *((sigc::signal<void>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Disconnected of class  Eris::Connection */
#ifndef TOLUA_DISABLE_tolua_get_Eris__Connection_Disconnected
static int tolua_get_Eris__Connection_Disconnected(lua_State* tolua_S)
{
  Eris::Connection* self = (Eris::Connection*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Disconnected'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->Disconnected,"sigc::signal<void>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Disconnected of class  Eris::Connection */
#ifndef TOLUA_DISABLE_tolua_set_Eris__Connection_Disconnected
static int tolua_set_Eris__Connection_Disconnected(lua_State* tolua_S)
{
  Eris::Connection* self = (Eris::Connection*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Disconnected'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Disconnected = *((sigc::signal<void>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: lookQueueSize of class  Eris::View */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_View_lookQueueSize00
static int tolua_Eris_Eris_View_lookQueueSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::View",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::View* self = (const Eris::View*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'lookQueueSize'",NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->lookQueueSize();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'lookQueueSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isA of class  Eris::TypeInfo */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_TypeInfo_isA00
static int tolua_Eris_Eris_TypeInfo_isA00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Eris::TypeInfo",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Eris::TypeInfo",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Eris::TypeInfo* self = (Eris::TypeInfo*)  tolua_tousertype(tolua_S,1,0);
  Eris::TypeInfo* ti = ((Eris::TypeInfo*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isA'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isA(ti);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isA'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isBound of class  Eris::TypeInfo */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_TypeInfo_isBound00
static int tolua_Eris_Eris_TypeInfo_isBound00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::TypeInfo",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::TypeInfo* self = (const Eris::TypeInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isBound'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isBound();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isBound'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: hasUnresolvedChildren of class  Eris::TypeInfo */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_TypeInfo_hasUnresolvedChildren00
static int tolua_Eris_Eris_TypeInfo_hasUnresolvedChildren00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::TypeInfo",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::TypeInfo* self = (const Eris::TypeInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hasUnresolvedChildren'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->hasUnresolvedChildren();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'hasUnresolvedChildren'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: resolveChildren of class  Eris::TypeInfo */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_TypeInfo_resolveChildren00
static int tolua_Eris_Eris_TypeInfo_resolveChildren00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Eris::TypeInfo",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Eris::TypeInfo* self = (Eris::TypeInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'resolveChildren'",NULL);
#endif
  {
   self->resolveChildren();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'resolveChildren'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getName of class  Eris::TypeInfo */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_TypeInfo_getName00
static int tolua_Eris_Eris_TypeInfo_getName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::TypeInfo",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::TypeInfo* self = (const Eris::TypeInfo*)  tolua_tousertype(tolua_S,1,0);
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

/* method: getChildren of class  Eris::TypeInfo */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_TypeInfo_getChildren00
static int tolua_Eris_Eris_TypeInfo_getChildren00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::TypeInfo",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::TypeInfo* self = (const Eris::TypeInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getChildren'",NULL);
#endif
  {
    const std::set<Eris::TypeInfo*>& tolua_ret = (  const std::set<Eris::TypeInfo*>&)  self->getChildren();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const std::set<Eris::TypeInfo*>");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getChildren'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getParents of class  Eris::TypeInfo */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_TypeInfo_getParents00
static int tolua_Eris_Eris_TypeInfo_getParents00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::TypeInfo",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::TypeInfo* self = (const Eris::TypeInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getParents'",NULL);
#endif
  {
    const std::set<Eris::TypeInfo*>& tolua_ret = (  const std::set<Eris::TypeInfo*>&)  self->getParents();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const std::set<Eris::TypeInfo*>");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getParents'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: name of class  Eris::Task */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_Task_name00
static int tolua_Eris_Eris_Task_name00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::Task",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::Task* self = (const Eris::Task*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'name'",NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->name();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'name'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: progress of class  Eris::Task */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_Task_progress00
static int tolua_Eris_Eris_Task_progress00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::Task",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::Task* self = (const Eris::Task*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'progress'",NULL);
#endif
  {
   double tolua_ret = (double)  self->progress();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'progress'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isComplete of class  Eris::Task */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_Task_isComplete00
static int tolua_Eris_Eris_Task_isComplete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::Task",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::Task* self = (const Eris::Task*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isComplete'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isComplete();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isComplete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Completed of class  Eris::Task */
#ifndef TOLUA_DISABLE_tolua_get_Eris__Task_Completed
static int tolua_get_Eris__Task_Completed(lua_State* tolua_S)
{
  Eris::Task* self = (Eris::Task*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Completed'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->Completed,"sigc::signal<void>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Completed of class  Eris::Task */
#ifndef TOLUA_DISABLE_tolua_set_Eris__Task_Completed
static int tolua_set_Eris__Task_Completed(lua_State* tolua_S)
{
  Eris::Task* self = (Eris::Task*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Completed'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Completed = *((sigc::signal<void>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Cancelled of class  Eris::Task */
#ifndef TOLUA_DISABLE_tolua_get_Eris__Task_Cancelled
static int tolua_get_Eris__Task_Cancelled(lua_State* tolua_S)
{
  Eris::Task* self = (Eris::Task*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Cancelled'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->Cancelled,"sigc::signal<void>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Cancelled of class  Eris::Task */
#ifndef TOLUA_DISABLE_tolua_set_Eris__Task_Cancelled
static int tolua_set_Eris__Task_Cancelled(lua_State* tolua_S)
{
  Eris::Task* self = (Eris::Task*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Cancelled'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Cancelled = *((sigc::signal<void>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Progressed of class  Eris::Task */
#ifndef TOLUA_DISABLE_tolua_get_Eris__Task_Progressed
static int tolua_get_Eris__Task_Progressed(lua_State* tolua_S)
{
  Eris::Task* self = (Eris::Task*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Progressed'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->Progressed,"sigc::signal<void>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Progressed of class  Eris::Task */
#ifndef TOLUA_DISABLE_tolua_set_Eris__Task_Progressed
static int tolua_set_Eris__Task_Progressed(lua_State* tolua_S)
{
  Eris::Task* self = (Eris::Task*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Progressed'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Progressed = *((sigc::signal<void>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTypeByName of class  Eris::TypeService */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_TypeService_getTypeByName00
static int tolua_Eris_Eris_TypeService_getTypeByName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Eris::TypeService",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Eris::TypeService* self = (Eris::TypeService*)  tolua_tousertype(tolua_S,1,0);
  const std::string tynm = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTypeByName'",NULL);
#endif
  {
   Eris::TypeInfo* tolua_ret = (Eris::TypeInfo*)  self->getTypeByName(tynm);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Eris::TypeInfo");
   tolua_pushcppstring(tolua_S,(const char*)tynm);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTypeByName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTypeForAtlas of class  Eris::TypeService */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_TypeService_getTypeForAtlas00
static int tolua_Eris_Eris_TypeService_getTypeForAtlas00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Eris::TypeService",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Atlas::Objects::Root",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Eris::TypeService* self = (Eris::TypeService*)  tolua_tousertype(tolua_S,1,0);
  const Atlas::Objects::Root* obj = ((const Atlas::Objects::Root*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTypeForAtlas'",NULL);
#endif
  {
   Eris::TypeInfo* tolua_ret = (Eris::TypeInfo*)  self->getTypeForAtlas(*obj);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Eris::TypeInfo");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTypeForAtlas'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: findTypeByName of class  Eris::TypeService */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_TypeService_findTypeByName00
static int tolua_Eris_Eris_TypeService_findTypeByName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Eris::TypeService",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Eris::TypeService* self = (Eris::TypeService*)  tolua_tousertype(tolua_S,1,0);
  const std::string tynm = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'findTypeByName'",NULL);
#endif
  {
   Eris::TypeInfo* tolua_ret = (Eris::TypeInfo*)  self->findTypeByName(tynm);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Eris::TypeInfo");
   tolua_pushcppstring(tolua_S,(const char*)tynm);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'findTypeByName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: BoundType of class  Eris::TypeService */
#ifndef TOLUA_DISABLE_tolua_get_Eris__TypeService_BoundType
static int tolua_get_Eris__TypeService_BoundType(lua_State* tolua_S)
{
  Eris::TypeService* self = (Eris::TypeService*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'BoundType'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->BoundType,"sigc::signal<void,Eris::TypeInfo*>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: BoundType of class  Eris::TypeService */
#ifndef TOLUA_DISABLE_tolua_set_Eris__TypeService_BoundType
static int tolua_set_Eris__TypeService_BoundType(lua_State* tolua_S)
{
  Eris::TypeService* self = (Eris::TypeService*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'BoundType'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,Eris::TypeInfo*>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->BoundType = *((sigc::signal<void,Eris::TypeInfo*>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: BadType of class  Eris::TypeService */
#ifndef TOLUA_DISABLE_tolua_get_Eris__TypeService_BadType
static int tolua_get_Eris__TypeService_BadType(lua_State* tolua_S)
{
  Eris::TypeService* self = (Eris::TypeService*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'BadType'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->BadType,"sigc::signal<void,Eris::TypeInfo*>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: BadType of class  Eris::TypeService */
#ifndef TOLUA_DISABLE_tolua_set_Eris__TypeService_BadType
static int tolua_set_Eris__TypeService_BadType(lua_State* tolua_S)
{
  Eris::TypeService* self = (Eris::TypeService*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'BadType'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,Eris::TypeInfo*>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->BadType = *((sigc::signal<void,Eris::TypeInfo*>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getId of class  Eris::Avatar */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_Avatar_getId00
static int tolua_Eris_Eris_Avatar_getId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::Avatar",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::Avatar* self = (const Eris::Avatar*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getId'",NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getId();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getEntity of class  Eris::Avatar */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_Avatar_getEntity00
static int tolua_Eris_Eris_Avatar_getEntity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::Avatar",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::Avatar* self = (const Eris::Avatar*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getEntity'",NULL);
#endif
  {
   Eris::Entity* tolua_ret = (Eris::Entity*)  self->getEntity();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Eris::Entity");
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

/* method: getView of class  Eris::Avatar */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_Avatar_getView00
static int tolua_Eris_Eris_Avatar_getView00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::Avatar",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::Avatar* self = (const Eris::Avatar*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getView'",NULL);
#endif
  {
   Eris::View* tolua_ret = (Eris::View*)  self->getView();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Eris::View");
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

/* method: getConnection of class  Eris::Avatar */
#ifndef TOLUA_DISABLE_tolua_Eris_Eris_Avatar_getConnection00
static int tolua_Eris_Eris_Avatar_getConnection00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Eris::Avatar",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Eris::Avatar* self = (const Eris::Avatar*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getConnection'",NULL);
#endif
  {
   Eris::Connection* tolua_ret = (Eris::Connection*)  self->getConnection();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Eris::Connection");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getConnection'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_Eris_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_module(tolua_S,"Atlas",0);
  tolua_beginmodule(tolua_S,"Atlas");
   tolua_module(tolua_S,"Message",0);
   tolua_beginmodule(tolua_S,"Message");
    tolua_cclass(tolua_S,"Element","Atlas::Message::Element","",NULL);
    tolua_beginmodule(tolua_S,"Element");
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"Eris",0);
  tolua_beginmodule(tolua_S,"Eris");
   tolua_constant(tolua_S,"NO_ERR",Eris::NO_ERR);
   tolua_constant(tolua_S,"NOT_CONNECTED",Eris::NOT_CONNECTED);
   tolua_constant(tolua_S,"NOT_LOGGED_IN",Eris::NOT_LOGGED_IN);
   tolua_constant(tolua_S,"ALREADY_LOGGED_IN",Eris::ALREADY_LOGGED_IN);
   tolua_constant(tolua_S,"DUPLICATE_CHAR_ACTIVE",Eris::DUPLICATE_CHAR_ACTIVE);
   tolua_constant(tolua_S,"BAD_CHARACTER_ID",Eris::BAD_CHARACTER_ID);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"Eris",0);
  tolua_beginmodule(tolua_S,"Eris");
   tolua_cclass(tolua_S,"Entity","Eris::Entity","",NULL);
   tolua_beginmodule(tolua_S,"Entity");
    tolua_function(tolua_S,"numContained",tolua_Eris_Eris_Entity_numContained00);
    tolua_function(tolua_S,"getContained",tolua_Eris_Eris_Entity_getContained00);
    tolua_function(tolua_S,"valueOfAttr",tolua_Eris_Eris_Entity_valueOfAttr00);
    tolua_function(tolua_S,"hasAttr",tolua_Eris_Eris_Entity_hasAttr00);
    tolua_function(tolua_S,"observe",tolua_Eris_Eris_Entity_observe00);
    tolua_function(tolua_S,"getId",tolua_Eris_Eris_Entity_getId00);
    tolua_function(tolua_S,"getName",tolua_Eris_Eris_Entity_getName00);
    tolua_function(tolua_S,"getStamp",tolua_Eris_Eris_Entity_getStamp00);
    tolua_function(tolua_S,"getType",tolua_Eris_Eris_Entity_getType00);
    tolua_function(tolua_S,"getLocation",tolua_Eris_Eris_Entity_getLocation00);
    tolua_function(tolua_S,"getPosition",tolua_Eris_Eris_Entity_getPosition00);
    tolua_function(tolua_S,"getAttributes",tolua_Eris_Eris_Entity_getAttributes00);
    tolua_function(tolua_S,"isMoving",tolua_Eris_Eris_Entity_isMoving00);
    tolua_function(tolua_S,"getPredictedPos",tolua_Eris_Eris_Entity_getPredictedPos00);
    tolua_function(tolua_S,"getPredictedVelocity",tolua_Eris_Eris_Entity_getPredictedVelocity00);
    tolua_function(tolua_S,"getViewPosition",tolua_Eris_Eris_Entity_getViewPosition00);
    tolua_function(tolua_S,"getViewOrientation",tolua_Eris_Eris_Entity_getViewOrientation00);
    tolua_function(tolua_S,"getVelocity",tolua_Eris_Eris_Entity_getVelocity00);
    tolua_function(tolua_S,"getOrientation",tolua_Eris_Eris_Entity_getOrientation00);
    tolua_function(tolua_S,"getBBox",tolua_Eris_Eris_Entity_getBBox00);
    tolua_function(tolua_S,"hasBBox",tolua_Eris_Eris_Entity_hasBBox00);
    tolua_function(tolua_S,"hasChild",tolua_Eris_Eris_Entity_hasChild00);
    tolua_function(tolua_S,"isVisible",tolua_Eris_Eris_Entity_isVisible00);
    tolua_variable(tolua_S,"ChildAdded",tolua_get_Eris__Entity_ChildAdded,tolua_set_Eris__Entity_ChildAdded);
    tolua_variable(tolua_S,"ChildRemoved",tolua_get_Eris__Entity_ChildRemoved,tolua_set_Eris__Entity_ChildRemoved);
    tolua_variable(tolua_S,"LocationChanged",tolua_get_Eris__Entity_LocationChanged,tolua_set_Eris__Entity_LocationChanged);
    tolua_variable(tolua_S,"Changed",tolua_get_Eris__Entity_Changed,tolua_set_Eris__Entity_Changed);
    tolua_variable(tolua_S,"Moved",tolua_get_Eris__Entity_Moved,tolua_set_Eris__Entity_Moved);
    tolua_variable(tolua_S,"Moving",tolua_get_Eris__Entity_Moving,tolua_set_Eris__Entity_Moving);
    tolua_variable(tolua_S,"Say",tolua_get_Eris__Entity_Say,tolua_set_Eris__Entity_Say);
    tolua_variable(tolua_S,"Emote",tolua_get_Eris__Entity_Emote,tolua_set_Eris__Entity_Emote);
    tolua_variable(tolua_S,"Acted",tolua_get_Eris__Entity_Acted,tolua_set_Eris__Entity_Acted);
    tolua_variable(tolua_S,"Noise",tolua_get_Eris__Entity_Noise,tolua_set_Eris__Entity_Noise);
    tolua_variable(tolua_S,"VisibilityChanged",tolua_get_Eris__Entity_VisibilityChanged,tolua_set_Eris__Entity_VisibilityChanged);
    tolua_variable(tolua_S,"BeingDeleted",tolua_get_Eris__Entity_BeingDeleted,tolua_set_Eris__Entity_BeingDeleted);
    tolua_variable(tolua_S,"TaskAdded",tolua_get_Eris__Entity_TaskAdded,tolua_set_Eris__Entity_TaskAdded);
    tolua_variable(tolua_S,"TaskRemoved",tolua_get_Eris__Entity_TaskRemoved,tolua_set_Eris__Entity_TaskRemoved);
    tolua_function(tolua_S,"getTasks",tolua_Eris_Eris_Entity_getTasks00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"Eris",0);
  tolua_beginmodule(tolua_S,"Eris");
   tolua_cclass(tolua_S,"ServerInfo","Eris::ServerInfo","",NULL);
   tolua_beginmodule(tolua_S,"ServerInfo");
    tolua_constant(tolua_S,"INVALID",Eris::ServerInfo::INVALID);
    tolua_constant(tolua_S,"QUERYING",Eris::ServerInfo::QUERYING);
    tolua_constant(tolua_S,"VALID",Eris::ServerInfo::VALID);
    tolua_constant(tolua_S,"TIMEOUT",Eris::ServerInfo::TIMEOUT);
    tolua_function(tolua_S,"getStatus",tolua_Eris_Eris_ServerInfo_getStatus00);
    tolua_function(tolua_S,"getHostname",tolua_Eris_Eris_ServerInfo_getHostname00);
    tolua_function(tolua_S,"getServername",tolua_Eris_Eris_ServerInfo_getServername00);
    tolua_function(tolua_S,"getRuleset",tolua_Eris_Eris_ServerInfo_getRuleset00);
    tolua_function(tolua_S,"getServer",tolua_Eris_Eris_ServerInfo_getServer00);
    tolua_function(tolua_S,"getVersion",tolua_Eris_Eris_ServerInfo_getVersion00);
    tolua_function(tolua_S,"getBuildDate",tolua_Eris_Eris_ServerInfo_getBuildDate00);
    tolua_function(tolua_S,"getNumClients",tolua_Eris_Eris_ServerInfo_getNumClients00);
    tolua_function(tolua_S,"getPing",tolua_Eris_Eris_ServerInfo_getPing00);
    tolua_function(tolua_S,"getUptime",tolua_Eris_Eris_ServerInfo_getUptime00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"Eris",0);
  tolua_beginmodule(tolua_S,"Eris");
   tolua_cclass(tolua_S,"Meta","Eris::Meta","",NULL);
   tolua_beginmodule(tolua_S,"Meta");
    tolua_function(tolua_S,"getGameServerCount",tolua_Eris_Eris_Meta_getGameServerCount00);
    tolua_function(tolua_S,"getInfoForServer",tolua_Eris_Eris_Meta_getInfoForServer00);
    tolua_function(tolua_S,"queryServerByIndex",tolua_Eris_Eris_Meta_queryServerByIndex00);
    tolua_function(tolua_S,"refresh",tolua_Eris_Eris_Meta_refresh00);
    tolua_function(tolua_S,"cancel",tolua_Eris_Eris_Meta_cancel00);
    tolua_variable(tolua_S,"ReceivedServerInfo",tolua_get_Eris__Meta_ReceivedServerInfo,tolua_set_Eris__Meta_ReceivedServerInfo);
    tolua_variable(tolua_S,"CompletedServerList",tolua_get_Eris__Meta_CompletedServerList,tolua_set_Eris__Meta_CompletedServerList);
    tolua_variable(tolua_S,"AllQueriesDone",tolua_get_Eris__Meta_AllQueriesDone,tolua_set_Eris__Meta_AllQueriesDone);
    tolua_variable(tolua_S,"Failure",tolua_get_Eris__Meta_Failure,tolua_set_Eris__Meta_Failure);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"Eris",0);
  tolua_beginmodule(tolua_S,"Eris");
   tolua_cclass(tolua_S,"Connection","Eris::Connection","",NULL);
   tolua_beginmodule(tolua_S,"Connection");
    tolua_function(tolua_S,"getTypeService",tolua_Eris_Eris_Connection_getTypeService00);
    tolua_variable(tolua_S,"GotServerInfo",tolua_get_Eris__Connection_GotServerInfo,tolua_set_Eris__Connection_GotServerInfo);
    tolua_variable(tolua_S,"Connected",tolua_get_Eris__Connection_Connected,tolua_set_Eris__Connection_Connected);
    tolua_variable(tolua_S,"Disconnected",tolua_get_Eris__Connection_Disconnected,tolua_set_Eris__Connection_Disconnected);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"Eris",0);
  tolua_beginmodule(tolua_S,"Eris");
   tolua_cclass(tolua_S,"Account","Eris::Account","",NULL);
   tolua_beginmodule(tolua_S,"Account");
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"Eris",0);
  tolua_beginmodule(tolua_S,"Eris");
   tolua_cclass(tolua_S,"View","Eris::View","",NULL);
   tolua_beginmodule(tolua_S,"View");
    tolua_function(tolua_S,"lookQueueSize",tolua_Eris_Eris_View_lookQueueSize00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"Eris",0);
  tolua_beginmodule(tolua_S,"Eris");
   tolua_cclass(tolua_S,"TypeInfo","Eris::TypeInfo","",NULL);
   tolua_beginmodule(tolua_S,"TypeInfo");
    tolua_function(tolua_S,"isA",tolua_Eris_Eris_TypeInfo_isA00);
    tolua_function(tolua_S,"isBound",tolua_Eris_Eris_TypeInfo_isBound00);
    tolua_function(tolua_S,"hasUnresolvedChildren",tolua_Eris_Eris_TypeInfo_hasUnresolvedChildren00);
    tolua_function(tolua_S,"resolveChildren",tolua_Eris_Eris_TypeInfo_resolveChildren00);
    tolua_function(tolua_S,"getName",tolua_Eris_Eris_TypeInfo_getName00);
    tolua_function(tolua_S,"getChildren",tolua_Eris_Eris_TypeInfo_getChildren00);
    tolua_function(tolua_S,"getParents",tolua_Eris_Eris_TypeInfo_getParents00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"Eris",0);
  tolua_beginmodule(tolua_S,"Eris");
   tolua_cclass(tolua_S,"Task","Eris::Task","",NULL);
   tolua_beginmodule(tolua_S,"Task");
    tolua_function(tolua_S,"name",tolua_Eris_Eris_Task_name00);
    tolua_function(tolua_S,"progress",tolua_Eris_Eris_Task_progress00);
    tolua_function(tolua_S,"isComplete",tolua_Eris_Eris_Task_isComplete00);
    tolua_variable(tolua_S,"Completed",tolua_get_Eris__Task_Completed,tolua_set_Eris__Task_Completed);
    tolua_variable(tolua_S,"Cancelled",tolua_get_Eris__Task_Cancelled,tolua_set_Eris__Task_Cancelled);
    tolua_variable(tolua_S,"Progressed",tolua_get_Eris__Task_Progressed,tolua_set_Eris__Task_Progressed);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"Eris",0);
  tolua_beginmodule(tolua_S,"Eris");
   tolua_cclass(tolua_S,"TypeService","Eris::TypeService","virtualsigc::trackable",NULL);
   tolua_beginmodule(tolua_S,"TypeService");
    tolua_function(tolua_S,"getTypeByName",tolua_Eris_Eris_TypeService_getTypeByName00);
    tolua_function(tolua_S,"getTypeForAtlas",tolua_Eris_Eris_TypeService_getTypeForAtlas00);
    tolua_function(tolua_S,"findTypeByName",tolua_Eris_Eris_TypeService_findTypeByName00);
    tolua_variable(tolua_S,"BoundType",tolua_get_Eris__TypeService_BoundType,tolua_set_Eris__TypeService_BoundType);
    tolua_variable(tolua_S,"BadType",tolua_get_Eris__TypeService_BadType,tolua_set_Eris__TypeService_BadType);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"Eris",0);
  tolua_beginmodule(tolua_S,"Eris");
   tolua_cclass(tolua_S,"Avatar","Eris::Avatar","",NULL);
   tolua_beginmodule(tolua_S,"Avatar");
    tolua_function(tolua_S,"getId",tolua_Eris_Eris_Avatar_getId00);
    tolua_function(tolua_S,"getEntity",tolua_Eris_Eris_Avatar_getEntity00);
    tolua_function(tolua_S,"getView",tolua_Eris_Eris_Avatar_getView00);
    tolua_function(tolua_S,"getConnection",tolua_Eris_Eris_Avatar_getConnection00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_Eris (lua_State* tolua_S) {
 return tolua_Eris_open(tolua_S);
};
#endif

