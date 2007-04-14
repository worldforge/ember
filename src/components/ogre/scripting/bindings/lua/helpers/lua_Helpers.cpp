/*
** Lua binding: Helpers
** Generated automatically by tolua++-1.0.92 on Thu Apr 12 22:31:42 2007.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_Helpers_open (lua_State* tolua_S);

#include "required.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_EmberOgre__LuaConsoleObject (lua_State* tolua_S)
{
 EmberOgre::LuaConsoleObject* self = (EmberOgre::LuaConsoleObject*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__LuaConnector (lua_State* tolua_S)
{
 EmberOgre::LuaConnector* self = (EmberOgre::LuaConnector*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"sigc::signal<void,EmberOgre::EmberEntityFactory*>");
 tolua_usertype(tolua_S,"EmberOgre::LuaConnector");
 tolua_usertype(tolua_S,"sigc::signal<void,const EmberOgre::MousePickerArgs&>");
 tolua_usertype(tolua_S,"sigc::signal<void,EmberOgre::Jesus*>");
 tolua_usertype(tolua_S,"EmberOgre::LuaConsoleObject");
 tolua_usertype(tolua_S,"sigc::signal<void,const EmberOgre::EntityPickResult&,const EmberOgre::MousePickerArgs&>");
 tolua_usertype(tolua_S,"sigc::signal<void,const std::string&,const std::string&>");
 tolua_usertype(tolua_S,"SigC::Signal1<void,const Eris::ServerInfo&>");
 tolua_usertype(tolua_S,"sigc::signal<void,EmberOgre::EmberEntity*>");
 tolua_usertype(tolua_S,"Ogre::Mesh");
 tolua_usertype(tolua_S,"sigc::signal<void,EmberOgre::AvatarEmberEntity*>");
 tolua_usertype(tolua_S,"sigc::signal<void,const std::string&>");
 tolua_usertype(tolua_S,"EmberOgre::OgreUtils");
 tolua_usertype(tolua_S,"sigc::signal<void,EmberOgre::BasePointUserObject*>");
 tolua_usertype(tolua_S,"sigc::signal<void,Eris::Task*>");
 tolua_usertype(tolua_S,"sigc::signal<void,EmberOgre::TerrainEditAction*>");
 tolua_usertype(tolua_S,"sigc::signal<void,EmberOgre::Input::MouseButton,EmberOgre::Input::InputMode>");
 tolua_usertype(tolua_S,"sigc::signal<void>");
 tolua_usertype(tolua_S,"sigc::signal<void,Eris::Connection*>");
 tolua_usertype(tolua_S,"sigc::signal<void,float>");
 tolua_usertype(tolua_S,"sigc::signal<void,const std::string&,EmberOgre::EmberEntity*>");
}

/* method: connect of class  EmberOgre::LuaConnector */
#ifndef TOLUA_DISABLE_tolua_Helpers_EmberOgre_LuaConnector_connect00
static int tolua_Helpers_EmberOgre_LuaConnector_connect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::LuaConnector",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::LuaConnector* self = (EmberOgre::LuaConnector*)  tolua_tousertype(tolua_S,1,0);
  const std::string luaMethod = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'connect'",NULL);
