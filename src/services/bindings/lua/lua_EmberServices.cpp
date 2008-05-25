/*
** Lua binding: EmberServices
** Generated automatically by tolua++-1.0.92 on Sun May 25 22:54:23 2008.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_EmberServices_open (lua_State* tolua_S);

#include "required.h"
#include "services/EmberServices.h"
#include "services/logging/LoggingService.h"
#include "services/server/ServerService.h"
#include "services/config/ConfigService.h"
#include "services/metaserver/MetaserverService.h"
#include "services/scripting/ScriptingService.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_varconf__Variable (lua_State* tolua_S)
{
 varconf::Variable* self = (varconf::Variable*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_std__vector_std__string_ (lua_State* tolua_S)
{
 std::vector<std::string>* self = (std::vector<std::string>*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"varconf::Variable");
 tolua_usertype(tolua_S,"WFMath::Quaternion");
 tolua_usertype(tolua_S,"WFMath::Vector<3>");
 tolua_usertype(tolua_S,"Eris::Meta");
 tolua_usertype(tolua_S,"sigc::signal<void,const Atlas::Objects::Entity::RootEntity&>");
 tolua_usertype(tolua_S,"Ember::ServerService");
 tolua_usertype(tolua_S,"sigc::signal<void,const std::string&,const std::string&>");
 tolua_usertype(tolua_S,"Ember::EmberServices");
 tolua_usertype(tolua_S,"std::vector<std::string>");
 tolua_usertype(tolua_S,"sigc::signal<void,const std::string&>");
 tolua_usertype(tolua_S,"Eris::Entity");
 tolua_usertype(tolua_S,"Ember::IScriptingProvider");
 tolua_usertype(tolua_S,"Ember::ScriptingService");
 tolua_usertype(tolua_S,"sigc::signal<void,Eris::Avatar*>");
 tolua_usertype(tolua_S,"Ember::MetaserverService");
 tolua_usertype(tolua_S,"sigc::signal<void,Eris::Account*,const std::string&>");
 tolua_usertype(tolua_S,"Ember::ConfigService");
 tolua_usertype(tolua_S,"WFMath::Point<3>");
 tolua_usertype(tolua_S,"sigc::signal<void,Eris::Connection*>");
 tolua_usertype(tolua_S,"sigc::signal<void,Eris::Account*>");
 tolua_usertype(tolua_S,"Ember::LoggingService");
 tolua_usertype(tolua_S,"sigc::signal<void,Eris::View*>");
 tolua_usertype(tolua_S,"Atlas::Message::MapType");
}

/* method: getSingletonPtr of class  Ember::LoggingService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_LoggingService_getSingletonPtr00
static int tolua_EmberServices_Ember_LoggingService_getSingletonPtr00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ember::LoggingService",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Ember::LoggingService* tolua_ret = (Ember::LoggingService*)  Ember::LoggingService::getSingletonPtr();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ember::LoggingService");
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

/* method: getSingleton of class  Ember::LoggingService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_LoggingService_getSingleton00
static int tolua_EmberServices_Ember_LoggingService_getSingleton00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ember::LoggingService",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Ember::LoggingService& tolua_ret = (Ember::LoggingService&)  Ember::LoggingService::getSingleton();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"Ember::LoggingService");
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

/* get function: SETVALUE of class  Ember::ConfigService */
#ifndef TOLUA_DISABLE_tolua_get_Ember__ConfigService_SETVALUE
static int tolua_get_Ember__ConfigService_SETVALUE(lua_State* tolua_S)
{
  tolua_pushcppstring(tolua_S,(const char*)Ember::ConfigService::SETVALUE);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: GETVALUE of class  Ember::ConfigService */
#ifndef TOLUA_DISABLE_tolua_get_Ember__ConfigService_GETVALUE
static int tolua_get_Ember__ConfigService_GETVALUE(lua_State* tolua_S)
{
  tolua_pushcppstring(tolua_S,(const char*)Ember::ConfigService::GETVALUE);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getValue of class  Ember::ConfigService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ConfigService_getValue00
static int tolua_EmberServices_Ember_ConfigService_getValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ember::ConfigService",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ember::ConfigService* self = (const Ember::ConfigService*)  tolua_tousertype(tolua_S,1,0);
  const std::string section = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string key = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getValue'",NULL);
#endif
  {
   varconf::Variable tolua_ret = (varconf::Variable)  self->getValue(section,key);
   {
#ifdef __cplusplus
    void* tolua_obj = new varconf::Variable(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"varconf::Variable");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(varconf::Variable));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"varconf::Variable");
#endif
   }
   tolua_pushcppstring(tolua_S,(const char*)section);
   tolua_pushcppstring(tolua_S,(const char*)key);
  }
 }
 return 3;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getValue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setValue of class  Ember::ConfigService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ConfigService_setValue00
static int tolua_EmberServices_Ember_ConfigService_setValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::ConfigService",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"const varconf::Variable",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::ConfigService* self = (Ember::ConfigService*)  tolua_tousertype(tolua_S,1,0);
  const std::string section = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string key = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  const varconf::Variable* value = ((const varconf::Variable*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setValue'",NULL);
#endif
  {
   self->setValue(section,key,*value);
   tolua_pushcppstring(tolua_S,(const char*)section);
   tolua_pushcppstring(tolua_S,(const char*)key);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setValue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isItemSet of class  Ember::ConfigService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ConfigService_isItemSet00
static int tolua_EmberServices_Ember_ConfigService_isItemSet00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ember::ConfigService",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ember::ConfigService* self = (const Ember::ConfigService*)  tolua_tousertype(tolua_S,1,0);
  const std::string section = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string key = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  const std::string value = ((const std::string)  tolua_tocppstring(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isItemSet'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isItemSet(section,key,value);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)section);
   tolua_pushcppstring(tolua_S,(const char*)key);
   tolua_pushcppstring(tolua_S,(const char*)value);
  }
 }
 return 4;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isItemSet'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: itemExists of class  Ember::ConfigService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ConfigService_itemExists00
static int tolua_EmberServices_Ember_ConfigService_itemExists00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ember::ConfigService",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ember::ConfigService* self = (const Ember::ConfigService*)  tolua_tousertype(tolua_S,1,0);
  const std::string section = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string key = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'itemExists'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->itemExists(section,key);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)section);
   tolua_pushcppstring(tolua_S,(const char*)key);
  }
 }
 return 3;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'itemExists'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: deleteItem of class  Ember::ConfigService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ConfigService_deleteItem00
