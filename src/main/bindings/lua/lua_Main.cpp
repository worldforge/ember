/*
** Lua binding: Application
** Generated automatically by tolua++-1.0.92 on Thu Apr 12 22:40:51 2007.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_Application_open (lua_State* tolua_S);

#include "required.h"

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"Ember::Application");
 tolua_usertype(tolua_S,"sigc::signal<void>");
 tolua_usertype(tolua_S,"Eris::View");
 tolua_usertype(tolua_S,"sigc::signal<void,bool&>");
}

/* method: getSingleton of class  Ember::Application */
#ifndef TOLUA_DISABLE_tolua_Application_Ember_Application_getSingleton00
static int tolua_Application_Ember_Application_getSingleton00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ember::Application",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Ember::Application& tolua_ret = (Ember::Application&)  Ember::Application::getSingleton();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"Ember::Application");
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

/* method: shouldQuit of class  Ember::Application */
#ifndef TOLUA_DISABLE_tolua_Application_Ember_Application_shouldQuit00
static int tolua_Application_Ember_Application_shouldQuit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::Application",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::Application* self = (Ember::Application*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'shouldQuit'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->shouldQuit();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'shouldQuit'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventServicesInitialized of class  Ember::Application */
#ifndef TOLUA_DISABLE_tolua_get_Ember__Application_EventServicesInitialized
static int tolua_get_Ember__Application_EventServicesInitialized(lua_State* tolua_S)
{
  Ember::Application* self = (Ember::Application*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventServicesInitialized'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventServicesInitialized,"sigc::signal<void>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventServicesInitialized of class  Ember::Application */
#ifndef TOLUA_DISABLE_tolua_set_Ember__Application_EventServicesInitialized
static int tolua_set_Ember__Application_EventServicesInitialized(lua_State* tolua_S)
{
  Ember::Application* self = (Ember::Application*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventServicesInitialized'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventServicesInitialized = *((sigc::signal<void>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: quit of class  Ember::Application */
#ifndef TOLUA_DISABLE_tolua_Application_Ember_Application_quit00
static int tolua_Application_Ember_Application_quit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::Application",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::Application* self = (Ember::Application*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'quit'",NULL);
#endif
  {
   self->quit();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'quit'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setErisPolling of class  Ember::Application */
#ifndef TOLUA_DISABLE_tolua_Application_Ember_Application_setErisPolling00
static int tolua_Application_Ember_Application_setErisPolling00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::Application",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::Application* self = (Ember::Application*)  tolua_tousertype(tolua_S,1,0);
  bool doPoll = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setErisPolling'",NULL);
#endif
  {
   self->setErisPolling(doPoll);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setErisPolling'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getErisPolling of class  Ember::Application */
#ifndef TOLUA_DISABLE_tolua_Application_Ember_Application_getErisPolling00
static int tolua_Application_Ember_Application_getErisPolling00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ember::Application",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ember::Application* self = (const Ember::Application*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getErisPolling'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->getErisPolling();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getErisPolling'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventRequestQuit of class  Ember::Application */
#ifndef TOLUA_DISABLE_tolua_get_Ember__Application_EventRequestQuit
static int tolua_get_Ember__Application_EventRequestQuit(lua_State* tolua_S)
{
  Ember::Application* self = (Ember::Application*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventRequestQuit'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventRequestQuit,"sigc::signal<void,bool&>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventRequestQuit of class  Ember::Application */
#ifndef TOLUA_DISABLE_tolua_set_Ember__Application_EventRequestQuit
static int tolua_set_Ember__Application_EventRequestQuit(lua_State* tolua_S)
{
  Ember::Application* self = (Ember::Application*)  tolua_tousertype(tolua_S,1,0);
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
#endif //#ifndef TOLUA_DISABLE

/* get function: EventStartErisPoll of class  Ember::Application */
#ifndef TOLUA_DISABLE_tolua_get_Ember__Application_EventStartErisPoll
static int tolua_get_Ember__Application_EventStartErisPoll(lua_State* tolua_S)
{
  Ember::Application* self = (Ember::Application*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventStartErisPoll'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventStartErisPoll,"sigc::signal<void>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventStartErisPoll of class  Ember::Application */
#ifndef TOLUA_DISABLE_tolua_set_Ember__Application_EventStartErisPoll
static int tolua_set_Ember__Application_EventStartErisPoll(lua_State* tolua_S)
{
  Ember::Application* self = (Ember::Application*)  tolua_tousertype(tolua_S,1,0);
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
#endif //#ifndef TOLUA_DISABLE

/* get function: EventEndErisPoll of class  Ember::Application */
#ifndef TOLUA_DISABLE_tolua_get_Ember__Application_EventEndErisPoll
static int tolua_get_Ember__Application_EventEndErisPoll(lua_State* tolua_S)
{
  Ember::Application* self = (Ember::Application*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventEndErisPoll'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventEndErisPoll,"sigc::signal<void>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventEndErisPoll of class  Ember::Application */
#ifndef TOLUA_DISABLE_tolua_set_Ember__Application_EventEndErisPoll
static int tolua_set_Ember__Application_EventEndErisPoll(lua_State* tolua_S)
{
  Ember::Application* self = (Ember::Application*)  tolua_tousertype(tolua_S,1,0);
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
#endif //#ifndef TOLUA_DISABLE

/* method: requestQuit of class  Ember::Application */
#ifndef TOLUA_DISABLE_tolua_Application_Ember_Application_requestQuit00
static int tolua_Application_Ember_Application_requestQuit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::Application",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::Application* self = (Ember::Application*)  tolua_tousertype(tolua_S,1,0);
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
#endif //#ifndef TOLUA_DISABLE

/* method: getMainView of class  Ember::Application */
#ifndef TOLUA_DISABLE_tolua_Application_Ember_Application_getMainView00
static int tolua_Application_Ember_Application_getMainView00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::Application",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::Application* self = (Ember::Application*)  tolua_tousertype(tolua_S,1,0);
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
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_Application_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_module(tolua_S,"Ember",0);
  tolua_beginmodule(tolua_S,"Ember");
   tolua_cclass(tolua_S,"Application","Ember::Application","",NULL);
   tolua_beginmodule(tolua_S,"Application");
    tolua_function(tolua_S,"getSingleton",tolua_Application_Ember_Application_getSingleton00);
    tolua_function(tolua_S,"shouldQuit",tolua_Application_Ember_Application_shouldQuit00);
    tolua_variable(tolua_S,"EventServicesInitialized",tolua_get_Ember__Application_EventServicesInitialized,tolua_set_Ember__Application_EventServicesInitialized);
    tolua_function(tolua_S,"quit",tolua_Application_Ember_Application_quit00);
    tolua_function(tolua_S,"setErisPolling",tolua_Application_Ember_Application_setErisPolling00);
    tolua_function(tolua_S,"getErisPolling",tolua_Application_Ember_Application_getErisPolling00);
    tolua_variable(tolua_S,"EventRequestQuit",tolua_get_Ember__Application_EventRequestQuit,tolua_set_Ember__Application_EventRequestQuit);
    tolua_variable(tolua_S,"EventStartErisPoll",tolua_get_Ember__Application_EventStartErisPoll,tolua_set_Ember__Application_EventStartErisPoll);
    tolua_variable(tolua_S,"EventEndErisPoll",tolua_get_Ember__Application_EventEndErisPoll,tolua_set_Ember__Application_EventEndErisPoll);
    tolua_function(tolua_S,"requestQuit",tolua_Application_Ember_Application_requestQuit00);
    tolua_function(tolua_S,"getMainView",tolua_Application_Ember_Application_getMainView00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_Application (lua_State* tolua_S) {
 return tolua_Application_open(tolua_S);
};
#endif

