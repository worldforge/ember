/*
** Lua binding: atlas_adapters
** Generated automatically by tolua++-1.0.92 on Sat Jun 23 17:55:37 2007.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_atlas_adapters_open (lua_State* tolua_S);

#include "../../AdapterBase.h"
#include "../../AdapterFactory.h"
#include "../../StringAdapter.h"
#include "../../NumberAdapter.h"
#include "../../SizeAdapter.h"
#include "../../MapAdapter.h"
#include "../../ListAdapter.h"
#include "../../PositionAdapter.h"
#include "../../Position2DAdapter.h"
#include "../../OrientationAdapter.h"
#include "../../StaticAdapter.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_EmberOgre__Gui__Adapters__Atlas__AdapterBase (lua_State* tolua_S)
{
 EmberOgre::Gui::Adapters::Atlas::AdapterBase* self = (EmberOgre::Gui::Adapters::Atlas::AdapterBase*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Gui__Adapters__Atlas__SizeAdapter (lua_State* tolua_S)
{
 EmberOgre::Gui::Adapters::Atlas::SizeAdapter* self = (EmberOgre::Gui::Adapters::Atlas::SizeAdapter*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_std__vector_std__string_ (lua_State* tolua_S)
{
 std::vector<std::string>* self = (std::vector<std::string>*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Gui__Adapters__Atlas__StaticAdapter (lua_State* tolua_S)
{
 EmberOgre::Gui::Adapters::Atlas::StaticAdapter* self = (EmberOgre::Gui::Adapters::Atlas::StaticAdapter*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Gui__Adapters__Atlas__ListAdapter (lua_State* tolua_S)
{
 EmberOgre::Gui::Adapters::Atlas::ListAdapter* self = (EmberOgre::Gui::Adapters::Atlas::ListAdapter*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Gui__Adapters__Atlas__OrientationAdapter (lua_State* tolua_S)
{
 EmberOgre::Gui::Adapters::Atlas::OrientationAdapter* self = (EmberOgre::Gui::Adapters::Atlas::OrientationAdapter*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Gui__Adapters__Atlas__Position2DAdapter (lua_State* tolua_S)
{
 EmberOgre::Gui::Adapters::Atlas::Position2DAdapter* self = (EmberOgre::Gui::Adapters::Atlas::Position2DAdapter*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Gui__Adapters__Atlas__AdapterFactory (lua_State* tolua_S)
{
 EmberOgre::Gui::Adapters::Atlas::AdapterFactory* self = (EmberOgre::Gui::Adapters::Atlas::AdapterFactory*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Gui__Adapters__Atlas__PositionAdapter (lua_State* tolua_S)
{
 EmberOgre::Gui::Adapters::Atlas::PositionAdapter* self = (EmberOgre::Gui::Adapters::Atlas::PositionAdapter*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Gui__Adapters__Atlas__StringAdapter (lua_State* tolua_S)
{
 EmberOgre::Gui::Adapters::Atlas::StringAdapter* self = (EmberOgre::Gui::Adapters::Atlas::StringAdapter*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Gui__Adapters__Atlas__NumberAdapter (lua_State* tolua_S)
{
 EmberOgre::Gui::Adapters::Atlas::NumberAdapter* self = (EmberOgre::Gui::Adapters::Atlas::NumberAdapter*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_EmberOgre__Gui__Adapters__Atlas__MapAdapter (lua_State* tolua_S)
{
 EmberOgre::Gui::Adapters::Atlas::MapAdapter* self = (EmberOgre::Gui::Adapters::Atlas::MapAdapter*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"EmberOgre::Gui::Adapters::Atlas::AdapterBase");
 tolua_usertype(tolua_S,"::Atlas::Message::Element");
 tolua_usertype(tolua_S,"EmberOgre::Gui::Adapters::Atlas::SizeAdapter");
 tolua_usertype(tolua_S,"EmberOgre::Gui::Adapters::Atlas::ListAdapter");
 tolua_usertype(tolua_S,"std::map<std::string,Atlas::Message::Element>");
 tolua_usertype(tolua_S,"EmberOgre::Gui::Adapters::Atlas::StaticAdapter");
 tolua_usertype(tolua_S,"CEGUI::Window");
 tolua_usertype(tolua_S,"Eris::Entity");
 tolua_usertype(tolua_S,"EmberOgre::Gui::Adapters::Atlas::NumberAdapter");
 tolua_usertype(tolua_S,"CEGUI::Combobox");
 tolua_usertype(tolua_S,"std::vector<std::string>");
 tolua_usertype(tolua_S,"EmberOgre::Gui::Adapters::Atlas::PositionAdapter");
 tolua_usertype(tolua_S,"EmberOgre::Gui::Adapters::Atlas::OrientationAdapter");
 tolua_usertype(tolua_S,"EmberOgre::Gui::Adapters::Atlas::MapAdapter");
 tolua_usertype(tolua_S,"EmberOgre::Gui::Adapters::Atlas::Position2DAdapter");
 tolua_usertype(tolua_S,"EmberOgre::Gui::Adapters::Atlas::AdapterFactory");
 tolua_usertype(tolua_S,"sigc::signal<void>");
 tolua_usertype(tolua_S,"EmberOgre::Gui::Adapters::Atlas::StringAdapter");
 tolua_usertype(tolua_S,"Atlas::Message::Element");
 tolua_usertype(tolua_S,"CEGUI::Slider");
}

/* method: delete of class  EmberOgre::Gui::Adapters::Atlas::AdapterBase */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterBase_delete00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterBase_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::AdapterBase",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Adapters::Atlas::AdapterBase* self = (EmberOgre::Gui::Adapters::Atlas::AdapterBase*)  tolua_tousertype(tolua_S,1,0);
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