static int tolua_EmberServices_Ember_ConfigService_deleteItem00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::ConfigService",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::ConfigService* self = (Ember::ConfigService*)  tolua_tousertype(tolua_S,1,0);
  const std::string section = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string key = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'deleteItem'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->deleteItem(section,key);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)section);
   tolua_pushcppstring(tolua_S,(const char*)key);
  }
 }
 return 3;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'deleteItem'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: loadSavedConfig of class  Ember::ConfigService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ConfigService_loadSavedConfig00
static int tolua_EmberServices_Ember_ConfigService_loadSavedConfig00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::ConfigService",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::ConfigService* self = (Ember::ConfigService*)  tolua_tousertype(tolua_S,1,0);
  const std::string filename = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'loadSavedConfig'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->loadSavedConfig(filename);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)filename);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'loadSavedConfig'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: saveConfig of class  Ember::ConfigService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ConfigService_saveConfig00
static int tolua_EmberServices_Ember_ConfigService_saveConfig00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::ConfigService",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::ConfigService* self = (Ember::ConfigService*)  tolua_tousertype(tolua_S,1,0);
  const std::string filename = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'saveConfig'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->saveConfig(filename);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)filename);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'saveConfig'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getHomeDirectory of class  Ember::ConfigService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ConfigService_getHomeDirectory00