#endif
  {
   self->connect(luaMethod);
   tolua_pushcppstring(tolua_S,(const char*)luaMethod);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'connect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::LuaConnector */
#ifndef TOLUA_DISABLE_tolua_Helpers_EmberOgre_LuaConnector_new00
static int tolua_Helpers_EmberOgre_LuaConnector_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::LuaConnector",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"sigc::signal<void>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  sigc::signal<void>* signal = ((sigc::signal<void>*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::LuaConnector* tolua_ret = (EmberOgre::LuaConnector*)  new EmberOgre::LuaConnector(*signal);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::LuaConnector");
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

/* method: new_local of class  EmberOgre::LuaConnector */
#ifndef TOLUA_DISABLE_tolua_Helpers_EmberOgre_LuaConnector_new00_local
static int tolua_Helpers_EmberOgre_LuaConnector_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::LuaConnector",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"sigc::signal<void>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  sigc::signal<void>* signal = ((sigc::signal<void>*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::LuaConnector* tolua_ret = (EmberOgre::LuaConnector*)  new EmberOgre::LuaConnector(*signal);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::LuaConnector");
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

/* method: new of class  EmberOgre::LuaConnector */
#ifndef TOLUA_DISABLE_tolua_Helpers_EmberOgre_LuaConnector_new01
static int tolua_Helpers_EmberOgre_LuaConnector_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::LuaConnector",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"sigc::signal<void,const std::string&,EmberOgre::EmberEntity*>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  sigc::signal<void,const std::string&,EmberOgre::EmberEntity*>* signal = ((sigc::signal<void,const std::string&,EmberOgre::EmberEntity*>*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::LuaConnector* tolua_ret = (EmberOgre::LuaConnector*)  new EmberOgre::LuaConnector(*signal);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::LuaConnector");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Helpers_EmberOgre_LuaConnector_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::LuaConnector */
#ifndef TOLUA_DISABLE_tolua_Helpers_EmberOgre_LuaConnector_new01_local
static int tolua_Helpers_EmberOgre_LuaConnector_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::LuaConnector",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"sigc::signal<void,const std::string&,EmberOgre::EmberEntity*>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  sigc::signal<void,const std::string&,EmberOgre::EmberEntity*>* signal = ((sigc::signal<void,const std::string&,EmberOgre::EmberEntity*>*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::LuaConnector* tolua_ret = (EmberOgre::LuaConnector*)  new EmberOgre::LuaConnector(*signal);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::LuaConnector");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Helpers_EmberOgre_LuaConnector_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::LuaConnector */
#ifndef TOLUA_DISABLE_tolua_Helpers_EmberOgre_LuaConnector_new02
static int tolua_Helpers_EmberOgre_LuaConnector_new02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::LuaConnector",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"sigc::signal<void,Eris::Connection*>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  sigc::signal<void,Eris::Connection*>* signal = ((sigc::signal<void,Eris::Connection*>*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::LuaConnector* tolua_ret = (EmberOgre::LuaConnector*)  new EmberOgre::LuaConnector(*signal);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::LuaConnector");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Helpers_EmberOgre_LuaConnector_new01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::LuaConnector */
#ifndef TOLUA_DISABLE_tolua_Helpers_EmberOgre_LuaConnector_new02_local
static int tolua_Helpers_EmberOgre_LuaConnector_new02_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::LuaConnector",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"sigc::signal<void,Eris::Connection*>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  sigc::signal<void,Eris::Connection*>* signal = ((sigc::signal<void,Eris::Connection*>*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::LuaConnector* tolua_ret = (EmberOgre::LuaConnector*)  new EmberOgre::LuaConnector(*signal);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::LuaConnector");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Helpers_EmberOgre_LuaConnector_new01_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::LuaConnector */
#ifndef TOLUA_DISABLE_tolua_Helpers_EmberOgre_LuaConnector_new03
static int tolua_Helpers_EmberOgre_LuaConnector_new03(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::LuaConnector",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"SigC::Signal1<void,const Eris::ServerInfo&>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  SigC::Signal1<void,const Eris::ServerInfo&>* signal = ((SigC::Signal1<void,const Eris::ServerInfo&>*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::LuaConnector* tolua_ret = (EmberOgre::LuaConnector*)  new EmberOgre::LuaConnector(*signal);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::LuaConnector");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Helpers_EmberOgre_LuaConnector_new02(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::LuaConnector */
#ifndef TOLUA_DISABLE_tolua_Helpers_EmberOgre_LuaConnector_new03_local
static int tolua_Helpers_EmberOgre_LuaConnector_new03_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::LuaConnector",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"SigC::Signal1<void,const Eris::ServerInfo&>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  SigC::Signal1<void,const Eris::ServerInfo&>* signal = ((SigC::Signal1<void,const Eris::ServerInfo&>*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::LuaConnector* tolua_ret = (EmberOgre::LuaConnector*)  new EmberOgre::LuaConnector(*signal);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::LuaConnector");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Helpers_EmberOgre_LuaConnector_new02_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::LuaConnector */
#ifndef TOLUA_DISABLE_tolua_Helpers_EmberOgre_LuaConnector_new04
static int tolua_Helpers_EmberOgre_LuaConnector_new04(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::LuaConnector",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"sigc::signal<void,float>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  sigc::signal<void,float>* signal = ((sigc::signal<void,float>*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::LuaConnector* tolua_ret = (EmberOgre::LuaConnector*)  new EmberOgre::LuaConnector(*signal);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::LuaConnector");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Helpers_EmberOgre_LuaConnector_new03(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::LuaConnector */
#ifndef TOLUA_DISABLE_tolua_Helpers_EmberOgre_LuaConnector_new04_local
static int tolua_Helpers_EmberOgre_LuaConnector_new04_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::LuaConnector",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"sigc::signal<void,float>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  sigc::signal<void,float>* signal = ((sigc::signal<void,float>*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::LuaConnector* tolua_ret = (EmberOgre::LuaConnector*)  new EmberOgre::LuaConnector(*signal);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::LuaConnector");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Helpers_EmberOgre_LuaConnector_new03_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::LuaConnector */
#ifndef TOLUA_DISABLE_tolua_Helpers_EmberOgre_LuaConnector_new05
static int tolua_Helpers_EmberOgre_LuaConnector_new05(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::LuaConnector",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"sigc::signal<void,const EmberOgre::EntityPickResult&,const EmberOgre::MousePickerArgs&>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  sigc::signal<void,const EmberOgre::EntityPickResult&,const EmberOgre::MousePickerArgs&>* signal = ((sigc::signal<void,const EmberOgre::EntityPickResult&,const EmberOgre::MousePickerArgs&>*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::LuaConnector* tolua_ret = (EmberOgre::LuaConnector*)  new EmberOgre::LuaConnector(*signal);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::LuaConnector");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Helpers_EmberOgre_LuaConnector_new04(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::LuaConnector */
#ifndef TOLUA_DISABLE_tolua_Helpers_EmberOgre_LuaConnector_new05_local
static int tolua_Helpers_EmberOgre_LuaConnector_new05_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::LuaConnector",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"sigc::signal<void,const EmberOgre::EntityPickResult&,const EmberOgre::MousePickerArgs&>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  sigc::signal<void,const EmberOgre::EntityPickResult&,const EmberOgre::MousePickerArgs&>* signal = ((sigc::signal<void,const EmberOgre::EntityPickResult&,const EmberOgre::MousePickerArgs&>*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::LuaConnector* tolua_ret = (EmberOgre::LuaConnector*)  new EmberOgre::LuaConnector(*signal);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::LuaConnector");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Helpers_EmberOgre_LuaConnector_new04_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::LuaConnector */
#ifndef TOLUA_DISABLE_tolua_Helpers_EmberOgre_LuaConnector_new06
static int tolua_Helpers_EmberOgre_LuaConnector_new06(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::LuaConnector",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"sigc::signal<void,const EmberOgre::MousePickerArgs&>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  sigc::signal<void,const EmberOgre::MousePickerArgs&>* signal = ((sigc::signal<void,const EmberOgre::MousePickerArgs&>*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::LuaConnector* tolua_ret = (EmberOgre::LuaConnector*)  new EmberOgre::LuaConnector(*signal);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::LuaConnector");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Helpers_EmberOgre_LuaConnector_new05(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::LuaConnector */
#ifndef TOLUA_DISABLE_tolua_Helpers_EmberOgre_LuaConnector_new06_local
static int tolua_Helpers_EmberOgre_LuaConnector_new06_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::LuaConnector",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"sigc::signal<void,const EmberOgre::MousePickerArgs&>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  sigc::signal<void,const EmberOgre::MousePickerArgs&>* signal = ((sigc::signal<void,const EmberOgre::MousePickerArgs&>*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::LuaConnector* tolua_ret = (EmberOgre::LuaConnector*)  new EmberOgre::LuaConnector(*signal);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::LuaConnector");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Helpers_EmberOgre_LuaConnector_new05_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::LuaConnector */
#ifndef TOLUA_DISABLE_tolua_Helpers_EmberOgre_LuaConnector_new07
static int tolua_Helpers_EmberOgre_LuaConnector_new07(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::LuaConnector",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"sigc::signal<void,EmberOgre::Input::MouseButton,EmberOgre::Input::InputMode>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  sigc::signal<void,EmberOgre::Input::MouseButton,EmberOgre::Input::InputMode>* signal = ((sigc::signal<void,EmberOgre::Input::MouseButton,EmberOgre::Input::InputMode>*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::LuaConnector* tolua_ret = (EmberOgre::LuaConnector*)  new EmberOgre::LuaConnector(*signal);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::LuaConnector");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Helpers_EmberOgre_LuaConnector_new06(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::LuaConnector */
#ifndef TOLUA_DISABLE_tolua_Helpers_EmberOgre_LuaConnector_new07_local
static int tolua_Helpers_EmberOgre_LuaConnector_new07_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::LuaConnector",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"sigc::signal<void,EmberOgre::Input::MouseButton,EmberOgre::Input::InputMode>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  sigc::signal<void,EmberOgre::Input::MouseButton,EmberOgre::Input::InputMode>* signal = ((sigc::signal<void,EmberOgre::Input::MouseButton,EmberOgre::Input::InputMode>*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::LuaConnector* tolua_ret = (EmberOgre::LuaConnector*)  new EmberOgre::LuaConnector(*signal);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::LuaConnector");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Helpers_EmberOgre_LuaConnector_new06_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::LuaConnector */
#ifndef TOLUA_DISABLE_tolua_Helpers_EmberOgre_LuaConnector_new08
static int tolua_Helpers_EmberOgre_LuaConnector_new08(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::LuaConnector",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"sigc::signal<void,EmberOgre::EmberEntityFactory*>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  sigc::signal<void,EmberOgre::EmberEntityFactory*>* signal = ((sigc::signal<void,EmberOgre::EmberEntityFactory*>*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::LuaConnector* tolua_ret = (EmberOgre::LuaConnector*)  new EmberOgre::LuaConnector(*signal);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::LuaConnector");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Helpers_EmberOgre_LuaConnector_new07(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::LuaConnector */
#ifndef TOLUA_DISABLE_tolua_Helpers_EmberOgre_LuaConnector_new08_local
static int tolua_Helpers_EmberOgre_LuaConnector_new08_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::LuaConnector",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"sigc::signal<void,EmberOgre::EmberEntityFactory*>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  sigc::signal<void,EmberOgre::EmberEntityFactory*>* signal = ((sigc::signal<void,EmberOgre::EmberEntityFactory*>*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::LuaConnector* tolua_ret = (EmberOgre::LuaConnector*)  new EmberOgre::LuaConnector(*signal);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::LuaConnector");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Helpers_EmberOgre_LuaConnector_new07_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::LuaConnector */
#ifndef TOLUA_DISABLE_tolua_Helpers_EmberOgre_LuaConnector_new09
static int tolua_Helpers_EmberOgre_LuaConnector_new09(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::LuaConnector",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"sigc::signal<void,EmberOgre::AvatarEmberEntity*>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  sigc::signal<void,EmberOgre::AvatarEmberEntity*>* signal = ((sigc::signal<void,EmberOgre::AvatarEmberEntity*>*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::LuaConnector* tolua_ret = (EmberOgre::LuaConnector*)  new EmberOgre::LuaConnector(*signal);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::LuaConnector");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Helpers_EmberOgre_LuaConnector_new08(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::LuaConnector */
#ifndef TOLUA_DISABLE_tolua_Helpers_EmberOgre_LuaConnector_new09_local
static int tolua_Helpers_EmberOgre_LuaConnector_new09_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::LuaConnector",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"sigc::signal<void,EmberOgre::AvatarEmberEntity*>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  sigc::signal<void,EmberOgre::AvatarEmberEntity*>* signal = ((sigc::signal<void,EmberOgre::AvatarEmberEntity*>*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::LuaConnector* tolua_ret = (EmberOgre::LuaConnector*)  new EmberOgre::LuaConnector(*signal);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::LuaConnector");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Helpers_EmberOgre_LuaConnector_new08_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::LuaConnector */
#ifndef TOLUA_DISABLE_tolua_Helpers_EmberOgre_LuaConnector_new10
static int tolua_Helpers_EmberOgre_LuaConnector_new10(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::LuaConnector",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"sigc::signal<void,EmberOgre::Jesus*>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  sigc::signal<void,EmberOgre::Jesus*>* signal = ((sigc::signal<void,EmberOgre::Jesus*>*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::LuaConnector* tolua_ret = (EmberOgre::LuaConnector*)  new EmberOgre::LuaConnector(*signal);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::LuaConnector");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Helpers_EmberOgre_LuaConnector_new09(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::LuaConnector */
#ifndef TOLUA_DISABLE_tolua_Helpers_EmberOgre_LuaConnector_new10_local
static int tolua_Helpers_EmberOgre_LuaConnector_new10_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::LuaConnector",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"sigc::signal<void,EmberOgre::Jesus*>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  sigc::signal<void,EmberOgre::Jesus*>* signal = ((sigc::signal<void,EmberOgre::Jesus*>*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::LuaConnector* tolua_ret = (EmberOgre::LuaConnector*)  new EmberOgre::LuaConnector(*signal);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::LuaConnector");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Helpers_EmberOgre_LuaConnector_new09_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::LuaConnector */
#ifndef TOLUA_DISABLE_tolua_Helpers_EmberOgre_LuaConnector_new11
static int tolua_Helpers_EmberOgre_LuaConnector_new11(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::LuaConnector",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"sigc::signal<void,EmberOgre::EmberEntity*>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  sigc::signal<void,EmberOgre::EmberEntity*>* signal = ((sigc::signal<void,EmberOgre::EmberEntity*>*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::LuaConnector* tolua_ret = (EmberOgre::LuaConnector*)  new EmberOgre::LuaConnector(*signal);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::LuaConnector");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Helpers_EmberOgre_LuaConnector_new10(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::LuaConnector */
#ifndef TOLUA_DISABLE_tolua_Helpers_EmberOgre_LuaConnector_new11_local
static int tolua_Helpers_EmberOgre_LuaConnector_new11_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::LuaConnector",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"sigc::signal<void,EmberOgre::EmberEntity*>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  sigc::signal<void,EmberOgre::EmberEntity*>* signal = ((sigc::signal<void,EmberOgre::EmberEntity*>*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::LuaConnector* tolua_ret = (EmberOgre::LuaConnector*)  new EmberOgre::LuaConnector(*signal);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::LuaConnector");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Helpers_EmberOgre_LuaConnector_new10_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::LuaConnector */
#ifndef TOLUA_DISABLE_tolua_Helpers_EmberOgre_LuaConnector_new12
static int tolua_Helpers_EmberOgre_LuaConnector_new12(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::LuaConnector",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"sigc::signal<void,const std::string&>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  sigc::signal<void,const std::string&>* signal = ((sigc::signal<void,const std::string&>*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::LuaConnector* tolua_ret = (EmberOgre::LuaConnector*)  new EmberOgre::LuaConnector(*signal);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::LuaConnector");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Helpers_EmberOgre_LuaConnector_new11(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::LuaConnector */
#ifndef TOLUA_DISABLE_tolua_Helpers_EmberOgre_LuaConnector_new12_local
static int tolua_Helpers_EmberOgre_LuaConnector_new12_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::LuaConnector",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"sigc::signal<void,const std::string&>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  sigc::signal<void,const std::string&>* signal = ((sigc::signal<void,const std::string&>*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::LuaConnector* tolua_ret = (EmberOgre::LuaConnector*)  new EmberOgre::LuaConnector(*signal);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::LuaConnector");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Helpers_EmberOgre_LuaConnector_new11_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::LuaConnector */
#ifndef TOLUA_DISABLE_tolua_Helpers_EmberOgre_LuaConnector_new13
static int tolua_Helpers_EmberOgre_LuaConnector_new13(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::LuaConnector",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"sigc::signal<void,const std::string&,const std::string&>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  sigc::signal<void,const std::string&,const std::string&>* signal = ((sigc::signal<void,const std::string&,const std::string&>*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::LuaConnector* tolua_ret = (EmberOgre::LuaConnector*)  new EmberOgre::LuaConnector(*signal);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::LuaConnector");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Helpers_EmberOgre_LuaConnector_new12(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::LuaConnector */
#ifndef TOLUA_DISABLE_tolua_Helpers_EmberOgre_LuaConnector_new13_local
static int tolua_Helpers_EmberOgre_LuaConnector_new13_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::LuaConnector",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"sigc::signal<void,const std::string&,const std::string&>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  sigc::signal<void,const std::string&,const std::string&>* signal = ((sigc::signal<void,const std::string&,const std::string&>*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::LuaConnector* tolua_ret = (EmberOgre::LuaConnector*)  new EmberOgre::LuaConnector(*signal);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::LuaConnector");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Helpers_EmberOgre_LuaConnector_new12_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::LuaConnector */
#ifndef TOLUA_DISABLE_tolua_Helpers_EmberOgre_LuaConnector_new14
static int tolua_Helpers_EmberOgre_LuaConnector_new14(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::LuaConnector",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"sigc::signal<void,EmberOgre::BasePointUserObject*>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  sigc::signal<void,EmberOgre::BasePointUserObject*>* signal = ((sigc::signal<void,EmberOgre::BasePointUserObject*>*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::LuaConnector* tolua_ret = (EmberOgre::LuaConnector*)  new EmberOgre::LuaConnector(*signal);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::LuaConnector");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Helpers_EmberOgre_LuaConnector_new13(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::LuaConnector */
#ifndef TOLUA_DISABLE_tolua_Helpers_EmberOgre_LuaConnector_new14_local
static int tolua_Helpers_EmberOgre_LuaConnector_new14_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::LuaConnector",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"sigc::signal<void,EmberOgre::BasePointUserObject*>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  sigc::signal<void,EmberOgre::BasePointUserObject*>* signal = ((sigc::signal<void,EmberOgre::BasePointUserObject*>*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::LuaConnector* tolua_ret = (EmberOgre::LuaConnector*)  new EmberOgre::LuaConnector(*signal);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::LuaConnector");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Helpers_EmberOgre_LuaConnector_new13_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::LuaConnector */
#ifndef TOLUA_DISABLE_tolua_Helpers_EmberOgre_LuaConnector_new15
static int tolua_Helpers_EmberOgre_LuaConnector_new15(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::LuaConnector",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"sigc::signal<void,EmberOgre::TerrainEditAction*>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  sigc::signal<void,EmberOgre::TerrainEditAction*>* signal = ((sigc::signal<void,EmberOgre::TerrainEditAction*>*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::LuaConnector* tolua_ret = (EmberOgre::LuaConnector*)  new EmberOgre::LuaConnector(*signal);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::LuaConnector");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Helpers_EmberOgre_LuaConnector_new14(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::LuaConnector */
#ifndef TOLUA_DISABLE_tolua_Helpers_EmberOgre_LuaConnector_new15_local
static int tolua_Helpers_EmberOgre_LuaConnector_new15_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::LuaConnector",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"sigc::signal<void,EmberOgre::TerrainEditAction*>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  sigc::signal<void,EmberOgre::TerrainEditAction*>* signal = ((sigc::signal<void,EmberOgre::TerrainEditAction*>*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::LuaConnector* tolua_ret = (EmberOgre::LuaConnector*)  new EmberOgre::LuaConnector(*signal);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::LuaConnector");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Helpers_EmberOgre_LuaConnector_new14_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::LuaConnector */
#ifndef TOLUA_DISABLE_tolua_Helpers_EmberOgre_LuaConnector_new16
static int tolua_Helpers_EmberOgre_LuaConnector_new16(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::LuaConnector",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"sigc::signal<void,Eris::Task*>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  sigc::signal<void,Eris::Task*>* signal = ((sigc::signal<void,Eris::Task*>*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::LuaConnector* tolua_ret = (EmberOgre::LuaConnector*)  new EmberOgre::LuaConnector(*signal);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::LuaConnector");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Helpers_EmberOgre_LuaConnector_new15(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::LuaConnector */
#ifndef TOLUA_DISABLE_tolua_Helpers_EmberOgre_LuaConnector_new16_local
static int tolua_Helpers_EmberOgre_LuaConnector_new16_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::LuaConnector",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"sigc::signal<void,Eris::Task*>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  sigc::signal<void,Eris::Task*>* signal = ((sigc::signal<void,Eris::Task*>*)  tolua_tousertype(tolua_S,2,0));
  {
   EmberOgre::LuaConnector* tolua_ret = (EmberOgre::LuaConnector*)  new EmberOgre::LuaConnector(*signal);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::LuaConnector");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Helpers_EmberOgre_LuaConnector_new15_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::LuaConsoleObject */
#ifndef TOLUA_DISABLE_tolua_Helpers_EmberOgre_LuaConsoleObject_new00
static int tolua_Helpers_EmberOgre_LuaConsoleObject_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::LuaConsoleObject",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::string command = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string luaMethod = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  {
   EmberOgre::LuaConsoleObject* tolua_ret = (EmberOgre::LuaConsoleObject*)  new EmberOgre::LuaConsoleObject(command,luaMethod);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::LuaConsoleObject");
   tolua_pushcppstring(tolua_S,(const char*)command);
   tolua_pushcppstring(tolua_S,(const char*)luaMethod);
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

/* method: new_local of class  EmberOgre::LuaConsoleObject */
#ifndef TOLUA_DISABLE_tolua_Helpers_EmberOgre_LuaConsoleObject_new00_local
static int tolua_Helpers_EmberOgre_LuaConsoleObject_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::LuaConsoleObject",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::string command = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string luaMethod = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  {
   EmberOgre::LuaConsoleObject* tolua_ret = (EmberOgre::LuaConsoleObject*)  new EmberOgre::LuaConsoleObject(command,luaMethod);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::LuaConsoleObject");
   tolua_pushcppstring(tolua_S,(const char*)command);
   tolua_pushcppstring(tolua_S,(const char*)luaMethod);
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

/* method: new of class  EmberOgre::LuaConsoleObject */
#ifndef TOLUA_DISABLE_tolua_Helpers_EmberOgre_LuaConsoleObject_new01
static int tolua_Helpers_EmberOgre_LuaConsoleObject_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::LuaConsoleObject",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const std::string command = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string luaMethod = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  const std::string description = ((const std::string)  tolua_tocppstring(tolua_S,4,0));
  {
   EmberOgre::LuaConsoleObject* tolua_ret = (EmberOgre::LuaConsoleObject*)  new EmberOgre::LuaConsoleObject(command,luaMethod,description);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::LuaConsoleObject");
   tolua_pushcppstring(tolua_S,(const char*)command);
   tolua_pushcppstring(tolua_S,(const char*)luaMethod);
   tolua_pushcppstring(tolua_S,(const char*)description);
  }
 }
 return 4;
tolua_lerror:
 return tolua_Helpers_EmberOgre_LuaConsoleObject_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  EmberOgre::LuaConsoleObject */
#ifndef TOLUA_DISABLE_tolua_Helpers_EmberOgre_LuaConsoleObject_new01_local
static int tolua_Helpers_EmberOgre_LuaConsoleObject_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::LuaConsoleObject",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const std::string command = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string luaMethod = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  const std::string description = ((const std::string)  tolua_tocppstring(tolua_S,4,0));
  {
   EmberOgre::LuaConsoleObject* tolua_ret = (EmberOgre::LuaConsoleObject*)  new EmberOgre::LuaConsoleObject(command,luaMethod,description);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::LuaConsoleObject");
   tolua_pushcppstring(tolua_S,(const char*)command);
   tolua_pushcppstring(tolua_S,(const char*)luaMethod);
   tolua_pushcppstring(tolua_S,(const char*)description);
  }
 }
 return 4;
tolua_lerror:
 return tolua_Helpers_EmberOgre_LuaConsoleObject_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  EmberOgre::LuaConsoleObject */
#ifndef TOLUA_DISABLE_tolua_Helpers_EmberOgre_LuaConsoleObject_delete00
static int tolua_Helpers_EmberOgre_LuaConsoleObject_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::LuaConsoleObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::LuaConsoleObject* self = (EmberOgre::LuaConsoleObject*)  tolua_tousertype(tolua_S,1,0);
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

/* method: getSubMeshName of class  EmberOgre::OgreUtils */
#ifndef TOLUA_DISABLE_tolua_Helpers_EmberOgre_OgreUtils_getSubMeshName00
static int tolua_Helpers_EmberOgre_OgreUtils_getSubMeshName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::OgreUtils",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Ogre::Mesh",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ogre::Mesh* mesh = ((Ogre::Mesh*)  tolua_tousertype(tolua_S,2,0));
  unsigned int subMeshIndex = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
  {
   const std::string tolua_ret = (const std::string)  EmberOgre::OgreUtils::getSubMeshName(mesh,subMeshIndex);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSubMeshName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_Helpers_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"LuaConnector","EmberOgre::LuaConnector","",tolua_collect_EmberOgre__LuaConnector);
   #else
   tolua_cclass(tolua_S,"LuaConnector","EmberOgre::LuaConnector","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"LuaConnector");
    tolua_function(tolua_S,"connect",tolua_Helpers_EmberOgre_LuaConnector_connect00);
    tolua_function(tolua_S,"new",tolua_Helpers_EmberOgre_LuaConnector_new00);
    tolua_function(tolua_S,"new_local",tolua_Helpers_EmberOgre_LuaConnector_new00_local);
    tolua_function(tolua_S,".call",tolua_Helpers_EmberOgre_LuaConnector_new00_local);
    tolua_function(tolua_S,"new",tolua_Helpers_EmberOgre_LuaConnector_new01);
    tolua_function(tolua_S,"new_local",tolua_Helpers_EmberOgre_LuaConnector_new01_local);
    tolua_function(tolua_S,".call",tolua_Helpers_EmberOgre_LuaConnector_new01_local);
    tolua_function(tolua_S,"new",tolua_Helpers_EmberOgre_LuaConnector_new02);
    tolua_function(tolua_S,"new_local",tolua_Helpers_EmberOgre_LuaConnector_new02_local);
    tolua_function(tolua_S,".call",tolua_Helpers_EmberOgre_LuaConnector_new02_local);
    tolua_function(tolua_S,"new",tolua_Helpers_EmberOgre_LuaConnector_new03);
    tolua_function(tolua_S,"new_local",tolua_Helpers_EmberOgre_LuaConnector_new03_local);
    tolua_function(tolua_S,".call",tolua_Helpers_EmberOgre_LuaConnector_new03_local);
    tolua_function(tolua_S,"new",tolua_Helpers_EmberOgre_LuaConnector_new04);
    tolua_function(tolua_S,"new_local",tolua_Helpers_EmberOgre_LuaConnector_new04_local);
    tolua_function(tolua_S,".call",tolua_Helpers_EmberOgre_LuaConnector_new04_local);
    tolua_function(tolua_S,"new",tolua_Helpers_EmberOgre_LuaConnector_new05);
    tolua_function(tolua_S,"new_local",tolua_Helpers_EmberOgre_LuaConnector_new05_local);
    tolua_function(tolua_S,".call",tolua_Helpers_EmberOgre_LuaConnector_new05_local);
    tolua_function(tolua_S,"new",tolua_Helpers_EmberOgre_LuaConnector_new06);
    tolua_function(tolua_S,"new_local",tolua_Helpers_EmberOgre_LuaConnector_new06_local);
    tolua_function(tolua_S,".call",tolua_Helpers_EmberOgre_LuaConnector_new06_local);
    tolua_function(tolua_S,"new",tolua_Helpers_EmberOgre_LuaConnector_new07);
    tolua_function(tolua_S,"new_local",tolua_Helpers_EmberOgre_LuaConnector_new07_local);
    tolua_function(tolua_S,".call",tolua_Helpers_EmberOgre_LuaConnector_new07_local);
    tolua_function(tolua_S,"new",tolua_Helpers_EmberOgre_LuaConnector_new08);
    tolua_function(tolua_S,"new_local",tolua_Helpers_EmberOgre_LuaConnector_new08_local);
    tolua_function(tolua_S,".call",tolua_Helpers_EmberOgre_LuaConnector_new08_local);
    tolua_function(tolua_S,"new",tolua_Helpers_EmberOgre_LuaConnector_new09);
    tolua_function(tolua_S,"new_local",tolua_Helpers_EmberOgre_LuaConnector_new09_local);
    tolua_function(tolua_S,".call",tolua_Helpers_EmberOgre_LuaConnector_new09_local);
    tolua_function(tolua_S,"new",tolua_Helpers_EmberOgre_LuaConnector_new10);
    tolua_function(tolua_S,"new_local",tolua_Helpers_EmberOgre_LuaConnector_new10_local);
    tolua_function(tolua_S,".call",tolua_Helpers_EmberOgre_LuaConnector_new10_local);
    tolua_function(tolua_S,"new",tolua_Helpers_EmberOgre_LuaConnector_new11);
    tolua_function(tolua_S,"new_local",tolua_Helpers_EmberOgre_LuaConnector_new11_local);
    tolua_function(tolua_S,".call",tolua_Helpers_EmberOgre_LuaConnector_new11_local);
    tolua_function(tolua_S,"new",tolua_Helpers_EmberOgre_LuaConnector_new12);
    tolua_function(tolua_S,"new_local",tolua_Helpers_EmberOgre_LuaConnector_new12_local);
    tolua_function(tolua_S,".call",tolua_Helpers_EmberOgre_LuaConnector_new12_local);
    tolua_function(tolua_S,"new",tolua_Helpers_EmberOgre_LuaConnector_new13);
    tolua_function(tolua_S,"new_local",tolua_Helpers_EmberOgre_LuaConnector_new13_local);
    tolua_function(tolua_S,".call",tolua_Helpers_EmberOgre_LuaConnector_new13_local);
    tolua_function(tolua_S,"new",tolua_Helpers_EmberOgre_LuaConnector_new14);
    tolua_function(tolua_S,"new_local",tolua_Helpers_EmberOgre_LuaConnector_new14_local);
    tolua_function(tolua_S,".call",tolua_Helpers_EmberOgre_LuaConnector_new14_local);
    tolua_function(tolua_S,"new",tolua_Helpers_EmberOgre_LuaConnector_new15);
    tolua_function(tolua_S,"new_local",tolua_Helpers_EmberOgre_LuaConnector_new15_local);
    tolua_function(tolua_S,".call",tolua_Helpers_EmberOgre_LuaConnector_new15_local);
    tolua_function(tolua_S,"new",tolua_Helpers_EmberOgre_LuaConnector_new16);
    tolua_function(tolua_S,"new_local",tolua_Helpers_EmberOgre_LuaConnector_new16_local);
    tolua_function(tolua_S,".call",tolua_Helpers_EmberOgre_LuaConnector_new16_local);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"LuaConsoleObject","EmberOgre::LuaConsoleObject","",tolua_collect_EmberOgre__LuaConsoleObject);
   #else
   tolua_cclass(tolua_S,"LuaConsoleObject","EmberOgre::LuaConsoleObject","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"LuaConsoleObject");
    tolua_function(tolua_S,"new",tolua_Helpers_EmberOgre_LuaConsoleObject_new00);
    tolua_function(tolua_S,"new_local",tolua_Helpers_EmberOgre_LuaConsoleObject_new00_local);
    tolua_function(tolua_S,".call",tolua_Helpers_EmberOgre_LuaConsoleObject_new00_local);
    tolua_function(tolua_S,"new",tolua_Helpers_EmberOgre_LuaConsoleObject_new01);
    tolua_function(tolua_S,"new_local",tolua_Helpers_EmberOgre_LuaConsoleObject_new01_local);
    tolua_function(tolua_S,".call",tolua_Helpers_EmberOgre_LuaConsoleObject_new01_local);
    tolua_function(tolua_S,"delete",tolua_Helpers_EmberOgre_LuaConsoleObject_delete00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_cclass(tolua_S,"OgreUtils","EmberOgre::OgreUtils","",NULL);
   tolua_beginmodule(tolua_S,"OgreUtils");
    tolua_function(tolua_S,"getSubMeshName",tolua_Helpers_EmberOgre_OgreUtils_getSubMeshName00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_Helpers (lua_State* tolua_S) {
 return tolua_Helpers_open(tolua_S);
};
#endif

