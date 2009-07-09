/*
** Lua binding: Varconf
** Generated automatically by tolua++-1.0.92.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_Varconf_open (lua_State* tolua_S);

#include <varconf/variable.h>

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_varconf__Variable (lua_State* tolua_S)
{
 varconf::Variable* self = (varconf::Variable*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"varconf::Variable");
}

/* method: delete of class  varconf::Variable */
#ifndef TOLUA_DISABLE_tolua_Varconf_varconf_Variable_delete00
static int tolua_Varconf_varconf_Variable_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"varconf::Variable",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  varconf::Variable* self = (varconf::Variable*)  tolua_tousertype(tolua_S,1,0);
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

/* method: operatorbool of class  varconf::Variable */
#ifndef TOLUA_DISABLE_tolua_Varconf_varconf_Variable__bool00
static int tolua_Varconf_varconf_Variable__bool00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"varconf::Variable",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  varconf::Variable* self = (varconf::Variable*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operatorbool'",NULL);
#endif
  {
   bool tolua_ret = (bool)  static_cast< bool>(*self);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.bool'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operatorint of class  varconf::Variable */
#ifndef TOLUA_DISABLE_tolua_Varconf_varconf_Variable__int00
static int tolua_Varconf_varconf_Variable__int00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"varconf::Variable",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  varconf::Variable* self = (varconf::Variable*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operatorint'",NULL);
#endif
  {
   int tolua_ret = (int)  static_cast< int>(*self);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.int'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operatordouble of class  varconf::Variable */
#ifndef TOLUA_DISABLE_tolua_Varconf_varconf_Variable__double00
static int tolua_Varconf_varconf_Variable__double00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"varconf::Variable",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  varconf::Variable* self = (varconf::Variable*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operatordouble'",NULL);
#endif
  {
   double tolua_ret = (double)  static_cast< double>(*self);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.double'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: as_string of class  varconf::Variable */
#ifndef TOLUA_DISABLE_tolua_Varconf_varconf_Variable_as_string00
static int tolua_Varconf_varconf_Variable_as_string00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const varconf::Variable",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const varconf::Variable* self = (const varconf::Variable*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'as_string'",NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->as_string();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'as_string'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: is_bool of class  varconf::Variable */
#ifndef TOLUA_DISABLE_tolua_Varconf_varconf_Variable_is_bool00
static int tolua_Varconf_varconf_Variable_is_bool00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const varconf::Variable",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const varconf::Variable* self = (const varconf::Variable*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'is_bool'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->is_bool();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_bool'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: is_int of class  varconf::Variable */
#ifndef TOLUA_DISABLE_tolua_Varconf_varconf_Variable_is_int00
static int tolua_Varconf_varconf_Variable_is_int00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const varconf::Variable",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const varconf::Variable* self = (const varconf::Variable*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'is_int'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->is_int();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_int'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: is_double of class  varconf::Variable */
#ifndef TOLUA_DISABLE_tolua_Varconf_varconf_Variable_is_double00
static int tolua_Varconf_varconf_Variable_is_double00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const varconf::Variable",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const varconf::Variable* self = (const varconf::Variable*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'is_double'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->is_double();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_double'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: is_string of class  varconf::Variable */
#ifndef TOLUA_DISABLE_tolua_Varconf_varconf_Variable_is_string00
static int tolua_Varconf_varconf_Variable_is_string00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const varconf::Variable",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const varconf::Variable* self = (const varconf::Variable*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'is_string'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->is_string();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_string'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: is_array of class  varconf::Variable */
#ifndef TOLUA_DISABLE_tolua_Varconf_varconf_Variable_is_array00
static int tolua_Varconf_varconf_Variable_is_array00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const varconf::Variable",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const varconf::Variable* self = (const varconf::Variable*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'is_array'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->is_array();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_array'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_Varconf_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_module(tolua_S,"varconf",0);
  tolua_beginmodule(tolua_S,"varconf");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"Variable","varconf::Variable","",tolua_collect_varconf__Variable);
   #else
   tolua_cclass(tolua_S,"Variable","varconf::Variable","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"Variable");
    tolua_function(tolua_S,"delete",tolua_Varconf_varconf_Variable_delete00);
    tolua_function(tolua_S,".bool",tolua_Varconf_varconf_Variable__bool00);
    tolua_function(tolua_S,".int",tolua_Varconf_varconf_Variable__int00);
    tolua_function(tolua_S,".double",tolua_Varconf_varconf_Variable__double00);
    tolua_function(tolua_S,"as_string",tolua_Varconf_varconf_Variable_as_string00);
    tolua_function(tolua_S,"is_bool",tolua_Varconf_varconf_Variable_is_bool00);
    tolua_function(tolua_S,"is_int",tolua_Varconf_varconf_Variable_is_int00);
    tolua_function(tolua_S,"is_double",tolua_Varconf_varconf_Variable_is_double00);
    tolua_function(tolua_S,"is_string",tolua_Varconf_varconf_Variable_is_string00);
    tolua_function(tolua_S,"is_array",tolua_Varconf_varconf_Variable_is_array00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_Varconf (lua_State* tolua_S) {
 return tolua_Varconf_open(tolua_S);
};
#endif

