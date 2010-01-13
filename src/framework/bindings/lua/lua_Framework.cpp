/*
** Lua binding: Framework
** Generated automatically by tolua++-1.0.92.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "components/lua/tolua++.h"

/* Exported function */
TOLUA_API int  tolua_Framework_open (lua_State* tolua_S);

#include "required.h"
#include <ostream>
#include <istream>
#include <sstream>
#include "framework/AttributeObserver.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_Ember__AttributeObserver (lua_State* tolua_S)
{
 Ember::AttributeObserver* self = (Ember::AttributeObserver*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_std__stringstream (lua_State* tolua_S)
{
 std::stringstream* self = (std::stringstream*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

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
 tolua_usertype(tolua_S,"std::ostream");
 tolua_usertype(tolua_S,"std::iostream");
 tolua_usertype(tolua_S,"std::vector<std::string>");
 tolua_usertype(tolua_S,"std::stringstream");
 tolua_usertype(tolua_S,"sigc::signal<bool,const std::string&>");
 tolua_usertype(tolua_S,"sigc::signal<void,const Atlas::Message::Element&>");
 tolua_usertype(tolua_S,"Eris::Entity");
 tolua_usertype(tolua_S,"Ember::Tokeniser");
 tolua_usertype(tolua_S,"Ember::AttributeObserver");
 tolua_usertype(tolua_S,"Ember::Log");
 tolua_usertype(tolua_S,"std::istream");
 tolua_usertype(tolua_S,"Ember::ConsoleBackend");
}

/* method: getSingletonPtr of class  Ember::ConsoleBackend */
#ifndef TOLUA_DISABLE_tolua_Framework_Ember_ConsoleBackend_getSingletonPtr00
static int tolua_Framework_Ember_ConsoleBackend_getSingletonPtr00(lua_State* tolua_S)
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
   Ember::ConsoleBackend* tolua_ret = (Ember::ConsoleBackend*)  Ember::ConsoleBackend::getSingletonPtr();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ember::ConsoleBackend");
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

/* method: getSingleton of class  Ember::ConsoleBackend */
#ifndef TOLUA_DISABLE_tolua_Framework_Ember_ConsoleBackend_getSingleton00
static int tolua_Framework_Ember_ConsoleBackend_getSingleton00(lua_State* tolua_S)
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
   Ember::ConsoleBackend& tolua_ret = (Ember::ConsoleBackend&)  Ember::ConsoleBackend::getSingleton();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"Ember::ConsoleBackend");
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

/* method: log of class  Ember::Log */
#ifndef TOLUA_DISABLE_tolua_Framework_Ember_Log_log00
static int tolua_Framework_Ember_Log_log00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ember::Log",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* message = ((const char*)  tolua_tostring(tolua_S,2,0));
  {
   Ember::Log::log(message);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'log'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: log of class  Ember::Log */
#ifndef TOLUA_DISABLE_tolua_Framework_Ember_Log_log01
static int tolua_Framework_Ember_Log_log01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ember::Log",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Ember::Log::MessageImportance importance = ((const Ember::Log::MessageImportance)  tolua_tonumber(tolua_S,2,0));
  const char* message = ((const char*)  tolua_tostring(tolua_S,3,0));
  {
   Ember::Log::log(importance,message);
  }
 }
 return 0;
tolua_lerror:
 return tolua_Framework_Ember_Log_log00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* function: S_LOG_VERBOSE */
#ifndef TOLUA_DISABLE_tolua_Framework_S_LOG_VERBOSE00
static int tolua_Framework_S_LOG_VERBOSE00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* message = ((const char*)  tolua_tostring(tolua_S,1,0));
  {
   S_LOG_VERBOSE(message);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'S_LOG_VERBOSE'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: S_LOG_INFO */
#ifndef TOLUA_DISABLE_tolua_Framework_S_LOG_INFO00
static int tolua_Framework_S_LOG_INFO00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* message = ((const char*)  tolua_tostring(tolua_S,1,0));
  {
   S_LOG_INFO(message);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'S_LOG_INFO'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: S_LOG_WARNING */
#ifndef TOLUA_DISABLE_tolua_Framework_S_LOG_WARNING00
static int tolua_Framework_S_LOG_WARNING00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* message = ((const char*)  tolua_tostring(tolua_S,1,0));
  {
   S_LOG_WARNING(message);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'S_LOG_WARNING'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: S_LOG_FAILURE */
#ifndef TOLUA_DISABLE_tolua_Framework_S_LOG_FAILURE00
static int tolua_Framework_S_LOG_FAILURE00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* message = ((const char*)  tolua_tostring(tolua_S,1,0));
  {
   S_LOG_FAILURE(message);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'S_LOG_FAILURE'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: S_LOG_CRITICAL */
#ifndef TOLUA_DISABLE_tolua_Framework_S_LOG_CRITICAL00
static int tolua_Framework_S_LOG_CRITICAL00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* message = ((const char*)  tolua_tostring(tolua_S,1,0));
  {
   S_LOG_CRITICAL(message);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'S_LOG_CRITICAL'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  std::stringstream */
#ifndef TOLUA_DISABLE_tolua_Framework_std_stringstream_new00
static int tolua_Framework_std_stringstream_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"std::stringstream",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   std::stringstream* tolua_ret = (std::stringstream*)  new std::stringstream();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"std::stringstream");
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

/* method: new_local of class  std::stringstream */
#ifndef TOLUA_DISABLE_tolua_Framework_std_stringstream_new00_local
static int tolua_Framework_std_stringstream_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"std::stringstream",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   std::stringstream* tolua_ret = (std::stringstream*)  new std::stringstream();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"std::stringstream");
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

/* method: new of class  std::stringstream */
#ifndef TOLUA_DISABLE_tolua_Framework_std_stringstream_new01
static int tolua_Framework_std_stringstream_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"std::stringstream",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const std::string str = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  {
   std::stringstream* tolua_ret = (std::stringstream*)  new std::stringstream(str);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"std::stringstream");
   tolua_pushcppstring(tolua_S,(const char*)str);
  }
 }
 return 2;
tolua_lerror:
 return tolua_Framework_std_stringstream_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  std::stringstream */
#ifndef TOLUA_DISABLE_tolua_Framework_std_stringstream_new01_local
static int tolua_Framework_std_stringstream_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"std::stringstream",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const std::string str = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  {
   std::stringstream* tolua_ret = (std::stringstream*)  new std::stringstream(str);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"std::stringstream");
   tolua_pushcppstring(tolua_S,(const char*)str);
  }
 }
 return 2;
tolua_lerror:
 return tolua_Framework_std_stringstream_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  std::stringstream */
#ifndef TOLUA_DISABLE_tolua_Framework_std_stringstream_delete00
static int tolua_Framework_std_stringstream_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"std::stringstream",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::stringstream* self = (std::stringstream*)  tolua_tousertype(tolua_S,1,0);
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

/* method: str of class  std::stringstream */
#ifndef TOLUA_DISABLE_tolua_Framework_std_stringstream_str00
static int tolua_Framework_std_stringstream_str00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const std::stringstream",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::stringstream* self = (const std::stringstream*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'str'",NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->str();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'str'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: __std of class  std::stringstream */
#ifndef TOLUA_DISABLE_tolua_get_std__stringstream___std__istream__
static int tolua_get_std__stringstream___std__istream__(lua_State* tolua_S)
{
  std::stringstream* self = (std::stringstream*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable '__std'",NULL);
#endif
#ifdef __cplusplus
  tolua_pushusertype(tolua_S,(void*)static_cast<std::istream*>(self), "std::istream");
#else
  tolua_pushusertype(tolua_S,(void*)((std::istream*)self), "std::istream");
#endif
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: __std of class  std::stringstream */
#ifndef TOLUA_DISABLE_tolua_get_std__stringstream___std__ostream__
static int tolua_get_std__stringstream___std__ostream__(lua_State* tolua_S)
{
  std::stringstream* self = (std::stringstream*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable '__std'",NULL);
#endif
#ifdef __cplusplus
  tolua_pushusertype(tolua_S,(void*)static_cast<std::ostream*>(self), "std::ostream");
#else
  tolua_pushusertype(tolua_S,(void*)((std::ostream*)self), "std::ostream");
#endif
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Ember::AttributeObserver */
#ifndef TOLUA_DISABLE_tolua_Framework_Ember_AttributeObserver_new00
static int tolua_Framework_Ember_AttributeObserver_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ember::AttributeObserver",0,&tolua_err) ||
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
   Ember::AttributeObserver* tolua_ret = (Ember::AttributeObserver*)  new Ember::AttributeObserver(*entity,attributeName);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ember::AttributeObserver");
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

/* method: new_local of class  Ember::AttributeObserver */
#ifndef TOLUA_DISABLE_tolua_Framework_Ember_AttributeObserver_new00_local
static int tolua_Framework_Ember_AttributeObserver_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ember::AttributeObserver",0,&tolua_err) ||
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
   Ember::AttributeObserver* tolua_ret = (Ember::AttributeObserver*)  new Ember::AttributeObserver(*entity,attributeName);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"Ember::AttributeObserver");
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

/* method: new of class  Ember::AttributeObserver */
#ifndef TOLUA_DISABLE_tolua_Framework_Ember_AttributeObserver_new01
static int tolua_Framework_Ember_AttributeObserver_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ember::AttributeObserver",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Eris::Entity",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const std::vector<std::string>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Eris::Entity* entity = ((Eris::Entity*)  tolua_tousertype(tolua_S,2,0));
  const std::vector<std::string>* attributePath = ((const std::vector<std::string>*)  tolua_tousertype(tolua_S,3,0));
  {
   Ember::AttributeObserver* tolua_ret = (Ember::AttributeObserver*)  new Ember::AttributeObserver(*entity,*attributePath);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ember::AttributeObserver");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Framework_Ember_AttributeObserver_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Ember::AttributeObserver */
#ifndef TOLUA_DISABLE_tolua_Framework_Ember_AttributeObserver_new01_local
static int tolua_Framework_Ember_AttributeObserver_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ember::AttributeObserver",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Eris::Entity",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const std::vector<std::string>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Eris::Entity* entity = ((Eris::Entity*)  tolua_tousertype(tolua_S,2,0));
  const std::vector<std::string>* attributePath = ((const std::vector<std::string>*)  tolua_tousertype(tolua_S,3,0));
  {
   Ember::AttributeObserver* tolua_ret = (Ember::AttributeObserver*)  new Ember::AttributeObserver(*entity,*attributePath);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"Ember::AttributeObserver");
  }
 }
 return 1;
tolua_lerror:
 return tolua_Framework_Ember_AttributeObserver_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Ember::AttributeObserver */
#ifndef TOLUA_DISABLE_tolua_Framework_Ember_AttributeObserver_new02
static int tolua_Framework_Ember_AttributeObserver_new02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ember::AttributeObserver",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Eris::Entity",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Eris::Entity* entity = ((Eris::Entity*)  tolua_tousertype(tolua_S,2,0));
  const std::string attributePath = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  const std::string delimitor = ((const std::string)  tolua_tocppstring(tolua_S,4,0));
  {
   Ember::AttributeObserver* tolua_ret = (Ember::AttributeObserver*)  new Ember::AttributeObserver(*entity,attributePath,delimitor);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Ember::AttributeObserver");
   tolua_pushcppstring(tolua_S,(const char*)attributePath);
   tolua_pushcppstring(tolua_S,(const char*)delimitor);
  }
 }
 return 3;
tolua_lerror:
 return tolua_Framework_Ember_AttributeObserver_new01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Ember::AttributeObserver */
#ifndef TOLUA_DISABLE_tolua_Framework_Ember_AttributeObserver_new02_local
static int tolua_Framework_Ember_AttributeObserver_new02_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Ember::AttributeObserver",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Eris::Entity",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Eris::Entity* entity = ((Eris::Entity*)  tolua_tousertype(tolua_S,2,0));
  const std::string attributePath = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  const std::string delimitor = ((const std::string)  tolua_tocppstring(tolua_S,4,0));
  {
   Ember::AttributeObserver* tolua_ret = (Ember::AttributeObserver*)  new Ember::AttributeObserver(*entity,attributePath,delimitor);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"Ember::AttributeObserver");
   tolua_pushcppstring(tolua_S,(const char*)attributePath);
   tolua_pushcppstring(tolua_S,(const char*)delimitor);
  }
 }
 return 3;
