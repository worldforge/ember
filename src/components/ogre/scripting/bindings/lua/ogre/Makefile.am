INCLUDES = -I$(top_srcdir)/src  -I$(top_builddir)/src -DPREFIX=\"@prefix@\"
METASOURCES = AUTO

#if USE_LUA
#liblua_scripting_a_LDFLAGS = -nodefaultlibs -rdynamic -export-dynamic -dlpreopen self
noinst_LIBRARIES = liblua_Ogre.a
liblua_Ogre_a_SOURCES = lua_Ogre.cpp 
noinst_HEADERS = required.h
#endif
