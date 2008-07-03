#! /bin/sh
#call our common bindings generating script
../../../../../scripts/update_lua_bindings.sh Atlas Atlas.pkg lua_Atlas.cpp

sed -i -e 's/tolua_is\(number\|string\|cppstring\)/&_type/g' lua_Atlas.cpp