tolua_lerror:
 return tolua_Framework_Ember_AttributeObserver_new01_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  Ember::AttributeObserver */
#ifndef TOLUA_DISABLE_tolua_Framework_Ember_AttributeObserver_delete00
static int tolua_Framework_Ember_AttributeObserver_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::AttributeObserver",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::AttributeObserver* self = (Ember::AttributeObserver*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: EventChanged of class  Ember::AttributeObserver */
#ifndef TOLUA_DISABLE_tolua_get_Ember__AttributeObserver_EventChanged
static int tolua_get_Ember__AttributeObserver_EventChanged(lua_State* tolua_S)
{
  Ember::AttributeObserver* self = (Ember::AttributeObserver*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventChanged'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventChanged,"sigc::signal<void,const Atlas::Message::Element&>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventChanged of class  Ember::AttributeObserver */
#ifndef TOLUA_DISABLE_tolua_set_Ember__AttributeObserver_EventChanged
static int tolua_set_Ember__AttributeObserver_EventChanged(lua_State* tolua_S)
{
  Ember::AttributeObserver* self = (Ember::AttributeObserver*)  tolua_tousertype(tolua_S,1,0);
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

/* method: forceEvaluation of class  Ember::AttributeObserver */
#ifndef TOLUA_DISABLE_tolua_Framework_Ember_AttributeObserver_forceEvaluation00
static int tolua_Framework_Ember_AttributeObserver_forceEvaluation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Ember::AttributeObserver",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Ember::AttributeObserver* self = (Ember::AttributeObserver*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'forceEvaluation'",NULL);
#endif
  {
   self->forceEvaluation();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'forceEvaluation'.",&tolua_err);
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
    tolua_function(tolua_S,"getSingletonPtr",tolua_Framework_Ember_ConsoleBackend_getSingletonPtr00);
    tolua_function(tolua_S,"getSingleton",tolua_Framework_Ember_ConsoleBackend_getSingleton00);
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
   tolua_cclass(tolua_S,"Log","Ember::Log","",NULL);
   tolua_beginmodule(tolua_S,"Log");
    tolua_constant(tolua_S,"VERBOSE",Ember::Log::VERBOSE);
    tolua_constant(tolua_S,"INFO",Ember::Log::INFO);
    tolua_constant(tolua_S,"WARNING",Ember::Log::WARNING);
    tolua_constant(tolua_S,"FAILURE",Ember::Log::FAILURE);
    tolua_constant(tolua_S,"CRITICAL",Ember::Log::CRITICAL);
    tolua_constant(tolua_S,"END_MESSAGE",Ember::Log::END_MESSAGE);
    tolua_function(tolua_S,"log",tolua_Framework_Ember_Log_log00);
    tolua_function(tolua_S,"log",tolua_Framework_Ember_Log_log01);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_function(tolua_S,"S_LOG_VERBOSE",tolua_Framework_S_LOG_VERBOSE00);
  tolua_function(tolua_S,"S_LOG_INFO",tolua_Framework_S_LOG_INFO00);
  tolua_function(tolua_S,"S_LOG_WARNING",tolua_Framework_S_LOG_WARNING00);
  tolua_function(tolua_S,"S_LOG_FAILURE",tolua_Framework_S_LOG_FAILURE00);
  tolua_function(tolua_S,"S_LOG_CRITICAL",tolua_Framework_S_LOG_CRITICAL00);

  { /* begin embedded lua code */
   int top = lua_gettop(tolua_S);
   static unsigned char B[] = {
    10,108,111,103, 32, 61, 10,123, 10,118,101,114, 98,111,115,
    101, 32, 61, 32, 83, 95, 76, 79, 71, 95, 86, 69, 82, 66, 79,
     83, 69, 59, 10,105,110,102,111, 32, 61, 32, 83, 95, 76, 79,
     71, 95, 73, 78, 70, 79, 59, 10,119, 97,114,110,105,110,103,
     32, 61, 32, 83, 95, 76, 79, 71, 95, 87, 65, 82, 78, 73, 78,
     71, 59, 10,102, 97,105,108,117,114,101, 32, 61, 32, 83, 95,
     76, 79, 71, 95, 70, 65, 73, 76, 85, 82, 69, 59, 10, 99,114,
    105,116,105, 99, 97,108, 32, 61, 32, 83, 95, 76, 79, 71, 95,
     67, 82, 73, 84, 73, 67, 65, 76, 10,125,32
   };
   tolua_dobuffer(tolua_S,(char*)B,sizeof(B),"tolua: embedded Lua code 1");
   lua_settop(tolua_S, top);
  } /* end of embedded lua code */

  tolua_module(tolua_S,"std",0);
  tolua_beginmodule(tolua_S,"std");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"stringstream","std::stringstream","std::iostream",tolua_collect_std__stringstream);
   #else
   tolua_cclass(tolua_S,"stringstream","std::stringstream","std::iostream",NULL);
   #endif
   tolua_beginmodule(tolua_S,"stringstream");
    tolua_function(tolua_S,"new",tolua_Framework_std_stringstream_new00);
    tolua_function(tolua_S,"new_local",tolua_Framework_std_stringstream_new00_local);
    tolua_function(tolua_S,".call",tolua_Framework_std_stringstream_new00_local);
    tolua_function(tolua_S,"new",tolua_Framework_std_stringstream_new01);
    tolua_function(tolua_S,"new_local",tolua_Framework_std_stringstream_new01_local);
    tolua_function(tolua_S,".call",tolua_Framework_std_stringstream_new01_local);
    tolua_function(tolua_S,"delete",tolua_Framework_std_stringstream_delete00);
    tolua_function(tolua_S,"str",tolua_Framework_std_stringstream_str00);
    tolua_variable(tolua_S,"__std__istream__",tolua_get_std__stringstream___std__istream__,NULL);
    tolua_variable(tolua_S,"__std__ostream__",tolua_get_std__stringstream___std__ostream__,NULL);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"Ember",0);
  tolua_beginmodule(tolua_S,"Ember");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"AttributeObserver","Ember::AttributeObserver","",tolua_collect_Ember__AttributeObserver);
   #else
   tolua_cclass(tolua_S,"AttributeObserver","Ember::AttributeObserver","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"AttributeObserver");
    tolua_function(tolua_S,"new",tolua_Framework_Ember_AttributeObserver_new00);
    tolua_function(tolua_S,"new_local",tolua_Framework_Ember_AttributeObserver_new00_local);
    tolua_function(tolua_S,".call",tolua_Framework_Ember_AttributeObserver_new00_local);
    tolua_function(tolua_S,"new",tolua_Framework_Ember_AttributeObserver_new01);
    tolua_function(tolua_S,"new_local",tolua_Framework_Ember_AttributeObserver_new01_local);
    tolua_function(tolua_S,".call",tolua_Framework_Ember_AttributeObserver_new01_local);
    tolua_function(tolua_S,"new",tolua_Framework_Ember_AttributeObserver_new02);
    tolua_function(tolua_S,"new_local",tolua_Framework_Ember_AttributeObserver_new02_local);
    tolua_function(tolua_S,".call",tolua_Framework_Ember_AttributeObserver_new02_local);
    tolua_function(tolua_S,"delete",tolua_Framework_Ember_AttributeObserver_delete00);
    tolua_variable(tolua_S,"EventChanged",tolua_get_Ember__AttributeObserver_EventChanged,tolua_set_Ember__AttributeObserver_EventChanged);
    tolua_function(tolua_S,"forceEvaluation",tolua_Framework_Ember_AttributeObserver_forceEvaluation00);
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