/* method: setValue of class  EmberOgre::Gui::Adapters::Atlas::AdapterBase */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterBase_setValue00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterBase_setValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::AdapterBase",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Atlas::Message::Element",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Adapters::Atlas::AdapterBase* self = (EmberOgre::Gui::Adapters::Atlas::AdapterBase*)  tolua_tousertype(tolua_S,1,0);
  Atlas::Message::Element* element = ((Atlas::Message::Element*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setValue'",NULL);
#endif
  {
   self->setValue(*element);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setValue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getValue of class  EmberOgre::Gui::Adapters::Atlas::AdapterBase */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterBase_getValue00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterBase_getValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::AdapterBase",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Adapters::Atlas::AdapterBase* self = (EmberOgre::Gui::Adapters::Atlas::AdapterBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getValue'",NULL);
#endif
  {
   Atlas::Message::Element& tolua_ret = (Atlas::Message::Element&)  self->getValue();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"Atlas::Message::Element");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getValue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getOriginalValue of class  EmberOgre::Gui::Adapters::Atlas::AdapterBase */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterBase_getOriginalValue00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterBase_getOriginalValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Gui::Adapters::Atlas::AdapterBase",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Gui::Adapters::Atlas::AdapterBase* self = (const EmberOgre::Gui::Adapters::Atlas::AdapterBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getOriginalValue'",NULL);
#endif
  {
   const Atlas::Message::Element& tolua_ret = (const Atlas::Message::Element&)  self->getOriginalValue();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Atlas::Message::Element");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getOriginalValue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EventValueChanged of class  EmberOgre::Gui::Adapters::Atlas::AdapterBase */
#ifndef TOLUA_DISABLE_tolua_get_EmberOgre__Gui__Adapters__Atlas__AdapterBase_EventValueChanged
static int tolua_get_EmberOgre__Gui__Adapters__Atlas__AdapterBase_EventValueChanged(lua_State* tolua_S)
{
  EmberOgre::Gui::Adapters::Atlas::AdapterBase* self = (EmberOgre::Gui::Adapters::Atlas::AdapterBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventValueChanged'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->EventValueChanged,"sigc::signal<void>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EventValueChanged of class  EmberOgre::Gui::Adapters::Atlas::AdapterBase */
#ifndef TOLUA_DISABLE_tolua_set_EmberOgre__Gui__Adapters__Atlas__AdapterBase_EventValueChanged
static int tolua_set_EmberOgre__Gui__Adapters__Atlas__AdapterBase_EventValueChanged(lua_State* tolua_S)
{
  EmberOgre::Gui::Adapters::Atlas::AdapterBase* self = (EmberOgre::Gui::Adapters::Atlas::AdapterBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EventValueChanged'",NULL);
  if (!tolua_isusertype(tolua_S,2,"sigc::signal<void>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EventValueChanged = *((sigc::signal<void>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: updateGui of class  EmberOgre::Gui::Adapters::Atlas::AdapterBase */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterBase_updateGui00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterBase_updateGui00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::AdapterBase",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Atlas::Message::Element",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Adapters::Atlas::AdapterBase* self = (EmberOgre::Gui::Adapters::Atlas::AdapterBase*)  tolua_tousertype(tolua_S,1,0);
  const Atlas::Message::Element* element = ((const Atlas::Message::Element*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'updateGui'",NULL);
#endif
  {
   self->updateGui(*element);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'updateGui'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: hasChanges of class  EmberOgre::Gui::Adapters::Atlas::AdapterBase */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterBase_hasChanges00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterBase_hasChanges00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::AdapterBase",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Adapters::Atlas::AdapterBase* self = (EmberOgre::Gui::Adapters::Atlas::AdapterBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hasChanges'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->hasChanges();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'hasChanges'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: remove of class  EmberOgre::Gui::Adapters::Atlas::AdapterBase */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterBase_remove00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterBase_remove00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::AdapterBase",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Adapters::Atlas::AdapterBase* self = (EmberOgre::Gui::Adapters::Atlas::AdapterBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'remove'",NULL);
#endif
  {
   self->remove();
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

/* method: isRemoved of class  EmberOgre::Gui::Adapters::Atlas::AdapterBase */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterBase_isRemoved00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterBase_isRemoved00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Gui::Adapters::Atlas::AdapterBase",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Gui::Adapters::Atlas::AdapterBase* self = (const EmberOgre::Gui::Adapters::Atlas::AdapterBase*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isRemoved'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isRemoved();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isRemoved'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addSuggestion of class  EmberOgre::Gui::Adapters::Atlas::AdapterBase */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterBase_addSuggestion00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterBase_addSuggestion00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::AdapterBase",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Adapters::Atlas::AdapterBase* self = (EmberOgre::Gui::Adapters::Atlas::AdapterBase*)  tolua_tousertype(tolua_S,1,0);
  const std::string suggestedValue = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addSuggestion'",NULL);
#endif
  {
   self->addSuggestion(suggestedValue);
   tolua_pushcppstring(tolua_S,(const char*)suggestedValue);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addSuggestion'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::Gui::Adapters::Atlas::AdapterFactory */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_new00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::AdapterFactory",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::string prefix = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  {
   EmberOgre::Gui::Adapters::Atlas::AdapterFactory* tolua_ret = (EmberOgre::Gui::Adapters::Atlas::AdapterFactory*)  new EmberOgre::Gui::Adapters::Atlas::AdapterFactory(prefix);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::Adapters::Atlas::AdapterFactory");
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

/* method: new_local of class  EmberOgre::Gui::Adapters::Atlas::AdapterFactory */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_new00_local
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::AdapterFactory",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::string prefix = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  {
   EmberOgre::Gui::Adapters::Atlas::AdapterFactory* tolua_ret = (EmberOgre::Gui::Adapters::Atlas::AdapterFactory*)  new EmberOgre::Gui::Adapters::Atlas::AdapterFactory(prefix);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Gui::Adapters::Atlas::AdapterFactory");
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

/* method: delete of class  EmberOgre::Gui::Adapters::Atlas::AdapterFactory */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_delete00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::AdapterFactory",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Adapters::Atlas::AdapterFactory* self = (EmberOgre::Gui::Adapters::Atlas::AdapterFactory*)  tolua_tousertype(tolua_S,1,0);
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

/* method: createStringAdapter of class  EmberOgre::Gui::Adapters::Atlas::AdapterFactory */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_createStringAdapter00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_createStringAdapter00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::AdapterFactory",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"const Atlas::Message::Element",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Adapters::Atlas::AdapterFactory* self = (EmberOgre::Gui::Adapters::Atlas::AdapterFactory*)  tolua_tousertype(tolua_S,1,0);
  CEGUI::Window* container = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
  const std::string adapterPrefix = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  const Atlas::Message::Element* element = ((const Atlas::Message::Element*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createStringAdapter'",NULL);
#endif
  {
   EmberOgre::Gui::Adapters::Atlas::StringAdapter* tolua_ret = (EmberOgre::Gui::Adapters::Atlas::StringAdapter*)  self->createStringAdapter(container,adapterPrefix,*element);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::Adapters::Atlas::StringAdapter");
   tolua_pushcppstring(tolua_S,(const char*)adapterPrefix);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createStringAdapter'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createNumberAdapter of class  EmberOgre::Gui::Adapters::Atlas::AdapterFactory */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_createNumberAdapter00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_createNumberAdapter00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::AdapterFactory",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"const Atlas::Message::Element",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Adapters::Atlas::AdapterFactory* self = (EmberOgre::Gui::Adapters::Atlas::AdapterFactory*)  tolua_tousertype(tolua_S,1,0);
  CEGUI::Window* container = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
  const std::string adapterPrefix = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  const Atlas::Message::Element* element = ((const Atlas::Message::Element*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createNumberAdapter'",NULL);
#endif
  {
   EmberOgre::Gui::Adapters::Atlas::NumberAdapter* tolua_ret = (EmberOgre::Gui::Adapters::Atlas::NumberAdapter*)  self->createNumberAdapter(container,adapterPrefix,*element);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::Adapters::Atlas::NumberAdapter");
   tolua_pushcppstring(tolua_S,(const char*)adapterPrefix);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createNumberAdapter'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createSizeAdapter of class  EmberOgre::Gui::Adapters::Atlas::AdapterFactory */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_createSizeAdapter00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_createSizeAdapter00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::AdapterFactory",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"const Atlas::Message::Element",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Adapters::Atlas::AdapterFactory* self = (EmberOgre::Gui::Adapters::Atlas::AdapterFactory*)  tolua_tousertype(tolua_S,1,0);
  CEGUI::Window* container = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
  const std::string adapterPrefix = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  const Atlas::Message::Element* element = ((const Atlas::Message::Element*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createSizeAdapter'",NULL);
#endif
  {
   EmberOgre::Gui::Adapters::Atlas::SizeAdapter* tolua_ret = (EmberOgre::Gui::Adapters::Atlas::SizeAdapter*)  self->createSizeAdapter(container,adapterPrefix,*element);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::Adapters::Atlas::SizeAdapter");
   tolua_pushcppstring(tolua_S,(const char*)adapterPrefix);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createSizeAdapter'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createPositionAdapter of class  EmberOgre::Gui::Adapters::Atlas::AdapterFactory */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_createPositionAdapter00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_createPositionAdapter00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::AdapterFactory",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"const Atlas::Message::Element",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Adapters::Atlas::AdapterFactory* self = (EmberOgre::Gui::Adapters::Atlas::AdapterFactory*)  tolua_tousertype(tolua_S,1,0);
  CEGUI::Window* container = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
  const std::string adapterPrefix = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  const Atlas::Message::Element* element = ((const Atlas::Message::Element*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createPositionAdapter'",NULL);
#endif
  {
   EmberOgre::Gui::Adapters::Atlas::PositionAdapter* tolua_ret = (EmberOgre::Gui::Adapters::Atlas::PositionAdapter*)  self->createPositionAdapter(container,adapterPrefix,*element);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::Adapters::Atlas::PositionAdapter");
   tolua_pushcppstring(tolua_S,(const char*)adapterPrefix);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createPositionAdapter'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createPosition2DAdapter of class  EmberOgre::Gui::Adapters::Atlas::AdapterFactory */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_createPosition2DAdapter00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_createPosition2DAdapter00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::AdapterFactory",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"const Atlas::Message::Element",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Adapters::Atlas::AdapterFactory* self = (EmberOgre::Gui::Adapters::Atlas::AdapterFactory*)  tolua_tousertype(tolua_S,1,0);
  CEGUI::Window* container = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
  const std::string adapterPrefix = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  const Atlas::Message::Element* element = ((const Atlas::Message::Element*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createPosition2DAdapter'",NULL);
#endif
  {
   EmberOgre::Gui::Adapters::Atlas::Position2DAdapter* tolua_ret = (EmberOgre::Gui::Adapters::Atlas::Position2DAdapter*)  self->createPosition2DAdapter(container,adapterPrefix,*element);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::Adapters::Atlas::Position2DAdapter");
   tolua_pushcppstring(tolua_S,(const char*)adapterPrefix);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createPosition2DAdapter'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createMapAdapter of class  EmberOgre::Gui::Adapters::Atlas::AdapterFactory */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_createMapAdapter00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_createMapAdapter00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::AdapterFactory",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"const Atlas::Message::Element",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Adapters::Atlas::AdapterFactory* self = (EmberOgre::Gui::Adapters::Atlas::AdapterFactory*)  tolua_tousertype(tolua_S,1,0);
  CEGUI::Window* container = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
  const std::string adapterPrefix = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  const Atlas::Message::Element* element = ((const Atlas::Message::Element*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createMapAdapter'",NULL);
#endif
  {
   EmberOgre::Gui::Adapters::Atlas::MapAdapter* tolua_ret = (EmberOgre::Gui::Adapters::Atlas::MapAdapter*)  self->createMapAdapter(container,adapterPrefix,*element);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::Adapters::Atlas::MapAdapter");
   tolua_pushcppstring(tolua_S,(const char*)adapterPrefix);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createMapAdapter'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createMapAdapter of class  EmberOgre::Gui::Adapters::Atlas::AdapterFactory */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_createMapAdapter01
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_createMapAdapter01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::AdapterFactory",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"const std::map<std::string,Atlas::Message::Element>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  EmberOgre::Gui::Adapters::Atlas::AdapterFactory* self = (EmberOgre::Gui::Adapters::Atlas::AdapterFactory*)  tolua_tousertype(tolua_S,1,0);
  CEGUI::Window* container = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
  const std::string adapterPrefix = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  const std::map<std::string,Atlas::Message::Element> attributes = *((const std::map<std::string,Atlas::Message::Element>*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createMapAdapter'",NULL);
#endif
  {
   EmberOgre::Gui::Adapters::Atlas::MapAdapter* tolua_ret = (EmberOgre::Gui::Adapters::Atlas::MapAdapter*)  self->createMapAdapter(container,adapterPrefix,attributes);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::Adapters::Atlas::MapAdapter");
   tolua_pushcppstring(tolua_S,(const char*)adapterPrefix);
  }
 }
 return 2;
tolua_lerror:
 return tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_createMapAdapter00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: createMapAdapter of class  EmberOgre::Gui::Adapters::Atlas::AdapterFactory */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_createMapAdapter02
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_createMapAdapter02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::AdapterFactory",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"Eris::Entity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  EmberOgre::Gui::Adapters::Atlas::AdapterFactory* self = (EmberOgre::Gui::Adapters::Atlas::AdapterFactory*)  tolua_tousertype(tolua_S,1,0);
  CEGUI::Window* container = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
  const std::string adapterPrefix = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  Eris::Entity* entity = ((Eris::Entity*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createMapAdapter'",NULL);
#endif
  {
   EmberOgre::Gui::Adapters::Atlas::MapAdapter* tolua_ret = (EmberOgre::Gui::Adapters::Atlas::MapAdapter*)  self->createMapAdapter(container,adapterPrefix,entity);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::Adapters::Atlas::MapAdapter");
   tolua_pushcppstring(tolua_S,(const char*)adapterPrefix);
  }
 }
 return 2;
tolua_lerror:
 return tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_createMapAdapter01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: createListAdapter of class  EmberOgre::Gui::Adapters::Atlas::AdapterFactory */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_createListAdapter00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_createListAdapter00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::AdapterFactory",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"const Atlas::Message::Element",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Adapters::Atlas::AdapterFactory* self = (EmberOgre::Gui::Adapters::Atlas::AdapterFactory*)  tolua_tousertype(tolua_S,1,0);
  CEGUI::Window* container = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
  const std::string adapterPrefix = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  const Atlas::Message::Element* element = ((const Atlas::Message::Element*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createListAdapter'",NULL);
#endif
  {
   EmberOgre::Gui::Adapters::Atlas::ListAdapter* tolua_ret = (EmberOgre::Gui::Adapters::Atlas::ListAdapter*)  self->createListAdapter(container,adapterPrefix,*element);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::Adapters::Atlas::ListAdapter");
   tolua_pushcppstring(tolua_S,(const char*)adapterPrefix);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createListAdapter'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createOrientationAdapter of class  EmberOgre::Gui::Adapters::Atlas::AdapterFactory */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_createOrientationAdapter00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_createOrientationAdapter00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::AdapterFactory",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"const Atlas::Message::Element",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Adapters::Atlas::AdapterFactory* self = (EmberOgre::Gui::Adapters::Atlas::AdapterFactory*)  tolua_tousertype(tolua_S,1,0);
  CEGUI::Window* container = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
  const std::string adapterPrefix = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  const Atlas::Message::Element* element = ((const Atlas::Message::Element*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createOrientationAdapter'",NULL);
#endif
  {
   EmberOgre::Gui::Adapters::Atlas::OrientationAdapter* tolua_ret = (EmberOgre::Gui::Adapters::Atlas::OrientationAdapter*)  self->createOrientationAdapter(container,adapterPrefix,*element);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::Adapters::Atlas::OrientationAdapter");
   tolua_pushcppstring(tolua_S,(const char*)adapterPrefix);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createOrientationAdapter'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createStaticAdapter of class  EmberOgre::Gui::Adapters::Atlas::AdapterFactory */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_createStaticAdapter00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_createStaticAdapter00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::AdapterFactory",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"const Atlas::Message::Element",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Adapters::Atlas::AdapterFactory* self = (EmberOgre::Gui::Adapters::Atlas::AdapterFactory*)  tolua_tousertype(tolua_S,1,0);
  CEGUI::Window* container = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
  const std::string adapterPrefix = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  const Atlas::Message::Element* element = ((const Atlas::Message::Element*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createStaticAdapter'",NULL);
#endif
  {
   EmberOgre::Gui::Adapters::Atlas::StaticAdapter* tolua_ret = (EmberOgre::Gui::Adapters::Atlas::StaticAdapter*)  self->createStaticAdapter(container,adapterPrefix,*element);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::Adapters::Atlas::StaticAdapter");
   tolua_pushcppstring(tolua_S,(const char*)adapterPrefix);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createStaticAdapter'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: loadLayoutIntoContainer of class  EmberOgre::Gui::Adapters::Atlas::AdapterFactory */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_loadLayoutIntoContainer00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_loadLayoutIntoContainer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::AdapterFactory",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CEGUI::Window",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Adapters::Atlas::AdapterFactory* self = (EmberOgre::Gui::Adapters::Atlas::AdapterFactory*)  tolua_tousertype(tolua_S,1,0);
  CEGUI::Window* container = ((CEGUI::Window*)  tolua_tousertype(tolua_S,2,0));
  const std::string adapterPrefix = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  const std::string layoutfile = ((const std::string)  tolua_tocppstring(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'loadLayoutIntoContainer'",NULL);
#endif
  {
   CEGUI::Window* tolua_ret = (CEGUI::Window*)  self->loadLayoutIntoContainer(container,adapterPrefix,layoutfile);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"CEGUI::Window");
   tolua_pushcppstring(tolua_S,(const char*)adapterPrefix);
   tolua_pushcppstring(tolua_S,(const char*)layoutfile);
  }
 }
 return 3;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'loadLayoutIntoContainer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getCurrentPrefix of class  EmberOgre::Gui::Adapters::Atlas::AdapterFactory */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_getCurrentPrefix00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_getCurrentPrefix00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Gui::Adapters::Atlas::AdapterFactory",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Gui::Adapters::Atlas::AdapterFactory* self = (const EmberOgre::Gui::Adapters::Atlas::AdapterFactory*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCurrentPrefix'",NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getCurrentPrefix();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getCurrentPrefix'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::Gui::Adapters::Atlas::StringAdapter */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_StringAdapter_new00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_StringAdapter_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::StringAdapter",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Atlas::Message::Element",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"CEGUI::Combobox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Atlas::Message::Element* element = ((const Atlas::Message::Element*)  tolua_tousertype(tolua_S,2,0));
  CEGUI::Combobox* textWindow = ((CEGUI::Combobox*)  tolua_tousertype(tolua_S,3,0));
  {
   EmberOgre::Gui::Adapters::Atlas::StringAdapter* tolua_ret = (EmberOgre::Gui::Adapters::Atlas::StringAdapter*)  new EmberOgre::Gui::Adapters::Atlas::StringAdapter(*element,textWindow);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::Adapters::Atlas::StringAdapter");
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

/* method: new_local of class  EmberOgre::Gui::Adapters::Atlas::StringAdapter */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_StringAdapter_new00_local
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_StringAdapter_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::StringAdapter",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Atlas::Message::Element",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"CEGUI::Combobox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Atlas::Message::Element* element = ((const Atlas::Message::Element*)  tolua_tousertype(tolua_S,2,0));
  CEGUI::Combobox* textWindow = ((CEGUI::Combobox*)  tolua_tousertype(tolua_S,3,0));
  {
   EmberOgre::Gui::Adapters::Atlas::StringAdapter* tolua_ret = (EmberOgre::Gui::Adapters::Atlas::StringAdapter*)  new EmberOgre::Gui::Adapters::Atlas::StringAdapter(*element,textWindow);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Gui::Adapters::Atlas::StringAdapter");
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

/* method: delete of class  EmberOgre::Gui::Adapters::Atlas::StringAdapter */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_StringAdapter_delete00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_StringAdapter_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::StringAdapter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Adapters::Atlas::StringAdapter* self = (EmberOgre::Gui::Adapters::Atlas::StringAdapter*)  tolua_tousertype(tolua_S,1,0);
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

/* method: new of class  EmberOgre::Gui::Adapters::Atlas::NumberAdapter */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_NumberAdapter_new00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_NumberAdapter_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::NumberAdapter",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Atlas::Message::Element",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"CEGUI::Combobox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Atlas::Message::Element* element = ((const Atlas::Message::Element*)  tolua_tousertype(tolua_S,2,0));
  CEGUI::Combobox* textWindow = ((CEGUI::Combobox*)  tolua_tousertype(tolua_S,3,0));
  {
   EmberOgre::Gui::Adapters::Atlas::NumberAdapter* tolua_ret = (EmberOgre::Gui::Adapters::Atlas::NumberAdapter*)  new EmberOgre::Gui::Adapters::Atlas::NumberAdapter(*element,textWindow);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::Adapters::Atlas::NumberAdapter");
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

/* method: new_local of class  EmberOgre::Gui::Adapters::Atlas::NumberAdapter */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_NumberAdapter_new00_local
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_NumberAdapter_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::NumberAdapter",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Atlas::Message::Element",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"CEGUI::Combobox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Atlas::Message::Element* element = ((const Atlas::Message::Element*)  tolua_tousertype(tolua_S,2,0));
  CEGUI::Combobox* textWindow = ((CEGUI::Combobox*)  tolua_tousertype(tolua_S,3,0));
  {
   EmberOgre::Gui::Adapters::Atlas::NumberAdapter* tolua_ret = (EmberOgre::Gui::Adapters::Atlas::NumberAdapter*)  new EmberOgre::Gui::Adapters::Atlas::NumberAdapter(*element,textWindow);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Gui::Adapters::Atlas::NumberAdapter");
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

/* method: delete of class  EmberOgre::Gui::Adapters::Atlas::NumberAdapter */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_NumberAdapter_delete00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_NumberAdapter_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::NumberAdapter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Adapters::Atlas::NumberAdapter* self = (EmberOgre::Gui::Adapters::Atlas::NumberAdapter*)  tolua_tousertype(tolua_S,1,0);
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

/* method: new of class  EmberOgre::Gui::Adapters::Atlas::SizeAdapter */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_SizeAdapter_new00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_SizeAdapter_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::SizeAdapter",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Atlas::Message::Element",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,6,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,7,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,8,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,9,"CEGUI::Slider",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,10,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,11,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Atlas::Message::Element* element = ((const Atlas::Message::Element*)  tolua_tousertype(tolua_S,2,0));
  CEGUI::Window* lowerXWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,3,0));
  CEGUI::Window* lowerYWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,4,0));
  CEGUI::Window* lowerZWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,5,0));
  CEGUI::Window* upperXWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,6,0));
  CEGUI::Window* upperYWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,7,0));
  CEGUI::Window* upperZWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,8,0));
  CEGUI::Slider* scaler = ((CEGUI::Slider*)  tolua_tousertype(tolua_S,9,0));
  CEGUI::Window* infoWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,10,0));
  {
   EmberOgre::Gui::Adapters::Atlas::SizeAdapter* tolua_ret = (EmberOgre::Gui::Adapters::Atlas::SizeAdapter*)  new EmberOgre::Gui::Adapters::Atlas::SizeAdapter(*element,lowerXWindow,lowerYWindow,lowerZWindow,upperXWindow,upperYWindow,upperZWindow,scaler,infoWindow);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::Adapters::Atlas::SizeAdapter");
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

/* method: new_local of class  EmberOgre::Gui::Adapters::Atlas::SizeAdapter */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_SizeAdapter_new00_local
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_SizeAdapter_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::SizeAdapter",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Atlas::Message::Element",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,6,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,7,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,8,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,9,"CEGUI::Slider",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,10,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,11,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Atlas::Message::Element* element = ((const Atlas::Message::Element*)  tolua_tousertype(tolua_S,2,0));
  CEGUI::Window* lowerXWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,3,0));
  CEGUI::Window* lowerYWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,4,0));
  CEGUI::Window* lowerZWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,5,0));
  CEGUI::Window* upperXWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,6,0));
  CEGUI::Window* upperYWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,7,0));
  CEGUI::Window* upperZWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,8,0));
  CEGUI::Slider* scaler = ((CEGUI::Slider*)  tolua_tousertype(tolua_S,9,0));
  CEGUI::Window* infoWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,10,0));
  {
   EmberOgre::Gui::Adapters::Atlas::SizeAdapter* tolua_ret = (EmberOgre::Gui::Adapters::Atlas::SizeAdapter*)  new EmberOgre::Gui::Adapters::Atlas::SizeAdapter(*element,lowerXWindow,lowerYWindow,lowerZWindow,upperXWindow,upperYWindow,upperZWindow,scaler,infoWindow);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Gui::Adapters::Atlas::SizeAdapter");
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

/* method: delete of class  EmberOgre::Gui::Adapters::Atlas::SizeAdapter */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_SizeAdapter_delete00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_SizeAdapter_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::SizeAdapter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Adapters::Atlas::SizeAdapter* self = (EmberOgre::Gui::Adapters::Atlas::SizeAdapter*)  tolua_tousertype(tolua_S,1,0);
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

/* method: new of class  EmberOgre::Gui::Adapters::Atlas::MapAdapter */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_MapAdapter_new00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_MapAdapter_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::MapAdapter",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const ::Atlas::Message::Element",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const ::Atlas::Message::Element* element = ((const ::Atlas::Message::Element*)  tolua_tousertype(tolua_S,2,0));
  CEGUI::Window* childContainer = ((CEGUI::Window*)  tolua_tousertype(tolua_S,3,0));
  {
   EmberOgre::Gui::Adapters::Atlas::MapAdapter* tolua_ret = (EmberOgre::Gui::Adapters::Atlas::MapAdapter*)  new EmberOgre::Gui::Adapters::Atlas::MapAdapter(*element,childContainer);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::Adapters::Atlas::MapAdapter");
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

/* method: new_local of class  EmberOgre::Gui::Adapters::Atlas::MapAdapter */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_MapAdapter_new00_local
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_MapAdapter_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::MapAdapter",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const ::Atlas::Message::Element",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const ::Atlas::Message::Element* element = ((const ::Atlas::Message::Element*)  tolua_tousertype(tolua_S,2,0));
  CEGUI::Window* childContainer = ((CEGUI::Window*)  tolua_tousertype(tolua_S,3,0));
  {
   EmberOgre::Gui::Adapters::Atlas::MapAdapter* tolua_ret = (EmberOgre::Gui::Adapters::Atlas::MapAdapter*)  new EmberOgre::Gui::Adapters::Atlas::MapAdapter(*element,childContainer);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Gui::Adapters::Atlas::MapAdapter");
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

/* method: delete of class  EmberOgre::Gui::Adapters::Atlas::MapAdapter */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_MapAdapter_delete00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_MapAdapter_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::MapAdapter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Adapters::Atlas::MapAdapter* self = (EmberOgre::Gui::Adapters::Atlas::MapAdapter*)  tolua_tousertype(tolua_S,1,0);
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

/* method: getAttributeNames of class  EmberOgre::Gui::Adapters::Atlas::MapAdapter */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_MapAdapter_getAttributeNames00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_MapAdapter_getAttributeNames00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::MapAdapter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Adapters::Atlas::MapAdapter* self = (EmberOgre::Gui::Adapters::Atlas::MapAdapter*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAttributeNames'",NULL);
#endif
  {
   std::vector<std::string> tolua_ret = (std::vector<std::string>)  self->getAttributeNames();
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
 tolua_error(tolua_S,"#ferror in function 'getAttributeNames'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: valueOfAttr of class  EmberOgre::Gui::Adapters::Atlas::MapAdapter */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_MapAdapter_valueOfAttr00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_MapAdapter_valueOfAttr00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Gui::Adapters::Atlas::MapAdapter",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Gui::Adapters::Atlas::MapAdapter* self = (const EmberOgre::Gui::Adapters::Atlas::MapAdapter*)  tolua_tousertype(tolua_S,1,0);
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

/* method: hasAttr of class  EmberOgre::Gui::Adapters::Atlas::MapAdapter */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_MapAdapter_hasAttr00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_MapAdapter_hasAttr00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Gui::Adapters::Atlas::MapAdapter",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Gui::Adapters::Atlas::MapAdapter* self = (const EmberOgre::Gui::Adapters::Atlas::MapAdapter*)  tolua_tousertype(tolua_S,1,0);
  const std::string attr = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hasAttr'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->hasAttr(attr);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)attr);
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

/* method: addAttributeAdapter of class  EmberOgre::Gui::Adapters::Atlas::MapAdapter */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_MapAdapter_addAttributeAdapter00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_MapAdapter_addAttributeAdapter00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::MapAdapter",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"EmberOgre::Gui::Adapters::Atlas::AdapterBase",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Adapters::Atlas::MapAdapter* self = (EmberOgre::Gui::Adapters::Atlas::MapAdapter*)  tolua_tousertype(tolua_S,1,0);
  const std::string attributeName = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  EmberOgre::Gui::Adapters::Atlas::AdapterBase* adapter = ((EmberOgre::Gui::Adapters::Atlas::AdapterBase*)  tolua_tousertype(tolua_S,3,0));
  CEGUI::Window* containerWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addAttributeAdapter'",NULL);
#endif
  {
   self->addAttributeAdapter(attributeName,adapter,containerWindow);
   tolua_pushcppstring(tolua_S,(const char*)attributeName);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addAttributeAdapter'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeAdapters of class  EmberOgre::Gui::Adapters::Atlas::MapAdapter */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_MapAdapter_removeAdapters00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_MapAdapter_removeAdapters00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::MapAdapter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Adapters::Atlas::MapAdapter* self = (EmberOgre::Gui::Adapters::Atlas::MapAdapter*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeAdapters'",NULL);
#endif
  {
   self->removeAdapters();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeAdapters'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::Gui::Adapters::Atlas::ListAdapter */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_ListAdapter_new00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_ListAdapter_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::ListAdapter",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Atlas::Message::Element",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Atlas::Message::Element* element = ((const Atlas::Message::Element*)  tolua_tousertype(tolua_S,2,0));
  CEGUI::Window* childContainer = ((CEGUI::Window*)  tolua_tousertype(tolua_S,3,0));
  {
   EmberOgre::Gui::Adapters::Atlas::ListAdapter* tolua_ret = (EmberOgre::Gui::Adapters::Atlas::ListAdapter*)  new EmberOgre::Gui::Adapters::Atlas::ListAdapter(*element,childContainer);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::Adapters::Atlas::ListAdapter");
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

/* method: new_local of class  EmberOgre::Gui::Adapters::Atlas::ListAdapter */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_ListAdapter_new00_local
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_ListAdapter_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::ListAdapter",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Atlas::Message::Element",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Atlas::Message::Element* element = ((const Atlas::Message::Element*)  tolua_tousertype(tolua_S,2,0));
  CEGUI::Window* childContainer = ((CEGUI::Window*)  tolua_tousertype(tolua_S,3,0));
  {
   EmberOgre::Gui::Adapters::Atlas::ListAdapter* tolua_ret = (EmberOgre::Gui::Adapters::Atlas::ListAdapter*)  new EmberOgre::Gui::Adapters::Atlas::ListAdapter(*element,childContainer);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Gui::Adapters::Atlas::ListAdapter");
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

/* method: delete of class  EmberOgre::Gui::Adapters::Atlas::ListAdapter */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_ListAdapter_delete00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_ListAdapter_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::ListAdapter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Adapters::Atlas::ListAdapter* self = (EmberOgre::Gui::Adapters::Atlas::ListAdapter*)  tolua_tousertype(tolua_S,1,0);
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

/* method: getSize of class  EmberOgre::Gui::Adapters::Atlas::ListAdapter */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_ListAdapter_getSize00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_ListAdapter_getSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::ListAdapter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Adapters::Atlas::ListAdapter* self = (EmberOgre::Gui::Adapters::Atlas::ListAdapter*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSize'",NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->getSize();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
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

/* method: valueOfAttr of class  EmberOgre::Gui::Adapters::Atlas::ListAdapter */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_ListAdapter_valueOfAttr00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_ListAdapter_valueOfAttr00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const EmberOgre::Gui::Adapters::Atlas::ListAdapter",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const EmberOgre::Gui::Adapters::Atlas::ListAdapter* self = (const EmberOgre::Gui::Adapters::Atlas::ListAdapter*)  tolua_tousertype(tolua_S,1,0);
  unsigned int index = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'valueOfAttr'",NULL);
#endif
  {
   const Atlas::Message::Element& tolua_ret = (const Atlas::Message::Element&)  self->valueOfAttr(index);
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Atlas::Message::Element");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'valueOfAttr'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addAttributeAdapter of class  EmberOgre::Gui::Adapters::Atlas::ListAdapter */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_ListAdapter_addAttributeAdapter00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_ListAdapter_addAttributeAdapter00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::ListAdapter",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"EmberOgre::Gui::Adapters::Atlas::AdapterBase",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Adapters::Atlas::ListAdapter* self = (EmberOgre::Gui::Adapters::Atlas::ListAdapter*)  tolua_tousertype(tolua_S,1,0);
  EmberOgre::Gui::Adapters::Atlas::AdapterBase* adapter = ((EmberOgre::Gui::Adapters::Atlas::AdapterBase*)  tolua_tousertype(tolua_S,2,0));
  CEGUI::Window* containerWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addAttributeAdapter'",NULL);
#endif
  {
   self->addAttributeAdapter(adapter,containerWindow);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addAttributeAdapter'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeAdapters of class  EmberOgre::Gui::Adapters::Atlas::ListAdapter */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_ListAdapter_removeAdapters00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_ListAdapter_removeAdapters00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::ListAdapter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Adapters::Atlas::ListAdapter* self = (EmberOgre::Gui::Adapters::Atlas::ListAdapter*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeAdapters'",NULL);
#endif
  {
   self->removeAdapters();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeAdapters'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  EmberOgre::Gui::Adapters::Atlas::PositionAdapter */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_PositionAdapter_new00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_PositionAdapter_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::PositionAdapter",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const ::Atlas::Message::Element",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const ::Atlas::Message::Element* element = ((const ::Atlas::Message::Element*)  tolua_tousertype(tolua_S,2,0));
  CEGUI::Window* xWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,3,0));
  CEGUI::Window* yWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,4,0));
  CEGUI::Window* zWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,5,0));
  {
   EmberOgre::Gui::Adapters::Atlas::PositionAdapter* tolua_ret = (EmberOgre::Gui::Adapters::Atlas::PositionAdapter*)  new EmberOgre::Gui::Adapters::Atlas::PositionAdapter(*element,xWindow,yWindow,zWindow);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::Adapters::Atlas::PositionAdapter");
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

/* method: new_local of class  EmberOgre::Gui::Adapters::Atlas::PositionAdapter */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_PositionAdapter_new00_local
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_PositionAdapter_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::PositionAdapter",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const ::Atlas::Message::Element",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const ::Atlas::Message::Element* element = ((const ::Atlas::Message::Element*)  tolua_tousertype(tolua_S,2,0));
  CEGUI::Window* xWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,3,0));
  CEGUI::Window* yWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,4,0));
  CEGUI::Window* zWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,5,0));
  {
   EmberOgre::Gui::Adapters::Atlas::PositionAdapter* tolua_ret = (EmberOgre::Gui::Adapters::Atlas::PositionAdapter*)  new EmberOgre::Gui::Adapters::Atlas::PositionAdapter(*element,xWindow,yWindow,zWindow);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Gui::Adapters::Atlas::PositionAdapter");
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

/* method: delete of class  EmberOgre::Gui::Adapters::Atlas::PositionAdapter */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_PositionAdapter_delete00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_PositionAdapter_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::PositionAdapter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Adapters::Atlas::PositionAdapter* self = (EmberOgre::Gui::Adapters::Atlas::PositionAdapter*)  tolua_tousertype(tolua_S,1,0);
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

/* method: new of class  EmberOgre::Gui::Adapters::Atlas::Position2DAdapter */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_Position2DAdapter_new00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_Position2DAdapter_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::Position2DAdapter",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const ::Atlas::Message::Element",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const ::Atlas::Message::Element* element = ((const ::Atlas::Message::Element*)  tolua_tousertype(tolua_S,2,0));
  CEGUI::Window* xWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,3,0));
  CEGUI::Window* yWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,4,0));
  {
   EmberOgre::Gui::Adapters::Atlas::Position2DAdapter* tolua_ret = (EmberOgre::Gui::Adapters::Atlas::Position2DAdapter*)  new EmberOgre::Gui::Adapters::Atlas::Position2DAdapter(*element,xWindow,yWindow);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::Adapters::Atlas::Position2DAdapter");
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

/* method: new_local of class  EmberOgre::Gui::Adapters::Atlas::Position2DAdapter */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_Position2DAdapter_new00_local
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_Position2DAdapter_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::Position2DAdapter",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const ::Atlas::Message::Element",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const ::Atlas::Message::Element* element = ((const ::Atlas::Message::Element*)  tolua_tousertype(tolua_S,2,0));
  CEGUI::Window* xWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,3,0));
  CEGUI::Window* yWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,4,0));
  {
   EmberOgre::Gui::Adapters::Atlas::Position2DAdapter* tolua_ret = (EmberOgre::Gui::Adapters::Atlas::Position2DAdapter*)  new EmberOgre::Gui::Adapters::Atlas::Position2DAdapter(*element,xWindow,yWindow);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Gui::Adapters::Atlas::Position2DAdapter");
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

/* method: delete of class  EmberOgre::Gui::Adapters::Atlas::Position2DAdapter */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_Position2DAdapter_delete00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_Position2DAdapter_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::Position2DAdapter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Adapters::Atlas::Position2DAdapter* self = (EmberOgre::Gui::Adapters::Atlas::Position2DAdapter*)  tolua_tousertype(tolua_S,1,0);
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

/* method: new of class  EmberOgre::Gui::Adapters::Atlas::OrientationAdapter */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_OrientationAdapter_new00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_OrientationAdapter_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::OrientationAdapter",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Atlas::Message::Element",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,6,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Atlas::Message::Element* element = ((const Atlas::Message::Element*)  tolua_tousertype(tolua_S,2,0));
  CEGUI::Window* xWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,3,0));
  CEGUI::Window* yWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,4,0));
  CEGUI::Window* zWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,5,0));
  CEGUI::Window* scalarWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,6,0));
  {
   EmberOgre::Gui::Adapters::Atlas::OrientationAdapter* tolua_ret = (EmberOgre::Gui::Adapters::Atlas::OrientationAdapter*)  new EmberOgre::Gui::Adapters::Atlas::OrientationAdapter(*element,xWindow,yWindow,zWindow,scalarWindow);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::Adapters::Atlas::OrientationAdapter");
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

/* method: new_local of class  EmberOgre::Gui::Adapters::Atlas::OrientationAdapter */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_OrientationAdapter_new00_local
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_OrientationAdapter_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::OrientationAdapter",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Atlas::Message::Element",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,6,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Atlas::Message::Element* element = ((const Atlas::Message::Element*)  tolua_tousertype(tolua_S,2,0));
  CEGUI::Window* xWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,3,0));
  CEGUI::Window* yWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,4,0));
  CEGUI::Window* zWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,5,0));
  CEGUI::Window* scalarWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,6,0));
  {
   EmberOgre::Gui::Adapters::Atlas::OrientationAdapter* tolua_ret = (EmberOgre::Gui::Adapters::Atlas::OrientationAdapter*)  new EmberOgre::Gui::Adapters::Atlas::OrientationAdapter(*element,xWindow,yWindow,zWindow,scalarWindow);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Gui::Adapters::Atlas::OrientationAdapter");
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

/* method: delete of class  EmberOgre::Gui::Adapters::Atlas::OrientationAdapter */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_OrientationAdapter_delete00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_OrientationAdapter_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::OrientationAdapter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Adapters::Atlas::OrientationAdapter* self = (EmberOgre::Gui::Adapters::Atlas::OrientationAdapter*)  tolua_tousertype(tolua_S,1,0);
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

/* method: new of class  EmberOgre::Gui::Adapters::Atlas::StaticAdapter */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_StaticAdapter_new00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_StaticAdapter_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::StaticAdapter",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const ::Atlas::Message::Element",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const ::Atlas::Message::Element* element = ((const ::Atlas::Message::Element*)  tolua_tousertype(tolua_S,2,0));
  CEGUI::Window* textWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,3,0));
  {
   EmberOgre::Gui::Adapters::Atlas::StaticAdapter* tolua_ret = (EmberOgre::Gui::Adapters::Atlas::StaticAdapter*)  new EmberOgre::Gui::Adapters::Atlas::StaticAdapter(*element,textWindow);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"EmberOgre::Gui::Adapters::Atlas::StaticAdapter");
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

/* method: new_local of class  EmberOgre::Gui::Adapters::Atlas::StaticAdapter */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_StaticAdapter_new00_local
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_StaticAdapter_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::StaticAdapter",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const ::Atlas::Message::Element",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"CEGUI::Window",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const ::Atlas::Message::Element* element = ((const ::Atlas::Message::Element*)  tolua_tousertype(tolua_S,2,0));
  CEGUI::Window* textWindow = ((CEGUI::Window*)  tolua_tousertype(tolua_S,3,0));
  {
   EmberOgre::Gui::Adapters::Atlas::StaticAdapter* tolua_ret = (EmberOgre::Gui::Adapters::Atlas::StaticAdapter*)  new EmberOgre::Gui::Adapters::Atlas::StaticAdapter(*element,textWindow);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"EmberOgre::Gui::Adapters::Atlas::StaticAdapter");
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

/* method: delete of class  EmberOgre::Gui::Adapters::Atlas::StaticAdapter */
#ifndef TOLUA_DISABLE_tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_StaticAdapter_delete00
static int tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_StaticAdapter_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"EmberOgre::Gui::Adapters::Atlas::StaticAdapter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  EmberOgre::Gui::Adapters::Atlas::StaticAdapter* self = (EmberOgre::Gui::Adapters::Atlas::StaticAdapter*)  tolua_tousertype(tolua_S,1,0);
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

/* Open function */
TOLUA_API int tolua_atlas_adapters_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_module(tolua_S,"Gui",0);
   tolua_beginmodule(tolua_S,"Gui");
    tolua_module(tolua_S,"Adapters",0);
    tolua_beginmodule(tolua_S,"Adapters");
     tolua_module(tolua_S,"Atlas",0);
     tolua_beginmodule(tolua_S,"Atlas");
      #ifdef __cplusplus
      tolua_cclass(tolua_S,"AdapterBase","EmberOgre::Gui::Adapters::Atlas::AdapterBase","",tolua_collect_EmberOgre__Gui__Adapters__Atlas__AdapterBase);
      #else
      tolua_cclass(tolua_S,"AdapterBase","EmberOgre::Gui::Adapters::Atlas::AdapterBase","",NULL);
      #endif
      tolua_beginmodule(tolua_S,"AdapterBase");
       tolua_function(tolua_S,"delete",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterBase_delete00);
       tolua_function(tolua_S,"setValue",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterBase_setValue00);
       tolua_function(tolua_S,"getValue",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterBase_getValue00);
       tolua_function(tolua_S,"getOriginalValue",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterBase_getOriginalValue00);
       tolua_variable(tolua_S,"EventValueChanged",tolua_get_EmberOgre__Gui__Adapters__Atlas__AdapterBase_EventValueChanged,tolua_set_EmberOgre__Gui__Adapters__Atlas__AdapterBase_EventValueChanged);
       tolua_function(tolua_S,"updateGui",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterBase_updateGui00);
       tolua_function(tolua_S,"hasChanges",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterBase_hasChanges00);
       tolua_function(tolua_S,"remove",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterBase_remove00);
       tolua_function(tolua_S,"isRemoved",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterBase_isRemoved00);
       tolua_function(tolua_S,"addSuggestion",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterBase_addSuggestion00);
      tolua_endmodule(tolua_S);
     tolua_endmodule(tolua_S);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_module(tolua_S,"Gui",0);
   tolua_beginmodule(tolua_S,"Gui");
    tolua_module(tolua_S,"Adapters",0);
    tolua_beginmodule(tolua_S,"Adapters");
     tolua_module(tolua_S,"Atlas",0);
     tolua_beginmodule(tolua_S,"Atlas");
      #ifdef __cplusplus
      tolua_cclass(tolua_S,"AdapterFactory","EmberOgre::Gui::Adapters::Atlas::AdapterFactory","",tolua_collect_EmberOgre__Gui__Adapters__Atlas__AdapterFactory);
      #else
      tolua_cclass(tolua_S,"AdapterFactory","EmberOgre::Gui::Adapters::Atlas::AdapterFactory","",NULL);
      #endif
      tolua_beginmodule(tolua_S,"AdapterFactory");
       tolua_function(tolua_S,"new",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_new00);
       tolua_function(tolua_S,"new_local",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_new00_local);
       tolua_function(tolua_S,".call",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_new00_local);
       tolua_function(tolua_S,"delete",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_delete00);
       tolua_function(tolua_S,"createStringAdapter",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_createStringAdapter00);
       tolua_function(tolua_S,"createNumberAdapter",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_createNumberAdapter00);
       tolua_function(tolua_S,"createSizeAdapter",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_createSizeAdapter00);
       tolua_function(tolua_S,"createPositionAdapter",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_createPositionAdapter00);
       tolua_function(tolua_S,"createPosition2DAdapter",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_createPosition2DAdapter00);
       tolua_function(tolua_S,"createMapAdapter",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_createMapAdapter00);
       tolua_function(tolua_S,"createMapAdapter",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_createMapAdapter01);
       tolua_function(tolua_S,"createMapAdapter",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_createMapAdapter02);
       tolua_function(tolua_S,"createListAdapter",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_createListAdapter00);
       tolua_function(tolua_S,"createOrientationAdapter",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_createOrientationAdapter00);
       tolua_function(tolua_S,"createStaticAdapter",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_createStaticAdapter00);
       tolua_function(tolua_S,"loadLayoutIntoContainer",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_loadLayoutIntoContainer00);
       tolua_function(tolua_S,"getCurrentPrefix",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_AdapterFactory_getCurrentPrefix00);
      tolua_endmodule(tolua_S);
     tolua_endmodule(tolua_S);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_module(tolua_S,"Gui",0);
   tolua_beginmodule(tolua_S,"Gui");
    tolua_module(tolua_S,"Adapters",0);
    tolua_beginmodule(tolua_S,"Adapters");
     tolua_module(tolua_S,"Atlas",0);
     tolua_beginmodule(tolua_S,"Atlas");
      #ifdef __cplusplus
      tolua_cclass(tolua_S,"StringAdapter","EmberOgre::Gui::Adapters::Atlas::StringAdapter","EmberOgre::Gui::Adapters::Atlas::AdapterBase",tolua_collect_EmberOgre__Gui__Adapters__Atlas__StringAdapter);
      #else
      tolua_cclass(tolua_S,"StringAdapter","EmberOgre::Gui::Adapters::Atlas::StringAdapter","EmberOgre::Gui::Adapters::Atlas::AdapterBase",NULL);
      #endif
      tolua_beginmodule(tolua_S,"StringAdapter");
       tolua_function(tolua_S,"new",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_StringAdapter_new00);
       tolua_function(tolua_S,"new_local",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_StringAdapter_new00_local);
       tolua_function(tolua_S,".call",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_StringAdapter_new00_local);
       tolua_function(tolua_S,"delete",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_StringAdapter_delete00);
      tolua_endmodule(tolua_S);
     tolua_endmodule(tolua_S);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_module(tolua_S,"Gui",0);
   tolua_beginmodule(tolua_S,"Gui");
    tolua_module(tolua_S,"Adapters",0);
    tolua_beginmodule(tolua_S,"Adapters");
     tolua_module(tolua_S,"Atlas",0);
     tolua_beginmodule(tolua_S,"Atlas");
      #ifdef __cplusplus
      tolua_cclass(tolua_S,"NumberAdapter","EmberOgre::Gui::Adapters::Atlas::NumberAdapter","EmberOgre::Gui::Adapters::Atlas::AdapterBase",tolua_collect_EmberOgre__Gui__Adapters__Atlas__NumberAdapter);
      #else
      tolua_cclass(tolua_S,"NumberAdapter","EmberOgre::Gui::Adapters::Atlas::NumberAdapter","EmberOgre::Gui::Adapters::Atlas::AdapterBase",NULL);
      #endif
      tolua_beginmodule(tolua_S,"NumberAdapter");
       tolua_function(tolua_S,"new",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_NumberAdapter_new00);
       tolua_function(tolua_S,"new_local",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_NumberAdapter_new00_local);
       tolua_function(tolua_S,".call",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_NumberAdapter_new00_local);
       tolua_function(tolua_S,"delete",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_NumberAdapter_delete00);
      tolua_endmodule(tolua_S);
     tolua_endmodule(tolua_S);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_module(tolua_S,"Gui",0);
   tolua_beginmodule(tolua_S,"Gui");
    tolua_module(tolua_S,"Adapters",0);
    tolua_beginmodule(tolua_S,"Adapters");
     tolua_module(tolua_S,"Atlas",0);
     tolua_beginmodule(tolua_S,"Atlas");
      #ifdef __cplusplus
      tolua_cclass(tolua_S,"SizeAdapter","EmberOgre::Gui::Adapters::Atlas::SizeAdapter","EmberOgre::Gui::Adapters::Atlas::AdapterBase",tolua_collect_EmberOgre__Gui__Adapters__Atlas__SizeAdapter);
      #else
      tolua_cclass(tolua_S,"SizeAdapter","EmberOgre::Gui::Adapters::Atlas::SizeAdapter","EmberOgre::Gui::Adapters::Atlas::AdapterBase",NULL);
      #endif
      tolua_beginmodule(tolua_S,"SizeAdapter");
       tolua_function(tolua_S,"new",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_SizeAdapter_new00);
       tolua_function(tolua_S,"new_local",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_SizeAdapter_new00_local);
       tolua_function(tolua_S,".call",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_SizeAdapter_new00_local);
       tolua_function(tolua_S,"delete",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_SizeAdapter_delete00);
      tolua_endmodule(tolua_S);
     tolua_endmodule(tolua_S);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_module(tolua_S,"Gui",0);
   tolua_beginmodule(tolua_S,"Gui");
    tolua_module(tolua_S,"Adapters",0);
    tolua_beginmodule(tolua_S,"Adapters");
     tolua_module(tolua_S,"Atlas",0);
     tolua_beginmodule(tolua_S,"Atlas");
      #ifdef __cplusplus
      tolua_cclass(tolua_S,"MapAdapter","EmberOgre::Gui::Adapters::Atlas::MapAdapter","EmberOgre::Gui::Adapters::Atlas::AdapterBase",tolua_collect_EmberOgre__Gui__Adapters__Atlas__MapAdapter);
      #else
      tolua_cclass(tolua_S,"MapAdapter","EmberOgre::Gui::Adapters::Atlas::MapAdapter","EmberOgre::Gui::Adapters::Atlas::AdapterBase",NULL);
      #endif
      tolua_beginmodule(tolua_S,"MapAdapter");
       tolua_function(tolua_S,"new",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_MapAdapter_new00);
       tolua_function(tolua_S,"new_local",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_MapAdapter_new00_local);
       tolua_function(tolua_S,".call",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_MapAdapter_new00_local);
       tolua_function(tolua_S,"delete",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_MapAdapter_delete00);
       tolua_function(tolua_S,"getAttributeNames",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_MapAdapter_getAttributeNames00);
       tolua_function(tolua_S,"valueOfAttr",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_MapAdapter_valueOfAttr00);
       tolua_function(tolua_S,"hasAttr",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_MapAdapter_hasAttr00);
       tolua_function(tolua_S,"addAttributeAdapter",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_MapAdapter_addAttributeAdapter00);
       tolua_function(tolua_S,"removeAdapters",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_MapAdapter_removeAdapters00);
      tolua_endmodule(tolua_S);
     tolua_endmodule(tolua_S);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_module(tolua_S,"Gui",0);
   tolua_beginmodule(tolua_S,"Gui");
    tolua_module(tolua_S,"Adapters",0);
    tolua_beginmodule(tolua_S,"Adapters");
     tolua_module(tolua_S,"Atlas",0);
     tolua_beginmodule(tolua_S,"Atlas");
      #ifdef __cplusplus
      tolua_cclass(tolua_S,"ListAdapter","EmberOgre::Gui::Adapters::Atlas::ListAdapter","EmberOgre::Gui::Adapters::Atlas::AdapterBase",tolua_collect_EmberOgre__Gui__Adapters__Atlas__ListAdapter);
      #else
      tolua_cclass(tolua_S,"ListAdapter","EmberOgre::Gui::Adapters::Atlas::ListAdapter","EmberOgre::Gui::Adapters::Atlas::AdapterBase",NULL);
      #endif
      tolua_beginmodule(tolua_S,"ListAdapter");
       tolua_function(tolua_S,"new",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_ListAdapter_new00);
       tolua_function(tolua_S,"new_local",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_ListAdapter_new00_local);
       tolua_function(tolua_S,".call",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_ListAdapter_new00_local);
       tolua_function(tolua_S,"delete",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_ListAdapter_delete00);
       tolua_function(tolua_S,"getSize",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_ListAdapter_getSize00);
       tolua_function(tolua_S,"valueOfAttr",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_ListAdapter_valueOfAttr00);
       tolua_function(tolua_S,"addAttributeAdapter",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_ListAdapter_addAttributeAdapter00);
       tolua_function(tolua_S,"removeAdapters",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_ListAdapter_removeAdapters00);
      tolua_endmodule(tolua_S);
     tolua_endmodule(tolua_S);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_module(tolua_S,"Gui",0);
   tolua_beginmodule(tolua_S,"Gui");
    tolua_module(tolua_S,"Adapters",0);
    tolua_beginmodule(tolua_S,"Adapters");
     tolua_module(tolua_S,"Atlas",0);
     tolua_beginmodule(tolua_S,"Atlas");
      #ifdef __cplusplus
      tolua_cclass(tolua_S,"PositionAdapter","EmberOgre::Gui::Adapters::Atlas::PositionAdapter","EmberOgre::Gui::Adapters::Atlas::AdapterBase",tolua_collect_EmberOgre__Gui__Adapters__Atlas__PositionAdapter);
      #else
      tolua_cclass(tolua_S,"PositionAdapter","EmberOgre::Gui::Adapters::Atlas::PositionAdapter","EmberOgre::Gui::Adapters::Atlas::AdapterBase",NULL);
      #endif
      tolua_beginmodule(tolua_S,"PositionAdapter");
       tolua_function(tolua_S,"new",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_PositionAdapter_new00);
       tolua_function(tolua_S,"new_local",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_PositionAdapter_new00_local);
       tolua_function(tolua_S,".call",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_PositionAdapter_new00_local);
       tolua_function(tolua_S,"delete",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_PositionAdapter_delete00);
      tolua_endmodule(tolua_S);
     tolua_endmodule(tolua_S);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_module(tolua_S,"Gui",0);
   tolua_beginmodule(tolua_S,"Gui");
    tolua_module(tolua_S,"Adapters",0);
    tolua_beginmodule(tolua_S,"Adapters");
     tolua_module(tolua_S,"Atlas",0);
     tolua_beginmodule(tolua_S,"Atlas");
      #ifdef __cplusplus
      tolua_cclass(tolua_S,"Position2DAdapter","EmberOgre::Gui::Adapters::Atlas::Position2DAdapter","EmberOgre::Gui::Adapters::Atlas::AdapterBase",tolua_collect_EmberOgre__Gui__Adapters__Atlas__Position2DAdapter);
      #else
      tolua_cclass(tolua_S,"Position2DAdapter","EmberOgre::Gui::Adapters::Atlas::Position2DAdapter","EmberOgre::Gui::Adapters::Atlas::AdapterBase",NULL);
      #endif
      tolua_beginmodule(tolua_S,"Position2DAdapter");
       tolua_function(tolua_S,"new",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_Position2DAdapter_new00);
       tolua_function(tolua_S,"new_local",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_Position2DAdapter_new00_local);
       tolua_function(tolua_S,".call",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_Position2DAdapter_new00_local);
       tolua_function(tolua_S,"delete",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_Position2DAdapter_delete00);
      tolua_endmodule(tolua_S);
     tolua_endmodule(tolua_S);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_module(tolua_S,"Gui",0);
   tolua_beginmodule(tolua_S,"Gui");
    tolua_module(tolua_S,"Adapters",0);
    tolua_beginmodule(tolua_S,"Adapters");
     tolua_module(tolua_S,"Atlas",0);
     tolua_beginmodule(tolua_S,"Atlas");
      #ifdef __cplusplus
      tolua_cclass(tolua_S,"OrientationAdapter","EmberOgre::Gui::Adapters::Atlas::OrientationAdapter","EmberOgre::Gui::Adapters::Atlas::AdapterBase",tolua_collect_EmberOgre__Gui__Adapters__Atlas__OrientationAdapter);
      #else
      tolua_cclass(tolua_S,"OrientationAdapter","EmberOgre::Gui::Adapters::Atlas::OrientationAdapter","EmberOgre::Gui::Adapters::Atlas::AdapterBase",NULL);
      #endif
      tolua_beginmodule(tolua_S,"OrientationAdapter");
       tolua_function(tolua_S,"new",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_OrientationAdapter_new00);
       tolua_function(tolua_S,"new_local",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_OrientationAdapter_new00_local);
       tolua_function(tolua_S,".call",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_OrientationAdapter_new00_local);
       tolua_function(tolua_S,"delete",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_OrientationAdapter_delete00);
      tolua_endmodule(tolua_S);
     tolua_endmodule(tolua_S);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"EmberOgre",0);
  tolua_beginmodule(tolua_S,"EmberOgre");
   tolua_module(tolua_S,"Gui",0);
   tolua_beginmodule(tolua_S,"Gui");
    tolua_module(tolua_S,"Adapters",0);
    tolua_beginmodule(tolua_S,"Adapters");
     tolua_module(tolua_S,"Atlas",0);
     tolua_beginmodule(tolua_S,"Atlas");
      #ifdef __cplusplus
      tolua_cclass(tolua_S,"StaticAdapter","EmberOgre::Gui::Adapters::Atlas::StaticAdapter","EmberOgre::Gui::Adapters::Atlas::AdapterBase",tolua_collect_EmberOgre__Gui__Adapters__Atlas__StaticAdapter);
      #else
      tolua_cclass(tolua_S,"StaticAdapter","EmberOgre::Gui::Adapters::Atlas::StaticAdapter","EmberOgre::Gui::Adapters::Atlas::AdapterBase",NULL);
      #endif
      tolua_beginmodule(tolua_S,"StaticAdapter");
       tolua_function(tolua_S,"new",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_StaticAdapter_new00);
       tolua_function(tolua_S,"new_local",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_StaticAdapter_new00_local);
       tolua_function(tolua_S,".call",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_StaticAdapter_new00_local);
       tolua_function(tolua_S,"delete",tolua_atlas_adapters_EmberOgre_Gui_Adapters_Atlas_StaticAdapter_delete00);
      tolua_endmodule(tolua_S);
     tolua_endmodule(tolua_S);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_atlas_adapters (lua_State* tolua_S) {
 return tolua_atlas_adapters_open(tolua_S);
};
#endif