static int tolua_EmberServices_Ember_ConfigService_getHomeDirectory00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ember::ConfigService",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ember::ConfigService* self = (const Ember::ConfigService*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getHomeDirectory'",NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getHomeDirectory();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getHomeDirectory'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSharedDataDirectory of class  Ember::ConfigService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ConfigService_getSharedDataDirectory00
static int tolua_EmberServices_Ember_ConfigService_getSharedDataDirectory00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ember::ConfigService",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ember::ConfigService* self = (const Ember::ConfigService*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSharedDataDirectory'",NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getSharedDataDirectory();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSharedDataDirectory'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getEmberDataDirectory of class  Ember::ConfigService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ConfigService_getEmberDataDirectory00
static int tolua_EmberServices_Ember_ConfigService_getEmberDataDirectory00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ember::ConfigService",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ember::ConfigService* self = (const Ember::ConfigService*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getEmberDataDirectory'",NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getEmberDataDirectory();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getEmberDataDirectory'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getEmberMediaDirectory of class  Ember::ConfigService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ConfigService_getEmberMediaDirectory00
static int tolua_EmberServices_Ember_ConfigService_getEmberMediaDirectory00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ember::ConfigService",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ember::ConfigService* self = (const Ember::ConfigService*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getEmberMediaDirectory'",NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getEmberMediaDirectory();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getEmberMediaDirectory'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getUserMediaDirectory of class  Ember::ConfigService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ConfigService_getUserMediaDirectory00
static int tolua_EmberServices_Ember_ConfigService_getUserMediaDirectory00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ember::ConfigService",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ember::ConfigService* self = (const Ember::ConfigService*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getUserMediaDirectory'",NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getUserMediaDirectory();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getUserMediaDirectory'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSharedMediaDirectory of class  Ember::ConfigService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ConfigService_getSharedMediaDirectory00
static int tolua_EmberServices_Ember_ConfigService_getSharedMediaDirectory00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ember::ConfigService",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ember::ConfigService* self = (const Ember::ConfigService*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSharedMediaDirectory'",NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getSharedMediaDirectory();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSharedMediaDirectory'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSharedConfigDirectory of class  Ember::ConfigService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ConfigService_getSharedConfigDirectory00
static int tolua_EmberServices_Ember_ConfigService_getSharedConfigDirectory00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ember::ConfigService",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ember::ConfigService* self = (const Ember::ConfigService*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSharedConfigDirectory'",NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getSharedConfigDirectory();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSharedConfigDirectory'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventChangedConfigItem of class  Ember::ConfigService */
#ifndef TOLUA_DISABLE_tolua_get_Ember__ConfigService_EventChangedConfigItem
static int tolua_get_Ember__ConfigService_EventChangedConfigItem(lua_State* tolua_S)
{
  Ember::ConfigService* self = (Ember::ConfigService*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventChangedConfigItem'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventChangedConfigItem,"sigc::signal<void,const std::string&,const std::string&>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventChangedConfigItem of class  Ember::ConfigService */
#ifndef TOLUA_DISABLE_tolua_set_Ember__ConfigService_EventChangedConfigItem
static int tolua_set_Ember__ConfigService_EventChangedConfigItem(lua_State* tolua_S)
{
  Ember::ConfigService* self = (Ember::ConfigService*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventChangedConfigItem'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,const std::string&,const std::string&>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventChangedConfigItem = *((sigc::signal<void,const std::string&,const std::string&>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMetaServer of class  Ember::MetaserverService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_MetaserverService_getMetaServer00
static int tolua_EmberServices_Ember_MetaserverService_getMetaServer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ember::MetaserverService",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ember::MetaserverService* self = (const Ember::MetaserverService*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMetaServer'",NULL);
#endif
  {
   Eris::Meta& tolua_ret = (Eris::Meta&)  self->getMetaServer();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"Eris::Meta");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMetaServer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isConnected of class  Ember::ServerService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ServerService_isConnected00
static int tolua_EmberServices_Ember_ServerService_isConnected00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ember::ServerService",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ember::ServerService* self = (const Ember::ServerService*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isConnected'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isConnected();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isConnected'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: connect of class  Ember::ServerService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ServerService_connect00
static int tolua_EmberServices_Ember_ServerService_connect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::ServerService",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::ServerService* self = (Ember::ServerService*)  tolua_tousertype(tolua_S,1,0);
  const std::string host = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  short port = ((short)  tolua_tonumber(tolua_S,3,6767));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'connect'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->connect(host,port);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)host);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'connect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: disconnect of class  Ember::ServerService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ServerService_disconnect00
static int tolua_EmberServices_Ember_ServerService_disconnect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::ServerService",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::ServerService* self = (Ember::ServerService*)  tolua_tousertype(tolua_S,1,0);
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

/* method: takeCharacter of class  Ember::ServerService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ServerService_takeCharacter00
static int tolua_EmberServices_Ember_ServerService_takeCharacter00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::ServerService",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::ServerService* self = (Ember::ServerService*)  tolua_tousertype(tolua_S,1,0);
  const std::string id = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'takeCharacter'",NULL);
#endif
  {
   self->takeCharacter(id);
   tolua_pushcppstring(tolua_S,(const char*)id);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'takeCharacter'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createCharacter of class  Ember::ServerService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ServerService_createCharacter00
static int tolua_EmberServices_Ember_ServerService_createCharacter00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::ServerService",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::ServerService* self = (Ember::ServerService*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string sex = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  const std::string type = ((const std::string)  tolua_tocppstring(tolua_S,4,0));
  const std::string description = ((const std::string)  tolua_tocppstring(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createCharacter'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->createCharacter(name,sex,type,description);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)name);
   tolua_pushcppstring(tolua_S,(const char*)sex);
   tolua_pushcppstring(tolua_S,(const char*)type);
   tolua_pushcppstring(tolua_S,(const char*)description);
  }
 }
 return 5;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createCharacter'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: moveToPoint of class  Ember::ServerService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ServerService_moveToPoint00
static int tolua_EmberServices_Ember_ServerService_moveToPoint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::ServerService",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const WFMath::Point<3>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::ServerService* self = (Ember::ServerService*)  tolua_tousertype(tolua_S,1,0);
  const WFMath::Point<3>* dest = ((const WFMath::Point<3>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'moveToPoint'",NULL);
#endif
  {
   self->moveToPoint(*dest);
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

/* method: moveInDirection of class  Ember::ServerService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ServerService_moveInDirection00
static int tolua_EmberServices_Ember_ServerService_moveInDirection00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::ServerService",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const WFMath::Vector<3>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const WFMath::Quaternion",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::ServerService* self = (Ember::ServerService*)  tolua_tousertype(tolua_S,1,0);
  const WFMath::Vector<3>* velocity = ((const WFMath::Vector<3>*)  tolua_tousertype(tolua_S,2,0));
  const WFMath::Quaternion* orientation = ((const WFMath::Quaternion*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'moveInDirection'",NULL);
#endif
  {
   self->moveInDirection(*velocity,*orientation);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'moveInDirection'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: moveInDirection of class  Ember::ServerService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ServerService_moveInDirection01
static int tolua_EmberServices_Ember_ServerService_moveInDirection01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::ServerService",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const WFMath::Vector<3>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ember::ServerService* self = (Ember::ServerService*)  tolua_tousertype(tolua_S,1,0);
  const WFMath::Vector<3>* velocity = ((const WFMath::Vector<3>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'moveInDirection'",NULL);
#endif
  {
   self->moveInDirection(*velocity);
  }
 }
 return 0;
tolua_lerror:
 return tolua_EmberServices_Ember_ServerService_moveInDirection00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: say of class  Ember::ServerService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ServerService_say00
static int tolua_EmberServices_Ember_ServerService_say00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::ServerService",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::ServerService* self = (Ember::ServerService*)  tolua_tousertype(tolua_S,1,0);
  const std::string message = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'say'",NULL);
#endif
  {
   self->say(message);
   tolua_pushcppstring(tolua_S,(const char*)message);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'say'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: touch of class  Ember::ServerService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ServerService_touch00
static int tolua_EmberServices_Ember_ServerService_touch00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::ServerService",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Eris::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::ServerService* self = (Ember::ServerService*)  tolua_tousertype(tolua_S,1,0);
  Eris::Entity* entity = ((Eris::Entity*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'touch'",NULL);
#endif
  {
   self->touch(entity);
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

/* method: emote of class  Ember::ServerService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ServerService_emote00
static int tolua_EmberServices_Ember_ServerService_emote00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::ServerService",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::ServerService* self = (Ember::ServerService*)  tolua_tousertype(tolua_S,1,0);
  const std::string emote = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'emote'",NULL);
#endif
  {
   self->emote(emote);
   tolua_pushcppstring(tolua_S,(const char*)emote);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'emote'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: drop of class  Ember::ServerService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ServerService_drop00
static int tolua_EmberServices_Ember_ServerService_drop00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::ServerService",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Eris::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::ServerService* self = (Ember::ServerService*)  tolua_tousertype(tolua_S,1,0);
  Eris::Entity* entity = ((Eris::Entity*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'drop'",NULL);
#endif
  {
   self->drop(entity);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'drop'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: drop of class  Ember::ServerService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ServerService_drop01
static int tolua_EmberServices_Ember_ServerService_drop01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::ServerService",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Eris::Entity",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const WFMath::Vector<3>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ember::ServerService* self = (Ember::ServerService*)  tolua_tousertype(tolua_S,1,0);
  Eris::Entity* entity = ((Eris::Entity*)  tolua_tousertype(tolua_S,2,0));
  const WFMath::Vector<3>* offset = ((const WFMath::Vector<3>*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'drop'",NULL);
#endif
  {
   self->drop(entity,*offset);
  }
 }
 return 0;
tolua_lerror:
 return tolua_EmberServices_Ember_ServerService_drop00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: place of class  Ember::ServerService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ServerService_place00
static int tolua_EmberServices_Ember_ServerService_place00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::ServerService",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Eris::Entity",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"Eris::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::ServerService* self = (Ember::ServerService*)  tolua_tousertype(tolua_S,1,0);
  Eris::Entity* entity = ((Eris::Entity*)  tolua_tousertype(tolua_S,2,0));
  Eris::Entity* target = ((Eris::Entity*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'place'",NULL);
#endif
  {
   self->place(entity,target);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'place'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: wield of class  Ember::ServerService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ServerService_wield00
static int tolua_EmberServices_Ember_ServerService_wield00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::ServerService",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Eris::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::ServerService* self = (Ember::ServerService*)  tolua_tousertype(tolua_S,1,0);
  Eris::Entity* entity = ((Eris::Entity*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'wield'",NULL);
#endif
  {
   self->wield(entity);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'wield'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: take of class  Ember::ServerService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ServerService_take00
static int tolua_EmberServices_Ember_ServerService_take00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::ServerService",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Eris::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::ServerService* self = (Ember::ServerService*)  tolua_tousertype(tolua_S,1,0);
  Eris::Entity* entity = ((Eris::Entity*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'take'",NULL);
#endif
  {
   self->take(entity);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'take'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: use of class  Ember::ServerService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ServerService_use00
static int tolua_EmberServices_Ember_ServerService_use00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::ServerService",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Eris::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::ServerService* self = (Ember::ServerService*)  tolua_tousertype(tolua_S,1,0);
  Eris::Entity* entity = ((Eris::Entity*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'use'",NULL);
#endif
  {
   self->use(entity);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'use'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: use of class  Ember::ServerService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ServerService_use01
static int tolua_EmberServices_Ember_ServerService_use01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::ServerService",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Eris::Entity",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"WFMath::Point<3>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ember::ServerService* self = (Ember::ServerService*)  tolua_tousertype(tolua_S,1,0);
  Eris::Entity* entity = ((Eris::Entity*)  tolua_tousertype(tolua_S,2,0));
  WFMath::Point<3> pos = *((WFMath::Point<3>*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'use'",NULL);
#endif
  {
   self->use(entity,pos);
  }
 }
 return 0;
tolua_lerror:
 return tolua_EmberServices_Ember_ServerService_use00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: use of class  Ember::ServerService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ServerService_use02
static int tolua_EmberServices_Ember_ServerService_use02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::ServerService",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Eris::Entity",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"WFMath::Point<3>",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ember::ServerService* self = (Ember::ServerService*)  tolua_tousertype(tolua_S,1,0);
  Eris::Entity* entity = ((Eris::Entity*)  tolua_tousertype(tolua_S,2,0));
  WFMath::Point<3> pos = *((WFMath::Point<3>*)  tolua_tousertype(tolua_S,3,0));
  const std::string operation = ((const std::string)  tolua_tocppstring(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'use'",NULL);
#endif
  {
   self->use(entity,pos,operation);
   tolua_pushcppstring(tolua_S,(const char*)operation);
  }
 }
 return 1;
tolua_lerror:
 return tolua_EmberServices_Ember_ServerService_use01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: useStop of class  Ember::ServerService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ServerService_useStop00
static int tolua_EmberServices_Ember_ServerService_useStop00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::ServerService",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::ServerService* self = (Ember::ServerService*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'useStop'",NULL);
#endif
  {
   self->useStop();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'useStop'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: actuate of class  Ember::ServerService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ServerService_actuate00
static int tolua_EmberServices_Ember_ServerService_actuate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::ServerService",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Eris::Entity",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::ServerService* self = (Ember::ServerService*)  tolua_tousertype(tolua_S,1,0);
  Eris::Entity* entity = ((Eris::Entity*)  tolua_tousertype(tolua_S,2,0));
  const std::string action = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'actuate'",NULL);
#endif
  {
   self->actuate(entity,action);
   tolua_pushcppstring(tolua_S,(const char*)action);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'actuate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: attack of class  Ember::ServerService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ServerService_attack00
static int tolua_EmberServices_Ember_ServerService_attack00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::ServerService",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Eris::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::ServerService* self = (Ember::ServerService*)  tolua_tousertype(tolua_S,1,0);
  Eris::Entity* entity = ((Eris::Entity*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'attack'",NULL);
#endif
  {
   self->attack(entity);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'attack'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: eat of class  Ember::ServerService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ServerService_eat00
static int tolua_EmberServices_Ember_ServerService_eat00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::ServerService",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Eris::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::ServerService* self = (Ember::ServerService*)  tolua_tousertype(tolua_S,1,0);
  Eris::Entity* entity = ((Eris::Entity*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'eat'",NULL);
#endif
  {
   self->eat(entity);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'eat'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: deleteEntity of class  Ember::ServerService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ServerService_deleteEntity00
static int tolua_EmberServices_Ember_ServerService_deleteEntity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::ServerService",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Eris::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::ServerService* self = (Ember::ServerService*)  tolua_tousertype(tolua_S,1,0);
  Eris::Entity* entity = ((Eris::Entity*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'deleteEntity'",NULL);
#endif
  {
   self->deleteEntity(entity);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'deleteEntity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setAttributes of class  Ember::ServerService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ServerService_setAttributes00
static int tolua_EmberServices_Ember_ServerService_setAttributes00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::ServerService",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Eris::Entity",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"Atlas::Message::MapType",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::ServerService* self = (Ember::ServerService*)  tolua_tousertype(tolua_S,1,0);
  Eris::Entity* entity = ((Eris::Entity*)  tolua_tousertype(tolua_S,2,0));
  Atlas::Message::MapType* attributes = ((Atlas::Message::MapType*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAttributes'",NULL);
#endif
  {
   self->setAttributes(entity,*attributes);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAttributes'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: GotAvatar of class  Ember::ServerService */
#ifndef TOLUA_DISABLE_tolua_get_Ember__ServerService_GotAvatar
static int tolua_get_Ember__ServerService_GotAvatar(lua_State* tolua_S)
{
  Ember::ServerService* self = (Ember::ServerService*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'GotAvatar'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->GotAvatar,"sigc::signal<void,Eris::Avatar*>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: GotAvatar of class  Ember::ServerService */
#ifndef TOLUA_DISABLE_tolua_set_Ember__ServerService_GotAvatar
static int tolua_set_Ember__ServerService_GotAvatar(lua_State* tolua_S)
{
  Ember::ServerService* self = (Ember::ServerService*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'GotAvatar'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,Eris::Avatar*>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->GotAvatar = *((sigc::signal<void,Eris::Avatar*>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: GotView of class  Ember::ServerService */
#ifndef TOLUA_DISABLE_tolua_get_Ember__ServerService_GotView
static int tolua_get_Ember__ServerService_GotView(lua_State* tolua_S)
{
  Ember::ServerService* self = (Ember::ServerService*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'GotView'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->GotView,"sigc::signal<void,Eris::View*>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: GotView of class  Ember::ServerService */
#ifndef TOLUA_DISABLE_tolua_set_Ember__ServerService_GotView
static int tolua_set_Ember__ServerService_GotView(lua_State* tolua_S)
{
  Ember::ServerService* self = (Ember::ServerService*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'GotView'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,Eris::View*>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->GotView = *((sigc::signal<void,Eris::View*>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: GotConnection of class  Ember::ServerService */
#ifndef TOLUA_DISABLE_tolua_get_Ember__ServerService_GotConnection
static int tolua_get_Ember__ServerService_GotConnection(lua_State* tolua_S)
{
  Ember::ServerService* self = (Ember::ServerService*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'GotConnection'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->GotConnection,"sigc::signal<void,Eris::Connection*>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: GotConnection of class  Ember::ServerService */
#ifndef TOLUA_DISABLE_tolua_set_Ember__ServerService_GotConnection
static int tolua_set_Ember__ServerService_GotConnection(lua_State* tolua_S)
{
  Ember::ServerService* self = (Ember::ServerService*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'GotConnection'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,Eris::Connection*>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->GotConnection = *((sigc::signal<void,Eris::Connection*>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: GotAccount of class  Ember::ServerService */
#ifndef TOLUA_DISABLE_tolua_get_Ember__ServerService_GotAccount
static int tolua_get_Ember__ServerService_GotAccount(lua_State* tolua_S)
{
  Ember::ServerService* self = (Ember::ServerService*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'GotAccount'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->GotAccount,"sigc::signal<void,Eris::Account*>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: GotAccount of class  Ember::ServerService */
#ifndef TOLUA_DISABLE_tolua_set_Ember__ServerService_GotAccount
static int tolua_set_Ember__ServerService_GotAccount(lua_State* tolua_S)
{
  Ember::ServerService* self = (Ember::ServerService*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'GotAccount'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,Eris::Account*>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->GotAccount = *((sigc::signal<void,Eris::Account*>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: LoginSuccess of class  Ember::ServerService */
#ifndef TOLUA_DISABLE_tolua_get_Ember__ServerService_LoginSuccess
static int tolua_get_Ember__ServerService_LoginSuccess(lua_State* tolua_S)
{
  Ember::ServerService* self = (Ember::ServerService*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'LoginSuccess'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->LoginSuccess,"sigc::signal<void,Eris::Account*>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: LoginSuccess of class  Ember::ServerService */
#ifndef TOLUA_DISABLE_tolua_set_Ember__ServerService_LoginSuccess
static int tolua_set_Ember__ServerService_LoginSuccess(lua_State* tolua_S)
{
  Ember::ServerService* self = (Ember::ServerService*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'LoginSuccess'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,Eris::Account*>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->LoginSuccess = *((sigc::signal<void,Eris::Account*>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: LoginFailure of class  Ember::ServerService */
#ifndef TOLUA_DISABLE_tolua_get_Ember__ServerService_LoginFailure
static int tolua_get_Ember__ServerService_LoginFailure(lua_State* tolua_S)
{
  Ember::ServerService* self = (Ember::ServerService*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'LoginFailure'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->LoginFailure,"sigc::signal<void,Eris::Account*,const std::string&>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: LoginFailure of class  Ember::ServerService */
#ifndef TOLUA_DISABLE_tolua_set_Ember__ServerService_LoginFailure
static int tolua_set_Ember__ServerService_LoginFailure(lua_State* tolua_S)
{
  Ember::ServerService* self = (Ember::ServerService*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'LoginFailure'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,Eris::Account*,const std::string&>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->LoginFailure = *((sigc::signal<void,Eris::Account*,const std::string&>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: GotCharacterInfo of class  Ember::ServerService */
#ifndef TOLUA_DISABLE_tolua_get_Ember__ServerService_GotCharacterInfo
static int tolua_get_Ember__ServerService_GotCharacterInfo(lua_State* tolua_S)
{
  Ember::ServerService* self = (Ember::ServerService*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'GotCharacterInfo'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->GotCharacterInfo,"sigc::signal<void,const Atlas::Objects::Entity::RootEntity&>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: GotCharacterInfo of class  Ember::ServerService */
#ifndef TOLUA_DISABLE_tolua_set_Ember__ServerService_GotCharacterInfo
static int tolua_set_Ember__ServerService_GotCharacterInfo(lua_State* tolua_S)
{
  Ember::ServerService* self = (Ember::ServerService*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'GotCharacterInfo'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,const Atlas::Objects::Entity::RootEntity&>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->GotCharacterInfo = *((sigc::signal<void,const Atlas::Objects::Entity::RootEntity&>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: GotAllCharacters of class  Ember::ServerService */
#ifndef TOLUA_DISABLE_tolua_get_Ember__ServerService_GotAllCharacters
static int tolua_get_Ember__ServerService_GotAllCharacters(lua_State* tolua_S)
{
  Ember::ServerService* self = (Ember::ServerService*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'GotAllCharacters'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->GotAllCharacters,"sigc::signal<void,Eris::Account*>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: GotAllCharacters of class  Ember::ServerService */
#ifndef TOLUA_DISABLE_tolua_set_Ember__ServerService_GotAllCharacters
static int tolua_set_Ember__ServerService_GotAllCharacters(lua_State* tolua_S)
{
  Ember::ServerService* self = (Ember::ServerService*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'GotAllCharacters'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void,Eris::Account*>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->GotAllCharacters = *((sigc::signal<void,Eris::Account*>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: registerScriptingProvider of class  Ember::ScriptingService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ScriptingService_registerScriptingProvider00
static int tolua_EmberServices_Ember_ScriptingService_registerScriptingProvider00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::ScriptingService",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ember::IScriptingProvider",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::ScriptingService* self = (Ember::ScriptingService*)  tolua_tousertype(tolua_S,1,0);
  Ember::IScriptingProvider* provider = ((Ember::IScriptingProvider*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'registerScriptingProvider'",NULL);
#endif
  {
   self->registerScriptingProvider(provider);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'registerScriptingProvider'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: loadScript of class  Ember::ScriptingService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ScriptingService_loadScript00
static int tolua_EmberServices_Ember_ScriptingService_loadScript00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::ScriptingService",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::ScriptingService* self = (Ember::ScriptingService*)  tolua_tousertype(tolua_S,1,0);
  const std::string script = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'loadScript'",NULL);
#endif
  {
   self->loadScript(script);
   tolua_pushcppstring(tolua_S,(const char*)script);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'loadScript'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: executeCode of class  Ember::ScriptingService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ScriptingService_executeCode00
static int tolua_EmberServices_Ember_ScriptingService_executeCode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::ScriptingService",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::ScriptingService* self = (Ember::ScriptingService*)  tolua_tousertype(tolua_S,1,0);
  const std::string scriptCode = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string scriptType = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'executeCode'",NULL);
#endif
  {
   self->executeCode(scriptCode,scriptType);
   tolua_pushcppstring(tolua_S,(const char*)scriptCode);
   tolua_pushcppstring(tolua_S,(const char*)scriptType);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'executeCode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getEventScriptError of class  Ember::ScriptingService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ScriptingService_getEventScriptError00
static int tolua_EmberServices_Ember_ScriptingService_getEventScriptError00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::ScriptingService",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::ScriptingService* self = (Ember::ScriptingService*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getEventScriptError'",NULL);
#endif
  {
   sigc::signal<void,const std::string&>& tolua_ret = (sigc::signal<void,const std::string&>&)  self->getEventScriptError();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"sigc::signal<void,const std::string&>");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getEventScriptError'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getProviderFor of class  Ember::ScriptingService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ScriptingService_getProviderFor00
static int tolua_EmberServices_Ember_ScriptingService_getProviderFor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::ScriptingService",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::ScriptingService* self = (Ember::ScriptingService*)  tolua_tousertype(tolua_S,1,0);
  const std::string providerName = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getProviderFor'",NULL);
#endif
  {
   Ember::IScriptingProvider* tolua_ret = (Ember::IScriptingProvider*)  self->getProviderFor(providerName);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ember::IScriptingProvider");
   tolua_pushcppstring(tolua_S,(const char*)providerName);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getProviderFor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getProviderNames of class  Ember::ScriptingService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ScriptingService_getProviderNames00
static int tolua_EmberServices_Ember_ScriptingService_getProviderNames00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::ScriptingService",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::ScriptingService* self = (Ember::ScriptingService*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getProviderNames'",NULL);
#endif
  {
   std::vector<std::string> tolua_ret = (std::vector<std::string>)  self->getProviderNames();
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
 tolua_error(tolua_S,"#ferror in function 'getProviderNames'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAlwaysLookup of class  Ember::ScriptingService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ScriptingService_getAlwaysLookup00
static int tolua_EmberServices_Ember_ScriptingService_getAlwaysLookup00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ember::ScriptingService",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ember::ScriptingService* self = (const Ember::ScriptingService*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAlwaysLookup'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->getAlwaysLookup();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAlwaysLookup'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setAlwaysLookup of class  Ember::ScriptingService */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_ScriptingService_setAlwaysLookup00
static int tolua_EmberServices_Ember_ScriptingService_setAlwaysLookup00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::ScriptingService",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::ScriptingService* self = (Ember::ScriptingService*)  tolua_tousertype(tolua_S,1,0);
  bool alwaysLookup = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAlwaysLookup'",NULL);
#endif
  {
   self->setAlwaysLookup(alwaysLookup);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAlwaysLookup'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSingleton of class  Ember::EmberServices */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_EmberServices_getSingleton00
static int tolua_EmberServices_Ember_EmberServices_getSingleton00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ember::EmberServices",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Ember::EmberServices& tolua_ret = (Ember::EmberServices&)  Ember::EmberServices::getSingleton();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"Ember::EmberServices");
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

/* method: getLoggingService of class  Ember::EmberServices */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_EmberServices_getLoggingService00
static int tolua_EmberServices_Ember_EmberServices_getLoggingService00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::EmberServices",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::EmberServices* self = (Ember::EmberServices*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLoggingService'",NULL);
#endif
  {
   Ember::LoggingService* tolua_ret = (Ember::LoggingService*)  self->getLoggingService();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ember::LoggingService");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getLoggingService'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getConfigService of class  Ember::EmberServices */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_EmberServices_getConfigService00
static int tolua_EmberServices_Ember_EmberServices_getConfigService00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::EmberServices",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::EmberServices* self = (Ember::EmberServices*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getConfigService'",NULL);
#endif
  {
   Ember::ConfigService* tolua_ret = (Ember::ConfigService*)  self->getConfigService();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ember::ConfigService");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getConfigService'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMetaserverService of class  Ember::EmberServices */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_EmberServices_getMetaserverService00
static int tolua_EmberServices_Ember_EmberServices_getMetaserverService00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::EmberServices",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::EmberServices* self = (Ember::EmberServices*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMetaserverService'",NULL);
#endif
  {
   Ember::MetaserverService* tolua_ret = (Ember::MetaserverService*)  self->getMetaserverService();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ember::MetaserverService");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMetaserverService'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getServerService of class  Ember::EmberServices */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_EmberServices_getServerService00
static int tolua_EmberServices_Ember_EmberServices_getServerService00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::EmberServices",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::EmberServices* self = (Ember::EmberServices*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getServerService'",NULL);
#endif
  {
   Ember::ServerService* tolua_ret = (Ember::ServerService*)  self->getServerService();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ember::ServerService");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getServerService'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getScriptingService of class  Ember::EmberServices */
#ifndef TOLUA_DISABLE_tolua_EmberServices_Ember_EmberServices_getScriptingService00
static int tolua_EmberServices_Ember_EmberServices_getScriptingService00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::EmberServices",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::EmberServices* self = (Ember::EmberServices*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getScriptingService'",NULL);
#endif
  {
   Ember::ScriptingService* tolua_ret = (Ember::ScriptingService*)  self->getScriptingService();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ember::ScriptingService");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getScriptingService'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_EmberServices_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_module(tolua_S,"Ember",0);
  tolua_beginmodule(tolua_S,"Ember");
   tolua_cclass(tolua_S,"LoggingService","Ember::LoggingService","",NULL);
   tolua_beginmodule(tolua_S,"LoggingService");
    tolua_constant(tolua_S,"VERBOSE",Ember::LoggingService::VERBOSE);
    tolua_constant(tolua_S,"INFO",Ember::LoggingService::INFO);
    tolua_constant(tolua_S,"WARNING",Ember::LoggingService::WARNING);
    tolua_constant(tolua_S,"FAILURE",Ember::LoggingService::FAILURE);
    tolua_constant(tolua_S,"CRITICAL",Ember::LoggingService::CRITICAL);
    tolua_constant(tolua_S,"END_MESSAGE",Ember::LoggingService::END_MESSAGE);
    tolua_function(tolua_S,"getSingletonPtr",tolua_EmberServices_Ember_LoggingService_getSingletonPtr00);
    tolua_function(tolua_S,"getSingleton",tolua_EmberServices_Ember_LoggingService_getSingleton00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"Ember",0);
  tolua_beginmodule(tolua_S,"Ember");
   tolua_cclass(tolua_S,"ConfigService","Ember::ConfigService","",NULL);
   tolua_beginmodule(tolua_S,"ConfigService");
    tolua_variable(tolua_S,"SETVALUE",tolua_get_Ember__ConfigService_SETVALUE,NULL);
    tolua_variable(tolua_S,"GETVALUE",tolua_get_Ember__ConfigService_GETVALUE,NULL);
    tolua_function(tolua_S,"getValue",tolua_EmberServices_Ember_ConfigService_getValue00);
    tolua_function(tolua_S,"setValue",tolua_EmberServices_Ember_ConfigService_setValue00);
    tolua_function(tolua_S,"isItemSet",tolua_EmberServices_Ember_ConfigService_isItemSet00);
    tolua_function(tolua_S,"itemExists",tolua_EmberServices_Ember_ConfigService_itemExists00);
    tolua_function(tolua_S,"deleteItem",tolua_EmberServices_Ember_ConfigService_deleteItem00);
    tolua_function(tolua_S,"loadSavedConfig",tolua_EmberServices_Ember_ConfigService_loadSavedConfig00);
    tolua_function(tolua_S,"saveConfig",tolua_EmberServices_Ember_ConfigService_saveConfig00);
    tolua_function(tolua_S,"getHomeDirectory",tolua_EmberServices_Ember_ConfigService_getHomeDirectory00);
    tolua_function(tolua_S,"getSharedDataDirectory",tolua_EmberServices_Ember_ConfigService_getSharedDataDirectory00);
    tolua_function(tolua_S,"getEmberDataDirectory",tolua_EmberServices_Ember_ConfigService_getEmberDataDirectory00);
    tolua_function(tolua_S,"getEmberMediaDirectory",tolua_EmberServices_Ember_ConfigService_getEmberMediaDirectory00);
    tolua_function(tolua_S,"getUserMediaDirectory",tolua_EmberServices_Ember_ConfigService_getUserMediaDirectory00);
    tolua_function(tolua_S,"getSharedMediaDirectory",tolua_EmberServices_Ember_ConfigService_getSharedMediaDirectory00);
    tolua_function(tolua_S,"getSharedConfigDirectory",tolua_EmberServices_Ember_ConfigService_getSharedConfigDirectory00);
    tolua_variable(tolua_S,"EventChangedConfigItem",tolua_get_Ember__ConfigService_EventChangedConfigItem,tolua_set_Ember__ConfigService_EventChangedConfigItem);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"Ember",0);
  tolua_beginmodule(tolua_S,"Ember");
   tolua_cclass(tolua_S,"MetaserverService","Ember::MetaserverService","",NULL);
   tolua_beginmodule(tolua_S,"MetaserverService");
    tolua_function(tolua_S,"getMetaServer",tolua_EmberServices_Ember_MetaserverService_getMetaServer00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"Ember",0);
  tolua_beginmodule(tolua_S,"Ember");
   tolua_cclass(tolua_S,"ServerService","Ember::ServerService","",NULL);
   tolua_beginmodule(tolua_S,"ServerService");
    tolua_function(tolua_S,"isConnected",tolua_EmberServices_Ember_ServerService_isConnected00);
    tolua_function(tolua_S,"connect",tolua_EmberServices_Ember_ServerService_connect00);
    tolua_function(tolua_S,"disconnect",tolua_EmberServices_Ember_ServerService_disconnect00);
    tolua_function(tolua_S,"takeCharacter",tolua_EmberServices_Ember_ServerService_takeCharacter00);
    tolua_function(tolua_S,"createCharacter",tolua_EmberServices_Ember_ServerService_createCharacter00);
    tolua_function(tolua_S,"moveToPoint",tolua_EmberServices_Ember_ServerService_moveToPoint00);
    tolua_function(tolua_S,"moveInDirection",tolua_EmberServices_Ember_ServerService_moveInDirection00);
    tolua_function(tolua_S,"moveInDirection",tolua_EmberServices_Ember_ServerService_moveInDirection01);
    tolua_function(tolua_S,"say",tolua_EmberServices_Ember_ServerService_say00);
    tolua_function(tolua_S,"touch",tolua_EmberServices_Ember_ServerService_touch00);
    tolua_function(tolua_S,"emote",tolua_EmberServices_Ember_ServerService_emote00);
    tolua_function(tolua_S,"drop",tolua_EmberServices_Ember_ServerService_drop00);
    tolua_function(tolua_S,"drop",tolua_EmberServices_Ember_ServerService_drop01);
    tolua_function(tolua_S,"place",tolua_EmberServices_Ember_ServerService_place00);
    tolua_function(tolua_S,"wield",tolua_EmberServices_Ember_ServerService_wield00);
    tolua_function(tolua_S,"take",tolua_EmberServices_Ember_ServerService_take00);
    tolua_function(tolua_S,"use",tolua_EmberServices_Ember_ServerService_use00);
    tolua_function(tolua_S,"use",tolua_EmberServices_Ember_ServerService_use01);
    tolua_function(tolua_S,"use",tolua_EmberServices_Ember_ServerService_use02);
    tolua_function(tolua_S,"useStop",tolua_EmberServices_Ember_ServerService_useStop00);
    tolua_function(tolua_S,"actuate",tolua_EmberServices_Ember_ServerService_actuate00);
    tolua_function(tolua_S,"attack",tolua_EmberServices_Ember_ServerService_attack00);
    tolua_function(tolua_S,"eat",tolua_EmberServices_Ember_ServerService_eat00);
    tolua_function(tolua_S,"deleteEntity",tolua_EmberServices_Ember_ServerService_deleteEntity00);
    tolua_function(tolua_S,"setAttributes",tolua_EmberServices_Ember_ServerService_setAttributes00);
    tolua_variable(tolua_S,"GotAvatar",tolua_get_Ember__ServerService_GotAvatar,tolua_set_Ember__ServerService_GotAvatar);
    tolua_variable(tolua_S,"GotView",tolua_get_Ember__ServerService_GotView,tolua_set_Ember__ServerService_GotView);
    tolua_variable(tolua_S,"GotConnection",tolua_get_Ember__ServerService_GotConnection,tolua_set_Ember__ServerService_GotConnection);
    tolua_variable(tolua_S,"GotAccount",tolua_get_Ember__ServerService_GotAccount,tolua_set_Ember__ServerService_GotAccount);
    tolua_variable(tolua_S,"LoginSuccess",tolua_get_Ember__ServerService_LoginSuccess,tolua_set_Ember__ServerService_LoginSuccess);
    tolua_variable(tolua_S,"LoginFailure",tolua_get_Ember__ServerService_LoginFailure,tolua_set_Ember__ServerService_LoginFailure);
    tolua_variable(tolua_S,"GotCharacterInfo",tolua_get_Ember__ServerService_GotCharacterInfo,tolua_set_Ember__ServerService_GotCharacterInfo);
    tolua_variable(tolua_S,"GotAllCharacters",tolua_get_Ember__ServerService_GotAllCharacters,tolua_set_Ember__ServerService_GotAllCharacters);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"Ember",0);
  tolua_beginmodule(tolua_S,"Ember");
   tolua_cclass(tolua_S,"ScriptingService","Ember::ScriptingService","",NULL);
   tolua_beginmodule(tolua_S,"ScriptingService");
    tolua_function(tolua_S,"registerScriptingProvider",tolua_EmberServices_Ember_ScriptingService_registerScriptingProvider00);
    tolua_function(tolua_S,"loadScript",tolua_EmberServices_Ember_ScriptingService_loadScript00);
    tolua_function(tolua_S,"executeCode",tolua_EmberServices_Ember_ScriptingService_executeCode00);
    tolua_function(tolua_S,"getEventScriptError",tolua_EmberServices_Ember_ScriptingService_getEventScriptError00);
    tolua_function(tolua_S,"getProviderFor",tolua_EmberServices_Ember_ScriptingService_getProviderFor00);
    tolua_function(tolua_S,"getProviderNames",tolua_EmberServices_Ember_ScriptingService_getProviderNames00);
    tolua_function(tolua_S,"getAlwaysLookup",tolua_EmberServices_Ember_ScriptingService_getAlwaysLookup00);
    tolua_function(tolua_S,"setAlwaysLookup",tolua_EmberServices_Ember_ScriptingService_setAlwaysLookup00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"Ember",0);
  tolua_beginmodule(tolua_S,"Ember");
   tolua_cclass(tolua_S,"EmberServices","Ember::EmberServices","",NULL);
   tolua_beginmodule(tolua_S,"EmberServices");
    tolua_function(tolua_S,"getSingleton",tolua_EmberServices_Ember_EmberServices_getSingleton00);
    tolua_function(tolua_S,"getLoggingService",tolua_EmberServices_Ember_EmberServices_getLoggingService00);
    tolua_function(tolua_S,"getConfigService",tolua_EmberServices_Ember_EmberServices_getConfigService00);
    tolua_function(tolua_S,"getMetaserverService",tolua_EmberServices_Ember_EmberServices_getMetaserverService00);
    tolua_function(tolua_S,"getServerService",tolua_EmberServices_Ember_EmberServices_getServerService00);
    tolua_function(tolua_S,"getScriptingService",tolua_EmberServices_Ember_EmberServices_getScriptingService00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_EmberServices (lua_State* tolua_S) {
 return tolua_EmberServices_open(tolua_S);
};
#endif

