/*
** Lua binding: Framework
** Generated automatically by tolua++-1.0.92 on Sun May 27 17:32:42 2007.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_Framework_open (lua_State* tolua_S);

#include "required.h"
#include "../../IScriptingProvider.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_Ember__Tokeniser (lua_State* tolua_S)
{
 Ember::Tokeniser* self = (Ember::Tokeniser*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"sigc::signal<bool,const std::string&>");
 tolua_usertype(tolua_S,"Ember::ConsoleBackend");
 tolua_usertype(tolua_S,"Ember::IScriptingProvider");
 tolua_usertype(tolua_S,"Ember::Tokeniser");
}

/* method: getMainConsole of class  Ember::ConsoleBackend */
#ifndef TOLUA_DISABLE_tolua_Framework_Ember_ConsoleBackend_getMainConsole00
static int tolua_Framework_Ember_ConsoleBackend_getMainConsole00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ember::ConsoleBackend",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Ember::ConsoleBackend* tolua_ret = (Ember::ConsoleBackend*)  Ember::ConsoleBackend::getMainConsole();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ember::ConsoleBackend");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMainConsole'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: runCommand of class  Ember::ConsoleBackend */
#ifndef TOLUA_DISABLE_tolua_Framework_Ember_ConsoleBackend_runCommand00
static int tolua_Framework_Ember_ConsoleBackend_runCommand00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::ConsoleBackend",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::ConsoleBackend* self = (Ember::ConsoleBackend*)  tolua_tousertype(tolua_S,1,0);
  const std::string command = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  bool addToHistory = ((bool)  tolua_toboolean(tolua_S,3,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'runCommand'",NULL);
#endif
  {
   self->runCommand(command,addToHistory);
   tolua_pushcppstring(tolua_S,(const char*)command);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'runCommand'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: pushMessage of class  Ember::ConsoleBackend */
#ifndef TOLUA_DISABLE_tolua_Framework_Ember_ConsoleBackend_pushMessage00
static int tolua_Framework_Ember_ConsoleBackend_pushMessage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::ConsoleBackend",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::ConsoleBackend* self = (Ember::ConsoleBackend*)  tolua_tousertype(tolua_S,1,0);
  const std::string message = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'pushMessage'",NULL);
#endif
  {
   self->pushMessage(message);
   tolua_pushcppstring(tolua_S,(const char*)message);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'pushMessage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: runCommand of class  Ember::ConsoleBackend */
#ifndef TOLUA_DISABLE_tolua_Framework_Ember_ConsoleBackend_runCommand01
static int tolua_Framework_Ember_ConsoleBackend_runCommand01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::ConsoleBackend",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Ember::ConsoleBackend* self = (Ember::ConsoleBackend*)  tolua_tousertype(tolua_S,1,0);
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
tolua_lerror:
 return tolua_Framework_Ember_ConsoleBackend_runCommand00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* get function: GotMessage of class  Ember::ConsoleBackend */
#ifndef TOLUA_DISABLE_tolua_get_Ember__ConsoleBackend_GotMessage
static int tolua_get_Ember__ConsoleBackend_GotMessage(lua_State* tolua_S)
{
  Ember::ConsoleBackend* self = (Ember::ConsoleBackend*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'GotMessage'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->GotMessage,"sigc::signal<bool,const std::string&>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: GotMessage of class  Ember::ConsoleBackend */
#ifndef TOLUA_DISABLE_tolua_set_Ember__ConsoleBackend_GotMessage
static int tolua_set_Ember__ConsoleBackend_GotMessage(lua_State* tolua_S)
{
  Ember::ConsoleBackend* self = (Ember::ConsoleBackend*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'GotMessage'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<bool,const std::string&>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->GotMessage = *((sigc::signal<bool,const std::string&>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Ember::Tokeniser */
#ifndef TOLUA_DISABLE_tolua_Framework_Ember_Tokeniser_new00
static int tolua_Framework_Ember_Tokeniser_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ember::Tokeniser",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Ember::Tokeniser* tolua_ret = (Ember::Tokeniser*)  new Ember::Tokeniser();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ember::Tokeniser");
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

/* method: new_local of class  Ember::Tokeniser */
#ifndef TOLUA_DISABLE_tolua_Framework_Ember_Tokeniser_new00_local
static int tolua_Framework_Ember_Tokeniser_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ember::Tokeniser",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Ember::Tokeniser* tolua_ret = (Ember::Tokeniser*)  new Ember::Tokeniser();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"Ember::Tokeniser");
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

/* method: delete of class  Ember::Tokeniser */
#ifndef TOLUA_DISABLE_tolua_Framework_Ember_Tokeniser_delete00
static int tolua_Framework_Ember_Tokeniser_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::Tokeniser",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::Tokeniser* self = (Ember::Tokeniser*)  tolua_tousertype(tolua_S,1,0);
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

/* method: initTokens of class  Ember::Tokeniser */
#ifndef TOLUA_DISABLE_tolua_Framework_Ember_Tokeniser_initTokens00
static int tolua_Framework_Ember_Tokeniser_initTokens00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::Tokeniser",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::Tokeniser* self = (Ember::Tokeniser*)  tolua_tousertype(tolua_S,1,0);
  const std::string tokens = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'initTokens'",NULL);
#endif
  {
   self->initTokens(tokens);
   tolua_pushcppstring(tolua_S,(const char*)tokens);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'initTokens'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: nextToken of class  Ember::Tokeniser */
#ifndef TOLUA_DISABLE_tolua_Framework_Ember_Tokeniser_nextToken00
static int tolua_Framework_Ember_Tokeniser_nextToken00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::Tokeniser",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::Tokeniser* self = (Ember::Tokeniser*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'nextToken'",NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->nextToken();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'nextToken'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: remainingTokens of class  Ember::Tokeniser */
#ifndef TOLUA_DISABLE_tolua_Framework_Ember_Tokeniser_remainingTokens00
static int tolua_Framework_Ember_Tokeniser_remainingTokens00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::Tokeniser",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::Tokeniser* self = (Ember::Tokeniser*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'remainingTokens'",NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->remainingTokens();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'remainingTokens'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: executeScript of class  Ember::IScriptingProvider */
#ifndef TOLUA_DISABLE_tolua_Framework_Ember_IScriptingProvider_executeScript00
static int tolua_Framework_Ember_IScriptingProvider_executeScript00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::IScriptingProvider",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::IScriptingProvider* self = (Ember::IScriptingProvider*)  tolua_tousertype(tolua_S,1,0);
  const std::string scriptCode = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'executeScript'",NULL);
#endif
  {
   self->executeScript(scriptCode);
   tolua_pushcppstring(tolua_S,(const char*)scriptCode);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'executeScript'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: willLoadScript of class  Ember::IScriptingProvider */
#ifndef TOLUA_DISABLE_tolua_Framework_Ember_IScriptingProvider_willLoadScript00
static int tolua_Framework_Ember_IScriptingProvider_willLoadScript00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::IScriptingProvider",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::IScriptingProvider* self = (Ember::IScriptingProvider*)  tolua_tousertype(tolua_S,1,0);
  const std::string scriptName = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'willLoadScript'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->willLoadScript(scriptName);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)scriptName);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'willLoadScript'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getName of class  Ember::IScriptingProvider */
#ifndef TOLUA_DISABLE_tolua_Framework_Ember_IScriptingProvider_getName00
static int tolua_Framework_Ember_IScriptingProvider_getName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Ember::IScriptingProvider",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Ember::IScriptingProvider* self = (const Ember::IScriptingProvider*)  tolua_tousertype(tolua_S,1,0);
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

/* method: forceGC of class  Ember::IScriptingProvider */
#ifndef TOLUA_DISABLE_tolua_Framework_Ember_IScriptingProvider_forceGC00
static int tolua_Framework_Ember_IScriptingProvider_forceGC00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::IScriptingProvider",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::IScriptingProvider* self = (Ember::IScriptingProvider*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'forceGC'",NULL);
#endif
  {
   self->forceGC();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'forceGC'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_Framework_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_module(tolua_S,"Ember",0);
  tolua_beginmodule(tolua_S,"Ember");
   tolua_cclass(tolua_S,"ConsoleBackend","Ember::ConsoleBackend","",NULL);
   tolua_beginmodule(tolua_S,"ConsoleBackend");
    tolua_function(tolua_S,"getMainConsole",tolua_Framework_Ember_ConsoleBackend_getMainConsole00);
    tolua_function(tolua_S,"runCommand",tolua_Framework_Ember_ConsoleBackend_runCommand00);
    tolua_function(tolua_S,"pushMessage",tolua_Framework_Ember_ConsoleBackend_pushMessage00);
    tolua_function(tolua_S,"runCommand",tolua_Framework_Ember_ConsoleBackend_runCommand01);
    tolua_variable(tolua_S,"GotMessage",tolua_get_Ember__ConsoleBackend_GotMessage,tolua_set_Ember__ConsoleBackend_GotMessage);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"Ember",0);
  tolua_beginmodule(tolua_S,"Ember");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Tokeniser","Ember::Tokeniser","",tolua_collect_Ember__Tokeniser);
   #else
   tolua_cclass(tolua_S,"Tokeniser","Ember::Tokeniser","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Tokeniser");
    tolua_function(tolua_S,"new",tolua_Framework_Ember_Tokeniser_new00);
    tolua_function(tolua_S,"new_local",tolua_Framework_Ember_Tokeniser_new00_local);
    tolua_function(tolua_S,".call",tolua_Framework_Ember_Tokeniser_new00_local);
    tolua_function(tolua_S,"delete",tolua_Framework_Ember_Tokeniser_delete00);
    tolua_function(tolua_S,"initTokens",tolua_Framework_Ember_Tokeniser_initTokens00);
    tolua_function(tolua_S,"nextToken",tolua_Framework_Ember_Tokeniser_nextToken00);
    tolua_function(tolua_S,"remainingTokens",tolua_Framework_Ember_Tokeniser_remainingTokens00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"Ember",0);
  tolua_beginmodule(tolua_S,"Ember");
   tolua_cclass(tolua_S,"IScriptingProvider","Ember::IScriptingProvider","",NULL);
   tolua_beginmodule(tolua_S,"IScriptingProvider");
    tolua_function(tolua_S,"executeScript",tolua_Framework_Ember_IScriptingProvider_executeScript00);
    tolua_function(tolua_S,"willLoadScript",tolua_Framework_Ember_IScriptingProvider_willLoadScript00);
    tolua_function(tolua_S,"getName",tolua_Framework_Ember_IScriptingProvider_getName00);
    tolua_function(tolua_S,"forceGC",tolua_Framework_Ember_IScriptingProvider_forceGC00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_Framework (lua_State* tolua_S) {
 return tolua_Framework_open(tolua_S);
};
#endif

